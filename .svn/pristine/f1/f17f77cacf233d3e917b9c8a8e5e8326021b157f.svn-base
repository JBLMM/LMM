#ifndef LMM_CALIBRATION_ATMSWAPTIONMARKETDATA_H
#define LMM_CALIBRATION_ATMSWAPTIONMARKETDATA_H
#pragma once

#include <stdlib.h>
#include <vector>
#include <string>

#include <LMM/helper/Name.h>
#include <LMM/helper/TenorType.h>

/*! \class ATMSwaptionMarketData
 *
 *  Read and handle the data of At-The-Money Swaption retreived from Bloomberg
 *  Loaded Data in this class are strictly Read-Only. If need to treat the data,
 *  copy them from here and do what you want.  
 *
 *  - EndDates of LIBORS             Tk           k=  1... for L(0,T_{k-1},T_{k})  // note that there are no date 0
 *  - Libors                         L(0,Ti,Tj)
 *  - ZeroCoupons                    P(0,Tk)
 *  - Maturities of ZC               Tk           k=0,1...
 *
 *  - Swaption Terms                 
 *  - Swaption Maturities
 *  - Matrix of Swap Rate (Strike)
 *  - Matrix of Volatilities
 *
 *  Check the file '$LUNA_ROOT/data/input/testATMSwaption_Mkt_info.csv' to see the format
 */
class ATMSwaptionMarketData
{
public:
	
	typedef std::vector<std::vector<double> > RealMatrix;
	
	ATMSwaptionMarketData();
	
	void clear_all();
	
	void readDataFromFile(const std::string& inputfile);

	//ctntodo 
	// ATMSwaptionMarketData can be intelligent that it can create an appropriated LMMTenorStructure
	//LMMTenorStructure_PTR Create_LMMTenorStructure() const { todo }


	//! getters
	const std::vector<double>& get_LIBOR()             const ;
	const std::vector<double>& get_ZC_BOND()           const ;
	const std::vector<double>& get_ZC_MATURITIES()     const ;
	const std::vector<double>& get_LIBOR_ENDDATES()    const ;

	const std::vector<double>& get_SWPM_TERMS()        const ;// end date - start date
	const std::vector<double>& get_SWPM_MATURITIES()   const ;	
	
	const RealMatrix&          get_SWAPRATE_MATRIX()   const ;
	const RealMatrix&          get_VOLATILITY_MATRIX() const ;

	void print(const std::string& filename) const;

private:

	// Loading data From an ATM Swaption Market Data files
	std::vector<double> LIBOR_            ;
	std::vector<double> ZC_BOND_          ;
	std::vector<double> ZC_MATURITIES_    ;
	std::vector<double> LIBOR_ENDDATES_   ;

	std::vector<double> SWPM_TERMS_      ; // list of terms of swaptions
	std::vector<double> SWPM_MATURITIES_ ; // list of maturities of swaptions
	
	RealMatrix SWAPRATE_MATRIX_     ;
	RealMatrix VOLATILITY_MATRIX_   ;	



	// functions useful for parsing data from .csv files

	// The second line of Excel file containe information about Swaption Terms
	void parseTermsLine(const std::string& firstLine);

	// Convert string to appropriated double value 
	double numerical_value(std::string s) const ;

	// check if case is a Tenor, containing text as YR, MO, ...
	bool isTenor(const std::string& s)  const;

	// check if case is just label text as pillar, P, L, Expiry, ##### , empty case ...
	bool isNotUsefulCase(const std::string& s) const; 

	// functions use for converting data after read, convert the volatilities unit from bp to percentage
	void convertBpToPercent();


	// check if the csv file is ended
	bool isEndFile(const std::string& line);

	bool end_libor_block ;
	bool end_zc_block    ;
	bool end_swap_block  ;
};

#endif /* LMM_CALIBRATION_ATMSWAPTIONMARKETDATA_H */