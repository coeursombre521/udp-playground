#ifndef __UDPRCV_H_
#define __UDPRCV_H_

#include <string>
#ifdef _MSC_VER
#include <winsock2.h>
#include <ws2tcpip.h>
#elif defined(__unix__) || defined(__GNUC__)
#include <netinet/in.h>
#endif

static constexpr unsigned MAXLINE = 1024;

class UdpReceive
{
public:
    UdpReceive(const std::string &ip, const std::string &port);
    ~UdpReceive();

    void create_socket();
    void receive();
private:
    struct sockaddr_in m_serv_addr;
    struct sockaddr_in m_cli_addr;
    char m_buf[MAXLINE];
#ifdef _MSC_VER
    WSADATA m_wsaData;
    SOCKET m_fd;
#elif defined(__unix__) || defined(__GNUC__)
    int m_fd;
#endif

    std::string m_ip;
    std::string m_port;
};


#endif
