#ifndef ESRCASCADEDFERNS_H
#define ESRCASCADEDFERNS_H

#include <fstream>
#include <vector>
#include "ESRFern.hpp"
#include "ESRBbox.hpp"
#include <opencv2/core/core.hpp>


namespace ESR
{
	class CascadedFerns
	{
	public:
		void loadModel(std::ifstream& fin);
		void predict(const cv::Mat& image, Bbox bbox, const Mat& curShape, const Mat& meanShape, cv::Mat& shape);

	private:
		int numRegressor;
		std::vector<Fern> regressors;
	};
}


#endif