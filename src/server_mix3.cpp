#include "server.h"
#include "serialisation.h"  
#include "game_items.h"     

#include <thread>
#include <algorithm>  
#include <random>     

std::vector<objkt1> objectos;
std::vector<Client> clients;
std::mutex objectos_mutex;
std::mutex clients_mutex;
std::atomic<bool> server_running(true);
std::thread broadcastThread;

std::tuple<int, int, int> generate_random_color() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dist(0, 255);
    return {dist(gen), dist(gen), dist(gen)};
}

void initialize_client_object(objkt1& obj, int client_id) {
    auto [r, g, b] = generate_random_color();
    obj.set_color(r, g, b);
     
    int initial_x = 375 + client_id * 100;  
    int initial_y = 275;
    obj.set(1, initial_x);
    obj.set(2, initial_y);
    std::cout << "Initialized client " << client_id << " at ("
              << obj.get_x() << "," << obj.get_y() << ") with color ("
              << r << "," << g << "," << b << ")" << std::endl;
}

void broadcast_states() {
    std::lock_guard<std::mutex> lock_clients(clients_mutex);
    std::lock_guard<std::mutex> lock_objects(objectos_mutex);

    if (objectos.empty() || clients.empty()) {
        return;   
    }

    std::cout << "\nBroadcasting state (" << objectos.size() << " objects):" << std::endl;

     
    for (size_t i = 0; i < objectos.size(); ++i) {
        std::cout << "Object " << i << ": "
                  << "pos(" << objectos[i].get_x() << "," << objectos[i].get_y() << ") "
                  << "color(" << objectos[i].get_r() << ","
                  << objectos[i].get_g() << ","
                  << objectos[i].get_b() << ") "
                  << "name(" << objectos[i].get_name() << ")" << std::endl;
    }

     
    std::vector<char> serialized_data = serialize(objectos);
    for (const auto& client : clients) {
        int sent = SDLNet_TCP_Send(client.socket, serialized_data.data(), serialized_data.size());
        if (sent < static_cast<int>(serialized_data.size())) {
            std::cerr << "Error sending to client " << client.id
                      << ": " << SDLNet_GetError() << std::endl;
        } else {
            std::cout << "Sent " << sent << " bytes to client " << client.id << std::endl;
        }
    }
}

 
void handle_client(Client client) {
    std::vector<char> buffer(512);
    while (server_running) {
        int received = SDLNet_TCP_Recv(client.socket, buffer.data(), buffer.size());
        if (received <= 0) {
            std::cout << "Client " << client.id << " disconnected." << std::endl;
            {
                std::lock_guard<std::mutex> lock(clients_mutex);
                clients.erase(std::remove_if(
                    clients.begin(),
                    clients.end(),
                    [&](const Client& c) { return c.id == client.id; }),
                    clients.end());
            }
            {
                std::lock_guard<std::mutex> lock(objectos_mutex);
                if (client.id < static_cast<int>(objectos.size())) {
                     
                    objectos[client.id] = objkt1();
                }
            }
            broadcast_states();
            SDLNet_TCP_Close(client.socket);
            break;
        } else {
            buffer.resize(received);
             
            std::vector<objkt1> newState = deserialize_vector(buffer);
            if (!newState.empty()) {
                std::lock_guard<std::mutex> lock(objectos_mutex);
                if (client.id < static_cast<int>(objectos.size())) {
                     
                    objectos[client.id].set(1, newState[0].get_x());
                    objectos[client.id].set(2, newState[0].get_y());
                    objectos[client.id].set_name(newState[0].get_name());

                    std::cout << "Updated client " << client.id << " to position ("
                              << objectos[client.id].get_x() << ","
                              << objectos[client.id].get_y() << ") name("
                              << objectos[client.id].get_name() << ")" << std::endl;
                } else {
                    std::cerr << "Client ID " << client.id << " out of range." << std::endl;
                }
            }
            broadcast_states();
        }
        SDL_Delay(1);
    }
}

int main() {
    if (SDL_Init(0) < 0) {
        std::cerr << "SDL init error: " << SDL_GetError() << std::endl;
        return 1;
    }
    if (SDLNet_Init() < 0) {
        std::cerr << "SDLNet init error: " << SDLNet_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, NULL, SERVER_PORT) < 0) {
        std::cerr << "ResolveHost error: " << SDLNet_GetError() << std::endl;
        SDLNet_Quit();
        SDL_Quit();
        return 1;
    }

    TCPsocket server = SDLNet_TCP_Open(&ip);
    if (!server) {
        std::cerr << "Server open error: " << SDLNet_GetError() << std::endl;
        SDLNet_Quit();
        SDL_Quit();
        return 1;
    }

    std::cout << "Server listening on port " << SERVER_PORT << "..." << std::endl;
    int client_id = 0;

    broadcastThread = std::thread([]() {
        while (server_running) {
            SDL_Delay(2000);  
            broadcast_states();
        }
    });

    while (server_running) {
        TCPsocket new_client = SDLNet_TCP_Accept(server);
        if (new_client) {
            Client client{new_client, client_id};
            {
                std::lock_guard<std::mutex> lock(clients_mutex);
                clients.push_back(client);
            }
            {
                std::lock_guard<std::mutex> lock(objectos_mutex);
                if (client_id >= static_cast<int>(objectos.size())) {
                    objectos.resize(client_id + 1);
                }
                objectos[client.id] = objkt1();
                initialize_client_object(objectos[client.id], client_id);
            }

            std::cout << "New client connected with ID: " << client_id << std::endl;
            broadcast_states();  
            std::thread(handle_client, client).detach();
            client_id++;
        }
        SDL_Delay(10);
    }

    server_running = false;
    
    if (broadcastThread.joinable()) {
        broadcastThread.join();
    }

    SDLNet_TCP_Close(server);
    SDLNet_Quit();
    SDL_Quit();
    return 0;
}