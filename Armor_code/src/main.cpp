#include <iostream>
#include <opencv2/opencv.hpp>
#include "Armor.h"

using namespace std;
using namespace cv;

int main()
{
    VideoCapture vid("/home/peng-ub/Armor/Armor.avi");
    if (vid.isOpened() == 0)
    {
        cout << "Failed to open video" << endl;
        return 0;
    }
kalman_init();
    while (1)
    {
        Mat img;
        vid >> img;
        if (img.empty())
        {
            destroyAllWindows;
            break;
        }
        Armor test;
        test.img=img;
        imshow("video", img);
        test.Draw();
        test.Draw2();
        imshow("test", img);
        waitKey(1000 / vid.get(CAP_PROP_FPS));
        // waitKey(100);
    }
    waitKey();
    return 0;
}