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

 static vector3d light = {1,1,1};
const static vector3d camera = {1,1,3};
const static vector3d center = {0,0,0};

struct GouraudShader : public IShader {
    vector3d varying_intensity;
    const Model &model;

    GouraudShader(Model &model): model(model) {
    }


    virtual vector4d vertex(int iface, int nthvert) {

        double temp = max(0.f, ((float)dotProduct(model.getVn(model.getFaceVn(iface)[nthvert]),light))); // get diffuse lighting intensity

        switch (nthvert) {
            case 0:
                varying_intensity.x = temp;
                break;
            case 1:
                varying_intensity.y = temp;
                break;
            case 2:
                varying_intensity.z = temp;
                break;
            default:
                break;
        }
        vector3d tempV = model.vertex(model.getVerticesFace(iface)[nthvert]);
        vector4d gl_Vertex;// = vector4d(tempV.x,tempV.y,tempV.z,1); // read the vertex from .obj file

        Matrix viewPort = viewport(0,0,model.image.get_width(),model.image.get_height());
        Matrix modelView = lookat(camera,center,{0,1,0});
        Matrix proj = Matrix::identity(4);
        proj[3][2] = -1.0/camera.z;

        tempV = m2v(viewPort * proj * modelView * v2m(tempV));

        gl_Vertex = vector4d(tempV,1);


        return gl_Vertex;
    }

    virtual bool fragment(vector3d bar, TGAColor &color) {

        float intensity = dotProduct(varying_intensity,bar);   // interpolate intensity for the current pixel
        color = TGAColor(255, 255, 255)*intensity; // well duh
        return false;                              // no, we do not discard this pixel
    }
};



void render(Model &model,double zbuffer[]){

    Matrix proj = Matrix::identity(4);
    proj[3][2] = -1.0/camera.z;

    Matrix viewPort = viewport(0,0,model.image.get_width(),model.image.get_height());
    Matrix modelView = lookat(camera,center,{0,1,0});


    for (int i = 0; i < model.nfaces();i++)
    {

        //on récupère les sommets de la getVerticesFace i et on fait la projection par rapport à la caméra
        vector3d v0 = model.vertex(model.getVerticesFace(i)[0]);


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
        vector2d v0Tex = model.getVt(model.getFaceVt(i)[0]);
        vector2d v1Tex = model.getVt(model.getFaceVt(i)[1]);
        vector2d v2Tex = model.getVt(model.getFaceVt(i)[2]);

        vector2d pointsTex[3] = {v0Tex,v1Tex,v2Tex};

        //cout << i <<" coucou : "<<model.getTextureVerticesFace(i)[0] <<" coucou2:" << model.getTextureVerticesFace(i)[1] << " coucou3:" << model.getFaceVt(i)[2] << "\n";
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

void Gouraudrender(Model &model, double *zbuffer){

    GouraudShader shader(model);

    for (int i = 0; i < model.nfaces();i++)
    {
        vector4d screen_coords[3];

        for(int j = 0; j < 3;j++){
            screen_coords[j] = shader.vertex(i,j);
        }

        triangle(screen_coords,shader,model.image,zbuffer);
    }

}




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


int main(){




    Model african("obj/african_head/african_head.obj","obj/african_head/african_head_diffuse.tga",width,height);
    Model eye("obj/african_head/african_head_eye_inner.obj","obj/african_head/african_head_eye_inner_diffuse.tga",width,height);

    double zbuffer[african.image.get_height() * african.image.get_width()];
    for(int i = 0 ; i < african.image.get_height() * african.image.get_width();i++){
        zbuffer[i] = std::numeric_limits<int>::min();
    }

    light.normalize();
    Gouraudrender(african, zbuffer);

    african.image.write_tga_file("out.tga");

/*
    double zbuffer[african.image.get_height() * african.image.get_width()];
    for(int i = 0 ; i < african.image.get_height() * african.image.get_width();i++){
        zbuffer[i] = std::numeric_limits<int>::min();
    }
    render(african,zbuffer);
    eye.setImage(african.image);
    render(eye,zbuffer);
    eye.image.write_tga_file("african.tga");
*/
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