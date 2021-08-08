#ifndef UDP_SERVER_H_
#define UDP_SERVER_H_

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
// #include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <atomic>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <mutex>
#include <numeric>
#include <queue>
#include <stdexcept>
#include <string>
#include <thread>

#include "communication/received_data.h"
#include "dvs/dvs.h"

class UdpServer
{
private:
    int port_num_;
    std::thread* receive_thread_;
    int file_descr_;
    socklen_t client_len;
    struct sockaddr_in claddr;
    struct sockaddr_in myaddr;
    char* receive_buffer_;
    std::mutex mtx_;

    std::unique_ptr<const ReceivedData> received_data_;
    std::queue<std::unique_ptr<const ReceivedData>> received_data_buffer_;

public:
    static constexpr size_t max_buffer_size = 100000000;

    bool hasReceivedData()
    {
        const std::lock_guard<std::mutex> lg(mtx_);
        return received_data_buffer_.size() > 0;
    }

    UdpServer() = delete;
    UdpServer(const UdpServer& other) = delete;
    UdpServer(UdpServer&& other) = delete;

    UdpServer(const int port_num) : port_num_(port_num)
    {
        receive_buffer_ = new char[max_buffer_size];

        if ((file_descr_ = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        {
            throw std::runtime_error("Cannot create socket");
        }

        memset((char*)&myaddr, 0, sizeof(myaddr));
        myaddr.sin_family = AF_INET;
        myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        myaddr.sin_port = htons(port_num_);

        if (bind(file_descr_, (struct sockaddr*)&myaddr, sizeof(myaddr)) < 0)
        {
            throw std::runtime_error("Cannot bind");
        }
    }

    void start()
    {
        receive_thread_ = new std::thread(&UdpServer::receiveThreadFunction, this);
    }

    std::unique_ptr<const ReceivedData> getReceivedData()
    {
        const std::lock_guard<std::mutex> lg(mtx_);
        if (received_data_buffer_.size() > 0)
        {
            std::unique_ptr<const ReceivedData> res(std::move(received_data_buffer_.front()));
            received_data_buffer_.pop();
            return res;
        }
        else
        {
            return std::unique_ptr<const ReceivedData>();
        }
    }

    void sendData(char data[256], const int num_bytes_to_send)
    {
        struct sockaddr* tx_addr_ptr;
        tx_addr_ptr = (struct sockaddr*)&claddr;
        if (file_descr_ == -1)
        {
            perror("Invalid socket!");
        }
        else if (sendto(file_descr_, data, num_bytes_to_send, 0, tx_addr_ptr, sizeof(struct sockaddr_in)) < 0)
        {
            perror("sendto failed");
        }
    }

    void sendAck()
    {
        char data[256];
        data[0] = 'a';
        data[1] = 'c';
        data[2] = 'k';
        data[3] = '#';
        data[4] = '\0';
        sendData(data, 5);
    }

    void receiveThreadFunction()
    {
        client_len = sizeof(claddr);

        const size_t max_bytes_for_one_msg = 1380;

        bool should_run = true;
        while (should_run)
        {
            size_t num_received_bytes_total = 0;
            int num_received_bytes =
                recvfrom(file_descr_, receive_buffer_, max_buffer_size, 0, (struct sockaddr*)&claddr, &client_len);

            if (num_received_bytes < 0)
            {
                throw std::runtime_error("recvfrom returned error!");
            }

            num_received_bytes_total += num_received_bytes;

            uint64_t num_expected_bytes;
            std::memcpy(&num_expected_bytes, &(receive_buffer_[sizeof(uint64_t) + 1]), sizeof(uint64_t));

            if (static_cast<size_t>(num_expected_bytes) >= max_buffer_size)
            {
                throw std::runtime_error("Too many bytes received!");
            }
            sendAck();

            if (num_expected_bytes > max_bytes_for_one_msg)
            {
                while (num_received_bytes_total < num_expected_bytes)
                {
                    num_received_bytes = recvfrom(file_descr_,
                                                  &(receive_buffer_[num_received_bytes_total]),
                                                  max_buffer_size,
                                                  0,
                                                  (struct sockaddr*)&claddr,
                                                  &client_len);

                    num_received_bytes_total += num_received_bytes;
                    sendAck();
                }
            }

            const uint8_t* const uint8_ptr = reinterpret_cast<const uint8_t* const>(receive_buffer_);

            // TODO: Convert to other endianness and check also if magic number if valid for other endianness
            uint64_t rec_magic_num;
            std::memcpy(&rec_magic_num, &(uint8_ptr[1]), sizeof(uint64_t));

            if (rec_magic_num != dvs::internal::magic_num)
            {
                throw std::runtime_error("Invalid magic number!");
            }

            // received_data_ = std::make_unique<const ReceivedData>(uint8_ptr, num_received_bytes_total);
            {
                const std::lock_guard<std::mutex> lg(mtx_);
                received_data_buffer_.push(std::make_unique<const ReceivedData>(uint8_ptr, num_received_bytes_total));
            }
        }
    }

    ~UdpServer() {}
};

#endif
