# include "../../includes/Server.hpp"

void Server::handle_pass(Client& client, std::vector<std::string> &args)
{
    if (client.get_client_authe())
    {
        send(client.get_client_fd(), "You are already registed :)\n", 28, 0);
        return ;
    }
    if (args.size() != 2)
    {
        send(client.get_client_fd(), "Not enough parameters\n", 22, 0);
        return ;
    }
    if (pass == args[1])
    {
        client.set_client_authe();
        send(client.get_client_fd(), "You have registred successfully, Yay!\n", 38, 0);
        std::cout << "Client registred successfully !" << std::endl;
    }
    else
        send(client.get_client_fd(), "Incorrect password. Try again!\n", 31, 0);
}