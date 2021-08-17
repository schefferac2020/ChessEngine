#ifndef BIT_OPS_HPP
#define BIT_OPS_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <bitset>

#include "Utils.hpp"

using namespace std;


enum class RayDirection : int{
    North = 0, Northeast, East, Southeast, South, Southwest, West, Northwest
};

class BitOps {
    public:

        bitset<64> rays[8][64]; //ray[Direction][Location]
        bitset<64> trivial[64];

        BitOps() {
            createNorthRays();
            createEastRays();
            createSouthRays();
            createWestRays();

            createNorteastRays();
            createNorthwestRays();
            createSoutheastRays();
            createSouthwestRays();

            createTrivial();
        }

        void printRay(RayDirection direction, int position){
            bitset<64> selected = rays[(int)direction][position];
            for (int i = 0; i< 64; ++i){
                if (i % 8 == 0){
                    cout << '\n';
                }
                if (i == position /*&& false*/)
                    cout << "X "; 
                else
                    cout << selected[i] << ' ';
                
            }
            cout << endl;
        }

        //Returns the direction bitmap from the given integer position
        bitset<64>& getDirectionBitmap(RayDirection direction, int position){
            return rays[(int)direction][position];
        }
        
    private:
        void createTrivial(){
            for (int i = 0; i < 64; ++i){
                bitset<64> curr;
                curr[i] = 1;
                trivial[i] = curr;
            }
        }


        //STATUS: TESTED
        void createNorteastRays(){
            for (int i = 0; i < 64; ++i){
                bitset<64> curr;
                int from_col = i % 8;


                int val = -7;
                
                int current_num = i + val;
                int current_col = (from_col + 1)%8;
                while (current_num >= 0 && current_col > from_col){
                    
                    curr[current_num] = 1;
                    current_num += val;
                    current_col = (current_col + 1)%8;
                    
                }

                rays[(int)RayDirection::Northeast][i] = curr;
            }
        }

        //STATUS: TESTED
        void createNorthwestRays(){
            for (int i = 0; i < 64; ++i){
                bitset<64> curr;
                int from_col = i % 8;


                int val = -9;
                
                int current_num = i + val;
                int current_col = (from_col - 1 + 8)%8;
                while (current_num >= 0 &&  from_col > current_col){
                    
                    curr[current_num] = 1;
                    current_num += val;
                    current_col = (current_col - 1 + 8)%8;
                    
                }

                rays[(int)RayDirection::Northwest][i] = curr;
            }
        }

        //STATUS: TESTED
        void createSoutheastRays(){
            for (int i = 0; i < 64; ++i){
                bitset<64> curr;
                int from_col = i % 8;

                int val = 9;
                
                int current_num = i + val;
                int current_col = (from_col + 1)%8;
                while (current_num < 64 && current_col > from_col){
                    
                    curr[current_num] = 1;
                    current_num += val;
                    current_col = (current_col + 1)%8;
                }

                rays[(int)RayDirection::Southeast][i] = curr;
            }
        }
        
        //STATUS: TESTED
        void createSouthwestRays(){
            for (int i = 0; i < 64; ++i){
                bitset<64> curr;
                int from_col = i % 8;

                int val = 7;
                
                int current_num = i + val;
                int current_col = (from_col -1 + 8)%8;
                while (current_num < 64 && from_col > current_col){
                    
                    curr[current_num] = 1;
                    current_num += val;
                    current_col = (current_col - 1 + 8)%8;
                }

                rays[(int)RayDirection::Southwest][i] = curr;
            }
        }
        
        
        
        //STATUS: TESTED
        void createNorthRays(){
            for (int i = 0; i < 64; ++i){
                bitset<64> curr;
                int val = -8;
                int current_num = i + val;
                while (current_num >= 0){
                    curr[current_num] = 1;
                    current_num += val;
                }

                rays[(int)RayDirection::North][i] = curr;
            }
        }

        //STATUS: TESTED
        void createSouthRays(){
            for (int i = 0; i < 64; ++i){
                bitset<64> curr;
                int val = 8;
                int current_num = i + val;
                while (current_num < 64){
                    curr[current_num] = 1;
                    current_num += val;
                }

                rays[(int)RayDirection::South][i] = curr;
            }
        }

        //STATUS: TESTED
        void createEastRays(){
            for (int i = 0; i < 64; ++i){
                bitset<64> curr;
                int val = 1;
                int current_num = i + val;
                while (current_num % 8 != 0){
                    curr[current_num] = 1;
                    current_num += val;
                }

                rays[(int)RayDirection::East][i] = curr;
            }
        }

        //STATUS: TESTED
        void createWestRays(){
            for (int i = 0; i < 64; ++i){
                bitset<64> curr;
                if (i % 8 == 0){
                    rays[(int)RayDirection::West][i] = curr;
                    continue;
                }

                int val = -1;
                int current_num = i + val;
                while (current_num % 8 != 0){
                    curr[current_num] = 1;
                    current_num += val;
                }
                curr[current_num] = 1;

                rays[(int)RayDirection::West][i] = curr;
            }
        }

};

#endif