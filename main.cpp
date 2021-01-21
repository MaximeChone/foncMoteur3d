#include <iostream> // std::cout
#include "lib/tgaimage.h"
#include "render/draw.h"
#include "model/Model.h"
#include <iostream>
#include <fstream>
#include <list>
#include <array>
#include "render/renderer.h"

using namespace std;

constexpr int SIZE = 1000;
constexpr int width  = SIZE;
constexpr int height = SIZE;
const TGAColor white = {255, 255, 255, 255};
const TGAColor red   = {255,   0,   0, 255};
const TGAColor green = {  0, 255,   0, 255};
const TGAColor blue  = {  0,   0, 255, 255};

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



        vector3d v0 = model.vertex(model.face(i)[0]);
        vector3d v1 = model.vertex(model.face(i)[1]);
        vector3d v2 = model.vertex(model.face(i)[2]);

        vector2i v0_trans = {(int)((v0.x + 1) * width / 2), (int)((v0.y + 1) * height / 2)};
        vector2i v1_trans = {(int)((v1.x + 1) * width / 2), (int)((v1.y + 1) * height / 2)};
        vector2i v2_trans = {(int)((v2.x + 1) * width / 2), (int)((v2.y + 1) * height / 2)};

        vector2i points[3] = {v0_trans,v1_trans,v2_trans};
        //face
        triangle(points, imageFace, color);
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


    TGAImage imageFace(width, height, TGAImage::RGB);

    //line(50,50,51,51,image,red);

    Model model("obj/african_head/african_head.obj");

    //drawV(model,image);
    flatShading(model,imageFace);
    imageFace.write_tga_file("face.tga");


/*
    TGAImage image(200,200,TGAImage::RGB);

    vector2i points[3] = {{10,150},{60,150},{50,22}};
    triangle(points,image,green);
    vector2i points1[3] = {{180,100},{140,110},{200,200}};
    triangle(points1,image,blue);
    vector2i points2[3] = {{120,10},{80,190},{140,60}};
    triangle(points2,image,red);

    image.write_tga_file("image.tga");
*/

    return 0;
}