#include "game_items.h"

string Moto::get_name(){ 
    return this->name; 
}

void Moto::affiche(){
        cout << "Nom de la moto: " << this->name
            << "\nSa vitesse instantanée: " << this->velocity
            << "\nIl est a: x= " << this->x << " et a, y= " << this->y << endl;
        return;
    }
