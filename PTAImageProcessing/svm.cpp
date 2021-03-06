#include "stdafx.h"
#include "opencv2/core.hpp"
#include <opencv2/ml.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <cstdio>
#include <vector>
#include <iostream>
#include<vector>


using namespace std;
using namespace cv;
using namespace cv::ml;

//https://stackoverflow.com/questions/14694810/using-opencv-and-svm-with-images followed.


int main()
{
	
	

	int num_files = 17; // # of rows of the training matrix
	int img_area = 512 * 424; //# of columns of the training matrix

	Mat training_mat(num_files, img_area, CV_32FC1); //TODO: Check for CV_32FC1 and update here,
	Mat labels(num_files, 1, CV_32FC1);
	Mat train_labels;
	Mat img_mat;
	int ii = 0; //Current column of training_mat

	cv::setBreakOnError(true); //Can be commented during the run.

	//I just tried to read images from a folder. In my case there was 17 images. Thats why num_files = 17.
	for (int i = 0; i < 16; i++)
	{
		img_mat = imread("A-FILE-PATH-HERE", IMREAD_COLOR);
		Mat float_data;
		img_mat.convertTo(float_data, CV_32FC1);
		training_mat.push_back(float_data);
		train_labels.push_back(labels);
	}


	//Set up SVM's parameters. TODO: Play with parameters for better results.
	Ptr<SVM> svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::LINEAR);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));

	//This is where I'm stuck. 
	Ptr<TrainData> td = TrainData::create(img_mat, ROW_SAMPLE, train_labels);
	svm->train(td);

	Vec3b green(0, 255, 0), blue(255, 0, 0);
	for (int i = 0; i < img_mat.rows; i++)
	{
		for (int j = 0; j < img_mat.cols; j++)
		{
			Mat sampleMat = (Mat_<float>(1, 2) << j, i);
			float response = svm->predict(sampleMat);
			
			if (response == 1)
			{
				img_mat.at<Vec3b>(i, j) = green;
			}
			else if (response == -1)
			{
				img_mat.at<Vec3b>(i, j) = blue;
			}
		}
	}

	// Show the training data
	int thickness = -1;
	int lineType = 8;
	circle(img_mat, Point(501, 10), 5, Scalar(0, 0, 0), thickness, lineType);
	circle(img_mat, Point(255, 10), 5, Scalar(255, 255, 255), thickness, lineType);
	circle(img_mat, Point(501, 255), 5, Scalar(255, 255, 255), thickness, lineType);
	circle(img_mat, Point(10, 501), 5, Scalar(255, 255, 255), thickness, lineType);

	// Show support vectors
	thickness = 2;
	lineType = 8;
	Mat sv = svm->getSupportVectors();

	for (int i = 0; i < sv.rows; ++i)
	{
		const float* v = sv.ptr<float>(i);
		circle(img_mat, Point((int)v[0], (int)v[1]), 6, Scalar(128, 128, 128), thickness, lineType);
	}

	imwrite("result.png", img_mat);        // save the image

	imshow("SVM Simple Example", img_mat); // show it to the user
	waitKey(0);


	//float labels[4] = { 1.0, -1.0, -1.0, -1.0 };




}