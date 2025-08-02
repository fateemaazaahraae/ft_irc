# include "../../includes/Server.hpp"

void Server::handle_pass(Client& client, std::vector<std::string> &args)
{
    if (client.get_client_authe())
    {
        replyCode = 462;
        std::string rep = reply(client.get_client_nickname(), "You are already authenticated");
        send_to_client(client.get_client_fd(), rep);
        return ;
    }
    if (args.size() < 2)
    {
        replyCode = 461;
        std::string rep = reply(client.get_client_nickname(), "Not enough parameters");
        send_to_client(client.get_client_fd(), rep);
        return ;
    }
    if (pass == args[1])
    {
        client.set_client_authe();
        std::cout << "Client authenticated successfully !" << std::endl;
    }
    else
    {
        replyCode = 464;
        std::string rep = reply(client.get_client_nickname(), "Password incorrect");
        send_to_client(client.get_client_fd(), rep);
    }
}