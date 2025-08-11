# include "../../includes/Server.hpp"

void Server::send_to_client(int clientFd, const std::string& message) 
{
    std::string msg = message;
    if (msg.substr(msg.size() - 2) != "\r\n")
        msg += "\r\n";
    if (send(clientFd, msg.c_str(), msg.length(), 0) == -1)
        std::cout << "Send failed " << std::endl;
}

Channel* Server::findChannel(const std::string& name) 
{
    size_t i = 0;
    while (i < my_channels.size()) 
    {
        if (my_channels[i]->get_name() == name)
            return my_channels[i];
        i++;
    }
    return NULL;
}

void Server::handle_join(Client* client, std::vector<std::string>& args)
{
    if (!checkClientAuthorization(client))
        return ;
    if (!checkClientRegistration(client))
        return ;
    if (args.size() < 2)
    {
        replyCode = 461;
        send_to_client(client->get_client_fd(), reply(client->get_client_nickname(), "JOIN :Not enough parameters"));
        return;
    }
    std::string name = args[1];
    if (name.empty() || name[0] != '#')
    {
        replyCode = 479;
        send_to_client(client->get_client_fd(), reply(client->get_client_nickname(), name + " :Invalid channel name"));
        return ;
    }
    Channel* chan = findChannel(name);
    if (chan == NULL)
    {
        Channel* new_channel = new Channel(name);
        new_channel->add_client(client, this);
        new_channel->add_operator(client->get_client_fd());
        my_channels.push_back(new_channel);
        return;
    }
    if (chan->get_inv_only())
    {
        if (!client->get_invitedChannels(chan))
        {
            replyCode = 473;
            send_to_client(client->get_client_fd(), reply(client->get_client_nickname(), name + " :Cannot join channel (+i) - invite only"));
            return ;
        }
    }
    if (chan->isKeyProtected())
    {
        if (args.size() != 3)
        {
            replyCode = 475;
            send_to_client(client->get_client_fd(), reply(client->get_client_nickname(), name + " :Cannot join channel (+k) - key required"));
            return ;
        }
        if (chan->get_key_word() != args[2])
        {
            replyCode = 475;
            send_to_client(client->get_client_fd(), reply(client->get_client_nickname(), name + " :Cannot join channel (+k) - incorrect key"));
            return ;
        }
    }
    if (chan->get_has_mem_lim() && chan->get_mem_lim() <= (int)chan->get_clients().size())
    {
        replyCode = 471;
        send_to_client(client->get_client_fd(), reply(client->get_client_nickname(), name + " :Cannot join channel (+l) - channel is full"));
        return ;
    }
    chan->add_client(client, this);
    send_to_client(client->get_client_fd(), ":" + client->get_prefix() + " JOIN :" + name);
}
