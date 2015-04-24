#include <LMM/LmmModel/LmmSwaptionMarketData.h>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <algorithm>

#include <LMM/helper/GenericPath.h>
#include <LMM/helper/BuildVariable.h>
#include <LMM/helper/Printer.h>

#include <LMM/numeric/Interpolation.h>
#include <LMM/pricer/LmmVanillaSwapPricer.h>

LmmSwaptionMarketData::LmmSwaptionMarketData(const Tenor& fixedtenor, const Tenor& floattenor, const size_t maxNbYear)
	: nbYear_(maxNbYear)
	, fixedtenor_(fixedtenor)
	, floattenor_(floattenor)
	, pLMMTenorStructure_( new LMMTenorStructure(floattenor,maxNbYear) )
	, LIBOR_LABEL_STR_("LIBOR_DF")
	, SWPM_LABEL_STR_("SwaptionVol")
	, END_BLOCK_STR_("#")
{

}

bool LmmSwaptionMarketData::check_data_consistency() const 
{
	if(!get_LiborQuotes()->check_data_consistency() ) return false;

	throw(" LmmSwaptionMarketData::check_data_consistency() is not yet implemented DO NOT CALL");
	return false;
}

std::string LmmSwaptionMarketData::get_MarketDataBaseFileName() const 
{
	std::stringstream copy_input_file_name(mkt_file_name) ;
	std::string base_file_name ;
	getline(copy_input_file_name,base_file_name,'.');
	return base_file_name ;
}

void LmmSwaptionMarketData::print(const std::string& filename) const 
{
	//PrintElement_PTR end_print_ptr   = PrintElement_PTR(new ScalarPrintElement<std::string>("######", ",\n\n"));
	std::string path_OutPut = LMMPATH::get_output_path() + filename;

	{
		std::ofstream data_stream ;
		data_stream.open(path_OutPut.c_str());
		data_stream<<general_info_<<"#####"
			<<std::endl<<std::endl<<"LIBOR Quote,"<<std::endl;
		data_stream.close();
	}

	pLiborQuotes_->print(filename, false);

	// Print ATM Quotes and strike error
	{
		std::ofstream data_stream ;
		data_stream.open(path_OutPut.c_str(), std::ios::app);
		data_stream<<std::endl<<"####################################################"<<std::endl<<std::endl<<"ATM Swaption Quotes,"<<std::endl;
		data_stream.close();
	}

	swaptionMarketData_ATM_.first->print(filename, false);
	{
		std::ofstream data_stream ;
		data_stream.open(path_OutPut.c_str(), std::ios::app);
		data_stream<<std::endl<<"strike bump,"<<swaptionMarketData_ATM_.second<<","<<std::endl;
		data_stream.close();

		std::vector<PrintElement_PTR> elements_print;
		UpperTriangularDoubleMatrix strike_error_atm = get_strike_error(swaptionMarketData_ATM_);
		PrintElement_PTR strike_error_print = PrintElement_PTR(new MatrixPrintElement<UpperTriangularDoubleMatrix>("ATM Strike Error (%)",  strike_error_atm));
		elements_print.push_back(strike_error_print);			
		Printer printer(path_OutPut, elements_print);
		printer.print(false);
	}		

	// Print ATM++ Quotes and strike error
	{
		std::ofstream data_stream ;
		data_stream.open(path_OutPut.c_str(), std::ios::app);
		data_stream<<std::endl<<"####################################################"<<std::endl<<std::endl<<"ATM++ Swaption Quotes,"<<std::endl;
		data_stream.close();
	}
	swaptionMarketData_ATMpp_.first->print(filename, false);
	{
		std::ofstream data_stream ;
		data_stream.open(path_OutPut.c_str(), std::ios::app);
		data_stream<<std::endl<<"strike bump,"<<swaptionMarketData_ATMpp_.second<<","<<std::endl;
		data_stream.close();

		std::vector<PrintElement_PTR> elements_print;
		UpperTriangularDoubleMatrix strike_error_atmpp = get_strike_error(swaptionMarketData_ATMpp_);
		PrintElement_PTR strike_error_print = PrintElement_PTR(new MatrixPrintElement<UpperTriangularDoubleMatrix>("ATM++ Strike Error (%)",  strike_error_atmpp));
		elements_print.push_back(strike_error_print);			
		Printer printer(path_OutPut, elements_print);
		printer.print(false);
	}

	// Print ATM-- Quotes and strike error
	{
		std::ofstream data_stream ;
		data_stream.open(path_OutPut.c_str(), std::ios::app);
		data_stream<<std::endl<<"####################################################"<<std::endl<<std::endl<<"ATM-- Swaption Quotes,"<<std::endl;
		data_stream.close();
	}
	swaptionMarketData_ATMmm_.first->print(filename, false);
	{
		std::ofstream data_stream ;
		data_stream.open(path_OutPut.c_str(), std::ios::app);
		data_stream<<std::endl<<"strike bump,"<<swaptionMarketData_ATMmm_.second<<","<<std::endl;
		data_stream.close();

		std::vector<PrintElement_PTR> elements_print;
		UpperTriangularDoubleMatrix strike_error_atmmm = get_strike_error(swaptionMarketData_ATMmm_);
		PrintElement_PTR strike_error_print = PrintElement_PTR(new MatrixPrintElement<UpperTriangularDoubleMatrix>("ATM-- Strike Error (%)",  strike_error_atmmm));
		elements_print.push_back(strike_error_print);			
		Printer printer(path_OutPut, elements_print);
		printer.print(false);
	}	


	// Print vol skey
	{
		std::ofstream data_stream ;
		data_stream.open(path_OutPut.c_str(), std::ios::app);
		data_stream<<std::endl<<"####################################################"<<std::endl<<std::endl<<"Skew Swaption Quotes,"<<std::endl;
		data_stream.close();
	}
	pSwaptionVol_skew_->print(filename, false);

}

