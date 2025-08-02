#pragma once 

#include "Client.hpp"
class Server;
class Client;

class Channel
{
    private :
        std::string name;
        std::vector<Client *> my_clients;
        std::vector<int> oprators;

    public :
        Channel(const std::string& name);
        //================geters======================
        const std::string& get_name() const;
        const std::vector<Client*>& get_clients() const;
        const std::vector<int>& get_opaer();
        //================methods======================
        bool it_is_a_channel_member(Client* client) const;
        void add_client(Client* client);
        void add_operator(int fd);

};