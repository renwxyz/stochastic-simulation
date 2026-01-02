#include <cstdlib>  // rand() & srand()
#include <ctime>    // time(0) -> seed
#include <chrono>   // Presisi (ms)

#include <cmath>    // log()
#include <thread>   // sleep main thread

#include <unistd.h> // Syscall Posix
#include <arpa/inet.h> // IPv4

#include <iomanip>
#include <cstring>
#include <iostream>
using namespace std;

int main(){

    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(client_fd < 0){
        cout << "[Error]: Socket gagal" << endl;
        return 1;
    }

    sockaddr_in server_addr{};
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family =  AF_INET;
    server_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);


    if(connect(client_fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        cout << "[Error]: Koneksi gagal" << endl;
        close(client_fd);
        return 1;
    }

    srand(time(0)); // Seed random
    double lambda = 0.5; // Mean waiting time 1/lambda = 2 detik
    string req_message = "Hello Server!\n";

    while (true){
        double U = (rand()+1.0)/(RAND_MAX + 1.0);
        double waiting_time = -log(U)/lambda;
        
        int sleep_ms = (int)(waiting_time * 1000);
        if (sleep_ms < 1) sleep_ms = 1;

        this_thread::sleep_for(chrono::milliseconds(sleep_ms));
        send(client_fd, req_message.c_str(),req_message.length(),0);

        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_fd, buffer, sizeof(buffer)-1, 0);

        cout << "Request send! | Waiting time: " << fixed << setprecision(2) << waiting_time << " | " << "Respon server: " << buffer;
    }

    close(client_fd);
    return 0;
}