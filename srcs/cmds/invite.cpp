# include "../../includes/Server.hpp"

void Server::handle_invite(Client* client, std::vector<std::string>& args)
{
    if (!checkClientAuthorization(client))
        return ;
    if (!checkClientRegistration(client))
        return ;
    if (args.size() != 3)
    {
        replyCode = 461;
        std::string rep = reply(client->get_client_nickname(), "Wrong number of parameters");
        send_to_client(client->get_client_fd(), rep);
        return ;
    }
    std::string channelName = args[2];
    std::string targetNick = args[1];
    Client* targetClient = getClient(targetNick);
    if (!targetClient)
    {
        replyCode = 401;
        std::string rep = reply(client->get_client_nickname(), "No such user with that nickname");
        send_to_client(client->get_client_fd(), rep);
        return ;
    }
    Channel* channel = findChannel(channelName);
    if (!channel)
    {
        replyCode = 403;
        std::string rep = reply(client->get_client_nickname(), "No such channel");
        send_to_client(client->get_client_fd(), rep);
        return ;
    }
    if (!channel->is_client_in_channel(client))
    {
        replyCode = 442;
        std::string rep = reply(client->get_client_nickname(), "You're not on that channel");
        send_to_client(client->get_client_fd(), rep);
        return ;
    }
    if (channel->hasInviteOnly() && !channel->is_operator_in_channel(client->get_client_fd()))
    {
        replyCode = 482;
        std::string rep = reply(client->get_client_nickname(), (channel->get_name() + " :You're not channel operator"));
        send_to_client(client->get_client_fd(), rep);
        return ;
    }
    if (channel->is_client_in_channel(targetClient))
    {
        replyCode = 443;
        std::string rep = reply(client->get_client_nickname(), "User is already in the channel");
        send_to_client(client->get_client_fd(), rep);
        return ;
    }
    targetClient->set_invitedChannels(channelName);
    replyCode = 341;
    std::string rep = reply(client->get_client_nickname(), "Invited you to " + channelName);
    send_to_client(targetClient->get_client_fd(), rep);
}