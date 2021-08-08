#ifndef DVS_COMMUNICATION_H_
#define DVS_COMMUNICATION_H_

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

#define DVS_PORT_NUM 9547

typedef struct S_SocketStructure
{
    int file_descr;
    struct sockaddr_in tx_addr;
    socklen_t client_len;
} SocketStructure;

SocketStructure createSocket(const int port_num)
{
    SocketStructure sock_struct;
    sock_struct.file_descr = socket(AF_INET, SOCK_DGRAM, 0);
    sock_struct.client_len = sizeof(sock_struct.tx_addr);

    memset(&(sock_struct.tx_addr), 0, sizeof(sock_struct.tx_addr));
    sock_struct.tx_addr.sin_family = AF_INET;
    sock_struct.tx_addr.sin_port = htons(port_num);
    sock_struct.tx_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    return sock_struct;
}

int receiveData(SocketStructure* sock_struct, char data[256])
{
    struct sockaddr* tx_addr_ptr = (struct sockaddr*)&(sock_struct->tx_addr);
    const int num_received_bytes =
        recvfrom(sock_struct->file_descr, data, 256, 0, tx_addr_ptr, &(sock_struct->client_len));
    return num_received_bytes;
}

void sendData(SocketStructure* sock_struct, const uint8_t* const data, const uint64_t num_bytes)
{
    struct sockaddr* tx_addr_ptr = (struct sockaddr*)&(sock_struct->tx_addr);
    sendto(sock_struct->file_descr, data, num_bytes, 0, tx_addr_ptr, sizeof(sock_struct->tx_addr));
}

#endif
