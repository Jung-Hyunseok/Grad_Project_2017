#include "tracking.h"

int main(void) {
	FrameProc cam_video;
	Tracker cam_tracker;

	cv::Mat frame;
	cv::Mat output;

	cam_video.setCamera();
	cam_video.displayInput(std::string("Input"));
	cam_video.displayOutput(std::string("Output"));

	Servo::hwPwmSetup();
	Servo::allignServo();
	
	using namespace std::placeholders;

	cam_video.setProcess(std::bind(&Tracker::trackFace,
		&cam_tracker, _1, _2));
	cam_video.runProcess(frame, output);

}