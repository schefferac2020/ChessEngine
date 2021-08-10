#pragma once
#include <iostream>

using namespace std;

struct Vector2f {
    Vector2f(float x_in, float y_in) : x(x_in), y(y_in) { }

    Vector2f() : x(0.0), y(0.0) { }


    void print(){
        cout << '(' << x << ", " << y << ')' << endl;
    } 
    float x, y;
};

struct Vector2i {
    Vector2i(int x_in, int y_in) : x(x_in), y(y_in) { }

    Vector2i() : x(0), y(0) { }


    void print(){
        cout << '(' << x << ", " << y << ')' << endl;
    } 
    int x, y;
};