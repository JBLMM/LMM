#include "CallableSwap.h"


CallableSwap::CallableSwap(GeneticSwap_CONSTPTR geneticSwap, const std::vector<LMM::Index>& exerciseTimes)
	:
	geneticSwap_(geneticSwap),
	exerciseTimes_(exerciseTimes)
{
}
