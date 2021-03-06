#include <iostream> // std::cout
#include "lib/tgaimage.h"
#include "model/Model.h"
#include <iostream>
#include <fstream>
#include <list>
#include <array>
#include "render/our_gl.h"

using namespace std;

constexpr int SIZE = 1000;
constexpr int width  = SIZE;
constexpr int height = SIZE;


vector<array<double,3>> readVFromFile(const string fileName){
    string sline;
    string vlines;
    ifstream myfile (fileName);
    string delimiter = " ";
    vector<array<double,3>> liste;
    array<double,3> tab{-1,-1,-1};

    if (myfile.is_open())
    {
        while ( getline (myfile, sline) )
        {
            if(!sline.empty()){
                if(!sline.rfind("v ", 0)){
                    size_t pos = 0;
                    string token;




                    if((pos = sline.find(delimiter)) != string::npos){
                        token = sline.substr(0, pos);
                        sline.erase(0, pos + delimiter.length());
                    }
                    if((pos = sline.find(delimiter)) != string::npos){
                        token = sline.substr(0, pos);
                        tab[0] = stof(token);
                        sline.erase(0, pos + delimiter.length());
                    }
                    if((pos = sline.find(delimiter)) != string::npos){
                        token = sline.substr(0, pos);
                        tab[1] = stof(token);
                        sline.erase(0, pos + delimiter.length());
                    }
                    if((pos = sline.find(delimiter)) != string::npos){
                        token = sline.substr(0, pos);
                        tab[2] = stof(token);
                        sline.erase(0, pos + delimiter.length());
                    }
                    liste.push_back(tab);
                }
            }
        }
        myfile.close();
    }

    else cout << "Unable to open file";

    return liste;
}


void drawEdge(const Model &model, TGAImage &imageFace){
    for (int i = 0; i < model.nfaces();i++) // access by reference to avoid copying
    {
        TGAColor color = TGAColor(rand()%255,rand()%255,rand()%255,255);



        vector3d v0 = model.vertex(model.getVerticesFace(i)[0]);
        vector3d v1 = model.vertex(model.getVerticesFace(i)[1]);
        vector3d v2 = model.vertex(model.getVerticesFace(i)[2]);

        vector2i v0_trans = {(int)((v0.x + 1) * width / 2), (int)((v0.y + 1) * height / 2)};
        vector2i v1_trans = {(int)((v1.x + 1) * width / 2), (int)((v1.y + 1) * height / 2)};
        vector2i v2_trans = {(int)((v2.x + 1) * width / 2), (int)((v2.y + 1) * height / 2)};

        vector2i points[3] = {v0_trans,v1_trans,v2_trans};
        //getVerticesFace
        //triangle(points, imageFace, color);
/*
        //profil

        line((int)((model.vertex(v1)[2] + 1) * width/2), (int)((model.vertex(v1)[1] + 1) * height/2),(int)((model.vertex(v2)[2] + 1) * width/2), (int)((model.vertex(v2)[1] + 1) * height/2),imageProfil,color);
        line((int)((model.vertex(v1)[2] + 1) * width/2), (int)((model.vertex(v1)[1] + 1) * height/2),(int)((model.vertex(v3)[2] + 1) * width/2), (int)((model.vertex(v3)[1] + 1) * height/2),imageProfil,color);
        line((int)((model.vertex(v2)[2] + 1) * width/2), (int)((model.vertex(v2)[1] + 1) * height/2),(int)((model.vertex(v3)[2] + 1) * width/2), (int)((model.vertex(v3)[1] + 1) * height/2),imageProfil,color);

        //dessus

        line((int)((model.vertex(v1)[2] + 1) * width/2), (int)((model.vertex(v1)[0] + 1) * height/2),(int)((model.vertex(v2)[2] + 1) * width/2), (int)((model.vertex(v2)[0] + 1) * height/2),imageDessus,color);
        line((int)((model.vertex(v1)[2] + 1) * width/2), (int)((model.vertex(v1)[0] + 1) * height/2),(int)((model.vertex(v3)[2] + 1) * width/2), (int)((model.vertex(v3)[0] + 1) * height/2),imageDessus,color);
        line((int)((model.vertex(v2)[2] + 1) * width/2), (int)((model.vertex(v2)[0] + 1) * height/2),(int)((model.vertex(v3)[2] + 1) * width/2), (int)((model.vertex(v3)[0] + 1) * height/2),imageDessus,color);
*/

     }
}


int main(){



    //line(50,50,51,51,image,red);

    Model african("obj/african_head/african_head.obj","obj/african_head/african_head_diffuse.tga",1000,1000);
    Model eye("obj/african_head/african_head_eye_inner.obj","obj/african_head/african_head_eye_inner_diffuse.tga",200,200);
    //drawV(model,image);
    //flatShading(model,imageFace);

    double zbuffer[african.image.get_height() * african.image.get_width()];
    for(int i = 0 ; i < african.image.get_height() * african.image.get_width();i++){
        zbuffer[i] = std::numeric_limits<int>::min();
    }
    render(african,zbuffer);
    eye.setImage(african.image);
    render(eye,zbuffer);
    eye.image.write_tga_file("african.tga");
/*
    Model diablo("obj/diablo3_pose/diablo3_pose.obj","obj/diablo3_pose/diablo3_pose_diffuse.tga");
    texture(diablo);
    diablo.image.write_tga_file("diablo.tga");


    Model boggie("obj/boggie/head.obj","obj/boggie/head_diffuse.tga");
    Model body("obj/boggie/body.obj","obj/boggie/body_diffuse.tga");
    texture(body);
    boggie.setImage(body.image);
    render(boggie);
    boggie.image.write_tga_file("boggie.tga");


    TGAImage render(width, 1, TGAImage::RGB);
    int ybuffer[width];
    for (int i=0; i<width; i++) {
        ybuffer[i] = std::numeric_limits<int>::min();
    }
    rasterize(vector2i(20, 34),   vector2i(744, 400), render, red,   ybuffer);
    rasterize(vector2i(120, 434), vector2i(444, 400), render, green, ybuffer);
    rasterize(vector2i(330, 463), vector2i(594, 200), render, blue,  ybuffer);

    render.write_tga_file("render.tga");
*/
    return 0;
}