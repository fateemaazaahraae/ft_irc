# include "../../includes/Server.hpp"

int Server::isNickTaken(std::string nick)
{
    for (size_t i = 0; i < myClients.size(); i++)
    {
        if (myClients[i]->get_client_nickname() == nick)
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

void Server::handle_nick(Client* client, std::vector<std::string> &args)
{
    if (!checkClientAuthorization(client))
        return ;
    if (args.size() < 2)
    {
        replyCode = 431;
        std::string rep = reply(client->get_client_nickname(), "No nickname given");
        send_to_client(client->get_client_fd(), rep);
        return ;
    }
    std::string newNick = args[1];
    std::string oldNick = client->get_client_nickname();
    if (isNickTaken(newNick))
    {
        replyCode = 433;
        std::string rep = reply(client->get_client_nickname(), newNick + " :Nickname is already in use");
        send_to_client(client->get_client_fd(), rep);
        return ;
    }
    if (!isValidNickName(newNick))
    {
        replyCode = 432;
        std::string rep = reply(newNick, newNick + " :Erroneous nickname");
        send_to_client(client->get_client_fd(), rep);
        return ;
    }
    client->set_client_nickname(newNick);
    if (!oldNick.empty() && oldNick != newNick)
    {
        std::string nickChangeMsg = ":" + client->get_prefix() + " NICK :" + newNick + "\r\n";
        for (size_t i = 0; i < myClients.size(); i++)
        {
            if (myClients[i] != client)
                send_to_client(myClients[i]->get_client_fd(), nickChangeMsg);
        }
    }
    //* welcome message
    if (!client->get_client_nickname().empty() && !client->get_client_username().empty() && !client->get_client_realname().empty())
        welcomeClient(client);
}
