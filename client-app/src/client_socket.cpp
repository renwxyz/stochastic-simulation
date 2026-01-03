#include "../include/client_socket.hpp"

#include <unistd.h>
#include <arpa/inet.h> 
#include <sys/socket.h>
#include <cstring>

#include <iostream>
using namespace std;

int connect_to_server(const char* server_ip, int server_port){
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(client_fd < 0){
        cout << "[Error]: Socket gagal" << endl;
        return -1;
    }

    sockaddr_in server_addr{};
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family =  AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    if(connect(client_fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        cout << "Connetion failed" << endl;
        close(client_fd);
        return -1;
    }

    return client_fd;
}