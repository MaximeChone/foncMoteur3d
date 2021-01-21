//
// Created by Maxime Choné on 13/01/2021.
//


#include "Model.h"


Model::Model(const std::string filename) {
    ifstream myfile (filename);
    string line;
    char temp;

    array<double,3> coordVertex{-1,-1,-1};

    if (myfile.is_open()){

        while (!myfile.eof()) {
            getline(myfile,line);
            std::istringstream lineParser(line);

            if(!line.compare(0,2,"v ")){
                double x;
                double y;
                double z;

                lineParser >> temp;
                lineParser >> x;
                lineParser >> y;
                lineParser >> z;

                vertices.push_back({x,y,z});
            } else if(!line.compare(0,2,"f ")) {
                int temp1;//à remplacer plus tard pour récupérer l'info
                int v1, v2, v3;
                lineParser >> temp;

                lineParser >> v1 >> temp >> temp1 >> temp >> temp1;

                lineParser >> v2 >> temp >> temp1 >> temp >> temp1;

                lineParser >> v3 >> temp >> temp1 >> temp >> temp1;

                faces.push_back({v1 - 1, v2 - 1, v3 - 1});

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

array<int, 3> Model::face(const int i) const {
    return faces[i];
}

int Model::nfaces() const {
    return faces.size();
}



