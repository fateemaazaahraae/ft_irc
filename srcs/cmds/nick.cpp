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
    if (!isalpha(str[0]))
        return 0;
    for (size_t i = 0; i < str.size(); i++)
    {
        if (!isalpha(str[i]) && !isalnum(str[i]))
            return 0;
    }
    return 1;
}

void Server::handle_nick(Client& client, std::vector<std::string> &args)
{
    if (!client.get_client_authe())
    {
        send(client.get_client_fd(), "You have to register!\n", 22, 0);
        return ;
    }
    if (args.size() != 2)
    {
        send(client.get_client_fd(), "Bad arguments\n", 14, 0);
        return ;
    }
    if (isNickTaken(args[1]))
    {
        send(client.get_client_fd(), "Nickname is already in use\n", 27, 0);
        return ;
    }
    if (isValidNickName(args[1]))
    {
        client.set_client_nickname(args[1]);
        send(client.get_client_fd(), "Your nickname set up successfully\n", 34, 0);
        std::cout << "Client set nickname" << std::endl;
    }
    else
        send(client.get_client_fd(), "NICK: Bad nickname\n",19, 0);
}
