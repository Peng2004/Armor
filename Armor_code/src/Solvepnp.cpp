#include "Armor.h"

void Armor::Solvepnp()
{
    Mat rot_vector, tveclation_vector;
    vector<Point2f> object2d_point;
    Point2f vertices[4];
    rect2d.points(vertices); // 把矩形的四个点复制给四维点向量
    cv::Point2f tl, bl, tr, br;
    sort(vertices, vertices + 4, [](const Point2f &p1, const Point2f &p2)
         { return p1.x < p2.x; }); // 从4个点的第一个到最后一个进行排序
    if (vertices[0].y < vertices[1].y)
    {
        tl = vertices[0];
        bl = vertices[1];
    }
    else
    {
        tl = vertices[1];
        bl = vertices[0];
    }
    if (vertices[2].y < vertices[3].y)
    {
        tr = vertices[2];
        br = vertices[3];
    }
    else
    {
        tr = vertices[3];
        br = vertices[2];
    }
    object2d_point.push_back(tl);
    object2d_point.push_back(tr);
    object2d_point.push_back(br);
    object2d_point.push_back(bl);
    vector<Point3f> point3d;
    float half_x = 13.4 / 2;
    float half_y = 5.7 / 2;
    point3d.push_back(Point3f(-half_x, half_y, 0));
    point3d.push_back(Point3f(half_x, half_y, 0));
    point3d.push_back(Point3f(half_x, -half_y, 0));
    point3d.push_back(Point3f(-half_x, -half_y, 0));
    Mat rvec;
    Mat tvec;
    Mat target_rvec;
    Mat cam_matrix = (Mat_<double>(3, 3) << 1.6041191539594568e+03, 0, 6.3983687194220943e+02,
                      0, 1.6047833493341714e+03, 5.1222951297937527e+02, 0, 0, 1); // 相机参数
    Mat distortion_coeff = (Mat_<double>(5, 1) << -6.4910709385278609e-01, 8.6914328787426987e-01,
                            5.1733428362687644e-03, -4.1111054148847371e-03, 0); // 畸变系数
    solvePnP(point3d, object2d_point, cam_matrix, distortion_coeff, target_rvec, tvec);
    // Mat R;
    // Rodrigues(rvec, R);
    // Mat target_R = -R.t();
    // Mat target_rvec;
    // Rodrigues(target_R, target_rvec);
    double X_angle = -atan2(target_rvec.at<double>(2, 1), target_rvec.at<double>(2, 2)) * 180 / CV_PI;
    double Y_angle = -atan2(-target_rvec.at<double>(2, 0), sqrt(target_rvec.at<double>(2, 1) * target_rvec.at<double>(2, 1) + target_rvec.at<double>(2, 2) * target_rvec.at<double>(2, 2))) * 180 / CV_PI;
    double Z_angle = -atan2(target_rvec.at<double>(1, 0), target_rvec.at<double>(0, 0)) * 180 / CV_PI;
    double tx = tvec.at<double>(0, 0);
    double ty = tvec.at<double>(1, 0);
    double tz = tvec.at<double>(2, 0);
    double dis = sqrt(tx * tx + ty * ty + tz * tz);
    // double yaw_angle = atan(tx / ty) * 180.0 / M_PI;
    // double pitch_angle = atan(tz / ty) * 180.0 / M_PI;
    this->distance = dis;
    this->x_angle = X_angle;
    this->y_angle = Y_angle;
    this->z_angle = Z_angle;
    // this->yaw_angle_pnp = yaw_angle;
}