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

using namespace std;


class Model {
private:
    vector<vector3d> vertices;
    vector<array<int,3>> faces;


public:
    Model(const std::string filename);
    int nvertices() const;
    int nfaces() const;
    vector3d vertex(const int i) const;
    array<int,3> face(const int i) const;
};


#endif //FONCMO3D_MODEL_H
