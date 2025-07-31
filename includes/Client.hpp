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
        bool    is_authorized;

    public :
        Client();
        ~Client();

    //======================== getters =======================
        int get_client_fd() const;
        std::string &get_client_buffer();
        struct sockaddr_in &get_client_addr();
        int get_client_authe();
        std::string& get_client_nickname();

    //======================= setters =======================
        void set_client_fd(int fd);
        void set_client_buffer(std::string buffer);
        void set_client_addr(struct sockaddr_in addr);
        void set_client_authe();
        void set_client_nickname(std::string& nick);

    //======================= client methods =================
        void handle_user(std::vector<std::string> &args);
        void handle_join(std::vector<std::string> &args);
        void handle_private_msg(std::vector<std::string> &args);
        void executeCommand(std::vector<std::string> &args);

};