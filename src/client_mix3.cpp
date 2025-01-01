#include "client.h"
#include <ctime> // For time()

std::atomic<bool> running(true);
std::atomic<bool> connected(false);
std::mutex net_mutex;
TCPsocket client = nullptr;
std::vector<objkt1> allPlayers;
std::mutex allPlayers_mutex;
std::mutex log_queue_mutex;
std::queue<std::string> log_queue;
TTF_Font* font = nullptr;

bool isPaused = false;
Menu pauseMenu(nullptr, nullptr); // Placeholder for pause menu
objkt1 localObject;

const char* SERVER_IP = "127.0.0.1";
const int SERVER_PORT = 12345;

void add_log(const std::string& msg) {
    std::lock_guard<std::mutex> lock(log_queue_mutex);
    log_queue.push(msg);
}

void initialize_pause_menu(SDL_Renderer* renderer, TTF_Font* loadedFont) {
    pauseMenu = Menu(renderer, loadedFont);
    pauseMenu.addButton("Resume");
    pauseMenu.addButton("Options");
    pauseMenu.addButton("Quit");
}

void log_thread_func() {
    while (running) {
        SDL_Delay(100);
        std::lock_guard<std::mutex> lock(log_queue_mutex);
        while (!log_queue.empty()) {
            std::cout << log_queue.front() << std::endl;
            log_queue.pop();
        }
    }
}

void client_thread_func() {
    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, SERVER_IP, SERVER_PORT) < 0) {
        add_log("Failed to resolve host: " + std::string(SDLNet_GetError()));
        running = false;
        return;
    }

    TCPsocket client_socket = SDLNet_TCP_Open(&ip);
    if (!client_socket) {
        add_log("Failed to connect: " + std::string(SDLNet_GetError()));
        running = false;
        return;
    }

    {
        std::lock_guard<std::mutex> lock(net_mutex);
        client = client_socket;
    }

    add_log("Connected to server.");
    connected = true;

    std::vector<char> buffer(512);
    while (running) {
        TCPsocket current_client;
        {
            std::lock_guard<std::mutex> lock(net_mutex);
            current_client = client;
        }
        if (!current_client) break;

        int received = SDLNet_TCP_Recv(current_client, buffer.data(), buffer.size());
        if (received > 0) {
            try {
                std::vector<objkt1> newPlayers = deserialize_vector(buffer);
                add_log("Deserialized players count: " + std::to_string(newPlayers.size()));
                for (size_t i = 0; i < newPlayers.size(); i++) {
                    add_log("Player " + std::to_string(i) + " at (" +
                            std::to_string(newPlayers[i].get_x()) + "," +
                            std::to_string(newPlayers[i].get_y()) + ") color(" +
                            std::to_string(newPlayers[i].get_r()) + "," +
                            std::to_string(newPlayers[i].get_g()) + "," +
                            std::to_string(newPlayers[i].get_b()) + ")");
                }
                {
                    std::lock_guard<std::mutex> lock(allPlayers_mutex);
                    allPlayers = std::move(newPlayers);
                }
            }
            catch (const std::exception& e) {
                add_log("Error deserializing data: " + std::string(e.what()));
            }
        }
        else if (received == 0) {
            add_log("Server closed connection.");
            {
                std::lock_guard<std::mutex> lock(net_mutex);
                SDLNet_TCP_Close(client);
                client = nullptr;
            }
            running = false;
            break;
        }
        else {
            add_log("Error receiving data: " + std::string(SDLNet_GetError()));
            running = false;
            break;
        }

        SDL_Delay(1);
    }

    {
        std::lock_guard<std::mutex> lock(net_mutex);
        if (client) {
            SDLNet_TCP_Close(client);
            client = nullptr;
        }
    }
}

void renderingDetail(SDL_Renderer* renderer, TTF_Font* font, const std::vector<objkt1>& objects) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (const auto& obj : objects) {
        auto [r, g, b] = obj.get_color();
        if (r == 255 && g == 255 && b == 255) {
            continue;
        }
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_Rect rect{(int)obj.get_x(), (int)obj.get_y(), 32, 32};
        SDL_RenderFillRect(renderer, &rect);

        SDL_Color textColor = {255, 255, 255, 255};
        SDL_Surface* textSurface = TTF_RenderText_Blended(font, obj.get_name().c_str(), textColor);
        if (!textSurface) {
            std::cerr << "Failed to render text: " << TTF_GetError() << std::endl;
            continue;
        }
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (!textTexture) {
            std::cerr << "Failed to create text texture: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(textSurface);
            continue;
        }
        SDL_Rect textRect = {(int)obj.get_x(), (int)obj.get_y() - textSurface->h,
                             textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }

    SDL_RenderPresent(renderer);
}

void renderPauseMenu(SDL_Renderer* renderer, Menu& pauseMenu) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128); // 50% opacity
    SDL_Rect overlayRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &overlayRect);

    pauseMenu.render();
}

