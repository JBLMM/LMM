#pragma once
#include <boost/shared_ptr.hpp>

#include "CheyetteDD_Model.h"
#include <LMM\instrument\VanillaSwaption.h>  
#include <LMM\RNGenerator\RNGenerator.h>


//pricing sous Q de la swaption par MC

class CheyetteDD_VanillaSwaptionMCPricer
{
private:
	CheyetteDD_Model_PTR	cheyetteModel_PTR_ ;
	//VanillaSwaption			vanillaSwaption_ ;  //pricer : ne pas mettre le produit en attribut
	RNGenerator_PTR			rnGenerator_ ;				//classe abstraite
	

public:
	CheyetteDD_VanillaSwaptionMCPricer(	CheyetteDD_Model_PTR cheyetteModel_PTR,
										RNGenerator_PTR rnGenerator);

	virtual ~CheyetteDD_VanillaSwaptionMCPricer(void){}

	double TrajectoireEuler(size_t nbPas) const ;
	
	//! Pricing at time T0 = 0
	double price(const VanillaSwaption& vanillaSwaption, size_t nbSimulation, size_t nbPas)  const ;
	
};

typedef boost::shared_ptr<CheyetteDD_VanillaSwaptionMCPricer> CheyetteDD_VanillaSwaptionMCPricer_PTR;
typedef boost::shared_ptr<const CheyetteDD_VanillaSwaptionMCPricer> CheyetteDD_VanillaSwaptionMCPricer_CONSTPTR;


