#pragma once

#include <minwindef.h>
#include <psdk_inc/_socket_types.h>
#include <thread>
#include <vector>
#include <winsock2.h>

class server{
    
private:
    SOCKET m_server;
    std::vector<SOCKET> clients_pool;
    std::vector<std::thread> thread_pool;
    void handelClients(SOCKET cfd );

public:
    server(const char* addr , int port);
    ~server();
    void run();

};