//
//  displaySodukuBoard.cpp
//  sodukusolver
//
//  Created by Amol Budhiraja on 1/26/23.
//

#include <vector>
#include <opencv2/opencv.hpp>
#include "displaySodukuBoard.hpp"

using namespace std;
using namespace cv;

/** Display an image representation of the Soduku Board. */
void displaySodokuBoard(vector<vector<char>> &board) {
    Mat img(450, 450, CV_8UC3, Scalar(255, 255, 255));
    int xOffset = 0, yOffset = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            rectangle(img, Point(xOffset + j * 50, yOffset + i * 50), Point(xOffset + (j + 1) * 50, yOffset + (i + 1) * 50), Scalar(0, 0, 0), 1);
            if (board[i][j] != '.') {
                putText(img, string(1, board[i][j]), Point(xOffset + 25 + j * 50, yOffset + 25 + i * 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0), 2);
            }
        }
    }
    imshow("Soduku Board", img);
    waitKey(0);
}
