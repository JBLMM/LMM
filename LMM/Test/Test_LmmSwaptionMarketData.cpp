#include <LMM/Test/Tests.h>

#include <ctime>
#include <iostream>

#include <LMM/helper/TenorType.h>
#include <LMM/helper/LMMTenorStructure.h>

#include <LMM/LmmModel/LmmSwaptionMarketData.h>

void test_ParseData_5YR();
void test_ParseData_10YR();
void test_ParseData_30YR();
void test_All_MarketDataFile();

void test_LmmSwaptionMarketData()
{
	//test_ParseData_5YR();

	//test_ParseData_10YR();

	test_ParseData_30YR();

	//test_All_MarketDataFile();
}



void test_ParseData_5YR()
{
	
	Tenor tenorfixedleg(Tenor::_1YR) ;
	Tenor tenorfloatleg(Tenor::_6M)  ;
	size_t nbYear = 6;
	std::string input_file("swaption_mkt_info_30YR.csv");
	std::string output_file =  "out_swaption_mkt_info_5YR.csv";

	LmmSwaptionMarketData_PTR pLmmSwaptionMarketData( new LmmSwaptionMarketData(tenorfixedleg, tenorfloatleg , nbYear ) );

	pLmmSwaptionMarketData->parseFromMarketData(input_file);
	pLmmSwaptionMarketData->print(output_file);

	LiborQuotes_PTR libor_quotes_ptr = pLmmSwaptionMarketData->get_LiborQuotes();
	bool liborquote_consistency = libor_quotes_ptr->check_data_consistency();
	libor_quotes_ptr->print("test_ParseData_5YR_LiborQuotes.csv");

	UpperTriangleVanillaSwaptionQuotes_PTR pSwaptionQuotes_ATM = pLmmSwaptionMarketData->get_SwaptionQuotes_ATM() ;
	pSwaptionQuotes_ATM->print("test_ParseData_5YR_SwaptionQuotes_ATM.csv");

	UpperTriangleVanillaSwaptionQuotes_PTR pSwaptionQuotes_ATMpp = pLmmSwaptionMarketData->get_SwaptionQuotes_ATMpp() ;
	pSwaptionQuotes_ATMpp->print("test_ParseData_5YR_SwaptionQuotes_ATMpp.csv");

	UpperTriangleVanillaSwaptionQuotes_PTR pSwaptionQuotes_ATMmm = pLmmSwaptionMarketData->get_SwaptionQuotes_ATMmm() ;
	pSwaptionQuotes_ATMmm->print("test_ParseData_5YR_SwaptionQuotes_ATMmm.csv");

	const std::vector<double>& maturities = pLmmSwaptionMarketData->get_QuotedMaturities();
	const std::vector<double>& firstColumnQuotes = pLmmSwaptionMarketData->get_ATMVolFirstColumn();
	assert(maturities.size() == firstColumnQuotes.size() );
}

void test_ParseData_10YR()
{
	Tenor tenorfixedleg(Tenor::_1YR) ;
	Tenor tenorfloatleg(Tenor::_6M)  ;
	size_t nbYear = 11;
	std::string input_file("swaption_mkt_info_30YR.csv");
	std::string output_file =  "out_swaption_mkt_info_10YR.csv";

	LmmSwaptionMarketData_PTR pLmmSwaptionMarketData( new LmmSwaptionMarketData(tenorfixedleg, tenorfloatleg , nbYear ) );

	pLmmSwaptionMarketData->parseFromMarketData(input_file);
	pLmmSwaptionMarketData->print(output_file);

	LiborQuotes_PTR libor_quotes_ptr = pLmmSwaptionMarketData->get_LiborQuotes();
	bool liborquote_consistency = libor_quotes_ptr->check_data_consistency();
	libor_quotes_ptr->print("test_ParseData_10YR_LiborQuotes.csv");

	UpperTriangleVanillaSwaptionQuotes_PTR pSwaptionQuotes_ATM = pLmmSwaptionMarketData->get_SwaptionQuotes_ATM() ;
	pSwaptionQuotes_ATM->print("test_ParseData_10YR_SwaptionQuotes_ATM.csv");
	
	UpperTriangleVanillaSwaptionQuotes_PTR pSwaptionQuotes_ATMpp = pLmmSwaptionMarketData->get_SwaptionQuotes_ATMpp() ;
	pSwaptionQuotes_ATMpp->print("test_ParseData_10YR_SwaptionQuotes_ATMpp.csv");
	
	UpperTriangleVanillaSwaptionQuotes_PTR pSwaptionQuotes_ATMmm = pLmmSwaptionMarketData->get_SwaptionQuotes_ATMmm() ;
	pSwaptionQuotes_ATMmm->print("test_ParseData_10YR_SwaptionQuotes_ATMmm.csv");
	
	const std::vector<double>& maturities = pLmmSwaptionMarketData->get_QuotedMaturities();
	const std::vector<double>& firstColumnQuotes = pLmmSwaptionMarketData->get_ATMVolFirstColumn();
	assert(maturities.size() == firstColumnQuotes.size() );
}

