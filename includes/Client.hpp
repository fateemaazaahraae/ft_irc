#pragma once

#include "Server.hpp"

class Server;

class Client 
{
    private :
        int fd;
        std::string buffer;
        std::string nickname;
        std::string username;
        struct sockaddr_in clientAddr;
    public :
        Client();
        ~Client();
    //======================== geters =======================
        int get_client_fd() const;
        std::string &get_client_buffer();
        struct sockaddr_in &get_client_addr();

    //======================= seters =======================
        void set_client_fd(int fd);
        void set_client_buffer(std::string buffer);
        void set_client_addr(struct sockaddr_in addr);

};