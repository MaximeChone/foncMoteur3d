//
// Created by Maxime Choné on 13/01/2021.
//

#ifndef FONCMO3D_MODEL_H
#define FONCMO3D_MODEL_H

#include <fstream>
#include <array>
#include <sstream>
#include <iostream>
#include <vector>
#include "Vectors.h"
#include "../lib/tgaimage.h"

using namespace std;


class Model {
private:
    vector<vector3d> vertices;
    vector<array<int,3>> facesVertices;
    vector<vector2d> uvs;
    vector<array<int,3>> facesUvs;
    vector<vector3d> vns;
    vector<array<int,3>> facesVns;


public:
    Model(const std::string filename,const std::string textureFileName,const std::string normalmappingFileName,const int w,const int h);
    int nvertices() const;
    void setImage(TGAImage image);
    int nfaces() const;
    vector3d vertex(const int i) const;
    array<int,3> getVerticesFace(const int i) const;
    vector2d getVt(const int i) const;
    array<int,3> getFaceVt(const int i) const;
    vector3d getVn(const int i) const;
    array<int,3> getFaceVn(const int face) const;
    void saveImage(string name);
    TGAImage image;
    TGAImage texture;
    TGAImage normalmapping;
};


#endif //FONCMO3D_MODEL_H
