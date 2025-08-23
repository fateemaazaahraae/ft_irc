# include "../includes/Client.hpp"

Client::Client()
{
    fd = -1;
    buffer = "";
    nickname = "";
    username = "";
    realname = "";
    hostname = "127.0.0.1";
    is_authorized = false;
    is_registered = false;
}



Client::Client(const Client& other)
{
    fd = other.fd;
    buffer = other.buffer;
    nickname = other.nickname;
    username = other.username;
    realname = other.realname;
    hostname = other.hostname;
    clientAddr = other.clientAddr;
    is_authorized = other.is_authorized;
    is_registered = other.is_registered;
}

//======================== getters =======================

int Client::get_client_fd() const { return fd; }

std::string &Client::get_client_buffer() { return buffer; }

struct sockaddr_in& Client::get_client_addr() { return clientAddr; }

int Client::get_client_authe() { return is_authorized; }

int Client::get_client_registered() { return is_registered; }

std::string& Client::get_client_nickname() { return nickname; }

std::string& Client::get_client_username() { return username; }

std::string& Client::get_client_realname() { return realname; }

std::string Client::get_client_hostname() const { return hostname; }

std::string Client::get_prefix() const
{
    return (nickname + "!" + username + "@" + hostname);
}

//======================= setters =======================

void Client::set_client_realname(const std::string& realname)
{
    this->realname = realname;
}

void Client::set_client_username(const std::string& username)
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

void Client::set_client_nickname(const std::string& nickname)
{
    this->nickname = nickname;
}

void Client::set_invitedChannels(std::string& channelName)
{
    this->invitedChannels.push_back(channelName);
}

bool Client::get_invitedChannels(Channel *channel) const
{
    return std::find(invitedChannels.begin(), invitedChannels.end(), channel->get_name()) != invitedChannels.end();
}

void Client::removeInvitedChannel(std::string &channel)
{
    for (size_t i = 0; i < invitedChannels.size(); i++)
    {
        if (invitedChannels[i] == channel)
        {
            invitedChannels.erase(invitedChannels.begin() + i);
            break;
        }
    }
}

Client::~Client() {}
