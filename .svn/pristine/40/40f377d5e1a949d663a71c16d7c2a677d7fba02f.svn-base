#ifndef LMM_NUMERIC_BILINEAR_INTERPOLATION_H
#define LMM_NUMERIC_BILINEAR_INTERPOLATION_H
#pragma once

#include <vector>
#include <LMM/numeric/Interpolation.h>

typedef std::vector<std::vector<double> > matrix_;

namespace numeric 
{

class BilinearInterpolation
{

public:
           
	BilinearInterpolation();

	//-- Must give the four inputs from lower left, then lower right, ... 
	double interpolate(const double& xp, const double& yp, // interpolation on coordinates (xp,yp)
		               const double& x1, const double& x2,
					   const double& y1, const double& y2,
					   const double& a , const double& b,
					   const double& c , const double& d) const;
};


} //end numeric

#endif /* LMM_NUMERIC_BILINEAR_INTERPOLATION_H */
