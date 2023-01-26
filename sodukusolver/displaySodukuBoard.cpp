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

void displaySudokuBoard(vector<vector<char>>& board) {
    int cellSize = 50;
    int boardSize = cellSize * 9;
    Mat sudokuBoard(boardSize, boardSize, CV_8UC3, Scalar(255, 255, 255));
    Scalar black(0, 0, 0);
    Scalar gray(128, 128, 128);

    for (int i = 1; i < 9; i++) {
        line(sudokuBoard, Point(i * cellSize, 0), Point(i * cellSize, boardSize), gray, 2);
        line(sudokuBoard, Point(0, i * cellSize), Point(boardSize, i * cellSize), gray, 2);
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int x = i * 3 * cellSize;
            int y = j * 3 * cellSize;
            rectangle(sudokuBoard, Rect(x, y, 3 * cellSize, 3 * cellSize), gray, 2);
        }
    }

    Point textOrg(cellSize / 4, cellSize * (3 / 4));
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] != '.') {
                putText(sudokuBoard, string(1, board[i][j]), Point(j * cellSize + textOrg.x, i * cellSize + textOrg.y), FONT_HERSHEY_SIMPLEX, 1, black);
            }
        }
    }

    imshow("Solved Sudoku Board", sudokuBoard);
    waitKey(0);
}
