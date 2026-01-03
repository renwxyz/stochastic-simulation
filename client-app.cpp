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
using namespace std::chrono;

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
    double lambda = 3; // Mean waiting time 1/lambda = 2 detik
    string req_message = "Hello Server!\n";
    int no = 1;

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

    while (true){

        auto now_sys = system_clock::now();
        time_t now_c = system_clock::to_time_t(now_sys);
        tm local_time = *localtime(&now_c);

        int hour = local_time.tm_hour;
        int min  = local_time.tm_min;
        int sec  = local_time.tm_sec;

        auto now_steady = steady_clock::now();

        double U = (rand()+1.0)/(RAND_MAX + 1.0);
        double waiting_time = -log(U)/lambda;
        
        int sleep_ms = (int)(waiting_time * 1000);
        if (sleep_ms < 1) sleep_ms = 1;

        this_thread::sleep_for(chrono::milliseconds(sleep_ms));
        
        string req_status = "Sent";
        if (send(client_fd, req_message.c_str(),req_message.length(),0) < 0){
            req_status = "failed";
        }

        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_fd, buffer, sizeof(buffer)-1, 0);

        if(bytes_received < 0){
            cout << "[Error]: recv failed\n";
            break;
        } else if (bytes_received == 0){
            cout << "Disconnected from server\n";
            break;
        }

        cout << left
             << setw(3) << no << "  "
             << setw(6) << req_status << "  "
             << setw(13)
             << ((hour < 10 ? "0" : "") + to_string(hour) + ":" +
                 (min  < 10 ? "0" : "") + to_string(min)  + ":" +
                 (sec  < 10 ? "0" : "") + to_string(sec))
             << right
             << setw(12)  << fixed << setprecision(2) << waiting_time
             << setw(12)  << buffer;
        
        no++;
    }

    close(client_fd);
    return 0;
}