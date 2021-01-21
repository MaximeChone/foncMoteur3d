//
// Created by Maxime Choné on 07/01/2021.
//

#include "../lib/tgaimage.h"
#include "../model/Vectors.h"

using namespace std;


#ifndef FONCMO3D_DRAW_H
#define FONCMO3D_DRAW_H


void line(vector2i v0, vector2i v1, TGAImage &image, TGAColor color);
void triangleOldSchool(vector2i v0, vector2i v1, vector2i v2, TGAImage &image, TGAColor color);
void triangle(vector2i *points, TGAImage &image, TGAColor color);
vector3d barycentric(vector2i *points,vector2i p);

#endif //FONCMO3D_DRAW_H


