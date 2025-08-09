# include "../../includes/Server.hpp"

Client* Server::getClient(std::string nickname)
{
    for (size_t i = 0; i < myClients.size(); ++i)
    {
        if (myClients[i]->get_client_nickname() == nickname)
            return myClients[i];
    }
    return NULL;
}

#include "../../includes/Server.hpp"

void Server::handle_kick(Client* client, std::vector<std::string>& args)
{
    if (!checkClientAuthorization(client))
        return;
    if (!checkClientRegistration(client))
        return;

    if (args.size() < 3)
    {
        replyCode = 461;
        send_to_client(client->get_client_fd(), reply(client->get_client_nickname(), args[0] + " :Not enough parameters"));
        return;
    }

    std::string channelName = args[1];
    std::string targetNickname = args[2];
    std::string reason = (args.size() > 3 && !args[3].empty())
                         ? trim(args[3].substr(1))
                         : "No reason given";
    Channel* channel = findChannel(channelName);
    if (!channel)
    {
        replyCode = 403;
        send_to_client(client->get_client_fd(), reply(client->get_client_nickname(), channelName + " :No such channel"));
        return;
    }
    if (!channel->is_client_in_channel(client))
    {
        replyCode = 442;
        send_to_client(client->get_client_fd(), reply(client->get_client_nickname(), channelName + " :You're not on that channel"));
        return;
    }
    if (!channel->is_operator_in_channel(client->get_client_fd()))
    {
        replyCode = 482;
        send_to_client(client->get_client_fd(), reply(client->get_client_nickname(), channelName + " :You're not channel operator"));
        return;
    }

    Client* targetClient = getClient(targetNickname);
    if (!targetClient)
    {
        replyCode = 401;
        send_to_client(client->get_client_fd(), reply(client->get_client_nickname(), targetNickname + " :No such nick/channel"));
        return;
    }

    if (!channel->is_client_in_channel(targetClient))
    {
        replyCode = 442;
        send_to_client(client->get_client_fd(), reply(client->get_client_nickname(), channelName + " :User is not on that channel"));
        return;
    }
    channel->removeClient(targetClient->get_client_fd());
    std::string kickMsg = ":" + client->get_prefix() + " KICK " + channelName + " " + targetClient->get_client_nickname() + " :" + reason + "\r\n";
    sending_msg_in_chan(client, kickMsg, channelName);
}
