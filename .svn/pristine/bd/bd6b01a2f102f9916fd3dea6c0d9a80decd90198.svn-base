#include <cassert>
#include <LMM/numeric/NumericalMethods.h>

namespace NumericalMethods
{
	
struct BS_function_helper
{
private:
	boost::function<double(double)> _f;
	boost::function<double(double)> _f_derivative;
	double _target;
	
public:
	//! constructor
	BS_function_helper(boost::function<double(double)>& f,
		               boost::function<double(double)>& f_derivative, 
					   double target) 
					   : _f(f), _f_derivative(f_derivative), _target(target){}

	//! evaluation
	boost::math::tuple<double, double> operator()(double x)
	{
		return boost::math::make_tuple(
			_f(x) - _target,
			_f_derivative(x));
	}
};

double df(const double& f_pp, const double& f_mm, const double& dx)
{
	assert(dx>0);
	double df = f_pp - f_mm;
	return df/(2*dx);
}

double d1(const double& fwd, const double& strike, const double& vol, const double& T)
{
	double variance = vol*vol*T; 
	double d1 = (log(fwd/strike) + 0.5*variance)/sqrt(variance);
	return d1;
}

double d2(const double& fwd, const double& strike, const double& vol, const double& T)
{
	double variance = vol*vol*T; 

	double d1 = (log(fwd/strike) + 0.5*variance)/sqrt(variance);
	double d2 = d1 - sqrt(variance);
	return d2;
}


//! without discount factor: r=0
double Black_Price(const double& fwd, const double& strike, const double& vol, const double& T) 
{
    assert(vol > 0 && T > 0 && fwd >0 && strike >0);

	double variance = vol*vol*T; 

	double d1 = (log(fwd/strike) + 0.5*variance)/sqrt(variance);
	double d2 = d1 - sqrt(variance);

	boost::math::normal_distribution<> nd(0,1); 
	double N1 = cdf(nd,d1);
	double N2 = cdf(nd,d2); 

	return fwd*N1-strike*N2;
}


double Black_Vega(const double& fwd, const double& strike, const double& vol, const double& T)
{
	double variance = vol*vol*T; 

	double d1 = ( log(fwd/strike) + 0.5*variance) / sqrt(variance);

	QuantLib::NormalDistribution ND;
	return fwd*ND(d1)*sqrt(T);
}

//! TT: not that efficient, but who cares ...
double Black_Volga(const double& fwd, const double& strike, const double& vol, const double& T) 
{
	
	double variance = vol*vol*T; 
	double d1 = ( log(fwd/strike) + 0.5*variance ) / sqrt(variance);
	double d2 = d1 - sqrt(variance);

	double vega = Black_Vega(fwd,strike,vol,T);
	return vega*(d1*d2/vol);
}



double Black_impliedVolatility(const double&  bs_call_price, 							  
							   const double&  fwd, 
							   const double&  strike,
							   const double&  T)
{
	//throw("Error bug, it doesn't work."); // TODO, correct it latter. 
	//-- !! variable _1 represents the SQUARED volatility 
	boost::function<double(double)> f = boost::bind(Black_Price,fwd,strike,_1,T);
	boost::function<double(double)> f_derivative = boost::bind(Black_Vega,fwd,strike,_1,T);
	
	BS_function_helper bs_function_helper(f,f_derivative,bs_call_price);
	double initial_guess = 0.02;
	double min    = 10e-5;
	double max    = 1.0;
    size_t nDigits   = 15;
	boost::uintmax_t nMaxIter  = 100;
	double result_newton_raphson = boost::math::tools::newton_raphson_iterate(bs_function_helper, initial_guess, min, max, nDigits);
	return result_newton_raphson;

}


//-- For now, we assume the first point of set is the T0=0 maturity rate (L(0,T0) or P(0,T0))
double linearInterpolation(const double& t, 
						   const std::vector<double>& maturities,
						   const std::vector<double>& set_of_points)
{
	
	size_t index_maturiy_before_t = 0;
	//-- Search the maturities bounding date t
	for (size_t i = 0; i < maturities.size(); ++i)
	{
		if (t > maturities[i])
			index_maturiy_before_t++;
	}

	double date_prev = maturities[index_maturiy_before_t-1];
	double date_next = maturities[index_maturiy_before_t];

	double point_prev = set_of_points[index_maturiy_before_t-1];
	double point_next = set_of_points[index_maturiy_before_t];

	double coeff_1 = (date_next - t)/(date_next - date_prev);
	double coeff_2 = (t - date_prev)/(date_next - date_prev);

	double interpolatedValue = coeff_1*point_prev + coeff_2*point_next;
	return interpolatedValue;
}


double vectorProduct(std::vector<double>& v1, std::vector<double>& v2)
{
    assert(v1.size() == v2.size());

	double result = 0.0;
	for(size_t i=0; i<v1.size(); ++i)
		result += v1[i] * v2[i];

	return result;
}


}// end NumericalMethods