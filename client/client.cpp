#include <cstdlib>
#include <iostream>
#include <minwindef.h>
#include <psdk_inc/_ip_types.h>
#include <psdk_inc/_wsadata.h>
#include <string>
#include <thread>
#include <winsock2.h>
#include <psdk_inc/_socket_types.h>
#include <ws2tcpip.h>

void receiveData(const SOCKET& fd){
    
    char buff[1024];
    while (true) {

        int rs = recv(fd, buff, 1024, 0);

        if(rs <= 0) break;

        buff[rs] = '\0';

        std::cout<<buff<<"\n";
    }

}

void cleanup(SOCKET& fd){
    closesocket(fd);
    WSACleanup();
    exit(0);
}

int main(int argv, char** argc){

    if (argv != 3) return 1;

    WSADATA windata;
    WORD ver = MAKEWORD(2, 2);

    std::string name;
    std::cout<<"Enter your name: ";
    std::getline(std::cin , name);

    int winerr = WSAStartup(ver, &windata);
    if(winerr != 0){
        std::cerr<<"[Error] Dll failed to load\n";
        return 0;
    }

    SOCKET client = socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);

    if(client == INVALID_SOCKET){
        std::cerr<<"[Error] Socket not made\n";
        cleanup(client);
    }

    sockaddr_in sockdata;
    sockdata.sin_family = AF_INET;
    InetPton(AF_INET, argc[1],&sockdata.sin_addr.S_un);
    sockdata.sin_port = htons(std::stoi(argc[2]));

    int connerr = connect(client, (SOCKADDR*)&sockdata, sizeof(sockdata));

    if(connerr == SOCKET_ERROR){
        std::cerr<<"[Error] not Connected\n";
        cleanup(client);
    }

    std::thread Rworker(receiveData,std::ref(client));

    while(true){
        std::string line;
        std::getline(std::cin,line);

        if(line == "EXIT") exit(0);
        line = name +": "+line;

        send(client, line.c_str(), line.length(), 0);
    }


    Rworker.join(); 

    cleanup(client);
    return 0;
}