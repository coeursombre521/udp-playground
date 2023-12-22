cmake_minimum_required(VERSION 3.10)

project(udp_playground)

add_subdirectory(udpchat_nonblocking)
add_subdirectory(udpchat_threads)
add_subdirectory(udprcv)
add_subdirectory(udpsnd)
