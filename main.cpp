#include "tgaimage.h"
#include "model.h"
#include "math.hpp"
#include <limits>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <opencv2/opencv.hpp>
#include "OBJ_Loader.h"
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

const int WIDTH=800;
const int HEIGHT=800;
const int DEPTH=255;
float aspect=WIDTH/HEIGHT;
Model *model=NULL;
float *zbuffer=NULL;
std::vector<Vec3f> framebuffer;
Vec3f light_dir=Vec3f(0,0,1).normalize();
Vec3f camera(0,0,2);//theCameraPosition
Vec3f center(0,0,0);//worldCenter
Vec3f up(0,1.f,0);
template<class T> T clamp(T NUM,T MINN,T MAXN){
    return std::min(std::max(NUM,MINN),MAXN);
}

Matrix get_move_Matrix(Vec3f move){
    Matrix moveMatrix=Matrix::identity(4);
    std::vector<std::vector<float>> moveArray={{0,0,0,move.x},{0,0,0,move.y},{0,0,0,move.z},{0,0,0,1}};
    moveMatrix=moveArray;
    return moveMatrix;
}
Matrix get_rotation_Matrix(Vec3f Axis,float angle){//罗德里格斯旋转公式
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
Matrix LookAt(Vec3f camera,Vec3f center,Vec3f up){//up:the camera
    Vec3f z=(camera-center).normalize();//view direction(maybe reverse?)
    Vec3f x=z.cross(up).normalize();//compute x-Axis and camera is Left_hand coordinate
    Vec3f y=x.cross(z).normalize();
    std::cout<<"cameraZ:"<<z<<std::endl;
    std::cout<<"cameraX:"<<x<<std::endl;
    std::cout<<"cameraY:"<<y<<std::endl;
    Matrix view=Matrix::identity(4);
    Matrix viewTrans=Matrix::identity(4);
    std::vector<std::vector<float>> viewArray_first={{1,0,0,-camera[0]},
                                               {0,1,0,-camera[1]},
                                               {0,0,1,-camera[2]},
                                               {0,0,0,1}};
    std::vector<std::vector<float>> viewArray_second={{x[0],x[1],x[2],0},
                                                      {y[0],y[1],y[2],0},
                                                      {z[0],z[1],z[2],0},
                                                      {0   ,0   ,0   ,1}};

    view=viewArray_first;
    viewTrans=viewArray_second;
    view=viewTrans*view;
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
                                                    {0,0,(zNear+zFar),-1*zNear*zFar},
                                                     {0,0,1,0}};
    std::vector<std::vector<float>> scaleArray={{2.f/width,0,0,0},
                                                {0,2.f/height,0,0},
                                                {0,0,2.f/(zFar-zNear),0}
                                                ,{0,0,0,1}};
    std::vector<std::vector<float>> transArray={{1,0,0,0},
                                                {0,1,0,0},
                                                {0,0,1,-1*(zFar+zNear)/(zFar-zNear)},
                                                {0,0,0,1}};
    perspToOrth=pertoOrthArray;
    scale=scaleArray;
    trans=transArray;
    projection=scale*perspToOrth;
    projection=trans*projection;
    return projection;
}
void CVVCulling(Vec4f v){
    v.x=clamp<float>(v.x,-1*v.w,v.w);
    v.y=clamp<float>(v.y,-1*v.w,v.w);
    v.z=clamp<float>(v.z,-1*v.w,v.w);
}
Vec3f toHomogenize(Vec4f v){
 //   assert(v.w>0);
    v.x=v.x/v.w;
    v.y=v.y/v.w;
    v.z=v.z/v.w;
    return Vec3f((v.x+1.f)*WIDTH/2,(1.f-v.y)*HEIGHT/2,v.z);
}
void drawLineBrensenham(int x0,int y0,int x1,int y1,TGAImage &image,TGAColor color){
    bool steep=false;
    if(std::abs(x0-x1)<std::abs(y0-y1)){
        std::swap(x0,y0);
        std::swap(x1,y1);
        steep=true;
    }
    if(x0>x1){
        std::swap(x0,x1);
        std::swap(y0,y1);
    }
    int dx=x1-x0;
    int dy=y1-y0;
    float derror2=std::abs(dy)*2;
    float error2=0;
    int y=y0;
    for(int x=x0;x<=x1;x++){
        //You can also use interpolate to compute shading point,which is slower than what we use now
        //float t=(x-x0)/(float)(x1-x0);
        //y=y0+t*(y1-y0);
        if(steep){
            image.set(y,x,color);
        }else{
            image.set(x,y,color);
        }
        error2+=derror2;
        if(error2>dx){
            y+=(y1>y0?1:-1);
            error2-=dx*2;
        }
    }
}
Vec3f computeBary(Vec3f *vs,Vec2f p){

    float alpha=(-1*(p.x-vs[1].x)*(vs[2].y-vs[1].y)+(p.y-vs[1].y)*(vs[2].x-vs[1].x))/
                 (-1*(vs[0].x-vs[1].x)*(vs[2].y-vs[1].y)+(vs[0].y-vs[1].y)*(vs[2].x-vs[1].x));
    float beta=(-1*(p.x-vs[2].x)*(vs[0].y-vs[2].y)+(p.y-vs[2].y)*(vs[0].x-vs[2].x))/
                (-1*(vs[1].x-vs[2].x)*(vs[0].y-vs[2].y)+(vs[1].y-vs[2].y)*(vs[0].x-vs[2].x));
    float gamma=1.0-beta-alpha;
    Vec3f bary=Vec3f(alpha,beta,gamma);
    return bary;
}
Vec2f computeUV(Vec2f *uvs,Vec3f bary,float z_interpolated,Vec3f *vs){
    Vec2f uv;
    float alpha=bary.x;
    float beta=bary.y;
    float gamma=bary.z;
    uv.x=(alpha*uvs[0].x/vs[0].z+beta*uvs[1].x/vs[1].z+gamma*uvs[2].x/vs[2].z)*z_interpolated;
    uv.y=(alpha*uvs[0].y/vs[0].z+beta*uvs[1].y/vs[1].z+gamma*uvs[2].y/vs[2].z)*z_interpolated;
    return uv;
}
float perspectiveCorrect(Vec3f bary,Vec3f *vs){
    float z_interpolated;
    z_interpolated=1.f/(bary.x/vs[0].z+bary.y/vs[1].z+bary.z/vs[2].z);
    return z_interpolated;
}

