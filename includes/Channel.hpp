#pragma once 

#include "Client.hpp"

class Server;
class Client;

class Channel
{
    private :
        std::string name;
        std::vector<Client *> my_clients;
        std::vector<int> operators;

    public :
        Channel(const std::string& name);
        Channel(const Channel& other);
        //================geters======================
        const std::string& get_name() const;
        const std::vector<Client*>& get_clients() const;
        const std::vector<int>& get_operators();
        //================methods======================
        void add_client(Client* client);
        void add_operator(int fd);
        bool is_client_in_channel(Client* client) const;
        bool isClientAnOperator(Client* client) const;

        Client* findClient(std::string nickname) const;

};