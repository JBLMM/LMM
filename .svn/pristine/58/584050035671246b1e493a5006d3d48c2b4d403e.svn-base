#include <LMM/calibration/ATMSwaptionMarketData.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <algorithm>

#include <LMM/helper/GenericPath.h>


ATMSwaptionMarketData::ATMSwaptionMarketData()
	: end_libor_block(false)
	, end_zc_block(false)
	, end_swap_block(false)
{

}

const std::vector<double>&               ATMSwaptionMarketData::get_LIBOR()             const { return LIBOR_             ; }
const std::vector<double>&               ATMSwaptionMarketData::get_ZC_BOND()           const { return ZC_BOND_           ; }
const std::vector<double>&               ATMSwaptionMarketData::get_ZC_MATURITIES()     const { return ZC_MATURITIES_     ; }
const std::vector<double>&               ATMSwaptionMarketData::get_LIBOR_ENDDATES()    const { return LIBOR_ENDDATES_    ; }
const std::vector<double>&               ATMSwaptionMarketData::get_SWPM_TERMS()        const { return SWPM_TERMS_        ; } 
const std::vector<double>&               ATMSwaptionMarketData::get_SWPM_MATURITIES()   const { return SWPM_MATURITIES_   ; }	
const ATMSwaptionMarketData::RealMatrix& ATMSwaptionMarketData::get_SWAPRATE_MATRIX()   const { return SWAPRATE_MATRIX_   ; }
const ATMSwaptionMarketData::RealMatrix& ATMSwaptionMarketData::get_VOLATILITY_MATRIX() const { return VOLATILITY_MATRIX_ ; }



void ATMSwaptionMarketData::readDataFromFile(const std::string& fileIn)
{
	std::ifstream instream;
	instream.open(fileIn.c_str());

	RealMatrix result;
	std::string line;

	if (instream.is_open())
	{
		//-- Ignore the first two lines of xls file
		getline (instream,line);
		getline (instream,line);

		getline (instream,line);
		this->parseTermsLine(line);

		while (instream.good())
		{
			getline (instream,line);
			std::stringstream ss(line); 

			const unsigned int nbLine = result.size(); // for debug use only

			//-- all the line is ########, then that say terminated
			if ( this->isEndFile(line) ) break;

			std::string cell;
			std::vector<double> line_vector;

			//-- Loop on each cell of the current line
			while(getline(ss,cell,';'))
			{
				double convertedString = this->numerical_value(cell); // to handle float format in excel

				line_vector.push_back(convertedString);
			}

			// pop back zeros at the en of reading line
			while (!line_vector.empty() && line_vector.back() == 0.0 ) line_vector.pop_back();

			if( !line_vector.empty() && !result.empty() ) 
			{
				// if the swap block is finished sooner then other block, restore the vector size as the line before
				// in order to get the result a regular grid
				while (line_vector.size() < result.back().size() ) line_vector.push_back(0.);
			}

			if(!line_vector.empty()) // push tu result only the line which is not a zero line
				result.push_back(line_vector);
		}
	}
	else
	{
		std::cout << "LMM ATMSwaptionMarketData::readDataFromFile --> Cannot open requested file" << std::endl << fileIn<< std::endl ;
		exit(0);
	}

	for (size_t i = 0; i < result.size(); ++i)
	{
		if (result[i][0] != 0.)	//-- Data of LIBOR will be  traited only its maturity is non zero
		{
			LIBOR_ENDDATES_.push_back(result[i][0]);  // 1th column
			LIBOR_.push_back(result[i][1]);           // second column
		}

		if (result[i][3] != 0.)	//-- Data of ZC will be  traited only its maturity is non zero
		{
			ZC_MATURITIES_.push_back(result[i][3]);   // 4th column
			ZC_BOND_.push_back(result[i][4]);         // 5th column		
		}
	}

	//-- Data of Swaption matrix 
	for (size_t i = 0; i < result.size(); i+=2)
	{
		const double & _swaption_maturity = result[i][6]; // 7th column

		//-- Data of Swaption matrix will be  traited only its maturity is non zero
		if( _swaption_maturity !=0 )
		{

			SWPM_MATURITIES_.push_back( _swaption_maturity );             

			// fill the volatility matrix
			std::vector<double> _vol_line_buffer;
			for (size_t j = 8; j < result[i].size(); ++j)
			{
				_vol_line_buffer.push_back(result[i][j]);
			}
			VOLATILITY_MATRIX_.push_back(_vol_line_buffer);

			// fill the swaprate matrix
			std::vector<double> _swap_rate_buffer;
			for (size_t j = 8; j < result[i+1].size(); ++j)
			{
				_swap_rate_buffer.push_back(result[i+1][j]);
			}
			SWAPRATE_MATRIX_.push_back(_swap_rate_buffer);
		}
	}

	this->convertBpToPercent();

	instream.close();
}

