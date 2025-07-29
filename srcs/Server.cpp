#include "../includes/Server.hpp"

Server::Server()
{
    fd = -1;
    port = 0;
    pass = "";
}

int   check_port(std::string port)
{
    if (port.find_first_not_of("0123456789") != std::string::npos)
        throw std::runtime_error("Port contain non numeric characters");
    int res = atoi(port.c_str());
    if (res < 1024 || res > 65535)
        throw std::runtime_error("Invalid port range");
    return res;
}

void Server::initServer(std::string port, std::string pass)
{
    this->port = check_port(port);
    this->pass = pass;
}

void Server::createServerSocket()
{
    struct pollfd pollServer;

    this->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        throw std::runtime_error("Failed to create Server Socket");
    
    int option = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0)
        throw std::runtime_error("Failed to set Socket option");
    if (fcntl(this->fd, F_SETFL, O_NONBLOCK) < 0)
        throw std::runtime_error("fcntl() failed");
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    if (bind(fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
        throw std::runtime_error("Bind() failed");
    if (listen(fd, MAX_CLIENT) < 0)
        throw std::runtime_error("Listen() failed");
    pollServer.fd = this->fd;
    pollServer.events = POLLIN;
    pollServer.revents = 0;
    poll_fd.push_back(pollServer);
}

void Server::acceptNewConnection()
{
    Client new_client;
    struct pollfd clientPoll;

    socklen_t len = sizeof(new_client.get_client_addr());
    int new_fd = accept(fd, (struct sockaddr*)&new_client.get_client_addr(), &len);
    if (new_fd < 0)
        return ;
    new_client.set_client_fd(new_fd);
    myClients.push_back(new_client);
    clientPoll.fd = new_fd;
    clientPoll.events = POLLIN;
    clientPoll.revents = 0;
    poll_fd.push_back(clientPoll);
}

void Server::removeClient(int clientFd)
{
    for (std::vector<struct pollfd>::iterator it = poll_fd.begin(); it != poll_fd.end(); it++)
    {
        if (it->fd == clientFd)
            poll_fd.erase(it);
    }

    for (std::vector<Client>::iterator it = myClients.begin(); it != myClients.end(); it++)
    {
        if (it->get_client_fd() == clientFd)
            myClients.erase(it);
    }
}

void Server::receiveNewData(int clientFd)
{
    char buffer[1024];
    
    std::memset(&buffer, 0, sizeof(buffer));
    int readBytes = recv(clientFd, &buffer, sizeof(buffer) - 1, 0);
    if (readBytes <= 0)
    {
        std::cout << "Client Disconnected (fd = " << clientFd << ")" << std::endl;
        close(clientFd);
        removeClient(clientFd);
    }
    else
    {
        buffer[readBytes] = '\0';
        for (size_t i = 0; i < myClients.size(); i++)
        {
            if (myClients[i].get_client_fd() == clientFd)
            {
                myClients[i].set_client_buffer(myClients[i].get_client_buffer() + buffer);
                // executeClientCommand(myClients[i]);
                std::cout << "--> " << myClients[i].get_client_buffer() << std::endl;
                myClients[i].set_client_buffer("");
                break;
            }
        }
    }
}

void Server::serverLoop()
{
    while (true) //! to add signalHandler
    {
        int poll_count = poll(this->poll_fd.data(), this->poll_fd.size(), -1);
        if (poll_count < 0)
            throw std::runtime_error("poll() failed");
        for (size_t i = 0; i < this->poll_fd.size(); i++)
        {
            if (poll_fd[i].revents & POLLIN)
            {
                if (poll_fd[i].fd == this->fd)
                    acceptNewConnection();
                else
                    receiveNewData(poll_fd[i].fd);
            }
        }
    }
}

Server::~Server(){}

//TODO ---->   1. handle signal
//TODO ---->   2. \r\n