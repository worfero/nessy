#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "../include/NES.h"

#define WIDTH   256
#define HEIGHT  240

int main(){
    NES nes;
    nes.powerOn();

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(
        "NES Emulator!", 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, 
        WIDTH*3, 
        HEIGHT*3, 
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH,
        HEIGHT
    );

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

        SDL_UpdateTexture(
            texture,
            nullptr,
            nes.getPpuFramebuffer(),
            WIDTH * sizeof(uint32_t)
        );

        SDL_RenderClear(renderer);

        SDL_Rect dst = {
            0,
            0,
            WIDTH*3,
            HEIGHT*3
        };

        SDL_RenderCopy(renderer, texture, nullptr, &dst);

        SDL_RenderPresent(renderer);

        nes.nextFrame();
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}