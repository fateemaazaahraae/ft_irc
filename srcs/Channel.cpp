# include "../includes/Channel.hpp"

Channel::Channel(const std::string& n)
{
    name = n;
    inv_only = false;
    topic_res = false;
    has_key = false;
    has_mem_lim = false;
    key = "";
    mem_lim = 0;
}

Channel::Channel(const Channel& other)
{
    name = other.name;
    my_clients = other.my_clients;
    operators = other.operators;
    inv_only = false;
    topic_res = false;
    has_key = false;
    has_mem_lim = false;
    key = "";
    mem_lim = 0;
}

const std::string& Channel::getTopic() const { return topic; }

const std::string& Channel::get_name() const { return name; }

const std::vector<Client*>& Channel::get_clients() const { return my_clients; }

const std::vector<int>& Channel::get_operators() const { return operators; }

void Channel::add_client(Client* client, Server* server)
{
    if (is_client_in_channel(client))
    {
        server->setReplyCode(443);
        std::string rep = server->reply(client->get_client_nickname(), ": You are already in this channel");
        return;
    }
    my_clients.push_back(client);
    std::string joinMsg = ":" + client->get_prefix() + " JOIN :" + name;
    for (size_t i = 0; i < my_clients.size(); ++i)
        server->send_to_client(my_clients[i]->get_client_fd(), joinMsg);
}

void Channel::add_operator(int fd)
{
    size_t i = 0;
    while (i < operators.size())
    {
        if (operators[i] == fd)
            return;
        i++;
    }
    operators.push_back(fd);
}

bool Channel::is_client_in_channel(Client* client) const
{
    size_t i = 0;
    while (i < my_clients.size())
    {
        if (my_clients[i]->get_client_fd() == client->get_client_fd())
            return true;
        i++;
    }
    return false;
}


bool Channel::is_operator_in_channel(int fd) const
{
    size_t i = 0;
    while (i < operators.size())
    {
        if (fd == operators[i])
            return true;
        i++;
    }
    return false;
}

void Channel::removeClient(int fd)
{
    size_t i = 0;
    while (i < my_clients.size())
    {
        if (my_clients[i]->get_client_fd() == fd)
        {
            my_clients.erase(my_clients.begin() + i);
            break;
        }
        i++;
    }
    i = 0;
    while (i < operators.size())
    {
        if (operators[i] == fd)
        {
            operators.erase(operators.begin() + i);
            break;
        }
        i++;
    }
}

void Channel::set_topic(const std::string& t)
{
    topic = t;
}
void Channel::set_invite_only(bool var)
{
    inv_only = var;
}
void Channel::set_topic_restricted(bool var)
{
    topic_res = var;
}
void Channel::set_key(const std::string& key, bool var)
{
    has_key = var;
    this->key = key;
}
void Channel::set_member_limit(int limit, bool var)
{
    has_mem_lim = var;
    this->mem_lim = limit;
}
void Channel::remove_operator(int fd)
{
    size_t i = 0;
    while (i < operators.size())
    {
        if (operators[i] == fd) 
        {
            operators.erase(operators.begin() + i);
            return;
        }
        i++;
    }
}

bool Channel::hasInviteOnly() const
{
    return inv_only;
}

bool Channel::hasTopicProtection() const
{
    return topic_res;
}

bool Channel::isKeyProtected() const
{
    return has_key;
}

bool Channel::get_has_mem_lim() const
{
    return has_mem_lim;
}

int Channel::get_mem_lim() const
{
    return mem_lim;
}

std::string Channel::get_key_word() const
{
    return key;
}

bool Channel::get_inv_only() const
{
    return inv_only;
}