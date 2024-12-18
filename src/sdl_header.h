#ifdef _WIN32 
/*
* pre processing macro to check if target machine
* is windows if yes then reorient it to <SDL.h> and <SDL_net.h>
*/

// Windows code
#include <SDL.h>
#include <SDL_net.h>
#include <SDL_main.h>
#else
// macOS - Linux code 
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_main.h>
#endif


