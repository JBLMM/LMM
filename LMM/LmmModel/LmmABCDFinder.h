#pragma once

#include <iostream>
#include <cassert>
#include <string.h>
#include <cmath>
#include <fstream>

#include <ql/types.hpp>
#include <ql/math/array.hpp>
#include <ql/math/optimization/costfunction.hpp>
#include <ql/math/optimization/endcriteria.hpp>
#include <ql/math/optimization/constraint.hpp>
#include <ql/math/optimization/problem.hpp>
#include <ql/math/optimization/simplex.hpp>
#include <ql/math/optimization/bfgs.hpp> 
#include <ql/math/optimization/conjugategradient.hpp>
#include <ql/math/optimization/levenbergmarquardt.hpp>

#include <ql/termstructures/volatility/abcd.hpp>



/*! \class LmmABCDFinder
 *
 *	a small calibrator to find abcd coherent with the market quotation black vol
 *
 */

QuantLib::Disposable<QuantLib::Array> comp_blackVol(const std::vector<double>& timeline, const QuantLib::Array& abcd) ;

class ABCDSimpleCostFunction : public QuantLib::CostFunction
{
public:

	//! constructor 
	ABCDSimpleCostFunction( const std::vector<double>& time_line, const std::vector<double>& vol_values);
		
	virtual QuantLib::Real value(const QuantLib::Array & x) const ; 
	
	virtual QuantLib::Disposable<QuantLib::Array> values(const QuantLib::Array& x) const ; 

	//! getters
	std::vector<double> get_mkt_timeline() const   
	{ 
		std::vector<double> timeline(mkt_timeline_.size() );
		for(size_t i=0;i<timeline.size();++i) timeline[i]=mkt_timeline_[i];		
		return timeline  ;
	}

	std::vector<double> get_mkt_vol_values() const 
	{
		std::vector<double> vols(mkt_vol_values_.size() );
		for(size_t i=0;i<vols.size();++i) vols[i]=mkt_vol_values_[i];		
		return vols ;
	}

protected :
	
	std::vector<double> mkt_timeline_;	
	QuantLib::Array mkt_vol_values_;	
};


class ABCDCumulativeCostFunction : public ABCDSimpleCostFunction
{
public:

	//! constructor 
	ABCDCumulativeCostFunction( const std::vector<double>& time_line, const std::vector<double>& vol_values) : ABCDSimpleCostFunction(time_line,vol_values){}
		
	virtual QuantLib::Disposable<QuantLib::Array> values(const QuantLib::Array& x) const ; 

private :
};


class LmmABCDFinder
{
public:
	
	static void print( const QuantLib::Array & abcd
					 , const std::vector<double>& time_line
					 , const std::vector<double>& black_vol 
		             , const std::string & filename);

	//! constructor 
	LmmABCDFinder(const std::vector<double>& time_line, const std::vector<double>& vol_values)
	: abdcCostFunc_(time_line,vol_values)
	, stopCriteria_(1000, 4 , 1e-12, 1e-12, 0.)
	, found_abcd(4,-100000)
	{
	
	}

	void solve();
	
	void print(const std::string& filename);

private:
	//ABCDSimpleCostFunction abdcCostFunc_;
	ABCDCumulativeCostFunction abdcCostFunc_;

	QuantLib::EndCriteria stopCriteria_;
	QuantLib::EndCriteria::Type endConvergenceType_;

	QuantLib::Array found_abcd; 
};