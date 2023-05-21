#include "Armor.h"

Mat Armor::Pretreatment()
{
    Mat hsv;
    img.copyTo(hsv);
    cvtColor(img, hsv, COLOR_BGR2HSV);
    inRange(hsv, Scalar(11, 43, 46), Scalar(34, 255, 255), hsv);
    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
    erode(hsv, hsv, element, Point(-1, -1), 1);
    dilate(hsv, hsv, element, Point(-1, -1), 1);
    return hsv;
}

vector<Point> Armor::Filtration()
{
    Mat gray;
    gray = Pretreatment();
    vector<vector<Point>> contours;
    vector<Point> contours_final;
    vector<Vec4i> hierachy;
    vector<int> erase_temp;
    findContours(gray, contours, hierachy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    for (size_t i = 0; i < contours.size(); i++)
    {
        double area = contourArea(contours[i]);
        if (area > 200)
        {
            for (size_t j = 0; j < contours[i].size(); j++)
            {
                contours_final.push_back(contours[i][j]);
            }
        }
    }

    // cout << contours.size() << endl;
    //  bool judge = true;
    //  contours_temp = contours;
    //  for (int i = 0; i < contours.size(); i++)//长宽比筛选
    //  {
    //          RotatedRect minArea = minAreaRect(contours[i]);
    //          float height = minArea.size.height;
    //          float width = minArea.size.width;
    //          if (height < width)
    //          {
    //                  swap(height, width);
    //          }
    //          float rate = height / width;
    //          //cout << "过滤算法r" <<rate<< endl;
    //          if (rate < 3.3 || rate > 5)
    //          {
    //                  //contours.erase(contours.begin() + (erase_temp[i] - i));
    //                  erase_temp.push_back(i);
    //          }
    //  }

    // int num = erase_temp.size();
    // for (int i = 0; i < num; i++)
    // {
    //         contours.erase(contours.begin() + (erase_temp[i] - i));
    // }
    // if (contours.size() < 2)//轮廓集为空则判定筛除失败
    // {
    //         //cout << "长宽比筛选失败" << endl;
    //         return contours_temp;
    // }
    // contours_temp = contours;
    // erase_temp.clear();
    // for (int i = 0; i < contours.size(); i++)//轮廓和轮廓最小矩形的面积比
    // {
    //         RotatedRect minArea = minAreaRect(contours[i]);
    //         float area = contourArea(contours[i]);
    //         float rate = minArea.size.area() / area;
    //         if (rate < 1 || rate > 1.65)
    //         {
    //                 erase_temp.push_back(i);
    //         }
    // }
    // num = erase_temp.size();
    // for (int i = 0; i < num; i++)
    // {
    //         contours.erase(contours.begin() + (erase_temp[i] - i));
    // }
    // if (contours.size() < 2)
    // {
    //         //cout << "面积比筛选失败" << endl;
    //         return contours_temp;
    // }
    // contours_temp = contours;
    // erase_temp.clear();
    // for (int i = 0; i < contours.size(); i++)//凸度筛选
    // {
    //         vector<vector<Point>>HullPoint(contours.size());
    //         float area = contourArea(contours[i]);
    //         convexHull(Mat(contours[i]), HullPoint[i], false);
    //         float solidity = contourArea(HullPoint[i]) / area;
    //         if (solidity < 0.5)
    //         {
    //                 erase_temp.push_back(i);
    //         }
    // }
    // num = erase_temp.size();
    // for (int i = 0; i < num; i++)
    // {
    //         contours.erase(contours.begin() + (erase_temp[i] - i));
    // }
    // if (contours.size() < 2)
    // {
    //         //cout << "凸度筛选失败" << endl;
    //         return contours_temp;
    // }
    // erase_temp.clear();
    // contours_temp = contours;
    // for (int i = 0; i < contours.size(); i++)//高度差筛选
    // {
    //         int erase = 1;
    //         RotatedRect minArea1 = minAreaRect(contours[i]);
    //         for (int j = 0; j < contours.size(); j++)//两两对比高度差
    //         {
    //                 if (i == j)
    //                 {
    //                         continue;
    //                 }
    //                 RotatedRect minArea2 = minAreaRect(contours[j]);
    //                 float minus = abs(minArea1.center.y - minArea2.center.y);//计算两个轮廓之间的高度差
    //                 if (minus < 32)
    //                 {
    //                         erase = 0;
    //                 }
    //         }
    //         if (erase == 1)
    //         {
    //                 erase_temp.push_back(i);
    //         }
    // }
    // num = erase_temp.size();
    // for (int i = 0; i < num; i++)
    // {
    //         contours.erase(contours.begin() + (erase_temp[i] - i));
    // }
    // if (contours.size() < 2)
    // {
    //         //cout << "高度差筛选失败" << endl;
    //         return contours_temp;
    // }
    return contours_final;
}

void Armor::Draw()
{
    Mat dst;
    img.copyTo(dst);
    vector<Point> contours = Filtration();
    if (contours.empty())
    {
        cout << "omg";
    }
    rect2d = minAreaRect(contours);
    Point2f points[4];
    rect2d.points(points);
    line(dst, points[0], points[1], Scalar(255, 255, 255), 2, 8, 0);
    line(dst, points[2], points[1], Scalar(255, 255, 255), 2, 8, 0);
    line(dst, points[2], points[3], Scalar(255, 255, 255), 2, 8, 0);
    line(dst, points[0], points[3], Scalar(255, 255, 255), 2, 8, 0);
    line(dst, points[0], points[2], Scalar(255, 255, 255), 2, 8, 0);
    line(dst, points[1], points[3], Scalar(255, 255, 255), 2, 8, 0);
    circle(dst, rect2d.center, 5, Scalar(0, 0, 255), -1);
    imshow("test", dst);
}

void Armor::Draw2()
{
    Mat dst(Size(400, 400), CV_8UC3, Scalar(0));
    Solvepnp();
    namedWindow("math", WINDOW_AUTOSIZE);
    putText(dst, "distance:" + to_string(distance) + "cm", Point(10, 100), 2, 1, Scalar(255, 255, 255));
    putText(dst, "x_angle:" + to_string(x_angle), Point(10, 200), 2, 1, Scalar(255, 255, 255));
    putText(dst, "y_angle:" + to_string(y_angle), Point(10, 300), 2, 1, Scalar(255, 255, 255));
    putText(dst, "z_angle:" + to_string(z_angle), Point(10, 400), 2, 1, Scalar(255, 255, 255));
    imshow("math", dst);
}