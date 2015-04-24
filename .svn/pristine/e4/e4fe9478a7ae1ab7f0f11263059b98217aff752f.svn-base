#pragma once 
#include <LMM/Test/Tests.h>

#include <iostream>

#include <LMM/helper/Name.h>
#include <LMM/helper/GenericPath.h>
#include <LMM/helper/Printer.h>
#include <LMM/helper/Noise.h>
#include <LMM/helper/CalibrationType.h>
#include <LMM/helper/LMMTenorStructure.h>

#include <LMM/LmmModel/ConstShifted_HGVolatilityFunction.h>
#include <LMM/LmmModel/McTerminalLmm.h>

#include <LMM/pricer/LmmVanillaSwaptionApproxPricer_Rebonato.h>

#include <LMM/LmmModel/LmmPenalty.h>
#include <LMM/LmmModel/GMatrixMapping.h>
#include <LMM/LmmModel/UpperTriangleVanillaSwaptionQuotes.h>

#include <LMM/LmmModel/LmmBaseCostFunction.h>
/*! \class Test_CalibrationConfig
*
*	class use for configurating a calibration test
*
*
*/
class Test_CalibrationConfig
{
public:

	static void printExacCalibResult(const std::string& filename, const QuantLib::Array& init_shift, const QuantLib::Array& perturb_shift, const QuantLib::Array& calib_shift) ;
	static void printExacCalibResult(const std::string& filename, const LowerTriangularDoubleMatrix& init_g, const LowerTriangularDoubleMatrix& perturb_g, const LowerTriangularDoubleMatrix& calib_g) ;
	static void printDiagComparison(const std::string& filename, const LowerTriangularDoubleMatrix& init_g, const LowerTriangularDoubleMatrix& perturb_g, const LowerTriangularDoubleMatrix& calib_g)  ;

	Test_CalibrationConfig()
		: tenorfixedleg(Tenor::_1YR)
		, tenorfloatleg(Tenor::_6M)
		, nbYear( 10 )
		, calib_type(CalibrationType::GLOBAL)
		, console_stop(false)// set to true if one wish to pause calibration process during iterations
	{
		a=0.22;b=0.4;c=0.6;d=0.15;
		//nb_regularization_loop=2; //set to 1 if don't wish the penalty

		constant_rate = 0.02;
		skew_strike_bump = 0.0005;
		const_shift = 0.01;

		perturbe_noise = 0.5; // relative perturbation noise 
		g_diag_noise   = 0.2; // set to zero if one don't wish the noises on g matrix's diagonales
		penlt_time_homo = 0. ;//
		penlt_libor = 0. ;//
		x_epsilon = 1e-10;
		f_epsilon = 1e-12;
		maxIter = 5000;
		maxIterPenalty = 10;

		unique_seed = 0;

		want_print_full_vol_init   =true;
		want_print_full_vol_perturb=true;
		want_print_full_vol_calib  =true;
		want_print_full_vol_calib_gnuplot=true;
		want_print_exact_result    =true;
		want_print_full_vol_diag_compare=true;

		re_init();		
	}	

	// lmm config
	Tenor tenorfixedleg; 
	Tenor tenorfloatleg;
	size_t nbYear;
	double a,b,c,d;
	size_t fixedfloatRatio;
	double constant_rate;
	double skew_strike_bump;
	double const_shift;

	// calibration test noises config
	unsigned int unique_seed;
	double perturbe_noise, g_diag_noise ; // relative noises
	double penlt_time_homo;
	double penlt_libor;
	double x_epsilon, f_epsilon;
	size_t maxIter, maxIterPenalty;

	std::vector<size_t> unactive_swpm_rows; 
	std::vector<size_t> unactive_swpm_cols; 
	std::vector<std::pair<size_t,size_t> > truncated_cells;

	bool console_stop;

	// post calibration
	bool want_print_full_vol_init;
	bool want_print_full_vol_perturb;
	bool want_print_full_vol_calib;
	bool want_print_full_vol_calib_gnuplot;
	bool want_print_exact_result;
	bool want_print_full_vol_diag_compare;

	const std::string & get_basefilename() const { return basefilename ;}

	void re_init()
	{
		max_quote_error=10000000;
		max_param_error=10000000;
		
		fixedfloatRatio = tenorfixedleg.ratioTo(tenorfloatleg);	
		re_init_LMM_Components();
		re_init_filenames();
	}
	void re_init_randomly()
	{
		unique_seed = NoiseHelper::get_Seed(); 
		re_init();
	}
		
	QuantLib::Array get_Perturbed_Array(const QuantLib::Array& init_array )const;

	void update_gDelegateChange() const ;

	void write_to_stream(std::ostream& os) const ;
	void print(const std::string& filename)const ;

	//! LMM Components , once initialize, never change
	LmmPenalty_PTR pLmmPenalty;
	LMMTenorStructure_PTR pLMMTenorStructure ;
	LiborQuotes_ConstPTR pLiborQuotes;
	UpperTriangleVanillaSwaptionQuotes_ConstPTR pUpperTriangleVanillaSwaptionQuotes;
	UpperTriangleVanillaSwaptionQuotes_ConstPTR pUpperTriangleSkewQuotes;

	//! volatility param and other related component plays as buffer
	mutable Correlation_PTR pCorrelation;
	mutable ConstShifted_HGVolatilityParam_PTR pNoShifted_HGVolatilityParam;
	mutable ConstShifted_HGVolatilityParam_PTR pShifted_HGVolatilityParam;
	mutable LmmVanillaSwaptionApproxPricer_Rebonato_PTR pLmmVanillaSwaptionApproxPricer_Rebonato;
	mutable GMatrixMapping_PTR pGMatrixMapping;		
	mutable LmmBaseCostFunction_PTR pLmmBaseCostFunction;

	// post calibration test
	mutable double max_quote_error;
	mutable double max_param_error;

	mutable CalibrationType calib_type ; // test can modify this

private:
	void re_init_LMM_Components();
	void re_init_filenames() const ;

	LMMTenorStructure_PTR create_LMMTenorStructure() const;
	Correlation_PTR create_Correlation() const;
	ConstShifted_HGVolatilityParam_PTR create_NoShifted_HGVolatilityParam() const;
	ConstShifted_HGVolatilityParam_PTR create_Shifted_HGVolatilityParam() const;
	LmmVanillaSwaptionApproxPricer_Rebonato_PTR create_LmmVanillaSwaptionApproxPricer_Rebonato() const;
	UpperTriangleVanillaSwaptionQuotes_ConstPTR create_UpperTriangleVanillaSwaptionQuotes() const ;
	UpperTriangleVanillaSwaptionQuotes_ConstPTR create_UpperTriangleTriangleSkewQuotes() const ;
	GMatrixMapping_PTR create_GMatrixMapping() ; // create and initialize vol param, re_initialize all market data created
	LmmBaseCostFunction_PTR create_LmmBaseCostFunction() const ;

	void initialize_gDelegate_with_noise(GMatrixMapping_PTR pGMatrixMapping ) ;

	// calibration test config

	mutable std::string basefilename;//,basefilename_global_calibration,basefilename_local_calibration;
};

inline std::ostream& operator<<(std::ostream& os, const Test_CalibrationConfig& calib_config){calib_config.write_to_stream(os); return os;}