#define BOOST_TEST_MODULE LMM_unit_test_NumericalsMethods

#include <boost/test/included/unit_test.hpp>

#include <LMM/numeric/NumericalMethods.h>

BOOST_AUTO_TEST_SUITE(lmm_numeric_numericalmethods)

BOOST_AUTO_TEST_CASE(test_interpolator)
{
	size_t number_of_points = 10;

	std::vector<double> maturities(number_of_points);
	for (size_t i = 0; i < number_of_points; ++i)
		maturities[i] = i*0.5;

	std::vector<double> points(number_of_points); 
	for (size_t i = 0; i < number_of_points; ++i)
		points[i] = i;

	double t = 1.25;
	double interpolated_point = NumericalMethods::linearInterpolation(t,maturities,points);
	std::cout << "Date before: " << 1 << "-- Point: " << points[2] << std::endl;
	std::cout << "Date after: " << 1.5 << "-- Point: " << points[3] << std::endl;
	std::cout << "Given date: " << t << " -- Interpolated point: " << interpolated_point << std::endl << std::endl;

	t = 2.3;
	interpolated_point = NumericalMethods::linearInterpolation(t,maturities,points);
	std::cout << "Date before: " << 2 << "-- Point: " << points[4] << std::endl;
	std::cout << "Date after: " << 2.5 << "-- Point: " << points[5] << std::endl;
	std::cout << "Given date: " << t << " -- Interpolated point: " << interpolated_point << std::endl << std::endl;

	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_SUITE_END()