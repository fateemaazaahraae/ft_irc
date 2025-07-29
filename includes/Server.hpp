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

# define MAX_CLIENT 2000

class Server
{
    private:
        int fd;
        int port;
        std::string pass;
        std::vector<struct pollfd> poll_fd;
        struct sockaddr_in serverAddr;
        
};
