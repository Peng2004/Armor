#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;
using namespace cv;

class Armor
{
private:
    Mat img;
    RotatedRect rect2d;

public:
    Armor(Mat _img) { img = _img; }
    Mat getimg() { return img; }
    Mat Pretreatment();
    vector<Point> Filtration();
    void kalman_init();                            // 卡尔曼滤波初始化函数
    Point2f kalman_predict(Point2f target_centre); // 卡尔曼预测函数
    void Draw();
    void Draw2();
    double distance,x_angle, y_angle, z_angle; // 算出来的角度和距离
    void Solvepnp();                                    // 利用solvepnp位姿解算
};
