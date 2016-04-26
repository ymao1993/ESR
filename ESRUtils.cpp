#include "ESRUtils.hpp"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;


namespace ESR
{
	void dispImg(const Mat& mat, bool closeByKey, bool alwaysNewWindow)
	{
		const static std::string titlePrefix = "dispImg";
		static int idx = 0;

		if(mat.data == NULL)
		{
			std::cout << "[Error](dispImg): the matrix contains no data" << std::endl;
			return;
		}
		idx ++;
		std::string windowName = alwaysNewWindow?titlePrefix + std::to_string(idx):titlePrefix;
		if(alwaysNewWindow) namedWindow(windowName, WINDOW_AUTOSIZE);
		imshow(windowName, mat);
		if(closeByKey)
		{
			waitKey(0);
			destroyWindow(windowName);
		}
		return;
	}

	void dispImg(const std::string& filename, bool closeByKey, bool alwaysNewWindow)
	{
		Mat mat = imread(filename, CV_LOAD_IMAGE_UNCHANGED);

		if(mat.data == NULL)
		{
			std::cout << "[Error](dispImg): cannot read image from '" << filename << "'" << std::endl;
			return;
		}		

		dispImg(mat, closeByKey, alwaysNewWindow);
		return;
	}

	void dispImgWithDetection(const cv::Mat& mat, const Bbox& bbox, bool closeByKey, bool alwaysNewWindow)
	{
		Mat temp = mat.clone();
		rectangle(temp, Point(bbox.sx, bbox.sy), Point(bbox.sx + bbox.w, bbox.sy + bbox.h), Scalar(0.0,0.0,255.0,1.0));

		dispImg(temp, closeByKey, alwaysNewWindow);

		return;
	}

	void dispImgWithDetection(const std::string& filename, const Bbox& bbox, bool closeByKey, bool alwaysNewWindow)
	{
		Mat mat = imread(filename, CV_LOAD_IMAGE_UNCHANGED);

		if(mat.data == NULL)
		{
			std::cout << "[Error](dispImgWithDetection): cannot read image from '" << filename << "'" << std::endl;
			return;
		}
		dispImgWithDetection(mat,bbox,closeByKey, alwaysNewWindow);

		return;
	}

	void dispImgWithLandmarks(const cv::Mat& mat, const cv::Mat& landmarks, bool closeByKey, bool alwaysNewWindow)
	{
		Mat temp = mat.clone();
		for(int i=0; i<landmarks.rows; i++)
		{
			circle(temp, Point(landmarks.at<double>(i,0), landmarks.at<double>(i,1)), 3, Scalar(0.0,255.0,0.0,255.0),CV_FILLED);;
		}
		dispImg(temp, closeByKey, alwaysNewWindow);

		return;
	}

	void dispImgWithLandmarks(const std::string& filename, const Mat& landmarks, bool closeByKey, bool alwaysNewWindow)
	{
		Mat mat = imread(filename, CV_LOAD_IMAGE_UNCHANGED);

		if(mat.data == NULL)
		{
			std::cout << "[Error](dispImgWithLandmarks): cannot read image from '" << filename << "'" << std::endl;
			return;
		}

		dispImgWithLandmarks(mat, landmarks, closeByKey, alwaysNewWindow);

		return;		
	}

	void dispImgWithDetectionAndLandmarks(const cv::Mat& mat, const cv::Mat& landmarks, const Bbox& bbox, bool closeByKey, bool alwaysNewWindow)
	{
		Mat temp = mat.clone();
		rectangle(temp, Point(bbox.sx, bbox.sy), Point(bbox.sx + bbox.w, bbox.sy + bbox.h), Scalar(0.0,0.0,255.0,1.0));
		for(int i=0; i<landmarks.rows; i++)
		{
			circle(temp, Point(landmarks.at<double>(i,0), landmarks.at<double>(i,1)), 3, Scalar(0.0,255.0,0.0,255.0),CV_FILLED);;
		}
		dispImg(temp/*(Range(bbox.sy, bbox.sy + bbox.h),Range(bbox.sx, bbox.sx + bbox.w))*/, closeByKey, alwaysNewWindow);
		return;		
	}

