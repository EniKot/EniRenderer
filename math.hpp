#ifndef ENIRENDERER_GEOMETRY_H
#define ENIRENDERER_GEOMETRY_H
#define PI 3.1415926
#include <cmath>
#include <iostream>
#include <cassert>
#include <vector>
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class t> struct Vec2 {
    t x, y;
    Vec2<t>() : x(t()), y(t()) {}
    Vec2<t>(t _x, t _y) : x(_x), y(_y) {}
 //   Vec2<t>(const Vec2<t> &v) : x(t()), y(t()) { *this = v; }
    Vec2<t> operator +(const Vec2<t> &V) const { return Vec2<t>(x+V.x, y+V.y); }
    Vec2<t> operator -(const Vec2<t> &V) const { return Vec2<t>(x-V.x, y-V.y); }
    Vec2<t> operator *(float f)          const { return Vec2<t>(x*f, y*f); }
    t& operator[](const int i) { if (x<=0) return x; else return y; }
    template <class > friend std::ostream& operator<<(std::ostream& s, Vec2<t>& v);
};


template <class t> struct Vec3 {
   t x,y,z;
    Vec3<t>() : x(0), y(0), z(0) {}
    Vec3<t>(t _x, t _y, t _z) : x(_x),y(_y),z(_z) {}
    Vec3<t>(Vec2<t> vec,t _z) : x(vec.x),y(vec.y),z(_z){}
    Vec3<t>(t _x,Vec2<t> vec) : x(_x),y(vec.y),z(vec.z){}
    template <class u> Vec3<t>(const Vec3<u> &v);
   // Vec3<t>(const Vec3<t> &v) : x(t()), y(t()), z(t()) { *this = v; }//拷贝构造函数
    inline Vec3<t> operator ^(const Vec3<t> &v) const { return Vec3<t>(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x); }
    inline Vec3<t> operator +(const Vec3<t> &v) const { return Vec3<t>(x+v.x, y+v.y, z+v.z); }
    inline Vec3<t> operator -(const Vec3<t> &v) const { return Vec3<t>(x-v.x, y-v.y, z-v.z); }
    inline Vec3<t> operator *(float f)          const { return Vec3<t>(x*f, y*f, z*f); }
    inline t       operator *(const Vec3<t> &v) const { return x*v.x + y*v.y + z*v.z; }
    float norm () const {
        return std::sqrt(x*x+y*y+z*z);
    }
    Vec3<t> & normalize(t l=1) {
        *this = (*this)*(l/norm()); return *this;
    }
    Vec3<t> cross(const Vec3<t> &v) const{
        return Vec3<t>(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x);
    }

    t& operator[](const int i) {
        if(i<=0){
            return x;
        }else if(i==1){
            return y;
        }else{
            return z;
        }
    }
    template <class > friend std::ostream& operator<<(std::ostream& s, Vec3<t>& v);
};
template <class t> struct Vec4 {
    t x,y,z,w;
    Vec4():x(0),y(0),z(0),w(1) {}
    Vec4(t _x,t _y,t _z,t _w):x(_x),y(_y),z(_z),w(_w) {}
    Vec4(Vec3<t> vec,t _w) : x(vec.x),y(vec.y),z(vec.z),w(_w){}
    inline Vec4<t> operator +(const Vec4<t> &v) const { return Vec4<t>(x+v.x, y+v.y, z+v.z,w+v.w); }
    inline Vec4<t> operator -(const Vec4<t> &v) const { return Vec4<t>(x-v.w, y-v.y, z-v.z,w-v.w); }
    inline Vec4<t> operator *(float f) const { return Vec4<t>(x*f, y*f, z*f,w*f); }

    float norm() const {
        return std::sqrt(x*x+y*y+z*z+w*w);
    }
    Vec3<t> toVector3() {
        return Vec3<t>(x/w,y/w,z/w);
    }
    t& operator[](const int idx){
        if(idx<=0){
            return x;
        }else if(idx==1){
            return y;
        }else if(idx==2){
            return z;
        }else{
            return w;
        }
    }
    template <class > friend std::ostream& operator<<(std::ostream& s, Vec4<t>& v);
};
template <class t> struct Quaternion {
    union {
        struct {t w,i,j,k;};
        struct { t ivert, iuv, inorm; };
        t raw[4];
    };
    Quaternion():w(0),i(0),j(0),k(0) {}
    Quaternion(t _w,t _i,t _j,t _k):w(_w),i(_i),j(_k),k(_k) {}
    Quaternion(t _w,Vec3<t> vec) : w(_w),i(vec.x),j(vec.y),k(vec.z){}
    inline Quaternion<t> operator +(const Quaternion<t> &q) const { return Quaternion<t>(w+q.w, i+q.i, j+q.j,k+q.k); }
    inline Quaternion<t> operator -(const Quaternion<t> &q) const { return Quaternion<t>(w-q.w, i-q.i, j-q.j,k-q.k); }
    inline Quaternion<t> operator *(float f) const { return Quaternion<t>(w*f, i*f, j*f,k*f); }
    inline Quaternion<t> operator *(const Quaternion<t> &q) const {
        Vec3<t> v1(i,j,k);
        Vec3<t> v2(q.i,q.j,q.k);
        return Quaternion<t>(w*q.w-v1*v2,w*v2+q.w*v1+v1.cross(v2));
    }
    float norm() const {
        return std::sqrt(w*w+i*i+j*j+k*k);
    }
    Vec3<t> getVector(const Quaternion<t> &q) {
        return Vec3<t>(q.i,q.j,q.k);
    }
    t& operator[](const int idx){
        if(idx<=0){
            return w;
        }else if(idx==1){
            return i;
        }else if(idx==2){
            return j;
        }else{
            return k;
        }
    }
    template <class > friend std::ostream& operator<<(std::ostream& s, Quaternion<t>& q);
};

typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;
typedef Vec3<int> Vec3i;
typedef Vec3<float> Vec3f;
typedef Vec4<float> Vec4f;
typedef Quaternion<float> Quaternionf;


template <> template <> Vec3<int>::Vec3(const Vec3<float> &v);
template <> template <> Vec3<float>::Vec3(const Vec3<int> &v);

template <class t> std::ostream& operator<<(std::ostream& s, Vec2<t>& v) {
    s << "(" << v.x << ", " << v.y << ")\n";
    return s;
}

template <class t> std::ostream& operator<<(std::ostream& s, Vec3<t>& v) {
    s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
    return s;
}
template <class t> std::ostream& operator<<(std::ostream& s, Vec4<t>& v) {
    s << "(" << v.x << ", " << v.y << ", " <<v.z << ", "<<v.w<<")\n";
    return s;
}
template <class t> std::ostream& operator<<(std::ostream& s, Quaternion<t>& q) {
    s << "(" << q.w << ", " << q.i << ", " << q.j << q.k << ")\n";
    return s;
}
Vec4f toVec4(Vec3f v){
    return Vec4f(v.x,v.y,v.z,1.0f);
}
const int DEFAULT_ALLOC=4;

class Matrix{
    std::vector<std::vector<float>> m;
    int rows,cols;
public:
    Matrix(int r=DEFAULT_ALLOC,int c=DEFAULT_ALLOC);
    inline int getRows();
    inline int getCols();

    static Matrix identity(int dimensions);
    std::vector<float>& operator[](const int i);
    Matrix operator*(const Matrix& mat)const;//const:read-only function
    Matrix transpose()const;
    Vec3f operator*(Vec3f& v)const;
    Vec4f operator*(Vec4f& v)const;
    Matrix inverse()const;
    void operator=(const std::vector<std::vector<float>> &array);
    friend std::ostream& operator<<(std::ostream& s,Matrix &m);
};
#endif //ENIRENDERER_GEOMETRY_H
