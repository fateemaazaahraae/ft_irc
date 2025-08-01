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
# include <signal.h>
#include <algorithm>
#include <cctype>
#include "Client.hpp"

#define RED "\e[1;31m"
#define WHI "\e[0;37m"
#define GRE "\e[1;32m"
#define BROW "\e[1;33m"
#define MAG "\e[1;35m"
#define CYN "\e[1;36m"
#define RESET "\e[0m"

# define MAX_CLIENT 2000


class Client;

class Server
{
    private :
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
        void executeClientCommand(Client& client, const std::string& cmd);
        void executeCommand(Client &client, std::vector<std::string> &args);
        void handle_pass(Client& client, std::vector<std::string> &args);
        void handle_nick(Client& client, std::vector<std::string> &args);
        std::string &convertCmdToUpperCase(std::string &str);
        int isNickTaken(std::string nick);
        int isValidNickName(std::string str);
        int check_port(std::string port);
        std::string trim(const std::string& str);
        std::vector<std::string> get_arg(std::string cmd);
        void handle_user(Client& client, std::vector<std::string> &args);

};
