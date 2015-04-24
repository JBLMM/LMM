#include <LMM/Test/Tests.h>
#include <LMM/LmmModel/LmmABCDFinder.h>

#include <iostream>
#include <cassert>
#include <string.h>
#include <cmath>
#include <fstream>

#include <ql/termstructures/volatility/abcdcalibration.hpp>
#include <LMM/helper/TenorType.h>
#include <LMM/helper/LMMTenorStructure.h>
#include <LMM/helper/InputFileManager.h>
#include <LMM/LmmModel/LmmSwaptionMarketData.h>
#include <LMM/helper/GenericPath.h>

#include <LMM/LmmModel/ConstShifted_HGVolatilityFunction.h>
#include <LMM/LmmModel/LmmABCDCostFunction.h>
#include <LMM/LmmModel/LmmABCDCalibrator.h>

enum SWAPTIONCHOICE
{
	WHOLEMATRIX, FIRSTROW, FIRSTCOL, MIDDIAG 
};

void test_getABCDByFirstCol(size_t nbYear ,const std::string& inputfile);
void test_FindABCDByQuantLib(size_t nbYear ,const std::string& input_file);
void test_LMM_ABCD_calibration(size_t nbYear ,const std::string& input_file, const SWAPTIONCHOICE swaptionchoice);

void test_LmmABCDFinder()
{
	size_t nbYear =16;
	std::vector<std::string> mkt_file_list = InputFileManager::get_VCUB_FileList();
	size_t nbFile = mkt_file_list.size();

	//test_getABCDByFirstCol(nbYear,mkt_file_list[0]);

	//test_FindABCDByQuantLib(nbYear,mkt_file_list[2]);

	SWAPTIONCHOICE swaptionchoice=WHOLEMATRIX;
	for(size_t iFile=0;iFile<nbFile;++iFile) {	test_LMM_ABCD_calibration(nbYear,mkt_file_list[iFile],swaptionchoice); }
	//swaptionchoice=FIRSTROW;
	//for(size_t iFile=0;iFile<nbFile;++iFile) {	test_LMM_ABCD_calibration(nbYear,mkt_file_list[iFile],swaptionchoice); }
	//swaptionchoice=MIDDIAG;
	//for(size_t iFile=0;iFile<nbFile;++iFile) {	test_LMM_ABCD_calibration(nbYear,mkt_file_list[iFile],swaptionchoice); }
	//swaptionchoice=FIRSTCOL;
	//for(size_t iFile=0;iFile<nbFile;++iFile) {	test_LMM_ABCD_calibration(nbYear,mkt_file_list[iFile],swaptionchoice); }

}

void test_getABCDByFirstCol(size_t nbYear ,const std::string& input_file)
{
	std::cout<<"void test_ParseData_30YR() called "<<std::endl;
	Tenor tenorfixedleg(Tenor::_1YR) ;
	Tenor tenorfloatleg(Tenor::_6M)  ;

	LmmSwaptionMarketData_PTR pLmmSwaptionMarketData( new LmmSwaptionMarketData(tenorfixedleg, tenorfloatleg , nbYear ) );

	pLmmSwaptionMarketData->parseFromMarketData(input_file);

	const std::vector<double>& maturities = pLmmSwaptionMarketData->get_QuotedMaturities();
	const std::vector<double>& firstColumnQuotes = pLmmSwaptionMarketData->get_ATMVolFirstColumn();
	assert(maturities.size() == firstColumnQuotes.size() );

	LmmABCDFinder calibrator(maturities,firstColumnQuotes);

	calibrator.solve();

	calibrator.print("test_getABCDByFirstCol_result.csv");
}

void test_FindABCDByQuantLib(size_t nbYear ,const std::string& input_file)
{
	Tenor tenorfixedleg(Tenor::_1YR) ;
	Tenor tenorfloatleg(Tenor::_6M)  ;

	LmmSwaptionMarketData_PTR pLmmSwaptionMarketData( new LmmSwaptionMarketData(tenorfixedleg, tenorfloatleg , nbYear ) );

	pLmmSwaptionMarketData->parseFromMarketData(input_file);

	const std::vector<double>& maturities = pLmmSwaptionMarketData->get_QuotedMaturities();
	const std::vector<double>& firstColumnQuotes = pLmmSwaptionMarketData->get_ATMVolFirstColumn();
	assert(maturities.size() == firstColumnQuotes.size() );

	QuantLib::AbcdCalibration abcd_calibrator(maturities, firstColumnQuotes);
	abcd_calibrator.compute();

	std::cout<<std::endl<<"test_FindABCDByQuantLib()" <<std::endl;
	std::cout<<" a="<<abcd_calibrator.a() <<std::endl;
	std::cout<<" b="<<abcd_calibrator.b() <<std::endl;
	std::cout<<" c="<<abcd_calibrator.c() <<std::endl;
	std::cout<<" d="<<abcd_calibrator.d() <<std::endl;

	std::string output_file_name = "test_FindABCDByQuantLib_result.csv";

	QuantLib::Array found_abcd(4);
	found_abcd[0]=abcd_calibrator.a();
	found_abcd[1]=abcd_calibrator.b();
	found_abcd[2]=abcd_calibrator.c();
	found_abcd[3]=abcd_calibrator.d();

	LmmABCDFinder::print(found_abcd, maturities, firstColumnQuotes, output_file_name);
}

void test_LMM_ABCD_calibration(size_t nbYear ,const std::string& input_file, const SWAPTIONCHOICE swaptionchoice)
{

	std::string folder_name ;
	switch( swaptionchoice ) 
	{
	case WHOLEMATRIX:
		folder_name  = "ABCD_WholeMatrix\\";
		break;
	case FIRSTROW:
		folder_name  = "ABCD_FirstRow\\";
		break;
	case FIRSTCOL:
		folder_name  = "ABCD_FirstCol\\";
		break;
	case MIDDIAG:
		folder_name  = "ABCD_MidDiag\\";
		break;
	}
	
	std::string base_name_file = LMMPATH::get_BaseFileName(input_file) + "\\";

	folder_name +=base_name_file ;
	LMMPATH::reset_Output_SubFolder(folder_name );
	

	LmmCalibrationConfig config;
	config.reset_nbYear( nbYear );
	config.correl_ReducedRank_= 30;

	config.correl_alpha_ = 0.000000001;
	
	config.correl_beta_  = 0.05;
	
	config.use_positive_constraint_=true;

	LmmSwaptionMarketData_PTR pLmmSwaptionMarketData=get_LmmSwaptionMarketData(config, input_file );

	QuantLib::Array calibrated_abcd = marketData_LMM_ABCD_calibration(config,pLmmSwaptionMarketData);

	//{
	//	std::string general_result_file_name = folder_name + "test_LMM_ABCD_calibration_AllResult.csv";
	//	std::string path_OutPut = LMMPATH::get_output_path() + general_result_file_name;
	//	std::ofstream all_result ; all_result.open(path_OutPut.c_str(), std::ios::app);

	//	all_result<<"########################"<< ",,,,"<< nbYear<<"YR,,"<<base_name<< std::endl;
	//	all_result<<lmm_abcd_calibrator.get_BaseGeneral_Result_Info()<<std::endl<<std::endl;

	//	all_result.close();	
	//}
}


