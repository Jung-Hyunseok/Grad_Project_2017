#ifndef TRACKING_H

#define TRACKING_H

#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include "raspicam/raspicam_cv.h"
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "wiringPi.h"

// Motor Controller
namespace Servo {
	void hwPwmSetup(); // HW PWM setup
	void allignServo(); // Allign servo motors
	void tiltControl(const int &num);
	void panControl(const int &num);
};

// Frame Processoer
class  FrameProc {
public:
	bool setCamera(); // Camera Settings: 480p
	void displayInput(std::string window_name);
	void displayOutput(std::string window_name);
	void setProcess(std::function<void(cv::Mat &, cv::Mat &)> function);
	void runProcess();	// Caution! Must allign motors before using it!
	
private:
	raspicam::RaspiCam_cv camera;
	double fps_input;
	std::string windows_name_input;
	std::string windows_name_output;
	std::function<void(cv::Mat &, cv::Mat &)> process;
};

// Face tracker
class Tracker {
public:
	Tracker(): max_count(30), q_level(0.01), min_dist(10.0) {}
	void getFace(cv::Mat &frame);
	void trackFace(cv::Mat &frame, cv::Mat &output);
	bool addCorners();
	bool acceptCorners();
	void drawBox(cv::Mat &frame);
	void getCenterPoint();

private:
	cv::Mat gray;
	cv::Mat gray_prev;
	cv::Point2f center;
	std::vector<cv::Point2f> features;
	std::vector<cv::Point2f> position;
	std::vector<cv::Point2f> corners[2];
	int max_count;
	double q_level;
	double min_dist;
	std::vector<unsigned char> status;
	std::vector<float> err;
};

// Functions for overlaying string
void overlayText(cv::Mat &frame, std::string &text);
void overlayFps(cv::Mat &frame, int fps);
#endif