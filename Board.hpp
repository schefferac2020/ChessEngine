#ifndef BOARD_HPP
#define BOARD_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdint>
#include <vector>
#include <bitset>

#include "Math.hpp"
#include "Move.hpp"
#include "Utils.hpp"

#define TILE_WIDTH 75
#define PIECE_WIDTH 50

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

enum class BitboardPieceType : uint8_t {
        White = 0, Black, Pawn, Knight, Bishop, Queen, King, Rook
};



class Board{
    public:
        void render(){
            drawBoard();
            drawPieces();
        }

        //Sets the renderer
        void setRenderer(SDL_Renderer* in_rend){
            this->renderer = in_rend;
        }

        //Initializes the bitboards. Could initialize from fen later
        void init_board(){
            bitboards.resize(8);
            this->squares = {
                                {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
                                {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
                                {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                                {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                                {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                                {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                                {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
                                {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}

                            };
            
            bitset<64> first_bit_string; first_bit_string.set(63, true); // Something like 10000000000... for the top left corner
            for (int row = 0; row < 8; ++row){
                for (int col = 0; col < 8; ++col){
                    char current_piece = this->squares[row][col];
                    
                    bitset<64> curr_piece_bit_string = first_bit_string >> (row*8+col);
                    switch (tolower(current_piece))
                    {
                    case 'p':
                        bitboards[(int)BitboardPieceType::Pawn] = bitboards[(int)BitboardPieceType::Pawn] | curr_piece_bit_string;
                        break;

                    case 'r':
                        bitboards[(int)BitboardPieceType::Rook] = bitboards[(int)BitboardPieceType::Rook] | curr_piece_bit_string;
                        break;
                    
                    case 'n':
                        bitboards[(int)BitboardPieceType::Knight] = bitboards[(int)BitboardPieceType::Knight] | curr_piece_bit_string;
                        break;
                    
                    case 'b':
                        bitboards[(int)BitboardPieceType::Bishop] = bitboards[(int)BitboardPieceType::Bishop] | curr_piece_bit_string;
                        break;
                    
                    case 'q':
                        bitboards[(int)BitboardPieceType::Queen] = bitboards[(int)BitboardPieceType::Queen] | curr_piece_bit_string;
                        break;
    
                    case 'k':
                        bitboards[(int)BitboardPieceType::King] = bitboards[(int)BitboardPieceType::King] | curr_piece_bit_string;
                        break;
                    
                    default:
                        break;
                    }

                    if (current_piece != ' '){
                        if (tolower(current_piece) == current_piece){
                            //it is black
                            bitboards[(int)BitboardPieceType::Black] = bitboards[(int)BitboardPieceType::Black] | curr_piece_bit_string;
                            //cout << bitboards[(int)BitboardPieceType::Black] << endl;
                        }else{
                            //it is white
                            bitboards[(int)BitboardPieceType::White] = bitboards[(int)BitboardPieceType::White] | curr_piece_bit_string;
                        }
                    }

                }
            }
            
        }

        Position getTilePos(SDL_Point mouse_pos){
            this->mouse_pos = mouse_pos;
            this->is_dragging = true;

            int start_x = (SCREEN_WIDTH/2) - 4*TILE_WIDTH; int start_y = (SCREEN_HEIGHT/2) - 4*TILE_WIDTH;

            mouse_pos.x -= start_x;
            mouse_pos.y -= start_y;

            int row_remainder = mouse_pos.y % TILE_WIDTH;
            int row_ind = (int)(mouse_pos.y - row_remainder) / TILE_WIDTH;

            int col_remainder = mouse_pos.x % TILE_WIDTH;
            int col_ind = (int)(mouse_pos.x - col_remainder) / TILE_WIDTH;

            cout << "From --> Row: " << row_ind << " , Col: " << col_ind << endl;
            drag_from = {row_ind, col_ind};
            return drag_from;

        }

        void updateMouse(SDL_Point mouse_pos) {
            this->mouse_pos = mouse_pos;
        }

        void stopDragging(){
            this->is_dragging = false;
        }

        void makeMove(Move* move){
            cout << "Recieved the move" << endl;
            cout << "Description: \nFrom " << move->from.row << ", " << move->from.col
                 << "\nTo " << move->to.row << ", " << move->to.col << endl;

            int from_i = 63 - (move->from.row*8 + move->from.col);
            int to_i = 63 - (move->to.row*8 + move->to.col);


            cout << "White Bitboard Before" << endl;
            cout << bitboards[(int)BitboardPieceType::White] << endl;
            cout << "to_i: " << to_i << "  from_i: " << from_i << endl;

            bool is_white = bitboards[(int)BitboardPieceType::White][from_i] == 1;
            cout << "is_white: " << is_white << endl;
            cout << "val: " << bitboards[(int)BitboardPieceType::White][from_i] << endl;
            for (int i = 2; i < 8; ++i){ // Basically untested
                bitboards[i][to_i] = 0;

                if (bitboards[i][from_i] == 1){
                    bitboards[i][from_i] = 0;
                    bitboards[i][to_i] = 1;
                }
            }

            if (is_white){
                bitboards[0][from_i] = 0;
                bitboards[0][to_i] = 1;

                bitboards[1][from_i] = 0;
                bitboards[1][to_i] = 0;
            }else{
                bitboards[1][from_i] = 0;
                bitboards[1][to_i] = 1;

                bitboards[0][from_i] = 0;
                bitboards[0][to_i] = 0;
            }
            
            bitboards[(int)!is_white][to_i] = 1;

            cout << "White Bitboard After" << endl;
            cout << bitboards[(int)BitboardPieceType::White] << endl;

        }



    private:



        //              DRAWING AND RENDERING SECTION
        void drawPieces(){

            bitset<64> top_left; top_left.set(63, true);
            int start_x = (SCREEN_WIDTH/2) - 4*TILE_WIDTH; int start_y = (SCREEN_HEIGHT/2) - 4*TILE_WIDTH;
            
            for (int row = 0; row < 8; ++row){
                for (int col = 0; col < 8; ++col){
                    int new_x = start_x + col * TILE_WIDTH;
                    int new_y = start_y + row * TILE_WIDTH;

                    //Put in center of tile
                    new_x += TILE_WIDTH/2 - PIECE_WIDTH/2;
                    new_y += TILE_WIDTH/2 - PIECE_WIDTH/2;

                    Vector2i new_pos(new_x, new_y);
                    
                    //If this is the piece that is being dragged...
                    if (is_dragging && row == drag_from.row && col == drag_from.col){
                        new_pos = {mouse_pos.x - PIECE_WIDTH/2, mouse_pos.y - PIECE_WIDTH/2};
                    }

                    bitset<64> current_spot = top_left >> (row*8 + col);
                    if ( (current_spot & bitboards[(int)BitboardPieceType::White]) != 0){ //It is a white piece
                        //cout << "Trying to draw a piece!!" << endl;
                        drawTile(new_pos, PIECE_WIDTH, this->lightPieceColor, true);
                    }
                    if ( (current_spot & bitboards[(int)BitboardPieceType::Black]) != 0){ //It is a Black piece
                        drawTile(new_pos, PIECE_WIDTH, this->darkPieceColor, true);
                    }
                }
            }
        }

        // void drawPiece(Vector2i& pos, bool is_light, int width){
        //     SDL_Rect rect = {pos.x, pos.y, width, width};
        //     is_light ? SDL_SetRenderDrawColor(renderer, 230, 220, 186, 255) : SDL_SetRenderDrawColor(renderer, 202, 167, 132, 255);
        //     SDL_RenderFillRect(renderer, &rect);
        // }

        void drawBoard(){
            //background color
            SDL_SetRenderDrawColor(renderer, 172, 221, 214, 120); 
            SDL_RenderClear(renderer);


            int start_x = (SCREEN_WIDTH/2) - 4*TILE_WIDTH; int start_y = (SCREEN_HEIGHT/2) - 4*TILE_WIDTH;
            for (int row = 0; row < 8; ++row){
                for (int col = 0; col < 8; ++col){
                    int new_x = start_x + col * TILE_WIDTH;
                    int new_y = start_y + row * TILE_WIDTH;
                    Vector2i new_pos(new_x, new_y);


                    if (abs(row - col) % 2 == 0){
                        //Draw a light tile
                        drawTile(new_pos, TILE_WIDTH, this->lightTileColor);
                    }else{
                        //Draw a dark tile
                        drawTile(new_pos, TILE_WIDTH, this->darkTileColor);
                    }

                }
            }
        }

        void drawTile(Vector2i& pos, int width, Color color, bool is_piece = false){
            SDL_Rect* rect = new SDL_Rect();
            rect->x = pos.x; rect->y = pos.y; rect->w = width; rect->h = width;
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer, rect);
        }

        //*********************************//
        //***** Class Level Variables *****//
        //*********************************//

        SDL_Renderer* renderer;
        bool player_side;
        bool first = true;

        Position drag_from;
        SDL_Point mouse_pos;
        bool is_dragging;

        Color lightTileColor = {230, 220, 186, 255};
        Color darkTileColor = {202, 167, 132, 255};
        Color lightPieceColor = {255, 191, 134, 255};
        Color darkPieceColor = {51, 153, 255, 255};

        vector<bitset<64> > bitboards;
        uint64_t empty_board = 0;
        uint64_t full_board;

        vector<vector<char> > squares;
};

#endif