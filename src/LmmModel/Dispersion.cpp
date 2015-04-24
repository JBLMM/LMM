#include <cassert>
#include <LMM/LmmModel/Dispersion.h>


Dispersion::Dispersion(VolatilityFunction_PTR volatilityFunction)
: nbFactor_(volatilityFunction->get_Correlation_PTR()->get_reducedRank() )
, pLMMTenorStructure_(volatilityFunction->get_LMMTenorStructure() )
, pVolatilityFunction_(volatilityFunction)
{

	assert(volatilityFunction->get_Correlation_PTR()->get_fullRank() == pLMMTenorStructure_->get_horizon()+1);
	
	if(!volatilityFunction->get_Correlation_PTR()->get_flagApproxConstruced()) 
		volatilityFunction->get_Correlation_PTR()->calculate();
}

void Dispersion::update_VolatilityParam(VolatilityParam_PTR vol_param_ptr)
{
	this->pVolatilityFunction_->update_VolatilityParam( vol_param_ptr );
}

//! YY: TODO make it more efficient, covIntegral(.) recalculate lots of thing. can use pre-calculation here.
//           but the problem doesn't seems so grave, because of the pre-calculation in MC pricer.
double Dispersion::covIntegral(size_t indexTime_i,
							   size_t indexTime_j,
							   size_t indexLibor_i,
							   size_t indexLibor_j) const
{
	//return volatilityFunction->get_Correlation_ptr()->correlApprox(indexLibor_i,indexLibor_j) * pVolatilityFunction_->covIntegral(indexTime_i, indexTime_j, indexLibor_i, indexLibor_j);
	return pVolatilityFunction_->covIntegral(indexTime_i, indexTime_j, indexLibor_i, indexLibor_j);
}

double Dispersion::varIntegral(size_t indexTime_i,
							   size_t indexTime_j,
							   size_t indexLibor) const
{
	return pVolatilityFunction_->varIntegral(indexTime_i, indexTime_j, indexLibor);
}
