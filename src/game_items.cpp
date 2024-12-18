#include "game_items.h"

 
int objkt1::get(int x1ory) {
    return x1ory == 1 ? posX : posY;
}

void objkt1::add(int u, int x1ory) {
    if (x1ory == 1) posX += u;
    else posY += u;
}