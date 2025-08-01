# include "../../includes/Server.hpp"

void Server::send_to_client(int clientFd, const std::string& message) 
{
    if (send(clientFd, message.c_str(), message.length(), 0) == -1)
        std::cout << "Send failed--> fd = " << clientFd << " \n";
}

Channel* Server::findChannel(const std::string& name) 
{
    size_t i = 0;
    while ( i < my_channels.size()) 
    {
        if (my_channels[i].get_name() == name)
            return &my_channels[i];
        i++;
    }
    return NULL;
}

void Server::handle_join(Client& client, std::vector<std::string>& args)
{
    if (!checkClientAuthorization(client))
        return ;
    if (!client.get_client_registered())
    {
        send_to_client(client.get_client_fd(), "You are not registered yet\n");
        return ;
    }
    if (args.size() != 2)
    {
        send_to_client(client.get_client_fd(),  "bad join argument -_-\n");
        return ;
    }
    std::string name = args[1];
    if (name.empty() || name[0] != '#')
    {
        send_to_client(client.get_client_fd(),  name + " : is invalid channel name\n");
        return ;
    }
    Channel* chan = findChannel(name);
    if (chan == NULL) 
    {
        // client.set_client_operator();
        Channel new_channel(name);
        new_channel.add_client(&client);
        my_channels.push_back(new_channel);
        new_channel.add_operator(client.get_client_fd());
        std::cout << "channel " << name << " has been created successfully !\n";
        chan = &new_channel;
    } 
    else 
        chan->add_client(&client);
    send_to_client(client.get_client_fd(), "welcome: you have been added to " + name + " channel\n");
    const std::vector<int>& vec = chan->get_opaer();
    size_t j = 0;
    while (j < vec.size())
    {
        std::cout << "the client fd = " << client.get_client_fd() << " and the opp = " << vec[j] << "\n";
        if (client.get_client_fd() == vec[j])
            std::cout << "operator " << j << " -- " << vec[j] << std::endl;
        j++; 
    }
}