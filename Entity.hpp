#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "Math.hpp"

class Entity {

    public:
        Entity(Vector2f pos_in, SDL_Texture *in_tex) : pos(pos_in), tex(in_tex) { 
            current_frame.x = 0;
            current_frame.y = 0;
            current_frame.w = 32;
            current_frame.h = 32;
        }

        //Getters
        Vector2f& getPos(){
            return pos;
        }

        SDL_Texture* getTexture(){
            return tex;
        } 

        SDL_Rect getCurrFrame(){
            return current_frame;
        }

    private:
        Vector2f pos;
        SDL_Rect current_frame;
        SDL_Texture* tex;   
};