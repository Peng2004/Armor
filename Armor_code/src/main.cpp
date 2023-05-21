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
    Mat img;
    while (1)
    {
        vid >> img;
        if (img.empty())
        {
            destroyAllWindows;
            break;
        }
        Armor test(img);
        imshow("video", img);
        test.Draw();
        test.Draw2();
        waitKey(1000 / vid.get(CAP_PROP_FPS));
    }
    waitKey();
    return 0;
}