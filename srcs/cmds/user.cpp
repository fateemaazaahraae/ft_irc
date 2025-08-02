#include "../../includes/Server.hpp"

bool Server::isValidUsername(std::string& username)
{
    if (username.empty())
        return false;
    for (size_t i = 0; i < username.length(); ++i)
    {
        char c = username[i];
        if (!isalnum(c) && c != '-' && c != '_' && c != '.')
            return false;
    }
    return true;
}

void Server::handle_user(Client& client, std::vector<std::string> &args)
{
    if (!checkClientAuthorization(client))
        return ;
    if (!checkClientRegistration(client))
        return ;
    if (args.size() < 5)
    {
        replyCode = 461;
        std::string rep = reply(client.get_client_nickname(), "USER :Not enough parameters");
        send_to_client(client.get_client_fd(), rep);
        return ;
    }
    client.set_client_username(args[1]);
    if (!isValidUsername(client.get_client_username()))
    {
        replyCode = 432;
        std::string rep = reply(client.get_client_nickname(), args[1] + " :Erroneous username");
        send_to_client(client.get_client_fd(), rep);
        return ;
    }
    if (args[4][0] == ':')
    {
        std::string realname = trim(args[4].substr(1));
        client.set_client_realname(realname);
    }

    if (client.get_client_realname().empty())
    {
        send_to_client(client.get_client_fd(), "Realname is required\n");
        return ;
    }
    //* welcome message
    if (!client.get_client_nickname().empty() && !client.get_client_username().empty() && !client.get_client_realname().empty())
        welcomeClient(client);
}