void test_ParseData_30YR()
{
	std::cout<<"void test_ParseData_30YR() called "<<std::endl;
	Tenor tenorfixedleg(Tenor::_1YR) ;
	Tenor tenorfloatleg(Tenor::_6M)  ;
	size_t nbYear = 31;
	//std::string input_file("swaption_mkt_info_30YR.csv");
	std::string input_file("VCUB_14_augst_2014.csv");
	std::string output_file = "out_"+ input_file;

	LmmSwaptionMarketData_PTR pLmmSwaptionMarketData( new LmmSwaptionMarketData(tenorfixedleg, tenorfloatleg , nbYear ) );

	pLmmSwaptionMarketData->parseFromMarketData(input_file);
	pLmmSwaptionMarketData->print(output_file);

	LiborQuotes_PTR libor_quotes_ptr = pLmmSwaptionMarketData->get_LiborQuotes();
	bool liborquote_consistency = libor_quotes_ptr->check_data_consistency();
	libor_quotes_ptr->print("test_ParseData_30YR_LiborQuotes.csv");

	UpperTriangleVanillaSwaptionQuotes_PTR pSwaptionQuotes_ATM = pLmmSwaptionMarketData->get_SwaptionQuotes_ATM() ;
	pSwaptionQuotes_ATM->print("test_ParseData_30YR_SwaptionQuotes_ATM.csv");
	
	UpperTriangleVanillaSwaptionQuotes_PTR pSwaptionQuotes_ATMpp = pLmmSwaptionMarketData->get_SwaptionQuotes_ATMpp() ;
	pSwaptionQuotes_ATMpp->print("test_ParseData_30YR_SwaptionQuotes_ATMpp.csv");
	
	UpperTriangleVanillaSwaptionQuotes_PTR pSwaptionQuotes_ATMmm = pLmmSwaptionMarketData->get_SwaptionQuotes_ATMmm() ;
	pSwaptionQuotes_ATMmm->print("test_ParseData_30YR_SwaptionQuotes_ATMmm.csv");

	const std::vector<double>& maturities = pLmmSwaptionMarketData->get_QuotedMaturities();
	const std::vector<double>& firstColumnQuotes = pLmmSwaptionMarketData->get_ATMVolFirstColumn();
	assert(maturities.size() == firstColumnQuotes.size() );
	
}

void test_All_MarketDataFile()
{
	std::vector<std::string> file_list;
	file_list.push_back("VCUB_01_augst_2014.csv");
	file_list.push_back("VCUB_04_augst_2014.csv");
	file_list.push_back("VCUB_05_augst_2014.csv");
	file_list.push_back("VCUB_06_augst_2014.csv");
	file_list.push_back("VCUB_07_augst_2014.csv");
	file_list.push_back("VCUB_08_augst_2014.csv");
	file_list.push_back("VCUB_14_augst_2014.csv");

	Tenor tenorfixedleg(Tenor::_1YR) ;
	Tenor tenorfloatleg(Tenor::_6M)  ;
	size_t nbYear = 31;

	for(size_t iFile=0;iFile<file_list.size();++iFile)
	{
		LmmSwaptionMarketData_PTR pLmmSwaptionMarketData( new LmmSwaptionMarketData(tenorfixedleg, tenorfloatleg , nbYear ) );

		pLmmSwaptionMarketData->parseFromMarketData(file_list[iFile]);

		std::string output_filename = "out_"+file_list[iFile];

		pLmmSwaptionMarketData->print(output_filename);
	}
}