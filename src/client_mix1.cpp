#include "client.h"

//#define SDL_MAIN_HANDLED


std::atomic<bool> running(true);
std::mutex log_queue_mutex;
std::queue<std::string> log_queue;
TCPsocket client;
std::mutex net_mutex; // ajouter mutex pour syncronisation

void log_thread() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));  

         
        std::lock_guard<std::mutex> lock(log_queue_mutex);
        while (!log_queue.empty()) {
            std::cout << log_queue.front() << std::endl;
            log_queue.pop();
        }
    }
}


void add_log(const std::string& message) {
    std::lock_guard<std::mutex> lock(log_queue_mutex);
    log_queue.push(message);
}

void server_thread() {
	add_log("\nServer thread started.");
    //std::lock_guard<std::mutex> lock(net_mutex); // Ensure mutual exclusion
    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, "127.0.0.1", SERVER_PORT) < 0) {
        add_log("Failed to resolve host: " + std::string(SDLNet_GetError()));
        running = false;
        return;
    }


    // Lock mutex when accessing shared resource 'client'
    {
        std::lock_guard<std::mutex> lock(net_mutex);
        client = SDLNet_TCP_Open(&ip);
        if (!client) {
            add_log("Failed to connect to server: " + std::string(SDLNet_GetError()));
            running = false;
            return;
        }
    }

    add_log("Connected to server.");

    // Create a socket set
    SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(1);
    if (!socketSet) {
        add_log("Failed to allocate socket set: " + std::string(SDLNet_GetError()));
        running = false;
        return;
    }

    {
        std::lock_guard<std::mutex> lock(net_mutex);
        if (SDLNet_TCP_AddSocket(socketSet, client) == -1) {
            add_log("Failed to add socket to set: " + std::string(SDLNet_GetError()));
            running = false;
            SDLNet_TCP_Close(client);
            client = nullptr;
            SDLNet_FreeSocketSet(socketSet);
            return;
        }
    }

    std::vector<char> buffer(512);
    std::vector<char> previousData;
    bool firstIteration = true;

    while (running) {
        int received;
        {
            //this mutex lock blocks the tcp connection
            //std::lock_guard<std::mutex> lock(net_mutex);
            if (client) {
                received = SDLNet_TCP_Recv(client, buffer.data(), buffer.size());
            }
            else {
                add_log("Client disconnected.");
                running = false;
                break;
            }
        }

        if (received > 0) {
            buffer.resize(received);

            if (firstIteration || buffer != previousData) {
                add_log("Received from server: " + std::string(buffer.begin(), buffer.end()));
                previousData = buffer;
                firstIteration = false;
            }
        }
        else if (received == 0) {
            add_log("Server closed connection.");
            running = false;
        }
        else {
            add_log("Error receiving data from server: " + std::string(SDLNet_GetError()));
            running = false;
        }
    }

    {
        std::lock_guard<std::mutex> lock(net_mutex);
        if (client) {
            const char* disconnect_msg = "DISCONNECT";
            SDLNet_TCP_Send(client, disconnect_msg, strlen(disconnect_msg) + 1);
            SDLNet_TCP_Close(client);
            client = nullptr;
        }
    }
    SDLNet_FreeSocketSet(socketSet);
}


void renderingDetail(SDL_Renderer* renderer, std::vector<objkt1> objectos) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    for (objkt1 obj : objectos) {
        SDL_Rect square = { obj.get(1), obj.get(2), SQUARE_SIZE, SQUARE_SIZE };
        SDL_RenderFillRect(renderer, &square);
    }

    SDL_RenderPresent(renderer);
}

int gameEngine() {

    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Game Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        add_log("Failed to create window: " + std::string(SDL_GetError()));
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        add_log("Failed to create renderer: " + std::string(SDL_GetError()));
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    
    //char _buffer[512];
    SDL_Event event;
    objkt1 karim;
    std::vector<objkt1> objectos;
    objectos.push_back(karim);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP: karim.add(-10, 2); break;
                    case SDLK_DOWN: karim.add(10, 2); break;
                    case SDLK_LEFT: karim.add(-10, 1); break;
                    case SDLK_RIGHT: karim.add(10, 1); break;
                }
                objectos[0] = karim;
            }
        }
        //char* _buffer = new char[sizeof(karim) + 1]; // no need for dynamic allocation as we 
		                                               // are using a fixed size buffer and dont need 
                                                       // to access the data somewhere else 
        
        /*
        *char _buffer[128];
        *memset(_buffer, 0, sizeof(_buffer));
        *serialize(karim, _buffer, sizeof(karim)+1);
        */

        std::vector<char> _buffer = serialize(karim);

        //0xC0000005: Access violation reading location 0x0000000000000020.
                                        // fuck no im not even trying to retreive 0x0000000000000020
        /*
        *0xC0000005 was caused by the mutex locks not being well handled and creating threads confusion
        * thus the client wasnt properly initialised as it had 0x00000 (null) but the game tends to communicate anyway
        * communicating that result in derefrencing the pointer client which in this case was null ! tadah
        */
        int sent_rsl=-1;
        //sent_rsl = SDLNet_TCP_Send(client, _buffer.data(), _buffer.size()); //you are wrong little one

        {
            std::lock_guard<std::mutex> lock(net_mutex);
            if (client) {
                sent_rsl = SDLNet_TCP_Send(client, _buffer.data(), _buffer.size());
            }
            else {
                add_log("Client not connected.");
            }
        }

        if (sent_rsl < 0) {
            add_log("Failed to send data to server: " + std::string(SDLNet_GetError()));
        }
        renderingDetail(renderer, objectos);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}

