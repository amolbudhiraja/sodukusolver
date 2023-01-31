//
//  sodukuSolver.cpp
//  sodukusolver
//
//  Created by Amol Budhiraja on 1/21/23.
//

#include <iostream>
#include <vector>
#include "sodukuSolver.hpp"

using namespace std;

/** Checks to see if a given number placement is a valid placement. */
bool isValid(vector<vector<char>>& board, int row, int col, char num) {
    // Check the row
    for (int x = 0; x < 9; x++)
        if (board[row][x] == num)
            return false;
    // Check the column
    for (int y = 0; y < 9; y++)
        if (board[y][col] == num)
            return false;
    // Check the surrounding 3x3 subgrid.
    int subgridRow = row - row % 3;
    int subgridCol = col - col % 3;
    for (int x = 0; x < 3; x++)
        for (int y = 0; y < 3; y++)
            if (board[subgridRow + x][subgridCol + y] == num)
                return false;
    return true;
}

/** Make sure that it is safe to place the number in the position on the board. */
bool isSafeToPlace(vector<vector<char>>& board, int row, int col, char num) {
    return isValid(board, row, col, num) && board[row][col] == '.';
}

/** Solve the Soduku Board. */
bool solveSudoku(vector<vector<char>>& board) {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (board[row][col] == '.') {
                for (char num = '1'; num <= '9'; num++) {
                    if (isSafeToPlace(board, row, col, num)) {
                        board[row][col] = num;
                        if (solveSudoku(board))
                            return true;
                        board[row][col] = '.';
                    }
                }
                return false;
            }
        }
    }
    return true;
}
