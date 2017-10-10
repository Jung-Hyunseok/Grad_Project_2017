#ifndef RECOGNITION_TRACKING_H
#define RECOGNITION_TRACKING_H

#include <string>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include "raspicam/raspicam_cv.h"
#include <vector>

const int max_corners = 6;
const double quality_level = 0.1;
const double min_distance = 10;

class face_tracking {
    public:
        face_tracking();
        void lbp_detect();
        void tracking();
        void kalman();
    private:
        raspicam::RaspiCam_cv camera;
        int x_pos;
        int y_pos;
        cv::Rect face;
        cv::Mat capture;
}

class servo_control {

}

#endif