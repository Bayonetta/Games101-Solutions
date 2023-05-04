/*
这段C++代码定义了一个名为Sphere的类，表示一个球体物体，它继承自Object类，并实现了虚函数intersect和getSurfaceProperties。

成员变量：

- center: 类型为 Vector3f，表示球体的中心坐标。
- radius: 类型为 float，表示球体的半径。
- radius2: 类型为 float，表示球体的半径的平方值。

成员函数：

- Sphere(const Vector3f& c, const float& r): 构造函数，使用给定的中心坐标和半径初始化球体。
- bool intersect(const Vector3f& orig, const Vector3f& dir, float& tnear, uint32_t&, Vector2f&) const override: 实现虚函数intersect，用于检测光线与球体是否相交。

- const Vector3f& orig: 光线的起始点。
- const Vector3f& dir: 光线的方向。
- float& tnear: 离光线起点最近的相交距离。
- uint32_t&: 暂不使用。
- Vector2f&: 暂不使用。

- void getSurfaceProperties(const Vector3f& P, const Vector3f&, const uint32_t&, const Vector2f&, Vector3f& N, Vector2f&) const override: 实现虚函数getSurfaceProperties，用于获取球体表面属性。

- const Vector3f& P: 相交点的位置。
- const Vector3f&: 暂不使用。
- const uint32_t&: 暂不使用。
- const Vector2f&: 暂不使用。
- Vector3f& N: 获得相交点法向量。
- Vector2f&: 暂不使用。

这段代码通过解析一个二次方程，判断光线与球体是否相交。如果相交，返回距离最近的相交点，并返回该点的法向量。

总的来说，这段代码是光线跟踪算法中的关键之一，用于实现光线与球体的交互操作。
*/


#pragma once

#include "Object.hpp"
#include "Vector.hpp"

class Sphere : public Object
{
public:
    Sphere(const Vector3f& c, const float& r)
        : center(c)
        , radius(r)
        , radius2(r * r)
    {}

    bool intersect(const Vector3f& orig, const Vector3f& dir, float& tnear, uint32_t&, Vector2f&) const override
    {
        // analytic solution
        Vector3f L = orig - center;
        float a = dotProduct(dir, dir);
        float b = 2 * dotProduct(dir, L);
        float c = dotProduct(L, L) - radius2;
        float t0, t1;
        if (!solveQuadratic(a, b, c, t0, t1))
            return false;
        if (t0 < 0)
            t0 = t1;
        if (t0 < 0)
            return false;
        tnear = t0;

        return true;
    }

    void getSurfaceProperties(const Vector3f& P, const Vector3f&, const uint32_t&, const Vector2f&,
                              Vector3f& N, Vector2f&) const override
    {
        N = normalize(P - center);
    }

    Vector3f center;
    float radius, radius2;
};
