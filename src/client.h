#ifndef CLIENT_H
#define CLIENT_H

#include "sdl_header.h"

#include "game_items.h"
#include "serialisation.h"

#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <queue>
#include <cstring>

using namespace std;

#define SERVER_PORT 12345

 
void server_thread();
void log_thread();
void add_log(const std::string& message);
void renderingDetail(SDL_Renderer* renderer, std::vector<objkt1> objectos);
int gameEngine();

void networking_thread(TCPsocket socket);
void send_to_server(TCPsocket socket, const char* data, int size);

extern std::atomic<bool> running;
extern std::queue<std::string> incoming_messages;
extern std::mutex queue_mutex;


#endif 