#include "ESRCascadedFerns.hpp"

#include <fstream>

namespace ESR
{
	void CascadedFerns::loadModel(std::ifstream& fin)
	{
		fin >> numRegressor;
		regressors.resize(numRegressor);
		for(int i=0; i<numRegressor; i++)
		{
			regressors[i].loadModel(fin);
		}
		return;
	}

	/*
	*
	*/
}