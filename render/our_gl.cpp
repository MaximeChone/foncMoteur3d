//
// Created by Maxime Choné on 21/01/2021.
//

#include "our_gl.h"
/*
void flatShading(const Model &model, TGAImage &image){


    double zbuffer[image.get_height() * image.get_width()];

    for(int i = 0 ; i < image.get_height() * image.get_width();i++){
        zbuffer[i] = std::numeric_limits<int>::min();
    }

    for (int i = 0; i < model.nfaces();i++) // access by reference to avoid copying
    {


        //on récupère les sommets de la getVerticesFace i
        vector3d v0 = model.vertex(model.getVerticesFace(i)[0]);
        vector3d v1 = model.vertex(model.getVerticesFace(i)[1]);
        vector3d v2 = model.vertex(model.getVerticesFace(i)[2]);

        //on transforme les sommets en position de l'écran
        vector3d v0_trans = {((v0.x + 1) * image.get_width() / 2), ((v0.y + 1) * image.get_height() / 2),v0.z};
        vector3d v1_trans = {((v1.x + 1) * image.get_width() / 2), ((v1.y + 1) * image.get_height() / 2),v1.z};
        vector3d v2_trans = {((v2.x + 1) * image.get_width() / 2), ((v2.y + 1) * image.get_height() / 2),v2.z};

        vector3d points[3] = {v0_trans, v1_trans, v2_trans};

        vector3d AB = {v1.x - v0.x,
                       v1.y - v0.y,
                       v1.z - v0.z};
        vector3d AC = {v2.x - v0.x,
                       v2.y - v0.y,
                       v2.z - v0.z};

        //AB.normalize();
        //AC.normalize();

        vector3d crossProd = crossProduct(AB, AC);

        crossProd.normalize();

        double intensity = dotProduct(light, crossProd);

        if (intensity > 0) {
        //on dessine le triangle
            int in = 200;
            TGAColor color = TGAColor(intensity * in, intensity * in, intensity * in, in);

            //triangle(points, image, color,zbuffer);
        }
    }
}
*/




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
    viewport[2][3] =  + depth/2.0;

    viewport[0][0] = w/2.0;
    viewport[1][1] = h/2.0;
    viewport[2][2] =  + depth/2.0;

    return viewport;

}

void line(vector2i v0, vector2i v1, TGAImage &image, TGAColor color){

    int x0 = v0.x;
    int y0 = v0.y;

    int x1 = v1.x;
    int y1 = v1.y;

    bool steep = false;
    //if the line is steep, we transpose the image
    if(std::abs(x1-x0)<std::abs(y1-y0)){
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }

    if(x0>x1){ //make it left-to-right
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1-x0;
    int dy = y1-y0;

    int derror3 = 2*std::abs(dy);
    int error3 = 0;

    int y = y0;
    for(int x = x0; x < x1 ; x++){
        if(steep){ //if transposed, de-transpose
            image.set(y, x, color);
        }
        else{
            image.set(x, y, color);
        }
        error3 += derror3;
        if(error3>dx){
            y+= (dy > 0 ? 1 : -1);
            error3 -=2*dx;
        }
    }
}

vector3d barycentric(vector3d *points,vector3d p){
    vector3d v1 = vector3d(points[2].x - points[0].x,points[1].x - points[0].x,points[0].x - p.x);
    vector3d v2 = vector3d(points[2].y - points[0].y,points[1].y - points[0].y,points[0].y - p.y);
    vector3d u = crossProduct(v1,v2);

    if (abs(u.z)<1) return vector3d(-1,1,1);

    return {1 - (u.x + u.y)/u.z, u.y/u.z, u.x/u.z};
}

void triangle(vector3d *points, vector2d *pointsTexture,TGAImage &image, TGAImage &texture, double zbuffer[],float intensity){
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

            vector3d barycenter = barycentric(points,p);

            if(!(barycenter.x < 0 || barycenter.y < 0 || barycenter.z < 0)){

                p.z = 0;

                p.z += points[0].z * barycenter.x;
                p.z += points[1].z * barycenter.y;
                p.z += points[2].z * barycenter.z;

                //calcul color
                double u = (barycenter.x * pointsTexture[0].x
                            + barycenter.y * pointsTexture[1].x
                            + barycenter.z * pointsTexture[2].x);

                double v = (barycenter.x * pointsTexture[0].y
                            + barycenter.y * pointsTexture[1].y
                            + barycenter.z * pointsTexture[2].y);


                TGAColor color = texture.get(u * texture.get_width(),v * texture.get_height()) * intensity;


                if(zbuffer[i + j * image.get_width()] < p.z){
                    image.set(p.x,p.y,color);
                    zbuffer[i + j * image.get_width()] = p.z;
                }
            }
        }
    }
}

