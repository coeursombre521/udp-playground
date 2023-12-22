#ifndef __UDPSND_H_
#define __UDPSND_H_

#include <string>
#ifdef _MSC_VER
#include <winsock2.h>
#include <ws2tcpip.h>
#elif defined(__unix__) || defined(__GNUC__)
#include <netinet/in.h>
#endif

class UdpSend
{
public:
    UdpSend(const std::string &ip, const std::string &port);
    ~UdpSend();

    void create_socket();
    void send(const std::string &msg);
private:
    struct sockaddr_in m_addr;
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
