#ifndef UTILS_HPP
#define UTILS_HPP
#include <SDL2/SDL.h>


struct Color{
    int r, g, b, a;
};

struct Position{
    int row, col;
};

enum class PieceType {
        WhitePawn=0, WhiteKnight, WhiteBishop, WhiteQueen, WhiteKing, WhiteRook, BlackPawn, BlackKnight, BlackBishop, BlackQueen, BlackKing, BlackRook, Nothing
};

namespace utils {
    inline float hireTimeInSeconds() {
        float t = SDL_GetTicks();
        t *= 0.001f;

        return t;
    }


    


    //Do everthing with bit operations....
    // uint8_t WhitePawn   = 1; // 00000001;
    // uint8_t WhiteKnight = 2; //00000010;
    // uint8_t WhiteBishop = 4; //00000100;
    // uint8_t WhiteQueen  = 8; //00001000;
    // uint8_t WhiteKing   = 16; //00010000;
    // uint8_t WhiteRook   = 32; //00100000;

    // uint8_t BlackPawn   = 129; // 10000001;
    // uint8_t BlackKnight = 130; // 10000010;
    // uint8_t BlackBishop = 132; // 10000100;
    // uint8_t BlackQueen  = 136; // 10001000;
    // uint8_t BlackKing   = 144; // 10010000;
    // uint8_t BlackRook   = 160; //10100000;

    // inline bool isWhitePiece(uint8_t piece){
    //     return (piece & (uint8_t)128) == 0; // if it is black it should be 128
    // }
    
    // inline bool isKnight(uint8_t piece){
    //     return (piece & (uint8_t)2) == 2;
    // }

    // inline bool isBishop(uint8_t piece){
    //     return (piece & (uint8_t)4) == 4;
    // }
    
    // inline bool isQueen(uint8_t piece){
    //     return (piece & (uint8_t)8) == 8;
    // }

    // inline bool isKing(uint8_t piece){
    //     return (piece & (uint8_t)16) == 16;
    // }

    // inline bool isRook(uint8_t piece){
    //     return (piece & (uint8_t)32) == 32;
    // }


}
#endif