#include <LMM/numeric/BilinearInterpolation.h>

namespace numeric 
{

BilinearInterpolation::BilinearInterpolation()
{}

double BilinearInterpolation::interpolate(const double& xp, const double& yp, // interpolation on coordinates (xp,yp)
										  const double& x1, const double& x2,
										  const double& y1, const double& y2,
					                      const double& a , const double& b,
					                      const double& c , const double& d) const
	                                      
{
	/* NumericalTools::Interpolation interpolator;
	 * double f_ab = interpolator.linearInterpolation(x1,x2,a,b,xp);
	 * double f_cd = interpolator.linearInterpolation(x1,x2,c,d,xp);
	 * double fp = interpolator.linearInterpolation(y1,y2,f_ab,f_cd,yp);
	 */

	double t = (xp-x1)/(x2-x1);
	double u = (yp-y1)/(y2-y1);

	double res = (1-t)*(1-u)*a + t*(1-u)*b + t*u*c + (1-t)*u*d;
	return res;
} 

} // end numeric