# include "../includes/Channel.hpp"

Channel::Channel(const std::string& n): name(n){}

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

const std::vector<int> &Channel::get_opaer()
{
    return oprators;
}

void Channel::add_client(Client* client)
{
    if (!it_is_a_channel_member(client))
        my_clients.push_back(client);
}

void Channel::add_operator(int fd)
{
    std::cout<<">>>>>> fd = " << fd << "\n";
    size_t i = 0;
    while (i < oprators.size())
    {
        if (oprators[i] == fd)
            return;
        i++;
    }
    oprators.push_back(fd);
    std::cout << "opp[0] = " << oprators[0]<< "\n";
}
