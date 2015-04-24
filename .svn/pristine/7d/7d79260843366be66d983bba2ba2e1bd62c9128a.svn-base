#pragma once

#include <stdlib.h>
#include <vector>
#include <string>

#include <boost/shared_ptr.hpp>

#include <LMM/helper/Name.h>
#include <LMM/helper/LMMTenorStructure.h>
#include <LMM/instrument/VanillaSwaption.h>

#include <LMM/LmmModel/UpperTriangleVanillaSwaptionQuotes.h>

/*! \class LmmSwaptionMarketData
 *  This class parse swaption market data from a .csv file. The file has to respect the specified format
 *
 *  Data file are subsituted into blocs, each bloc end by "#####" (NOTE Exactly 5 repeats)
 *
 *  The block storing libor information has to start by "LIBOR_DF"
 *
 *  The blokc storing swaption information has to start by "SwaptionVol , ATM, bump"
 *  The information about swaption's moneyness is store after the tag ATM, a real indicate the bump vale
 *  If bump == 0, this is a ATM swaaption
 *  If not, this is a bumped swaption
 */



class LmmSwaptionMarketData
{
public: 
	
	LmmSwaptionMarketData(const Tenor& fixedtenor, const Tenor& floattenor, const size_t maxNbYear);

	void parseFromMarketData(const std::string& filename);

	LiborQuotes_PTR get_LiborQuotes() const { return pLiborQuotes_ ; }
	UpperTriangleVanillaSwaptionQuotes_PTR get_SwaptionQuotes_ATM()const { return swaptionMarketData_ATM_.first;     }
	UpperTriangleVanillaSwaptionQuotes_PTR get_SwaptionQuotes_ATMpp()const { return swaptionMarketData_ATMpp_.first; }
	UpperTriangleVanillaSwaptionQuotes_PTR get_SwaptionQuotes_ATMmm()const { return swaptionMarketData_ATMmm_.first; }
	UpperTriangleVanillaSwaptionQuotes_PTR get_SwaptionQuotes_skew()const { return pSwaptionVol_skew_;   }

	const size_t get_nbYear() const { return nbYear_ ; } 
	const std::string& get_MarketDataFileName() const { return mkt_file_name ; } 
	std::string get_MarketDataBaseFileName() const ;

	const std::vector<double>& get_QuotedMaturities() const { return swpmATM_expirities_;}
	const std::vector<double>& get_ATMVolFirstColumn() const { return swpmATM_FirstColumn_;}

	bool check_data_consistency() const ;

	void print(const std::string& filename) const ; 

private:
	// Quote & moneyness
	typedef std::pair<UpperTriangleVanillaSwaptionQuotes_PTR, double> SwaptionMarketData;

	//! remove white space from a word string
	void remove_white_space(std::string& word) const;

	//! check if a string line is a mark of block's end, usually "######"
	bool isLineEndOfBlock(const std::string& text_line) const;

	//! Convert market date string to real number (year fraction), "1Yr" "2YR" "3Mo" --> to double
	double convertMarketDateToDouble(const std::string & market_date_str) const ;

	//! parse a timeline and store dates values into double vector
	void parseTimeLine(const std::string& maturity_text_line, std::vector<double>& df_dates)  const ;// parse timeline for libors or swaptions's tenor

	//! get the first information block of the data files where stored general information
	void getGeneralInfo(std::ifstream & inStream) ;

	//! check if a line is start of libor quotes block
	bool isLineStartOfLIBORBlock(const std::string& text_line) const;
	
	void parseDF_Values(const std::string& df_text_line, std::vector<double>& df_value)  const ;
	void parseLIBORS_Values(const std::string& df_text_line, std::vector<double>& libor_quote_values)  const ;
	void getDF_MarketData(std::ifstream & inStream, std::vector<double>& df_dates, std::vector<double>& df_values)  const ;
	void interpolateAndBuildLiborQuotes(const std::vector<double>& df_dates, const std::vector<double>& df_values);

	bool isLineStartOfSwaptionBlock(const std::string& text_line) const;
	double getStrikeBump(const std::string & swpm_start_line) const ;
	void parse_SWPM_QuoteLine(const std::string & swpm_quote_line   , std::vector<double>& swpm_quote_row, double & maturity) const ;
	void parse_SWPM_StrikeLine(const std::string & swpm_strike_line , std::vector<double>& swpm_strike_row) const ;
	
	bool isConsistentFormat( const std::vector<double>& swpm_expirities
		                  , const std::vector<double>& swpm_tenor
						  , const std::vector<std::vector<double> >& swpm_data)const ;

	void getSwaptionMarketData( std::ifstream & inStream
		                      , double & strike_bump
		                      , std::vector<double>& swpm_expirities
		                      , std::vector<double>& swpm_tenor
							  , std::vector<std::vector<double> >& swpm_quote_buffer
							  , std::vector<std::vector<double> >& swpm_strike_buffer) const;

	//! get the first column of VCUB matrix, this data will use for finding abcd (h function)
	void getSwaptionQuoteFirstColumn(std::vector<double>& swpm_expirities , std::vector<std::vector<double> >& swpm_quote_buffer);

	void fill_missing_strike( UpperTriangularDoubleMatrix& strike_rate_matrix
		                    , const double & strike_bump
							, const std::vector<size_t>& missed_rows  
							, const std::vector<size_t>& missed_cols  ) const;

	void buildSwationQuotes( const double & strike_bump
		                   , const std::vector<double>& swpm_expirities
		                   , const std::vector<double>& swpm_tenor
						   , const std::vector<std::vector<double> >& swpm_quote_buffer
						   , const std::vector<std::vector<double> >& swpm_strike_buffer);

	//! abs[ (quoted_strike - analytic_swaprate - strike_bump)] / (quoted_strike)     ] 
	UpperTriangularDoubleMatrix get_strike_error(const SwaptionMarketData& market_data) const;

	void buildSwationQuotesVol_skew();

	const size_t nbYear_ ;
	std::string  mkt_file_name;

	const Tenor fixedtenor_;
	const Tenor floattenor_;
	const LMMTenorStructure_PTR pLMMTenorStructure_ ;

	//! strings helper for parsing data text file
	const std::string LIBOR_LABEL_STR_;
	const std::string SWPM_LABEL_STR_;
	const std::string END_BLOCK_STR_;

	//! parsed data
	std::string general_info_;
	LiborQuotes_PTR pLiborQuotes_;
	SwaptionMarketData swaptionMarketData_ATM_;
	
	std::vector<double> swpmATM_expirities_;
	std::vector<double> swpmATM_FirstColumn_;

	SwaptionMarketData swaptionMarketData_ATMpp_; // atm ++
	SwaptionMarketData swaptionMarketData_ATMmm_; // atm --

	UpperTriangleVanillaSwaptionQuotes_PTR pSwaptionVol_skew_;
	
};

typedef boost::shared_ptr<LmmSwaptionMarketData> LmmSwaptionMarketData_PTR;