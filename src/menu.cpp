#include "menu.h"
#include <iostream>

Menu::Menu(SDL_Renderer* renderer, TTF_Font* font)
    : renderer(renderer), font(font) {
    // Default buttons (if needed for main menu)
    std::vector<std::string> labels = {"Play", "Options", "Quit"};
    for (const auto& label : labels) {
        addButton(label);
    }
    std::cout << "Menu initialized with " << buttons.size() << " buttons." << std::endl;
}

Menu::~Menu() {
    for (auto& btn : buttons) {
        if (btn.texture) SDL_DestroyTexture(btn.texture);
    }
}

void Menu::handleEvent(SDL_Event& e, bool& inMenu, bool& shouldQuit, MenuOption& selectedOption) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int mx = e.button.x, my = e.button.y;
        for (size_t i = 0; i < buttons.size(); i++) {
            if (mx >= buttons[i].rect.x && mx <= buttons[i].rect.x + buttons[i].rect.w &&
                my >= buttons[i].rect.y && my <= buttons[i].rect.y + buttons[i].rect.h) {
                if (buttons[i].label == "Play") {
                    inMenu = false;
                    selectedOption = MenuOption::Play;
                    std::cout << "Play button clicked." << std::endl;
                }
                else if (buttons[i].label == "Options") {
                    inMenu = false;
                    selectedOption = MenuOption::Options;
                    std::cout << "Options button clicked." << std::endl;
                }
                else if (buttons[i].label == "Quit") {
                    shouldQuit = true;
                    selectedOption = MenuOption::Quit;
                    std::cout << "Quit button clicked." << std::endl;
                }
                else if (buttons[i].label == "Resume") {
                    // Used in pause menu
                    inMenu = false;
                    selectedOption = MenuOption::Resume;
                    std::cout << "Resume button clicked." << std::endl;
                }
            }
        }
    }
}

void Menu::updateButtonPositions(int w, int h) {
    for (size_t i = 0; i < buttons.size(); i++) {
        buttons[i].rect.x = (w - buttons[i].rect.w) / 2;
        // center them vertically around the middle of the screen
        buttons[i].rect.y = (h / 2 - buttons[i].rect.h * 2) + i * (buttons[i].rect.h + 20);
    }
}

void Menu::render() {
    for (const auto& btn : buttons) {
        if (btn.texture) {
            SDL_RenderCopy(renderer, btn.texture, nullptr, &btn.rect);
        }
    }
}

void Menu::addButton(const std::string& label) {
    SDL_Color color = {255, 255, 255, 255};
    Button btn;
    btn.label = label;

    SDL_Surface* surf = TTF_RenderText_Blended(font, label.c_str(), color);
    if (!surf) {
        std::cerr << "Failed to render text surface: " << TTF_GetError() << std::endl;
        return;
    }

    btn.texture = SDL_CreateTextureFromSurface(renderer, surf);
    if (!btn.texture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surf);
        return;
    }

    btn.rect.w = surf->w;
    btn.rect.h = surf->h;
    SDL_FreeSurface(surf);
    buttons.push_back(btn);
}