void ATMSwaptionMarketData::clear_all()
{

	LIBOR_.clear();
	ZC_BOND_.clear();
	ZC_MATURITIES_.clear();
	LIBOR_ENDDATES_.clear();

	SWPM_TERMS_.clear();
	SWPM_MATURITIES_.clear();

	for (auto swpMatRow : VOLATILITY_MATRIX_){ swpMatRow.clear(); }
	VOLATILITY_MATRIX_.clear();

	for (auto swpRateRow : SWAPRATE_MATRIX_){	swpRateRow.clear();	}
	SWAPRATE_MATRIX_.clear();

	end_libor_block= false;
	end_zc_block   = false;
	end_swap_block = false;
}


void ATMSwaptionMarketData::parseTermsLine(const std::string& firstLine)
{
	std::stringstream ss(firstLine) ; std::string s ;

	//-- Loop on each cell of the current line
	while(getline(ss,s,';'))
	{
		double term = numerical_value(s); // to handle float format in excel

		if(term != 0.)	SWPM_TERMS_.push_back(term);
	}
}





double ATMSwaptionMarketData::numerical_value(std::string s) const
{
	if ( isNotUsefulCase(s) ) return 0;

	if ( isTenor(s) )
	{
		// calculate for tenors in Year unit "YR", "Y"
		size_t foundYR = s.find("YR");
		if(foundYR != std::string::npos ) 
		{
			s.erase(foundYR, 2);          // if found "YR", remove two characters
			return std::atof(s.c_str());  // the string left is tenor in year unit
		}
		size_t foundY = s.find("Y");
		if(foundY != std::string::npos ) 
		{
			s.erase(foundY, 1);           // if found "Y", remove 1 character
			return std::atof(s.c_str());  // the string left is tenor in year unit
		}

		// calculate for tenors in Month unit "MO", "M"
		size_t foundMO = s.find("MO");
		if(foundMO != std::string::npos ) 
		{
			s.erase(foundMO, 2);             // if found "MO", remove two characters
			return std::atof(s.c_str())/12.; // the string left is tenor in month unit
		}
		size_t foundM = s.find("M");
		if(foundM != std::string::npos ) 
		{
			s.erase(foundM, 1);              // if found "M", remove 1 characters
			return std::atof(s.c_str())/12.; // the string left is tenor in month unit
		}


		// calculate for tenors in Week unit
		size_t foundW = s.find("W");
		if(foundW != std::string::npos ) 
		{
			s.erase(foundW, 1);              // if found "W", remove 1 characters
			return std::atof(s.c_str())/72.; // the string left is tenor in week unit
		}

		// calculate for tenors in Day unit
		size_t foundD = s.find("D");
		if(foundD != std::string::npos ) 
		{
			s.erase(foundD, 1);               // if found "D", remove 1 characters
			return std::atof(s.c_str())/360.; // the string left is tenor in day unit
		}
	}

	std::replace(s.begin(), s.end(), ',', '.');
	return std::atof(s.c_str());
}

bool ATMSwaptionMarketData::isTenor(const std::string& s) const
{
	// if text contain 'Y' or 'M' , that is a tenor 
	return (s.find('Y') != std::string::npos || s.find('M') != std::string::npos || s.find('W') != std::string::npos || s.find('D') != std::string::npos ) ;
}

