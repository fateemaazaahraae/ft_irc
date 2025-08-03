# include "../../includes/Server.hpp"

void Server::handle_mode(Client* client, std::vector<std::string>& args)
{
    if (!checkClientAuthorization(client))
        return ;
    if (!client->get_client_registered())
    {
        send_to_client(client->get_client_fd(), "You are not registered yet\n");
        return ;
    }
    if (args.size() < 3)
    {
        send_to_client(client->get_client_fd(),  "not enough parameters for mode -_-\n");
        return ;
    }
    std::string name = args[1];
    if (name.empty() || name[0] != '#')
    {
        send_to_client(client->get_client_fd(),  name + " : is invalid channel name\n");
        return ;
    }
    Channel* chan = findChannel(name);
    if (!chan || !chan->is_client_in_channel(client))
    {
        send_to_client(client->get_client_fd(), "you are not a member of a channel named: " + name + "\n");
        return;
    }
    if (!chan->is_operator_in_channel(client->get_client_fd()))
    {
        send_to_client(client->get_client_fd(), "you are not an operator of a channel named: " + name + "\n");
        return;
    }
    //parse the flags
    //seting the new conf to the chan
    //send the notifs


}