//
// Created by 29839 on 2022/5/12.
//

#ifndef ENIRENDERER_TEXTURE_HPP
#define ENIRENDERER_TEXTURE_HPP
#include "tgaimage.h"
#include "math.hpp"
#include "model.h"
class Texture {
public:
    Model *model;
    Texture(std::string filename,TGAImage &img){
        std::string texfile(filename);
        std::cerr<<"Texfile"<<texfile<<" Loading "<<(img.read_tga_file(texfile.c_str())?"ok":"failed")<<std::endl;
        img.flip_vertically();
    }

    ~Texture();
    Vec2f uv;
    TGAImage DiffuseMap;
    float lerp(float x0,float x1,float t){
      return x0+t*(x1-x0);
    }
    TGAColor getColor(Vec2f uv){
        return DiffuseMap.get((int)uv.x,(int)uv.y);
    }
    Vec2f j(int face_idx,int vertex_idx){
        int idx=model->faceDatas[face_idx][vertex_idx][1];
       // return Vec2f(model->uv[idx].x*DiffuseMap.get_width(),model->uv[idx].y*DiffuseMap.get_height());
       return Vec2f(0,0);
    }
};


#endif //ENIRENDERER_TEXTURE_HPP
