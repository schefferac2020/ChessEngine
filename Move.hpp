#ifndef MOVE_HPP
#define MOVE_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <sstream>
#include "Utils.hpp"

struct Move {
        Move() { }
        Move(Position in_from, Position in_to, PieceType in_type) : from(in_from), to(in_to), piece_type(in_type) { }

        
        string to_string(){
                stringstream os;
                os << "From: " << from.row << " " << from.col << endl;
                os << "To: " << to.row << " " << to.col << endl;
                
                return os.str();
        }

        Position from;
        Position to;
        PieceType piece_type;
};

ostream& operator<<(ostream& os, Move const& rhs){
        
        return os;
}

#endif