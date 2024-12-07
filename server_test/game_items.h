#ifndef GAME_ITEMS_H
#define GAME_ITEMS_H

#include <iostream>
#include <string>

using namespace std;

class Moto {
private:
    string name;
    float velocity;
    float x, y;
public:
    Moto(string a = "", float v = 0, float pos_x = 0, float pos_y = 0) : 
        name(a), velocity(v), x(pos_x), y(pos_y) {}

    void affiche();

    string get_name();
};

#endif