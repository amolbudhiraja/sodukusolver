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

/** Take the training image and split it into smaller subimages. */
vector<Mat> splitTrainData(Mat image) {
    vector<Mat> trainImages;
    int subDivision = 50;
    for (int i = 0; i < subDivision; i++) {
        for (int j = 0; j < subDivision; j++) {
            int x = i * image.cols / subDivision;
            int y = j * image.rows / subDivision;
            Mat subImage = image(Rect(x, y, image.cols / subDivision, image.rows / subDivision));
            cvtColor(subImage, subImage, COLOR_BGRA2GRAY);
            subImage.convertTo(subImage, CV_32F);
            subImage.reshape(1, 1);
            trainImages.push_back(subImage);
        }
    }
    return trainImages;
}

/** Get the labels for the data. */
vector<int> getLabels() {
    vector<int> labels;
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 10; j++) {
            for (int k = 0; k < 5; k++) {
                labels.push_back(j);
            }
        }
    }
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
string classifyTextFromImage(Mat trainImage, Mat imageClassify) {
    vector<Mat> trainingImages = splitTrainData(trainImage);
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
