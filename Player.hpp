#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>
#include <vector>

#include "Utils.hpp"
#include "Move.hpp"
#include "BitOps.hpp"
#include "Board.hpp"
#include "MoveGenerator.hpp"

using namespace std;

class Player {
    public:
        Player(string in_name, bool in_is_human, bool white_in, bool on_top) : name(in_name), is_human(in_is_human), is_white(white_in) {
            moveGenerator = new MoveGenerator(white_in, on_top);
        }
        

        //Actually makes the move;
        Move* performMove(Board& board){
            vector<Move*> moves = moveGenerator->generateMoves(board);
            if (is_human){
                if (next_move->to.row == next_move->from.row && next_move->to.col == next_move->from.col){
                    return nullptr;
                }
                
                //Find out if move is in legal moves set
                for (auto& move : moves){
                    // cout << "Move!:" << endl;
                    // cout << move->to_string() << endl;
                    if (move->from == next_move->from && move->to == next_move->to){
                        return next_move;
                    }
                }

                //Not in the set.
                return nullptr;
            }else{
                if (moves.empty()){
                    return nullptr;
                }else{
                    return moves[rand() % moves.size()];
                }
            } 
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




        //Getters

        bool isHuman(){
            return is_human;
        }

    private:

        Move* next_move;
        MoveGenerator* moveGenerator;

        string name;
        bool is_human;
        bool is_white;


        bool is_first_move = true;
};

#endif