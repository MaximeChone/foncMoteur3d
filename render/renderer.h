//
// Created by Maxime Choné on 21/01/2021.
//

#ifndef FONCMO3D_RENDERER_H
#define FONCMO3D_RENDERER_H


#include "../model/Model.h"
#include "draw.h"

const static vector3d light = {0,0,1};


void flatShading(const Model &model, TGAImage &image);
void texture(const Model &model, TGAImage &image,TGAImage &texture);

#endif //FONCMO3D_RENDERER_H
