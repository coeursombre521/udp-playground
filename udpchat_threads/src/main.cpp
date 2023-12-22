
#include "udpsnd.h"
#include "udprcv.h"

#include <iostream>
#include <string>
#include <sstream>
#include <atomic>
#include <thread>
#include <csignal>
#include <memory>

std::atomic<bool> running(true);

void
signal_handler (int signal)
{
    running = false;
}

bool
is_valid_port (const std::string& port) {
    std::istringstream iss(port);
    int port_num;
    iss >> port_num;
    return !iss.fail() && iss.eof() && port_num >= 0 && port_num <= 65535;
}

void
send_thread(const std::string& ip, const std::string &port)
{
    std::unique_ptr<UdpSend> udp_send_ptr = std::make_unique<UdpSend>(ip, port);
    udp_send_ptr->create_socket();

    while (running)
    {
        std::string msg;
        std::getline(std::cin, msg);
        udp_send_ptr->send(msg);
    }
}

void
receive_thread(const std::string &port, const std::string &ip = "127.0.0.1")
{
    std::unique_ptr<UdpReceive> udp_recv_ptr = std::make_unique<UdpReceive>(ip, port);
    udp_recv_ptr->create_socket();

    while (running)
    {
        udp_recv_ptr->receive();
    }
}

int
main (void)
{
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
            } else {
                std::cout << "Invalid port. Please try again.\n";
            }
        } else {
            std::cout << "Invalid format. Please ensure you enter the string in the format [ip]:[port].\n";
        }
    }

    std::cout << "\n\nHappy chatting!\n\n";

    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    std::thread _send(send_thread, ip, port);
    std::thread _receive(receive_thread, port, ip);

    _send.join();
    _receive.join();

    return 0;
}
