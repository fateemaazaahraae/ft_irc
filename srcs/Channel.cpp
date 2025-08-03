# include "../includes/Channel.hpp"

Channel::Channel(const std::string& n): name(n){}

Channel::Channel(const Channel& other)
{
    name = other.name;
    my_clients = other.my_clients;
    operators = other.operators;
}

const std::string& Channel::get_name() const
{
    return name;
}

const std::vector<Client*>& Channel::get_clients() const
{
    return my_clients;
}

bool Channel::it_is_a_channel_member(Client* client) const
{
    size_t i = 0;
    while (i < my_clients.size())
    {
        if (my_clients[i] == client)
            return true;
        i++;
    }
    return false;
}

const std::vector<int> &Channel::get_operators()
{
    return operators;
}

void Channel::add_client(Client* client)
{
    if (!it_is_a_channel_member(client))
        my_clients.push_back(client);
}

void Channel::add_operator(int fd)
{
    std::cout<<">>>>> fd = " << fd << "\n";
    size_t i = 0;
    while (i < operators.size())
    {
        if (operators[i] == fd)
            return;
        i++;
    }
    operators.push_back(fd);
    std::cout << "opp[0] = " << operators[0]<< "\n";
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
