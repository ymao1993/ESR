#include "ESRCascadedFerns.hpp"

#include <iostream>
#include "ESRUtils.hpp"

using namespace cv;

namespace ESR
{
	void CascadedFerns::loadModel(std::ifstream& fin)
	{
		fin >> numRegressor;
		regressors.resize(numRegressor);
		for(int i=0; i<numRegressor; i++)
		{
			regressors[i].loadModel(fin);
		}
		return;
	}

	void CascadedFerns::predict(const Mat& image, Bbox bbox, const Mat& curShape, const Mat& meanShape, Mat& deltaShape)
	{
		RSTransform t;
		similarityTransform(meanShape, curShape, t);
		deltaShape = Mat::zeros(curShape.rows, curShape.cols, CV_64F);
		for(int i=0; i<regressors.size(); i++)
		{
			Mat deltadeltaShape;
			regressors[i].predict(image, bbox, t, curShape, deltadeltaShape);
			deltaShape += deltadeltaShape;
		}
		return;
	}
}