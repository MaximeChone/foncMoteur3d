//
// Created by Maxime Choné on 20/01/2021.
//

#include "Vertex.h"

int Vertex::getX() const {
    return x;
}

void Vertex::setX(int x) {
    Vertex::x = x;
}

int Vertex::getY() const {
    return y;
}

void Vertex::setY(int y) {
    Vertex::y = y;
}

int Vertex::getZ() const {
    return z;
}

void Vertex::setZ(int z) {
    Vertex::z = z;
}

Vertex::Vertex(int x, int y, int z) {
    this->x = x;
    this->y = y;
    this->z = z;
}
