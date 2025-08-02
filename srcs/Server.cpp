#include "../includes/Server.hpp"

extern int g_is_running; 

Server::Server()
{
    fd = -1;
    port = 0;
    pass = "";
    serverName = "ft_irc_server.com";
    replyCode = 0;
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
    if (listen(fd, SOMAXCONN) < 0)
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
    std::cout << GRE << "New Client connected (fd = " << new_client.get_client_fd() << ")" << RESET << std::endl;
}

void Server::removeClient(int clientFd)
{
    for (std::vector<struct pollfd>::iterator it = poll_fd.begin(); it != poll_fd.end(); )
    {
        if (it->fd == clientFd)
            it = poll_fd.erase(it);
        else
            ++it;
    }

    for (std::vector<Client>::iterator it = myClients.begin(); it != myClients.end(); )
    {
        if (it->get_client_fd() == clientFd)
            it = myClients.erase(it);
        else
            ++it;
    }
}

void Server::receiveNewData(int clientFd)
{
    char buffer[1024];
    
    std::memset(&buffer, 0, sizeof(buffer));
    int readBytes = recv(clientFd, buffer, sizeof(buffer), 0);
    if (readBytes <= 0)
    {
        std::cout << RED << "Client Disconnected (fd = " << clientFd << ")" << RESET << std::endl;
        close(clientFd);
        removeClient(clientFd);
    }
    else
    {
        buffer[readBytes] = '\0';
         for (size_t i = 0; i < myClients.size(); ++i)
        {
            if (myClients[i].get_client_fd() == clientFd)
            {
                std::string &buf = myClients[i].get_client_buffer();
                buf += buffer;
                size_t pos;
                while ((pos = buf.find("\n")) != std::string::npos)
                {
                    std::string cmd = buf.substr(0, pos);
                    buf.erase(0, pos + 1);
                    executeClientCommand(myClients[i], cmd);
                }
                break;
            }
        }
    }
}

void Server::serverLoop()
{
    while (g_is_running)
    {
        int poll_count = poll(this->poll_fd.data(), this->poll_fd.size(), -1);
        if (poll_count < 0)
        {
            if (!g_is_running)
                break;
            throw std::runtime_error("poll() failed");
        }
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

std::vector<std::string> Server::get_arg(std::string cmd)
{
    std::vector<std::string> args;
    size_t i = 0;

    cmd = trim(cmd);

    while (i < cmd.size())
    {
        while (i < cmd.size() && (cmd[i] == ' ' || cmd[i] == '\t'))
            i++;

        if (i >= cmd.size())
            break;
        if (cmd[i] == ':')
        {
            // while (i < cmd.size() && cmd[i] == ':')
            //     i++;
            // while (i < cmd.size() && (cmd[i] == ' ' || cmd[i] == '\t'))
            //     i++;
            if (i < cmd.size())
                args.push_back(cmd.substr(i));
            break;
        }

        size_t j = i;
        while (j < cmd.size() && cmd[j] != ' ' && cmd[j] != '\t' && cmd[j] != ':')
            j++;
        if (i != j)
            args.push_back(cmd.substr(i, j - i));
        i = j;
    }

    return args;
}


void Server::executeClientCommand(Client& client, const std::string& cmd)
{
    std::vector<std::string> arg;
    if (cmd.empty())
        return ;
    arg = get_arg(cmd);
    if (arg.empty())
        return ;
    executeCommand(client, arg);
}

void Server::executeCommand(Client &client, std::vector<std::string> &args)
{
    if (args.empty())
        return;
    args[0] = convertCmdToUpperCase(args[0]);
    if (args[0] == "PASS")
        handle_pass(client, args);
    else if (args[0] == "NICK")
        handle_nick(client, args);
    else if (args[0] == "USER")
        handle_user(client, args);
   else if (args[0] == "JOIN")
        handle_join(client, args);
    else if (args[0] == "PRIVMSG")
        handle_priv_msg(client, args);
    else
        send_to_client(client.get_client_fd(), "Unknown command\n");
}


Server::~Server(){}

//TODO   ## client command ##
//TODO ---->   (ouiam) 1. JOIN & PRIVMSG (9rib ghanhma9 🙂​)
//TODO ---->   (tiima) 2. PASS & NICK & USER