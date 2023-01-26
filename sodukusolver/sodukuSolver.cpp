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

// Function to check if a given number is valid in the current cell
bool isValid(vector<vector<char>>& board, int row, int col, char num) {
    // Check row
    for (int x = 0; x < 9; x++)
        if (board[row][x] == num)
            return false;

    // Check column
    for (int y = 0; y < 9; y++)
        if (board[y][col] == num)
            return false;

    // Check 3x3 subgrid
    int subgridRow = row - row % 3;
    int subgridCol = col - col % 3;
    for (int x = 0; x < 3; x++)
        for (int y = 0; y < 3; y++)
            if (board[subgridRow + x][subgridCol + y] == num)
                return false;

    return true;
}

// Function to check if it is safe to place a number in a given cell
bool isSafe(vector<vector<char>>& board, int row, int col, char num) {
    return board[row][col] == '.' && isValid(board, row, col, num);
}

bool solveSudoku(vector<vector<char>>& board) {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (board[row][col] == '.') {
                for (char num = '1'; num <= '9'; num++) {
                    if (isSafe(board, row, col, num)) {
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
