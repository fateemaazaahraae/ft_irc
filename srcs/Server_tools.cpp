# include "../includes/Server.hpp"

int   Server::check_port(std::string port)
{
    if (port.find_first_not_of("0123456789") != std::string::npos)
        throw std::runtime_error("Port contain non numeric characters");
    int res = atoi(port.c_str());
    if (res < 1024 || res > 65535)
        throw std::runtime_error("Invalid port range");
    return res;
}

std::string Server::trim(const std::string& str)
{
    size_t start = 0;
    while (start < str.size() && std::isspace(static_cast<unsigned char>(str[start])))
        start++;

    size_t end = str.size();
    while (end > start && std::isspace(static_cast<unsigned char>(str[end - 1])))
        end--;

    return str.substr(start, end - start);
}

std::string &Server::convertCmdToUpperCase(std::string &str)
{
    for (size_t i = 0; i < str.size(); i++)
        str[i] = toupper(str[i]);
    return str;
}

std::string Server::reply(std::string target, const std::string& message)
{
    std::ostringstream reply;
    if (target.empty())
        target = "*";
    reply << ":" << serverName << " " << std::setfill('0') << std::setw(3) << replyCode << " " << target << " :" << message << "\n";
    return reply.str();
}

void Server::welcomeClient(Client* client)
{
    replyCode = 001;
    std::string welcomeMessage = "Welcome to the Internet Relay Network " + client->get_client_nickname() + "!" +
                                    client->get_client_username() + "@" + client->get_client_realname();
    std::string rep = reply(client->get_client_nickname(), welcomeMessage);
    send_to_client(client->get_client_fd(), rep);

    replyCode = 002;
    std::string serverInfo = "Your host is " + serverName + ", running version ft_irc";
    rep = reply(client->get_client_nickname(), serverInfo);
    send_to_client(client->get_client_fd(), rep);

    replyCode = 003;
    std::string createdInfo = "This server was created on " + std::string(__DATE__) + " at " + std::string(__TIME__);
    rep = reply(client->get_client_nickname(), createdInfo);
    send_to_client(client->get_client_fd(), rep);

    replyCode = 004;
    std::string serverVersion = serverName + " ft_irc 1.0";
    rep = reply(client->get_client_nickname(), serverVersion);
    send_to_client(client->get_client_fd(), rep);
    std::cout << "Client " << client->get_client_nickname() << " is now registered." << std::endl;
    client->set_client_registered();
}

int Server::checkClientAuthorization(Client* client)
{
    if (!client->get_client_authe())
    {
        replyCode = 464;
        std::string rep = reply(client->get_client_nickname(), "You are not authorized");
        send_to_client(client->get_client_fd(), rep);
        return 0;
    }
    return 1;
}

int Server::checkDoubleClientRegistration(Client* client)
{
    if (client->get_client_registered())
    {
        replyCode = 462;
        std::string rep = reply(client->get_client_nickname(), "You are already registered");
        send_to_client(client->get_client_fd(), rep);
        return 0;
    }
    return 1;
}

bool Server::checkClientRegistration(Client* client)
{
    if (!client->get_client_registered())
    {
        replyCode = 451;
        std::string rep = reply(client->get_client_nickname(), "You are not registered");
        send_to_client(client->get_client_fd(), rep);
        return false;
    }
    return true;
}

Client* Server::find_client_by_nick(std::string nick)
{
    size_t i = 0;
    while (i < myClients.size())
    {
        if (myClients[i]->get_client_nickname() == nick)
            return myClients[i];
        i++;
    }
    return NULL;
}