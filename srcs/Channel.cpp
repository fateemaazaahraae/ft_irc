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
void Channel::add_client(Client* client)
{
    if (!it_is_a_channel_member(client))
        my_clients.push_back(client);
}