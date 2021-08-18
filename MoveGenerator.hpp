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

            //Generate pawn moves ----> TODO <--------------------------------- NOTE NOT EN PASSANT YET
            generatePawnOneForwardMoves(board);
            generatePawnTwoForwardMoves(board);            
            generatePawnAttackingMovesLeft(board);
            generatePawnAttackingMovesRight(board);

            

            //Generate Knight Moves
            generateKnightMoves(board);

            //Generate rook moves
            generateRookMoves(board);

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

                enemy_pieces = board.bitboards[(int)BitboardPieceType::Black];
            }else{
                eligable = ~board.bitboards[(int)BitboardPieceType::Black];
                friendly_pawns = board.bitboards[(int)BitboardPieceType::Black] & board.bitboards[(int)BitboardPieceType::Pawn];
                friendly_bishops = board.bitboards[(int)BitboardPieceType::Black] & board.bitboards[(int)BitboardPieceType::Bishop];
                friendly_rooks = board.bitboards[(int)BitboardPieceType::Black] & board.bitboards[(int)BitboardPieceType::Rook];
                friendly_knights = board.bitboards[(int)BitboardPieceType::Black] & board.bitboards[(int)BitboardPieceType::Knight];
                friendly_queens = board.bitboards[(int)BitboardPieceType::Black] & board.bitboards[(int)BitboardPieceType::Queen];
                friendly_kings = board.bitboards[(int)BitboardPieceType::Black] & board.bitboards[(int)BitboardPieceType::King];

                enemy_pieces = board.bitboards[(int)BitboardPieceType::White];
            }
        }

        //Generates all single forward pawn moves
        void generatePawnOneForwardMoves(Board& board){
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

        //Generates all double forward pawn moves
        void generatePawnTwoForwardMoves(Board& board){
            //This is a non_attacking move so we want to make any place with a piece not eligable
            bitset<64> current_eligable = ~board.bitboards[(int)BitboardPieceType::White] & ~board.bitboards[(int)BitboardPieceType::Black];
            bitset<64> shifted = on_top ? ((friendly_pawns & pawn_starting_pos) >> 16) & current_eligable : ((friendly_pawns & pawn_starting_pos) << 16) & current_eligable;

            while (shifted.count()){
                int lsb = bitscanForward(shifted);
                int to_square = 63 - lsb;
                Position to = {(to_square - to_square%8)/8, to_square % 8};
                int from_square = on_top ? to_square -16 : to_square + 16;
                Position from = {(from_square - from_square%8)/8, from_square % 8};

                if (is_white)
                    move_list.push_back(new Move(from, to, PieceType::WhitePawn));
                else
                    move_list.push_back(new Move(from, to, PieceType::BlackPawn));


                shifted ^= bitOps.trivial[lsb];
            }
        }

        //Generates all attacking pawn moves to the left
        
        void generatePawnAttackingMovesLeft(Board& board){
            //Do left attacks first
            bitset<64> shifted = on_top ? (friendly_pawns >> 9) : ((friendly_pawns) << 9);
            shifted &= eligable & pawn_attack_right_mask & enemy_pieces;

            while (shifted.count()){
                int lsb = bitscanForward(shifted);
                int to_square = 63 - lsb;
                Position to = {(to_square - to_square%8)/8, to_square % 8};
                int from_square = on_top ? to_square -9 : to_square + 9;
                Position from = {(from_square - from_square%8)/8, from_square % 8};

                if (is_white)
                    move_list.push_back(new Move(from, to, PieceType::WhitePawn));
                else
                    move_list.push_back(new Move(from, to, PieceType::BlackPawn));


                shifted ^= bitOps.trivial[lsb];
            }
        }

        //Generates all attacking pawn moves to the right
        void generatePawnAttackingMovesRight(Board& board){
            //Do left attacks first
            bitset<64> shifted = on_top ? (friendly_pawns >> 7) : ((friendly_pawns) << 7);
            shifted &= eligable & pawn_attack_right_mask & enemy_pieces;
            

            while (shifted.count()){
                int lsb = bitscanForward(shifted);
                int to_square = 63 - lsb;
                Position to = {(to_square - to_square%8)/8, to_square % 8};
                int from_square = on_top ? to_square -7 : to_square + 7;
                Position from = {(from_square - from_square%8)/8, from_square % 8};

                if (is_white)
                    move_list.push_back(new Move(from, to, PieceType::WhitePawn));
                else
                    move_list.push_back(new Move(from, to, PieceType::BlackPawn));


                shifted ^= bitOps.trivial[lsb];
            }
        }

        //Generates all moves for the knight
        void generateKnightMoves(Board& board){
            
            while (friendly_knights.any()){
                int lsb_from = bitscanForward(friendly_knights);
                int from_square = 63 - lsb_from;
                Position from = {(from_square - from_square%8)/8, from_square % 8};

                bitset<64> generated_moves = bitOps.knights[from_square] & eligable;
                while (generated_moves.any()){
                    int lsb_to = bitscanForward(generated_moves);
                    int to_square = 63 - lsb_to;
                    Position to = {(to_square - to_square%8)/8, to_square % 8};
                    if (is_white)
                        move_list.push_back(new Move(from, to, PieceType::WhiteKnight));
                    else
                        move_list.push_back(new Move(from, to, PieceType::BlackKnight));
                    generated_moves ^= bitOps.trivial[lsb_to];
                }

                friendly_knights ^= bitOps.trivial[lsb_from];
            }
            //loop through all the knights

                //loop through their potential moves and add them to the dict
        }

        void generateRookMoves(Board& board){
            bitset<64> blockers = board.bitboards[(int)BitboardPieceType::Black] | board.bitboards[(int)BitboardPieceType::White];
            
            while (friendly_rooks.any()){
                int lsb_from = bitscanForward(friendly_rooks);
                int from_square = 63 - lsb_from;
                Position from = {(from_square - from_square%8)/8, from_square % 8};
                bitset<64> attacks;
                
                //North part
                attacks |= bitOps.rays[(int)RayDirection::North][from_square];
                bitset<64> masked_blockers = blockers & bitOps.rays[(int)RayDirection::North][from_square];
                if (masked_blockers.any()){
                    int blocker_index = 63 - bitscanForward(masked_blockers);
                    attacks &= ~bitOps.rays[(int)RayDirection::North][blocker_index];
                }


                //East part
                attacks |= bitOps.rays[(int)RayDirection::East][from_square];
                masked_blockers = blockers & bitOps.rays[(int)RayDirection::East][from_square];
                if (masked_blockers.any()){
                    int blocker_index = 63 - bitscanReverse(masked_blockers);
                    attacks &= ~bitOps.rays[(int)RayDirection::East][blocker_index];
                }

                //South part
                attacks |= bitOps.rays[(int)RayDirection::South][from_square];
                masked_blockers = blockers & bitOps.rays[(int)RayDirection::South][from_square];
                if (masked_blockers.any()){
                    int blocker_index = 63 - bitscanReverse(masked_blockers);
                    attacks &= ~bitOps.rays[(int)RayDirection::South][blocker_index];
                }

                //West part
                attacks |= bitOps.rays[(int)RayDirection::West][from_square];
                masked_blockers = blockers & bitOps.rays[(int)RayDirection::West][from_square];
                if (masked_blockers.any()){
                    int blocker_index = 63 - bitscanForward(masked_blockers);
                    attacks &= ~bitOps.rays[(int)RayDirection::West][blocker_index];
                }
                
                attacks &= eligable;


                while (attacks.any()){
                    int lsb_to = bitscanForward(attacks);
                    int to_square = 63 - lsb_to;
                    Position to = {(to_square - to_square%8)/8, to_square % 8};
                    if (is_white)
                        move_list.push_back(new Move(from, to, PieceType::WhiteRook));
                    else
                        move_list.push_back(new Move(from, to, PieceType::BlackRook));
                    attacks ^= bitOps.trivial[lsb_to];
                }
                friendly_rooks ^= bitOps.trivial[lsb_from];
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

        //Returns bit index of MSB
        int bitscanReverse(bitset<64>& bits){
            for (int i = 63; i >= 0; --i){
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

        /// - Pawn move generation
        bitset<64> pawn_starting_pos{"0000000011111111000000000000000000000000000000001111111100000000"};
        bitset<64> pawn_attack_right_mask{"0111111101111111011111110111111101111111011111110111111101111111"};
        bitset<64> pawn_attack_left_mask{"1111111011111110111111101111111011111110111111101111111011111110"};

        bitset<64> eligable;
        bitset<64> friendly_pawns;
        bitset<64> friendly_bishops;
        bitset<64> friendly_rooks;
        bitset<64> friendly_knights;
        bitset<64> friendly_queens;
        bitset<64> friendly_kings;

        bitset<64> enemy_pieces;
        bitset<64> occupied;
};

#endif