#include <cassert>

#include <LMM/helper/GenericPath.h>
#include <LMM/helper/Printer.h>
#include <LMM/LmmModel/ConstShifted_HGVolatilityFunction.h>
#include <cmath>

//----------------------------------------------------------------------------------------
//
//					ConstShifted_HGVolatilityParam
//
//----------------------------------------------------------------------------------------
Shifted_HGVolatilityParam::LowerTriangularMatrix ConstShifted_HGVolatilityParam::createShiftLTM(const std::vector<double>& shiftConstVector)
{
	//unused first colum, first row is automatically null since it is lower triangular matrix
	Shifted_HGVolatilityParam::LowerTriangularMatrix m(shiftConstVector.size(), shiftConstVector.size());

	for(size_t k=0; k<m.size1();++k)
	{
		m(k,0)= 1.0e100;	
	}

	for(size_t i=1;i<m.size1();++i)
	{
		for(size_t j=1;j<=i;++j)
		{
			m(i,j) = shiftConstVector[i];
		}	
	} 
	return m;
}

ConstShifted_HGVolatilityParam::ConstShifted_HGVolatilityParam( 
	LMMTenorStructure_PTR  pLMMTenorStructure, 
	const Shifted_HGVolatilityParam::ABCDParameter& h_abcdParameter,
	const double g_constParam,
	const double shift_constParam)
	:Shifted_HGVolatilityParam(pLMMTenorStructure,h_abcdParameter,g_constParam,shift_constParam){}

ConstShifted_HGVolatilityParam::ConstShifted_HGVolatilityParam( 
	LMMTenorStructure_PTR  pLMMTenorStructure, 
	const Shifted_HGVolatilityParam::ABCDParameter& h_abcdParameter,
	const double g_constParam,
	const std::vector<double>& shift_constParam)
	:Shifted_HGVolatilityParam(pLMMTenorStructure,h_abcdParameter,g_constParam,shift_constParam){}

ConstShifted_HGVolatilityParam::ConstShifted_HGVolatilityParam(
	LMMTenorStructure_PTR        pLMMTenorStructure, 
	const Shifted_HGVolatilityParam::ABCDParameter& h_abcdParameter,
	const LowerTriangularMatrix& g_matrix,
	const std::vector<double>&   shiftConstVector)
	:Shifted_HGVolatilityParam(pLMMTenorStructure,h_abcdParameter,g_matrix,createShiftLTM(shiftConstVector)){}

//Fully getting from shift matrix  by row
QuantLib::Array ConstShifted_HGVolatilityParam::get_ArrayFrom_Shift() const
{
	size_t nbRow = shift_matrix_.size1() ;
	QuantLib::Array x( nbRow ) ; 
	
	for(size_t iRow=0;iRow<nbRow;++iRow)
	{
		x[iRow] = shift_matrix_(iRow,0);
	}

	return x; 
} 

//Fully setting from shift matrix by row
void ConstShifted_HGVolatilityParam::reset_Shift_FromArray(const QuantLib::Array& x)
{
	size_t nbRow = shift_matrix_.size1() ;
	assert (x.size() == nbRow );

	for(size_t iRow = 0 ;iRow<nbRow;++iRow)
	{
		for(size_t jCol=0;jCol<=iRow;++jCol)
		{
			shift_matrix_(iRow,jCol) = x[iRow];
		}
	}
}


void ConstShifted_HGVolatilityParam::reset_ABCD(const QuantLib::Array& x)
{
	abcd_param_.a_ = x[0];
	abcd_param_.b_ = x[1];
	abcd_param_.c_ = x[2];
	abcd_param_.d_ = x[3];

	for(size_t indexLibor=1;indexLibor<h_matrix_.size1();++indexLibor)
	{
		const double&  T_i = pLMMTenorStructure_->get_tenorDate(indexLibor);

		for(size_t indexTime=1;indexTime<=indexLibor;++indexTime)
		{
			const double& T_j   = pLMMTenorStructure_->get_tenorDate(indexTime);

			double tau = T_i - T_j;

			////// [a+b.tau]e^{-c.tau} + d // http://quantlib.sourcearchive.com/documentation/1.1-2build1/classQuantLib_1_1AbcdFunction.html
				
			QuantLib::AbcdFunction  abcdFunction_(abcd_param_.a_,abcd_param_.b_,abcd_param_.c_,abcd_param_.d_); 
			double h_value = abcdFunction_(tau); 

			h_matrix_(indexLibor,indexTime) = h_value ;
		}	
	}
}

//----------------------------------------------------------------------------------------
//
//					Shifted_HGVolatilityFunction
//
//----------------------------------------------------------------------------------------
ConstShifted_HGVolatilityFunction::ConstShifted_HGVolatilityFunction(
	LMMTenorStructure_PTR pLMMTenorStructure,
	Correlation_PTR	      correlation,
	ConstShifted_HGVolatilityParam_PTR pHGVolatilityParam)
	: Shifted_HGVolatilityFunction(pLMMTenorStructure, correlation, pHGVolatilityParam)
{}


double ConstShifted_HGVolatilityFunction::get_varSigma_timeIndexVersion(LMM::Index indexLibor, LMM::Index indexTime) const// operator(i,j) --return vol--> hij*g_ij, i>=j
{
	if(indexLibor<indexTime)
		throw("Error, index relation error!");

	if(indexLibor<1 || indexLibor>pLMMTenorStructure_->get_horizon() )  // size1 = horizon
		throw("Error, invalid indexLibor!");

	if(indexTime <1)  // size1 = horizon
		throw("Error, invalid indexTime!");

	return pShifted_HGVolatilityParam_->h(indexLibor, indexTime)*pShifted_HGVolatilityParam_->g(indexLibor, indexTime);
}

double ConstShifted_HGVolatilityFunction::get_varSigma_timeContinuousVersion(LMM::Index liborIndex, double t) const
{
	const std::vector<double>& tenor_dates = pLMMTenorStructure_->get_tenorDate();

	assert(liborIndex < pLMMTenorStructure_->get_nbLIBOR() );
	assert(t <= tenor_dates[liborIndex] ); // vol_i function die after time T_i 

	//! ChiThan: search the nearest time index after t
	if(t == 0.0)
		return ConstShifted_HGVolatilityFunction::get_varSigma_timeIndexVersion(liborIndex,1);

	size_t timeIndex=0;
	while (t> tenor_dates[timeIndex])
	{
		++timeIndex;
	}

	//return gPWConstFunc_(liborIndex,timeIndex)*hPWConstFunc_(liborIndex,timeIndex);
	return ConstShifted_HGVolatilityFunction::get_varSigma_timeIndexVersion(liborIndex,timeIndex) ;
}
