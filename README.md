# udp_playground

This repository contains a collection of projects that demonstrate the use of UDP sockets to create a chat client. The projects are built using CMake and are written in C++. The main purpose of these projects is to illustrate a simple exchange of text information between two clients using the UDP protocol.

## Projects

1. **udpsnd**: This project is a simple C++ module for sending messages to a specified IP address using UDP sockets. It demonstrates how to create a UDP socket, bind it to a server address, and send datagrams to a client.

2. **udprcv**: This project complements `udpsnd` by implementing a C++ module for receiving UDP packets. It showcases the server-side functionality where the server waits for datagrams from clients without establishing a persistent connection.

3. **udpchat_threads**: This project integrates both `udpsnd` and `udprcv` to simulate a chat client. It uses two separate threads to handle sending and receiving messages concurrently, thus allowing for real-time communication between users.

4. **udpchat_nonblocking**: Similar to `udpchat_threads`, this project facilitates a chat client using UDP sockets. However, it employs non-blocking I/O calls to manage message exchange, ensuring that the main thread is not blocked during operations, which allows for other tasks to be performed simultaneously.


## Building the Projects

The projects are configured using CMake and can be built on both GNU/Linux and Windows. There is no need for third-party libraries to achieve the functionality described above. Here are the separate instructions for each operating system: 

### GNU/Linux

#### Requirements
- A GNU/Linux system (*duh*)
- GCC
- CMake

#### Instructions

1. Navigate to the project's directory.
2. Use the following commands to build the project:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Windows

For Windows, you will need Visual Studio (with Visual C++) in addition to CMake. Here are the steps:

1. Open the project in Visual Studio.
2. Visual Studio will automatically detect the CMake configuration.
3. Build the project using the build option in Visual Studio.

Please note that these instructions assume that you have CMake and Visual Studio (with Visual C++) installed on your system.