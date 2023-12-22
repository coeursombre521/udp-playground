
#include "udprcv.h"

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

    UdpReceive* receiver = new UdpReceive(ip, port);
    receiver->create_socket();

    std::cout << "Listening on " << ip << ":" << port << " for UDP blabbering. Press Ctrl-C to exit.\n";
    for (;;) {
        receiver->receive();
    }

    delete receiver;

    return 0;
}
