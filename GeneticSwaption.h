#pragma once

#include <boost/shared_ptr.hpp>

#include <LMM/helper/Name.h>
#include <GeneticSwap.h>

class GeneticSwaption
{
	LMM::Index maturity_;
	GeneticSwap_CONSTPTR geneticSwap_;

	//check maturity_ <= firstindex
	bool check()const; 

public:

	//constructeur
	GeneticSwaption(LMM::Index maturity, GeneticSwap_CONSTPTR geneticSwap);   

	//getter
	LMM::Index getMaturity()const{return maturity_;}							
	GeneticSwap_CONSTPTR getGeneticSwap()const{return geneticSwap_;}    
};
typedef boost::shared_ptr<GeneticSwaption> GeneticSwaption_PTR;
typedef boost::shared_ptr<const GeneticSwaption> GeneticSwaption_CONSTPTR;


