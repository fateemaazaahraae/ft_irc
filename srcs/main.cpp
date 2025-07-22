#include <iostream>
#include <cstring>      // for memset
#include <cstdlib>      // for exit
#include <unistd.h>     // for close()
#include <netinet/in.h> // for sockaddr_in
#include <sys/socket.h> // for socket functions
#include <arpa/inet.h>  // for inet_ntoa

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    // 1. Create a socket (IPv4, TCP)
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    // 2. Allow address reuse (helps avoid "Address already in use" error)
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 3. Bind the socket to an IP and port
    struct sockaddr_in address;
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0 (listen on all interfaces)
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed\n";
        close(server_fd);
        return 1;
    }

    // 4. Start listening
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed\n";
        close(server_fd);
        return 1;
    }

    std::cout << "Server is listening on port " << PORT << "...\n";

    // 5. Accept a client connection
    socklen_t addrlen = sizeof(address);
    int client_fd = accept(server_fd, (struct sockaddr*)&address, &addrlen);
    if (client_fd < 0) {
        std::cerr << "Accept failed\n";
        close(server_fd);
        return 1;
    }

    std::cout << "Client connected from: " << inet_ntoa(address.sin_addr) << "\n";

    // 6. Read data from the client
    char buffer[BUFFER_SIZE];
    int bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        std::cout << "Client says: " << buffer << "\n";
    }

    // 7. Send response
    const char* message = "Hello from server!\n";
    send(client_fd, message, std::strlen(message), 0);

    // 8. Close sockets
    close(client_fd);
    close(server_fd);
    return 0;
}
