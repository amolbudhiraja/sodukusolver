//
//  main.cpp
//  sodukusolver
//
//  Created by Amol Budhiraja on 1/9/23.
//

#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

using namespace cv;
using namespace std; 

int main() {
    VideoCapture cap(0);
    Mat img;
    while (true) {
        cap.read(img);
        imshow("Image", img);
        waitKey(1);
    }
    return 0;
}
