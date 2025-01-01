#ifndef OPTIONS_H
#define OPTIONS_H

#include "sdl_header.h"
#include "game_items.h"
#include <string>
#include <vector>
#include <functional>

class Options {
public:
    Options(SDL_Renderer* renderer, TTF_Font* font);
    ~Options();

    void handleEvent(SDL_Event& e, bool& inOptions, bool& inMenu, objkt1& localObject);
    void render();

    std::string playerName;

private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    std::vector<std::string> resolutions;
    int currentResolution;

    SDL_Texture* nameTexture;
    SDL_Rect nameRect;
    SDL_Texture* resTexture;
    SDL_Rect resRect;
    SDL_Texture* backBtnTex;
    SDL_Rect backBtnRect;
    
    void updateNameTexture();
    void updateResTexture();
    SDL_Texture* applyBtnTex;
    SDL_Rect applyBtnRect;
    
    std::function<void(const std::string&, const std::string&)> onApplySettings;

};

#endif