cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
project(main)


aux_source_directory("./client" CLIENT_SOURCES)
aux_source_directory("./server" SERVER_SOURCES)
aux_source_directory("./public" PUBLIC_SOURCES)

include_directories("./client")
include_directories("./server")
include_directories("./public")

add_executable(chatroom-client ${CLIENT_SOURCES} ${PUBLIC_SOURCES})
add_executable(chatroom-server ${SERVER_SOURCES} ${PUBLIC_SOURCES})