#include "ESRRegressor.hpp"

#include <fstream>
#include <iostream>

#include "ESRUtils.hpp"

namespace ESR
{
	void Regressor::predict(const Mat& image, Bbox bbox, Mat& shape)
	{
		//use mean shape as initial shape
		Mat cur = meanShape.clone();
		for(int i=0; i<numRegressor; i++)
		{
			//compute delta shape
			Mat deltaShape;
			regressors[i].predict(image, bbox, cur, meanShape, deltaShape);

			//update current shape
			cur += deltaShape;
		}
		transformBBox2Image(cur, bbox, shape);
		return;
	}

	void Regressor::loadModel(std::string filepath)
	{
		std::ifstream fin;
		fin.open(filepath);
		if(!fin.good())
		{
			std::cout << "[Error](loadModel): unable to open '" << filepath << "'" << std::endl;
			return;
		}
		loadModel(fin);
		fin.close();
		return;
	}

	void Regressor::loadModel(std::ifstream& fin)
	{
		//number of external regressor
		fin >> numRegressor;

		//number of land mark
		fin >> numLandmark;

		//mean shape
		meanShape = Mat::zeros(numLandmark,2,CV_64F);
		for(int i = 0; i< numLandmark; i++)
		{
			fin >> meanShape.at<double>(i,0) >> meanShape.at<double>(i,1);
		}

		//training data (for multiple initialization)
		int numTraining;
		fin >> numTraining;
		trainingShapes.resize(numTraining);
		trainingBboxes.resize(numTraining);
		for (int i=0; i<numTraining; i++)
		{
			//training bbox
			double sx,sy,w,h,cx,cy;
			fin >> sx >> sy >> w >> h >> cx >> cy;
			trainingBboxes[i] = Bbox(sx,sy,cx,cy,w,h);

			//training shape
			Mat shape(numLandmark, 2, CV_64F);
			for(int j=0; j<numLandmark; j++)
			{
				fin >> shape.at<double>(j,0) >> shape.at<double>(j,1);
			}
			trainingShapes[i] = shape;
		}

		//iteratively load model data into first level regressors
		regressors.resize(numRegressor);
		for(int i=0; i<regressors.size(); i++)
		{
			regressors[i].loadModel(fin);
		}

		std::cout << "[Info](loadModel): model loading completed." << std::endl;

	}

}