void LmmSwaptionMarketData::remove_white_space(std::string& word) const
{
	word.erase( remove( word.begin(), word.end(), ' ' ), word.end() );
}

bool LmmSwaptionMarketData::isLineEndOfBlock(const std::string& text_line) const
{
	std::stringstream line_stream(text_line) ;
	std::string text_cell ;
	bool isEnd = false ;
	getline(line_stream,text_cell,',');

	if( text_cell.find(END_BLOCK_STR_) !=std::string::npos ) isEnd = true;

	//std::cout<<"		LmmSwaptionMarketData::isLineEndOfBlock  "<<std::endl
	//	     <<"			line="<<text_line<<std::endl
	//	     <<"			cell="<<text_cell<<std::endl;

	return isEnd;
}

double LmmSwaptionMarketData::convertMarketDateToDouble(const std::string & market_date_str) const 
{
	// market_date_str usually of the form "3 MO", "6 MO", "1 YR", "1Yr", "3Mo"
	double numerical_date=-100000000;
	double year_fraction=-100000000;

	std::stringstream date_word(market_date_str) ;
	std::string number_text ;

	if(  market_date_str.find("D") !=std::string::npos  )
	{
		year_fraction= 1./365;// Day Unit, convention 365 days/ year
		getline(date_word,number_text,'D') ;
	}
	else if(  market_date_str.find("W") !=std::string::npos  )
	{
		year_fraction= 1./52;// Week Unit, convention 52 week/ year
		getline(date_word,number_text,'W') ;
	}
	else if( market_date_str.find("M") !=std::string::npos ) // MO, Mo
	{
		year_fraction= 1./12;// Month unit, 12 months/ year
		getline(date_word,number_text,'M') ;
	}	
	else // YR
	{
		assert( market_date_str.find("Y") !=std::string::npos );
		getline(date_word,number_text,'Y') ;
		year_fraction=1.;
	}
	remove_white_space(number_text) ;

	numerical_date = std::stod(number_text);	

	numerical_date*=year_fraction;

	return numerical_date;
}

void LmmSwaptionMarketData::parseTimeLine(const std::string& timeline_text, std::vector<double>& dates)  const 
{
	std::stringstream line_stream(timeline_text) ;

	std::string cell_buffer ;

	getline(line_stream,cell_buffer,',');

	remove_white_space(cell_buffer);

	if(cell_buffer.compare("Expiry") ==0)// case tenor dates line of swaption vol
	{
		getline(line_stream,cell_buffer,',');//move the 2 first case
		getline(line_stream,cell_buffer,',');
	}
	else// case discount factor matirities dates line
	{
		dates.push_back(0); // first discount factor date is zero
	}

	while(!cell_buffer.empty() )
	{
		double date = convertMarketDateToDouble(cell_buffer);

		dates.push_back(date);

		getline(line_stream,cell_buffer,',');
	}
}

double LmmSwaptionMarketData::getStrikeBump(const std::string & swpm_start_line) const 
{
	// "SwaptionVol, ATM, 0"
	double strike_bump=-1000000000;

	std::stringstream start_swaption_line(swpm_start_line) ;
	std::string bump_text ;

	getline(start_swaption_line,bump_text,',') ;
	getline(start_swaption_line,bump_text,',') ;
	getline(start_swaption_line,bump_text,',') ;

	remove_white_space(bump_text);

	strike_bump = std::stod(bump_text);
	// convert bp unit to 10e-4
	strike_bump/=10000.;

	return strike_bump;
}

