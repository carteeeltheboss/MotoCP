#include "server.h"

std::vector<Client> clients;

void broadcast_message(const std::string& message, int sender_id) {
    for (const auto& client : clients) {
        if (client.id != sender_id) { // Skip the sender
            SDLNet_TCP_Send(client.socket, message.c_str(), message.size());
        }
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(0) < 0 || SDLNet_Init() < 0) {
        std::cerr << "Failed to initialize SDL or SDL_net: " << SDLNet_GetError() << std::endl;
        return 1;
    }

    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, NULL, 12345) < 0) {
        std::cerr << "Failed to resolve host: " << SDLNet_GetError() << std::endl;
        return 1;
    }

    TCPsocket server = SDLNet_TCP_Open(&ip);
    if (!server) {
        std::cerr << "Failed to open server socket: " << SDLNet_GetError() << std::endl;
        return 1;
    }

    std::cout << "Server started on port 12345..." << std::endl;

    int client_id = 1;
    while (true) {
        TCPsocket new_client = SDLNet_TCP_Accept(server);
        if (new_client) {
            clients.push_back({new_client, client_id});
            std::cout << "New client connected with ID: " << client_id << std::endl;
            client_id++;
        }

        char buffer[512];
        for (auto it = clients.begin(); it != clients.end();) {
            int received = SDLNet_TCP_Recv(it->socket, buffer, sizeof(buffer));
            if (received <= 0) { // Client disconnected
                std::cout << "Client " << it->id << " disconnected." << std::endl;
                SDLNet_TCP_Close(it->socket);
                it = clients.erase(it);
            } else {
                buffer[received] = '\0'; // Null-terminate the message
                std::cout << "Received from client " << it->id << ": " << buffer << std::endl;
                broadcast_message(buffer, it->id); // Send to all other clients
                ++it;
            }
        }

        SDL_Delay(10); // Prevent CPU overuse
    }

    SDLNet_TCP_Close(server);
    SDLNet_Quit();
    SDL_Quit();
    return 0;
}
