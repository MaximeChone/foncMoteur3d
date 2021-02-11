//
// Created by Maxime Choné on 21/01/2021.
//

#ifndef FONCMO3D_RENDERER_H
#define FONCMO3D_RENDERER_H


#include "../model/Model.h"
#include "draw.h"

const static vector3d light = {0,0,1};
const static vector3d camera = {0,0,3};

void flatShading( Model &model, TGAImage &image);
void render(Model &model);
vector3d projection( vector3d vec, Matrix m);

#endif //FONCMO3D_RENDERER_H
