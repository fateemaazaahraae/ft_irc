#pragma once

#include "Server.hpp"

class Server;
class Channel;


class Client 
{
    private :
        int fd;
        std::string buffer;
        std::string nickname;
        std::string username;
        std::string realname;
        struct sockaddr_in clientAddr;
        bool    is_authorized;
        bool    is_registered;
        std::vector<std::string> invitedChannels;

    public :
        Client();
        ~Client();
        Client(const Client& other);

    //======================== getters =======================
        int get_client_fd() const;
        std::string &get_client_buffer();
        struct sockaddr_in &get_client_addr();
        int get_client_authe();
        int get_client_registered();
        std::string& get_client_nickname();
        std::string& get_client_username();
        std::string& get_client_realname();
        bool get_client_operator();

    //======================= setters =======================
        void set_client_fd(int fd);
        void set_client_buffer(std::string buffer);
        void set_client_addr(struct sockaddr_in addr);
        void set_client_authe();
        void set_client_registered();
        void set_client_nickname(std::string& nickname);
        void set_client_username(std::string& username);
        void set_client_realname(std::string& realname);
        void set_invitedChannels(std::string& channelName);

};