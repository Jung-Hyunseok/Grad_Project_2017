#ifndef RECOGNITION_TRACKING_H
#define RECOGNITION_TRACKING_H

#include <string>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

class face_detect {
    public:
        face_detect();
        auto lbp_detect(void);
    private:
        cv::VideoCapture camera;
}

class tracking