#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>

#include "Utils.hpp"
#include "Move.hpp"

using namespace std;

class Player {
    public:
        Player(string in_name, bool in_is_human) : name(in_name), is_human(in_is_human) { }
        
        Move* MakeMove(){
            if (next_move->to.row == next_move->from.row && next_move->to.col == next_move->from.col){
                return nullptr;
            }
            return next_move;
        }

        void setMoveFrom(Position& p){
            if (!is_first_move){
                delete next_move;
            }

            next_move = new Move();
            next_move->from = p;
            is_first_move = false;
        }  

        void setMoveTo(Position& p){
            next_move->to = p;
        }   

        bool isHuman(){
            return is_human;
        }

    private:
        Move* next_move;

        string name;
        bool is_human;

        bool is_first_move = true;
};

#endif