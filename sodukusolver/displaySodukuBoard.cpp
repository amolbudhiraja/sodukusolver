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

void displaySodokuBoard(vector<vector<char>> &board) {
    Mat img(450, 450, CV_8UC3, Scalar(255, 255, 255));
    int x_offset = 0, y_offset = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            rectangle(img, Point(x_offset + j * 50, y_offset + i * 50), Point(x_offset + (j + 1) * 50, y_offset + (i + 1) * 50), Scalar(0, 0, 0), 1);
            if (board[i][j] != '.') {
                putText(img, string(1, board[i][j]), Point(x_offset + 25 + j * 50, y_offset + 25 + i * 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0), 2);
            }
        }
    }
    imshow("Soduku Board", img);
    waitKey(0);
}
