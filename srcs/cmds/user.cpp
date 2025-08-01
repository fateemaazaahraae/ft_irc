#include "../../includes/Server.hpp"

void Server::handle_user(Client& client, std::vector<std::string> &args)
{
    if (!client.get_client_authe())
    {
        send(client.get_client_fd(), "You have to register!\n", 22, 0);
        return ;
    }
    if (args.size() < 4)
    {
        send(client.get_client_fd(), "Bad arguments\n", 14, 0);
        return ;
    }
    if (args[1] != "0" && args[1] != "0*")
        client.set_client_username(args[1]);
    if (client.get_client_username().empty())
    {
        send(client.get_client_fd(), "Username is required\n", 22, 0);
        return ;
    }

    for (size_t i = 2; i < args.size(); i++)
    {
        if (args[i][0] == ':')
        {
            args[i] = args[i].substr(1);
            args[i] = trim(args[i]);
            client.set_client_realname(args[i]);
            break;
        }
    }
    if (client.get_client_realname().empty())
    {
        send(client.get_client_fd(), "Realname is required\n", 21, 0);
        return ;
    }
    if (!client.get_client_nickname().empty() && !client.get_client_username().empty() && !client.get_client_realname().empty())
    {
        std::string welcome_msg = "Welcome to the IRC " + client.get_client_nickname() + "!" + client.get_client_username() + "@irc.com\n";
        send(client.get_client_fd(), welcome_msg.c_str(), welcome_msg.size(), 0);
    }
}
