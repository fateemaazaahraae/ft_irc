# include "../../includes/Server.hpp"

void Server::bot_check_message(Channel* chan, const std::string& msg)
{
    std::string reply;

    if (msg == "!happy")
    {
        reply = ":" + bot->get_client_nickname() +
                "!" + bot->get_client_username() +
                "@server PRIVMSG " + chan->get_name() +
                " 🤲 الحمد لله\r\n";
    }
    else if (msg == "!sad")
    {
        reply = ":" + bot->get_client_nickname() +
                "!" + bot->get_client_username() +
                "@server PRIVMSG " + chan->get_name() +
                " { قُلْ يَا عِبَادِيَ الَّذِينَ أَسْرَفُوا عَلَى أَنفُسِهِمْ لَا تَقْنَطُوا مِن رَّحْمَةِ اللَّهِ } 🤍\r\n";
    }
    else if (msg == "!time")
    {
        time_t now = time(0);
        char buffer[80];
        strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&now));
        
        reply = ":" + bot->get_client_nickname() +
                "!" + bot->get_client_username() +
                "@server PRIVMSG " + chan->get_name() +
                " ⏰ Current server time is: " + buffer + "\r\n";
    }
    else if (msg == "!help")
    {
        reply = ":" + bot->get_client_nickname() +
                "!" + bot->get_client_username() +
                "@server PRIVMSG " + chan->get_name() +
                " 📜 Commands: !happy, !sad, !time, !joke, !help, !lonely, !angry\r\n";
    }
    else if (msg == "!joke")
    {
        reply = ":" + bot->get_client_nickname() +
                "!" + bot->get_client_username() +
                "@server PRIVMSG " + chan->get_name() +
                " 😄 Why don’t programmers like nature? Too many bugs!\r\n";
    }
    else if (msg == "!angry")
    {
        reply = ":" + bot->get_client_nickname() +
                "!" + bot->get_client_username() +
                "@server PRIVMSG " + chan->get_name() +
                " { وَالْكَاظِمِينَ الْغَيْظَ وَالْعَافِينَ عَنِ النَّاسِ ۗ وَاللَّهُ يُحِبُّ الْمُحْسِنِينَ } 🤲\r\n";
    }
    else if (msg == "!lonely")
    {
        reply = ":" + bot->get_client_nickname() +
                "!" + bot->get_client_username() +
                "@server PRIVMSG " + chan->get_name() +
                " { وَاصْبِرْ لِحُكْمِ رَبِّكَ فَإِنَّكَ بِأَعْيُنِنَا وَسَبِّحْ بِحَمْدِ رَبِّكَ حِينَ تَقُومُ } 🤍\r\n";
    }
    if (!reply.empty())
        broadcastMessage(chan, reply);
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