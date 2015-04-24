#pragma once

#include <vector>

#include <ql/math/randomnumbers/sobolrsg.hpp>
#include <ql/math/distributions/normaldistribution.hpp>
#include <ql/math/randomnumbers/inversecumulativersg.hpp>
#include <ql/types.hpp>
#include <ql/math/array.hpp>

#include <LMM/RNGenerator/RNGenerator.h>

using namespace QuantLib ;

class QMcGenerator : RNGenerator
{
private:
	size_t   sequence_size_; 
	size_t   skipRank_; // Number of sobol points to skip (to avoid badly generated sequences)
	SobolRsg sobol_;    // QuantLib Sobol LDS generator
	InverseCumulativeRsg<SobolRsg,InverseCumulativeNormal> generator_;
	
public:
	QMcGenerator(unsigned long qmcSeed, size_t sequence_size, size_t skipRank);

	virtual void generate(std::vector<double>& out_randomSequence);
	
	virtual void resetGeneratorToinitSeed();

	size_t getSequenceSize();
	void setSequenceSize(size_t size);
};