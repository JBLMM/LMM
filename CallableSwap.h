#pragma once
#include <GeneticSwap.h>

#include <vector>

#include <LMM/helper/Name.h>

class CallableSwap
{
	GeneticSwap_PTR geneticSwap_;
	std::vector<LMM::Index> exerciseTimes_;

	//check: exerciseTikme in [fistIndex, lastindex]
};

