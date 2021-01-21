//
// Created by Maxime Choné on 21/01/2021.
//

#include "renderer.h"

void flatShading(const Model &model, TGAImage &image){
    for (int i = 0; i < model.nfaces();i++) // access by reference to avoid copying
    {


        //on récupère les sommets de la face i
        vector3d v0 = model.vertex(model.face(i)[0]);
        vector3d v1 = model.vertex(model.face(i)[1]);
        vector3d v2 = model.vertex(model.face(i)[2]);

        //on transforme les sommets en position de l'écran
        vector2i v0_trans = {(int) ((v0.x + 1) * image.get_width() / 2), (int) ((v0.y + 1) * image.get_height() / 2)};
        vector2i v1_trans = {(int) ((v1.x + 1) * image.get_width() / 2), (int) ((v1.y + 1) * image.get_height() / 2)};
        vector2i v2_trans = {(int) ((v2.x + 1) * image.get_width() / 2), (int) ((v2.y + 1) * image.get_height() / 2)};

        vector2i points[3] = {v0_trans, v1_trans, v2_trans};

        vector3d AB = {v1.x - v0.x,
                       v1.y - v0.y,
                       v1.z - v0.z};
        vector3d AC = {v2.x - v0.x,
                       v2.y - v0.y,
                       v2.z - v0.z};

        vector3d crossProd = crossProduct(AB, AC);
        crossProd.normalize();

        double intensity = dotProduct(light, crossProd);


        if (intensity > 0) {
        //on dessine le triangle
        cout << "intensity=" << intensity << "\n";
            TGAColor color = TGAColor(intensity * 255, intensity * 255, intensity * 255, 255);

            triangle(points, image, color);
        }
    }
}
