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
static vector3d camera = {1,1,4};
const static vector3d center = {0,0,0.25};

struct GouraudShader : public IShader {
    vector3d varying_intensity;
    const Model &model;
    Matrix varying_uv;


    GouraudShader(Model &model): model(model) {
        varying_uv = Matrix(2,3);
    }


    virtual vector4d vertex(int iface, int nthvert) {

        double temp = max(0.f, ((float)dotProduct(model.getVn(model.getFaceVn(iface)[nthvert]),light))); // get diffuse lighting intensity

        vector2d uv = model.getVt(model.getFaceVt(iface)[nthvert]);


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

        varying_uv[0][nthvert] = uv.x;
        varying_uv[1][nthvert] = uv.y;


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

        float u = (varying_uv[0][0] * bar.x + varying_uv[0][1] * bar.y + varying_uv[0][2] * bar.z);
        float v = (varying_uv[1][0] * bar.x + varying_uv[1][1] * bar.y + varying_uv[1][2] * bar.z);


        color = model.normalmapping.get(u * model.normalmapping.get_width(),v * model.normalmapping.get_height());

        vector3d colorV = {((double)color.bgra[0]),(double)(color.bgra[1]),(double)(color.bgra[2])};
        colorV.normalize();

        float intensity = dotProduct(colorV,bar);

        color = model.texture.get(u * model.texture.get_width(),v * model.texture.get_height())*intensity;

        return false;
    }
};

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



   // Model diablo("obj/diablo3_pose/diablo3_pose.obj","obj/diablo3_pose/diablo3_pose_diffuse.tga",,width,height);

    Model african("obj/african_head/african_head.obj","obj/african_head/african_head_diffuse.tga","obj/african_head/african_head_nm.tga",width,height);
    //Model shrek("obj/shrek/shrek.obj","obj/african_head/african_head_diffuse.tga",width,height);
    Model eye("obj/african_head/african_head_eye_inner.obj","obj/african_head/african_head_eye_inner_diffuse.tga","obj/african_head/african_head_eye_inner_nm.tga",width,height);



    double zbuffer[height * width];
    for(int i = 0 ; i < width * height;i++){
        zbuffer[i] = std::numeric_limits<int>::min();
    }

    light.normalize();
    Gouraudrender(african, zbuffer);
    eye.setImage(african.image);
    Gouraudrender(eye,zbuffer);
    //shrek.image.write_tga_file("shrek_out.tga");
    eye.image.write_tga_file("out.tga");


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