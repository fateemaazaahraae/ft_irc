# include "../../includes/Server.hpp"

void Server::handle_mode_i(Channel* chan, bool add)
{
        chan->set_invite_only(add);   
}

void Server::handle_mode_t(Channel* chan, bool add)
{
        chan->set_topic_restricted(add);
}

int Server::handle_mode_k(Client* client, Channel* chan, std::vector<std::string>& args, bool add, int &index)
{
    if (add)
    {
        if (index >= (int)args.size())
        {
            replyCode = 461;
            std::string msg = reply(client->get_client_nickname(), "MODE " + chan->get_name() + " :Key required for +k mode");
            send_to_client(client->get_client_fd(), msg);
            return 0;
        }
        chan->set_key(args[index++], add);
    }
    else
        chan->set_key("", add);
    return 1;
}

int Server::handle_mode_l(Client* client, Channel* chan, std::vector<std::string>& args, bool add, int &index)
{
    if (add)
    {
        if (index >= (int)args.size())
        {
            replyCode = 461;
            std::string msg = reply(client->get_client_nickname(), "MODE " + chan->get_name() + " :Limit required for +l mode");
            send_to_client(client->get_client_fd(), msg);
            return 0;
        }
        int limit = std::atoi(args[index++].c_str());
        if (limit <= 0)
        {
            replyCode = 461;
            std::string msg = reply(client->get_client_nickname(), "MODE " + chan->get_name() + " :Invalid limit for +l mode");
            send_to_client(client->get_client_fd(), msg);
            return 0;
        }
        chan->set_member_limit(limit, add);
    }
    else
        chan->set_member_limit(0, add);
    return 1;
}

int Server::handle_mode_o(Client* client, Channel* chan, std::vector<std::string>& args, bool add, int &index)
{
    if (index >= (int)args.size())
    {
        replyCode = 461;
        std::string msg = reply(client->get_client_nickname(), "MODE " + chan->get_name() + " :Nickname required for +o/-o mode");
        send_to_client(client->get_client_fd(), msg);
        return 0;
    }

    std::string nick = args[index++];
    Client* target = find_client_by_nick(nick);
    if (!target || !chan->is_client_in_channel(target))
    {
        std::cout << "line 69\n";
        replyCode = 441;
        std::string msg = reply(client->get_client_nickname(), nick + " :They aren't on that channel");
        send_to_client(client->get_client_fd(), msg);
        return 0;
    }

    if (add)
        chan->add_operator(target->get_client_fd());
    else if (!add && chan->is_operator_in_channel(target->get_client_fd()))
        chan->remove_operator(target->get_client_fd());
    else if (!add)
    {
        replyCode = 482;
        std::string msg = reply(client->get_client_nickname(), nick + " :is not a channel operator");
        send_to_client(client->get_client_fd(), msg);
        return 0;
    }
    return 1;
}

void Server::notify_channel_mode_change(Client* client, Channel* chan, const std::vector<std::string>& args)
{
    std::string param = "";
    size_t i = 3;
    while( i < args.size())
    {
        param += " " + args[i];
        i++;
    }
    std::string notif = ":" + client->get_prefix() + " MODE " + chan->get_name() + " " + args[2] + param + "\r\n";
    std::vector<Client *> mem = chan->get_clients();
    i = 0;
    while(i  < mem.size())
    {
        send_to_client(mem[i]->get_client_fd(), notif);
        i++;
    }
}


void Server::apply_channel_mode_flags(Client* client, Channel* chan, std::vector<std::string>& args)
{
    std::string flag_str = args[2];
    bool add = true;
    int index = 3;
    size_t i = 0;
    int rtrn = 1;

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
            rtrn = handle_mode_k(client, chan, args, add, index);
        else if (flag == 'l')
            rtrn = handle_mode_l(client, chan, args, add, index);
        else if (flag == 'o')
            rtrn = handle_mode_o(client, chan, args, add, index);
        else
        {
            replyCode = 472;
            std::ostringstream oss;
            oss << ":" << serverName << " 472 "
                << client->get_client_nickname() << " "
                << flag << " :is unknown mode char to me\r\n";
            send_to_client(client->get_client_fd(), oss.str());
            return;
        }
        ++i;
    }
    if (rtrn == 1)
        notify_channel_mode_change(client, chan, args);
}


void Server::handle_mode(Client* client, std::vector<std::string>& args)
{
    if (!checkClientAuthorization(client))
        return ;
    if (!checkClientRegistration(client))
        return ;
    if (args.size() < 3)
    {
        if (args.size() == 2)
            return ;
        replyCode = 461;
        std::string rep = reply(client->get_client_nickname(), "MODE :Not enough parameters");
        send_to_client(client->get_client_fd(), rep);
        return ;
    }
    std::string name = args[1];
    if (name.empty() || name[0] != '#')
    {
        replyCode = 403;
        std::string rep = reply(client->get_client_nickname(), "MODE " + name + " :No such channel");
        send_to_client(client->get_client_fd(), rep);
        return ;
    }
    Channel* chan = findChannel(name);
    if (!chan)
    {
        replyCode = 403;
        std::string rep = reply(client->get_client_nickname(), "MODE " + name + " :No such channel");
        send_to_client(client->get_client_fd(), rep);
        return ;
    }
    if (!chan->is_client_in_channel(client))
    {
        replyCode = 442;
        std::string rep = reply(client->get_client_nickname(), "MODE " + name + " :You're not on that channel");
        send_to_client(client->get_client_fd(), rep);
        return;
    }
    if (!chan->is_operator_in_channel(client->get_client_fd()))
    {
        replyCode = 482;
        std::string rep = reply(client->get_client_nickname(), "MODE " + name + " :You're not channel operator");
        send_to_client(client->get_client_fd(), rep);
        return;
    }
    apply_channel_mode_flags(client, chan, args);
}