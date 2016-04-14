#ifndef ESRREGRESSOR_H
#define ESRREGRESSOR_H

#include <string>
#include <opencv2/core/core.hpp>
#include "ESRBbox.hpp"
#include "ESRCascadedFerns.hpp"

#include <fstream>

#define DEBUG_MODE

using namespace cv;

namespace ESR
{
	class Regressor
	{

	public:
		/**
		 * load model from file
		 */
		void loadModel(std::string filepath);

		/**
		 * load model from stream
		 */
		void loadModel(std::ifstream& fin);

		/**
		 * predict landmark positions by applying regression
		 */
		Mat regress(Mat image, Bbox bbox, Mat S0);

#ifndef DEBUG_MODE
	private:
#endif
		int numRegressor;
		int numLandmark;
        Mat meanShape;
        std::vector<CascadedFerns> regressors;

        //training data
        std::vector<Mat> trainingShapes;
        std::vector<Bbox> trainingBboxes;
        
	};
}


#endif