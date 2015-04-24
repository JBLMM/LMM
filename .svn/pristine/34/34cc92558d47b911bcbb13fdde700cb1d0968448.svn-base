#pragma once

#include <iostream>
#include <cassert>
#include <string>
#include <cassert>

#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <LMM/helper/Name.h>
#include <LMM/instrument/VanillaSwap.h>
#include <LMM/pricer/McLmmVanillaSwapPricer.h>
#include <cmath>
#include <vector>

#pragma once

typedef boost::numeric::ublas::matrix<double> matrix;
//-----------------------------------------------------------------------------------
//
//                         EV
//
//-----------------------------------------------------------------------------------
class EV
{
public:
    virtual double evaluate_ev(LMM::Index indexValuationDate, 
		                 const matrix& liborMatrix, 
						 const std::vector<double>& numeraire) const = 0;

	virtual ~EV(){}
};
typedef boost::shared_ptr<EV> EV_PTR;

class EV_VanillaSwapRate : public EV
{
	VanillaSwap vanillaSwap_;
	McLmmVanillaSwapPricer_PTR mcLmmVanillaSwapPricer_PTR_;

public:
	EV_VanillaSwapRate(const VanillaSwap& vanillaSwap,
					   McLmmVanillaSwapPricer_PTR mcLmmVanillaSwapPricer_PTR) 
					   : vanillaSwap_(vanillaSwap)
						,mcLmmVanillaSwapPricer_PTR_(mcLmmVanillaSwapPricer_PTR)
	{}
	virtual ~EV_VanillaSwapRate(){}

	virtual double evaluate_ev(LMM::Index indexValuationDate, 
						  const matrix& liborMatrix, 
						  const std::vector<double>& numeraire) const
	{
		return mcLmmVanillaSwapPricer_PTR_->swapRate(indexValuationDate, vanillaSwap_, numeraire, liborMatrix);
	}

};
typedef boost::shared_ptr<EV_VanillaSwapRate> EV_VanillaSwapRate_PTR;

class EV_LiborRate : public EV
{
	LMM::Index liborIndex_;

public:
	EV_LiborRate(LMM::Index liborIndex): liborIndex_(liborIndex_){}
	virtual ~EV_LiborRate(){}
	virtual double evaluate_ev(LMM::Index indexValuationDate, 
						  const matrix& liborMatrix, 
						  const std::vector<double>& numeraire) const
	{
		return liborMatrix(liborIndex_, indexValuationDate); // numeraire not used !
	} 
};
typedef boost::shared_ptr<EV_LiborRate> EV_LiborRate_PTR;

class EV_payoffCall: public EV
{
	EV_PTR ev_ptr_;
	double strike_;

public:
	EV_payoffCall(EV_PTR ev_ptr, double strike)
		:ev_ptr_(ev_ptr), strike_(strike){}
	virtual ~EV_payoffCall(){};

	virtual double evaluate_ev(LMM::Index indexValuationDate, 
						  const matrix& liborMatrix, 
						  const std::vector<double>& numeraire) const
	{
		double rate = ev_ptr_->evaluate_ev(indexValuationDate, liborMatrix, numeraire);
		return std::max(rate-strike_, 0.0);  
	} 
}; 
typedef boost::shared_ptr<EV_payoffCall> EV_payoffCall_PTR;

class EV_payoffPut: public EV
{
	EV_PTR ev_ptr_;
	double strike_;

public:
	EV_payoffPut(EV_PTR ev_ptr, double strike)
		: ev_ptr_(ev_ptr), strike_(strike){}
	virtual ~EV_payoffPut(){};

	virtual double evaluate_ev(LMM::Index indexValuationDate, 
		const matrix& liborMatrix, 
		const std::vector<double>& numeraire) const
	{
		double rate = ev_ptr_->evaluate_ev(indexValuationDate, liborMatrix, numeraire);
		return std::max(strike_-rate, 0.0);  
	} 
}; 
typedef boost::shared_ptr<EV_payoffPut> EV_payoffPut_PTR;


//-----------------------------------------------------------------------------------
//
//                         RegressionBasis
//
//-----------------------------------------------------------------------------------
class RegressionBasis
{
	//size_t basisSize_;
protected:
	std::vector<double> basis_;

public:
	RegressionBasis(){}
	virtual ~RegressionBasis(){};

	virtual void calculateBasisFromEV(const std::vector<double>& evs) =0;
	const std::vector<double>& get_basisRef() {return basis_;}
	size_t get_nbBasis() const {return basis_.size();}
};
typedef boost::shared_ptr<RegressionBasis> RegressionBasis_PTR;