void LmmSwaptionMarketData::parseFromMarketData(const std::string& filename)
{
	mkt_file_name.clear();
	mkt_file_name = filename ;

	std::ifstream instream;
	std::string input_file_fullpath = LMMPATH::get_runtime_datapath() + filename;
	instream.open(input_file_fullpath.c_str());

	getGeneralInfo(instream);

	std::vector<double> df_dates_buffer;
	std::vector<double> df_values_buffer;
	getDF_MarketData(instream, df_dates_buffer, df_values_buffer);
	interpolateAndBuildLiborQuotes(df_dates_buffer, df_values_buffer);

	// If a curve of forward rate quote is specified, use this curve instead of interpolation from Discount Factor curve
	std::string text_line;
	getline(instream,text_line);
	if(!isLineEndOfBlock(text_line))
	{
		std::vector<double> libor_quote_values;
		parseLIBORS_Values(text_line,libor_quote_values);
		//pLiborQuotes_->reset_Libor(libor_quote_values); just for testing if the data taken from the bloomberg forward curve analysis is better, but it is not. 
	}


	for(size_t itSWPMTab=0;itSWPMTab<3;++itSWPMTab)
	{
		double strike_bump = -100000000;
		std::vector<double> atm_swpm_experities_buffer;
		std::vector<double> atm_swpm_tenor_buffer;
		std::vector< std::vector<double> > atm_swpm_quote_buffer;
		std::vector< std::vector<double> > atm_swpm_strike_buffer;
		getSwaptionMarketData(instream,strike_bump, atm_swpm_experities_buffer, atm_swpm_tenor_buffer, atm_swpm_quote_buffer, atm_swpm_strike_buffer);

		if(itSWPMTab==0)// first block is ATM Swaption VCUB
		{
			getSwaptionQuoteFirstColumn(atm_swpm_experities_buffer, atm_swpm_quote_buffer);
		}

		buildSwationQuotes( strike_bump, atm_swpm_experities_buffer, atm_swpm_tenor_buffer, atm_swpm_quote_buffer, atm_swpm_strike_buffer );
	}

	swaptionMarketData_ATM_.first->set_Data_FileName(filename);

	buildSwationQuotesVol_skew();

	swaptionMarketData_ATMpp_.first->set_Data_FileName(filename);
	swaptionMarketData_ATMmm_.first->set_Data_FileName(filename);

	instream.close();
}

void LmmSwaptionMarketData::getGeneralInfo(std::ifstream & inStream) 
{
	general_info_.clear();

	std::string text_line;
	std::stringstream general_info_buffer;

	if (inStream.is_open())
	{
		//getline(inStream,text_line);
		while( !isLineEndOfBlock(text_line) )
		{
			//std::cout<<"	LmmSwaptionMarketData::getGeneralInfo"<<std::endl
			//	     <<"		text_line="<<text_line<<std::endl;
			if(!text_line.empty() )
				general_info_buffer<<text_line<<std::endl;

			getline(inStream,text_line);
		}
		general_info_ = general_info_buffer.str();
		//std::cout<<"general_info_ "<<std::endl<<general_info_<<std::endl;
	}

}

bool LmmSwaptionMarketData::isLineStartOfLIBORBlock(const std::string& text_line) const
{
	std::stringstream line_stream(text_line) ;
	std::string text_cell ;
	bool is_libor_block = false ;
	getline(line_stream,text_cell,',');

	remove_white_space(text_cell);

	if(text_cell.compare(LIBOR_LABEL_STR_) == 0 ) is_libor_block = true;

	//std::cout<<"		LmmSwaptionMarketData::isLineStartOfLIBORBlock  "<<std::endl
	//	<<"			line="<<text_line<<std::endl
	//	<<"			cell="<<text_cell<<std::endl;

	return is_libor_block;
}

void LmmSwaptionMarketData::parseDF_Values(const std::string& df_text_line, std::vector<double>& df_value)  const 
{
	std::stringstream line_stream(df_text_line) ;

	std::string cell_buffer ;

	getline(line_stream,cell_buffer,',');

	df_value.push_back(1.); // first discount factor at date zero is 1.

	while(!cell_buffer.empty() )
	{
		double df = std::stod(cell_buffer) ;

		df_value.push_back(df);

		getline(line_stream,cell_buffer,',');
	}
}

