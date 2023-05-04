/*
这段代码实现了一个MeshTriangle类，用于表示具有多个三角形面的网格模型。

rayTriangleIntersect()函数实现了光线和三角形的相交检测，并更新tnear、u和v的值来表示与光线相交的三角形。该函数的参数包括三个顶点坐标(v0、v1和v2)、光线的起点和方向(orig和dir)，以及tnear、u和v参数。

MeshTriangle类包含以下成员变量：

- vertices：每个顶点的三维坐标数组(指向std::unique_ptr)。
- vertexIndex：表示每个三角形的三个顶点在vertices数组中的索引的数组(指向std::unique_ptr)。
- numTriangles：该网格模型中三角形的数量。
- stCoordinates：每个顶点的纹理坐标数组(指向std::unique_ptr)。

MeshTriangle类还实现了Object接口提供的方法，包括：

- intersect()函数：通过迭代检测光线和三角形来确定与光线相交的三角形，返回true或false。
- getSurfaceProperties()函数：返回相交点的法线和纹理坐标。
- evalDiffuseColor()函数：返回网格的漫反射颜色。

需要注意的是，这里的evalDiffuseColor()函数使用了一个简单的模式来生成网格的颜色，但在实际应用中，通常需要实现更复杂的纹理映射算法以获得更真实的外观。
*/

#pragma once

#include "Object.hpp"

#include <cstring>

bool rayTriangleIntersect(const Vector3f& v0, const Vector3f& v1, const Vector3f& v2, const Vector3f& orig,
                          const Vector3f& dir, float& tnear, float& u, float& v)
{
    // TODO: Implement this function that tests whether the triangle
    // that's specified bt v0, v1 and v2 intersects with the ray (whose
    // origin is *orig* and direction is *dir*)
    // Also don't forget to update tnear, u and v.
    Vector3f E1, E2, S, S1, S2, re;
    E1 = v1 - v0;
    E2 = v2 - v0;
    S = orig - v0;
    S1 = crossProduct(dir, E2);
    S2 = crossProduct(S, E1);
    re = Vector3f(dotProduct(S2, E2), dotProduct(S1, S), dotProduct(S2, dir));
    re = re / dotProduct(S1, E1);
    tnear = re.x;
    u = re.y;
    v = re.z;
    if(tnear > 0 && v >= 0 && v <= 1 && u >= 0 && u <= 1)
        return true;
    return false;
}


class MeshTriangle : public Object
{
public:
    MeshTriangle(const Vector3f* verts, const uint32_t* vertsIndex, const uint32_t& numTris, const Vector2f* st)
    {
        uint32_t maxIndex = 0;
        for (uint32_t i = 0; i < numTris * 3; ++i)
            if (vertsIndex[i] > maxIndex)
                maxIndex = vertsIndex[i];
        maxIndex += 1;
        vertices = std::unique_ptr<Vector3f[]>(new Vector3f[maxIndex]);
        memcpy(vertices.get(), verts, sizeof(Vector3f) * maxIndex);
        vertexIndex = std::unique_ptr<uint32_t[]>(new uint32_t[numTris * 3]);
        memcpy(vertexIndex.get(), vertsIndex, sizeof(uint32_t) * numTris * 3);
        numTriangles = numTris;
        stCoordinates = std::unique_ptr<Vector2f[]>(new Vector2f[maxIndex]);
        memcpy(stCoordinates.get(), st, sizeof(Vector2f) * maxIndex);
    }

    bool intersect(const Vector3f& orig, const Vector3f& dir, float& tnear, uint32_t& index,
                   Vector2f& uv) const override
    {
        bool intersect = false;
        for (uint32_t k = 0; k < numTriangles; ++k)
        {
            const Vector3f& v0 = vertices[vertexIndex[k * 3]];
            const Vector3f& v1 = vertices[vertexIndex[k * 3 + 1]];
            const Vector3f& v2 = vertices[vertexIndex[k * 3 + 2]];
            float t, u, v;
            if (rayTriangleIntersect(v0, v1, v2, orig, dir, t, u, v) && t < tnear)
            {
                tnear = t;
                uv.x = u;
                uv.y = v;
                index = k;
                intersect |= true;
            }
        }

        return intersect;
    }

    void getSurfaceProperties(const Vector3f&, const Vector3f&, const uint32_t& index, const Vector2f& uv, Vector3f& N,
                              Vector2f& st) const override
    {
        const Vector3f& v0 = vertices[vertexIndex[index * 3]];
        const Vector3f& v1 = vertices[vertexIndex[index * 3 + 1]];
        const Vector3f& v2 = vertices[vertexIndex[index * 3 + 2]];
        Vector3f e0 = normalize(v1 - v0);
        Vector3f e1 = normalize(v2 - v1);
        N = normalize(crossProduct(e0, e1));
        const Vector2f& st0 = stCoordinates[vertexIndex[index * 3]];
        const Vector2f& st1 = stCoordinates[vertexIndex[index * 3 + 1]];
        const Vector2f& st2 = stCoordinates[vertexIndex[index * 3 + 2]];
        st = st0 * (1 - uv.x - uv.y) + st1 * uv.x + st2 * uv.y;
    }

    Vector3f evalDiffuseColor(const Vector2f& st) const override
    {
        float scale = 5;
        float pattern = (fmodf(st.x * scale, 1) > 0.5) ^ (fmodf(st.y * scale, 1) > 0.5);
        return lerp(Vector3f(0.815, 0.235, 0.031), Vector3f(0.937, 0.937, 0.231), pattern);
    }

    std::unique_ptr<Vector3f[]> vertices;
    uint32_t numTriangles;
    std::unique_ptr<uint32_t[]> vertexIndex;
    std::unique_ptr<Vector2f[]> stCoordinates;
};