bool ATMSwaptionMarketData::isNotUsefulCase(const std::string& s) const
{
	return (s.empty()||s[0] ==' '|| s[0] == 'p' || s[0] == 'L' || s[0] == 'P' || s[0] == 'E' || s[0] == '#');
}


void ATMSwaptionMarketData::convertBpToPercent()
{
	// convert the vol unit from bp to percentage
	for (size_t i = 0 ; i<VOLATILITY_MATRIX_.size();++i)
	{

		for(size_t j = 0; j< VOLATILITY_MATRIX_[i].size();++j)
		{
			VOLATILITY_MATRIX_[i][j]*=0.01;
		}
	}
}

bool ATMSwaptionMarketData::isEndFile(const std::string& textline)
{
	std::stringstream _line(textline); 
	std::string _cell;

	//-- Loop on each _cell of the current line
	unsigned int _cellcounter = 0;
	while(getline(_line,_cell,';') && _cellcounter < 7 ) // count the first 6 cells
	{
		// first _cell is #########, that end the libor column
		if(!end_libor_block && _cellcounter==0 && !_cell.empty() && _cell.at(0)=='#' ) end_libor_block = true;

		// 4th _cell is #########, that end the zero coupon column
		if(!end_zc_block    && _cellcounter==3 && !_cell.empty() && _cell.at(0)=='#' ) end_zc_block    = true;

		// 7th _cell is #########, that end the zero coupon column
		if(!end_swap_block  && _cellcounter==6 && !_cell.empty() && _cell.at(0)=='#' ) end_swap_block  = true;

		++_cellcounter;
	}

	return (end_libor_block && end_zc_block && end_swap_block);
}















void ATMSwaptionMarketData::print(const std::string& filename) const
{
	std::string path_FileOut = LMMPATH::get_output_path() + filename;

	std::ofstream outputFile  ;  outputFile.open(path_FileOut.c_str() );

	outputFile << "- LIBORS ---" << std::endl;
	for (auto lib_bb : LIBOR_) outputFile <<" "<< lib_bb << ";";
	outputFile << std::endl << std::endl;

	outputFile << "- ZC BONDS ---" << std::endl;
	for (auto zc_bb : ZC_BOND_) outputFile <<" "<<zc_bb << ";";
	outputFile << std::endl << std::endl;

	outputFile << "- ZC MATURITIES ---" << std::endl;
	for (auto zc_m : ZC_MATURITIES_) outputFile <<" "<<zc_m << ";";
	outputFile << std::endl << std::endl<< std::endl;

	outputFile << "- LIBOR MATURITIES ---" << std::endl;
	for (auto libor_m : LIBOR_ENDDATES_) outputFile <<" "<<libor_m << ";";
	outputFile << std::endl << std::endl<< std::endl;


	outputFile << "- SWPN TERMS ---" << std::endl;
	for (auto swpn_t : SWPM_TERMS_) outputFile <<" "<<swpn_t << ";";
	outputFile << std::endl << std::endl;

	outputFile << "- SWPN MATURITIES ---" << std::endl;
	for (auto swpn_m : SWPM_MATURITIES_) outputFile <<" "<<swpn_m << ";";
	outputFile << std::endl << std::endl;

	outputFile <<  "=======    ATM SWAPTION VOL MATRIX   ========" << std::endl;
	for (auto swpMatRow : VOLATILITY_MATRIX_)
	{
		for (auto swpVol : swpMatRow) outputFile <<" "<< swpVol << ";";
		outputFile << std::endl;
	}
	outputFile << std::endl << std::endl;

	outputFile << "=======    SWAP RATES   =========" << std::endl;
	for (auto swpRateRow : SWAPRATE_MATRIX_)
	{
		for (auto swpR : swpRateRow) outputFile <<" "<< swpR << ";";
		outputFile << std::endl;
	}
	outputFile << std::endl;

	outputFile.close();	
}