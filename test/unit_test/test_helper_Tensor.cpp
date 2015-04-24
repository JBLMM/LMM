#define BOOST_TEST_MODULE LMM_unit_test_helper_Tensor

#include <boost/test/included/unit_test.hpp>

#include <LMM/helper/Tensor.h>

BOOST_AUTO_TEST_SUITE(lmm_test_helper_tensor)

BOOST_AUTO_TEST_CASE(test_Tensor_constructor)
{
	Tensor small_tensor(2,4,5);

	std::cout<<small_tensor<<std::endl;
	small_tensor.print("unitTest_helper_Tensor.csv");
}

BOOST_AUTO_TEST_CASE(test_Tensor_methods)
{

}


BOOST_AUTO_TEST_SUITE_END()