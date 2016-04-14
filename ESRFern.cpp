#include "ESRFern.hpp"

#include <opencv2/core/core.hpp>
#include <iostream>

#include <fstream>
#include <math.h>

using namespace cv;

namespace ESR
{
	void Fern::loadModel(std::ifstream& fin)
	{
		fin >> numFeature;
		fin >> numLandmark;
		featureLandmarks.create(numFeature, 2, CV_64F);
		featurePositions.create(numFeature, 4, CV_64F);
		binThresholds.create(numFeature, 1, CV_64F);
		
		//load position indexed feature definition
		for(int i=0; i<numFeature; i++)
		{
			fin >> featurePositions.at<double>(i,0) >> featurePositions.at<double>(i,1)
				>> featurePositions.at<double>(i,2) >> featurePositions.at<double>(i,3);
			fin >> featureLandmarks.at<double>(i,0) >> featureLandmarks.at<double>(i,1);
			fin >> binThresholds.at<double>(i);
		}
		
		//load bins' output table
		numBin = std::pow(2,numFeature);
		output.resize(numBin);
		for(int i = 0; i < numBin; i++)
		{
			Mat tmp(numLandmark, 2, CV_64F);
			for(int j =0; j< numLandmark; j++)
			{
				fin >> tmp.at<double>(j,0) 
					>> tmp.at<double>(j,1); 
			}
			output[i] = tmp;
		}

		return;
	}


}