# include "../includes/Channel.hpp"

Channel::Channel(const std::string& n): name(n){}

Channel::Channel(const Channel& other)
{
    name = other.name;
    my_clients = other.my_clients;
    operators = other.operators;
}

const std::string& Channel::getTopic() const { return topic; }

const std::string& Channel::get_name() const { return name; }

const std::vector<Client*>& Channel::get_clients() const { return my_clients; }

const std::vector<int>& Channel::get_operators() const { return operators; }

void Channel::add_client(Client* client)
{
    if (!is_client_in_channel(client))
        my_clients.push_back(client);
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
            return;
        }
        i++;
    }
}

void Channel::set_topic(const std::string& t)
{
    topic = t;
}