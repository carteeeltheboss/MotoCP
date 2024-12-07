#define SERVER_PORT 12345

#include "client.h"

int main(int argc, char* argv[]) {
    if (SDL_Init(0) < 0 || SDLNet_Init() < 0) {
        std::cerr << "Failed to initialize SDL or SDL_net: " << SDLNet_GetError() << std::endl;
        return 1;
    }

    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, "127.0.0.1", 12345) < 0) {
        std::cerr << "Failed to resolve host: " << SDLNet_GetError() << std::endl;
        return 1;
    }

    TCPsocket client = SDLNet_TCP_Open(&ip);
    if (!client) {
        std::cerr << "Failed to connect to server: " << SDLNet_GetError() << std::endl;
        return 1;
    }

    std::cout << "Connected to server." << std::endl;

    char buffer[512];
    while (true) {
        // Send a message to the server
        std::cout << "Enter message: ";
        std::string message;
        std::getline(std::cin, message);
        SDLNet_TCP_Send(client, message.c_str(), message.size());

        // Receive messages from the server
        int received = SDLNet_TCP_Recv(client, buffer, sizeof(buffer) - 1);
        if (received > 0) {
            buffer[received] = '\0'; // Null-terminate the message
            std::cout << "Message from server: " << buffer << std::endl;
        }
    }

    SDLNet_TCP_Close(client);
    SDLNet_Quit();
    SDL_Quit();
    return 0;
}

