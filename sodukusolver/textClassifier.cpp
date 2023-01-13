//
//  textClassifier.cpp
//  sodukusolver
//
//  Created by Amol Budhiraja on 1/10/23.
//

#include <opencv2/ml.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include "sodukuBoardDetector.hpp"
#include "textClassifier.hpp"

using namespace std;
using namespace cv;
using namespace cv::dnn;


/** Get the labels for the training data. Note: 10 represents an empty image. */
vector<int> getLabels() {
    vector<int> board1 = {10, 10, 10, 10, 1, 9, 4, 10, 10, 10, 10, 10, 10, 10, 10, 5, 8, 7, 10, 10, 10, 10, 10, 3, 10, 6, 10, 10, 3, 10, 10, 10, 2, 1, 10, 10, 5, 10, 10, 7, 10, 8, 10, 10, 9, 10, 10, 4, 6, 10, 10, 10, 5, 10, 10, 2, 10, 4, 10, 10, 10, 10, 10, 7, 5, 3, 10, 10, 10, 10, 10, 10, 10, 10, 8, 3, 2, 10, 10, 10, 10};
    vector<int> board2 = {10, 10, 5, 10, 10, 10, 6, 10, 10, 10, 10, 10, 10, 10, 2, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 3, 10, 8, 10, 10, 10, 10, 10, 10, 4, 2, 10, 10, 10, 7, 10, 3, 10, 10, 10, 1, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 8, 1, 10, 10, 10, 6, 10, 4, 10, 10, 10, 10, 7, 10, 10, 10, 3, 10, 10, 10, 10, 5, 10, 10};
    vector<int> board3 = {10, 10, 10, 10, 9, 3, 1, 10, 10, 10, 1, 5, 10, 10, 10, 8, 7, 10, 10, 10, 4, 10, 10, 8, 10, 9, 10, 10, 10, 10, 10, 1, 4, 5, 6, 8, 10, 10, 10, 10, 3, 10, 10, 10, 10, 5, 8, 2, 9, 6, 10, 10, 10, 10, 10, 6, 10, 5, 10, 10, 3, 10, 10, 10, 3, 7, 10, 10, 10, 6, 1, 10, 10, 10, 8, 3, 7, 10, 10, 10, 10};
    vector<int> board4 = {2, 10, 8, 10, 10, 5, 10, 10, 10, 9, 3, 10, 10, 10, 10, 10, 2, 5, 5, 1, 6, 7, 10, 10, 10, 10, 4, 7, 8, 10, 10, 3, 10, 5, 6, 10, 10, 6, 10, 5, 8, 1, 10, 10, 10, 10, 5, 10, 10, 7, 6, 10, 10, 8, 8, 10, 10, 10, 10, 7, 1, 3, 6, 6, 2, 10, 10, 10, 10, 10, 5, 7, 10, 10, 10, 6, 10, 10, 9, 10, 2};
    vector<int> board5 = {10, 10, 10, 10, 7, 10, 10, 10, 10, 10, 9, 10, 5, 10, 2, 10, 1, 10, 10, 10, 8, 9, 10, 3, 5, 10, 10, 10, 5, 4, 10, 10, 10, 3, 6, 10, 7, 10, 10, 10, 10, 10, 10, 10, 1, 10, 6, 1, 10, 10, 10, 7, 8, 10, 10, 10, 2, 8, 10, 5, 4, 10, 10, 10, 8, 10, 4, 10, 7, 10, 9, 10, 10, 10, 10, 10, 6, 10, 10, 10, 10};
    vector<int> labels;
    labels.insert(labels.end(), board1.begin(), board1.end());
    labels.insert(labels.end(), board2.begin(), board2.end());
    labels.insert(labels.end(), board3.begin(), board3.end());
    labels.insert(labels.end(), board4.begin(), board4.end());
    labels.insert(labels.end(), board5.begin(), board5.end());
    return labels;
}

/** Create and return the KNN classifier used to classify the digits from the images of digits. */
Ptr<ml::KNearest> getKNNClassifier(vector<Mat> trainingImages, vector<int> labels) {
    Ptr<ml::KNearest> knn = ml::KNearest::create();
    vector<Mat> flattened_training_images;
    for(Mat img: trainingImages){
        Mat flat = img.clone();
        flat.convertTo(flat, CV_32F);
        flat = flat.reshape(1,1);
        flattened_training_images.push_back(flat);
    }
    Mat training_data;
    Mat labels_mat;
    for(int i=0; i<flattened_training_images.size(); i++){
        Mat img = flattened_training_images[i].clone();
        img.convertTo(img, CV_32F);
        img.reshape(1, 1);
        training_data.push_back(img);
        labels_mat.push_back(labels[i]);
    }
    knn->train(training_data, ml::ROW_SAMPLE, labels_mat);
    return knn;
}

/** Pre-process the test image to invert its colors, and ensure that its size and number of channels match the training data. */
Mat processTestImage(Mat imageClassify, vector<Mat> trainingImages) {
    bitwise_not(imageClassify, imageClassify); // The Soduku Board Images had a white background, however we need to invert the colors to match the traning data.
    cvtColor(imageClassify, imageClassify, COLOR_BGR2GRAY);
    resize(imageClassify, imageClassify, Size(trainingImages[0].cols,trainingImages[0].rows));
    Mat processedTestImage = imageClassify.clone();
    processedTestImage.convertTo(processedTestImage, CV_32F);
    processedTestImage = processedTestImage.reshape(1,1);
    return processedTestImage;
}

/** Classify the text in the image using a KNN (K-Nearest Neighbors Classifier). */
string classifyTextFromImage(vector<Mat> trainingImages, Mat imageClassify) {
    vector<int> labels = getLabels();
    Ptr<ml::KNearest> knn = getKNNClassifier(trainingImages, labels);
    Mat processedTestImage = processTestImage(imageClassify, trainingImages);
    Mat results;
    float response = knn->findNearest(processedTestImage, 3, results);
    string classification = to_string((int) response);
    if (classification == "10") { // 10 refers to an empty page.
        classification = ".";
    }
    cout << "Classification: " << classification << endl;
    return classification;
}
