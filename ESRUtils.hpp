/**
 * Utility functions used in ESR
 */

#ifndef ESRUTILS_H
#define ESRUTILS_H
#include <opencv2/core/core.hpp>
#include <string>

#include "ESRBbox.hpp"

namespace ESR
{

////Image IO and visualization helper functions

	/**
	 * display the image by always opening a new window and wait for key to close.
	 */
	void dispImg(const cv::Mat& mat, bool autoClose = false);
	void dispImg(const std::string& filename, bool autoClose = false);
	void dispImgWithDetection(const std::string& filename, const Bbox& bbox, bool autoClose = false);
	void dispImgWithLandmarks(const std::string& filename, const cv::Mat& landmarks, bool autoClose);
	void dispImgWithDetectionAndLandmarks(const std::string& filename, const cv::Mat& landmarks, const Bbox& bbox, bool autoClose);
	/**
	 * safe wrapper of image write function
	 */
	void saveImg(const std::string& filename, cv::Mat& mat);

	/**
	 * read file as gray scale image
	 */
	cv::Mat readImgGray(const std::string& filename);

	/**
	 * compute Pearson Correlation between the two vectors
	 */
	double pearsonCorrelation(const cv::Mat& vec1, const cv::Mat& vec2);

	/**
	 * transform the coordates from bbox's normalized space to image space
	 */
	cv::Mat TransformBBox2Image(const cv::Mat& shape, const Bbox& bbox);

	/**
	 * transform the coordates from image space to tbbox's normalized space
	 */
	cv::Mat TransformImage2BBox(const cv::Mat& shape, const Bbox& bbox);

	/**
	 * project the shape from image1's bbox into image2's bbox
	 *
	 * @param shape1 shape1's landmarks' coordinates in image space
	 * @param bbox1  shape1's corresponding bounding box
	 * @param bbox2  the bounding box where shape1 is going to be projected to
	 * @return projected shape1's landmarks' coordinate in image space
	 */
	cv::Mat ProjectBbox2Bbox(const cv::Mat& shape1, Bbox bbox1, Bbox bbox2);



}



#endif