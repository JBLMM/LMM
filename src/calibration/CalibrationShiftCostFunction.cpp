#include <LMM/calibration/CalibrationShiftCostFunction.h>

#include <cassert>
#include <ctime>

CalibrationShiftCostFunction::CalibrationShiftCostFunction
		(
		Shifted_HGVolatilityParam_PTR pVolatilityParam,
		SwaptionMarketDataContainer_PTR pSkewMarketDataContainer,
		LmmVanillaSwaptionApproxPricer_Rebonato_PTR approxiPricer_ptr					 
		)
: nbCalled(0)
, pHGVolatilityParamBuffer_(pVolatilityParam)
, pSkewMarketDataContainer_(pSkewMarketDataContainer)
, approxiPricer_ptr_(approxiPricer_ptr)
{

}

CalibrationShiftCostFunction::~CalibrationShiftCostFunction()
{

}

bool CalibrationShiftCostFunction::breakForPrintOut(unsigned int nbIter) const 
{
	return (nbIter%5 ==0);
}

void CalibrationShiftCostFunction::reset_CalibReference(const QuantLib::Array& calib_reference)
{
	calib_reference_.resize(calib_reference.size());
	for(size_t i=0;i<calib_reference.size();++i)
	{
		calib_reference_[i]= calib_reference[i];
	}
}

unsigned int CalibrationShiftCostFunction::get_nbCalled() const
{
	return nbCalled;
}


Real CalibrationShiftCostFunction::value(const Array & shift_array) const
{
	Array squared_diffs = values(shift_array);

	Real res = 0;
	for (size_t i = 0; i < shift_array.size(); ++i)
	{
		res += shift_array[i];
	}

	res = sqrt(res);

	return res;
}

Disposable<Array> CalibrationShiftCostFunction::values(const Array& shift_array) const
{
	pHGVolatilityParamBuffer_->reset_Shift_FromArray(shift_array);

	approxiPricer_ptr_->update_VolatilityParam(pHGVolatilityParamBuffer_);

	const std::vector<VanillaSwaption> & mkt_swaption = pSkewMarketDataContainer_->get_SWAPTION_VECTOR();
	const std::vector<double> & mkt_skews = pSkewMarketDataContainer_->get_VOLATILITY_VECTOR();
	const std::vector<double> & mkt_libor_init = pSkewMarketDataContainer_->get_LIBOR_INIT();
	const double & strike_bump = pSkewMarketDataContainer_->get_StrikeBump();
	size_t nbValues = mkt_skews.size();
	
	Array squared_diff_vol(nbValues,0. );

	for(size_t iSwaption=0;iSwaption<nbValues;++iSwaption)
	{
		VanillaSwaption copy_of_mkt_swaption(mkt_swaption[iSwaption]);
		const double & mkt_skew = mkt_skews[iSwaption];
		const double approx_skew = approxiPricer_ptr_->skew(copy_of_mkt_swaption,mkt_libor_init, strike_bump);
		squared_diff_vol[iSwaption]= (mkt_skew - approx_skew)*(mkt_skew - approx_skew);
	}



	if(LMM::DEUBGLMM)
	{
		if( breakForPrintOut(nbCalled) ) 
		{
			std::cout<<LMM::NOTIF_MSG<<" iter."<<nbCalled<< std::endl;
			if(calib_reference_.empty())
			{
				std::cout <<"      x = "<<shift_array<<std::endl;				 
			}
			else
			{
				QuantLib::Array error = computeCalibError(shift_array);
				std::cout <<"  error = "<<error<<std::endl;				 
			}
			std::cout <<" f(x,p) = "<<squared_diff_vol<<std::endl;			
		}
	}

	++nbCalled;
	return squared_diff_vol;
}

QuantLib::Array CalibrationShiftCostFunction::computeCalibError(const Array& x)const
{
	assert(!calib_reference_.empty() && calib_reference_.size() == x.size() );

	QuantLib::Array error( x.size() );
	for(size_t i=0;i<x.size();++i)
	{
		error[i]= x[i]-calib_reference_[i];
	}

	return error;
}