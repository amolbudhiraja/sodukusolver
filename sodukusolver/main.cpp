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
#include "textClassifier.hpp"

using namespace cv;
using namespace std;

int main() {
    Mat sodukuBoardImage = imread("/Users/abudhiraja/Downloads/soduku.jpeg");
    Mat trainData = imread("/Users/abudhiraja/Downloads/digits.png");
    Mat testDigit = imread("/Users/abudhiraja/Downloads/testdigit9.png");
    Mat sodukuBoardTrain = imread("/Users/abudhiraja/Downloads/sodukuboardtrain.jpeg");
    sodukuBoardDetector(sodukuBoardImage, trainData);
    return 0;
}
