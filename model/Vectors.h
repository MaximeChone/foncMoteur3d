//
// Created by Maxime Choné on 20/01/2021.
//

#ifndef FONCMO3D_VECTORS_H
#define FONCMO3D_VECTORS_H

#include <ostream>
#include <vector>

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

///Struct by Dmitry Sokolov
//////////////////////////////////////////////////////////////////////////////////////////////

const int DEFAULT_ALLOC=4;

class Matrix {
    vector<vector<float> > m;
    int rows, cols;
public:
    Matrix(int r=DEFAULT_ALLOC, int c=DEFAULT_ALLOC);
    inline int nrows();
    inline int ncols();

    static Matrix identity(int dimensions);
    vector<float>& operator[](const int i);
    Matrix operator*(const Matrix& a);
    Matrix transpose();
    Matrix inverse();

    friend ostream& operator<<(ostream& s, Matrix& m);
};

vector3d m2v(Matrix m);

Matrix v2m(vector3d v);



/////////////////////////////////////////////////////////////////////////////////////////////

#endif //FONCMO3D_VECTORS_H
