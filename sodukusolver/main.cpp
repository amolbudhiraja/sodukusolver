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
#include "sodukuBoardDetector.hpp"

using namespace cv;
using namespace std;

int main() {
    Mat sodukuBoardImage = imread("/Users/abudhiraja/Downloads/soduku.jpeg", 0);
    sodukuBoardDetector(sodukuBoardImage);
    return 0;
}
