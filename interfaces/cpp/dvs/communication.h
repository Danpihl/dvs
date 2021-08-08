#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

#include <arpa/inet.h>
#include <errno.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
// #include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

#define DVS_PORT_NUM 9547

inline int& Var_socket_file_descr()
{
    static int socket_file_descr;
    return socket_file_descr;
}

class UdpClient
{
private:
    int sock_file_descr;
    struct sockaddr_in tx_addr;
    struct sockaddr* tx_addr_ptr;
    socklen_t client_len;

public:
    UdpClient() : sock_file_descr(-1), tx_addr_ptr(nullptr) {}

    UdpClient(const uint64_t port_num)
    {
        if ((sock_file_descr = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        {
            perror("socket failed");
            sock_file_descr = -1;
        }

        memset(&tx_addr, 0, sizeof(tx_addr));
        tx_addr.sin_family = AF_INET;
        tx_addr.sin_port = htons(port_num);
        tx_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        tx_addr_ptr = (struct sockaddr*)&tx_addr;
    }

    ~UdpClient()
    {
        close(sock_file_descr);
    }

    int receiveData(char data[256])
    {
        client_len = sizeof(tx_addr);
        const int num_received_bytes = recvfrom(sock_file_descr, data, 256, 0, (struct sockaddr*)&tx_addr, &client_len);
        return num_received_bytes;
    }

    void sendData(const uint8_t* const data, const uint64_t num_bytes)
    {
        if (sock_file_descr == -1)
        {
            perror("Invalid socket!");
        }
        else if (sendto(sock_file_descr, data, num_bytes, 0, tx_addr_ptr, sizeof(tx_addr)) < 0)
        {
            perror("sendto failed");
        }
    }
};

#endif
