#include <cassert>

#include <LMM/helper/GenericPath.h>
#include <LMM/helper/Printer.h>
#include <LMM/LmmModel/BetaShifted_HGVolatilityFunction.h>
#include <cmath>

//----------------------------------------------------------------------------------------
//
//					ConstShifted_HGVolatilityParam
//
//----------------------------------------------------------------------------------------
BetaShifted_HGVolatilityParam::BetaShifted_HGVolatilityParam( 
	LMMTenorStructure_PTR  pLMMTenorStructure, 
	const Shifted_HGVolatilityParam::ABCDParameter& h_abcdParameter,
	const LowerTriangularMatrix& g_matrix,
	const LowerTriangularMatrix& betas,
	const std::vector<double>&   liborsInitValuesToDefineShift)
	:Shifted_HGVolatilityParam(pLMMTenorStructure, h_abcdParameter, g_matrix),
	 betas_(betas)
{
	initializeShiftMatrix(pLMMTenorStructure, betas, liborsInitValuesToDefineShift);
}

void BetaShifted_HGVolatilityParam::initializeShiftMatrix(
	LMMTenorStructure_PTR        pLMMTenorStructure, 
	const LowerTriangularMatrix& betas, 
	const std::vector<double>&   liborsInitValues)
{
	assert(betas.size1() == betas.size2() && betas.size2() == liborsInitValues.size() && pLMMTenorStructure->get_nbLIBOR());

	//unused first column, first row is automatically null since it is lower triangular matrix
	LowerTriangularMatrix m(betas.size1(), betas.size2());

	for(size_t k=0; k<m.size1();++k)
	{
		shift_matrix_(k,0) = 1.0e100;	
	}

	for(size_t liborIndex=1; liborIndex<m.size1();++liborIndex)
	{
		for(size_t timeIndex=1;timeIndex<=liborIndex;++timeIndex)
		{
			double beta = betas(liborIndex,timeIndex);
			double liborInitValue = liborsInitValues[liborIndex];
			shift_matrix_(liborIndex,timeIndex) = 1+ (1-beta)/beta*liborInitValue;
		}	
	} 
}


//----------------------------------------------------------------------------------------
//
//					BetaShifted_HGVolatilityFunction
//
//----------------------------------------------------------------------------------------

BetaShifted_HGVolatilityFunction::BetaShifted_HGVolatilityFunction(
	LMMTenorStructure_PTR pLMMTenorStructure,
	Correlation_PTR correlation,
	BetaShifted_HGVolatilityParam_PTR pHGVolatilityParam)       // horizon = N, total number of libor: L_k, k = [0,N]
	: Shifted_HGVolatilityFunction(pLMMTenorStructure, correlation, pHGVolatilityParam)
	, betas_(pHGVolatilityParam->get_betas_Ref())
{}

double BetaShifted_HGVolatilityFunction::get_beta_timeIndexVersion(LMM::Index liborIndex, LMM::Index timeIndex) const
{
	assert(liborIndex !=0 && liborIndex >= timeIndex);
	return betas_(liborIndex,timeIndex);
}

double BetaShifted_HGVolatilityFunction::get_beta_timeContinuousVersion(LMM::Index liborIndex, double t) const
{
	const std::vector<double>& tenor_dates = pLMMTenorStructure_->get_tenorDate();

	assert(liborIndex < pLMMTenorStructure_->get_horizon() );
	assert(t <= tenor_dates[liborIndex]); // vol_i function die after time T_i 

	//! YY TODO: not efficient, improve the algorithm latter.
	if(t == 0.0)
		return BetaShifted_HGVolatilityFunction::get_beta_timeIndexVersion(liborIndex,1);

	size_t timeIndex=0;
	while (t> tenor_dates[timeIndex])
	{
		++timeIndex;
	}

	double beta = BetaShifted_HGVolatilityFunction::get_beta_timeIndexVersion(liborIndex, timeIndex);  // beta time dependent
	return beta;
}


double BetaShifted_HGVolatilityFunction::get_varSigma_timeIndexVersion(LMM::Index indexLibor, LMM::Index indexTime) const// operator(i,j) --return vol--> hij*g_ij, i>=j
{
	if(indexLibor<indexTime)
		throw("Error, index relation error!");

	if(indexLibor<1 || indexLibor>pLMMTenorStructure_->get_horizon() )  // size1 = horizon
		throw("Error, invalid indexLibor!");

	if(indexTime <1)  // size1 = horizon
		throw("Error, invalid indexTime!");

	return get_beta_timeIndexVersion(indexLibor, indexTime)*pShifted_HGVolatilityParam_->h(indexLibor, indexTime)*pShifted_HGVolatilityParam_->g(indexLibor, indexTime);
}

double BetaShifted_HGVolatilityFunction::get_varSigma_timeContinuousVersion(LMM::Index liborIndex, double t) const
{
	const std::vector<double>& tenor_dates = pLMMTenorStructure_->get_tenorDate();

	assert(liborIndex < pLMMTenorStructure_->get_nbLIBOR() );
	assert(t <= tenor_dates[liborIndex] ); // vol_i function die after time T_i 

	if(t == 0.0)
		return BetaShifted_HGVolatilityFunction::get_varSigma_timeIndexVersion(liborIndex,1);

	size_t timeIndex=0;
	while (t> tenor_dates[timeIndex])
	{
		++timeIndex;
	}
	return BetaShifted_HGVolatilityFunction::get_varSigma_timeIndexVersion(liborIndex,timeIndex) ;
}