class PolynormialRegressionBasis: public RegressionBasis
{
    size_t monomialMaxOrder_;    // example: ==4, x1,x1^2,x1^3,x1^4, (but x1^5 not allowed)
	size_t corssnomialMaxOrder_; // fixed for the moment to be 2: i.e: x1*x2, x2*x3 OK (x1*x2*x3 or x1^2*x3 not allowed)

public:
	PolynormialRegressionBasis(size_t monomialMaxOrder, 
							   size_t corssnomialMaxOrder)
		 : RegressionBasis()
		  ,monomialMaxOrder_(monomialMaxOrder)
		  ,corssnomialMaxOrder_(corssnomialMaxOrder)
	{
		assert(corssnomialMaxOrder_ == 2);
	}
	virtual ~PolynormialRegressionBasis(){};

	virtual void calculateBasisFromEV(const std::vector<double>& evs)
	{
		size_t totalBasisSize = 1 + evs.size()*monomialMaxOrder_ + evs.size()*(evs.size()-1)/2;
		basis_ = std::vector<double>(totalBasisSize,-1.0e100);

		size_t index = 0;
		//! const size =1
		basis_[index] = 1.0;
		++index; 

		//! monomial: size = evs.size()*monomialMaxOrder_
		for(size_t i=0; i<monomialMaxOrder_; ++i)
		{
			for(size_t j=0; j<monomialMaxOrder_; ++j)
			{
				basis_[index] = std::pow(evs[i], j+1); // YY TODO: not efficient at all ! 
				++index;
			}
		}

		//!corssnomial: size = evs.size()*(evs.size()-1)/2
		// ATTENTION: it is supposed that: corssnomialMaxOrder_=2
		for(size_t i=0; i<evs.size(); ++i)
		{
			for(size_t j=i+1; j<evs.size(); ++j)
			{
				basis_[index] = evs[i]*evs[j];
			}
		}
	}
};
typedef boost::shared_ptr<PolynormialRegressionBasis> PolynormialRegressionBasis_PTR;


//-----------------------------------------------------------------------------------
//
//                         Regression
//
//-----------------------------------------------------------------------------------
class Regression
{
protected:
	size_t dim_Y_; // == 1
	size_t dim_X_; // == m 

	//matrix coeff_;  // column vector: size = (m,1)
	std::vector<double> coeff_; //! the type doesn't matter, can be changed latter.

	//matrix regressed_Y_; // X*coeff_;  X may be different than the one used for calculate coeff_;
	                     // size = (N,1)

public:
	Regression(size_t nbVariable_Y, size_t nbVarialbe_X) 
		:  dim_Y_(nbVariable_Y)
		 , dim_X_(nbVarialbe_X)
	     , coeff_(dim_X_,1)
	{ 
		assert(nbVariable_Y ==1 && nbVarialbe_X >=1);
	}
	virtual ~Regression(){};

	virtual void doRegression(const matrix& Y, const matrix& X) = 0;  // Y: size=(N,1), X=(X_1, X_2, ..., X_m): size = (N,m) 

	//!YY TODO: this function called thousand times, need to optimize it.
	double evaluateByRegression(const std::vector<double>& X_example) const// X_example i.e the basis example, row vector, a simulation example size = (1,m) 
	{
		assert(X_example.size() == coeff_.size()); 
		double product = 0.0;
		for(size_t i=0; i<X_example.size(); ++i)
		{
			product += X_example[i] * coeff_[i];
		}
		return product;
	}

	//void evaluateByRegression(const matrix& X) // X_example row vector, a simulation example size = (1,m) 
	//{
	//	regressed_Y_ = boost::numeric::ublas::prod(X, coeff_);
	//}

	//const matrix&  get_regressed_Y() const {return regressed_Y_;}
};
typedef boost::shared_ptr<Regression> Regression_PTR;


//! Y regress on X = {X_1, X_2, ..., X_m}
//  X^t (Y - X*coeff) = 0 => X^t Y = X^t X * coeff => coeff = (X^t X)^{-1} X^t Y
class LSRegression : public Regression
{
public:
	LSRegression(size_t nbVariable_Y, size_t nbVarialbe_X)
		:Regression(nbVariable_Y,nbVarialbe_X){}
	virtual ~LSRegression(){};

	virtual void doRegression(const matrix& Y, const matrix& X)							  
	{
		assert(Y.size2() == dim_Y_ && X.size2() == dim_X_); 

		//! SVD solve the linear equation to find coeff
	}
};
typedef boost::shared_ptr<LSRegression> LSRegression_PTR;