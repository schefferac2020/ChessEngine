#ifndef GAME_HPP
#define GAME_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Board.hpp"



class Game{
    public:
        Game() {
            board.init_board();
        }

        void setRenderer(SDL_Renderer* renderer){
            board.setRenderer(renderer);
        }

        void render(){
            board.render();
        }
    private:
        Board board;
};

#endif