//
// Created by Maxime Choné on 13/01/2021.
//


#include "Model.h"


Model::Model(const std::string filename,const std::string textureFileName,const int w,const int h) {

    image = TGAImage(w,h,TGAImage::RGB);
    //render =  TGAImage(1024,1024,TGAImage::RGB);
    texture.read_tga_file(textureFileName);
    texture.flip_vertically();

    ifstream myfile (filename);
    string line;
    char temp;

    array<double,3> coordVertex{-1,-1,-1};

    if (myfile.is_open()){

        while (!myfile.eof()) {
            getline(myfile, line);
            std::istringstream lineParser(line);

            if (!line.compare(0, 2, "v ")) {
                double x;
                double y;
                double z;

                lineParser >> temp;
                lineParser >> x;
                lineParser >> y;
                lineParser >> z;

                vertices.push_back({x, y, z});
            } else if (!(line.compare(0, 3, "vt "))) {
                double u;
                double v;

                lineParser >> temp >> temp;
                lineParser >> u;
                lineParser >> v;

                textureVertices.push_back({u, v});

            } else if (!line.compare(0, 2, "f ")) {
                int vt1,vt2,vt3;//à remplacer plus tard pour récupérer l'info
                int vn1,vn2,vn3;
                int v1, v2, v3;
                lineParser >> temp;

                lineParser >> v1 >> temp >> vt1 >> temp >> vn1;

                lineParser >> v2 >> temp >> vt2 >> temp >> vn2;

                lineParser >> v3 >> temp >> vt3 >> temp >> vn3;

                facesTextureVertices.push_back({vt1 - 1,vt2 - 1,vt3 -1});
                facesVertices.push_back({v1 - 1, v2 - 1, v3 - 1});

            }
        }


        myfile.close();

    }
}

int Model::nvertices() const {
    return vertices.size();
}

vector3d Model::vertex(const int i) const {
    return vertices[i];
}

array<int, 3> Model::getVerticesFace(const int i) const {
    return facesVertices[i];
}

int Model::nfaces() const {
    return facesVertices.size();
}

vector2d Model::getTextureVertices(const int i) const {
    return textureVertices[i];
}

array<int, 3> Model::getTextureVerticesFace(const int i) const {
    return facesTextureVertices[i];
}

void Model::setImage(TGAImage image) {
    this->image = image;
}



