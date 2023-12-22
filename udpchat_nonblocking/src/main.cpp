
#include "udpsnd.h"
#include "udprcv.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <memory>
#include <chrono>
#include <thread>

#ifdef _MSC_VER
#include <conio.h>
#include <winsock2.h>
#elif defined(__unix__) || defined(__GNUC__)
#include <sys/select.h>
#include <unistd.h>
#define FD_CAST_TYPE int
#endif

bool
is_valid_port(const std::string& port) {
    std::istringstream iss(port);
    int port_num;
    iss >> port_num;
    return !iss.fail() && iss.eof() && port_num >= 0 && port_num <= 65535;
}

int
main(void)
{
#ifdef _MSC_VER
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize winsock\n";
        return -1;
    }
#endif

    std::string input;
    std::string ip;
    std::string port;

    while (true) {
        std::cout << "Who do you want to talk to today? ([IP]:[PORT]) : ";
        std::getline(std::cin, input);

        size_t colon_pos = input.find(':');
        if (colon_pos != std::string::npos) {
            ip = input.substr(0, colon_pos);
            port = input.substr(colon_pos + 1);

            if (is_valid_port(port)) {
                std::cout << "Okay. Destination is IP " << ip << " with the " << port << ".\n"
                    << "Listening on localhost for blabbering on the same port\n";
                break;
            }
            else {
                std::cout << "Invalid port. Please try again.\n";
            }
        }
        else {
            std::cout << "Invalid format. Please ensure you enter the string in the format [ip]:[port].\n";
        }
    }

    std::cout << "\n\nHappy chatting!\n\n";

    std::unique_ptr<UdpSend> sender = std::make_unique<UdpSend>(ip, port);
    std::unique_ptr<UdpReceive> receiver = std::make_unique<UdpReceive>("127.0.0.1", port);

    sender->create_socket();
#ifdef _MSC_VER
    sender->set_non_blocking();
#endif

    receiver->create_socket();
#ifdef _MSC_VER
    sender->set_non_blocking();
#endif

    fd_set readfds;
    fd_set writefds;
    struct timeval timeout;

    while (true) {
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_SET(sender->get_fd(), &writefds);
        FD_SET(receiver->get_fd(), &readfds);
#if defined(__unix__) || defined(__GNUC__)
        FD_SET(STDIN_FILENO, &readfds);
#endif

        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

#ifdef _MSC_VER
        int activity = select(0, &readfds, &writefds, NULL, &timeout);
        if (activity == SOCKET_ERROR) {
            std::cerr << "Error with select: " << WSAGetLastError() << "\n";
            break;
        }
#elif defined(__unix__) || defined(__GNUC__)
        int max_fd = std::max<int>({ sender->get_fd(), receiver->get_fd(), STDIN_FILENO}) + 1;
        int activity = select(max_fd, &readfds, &writefds, NULL, &timeout);

        if (activity < 0) {
            std::cerr << "Error with select\n";
            break;
        }
#endif

#ifdef _MSC_VER
        if (activity > 0) {
            if (FD_ISSET(receiver->get_fd(), &readfds)) {
                receiver->receive();
            }

            if (FD_ISSET(sender->get_fd(), &writefds)) {
                sender->send_all_from_queue();
            }
        }

        if (_kbhit()) {
            std::string message;
            std::getline(std::cin, message);
            sender->add_to_queue(message);
        }
#elif defined(__unix__) || defined(__GNUC__)
        if (FD_ISSET(receiver->get_fd(), &readfds)) {
            receiver->receive();
        }

        if (FD_ISSET(sender->get_fd(), &writefds)) {
            sender->send_all_from_queue();
        }

        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            std::string message;
            std::getline(std::cin, message);
            sender->add_to_queue(message);
        }
#endif

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
