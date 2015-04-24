#pragma once

#include <vector>
#include <cassert>
#include <cmath>

template<class T> 
bool vectorEqual(const std::vector<T>& v1, const std::vector<T>& v2, const double& tolerance)
{
	assert(tolerance >= 0.0);

    if(v1.size()!= v2.size())
		return false;

	for(size_t i=0; i<v1.size(); ++i)
	{
		if(std::abs(v1[i]-v2[i])>tolerance)	return false;
	}

	return true;
}


