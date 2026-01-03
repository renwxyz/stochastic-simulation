#include "../include/client_socket.hpp"
#include "../include/arrival_process.hpp"
#include "../include/time_utils.hpp"

#include <unistd.h>        // close
#include <sys/socket.h>    // send, recv
#include <thread>

#include <iomanip>
#include <cstring>

#include <iostream>
using namespace std;

#include <chrono>
using namespace std::chrono;

void print_client_table_header() {
    cout << left
         << setw(3)  << "No" << "  "
         << setw(6) << "Status" << "  "
         << setw(13) << "Send Time" << "  "
         << setw(16) << "Waiting Time (s)" << "  "
         << setw(6) << "Resp-server" << endl;

    cout << left
         << setw(3)  << "---" << "  "
         << setw(6) << "------" << "  "
         << setw(13) << "-------------" << "  "
         << setw(16) << "----------------" << "  "
         << setw(6) << "------" << endl;
}

int main(){
    int client_fd = connect_to_server("127.0.0.1", 8080);
    if(client_fd < 0){
        cout << "Connection failed. Exiting..." << endl;
        return -1;
    }

    srand(time(0));
    double lambda = 0.5;
    string request_message = "Hello, Server!";
    int no = 1;

    print_client_table_header();

    while(true){
        double waiting_time = generate_interarrival_time(lambda);
        int sleep_ms =  (int)(waiting_time * 1000);
        if (sleep_ms < 1) sleep_ms = 1;

        this_thread::sleep_for(chrono::milliseconds(sleep_ms));

        string request_status = "Sent";
        if(send(client_fd, request_message.c_str(), request_message.size(), 0) < 0){
            request_status = "Fail";
        }

        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if(bytes_received <= 0){
            cout << "Server disconnected or error receiving data." << endl;
            break;
        }

        cout << left
             << setw(3) << no++ << "  "
             << setw(6) << request_status << "  "
             << setw(13)<< current_time_string() << "  "
             << right
             << setw(16)<< fixed << setprecision(2) << waiting_time << "  "
             << setw(6) << buffer;
    }

    close(client_fd);
    return 0;
}