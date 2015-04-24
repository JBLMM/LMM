#ifndef LMM_CALIBRATION_SWAPTION_MARKET_DATA_MANAGER_H
#define LMM_CALIBRATION_SWAPTION_MARKET_DATA_MANAGER_H
#pragma once

#include <stdlib.h>
#include <vector>
#include <string>

#include <boost/shared_ptr.hpp>

#include <LMM/helper/Name.h>
#include <LMM/helper/LMMTenorStructure.h>
#include <LMM/instrument/VanillaSwaption.h>
#include <LMM/calibration/TestHGVolatilityParam.h>
#include <LMM/pricer/LmmVanillaSwaptionApproxPricer_Rebonato.h>
#include <LMM/calibration/ATMSwaptionMarketData.h>
#include <LMM/calibration/SwaptionMarketDataContainer.h>

/*! \class SwaptionMarketDataManager
 *
 *	- read data from ATMSwaptionMarketData and write into SwaptionMarketDataContainer
 *	or
 *	- create market data in a simple way by giving the LMMTenorStructure and a constant interest rate
 *		- fully generating zero coupond and libors compute numeraire
 *		- fully creating swaptions data matrix
 */



class SwaptionMarketDataManager
{
public: 
	//forward typedef
	typedef SwaptionMarketDataContainer::RealVector     RealVector;
	typedef SwaptionMarketDataContainer::RealMatrix     RealMatrix;
	typedef SwaptionMarketDataContainer::SwaptionVector SwaptionVector;
	typedef SwaptionMarketDataContainer::SwaptionMatrix SwaptionMatrix;

	typedef SwaptionMarketDataContainer::VectorOfMatrixIndices VectorOfMatrixIndices;
	typedef SwaptionMarketDataContainer::MatrixOfMatrixIndices MatrixOfMatrixIndices;
	
	SwaptionMarketDataManager(const LMMTenorStructure_PTR& pLMMTenorStructure);

	/*! generate MarketData, 
	 *		- use a constant interest rate, compute all zero coupon and init libor, numeraire 
	 *		- if noise are different than zero, a additive absolute noise will be added to these zero coupon
	 *
	 *		- create full swaption matrix (every possible maturities & expirities)
	 *		- Swaprate are computed from init libors
	 *		- use a approximative swaption pricer to create a full ATM swaption matrix, Black Vol  
	 *
	 *  If strike_bump argument equal to zero, that generate the ATM Swaptions data
	 *  If strike_bump argument different of zero, that generate the Swaption with strikes different than swaprate 
	 */
	SwaptionMarketDataContainer_PTR generateMarketData( const Tenor& fixedtenor, 
														const Tenor& floattenor, 
														LmmVanillaSwaptionApproxPricer_Rebonato* ptr,
														const double constant_rate,
														const double strike_bump) const ;

	/*! Method generating skey vol data 
	 *  - creating a market data ATM++  by an atm_strike_diff factor
	 *  - creating a market data ATM--  by an atm_strike_diff factor
	 *  - skey data = finite difference of ATM++ and ATM--  using atm_strike_diff factor
	 */
	SwaptionMarketDataContainer_PTR generateSkewData( const Tenor& fixedtenor, 
													  const Tenor& floattenor, 
													  LmmVanillaSwaptionApproxPricer_Rebonato* ptr,
													  const double constant_rate,
													  const double strike_bump) const ;


	/*! - Parse the data from MarketData, take only data coherent with LMMTenorStructure dates, if these dates
	 *    does not exists int Libor or ZC maturities, data will be interpolated 
	 *  - Create appropriated VanillaSwaption giving the fixed Tenor and floatting Tenor for each case in Swaptions matrix 
	 */
	SwaptionMarketDataContainer_PTR parseFromMarketData(const ATMSwaptionMarketData& marketdata, 
														const Tenor& fixedtenor, 
														const Tenor& floattenor) const ;
		
	const LMMTenorStructure_PTR get_LMMTenorStructure()  const ;

private:
		
	const LMMTenorStructure_PTR pLMMTenorStructure_ ;

		//! parse and interpolate libor and zero coupond bonds from market data
	//! in coherent with the floating legs dates
	void parse_LIBORs_ZCs(const ATMSwaptionMarketData& marketdata, const Tenor& floattenor, SwaptionMarketDataContainer_PTR pSwaptionMarketDataContainer) const;

	//! generate full LIBOR, Zerocouponds, maturities, numeraires ... data
	//! every floattenor step, we create a 'simulated' interest rate data
	//! if noise == 0, generate in a determinist way
	//! if noise != 0, generate data with a small noise
	void generateLIBOR_ZC_Data(const double constant_rate, SwaptionMarketDataContainer_PTR pSwaptionMarketDataContainer) const ; 

	//! generate full swaption matrix data : vol, swaprate, strike using approximation VanillaSwaptionPricer
	//! every floattenor step, we create a 'simulated swaption'
	void generateSwaptionData(const Tenor& fixedtenor, 
		                      const Tenor& floattenor, 
							  LmmVanillaSwaptionApproxPricer_Rebonato* ptr,
							  const double strike_bump,
							  SwaptionMarketDataContainer_PTR pSwaptionMarketDataContainer) const ;

};

#endif /* LMM_CALIBRATION_SWAPTION_MARKET_DATA_MANAGER_H */