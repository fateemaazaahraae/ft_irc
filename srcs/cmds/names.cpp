#include "../../includes/Server.hpp"

void Server::handle_names(Client* client, std::vector<std::string>& args)
{
    if (!checkClientAuthorization(client) || !checkClientRegistration(client))
        return;

    if (args.size() == 1)
    {
        for (size_t i = 0; i < myChannels.size(); i++)
            send_names_for_channel(client, myChannels[i]);
    }
    std::string channel = args[1];
    Channel* chan = findChannel(chanName);
    if (chan)
        send_names_for_channel(client, chan);
    else
    {
        replyCode = 366;
        std::string rep = reply(client->get_client_nickname(), channel + " :End of /NAMES list.");
        send_to_client(client->get_client_fd(), rep);
    }
}

void Server::send_names_for_channel(Client* client, Channel* chan)
{
    std::string names;

    const std::vector<Client*>& members = chan->get_clients();
    for (size_t i = 0; i < members.size(); i++)
    {
        if (i > 0)
            names += " ";
        names += members[i]->get_client_nickname();
    }
    std::string rep = reply(client->get_client_nickname(),
        "353 " + client->get_client_nickname() + " = " + chan->get_name() + " :" + names);
    send_to_client(client->get_client_fd(), rep);
    rep = reply(client->get_client_nickname(),
        "366 " + client->get_client_nickname() + " " + chan->get_name() + " :End of /NAMES list.");
    send_to_client(client->get_client_fd(), rep);
}
