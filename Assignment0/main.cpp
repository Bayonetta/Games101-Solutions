
#include<cmath>
#include<Eigen/Core>
#include<iostream>

using namespace Eigen;
using namespace std;

int main() {

    Vector3f v(2.0, 1.0, 1.0);
    Matrix3f rm,tm;
    float arc = M_PI_4;

    //旋转矩阵
    rm << cos(arc), -sin(arc), 0,
          sin(arc), cos(arc), 0,
          0, 0, 1;
    //平移矩阵
    tm << 1, 0, 1,
          0, 1, 2,
          0, 0, 1;
    //先旋转后平移
    cout << tm * rm * v << endl;

    return 0;
}

