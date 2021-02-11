//
// Created by Maxime Choné on 21/01/2021.
//

#include "renderer.h"
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


void render(Model &model,double zbuffer[]){

    Matrix proj = Matrix::identity(4);
    proj[3][2] = -1.0/camera.z;

    Matrix viewPort = viewport(0,0,model.image.get_width(),model.image.get_height());


    for (int i = 0; i < model.nfaces();i++)
    {

        //on récupère les sommets de la getVerticesFace i et on fait la projection par rapport à la caméra
        vector3d v0 = model.vertex(model.getVerticesFace(i)[0]);


        Matrix modelView = lookat(camera,center,{0,1,0});
        Matrix vM0 = proj * modelView * v2m(v0);
        v0 = m2v(vM0);

        //v0 = projection(v0,proj);
        vector3d v1 = model.vertex(model.getVerticesFace(i)[1]);
        Matrix vM1 = proj * modelView * v2m(v1);
        v1 = m2v(vM1);

        //v1 = projection(v1,proj);
        vector3d v2 = model.vertex(model.getVerticesFace(i)[2]);
        Matrix vM2 = proj * modelView * v2m(v2);
        v2 = m2v(vM2);


        //v2 = projection(v2,proj);

        //on récupère les vt du triangle
        vector2d v0Tex = model.getTextureVertices(model.getTextureVerticesFace(i)[0]);
        vector2d v1Tex = model.getTextureVertices(model.getTextureVerticesFace(i)[1]);
        vector2d v2Tex = model.getTextureVertices(model.getTextureVerticesFace(i)[2]);

        vector2d pointsTex[3] = {v0Tex,v1Tex,v2Tex};

        //cout << i <<" coucou : "<<model.getTextureVerticesFace(i)[0] <<" coucou2:" << model.getTextureVerticesFace(i)[1] << " coucou3:" << model.getTextureVerticesFace(i)[2] << "\n";
        //cout << i <<" coucou : "<<pointsTex[0].x <<" coucou2:" << pointsTex[0].y  << "\n";

        //on transforme les sommets en position de l'écran
        //vector3d v0_trans = {((v0.x + 1) * model.image.get_width() / 2), ((v0.y + 1) * model.image.get_height() / 2),v0.z};
        vector3d v0_trans = m2v(viewPort * v2m(v0));
        //vector3d v1_trans = {((v1.x + 1) * model.image.get_width() / 2), ((v1.y + 1) * model.image.get_height() / 2),v1.z};
        vector3d v1_trans = m2v(viewPort * v2m(v1));
        //vector3d v2_trans = {((v2.x + 1) * model.image.get_width() / 2), ((v2.y + 1) * model.image.get_height() / 2),v2.z};
        vector3d v2_trans = m2v(viewPort * v2m(v2));

        vector3d points[3] = {v0_trans, v1_trans, v2_trans};

        //on récupère 
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

        float intensity = dotProduct(light, crossProd);

        if (intensity > 0) {
            //on dessine le triangle
            triangle(points, pointsTex,model.image,model.texture,zbuffer,intensity);
        }
    }
}

vector3d projection( vector3d vec, Matrix m){
    return m2v((m * v2m(vec)));
}

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

