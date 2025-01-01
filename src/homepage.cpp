#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

struct Button {
    SDL_Rect rect;
    SDL_Texture* texture;
};

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_Window* window = SDL_CreateWindow("SDL Menu",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600, SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    TTF_Font* font = TTF_OpenFont("/Volumes/Untitled 1/MotoCPP/FreebrushScriptPLng.ttf", 48);
    SDL_Color color = {255, 255, 255, 255};

    // Create buttons
    std::string labels[3] = {"Play", "Options", "Quit"};
    Button buttons[3];
    for(int i=0; i<3; i++){
        SDL_Surface* surf = TTF_RenderText_Blended(font, labels[i].c_str(), color);
        buttons[i].texture = SDL_CreateTextureFromSurface(renderer, surf);
        buttons[i].rect.w = surf->w;
        buttons[i].rect.h = surf->h;
        SDL_FreeSurface(surf);
    }

    bool running = true;
    while(running){
        SDL_Event e;
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT) running = false;
            if(e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                int w = e.window.data1, h = e.window.data2;
                for(int i=0; i<3; i++){
                    buttons[i].rect.x = (w - buttons[i].rect.w)/2;
                    buttons[i].rect.y = (h/2 - buttons[i].rect.h*2) + i* (buttons[i].rect.h + 20);
                }
            }
            if(e.type == SDL_MOUSEBUTTONDOWN){
                int mx = e.button.x, my = e.button.y;
                for(int i=0; i<3; i++){
                    if(mx >= buttons[i].rect.x && mx <= buttons[i].rect.x + buttons[i].rect.w &&
                    my >= buttons[i].rect.y && my <= buttons[i].rect.y + buttons[i].rect.h){
                        if(i == 2) running = false;
                    }
                }
            }
        }

        // Update button positions if window resized
        int ww, wh;
        SDL_GetWindowSize(window, &ww, &wh);
        for(int i=0; i<3; i++){
            buttons[i].rect.x = (ww - buttons[i].rect.w)/2;
            buttons[i].rect.y = (wh/2 - buttons[i].rect.h*2) + i*(buttons[i].rect.h + 20);
        }

        SDL_RenderClear(renderer);
        for(int i=0; i<3; i++){
            SDL_RenderCopy(renderer, buttons[i].texture, NULL, &buttons[i].rect);
        }
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    for(int i=0; i<3; i++){
        SDL_DestroyTexture(buttons[i].texture);
    }
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}