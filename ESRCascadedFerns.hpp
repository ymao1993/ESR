#ifndef ESRCASCADEDFERNS_H
#define ESRCASCADEDFERNS_H

#include <fstream>
#include <vector>
#include "ESRFern.hpp"

namespace ESR
{
	class CascadedFerns
	{
	public:
		void loadModel(std::ifstream& fin);

	private:
		int numRegressor;
		std::vector<Fern> regressors;
	};
}


#endif