// The game loop
int gameEngine(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Event e;
    Uint32 lastUpdate = SDL_GetTicks();
    MenuOption selectedOption = MenuOption::None;
    bool inOptions = false;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            } 
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    // Toggle pause
                    isPaused = !isPaused;
                }
                if (!isPaused) {
                    switch (e.key.keysym.sym) {
                        case SDLK_UP:    localObject.add(-20, 2); break;
                        case SDLK_DOWN:  localObject.add( 20, 2); break;
                        case SDLK_LEFT:  localObject.add(-20, 1); break;
                        case SDLK_RIGHT: localObject.add( 20, 1); break;
                    }
                }
            }
            else if (isPaused) {
                bool tempRunning = running.load();
                pauseMenu.handleEvent(e, isPaused, tempRunning, selectedOption);
                running.store(tempRunning);

                // If user selected something that unpauses the game
                if (!isPaused) {
                    switch (selectedOption) {
                        case MenuOption::Resume:
                            // resume game
                            break;
                        case MenuOption::Options:
                            //inOptions = true;
                            break;
                        case MenuOption::Quit:
                            running = false;
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        // Render phase
        if (!isPaused) {
            // Update game
            std::vector<objkt1> localState = {localObject};
            std::vector<char> data = serialize(localState);
            {
                std::lock_guard<std::mutex> lock(net_mutex);
                if (client) {
                    int sent = SDLNet_TCP_Send(client, data.data(), data.size());
                    if (sent < (int)data.size()) {
                        add_log("Failed to send all data");
                    } else {
                        add_log("Sent " + std::to_string(sent) + " bytes to server.");
                    }
                }
            }

            std::vector<objkt1> currentPlayers;
            {
                std::lock_guard<std::mutex> lock(allPlayers_mutex);
                currentPlayers = allPlayers;
            }
            currentPlayers.push_back(localObject);
            renderingDetail(renderer, font, currentPlayers);
        } else {
            // Show the pause overlay
            renderPauseMenu(renderer, pauseMenu);
            SDL_RenderPresent(renderer);
        }

        // Limit framerate
        Uint32 current = SDL_GetTicks();
        Uint32 elapsed = current - lastUpdate;
        if (elapsed < 16) {
            SDL_Delay(16 - elapsed);
        }
        lastUpdate = current;
    }
    return 0;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    // Seed the random number generator
    srand(static_cast<unsigned int>(time(nullptr)));

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL init error: " << SDL_GetError() << std::endl;
        return 1;
    }
    if (SDLNet_Init() < 0) {
        std::cerr << "SDLNet init error: " << SDLNet_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    if (TTF_Init() < 0) {
        std::cerr << "TTF init error: " << TTF_GetError() << std::endl;
        SDLNet_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Client",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_WIDTH,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        std::cerr << "Window error: " << SDL_GetError() << std::endl;
        SDLNet_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDLNet_Quit();
        SDL_Quit();
        return 1;
    }
        
    font = TTF_OpenFont("chilispepper.ttf", 24);
    if (!font) {
        std::cerr << "Font error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDLNet_Quit();
        SDL_Quit();
        return 1;
    }

    Menu menu(renderer, font);
    Options options(renderer, font);

    initialize_pause_menu(renderer, font);

    bool inMenu = true;
    bool inOptions = false;
    bool shouldQuit = false;
    bool threadsStarted = false;

    std::thread netThread;
    std::thread logging;

    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    menu.updateButtonPositions(w, h);

    // Enable text input for options typing
    SDL_StartTextInput();

    std::cout << "Entering main loop." << std::endl;

   while (running && !shouldQuit) {
        SDL_Event e;
        MenuOption selectedOption = MenuOption::None;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
                break;
            }
            else if (e.type == SDL_WINDOWEVENT &&
                     e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                w = e.window.data1;
                h = e.window.data2;
                menu.updateButtonPositions(w, h);
            }
            if (inMenu) {
                menu.handleEvent(e, inMenu, shouldQuit, selectedOption);
                if (!inMenu && !shouldQuit) {
                    if (selectedOption == MenuOption::Play) {
                        if (!threadsStarted) {
                            netThread = std::thread(client_thread_func);
                            logging = std::thread(log_thread_func);
                            threadsStarted = true;
                            std::cout << "Network threads started." << std::endl;
                        }
                    }
                    else if (selectedOption == MenuOption::Options) {
                        inOptions = true;
                        std::cout << "Entering Options menu." << std::endl;
                    }
                }
            }
            else if (inOptions) {
                options.handleEvent(e, inOptions, inMenu, localObject);
            }
        }

        // If "Options" was left, see if we returned to menu
        if (inMenu && inOptions == false) {
            // No action needed here specifically
        }

        // If the user clicked "Play" and net threads started
        if (!inMenu && !inOptions && threadsStarted) {
            if (connected) {
                std::cout << "Successfully connected to server. Launching game..." << std::endl;
                gameEngine(renderer, font);
                running = false;
            }
            else if (!running && !connected) {
                std::cerr << "Failed to connect to server. Exiting..." << std::endl;
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (inMenu) {
            menu.render();
        }
        else if (inOptions) {
            options.render();
        }
        else if (!connected && running && threadsStarted) {
            std::cout << "Connecting..." << std::endl;
        }

        SDL_RenderPresent(renderer);
    }

    SDL_StopTextInput();

    running = false;
    if (netThread.joinable()) netThread.join();
    if (logging.joinable()) logging.join();

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDLNet_Quit();
    SDL_Quit();

    std::cout << "Application exited gracefully." << std::endl;
    return 0;
}