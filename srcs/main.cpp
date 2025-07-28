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

# define PORT 9090
# define MAX_CLIENT 2000
# define BUFFER_SIZE 2000

int main()
{
    int serverSocket = -1, clientSocket = -1;
    std::vector<struct pollfd> fds;
    struct pollfd pollfd;
    int numFd = 0;
    char buffer[BUFFER_SIZE];
    try
    {
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket < 0)
            throw std::runtime_error("Server Socket Creation Failed");

        int opt = 1;
        if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
            throw std::runtime_error("Setsockopt() failed");
        
        struct sockaddr_in serverAddr;
        std::memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(PORT);

        std::cout << "SERVER START LISTENING ON IP " << inet_ntoa(serverAddr.sin_addr) << " PORT " << PORT << std::endl;
        if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
            throw std::runtime_error("Bind() failed");
        
        if (listen(serverSocket, MAX_CLIENT) < 0)
            throw std::runtime_error("Listen() failed");
        std::cout << "SERVER START LISTENING ON PORT " << ntohs(serverAddr.sin_port) << std::endl;

        pollfd.fd = serverSocket;
        pollfd.events = POLLIN;
        pollfd.revents = 0;
        fds.push_back(pollfd);
        numFd = 1;
        while (true)
        {
            int poll_count = poll(fds.data(), numFd, -1);
            if (poll_count < 0)
                throw std::runtime_error("Poll() failed");
            if (numFd >= MAX_CLIENT)
                throw std::runtime_error("MAX_CLIENT limit reached");
            
            for (int i = 0; i < numFd; i++)
            {
                if (fds[i].revents & POLLIN)
                {
                    if (fds[i].fd == serverSocket)
                    {
                        struct sockaddr_in clientAddr;
                        socklen_t clientLen = sizeof(clientAddr);
                        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
                        if (clientSocket < 0)
                            continue;
                        std::cout << "New client connected: " << inet_ntoa(clientAddr.sin_addr) << " (fd = " << clientSocket << ")" << std::endl;

                        pollfd.fd = clientSocket;
                        pollfd.events = POLLIN;
                        pollfd.revents = 0;
                        fds.push_back(pollfd);
                        numFd++;
                    }
                    else
                    {
                        std::memset(buffer, 0, BUFFER_SIZE);
                        int bytes = recv(fds[i].fd, &buffer, BUFFER_SIZE - 1, 0);
                        if (bytes <= 0)
                        {
                            std::cout << "Client disconnected (fd = " << fds[i].fd << ")" << std::endl;
                            close(fds[i].fd);
                            fds[i] = fds[numFd - 1];
                            fds.pop_back();
                            numFd--;
                            i--;
                        }
                        else
                        {
                            buffer[bytes] = '\0';
                            std::cout << "Received from client (fd = " << fds[i].fd << "): " << buffer << std::endl;
                        }
                    }
                }
            }
        }
    }
    catch(std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}
