#include <LMM/calibration/RebonatoVolatilityCostFunction.h>
#include <cassert>

RebonatoVolatilityCostFunction::RebonatoVolatilityCostFunction
	(
	VolatilityParam_PTR pVolatilityParam,
	ATMSwaptionStructuredData_PTR _pATMSwaptionStructuredData_,
										   const LmmApproxVanillaSwaptionPricer& approximation,										   
										   const std::vector<double>& libor_shifts,
										   matrix_ weights,
										   matrix_ weights_maturity,
										   matrix_ weights_tenor,
										   matrix_ weights_maturity_2,
										   matrix_ weights_tenor_2)
: pVolatilityParamBuffer_(pVolatilityParam)
, pATMSwaptionStructuredData_(_pATMSwaptionStructuredData_)
, approximation_(approximation)
, weights_(weights)
, weights_maturity_(weights_maturity)
, weights_tenor_(weights_tenor)
, weights_maturity_2_(weights_maturity_2)
, weights_tenor_2_(weights_tenor_2)
{

	assert( pATMSwaptionStructuredData_->check_data_consistance() ) ;

	//setting for the libor shift
	size_t nbLibor = pATMSwaptionStructuredData_->get_nbLIBOR() ;
	assert( libor_shifts.size() == nbLibor ) ;
	for(size_t kLibor=0 ; kLibor<nbLibor ; ++kLibor )
	{
		pATMSwaptionStructuredData_->libor_shift(kLibor) = libor_shifts[kLibor];
	}
	
	const std::vector<double> & libor_shift = pATMSwaptionStructuredData_->get_ZC_BOND() ;

	size_t nbSwaption = pATMSwaptionStructuredData_->get_nbSWAPTION() ;	
		
	const std::vector<VanillaSwaption> & swaption_vector = pATMSwaptionStructuredData_->get_SWAPTION_VECTOR() ;

	for(size_t iSwaption=0; iSwaption < nbSwaption ; ++iSwaption )
	{
		const LMM::VanillaSwap & swap_i = swaption_vector[iSwaption].getUnderlyingSwap()  ;
		const std::vector<size_t> & swpm_floattingIndices = swap_i.get_fixedLegPaymentIndexSchedule();

		const std::vector<double> & omega_0 = pATMSwaptionStructuredData_->get_OMEGA_0_MATRIX()[iSwaption];

		//ctntodo, do not use this function but directly use the omega precomputed in 
		// ATMSwaptionStructuredData for accumulated swaprate and strike
		approximation_.accumulateShiftedSwapRateAndStrike( pATMSwaptionStructuredData_->swaprate(iSwaption)
			                                             , pATMSwaptionStructuredData_->strike(iSwaption)
														 , omega_0
														 , libor_shift
														 , swpm_floattingIndices);	
	}
}

RebonatoVolatilityCostFunction::~RebonatoVolatilityCostFunction(){}

Real RebonatoVolatilityCostFunction::value(const Array& vol_param) const 
{
	Array tmpRes = values(vol_param);
	//Array weightArray = map_MatrixtoArray(weights_);//ctntodo uncomment in order to reuse this weight

	Real res = 0;
	for (size_t i = 0; i < tmpRes.size(); ++i)
	{
		//tmpRes[i] *= weightArray[i];
		res += tmpRes[i]*tmpRes[i];
	}

	res = sqrt(res);

	/* ctntodo to activate this part
	Real sum_of_weights = 0.;
	for ( auto vec : weights_)
	{
		for (auto weight : vec)
			sum_of_weights += weight;
	}

	res /= sum_of_weights;
	res += regularisation(x,0.1,0.05,0.05,0.08);
	*/
	return res;
}

Disposable<Array> RebonatoVolatilityCostFunction::values(const Array& param_array) const
{
	pVolatilityParamBuffer_->reset_FromArray(param_array);
	approximation_.reset_VolatilityParam(*pVolatilityParamBuffer_);

	size_t nbSwaption = pATMSwaptionStructuredData_->get_nbSWAPTION();

	const std::vector<double> & libor            = pATMSwaptionStructuredData_->get_LIBOR_INIT()      ;
	const std::vector<double> & libor_shift      = pATMSwaptionStructuredData_->get_LIBOR_SHIFT()     ;
	
	Array computed_vol( nbSwaption );

	for(size_t iSwaption=0;iSwaption<nbSwaption;++iSwaption)
	{
		const VanillaSwaption     & swaption = pATMSwaptionStructuredData_->get_SWAPTION_VECTOR()[iSwaption] ; 
		const std::vector<double> & omega    = pATMSwaptionStructuredData_->get_OMEGA_0_MATRIX()[iSwaption]  ;	
		const double & shifted_swaprate      = pATMSwaptionStructuredData_->get_SWAPRATE_VECTOR()[iSwaption] ;

		//ctntodo to compare with BlackVol method
		computed_vol[iSwaption] = approximation_.computeRebonatoVolatility(swaption ,libor,libor_shift,omega,shifted_swaprate);
	}
	
	return computed_vol;
}

