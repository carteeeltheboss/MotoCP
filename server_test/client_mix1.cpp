#include "client.h"
#include "serialisation.h"
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <queue>
#include <cstring>

 
std::atomic<bool> running(true);
std::mutex log_queue_mutex;
std::queue<std::string> log_queue;


 
int objkt1::get(int x1ory) {
    return x1ory == 1 ? posX : posY;
}

void objkt1::add(int u, int x1ory) {
    if (x1ory == 1) posX += u;
    else posY += u;
}

 
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
    if (SDL_Init(0) < 0 || SDLNet_Init() < 0) {
        add_log("Failed to initialize SDL or SDL_net: " + std::string(SDLNet_GetError()));
        running = false;
        return;
    }

    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, "127.0.0.1", SERVER_PORT) < 0) {
        add_log("Failed to resolve host: " + std::string(SDLNet_GetError()));
        running = false;
        return;
    }

    TCPsocket client = SDLNet_TCP_Open(&ip);
    if (!client) {
        add_log("Failed to connect to server: " + std::string(SDLNet_GetError()));
        running = false;
        return;
    }

    add_log("Connected to server.");

    char buffer[512];
    while (running) {
        int received = SDLNet_TCP_Recv(client, buffer, sizeof(buffer));
        if (received > 0) {
            add_log("Received from server: " + std::string(buffer, received));
        } else if (received == 0) {
            add_log("Server closed connection.");
            running = false;
        } else {
            add_log("Error receiving data from server: " + std::string(SDLNet_GetError()));
            running = false;
        }
    }

     
    if (!running) {
        const char* disconnect_msg = "DISCONNECT";
        SDLNet_TCP_Send(client, disconnect_msg, strlen(disconnect_msg) + 1);
    }

    SDLNet_TCP_Close(client);
    SDLNet_Quit();
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

int gameosss() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        add_log("Failed to initialize SDL: " + std::string(SDL_GetError()));
        return -1;
    }

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

        renderingDetail(renderer, objectos);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

int main() {
    std::thread server(server_thread);
    std::thread logger(log_thread);

    if (gameosss() < 0) {
        add_log("Game encountered an error.");
    }

    running = false;
    server.join();
    logger.join();

    return 0;
}
