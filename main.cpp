#include <iostream> // std::cout
#include "lib/tgaimage.h"
#include "render/draw.h"
#include "model/Model.h"
#include <iostream>
#include <fstream>
#include <list>
#include <array>

using namespace std;

constexpr int SIZE = 5000;
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

        array<double, 3> v0 = model.vertex(model.face(i)[0]);
        array<double, 3> v1 = model.vertex(model.face(i)[1]);
        array<double, 3> v2 = model.vertex(model.face(i)[2]);

        Vertex v0_trans = {(int)((v0[0] + 1) * width/2),(int)((v0[1] + 1) * height/2),(int)((v0[2] + 1) * width/2)};
        Vertex v1_trans = {(int)((v1[0] + 1) * width/2),(int)((v1[1] + 1) * height/2),(int)((v1[2] + 1) * width/2)};
        Vertex v2_trans = {(int)((v2[0] + 1) * width/2),(int)((v2[1] + 1) * height/2),(int)((v2[2] + 1) * width/2)};

        //face
        triangle(v0_trans,v1_trans,v2_trans,imageFace,color);
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
    drawEdge(model,imageFace);
    imageFace.write_tga_file("face.tga");

/*
    TGAImage image(200,200,TGAImage::RGB);

    triangle({10,150,20},{60,150,20},{50,22,20},image,green);
    triangle({180,100,20},{140,110,20},{200,200,20},image,blue);
    triangle({120,10,20},{80,190,20},{140,60,20},image,red);

    image.write_tga_file("image.tga");
*/

    return 0;
}