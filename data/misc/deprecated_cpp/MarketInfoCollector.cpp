#include <LMM/helper/MarketInfoCollector.h>

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <algorithm>

#include <LMM/generic_path.h>

using namespace std;


//-- Init static attributes
std::vector<double> MarketInfoCollector::swpnTerms_            ;
std::vector<double> MarketInfoCollector::swpnMaturities_       ;

MarketInfoCollector::matrix_ MarketInfoCollector::swapRates_               ;
MarketInfoCollector::matrix_ MarketInfoCollector::swaptionVolatilityMatrix_;

std::vector<double> MarketInfoCollector::libors_          ;
std::vector<double> MarketInfoCollector::zeroCouponBonds_ ;
std::vector<double> MarketInfoCollector::zcMaturities_    ;


std::vector<double>& MarketInfoCollector::get_swpnTerms()      { return swpnTerms_; }
std::vector<double>& MarketInfoCollector::get_swpnMaturities() { return swpnMaturities_; }

MarketInfoCollector::matrix_& MarketInfoCollector::get_swapRates(){return swapRates_;}
MarketInfoCollector::matrix_& MarketInfoCollector::get_swaptionVolatilityMatrix(){return swaptionVolatilityMatrix_;}
std::vector<double>& MarketInfoCollector::get_libors(){return libors_;}
std::vector<double>& MarketInfoCollector::get_zcBonds(){return zeroCouponBonds_;}
std::vector<double>& MarketInfoCollector::get_zcMaturities(){return zcMaturities_;}




void MarketInfoCollector::readMarketInfo(const std::string& fileIn)	                    
{
	clear_all();

	std::ifstream instream;
	instream.open(fileIn.c_str());

	matrix_ result;
	string line;
	char delimiter = '#';

	if (instream.is_open())
	{
		//-- Ignore the first two lines of xls file
		getline (instream,line);
		getline (instream,line);

		parseTermsLine(line);

		while (instream.good())
		{
			getline (instream,line);
			stringstream ss(line); 

			//-- first column and 3th column are empty and the 6th column is ########, then that say terminated
			if (line.at(0) == ';' && line.at(3) == ';' && line.at(6) ==  delimiter) break;

			string s;
			vector<double> res;

			//-- Loop on each cell of the current line
			while(getline(ss,s,';'))
			{
				double convertedString = numerical_value(s); // to handle float format in excel

				res.push_back(convertedString);
			}

			// pop back zeros at the en of reading line
			while (!res.empty() && res.back() == 0.0 ) res.pop_back();

			result.push_back(res);
		}
	}
	else
	{
		cout << ".. hello hello ... Market Info Collector: --> Cannot open requested file" << endl; 	exit(0);
	}

	for (size_t i = 0; i < result.size(); ++i)
	{
		if (result[i][1] != 0.) libors_.push_back(result[i][1]);         // second column

		if (result[i][3] != 0.)	zcMaturities_.push_back(result[i][3]);   // 4th column

		if (result[i][4] != 0.)	zeroCouponBonds_.push_back(result[i][4]);// 5th column		
	}

	//-- Swaption matrix
	for (size_t i = 0; i < result.size(); i=i+2)
	{
		std::vector<double> swaptionmatrix_tmp;
		for (size_t j = 8; j < result[i].size(); ++j)
		{
			swaptionmatrix_tmp.push_back(result[i][j]);
		}

		swaptionVolatilityMatrix_.push_back(swaptionmatrix_tmp);

		swpnMaturities_.push_back(result[i][6]);             // 7th column
	}

	//-- Swap rates
	for (size_t i = 1; i < result.size(); i=i+2)
	{
		std::vector<double> swapRates_tmp;;
		for (size_t j = 8; j < result[i].size(); ++j)
		{
			swapRates_tmp.push_back(result[i][j]);
		}
		swapRates_.push_back(swapRates_tmp);
	}


	instream.close();
}


double MarketInfoCollector::numerical_value(std::string s)
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

bool MarketInfoCollector::isTenor(const std::string& s)
{
	// if text contain 'Y' or 'M' , that is a tenor 
	return (s.find('Y') != std::string::npos || s.find('M') != std::string::npos || s.find('W') != std::string::npos || s.find('D') != std::string::npos ) ;
}

bool MarketInfoCollector::isNotUsefulCase(const std::string& s)
{
	return (s.empty()||s[0] ==' '|| s[0] == 'p' || s[0] == 'L' || s[0] == 'P' || s[0] == 'E' || s[0] == '#');
}

void MarketInfoCollector::parseTermsLine(const std::string& firstLine)
{
	stringstream ss(firstLine); 

	string s;
	
	//-- Loop on each cell of the current line
	while(getline(ss,s,';'))
	{
		double term = numerical_value(s); // to handle float format in excel

		if(term != 0.)	swpnTerms_.push_back(term);
	}
}

void MarketInfoCollector::clear_all()
{

	libors_.clear();
	zeroCouponBonds_.clear();
	zcMaturities_.clear();
	swpnTerms_.clear();
	swpnMaturities_.clear();
		
	for (auto swpMatRow : swaptionVolatilityMatrix_){	swpMatRow.clear(); }
	swaptionVolatilityMatrix_.clear();

	for (auto swpRateRow : swapRates_){	swpRateRow.clear();	}
	swapRates_.clear();
}


void MarketInfoCollector::print(const std::string& filename)
{
	std::string path_FileOut = LMM::get_output_path() + filename;

	std::ofstream outputFile  ;  outputFile.open(path_FileOut.c_str() );

	outputFile << "- LIBORS ---" << std::endl;
	for (auto lib_bb : libors_) outputFile <<" "<< lib_bb << ";";
	outputFile << std::endl << std::endl;

	outputFile << "- ZC BONDS ---" << std::endl;
	for (auto zc_bb : zeroCouponBonds_) outputFile <<" "<<zc_bb << ";";
	outputFile << std::endl << std::endl;

	outputFile << "- ZC MATURITIES ---" << std::endl;
	for (auto zc_m : zcMaturities_) outputFile <<" "<<zc_m << ";";
	outputFile << std::endl << std::endl<< std::endl;



	outputFile << "- SWPN TERMS ---" << std::endl;
	for (auto swpn_t : swpnTerms_) outputFile <<" "<<swpn_t << ";";
	outputFile << std::endl << std::endl;

	outputFile << "- SWPN MATURITIES ---" << std::endl;
	for (auto swpn_m : swpnMaturities_) outputFile <<" "<<swpn_m << ";";
	outputFile << std::endl << std::endl;

	outputFile <<  "=======    ATM SWAPTION VOL MATRIX   ========" << std::endl;
	for (auto swpMatRow : swaptionVolatilityMatrix_)
	{
		for (auto swpVol : swpMatRow) outputFile <<" "<< swpVol << ";";
		outputFile << std::endl;
	}
	outputFile << std::endl << std::endl;


	outputFile << "=======    SWAP RATES   =========" << std::endl;
	for (auto swpRateRow : swapRates_)
	{
		for (auto swpR : swpRateRow) outputFile <<" "<< swpR << ";";
		outputFile << std::endl;
	}
	outputFile << std::endl;

	outputFile.close();	

}