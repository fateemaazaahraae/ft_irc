# include "../../includes/Server.hpp"

// void  Server::sending_msg_in_chan(Client* client, std::string message, std::string target)
// {
//         Channel* chan = findChannel(target);
//         if (!chan || !chan->is_client_in_channel(client))
//         {
//             send_to_client(client->get_client_fd(), "you are not a member of a channel named: " + target + "\n");
//             return;
//         }
//         const std::vector<Client*>& members = chan->get_clients();
//         size_t j = 0;
//         while (j < members.size())
//         {
//             std::cout<< "member num [" << j << "] and it fd = " << members[j]->get_client_fd() << " and it nick name is: " << members[j]->get_client_nickname() << "\n";
//             j++;
//         }
//         size_t i = 0;
//         while (i < members.size())
//         {
//             if (members[i]->get_client_fd() != client->get_client_fd())
//                 send_to_client(members[i]->get_client_fd(),"PRIVMSG: " + message + " from: (" + client->get_client_nickname() + ") in " + target + " channel\n");
//             i++;
//         }
// }

// void Server::sending_msg_to_user(Client* client, std::string message, std::string target)
// {
//         size_t i = 0;
//         while (i < myClients.size())
//         {
//             if (myClients[i]->get_client_nickname() == target)
//             {
//                 send_to_client(myClients[i]->get_client_fd(), "PRIVMSG: " + message + " from: (" + client->get_client_nickname() + ")\n");
//                 return;
//             }
//             i++;
//         }
//         send_to_client(client->get_client_fd(), "the user named " + target + " not found\n");
// }

// void Server::handle_priv_msg(Client* client, std::vector<std::string>& args)
// {
//     if (!checkClientAuthorization(client))
//         return ;
//     if (!checkClientRegistration(client))
//         return ;
//     if (args.size() < 3)
//     {
//         send_to_client(client->get_client_fd(), "not enough parameters for PRIVMSG\n");
//         return;
//     }
//     std::string target = args[1];
//     std::string message;
//     size_t i = 2;
//     while (i < args.size())
//     {
//         if (i != 2)
//             message += " ";
//         message += args[i];
//         i++;
//     }
//     if (message[0] == ':')
//         message = message.substr(1);
//     if (target[0] == '#')
//         sending_msg_in_chan(client, message, target);
//     else 
//         sending_msg_to_user(client, message, target);
// }

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