void triangle(vector4d *points, IShader &shader, TGAImage &image, TGAImage &zbuffer){

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

            vector3d points3[3];
            for(int a = 0; a < 3;a++){
                points3[a] = v4tov3(points[a]);
            }

            vector3d barycenter = barycentric(points3,p);



            TGAColor color;

            bool discard = shader.fragment(barycenter,color);

            if(!discard){
                zbuffer.set(p.x, p.y, TGAColor(p.z));
                image.set(p.x, p.y, color);
            }
        }
    }

}


void triangleOldSchool(vector2i v0, vector2i v1, vector2i v2, TGAImage &image, TGAColor color) {
    if (v0.y == v1.y && v0.y == v2.y) return;
    if (v0.y > v1.y) swap(v0, v1);
    if (v0.y > v2.y) swap(v0, v2);
    if (v1.y > v2.y) swap(v1, v2);

    int total_height = v2.y - v0.y;
    int segment_height = v1.y - v0.y + 1;

    for (int y = v0.y; y <= v1.y; y++) {
        float alpha = (float) (y - v0.y) / total_height;
        float beta = (float) (y - v0.y) / segment_height;

        int xA = v0.x + (v2.x - v0.x) * alpha;
        int yA = v0.y + (v2.y - v0.y) * alpha;

        int xB = v0.x + (v1.x - v0.x) * beta;
        int yB = v0.y + (v1.y - v0.y) * beta;

        vector2i A = {xA, yA};
        vector2i B = {xB, yB};

        if (A.x > B.x) std::swap(A, B);
        for (int j = A.x; j <= B.x; j++) {
            image.set(j, y, color); // attention, due to int casts t0.y+i != A.y
        }
    }

    segment_height = v2.y - v1.y + 1;


    for (int y = v1.y; y <= v2.y; y++) {
        float alpha = (float) (y - v0.y) / total_height;
        float beta = (float) (y - v1.y) / segment_height;

        int xA = v0.x + (v2.x - v0.x) * alpha;
        int yA = v0.y + (v2.y - v0.y) * alpha;

        int xB = v1.x + (v2.x - v1.x) * beta;
        int yB = v1.y + (v2.y - v1.y) * beta;

        vector2i A = {xA, yA};
        vector2i B = {xB, yB};

        if (A.x > B.x) std::swap(A, B);
        for (int j = A.x; j <= B.x; j++) {
            image.set(j, y, color); // attention, due to int casts t0.y+i != A.y
        }
    }

}

void rasterize(vector2i p0, vector2i p1, TGAImage &image, TGAColor color, int ybuffer[]) {
    int y;
    float t;
    if(p0.x > p1.x)
        swap(p0,p1);

    for (int x = p0.x; x <= p1.x; x++) {
        t = (x - p0.x) / (float)(p1.x - p0.x);
        y = (p0.y * (1. - t) + p1.y * t);
        if (ybuffer[x] < y) {
            image.set(x,0,color);
            ybuffer[x] = y;
        }
    }
}

