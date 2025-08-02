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
    if (!checkClientAuthorization(client))
        return ;
    if (!checkClientRegistration(client))
        return ;
    if (args.size() < 2)
    {
        replyCode = 431;
        std::string rep = reply(client.get_client_nickname(), "No nickname given");
        send(client.get_client_fd(), rep.c_str(), rep.size(), 0);
        return ;
    }
    if (isNickTaken(args[1]))
    {
        replyCode = 433;
        std::string rep = reply(client.get_client_nickname(), "Nickname is already in use");
        send(client.get_client_fd(), rep.c_str(), rep.size(), 0);
        return ;
    }
    if (isValidNickName(args[1]))
    {
        client.set_client_nickname(args[1]);
        std::cout << "Client set nickname" << std::endl;
    }
    else
    {
        replyCode = 432;
        std::string rep = reply(args[1], "Bad nickname");
        send(client.get_client_fd(), rep.c_str(), rep.size(), 0);
    }
    //* welcome message
    if (!client.get_client_nickname().empty() && !client.get_client_username().empty() && !client.get_client_realname().empty())
        welcomeClient(client);
}
