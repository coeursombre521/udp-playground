
#include "udpsnd.h"

#include <iostream>
#include <string>

int
main (int argc, char *argv[])
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " [IP] [PORT]\n";
        return 1;
    }

    std::string ip(argv[1]);
    std::string port(argv[2]);

    UdpSend* sender = new UdpSend(ip, port);
    sender->create_socket();

    std::cout << "You are talking to " << ip << ":" << port << " right now.\n";
    std::cout << "Blabber your thoughts line by line and press Return to send. Press Ctrl-C to exit.\n";
    std::string msg;

    while (std::getline(std::cin, msg)) {
        sender->send(msg);
    }

    delete sender;

    return 0;
}
