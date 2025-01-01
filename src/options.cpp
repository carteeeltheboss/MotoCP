#include "options.h"
#include <iostream>

Options::Options(SDL_Renderer* renderer, TTF_Font* font)
    : renderer(renderer), font(font), currentResolution(0),
      nameTexture(nullptr), resTexture(nullptr), backBtnTex(nullptr), applyBtnTex(nullptr)
{
    resolutions = {"1280x720", "1600x900", "1920x1080"};
    playerName = "Player";
    
    // Prepare "Back" button
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface* btnSurf = TTF_RenderText_Blended(font, "Back", color);
    if (btnSurf) {
        backBtnTex = SDL_CreateTextureFromSurface(renderer, btnSurf);
        backBtnRect.w = btnSurf->w;
        backBtnRect.h = btnSurf->h;
        SDL_FreeSurface(btnSurf);
    }
    backBtnRect.x = 50;  // Position for the "Back" button
    backBtnRect.y = 50;

    SDL_Surface* applyBtnSurf = TTF_RenderText_Blended(font, "Apply", color);
    if (applyBtnSurf) {
        applyBtnTex = SDL_CreateTextureFromSurface(renderer, applyBtnSurf);
        applyBtnRect.w = applyBtnSurf->w;
        applyBtnRect.h = applyBtnSurf->h;
        applyBtnRect.x = 200;  // Position for the "Apply" button
        applyBtnRect.y = 400;  // Below resolution setting
        SDL_FreeSurface(applyBtnSurf);
    }

    updateNameTexture();
    updateResTexture();

    std::cout << "Options screen initialized.\n";
}

Options::~Options() {
    if(nameTexture) SDL_DestroyTexture(nameTexture);
    if(resTexture) SDL_DestroyTexture(resTexture);
    if(backBtnTex) SDL_DestroyTexture(backBtnTex);
    if(applyBtnTex) SDL_DestroyTexture(applyBtnTex);
}

void Options::handleEvent(SDL_Event& e, bool& inOptions, bool& inMenu, objkt1& localObject) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int mx = e.button.x, my = e.button.y;
        // Check if "Back" clicked
        if (mx >= backBtnRect.x && mx <= backBtnRect.x + backBtnRect.w &&
            my >= backBtnRect.y && my <= backBtnRect.y + backBtnRect.h) {
            inOptions = false;
            inMenu = true;
            std::cout << "Back to menu clicked.\n";
        }

        if (mx >= applyBtnRect.x && mx <= applyBtnRect.x + applyBtnRect.w &&
            my >= applyBtnRect.y && my <= applyBtnRect.y + applyBtnRect.h) {
            // Apply the settings
            std::cout << "Applying settings...\n";
            std::cout << "Player name: " << playerName << "\n";
            localObject.set_name(playerName);
            std::cout << "Resolution: " << resolutions[currentResolution] << "\n";
        }
    }
    else if (e.type == SDL_KEYDOWN) {
        // Simple resolution change with left/right
        if (e.key.keysym.sym == SDLK_LEFT) {
            currentResolution--;
            if (currentResolution < 0) currentResolution = (int)resolutions.size() - 1;
            updateResTexture();
        }
        else if (e.key.keysym.sym == SDLK_RIGHT) {
            currentResolution++;
            if (currentResolution >= (int)resolutions.size()) currentResolution = 0;
            updateResTexture();
        }
    }
    else if (e.type == SDL_TEXTINPUT) {
        // Append typed characters to playerName
        playerName += e.text.text;
        updateNameTexture();
    }
    else if (e.type == SDL_KEYUP) {
        // Handle backspace
        if (e.key.keysym.sym == SDLK_BACKSPACE && !playerName.empty()) {
            playerName.pop_back();
            updateNameTexture();
        }
    }
}

void Options::render() {
    // Clear screen first
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);

    if(backBtnTex) {
        SDL_RenderCopy(renderer, backBtnTex, NULL, &backBtnRect);
    }

    if (nameTexture) {
        SDL_RenderCopy(renderer, nameTexture, NULL, &nameRect);
    }
    
    if (resTexture) {
        SDL_RenderCopy(renderer, resTexture, NULL, &resRect);
    }

    if(applyBtnTex) {
        SDL_RenderCopy(renderer, applyBtnTex, NULL, &applyBtnRect);
    }
}

void Options::updateNameTexture() {
    if(nameTexture) SDL_DestroyTexture(nameTexture);
    SDL_Color color = {255, 255, 255, 255};
    std::string label = "Player Name: " + playerName;
    SDL_Surface* surf = TTF_RenderText_Blended(font, label.c_str(), color);
    if (!surf) return;
    nameTexture = SDL_CreateTextureFromSurface(renderer, surf);
    nameRect.w = surf->w;
    nameRect.h = surf->h;
    nameRect.x = 200;
    nameRect.y = 200;
    SDL_FreeSurface(surf);
}

void Options::updateResTexture() {
    if(resTexture) SDL_DestroyTexture(resTexture);
    SDL_Color color = {255, 255, 255, 255};
    std::string label = "Resolution: " + resolutions[currentResolution];
    SDL_Surface* surf = TTF_RenderText_Blended(font, label.c_str(), color);
    if (!surf) return;
    resTexture = SDL_CreateTextureFromSurface(renderer, surf);
    resRect.w = surf->w;
    resRect.h = surf->h;
    resRect.x = 200;
    resRect.y = 300;
    SDL_FreeSurface(surf);
}