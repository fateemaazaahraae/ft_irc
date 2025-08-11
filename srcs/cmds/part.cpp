# include "../../includes/Server.hpp"

void Server::handle_part(Client* client, std::vector<std::string>& args)
{
    if (!checkClientAuthorization(client))
        return;
    if (!checkClientRegistration(client))
        return;

    if (args.size() < 2)
    {
        replyCode = 461;
        std::string rep = reply(client->get_client_nickname(), "PART :Not enough parameters");
        send_to_client(client->get_client_fd(), rep);
        return;
    }
    std::string channelName = args[1];
    Channel* channel = findChannel(channelName);
    if (!channel)
    {
        replyCode = 403;
        std::string rep = reply(client->get_client_nickname(), channelName + " :No such channel");
        send_to_client(client->get_client_fd(), rep);
        return;
    }

    if (!channel->is_client_in_channel(client))
    {
        replyCode = 442;
        std::string rep = reply(client->get_client_nickname(), channelName + " :You're not on that channel");
        send_to_client(client->get_client_fd(), rep);
        return;
    }
    std::string partMsg = args.size() > 2 ? args[2].trim(args[2].substr(1)) : "";

    std::string broadcastMsg = ":" + client->get_prefix() + " PART " + channelName;
    if (!partMsg.empty())
        broadcastMsg += " :" + partMsg;
    broadcastMsg += "\r\n";
    broadcastMessage(channel, broadcastMsg);
    channel->removeClient(client->get_client_fd());
}
