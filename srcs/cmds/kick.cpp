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

void Server::handle_kick(Client* client, std::vector<std::string>& args)
{
    if (!checkClientAuthorization(client))
        return ;
    if (!checkClientRegistration(client))
        return ;
    if (args.size() < 3) //! Check if there are enough arguments
    {
        replyCode = 461;
        std::string rep = reply(client->get_client_nickname(), "Not enough parameters");
        send_to_client(client->get_client_fd(), rep);
        return ;
    }
    std::string channelName = args[1];
    std::string targetNickname = args[2];
    std::string reason = "";
    if (args.size() > 3) //! Check if the reason is provided
    if (!args[3].empty())
    reason = trim(args[3].substr(1));
    Channel* channel = findChannel(channelName);
    if (!channel) //! Check if the channel exists
    {
        replyCode = 403;
        std::string rep = reply(client->get_client_nickname(), "No such channel");
        send_to_client(client->get_client_fd(), rep);
        return ;
    }
    if (!channel->is_operator_in_channel(client->get_client_fd())) //! Check if the client is an operator in the channel
    {
        replyCode = 482;
        std::string rep = reply(client->get_client_nickname(), "You're not channel operator");
        send_to_client(client->get_client_fd(), rep);
        return ;
    }
    Client* targetClient = getClient(targetNickname);
    if (!targetClient) //! Check if the target client exists
    {
        replyCode = 401;
        std::string rep = reply(client->get_client_nickname(), "No such user with that nickname");
        send_to_client(client->get_client_fd(), rep);
        return ;
    }
    if (!channel->is_client_in_channel(targetClient)) //! Check if the target is a client in the channel
    {
        replyCode = 442;
        std::string rep = reply(client->get_client_nickname(), "User not on channel");
        send_to_client(client->get_client_fd(), rep);
        return ;
    }
    channel->removeClient(targetClient->get_client_fd()); //! Remove the target client from the channel
    std::string kickMessage = ":" + client->get_client_nickname() + " KICK " + channelName + " " + targetClient->get_client_nickname();
    if (!reason.empty())
        kickMessage += " :" + reason;
    else
        kickMessage += " :No reason provided";
    sending_msg_in_chan(client, kickMessage, channelName); //! Notify the channel about the kick
    std::string rep = reply(targetClient->get_client_nickname(), "You have been kicked from " + channelName);
    send_to_client(targetClient->get_client_fd(), rep);
}