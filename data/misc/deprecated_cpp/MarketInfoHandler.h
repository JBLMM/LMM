#ifndef LMM_HELPER_MARKETINFOHANDLER_H
#define LMM_HELPER_MARKETINFOHANDLER_H
#pragma once

#include <stdlib.h>
#include <vector>

#include <LMM/Name.h>

// ctntodo rename this class to ATMSwaptionMarketData because its structure is specificly for ATM Swaption Data
// ctntodo fusion this class with the MarketInfoCollector into a unique class ATMSwaptionMarketData

//-- handle data from bloomberg (interpolations,...)
class MarketInfoHandler
{
public:
	
	typedef std::vector<std::vector<double> > matrix_;
	
	MarketInfoHandler(const std::vector<double>& libors_Mkt,
		              const std::vector<double>& discountFactors_Mkt,
					  const std::vector<double>& discountMaturities,
					  const matrix_& swaptionVolMatrix_Mkt,
					  const matrix_& swapRates_Mkt);

	
	void convertBpToPercent();
	
	// insert additional date in discountMaturities, and interpolate theses points 
	// for having the appropriated inserted values in discountFactors_Mkt
	void interpolateDiscountFactors(const std::vector<double>& interpolation_dates);//ctntodo

	std::vector<double> createDiscountFactorArray();;//ctntodo
	matrix_ chooseSwaptionMatrix(size_t horizon); 
	matrix_ chooseSwapRates(size_t horizon);
	LMM::Matrix_PairOfYears chooseSwapMaturitiesAndTenors(size_t horizon, const LMM::Matrix_PairOfYears& matrix_of_years);

private:

	std::vector<double> libors_Mkt_;
	std::vector<double> discountFactors_Mkt_;
	std::vector<double> discountMaturities_;
	matrix_ swaptionVolMatrix_Mkt_;
	matrix_ swapRates_Mkt_;

};

#endif /* LMM_HELPER_MARKETINFOHANDLER_H */
