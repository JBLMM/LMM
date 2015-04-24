#define BOOST_TEST_MODULE LMM_unit_test_integration
#include <boost/test/included/unit_test.hpp>

#include <cmath>
#include <iostream>

#include <boost/function.hpp>

#include <ql/math/integrals/simpsonintegral.hpp>

BOOST_AUTO_TEST_SUITE(lmm_test_integration)


double lnFunc(double x){return std::log(x);}
double xCubicFunc(double x){return x*x*x;}

//! \int_{0}^{1} ln(x) dx
void test_Simpsonintegral()
{
    boost::function<double (double)> func;
	//func = lnFunc;
	func = xCubicFunc;

	double absoluteAccuracy = 1e-4;
	size_t maxEvaluations   = 1000;

	QuantLib::SimpsonIntegral numInt(absoluteAccuracy, maxEvaluations);

	double boundLower = 0;
	double boundUpper = 1;


	std::cout << "int_{0}^{1} ln(x) dx = " << numInt(func, boundLower, boundUpper) << std::endl;
}

BOOST_AUTO_TEST_CASE(test_InegrationMethod)
{

	test_Simpsonintegral();
	
	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_SUITE_END()
