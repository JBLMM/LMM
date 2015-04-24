#pragma once

#include <vector>

#include <boost/math/distributions.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include <ql/math/distributions/normaldistribution.hpp>

//TODO: delete const double &, use only double .
namespace NumericalMethods
{
	//! finite difference method for univariate function f
	double df(const double& f_pp, const double& f_mm, const double& dx);
	
	double d1(const double& fwd, const double& strike, const double& vol, const double& T);

	double d2(const double& fwd, const double& strike, const double& vol, const double& T);

	// Compute a derivative's price using Black's formula
	double Black_Price(const double& fwd, const double& strike, const double& vol, const double& T);

	double Black_Vega(const double& fwd, const double& strike, const double& vol, const double& T);

	double Black_Volga(const double& fwd, const double& strike, const double& vol, const double& T);

	double Black_impliedVolatility(const double& bs_call_price, const double& fwd, const double& strike, const double& T);

	double linearInterpolation(
							   const double& t, 
		                       const std::vector<double>& maturities,
		                       const std::vector<double>& set_of_points
							   );

	double vectorProduct(std::vector<double>& v1, std::vector<double>& v2) ;
	
} // end NumericalMethods

