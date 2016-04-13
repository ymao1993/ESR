#include "ESRRegressor.hpp"

#include <fstream>
#include <iostream>

namespace ESR
{
	Mat Regressor::regress(Mat image, Bbox bbox, Mat S0)
	{
		Mat result;
		return result;
	}

	void Regressor::loadModel(std::string filepath)
	{
		std::ifstream fin;
		fin.open(filepath);
		if(!fin.good())
		{
			std::cout << "[Error](loadModel): unable to open '" << filepath << "'" << std::endl;
			return;
		}
		loadModel(fin);
		fin.close();
		return;
	}

	void Regressor::loadModel(std::ifstream& fin)
	{
		//TODO: load in the model
	}

}