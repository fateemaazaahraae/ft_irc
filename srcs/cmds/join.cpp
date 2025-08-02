# include "../../includes/Server.hpp"

void Server::send_to_client(int clientFd, const std::string& message) 
{
    if (send(clientFd, message.c_str(), message.length(), 0) == -1)
    {
        std::cout << "send faild\n";
        return;
    }
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
    if (!client.get_client_authe())
    {
        send_to_client(client.get_client_fd(),  "You are not registed :(\n");
        return;
    }
    if (args.size() != 2)
    {
        send_to_client(client.get_client_fd(),  "bad join argument -_-\n");
        return;
    }
    std::string name = args[1];
    if (name.empty() || name[0] != '#') 
    {
        send_to_client(client.get_client_fd(),  name + " : is invalid channel name\n");
        return;
    }
    Channel* chan = findChannel(name);
    if (chan == NULL) 
    {
        Channel new_channel(name);
        new_channel.add_client(&client);
        my_channels.push_back(new_channel);
        std::cout<<"channel " << name << " has been created successfully !\n";
    } 
    else 
        chan->add_client(&client);
    send_to_client(client.get_client_fd(), "welcom: you have been added to " + name + " channel\n");
}