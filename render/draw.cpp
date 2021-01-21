//
// Created by Maxime Choné on 07/01/2021.
//

#include <array>
#include "draw.h"
#include <iostream>


void line(vector2i v0, vector2i v1, TGAImage &image, TGAColor color){

    int x0 = v0.x;
    int y0 = v0.y;

    int x1 = v1.x;
    int y1 = v1.y;

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

vector3d barycentric(vector2i *points,vector2i p){
    vector3d v1 = vector3d(points[2].x - points[0].x,points[1].x - points[0].x,points[0].x - p.x);
    vector3d v2 = vector3d(points[2].y - points[0].y,points[1].y - points[0].y,points[0].y - p.y);
    vector3d u = crossProduct(v1,v2);

    if (abs(u.z)<1) return vector3d(-1,1,1);

    return {1 - (u.x + u.y)/u.z, u.y/u.z, u.x/u.z};
}

void triangle(vector2i *points, TGAImage &image, TGAColor color){
    vector2i bboxmin(image.get_width()-1, image.get_height()-1);
    vector2i bboxmax(0,0);
    vector2i clamp(image.get_width()-1, image.get_height()-1);

    //border min
    bboxmin.x = max(0,min(bboxmin.x,min(points[0].x,min(points[1].x,points[2].x))));
    bboxmin.y = max(0,min(bboxmin.y,min(points[0].y,min(points[1].y,points[2].y))));

    //border max
    bboxmax.x = min(clamp.x,max(bboxmax.x,max(points[0].x,max(points[1].x,points[2].x))));
    bboxmax.y = min(clamp.y,max(bboxmax.y,max(points[0].y,max(points[1].y,points[2].y))));


    vector2i p;
    for(int i=bboxmin.x; i <= bboxmax.x; i++){

        for(int j= bboxmin.y; j <= bboxmax.y; j++){

            p.x = i;
            p.y = j;
            vector3d barycenter = barycentric(points,p);
            if(!(barycenter.x < 0 || barycenter.y < 0 || barycenter.z < 0)){
                image.set(p.x,p.y,color);
            }
        }
    }

}

void triangleOldSchool(vector2i v0, vector2i v1, vector2i v2, TGAImage &image, TGAColor color){
    if(v0.y==v1.y && v0.y == v2.y) return;
    if(v0.y > v1.y) swap(v0,v1);
    if(v0.y > v2.y) swap(v0,v2);
    if(v1.y > v2.y) swap(v1,v2);

    int total_height = v2.y - v0.y;
    int segment_height = v1.y - v0.y + 1;

    for(int y=v0.y; y <= v1.y;y++){
        float alpha = (float)(y-v0.y)/total_height;
        float beta = (float)(y-v0.y)/segment_height;

        int xA = v0.x + (v2.x - v0.x)* alpha;
        int yA = v0.y + (v2.y - v0.y)* alpha;

        int xB = v0.x + (v1.x - v0.x)* beta;
        int yB = v0.y + (v1.y - v0.y)* beta;

        vector2i A = {xA, yA};
        vector2i B = {xB, yB};

        if (A.x>B.x) std::swap(A, B);
        for (int j=A.x; j<=B.x; j++) {
            image.set(j, y, color); // attention, due to int casts t0.y+i != A.y
        }
    }

    segment_height = v2.y - v1.y + 1;


    for(int y=v1.y; y <= v2.y;y++){
        float alpha = (float)(y-v0.y)/total_height;
        float beta = (float)(y-v1.y)/segment_height;

        int xA = v0.x + (v2.x - v0.x)* alpha;
        int yA = v0.y + (v2.y - v0.y)* alpha;

        int xB = v1.x + (v2.x - v1.x)* beta;
        int yB = v1.y + (v2.y - v1.y)* beta;

        vector2i A = {xA, yA};
        vector2i B = {xB, yB};

        if (A.x>B.x) std::swap(A, B);
        for (int j=A.x; j<=B.x; j++) {
            image.set(j, y, color); // attention, due to int casts t0.y+i != A.y
        }
    }

}