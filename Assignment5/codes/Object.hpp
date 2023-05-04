/*
这是一个名为Object的类，作为光线追踪中物体的基类，包含一些虚函数和一些成员变量，用于描述物体的属性和行为。

类的成员变量包括：

- materialType：材质类型，包括DIFFUSE（漫反射）, MIRROR（镜面反射）, GLASS(折射/透明)等。
- ior：折射率（即折射光线的速度相对于真空中的速度）。
- Kd：漫反射系数（浅色的物体可能更具散射性，而不具有镜面反射的能力）。
- Ks：镜面反射系数（在物体表面上具有光泽的区域）。
- diffuseColor：物体的漫反射颜色（通过将光线散射到周围来混合反射光）。
- specularExponent：物体表面上的高光反射效果的强度。

该类还包含以下纯虚函数：

- bool intersect(const Vector3f&, const Vector3f&, float&, uint32_t&, Vector2f&) const：检查该对象是否与射线相交。
- void getSurfaceProperties(const Vector3f&, const Vector3f&, const uint32_t&, const Vector2f&, Vector3f&, Vector2f&) const：返回给定点表面的颜色和法向量。

该类还有一个虚函数：

- Vector3f evalDiffuseColor(const Vector2f&) const：返回物体的漫反射颜色。
*/

#pragma once

#include "Vector.hpp"
#include "global.hpp"

class Object
{
public:
    Object()
        : materialType(DIFFUSE_AND_GLOSSY)
        , ior(1.3)
        , Kd(0.8)
        , Ks(0.2)
        , diffuseColor(0.2)
        , specularExponent(25)
    {}

    virtual ~Object() = default;

    virtual bool intersect(const Vector3f&, const Vector3f&, float&, uint32_t&, Vector2f&) const = 0;

    virtual void getSurfaceProperties(const Vector3f&, const Vector3f&, const uint32_t&, const Vector2f&, Vector3f&,
                                      Vector2f&) const = 0;

    virtual Vector3f evalDiffuseColor(const Vector2f&) const
    {
        return diffuseColor;
    }

    // material properties
    MaterialType materialType;
    float ior;
    float Kd, Ks;
    Vector3f diffuseColor;
    float specularExponent;
};
