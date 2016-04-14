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

	void dispImgWithDetection(const std::string& filename, const Bbox& bbox, bool autoClose)
	{
		Mat mat = imread(filename, CV_LOAD_IMAGE_UNCHANGED);

		if(mat.data == NULL)
		{
			std::cout << "[Error](dispImgWithDetection): cannot read image from '" << filename << "'" << std::endl;
			return;
		}

		rectangle(mat, Point(bbox.sx, bbox.sy), Point(bbox.sx + bbox.w, bbox.sy + bbox.h), Scalar(255.0,0.0,0.0,1.0));

		dispImg(mat, autoClose);
		return;
	}

	void dispImgWithLandmarks(const std::string& filename, const Mat& landmarks, bool autoClose)
	{
		Mat mat = imread(filename, CV_LOAD_IMAGE_UNCHANGED);

		if(mat.data == NULL)
		{
			std::cout << "[Error](dispImgWithLandmarks): cannot read image from '" << filename << "'" << std::endl;
			return;
		}

		for(int i=0; i<landmarks.rows; i++)
		{
			circle(mat, Point(landmarks.at<double>(i,0), landmarks.at<double>(i,1)), 3, Scalar(255.0,0.0,0.0,1.0));
		}
		dispImg(mat, autoClose);
		return;		
	}

	void dispImgWithDetectionAndLandmarks(const std::string& filename, const cv::Mat& landmarks, const Bbox& bbox, bool autoClose)
	{
		Mat mat = imread(filename, CV_LOAD_IMAGE_UNCHANGED);

		if(mat.data == NULL)
		{
			std::cout << "[Error](dispImgWithDetectionAndLandmarks): cannot read image from '" << filename << "'" << std::endl;
			return;
		}

		rectangle(mat, Point(bbox.sx, bbox.sy), Point(bbox.sx + bbox.w, bbox.sy + bbox.h), Scalar(255.0,0.0,0.0,1.0));
		for(int i=0; i<landmarks.rows; i++)
		{
			circle(mat, Point(landmarks.at<double>(i,0), landmarks.at<double>(i,1)), 3, Scalar(255.0,0.0,0.0,1.0));
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

	cv::Mat TransformBBox2Image(const cv::Mat& shape, const Bbox& bbox)
	{
		Mat result;
		shape.copyTo(result);

		for (int i=0; i<result.rows; i++)
		{
			result.row(i).at<double>(i,0) = result.row(i).at<double>(i,0) * bbox.w + bbox.sx;
			result.row(i).at<double>(i,1) = result.row(i).at<double>(i,0) * bbox.h + bbox.sy;
		}

		return result;
	}

	cv::Mat TransformImage2BBox(const cv::Mat& shape, const Bbox& bbox)
	{
		Mat result;
		shape.copyTo(result);

		for (int i=0; i<result.rows; i++)
		{
			result.row(i).at<double>(i,0) = (result.row(i).at<double>(i,0) - bbox.sx)/bbox.w;
			result.row(i).at<double>(i,1) = (result.row(i).at<double>(i,0) - bbox.sy)/bbox.h;
		}
		
		return result;
	}

	cv::Mat ProjectBbox2Bbox(const cv::Mat& shape1, Bbox bbox1, Bbox bbox2)
	{
		return TransformBBox2Image(TransformImage2BBox(shape1,bbox1),bbox2);
	}

	double pearsonCorrelation(const cv::Mat& vec1, const cv::Mat& vec2)
	{
		//compute mean and standard deviation
		Scalar m1,m2;
		Scalar std1,std2;
		meanStdDev(vec1,m1,std1);
		meanStdDev(vec2,m2,std2);

		//compute covariance
		Mat vec3;
		multiply(vec1 - m1, vec2 - m1, vec3);
		double covariance = mean(vec3)[0];
		
		//compute pearson correlation
		//which is defined as covar(v1,v2)/(std(v1) * std(v2))
		double correlation = covariance / (std1[0] * std2[0]);

		return correlation;
	}


}