void LmmSwaptionMarketData::parseLIBORS_Values(const std::string& text_line, std::vector<double>& libor_quote_values)  const 
{
	std::stringstream line_stream(text_line) ;

	std::string cell_buffer ;

	getline(line_stream,cell_buffer,',');
	
	while(!cell_buffer.empty() )
	{
		double fwd_rate = std::stod(cell_buffer) ; fwd_rate/=100.; // Quotation are in % unit 

		libor_quote_values.push_back(fwd_rate);

		getline(line_stream,cell_buffer,',');
	}
}

void LmmSwaptionMarketData::getDF_MarketData(std::ifstream & inStream, std::vector<double>& df_dates, std::vector<double>& df_values) const
{
	df_dates.clear();
	df_values.clear();

	if (inStream.is_open())
	{
		{
			//! remove line anterior to LIBOR block
			std::string empty_text_line;			
			while( !isLineStartOfLIBORBlock(empty_text_line) )
			{
				getline(inStream,empty_text_line);
				//std::cout<<"	LmmSwaptionMarketData::getDF_MarketData"<<std::endl
				//	<<"		text_line="<<empty_text_line<<std::endl;
			}
		}

		{
			std::string maturity_text_line;			
			std::string df_text_line;
			getline(inStream,maturity_text_line);
			getline(inStream,df_text_line);

			//std::cout<<"	LmmSwaptionMarketData::getDF_MarketData"<<std::endl
			//	<<"		maturity_text_line="<<maturity_text_line<<std::endl	
			//	<<"		df_text_line="<<df_text_line<<std::endl;

			parseTimeLine(maturity_text_line,df_dates);
			parseDF_Values(df_text_line,df_values);

			assert(df_dates.size() == df_values.size() ); // after parsing, data has to be coherent
		}
	}
}
void LmmSwaptionMarketData::interpolateAndBuildLiborQuotes(const std::vector<double>& df_dates, const std::vector<double>& df_values)
{
	std::vector<double> cpy_df_dates(df_dates)   ;
	std::vector<double> cpy_df_values(df_values) ;
	assert(cpy_df_dates.size() == cpy_df_values.size() );

	numeric::Interpolation interpolator;

	size_t nbTenorDate = pLMMTenorStructure_->get_nbLIBOR() + 1;
	for(size_t iDate=0;iDate<nbTenorDate;++iDate)
	{
		const double & maturity_date = pLMMTenorStructure_->get_tenorDate(iDate);
		interpolator.linearInterpolation(cpy_df_values, cpy_df_dates, maturity_date);
	}

	assert(cpy_df_dates.size() == cpy_df_values.size() );

	const double end_lmm_date = pLMMTenorStructure_->get_EndDate();

	//! resize market data coherent to LMMTenorStructure
	while(cpy_df_dates.back() > end_lmm_date)
	{
		cpy_df_dates.pop_back();
		cpy_df_values.pop_back();
	}

	//! remove element that does not appear in LMMTenorStructure timeline as 7 MO, 8 MO ...
	for(size_t ipos=1;ipos<cpy_df_dates.size();++ipos)
	{
		const double& date_i = cpy_df_dates[ipos];
		LMM::Index date_index = pLMMTenorStructure_->get_Index(date_i);
		if(date_index==0) // mkt date not match Model date
		{
			cpy_df_dates.erase(  cpy_df_dates.begin() + ipos);
			cpy_df_values.erase( cpy_df_values.begin() + ipos);

			--ipos; // after erasing, one have to retest for the same index (position), that's why decrement ipos
		}
	}

	assert(cpy_df_dates.size() == (pLMMTenorStructure_->get_nbLIBOR()+1) );

	pLiborQuotes_.reset( new LiborQuotes(pLMMTenorStructure_, cpy_df_values)  );
}


bool LmmSwaptionMarketData::isLineStartOfSwaptionBlock(const std::string& text_line) const
{
	std::stringstream line_stream(text_line) ;
	std::string text_cell ;
	bool is_libor_block = false ;
	getline(line_stream,text_cell,',');

	remove_white_space(text_cell);

	if(text_cell.compare(SWPM_LABEL_STR_) == 0 ) is_libor_block = true;

	//std::cout<<"		LmmSwaptionMarketData::isLineStartOfSwaptionBlock  "<<std::endl
	//	<<"			line="<<text_line<<std::endl
	//	<<"			cell="<<text_cell<<std::endl;

	return is_libor_block;
}

