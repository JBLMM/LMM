#pragma once

#include<boost/function.hpp>

//! YY TOOD: add integration method option latter, now use simpson by default

//! only 1D version for the moment
//! E(f(X_T)) = f(F) + \int_{boundLower}^{F} f''(K) P(K) dK + \int_{0}^{boundUpper} f''(K) C(K) dK
//  F = E(X_T), C(K) = E{(X_T-K)_+}, C(K) = E{(K-X_T)_+}, 
//  you can choose whatever probability to evaluate E, but only to make sure all the E are under the same proba.
class StaticReplicationFormula
{

	boost::function<double (double)> f_;		// only used to evaluate f(F) 
	boost::function<double (double)> f_SecondDerivative_; 
	boost::function<double (double)> C_; 
	boost::function<double (double)> P_; 

	double integrantLowerFunc(double x) const {return f_SecondDerivative_(x)*P_(x);}
	double integrantUpperFunc(double x) const {return f_SecondDerivative_(x)*C_(x);}

	//boost::function<double (double)> integrantLower_;
	//boost::function<double (double)> integrantUpper_;

	double boundLower_;
	double boundUpper_;
	double F_;

	double absoluteAccuracy_;
	size_t maxEvaluations_;

public:
	StaticReplicationFormula(const boost::function<double (double)>& f,
							 const boost::function<double (double)>& f_SecondDerivative,
							 const boost::function<double (double)>& C,
							 const boost::function<double (double)>& P, 
							 double boundLower,
							 double boundUpper,
							 double F,
							 double absoluteAccuracy,
							 size_t maxEvaluations)
							 :
							 f_(f), 
							 f_SecondDerivative_(f_SecondDerivative), 
							 C_(C),
							 P_(P),
							 boundLower_(boundLower),
							 boundUpper_(boundUpper),
							 F_(F),
							 absoluteAccuracy_(absoluteAccuracy),
							 maxEvaluations_(maxEvaluations)

	{
		//integrantLower_ = integrantLower;
		//integrantUpper_ = integrantUpper;
    };

	double evaluate() const; // do integration

	//Getter
	const boost::function<double (double)>& get_f_SecondDerivative() const {return f_SecondDerivative_;}
	const boost::function<double (double)>& get_C() const {return C_;}
	const boost::function<double (double)>& get_P() const {return P_;}
	double get_boundLower() const {return boundLower_;}
	double get_boundUpper() const {return boundUpper_;}
	double get_F() const {return F_;}
};