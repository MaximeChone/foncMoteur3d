#include <iostream> // std::cout
#include "lib/tgaimage.h"
#include "render/draw.h"
#include "model/Model.h"
#include <iostream>
#include <fstream>
#include <list>
#include <array>

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

void drawV(const Model &model, TGAImage &image){

    for (int i = 0; i < model.nvertices();i++) // access by reference to avoid copying
    {
        line((int)((model.vertex(i)[0] + 1) * width/2), (int)((model.vertex(i)[1] + 1) * height/2),(int)((model.vertex(i)[0] + 1) * width/2 + 1), (int)((model.vertex(i)[1] + 1) * height/2 + 1),image,green);
    }


}

void drawEdge(const Model &model, TGAImage &imageFace,TGAImage &imageProfil,TGAImage &imageDessus){
    for (int i = 0; i < model.nfaces();i++) // access by reference to avoid copying
    {
        int v1 = model.face(i)[0];
        int v2 = model.face(i)[1];
        int v3 = model.face(i)[2];

        //face

        line((int)((model.vertex(v1)[0] + 1) * width/2), (int)((model.vertex(v1)[1] + 1) * height/2),(int)((model.vertex(v2)[0] + 1) * width/2), (int)((model.vertex(v2)[1] + 1) * height/2),imageFace,green);
        line((int)((model.vertex(v1)[0] + 1) * width/2), (int)((model.vertex(v1)[1] + 1) * height/2),(int)((model.vertex(v3)[0] + 1) * width/2), (int)((model.vertex(v3)[1] + 1) * height/2),imageFace,green);
        line((int)((model.vertex(v2)[0] + 1) * width/2), (int)((model.vertex(v2)[1] + 1) * height/2),(int)((model.vertex(v3)[0] + 1) * width/2), (int)((model.vertex(v3)[1] + 1) * height/2),imageFace,green);


        //profil

        line((int)((model.vertex(v1)[2] + 1) * width/2), (int)((model.vertex(v1)[1] + 1) * height/2),(int)((model.vertex(v2)[2] + 1) * width/2), (int)((model.vertex(v2)[1] + 1) * height/2),imageProfil,green);
        line((int)((model.vertex(v1)[2] + 1) * width/2), (int)((model.vertex(v1)[1] + 1) * height/2),(int)((model.vertex(v3)[2] + 1) * width/2), (int)((model.vertex(v3)[1] + 1) * height/2),imageProfil,green);
        line((int)((model.vertex(v2)[2] + 1) * width/2), (int)((model.vertex(v2)[1] + 1) * height/2),(int)((model.vertex(v3)[2] + 1) * width/2), (int)((model.vertex(v3)[1] + 1) * height/2),imageProfil,green);

        //dessus

        line((int)((model.vertex(v1)[2] + 1) * width/2), (int)((model.vertex(v1)[0] + 1) * height/2),(int)((model.vertex(v2)[2] + 1) * width/2), (int)((model.vertex(v2)[0] + 1) * height/2),imageDessus,green);
        line((int)((model.vertex(v1)[2] + 1) * width/2), (int)((model.vertex(v1)[0] + 1) * height/2),(int)((model.vertex(v3)[2] + 1) * width/2), (int)((model.vertex(v3)[0] + 1) * height/2),imageDessus,green);
        line((int)((model.vertex(v2)[2] + 1) * width/2), (int)((model.vertex(v2)[0] + 1) * height/2),(int)((model.vertex(v3)[2] + 1) * width/2), (int)((model.vertex(v3)[0] + 1) * height/2),imageDessus,green);


     }
}


int main(){
    TGAImage imageFace(width, height, TGAImage::RGB);
    TGAImage imageProfil(width, height, TGAImage::RGB);
    TGAImage imageDessus(width, height, TGAImage::RGB);

    //line(50,50,51,51,image,red);

    Model model("obj/african_head/african_head.obj");

    //drawV(model,image);
    drawEdge(model,imageFace,imageProfil,imageDessus);
    imageFace.write_tga_file("face.tga");
    imageProfil.write_tga_file("profil.tga");
    imageDessus.write_tga_file("dessus.tga");


    return 0;
}