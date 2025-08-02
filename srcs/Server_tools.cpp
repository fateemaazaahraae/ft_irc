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