#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

#include "ESRRegressor.hpp"

int main()
{
	std::cout << "ESRTest" << std::endl;

	//load in model
	ESR::Regressor regressor;
	regressor.loadModel("data/model.txt");

	//predict on test image file
	

	return 1;
}
