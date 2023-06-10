#include "Armor.h"

void Armor::Solvepnp()
{
    Point2f vertices[4] = {point2d[0], point2d[1], point2d[2], point2d[3]};
    point2d.clear();
    Point2f tl, bl, tr, br;
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
    tl=kalman_predict(tl);
    tl=kalman_predict(tr);
    tl=kalman_predict(br);
    tl=kalman_predict(bl);
    point2d.push_back(tl);
    point2d.push_back(tr);
    point2d.push_back(br);
    point2d.push_back(bl);
    // point2d.push_back(bl);
    // point2d.push_back(br);
    // point2d.push_back(tr);
    // point2d.push_back(tl);
    vector<Point3f> point3d;
    float half_x = 13.4 / 2;
    float half_y = 5.7 / 2;
    // point3d.push_back(Point3f(-half_x, half_y, 0));
    // point3d.push_back(Point3f(half_x, half_y, 0));
    // point3d.push_back(Point3f(half_x, -half_y, 0));
    // point3d.push_back(Point3f(-half_x, -half_y, 0));
    point3d.push_back(Point3f(-half_x, -half_y, 0));
    point3d.push_back(Point3f(half_x, -half_y, 0));
    point3d.push_back(Point3f(half_x, half_y, 0));
    point3d.push_back(Point3f(-half_x, half_y, 0));
    Mat rvec;
    Mat tvec;
    Mat cam_matrix = (Mat_<double>(3, 3) << 1.6041191539594568e+03, 0, 6.3983687194220943e+02,
                      0, 1.6047833493341714e+03, 5.1222951297937527e+02, 0, 0, 1); // 相机参数
    // Mat distortion_coeff = (Mat_<double>(5, 1) << -6.4910709385278609e-01, 8.6914328787426987e-01,
    //                         5.1733428362687644e-03, -4.1111054148847371e-03, 0); // 畸变系数
    Mat distortion_coeff = (Mat_<double>(5, 1) << -6.4910709385278609e-01, 8.6914328787426987e-01,
                            5.1733428362687644e-03, -4.1111054148847371e-03, 0); // 畸变系数
                            cout<<point2d<<endl;
    solvePnPRansac(point3d, point2d, cam_matrix, distortion_coeff, rvec, tvec);
    Mat R;
    Rodrigues(rvec, R);
    // cout<<R<<endl;
    double rx = (atan2(R.at<double>(2, 1), R.at<double>(2, 2))) * 180 / CV_PI;
    double ry = (atan2(-R.at<double>(2, 0), sqrt(pow(R.at<double>(2, 1), 2) + pow(R.at<double>(2, 2), 2)))) * 180 / CV_PI;
    double rz = (atan2(R.at<double>(1, 0), R.at<double>(0, 0))) * 180 / CV_PI;
    double tx = tvec.at<double>(0, 0);
    double ty = tvec.at<double>(1, 0);
    double tz = tvec.at<double>(2, 0);
    double dis = sqrt(tx * tx + ty * ty + tz * tz);
    vector<Point2f> cam;
    vector<Point3f> world = {Point3f(10, 0, 0), Point3f(0, 10, 0), Point3f(0, 0, 10)};
    projectPoints(world, rvec, tvec, cam_matrix, distortion_coeff, cam);
    line(img, center, cam[0], Scalar(255, 0, 0),4);
    line(img, center, cam[1], Scalar(0, 255, 0),4);
    line(img, center, cam[2], Scalar(0, 0, 255),4);
    putText(img,"X",cam[0],2,1,Scalar(255,0,0));
        putText(img,"Y",cam[1],2,1,Scalar(0,255,0));
            putText(img,"Z",cam[2],2,1,Scalar(0,0,255));
    this->distance = dis;
    this->x_angle = rx;
    this->y_angle = ry;
    this->z_angle = rz;
}