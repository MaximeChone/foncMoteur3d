//
// Created by Maxime Choné on 07/01/2021.
//

#include "../lib/tgaimage.h"
#include "../model/Vertex.h"

using namespace std;


#ifndef FONCMO3D_DRAW_H
#define FONCMO3D_DRAW_H

#endif //FONCMO3D_DRAW_H


void line(Vertex v0,Vertex v1,TGAImage &image,TGAColor color);
void triangle(Vertex v0,Vertex v1,Vertex v2,TGAImage &image,TGAColor color);