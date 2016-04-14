#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

#include "ESRRegressor.hpp"

#include "ESRUtils.hpp"

void dispTrainingImages();

int main()
{
	std::cout << "ESRTest" << std::endl;

	//load in model
	ESR::Regressor regressor;
	regressor.loadModel("data/model.txt");

	//predict on test image file
	

	return 1;
}

void dispTrainingImages()
{
	ESR::Regressor regressor;
	regressor.loadModel("data/model.txt");
	for(int i=0; i < regressor.trainingBboxes.size(); i++)
	{
		ESR::dispImgWithDetectionAndLandmarks("./data/COFW_Dataset/trainingImages/" + std::to_string(i+1) + ".jpg", regressor.trainingShapes[i], regressor.trainingBboxes[i],  true);
	}
}