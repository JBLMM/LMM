#pragma once

#include <vector>

#include <boost/shared_ptr.hpp>

#include <LMM/helper/Name.h>
#include <LMM/helper/Tensor.h>
#include <LMM/RNGenerator/RNGenerator.h>
#include <LMM/helper/LMMTenorStructure.h>
#include <LMM/LmmModel/Dispersion.h>


/*  -----------------------------------------------------------

    lmm Model: 

	dL_i(t) = phi_i(t,L_i(t)) dW_t^{T_{i+1}} 
	phi_i(t,L_i(t))  = varphi(t, L_i(t)) * vol(t) * B_i(t)
	(i.  shifted lmm: \varphi(t, L_i(t)) = L_i(t) + a_i
	(ii. Piterbarg lmm: beta_i(t)*L_i(t) + (1-beta_i(t))*L_i(0)

   -----------------------------------------------------------*/


class Lmm 
{
public:

	//! constructor
	//Lmm( const Dispersion & dispersion, const std::vector<double> & shiftsLibors);
	Lmm( const Dispersion & dispersion);

	//! destructor
	virtual ~Lmm(){}

	//!  reset the Volatility Parameters, recompute all covariance, cumulated covariance tensors
	virtual void update_VolatilityParam(VolatilityParam_PTR vol_param_ptr) ;

	//!  update the correlation after changing its parameters
	void update_Correlation() ;

	//! Getter
	LMM::Index   get_horizon()   const ;
	LMM::Index   get_nbLIBOR()   const ;
	LMMTenorStructure_PTR		 get_LMMTenorStructure () const ;	

	//const std::vector<double>&   get_shifts() const ;
	//double get_shift(LMM::Index indexLibor) const;

	//Todo: here is not to get the Tensor, but only get the element, so rename get_covariance ?
	const Tensor& get_covarianceTensor() const {return covarianceTensor_;}
	const Tensor& cumulatedCovarianceTensor() const {return cumulatedCovarianceTensor_;}

	double get_covarianceTensor         (size_t indexTime, size_t indexLibor_i, size_t indexLibor_j) const ; 
	double get_cumulatedcovarianceTensor(size_t indexTime, size_t indexLibor_i, size_t indexLibor_j) const ;

	const Dispersion& get_dispersionRef() const {return dispersion_;}

	virtual void write_to_stream(std::ostream& outputstream) const ;

	// // this is the equivalent shift: a_ij: libor_i for period [T_{j-1}, T_j]
	virtual double get_shift_timeIndexVersion(LMM::Index liborIndex, LMM::Index timeIndex) const
	{
		return  dispersion_.get_VolatilityFunction_PTR()->get_shift_timeIndexVersion(liborIndex, timeIndex);
	}

	virtual double get_shift_timeContinuousVersion(LMM::Index liborIndex, double t) const
	{
		return  dispersion_.get_VolatilityFunction_PTR()->get_shift_timeContinuousVersion(liborIndex, t);
	}

protected :

	Dispersion			dispersion_;
	//std::vector<double> shifts_;                 
	Tensor				covarianceTensor_;			 // tensor_(k,i,j): L_i, L_j 's integral of vol in [T_{k-1},T_k]
	Tensor				cumulatedCovarianceTensor_;  // tensor_(k,i,j): L_i, L_j 's integral of vol in [T_0,T_k] // used for approxSwaption Pricing

private :
	//! YY: TODO: for the moment suppose simulation step = delta_T, so: only need to pre-calculate 3D covarianceTensor.
	//            When need longStepping or more fine step, should modify this structure.
	//! covarianceTensor_(k,i,j): L_i, L_j 's integral of vol in [T_{k-1},T_k]
    //							  k = [1,...,N], i,j = [k,...,N]
	void initCovarianceTensor();
	void initCumulatedCovarianceTensor();

};

typedef boost::shared_ptr<Lmm> Lmm_PTR;
std::ostream& operator<<(std::ostream& os, const Lmm& lmm);
