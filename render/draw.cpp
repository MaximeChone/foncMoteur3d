//
// Created by Maxime Choné on 07/01/2021.
//

#include "draw.h"


void line(int x0,int y0,int x1,int y1,TGAImage &image,TGAColor color){
    bool steep = false;
    //if the line is steep, we transpose the image
    if(std::abs(x1-x0)<std::abs(y1-y0)){
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }

    if(x0>x1){ //make it left-to-right
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1-x0;
    int dy = y1-y0;

    int derror3 = 2*std::abs(dy);
    int error3 = 0;

    int y = y0;
    for(int x = x0; x < x1 ; x++){
        if(steep){ //if transposed, de-transpose
            image.set(y, x, color);
        }
        else{
            image.set(x, y, color);
        }
        error3 += derror3;
        if(error3>dx){
            y+= (dy > 0 ? 1 : -1);
            error3 -=2*dx;
        }
    }
}