#define BOOST_TEST_MODULE LMM_unit_test_Interpolation

#include <boost/test/included/unit_test.hpp>

#include <LMM/numeric/Interpolation.h>

using namespace numeric;

BOOST_AUTO_TEST_SUITE(lmm_numeric_interpolation)

BOOST_AUTO_TEST_CASE(test_LinearInterpolation)
{
	std::vector<double> someVector(3);
	someVector[0] = 0.01;  
	someVector[1] = 0.02; 
	someVector[2] = 0.03;

	std::vector<double> falseTenorDates(3);
	falseTenorDates[0] = 0.;
	falseTenorDates[1] = 0.5;
	falseTenorDates[2] = 1.;

	double maturity_date = 0.75; 
	
	double coeff_1 = (falseTenorDates[2]-maturity_date)/(falseTenorDates[2]-falseTenorDates[1]);
	double coeff_2 = (maturity_date-falseTenorDates[1])/(falseTenorDates[2]-falseTenorDates[1]);
	double expectedResult = coeff_1*someVector[1] + coeff_2*someVector[2];

	numeric::Interpolation interpolator;
	interpolator.linearInterpolation(someVector,falseTenorDates,maturity_date);
	
	BOOST_CHECK(someVector.size()      == 4 );
	BOOST_CHECK(falseTenorDates.size() == 4 );

	BOOST_CHECK(falseTenorDates[2] = maturity_date  );//additioinal point
	BOOST_CHECK(someVector[2]      = expectedResult );

	std::cout<<"someVector : ";
	for (auto inter_val : someVector) std::cout << inter_val << " "; std::cout<<std::endl;

	std::cout<<"falseTenorDates : ";
	for (auto inter_val : falseTenorDates) std::cout << inter_val << " ";  std::cout<<std::endl ;

	std::cout << std::endl;
	std::cout << "Expected result : " << expectedResult << std::endl;

	

}

BOOST_AUTO_TEST_CASE(test_LinearInterpolation_approx)
{
	std::vector<double> someVector(3);
	someVector[0] = 0.01;  
	someVector[1] = 0.02; 
	someVector[2] = 0.03;

	std::vector<double> falseTenorDates(3);
	falseTenorDates[0] = 0.  ;
	falseTenorDates[1] = 0.5 ;
	falseTenorDates[2] = 1.  ;

	double maturity_date = 0.50000001; // very closed date to an existing date --> no need to interpolate
	
	numeric::Interpolation interpolator;
	interpolator.linearInterpolation(someVector,falseTenorDates,maturity_date,0.000001);
	
	BOOST_CHECK(someVector.size()      == 3 );
	BOOST_CHECK(falseTenorDates.size() == 3 );

	std::cout<<"someVector : ";
	for (auto inter_val : someVector) std::cout << inter_val << " "; std::cout<<std::endl;

	std::cout<<"falseTenorDates : ";
	for (auto inter_val : falseTenorDates) std::cout << inter_val << " ";  std::cout<<std::endl ;

}

BOOST_AUTO_TEST_SUITE_END()
