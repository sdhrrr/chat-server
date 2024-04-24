#include "server.h"
#include <cstdlib>
#include <cstring>
#include <minwindef.h>
#include <psdk_inc/_ip_types.h>
#include <psdk_inc/_socket_types.h>
#include <psdk_inc/_wsadata.h>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>

#ifdef  DEBUG
#include <iostream>
#endif

void cleanup(SOCKET& socket){
    closesocket(socket);
    WSACleanup();
    exit(0);
}

server::server(const char* addr , int port){

    WSADATA metaData;
    WORD ver = MAKEWORD(2, 2);
    int err = WSAStartup(ver,&metaData);

#ifdef DEBUG
    if(err != 0){
        std::cerr<<"[Error] Loding DLL\n";
    }else{
        std::cerr<<"[Sucess] DLL Loded";
    }
#endif

    m_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

#ifdef DEBUG
    if(m_server == INVALID_SOCKET){
        std::cerr<<"[Error] Socket Not created properly."<<WSAGetLastError()<<"\n";
        cleanup(m_server);
    }else{
        std::cerr<<"[Sucess] Socket made.\n";
    }
#endif
    //binding the socket to the port
    sockaddr_in binddata;
    binddata.sin_family = AF_INET;
    binddata.sin_port = htons(port);
    InetPton(AF_INET,addr,&binddata.sin_addr.S_un);
    int binderr = bind(m_server, (sockaddr*)&binddata, sizeof(binddata));

#ifdef DEBUG
    if(binderr == SOCKET_ERROR){
        std::cerr<<"[Error] Socket not Bound succesfully to port: "<<port<<"\n";
        cleanup(m_server);
    }else{
        std::cerr<<"[Sucess] Socket Bound Sucessfully to port: "<<port<<" address: "<<addr<<"\n";
    }
#endif

    int listenerr = listen(m_server, 1);

#ifdef DEBUG
    if(listenerr == SOCKET_ERROR){
        std::cerr<<"[Error] Listening Failed on port: "<<port<<"("<<WSAGetLastError()<<")\n";
        cleanup(m_server);
    }else{
        std::cerr<<"[Sucess] Listening on port: "<<port<<"\n";
    }
#endif
}

void server::run(){

    while (true) {
        
        SOCKET client = accept(m_server, NULL, NULL);

        if(client != INVALID_SOCKET){
#ifdef DEBUG
            std::cerr<<"[Sucess] Client accepted\n";
#endif
            clients_pool.emplace_back(client);

            thread_pool.emplace_back(&server::handelClients,this,client); 
        }
#ifdef DEBUG
        else{
            std::cerr<<"[Error] Client Failed to connect\n";
        }
#endif

    }

}

void server::handelClients(SOCKET cfd){

    char buff[1024];
    while (true) {

        int rs = recv(cfd, buff, 1024, 0);
        
        if(rs <= 0) break;

        buff[rs] = '\0';
#ifdef DEBUG
        std::cout<<buff<<"\n";
#endif

        for(SOCKET i : clients_pool){

            if(i==cfd) continue;
            send(i, buff, strlen(buff), 0);
        }

    }

}

server::~server(){

    for(std::thread& i : thread_pool){
        i.join();
    }

    WSACleanup();
    closesocket(m_server);
}