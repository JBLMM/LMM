#include <iostream>
#include <vector>
#include <cassert>
#include <LMM/RNGenerator/McGenerator.h>



//McGenerator::McGenerator()
//	: mcSeed_(0.), generator_(boost::mt19937(0.),boost::normal_distribution<>())
//{}

McGenerator::McGenerator(unsigned long mcSeed)  //YY: shit how can use use double as seed ? 
	: RNGenerator(mcSeed), 
	  generator_(boost::mt19937(mcSeed),boost::normal_distribution<>())
{}

//-- Generate a pseudo random sequence 
void McGenerator::generate(std::vector<double>& out_randomSequence)
{
	//assert(out_randomSequence.size() > 0);
	for (size_t i = 0; i < out_randomSequence.size(); ++i)
		out_randomSequence[i] = generator_();
}


//double McGenerator::getSeed(){return mcSeed_;}
//void McGenerator::setSeed(unsigned long seed){mcSeed_=seed;}