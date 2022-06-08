#ifndef ENIRENDERER_MODEL_H
#define ENIRENDERER_MODEL_H

#include <vector>
#include "math.hpp"
#include "tgaimage.h"
class Model {
public:
    std::vector<Vec3f> verts_;
    std::vector<std::vector<Vec3i> > faceDatas;//面构成
    std::vector<Vec3f> faceNormals;//面法线
    std::vector<Vec3f> vertexNormals;
    std::vector<Vec2f> vertexTex;
    TGAImage DiffuseMap;
    void load_texture(std::string filename,TGAImage &image);

    Model(const char *filename);
    ~Model();
    int nverts();
    int nfaces();
    Vec3f getNormal(int face_idx,int nvert);
    Vec3f getVertex(int idx);
    Vec2f getTexureCoord(int iface,int nvert);
    TGAColor getColor(Vec2f uv);
    std::vector<int> getFace(int idx);
};

#endif //ENIRENDERER_MODEL_H
