#pragma once

#include <ql/types.hpp>
#include <ql/math/array.hpp>
#include <ql/math/optimization/costfunction.hpp>

#include <LMM/LmmModel/VolatilityFunction.h>
#include <LMM/pricer/LmmVanillaSwaptionApproxPricer_Rebonato.h>
#include <LMM/calibration/SwaptionMarketDataContainer.h>



class CalibrationVolCostFunction: public CostFunction
{
public:

	typedef std::vector<double>               vector_;
	typedef std::vector<std::vector<double> > matrix_;
	typedef TestHGVolatilityParam::VectorOfMatrixIndices VectorOfMatrixIndices;
	typedef SwaptionMarketDataContainer::SwaptionVector    SwaptionVector       ;
	
	CalibrationVolCostFunction
		(
		Shifted_HGVolatilityParam_PTR pVolatilityParam,
		SwaptionMarketDataContainer_PTR pSwaptionMarketDataContainer,
		LmmVanillaSwaptionApproxPricer_Rebonato_PTR approxiPricer_ptr,						 
		//const std::vector<double>& libor_shifts,
		vector_ swaptionweights,
		matrix_ weights_maturity,
		matrix_ weights_tenor,
		matrix_ weights_maturity_2,
		matrix_ weights_tenor_2);

	~CalibrationVolCostFunction();

	void reset_reference_calib(const QuantLib::Array & true_param);
	QuantLib::Array error_calib(const QuantLib::Array & actual_param) const;

	bool use_penalty;
	void reset_PenaltyCoeff(const double& coeff);
	
	Real value(const Array & x) const; 
	Disposable<Array> values(const Array& x) const; 

	//! number call of values is not exactly the number of minimizer iteration, but we have the idea of
	//! how much minimizer has to iterate by this counter.
	void reset_nbCalled()            ; // reset the iteration counter
	unsigned int get_nbCalled() const; // get the iteration counter

	void reset_HG_Indices(const VectorOfMatrixIndices& indices_vector);
	void reset_swpn_Vector(const SwaptionVector& swaption_vector);
	void reset_mkt_vol_Vector(const std::vector<double>& vol_vector);
	void reset_swaption_weights(const std::vector<double>& weight_vector);

	LmmVanillaSwaptionApproxPricer_Rebonato_PTR get_LmmVanillaSwaptionApproxPricer() ;

	//ctntodo to delete , not need seen new structure data are all vectors
	matrix_ map_ArrayToMatrix(const Array& x)   const ;
	Array   map_MatrixtoArray(const matrix_& mat) const ;


private:
	std::vector<double> reference_calib;
	
	//! optimizer's iteration counter, increment when value is called
	mutable unsigned int nbCalled;
	
	//! pointer to a vol param in order to hold the actual value of the volatility parameters
	mutable Shifted_HGVolatilityParam_PTR pHGVolatilityParamBuffer_;

	VectorOfMatrixIndices hg_IndicesVector_;
	SwaptionVector        swpn_Vector_     ;
	std::vector<double>   mkt_vol_Vector_  ;

	SwaptionMarketDataContainer_PTR pSwaptionMarketDataContainer_;

	mutable LmmVanillaSwaptionApproxPricer_Rebonato_PTR approxiPricer_ptr_; // Class implementing Rebonato's formula 

	vector_ swaption_weights_; // weights for regulation 
	
	double penalty_coeff_;
	
	matrix_ weights_maturity_;   // weights for regulation (1st order derivative with respect to maturity)
	matrix_ weights_tenor_;      // weights for regulation (1st order derivative with respect to tenor)
	matrix_ weights_maturity_2_; // weights for regulation (2nd order derivative with respect to maturity)
	matrix_ weights_tenor_2_;    // weights for regulation (2nd order derivative with respect to tenor)s



	//-- Compute regulation term 
	//-- coefficients ci control regulation terms
	Real regularisation(const Array& x, Real c1, Real c2, Real c3, Real c4) const;  // TODO: changer nom coef 
	Real sum_all_weights_regularisation(const matrix_& weights) const;
	//Real sum_all_derivatives_regularisation(const matrix_& weights, const matrix_& derivatives);

	//! get the calibration type information, check if it is global or by maturity calibration, then compute the sparse step and col index
	//! If it's a global calibration, return col_index as zero
	void get_CalibrationType(size_t& sparse_step, size_t& col_index, const VectorOfMatrixIndices& indices) const;

	/*! p(x) = 0 equivalent to x is in the satisfied constrained zone
	 *  In our case, that mean every vols parameter in the same diagonale, subdiagonal , are equal 
	 *  The penalty compute constraint squared values, returning into a vector,
	 *
	 *  If  present_column_index == 0, penalty compute sparsely the whole vol g_matrix, using sparse_step
	 *  if  present_column_index != 0, penalty compute only constraint of this present column sparsely related to the precedent
	*/	
	double penaltyCoeff()    const ;
	Disposable<Array>  penalty( size_t sparse_step, size_t present_column_index) const ;

	bool breakForPrintOut(unsigned int nbIter) const ;
};
