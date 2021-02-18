//
// Created by Maxime Choné on 21/01/2021.
//

#ifndef FONCMO3D_OUR_GL_H
#define FONCMO3D_OUR_GL_H


#include "../model/Model.h"


const int depth = 255;

struct IShader {
    virtual vector4d vertex(int iface, int nthvert) = 0;
    virtual bool fragment(vector3d bar, TGAColor &color) = 0;
};

Matrix lookat(vector3d eye, vector3d center, vector3d up);
Matrix viewport(int x,int y,int w, int h);
vector3d barycentric(vector3d *points,vector2i p);
void triangle(vector4d *pts, IShader &shader, TGAImage &image, vector<double> &zbuffer);


#endif //FONCMO3D_OUR_GL_H