	void dispImgWithDetectionAndLandmarks(const std::string& filename, const cv::Mat& landmarks, const Bbox& bbox, bool closeByKey, bool alwaysNewWindow)
	{
		Mat mat = imread(filename, CV_LOAD_IMAGE_UNCHANGED);
		if(mat.data == NULL)
		{
			std::cout << "[Error](dispImgWithDetectionAndLandmarks): cannot read image from '" << filename << "'" << std::endl;
			return;
		}
		dispImgWithDetectionAndLandmarks(mat,landmarks,bbox,closeByKey, alwaysNewWindow);
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

	void readImgGray(const std::string& filename, cv::Mat& result)
	{
		result = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
		if(result.data == NULL)
		{
			std::cout << "[Error](readImgGray): cannot read image from '" << filename << "'" << std::endl;
		}
		return;
	}

	void transformBBox2Image(const cv::Mat& shape, const Bbox& bbox, cv::Mat& result)
	{
		shape.copyTo(result);

		for (int i=0; i<result.rows; i++)
		{
			result.row(i).at<double>(0) = result.row(i).at<double>(0) * (bbox.w / 2.0) + bbox.cx;
			result.row(i).at<double>(1) = result.row(i).at<double>(1) * (bbox.h / 2.0) + bbox.cy;
		}
		return;
	}

	void transformImage2BBox(const cv::Mat& shape, const Bbox& bbox, cv::Mat& result)
	{

		shape.copyTo(result);

		for (int i=0; i<result.rows; i++)
		{
			result.row(i).at<double>(0) = (result.row(i).at<double>(0) - bbox.cx)/(bbox.w/2.0);
			result.row(i).at<double>(1) = (result.row(i).at<double>(1) - bbox.cy)/(bbox.h/2.0);
		}
		return;
	}

	void projectBbox2Bbox(const cv::Mat& shape1, Bbox bbox1, Bbox bbox2, cv::Mat& result)
	{
		transformImage2BBox(shape1,bbox1, result);
		transformBBox2Image(result, bbox2, result);
		return;
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

	double computeCovariance(const cv::Mat& vec1, const cv::Mat& vec2)
	{
		Mat vec1xvec2;
		multiply(vec1, vec2, vec1xvec2);
		double covar = mean(vec1xvec2)[0] - mean(vec1)[0] * mean(vec2)[0];
		return covar;
	}

	//compute the similarity transformation from shape1 to shape2
	void similarityTransform(const Mat& shape1, 
							 const Mat& shape2, 
							 RSTransform& transform)
	{
	    transform.rotation = Mat::zeros(2,2,CV_64FC1);
	    transform.scale = 0;
	    
	    // center the data
	    double centerx1 = mean(shape1.col(0))[0]; 	double centery1 = mean(shape1.col(1))[0];
	    double centerx2 = mean(shape2.col(0))[0];	double centery2 = mean(shape2.col(1))[0];
	    Mat x1,y1,x2,y2;
	    shape1.col(0).copyTo(x1);	shape1.col(1).copyTo(y1);
	    shape2.col(0).copyTo(x2);	shape2.col(1).copyTo(y2);
	    x1 -= centerx1;	y1 -= centery1;
	    x2 -= centerx2;	y2 -= centery2;

	    //compute a and b
	    double tmp = (x1.dot(x1) + y1.dot(y1));
	    double a   = (x1.dot(x2) + y1.dot(y2))/tmp;
	    double b   = (x1.dot(y2) - y1.dot(x2))/tmp;

	    //compute rotation and scaling
	    transform.scale = std::sqrt(a*a+b*b);
	    transform.rotation.create(2,2,CV_64F);
	    transform.rotation.at<double>(0,0) = a;	transform.rotation.at<double>(0,1) = -b;
	    transform.rotation.at<double>(1,0) = b;	transform.rotation.at<double>(1,1) = a;

	    return;
	}

	void applyTransform(const cv::Mat& shape, const RSTransform& transform, cv::Mat& result)
	{
		Mat rotationT;
		transpose(transform.rotation, rotationT);
		result = transform.scale * shape * rotationT;
		return;
	}

	void applyTransform(double x, double y, const RSTransform& transform, double& resultx, double& resulty)
	{
		resultx = (transform.rotation.at<double>(0,0) * x + transform.rotation.at<double>(0,1) * y) * transform.scale;
		resulty = (transform.rotation.at<double>(1,0) * x + transform.rotation.at<double>(1,1) * y) * transform.scale;
		return;
	}

	void computeMeanShape(const std::vector<Mat>& shapes, const std::vector<Bbox>& bboxes, Mat& meanShape)
	{
		if(shapes.size() == 0) return;
		meanShape = Mat::zeros(shapes[0].rows, shapes[0].cols, CV_64F);
		for(int i=0; i<shapes.size(); i++)
		{
			Mat normalizedShape;
			transformImage2BBox(shapes[i], bboxes[i], normalizedShape);
			meanShape += normalizedShape;
		}
		meanShape /= shapes.size();
		return;
	}
}