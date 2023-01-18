//
//  textClassifier.cpp
//  sodukusolver
//
//  Created by Amol Budhiraja on 1/10/23.
//

#include <string>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>
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
using namespace cv::ml;
using namespace tesseract;

/** Return the labels of the train image where 0 represents an empty box and 1 represents a filled box (contains a number). */
vector<int> getLabels() {
    vector<int> trainImageLabels = {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0};
    return trainImageLabels;
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
        img.reshape(1, img.rows);
        training_data.push_back(img);
        labels_mat.push_back(labels[i]);
    }
    knn->train(training_data, ml::ROW_SAMPLE, labels_mat);
    return knn;
}

/** Pre-process the training images to ensure that its size and number of channels match the training data. */
vector<Mat> processTrainImages(vector<Mat> trainingImages) {
    vector<Mat> processedTrainingImages;
    for (Mat image: trainingImages) {
        blur(image, image, Size(3, 3));
        cvtColor(image, image, COLOR_BGRA2GRAY);
        image.convertTo(image, CV_32F);
        image = image.reshape(1,1);
        processedTrainingImages.push_back(image);
    }
    return processedTrainingImages;
}


/** Pre-process the test image to ensure that its size and number of channels match the training data. */
Mat processTestImage(Mat imageClassify, vector<Mat> trainingImages) {
    cvtColor(imageClassify, imageClassify, COLOR_BGR2GRAY);
    blur(imageClassify, imageClassify, Size(3, 3));
    Mat processedTestImage = imageClassify.clone();
    processedTestImage.convertTo(processedTestImage, CV_32F);
    processedTestImage = processedTestImage.reshape(1,1);
    resize(processedTestImage, processedTestImage, Size(trainingImages[0].cols, trainingImages[0].rows));
    return processedTestImage;
}


/** Classify the text in the image using a KNN (K-Nearest Neighbors Classifier). 1:  Filled Box. (With number), 0: Empty Box. */
int classifyBoxType(vector<Mat> trainingImages, Mat imageClassify) {
    vector<int> trainLabels = getLabels();
    vector<Mat> processedTrainingImages = processTrainImages(trainingImages);
    Ptr<ml::KNearest> knn = getKNNClassifier(processedTrainingImages, trainLabels);
    knn->setIsClassifier(true);
    Mat processedTestImage = processTestImage(imageClassify, processedTrainingImages);
    Mat results;
    int boxType = knn->findNearest(processedTestImage, 3, results);
    return boxType;
}


/** Using the Tesseract OCR Library classify the digit in filled soduku board boxes. */
string classifyTextFromImage(vector<Mat> trainingImages, Mat image) {
    Ptr<TessBaseAPI> ocr = new TessBaseAPI();
    ocr->Init(NULL, "eng", tesseract::OEM_LSTM_ONLY);
    ocr->SetPageSegMode(tesseract::PSM_SINGLE_CHAR);
    ocr->SetImage(image.data, image.cols, image.rows, 3, (int) image.step);
    string imageText = string(ocr->GetUTF8Text());
    ocr ->End();
    return imageText;
}
