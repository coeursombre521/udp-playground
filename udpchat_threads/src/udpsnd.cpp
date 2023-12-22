
#include "udpsnd.h"

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

UdpSend::UdpSend(const std::string &ip, const std::string &port)
{
    this->m_ip = ip;
    this->m_port = port;
}

UdpSend::~UdpSend()
{
#ifdef _MSC_VER
    WSACleanup();
    closesocket(this->m_fd);
#elif defined(__unix__) || defined(__GNUC__)
    close(this->m_fd);
#endif
}

void
UdpSend::create_socket()
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

    memset(&this->m_addr, 0, sizeof(this->m_addr));

    this->m_addr.sin_family = AF_INET;
    this->m_addr.sin_port = htons(std::stoi(this->m_port));

    if (inet_pton(AF_INET, this->m_ip.c_str(), &this->m_addr.sin_addr) <= 0) {
#ifdef _MSC_VER
        std::cerr << "Invalid address: " << std::strerror(WSAGetLastError()) << "\n";
        closesocket(this->m_fd);
        WSACleanup();
#elif defined(__unix__) || defined(__GNUC__)
        std::cerr << "Invalid address: " << std::strerror(errno) << "\n";
        close(this->m_fd);
#endif
        exit(1);
    }
}

void
UdpSend::send(const std::string &msg)
{
#ifdef _MSC_VER
    if (sendto(this->m_fd, msg.c_str(), msg.size(), 0, (struct sockaddr *)&this->m_addr, sizeof(this->m_addr)) == SOCKET_ERROR) {
        std::cerr << "Failed to send: " << WSAGetLastError() << "\n";
    }
#elif defined(__unix__) || defined(__GNUC__)
    if (sendto(this->m_fd, msg.c_str(), msg.size(), 0, (struct sockaddr *)&this->m_addr, sizeof(this->m_addr)) < 0) {
        std::cerr << "Failed to send: " << std::strerror(errno) << "\n";
    }
#endif
}
