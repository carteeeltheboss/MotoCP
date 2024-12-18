#ifndef GAME_ITEMS_H
#define GAME_ITEMS_H

#include <iostream>
#include <string>


#define SERVER_PORT 12345

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int SQUARE_SIZE = 50;

using namespace std;


class objkt1{
	private:
		int posX;
		int posY;
		//int client_id_unique;
	public:
		int get(int x1ory);
		void add(int u, int x1ory);
        objkt1(int x = SCREEN_WIDTH / 2 - SQUARE_SIZE / 2, int y = SCREEN_HEIGHT / 2 - SQUARE_SIZE / 2) : posX(x), posY(y) {}
};


#endif