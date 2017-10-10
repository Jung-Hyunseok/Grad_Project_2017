#include "fact_tracking"

face_tracking::face_tracking() 
    // User-defined Constructor
    // Setting for picamera
    camera.set(CV_CAP_PROP_FORMAT, CV_8UC3);
    camera.set(CV_CAP_PROP_WIDTH, 720);
    camera.set(CV_CAP_PROP_WIDTH, 480);

    // Center coordinate
    x_pos = 720 / 2;
    y_pos = 480 / 2;

    // Exception : Fail to get camera
    try {
        if(camera.open() != true)
            throw "Camera failed";
    } catch(std::string msg) {
        std::cerr << msg << std::endl;
    }
}

void face_tracking::lbp_detect() { // Detection using lbp cascade
    cv::CascadeClassifier face_cascade;

    // Using LBP
    face_cascade.load("XML/lbpcascade_frontalface_improved.xml");

    // Getting image from camera
    camera.grab();
    camera.retrieve(capture);
    
    cv::Mat grayframe;
    cv::cvtColor(capture, grayframe, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(grayframe, grayframe);

    std::vector <cv::Rect> Recognized_faces;

    // Detect faces
    face_cascade.detectMultiScale(grayframe, Recognized_faces, 1.1, 3,
        CV_HAAR_FIND_BIGGEST_OBJECT | CV_HAAR_SCALE_IMAGE,
        cv::Size(30, 30), cv::Size(100, 100));

    // Use only one face
    face = Recognized_face[0];
}

// Tracking face by optical flow using lbp based feature
void face_tracking::tracking() { 
    // Point vectors for optical flow
    std::vector<cv::Point2f> corners_prev;
    std::vector<cv::Point2f> corners;

    // counter variable 
    int cnt = 0;

    // Middle coordinate for calculatind difference
    int x_middle = x_pos;
    int y_middle = y_pos;

    cv::Mat frame, frame_prev;
    cv::Mat roi;
    cv::Rect box;

    // Exception handler for calcOpticalFlowPryLK
    std::vector<uchar> status;
    std::vector<int> err;

    // Tracking start
    while(true) {
        if((cnt % 200) == 0) {
            // Getting face feature and frame
            face_tracking::lbp_detect();
            roi = frame_prev(face);

            // Convert to grayscale
            cvtColor(capture, frame_prev, CV_BGR2GRAY);
            cvtColor(roi, roi, CV_BGR2GRAY);
            cnt++;        
        }

        // Getting updated frame
        camera.grab();
        camera.retrieve(frame);
        cvtColor(frame, frame, CV_BGR2GRAY);
        
        // Extracting corners of face
        cv::goodFeaturesToTrack(roi, corners_prev, max_corners, 
            quality_level, min_distance, cv::Mat ());
        
        // Tracking face
        cv::calcOpticalFlowPyrLK(frame_prev, corners_prev, corners,
            status, error);
        
        // Drawing rectangular box
        box = boundingRect(corners);
        rectangle(frame, box, cv::CV_SCALAR(0, 255, 0));

        // Getting center point
        x_pos = box.x + 0.5 * box.width;
        y_pos = box.y + 0.5 * box.height;

        // Drawing circles
        circle(frame, cv::Point(x_pos, y_pos), 3, cv::CV_SCALAR(0, 0, 255));
        for(auto &i : corners) 
            circle(frame, i, 1, cv::CV_SCALAR(255, 0, 0));

        // Update previous frame
        frame_prev = frame;
    }
}