#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char *filename) : verts_(), faceDatas() ,vertexNormals(),vertexTex(){
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if(in.fail()){
        return;
    }
    std::string line;
    while(!in.eof()){
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if(!line.compare(0, 2, "v ")){
            iss >> trash;
            Vec3f v;
            for (int i = 0; i < 3; i++) iss >> v[i];
            verts_.push_back(v);
        }else if(!line.compare(0, 3, "vt ")){//cope with Textcoordinate data
            iss >> trash>>trash;
            Vec2f vt;
            for (int i = 0; i < 2; i++) iss >> vt[i];
            vertexTex.push_back(vt);
        }else if(!line.compare(0,3,"vn ")){//cope with vertex normaldata
            iss >> trash>>trash;
            Vec3f vn;
            for (int i = 0; i < 3; i++) iss >> vn[i];
            vertexNormals.push_back(vn);
        }else if(!line.compare(0, 2, "f ")){
            std::vector<Vec3i> f;
            Vec3i tmp;
            iss >> trash;
            while(iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2]){
                for(int i=0;i<3;i++)tmp[i]--;
                f.push_back(tmp);
            }
            faceDatas.push_back(f);
        }
    } std::cerr << " vt# " << vertexTex.size() << " vn# " << vertexNormals.size() << std::endl;std::cerr << "# v# " << verts_.size() << " f# "  << faceDatas.size() << std::endl;
    //load_texture(filename,DiffuseMap);
}

Model::~Model() {
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)faceDatas.size();
}

std::vector<int> Model::getFace(int idx) {
    std::vector<int> face;
    for(int i=0;i<(int)faceDatas[idx].size();i++){
        face.push_back(faceDatas[idx][i][0]);
    }
    return face;
}
Vec3f Model::getVertex(int i) {
    return verts_[i];
}
Vec3f Model::getNormal(int face_idx, int vert_idx) {
    int idx=faceDatas[face_idx][vert_idx][2];
    return vertexNormals[idx].normalize();
}
void Model::load_texture(std::string filename, TGAImage &image) {
    std::string texfile(filename);
    std::cerr<<"Texfile"<<texfile<<" Loading "<<(image.read_tga_file(texfile.c_str())?"ok":"failed")<<std::endl;
    image.flip_vertically();
}

TGAColor Model::getColor(Vec2f uv){
    return DiffuseMap.get((int)uv.x,(int)uv.y);
}
Vec2f Model::getTexureCoord(int iface, int nvert) {
    int idx=faceDatas[iface][nvert][1];
    return Vec2f(vertexTex[idx].x*DiffuseMap.get_width(),vertexTex[idx].y*DiffuseMap.get_height());
}