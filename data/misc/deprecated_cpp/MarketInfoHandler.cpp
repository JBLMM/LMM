#include <LMM/helper/MarketInfoHandler.h>

#include <LMM/numeric/Interpolation.h>


MarketInfoHandler::MarketInfoHandler(const std::vector<double>& libors_Mkt,
									 const std::vector<double>& discountFactors_Mkt,
									 const std::vector<double>& discountMaturities,
									 const matrix_& swaptionVolMatrix_Mkt,
									 const matrix_& swapRates_Mkt)
: libors_Mkt_(libors_Mkt)
, discountFactors_Mkt_(discountFactors_Mkt)
, discountMaturities_(discountMaturities)
, swaptionVolMatrix_Mkt_(swaptionVolMatrix_Mkt)
, swapRates_Mkt_(swapRates_Mkt)
{

}

void MarketInfoHandler::convertBpToPercent()
{
	for (auto lib_bb : libors_Mkt_)
			lib_bb *= 0.01;

	for (auto swaptionVolRow : swaptionVolMatrix_Mkt_)
	{
		for (auto swaptionVol : swaptionVolRow)
			swaptionVol *= 0.01;
	}

	for (auto swapRateRow : swapRates_Mkt_)
	{
		for (auto swaprate : swapRateRow)
			swaprate *= 0.01;
	}
}

void MarketInfoHandler::interpolateDiscountFactors(const std::vector<double>& interpolation_dates)
{
	numeric::Interpolation interpolator;

	for (auto inter_date : interpolation_dates)
		interpolator.linearInterpolation(discountFactors_Mkt_,discountMaturities_,inter_date);
}

std::vector<double> MarketInfoHandler::createDiscountFactorArray()
{
	 //-- the zc bond vector to use
	std::vector<double> zcVector_BB = discountFactors_Mkt_;//ctntodo for instant use the same as discountFactors imported from data

	/* BAD
	//-- Choose mkt discount factors with maturities = 6M, 1Y, 18M, 2Y,...
	zcVector_BB.push_back(1.);
	zcVector_BB.push_back(discountFactors_Mkt_[9]);
	zcVector_BB.push_back(discountFactors_Mkt_[15]);
	zcVector_BB.push_back(discountFactors_Mkt_[17]);
	zcVector_BB.push_back(discountFactors_Mkt_[19]);
	zcVector_BB.push_back(discountFactors_Mkt_[21]);
	zcVector_BB.push_back(discountFactors_Mkt_[23]);
	zcVector_BB.push_back(discountFactors_Mkt_[25]);
	zcVector_BB.push_back(discountFactors_Mkt_[27]);

	//-- Add interpolated discount factors 
	for (size_t i = 28; i < discountFactors_Mkt_.size(); ++i)
		zcVector_BB.push_back(discountFactors_Mkt_[i]);
		*/
	return zcVector_BB;
}

MarketInfoHandler::matrix_ MarketInfoHandler::chooseSwaptionMatrix(size_t horizon)
{
	matrix_ selectedSwaptionMatrix;

	// Start at maturity = 1Y
	size_t cpt = 0;
	for (size_t i = 4; i < horizon+4; ++i)// ctntodo why choose 4 here
	{
		std::vector<double> swaptionMatRow;
		for (size_t j = 0; j < horizon-cpt; ++j)
		{
			double vol = swaptionVolMatrix_Mkt_[i][j];
			swaptionMatRow.push_back(vol);
		}
		selectedSwaptionMatrix.push_back(swaptionMatRow);
		cpt++;
	}
	return selectedSwaptionMatrix;
}

MarketInfoHandler::matrix_ MarketInfoHandler::chooseSwapRates(size_t horizon)
{
	matrix_ selectedSwapRateMatrix;

	// Start at maturity = 1Y
	size_t cpt = 0;
	for (size_t i = 4; i < horizon+4; ++i)// ctntodo why choose 4 here
	{
		std::vector<double> swaprateMatRow;
		for (size_t j = 0; j < horizon-cpt; ++j)
		{
			double vol = swapRates_Mkt_[i][j];
			swaprateMatRow.push_back(vol);
		}
		selectedSwapRateMatrix.push_back(swaprateMatRow);
		cpt++;
	}
	return selectedSwapRateMatrix;
}

LMM::Matrix_PairOfYears MarketInfoHandler::chooseSwapMaturitiesAndTenors(size_t horizon, const LMM::Matrix_PairOfYears& matrix_of_years)
{
	LMM::Matrix_PairOfYears res;

	//-- Select pairs of years until wanted horizon is reached
	for (size_t i = 1; i <= horizon; ++i) //-- ignore first row
	{
		std::vector<std::pair<double,double> > row;
		for (size_t j = 1; j <= horizon-i+1; ++j) //-- ignore first column
		{
			row.push_back(matrix_of_years[i][j]);
		}
		res.push_back(row);
	}
	return res;
}
