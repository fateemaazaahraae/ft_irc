#include "../../includes/Server.hpp"

void Server::handle_invite(Client* client, std::vector<std::string>& args)
{
    if (!checkClientAuthorization(client))
        return;
    if (!checkClientRegistration(client))
        return;

    if (args.size() != 3)
    {
        replyCode = 461;
        send_to_client(client->get_client_fd(),
            reply(client->get_client_nickname(), args[0] + " :Not enough parameters"));
        return;
    }

    std::string targetNick = args[1];
    std::string channelName = args[2];

    Client* targetClient = getClient(targetNick);
    if (!targetClient)
    {
        replyCode = 401;
        send_to_client(client->get_client_fd(),
            reply(client->get_client_nickname(), targetNick + " :No such nick/channel"));
        return;
    }

    Channel* channel = findChannel(channelName);
    if (!channel)
    {
        replyCode = 403;
        send_to_client(client->get_client_fd(),
            reply(client->get_client_nickname(), channelName + " :No such channel"));
        return;
    }

    if (!channel->is_client_in_channel(client))
    {
        replyCode = 442;
        send_to_client(client->get_client_fd(),
            reply(client->get_client_nickname(), channelName + " :You're not on that channel"));
        return;
    }

    if (channel->hasInviteOnly() && !channel->is_operator_in_channel(client->get_client_fd()))
    {
        replyCode = 482;
        send_to_client(client->get_client_fd(),
            reply(client->get_client_nickname(), channelName + " :You're not channel operator"));
        return;
    }

    if (channel->is_client_in_channel(targetClient))
    {
        replyCode = 443;
        send_to_client(client->get_client_fd(),
            reply(client->get_client_nickname(), targetNick + " " + channelName + " :is already on channel"));
        return;
    }
    targetClient->set_invitedChannels(channelName);
    replyCode = 341;
    send_to_client(client->get_client_fd(), reply(client->get_client_nickname(), targetNick + " " + channelName));
    std::string inviteMsg = ":" + client->get_prefix() + " INVITE " + targetNick + " :" + channelName + "\r\n";
    send_to_client(targetClient->get_client_fd(), inviteMsg);
}