void LmmSwaptionMarketData::parse_SWPM_QuoteLine( const std::string & swpm_quote_line
												 , std::vector<double>& swpm_quote_row
												 , double & maturity) const 
{
	swpm_quote_row.clear();

	std::stringstream line_stream(swpm_quote_line) ;

	std::string cell_buffer ;

	getline(line_stream,cell_buffer,',');

	maturity = convertMarketDateToDouble(cell_buffer);

	getline(line_stream,cell_buffer,',');// "Vol"
	getline(line_stream,cell_buffer,',');

	while(!cell_buffer.empty() )
	{
		remove_white_space(cell_buffer);

		double quote_value = std::stod(cell_buffer) / 100.;// convert % unit to no unit value

		swpm_quote_row.push_back(quote_value);

		getline(line_stream,cell_buffer,',');
	}
}
void LmmSwaptionMarketData::parse_SWPM_StrikeLine(const std::string & swpm_strike_line , std::vector<double>& swpm_strike_row) const 
{
	swpm_strike_row.clear();

	std::stringstream line_stream(swpm_strike_line) ;

	std::string cell_buffer ;

	getline(line_stream,cell_buffer,',');// ""
	getline(line_stream,cell_buffer,',');// "strike"
	getline(line_stream,cell_buffer,',');// strike value

	while(!cell_buffer.empty() )
	{
		remove_white_space(cell_buffer);

		double strike_value = std::stod(cell_buffer) / 100.;// convert % unit to no unit value

		swpm_strike_row.push_back(strike_value);

		getline(line_stream,cell_buffer,',');
	}
}

bool LmmSwaptionMarketData::isConsistentFormat( const std::vector<double>& swpm_expirities
											   , const std::vector<double>& swpm_tenor
											   , const std::vector<std::vector<double> >& swpm_data)const 
{
	bool is_consistent_format = true;

	if( swpm_expirities.size() != swpm_data.size() ) return false;
	for(size_t iMaturity=0;iMaturity<swpm_data.size();++iMaturity)
	{
		if(swpm_data[iMaturity].size() != swpm_tenor.size() ) is_consistent_format=false;
	}

	return is_consistent_format;
}

void LmmSwaptionMarketData::getSwaptionMarketData( std::ifstream & inStream
												  , double & strike_bump
												  , std::vector<double>& swpm_expirities
												  , std::vector<double>& swpm_tenor
												  , std::vector<std::vector<double> >& swpm_quote_buffer
												  , std::vector<std::vector<double> >& swpm_strike_buffer) const
{
	swpm_expirities.clear();
	swpm_tenor.clear();
	swpm_quote_buffer.clear();  //attention
	swpm_strike_buffer.clear();

	if (inStream.is_open() )
	{
		{
			//! remove line anterior to SwaptionVol block and get the bump value
			std::string empty_text_line;			
			while( !isLineStartOfSwaptionBlock(empty_text_line) )
			{
				getline(inStream,empty_text_line);			

				//std::cout<<"	LmmSwaptionMarketData::getSwaptionMarketData"<<std::endl
				//	     <<"		text_line  ="<<empty_text_line<<std::endl;
			}
			strike_bump = getStrikeBump(empty_text_line);
			//std::cout<<"strike_bump="<<strike_bump<<std::endl;
		}

		{
			//! get the tenor timeline
			std::string tenors_line;				
			getline(inStream,tenors_line);
			parseTimeLine(tenors_line,swpm_tenor);
		}

		{
			//! get the quote matrix and strike matrix by iterating throw rows
			std::string swpm_quote_line  ; std::vector<double> swpm_quote_row;
			std::string swpm_strike_line ; std::vector<double> swpm_strike_row;

			getline(inStream,swpm_quote_line);
			getline(inStream,swpm_strike_line);
			while(!isLineEndOfBlock(swpm_quote_line) )
			{
				double swpm_maturity=-10000000;

				parse_SWPM_QuoteLine(swpm_quote_line   , swpm_quote_row , swpm_maturity);
				parse_SWPM_StrikeLine(swpm_strike_line , swpm_strike_row);

				assert(swpm_tenor.size() == swpm_quote_row.size() );
				assert(swpm_tenor.size() == swpm_strike_row.size() );

				swpm_expirities.push_back(swpm_maturity);
				swpm_quote_buffer.push_back(swpm_quote_row);
				swpm_strike_buffer.push_back(swpm_strike_row);

				getline(inStream,swpm_quote_line);
				getline(inStream,swpm_strike_line);
			}
		}

		assert( isConsistentFormat(swpm_expirities, swpm_tenor, swpm_quote_buffer) );
		assert( isConsistentFormat(swpm_expirities, swpm_tenor, swpm_strike_buffer) );
	}
}

