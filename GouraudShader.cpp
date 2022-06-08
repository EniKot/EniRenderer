//
// Created by 29839 on 2022/5/22.
//
#include "EniShader.h"
#include "model.h"
#include <iostream>
#include <vector>
struct GouraudShader:public Shader{
    Model *model;

    Vec3f light_dir;
    Vec3f varying_intensity;
    virtual Vec4f vertexShader(int face_idx,int vert_idx) override{
        varying_intensity[vert_idx]=std::max(0.f,model->getNormal(face_idx,vert_idx)*light_dir);
    }

};
