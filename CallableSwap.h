#pragma once

#include <vector>

#include <GeneticSwap.h>
#include <LMM/helper/Name.h>

class CallableSwap
{
	GeneticSwap_CONSTPTR geneticSwap_;
	std::vector<LMM::Index> exerciseTimes_;
	//check: exerciseTikme in [fistIndex, lastindex]

public:
	//getter
	GeneticSwap_CONSTPTR getGeneticSwap()const{return geneticSwap_;}
	const std::vector<LMM::Index>& getExerciseTimes()const{return exerciseTimes_;}

	//constructeur
	CallableSwap(GeneticSwap_CONSTPTR geneticSwap, const std::vector<LMM::Index>& exerciseTimes);

	//destructeur
	virtual ~CallableSwap(){}
	
};

