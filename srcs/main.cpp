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
        my_serv.serverLoop();
    }
    catch(std::exception &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
}
