//
// Created by Maxime Choné on 21/01/2021.
//

#ifndef FONCMO3D_RENDERER_H
#define FONCMO3D_RENDERER_H


#include "../model/Model.h"
#include "draw.h"

const static vector3d light = {0,0,1.25};
const static vector3d camera = {0,0.40,0.80};
const static vector3d center = {0,0,0};
const int depth = 255;

void flatShading( Model &model, TGAImage &image);
void render(Model &model,double zbuffer[]);
vector3d projection( vector3d vec, Matrix m);
Matrix lookat(vector3d eye, vector3d center, vector3d up);
Matrix viewport(int x,int y,int w, int h);

#endif //FONCMO3D_RENDERER_H
