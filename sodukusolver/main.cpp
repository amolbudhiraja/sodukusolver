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
    Mat sodukuBoardImage = imread("/Users/abudhiraja/Documents/sodukusolver/sodukusolver/media/soduku.jpeg");
    Mat trainingImage = imread("/Users/abudhiraja/Documents/sodukusolver/sodukusolver/media/sodukutrain1.png");
    vector<Mat> boxes = sodukuBoardDetector(sodukuBoardImage);
    vector<Mat> trainingImages = sodukuBoardDetector(trainingImage);
    for (Mat box: boxes) {
        string value = classifyTextFromImage(trainingImages, box); // Train model once before iteration for efficiency.
        cout << value << endl;
    }
    return 0;
}
