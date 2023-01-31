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
#include "displaySodukuBoard.hpp"

using namespace cv;
using namespace std;

/** Print a string representation of soduku board, BOARD. */
void printSodukuBoard(vector<vector<char>> board) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            cout << board.at(i).at(j) << endl;
        }
    }
}

/** Main Function. */
int main() {
    Mat sodukuBoardImage = imread("/Users/abudhiraja/Documents/sodukusolver/sodukusolver/media/soduku.jpeg");
    Mat trainingImage1 = imread("/Users/abudhiraja/Documents/sodukusolver/sodukusolver/media/sodukutrain1.png");
    Mat trainingImage2 = imread("/Users/abudhiraja/Documents/sodukusolver/sodukusolver/media/sodukutrain4.png");
    Mat trainingImage3 = imread("/Users/abudhiraja/Documents/sodukusolver/sodukusolver/media/sodukutrain5.png");
    Mat trainingImage4 = imread("/Users/abudhiraja/Documents/sodukusolver/sodukusolver/media/sodukutrain6.png");
    vector<Mat> boxes = sodukuBoardDetector(sodukuBoardImage);
    vector<Mat> trainingImages;
    vector<Mat> trainingImages1 = sodukuBoardDetector(trainingImage1);
    vector<Mat> trainingImages2 = sodukuBoardDetector(trainingImage2);
    vector<Mat> trainingImages3 = sodukuBoardDetector(trainingImage3);
    vector<Mat> trainingImages4 = sodukuBoardDetector(trainingImage4);
    trainingImages.insert(trainingImages.end(), trainingImages1.begin(), trainingImages1.end());
    trainingImages.insert(trainingImages.end(), trainingImages2.begin(), trainingImages2.end());
    trainingImages.insert(trainingImages.end(), trainingImages3.begin(), trainingImages3.end());
    trainingImages.insert(trainingImages.end(), trainingImages4.begin(), trainingImages4.end());
    vector<char> parsedSodukuString;
    for (Mat box: boxes) {
        string value = classifyTextFromImage(trainingImages, box);
        parsedSodukuString.push_back(value.at(0));
    }
    int numRows = 9, numCols = 9, index = 0;
    vector<vector<char>> sodukuBoard(9, vector<char>(9, '.'));
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            sodukuBoard[j][i] = parsedSodukuString[index];
            index++;
        }
    }
    sodukuBoard.at(5).at(6) = '.';
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
    solveSudoku(sodukuBoard);
    displaySodokuBoard(sodukuBoard);
    return 0;
}

