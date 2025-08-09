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
        std::string topic;
        bool inv_only;
        bool topic_res;
        bool has_key;
        bool has_mem_lim;
        std::string key;
        int mem_lim;

    public :
        Channel(const std::string& name);
        Channel(const Channel& other);

        //================geters======================
        const std::string& get_name() const;
        const std::vector<Client*>& get_clients() const;
        const std::vector<int>& get_operators() const;
        const std::string& getTopic() const;
        bool get_has_mem_lim() const;
        int get_mem_lim() const;
        std::string get_key_word() const;
        bool get_inv_only() const;

        //================setters======================
        void set_topic(const std::string& topic);
        void set_invite_only(bool var);
        void set_topic_restricted(bool var);
        void set_key(const std::string& key, bool var);
        void set_member_limit(int limit, bool var);
        
        
        //================methods======================
        void add_client(Client* client);
        void add_operator(int fd);
        bool is_client_in_channel(Client* client) const;
        
        Client* findClient(std::string nickname) const;
        bool is_operator_in_channel(int fd) const;
        void removeClient(int fd);
        void remove_operator(int fd);
        bool hasInviteOnly() const;
        bool hasTopicProtection() const;
        bool hasKey() const;
        bool hasMemberLimit() const;
        bool isKeyProtected() const;
        bool isMemberLimitProtected() const;
    };