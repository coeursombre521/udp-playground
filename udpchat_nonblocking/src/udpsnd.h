#ifndef __UDPSND_H_
#define __UDPSND_H_

#include <string>
#include <queue>
#ifdef _MSC_VER
#include <winsock2.h>
#include <ws2tcpip.h>
#elif defined(__unix__) || defined(__GNUC__)
#include <netinet/in.h>
#endif

#ifdef _MSC_VER
#pragma comment(lib, "ws2_32.lib")
#endif

class UdpSend
{
public:
    UdpSend(const std::string &ip, const std::string &port);
    ~UdpSend();

    void create_socket();
    void send(const std::string &msg);
    void add_to_queue(const std::string &msg);
    void send_all_from_queue();

#ifdef _MSC_VER
    SOCKET get_fd() const { return m_fd; }
#else
    int get_fd() const { return m_fd; }
#endif

#ifdef _MSC_VER
    void set_non_blocking() {
        u_long mode = 1;
        ioctlsocket(m_fd, FIONBIO, &mode);
    }
#endif
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

    std::queue<std::string> m_send_queue;
};

#endif
