# include "../../includes/Server.hpp"


void Server::handle_mode_i(Channel* chan, bool add)
{
        chan->set_invite_only(add);   
}

void Server::handle_mode_t(Channel* chan, bool add)
{
        chan->set_topic_restricted(add);
}

void Server::handle_mode_k(Client* client, Channel* chan, std::vector<std::string>& args, bool add, int &index)
{
    if (add)
    {
        if (index >= (int)args.size())
        {
            send_to_client(client->get_client_fd(), "MODE +k requires a key\n");
            return;
        }
        chan->set_key(args[index++], add);
    }
    else
        chan->set_key("", add);
}

void Server::handle_mode_l(Client* client, Channel* chan, std::vector<std::string>& args, bool add, int &index)
{
    if (add)
    {
        if (index >= (int)args.size())
        {
            send_to_client(client->get_client_fd(), "MODE +l requires a limit\n");
            return;
        }
        int limit = std::atoi(args[index++].c_str());
        chan->set_member_limit(limit, add);
    }
    else
        chan->set_member_limit(0, add);
}

void Server::handle_mode_o(Client* client, Channel* chan, std::vector<std::string>& args, bool add, int &index)
{
    if (index >= (int)args.size())
    {
        send_to_client(client->get_client_fd(), "MODE +o/-o requires a nickname\n");
        return;
    }

    std::string nick = args[index++];
    Client* target = find_client_by_nick(nick);
    if (!target || !chan->is_client_in_channel(target))
    {
        send_to_client(client->get_client_fd(), "there is no member named : " + nick + "\n");
        return;
    }

    if (add)
        chan->add_operator(target->get_client_fd());
    else if (!add && chan->is_operator_in_channel(target->get_client_fd()))
        chan->remove_operator(target->get_client_fd());
    else if (!add)
        send_to_client(client->get_client_fd(), "there is no operator named : " + nick + "\n");
}


void Server::apply_channel_mode_flags(Client* client, Channel* chan, std::vector<std::string>& args)
{
    std::string flag_str = args[2];
    bool add = true;
    int index = 3;
    size_t i = 0;

    while (i < flag_str.length())
    {
        char flag = flag_str[i];
        if (flag == '+') 
        {
            add = true;
            i++;
            continue;
        }
        if (flag == '-') 
        {
            add = false;
            i++;
            continue;
        }

        if (flag == 'i')
            handle_mode_i(chan, add);
        else if (flag == 't')
            handle_mode_t(chan, add);
        else if (flag == 'k')
            handle_mode_k(client, chan, args, add, index);
        else if (flag == 'l')
            handle_mode_l(client, chan, args, add, index);
        else if (flag == 'o')
            handle_mode_o(client, chan, args, add, index);
        else
        {
            send_to_client(client->get_client_fd(), std::string("Unknown mode: ") + flag + "\n");
            return;
        }

        ++i;
    }
    //add the the notification part 
}


void Server::handle_mode(Client* client, std::vector<std::string>& args)
{
    if (!checkClientAuthorization(client))
        return ;
    if (!client->get_client_registered())
    {
        send_to_client(client->get_client_fd(), "You are not registered yet\n");
        return ;
    }
    if (args.size() < 3)
    {
        send_to_client(client->get_client_fd(),  "not enough parameters for mode -_-\n");
        return ;
    }
    std::string name = args[1];
    if (name.empty() || name[0] != '#')
    {
        send_to_client(client->get_client_fd(),  name + " : is invalid channel name\n");
        return ;
    }
    Channel* chan = findChannel(name);
    if (!chan || !chan->is_client_in_channel(client))
    {
        send_to_client(client->get_client_fd(), "you are not a member of a channel named: " + name + "\n");
        return;
    }
    if (!chan->is_operator_in_channel(client->get_client_fd()))
    {
        send_to_client(client->get_client_fd(), "you are not an operator of a channel named: " + name + "\n");
        return;
    }
    apply_channel_mode_flags(client, chan, args);
    //parse the flags
    //seting the new conf to the chan
    //send the notifs


}