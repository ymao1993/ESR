#include <iostream>
#include <opencv2/opencv.hpp>

#include <vector>
#include <fstream>
#include <string>

#include "ESRUtils.hpp"
#include "ESRBbox.hpp"
#include "ESRRegressor.hpp"
#include "ESRCommon.hpp"

using namespace cv;

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
	regressor.storeModel(MODEL_FILE_NAME);

	return 0;
}

void loadTrainingData()
{
	std::cout << "loading training data..." << std::endl;

	std::vector<int> inValidIdx;
	//read in all training images and ground truth shapes
	for(int i=0; i<NUM_TRAINING_IMAGE; i++)
	{
		//read training image
		{
			char imageName[100];
			sprintf(imageName, "image_%04d.png", i+1);
			std::string filename = std::string(TRAIN_IMAGE_FOLDER_NAME) + imageName;
			Mat image = imread(filename,CV_LOAD_IMAGE_GRAYSCALE);
			if(image.data == NULL)
			{
				inValidIdx.push_back(i);
				continue;
			}
			trainImg.push_back(image);
		}

		//read training shapes
		{
			std::ifstream fin;
			char shapeName[100];
			sprintf(shapeName, "image_%04d.pts", i+1);
			std::string filename = std::string(TRAIN_IMAGE_FOLDER_NAME) + shapeName;
			fin.open(filename);

			//skip header
			string skip; fin >> skip >> skip >> skip >> skip >> skip;
 
			Mat temp(NUM_LAND_MARK, 2, CV_64F);
			for(int j=0; j<NUM_LAND_MARK; j++)
			{
				fin >> temp.at<double>(j,0) >> temp.at<double>(j,1);
			}
			trainImgShape.push_back(temp);
			fin.close();
		}
	}

	//read in all bboxes
	std::ifstream fin;
	fin.open(TRAIN_BBOX_FILE_NAME);
	for(int i=0; i<NUM_TRAINING_IMAGE; i++)
	{
		//skip all invalid idxes
		for(int j=0; j<inValidIdx.size(); j++)
		{
			if(inValidIdx[j] == i) continue;
		}

        ESR::Bbox temp;
        double brx, bry;
        fin>>temp.sx>>temp.sy>>brx>>bry;
        temp.w  = brx - temp.sx;
        temp.h  = bry - temp.sy;
        temp.cx = temp.sx + temp.w/2.0;
        temp.cy = temp.sy + temp.h/2.0;
        trainImgBbox.push_back(temp);		
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
