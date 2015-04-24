#pragma once

#include <vector>
#include <utility> // std::pair, std::make_pair
#include <stdlib.h>

#include <string>
#include <iostream>
/*
namespace Name
{
	typedef size_t NumberOfMonth;
	typedef size_t indexInLMMTenorStructure;
}*/

namespace OptionType
{
	enum OptionType {CALL, PUT};
}

namespace LMM
{
	typedef size_t Index;

	// Store a couple of years
	typedef std::vector<std::vector<std::pair<double,double> > > Matrix_PairOfYears   ;

	typedef std::vector<std::vector<std::pair<size_t,size_t> > > Matrix_PairOfMonths  ;

	typedef std::vector<std::vector<std::pair<size_t,size_t> > > Matrix_PairOfIndices ;
}

//helper printing pair of indices
inline std::ostream& operator<<(std::ostream& os, const std::pair<size_t, size_t>& indices)
{
	os << "(" << indices.first <<"|"<<indices.second <<")"; return os;
}
