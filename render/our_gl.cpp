//
// Created by Maxime Choné on 21/01/2021.
//

#include "our_gl.h"

Matrix lookat(vector3d eye, vector3d center, vector3d up) {
    vector3d z = vector3d((eye.x-center.x),(eye.y-center.y),(eye.z-center.z));
    z.normalize();
    vector3d x = crossProduct(up,z);
    x.normalize();
    vector3d y = crossProduct(z,x);
    y.normalize();
    Matrix m = Matrix::identity(4);

    m[0][0] = x.x;
    m[1][0] = y.x;
    m[2][0] = z.x;

    m[0][1] = x.y;
    m[1][1] = y.y;
    m[2][1] = z.y;

    m[0][2] = x.z;
    m[1][2] = y.z;
    m[2][2] = z.z;

    m[0][3] = -center.x;
    m[1][3] = -center.y;
    m[2][3] = -center.z;

    return m;
}

Matrix viewport(int x,int y,int w, int h){
    Matrix viewport = Matrix::identity(4);

    viewport[0][3] = x + w/2.0;
    viewport[1][3] = y + h/2.0;
    viewport[2][3] = +depth/2.0;

    viewport[0][0] = w/2.0;
    viewport[1][1] = h/2.0;
    viewport[2][2] = +depth/2.0;

    return viewport;

}

vector3d barycentric(vector3d *points,vector3d p){
    vector3d v1 = vector3d(points[2].x - points[0].x,points[1].x - points[0].x,points[0].x - p.x);
    vector3d v2 = vector3d(points[2].y - points[0].y,points[1].y - points[0].y,points[0].y - p.y);
    vector3d u = crossProduct(v1,v2);

    if (abs(u.z)<1) return vector3d(-1,1,1);

    return {1 - (u.x + u.y)/u.z, u.y/u.z, u.x/u.z};
}

void triangle(vector4d *points, IShader &shader, TGAImage &image, vector<double> &zbuffer){

    vector2d bboxmin(image.get_width()-1, image.get_height()-1);
    vector2d bboxmax(0,0);
    vector2d clamp(image.get_width()-1, image.get_height()-1);

    //border min
    bboxmin.x = max(0.,min(bboxmin.x,min(points[0].x,min(points[1].x,points[2].x))));
    bboxmin.y = max(0.,min(bboxmin.y,min(points[0].y,min(points[1].y,points[2].y))));

    //border max
    bboxmax.x = min(clamp.x,max(bboxmax.x,max(points[0].x,max(points[1].x,points[2].x))));
    bboxmax.y = min(clamp.y,max(bboxmax.y,max(points[0].y,max(points[1].y,points[2].y))));


    vector3d p;
    for(int i=bboxmin.x; i <= bboxmax.x; i++){

        for(int j= bboxmin.y; j <= bboxmax.y; j++){

            p.x = i;
            p.y = j;

            vector3d points3d[3];
            for(int a = 0; a < 3;a++){
                points3d[a] = v4tov3(points[a]);
            }



            vector3d barycenter = barycentric(points3d, p);

            p.z = 0;
            p.z += points[0].z * barycenter.x;
            p.z += points[1].z * barycenter.y;
            p.z += points[2].z * barycenter.z;

            if((barycenter.x < 0 || barycenter.y < 0 || barycenter.z < 0)) continue;

            TGAColor color;

            bool discard = shader.fragment(barycenter,color);

            if(!discard){
                if(zbuffer[i + j * image.get_width()] < p.z) {
                    image.set(p.x, p.y, color);
                    zbuffer[i + j * image.get_width()] = p.z;
                }
            }
        }
    }
}

