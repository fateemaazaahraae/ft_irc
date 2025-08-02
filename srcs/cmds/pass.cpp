# include "../../includes/Server.hpp"

void Server::handle_pass(Client& client, std::vector<std::string> &args)
{
    if (client.get_client_authe())
    {
        send(client.get_client_fd(), "You are already authenticated :)\n", 33, 0);
        return ;
    }
    if (args.size() < 2)
    {
        replyCode = 461;
        std::string rep = reply(client.get_client_nickname(), "Not enough parameters");
        send(client.get_client_fd(), rep.c_str(), rep.size(), 0);
        return ;
    }
    if (pass == args[1])
    {
        client.set_client_authe();
        std::cout << "Client registred successfully !" << std::endl;
    }
    else
    {
        replyCode = 464;
        std::string rep = reply(client.get_client_nickname(), "Password incorrect");
        send(client.get_client_fd(), rep.c_str(), rep.size(), 0);
    }
}