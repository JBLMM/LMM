#define BOOST_TEST_MODULE LMM_unit_test_helper

#include <boost/test/included/unit_test.hpp>

#include <LMM/helper/LMMTenorStructure.h>


BOOST_AUTO_TEST_SUITE(lmm_test_LMMTenorStructure)

BOOST_AUTO_TEST_CASE(test_LMMTenorStrucure_constructor)
{
	Tenor tenor6M = Tenor::_6M;
	const double maxYear = 3.2;

	LMMTenorStructure tenorstructure(tenor6M, maxYear);

	BOOST_CHECK( tenorstructure.get_EndDate() == 3.0 ) ;
	BOOST_CHECK( tenorstructure.get_horizon() == 5   ) ;
	BOOST_CHECK( tenorstructure.get_nbLIBOR() == 6   ) ;

	std::cout<<tenorstructure<<std::endl;
}


BOOST_AUTO_TEST_CASE(test_LMMTenorStrucure_is_TenorDate)
{
	Tenor tenorType = Tenor::_6M;
	double maxYear = 10;

	LMMTenorStructure lmmTenorStructure(tenorType, maxYear );

	BOOST_CHECK( lmmTenorStructure.is_TenorDate(  0.  ) );
	BOOST_CHECK( lmmTenorStructure.is_TenorDate(  0.5 ) );
	BOOST_CHECK( lmmTenorStructure.is_TenorDate(  1.0 ) );
	BOOST_CHECK( lmmTenorStructure.is_TenorDate( 10.0 ) );
	BOOST_CHECK(!lmmTenorStructure.is_TenorDate( 11.0 ) );

	//std::cout<<lmmTenorStructure<<std::endl;
}

BOOST_AUTO_TEST_CASE(test_LMMTenorStrucure_approximation_get_Index)
{
	Tenor tenorType = Tenor::_6M;
	double maxYear = 3;

	LMMTenorStructure lmmTenorStructure(tenorType, maxYear);

	BOOST_CHECK(lmmTenorStructure.get_tenorType(1./12) == Tenor::_1M );
	BOOST_CHECK(lmmTenorStructure.get_tenorType(3./12) == Tenor::_3M );
	BOOST_CHECK(lmmTenorStructure.get_tenorType(6./12) == Tenor::_6M );
	BOOST_CHECK(lmmTenorStructure.get_tenorType(9./12) == Tenor::_9M );
	BOOST_CHECK(lmmTenorStructure.get_tenorType(   1.) == Tenor::_1YR);

	BOOST_CHECK(lmmTenorStructure.get_tenorType(  0.7) == Tenor::_Non);


	BOOST_CHECK(lmmTenorStructure.get_Index(0.00000000001) == 0 );
	BOOST_CHECK(lmmTenorStructure.get_Index(0.50000000001) == 1 );
	BOOST_CHECK(lmmTenorStructure.get_Index(2.00000000001) == 4 );
	BOOST_CHECK(lmmTenorStructure.get_Index(3.00000000001) == 6 );

	lmmTenorStructure.print("unitTest_helper_LMMTenorStructure.csv");
}




BOOST_AUTO_TEST_CASE(test_LMMTenorStrucure_comparator)
{
	Tenor tenor1 = Tenor::_3M ; size_t nbYear1 = 2;
	Tenor tenor2 = Tenor::_3M ; size_t nbYear2 = 2;
	Tenor tenor3 = Tenor::_6M ; size_t nbYear3 = 2;
	Tenor tenor4 = Tenor::_3M ; size_t nbYear4 = 4;

	LMMTenorStructure lmmTenorStructure1(tenor1, nbYear1);
	LMMTenorStructure lmmTenorStructure2(tenor2, nbYear2);
	LMMTenorStructure lmmTenorStructure3(tenor3, nbYear3);
	LMMTenorStructure lmmTenorStructure4(tenor4, nbYear4);

	BOOST_CHECK(lmmTenorStructure1 == lmmTenorStructure2);
	BOOST_CHECK(lmmTenorStructure1 != lmmTenorStructure3);
	BOOST_CHECK(lmmTenorStructure1 != lmmTenorStructure4);
}

BOOST_AUTO_TEST_CASE(test_LMMTenorStrucure_dates)
{
	Tenor tenor = Tenor::_6M ; unsigned int tenorInMonth = tenor.NbOfMonth();
	size_t nbYear = 2;
	size_t horizonsize = nbYear * (12 / tenorInMonth) - 1;

	LMMTenorStructure lmmTenorStructure(tenor, nbYear);

	BOOST_CHECK(lmmTenorStructure.get_horizon() == horizonsize   ) ;
	BOOST_CHECK(lmmTenorStructure.get_nbLIBOR() == horizonsize+1 ) ;

	const std::vector<double>& deltaT = lmmTenorStructure.get_deltaT()    ;
	const std::vector<double>& dates  = lmmTenorStructure.get_tenorDate() ;

	BOOST_CHECK(deltaT.size() == lmmTenorStructure.get_horizon() + 1 );
	BOOST_CHECK( dates.size() == lmmTenorStructure.get_horizon() + 2 );

	BOOST_CHECK(deltaT[0] == 0.5 );
	BOOST_CHECK(deltaT[1] == 0.5 );
	BOOST_CHECK(deltaT[2] == 0.5 );
	BOOST_CHECK(deltaT[3] == 0.5 );

	BOOST_CHECK(dates[0] == 0.0 );
	BOOST_CHECK(dates[1] == 0.5 );
	BOOST_CHECK(dates[2] == 1.0 );
	BOOST_CHECK(dates[3] == 1.5 );
	BOOST_CHECK(dates[4] == 2.0 );
}

BOOST_AUTO_TEST_SUITE_END()
