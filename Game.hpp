#ifndef GAME_HPP
#define GAME_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Board.hpp"
#include "Player.hpp"


class Game{
    public:
        Game() {
            player1 = new Player("Player 1", true, true, false); // name, is_human, is_white, on_top
            player2 = new Player("Player 2 (AI)", false, false, true);

            this->current_player = player1;
            board.setFenPosition("4k3/6N1/5b2/3R4/8/8/8/4K3 b - - 0 3"); 
            board.init_board();
        }

        ~Game() {
            delete player1;
            delete player2;
        }

        void setRenderer(SDL_Renderer* renderer){
            board.setRenderer(renderer);
        }

        void render(){
            board.render();
        }

        void start_dragging(SDL_Point mouse_pos){
            if (current_player->isHuman()){
                //Get Row and col of "drag from" position
                Position move_from_pos = board.getTilePos(mouse_pos);
                current_player->setMoveFrom(move_from_pos);
            }
        }

        void update_mouse_pos(SDL_Point mouse_pos){
            board.updateMouse(mouse_pos);
        }

        void end_dragging(SDL_Point mouse_pos){

            if (current_player->isHuman()){
                //Get Row and col of "drag from" position
                Position move_to_pos = board.getTilePos(mouse_pos);
                current_player->setMoveTo(move_to_pos);

                //Make the human move;
                Move* human_move = current_player->performMove(board);
                if (human_move){ // If the move is valid;
                    board.makeMove(human_move);
                    switch_player();
                }
                board.stopDragging();

            }
        }

        void update(){
            if (!current_player->isHuman()){ //If it is an AI
                //Make the AI move;
                Move* ai_move = current_player->performMove(board);
                if (ai_move){ // If the move is valid;
                    board.makeMove(ai_move);
                    switch_player();
                }else{
                    cout << "There are no legal moves!!" << endl;
                }
            }
        }
    private:

        void switch_player(){            
            current_player = (current_player == player1) ? player2 : player1;
            cout << "It " << current_player->getName() << "\'s turn\n";
        }

        Board board;
        Player* player1;
        Player* player2;
        Player* current_player;
};

#endif