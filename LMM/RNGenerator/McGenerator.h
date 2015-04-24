#pragma once

#include <boost/random/normal_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>

#include <LMM/RNGenerator/RNGenerator.h>

typedef boost::variate_generator<boost::mt19937, boost::normal_distribution<> > RNG_Type;
//typedef std::vector<std::vector<double>> matrix_;

class McGenerator : public RNGenerator
{
private:
	RNG_Type generator_;

public:
	McGenerator();
	McGenerator(unsigned long mcSeed);

	//-- Generate a pseudo random sequence 
	void generate(std::vector<double>& out_randomSequence); 
	
	//double getSeed();
	void resetGeneratorToinitSeed()
	{
		generator_ = RNG_Type(boost::mt19937(seed_),boost::normal_distribution<>());
	}
};
