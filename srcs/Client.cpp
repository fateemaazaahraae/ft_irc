# include "../includes/Client.hpp"

Client::Client()
{
    fd = -1;
    buffer = "";
    nickname = "";
    username = "";
}

int Client::get_client_fd() const { return fd; }

std::string Client::get_client_buffer() const { return buffer; }

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


Client::~Client() {}