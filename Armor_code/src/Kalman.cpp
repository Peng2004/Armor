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


	// *函数名：卡尔曼滤波初始化  kalman_init
	// *函数功能描述：（非虚函数，子类可以直接继承使用）对卡尔曼滤波需要的几个参数进行初始化


void Armor:: kalman_init() {
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


	// *函数功能描述：对当前位置点进行下一帧的预测，返回预测的坐标点
	// * 函数参数：Point2f 类型点，为当前装甲板所在的位置的中心点
	// * 函数返回值：基于传入的中心点的预测点


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

/*************************************卡尔曼滤波模块结束*******************************************/