void LmmSwaptionMarketData::getSwaptionQuoteFirstColumn(std::vector<double>& swpm_expirities , std::vector<std::vector<double> >& swpm_quote_buffer)
{
	swpmATM_expirities_ = swpm_expirities;
	size_t nbRow = swpm_quote_buffer.size();
	for(size_t iRow=0;iRow<nbRow;++iRow)
	{
		swpmATM_FirstColumn_.push_back( swpm_quote_buffer[iRow][0] );
		//swpmATM_FirstColumn_.push_back( swpm_quote_buffer[iRow][5] );//ATTENTION, TO CHANGE LATTER, DO A LOOP TO CHECK THE GOOD COLUMN
	}

	assert(swpmATM_FirstColumn_.size() == swpmATM_expirities_.size() );
}

void LmmSwaptionMarketData::buildSwationQuotes( const double & strike_bump
											   , const std::vector<double>& mkt_expirities
											   , const std::vector<double>& mkt_tenor
											   , const std::vector<std::vector<double> >& mkt_quote
											   , const std::vector<std::vector<double> >& mkt_strike)
{
	size_t fix_float_ratio = fixedtenor_.ratioTo(floattenor_);
	size_t lastLiborIndex = pLMMTenorStructure_->get_horizon();

	size_t last_year = lastLiborIndex / fix_float_ratio ;
	size_t matrix_size = last_year + 1 ;

	UpperTriangularDoubleMatrix strike_rate_matrix(matrix_size,matrix_size);
	numeric::FullMatrix interpolated_black_vol_matrix(matrix_size,matrix_size); // use fully the quotation black vol matrix for interpolation,

	// stored all existing market swaption position (parsed) in teh swaption matrix
	std::vector<std::pair<size_t, size_t> > exist_mkt_swpm_position;

	for(size_t iMktExperity=0;iMktExperity<mkt_expirities.size();++iMktExperity)
	{
		const double mkt_experity_i = mkt_expirities[iMktExperity]; 
		LMM::Index swpm_startIndex  = pLMMTenorStructure_->get_Index(mkt_experity_i);

		if(swpm_startIndex>=fix_float_ratio)
		{
			for(size_t jMktTenor=0;jMktTenor<mkt_tenor.size();++jMktTenor)
			{
				const double mkt_tenor_j = mkt_tenor[jMktTenor];
				const double swpm_end_date = mkt_experity_i + mkt_tenor_j;

				// put the market strike into strike matrix (upper)
				// missed element will be calculated by analytical swap rate formula
				LMM::Index swpm_endIndex   = pLMMTenorStructure_->get_Index(swpm_end_date);
				if( (swpm_endIndex%fix_float_ratio==0) && ( (swpm_endIndex-swpm_startIndex)%fix_float_ratio==0 ) && swpm_endIndex>fix_float_ratio)
				{
					const size_t iRow = swpm_startIndex/fix_float_ratio;
					const size_t jCol = (swpm_endIndex - swpm_startIndex)/fix_float_ratio;
					exist_mkt_swpm_position.push_back( std::pair<size_t,size_t>(iRow,jCol) );

					strike_rate_matrix(iRow,jCol)=mkt_strike[iMktExperity][jMktTenor];					
				}

				// put the market quote into quote matrix (full)
				// missed element will be interpolated
				LMM::Index swpm_tenorIndex   = pLMMTenorStructure_->get_Index(mkt_tenor_j);
				if (swpm_tenorIndex%fix_float_ratio==0)
				{
					const size_t iRow = swpm_startIndex/fix_float_ratio;
					const size_t jCol = swpm_tenorIndex/fix_float_ratio;
				
					interpolated_black_vol_matrix(iRow,jCol) = mkt_quote[iMktExperity][jMktTenor];					
				}
			}	
		}
		else
		{
			if(LMM::WARNLMM ())
				std::cout<<LMM::WARN_MSG <<" SWPM Experity "<<mkt_experity_i<<" does not match in LMMTenorStructure's Swaption Matrix "<<std::endl;
		}
	}

	//getting the missed position in the swaption matrix
	std::vector<size_t > missed_row_indices;
	std::vector<size_t > missed_col_indices;

	for(size_t iExperityRow=1;iExperityRow<last_year;++iExperityRow)
	{
		std::pair<size_t,size_t> indices_pair_first_col(iExperityRow,1);
		if( std::find(exist_mkt_swpm_position.begin(), exist_mkt_swpm_position.end(),indices_pair_first_col)==exist_mkt_swpm_position.end() ) 
		{
			if( std::find(missed_row_indices.begin(), missed_row_indices.end(),iExperityRow)==missed_row_indices.end() )
				missed_row_indices.push_back(iExperityRow);
		}
	}
	std::sort( missed_row_indices.begin(), missed_row_indices.end() );

	for(size_t jTenorCol=1;jTenorCol<last_year;++jTenorCol)
	{
		std::pair<size_t,size_t> indices_pair_first_row(1,jTenorCol);

		if( std::find(exist_mkt_swpm_position.begin(), exist_mkt_swpm_position.end(),indices_pair_first_row)==exist_mkt_swpm_position.end() ) 
		{
			if( std::find(missed_col_indices.begin(), missed_col_indices.end(),jTenorCol)==missed_col_indices.end() )
				missed_col_indices.push_back(jTenorCol);
		}
	}
	std::sort( missed_col_indices.begin(), missed_col_indices.end() );
	
	fill_missing_strike(strike_rate_matrix,strike_bump,missed_row_indices,missed_col_indices);
	
	numeric::Interpolation interpolator;
	interpolator.fullMatrixInterpolate(interpolated_black_vol_matrix,missed_row_indices,missed_col_indices);

	//assigne the upper triangular part of interpolated matrix to a upper black vol matrix
	UpperTriangularDoubleMatrix black_vol_matrix(matrix_size,matrix_size);
	for(size_t iRow=1;iRow<interpolated_black_vol_matrix.size1();++iRow)
	{
		for(size_t jCol=1;jCol<interpolated_black_vol_matrix.size2()-iRow;++jCol)
		{
			black_vol_matrix(iRow,jCol)= interpolated_black_vol_matrix(iRow,jCol);		
		}
	}

	if(strike_bump ==0 ) //strike_bump ==0
	{
		swaptionMarketData_ATM_.first.reset(new UpperTriangleVanillaSwaptionQuotes( pLMMTenorStructure_
			, last_year
			, fixedtenor_
			, floattenor_
			, strike_rate_matrix
			, black_vol_matrix)
			);
		swaptionMarketData_ATM_.second=strike_bump;
	}
	else if(strike_bump >0 )
	{
		swaptionMarketData_ATMpp_.first.reset(new UpperTriangleVanillaSwaptionQuotes( pLMMTenorStructure_
			, last_year
			, fixedtenor_
			, floattenor_
			, strike_rate_matrix
			, black_vol_matrix)
			);
		swaptionMarketData_ATMpp_.second=strike_bump;
	}
	else if(strike_bump <0 )
	{
		swaptionMarketData_ATMmm_.first.reset(new UpperTriangleVanillaSwaptionQuotes( pLMMTenorStructure_
			, last_year
			, fixedtenor_
			, floattenor_
			, strike_rate_matrix
			, black_vol_matrix)
			);
		swaptionMarketData_ATMmm_.second=strike_bump;	
	}
}

