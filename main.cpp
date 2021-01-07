#include <iostream> // std::cout
#include "lib/tgaimage.h"
#include "render/draw.h"
#include <iostream>
#include <fstream>
#include <list>
#include <array>

using namespace std;

constexpr int SIZE = 500;
constexpr int width  = SIZE;
constexpr int height = SIZE;
const TGAColor white = {255, 255, 255, 255};
const TGAColor red   = {255,   0,   0, 255};
const TGAColor green = {  0, 255,   0, 255};
const TGAColor blue  = {  0,   0, 255, 255};

vector<array<float,3>> readVFromFile(string fileName){
    string sline;
    string vlines;
    ifstream myfile (fileName);
    string delimiter = " ";
    vector<array<float,3>> liste;
    array<float,3> tab{};

    if (myfile.is_open())
    {
        while ( getline (myfile, sline) )
        {
            if(!sline.empty()){
                if(!sline.rfind("v ", 0)){
                    size_t pos = 0;
                    string token;
                    float x = -1;
                    float y = -1;
                    float z = -1;

                    if((pos = sline.find(delimiter)) != string::npos){
                        token = sline.substr(0, pos);
                        sline.erase(0, pos + delimiter.length());
                    }
                    if((pos = sline.find(delimiter)) != string::npos){
                        token = sline.substr(0, pos);
                        x = stof(token);
                        sline.erase(0, pos + delimiter.length());
                    }
                    if((pos = sline.find(delimiter)) != string::npos){
                        token = sline.substr(0, pos);
                        y = stof(token);
                        sline.erase(0, pos + delimiter.length());
                    }
                    if((pos = sline.find(delimiter)) != string::npos){
                        token = sline.substr(0, pos);
                        z = stof(token);
                        sline.erase(0, pos + delimiter.length());
                    }
                    liste.push_back({x,y,z});
                }
            }
        }
        myfile.close();
    }

    else cout << "Unable to open file";

    return liste;
}

void drawV(vector<array<float,3>> liste, TGAImage &image){

    for (auto tab : liste) // access by reference to avoid copying
    {
        line((tab[0] + 1) * width/2, (tab[1] + 1) * height/2,(tab[0] + 1) * width/2 + 1, (tab[1] + 1) * height/2 + 1,image,green);
    }


}


int main(){
    TGAImage image(width, height, TGAImage::RGB);

    //line(50,50,51,51,image,red);


    drawV(readVFromFile("obj/african_head/african_head.obj"),image);
    image.write_tga_file("framebuffer.tga");

    return 0;
}