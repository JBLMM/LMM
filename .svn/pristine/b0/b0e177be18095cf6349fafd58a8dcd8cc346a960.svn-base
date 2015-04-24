#pragma once

#include <iostream>
#include <fstream>

#include <ql/math/array.hpp>

#include <boost/shared_ptr.hpp>

#include <LMM/helper/GenericPath.h>
#include <LMM/helper/BuildVariable.h>
#include <LMM/helper/LMMTenorStructure.h>
#include <LMM/LmmModel/Correlation.h>


//=== VOLATILITY PARAMS === VOLATILITY PARAMS === VOLATILITY PARAMS === VOLATILITY PARAMS =============//

/*! \class VolatilityParam
 *
 *  Abstract class for modelling Volatility Parametersin the LMM model
 *  This class play the role interface with QuantLib::Array, which is useful for calibration
 *  
 *  Note : - see dima-ql-intro-2.pdf p.53/148 to see the exemple of requirement for QuantLib::Array size
 *         - The best pratice is to use VolatilityParam as a param holder and not a Parameters container 
 */

class VolatilityParam
{
public :

	virtual ~VolatilityParam(){}

	//! write the volatility param out to a ostream
	virtual void write_to_stream(std::ostream & os) const  = 0 ;

	//!print Vol Param to file
	virtual void print(const std::string& filename)
	{
		std::string path_OutPut = LMMPATH::get_output_path() + filename;
		std::ofstream outputstream;
		outputstream.open(path_OutPut.c_str());
		this->write_to_stream(outputstream);
		outputstream.close();
	}
};

inline std::ostream& operator<<(std::ostream& os, const VolatilityParam& param){param.write_to_stream(os); return os;}
typedef boost::shared_ptr<VolatilityParam> VolatilityParam_PTR;
//=====================================================================================================//
//=====================================================================================================//

/*! \class VolatilityFunction
 *  Abstract class for modelling Volatility in the LMM model
 *	
 *  The process for Volatility works is
 *   - Step 1 : Create a new VolatilityParam 
 *   - Step 2 : VolatilityFunction update with the new VolatilityParam
 *   - Step 3 : Use the method covIntegral for returning the appropriated integral of vol
 *
 */
class VolatilityFunction
{
public:
	
	VolatilityFunction(LMMTenorStructure_PTR lmmTenorStructure,
					   Correlation_PTR       correlation) 
					   : pLMMTenorStructure_(lmmTenorStructure),
					     correlation_(correlation)
	{}

	virtual ~VolatilityFunction(){}

	LMMTenorStructure_PTR get_LMMTenorStructure() const {return pLMMTenorStructure_ ;}

//! \int_{T_i}^{T_j} varSigma_i(u)*varSigma_j(u)*rho_ij(u) du
	virtual double covIntegral( LMM::Index indexTime_i  ,
								LMM::Index indexTime_j  ,
								LMM::Index indexLibor_i ,
								LMM::Index indexLibor_j) const = 0;
	
	virtual double varIntegral( LMM::Index indexTime_i,LMM::Index indexTime_j, LMM::Index indexLibor) const 
	{
		return covIntegral(indexTime_i, indexTime_j, indexLibor, indexLibor);
	}

	//! ---- ---- varSigma = h*g
	//! varSigma: h*g of L_liborIndex for time period [indexTime-1,indexTime]
	virtual double get_varSigma_timeIndexVersion      (LMM::Index indexLibor, LMM::Index indexTime) const =0; // operator(ij) --return vol--> hij*g_ij, i>=j
	virtual double get_varSigma_timeContinuousVersion (LMM::Index liborIndex, double t) const =0; 	


	// YY TODO: very bad practice, it exist because I cannot do the dynamic casting  
	virtual bool   isConstShift() const =0;
	virtual double get_shift_timeIndexVersion      (LMM::Index liborIndex, LMM::Index timeIndex) const = 0;
	virtual double get_shift_timeContinuousVersion (LMM::Index liborIndex, double t) const =0; 

	Correlation_PTR get_Correlation_PTR() const {return correlation_;}
	//! print to text file ctntodo move this function to VolatilityParam, the parameters need to print, and not the function
	virtual void print(const std::string& filename) const = 0;

		
	/*! Reset the Volatility Parameters (use for calibration)
	 *  Each time calibrator find a new parameters, the VolatilityFunction has to update the parameters
	 */
	virtual void update_VolatilityParam(VolatilityParam_PTR vol_param_ptr)
	{
		throw("VolatilityFunction::update_VolatilityParam method is not yet implemented, user need to reimplement in order to use");
	}

	/*! Reset the Correlation calculation after reseting  correlation's parameters somewhere (use for calibration)
	 *  Each time calibrator find a new parameters, the Correlation has to update the parameters
	 */
	virtual void update_Correlation()
	{
		correlation_->calculate(); 
	}

protected:

	LMMTenorStructure_PTR pLMMTenorStructure_ ;
	Correlation_PTR       correlation_;
};

typedef boost::shared_ptr<VolatilityFunction> VolatilityFunction_PTR;


//! YY TODO: make this function more general: calculate covariance for whatever time interval.
//           so at least make it possible to use piecewise linear vol.
//! YY TODO:  to put these class in different class.