void LmmSwaptionMarketData::fill_missing_strike( UpperTriangularDoubleMatrix& strike_rate_matrix
												, const double & strike_bump
												, const std::vector<size_t>& missed_rows  
												, const std::vector<size_t>& missed_cols  ) const
{
	size_t m_size = strike_rate_matrix.size1();
	assert( m_size == strike_rate_matrix.size2()) ;
	if(missed_rows.size()>0){
		assert(missed_rows.front()>0);assert(missed_rows.back()<m_size-1);
	}
	if(missed_cols.size()>0){
		assert(missed_cols.front()>0);assert(missed_cols.back()<m_size-1);
	}
	//numeric::Interpolation interpolator;
	//interpolator.upperTriangularMatrixInterpolate(strike_rate_matrix,missed_rows,missed_cols);

	size_t fix_float_ratio = fixedtenor_.ratioTo(floattenor_);
	LmmVanillaSwapPricer swap_pricer(pLMMTenorStructure_);
	const std::vector<double>& libor_init = pLiborQuotes_->get_InitLibor();

	// Fill cols on a missing row
	for(size_t iMissedRow=0;iMissedRow<missed_rows.size();++iMissedRow)
	{
		size_t missed_row_index =  missed_rows[iMissedRow] ;
		size_t max_Col_index = m_size - missed_row_index - 1;
		for(size_t jCol=1;jCol<=max_Col_index;++jCol)
		{
			size_t experity_index =  missed_row_index*fix_float_ratio;
			size_t end_index      = (missed_row_index+jCol)*fix_float_ratio;

			VanillaSwap missed_swap(-1000000, experity_index, end_index,  floattenor_, fixedtenor_, pLMMTenorStructure_);
			const double atm_strike = swap_pricer.swapRate_Analytical(missed_swap, libor_init);

			strike_rate_matrix(missed_row_index,jCol) = atm_strike + strike_bump;
		}
	}

	// Fill rows on a missing col
	for(size_t jMissedCol=0;jMissedCol<missed_cols.size();++jMissedCol)
	{
		size_t missed_col_index = missed_cols[jMissedCol];
		size_t max_Row_index = m_size - missed_col_index - 1;
		for(size_t iRow=1;iRow<=max_Row_index;++iRow)
		{
			size_t experity_index = iRow*fix_float_ratio;
			size_t end_index      = (iRow+missed_col_index)*fix_float_ratio;

			VanillaSwap missed_swap(-1000000, experity_index, end_index,  floattenor_, fixedtenor_, pLMMTenorStructure_);
			const double atm_strike = swap_pricer.swapRate_Analytical(missed_swap, libor_init);

			strike_rate_matrix(iRow,missed_col_index) = atm_strike + strike_bump;
		}
	}
}

