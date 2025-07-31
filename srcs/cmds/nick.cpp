# include "../../includes/Server.hpp"

int Server::isNickTaken(std::string nick)
{
    for (size_t i = 0; i < myClients.size(); i++)
    {
        if (myClients[i].get_client_nickname() == nick)
            return 1;
    }
    return 0;
}

int Server::isValidNickName(std::string str)
{
    (void)str;
    return 0;
}

void Server::handle_nick(Client& client, std::vector<std::string> &args)
{
    if (args.size() != 2)
    {
        send(client.get_client_fd(), "NICK: Bad arguments\n", 20, 0);
        return ;
    }
    if (client.get_client_nickname() != "")
    {
        send(client.get_client_fd(), "You already have a nick name\n", 29, 0);
        return ;
    }
    if (isNickTaken(args[1]))
    {
        send(client.get_client_fd(), "Nickname is already in use\n", 29, 0);
        return ;
    }
    if (isValidNickName(args[1]))
    {}
    else
        send(client.get_client_fd(), "NICK: Bad nickname\n",19, 0);
}
