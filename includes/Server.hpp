#pragma once

# include <iostream>
# include <cstring>
# include <string>
# include <vector>
# include <fcntl.h>
# include <unistd.h>
# include <cstdlib>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <poll.h>
# include <exception>
# include <limits.h>
#include "Client.hpp"

# define MAX_CLIENT 2000

class Client;

class Server
{
    private:
        int fd;
        int port;
        std::string pass;
        std::vector<struct pollfd> poll_fd;
        std::vector<Client> myClients;
        struct sockaddr_in serverAddr;
    public :
        Server();
        ~Server();
        void initServer(std::string port, std::string pass);
        void createServerSocket();
        void serverLoop();
        void acceptNewConnection();
        void receiveNewData(int clientFd);
        void removeClient(int clientFd);
};
