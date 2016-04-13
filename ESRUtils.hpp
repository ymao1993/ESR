/**
 * Utility functions used in ESR
 */

#ifndef ESRUTILS_H
#define ESRUTILS_H
#include <opencv2/core/core.hpp>
#include <string>

namespace ESR
{

////Image IO and visualization helper functions

	/**
	 * display the image by always opening a new window and wait for key to close.
	 */
	void dispImg(const cv::Mat& mat, bool autoClose = false);
	void dispImg(const std::string& filename, bool autoClose = false);

	/**
	 * safe wrapper of image write function
	 */
	void saveImg(const std::string& filename, cv::Mat& mat);

	/**
	 * read file as gray scale image
	 */
	cv::Mat readImgGray(const std::string& filename);

}



#endif