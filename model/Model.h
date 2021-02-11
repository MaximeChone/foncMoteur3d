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
    vector<vector2d> textureVertices;
    vector<array<int,3>> facesTextureVertices;

public:
    Model(const std::string filename,const std::string textureFileName);
    int nvertices() const;
    void setImage(TGAImage image);
    int nfaces() const;
    vector3d vertex(const int i) const;
    array<int,3> getVerticesFace(const int i) const;
    vector2d getTextureVertices(const int i) const;
    array<int,3> getTextureVerticesFace(const int i) const;
    void saveImage(string name);
    TGAImage image;
    TGAImage texture;
};


#endif //FONCMO3D_MODEL_H
