# include "../includes/Client.hpp"

Client::Client()
{
    fd = -1;
    buffer = "";
    nickname = "";
    username = "";
    realname = "";
    is_authorized = false;
    is_registered = false;
    // is_operator = false;
}

//======================== getters =======================

int Client::get_client_fd() const { return fd; }

std::string &Client::get_client_buffer() { return buffer; }

struct sockaddr_in& Client::get_client_addr() 
{
    return clientAddr;
}

int Client::get_client_authe() { return is_authorized; }

int Client::get_client_registered() { return is_registered; }

std::string& Client::get_client_nickname() { return nickname; }

std::string& Client::get_client_username() { return username; }

std::string& Client::get_client_realname() { return realname; }
// bool Client::get_client_operator() { return is_operator; }


//======================= setters =======================

void Client::set_client_realname(std::string& realname)
{
    this->realname = realname;
}

void Client::set_client_username(std::string& username)
{
    this->username = username;
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

void Client::set_client_authe()
{
    is_authorized = !is_authorized;
}

void Client::set_client_registered()
{
    is_registered = !is_registered;
}

void Client::set_client_nickname(std::string& nickname)
{
    this->nickname = nickname;
}

// void Client::set_client_operator()
// {
//     is_operator = !is_operator;
// }

Client::~Client() {}