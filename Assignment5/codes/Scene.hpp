/*
这段代码定义了一个名为Scene的类，用于描述场景。该类包含以下成员变量：

- width：场景宽度，默认值为1280
- height：场景高度，默认值为960
- fov：视野角度，默认值为90度
- backgroundColor：背景色，默认为蓝色
- maxDepth：最大深度，默认值为5
- epsilon：一个浮点数常量，默认值为0.00001

该类包含以下成员函数：

- Add()：用于添加对象或光源，可以接受unique_ptr指针参数
- get_objects()：返回保存对象的数组
- get_lights()：返回保存光源的数组

其中，对象和光源是实现了Object和Light接口的类的实例。在创建场景时，可以使用Add()方法添加对象和光源，然后在其他地方使用场景类的实例来访问这些对象和光源。
*/


#pragma once

#include <vector>
#include <memory>
#include "Vector.hpp"
#include "Object.hpp"
#include "Light.hpp"

class Scene
{
public:
    // setting up options
    int width = 1280;
    int height = 960;
    double fov = 90;
    Vector3f backgroundColor = Vector3f(0.235294, 0.67451, 0.843137);
    int maxDepth = 5;
    float epsilon = 0.00001;

    Scene(int w, int h) : width(w), height(h)
    {}

    void Add(std::unique_ptr<Object> object) { objects.push_back(std::move(object)); }
    void Add(std::unique_ptr<Light> light) { lights.push_back(std::move(light)); }

    [[nodiscard]] const std::vector<std::unique_ptr<Object> >& get_objects() const { return objects; }
    [[nodiscard]] const std::vector<std::unique_ptr<Light> >&  get_lights() const { return lights; }

private:
    // creating the scene (adding objects and lights)
    std::vector<std::unique_ptr<Object> > objects;
    std::vector<std::unique_ptr<Light> > lights;
};
