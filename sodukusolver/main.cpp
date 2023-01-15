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
    Mat testImage = imread("/Users/abudhiraja/Documents/sodukusolver/sodukusolver/media/testdigit9.png");
    Mat sodukuBoardImage = imread("/Users/abudhiraja/Documents/sodukusolver/sodukusolver/media/soduku.jpeg");
    vector<string> trainBoardsLocations = {"/Users/abudhiraja/Documents/sodukusolver/sodukusolver/media/sodukutrain1.png", "/Users/abudhiraja/Documents/sodukusolver/sodukusolver/media/sodukutrain2.png", "/Users/abudhiraja/Documents/sodukusolver/sodukusolver/media/sodukutrain3.png", "/Users/abudhiraja/Documents/sodukusolver/sodukusolver/media/sodukutrain4.png", "/Users/abudhiraja/Documents/sodukusolver/sodukusolver/media/sodukutrain5.png"};
    vector<Mat> trainBoxes;
    for (string location : trainBoardsLocations) {
        Mat boardImage = imread(location);
        vector<Mat> boxes = sodukuBoardDetector(boardImage);
        for (Mat box : boxes) {
            trainBoxes.push_back(box);
        }
    }
    vector<Mat> boxes = sodukuBoardDetector(sodukuBoardImage);
    string result = classifyTextFromImage(trainBoxes, testImage);
    cout << result << endl;
    return 0;
}
