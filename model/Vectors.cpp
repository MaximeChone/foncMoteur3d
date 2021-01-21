//
// Created by Maxime Choné on 21/01/2021.
//


#include <cmath>
#include "Vectors.h"


vector3d crossProduct(const vector3d &v0,const vector3d &v1){
    return vector3d(v0.y * v1.z - v0.z * v1.y,
                    v0.z * v1.x - v0.x * v1.z,
                    v0.x * v1.y - v0.y * v1.x);
}

double dotProduct(const vector3d &v0, const vector3d &v1){
    return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
}

void vector3d::normalize() {
    this->x = this->x/norm();
    this->y = this->y/norm();
    this->z = this->z/norm();
}

double vector3d::norm() {
    return sqrt(x * x + y * y + z * z);
}
