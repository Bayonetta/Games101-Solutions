/*
这段代码创建了一个场景，场景包含两个球体和一个四边形网格，以及两个光源。
设置了渲染选项，包括图片的宽度和高度、最大递归深度、视野等参数。最后调用渲染函数进行渲染。
具体来说，球体1位于(-1,0,-12)，半径为2，材质类型为散射和光泽，散射颜色为(0.6,0.7,0.8)；球体2位于(0.5,-0.5,-8)，半径为1.5，折射率为1.5，材质类型为反射和折射；
四边形网格为一个平面，四个顶点分别为(-5,-3,-6)、(5,-3,-6)、(5,-3,-16)、(-5,-3,-16)，材质类型为散射和光泽。两个光源分别位于(-20,70,20)和(30,50,-12)，强度均为0.5。
*/

#include "Scene.hpp"
#include "Sphere.hpp"
#include "Triangle.hpp"
#include "Light.hpp"
#include "Renderer.hpp"

// In the main function of the program, we create the scene (create objects and lights)
// as well as set the options for the render (image width and height, maximum recursion
// depth, field-of-view, etc.). We then call the render function().
int main()
{
    Scene scene(1280, 960);

    auto sph1 = std::make_unique<Sphere>(Vector3f(-1, 0, -12), 2);
    sph1->materialType = DIFFUSE_AND_GLOSSY;
    sph1->diffuseColor = Vector3f(0.6, 0.7, 0.8);

    auto sph2 = std::make_unique<Sphere>(Vector3f(0.5, -0.5, -8), 1.5);
    sph2->ior = 1.5;
    sph2->materialType = REFLECTION_AND_REFRACTION;

    scene.Add(std::move(sph1));
    scene.Add(std::move(sph2));

    Vector3f verts[4] = {{-5,-3,-6}, {5,-3,-6}, {5,-3,-16}, {-5,-3,-16}};
    uint32_t vertIndex[6] = {0, 1, 3, 1, 2, 3};
    Vector2f st[4] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    auto mesh = std::make_unique<MeshTriangle>(verts, vertIndex, 2, st);
    mesh->materialType = DIFFUSE_AND_GLOSSY;

    scene.Add(std::move(mesh));
    scene.Add(std::make_unique<Light>(Vector3f(-20, 70, 20), 0.5));
    scene.Add(std::make_unique<Light>(Vector3f(30, 50, -12), 0.5));    

    Renderer r;
    r.Render(scene);

    return 0;
}
