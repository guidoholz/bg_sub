/**
 * @file bg_sub.cpp
 * @brief Background subtraction tutorial sample code
 * @author Domenico D. Bloisi
 */

//opencv
#include "opencv2/imgproc.hpp"
#include <opencv2/imgcodecs.hpp>
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
//C
#include <stdio.h>
//C++
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <stdint.h>

using namespace cv;
using namespace std;

// Global variables
Mat frame; //current frame
Mat fgMaskMOG2; //fg mask fg mask generated by MOG2 method
Ptr<BackgroundSubtractorMOG2> pMOG2; //MOG2 Background subtractor
char keyboard; //input from keyboard
double total_threshold;
VideoCapture capture;

//https://docs.opencv.org/master/d1/dc5/tutorial_background_subtraction.html
//https://www.theimpossiblecode.com/blog/fastest-background-subtraction-opencv/
//cat bg_sub.cpp erl_comm.cpp port.cpp > all.cpp ; g++ `pkg-config --cflags opencv` `pkg-config --libs opencv` all.cpp -o ../priv/bg_sub
/** Function Headers */


int
startup(){
    //create GUI windows
    namedWindow("Frame");
    namedWindow("FG Mask MOG 2");
    pMOG2 = createBackgroundSubtractorMOG2(10,20,true); //MOG2 approach
    capture.open(0); //opens videocamstream

    if(!capture.isOpened()){
        //error in opening the video input
        cerr << "Unable to open video-stream "<< endl;
        exit(EXIT_FAILURE);
    }
    return 0;
}

int
shutdown(){
    destroyAllWindows();
    capture.release();
    return 0; //EXIT_SUCCESS;
}

int
setVarThreshold(int threshold) {
  if(threshold < 10) {
    pMOG2->setVarThreshold(10);
  } else if(threshold > 70) {
    pMOG2->setVarThreshold(70);
  } else {
    pMOG2->setVarThreshold(threshold);
  }
  return 0;
}

float
check_threshold() {
  float threshold = 0.001;
  int TotalNumberOfPixels = fgMaskMOG2.rows * fgMaskMOG2.cols;
  int ZeroPixels = TotalNumberOfPixels - countNonZero(fgMaskMOG2);
  float relativ = 1.0-(float)ZeroPixels/TotalNumberOfPixels;
  if (relativ >= threshold) {
    return relativ;
  }
  return 0.0;
//     cout<<"The number of pixels that are zero is "<<relativ<<endl;
}

/**
 * @function processVideo
 */
float
processVideo() {
    if(!capture.read(frame)) {
        cerr << "Unable to read next frame." << endl;
        cerr << "Exiting..." << endl;
        exit(EXIT_FAILURE);
    }
    //update the background model
    pMOG2->apply(frame, fgMaskMOG2);
    //get the frame number and write it on the current frame
    stringstream ss;
    //rectangle(frame, cv::Point(10, 2), cv::Point(100,20),
    //          cv::Scalar(255,255,255), -1);
    //ss << capture.get(CAP_PROP_POS_FRAMES);
    //string frameNumberString = ss.str();
    //putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
    //        FONT_HERSHEY_SIMPLEX, 0.5 , cv::Scalar(0,0,0));
    //show the current frame and the fg masks
    imshow("Frame", frame);
    imshow("FG Mask MOG 2", fgMaskMOG2);
    float trigger = check_threshold();
    //cout << frame << endl;
    //get the input from the keyboard
    vector<int> compression_params;
    compression_params.push_back(IMWRITE_JPEG_QUALITY);
    compression_params.push_back(50);
    imwrite("./apps/picam_web/web/static/assets/images/mask.jpg", fgMaskMOG2, compression_params);
    imwrite("./apps/picam_web/web/static/assets/images/original.jpg", frame, compression_params);
    //imencode(".jpg", fgMaskMOG2, image, compression_params);
    waitKey( 30 );
    return trigger;
}
