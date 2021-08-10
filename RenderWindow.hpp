#ifndef RENDERWINDOW_HPP
#define RENDERWINDOW_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "Game.hpp"


using namespace std;

class RenderWindow{
    public:
        RenderWindow (const char* title_in, int width, int height) : window(nullptr), renderer(nullptr){
            window = SDL_CreateWindow(title_in, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);

            if (window == nullptr)
                cout << "WINDOW FAILED TO INIT WITH ERROR: " << SDL_GetError() << endl;
            
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            game.setRenderer(renderer);
        }

        SDL_Texture* loadTexture(const char* file_path){
            SDL_Texture* texture = nullptr;
            texture = IMG_LoadTexture(renderer, file_path);
            if (!texture)
                cout << "Failed to load the texture at " << file_path << " with error: " << SDL_GetError() << endl;
            
            return texture;
        }

        // Handles mouse down presses, quit game, etc
        void handleEvents(){ 
            SDL_Event event;
            SDL_PollEvent(&event);
            
            switch (event.type)
            {
            case SDL_KEYDOWN:
                printf("The %s key was pressed!\n", SDL_GetKeyName(event.key.keysym.sym));
                break;
            case SDL_QUIT:
                cout << "Thanks for playing!!!" << endl;
                this->is_running = false;
                break;
            default:
                break;
            }
            
        }

        void render(){
            game.render();
            display();
        }

        void display(){
            SDL_RenderPresent(renderer);
        }

        void update(){
            //game.update;
        }

        void clear(){
            SDL_RenderClear(renderer);
        }

        void cleanUp(){
            SDL_DestroyWindow(window);
        }

        bool running() {
            return this->is_running;
        }

    private:
        bool is_running = true;
        SDL_Window* window;
        SDL_Renderer* renderer;
        Game game;
        
};

#endif