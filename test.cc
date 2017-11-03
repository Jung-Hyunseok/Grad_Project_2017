#include "tracking.h"

int main(void) {
	FrameProc cam_video;
	Tracker cam_tracker;

	cam_video.setCamera();
	cam_video.displayInput(string("Input"));
	cam_video.displayOutput(string("Output"));

	Servo::hwPwmSetup();
	Servo::allignServo();
	
	cam_video.setProcess(std::bind(&cam_tracker.trackFace(), this,
		std::placeholders::_1, std::placeholders::_2));
	cam_video.runProcess();

}