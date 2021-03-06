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

            generateEnemyMoves(board);

            //Check if in check!
            int num_checkers = enemy_checkers.count();
            cout << num_checkers << endl;
            if (num_checkers == 2){
                //The only way out is that the king moves
                generateKingMoves(board, false);
                return this->move_list;
            }
            else if (num_checkers == 1){
                //Moves are heavily restricted
                generateKingMoves(board, false);

                // To get out of check, you can capture the checker
                bitset<64> capture_mask = enemy_checkers; 
                

                //Or you can block the checker
                bitset<64> blocking_mask;
                //bitset<64> blocking_mask = 
                if ((enemy_checkers & enemy_bishops).any() || (enemy_checkers & enemy_rooks).any()){
                    cout << "The enemy is checking with a sliding piece!!" << endl;
                    blocking_mask = getSliderRouteToKing(board);
                }else{
                    cout << "In check but not sliding piece" << endl;
                    blocking_mask.reset(); // Cannot block this piece
                }
                
                friendly_eligable &= (blocking_mask | capture_mask);
            }
            
            //Generate pawn moves ----> TODO <--------------------------------- NOTE NOT EN PASSANT YET
            generatePawnOneForwardMoves(board);
            generatePawnTwoForwardMoves(board);            
            generatePawnAttackingMovesLeft(board, false);
            generatePawnAttackingMovesRight(board, false);

            //Generate Knight Moves
            generateKnightMoves(board, false);

            //Generate rook moves
            generateRookMoves(board, false);

            //Generate bishop moves
            generateBishopMoves(board, false);

            //Generate king moves
            generateKingMoves(board, false);
            



            return this->move_list;
        }

        //Generates enemy moves and stores in enemyAttacking
        void generateEnemyMoves(Board& board) {
            this->enemy_attacking.reset();
            this->enemy_checkers.reset();
            generatePawnAttackingMovesLeft(board, true);
            generatePawnAttackingMovesRight(board, true);

            //Generate Knight Moves
            generateKnightMoves(board, true);

            //Generate rook moves
            generateRookMoves(board, true);

            //Generate bishop moves
            generateBishopMoves(board, true);

            //Generate king moves
            generateKingMoves(board, true);
        }



    private:

        void resetBitboards(Board& board){
            if (is_white){
                friendly_eligable = ~board.bitboards[(int)BitboardPieceType::White];
                enemy_eligable = ~board.bitboards[(int)BitboardPieceType::Black];

                friendly_pawns = board.bitboards[(int)BitboardPieceType::White] & board.bitboards[(int)BitboardPieceType::Pawn];
                enemy_pawns = board.bitboards[(int)BitboardPieceType::Black] & board.bitboards[(int)BitboardPieceType::Pawn];

                friendly_bishops = board.bitboards[(int)BitboardPieceType::White] & board.bitboards[(int)BitboardPieceType::Bishop];
                friendly_bishops |= board.bitboards[(int)BitboardPieceType::White] & board.bitboards[(int)BitboardPieceType::Queen];
                enemy_bishops = board.bitboards[(int)BitboardPieceType::Black] & board.bitboards[(int)BitboardPieceType::Bishop];
                enemy_bishops |= board.bitboards[(int)BitboardPieceType::Black] & board.bitboards[(int)BitboardPieceType::Queen];

                friendly_rooks = board.bitboards[(int)BitboardPieceType::White] & board.bitboards[(int)BitboardPieceType::Rook];
                friendly_rooks |= board.bitboards[(int)BitboardPieceType::White] & board.bitboards[(int)BitboardPieceType::Queen];
                enemy_rooks = board.bitboards[(int)BitboardPieceType::Black] & board.bitboards[(int)BitboardPieceType::Rook];
                enemy_rooks |= board.bitboards[(int)BitboardPieceType::Black] & board.bitboards[(int)BitboardPieceType::Queen];
                
                friendly_knights = board.bitboards[(int)BitboardPieceType::White] & board.bitboards[(int)BitboardPieceType::Knight];
                enemy_knights = board.bitboards[(int)BitboardPieceType::Black] & board.bitboards[(int)BitboardPieceType::Knight];

                friendly_kings = board.bitboards[(int)BitboardPieceType::White] & board.bitboards[(int)BitboardPieceType::King];
                enemy_kings = board.bitboards[(int)BitboardPieceType::Black] & board.bitboards[(int)BitboardPieceType::King];

                friendly_pieces = board.bitboards[(int)BitboardPieceType::White];
                enemy_pieces = board.bitboards[(int)BitboardPieceType::Black];
            }else{
                friendly_eligable = ~board.bitboards[(int)BitboardPieceType::Black];
                enemy_eligable = ~board.bitboards[(int)BitboardPieceType::White];

                friendly_pawns = board.bitboards[(int)BitboardPieceType::Black] & board.bitboards[(int)BitboardPieceType::Pawn];
                enemy_pawns = board.bitboards[(int)BitboardPieceType::White] & board.bitboards[(int)BitboardPieceType::Pawn];

                friendly_bishops = board.bitboards[(int)BitboardPieceType::Black] & board.bitboards[(int)BitboardPieceType::Bishop];
                friendly_bishops |= board.bitboards[(int)BitboardPieceType::Black] & board.bitboards[(int)BitboardPieceType::Queen];
                enemy_bishops = board.bitboards[(int)BitboardPieceType::White] & board.bitboards[(int)BitboardPieceType::Bishop];
                enemy_bishops |= board.bitboards[(int)BitboardPieceType::White] & board.bitboards[(int)BitboardPieceType::Queen];

                friendly_rooks = board.bitboards[(int)BitboardPieceType::Black] & board.bitboards[(int)BitboardPieceType::Rook];
                friendly_rooks |= board.bitboards[(int)BitboardPieceType::Black] & board.bitboards[(int)BitboardPieceType::Queen];
                enemy_rooks = board.bitboards[(int)BitboardPieceType::White] & board.bitboards[(int)BitboardPieceType::Rook];
                enemy_rooks |= board.bitboards[(int)BitboardPieceType::White] & board.bitboards[(int)BitboardPieceType::Queen];
                
                friendly_knights = board.bitboards[(int)BitboardPieceType::Black] & board.bitboards[(int)BitboardPieceType::Knight];
                enemy_knights = board.bitboards[(int)BitboardPieceType::White] & board.bitboards[(int)BitboardPieceType::Knight];

                friendly_kings = board.bitboards[(int)BitboardPieceType::Black] & board.bitboards[(int)BitboardPieceType::King];
                enemy_kings = board.bitboards[(int)BitboardPieceType::White] & board.bitboards[(int)BitboardPieceType::King];

                friendly_pieces = board.bitboards[(int)BitboardPieceType::Black];
                enemy_pieces = board.bitboards[(int)BitboardPieceType::White];
            }

        }

        //Generates all single forward pawn moves
        void generatePawnOneForwardMoves(Board& board){
            //This is a non_attacking move so we want to make any place with a piece not eligable
            bitset<64> current_eligable = ~board.bitboards[(int)BitboardPieceType::White] & ~board.bitboards[(int)BitboardPieceType::Black] & friendly_eligable;
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
            bitset<64> current_eligable = ~board.bitboards[(int)BitboardPieceType::White] & ~board.bitboards[(int)BitboardPieceType::Black] & friendly_eligable;
            bitset<64> shifted = on_top ? ((friendly_pawns & pawn_starting_pos) >> 16) & current_eligable : ((friendly_pawns & pawn_starting_pos) << 16) & current_eligable;

            while (shifted.count()){
                int lsb = bitscanForward(shifted);
                int to_square = 63 - lsb;
                Position to = {(to_square - to_square%8)/8, to_square % 8};
                int from_square = on_top ? to_square -16 : to_square + 16;
                Position from = {(from_square - from_square%8)/8, from_square % 8};                 // Test this it do be sketch sketch like a pest pest can you guess guess what to do next next? Me neither

                if (is_white)
                    move_list.push_back(new Move(from, to, PieceType::WhitePawn));
                else
                    move_list.push_back(new Move(from, to, PieceType::BlackPawn));


                shifted ^= bitOps.trivial[lsb];
            }
        }

        //Generates all attacking pawn moves to the left
        
        void generatePawnAttackingMovesLeft(Board& board, bool is_enemy){
            bitset<64> pawns = is_enemy ? enemy_pawns : friendly_pawns;
            bitset<64> eligable = is_enemy ? enemy_eligable : friendly_eligable;
            bitset<64> other_pieces = is_enemy ? friendly_pieces : enemy_pieces;
            bool on_top_temp = on_top;
            if (is_enemy){
                on_top_temp = !on_top;
            }

            //Do left attacks first
            bitset<64> shifted = on_top_temp ? (pawns >> 9) : ((pawns) << 9);
            shifted &= on_top_temp ? pawn_right_mask : pawn_left_mask;

            
            
            if (is_enemy){
                this->enemy_attacking |= shifted;
            }
            shifted &= other_pieces;
            shifted &= eligable;

            while (shifted.count()){
                int lsb = bitscanForward(shifted);
                int to_square = 63 - lsb;
                Position to = {(to_square - to_square%8)/8, to_square % 8};
                int from_square = on_top_temp ? to_square -9 : to_square + 9;
                Position from = {(from_square - from_square%8)/8, from_square % 8};

                //check if the enemy pawn is checking the king
                if (is_enemy){
                    int lsb_from = 63-from_square;
                    bool attacking_king = (bitOps.trivial[lsb] & friendly_kings).any();
                    cout << attacking_king << endl;
                    if (attacking_king){
                        this->enemy_checkers |= bitOps.trivial[lsb_from];
                    }
                }else{
                    if (is_white){
                        move_list.push_back(new Move(from, to, PieceType::WhitePawn));
                    }
                    else{
                        move_list.push_back(new Move(from, to, PieceType::BlackPawn));
                    }
                }

                

                shifted ^= bitOps.trivial[lsb];
            }
        }

        //Generates all attacking pawn moves to the right
        void generatePawnAttackingMovesRight(Board& board, bool is_enemy){
            bitset<64> pawns = is_enemy ? enemy_pawns : friendly_pawns;
            bitset<64> eligable = is_enemy ? enemy_eligable : friendly_eligable;
            bitset<64> other_pieces = is_enemy ? friendly_pieces : enemy_pieces;
            bool on_top_temp = on_top;
            if (is_enemy){
                on_top_temp = !on_top;
            }

            //Do left attacks first
            bitset<64> shifted = on_top_temp ? (pawns >> 7) : ((pawns) << 7);
            shifted &= on_top_temp ? pawn_left_mask : pawn_right_mask;
            
            
            if (is_enemy){
                this->enemy_attacking |= shifted;
            }
            
            shifted &= other_pieces;
            shifted &= eligable;

            while (shifted.count()){
                int lsb = bitscanForward(shifted);

    

                int to_square = 63 - lsb;
                Position to = {(to_square - to_square%8)/8, to_square % 8};
                int from_square = on_top_temp ? to_square -7 : to_square + 7;
                Position from = {(from_square - from_square%8)/8, from_square % 8};

                //check if the enemy pawn is checking the king
                if (is_enemy){
                    int lsb_from = 63-from_square;
                    bool attacking_king = (bitOps.trivial[lsb] & friendly_kings).any();
                    if (attacking_king){
                        this->enemy_checkers |= bitOps.trivial[lsb_from];
                    }
                }else{
                    if (is_white)
                        move_list.push_back(new Move(from, to, PieceType::WhitePawn));
                    else
                        move_list.push_back(new Move(from, to, PieceType::BlackPawn));
                }

                shifted ^= bitOps.trivial[lsb];
            }
        }

        //Generates all moves for the knight
        void generateKnightMoves(Board& board, bool is_enemy){
            bitset<64> knights = is_enemy ? enemy_knights : friendly_knights;
            bitset<64> eligable = is_enemy ? enemy_eligable : friendly_eligable;
            
            while (knights.any()){
                int lsb_from = bitscanForward(knights);
                int from_square = 63 - lsb_from;
                Position from = {(from_square - from_square%8)/8, from_square % 8};

                bitset<64> generated_moves = bitOps.knights[from_square];
                if (is_enemy){
                    this->enemy_attacking |= generated_moves;
                    knights ^= bitOps.trivial[lsb_from];
                    
                    //Check if this piece checks the king
                    bool attacking_king = (generated_moves & friendly_kings).any();
                    if (attacking_king){
                        this->enemy_checkers |= bitOps.trivial[lsb_from];
                    }

                    continue;
                }
                generated_moves &= eligable;

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

                knights ^= bitOps.trivial[lsb_from];
            }
        }

        //Generates all of the king moves (does not account for check currently)
        void generateKingMoves(Board& board, bool is_enemy){
            bitset<64> kings = is_enemy ? enemy_kings : friendly_kings;
            bitset<64> eligable = is_enemy ? enemy_eligable : friendly_eligable;

            while (kings.any()){
                int lsb_from = bitscanForward(kings);
                int from_square = 63 - lsb_from;
                Position from = {(from_square - from_square%8)/8, from_square % 8};

                bitset<64> generated_moves = bitOps.kings[from_square];
                
                
                if (is_enemy){
                    this->enemy_attacking |= generated_moves;
                    kings ^= bitOps.trivial[lsb_from];
                    continue;
                }
                generated_moves &= ~enemy_attacking;

                generated_moves &= eligable;
                

                while (generated_moves.any()){
                    int lsb_to = bitscanForward(generated_moves);
                    int to_square = 63 - lsb_to;
                    Position to = {(to_square - to_square%8)/8, to_square % 8};
                    if (is_white)
                        move_list.push_back(new Move(from, to, PieceType::WhiteKing));
                    else
                        move_list.push_back(new Move(from, to, PieceType::BlackKing));
                    generated_moves ^= bitOps.trivial[lsb_to];
                }

                kings ^= bitOps.trivial[lsb_from];
            }
        }

        //Generates all of the rook moves
        void generateRookMoves(Board& board, bool is_enemy){
            bitset<64> rooks = is_enemy ? enemy_rooks : friendly_rooks;
            bitset<64> eligable = is_enemy ? enemy_eligable : friendly_eligable;

            bitset<64> blockers = board.bitboards[(int)BitboardPieceType::Black] | board.bitboards[(int)BitboardPieceType::White];
            
            if (is_enemy){ //Dont consider the friendly king when generating attacking squares.
                blockers &= ~friendly_kings;
            }

            while (rooks.any()){
                int lsb_from = bitscanForward(rooks);
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
                
                

                if (is_enemy){
                    this->enemy_attacking |= attacks;
                    rooks ^= bitOps.trivial[lsb_from];

                    //Check if this piece checks the king
                    bool attacking_king = (attacks & friendly_kings).any();
                    if (attacking_king){
                        this->enemy_checkers |= bitOps.trivial[lsb_from];
                    }
                    continue;
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
                rooks ^= bitOps.trivial[lsb_from];
            }
        }


        //Generates all of the bishop moves
        void generateBishopMoves(Board& board, bool is_enemy){
            bitset<64> bishops = is_enemy ? enemy_bishops : friendly_bishops;
            bitset<64> eligable = is_enemy ? enemy_eligable : friendly_eligable;

            bitset<64> blockers = board.bitboards[(int)BitboardPieceType::Black] | board.bitboards[(int)BitboardPieceType::White];
            
            if (is_enemy){ //Dont consider the friendly king when generating attacking squares.
                blockers &= ~friendly_kings;
            }

            while (bishops.any()){
                int lsb_from = bitscanForward(bishops);
                int from_square = 63 - lsb_from;
                Position from = {(from_square - from_square%8)/8, from_square % 8};
                bitset<64> attacks;
                
                //Northeast part
                attacks |= bitOps.rays[(int)RayDirection::Northeast][from_square];
                bitset<64> masked_blockers = blockers & bitOps.rays[(int)RayDirection::Northeast][from_square];
                if (masked_blockers.any()){
                    int blocker_index = 63 - bitscanForward(masked_blockers);
                    attacks &= ~bitOps.rays[(int)RayDirection::Northeast][blocker_index];
                }

                //Southeast part
                attacks |= bitOps.rays[(int)RayDirection::Southeast][from_square];
                masked_blockers = blockers & bitOps.rays[(int)RayDirection::Southeast][from_square];
                if (masked_blockers.any()){
                    int blocker_index = 63 - bitscanReverse(masked_blockers);
                    attacks &= ~bitOps.rays[(int)RayDirection::Southeast][blocker_index];
                }

                //Southwest part
                attacks |= bitOps.rays[(int)RayDirection::Southwest][from_square];
                masked_blockers = blockers & bitOps.rays[(int)RayDirection::Southwest][from_square];
                if (masked_blockers.any()){
                    int blocker_index = 63 - bitscanReverse(masked_blockers);
                    attacks &= ~bitOps.rays[(int)RayDirection::Southwest][blocker_index];
                }

                //Northwest part
                attacks |= bitOps.rays[(int)RayDirection::Northwest][from_square];
                masked_blockers = blockers & bitOps.rays[(int)RayDirection::Northwest][from_square];
                if (masked_blockers.any()){
                    int blocker_index = 63 - bitscanForward(masked_blockers);
                    attacks &= ~bitOps.rays[(int)RayDirection::Northwest][blocker_index];
                }
                
                

                if (is_enemy){
                    this->enemy_attacking |= attacks;
                    bishops ^= bitOps.trivial[lsb_from];

                    //Check if this piece checks the king
                    bool attacking_king = (attacks & friendly_kings).any();
                    if (attacking_king){
                        this->enemy_checkers |= bitOps.trivial[lsb_from];
                    }
                    continue;
                }
                attacks &= eligable;

                while (attacks.any()){
                    int lsb_to = bitscanForward(attacks);
                    int to_square = 63 - lsb_to;
                    Position to = {(to_square - to_square%8)/8, to_square % 8};
                    if (is_white)
                        move_list.push_back(new Move(from, to, PieceType::WhiteBishop));
                    else
                        move_list.push_back(new Move(from, to, PieceType::BlackBishop));
                    attacks ^= bitOps.trivial[lsb_to];
                }
                bishops ^= bitOps.trivial[lsb_from];
            }
        }


        // Generates the sliding route towards the king from a position  <-------------- TODO: This is hack and can be made faster (eg by doing comp at beginning of the program)
        // Used for finding where the player can play to block a check
        // from_square = 63 - lsb 
        bitset<64> getSliderRouteToKing(Board& board){
            

            int lsb_from = bitscanForward(enemy_checkers);
            int from_square = 63 - lsb_from;
            bitset<64> attacks;
            bitset<64> blockers = friendly_kings;

            //Northeast part
            bitset<64> masked_blockers = blockers & bitOps.rays[(int)RayDirection::Northeast][from_square];
            if (masked_blockers.any()){
                attacks |= bitOps.rays[(int)RayDirection::Northeast][from_square];
                int blocker_index = 63 - bitscanForward(masked_blockers);
                attacks &= ~bitOps.rays[(int)RayDirection::Northeast][blocker_index];
            }

            //Southeast part
            masked_blockers = blockers & bitOps.rays[(int)RayDirection::Southeast][from_square];
            if (masked_blockers.any()){
                attacks |= bitOps.rays[(int)RayDirection::Southeast][from_square];
                int blocker_index = 63 - bitscanReverse(masked_blockers);
                attacks &= ~bitOps.rays[(int)RayDirection::Southeast][blocker_index];
            }

            //Southwest part
            masked_blockers = blockers & bitOps.rays[(int)RayDirection::Southwest][from_square];
            if (masked_blockers.any()){
                attacks |= bitOps.rays[(int)RayDirection::Southwest][from_square];
                int blocker_index = 63 - bitscanReverse(masked_blockers);
                attacks &= ~bitOps.rays[(int)RayDirection::Southwest][blocker_index];
            }

            //Northwest part
            masked_blockers = blockers & bitOps.rays[(int)RayDirection::Northwest][from_square];
            if (masked_blockers.any()){
                attacks |= bitOps.rays[(int)RayDirection::Northwest][from_square];
                int blocker_index = 63 - bitscanForward(masked_blockers);
                attacks &= ~bitOps.rays[(int)RayDirection::Northwest][blocker_index];
            }

            //North part 
            masked_blockers = blockers & bitOps.rays[(int)RayDirection::North][from_square];
            if (masked_blockers.any()){
                attacks |= bitOps.rays[(int)RayDirection::North][from_square];
                int blocker_index = 63 - bitscanForward(masked_blockers);
                attacks &= ~bitOps.rays[(int)RayDirection::North][blocker_index];
            }

            //East part
            masked_blockers = blockers & bitOps.rays[(int)RayDirection::East][from_square];
            if (masked_blockers.any()){
                attacks |= bitOps.rays[(int)RayDirection::East][from_square];
                int blocker_index = 63 - bitscanReverse(masked_blockers);
                attacks &= ~bitOps.rays[(int)RayDirection::East][blocker_index];
            }

            //South part
            masked_blockers = blockers & bitOps.rays[(int)RayDirection::South][from_square];
            if (masked_blockers.any()){
                attacks |= bitOps.rays[(int)RayDirection::South][from_square];
                int blocker_index = 63 - bitscanReverse(masked_blockers);
                attacks &= ~bitOps.rays[(int)RayDirection::South][blocker_index];
            }

            //West part
            masked_blockers = blockers & bitOps.rays[(int)RayDirection::West][from_square];
            if (masked_blockers.any()){
                attacks |= bitOps.rays[(int)RayDirection::West][from_square];
                int blocker_index = 63 - bitscanForward(masked_blockers);
                attacks &= ~bitOps.rays[(int)RayDirection::West][blocker_index];
            }
            return attacks;
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
        bitset<64> pawn_right_mask{"0111111101111111011111110111111101111111011111110111111101111111"};
        bitset<64> pawn_left_mask{"1111111011111110111111101111111011111110111111101111111011111110"};

        bitset<64> friendly_eligable;
        bitset<64> enemy_eligable;

        bitset<64> friendly_pawns;
        bitset<64> friendly_bishops;
        bitset<64> friendly_rooks;
        bitset<64> friendly_knights;
        bitset<64> friendly_queens;
        bitset<64> friendly_kings;

        bitset<64> enemy_pawns;
        bitset<64> enemy_bishops;
        bitset<64> enemy_rooks;
        bitset<64> enemy_knights;
        bitset<64> enemy_queens;
        bitset<64> enemy_kings;

        bitset<64> enemy_attacking;
        bitset<64> enemy_checkers;

        bitset<64> friendly_pieces;
        bitset<64> enemy_pieces;
        bitset<64> occupied;
};

#endif