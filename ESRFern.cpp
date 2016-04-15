#include "ESRFern.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#include <fstream>
#include <math.h>

#include "ESRUtils.hpp"

using namespace cv;

namespace ESR
{
	void Fern::loadModel(std::ifstream& fin)
	{
		fin >> numFeature;
		fin >> numLandmark;
		featureLandmarks.create(numFeature, 2, CV_32S);
		featurePositions.create(numFeature, 4, CV_64F);
		featureThresholds.create(numFeature, 1, CV_64F);
		
		//load position indexed feature definition
		for(int i=0; i<numFeature; i++)
		{
			 fin >> featurePositions.at<double>(i,0) >> featurePositions.at<double>(i,1)
			 	 >> featurePositions.at<double>(i,2) >> featurePositions.at<double>(i,3);

			fin >> featureLandmarks.at<int>(i,0) >> featureLandmarks.at<int>(i,1);
			fin >> featureThresholds.at<double>(i);
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

	void Fern::predict(const cv::Mat& image, Bbox bbox, const RSTransform& t, const Mat& curShape, cv::Mat& deltadeltaShape)
	{
		//classification
		int index = 0;
		for(int i=0; i<numFeature; i++)
		{
			double pixelDiff = extractFeature(i,image,bbox,curShape,t);

			if(pixelDiff >= featureThresholds.at<double>(i))
			{
				index += 1<<i;
			}
		}

		//look up output table
		applyTransform(output[index], t, deltadeltaShape);
		return;
	}


	/**
	 * extract pose index feature
	 */
	double Fern::extractFeature(int featureIdx, const Mat& image, const Bbox& bbox, const Mat& curShape, const RSTransform& t)
	{
			int landmark1 = featureLandmarks.at<int>(featureIdx,0);
			int landmark2 = featureLandmarks.at<int>(featureIdx,1);

			double pos1x  = featurePositions.at<double>(featureIdx,0);
			double pos1y  = featurePositions.at<double>(featureIdx,1);
			double pos2x  = featurePositions.at<double>(featureIdx,2);
			double pos2y  = featurePositions.at<double>(featureIdx,3);

			double x1,y1,x2,y2;
			applyTransform(pos1x, pos1y, t, x1, y1);
			applyTransform(pos2x, pos2y, t, x2, y2);

			int realx1 = (x1  + curShape.at<double>(landmark1,0)) * (bbox.w/2.0) + bbox.cx;
			int realy1 = (y1  + curShape.at<double>(landmark1,1)) * (bbox.h/2.0) + bbox.cy;
			int realx2 = (x2  + curShape.at<double>(landmark2,0)) * (bbox.w/2.0) + bbox.cx;
			int realy2 = (y2  + curShape.at<double>(landmark2,1)) * (bbox.h/2.0) + bbox.cy;

			realx1 = max(min(realx1,image.cols-1),0);
			realy1 = max(min(realy1,image.rows-1),0);
			realx2 = max(min(realx2,image.cols-1),0);
			realy2 = max(min(realy2,image.rows-1),0);

			double pixelDiff = (int)image.at<uint8_t>(realy1, realx1) - 
				   (int)image.at<uint8_t>(realy2, realx2);

			return 	pixelDiff;	
	}


}