//
// Created by Maxime Choné on 13/01/2021.
//

#ifndef FONCMO3D_MODEL_H
#define FONCMO3D_MODEL_H


#include <vector>

using namespace std;


class Model {
private:
    vector<array<double,3>> vertices;
    vector<array<int,3>> faces;


public:
    Model(const std::string filename);
    int nvertices() const;
    int nfaces() const;
    array<double,3> vertex(const int i) const;
    array<int,3> face(const int i) const;
};


#endif //FONCMO3D_MODEL_H
