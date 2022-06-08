//
// Created by 29839 on 2022/5/22.
//

#ifndef ENIRENDERER_ENISHADER_H
#define ENIRENDERER_ENISHADER_H
#include "math.hpp"
#include "tgaimage.h"

extern Matrix ModelView;
extern Matrix Viewport;
extern Matrix Projection;

void viewport(int x,int y,int w,int h);
void projection(float coeff=0.f);//coeff=-1/c
void lookat(Vec3f camera,Vec3f center,Vec3f up);
struct Vertex{
    Vec4f vertexpos;
    Vec2f uv;
    Vec3f normal;
};
struct Triangle{
    Vertex vertices[3];

};
struct Shader {
    virtual ~Shader();
    virtual Vec4f vertexShader(int face_idx,int vert_idx)=0;
    virtual bool fragmentShader(Vec3f bar,TGAColor &color)=0;
};
void triangle(Vec4f *pts,Shader *shader,TGAImage &image,TGAImage &DepthImage);

#endif //ENIRENDERER_ENISHADER_H