void drawTriangle(Vec4f *vss,Vec2f *uvs,float *zbuffer,TGAImage &image,float* vintensity,std::vector<Vec3f>&framebuffer){
    Vec3f vs[3];
    vs[0]=toHomogenize(vss[0]);
    vs[1]=toHomogenize(vss[1]);
    vs[2]=toHomogenize(vss[2]);
    int minx=clamp<int>(std::min(vs[0].x,std::min(vs[1].x,vs[2].x)),0,WIDTH-1);
    int maxx=clamp<int>(std::max(vs[0].x,std::max(vs[1].x,vs[2].x)),0,WIDTH-1);
    int miny=clamp<int>(std::min(vs[0].y,std::min(vs[1].y,vs[2].y)),0,HEIGHT-1);
    int maxy=clamp<int>(std::max(vs[0].y,std::max(vs[1].y,vs[2].y)),0,HEIGHT-1);
    for(int i=minx;i<=maxx;i++){
        for(int j=miny;j<=maxy;j++){
            Vec3f bary= computeBary(vs,Vec2f((float)i,(float)j));
            if(bary.x>=0.0f&&bary.y>=0.0f&&bary.z>=0.0f){
                int idx=i+WIDTH*j;
                float z_interpolated= perspectiveCorrect(bary,vs);//bary.x*vs[0].z+bary.y*vs[1].z+bary.z*vs[2].z;
                float intensity=bary.x*vintensity[0]+bary.y*vintensity[1]+bary.z*vintensity[2];//z_interpolated*(bary.x*vintensity[0]/vs[0].z,bary.x*vintensity[1]/vs[1].z,bary.x*vintensity[2]/vs[2].z);//利用透视矫正插值计算强度
                Vec2f uv= computeUV(uvs,bary,z_interpolated,vs);
                TGAColor color=model->getColor(uv);
                if(z_interpolated>zbuffer[idx]){
                    zbuffer[idx]=z_interpolated;
                    image.set(i,j,TGAColor(color.r*intensity,color.g*intensity,color.b*intensity,255));
                    framebuffer[idx]=Vec3f(color.r*intensity,color.g*intensity,color.b*intensity);
                }
            }
        }
    }
}
int main(int argc, char** argv) {

    if(2==argc){
        model=new Model(argv[1]);
    }else{
        model=new Model("C:\\Users\\29839\\Desktop\\ComputerGraphics\\Renderer\\EniRenderer\\obj\\african_head\\african_head.obj");
        model->load_texture("C:\\Users\\29839\\Desktop\\ComputerGraphics\\Renderer\\EniRenderer\\obj\\african_head\\african_head_diffuse.tga",model->DiffuseMap);
    }
	TGAImage image(WIDTH, HEIGHT, TGAImage::RGB);
    TGAImage depthImage(WIDTH,HEIGHT,TGAImage::RGB);
    zbuffer=new float[WIDTH*HEIGHT];
    for(int i=0;i<WIDTH*HEIGHT;i++){
        zbuffer[i]=-FLT_MAX;
        framebuffer.push_back(Vec3f(0,0,0));
    }
    for(int i=0;i<WIDTH;i++){
        for(int j=0;j<HEIGHT;j++){
            int idx=i+j*WIDTH;
            float depthValue=clamp<float>(zbuffer[idx]*255.f,-255.f,0.0f);
            depthValue+=255.f;
            TGAColor depthColor=TGAColor(depthValue,depthValue,depthValue,255);
            depthImage.set(i,j,depthColor);
        }
    }
    int key=0;
    float zNear=0.01;
    float zFar=10.0f;
    float fov=30;
    float angle=0.0f;
    float forwardValue=0.0f;
    float XValue=0.0f;
    int frame_count=0;
    bool P=false;
    Matrix projection= get_projection_Matrix(aspect,zNear ,zFar,fov);
    Vec4f centerP(0,0,0,1);
    while(key != 27)//RenderLoop
    {
        for(int i=0;i<WIDTH*HEIGHT;i++){
            zbuffer[i]=-1*FLT_MAX;
            framebuffer[i]=Vec3f(0,0,0);
        }
        Matrix rotation= get_rotation_Matrix(Vec3f(0,1,0),angle);
        camera=rotation*camera;
        camera.z+=forwardValue;
        camera.x+=XValue;
        angle=0.0f;
        forwardValue=0.0f;
        XValue=0.0f;
        Matrix view= LookAt(camera,center,up);
        for(int i=0;i<model->nfaces();i++){
            std::vector<int> face=model->getFace(i);
            Vec4f vertices[3];
            Vec2f uvs[3];
            float intensity[3];
            for(int j=0;j<face.size();j++){
                Vec4f v=toVec4(model->getVertex(face[j]));
                vertices[j]=view*v;
                //assert(vertices[j].z<=0);
                if(P){
                    vertices[j]=projection*vertices[j];
                }
                uvs[j]=model->getTexureCoord(i,j);
                intensity[j]= clamp<float>(model->getNormal(i,j)*light_dir,0.0,1.0);
            }
            drawTriangle(vertices,uvs,zbuffer,image,intensity,framebuffer);

        }
        cv::Mat CVimage(800, 800, CV_32FC3, framebuffer.data());
        CVimage.convertTo(CVimage, CV_8UC3, 1.0f);
        cv::cvtColor(CVimage, CVimage, cv::COLOR_RGB2BGR);

        cv::imshow("image", CVimage);
        cv::imwrite("output.png", CVimage);
        key = cv::waitKey(10);
        if (key == 'a' ){
            angle =- 10;
        }else if (key == 'd'){
            angle = 10;
        }else if(key=='w'){
            forwardValue=-1.0;
        }else if(key=='s'){
            forwardValue=1.0;
        }else if(key=='j'){
            XValue=-0.1;
        }else if(key=='k'){
            XValue=0.1;
        }else if(key=='l'){
            fov+=2.0;
        }else if(key==';'){
            fov-=2.0;
        }else if(key=='p'){
            P=!P;
        }
    }
//	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
    depthImage.flip_vertically();
    depthImage.write_tga_file("DepthImage.tga");
    delete model;
    delete[] zbuffer;
	return 0;
}

