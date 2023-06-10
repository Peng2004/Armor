#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;
using namespace cv;

void kalman_init();
class Armor
{

public:
    Mat img;
    vector<Point2f> point2d;
    Point2f center;
    Mat Pretreatment();
    vector<vector<Point>> Filtration();
               // 卡尔曼滤波初始化函数
    Point2f kalman_predict(Point2f target_centre); // 卡尔曼预测函数
    void Draw();
    void Draw2();
    double distance,x_angle, y_angle, z_angle; // 算出来的角度和距离
    void Solvepnp();                                    // 利用solvepnp位姿解算
};
