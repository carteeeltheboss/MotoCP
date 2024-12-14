#ifndef CLIENT_H
#define CLIENT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include "game_items.h"
#include "server.h"

#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <queue>
#include <cstring>


#define SERVER_PORT 12345

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int SQUARE_SIZE = 50;

class objkt1{
	private:
		int posX = SCREEN_WIDTH / 2 - SQUARE_SIZE / 2;
		int posY = SCREEN_HEIGHT / 2 - SQUARE_SIZE / 2;
	public:
		int get(int x1ory);
		void add(int u, int x1ory);
};

 
void server_thread();
void log_thread();
void add_log(const std::string& message);
void renderingDetail(SDL_Renderer* renderer, std::vector<objkt1> objectos);
int gameosss();


using namespace std;
/*int old_main();
int gameosss();
void renderingDetail(SDL_Renderer* a, std::vector<objkt1> b);
*/
void networking_thread(TCPsocket socket);
void send_to_server(TCPsocket socket, const char* data, int size);

extern std::atomic<bool> running;
extern std::queue<std::string> incoming_messages;
extern std::mutex queue_mutex;


#endif 