#ifndef SERVER_H
#define SERVER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <iostream>
#include <vector>
#include <string>
#include "game_items.h"

#define SERVER_PORT 12345

using namespace std;

struct Client {
    TCPsocket socket;
    int id;
};

void broadcast_message(const std::string& message, int sender_id);

#endif