#include <unistd.h>
#include <netinet/in.h>

#include <chrono>
#include <ctime>
#include <iomanip>
#include <cstring>

#include <iostream>
using namespace std;
using namespace std::chrono;

int main(){

    cout << "Starting server...\n";

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0){
        cout << "[Error]: Socket gagal\n";
        return 1; 
    }

    sockaddr_in server_addr{};
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if(bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        cout << "[Error]: Bind gagal\n";
        close(server_fd);
        return 1;
    }

    if(listen(server_fd, 1) < 0){
        cout << "[Error]: Listening failed\n";
        close(server_fd);
        return 1;
    }

    sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    socklen_t client_len = sizeof(client_addr);

    cout << "Server listening on port 8080\n";

    int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);

    if(client_fd < 0){
        cout << "[Error]: Accept failed\n" << endl;
        close(server_fd);
        return 1;
    }

    cout << "Client connected\n\n";

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

    steady_clock::time_point last_time;
    bool first = true;
    int no = 1;

    while(true){
        char buffer[1024];
        memset(&buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_fd, buffer, sizeof(buffer)-1, 0);

        if(bytes_received < 0){
            cout << "[Error]: recv failed\n";
            break;
        } else if (bytes_received == 0){
            cout << "Client disconnected\n";
            break;
        } 

        string response = "Oke";
        string response_message = response + "\n";
        send(client_fd, response_message.c_str(), response_message.length(),0);


        auto now_sys  = system_clock::now();
        time_t now_c  = system_clock::to_time_t(now_sys);
        tm local_time = *localtime(&now_c);

        int hour = local_time.tm_hour;
        int min  = local_time.tm_min;
        int sec  = local_time.tm_sec;

        auto now_steady = steady_clock::now();
        double inter_arrival = 0.0;

        if (!first) {
            inter_arrival = duration<double>(now_steady - last_time).count();
        } else {
            first = false;
        }

        cout << left
            << setw(3) << no << "   "
            << setw(13)
            << ((hour < 10 ? "0" : "") + to_string(hour) + ":" +
                (min  < 10 ? "0" : "") + to_string(min)  + ":" +
                (sec  < 10 ? "0" : "") + to_string(sec))
            << right
            << setw(6)  << fixed << setprecision(2) << inter_arrival
            << endl;

        last_time = now_steady;
        no++;
    }

    close(client_fd);
    close(server_fd);
    return 0;
}

