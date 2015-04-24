#pragma once

#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include <LMM/helper/LMMTenorStructure.h>
#include <LMM/LmmModel/Correlation.h>
#include <LMM/LmmModel/VolatilityFunction.h>

//! YY: suppose correlation is time dependent, so can separate: Correlation & Vol 

class Dispersion 
{

public:

	Dispersion(VolatilityFunction_PTR volatilityFunction);

	virtual ~Dispersion(){}

	//!  update the Volatility Parameters
	void update_VolatilityParam(VolatilityParam_PTR vol_param_ptr) ;

	//!  update the correlation after changing its parameters
	void update_Correlation() { pVolatilityFunction_->update_Correlation(); }

	//! int_{T_indexTime_i}^{T_indexTime_j} <vol_{indexLibor_i},vol_{indexLibor_j}>(u) du, 
	double covIntegral( size_t indexTime_i  ,
						size_t indexTime_j  ,
						size_t indexLibor_i ,
						size_t indexLibor_j ) const;

	double varIntegral( size_t indexTime_i ,
						size_t indexTime_j ,
						size_t indexLibor  ) const;

	// getter
	LMM::Index getNbFactors() const { return nbFactor_ ; }

	LMM::Index get_horizon()  const { return pLMMTenorStructure_->get_horizon() ; } // N
	LMM::Index get_nbLIBOR()  const { return pLMMTenorStructure_->get_nbLIBOR() ; } // N+1

	Correlation_PTR get_Correlation_PTR()        const { return pVolatilityFunction_->get_Correlation_PTR() ; }
	LMMTenorStructure_PTR get_LMMTenorStructure_PTR()  const { return pLMMTenorStructure_  ; }
	VolatilityFunction_PTR get_VolatilityFunction_PTR() const { return pVolatilityFunction_ ; }

	double get_varSigma_timeContinuousVersion(size_t liborIndex, double t) const {return pVolatilityFunction_->get_varSigma_timeContinuousVersion(liborIndex,t);} // YY: for MP

protected:	

	const size_t nbFactor_;                     
	LMMTenorStructure_PTR  pLMMTenorStructure_  ; 
	//Correlation_PTR        pCorrelation_        ;  
	VolatilityFunction_PTR pVolatilityFunction_ ;  
};

typedef  boost::shared_ptr<Dispersion>  Dispersion_PTR;


