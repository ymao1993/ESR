#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>

using namespace cv;

#include "ESRRegressor.hpp"
#include "ESRUtils.hpp"
#include "ESRFaceDetector.hpp"
#include "ESRBbox.hpp"
#include "ESRCommon.hpp"

void dispTrainingImages();
void testSimilarityTransform();
void testApplyTransform();

//#define USE_TEST_IMAGE
#define USE_WEBCAM
//#define USE_SINGLE_IMAGE

int main()
{
	std::cout << "ESRTest" << std::endl;

	//load in model
	ESR::Regressor regressor;
	regressor.loadModel("data/myModel.txt");
	//regressor.storeModel("data/model2.txt");

	//return 0;

#ifdef USE_TEST_IMAGE

	//Read testing image's bounding box
	std::vector<ESR::Bbox> testingBboxes;
	std::ifstream fin;
	fin.open("./data/COFW_Dataset/boundingbox_test.txt");
    for(int i = 0;i < TEST_IMAGE_NUM;i++){
        ESR::Bbox temp;
        fin>>temp.sx>>temp.sy>>temp.w>>temp.h;
        temp.cx = temp.sx + temp.w/2.0;
        temp.cy = temp.sy + temp.h/2.0; 
        testingBboxes.push_back(temp);
    }
    fin.close(); 

    //apply regression
	for(int i=0; i<regressor.trainingBboxes.size(); i++)
	{
		std::cout << "processing image " << i+1 <<std::endl;
		Mat image, shape;
		ESR::readImgGray("./data/COFW_Dataset/testImages/" + std::to_string(i+1) + ".jpg", image);
		regressor.predict(image, testingBboxes[i],shape);
		ESR::dispImgWithDetectionAndLandmarks(image, shape, testingBboxes[i], true, true);
	}
#endif

#ifdef USE_WEBCAM
	ESR::FaceDetector faceDetector;
	faceDetector.loadModel("data/haarcascade_frontalface_alt.xml");
	VideoCapture stream(0);  
	while(1)
	{
		 Mat image, shape, image_gray;
		 stream.read(image);
		 cvtColor(image, image_gray, CV_BGR2GRAY );

		 std::vector<ESR::Bbox> faces = faceDetector.detectFace(image_gray);
		 if(faces.size() != 0) //only perform regression when there is face
		 {
		 	regressor.predict(image_gray, faces[0],shape);
		 	ESR::dispImgWithDetectionAndLandmarks(image_gray, shape, faces[0], false, false);
		 }
		 waitKey(10);
	}
#endif

#ifdef USE_SINGLE_IMAGE
	ESR::FaceDetector faceDetector;
	faceDetector.loadModel("data/haarcascade_frontalface_alt.xml");
	cv::Mat image, shape;
	image = cv::imread("./data/test1.png",CV_LOAD_IMAGE_GRAYSCALE);
	std::vector<ESR::Bbox> faces = faceDetector.detectFace(image);
	if(faces.size() != 0) //only perform regression when there is face
	{
		regressor.predict(image, faces[0],shape);
		ESR::dispImgWithDetectionAndLandmarks(image, shape, faces[0], true, true);
	}
#endif

	return 0;
}

void dispTrainingImages()
{
	ESR::Regressor regressor;
	regressor.loadModel("data/model.txt");
	for(int i=0; i < regressor.trainingBboxes.size(); i++)
	{
		std::cout << "show image "<<i+1 << std::endl;
		ESR::dispImgWithDetectionAndLandmarks("./data/COFW_Dataset/trainingImages/" + std::to_string(i+1) + ".jpg", regressor.trainingShapes[i], regressor.trainingBboxes[i], true, true);
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