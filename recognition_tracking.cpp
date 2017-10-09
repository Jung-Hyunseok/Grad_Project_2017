#include "fact_tracking"

face_detect::face_detect() {
    cv::VideoCapture camera(0);

    try {
        if(camera.isOpened() != true)
            throw "Camera failed";
    } catch(std::string msg) {
        std::cerr << msg << std::endl;
    }
}

auto face_detect::lbp_detect(void) {
    cv::CascadeClassifier face_cascade;
    face_cascade.load("lbpcascade_frontalface_improved.xml");
    cv::Mat frame;
    bool isValid_frame = true;

    try {
        camera >> frame;
    } catch(cv::Exception &e) {
        isValid_frame = false;
        std::cerr << "Not valid frame. " << e.err << " Ignore it." <<std::endl;
    }

    if(isValide_frame == true) {
        
    }
    
}