int main() {
    cout << __FILE__;

    // Initialize SDL and SDL_net in the main thread
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        add_log("Failed to initialize SDL: " + std::string(SDL_GetError()));
        return -1;
    }

    if (SDLNet_Init() < 0) {
        add_log("Failed to initialize SDL_net: " + std::string(SDLNet_GetError()));
        SDL_Quit();
        return -1;
    }

    std::thread server(server_thread);
    std::thread logger(log_thread);

    if (gameEngine() < 0) {
        add_log("Game encountered an error.");
    }

    running = false;

    if (server.joinable()) server.join();
    if (logger.joinable()) logger.join();


    SDLNet_Quit();
    SDL_Quit();
    return 0;
}


/*
* 
void server_thread() {
    add_log("\nServer thread started.");

    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, "127.0.0.1", SERVER_PORT) < 0) {
        add_log("Failed to resolve host: " + std::string(SDLNet_GetError()));
        running = false;
        return;
    }

    // Establish connection without holding the mutex during blocking calls
    {
        std::lock_guard<std::mutex> lock(net_mutex);
        client = SDLNet_TCP_Open(&ip);
        if (!client) {
            add_log("Failed to connect to server: " + std::string(SDLNet_GetError()));
            running = false;
            return;
        }
    }

    add_log("Connected to server.");

    // Create a socket set
    SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(1);
    if (!socketSet) {
        add_log("Failed to allocate socket set: " + std::string(SDLNet_GetError()));
        running = false;
        return;
    }

    {
        std::lock_guard<std::mutex> lock(net_mutex);
        if (SDLNet_TCP_AddSocket(socketSet, client) == -1) {
            add_log("Failed to add socket to set: " + std::string(SDLNet_GetError()));
            running = false;
            SDLNet_TCP_Close(client);
            client = nullptr;
            SDLNet_FreeSocketSet(socketSet);
            return;
        }
    }

    std::vector<char> buffer(512);
    std::vector<char> previousData;
    bool firstIteration = true;

    while (running) {
        // Check if there is data to read
        int activeSockets = SDLNet_CheckSockets(socketSet, 100); // Timeout of 100ms
        if (activeSockets == -1) {
            add_log("SDLNet_CheckSockets error: " + std::string(SDLNet_GetError()));
            running = false;
            break;
        }

        if (activeSockets > 0 && SDLNet_SocketReady(client)) {
            int received = SDLNet_TCP_Recv(client, buffer.data(), buffer.size());
            if (received > 0) {
                buffer.resize(received);

                if (firstIteration || buffer != previousData) {
                    // Process the received data
                    // You may need to deserialize the data here
                    previousData = buffer;
                    firstIteration = false;
                }
            } else if (received == 0) {
                add_log("Server closed connection.");
                running = false;
            } else {
                add_log("Error receiving data from server: " + std::string(SDLNet_GetError()));
                running = false;
            }
        }

        // Sleep or yield here if needed
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // Clean up
    {
        std::lock_guard<std::mutex> lock(net_mutex);
        if (client) {
            const char* disconnect_msg = "DISCONNECT";
            SDLNet_TCP_Send(client, disconnect_msg, strlen(disconnect_msg) + 1);
            SDLNet_TCP_Close(client);
            client = nullptr;
        }
    }

    SDLNet_FreeSocketSet(socketSet);
}

*/

/*
int gameEngine() {
    // ... (initialization code remains the same)

    while (running) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP: karim.add(-10, 2); break;
                    case SDLK_DOWN: karim.add(10, 2); break;
                    case SDLK_LEFT: karim.add(-10, 1); break;
                    case SDLK_RIGHT: karim.add(10, 1); break;
                }
                objectos[0] = karim;
            }
        }

        // Serialize 'karim'
        std::vector<char> _buffer = serialize(karim);

        // Copy client socket pointer safely
        TCPsocket client_copy = nullptr;
        {
            std::lock_guard<std::mutex> lock(net_mutex);
            client_copy = client;
        }

        int sent_rsl = -1;
        // Send data if client is connected
        if (client_copy) {
            sent_rsl = SDLNet_TCP_Send(client_copy, _buffer.data(), _buffer.size());
            if (sent_rsl < _buffer.size()) {
                add_log("Failed to send all data to server: " + std::string(SDLNet_GetError()));
            }
        } else {
            add_log("Client not connected.");
        }

        // Render the scene
        renderingDetail(renderer, objectos);
        SDL_Delay(16);
    }

    // ... (cleanup code remains the same)
    return 0;
}
*/