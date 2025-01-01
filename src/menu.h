#ifndef MENU_H
#define MENU_H

#include "sdl_header.h"
#include <vector>
#include <string>

// Enum to represent menu options
enum class MenuOption {
    None,
    Play,
    Options,
    Quit,
    Resume
};

struct Button {
    std::string label;
    SDL_Texture* texture;
    SDL_Rect rect;
};

class Menu {
public:
    Menu(SDL_Renderer* renderer, TTF_Font* font);
    ~Menu();

    void handleEvent(SDL_Event& e, bool& inMenu, bool& shouldQuit, MenuOption& selectedOption);
    void updateButtonPositions(int w, int h);
    void render();
    void addButton(const std::string& label);

private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    std::vector<Button> buttons;
};

#endif