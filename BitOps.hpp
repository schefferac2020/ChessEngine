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
        bitset<64> knights[64];
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

            createKnightBitboards();
            createTrivialBitboards();

            
        }

        //Returns the direction bitmap from the given integer position
        bitset<64>& getDirectionBitmap(RayDirection direction, int position){
            return rays[(int)direction][position];
        }
        
    private:
        string base = "0000000000000000000000000000000000000000000000000000000000000000";
        void createKnightBitboards(){
            vector<int> mods = {-10, 6, 15, 17, 10, -6, -15, -17};
            
            
            for (int i = 0; i < 64; ++i){
                string str = base;

                int curr_row =  (i - (i % 8))/8;
                int curr_col = i % 8;
                
                for (int mod: mods){
                    int new_spot = i + mod;
                    int new_row =  (new_spot - (new_spot % 8))/8;
                    int new_col = new_spot % 8;

                    int rows_diff = new_row - curr_row;
                    int cols_diff = new_col - curr_col;

                    if (rows_diff*rows_diff + cols_diff*cols_diff == 5 && new_spot < 64 && new_spot >= 0){
                        str[new_spot] = '1';
                    }

                }

                bitset<64> curr(str);
                knights[i] = curr;
            }
        }


        void createTrivialBitboards(){
            for (int i = 0; i < 64; ++i){
                bitset<64> curr;
                curr[i] = 1;
                trivial[i] = curr;
            }
        }


        //STATUS: NOT TESTED
        void createNorteastRays(){
            for (int i = 0; i < 64; ++i){
                string str = base;
                int from_col = i % 8;


                int val = -7;
                
                int current_num = i + val;
                int current_col = (from_col + 1)%8;
                while (current_num >= 0 && current_col > from_col){
                    
                    str[current_num] = '1';
                    current_num += val;
                    current_col = (current_col + 1)%8;
                    
                }
                bitset<64> curr(str);
                rays[(int)RayDirection::Northeast][i] = curr;
            }
        }

        //STATUS: NOT TESTED
        void createNorthwestRays(){
            for (int i = 0; i < 64; ++i){
                string str = base;
                int from_col = i % 8;


                int val = -9;
                
                int current_num = i + val;
                int current_col = (from_col - 1 + 8)%8;
                while (current_num >= 0 &&  from_col > current_col){
                    
                    str[current_num] = '1';
                    current_num += val;
                    current_col = (current_col - 1 + 8)%8;
                    
                }

                bitset<64> curr(str);
                rays[(int)RayDirection::Northwest][i] = curr;
            }
        }

        //STATUS: NOT TESTED
        void createSoutheastRays(){
            for (int i = 0; i < 64; ++i){
                string str = base;
                int from_col = i % 8;

                int val = 9;
                
                int current_num = i + val;
                int current_col = (from_col + 1)%8;
                while (current_num < 64 && current_col > from_col){
                    
                    str[current_num] = '1';
                    current_num += val;
                    current_col = (current_col + 1)%8;
                }
                bitset<64> curr(str);
                rays[(int)RayDirection::Southeast][i] = curr;
            }
        }
        
        //STATUS: NOT TESTED
        void createSouthwestRays(){
            for (int i = 0; i < 64; ++i){
                string str = base;
                int from_col = i % 8;

                int val = 7;
                
                int current_num = i + val;
                int current_col = (from_col -1 + 8)%8;
                while (current_num < 64 && from_col > current_col){
                    
                    str[current_num] = '1';
                    current_num += val;
                    current_col = (current_col - 1 + 8)%8;
                }

                bitset<64> curr(str);
                rays[(int)RayDirection::Southwest][i] = curr;
            }
        }
        
        
        
        //STATUS: TESTED
        void createNorthRays(){
            for (int i = 0; i < 64; ++i){
                string str = base;
                int val = -8;
                int current_num = i + val;
                while (current_num >= 0){
                    str[current_num] = '1';
                    current_num += val;
                }
                
                
                bitset<64> curr(str);
                rays[(int)RayDirection::North][i] = curr;
            }
        }

        //STATUS: TESTED
        void createSouthRays(){
            for (int i = 0; i < 64; ++i){
                string str = base;
                int val = 8;
                int current_num = i + val;
                while (current_num < 64){
                    str[current_num] = '1';
                    current_num += val;
                }
                bitset<64> curr(str);
                rays[(int)RayDirection::South][i] = curr;
            }
        }

        //STATUS: TESTED
        void createEastRays(){
            for (int i = 0; i < 64; ++i){
                string str = base;
                int val = 1;
                int current_num = i + val;
                while (current_num % 8 != 0){
                    str[current_num] = '1';
                    current_num += val;
                }
                bitset<64> curr(str);
                rays[(int)RayDirection::East][i] = curr;
            }
        }

        //STATUS: TESTED
        void createWestRays(){
            for (int i = 0; i < 64; ++i){
                string str = base;
                if (i % 8 == 0){
                    continue;
                }

                int val = -1;
                int current_num = i + val;
                while (current_num % 8 != 0){
                    str[current_num] = '1';
                    current_num += val;
                }
                str[current_num] = '1';
                bitset<64> curr(str);
                rays[(int)RayDirection::West][i] = curr;
            }
        }

};

#endif