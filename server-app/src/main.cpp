#include "../include/server.hpp"
#include "../include/client_handler.hpp"

#include <unistd.h>        // close
#include <sys/socket.h>    // accept
#include <netinet/in.h>    // sockaddr_in

#include <iostream>
using namespace std;

int main(){
    cout << "Starting server on port 8080...\n";

    int server_fd = create_server_socket(8080);
    if(server_fd < 0){
        return 1;
    }

    cout << "Server is listening for connections...\n";

    while(true){
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);

        int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
        if(client_fd < 0){
            perror("Accept failed");
            continue;
        }

        cout << "Client connected.\n";
        handle_client(client_fd);
        close(client_fd);
    }

    close(server_fd);
    return 0;
}