#pragma once

#include <boost/shared_ptr.hpp>

#include <GeneticSwap.h>

class GeneticSwaption
{
	double maturity_;
	GeneticSwap_PTR geneticSwap_;
	//bool check(): maturity_ <= firstindex 	
};
typedef boost::shared_ptr<GeneticSwaption> GeneticSwaption_PTR;
typedef boost::shared_ptr<const GeneticSwaption> GeneticSwaption_CONSTPTR;


