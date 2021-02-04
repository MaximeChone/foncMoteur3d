//
// Created by Maxime Choné on 07/01/2021.
//

#include "../lib/tgaimage.h"
#include "../model/Model.h"

using namespace std;


#ifndef FONCMO3D_DRAW_H
#define FONCMO3D_DRAW_H


void line(vector2i v0, vector2i v1, TGAImage &image, TGAColor color);
void triangleOldSchool(vector2i v0, vector2i v1, vector2i v2, TGAImage &image, TGAColor color);
void triangle(vector3d *points, vector2d *pointsTexture,TGAImage &image, TGAImage &texture,double zbuffer[],float intensity);
vector3d barycentric(vector3d *points,vector2i p);
void rasterize(vector2i p0, vector2i p1, TGAImage &image, TGAColor color, int ybuffer[]);

#endif //FONCMO3D_DRAW_H


