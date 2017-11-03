#include "tracking.h"

#define PWM_0 1
#define PWM_1 23

// Function definitions of namespace Servo
void Servo::hwPwmSetup() {
	pinMode(PWM_0, PWM_OUT); // Mode setup for tilt control
	pinMode(PWM_1, PWM_OUT); // Mode setup for pan control

	pwmSetMode(PWM_MODE_MS);
	pwmSetClock(384);
	pwmSetRange(1000);
}

void Servo::allignServo() {
	pwmWrite(PWM_0, 70);
	pwmWrite(PWM_1, 70);
}

void Servo::tiltControl(const int &num) {
	pwmWrite(PWM_0, num);
}

void Servo::panControl(const int &num) {
	pwmWrite(PWM_1, num);
}

// Member function definitions of class FrameProc
#define WIDTH 720
#define HEIGHT 480
bool FrameProc::setCamera() {
	if (!camera.open()) {
		std::cerr << "Camera open failed" << std::endl;
		return false;
	}

	camera.set(CV_CAP_PROP_FORMAT, CV_8UC3);
	camera.set(CV_CAP_PROP_FRAME_WIDTH, WIDTH);
	camera.set(CV_CAP_PROP_FRAME_HEIGHT, HEIGHT);
	fps_input = camera.set(CV_CAP_PROP_FPS);

	return true;
}

void FrameProc::displayInput(std::string windows_name) {
	windows_name_input(windws_name);
	cv::namedWindow(windows_name_input);
}

void FrameProc::displayOutput(std::string windows_name) {
	windows_name_output(windows_name);
	cv::namedWindow(windows_name_input);
}

void FrameProc::setProcess(std::function<void(cv::Mat &, cv::Mat &)> function) {
	process = std::move(function);
}

void FrameProc::runProcess() {
	cv::Mat frame;
	cv::Mat output;

	while (true) {
		camera >> frame;

		if (windows_name_input.lenght() != 0)
			cv::imshow(windows_name_input, frame);

		process(frame, output);

		if (windows_name_output.length() != 0)
			cv::imshow(windows_name_output, output);

		if (waitkey(10) == 27) // If press ESC
			break; // End process
	}
}

// Member function definitions of class Tracker
#define FACE_XML_PATH "XML/lbpcascade_frontalface_improved.xml"

void Tracker::getFace(cv::Mat &frame) {
	cv::CascadeClassifier faceCascade;
	std::vector<cv::Rect> faces;
	cv::Mat roi;

	// Using LBP cascade classifier
	faceCascade.load(FACE_XML_PATH);
	faceCascade.detectMultiScale(gray, faces, 1.1, 3,
		CV_HAAR_FIND_BIGGEST_OBJECT | CV_HAAR_SCALE_IMAGE,
		cv::Size(30, 30), cv::Size(100, 100));

	// Getting features of recognized face
	roi = frame(faces[0]);
	cv::goodFeaturesToTrack(roi, features, max_count, q_level, min_dist);
}

void Tracker::trackFace(cv::Mat &frame, cv::Mat &output) {
	cv::cvtColor(frame, gray, CV_BGR2GRAY);

	static int tilt = 70; // Neutral angle 
	static int pan = 70; // Neutral angle 

	if (addCorners()) {
		getFace(gray);

		corners[0].insert(corners[0].end(), features.begin(),
			features.end());
		position.insert(position.end(), features.begin(),
			features.end());
	}

	if (gray_prev.empty())
		gray.copyTo(gray_prev);

	cv::calcOpticalFlowPyrLK(gray_prev, gray, corners[0],
		corners[1], status, err);

	int i = 0;
	for (int j = 0; j < corners[1].size(); j++) {
		if (acceptCorners(j)) {
			position[i] = position[j];
			corners[1][i++] = corners[1][j];
		}
	}

	corners[1].resize(i);
	position.resize(i);

	drawBox(frame, output);
	getCenterPoint();

	if (center.x > WIDTH / 2) {
		if (pan >= 100) // Over allowed angle(+90 degree)!
			overlayText(output, "Cannot Move!");
		else
			Servo::panControl(++pan);
	} else if (center.x < WIDTH / 2) {
		if (pan <= 50) // Over allowd angle(-90 degree)!
			overlayText(output, "Cannot Move!");
		else
			Servo::panControl(--pan);
	}

	if (center.y > HEIGHT / 2) {
		if (tilt <= 50) // Over allowed angle(-90 degree)!
			overlayText(output, "Cannot Move!");
		else
			Servo::tiltControl(--tilt);
	} else if (center.y < HEIGHT / 2) {
		if (tilt >= 80) // Physical frame restriction 
			overlayText(output, "Cannot Move!");
		else
			Servo::tiltControl(++tilt);
	}

	std::swap(corners[1], corners[0]);
	cv::swap(gray_prev, gray);
}

bool Tracker::addCorners() {
	return (corners[0].size() < 10);
}

bool Tracker::acceptCorners(int idx) {
	if (status[i] == false)
		return false;
	if ((corners[0][idx].x - corners[1][idx].x) != 0)
		return true;
	if ((corners[0][idx].y - corners[1][idx].y) != 0)
		return true;
}

void Tracker::drawBox(cv::Mat &frame) {
	// Draw rectangular box
	box = booundingRect(corners[0]);
	rectangle(frame, box, cv::CV_SCALAR(0, 255, 0));
}

void Tracker::getCenterPoint() {
	box = boundingRect(corners[0]);

	center.x = box.x + 0.5 * box.width;
	center.y = box.y + 0.5 * box.height;
}

void overlayText(cv::Mat &frame, std::string &text) {
	cv::putText(frame, text, cv::Point(WIDTH - 50, 50),
		cv::FONT_HERSHEY_COMPLEX_SMALL, 4, 
		cv::CV_SCALAR(255, 0, 0), 1, cv::CV_AA);
}

void overlayFps(cv::Mat &frame, int fps) {
	std::string tmp(fps); 

	cv::putText(frame, tmp, cv::Point(50, 50),
		cv::FONT_HERSHEY_COMPLEX_SMALL, 4,
		cv::CV_SCALAR(255, 0, 0), 1, cv::CV_AA);
}