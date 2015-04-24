#define BOOST_TEST_MODULE LMM_unit_test_model_RobonatoAngle
#include <boost/test/included/unit_test.hpp>

#include <math.h>
#include <cassert>

#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/math/constants/constants.hpp>

#include <ql/math/optimization/endcriteria.hpp>
#include <ql/math/optimization/constraint.hpp>
#include <ql/math/optimization/problem.hpp>
#include <ql/math/optimization/simplex.hpp>
#include <ql/math/optimization/levenbergmarquardt.hpp>
#include <ql/math/optimization/conjugategradient.hpp>
#include <ql/math/matrixutilities/tapcorrelations.hpp>
#include <ql/math/matrixutilities/pseudosqrt.hpp>
//#include <ql/errors.hpp>

#include <LMM/helper/GenericPath.h>
#include <LMM/ModelMCLMM/PCA.h>
#include <LMM/ModelMCLMM/RobonatoAngle.h>

BOOST_AUTO_TEST_SUITE(lmm_test_model_RobonatoAngle)


BOOST_AUTO_TEST_CASE(testSpecialAngles)
{
	size_t matrixSize = 40;
	size_t rank = 3;
	size_t angleSize = (rank-1) * matrixSize;
	Array angles(angleSize,0.25);
	RobonatoAngle ra(matrixSize, rank, angles);

	for(size_t i=0; i<angleSize; ++i)
	{
		std::cout << "index [" << i << "] is special ? " << ra.isSpecialAnlge(i) << std::endl;
	}
}


BOOST_AUTO_TEST_CASE(test4RandRA)
{
	Matrix B(4,2);


	//B[0][0] = 1.0;
	//B[0][1] = 0.125777;
	//B[0][2] = 0.0445788;
	//B[0][3] = 0.0158;

	//B[1][0] = 0.125777;
	//B[1][1] = 1.0;
	//B[1][2] = 0.354429;
	//B[1][3] = 0.12562;

	//B[2][0] = 0.0445788;
	//B[2][1] = 0.354429;
	//B[2][2] = 1.0;
	//B[2][3] = 0.354429;

	//B[3][0] = 0.0158;
	//B[3][1] = 0.12562;
	//B[3][2] = 0.354429;
	//B[3][3] = 1.0;

	//1	0.125777	0.0445788	0.0158
	//0.125777	1	0.354429	0.12562
	//0.0445788	0.354429	1	0.354429
	//0.0158	0.12562	0.354429	1

	B[0][0] = 1.0;
	B[0][1] = 0;

	B[1][0] = 0.930984;
	B[1][1] = 0.36506;

	B[2][0] = 0.981581;
	B[2][1] = -0.191045;

	B[3][0] = 0.85686;
	B[3][1] = -0.51555;



	//1	0
	//0.930984	0.36506
	//0.981581	-0.191045
	//0.85686	-0.51555



	RobonatoAngle rbAngle1(B);
	std::string fileName1 = LMM::get_output_path() + "unitTest_ModelMCLMM_Robonato_4Rank_RobonatoAngle_1.csv";
	rbAngle1.print_details(fileName1);


	RobonatoAngle rbAngle2(4, 2, rbAngle1.get_angles());
	std::string fileName2 = LMM::get_output_path() + "unitTest_ModelMCLMM_Robonato_4Rank_RobonatoAngle_2.csv";
	rbAngle2.print_details(fileName2);

}

BOOST_AUTO_TEST_CASE(testRobonatoAngle)
{

	//! angles -> BMatrix, correlMatrix
	size_t matrixSize = 40;
	size_t rank = 3;
	size_t angleSize = (rank-1) * matrixSize;

	Array angles(angleSize);
	boost::variate_generator<boost::mt19937, boost::random::uniform_real_distribution<> >
		generator(boost::mt19937(0.5),boost::random::uniform_real_distribution<>()); // uniform generator [0, 1]


	double PI = boost::math::constants::pi<double>();

	for(size_t i=0; i<angleSize; ++i)
	{
		if(RobonatoAngle::isSpecialAnlge(i,rank))
			angles[i] = generator()*2*PI; // [0,2*PI]
		else
			angles[i] = generator()*PI;   // [0,PI]
	}

	RobonatoAngle rbAngle1(matrixSize, rank, angles);
	std::string fileName1 = LMM::get_output_path() + "unitTest_ModelMCLMM_Robonato_Random_RobonatoAngle1.csv";
	rbAngle1.print_details(fileName1);


	//! BMAtrix -> angles, correlMatrix  // Test is passed when the input and output angle are the same.
	RobonatoAngle rbAngle2(rbAngle1.get_BMatrix());
	std::string fileName2 = LMM::get_output_path() + "unitTest_ModelMCLMM_Robonato_Random_RobonatoAngle2.csv";
	rbAngle2.print_details(fileName2);

	//! BMAtrix -> angles, correlMatrix  // Test is passed when the input and output angle are the same.
	RobonatoAngle rbAngle3(matrixSize, rank, rbAngle2.get_angles());
	std::string fileName3 = LMM::get_output_path() + "unitTest_ModelMCLMM_Robonato_Random_RobonatoAngle3.csv";
	rbAngle3.print_details(fileName3);
}

BOOST_AUTO_TEST_SUITE_END()