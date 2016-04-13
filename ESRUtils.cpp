#include "ESRUtils.hpp"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;


namespace ESR
{
	void dispImg(const Mat& mat, bool autoClose)
	{
		const static std::string titlePrefix = "dispImg ";
		static int idx = 0;

		if(mat.data == NULL)
		{
			std::cout << "[Error](dispImg): the matrix contains no data" << std::endl;
			return;
		}
		idx ++;
		std::string windowName = titlePrefix + std::to_string(idx);
		namedWindow(windowName, WINDOW_AUTOSIZE);
		imshow(windowName, mat);
		waitKey(0);
		if(autoClose)
		{
			destroyWindow(windowName);
		}
		return;
	}

	void dispImg(const std::string& filename, bool autoClose)
	{
		Mat mat = imread(filename, CV_LOAD_IMAGE_UNCHANGED);

		if(mat.data == NULL)
		{
			std::cout << "[Error](dispImg): cannot read image from '" << filename << "'" << std::endl;
			return;
		}		

		dispImg(mat, autoClose);
		return;
	}

	void saveImg(const std::string& filename, cv::Mat& mat)
	{
		bool result = imwrite(filename, mat);
		if(!result)
		{
			std::cout << "[Error](saveImg): cannot save image to '" << filename << "'" << std::endl;
		}
		return;
	}

	cv::Mat readImgGray(const std::string& filename)
	{
		Mat mat = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
		if(mat.data == NULL)
		{
			std::cout << "[Error](readImgGray): cannot read image from '" << filename << "'" << std::endl;
		}
		return mat;
	}

}