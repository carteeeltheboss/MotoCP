#include "server.h"
#include "serialisation.h"
#include <vector>
#include <cstring>

std::vector<Client> clients;

void broadcast_message(const char* data, int size, int sender_id) {
    for (const auto& client : clients) {
        SDLNet_TCP_Send(client.socket, data, size);  
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(0) < 0 || SDLNet_Init() < 0) {
        std::cerr << "Failed to initialize SDL or SDL_net: " << SDLNet_GetError() << std::endl;
        return 1;
    }

    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, NULL, SERVER_PORT) < 0) {
        std::cerr << "Failed to resolve host: " << SDLNet_GetError() << std::endl;
        return 1;
    }

    TCPsocket server = SDLNet_TCP_Open(&ip);
    if (!server) {
        std::cerr << "Failed to open server socket: " << SDLNet_GetError() << std::endl;
        return 1;
    }

    std::cout << "Server started on port " << SERVER_PORT << "..." << std::endl;

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
            if (received <= 0) {  
                std::cout << "Client " << it->id << " disconnected." << std::endl;
                SDLNet_TCP_Close(it->socket);
                it = clients.erase(it);
            } else {
                 
                kObjct obj = deserialize(buffer);
                std::cout << "Received object from client " << it->id << ":";
                obj.affiche();

                 
                broadcast_message(buffer, sizeof(kObjct), it->id);
                ++it;
            }
        }

        SDL_Delay(10);  
    }

    SDLNet_TCP_Close(server);
    SDLNet_Quit();
    SDL_Quit();
    return 0;
}
