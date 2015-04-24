#define BOOST_TEST_MODULE LMM_unit_test_instrument_VanillaSwap

#include <string>
#include <fstream>

#include <boost/test/included/unit_test.hpp>

#include <LMM/helper/Name.h>
#include <LMM/helper/GenericPath.h>
#include <LMM/helper/TenorType.h>
#include <LMM/helper/LMMTenorStructure.h>

#include <LMM/instrument/VanillaSwap.h>
#include <LMM/instrument/VanillaSwaption.h>


BOOST_AUTO_TEST_SUITE(lmm_instrument_VanillaSwap)

BOOST_AUTO_TEST_CASE(test_VanillaSwap_constructor)
{
	const double strike = 10.;
	LMM::Index startIndex = 2;
	LMM::Index endIndex   = 11;

	Tenor FixedTenor = Tenor::_9M;
	Tenor FloatTenor = Tenor::_3M;
	LMMTenorStructure_PTR simulationStructure(new LMMTenorStructure(Tenor::_3M , 10) );

	LMM::VanillaSwap swap(strike, startIndex, endIndex, FloatTenor, FixedTenor, simulationStructure);

	std::cout<<swap<<std::endl;	

	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(test_VanillaSwaption_constructor)
{
	const double strike = 10.;
	LMM::Index startIndex = 2;
	LMM::Index endIndex   = 11;

	Tenor FixedTenor = Tenor::_9M;
	Tenor FloatTenor = Tenor::_3M;
	LMMTenorStructure_PTR simulationStructure(new LMMTenorStructure(Tenor::_3M , 10) );

	LMM::VanillaSwap swap(strike, startIndex, endIndex, FloatTenor, FixedTenor, simulationStructure);
 
	BOOST_CHECK(swap.get_strike() == 10.);
	swap.strike() = 11.;                   //reset strike for VanillaSwap
	BOOST_CHECK(swap.get_strike() == 11.);

	VanillaSwaption swaption(swap,OptionType::CALL);

	BOOST_CHECK(swaption.get_strike() == 11.);
	swaption.strike() = 12.;                   //reset strike for VanillaSwaption
	BOOST_CHECK(swaption.get_strike() == 12.);
}


BOOST_AUTO_TEST_CASE(test_VanillaSwap_configure)
{
	const double strike = 10.;

	LMM::Index startIndex = 1;
	LMM::Index endIndex   = 5;

	Tenor FixedTenor = Tenor::_6M ; Tenor FloatTenor = Tenor::_3M;
	LMMTenorStructure_PTR simulationStructure(new LMMTenorStructure(Tenor::_3M , 2) );

	LMM::VanillaSwap swap(strike, startIndex, endIndex, FloatTenor, FixedTenor, simulationStructure);

	const std::vector<LMM::Index>& indiceFloatLeg = swap.get_floatingLegPaymentIndexSchedule() ;
	const std::vector<LMM::Index>& indiceFixedLeg = swap.get_fixedLegPaymentIndexSchedule()   ;

	const std::vector<double>& deltaTFloatLeg = swap.get_DeltaTFloatLeg() ;
	const std::vector<double>& deltaTFixedLeg = swap.get_DeltaTFixedLeg() ;

	BOOST_CHECK(indiceFloatLeg.size() == deltaTFloatLeg.size() );
	BOOST_CHECK(indiceFixedLeg.size() == deltaTFixedLeg.size() );

	BOOST_CHECK( swap.get_EndDate()   == 1.25 );
	BOOST_CHECK( swap.get_StartDate() == 0.25 );

	BOOST_CHECK(indiceFloatLeg[0] == 2);
	BOOST_CHECK(indiceFloatLeg[1] == 3);
	BOOST_CHECK(indiceFloatLeg[2] == 4);
	BOOST_CHECK(indiceFloatLeg[3] == 5);

	BOOST_CHECK(indiceFixedLeg[0] == 3);
	BOOST_CHECK(indiceFixedLeg[1] == 5);

	BOOST_CHECK(deltaTFloatLeg[0] == 0.25);
	BOOST_CHECK(deltaTFloatLeg[1] == 0.25);
	BOOST_CHECK(deltaTFloatLeg[2] == 0.25);
	BOOST_CHECK(deltaTFloatLeg[3] == 0.25);

	BOOST_CHECK(deltaTFixedLeg[0] == 0.5);
	BOOST_CHECK(deltaTFixedLeg[1] == 0.5);

	swap.print("unitTest_instrument_VanillaSwap_configure.csv");

	BOOST_CHECK(true);
}


BOOST_AUTO_TEST_SUITE_END()