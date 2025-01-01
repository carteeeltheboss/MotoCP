#ifndef GAME_ITEMS_H
#define GAME_ITEMS_H

#include <iostream>
#include <string>
#include <tuple>

#include "sdl_header.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SQUARE_SIZE = 50;
using namespace std;
class objkt1 {
    private:
        int x;
        int y;
        std::tuple<int, int, int> color; // (R, G, B)
        std::string name; // Add name attribute
        public:
    objkt1(int x = 0, int y = 0, std::tuple<int, int, int> color = std::make_tuple(255, 255, 255), std::string name = "Karim") {
        this->x = (x == 0) ? (randumVilue() % (SCREEN_WIDTH - SQUARE_SIZE + 1)) : x;
        cout << "X value is " << this->x << endl;
        this->y = (y == 0) ? (randumVilue() % (SCREEN_HEIGHT - SQUARE_SIZE + 1)) : y;
        cout << "Y value is " << this->y << endl;
        this->color = color;
        this->name = name;
    }

    // Setters
    int randumVilue() {
        cout << "Random value generated " << rand() << endl;
        return rand();
    }
    void set(int coordinate, int value) {
        if (coordinate == 1) {
            if (value >= 0 && value <= SCREEN_WIDTH) {
                x = value;
            }
        } else if (coordinate == 2) {
            if (value >= 0 && value <= SCREEN_HEIGHT) {
                y = value;
            }
        }
    }
    
    void set_color(int r, int g, int b) {
        if(r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255) {
            color = std::make_tuple(r, g, b);
        }
        }
/*
    void _set_color(int r, int g, int b) {
        if (color != std::make_tuple(r, g, b)) {
            if (r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255) {
                color = std::make_tuple(r, g, b);
            }
        }
    }
*/
    void set_name(std::string new_name) {
        name = new_name;
    }

    // Getters
    int get(int coordinate) const {
        if (coordinate == 1) {
            return x;
        } else if (coordinate == 2) {
            return y;
        }
        return 0;
    }

    std::tuple<int, int, int> get_color() const {
        return color;
    }

    std::string get_name() const {
        return name;
    }

    // Individual getters for serialization
    int get_x() const { return x; }
    int get_y() const { return y; }
    int get_r() const { return std::get<0>(color); }
    int get_g() const { return std::get<1>(color); }
    int get_b() const { return std::get<2>(color); }

    // Method to modify position
    void add(int delta, int coordinate) {
        if (coordinate == 1) {
            x += delta;
        } else if (coordinate == 2) {
            y += delta;
        }
    }

    // Equality operator for color comparison
    bool operator==(const objkt1& other) const {
        return color == other.color;
    }
};

struct Client {
    TCPsocket socket;
    int id;
};

#endif // GAME_ITEMS_H