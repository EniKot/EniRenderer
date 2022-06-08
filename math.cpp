//
// Created by 29839 on 2022/5/15.
//
#include "math.hpp"



template <> template <> Vec3<int>::Vec3(const Vec3<float> &v) : x(int(v.x+.5)), y(int(v.y+.5)), z(int(v.z+.5)) {}

template <> template <> Vec3<float>::Vec3(const Vec3<int> &v) : x(v.x), y(v.y), z(v.z) {}

Matrix::Matrix(int r, int c) : m(std::vector<std::vector<float> >(r, std::vector<float>(c, 0.f))), rows(r), cols(c) { }
int Matrix::getRows(){
    return rows;
}
int Matrix::getCols(){
    return cols;
}
Matrix Matrix::identity(int dimensions) {
    Matrix E(dimensions,dimensions);
    for(int i=0;i<dimensions;i++){
        for(int j=0;j<dimensions;j++){
            E[i][j]=(i==j?1.0f:0.0f);
        }
    }
    return E;
}

void Matrix::operator=(const std::vector<std::vector<float>> &array) {
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            m[i][j]=array[i][j];
        }
    }
}
std::vector<float>& Matrix::operator[](const int i) {
    return m[i];
}
Matrix Matrix::operator*(const Matrix &mat) const{
    assert(cols==mat.rows);
    Matrix result(rows,mat.cols);
    for(int i=0;i<rows;i++){
        for(int j=0;j<mat.cols;j++){
            result[i][j]=0.0f;
            for(int k=0;k<mat.rows;k++){
                result[i][j]+=m[i][k]*mat.m[k][j];
            }
        }
    }
    return result;
}
Vec3f Matrix::operator*(Vec3f& v)const{
    assert(cols==3);
    Vec3f result(0,0,0);
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            result[i]+=v[j]*m[i][j];
        }
    }
    return result;
}
Vec4f Matrix::operator*(Vec4f& v)const{
    assert(cols==4);
    Vec4f result(0,0,0,0);
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            result[i]+=v[j]*m[i][j];
        }
    }
    return result;
}
Matrix Matrix::transpose() const{
    Matrix result(rows,cols);
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            result[j][i]=m[i][j];
        }
    }
    return result;
}
Matrix Matrix::inverse() const{//矩阵求逆
    assert(rows==cols);
    // if(rows!=cols){
    //   return NULL;
    //  }
    if(rows==cols&&rows==1){
        Matrix re(1,1);
        re[0][0]=m[0][0];
        return re;
    }
    Matrix temp(rows,cols*2);
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            temp[i][j]=m[i][j];
            temp[i][j+cols]=(i==j?1.0f:0.0f);
        }
    }
    for(int i=0;i<cols-1;i++){
        float t=temp[i][i];
        for(int k=i;k<cols*2;k++){
            temp[i][k]/=t;
        }
        for(int j=i+1;j<rows;j++){
            float t1=temp[j][i];
            if(t1==0.0)continue;
            for(int k=i;k<cols*2;k++){
                temp[j][k]/=t1;
                temp[j][k]-=temp[i][k];
            }
        }
    }
    float t2=temp[rows-1][cols-1];
    for(int i=0;i<cols*2;i++){
        temp[rows-1][i]/=t2;
    }
    for(int i=cols-2;i>=0;i--){
        for(int j=i;j>=0;j--){
            float t3=temp[j][i+1];
            for(int k=cols*2-1;k>=i+1;k--){
                temp[j][k]-=t3*temp[i+1][k];
            }
        }
    }
    Matrix result(rows,cols);
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            result[i][j]=temp[i][j+cols];
        }
    }
    return result;
}
std::ostream& operator<<(std::ostream& s, Matrix& m) {
    for (int i=0; i<m.getRows(); i++)  {
        for (int j=0; j<m.getCols(); j++) {
            s << m[i][j];
            if (j<m.getCols()-1) s << "\t";
        }
        s << "\n";
    }
    return s;
}

