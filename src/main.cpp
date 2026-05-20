#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "../include/NES.h"

#define WIDTH   256
#define HEIGHT  240

int main(){
    NES nes;
    nes.powerOn();

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Framebuffer Rendering!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    SDL_Surface *surface = SDL_GetWindowSurface(window);
    uint32_t* pixels;
    pixels = (uint32_t*)surface->pixels;

    bool quit = false;
    SDL_Event e;

    while(!quit){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT) quit = true;
        }
        
        do
        {
            nes.clock();
        }
        while(!nes.isFrameComplete());

        SDL_LockSurface(surface);

        memset(pixels, 0, sizeof(uint32_t) * WIDTH * HEIGHT);

        memcpy(
            pixels,
            nes.getPpuFramebuffer(),
            256 * 240 * sizeof(uint32_t)
        );

        SDL_UpdateWindowSurface(window);

        SDL_UnlockSurface(surface);

        nes.nextFrame();
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}