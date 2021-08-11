#ifndef MOVE_HPP
#define MOVE_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Utils.hpp"

struct Move {
        Move() { }
        Move(Position in_from, Position in_to, PieceType in_type) : from(in_from), to(in_to), piece_type(in_type) { }

        Position from;
        Position to;
        PieceType piece_type;
};

#endif