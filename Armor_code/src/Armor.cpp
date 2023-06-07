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

vector<vector<Point>> Armor::Filtration()
{
    Mat gray;
    gray = Pretreatment();
    vector<vector<Point>> contours;
    vector<vector<Point>> contours_final;
    vector<Vec4i> hierachy;
    vector<int> erase_temp;
    findContours(gray, contours, hierachy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    for (size_t i = 0; i < contours.size(); i++)
    {
        double area = contourArea(contours[i]);
        if (area > 100)
        {
            contours_final.push_back(contours[i]);

        }
    }
    if(contours_final.size()!=2)
    {
        RotatedRect rect_test;
        contours=contours_final;
        contours_final.clear();
        for(size_t i=0;i<contours.size();i++)
        {
            int ratio=0;
            rect_test=minAreaRect(contours[i]);
            if(rect_test.size.width>rect_test.size.height)
            {
                ratio=rect_test.size.width/rect_test.size.height;
            }
            else 
            {
                ratio=rect_test.size.height/rect_test.size.width;
            }
            if(ratio>=3&&ratio<=15)
            {
                contours_final.push_back(contours[i]);
            }
        }

    }
    assert(contours_final.size()==2);
    return contours_final;
    

}

void Armor::Draw()
{
    Mat dst;
    img.copyTo(dst);
    vector<vector<Point>> contours = Filtration();
    if (contours.empty() == 0)
    {
        RotatedRect rect;
        for(size_t i=0;i<contours.size();i++)
        {
            Point2f point[4];
            Point2f t,b;
            rect = minAreaRect(contours[i]);
            rect.points(point);
            sort(point, point + 4, [](const Point2f &p1, const Point2f &p2)
            { return p1.y < p2.y; });
            t.x=(point[0].x+point[1].x)/2;
            t.y=(point[0].y+point[1].y)/2;
            b.x=(point[2].x+point[3].x)/2;
            b.y=(point[2].y+point[3].y)/2;
            point2d.push_back(t);
            point2d.push_back(b);
        }

    }
    //计算中心点坐标
    double k1 = (double)(point2d[3].y - point2d[0].y) / (double)(point2d[3].x - point2d[0].x);
    double b1 = point2d[0].y - k1 * point2d[0].x;
    double k2 = (double)(point2d[1].y - point2d[2].y) / (double)(point2d[1].x - point2d[2].x);
    double b2 = point2d[2].y - k2 * point2d[2].x;
    double x = (b2 - b1) / (k1 - k2);
    double y = k1 * x + b1;
    line(dst, point2d[0], point2d[3], Scalar(255, 255, 255), 2, 8, 0);
    line(dst, point2d[2], point2d[1], Scalar(255, 255, 255), 2, 8, 0);
    circle(dst, Point(x,y), 5, Scalar(0, 0, 255), -1);
    imshow("test", dst);
}

string ToString(double val)
{
    stringstream ss;
    ss<<setiosflags(ios::fixed)<<setprecision(2)<<val;
    string str=ss.str();
    return str;
}
void Armor::Draw2()
{
    Mat dst(Size(400, 400), CV_8UC3, Scalar(0));
    Solvepnp();
    namedWindow("math", WINDOW_AUTOSIZE);
    putText(dst, "distance:" + ToString(this->distance) + "cm", Point(10, 50), 2, 1, Scalar(255, 255, 255));
    putText(dst, "x_angle:" + ToString(this->x_angle)+"degree", Point(10, 150), 2, 1, Scalar(255, 255, 255));
    putText(dst, "y_angle:" +ToString(this->y_angle)+"degree", Point(10, 250), 2, 1, Scalar(255, 255, 255));
    putText(dst, "z_angle:" + ToString(this->z_angle)+"degree", Point(10, 350), 2, 1, Scalar(255, 255, 255));
    imshow("math", dst);
}