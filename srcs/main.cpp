# include <iostream>
# include <cstring>
# include <cstdlib>
# include <unistd.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <arpa/inet.h>

# define PORT 9090
# define BUFFER_SIZE 2000

int main()
{
    // -> 1- Create a socket server (IPv4, TCP)
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        std::cerr << "Server Socket Ceation Failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    // -> 2- Allow address reuse
    int yes = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
    {
        close(server_socket);
        std::cerr << "setsockopt() failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    // -> 3 bind the socket to an IP and port
    struct sockaddr_in server_address;
    std::memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
    {
        close(server_socket);
        std::cerr << "Bind failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    // -> 4 start lestening
    if (listen(server_socket, 100) < 0)
    {
        close(server_socket);
        std::cerr << "Listen failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "The Server is listening on PORT " << PORT << "..." << std::endl;

    // -> 5 Accept a client connection
    struct sockaddr_in client_address;
    socklen_t client_address_length = sizeof(client_address);
    int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_length);
    if (client_socket < 0)
    {
        close(server_socket);
        std::cerr << "Accept failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    // ONE NESSAGE
    // -> 6 Read data from client 
    // char buffer[BUFFER_SIZE];
    // int bytes = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
    // if (bytes < 0)
    // {
    //     close(server_socket);
    //     close(client_socket);
    //     std::cerr << "Receive failed" << std::endl;
    //     exit(EXIT_FAILURE);
    // }
    // buffer[bytes] = '\0';

    // std::cout << "Client says: " << buffer << std::endl;

    // // -> 7 Send response

    // const char* response = "Hello froooom the serveeer !!!! Whoooa";
    // if (send(client_socket, response, std::strlen(response), 0) < 0)
    // {
    //     close(server_socket);
    //     close(client_socket);
    //     std::cerr << "Send failed" << std::endl;
    //     exit(EXIT_FAILURE);
    // }

    // HANDLE MULTIPLE MESSAGES
    char buffer[BUFFER_SIZE];
    while (true)
    {
        std::memset(buffer, 0, BUFFER_SIZE);
        int bytes = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes <= 0)
        {
            std::cerr << "Receive failed or Client disconnected" << std::endl;
            break;
        }

        std::cout << "Client says: " << buffer << std::endl;
    }

    close(client_socket);
    close(server_socket);
    std::cout << "Server closed" << std::endl;
    return 0;
}