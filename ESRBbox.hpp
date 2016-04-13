#ifndef ESRBBOX
#define ESRBBOX

namespace ESR
{
	/**
	 * Bounding Box defined in image space
	 */
	struct Bbox
	{
		double sx; //start x
		double sy; //start y
		double cx; //center x
		double cy; //center y
		double w;  //width
		double h;  //height
		Bbox():sx(0),sy(0),cx(0),cy(0),w(0),h(0){}
	};
}

#endif