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
# include <algorithm>
# include <cctype>
# include <iomanip>
# include <sstream>
# include "Client.hpp"
# include "Channel.hpp"

#define RED "\e[1;31m"
#define WHI "\e[0;37m"
#define GRE "\e[1;32m"
#define BROW "\e[1;33m"
#define MAG "\e[1;35m"
#define CYN "\e[1;36m"
#define RESET "\e[0m"



class Client;
class Channel;

class Server
{
    private :
        int fd;
        int port;
        int replyCode;
        std::string pass;
        std::string serverName;
        std::vector<struct pollfd> poll_fd;//close fds
        std::vector<Client*> myClients;//free
        struct sockaddr_in serverAddr;
        std::vector<Channel*> my_channels;//free
        Client *bot;

    public :
        Server();
        ~Server();
        void initServer(std::string port, std::string pass);
        void createServerSocket();
        void serverLoop();
        void acceptNewConnection();
        void receiveNewData(int clientFd);
        void removeClient(int clientFd);
        void executeClientCommand(Client* client, const std::string& cmd);
        void executeCommand(Client* client, std::vector<std::string> &args);
        void handle_pass(Client* client, std::vector<std::string> &args);
        void handle_nick(Client* client, std::vector<std::string> &args);
        std::string &convertCmdToUpperCase(std::string &str);
        int isNickTaken(std::string nick);
        int isValidNickName(std::string str);
        int check_port(std::string port);
        std::string trim(const std::string& str);
        std::vector<std::string> get_arg(std::string cmd);
        void handle_user(Client* client, std::vector<std::string> &args);
        void handle_priv_msg(Client* client, std::vector<std::string>& args);
        void sending_msg_in_chan(Client* client, std::string message, std::string target);
        void sending_msg_to_user(Client* client, std::string message, std::string target);
        void send_to_client(int clientFd, const std::string& message);
        void handle_join(Client* client, std::vector<std::string>& args);
        void handle_part(Client* client, std::vector<std::string>& args);
        void handle_names(Client* client, std::vector<std::string>& args);
        void send_names_for_channel(Client* client, Channel* chan);
        Channel* findChannel(const std::string& name);
        std::string reply(std::string target, const std::string& message);
        void welcomeClient(Client* client);
        int checkClientAuthorization(Client* client);
        bool checkClientRegistration(Client* client);
        bool isValidUsername(std::string& username);
        void clean_up();
        void handle_mode(Client* client, std::vector<std::string>& args);
        void handle_kick(Client* client, std::vector<std::string>& args);
        Client* getClient(std::string nickname);
        int checkDoubleClientRegistration(Client* client);
        void handle_topic(Client* client, std::vector<std::string>& args);
        void displayTopic(Client* client, std::string channelName);
        void changeTopic(Client* client, std::string channelName, std::string topic);
        void handle_invite(Client* client, std::vector<std::string>& args);
        void setReplyCode(int code);
        void broadcastMessage(Channel* channel, std::string& message);

        int handle_mode_t(Channel* chan, bool add);
        int handle_mode_i(Channel* chan, bool add);
        int handle_mode_k(Client* client, Channel* chan, std::vector<std::string>& args, bool add, int& index);
        int handle_mode_l(Client* client, Channel* chan, std::vector<std::string>& args, bool add, int& index);
        int handle_mode_o(Client* client, Channel* chan, std::vector<std::string>& args, bool add, int& index);
        void apply_channel_mode_flags(Client* client, Channel* chan, std::vector<std::string>& args);
        // in Server.hpp
        void notify_channel_mode_change(Client* client, Channel* chan, const std::string& success_modes, const std::vector<std::string>& success_params);
        Client* find_client_by_nick(std::string nick);
        void init_bot();
        void bot_join_channel(Channel* chan);
        void bot_check_message(Channel* chan, const std::string& msg);
};
