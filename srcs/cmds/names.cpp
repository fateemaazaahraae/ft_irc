#include "../../includes/Server.hpp"

void Server::send_names_for_channel(Client* client, Channel* chan)
{
    std::string names;

    const std::vector<Client*>& members = chan->get_clients();
    for (size_t i = 0; i < members.size(); i++)
    {
        if (i > 0)
            names += " ";
        if (chan->is_operator_in_channel(members[i]->get_client_fd()))
            names += "@";
        names += members[i]->get_client_nickname();
    }
    replyCode = 353;
    std::string rep = reply(client->get_client_nickname(), " = " + chan->get_name() + " :" + names);
    send_to_client(client->get_client_fd(), rep);
    replyCode = 366;
    rep = reply(client->get_client_nickname(), chan->get_name() + " :End of /NAMES list.");
    send_to_client(client->get_client_fd(), rep);
}

void Server::handle_names(Client* client, std::vector<std::string>& args)
{
    if (!checkClientAuthorization(client) || !checkClientRegistration(client))
        return;

    if (args.size() == 1)
    {
        for (size_t i = 0; i < my_channels.size(); i++)
            send_names_for_channel(client, my_channels[i]);
    }
    std::string channel = args[1];
    Channel* chan = findChannel(channel);
    if (chan)
        send_names_for_channel(client, chan);
    else
    {
        replyCode = 366;
        std::string rep = reply(client->get_client_nickname(), channel + " :End of /NAMES list.");
        send_to_client(client->get_client_fd(), rep);
    }
}