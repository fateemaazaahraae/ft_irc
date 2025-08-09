# include "../../includes/Server.hpp"

void Server::send_to_client(int clientFd, const std::string& message) 
{
    if (send(clientFd, message.c_str(), message.length(), 0) == -1)
        std::cout << "Send failed--> fd = " << clientFd << " \n";
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
        send_to_client(client->get_client_fd(),  "bad join argument -_-\n");
        return ;
    }
    std::string name = args[1];
    if (name.empty() || name[0] != '#')
    {
        send_to_client(client->get_client_fd(),  name + " : invalid channel name\n");
        return ;
    }
    Channel* chan = findChannel(name);
    if (chan == NULL) 
    {
        Channel new_channel(name);
        new_channel.add_client(client);
        new_channel.add_operator(client->get_client_fd());
        my_channels.push_back(new Channel(new_channel));
        std::cout << "channel " << name << ": has been created successfully !\n";
        chan = my_channels.back();
        return ;
    }    
    if (chan->get_inv_only() && !client->get_invitedChannels(chan))
    {
        send_to_client(client->get_client_fd(),  "You are not invited to this channel\n");
        return ;
    }
    if (chan->isKeyProtected())
    {
        if (args.size() != 3)
        {
            send_to_client(client->get_client_fd(),  "please enter the password to join this channel\n");
            return ;
        }
        if (chan->get_key_word() != args[2])
        {
            send_to_client(client->get_client_fd(),  "incorrect password to join this channel\n");
            return ;
        }
    }
    if (chan->get_has_mem_lim())
    {
        if (chan->get_mem_lim() == (int)chan->get_clients().size())
        {
            send_to_client(client->get_client_fd(),  "you can not join to this channel, the lim of member has been reached\n");
            return ;
        }
    }
    chan->add_client(client);
}
