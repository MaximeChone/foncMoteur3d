//
// Created by Maxime Choné on 20/01/2021.
//

#ifndef FONCMO3D_VECTORS_H
#define FONCMO3D_VECTORS_H

using namespace std;

struct vector2i{
    vector2i() {

    }
    int x;
    int y;
    vector2i(int x, int y){
        this->x = x;
        this->y = y;
    }
};

struct vector2d{
    vector2d() {

    }
    double x;
    double y;
    vector2d(double x, double y){
        this->x = x;
        this->y = y;
    }
};

struct vector3d{
    vector3d() {

    }

    double x;
    double y;
    double z;
    vector3d(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
    }
    void normalize();
    double norm();
};


double dotProduct(const vector3d &v0, const vector3d &v1);
vector3d crossProduct(const vector3d &v0, const vector3d &v1);

#endif //FONCMO3D_VECTORS_H
