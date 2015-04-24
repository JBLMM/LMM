#define BOOST_TEST_MODULE LMM_unit_test_helper_QMcGenerator
#include <boost/test/included/unit_test.hpp>


#include <LMM/RNGenerator/QMcGenerator.h>

BOOST_AUTO_TEST_SUITE(lmm_test_helper_qmcgenerator)

BOOST_AUTO_TEST_CASE(test_SobolLsdGeneration)
{
	unsigned long qmcSeed = 1 ;
	size_t sequence_size  = 10;
	size_t skipRank       = 5 ;
	QMcGenerator qmcg(qmcSeed,sequence_size,skipRank);

	std::vector<double> random_sequence;
	qmcg.generate(random_sequence);

	std::cout << "-- Sobol LDS generation --" << std::endl;
	for (auto rand_num : random_sequence) std::cout << rand_num << " ";
	std::cout << std::endl;
}

BOOST_AUTO_TEST_CASE(test2)
{

	QuantLib::SobolRsg sobol(20,1); //-- Create a 20 dim sobol sequence generator
	sobol.skipTo(4096); //-- Skip the first sequences as they are badly distributed...
	std::vector<double> point = sobol.nextSequence().value;
	
	std::cout << "Sobol sequence dimension: " << point.size() << std::endl;
	std::cout << "Sobol sequence: " << std::endl;
	for(auto num : point) std::cout << num << " ";

	std::cout << std::endl << std::endl;

	//-- Fill a vector with a normally distributed sequence, obtained from sobol generator
	std::vector<double> rand_seq;
	QuantLib::InverseCumulativeRsg<SobolRsg,InverseCumulativeNormal> generator(sobol);

	rand_seq = generator.nextSequence().value;
	std::cout << "Normally distributed sequence: " << std::endl;
	for (auto num : rand_seq) std::cout << num << " ";

	std::cout << std::endl << std::endl;
}


BOOST_AUTO_TEST_SUITE_END()