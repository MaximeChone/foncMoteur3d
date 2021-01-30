//
// Created by Maxime Choné on 21/01/2021.
//

#ifndef FONCMO3D_RENDERER_H
#define FONCMO3D_RENDERER_H


#include "../model/Model.h"
#include "draw.h"

const static vector3d light = {0,0,1};


void flatShading( Model &model, TGAImage &image);
void texture( Model &model);

#endif //FONCMO3D_RENDERER_H
