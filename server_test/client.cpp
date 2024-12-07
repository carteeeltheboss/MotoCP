#include "client.h"
#include "serialisation.h"

int main(int argc, char* argv[]) {
    if (SDL_Init(0) < 0 || SDLNet_Init() < 0) {
        std::cerr << "Failed to initialize SDL or SDL_net: " << SDLNet_GetError() << std::endl;
        return 1;
    }

    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, "127.0.0.1", SERVER_PORT) < 0) {
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
        kObjct obj;
        int _p;
        std::cout << "Enter ID for the object: ";
        std::cin >> _p;
        obj.kSet(_p);
        serialize(obj, buffer, sizeof(buffer));
        SDLNet_TCP_Send(client, buffer, sizeof(kObjct));
        std::cout << "Object sent to server.";
        int received = SDLNet_TCP_Recv(client, buffer, sizeof(buffer));
        if (received > 0) {
            kObjct received_obj = deserialize(buffer);
            std::cout << "Received object from server:\n";
            received_obj.affiche();
        } else if (received == 0) {
            std::cerr << "Server closed connection.";
            break;
        } else {
            std::cerr << "Error receiving data from server: " << SDLNet_GetError() << std::endl;
            break;
        }
    }

    SDLNet_TCP_Close(client);
    SDLNet_Quit();
    SDL_Quit();
    return 0;
}
