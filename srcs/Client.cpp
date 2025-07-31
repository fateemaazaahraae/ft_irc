# include "../includes/Client.hpp"

Client::Client()
{
    fd = -1;
    buffer = "";
    nickname = "";
    username = "";
    is_authorized = false;
}

int Client::get_client_fd() const { return fd; }

std::string &Client::get_client_buffer() { return buffer; }

struct sockaddr_in& Client::get_client_addr() 
{
    return clientAddr;
}

void Client::set_client_fd(int fd)
{
    this->fd = fd;
}

void Client::set_client_buffer(std::string buffer)
{
    this->buffer = buffer;
}

void Client::set_client_addr(struct sockaddr_in addr)
{
    this->clientAddr.sin_addr.s_addr = addr.sin_addr.s_addr;
    this->clientAddr.sin_port = addr.sin_port;
    this->clientAddr.sin_family = addr.sin_family;
}

// void    Client::executeCommand(std::vector<std::string> &args)
// {
//     if (args[0] == "PASS")
//        this->handle_pass(args);
//     else if (args[0] == "NICK")
//         this->handle_nick(args);
//     else if (args[0] == "USER")
//         this->handle_user(args);
//     else if (args[0] == "JOIN")
//         this->handle_join(args);
//     else if (args[0] == "PRIVMSG")
//         this->handle_private_msg(args);
//     else
//         send(this->get_client_fd(), "Unknown command", 15, 0);

// }

void Client::handle_pass(std::vector<std::string> &args)
{}

void Client::handle_nick(std::vector<std::string> &args)
{}

void Client::handle_user(std::vector<std::string> &args)
{}


Client::~Client() {}