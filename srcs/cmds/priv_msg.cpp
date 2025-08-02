# include "../../includes/Server.hpp"

void  Server::sending_msg_in_chan(Client& client, std::string message, std::string target)
{
        Channel* chan = findChannel(target);
        if (!chan)
        {
            send_to_client(client.get_client_fd(), "you are not a member of a channel named: " + target + "\n");
            return;
        }
        const std::vector<Client*>& members = chan->get_clients();
        size_t i = 0;
        while (i < members.size())
        {
            if (members[i] != &client)
            {
                std::cout<< "this is >>> " << members[i]->get_client_fd();
                send_to_client(members[i]->get_client_fd(),"PRIVMSG: " + message + " from: (" + client.get_client_username() + ") in " + target + " channel\n");
            }
            i++;
        }
}

void Server::sending_msg_to_user(Client& client, std::string message, std::string target)
{
        size_t i = 0;
        while (i < myClients.size())
        {
            if (myClients[i].get_client_username() == target)
            {
                send_to_client(myClients[i].get_client_fd(), " PRIVMSG: " + message + " from: (" + client.get_client_username() + ")n");
                return;
            }
            i++;
        }
        send_to_client(client.get_client_fd(), "the user named " + target + " not found\n");   
}

void Server::handle_priv_msg(Client& client, std::vector<std::string>& args)
{
    if (!client.get_client_authe())
    {
        send_to_client(client.get_client_fd(),  "You are not registed :(\n");
        return;
    }
    if (args.size() < 3)
    {
        send_to_client(client.get_client_fd(), "not enough parameters for PRIVMSG\n");
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
        sending_msg_in_chan(client, message, target);//wlat xi 5trat katfaily i dont know whyyyyy i should sleeeepp (- -)zzzzzzzzz
    else 
        sending_msg_to_user(client, message, target);
}
