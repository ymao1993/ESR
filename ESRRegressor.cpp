#include "ESRRegressor.hpp"

#include <fstream>
#include <iostream>

#include "ESRUtils.hpp"
#include "ESRCommon.hpp"


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

	void Regressor::train(const std::vector<Mat>& images, 
						  const std::vector<Bbox>& bboxes, 
						  const std::vector<Mat>& target_shapes)
	{
		std::cout << "training ESR regressor" << std::endl;

		numRegressor = EXTERNAL_REGRESSOR_NUM;
		numLandmark = LAND_MARK_NUM;
		trainingBboxes = bboxes;
		trainingShapes = target_shapes;

		//compute mean shape
		computeMeanShape(target_shapes, bboxes, meanShape);

		//transform target shape to normalized bbox space
		std::vector<Mat> targetShapes(target_shapes.size());
		for(int i=0; i<targetShapes.size(); i++)
		{
			Mat temp;
			transformImage2BBox(target_shapes[i],bboxes[i],temp);
			targetShapes[i] = temp;
		}

		//training data augmentation
		int augment = 20;
		int augmentNum = augment*images.size();
		std::vector<Mat> augmentedImages(augmentNum);
		std::vector<Mat> augmentedTargetShape(augmentNum);
		std::vector<Bbox> augmentedBboxes(augmentNum);
		std::vector<Mat> currentShapes(augmentNum);

		RNG rng(getTickCount());
		for(int i=0; i<images.size(); i++)
		{
			for(int j=0; j<augment; j++)
			{
				//randomly select initial shape
				int index = 0;
				do{
					index = rng.uniform(0, images.size());
				}while(index == i);

				//augment data
				augmentedImages[i * augment + j] = images[i];
				augmentedTargetShape[i * augment + j] = targetShapes[i];
				augmentedBboxes[i*augment + j] = bboxes[i];
				targetShapes[index].copyTo(currentShapes[i*augment + j]);
			}
		}

		//cascaded training
		regressors.resize(EXTERNAL_REGRESSOR_NUM);
		for(int i=0; i<numRegressor; i++)
		{
			std::cout << "-training external regressor [" << i << "]" << std::endl;
			vector<Mat> predicts = regressors[i].train(augmentedImages, augmentedBboxes, meanShape, augmentedTargetShape, currentShapes);
			//update currentShapes

			for(int j=0; j<predicts.size(); j++)
			{
				currentShapes[j] += predicts[j];
			}
		}
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

	void Regressor::storeModel(std::string filepath)
	{
		std::ofstream fout;
		fout.open(filepath);
		if(!fout.good())
		{
			std::cout << "[Error](storeModel): unable to open '" << filepath << "'" << std::endl;
			return;			
		}
		storeModel(fout);
		fout.close();
		return;
	}

	void Regressor::storeModel(std::ofstream& fout)
	{
		std::cout << "regressors::storeModel" << std::endl;
		storeVal(fout, numRegressor);
		storeVal(fout, numLandmark);
		fout << std::endl;
		for(int i=0; i<numLandmark; i++)
		{
			storeVal(fout, meanShape.at<double>(i,0));
			storeVal(fout, meanShape.at<double>(i,1));
		}
		fout << std::endl;
		int numTraining = trainingShapes.size();
		storeVal(fout, numTraining);
		for(int i=0; i<numTraining; i++)
		{
			const Bbox& bbox = trainingBboxes[i];
			storeVal(fout, bbox.sx);
			storeVal(fout, bbox.sy);
			storeVal(fout, bbox.w);
			storeVal(fout, bbox.h);
			storeVal(fout, bbox.cx);
			storeVal(fout, bbox.cy);

			fout << std::endl;

			for(int j=0; j<numLandmark; j++)
			{
				storeVal(fout, trainingShapes[i].at<double>(j,0));
				storeVal(fout, trainingShapes[i].at<double>(j,1));
			}
			fout << std::endl;
		}
		fout << std::endl;

		for(int i = 0; i< regressors.size(); i++)
		{
			regressors[i].storeModel(fout);
		}

		std::cout << "[Info](storeModel): model storing completed." << std::endl;

	}
}