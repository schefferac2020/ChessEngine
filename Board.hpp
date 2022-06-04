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
#include "Utils.hpp"

#define TILE_WIDTH 75
#define PIECE_WIDTH 40

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

        //Currently does not do anything lol
        void load_from_fen(string fen){
            int row = 0; int col = 0;
            for (char ch : fen){
                if (ch ==' ')
                    break;

                if (isalpha(ch)){
                    this->squares[row][col] = ch;
                    ++col;
                }else if (isnumber(ch)){
                    col += ch - '0';
                }else if (ch == '/'){
                    ++row;
                    col = 0;
                }
            }
        }

        //Initializes the bitboards. Could initialize from fen later
        void init_board(){
            bitboards.resize(8);
            this->squares.resize(8, vector<char>(8, ' '));
            load_from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"); // Starting position
            //("4k3/p7/8/8/5R2/8/8/4K3 b - - 0 3");
            
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
                        }else{
                            //it is white
                            bitboards[(int)BitboardPieceType::White] = bitboards[(int)BitboardPieceType::White] | curr_piece_bit_string;
                        }
                    }

                }
            }

            filenames = {"orangePawn.png", "orangeKnight.png", "orangeBishop.png", "orangeQueen.png", "orangeKing.png", "orangeRook.png",
                        "bluePawn.png", "blueKnight.png", "blueBishop.png", "blueQueen.png", "blueKing.png", "blueRook.png"};
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

            // cout << "From --> Row: " << row_ind << " , Col: " << col_ind << endl;
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
            int from_i = 63 - (move->from.row*8 + move->from.col);
            int to_i = 63 - (move->to.row*8 + move->to.col);

            bool is_white = bitboards[(int)BitboardPieceType::White][from_i] == 1;

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
        vector<bitset<64> > bitboards;
        uint64_t empty_board = 0;
        uint64_t full_board;

        vector<vector<char> > squares;

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
                        //drawTile(new_pos, PIECE_WIDTH, this->lightPieceColor, true);

                        if ( (current_spot & bitboards[(int)BitboardPieceType::Pawn]) != 0){
                            drawPiece(new_pos, PIECE_WIDTH, PieceType::WhitePawn);
                        } else if ( (current_spot & bitboards[(int)BitboardPieceType::Rook]) != 0){
                            drawPiece(new_pos, PIECE_WIDTH, PieceType::WhiteRook);
                        } else if ( (current_spot & bitboards[(int)BitboardPieceType::Knight]) != 0){
                            drawPiece(new_pos, PIECE_WIDTH, PieceType::WhiteKnight);
                        }else if ( (current_spot & bitboards[(int)BitboardPieceType::Bishop]) != 0){
                            drawPiece(new_pos, PIECE_WIDTH, PieceType::WhiteBishop);
                        }else if ( (current_spot & bitboards[(int)BitboardPieceType::Queen]) != 0){
                            drawPiece(new_pos, PIECE_WIDTH, PieceType::WhiteQueen);
                        }else if ( (current_spot & bitboards[(int)BitboardPieceType::King]) != 0){
                            drawPiece(new_pos, PIECE_WIDTH, PieceType::WhiteKing);
                        }
                        
                    }
                    if ( (current_spot & bitboards[(int)BitboardPieceType::Black]) != 0){ //It is a Black piece
                        //drawTile(new_pos, PIECE_WIDTH, this->darkPieceColor, true);

                        if ( (current_spot & bitboards[(int)BitboardPieceType::Pawn]) != 0){
                            drawPiece(new_pos, PIECE_WIDTH, PieceType::BlackPawn);
                        } else if ( (current_spot & bitboards[(int)BitboardPieceType::Rook]) != 0){
                            drawPiece(new_pos, PIECE_WIDTH, PieceType::BlackRook);
                        } else if ( (current_spot & bitboards[(int)BitboardPieceType::Knight]) != 0){
                            drawPiece(new_pos, PIECE_WIDTH, PieceType::BlackKnight);
                        }else if ( (current_spot & bitboards[(int)BitboardPieceType::Bishop]) != 0){
                            drawPiece(new_pos, PIECE_WIDTH, PieceType::BlackBishop);
                        }else if ( (current_spot & bitboards[(int)BitboardPieceType::Queen]) != 0){
                            drawPiece(new_pos, PIECE_WIDTH, PieceType::BlackQueen);
                        }else if ( (current_spot & bitboards[(int)BitboardPieceType::King]) != 0){
                            drawPiece(new_pos, PIECE_WIDTH, PieceType::BlackKing);
                        }
                    }
                }
            }
        }

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

        void drawPiece(Vector2i& pos, int width, PieceType piece_type){
            
            SDL_Rect* dest = new SDL_Rect();
            dest->x = pos.x; dest->y = pos.y; dest->w = width; dest->h = width;
            string file = "./res/gfx/" + filenames[(int) piece_type];
            const char* file_cstr = file.c_str();
            SDL_Texture* texture = IMG_LoadTexture(renderer, file_cstr);


            SDL_RenderCopy(renderer, texture, NULL, dest);

            SDL_DestroyTexture(texture);
        }


        Color lightTileColor = {230, 220, 186, 255};
        Color darkTileColor = {202, 167, 132, 255};
        Color lightPieceColor = {255, 191, 134, 255};
        Color darkPieceColor = {51, 153, 255, 255};
        int i = 0;

        vector<string> filenames;
};

#endif