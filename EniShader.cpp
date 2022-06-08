//
// Created by 29839 on 2022/5/22.
//
/*
#include "geometry.hpp"
Matrix get_rotation_Matrix(Vec3f Axis,float angle){
    angle=angle/180.0*PI;
    Matrix rotation=Matrix::identity(3);
    float c=cos(angle);
    float s=sin(angle);
    std::vector<std::vector<float>> rotArray={{c+(1-c)*Axis.x*Axis.x,(1-c)*Axis.x*Axis.y-s*Axis.z,(1-c)*Axis.x*Axis.z+s*Axis.y},
                                              {(1-c)*Axis.x*Axis.y+s*Axis.z,c+(1-c)*Axis.y*Axis.y,(1-c)*Axis.y*Axis.z-s*Axis.x},
                                              {(1-c)*Axis.x*Axis.z-s*Axis.y,(1-c)*Axis.y*Axis.z+s*Axis.x,c+(1-c)*Axis.z*Axis.z}};
    rotation=rotArray;
    return rotation;
}
Matrix get_model_Matrix(const Vec3f p){
    return NULL;
}
Matrix LookAt(Vec3f camera,Vec3f center,Vec3f up){//up:the camera
    Vec3f z=(camera-center).normalize();//view direction(maybe reverse?)
    Vec3f x=z.cross(up).normalize();//compute x-Axis and camera is Left_hand coordinate
    Vec3f y=x.cross(z).normalize();
    Matrix view=Matrix::identity(4);
    std::vector<std::vector<float>> viewArray={{x[0],y[0],z[0],-camera[0]},
                                               {x[1],y[1],z[1],-camera[1]},
                                               {x[2],y[2],z[2],-camera[2]},
                                               {0,0,0,1}};
    view=viewArray;
    return view;
}
Matrix get_projection_Matrix(float aspect,float zNear,float zFar,float fov){
    Matrix projection=Matrix::identity(4);
    Matrix perspToOrth=Matrix::identity(4);
    Matrix scale=::Matrix::identity(4);
    Matrix trans=::Matrix::identity(4);
    float angle=fov/180.0*PI/2.0;
    float height=zNear*tan(angle)*2;
    float width=height*aspect;
    std::vector<std::vector<float>> pertoOrthArray={{-zNear,0,0,0},
                                                    {0,-zNear,0,0},
                                                    {0,0,(zNear+zFar),-zNear*zFar},
                                                    {0,0,1,0}};
    std::vector<std::vector<float>> scaleArray={{2.f/width,0,0,0},
                                                {0,2.f/height,0,0},
                                                {0,0,1/(zFar-zNear),0}
            ,{0,0,0,1}};
    std::vector<std::vector<float>> transArray={{1,0,0,0},
                                                {0,1,0,0},
                                                {0,0,1,-1*(zFar+zNear)/(zFar-zNear)},
                                                {0,0,0,1}};
    perspToOrth=pertoOrthArray;
    scale=scaleArray;
    trans=transArray;
    projection=trans*scale*perspToOrth;
    return projection;
}
Matrix viewport(int x,int y,int w,int h){
    Matrix m=Matrix::identity(4);
    std::vector<std::vector<float>> a={{w/2.f,0,0,x+w/2.f},
                                       {0,h/2.f,0,y+h/2.f},
                                       {0,0,255.f/2.f,255.f/2.f},
                                       {0,0,0,1}};
    m=a;
    return m;

}*/