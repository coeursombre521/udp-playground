
#include "udprcv.h"

#include <iostream>
#include <string>
#include <cstring>
#include <cerrno>
#include <cctype>

#ifdef _MSC_VER
#include <winsock2.h>
#include <ws2tcpip.h>
#elif defined(__unix__) || defined(__GNUC__)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#ifdef _MSC_VER
#pragma comment(lib, "ws2_32.lib")
#endif

UdpReceive::UdpReceive(const std::string &ip, const std::string &port)
{
    std::cout << "UdpChat - Server mode\n";
    std::cout << "Alex Anton - compot de pufuleti\n\n";

    this->m_ip = ip;
    this->m_port = port;
}

UdpReceive::~UdpReceive()
{
#ifdef _MSC_VER
    WSACleanup();
    closesocket(this->m_fd);
#elif defined(__unix__) || defined(__GNUC__)
    close(this->m_fd);
#endif
}

void
UdpReceive::create_socket()
{
#ifdef _MSC_VER
    if (WSAStartup(MAKEWORD(2, 2), &this->m_wsaData) != 0) {
        std::cerr << "Failed to initialize Windows Sockets: " << std::strerror(WSAGetLastError()) << "\n";
        exit(1);
    }
#endif

#ifdef _MSC_VER
    if ((this->m_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET) {
        std::cerr << "Failed to create socket: " << std::strerror(WSAGetLastError()) << "\n";
        exit(1);
    }
#elif defined(__unix__) || defined(__GNUC__)
    if ((this->m_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cerr << "Failed to create socket: " << std::strerror(errno) << "\n";
        exit(1);
    }
#endif

    memset(&this->m_serv_addr, 0, sizeof(this->m_serv_addr));
    memset(&this->m_cli_addr, 0, sizeof(this->m_cli_addr));

    this->m_serv_addr.sin_family = AF_INET;
    this->m_serv_addr.sin_port = htons(std::stoi(this->m_port));
    this->m_serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

#ifdef _MSC_VER
    if (bind(this->m_fd, (struct sockaddr *)&this->m_serv_addr, sizeof(this->m_serv_addr)) == SOCKET_ERROR) {
        std::cerr << "Invalid address: " << std::strerror(WSAGetLastError()) << "\n";
        closesocket(this->m_fd);
        WSACleanup();
#elif defined(__unix__) || defined(__GNUC__)
    if (bind(this->m_fd, (struct sockaddr *)&this->m_serv_addr, sizeof(this->m_serv_addr)) < 0) {
        std::cerr << "Invalid address: " << std::strerror(errno) << "\n";
        close(this->m_fd);
#endif
        exit(1);
    }
}

void
UdpReceive::receive()
{
    int len, n;
    len = sizeof(this->m_cli_addr);

    n = recvfrom(this->m_fd, this->m_buf, MAXLINE, 0, (struct sockaddr *)&this->m_cli_addr, (socklen_t *)&len);
    this->m_buf[n] = '\0';

    if (n > 0) std::printf("%s\n", this->m_buf);
}
