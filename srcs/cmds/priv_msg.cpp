# include "../../includes/Server.hpp"

void Server::bot_check_message(Channel* chan, const std::string& msg)
{
    std::cout <<"hiii";
    if (msg == "!dakir")
    {
        std::string reply = ":" + bot->get_client_nickname() +
                            "!" + bot->get_client_username() +
                            "@server PRIVMSG " + chan->get_name() +
                            " :الحمد لله\r\n";
        broadcastMessage(chan, reply);
    }
}


void Server::sending_msg_in_chan(Client* client, std::string message, std::string target)
{
    Channel* chan = findChannel(target);
    if (!chan || !chan->is_client_in_channel(client))
    {
        replyCode = 404;
        std::string rep = reply(client->get_client_nickname(), target + " :Cannot send to channel (not a member)");
        send_to_client(client->get_client_fd(), rep);
        return;
    }
    const std::vector<Client*>& members = chan->get_clients();

    std::string fullMsg = ":" + client->get_prefix() + " PRIVMSG " + target + " :" + message + "\r\n";

    size_t i = 0;
    while (i < members.size())
    {
        if (members[i]->get_client_fd() != client->get_client_fd())
            send_to_client(members[i]->get_client_fd(), fullMsg);
        i++;
    }
    std::cout << "line 38\n";
    bot_check_message(chan, message);
}

void Server::sending_msg_to_user(Client* client, std::string message, std::string target)
{
    size_t i = 0;
    while (i < myClients.size())
    {
        if (myClients[i]->get_client_nickname() == target)
        {
            std::string fullMsg = ":" + client->get_prefix() + " PRIVMSG " + target + " :" + message + "\r\n";
            send_to_client(myClients[i]->get_client_fd(), fullMsg);
            return;
        }
        i++;
    }
    replyCode = 401;
    std::string rep = reply(client->get_client_nickname(), target + " :No such nick/channel");
    send_to_client(client->get_client_fd(), rep);
}

void Server::handle_priv_msg(Client* client, std::vector<std::string>& args)
{
    if (!checkClientAuthorization(client))
        return ;
    if (!checkClientRegistration(client))
        return ;
    if (args.size() < 3)
    {
        replyCode = 411;
        std::string rep = reply(client->get_client_nickname(), "No text to send");
        send_to_client(client->get_client_fd(), rep);
        return;
    }
    std::string target = args[1];
    std::string message;
    size_t i = 2;
    while (i < args.size())
    {
        if (i != 2)
            message += " ";
        message += args[i];
        i++;
    }
    if (message[0] == ':')
        message = message.substr(1);

    if (target[0] == '#')
        sending_msg_in_chan(client, message, target);
    else 
        sending_msg_to_user(client, message, target);
}