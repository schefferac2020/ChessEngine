#ifndef MOVE_GENERATOR_HPP
#define MOVE_GENERATOR_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <bitset>
#include <iostream>
#include <vector>

#include "Utils.hpp"
#include "Move.hpp"
#include "BitOps.hpp"
#include "Board.hpp"

using namespace std;

class MoveGenerator {
    public:
        MoveGenerator(bool in_white, bool on_top_in) : is_white(in_white), on_top(on_top_in) { }

        vector<Move*>& generateMoves(Board& board){
            //Reset all of the env vars
            this->move_list = {};
            resetBitboards(board);

            //Generate pawn moves
            generatePawnMoves(board);


            return this->move_list;
        }



    private:

        void resetBitboards(Board& board){
            if (is_white){
                eligable = ~board.bitboards[(int)BitboardPieceType::White];
                friendly_pawns = board.bitboards[(int)BitboardPieceType::White] & board.bitboards[(int)BitboardPieceType::Pawn];
                friendly_bishops = board.bitboards[(int)BitboardPieceType::White] & board.bitboards[(int)BitboardPieceType::Bishop];
                friendly_rooks = board.bitboards[(int)BitboardPieceType::White] & board.bitboards[(int)BitboardPieceType::Rook];
                friendly_knights = board.bitboards[(int)BitboardPieceType::White] & board.bitboards[(int)BitboardPieceType::Knight];
                friendly_queens = board.bitboards[(int)BitboardPieceType::White] & board.bitboards[(int)BitboardPieceType::Queen];
                friendly_kings = board.bitboards[(int)BitboardPieceType::White] & board.bitboards[(int)BitboardPieceType::King];
            }else{
                eligable = ~board.bitboards[(int)BitboardPieceType::Black];
                friendly_pawns = board.bitboards[(int)BitboardPieceType::Black] & board.bitboards[(int)BitboardPieceType::Pawn];
                friendly_bishops = board.bitboards[(int)BitboardPieceType::Black] & board.bitboards[(int)BitboardPieceType::Bishop];
                friendly_rooks = board.bitboards[(int)BitboardPieceType::Black] & board.bitboards[(int)BitboardPieceType::Rook];
                friendly_knights = board.bitboards[(int)BitboardPieceType::Black] & board.bitboards[(int)BitboardPieceType::Knight];
                friendly_queens = board.bitboards[(int)BitboardPieceType::Black] & board.bitboards[(int)BitboardPieceType::Queen];
                friendly_kings = board.bitboards[(int)BitboardPieceType::Black] & board.bitboards[(int)BitboardPieceType::King];

            }
        }


        void generatePawnMoves(Board& board){
            //This is a non_attacking move so we want to make any place with a piece not eligable

            bitset<64> current_eligable = ~board.bitboards[(int)BitboardPieceType::White] & ~board.bitboards[(int)BitboardPieceType::Black];
            bitset<64> shifted = on_top ? (friendly_pawns >> 8) & current_eligable : (friendly_pawns << 8) & current_eligable;


            while (shifted.count()){
                int lsb = bitscanForward(shifted);
                int to_square = 63 - lsb;
                Position to = {(to_square - to_square%8)/8, to_square % 8};
                int from_square = on_top ? to_square -8 : to_square + 8;
                Position from = {(from_square - from_square%8)/8, from_square % 8};

                if (is_white)
                    move_list.push_back(new Move(from, to, PieceType::WhitePawn));
                else
                    move_list.push_back(new Move(from, to, PieceType::BlackPawn));


                shifted ^= bitOps.trivial[lsb];
            }
        }


        //Returns bit index of LSB (starting from left) 011000 would return 3
        int bitscanForward(bitset<64>& bits){
            for (int i = 0; i < 64; ++i){
                if (bits[i]){
                    return i;
                }
            }
            return -1;
        }

        vector<Move*> move_list;
        bool is_white;
        bool on_top;

        
        BitOps bitOps;

        //Bitboards
        bitset<64> eligable;
        bitset<64> friendly_pawns;
        bitset<64> friendly_bishops;
        bitset<64> friendly_rooks;
        bitset<64> friendly_knights;
        bitset<64> friendly_queens;
        bitset<64> friendly_kings;
};

#endif