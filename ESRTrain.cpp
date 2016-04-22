#include <iostream>
#include <opencv2/opencv.hpp>

#include <vector>
#include <fstream>
#include <string>

#include "ESRUtils.hpp"
#include "ESRBbox.hpp"

#include "ESRRegressor.hpp"

using namespace cv;

static const int NUM_TRAIN_IMAGE = 1345;
static const int NUM_LAND_MARK   = 29;

static const char* trainImgFodler    = "./data/COFW_Dataset/trainingImages/";
static const char* trainImgBboxFile  = "./data/COFW_Dataset/boundingbox.txt";
static const char* trainImgShapeFile = "./data/COFW_Dataset/keypoints.txt";

static std::vector<Mat> trainImg;
static std::vector<Mat> trainImgShape;
static std::vector<ESR::Bbox> trainImgBbox;

void loadTrainingData();
void showTrainingData();

int main()
{
	std::cout << "ESRTrain" << std::endl;
	loadTrainingData();
	ESR::Regressor regressor;
	regressor.train(trainImg, trainImgBbox, trainImgShape);
	regressor.storeModel("./data/myModel.txt");

	return 0;
}

void loadTrainingData()
{
	//read in all training images
	for(int i=0; i<NUM_TRAIN_IMAGE; i++)
	{
		Mat image;
		std::string filename = std::string(trainImgFodler) + std::to_string(i+1) + ".jpg";
		image = imread(filename,CV_LOAD_IMAGE_GRAYSCALE);
		trainImg.push_back(image);
	}

	//read in all bboxes
	std::ifstream fin;
	fin.open(trainImgBboxFile);
	for(int i=0; i<NUM_TRAIN_IMAGE; i++)
	{
        ESR::Bbox temp;
        fin>>temp.sx>>temp.sy>>temp.w>>temp.h;
        temp.cx = temp.sx + temp.w/2.0;
        temp.cy = temp.sy + temp.h/2.0;
        trainImgBbox.push_back(temp);		
	}
	fin.close();

	//read in all ground-truth shapes
	fin.open(trainImgShapeFile);
	for(int i=0; i<NUM_TRAIN_IMAGE; i++)
	{
		Mat temp(NUM_LAND_MARK, 2, CV_64F);
        for(int j = 0;j < NUM_LAND_MARK;j++)
        {
            fin>>temp.at<double>(j,0); 
        }
        for(int j = 0;j < NUM_LAND_MARK;j++)
        {
            fin>>temp.at<double>(j,1); 
        }
        trainImgShape.push_back(temp);
	}
	fin.close();

}

void showTrainingData()
{
	for(int i=0; i<trainImg.size(); i++)
	{
		ESR::dispImgWithDetectionAndLandmarks(trainImg[i], trainImgShape[i], trainImgBbox[i], true, true);
	}
}
