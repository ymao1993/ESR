#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

#include "ESRRegressor.hpp"

#include "ESRUtils.hpp"

void dispTrainingImages();

void testSimilarityTransform();
void testApplyTransform();

int main()
{
	std::cout << "ESRTest" << std::endl;

	//load in model
	ESR::Regressor regressor;
	regressor.loadModel("data/model.txt");

	for(int i=0; i<regressor.trainingBboxes.size(); i++)
	{
		std::cout << "processing image " << i+1 <<std::endl;
		Mat image, shape;
		ESR::readImgGray("./data/COFW_Dataset/trainingImages/" + std::to_string(i+1) + ".jpg", image);
		regressor.predict(image, regressor.trainingBboxes[i],shape);
		ESR::dispImgWithDetectionAndLandmarks(image, shape, regressor.trainingBboxes[i], true);
	}

	return 0;
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


void testSimilarityTransform()
{
	 Mat A = (Mat_<double>(4,2) << 1, 5, 2, 6, 3, 7, 4, 8);
	 Mat B = (Mat_<double>(4,2) << -1.6340, 4.8301, -1.2679, 6.1962, -0.9019, 7.5622, -0.5359, 8.9282);

	 std::cout << A << std::endl;
	 std::cout << B << std::endl;

	 ESR::RSTransform t;
	 ESR::similarityTransform(A,B,t);
	 std::cout << "Rotation:" << std::endl << " " << t.rotation << std::endl;
	 std::cout << "Scaling:" << t.scale << std::endl;
}

void testApplyTransform()
{
	Mat A = (Mat_<double>(4,2) << 1, 5, 2, 6, 3, 7, 4, 8);
	Mat r = (Mat_<double>(2,2) << 0.8660, -0.5000, 0.5000, 0.8660);

	std::cout << A << std::endl;
	std::cout << r << std::endl;

	double scale = 1;
	ESR::RSTransform t;
	t.scale = scale;
	t.rotation = r;
	Mat B;
	ESR::applyTransform(A, t, B);
	std::cout << "B:" << std::endl << " " << B << std::endl;
	return;
}
