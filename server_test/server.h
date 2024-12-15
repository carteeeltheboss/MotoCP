#ifndef SERVER_H
#define SERVER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

#include "game_items.h"
//#include "client.h"
#include "serialisation.h"

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <mutex>
#include <thread>
#include <atomic>
#include <csignal>      
#include <algorithm>


#define SERVER_PORT 12345

using namespace std;

struct Client {
    TCPsocket socket;
    int id;
};

void broadcast_message(const char* data, int size, int sender_id);
void listener_thread(TCPsocket server);
void signal_handler(int signal);

#endif