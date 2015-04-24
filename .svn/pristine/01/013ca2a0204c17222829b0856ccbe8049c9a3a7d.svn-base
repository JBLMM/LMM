#pragma once

#include <boost/numeric/ublas/triangular.hpp>  // Triangular matrix
#include <boost/numeric/ublas/io.hpp>

#include <ql/termstructures/volatility/abcd.hpp>

#include <LMM/helper/LMMTenorStructure.h>
#include <LMM/LmmModel/Shifted_HGVolatilityFunction.h>

#include <vector>

class ConstShifted_HGVolatilityParam : public Shifted_HGVolatilityParam
{
public:
	static Shifted_HGVolatilityParam::LowerTriangularMatrix createShiftLTM(const std::vector<double>& shiftConstVector);

	ConstShifted_HGVolatilityParam( 
		LMMTenorStructure_PTR  pLMMTenorStructure, 
		const Shifted_HGVolatilityParam::ABCDParameter& h_abcdParameter,
		const double g_constParam,
		const double shift_constParam);

	ConstShifted_HGVolatilityParam( 
		LMMTenorStructure_PTR  pLMMTenorStructure, 
		const Shifted_HGVolatilityParam::ABCDParameter& h_abcdParameter,
		const double g_constParam,
		const std::vector<double>& shift_constParam);

	ConstShifted_HGVolatilityParam( 
		LMMTenorStructure_PTR  pLMMTenorStructure, 
		const Shifted_HGVolatilityParam::ABCDParameter& h_abcdParameter,
		const LowerTriangularMatrix& g_matrix,
		const std::vector<double>& shiftConstVector);

	// get the first column of shift matrix
	virtual  QuantLib::Array get_ArrayFrom_Shift() const;	 

	// reset shift all constant by a row
	virtual void reset_Shift_FromArray(const QuantLib::Array& x);	

	// reset h matrix using abcd values stored in x
	virtual void reset_ABCD(const QuantLib::Array& x);
};
typedef boost::shared_ptr<ConstShifted_HGVolatilityParam> ConstShifted_HGVolatilityParam_PTR;


//! YY YODO: change the name to Constshifted_HGVolatilityFunction
class ConstShifted_HGVolatilityFunction : public Shifted_HGVolatilityFunction
{
	// horizon = N, total number of Libor = N+1: L_k, k = [0,N]
public:	

	//! Constructor
	ConstShifted_HGVolatilityFunction(
		LMMTenorStructure_PTR pLMMTenorStructure,
		Correlation_PTR	   correlation,
		ConstShifted_HGVolatilityParam_PTR pHGVolatilityParam);

	virtual ~ConstShifted_HGVolatilityFunction(){}

	//! ---- ---- varSigma = h*g
	//! varSigma: h*g of L_liborIndex for time period [indexTime-1,indexTime]
	virtual double get_varSigma_timeIndexVersion      (LMM::Index indexLibor, LMM::Index indexTime) const; // operator(ij) --return vol--> hij*g_ij, i>=j
	virtual double get_varSigma_timeContinuousVersion (LMM::Index liborIndex, double t) const; 

	//! analytical pricing purpose
	virtual bool   isConstShift() const {return true;}
};
typedef boost::shared_ptr<ConstShifted_HGVolatilityFunction> ConstShifted_HGVolatilityFunction_PTR;
