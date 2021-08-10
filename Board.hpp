#ifndef BOARD_HPP
#define BOARD_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdint>
#include <vector>
#include <bitset>

#include "Math.hpp"
#include "Utils.hpp"

#define TILE_WIDTH 75
#define PIECE_WIDTH 50

enum class BitboardPieceType : uint8_t {
        White, Black, Pawn, Knight, Bishop, Queen, King, Rook
};

struct Color{
    int r, g, b, a;
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
                                {'r', ' ', 'b', 'q', 'k', 'b', 'n', 'r'},
                                {'p', 'p', 'p', 'p', ' ', 'p', 'p', 'p'},
                                {' ', ' ', 'n', ' ', ' ', ' ', ' ', ' '},
                                {' ', 'B', ' ', ' ', 'p', ' ', ' ', ' '},
                                {' ', ' ', ' ', ' ', 'P', ' ', ' ', ' '},
                                {' ', ' ', ' ', ' ', ' ', 'N', ' ', ' '},
                                {'P', 'P', 'P', 'P', ' ', 'P', 'P', 'P'},
                                {'R', 'N', 'B', 'Q', 'K', ' ', ' ', 'R'}

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



    private:



        //              DRAWING AND RENDERING SECTION
        void drawPieces(){

            //Get display width and height;
            int screen_width = 1280; int screen_height = 720;
            bitset<64> top_left; top_left.set(63, true);


            int start_x = (screen_width/2) - 4*TILE_WIDTH; int start_y = (screen_height/2) - 4*TILE_WIDTH;
            
            for (int row = 0; row < 8; ++row){
                for (int col = 0; col < 8; ++col){
                    int new_x = start_x + col * TILE_WIDTH;
                    int new_y = start_y + row * TILE_WIDTH;

                    //Put in center of tile
                    new_x += TILE_WIDTH/2 - PIECE_WIDTH/2;
                    new_y += TILE_WIDTH/2 - PIECE_WIDTH/2;

                    Vector2i new_pos(new_x, new_y);
                    
                    bitset<64> current_spot = top_left >> (row*8 + col);
                    if ( (current_spot & bitboards[(int)BitboardPieceType::White]) != 0){ //It is a white piece
                        //cout << "Trying to draw a piece!!" << endl;
                        drawTile(new_pos, PIECE_WIDTH, this->lightPieceColor);
                    }
                    if ( (current_spot & bitboards[(int)BitboardPieceType::Black]) != 0){ //It is a Black piece
                        drawTile(new_pos, PIECE_WIDTH, this->darkPieceColor);
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

            //Get display width and height;
            int screen_width = 1280; int screen_height = 720;

            int start_x = (screen_width/2) - 4*TILE_WIDTH; int start_y = (screen_height/2) - 4*TILE_WIDTH;
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

        void drawTile(Vector2i& pos, int width, Color color){
            SDL_Rect rect = {pos.x, pos.y, width, width};
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer, &rect);
        }

        //*********************************//
        //***** Class Level Variables *****//
        //*********************************//

        SDL_Renderer* renderer;
        bool player_side;
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