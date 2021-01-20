//
// Created by Maxime Choné on 20/01/2021.
//

#ifndef FONCMO3D_VERTEX_H
#define FONCMO3D_VERTEX_H

class Vertex {
private:
    int x;
    int y;
    int z;
public:
    Vertex(int x, int y, int z);
    int getX() const;
    void setX(int x);
    int getY() const;
    void setY(int y);
    int getZ() const;
    void setZ(int z);
};

#endif //FONCMO3D_VERTEX_H