void LmmSwaptionMarketData::buildSwationQuotesVol_skew()
{
	assert(swaptionMarketData_ATMpp_.first->size1() == swaptionMarketData_ATMmm_.first->size1() );
	assert(swaptionMarketData_ATMpp_.first->size2() == swaptionMarketData_ATMmm_.first->size2() );

	size_t matrix_size = swaptionMarketData_ATMpp_.first->size1();

	UpperTriangularDoubleMatrix strike_matrix(matrix_size,matrix_size);
	UpperTriangularDoubleMatrix skew_vol_matrix(matrix_size,matrix_size);

	const UpperTriangularVanillaSwaptionQuotes & swaptionDataATM_pp = swaptionMarketData_ATMpp_.first->get_UpperTriangularVanillaSwaptionQuotes();
	const UpperTriangularVanillaSwaptionQuotes & swaptionDataATM_mm = swaptionMarketData_ATMmm_.first->get_UpperTriangularVanillaSwaptionQuotes();

	for(size_t iMktExperity=1;iMktExperity<swaptionMarketData_ATMpp_.first->size1();++iMktExperity)
	{
		for(size_t jTenorCol=1;jTenorCol<swaptionMarketData_ATMpp_.first->size2()-iMktExperity;++jTenorCol)
		{
			const double & strike_pp = swaptionDataATM_pp(iMktExperity,jTenorCol).first.get_strike();
			const double & strike_mm = swaptionDataATM_mm(iMktExperity,jTenorCol).first.get_strike();
			const double & vol_pp    = swaptionDataATM_pp(iMktExperity,jTenorCol).second;
			const double & vol_mm    = swaptionDataATM_mm(iMktExperity,jTenorCol).second;

			const double strike_mid   = 0.5 * (strike_pp + strike_mm);
			const double strike_diff  = (strike_pp - strike_mm)/2;
			const double skew_vol = NumericalMethods::df(vol_pp,vol_mm,strike_diff);

			strike_matrix(iMktExperity,jTenorCol)= strike_mid;
			skew_vol_matrix(iMktExperity,jTenorCol)= skew_vol;
		}	
	}

	pSwaptionVol_skew_.reset(new UpperTriangleVanillaSwaptionQuotes( pLMMTenorStructure_
		, swaptionMarketData_ATMpp_.first->get_lastYear()
		, swaptionMarketData_ATMpp_.first->get_fixedTenor()
		, swaptionMarketData_ATMpp_.first->get_floatingTenor()
		, strike_matrix
		, skew_vol_matrix)
		);
}


UpperTriangularDoubleMatrix LmmSwaptionMarketData::get_strike_error(const SwaptionMarketData& market_data) const
{

	assert(pLMMTenorStructure_ == pLiborQuotes_->get_LMMTenorStructure_PTR());

	size_t lastYear = market_data.first->get_lastYear();
		
	UpperTriangularDoubleMatrix error_matrix(lastYear+1,lastYear+1);

	assert(market_data.first->size1() == lastYear+1);
	assert(market_data.first->size2() == lastYear+1);

	LmmVanillaSwapPricer swapPricer(pLMMTenorStructure_);
	const std::vector<double>& libor_init = pLiborQuotes_->get_InitLibor();

	const UpperTriangularVanillaSwaptionQuotes &  upperTriangleVanillaSwaptionQuotes = market_data.first->get_UpperTriangularVanillaSwaptionQuotes();

	for(size_t iExperity=1;iExperity<lastYear;++iExperity)
	{
		for(size_t jTenor=1;jTenor<=(lastYear-iExperity);++jTenor)
		{
			const VanillaSwap& swap_ij = upperTriangleVanillaSwaptionQuotes(iExperity,jTenor).first.getUnderlyingSwap();
			const double quoted_strike = swap_ij.get_strike();
			const double theoretical_swaprate    =  swapPricer.swapRate_Analytical(swap_ij, libor_init); // atm strike is the pv swap rate

			const double & strike_bump = market_data.second;

			const double strike_error = 100*std::abs(quoted_strike - theoretical_swaprate - strike_bump)/(quoted_strike);
			error_matrix(iExperity,jTenor) = strike_error;
		}
	}
	return error_matrix;
}