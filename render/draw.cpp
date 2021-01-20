//
// Created by Maxime Choné on 07/01/2021.
//

#include <array>
#include "draw.h"
#include <iostream>



void line(Vertex v0,Vertex v1,TGAImage &image,TGAColor color){

    int x0 = v0.getX();
    int y0 = v0.getY();

    int x1 = v1.getX();
    int y1 = v1.getY();

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

void triangle(Vertex v0,Vertex v1,Vertex v2,TGAImage &image,TGAColor color){
    if(v0.getY()==v1.getY() && v0.getY() == v2.getY()) return;
    if(v0.getY() > v1.getY()) swap(v0,v1);
    if(v0.getY() > v2.getY()) swap(v0,v2);
    if(v1.getY() > v2.getY()) swap(v1,v2);

    int total_height = v2.getY() - v0.getY();
    int segment_height = v1.getY() - v0.getY() + 1;

    for(int y=v0.getY(); y <= v1.getY();y++){
        float alpha = (float)(y-v0.getY())/total_height;
        float beta = (float)(y-v0.getY())/segment_height;

        int xA = v0.getX() + (v2.getX() - v0.getX())* alpha;
        int yA = v0.getY() + (v2.getY() - v0.getY())* alpha;

        int xB = v0.getX() + (v1.getX() - v0.getX())* beta;
        int yB = v0.getY() + (v1.getY() - v0.getY())* beta;

        Vertex A = {xA,yA,0};
        Vertex B = {xB,yB,0};

        if (A.getX()>B.getX()) std::swap(A, B);
        for (int j=A.getX(); j<=B.getX(); j++) {
            image.set(j, y, color); // attention, due to int casts t0.y+i != A.y
        }
    }

    segment_height = v2.getY() - v1.getY() + 1;


    for(int y=v1.getY(); y <= v2.getY();y++){
        float alpha = (float)(y-v0.getY())/total_height;
        float beta = (float)(y-v1.getY())/segment_height;

        int xA = v0.getX() + (v2.getX() - v0.getX())* alpha;
        int yA = v0.getY() + (v2.getY() - v0.getY())* alpha;

        int xB = v1.getX() + (v2.getX() - v1.getX())* beta;
        int yB = v1.getY() + (v2.getY() - v1.getY())* beta;

        Vertex A = {xA,yA,0};
        Vertex B = {xB,yB,0};

        if (A.getX()>B.getX()) std::swap(A, B);
        for (int j=A.getX(); j<=B.getX(); j++) {
            image.set(j, y, color); // attention, due to int casts t0.y+i != A.y
        }
    }

}