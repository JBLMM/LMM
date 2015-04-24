#pragma once

#include <boost/numeric/ublas/triangular.hpp>  // Triangular matrix
#include <boost/numeric/ublas/io.hpp>

#include <ql/termstructures/volatility/abcd.hpp>

#include <LMM/helper/LMMTenorStructure.h>
#include <LMM/LmmModel/Shifted_HGVolatilityFunction.h>

class BetaShifted_HGVolatilityParam : public Shifted_HGVolatilityParam
{
public:
	//static Shifted_HGVolatilityParam::LowerTriangularMatrix createShiftLTM(const std::vector<double>& shiftConstVector);

	void initializeShiftMatrix(LMMTenorStructure_PTR  pLMMTenorStructure, 
		                       const LowerTriangularMatrix& betas,
							   const std::vector<double>& liborsInitValues);  

	BetaShifted_HGVolatilityParam( 
		LMMTenorStructure_PTR  pLMMTenorStructure, 
		const Shifted_HGVolatilityParam::ABCDParameter& h_abcdParameter,
		const LowerTriangularMatrix& g_matrix,
		const LowerTriangularMatrix& betas,
		const std::vector<double>&   liborsInitValuesToDefineShift); // liborsInitValues used to define the beta shift: it can be different frome the real liborInitValue during pricing.

	const LowerTriangularMatrix& get_betas_Ref(){return betas_;}

private:
	LowerTriangularMatrix betas_; 
};
typedef boost::shared_ptr<BetaShifted_HGVolatilityParam> BetaShifted_HGVolatilityParam_PTR;



//! YY change the name to BetaShifted_HGVolatilityFunction
class BetaShifted_HGVolatilityFunction : public Shifted_HGVolatilityFunction
{
	typedef boost::numeric::ublas::triangular_matrix<double, boost::numeric::ublas::lower> LowerTriangularMatrix;

public:
	 // horizon = N, total number of Libor: L_k, k = [0,N]

	//! Constructor
	BetaShifted_HGVolatilityFunction( LMMTenorStructure_PTR pLMMTenorStructure,
									  Correlation_PTR       correlation,
									  BetaShifted_HGVolatilityParam_PTR pHGVolatilityParam);


	virtual ~BetaShifted_HGVolatilityFunction(){}


	double get_beta_timeIndexVersion     (LMM::Index liborIndex, LMM::Index timeIndex) const;
	double get_beta_timeContinuousVersion(LMM::Index liborIndex, double t) const;

	virtual double get_varSigma_timeIndexVersion      (LMM::Index indexLibor, LMM::Index indexTime) const; // operator(ij) --return vol--> hij*g_ij, i>=j
	virtual double get_varSigma_timeContinuousVersion (LMM::Index liborIndex, double t) const; 

	virtual bool   isConstShift() const {return false;}

private:
	LowerTriangularMatrix betas_; 
};
typedef boost::shared_ptr<BetaShifted_HGVolatilityFunction> BetaShifted_HGVolatilityFunction_PTR;
