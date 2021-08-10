#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h> //Have to add linker to makefile I think still
#include <SDL2/SDL_image.h>
#include <vector>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Utils.hpp"

using namespace std;

int main(int argc, char** argv) {
    if (SDL_Init(SDL_INIT_VIDEO) > 0)
        cout << "SDL Init failed. SLD ERROR: " << SDL_GetError() << endl;

    if (!(IMG_Init(IMG_INIT_PNG)))
        cout << "SDL Init failed. SLD ERROR: " << SDL_GetError() << endl;


    const int frameDelay = 1000 / 100; // 60 fps

    int frameStart = SDL_GetTicks();
    RenderWindow window("Chess", 1280, 720);

    while (window.running()){
        int frameStart = SDL_GetTicks();
        window.handleEvents();
        window.update();
        window.render();

        int frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime){
            SDL_Delay(frameDelay - frameTime);
        }
    }

    window.cleanUp();
    // delete window;


    SDL_Quit();

    return 0;
}