#ifndef ESRFERN_H
#define ESRFERN_H

#include <fstream>
#include <vector>

#include <opencv2/core/core.hpp>
using namespace cv;

namespace ESR
{
	class Fern
	{
	public:
		void loadModel(std::ifstream& fin);

	private:
		//number of fern feature, bin
		int numFeature;
		int numBin;

		//shape indexed features
		int numLandmark;
		Mat featureLandmarks;
		Mat featurePositions;

		//threasholds for each feature
		Mat binThresholds;

		//output table
		std::vector<Mat> output;
	};
}

#endif