Real RebonatoVolatilityCostFunction::regularisation(const Array& x, Real c1, Real c2, Real c3, Real c4) const
{
	//-- Map array x to matrix;
	matrix_ H = map_ArrayToMatrix(x);

	//-- Compute derivatives

	double sum_derivatives_maturity = 0.;
	for (size_t i = 0; i < H.size(); ++i)
	{
		for (size_t j = 0; j < H[i].size()-1; ++j)
			sum_derivatives_maturity += weights_maturity_[i][j] * abs(H[i][j+1]-H[i][j]);
	}
	sum_derivatives_maturity *= (c1/sum_all_weights_regularisation(weights_maturity_));

	double sum_derivatives_tenor = 0;
	for (size_t i = 0; i < H.size()-1; ++i)
	{
		for (size_t j = 0; j < H[i].size(); ++j)
			sum_derivatives_tenor += weights_tenor_[i][j] * abs(H[i+1][j]-H[i][j]);
	}
	sum_derivatives_tenor *= (c2/sum_all_weights_regularisation(weights_tenor_));

	double delta = 1.;
	double sum_derivatives2_maturity = 0;
	for (size_t i = 0; i < H.size(); ++i)
	{
		for (size_t j = 0; j < H[i].size()-2; ++j)
			sum_derivatives2_maturity += weights_maturity_2_[i][j] * abs(H[i][j+2]-2*H[i][j+1]+H[i][j])/delta;
	}
	sum_derivatives2_maturity *= (c3/sum_all_weights_regularisation(weights_maturity_2_));

	double sum_derivatives2_tenor = 0;
	for (size_t i = 0; i < H.size()-2; ++i)
	{
		for (size_t j = 0; j < H[i].size(); ++j)
			sum_derivatives2_tenor += weights_tenor_2_[i][j] * abs(H[i+2][j]-2*H[i+1][j]+H[i][j])/delta;
	}
	sum_derivatives2_tenor *= (c4/sum_all_weights_regularisation(weights_tenor_2_));
	
	Real regulator = sum_derivatives_maturity + sum_derivatives_tenor + sum_derivatives2_maturity + sum_derivatives2_tenor;

	return regulator;
}


Real RebonatoVolatilityCostFunction::sum_all_weights_regularisation(const matrix_& weights) const 
{
	Real sum = 0.;
	for (size_t i = 0; i < weights.size(); ++i)
	{
		for (size_t j = 0; j < weights[i].size(); ++j)
			sum += weights[i][j];
	}
	return sum;
}

//Real RebonatoVolatilityCostFunction::sum_all_derivatives_regularisation(const matrix_& weights, const matrix_& derivatives)
//{
//	Real sum = 0.;
//	for (size_t i = 0; i < weights.size(); ++i)
//	{
//		for (size_t j = 0; j < weights[0].size(); ++j)
//			sum += weights[i][j] * derivatives[i][j];
//	}
//	return sum;
//}


RebonatoVolatilityCostFunction::matrix_ RebonatoVolatilityCostFunction::map_ArrayToMatrix(const Array& x) const
{
	matrix_ H;
	size_t x_size = x.size();
	size_t nb_alloc = 1;
	size_t current_index = 0;

	while (current_index < x_size)
	{
		std::vector<double> H_row;
		for (size_t i = 0; i < nb_alloc; ++i)
		{
			H_row.push_back(x[current_index]);
			current_index++;
		}
		H.push_back(H_row);
		nb_alloc++;
	}
	
	for (size_t i = 0; i < H.size(); ++i)
	{
		size_t current_index = H[i].size();
		for (size_t j = current_index; j < H.size(); ++j)
			H[i].push_back(0.);
	}

	return H;
}

Array RebonatoVolatilityCostFunction::map_MatrixtoArray(const matrix_& mat) const
{
	std::vector<double> tmpRes;

	for (auto vect : mat)
	{
		for (auto val : vect)
		{
			if (val == 0.)
				continue;

			tmpRes.push_back(val);
		}
	}

	size_t nbVol = tmpRes.size();
	Array res(nbVol);
	for (size_t i = 0; i < nbVol; ++i)
		res[i] = tmpRes[i];

	return res;
}