#include "BS.h"
#include <boost/math/distributions.hpp>


double BS::BlackFormula(double forward,
						double strike,
						double sigma,
						double discount) 
{
	if (sigma < 0)
		throw ("Error : the variance input cannot be negative");

	double d1 = (log(forward/strike) + 0.5*sigma*sigma)/sigma;
	double d2 = d1 - sigma;

	boost::math::normal_distribution<> nd(0,1); 
	double N1 = cdf(nd,d1);
	double N2 = cdf(nd,d2); 

	return discount*(forward*N1-strike*N2);
}