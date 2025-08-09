# include "../../includes/Server.hpp"

void Server::displayTopic(Client* client, std::string channelName)
{
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
    std::string topic = channel->getTopic();
    if (topic.empty())
    {
        replyCode = 331;
        std::string rep = reply(client->get_client_nickname(), channelName + " :No topic is set");
        send_to_client(client->get_client_fd(), rep);
        return;
    }
    replyCode = 332;
    std::string rep = reply(client->get_client_nickname(), channelName + " :" + topic);
    send_to_client(client->get_client_fd(), rep);
}

void Server::changeTopic(Client* client, std::string channelName, std::string topic)
{
    Channel* channel = findChannel(channelName);
    if (!channel)
    {
        replyCode = 403;
        std::string rep = reply(client->get_client_nickname(), channelName + " :No such channel");
        send_to_client(client->get_client_fd(), rep);
        return;
    }
    if (channel->hasTopicProtection() && !channel->is_operator_in_channel(client->get_client_fd()))
    {
        replyCode = 482;
        std::string rep = reply(client->get_client_nickname(), channelName + " :You're not channel operator");
        send_to_client(client->get_client_fd(), rep);
        return;
    }
    channel->set_topic(topic);
    std::string topicChangeMsg = ":" + client->get_prefix() + " TOPIC " + channelName + " :" + topic + "\r\n";
    sending_msg_in_chan(client, topicChangeMsg, channelName);
}

void Server::handle_topic(Client* client, std::vector<std::string>& args)
{
    if (!checkClientAuthorization(client))
        return ;
    if (!checkClientRegistration(client))
        return ;
    if (args.size() < 2)
    {
        replyCode = 461;
        std::string rep = reply(client->get_client_nickname(), "TOPIC :Not enough parameters");
        send_to_client(client->get_client_fd(), rep);
        return;
    }
    if (args.size() == 2)
        displayTopic(client, args[1]);
    else if (args.size() == 3)
        changeTopic(client, args[1], trim(args[2].substr(1)));
}