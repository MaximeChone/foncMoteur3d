//
// Created by Maxime Choné on 21/01/2021.
//

#ifndef FONCMO3D_OUR_GL_H
#define FONCMO3D_OUR_GL_H


#include "../model/Model.h"

const static vector3d light = {0,0,1};
const static vector3d camera = {1,1,3};
const static vector3d center = {0,0,0};

const int depth = 255;

void flatShading( Model &model, TGAImage &image);
void render(Model &model,double zbuffer[]);
vector3d projection( vector3d vec, Matrix m);
Matrix lookat(vector3d eye, vector3d center, vector3d up);
Matrix viewport(int x,int y,int w, int h);
void line(vector2i v0, vector2i v1, TGAImage &image, TGAColor color);
void triangleOldSchool(vector2i v0, vector2i v1, vector2i v2, TGAImage &image, TGAColor color);
void triangle(vector3d *points, vector2d *pointsTexture,TGAImage &image, TGAImage &texture,double zbuffer[],float intensity);
vector3d barycentric(vector3d *points,vector2i p);
void rasterize(vector2i p0, vector2i p1, TGAImage &image, TGAColor color, int ybuffer[]);

struct IShader {
    virtual ~IShader();
    virtual vector3d vertex(int iface, int nthvert) = 0;
    virtual bool fragment(vector3d bar, TGAColor &color) = 0;
};

#endif //FONCMO3D_OUR_GL_H
