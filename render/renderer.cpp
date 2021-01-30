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
void texture( Model &model){
    double zbuffer[model.image.get_height() * model.image.get_width()];

    for(int i = 0 ; i < model.image.get_height() * model.image.get_width();i++){
        zbuffer[i] = std::numeric_limits<int>::min();
    }

    for (int i = 0; i < model.nfaces();i++) // access by reference to avoid copying
    {

        //on récupère les sommets de la getVerticesFace i
        vector3d v0 = model.vertex(model.getVerticesFace(i)[0]);
        vector3d v1 = model.vertex(model.getVerticesFace(i)[1]);
        vector3d v2 = model.vertex(model.getVerticesFace(i)[2]);

        //on récupère les vt du triangle
        vector2d v0Tex = model.getTextureVertices(model.getTextureVerticesFace(i)[0]);
        vector2d v1Tex = model.getTextureVertices(model.getTextureVerticesFace(i)[1]);
        vector2d v2Tex = model.getTextureVertices(model.getTextureVerticesFace(i)[2]);

        vector2d pointsTex[3] = {v0Tex,v1Tex,v2Tex};

        //cout << i <<" coucou : "<<model.getTextureVerticesFace(i)[0] <<" coucou2:" << model.getTextureVerticesFace(i)[1] << " coucou3:" << model.getTextureVerticesFace(i)[2] << "\n";
        //cout << i <<" coucou : "<<pointsTex[0].x <<" coucou2:" << pointsTex[0].y  << "\n";

        //on transforme les sommets en position de l'écran
        vector3d v0_trans = {((v0.x + 1) * model.image.get_width() / 2), ((v0.y + 1) * model.image.get_height() / 2),v0.z};
        vector3d v1_trans = {((v1.x + 1) * model.image.get_width() / 2), ((v1.y + 1) * model.image.get_height() / 2),v1.z};
        vector3d v2_trans = {((v2.x + 1) * model.image.get_width() / 2), ((v2.y + 1) * model.image.get_height() / 2),v2.z};

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

        float intensity = dotProduct(light, crossProd);

        if (intensity > 0) {
            //on dessine le triangle
            triangle(points, pointsTex,model.image,model.texture,zbuffer,intensity);
        }
    }
}

