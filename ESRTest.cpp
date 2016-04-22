#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>

using namespace cv;

#include "ESRRegressor.hpp"
#include "ESRUtils.hpp"
#include "ESRFaceDetector.hpp"
#include "ESRBbox.hpp"
#include "ESRCommon.hpp"

void testSimilarityTransform();
void testApplyTransform();

int main()
{
	std::cout << "ESRTest" << std::endl;

	//load in model
	ESR::Regressor regressor;
	regressor.loadModel(MODEL_FILE_NAME);
	//return 0;

#ifdef USE_TEST_IMAGE

	std::vector<int> inValidIdx;
	std::vector<Mat> testImages;
	std::vector<ESR::Bbox> testingBboxes;

	//Read testing images
	for(int i=0; i<NUM_TESTING_IMAGE; i++)
	{
		char imageName[100];
		sprintf(imageName, "image_%04d.png", i+1);
		std::string filename = std::string(TEST_IMAGE_FOLDER_NAME) + imageName;
		Mat image = imread(filename,CV_LOAD_IMAGE_GRAYSCALE);
		if(image.data == NULL)
		{
			inValidIdx.push_back(i);
			continue;
		}
		testImages.push_back(image);
	}

	//Read testing image's bounding box
	std::ifstream fin;
	fin.open(TEST_BBOX_FILE_NAME);
    for(int i = 0;i < NUM_TESTING_IMAGE;i++){
		//skip all invalid idxes
		for(int j=0; j<inValidIdx.size(); j++)
		{
			if(inValidIdx[j] == i) continue;
		}
        ESR::Bbox temp;
        double brx, bry;
        fin>>temp.sx>>temp.sy>>brx>>bry;
        temp.w  = brx - temp.sx;
        temp.h  = bry - temp.sy;
        temp.cx = temp.sx + temp.w/2.0;
        temp.cy = temp.sy + temp.h/2.0;
        testingBboxes.push_back(temp);
    }
    fin.close(); 

    //apply regression
	for(int i=0; i<testImages.size(); i++)
	{
		std::cout << "processing image " << i+1 <<std::endl;

		Mat shape;
		regressor.predict(testImages[i], testingBboxes[i],shape);
		ESR::dispImgWithDetectionAndLandmarks(testImages[i], shape, testingBboxes[i], true, true);
	}
#endif

#ifdef USE_WEBCAM
	ESR::FaceDetector faceDetector;
	faceDetector.loadModel(FACE_DETECTION_MODEL_FILE_NAME);
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

	return 0;
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