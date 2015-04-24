#include <iostream>

#include <LMM/RNGenerator/QMcGenerator.h>


QMcGenerator::QMcGenerator(unsigned long qmcSeed, size_t sequence_size, size_t skipRank) 
	: RNGenerator(qmcSeed),
	  sequence_size_(sequence_size),
	  skipRank_(skipRank),
	  sobol_(sequence_size,qmcSeed),
	  generator_(sobol_)

{
	sobol_.skipTo(skipRank_);
	generator_ = InverseCumulativeRsg<SobolRsg,InverseCumulativeNormal>(sobol_);
}


void QMcGenerator::generate(std::vector<double>& out_randomSequence) 
{
	throw("Error: what a shit why use sequence_size other than real size for generating ?");
	for (size_t i = 0; i < sequence_size_; ++i)
	{
		out_randomSequence.push_back(generator_.nextSequence().value[i]);
	}
}
	
void QMcGenerator::resetGeneratorToinitSeed()
{
	std::cout<< "LMM WARNING QMcGenerator::resetGeneratorToinitSeed() is not yet implemented"<<std::endl;
}

//double QMcGenerator::getSeed() {return qmcSeed_;}
size_t QMcGenerator::getSequenceSize() {return sequence_size_;}

//void QMcGenerator::setSeed(unsigned long seed) {qmcSeed_ = seed;}
void QMcGenerator::setSequenceSize(size_t size) {sequence_size_ = size;}