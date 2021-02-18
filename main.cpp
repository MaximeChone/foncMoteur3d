#include <iostream> // std::cout
#include "lib/tgaimage.h"
#include "model/Model.h"
#include <iostream>
#include <fstream>
#include <list>
#include <array>
#include "render/our_gl.h"

using namespace std;

int width  = 2000;
int height = 2000;

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


        TGAColor tempColor = model.normalmapping.get(u * model.normalmapping.get_width(),v * model.normalmapping.get_height());

        vector3d colorV;// = {((double)tempColor.bgra[0]),(double)(tempColor.bgra[1]),(double)(tempColor.bgra[2])};

        colorV.x = tempColor[2]/255.*2 - 1;
        colorV.y = tempColor[1]/255.*2 - 1;
        colorV.z = tempColor[0]/255.*2 - 1;

        float intensity = dotProduct(colorV,light);
        //float intensity = dotProduct(varying_intensity,bar);

        color = model.texture.get(u * model.texture.get_width(),v * model.texture.get_height())*intensity;

        return false;
    }
};

void Gouraudrender(Model &model, vector<double> &zbuffer){

    GouraudShader shader(model);

    for (int i = 0; i < model.nfaces();i++)
    {
        vector4d screen_coords[3];

        for(int j = 0; j < 3;j++){
            screen_coords[j] = shader.vertex(i,j);
        }

        cout << i << endl;

        triangle(screen_coords,shader,model.image,zbuffer);
    }

}

int main(){
    Model diablo("obj/diablo3_pose/diablo3_pose.obj","obj/diablo3_pose/diablo3_pose_diffuse.tga","obj/diablo3_pose/diablo3_pose_nm.tga",width,height);
    Model boggie("obj/boggie/body.obj","obj/boggie/body_diffuse.tga","obj/boggie/body_nm_tangent.tga",width,height);
    Model boggie_eye("obj/boggie/eyes.obj","obj/boggie/eyes_diffuse.tga","obj/boggie/eyes_nm_tangent.tga",width,height);
    Model boggie_head("obj/boggie/head.obj","obj/boggie/head_diffuse.tga","obj/boggie/head_nm_tangent.tga",width,height);
    Model african("obj/african_head/african_head.obj","obj/african_head/african_head_diffuse.tga","obj/african_head/african_head_nm.tga",width,height);
    Model african_eye("obj/african_head/african_head_eye_inner.obj", "obj/african_head/african_head_eye_inner_diffuse.tga", "obj/african_head/african_head_eye_inner_nm.tga", width, height);

    vector<double> zbuffer(width * height,std::numeric_limits<int>::min());

    light.normalize();
    Gouraudrender(african, zbuffer);
    african_eye.setImage(african.image);

    Gouraudrender(african_eye, zbuffer);
    //shrek.image.write_tga_file("shrek_out.tga");


    african_eye.image.write_tga_file("african_out.tga");


    zbuffer = vector<double>(width * height,std::numeric_limits<int>::min());

    Gouraudrender(diablo,zbuffer);
    diablo.image.write_tga_file("diablo_out.tga");

    zbuffer = vector<double>(width * height,std::numeric_limits<int>::min());

    Gouraudrender(boggie,zbuffer);
    boggie_eye.image = boggie.image;
    Gouraudrender(boggie_eye,zbuffer);
    boggie_head.image = boggie_eye.image;
    Gouraudrender(boggie_head,zbuffer);
    boggie_head.image.write_tga_file("boggie_out.tga");

    return 0;

}