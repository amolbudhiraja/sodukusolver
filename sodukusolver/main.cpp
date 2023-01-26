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
#include "sodukuSolver.hpp"

using namespace cv;
using namespace std;

int main() {
    Mat sodukuBoardImage = imread("/Users/abudhiraja/Documents/sodukusolver/sodukusolver/media/soduku.jpeg");
    Mat trainingImage1 = imread("/Users/abudhiraja/Documents/sodukusolver/sodukusolver/media/sodukutrain1.png");
    Mat trainingImage2 = imread("/Users/abudhiraja/Documents/sodukusolver/sodukusolver/media/sodukutrain4.png");
    Mat trainingImage3 = imread("/Users/abudhiraja/Documents/sodukusolver/sodukusolver/media/sodukutrain5.png");
    vector<Mat> boxes = sodukuBoardDetector(sodukuBoardImage);
    vector<Mat> trainingImages;
    vector<Mat> trainingImages1 = sodukuBoardDetector(trainingImage1);
    vector<Mat> trainingImages2 = sodukuBoardDetector(trainingImage2);
    vector<Mat> trainingImages3 = sodukuBoardDetector(trainingImage3);
    trainingImages.insert(trainingImages.end(), trainingImages1.begin(), trainingImages1.end());
    trainingImages.insert(trainingImages.end(), trainingImages2.begin(), trainingImages2.end());
    trainingImages.insert(trainingImages.end(), trainingImages3.begin(), trainingImages3.end());
    vector<char> parsedSodukuString;
    for (Mat box: boxes) {
        string value = classifyTextFromImage(trainingImages, box);

        parsedSodukuString.push_back(value.at(0));
    }
    int numRows = 9, numCols = 9, index = 0;
    vector<vector<char>> sodukuBoard;
    for (int i = 0; i < numRows; i++) {
        vector<char> currRow;
        for (int j = 0; j < numCols; j++) {
            char item = parsedSodukuString[index];
            currRow.push_back(item);
            index++;
        }
        sodukuBoard.push_back(currRow);
    }
    vector<vector<char>> testBoard = {
        {'5', '3', '.', '.', '7', '.', '.', '.', '.'},
        {'6', '.', '.', '1', '9', '5', '.', '.', '.'},
        {'.', '9', '8', '.', '.', '.', '.', '6', '.'},
        {'8', '.', '.', '.', '6', '.', '.', '.', '3'},
        {'4', '.', '.', '8', '.', '3', '.', '.', '1'},
        {'7', '.', '.', '.', '2', '.', '.', '.', '6'},
        {'.', '6', '.', '.', '.', '.', '2', '8', '.'},
        {'.', '.', '.', '4', '1', '9', '.', '.', '5'},
        {'.', '.', '.', '.', '8', '.', '.', '7', '9'}
    };
    bool solveable = solveSudoku(testBoard);
    cout << solveable << endl;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            cout << testBoard.at(i).at(j) << endl; 
        }
    }
    return 0;
}
