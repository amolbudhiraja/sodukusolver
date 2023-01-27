//
//  sodukuBoardDetector.cpp
//  sodukusolver
//
//  Created by Amol Budhiraja on 1/9/23.
//

#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "sodukuBoardDetector.hpp"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <opencv2/text/ocr.hpp>
#include "textClassifier.hpp"

using namespace std;
using namespace cv;
using namespace tesseract;

/** Find the points of the largest area contour. */
vector<Point> getLargestContour(Mat image) {
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(image, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    vector<vector<Point>> conPoly(contours.size());
    vector<Rect> boundRect(contours.size());
    vector<Point> biggest;
    int maxArea=0;
    for (int i = 0; i < contours.size(); i++) {
        int area = contourArea(contours[i]);
        float peri = arcLength(contours[i], true);
        approxPolyDP(contours[i], conPoly[i], 0.05 * peri, true);
        if (area > maxArea && conPoly[i].size() == 4) {
                biggest = {conPoly[i][0], conPoly[i][1], conPoly[i][2], conPoly[i][3]};
                maxArea = area;
            }
        }
    return biggest;
}

/** Draw points and numbers in an image. */
Mat drawPoints(Mat imgOriginal, vector<Point> points, Scalar color) {
    for (int i = 0; i < points.size(); i++) {
        circle(imgOriginal, points[i], 10, color, FILLED);
        putText(imgOriginal, to_string(i), points[i], FONT_HERSHEY_PLAIN, 4, color, 4);
    }
    return imgOriginal;
}

/** Re order the points of the contour. */
vector<Point> reorder(vector<Point> points) {
    vector<Point> newPoints;
    vector<int> sumPoints, subPoints;
    for (int i = 0; i < 4; i++) {
        sumPoints.push_back(points[i].x + points[i].y);
        subPoints.push_back(points[i].x - points[i].y);
    }
    newPoints.push_back(points[min_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]); // 0
    newPoints.push_back(points[max_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]); // 1
    newPoints.push_back(points[min_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]); // 2
    newPoints.push_back(points[max_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]); // 3
    return newPoints;
}

/** Warp the image. */
Mat getWarp(Mat img, vector<Point> points, float w, float h) {
    Point2f src[4] = {points[0], points[1], points[2], points[3]};
    Point2f dst[4] = {{0.0f, 0.0f}, {w, 0.0f}, {0.0f, h}, {w, h}};
    Mat matrix = getPerspectiveTransform(src, dst);
    Mat imgWarpP2;
    warpPerspective(img, imgWarpP2, matrix, Point(w, h));
    for (int i = 0; i < 4; i++) {
        circle(img, src[i], 10, Scalar(0, 0, 225), FILLED); // Puts a circle at the selected points.
    }
    return imgWarpP2;
}

/** Crop out the Soduku Board from the Image. */
Mat getSodukuBoard(Mat sodukuBoardImage) {
    Mat sodukuBoardProcessed;
    GaussianBlur(sodukuBoardImage, sodukuBoardImage, Size(3, 3), 3);
    sodukuBoardImage.convertTo(sodukuBoardImage, CV_8UC1);
    Canny(sodukuBoardImage, sodukuBoardProcessed, 30, 80);
    Mat imgCanny = sodukuBoardProcessed;
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    dilate(sodukuBoardProcessed, sodukuBoardProcessed, kernel);
    vector<Point> initialPoints = getLargestContour(sodukuBoardProcessed);
    vector<Point> boardPoints = reorder(initialPoints);
    sodukuBoardProcessed = getWarp(sodukuBoardImage, boardPoints, 700, 700);
    return sodukuBoardProcessed;
}

void remove_border(const Mat &image, Mat &result) {
    // Convert the image to grayscale
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);

    // Find the edges in the image using canny edge detection
    Mat edges;
    Canny(gray, edges, 70, 150);

    // Find the contours in the edges image
    vector<vector<Point>> contours;
    findContours(edges, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // Find the largest contour in the list of contours
    int largest_contour_index = 0;
    double largest_area = 0;
    for (int i = 0; i < contours.size(); i++) {
        double area = contourArea(contours[i]);
        if (area > largest_area && area != edges.cols * edges.rows) {
            largest_area = area;
            largest_contour_index = i;
        }
    }

    // Create a mask for the image using the largest contour
    Mat mask = Mat::zeros(gray.size(), CV_8UC1);
    drawContours(mask, contours, largest_contour_index, Scalar(255), -1);

    // Apply the mask to the original image to remove the border
    bitwise_and(image, image, result, mask);
}

Mat processImage(Mat image) {
    Mat imgGray, imgCanny, imgBlur, imgProcessed;
    GaussianBlur(image, imgBlur, Size(3,3), 3, 0);
    threshold(imgBlur, imgProcessed, 50, 150, 1);
    imshow("PROCESSED IMG", imgProcessed);
    waitKey(400);
    return imgProcessed;
}


/** Detect and return a matrix version of the Soduku Board from the given image. */
vector<Mat> sodukuBoardDetector(Mat sodukuBoardImage) {
    Mat sodukuBoardCropped = getSodukuBoard(sodukuBoardImage);
//    imshow("CROP", sodukuBoardCropped);
//    waitKey(800);
    vector<Mat> boxes;
       for(int i = 0; i < 9; i++) {
           for(int j = 0; j < 9; j++) {
               int x = i * sodukuBoardCropped.cols / 9;
               int y = j * sodukuBoardCropped.rows / 9;
               Mat box = sodukuBoardCropped(Rect(x, y, (sodukuBoardCropped.cols / 9), (sodukuBoardCropped.rows / 9)));
               Mat boxCropped = box(Rect(11, 12, 57, 63));
               boxes.push_back(boxCropped);
           }
       }
    return boxes;
}
