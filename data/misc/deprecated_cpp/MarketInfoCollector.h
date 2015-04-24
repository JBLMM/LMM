#ifndef LMM_HELPER_MARKETINFOCOLLECTOR_H
#define LMM_HELPER_MARKETINFOCOLLECTOR_H
#pragma once


#include <cstring>
#include <fstream>
#include <vector>


/* \class MarketInfocollector read all info from an ATM Swaption Data into numerical data
 *
 *
 */
class MarketInfoCollector
{
public:
	typedef std::vector< std::vector<double> > matrix_;

	static void readMarketInfo(const std::string& fileIn);
		                
	//-- Getters --//
	static std::vector<double>& get_swpnTerms()      ;
	static std::vector<double>& get_swpnMaturities() ;
	static matrix_& get_swapRates()                  ; 
	static matrix_& get_swaptionVolatilityMatrix()   ;

	static std::vector<double>& get_libors()       ;
	static std::vector<double>& get_zcBonds()      ;
	static std::vector<double>& get_zcMaturities() ;


	//print all data to given file
	static void print(const std::string& filename);

private:

	static std::vector<double> libors_           ;
	static std::vector<double> zeroCouponBonds_  ;
	static std::vector<double> zcMaturities_     ;
	
	static std::vector<double> swpnTerms_        ;
	static std::vector<double> swpnMaturities_   ;

	static matrix_ swaptionVolatilityMatrix_     ; // volatilities matrix
	static matrix_ swapRates_                    ; // strike matrix at ATM Swaption
	
	


	// Convert string to appropriated double value 
	static double numerical_value(std::string s);

	// check if case is a Tenor, containing text as YR, MO, ...
	static bool isTenor(const std::string& s);

	// check if case is just label text as pillar, P, L, Expiry, ##### , empty case ...
	static bool isNotUsefulCase(const std::string& s); 

	// The second line of Excel file containe information about Swaption Terms
	static void parseTermsLine(const std::string& firstLine);

	static void clear_all();
};


#endif /* LMM_HELPER_MARKETINFOCOLLECTOR_H */