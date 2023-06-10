#include"Armor.h"


const int stateNum = 6;					//当前状态值6×1向量(x,y)
const int measureNum = 6;                               //测量值6×1向量(x,y)	
KalmanFilter KF(stateNum, measureNum, 0);		//实例化卡尔曼滤波类
int T = 1;						//卡尔曼滤波参考系下的时间
Point2f predict_point;					//基于卡尔曼滤波预测的预测点
int sumT = 0;						//卡尔曼滤波参考系下的时间的总和（用于算法）
int T_time[3];						//当前帧、上一帧、上上一帧的时间
int t_count = 0;					//计数
Point2f llastp, lastp, nowp;				//储存当前帧、上一帧、上上一帧的点信息
Mat measurement = Mat::zeros(measureNum, 1, CV_32F);

void kalman_init() {
	KF.transitionMatrix = (Mat_<float>(stateNum, measureNum) <<
		1, 0, T, 0, 1 / 2 * T * T, 0,
		0, 1, 0, T, 0, 1 / 2 * T * T,
		0, 0, 1, 0, T, 0,
		0, 0, 0, 1, 0, T,
		0, 0, 0, 0, 1, 0,
		0, 0, 0, 0, 0, 1);  //转移矩阵A

	setIdentity(KF.measurementMatrix);                                  //设置测量矩阵H
	setIdentity(KF.processNoiseCov, Scalar::all(5e-4));                 //设置系统噪声方差矩阵Q
	setIdentity(KF.measurementNoiseCov, Scalar::all(1e-3));             //设置测量噪声方差矩阵R
	setIdentity(KF.errorCovPost, Scalar::all(1));                       //设置后验错误估计协方差矩阵P

}


Point2f Armor:: kalman_predict(Point2f target_centre) {
	Mat prediction = KF.predict();			                //预测	
	llastp = lastp;					                //在新的一帧里，更新点的信息
	lastp = nowp;
	nowp = target_centre;
	predict_point = Point(prediction.at<float>(0), prediction.at<float>(1));//获取预测值(x',y')
	measurement.at<float>(0) = (float)target_centre.x;			//更新测量值
	measurement.at<float>(1) = (float)target_centre.y;
	measurement.at<float>(2) = (float)nowp.x - lastp.x;                     //速度
	measurement.at<float>(3) = (float)nowp.y - lastp.y;
	measurement.at<float>(4) = (nowp.x - lastp.x) - (lastp.x - llastp.x);   //加速度
	measurement.at<float>(5) = (nowp.y - lastp.y) - (lastp.y - llastp.y);
	KF.correct(measurement);			                        //根据测量值修正协方差矩阵
	return predict_point;				                        //返回预测的点
}
// int main()
// {
//     // 初始化Kalman滤波器
//     KalmanFilter KF(9, 3, 0);
//     KF.transitionMatrix = (Mat_<float>(9, 9) <<
//         1, 0, 0, 1, 0, 0, 0.5, 0, 0,
//         0, 1, 0, 0, 1, 0, 0, 0.5, 0,
//         0, 0, 1, 0, 0, 1, 0, 0, 0.5,
//         0, 0, 0, 1, 0, 0, 1, 0, 0,
//         0, 0, 0, 0, 1, 0, 0, 1, 0,
//         0, 0, 0, 0, 0, 1, 0, 0, 1,
//         0, 0, 0, 0, 0, 0, 1, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 1, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 1);
//     setIdentity(KF.measurementMatrix);
//     setIdentity(KF.processNoiseCov, Scalar::all(1e-5));
//     setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1));
//     setIdentity(KF.errorCovPost, Scalar::all(1));
//     Mat state(9, 1, CV_32F);

//     // 初始化3D点
//     std::vector<Point3f> points3d = {
//         Point3f(-0.5f, 0.5f, 0),
//         Point3f(0.5f, 0.5f, 0),
//         Point3f(0.5f, -0.5f, 0),
//         Point3f(-0.5f, -0.5f, 0)
//     };

//     // 读入2D点
//     std::vector<Point2f> points2d;
//     Mat img = imread("img.jpg");
//     points2d.push_back(Point2f(100, 100));
//     points2d.push_back(Point2f(200, 100));
//     points2d.push_back(Point2f(200, 200));
//     points2d.push_back(Point2f(100, 200));

//     // 循环处理每个3D点
//     for (auto &p : points3d)
//     {
//         // 初始化Kalman滤波器状态
//         state.at<float>(0) = p.x;
//         state.at<float>(1) = p.y;
//         state.at<float>(2) = p.z;
//         state.at<float>(3) = 0;
//         state.at<float>(4) = 0;
//         state.at<float>(5) = 0;
//         state.at<float>(6) = 0;
//         state.at<float>(7) = 0;
//         state.at<float>(8) = 0;
//         KF.statePost = state;

//         // 循环处理每个时刻
//         for (int i = 0; i < 10; i++)
//         {
//             // 预测Kalman滤波器状态
//             Mat prediction = KF.predict();

//             // 更新Kalman滤波器状态
//             Mat measurement = (Mat_<float>(3, 1) << points2d[i].x, points2d[i].y, 1);
//             Mat estimated = KF.correct(measurement);

//             // 获取平滑后的3D点坐标
//             p.x = estimated.at<float>(0);
//             p.y = estimated.at<float>(1);
//             p.z = estimated.at<float>(2);
//         }
//     }

//     return 0;
// }


