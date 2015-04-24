#define BOOST_TEST_MODULE LMM_unit_test_instrument_Tenor
#include <boost/test/included/unit_test.hpp>

#include <iostream>
#include <cassert>
#include <string.h>

#include <LMM/helper/TenorType.h>

BOOST_AUTO_TEST_SUITE(lmm_test_instrument_Tenor)


BOOST_AUTO_TEST_CASE(test_TenorType_values)
{
	Tenor TenorNon = Tenor::_Non;
	BOOST_CHECK(TenorNon.NbOfMonth()  == 0     ) ; 
	BOOST_CHECK(TenorNon.YearFraction() == 0./12 ) ; 
	BOOST_CHECK(strcmp(TenorNon.Name().c_str(), "Non")==0);
	std::cout<< TenorNon <<std::endl;

	Tenor Tenor1M  = Tenor::_1M;
	BOOST_CHECK( Tenor1M.NbOfMonth()  == 1     ) ; 
	BOOST_CHECK( Tenor1M.YearFraction() == 1./12 ) ; 
	BOOST_CHECK(strcmp( Tenor1M.Name().c_str() , "1M" )==0);
	std::cout<< Tenor1M <<std::endl;

	Tenor Tenor3M  = Tenor::_3M;
	BOOST_CHECK( Tenor3M.NbOfMonth()  == 3     ); 
	BOOST_CHECK( Tenor3M.YearFraction() == 3./12 ); 
	BOOST_CHECK(strcmp( Tenor3M.Name().c_str() , "3M" )==0);
	std::cout<< Tenor3M <<std::endl;

	Tenor Tenor6M  = Tenor::_6M;
	BOOST_CHECK( Tenor6M.NbOfMonth()  == 6     ) ;
	BOOST_CHECK( Tenor6M.YearFraction() == 6./12 ) ;
	BOOST_CHECK(strcmp( Tenor6M.Name().c_str() , "6M" )==0);
	std::cout<< Tenor6M <<std::endl;


	Tenor Tenor9M  = Tenor::_9M;
	BOOST_CHECK( Tenor9M.NbOfMonth()  == 9     ) ;
	BOOST_CHECK( Tenor9M.YearFraction() == 9./12 ) ;
	BOOST_CHECK(strcmp( Tenor9M.Name().c_str() , "9M" )==0);
	std::cout<< Tenor9M <<std::endl;

	Tenor Tenor12M = Tenor::_12M;
	BOOST_CHECK( Tenor12M.NbOfMonth()  == 12     ) ;
	BOOST_CHECK( Tenor12M.YearFraction() == 12./12 ) ;
	BOOST_CHECK(strcmp( Tenor12M.Name().c_str() , "12M" )==0);
	std::cout<< Tenor12M <<std::endl;


	Tenor Tenor1Y  = Tenor::_1YR;
	BOOST_CHECK( Tenor1Y.NbOfMonth()  == 12     ) ;
	BOOST_CHECK( Tenor1Y.YearFraction() == 12./12 ) ;
	BOOST_CHECK(strcmp( Tenor1Y.Name().c_str() , "1YR" )==0);
	std::cout<< Tenor1Y <<std::endl;
}

BOOST_AUTO_TEST_CASE(test_TenorType_comparing)
{
	Tenor TenorNon1 = Tenor::_Non;
	Tenor TenorNon2 = Tenor::_Non;
	BOOST_CHECK(TenorNon1 == TenorNon2);

	Tenor Tenor12M = Tenor::_12M;
	Tenor Tenor1Y  = Tenor::_1YR;
	BOOST_CHECK(Tenor12M == Tenor1Y);

	BOOST_CHECK(TenorNon1 != Tenor12M);

	BOOST_CHECK( !TenorNon1.isValidTenor() );
	BOOST_CHECK( Tenor12M.isValidTenor() );
}


BOOST_AUTO_TEST_CASE(test_TenorType_constructur_assignment)
{
	Tenor my_tenor  = Tenor::_1M ;
	Tenor my_tenor1 = Tenor::_6M ;
	Tenor my_tenor2 = Tenor::_1YR;

	BOOST_CHECK(my_tenor != my_tenor1 ) ; 

	my_tenor = my_tenor1;
	BOOST_CHECK(my_tenor == my_tenor1 ) ; 
	
	my_tenor = my_tenor2;
	BOOST_CHECK(my_tenor != my_tenor1 ) ; 
}



BOOST_AUTO_TEST_CASE(test_TenorType_calculus)
{
	Tenor Tenor1M  = Tenor::_1M;
	Tenor Tenor3M  = Tenor::_3M;
	Tenor Tenor6M  = Tenor::_6M;
	Tenor Tenor9M  = Tenor::_9M;
	Tenor Tenor12M = Tenor::_12M;
	Tenor Tenor1Y  = Tenor::_1YR;

	BOOST_CHECK(Tenor1Y.NbOfMonth()  == 12 );
	BOOST_CHECK(Tenor12M.NbOfMonth() == 12 );
	BOOST_CHECK(Tenor6M.NbOfMonth()  == 6  );

	BOOST_CHECK(Tenor3M.YearFraction()  == 0.25 );
	BOOST_CHECK(Tenor6M.YearFraction()   == 0.5 );
	BOOST_CHECK(Tenor12M.YearFraction()  == 1.0 );

	BOOST_CHECK(Tenor12M.ratioTo(Tenor1Y)  == 1 );
	BOOST_CHECK(Tenor12M.ratioTo(Tenor9M)  == 1 );
	BOOST_CHECK(Tenor12M.ratioTo(Tenor3M)  == 4 );
	BOOST_CHECK(Tenor12M.ratioTo(Tenor6M)  == 2 );
}

BOOST_AUTO_TEST_SUITE_END()