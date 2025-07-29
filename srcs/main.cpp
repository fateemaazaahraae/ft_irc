#include "../includes/Server.hpp"

#define BUFFER_SIZE 1000

int main(int ac, char **av)
{
    Server my_serv;

    try
    {
        if (ac != 3)
            throw std::runtime_error("Usage: ./ircserv <port> <password>");

        my_serv.initServer(av[1], av[2]);
        my_serv.createServerSocket();
        std::cout << MAG << "Server start Listening on " << av[1] << RESET << std::endl;
        my_serv.serverLoop();
        std::cout << MAG << "Server shut down" << RESET << std::endl;
    }
    catch (std::exception &e)
    {
        std::cout << RED << "Error: " << e.what() << RESET << std::endl;
    }
}
