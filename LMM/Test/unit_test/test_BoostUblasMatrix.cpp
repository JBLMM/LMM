// http://stackoverflow.com/questions/11110604/why-is-boosts-matrix-multiplication-slower-than-mine

#define BOOST_TEST_MODULE LMM_unit_test_boost
#include <boost/test/included/unit_test.hpp>

#include <iostream>
#include <ctime>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/operation.hpp>
#include <boost/numeric/ublas/operation_blocked.hpp>

#include <ql/math/matrix.hpp>

using namespace boost::numeric::ublas;

BOOST_AUTO_TEST_SUITE(lmm_test_boost)

	BOOST_AUTO_TEST_CASE(test_BoostUBLASMatrix)
{
	std::cout << "begin the calculation of matrix multiplication" << std::endl;
	size_t z1 = 10;
	size_t z2 = z1;
	boost::numeric::ublas::matrix<double> m1(z1,z2);
	for(size_t i=0; i<m1.size1(); ++i)
		for(size_t j=0; j<m1.size1(); ++j)
			m1(i,j) = i*10+j;


	boost::numeric::ublas::matrix<double> m2(z1,z2);
	for(size_t i=0; i<m1.size1(); ++i)
	{
		for(size_t j=0; j<m1.size1(); ++j)
		{
			m1(i,j) = i+j*10;
			std::cout<<"Mat["<<i<<","<<j<<"]"<<std::endl;
		}
	}


	clock_t t1,t2;
	double diff;
	boost::numeric::ublas::matrix<double> m;

	t1 = clock();
	m = boost::numeric::ublas::prod(m1,m2);
	t2 = clock();
	diff = t2 - t1;
	std::cout << "prod use time = " << diff << std::endl;

	t1 = clock();
	m = block_prod<matrix<double>,1024>(m1,m2); 
	t2 = clock();
	diff = t2 - t1;
	std::cout << "block_prod use time = " << diff << std::endl;


	size_t nbRow = 3; 
	size_t nbCol = 7;
	QuantLib::Matrix qlmatrix(nbRow,nbCol,-1);
	for(size_t i=0; i<nbRow; ++i)
	{
		for(size_t j=0; j<nbCol; ++j)
		{
			qlmatrix[i][j] = i*nbCol+j;			
		}
	}


	boost::numeric::ublas::matrix<double> umatrix(nbRow,nbCol);
	for(size_t i=0; i<nbRow; ++i)
	{
		for(size_t j=0; j<nbCol; ++j)
		{
			umatrix(i,j) = i*nbCol+j;			
		}
	}

	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(test_QuantLibMatrix)
{
	QuantLib::Matrix qlmatrix(10,10,-1);

}
BOOST_AUTO_TEST_SUITE_END()