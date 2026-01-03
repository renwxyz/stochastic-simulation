#include "../include/client_handler.hpp"
#include "../include/time_utils.hpp"

#include <unistd.h>      // Close
#include <sys/socket.h>  // Socket functions

#include <iomanip>
#include <chrono>
#include <cstring>

#include <iostream>
using namespace std;
using namespace std::chrono;

void handle_client(int client_fd){
    print_table_header();

    steady_clock::time_point last_time;
    bool first = true;
    int no = 1;

    while(true){
        
        char buffer[1024];
        memset(&buffer, 0, sizeof(buffer));

        int bytes_received = recv(client_fd, buffer, sizeof(buffer)-1, 0);
        if (bytes_received <= 0) break;

        send(client_fd, "Oke\n", 4, 0);
        
        auto now_steady = steady_clock::now();
        double inter_arrival_time = 0.0;

        if (!first) {
            inter_arrival_time = duration<double>(now_steady - last_time).count();
        } else {
            first = false;
        }

        cout << left
             << setw(3) << no << "   "
             << setw(13) << current_time_str()
             << right
             << setw(6)  << fixed << setprecision(2) << inter_arrival_time
             << endl;

        last_time = now_steady;
    }

    cout << "\nClient disconnected\n\n";
}

void print_table_header(){
    cout << left
        << setw(3)  << "No" << "  "
        << setw(13) << "Receive Time" << "  "
        << right
        << setw(6)  << "IAT(s)"
        << endl;  

    cout << left
        << setw(3)  << "---" << "  "
        << setw(13) << "-------------" << "  "
        << setw(6)  << "------"
        << endl;
}