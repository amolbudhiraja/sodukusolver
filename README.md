# sodukusolver
Computer vision based realtime Soduku game solver. 

## About
As a big fan of the Sodoku game, I decided to build an autonomous Sodoku solver that could take any image and solve the board in the image. Moreover, wanting to learn C++, I knew creating this project was the perfect learning experience. 

This program can solve any Soduku Board with the following proceedure: 
1. Given any image of a Sodoku Board, the `sodukuBoardDetector` method parses the Sodoku Board from the image and crops out each of the 81 individual cells. Note, this program is designed for 9x9 Sodoku Boards only. 
2. Given the cells, the program then attempts to classify the type of cell and the value of the cell. However, before it can do that it must discern whether the cell is empty or contains a number. This is done to avoid the classification of noise in empty cells as digits. To achomplish this task, I created a KNN Classifier using the OpenCV Machine Learning library. The `classifyBoxType` method completes this task. 
3. Given the cells that have been classified as containing a numerical value, I leverage the Tesseract and Leptonica libraries to classify the value contained in the cell. This task is accomplished by the `classifyTextFromImage` method. 
4. Then I construct a `vector<vector<char>>` representation of the board. I use the `'.'`  character to represent an empty cell. 
5. Next, I solve the Sodoku Board with the `solveSodoku` method. To solve the board, I designed a recursive backtracking algorithm. 
6. Finally, I construct a visual representation of the solved Sodoku Board and display it to the user. This task is handled by the `displaySodokuBoard` method. 

## Installation Instructions
To install the application, use `git clone` to clone the git repository. Then open the sodukusolver.xcodeproj file and click run. Note: this project can only be run on mac devices. 

To change the board being solved, upload the image to the code and pass its location into the `sodukuBoardImage`'s associated `imread` method. 
