#pragma once

#include <iostream>
#include <cassert>
#include <string>
#include <cassert>

#include <boost/numeric/ublas/matrix_proxy.hpp>

#include <ql/termstructures/volatility/abcd.hpp>

#include <LMM/helper/TypeConverter.h>
#include <LMM/RNGenerator/RNGenerator.h>
#include <LMM/LmmModel/McLmm.h>
#include <LMM/pricer/Regression.h>
#include <LMM/instrument/CallableOption.h>

/*
 MCLmm problem:
 1.  choice of seed : when I need to fix it
 2. change simulation reuslt: liborMatrix_ from matrix to lowerTriangle matrix 

 AMCLmm problem:
 1. copy-coller liborMatrix form MCLmm to AMCLmm

 MCPricer problem:
 1. change the MCLmmVanillaSwapPricer: to make it possible to return the swapRate for a single scenario ! 

*/

typedef boost::numeric::ublas::matrix<double> matrix;

class McLmmCallableOptionPricer
{
protected:
	McLmm_PTR mcLmm_; // used for simulation!

    //! 1st fwd simulation:
	size_t nb_1stFwdSimulation_;
	std::vector<matrix> liborMatrix_1stFwdSimulation_;
	std::vector<std::vector<double>> numeraire_1stFwdSimulation_;  // not efficient at all.

	//! regression
	std::vector<Regression> regressions_; // ith element for 

	//! 2nd fwd stoppable simulation:
	size_t nb_2ndFwdSimulation_;

	//! result
	std::vector<double> result_;

	//! buffer
	std::vector<LMM::Index> buffer_callableDatesIndex_;  // used in fwdPricing

public:

	McLmmCallableOptionPricer(McLmm_PTR mcLmm,
		   size_t nb_1stFwdSimulation,
		   size_t nb_2ndFwdSimulation);

	virtual ~McLmmCallableOptionPricer(){};

	//! 1. pre-calculation for this instrument
	virtual void preparePricerForInstrument(CallableOption_PTR callableOption_ptr); 

	//! 2. 1st fwd full simulation for regression
	void fwdSimulation();                  

	//! 3. bwd regression
	virtual void bwdRegression() = 0;      

	//! 4. 2nd fwd stoppable simulation: pricing
	double fwdPricing();                   
	virtual double continuousValue(size_t itr_callableTimeIndex, const matrix& liborMatrix, const std::vector<double>& numeraire) = 0;
	virtual double intrinsicValue (size_t itr_callableTimeIndex, const matrix& liborMatrix, const std::vector<double>& numeraire) = 0;

	void price(CallableOption_PTR callableOption_ptr)
	{
		preparePricerForInstrument(callableOption_ptr);
		fwdSimulation();
		bwdRegression();
		fwdPricing();
	}
};

typedef boost::shared_ptr<McLmmCallableOptionPricer> McLmmCallableOptionPricer_PTR;
