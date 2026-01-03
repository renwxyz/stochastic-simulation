#include "../include/server.hpp"

#include <unistd.h>      // Close
#include <sys/socket.h>  // Socket functions
#include <netinet/in.h>  // sockaddr_in
#include <cstring>       // memset

#include <iostream>
using namespace std;

int create_server_socket(int port){
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0){
        perror("Socket creation failed");
        return -1;
    }

    sockaddr_in server_addr{};
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if(bind(fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("Bind failed");
        close(fd);
        return -1;
    }

    if(listen(fd, 1) < 0){
        perror("Listen failed");
        close(fd);
        return -1;
    }

    return fd;
}