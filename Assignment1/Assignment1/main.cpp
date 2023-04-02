#include "Triangle.hpp"
#include "rasterizer.hpp"
#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>

constexpr double MY_PI = 3.1415926;

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();
 
    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -eye_pos[0], 0, 1, 0, -eye_pos[1], 0, 0, 1,
        -eye_pos[2], 0, 0, 0, 1;

    view = translate * view;

    return view;
}

Eigen::Matrix4f get_model_matrix(float rotation_angle)
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    // TODO: Implement this function
    // Create the model matrix for rotating the triangle around the Z axis.
    // Then return it.
    Eigen::Matrix4f rotation;
    float angle = rotation_angle * M_PI / 180;

    rotation << cos(angle), -sin(angle), 0, 0, sin(angle), cos(angle),0, 0,
        0, 0, 1, 0, 0, 0, 0, 1;
    model = rotation * model;

    return model;
}

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio,
                                      float zNear, float zFar)
{
    // Students will implement this function

    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();

    // TODO: Implement this function
    // Create the projection matrix for the given parameters.
    // Then return it.
    float fov_angle = eye_fov * M_PI / 180;

    float n = -zNear;
    float t = tan(fov_angle / 2) * zNear; 
    float b = -t;
    float r = t * aspect_ratio;
    float l = -r;
    float f = -zFar;

    projection << 2 * n /(r - l), 0, (l + r)/(l - r) , 0,
        0, 2 * n /(t-b), (b + r)/(b - t), 0,
        0, 0, (f + n)/(n - f), 2 * f * n / (f-n),
        0, 0 ,1, 0;    
    return projection;
}

//利用罗德里格斯旋转公式得到绕过原点任意直线的旋转矩阵
Eigen::Matrix4f get_rotation(Vector3f axis, float angle) {
    Eigen::Matrix3f I = Eigen::Matrix3f::Identity();
    float ra = angle * M_PI / 180;
    Eigen::Matrix3f R, N;

    N << 0, -axis.z(), axis.y(),
         axis.z(), 0, -axis.x(),
         -axis.y(), axis.x(), 0;

    R << cosf(ra) * I + (1 - cosf(ra)) * axis * axis.transpose() + sinf(ra) * N;

    Eigen::Matrix4f R_homo;

    /*将旋转矩阵扩展成齐次坐标形式*/
    for(int i = 0; i < 4; i++){
        for(int j = 0;j < 4; j++){
            if(i == 3 || j == 3){
                if(i!=j)
                    R_homo(i, j)=0;
                else
                    R_homo(i, j)=1;
            }
            else{
                R_homo(i, j)=R(i, j);
            }
        }
    }
    return R_homo;
}


int main(int argc, const char** argv)
{
    float angle = 0;
    bool command_line = false;
    bool rodrigue_rotation = false;
    std::string filename = "output.png";
    
    Eigen::Matrix4f rodrigueM;

    if (argc >= 3) {
        if (std::string(argv[1]) == "-r") {
            command_line = true;
            angle = std::stof(argv[2]); // -r by default
            if (argc == 4) {
                filename = std::string(argv[3]);
            }
        } else if (std::string(argv[1]) == "-c" && argc == 6) {
            angle = std::stof(argv[5]);
            rodrigue_rotation = true;
            Eigen::Vector3f axis(std::stof(argv[2]), std::stof(argv[3]), std::stof(argv[4]));
            rodrigueM = get_rotation(axis, angle);
        }
    }

    rst::rasterizer r(700, 700);

    Eigen::Vector3f eye_pos = {0, 0, 5};

    std::vector<Eigen::Vector3f> pos{{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};

    std::vector<Eigen::Vector3i> ind{{0, 1, 2}};

    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);

    int key = 0;
    int frame_count = 0;

    if (command_line) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);

        cv::imwrite(filename, image);

        return 0;
    }

    while (key != 27) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));

        if (rodrigue_rotation) {
           r.set_projection(get_projection_matrix(45, 1, 0.1, 50) * rodrigueM);
        } else {
           r.set_projection(get_projection_matrix(45, 1, 0.1, 50));
        }

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::imshow("image", image);
        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';

        if (key == 'a') {
            angle += 10;
        }
        else if (key == 'd') {
            angle -= 10;
        }
    }

    return 0;
}
