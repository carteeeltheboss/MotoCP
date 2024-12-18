#include "server.h"

std::vector<Client> clients;
std::mutex clients_mutex;
std::vector<std::thread> client_threads;

 
void broadcast_message(const char* data, int size, int sender_id) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (const auto& client : clients) {
        if (client.id != sender_id) {
            SDLNet_TCP_Send(client.socket, data, size);
        }
    }
}

 
void handle_client(Client client) {
    char buffer[512];
    objkt1 oldData;  
    bool firstIteration = true;  

    while (true) {
        int received = SDLNet_TCP_Recv(client.socket, buffer, sizeof(buffer));
        if (received <= 0) {
             
            std::cout << "Client " << client.id << " disconnected." << std::endl;
            SDLNet_TCP_Close(client.socket);

             
            std::lock_guard<std::mutex> lock(clients_mutex);
            clients.erase(std::remove_if(clients.begin(), clients.end(), [&client](const Client& c) {
                return c.id == client.id;
            }), clients.end());

            break;
        }

        objkt1 newData = deserialize(buffer);

        if (firstIteration 
        || oldData.get(1) != newData.get(1) 
        || oldData.get(2) != newData.get(2)) {
            std::cout << "Client " << client.id 
                      << " moved. New data x=" << newData.get(1) 
                      << " y=" << newData.get(2) << std::endl;
            oldData = newData;  
            firstIteration = false;  
        }

        broadcast_message(buffer, received, client.id);
    }
}

int main() {
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
            std::lock_guard<std::mutex> lock(clients_mutex);
            Client client = {new_client, client_id};
            clients.push_back(client);

            std::cout << "New client connected with ID: " << client_id++ << std::endl;

             
            client_threads.emplace_back(handle_client, client);
        }

        SDL_Delay(10);  
    }
    /*
    0x0000 0000
    
    offsetting of 0xf


    0x0000000F 0xf


    0xFFFFFFFF
    

    client 1 -> server
    client 2 -> server
    client 3 -> server

    server -> client 1 (client 1, client 2, client 3)
    server -> client 2 (client 1, client 2, client 3)
    server -> client 3 (client 1, client 2, client 3)
    
    */

     
    SDLNet_TCP_Close(server);
    SDLNet_Quit();
    SDL_Quit();

     
    for (std::thread& t : client_threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    return 0;
}
