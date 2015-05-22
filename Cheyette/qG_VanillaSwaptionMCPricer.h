#pragma once
#include <boost/shared_ptr.hpp>

#include "CheyetteDD_Model.h"
#include <LMM\instrument\VanillaSwaption.h>  
#include <LMM\RNGenerator\RNGenerator.h>

//pricing sous Q de la swaption par MC

class qG_VanillaSwaptionMCPricer
{
private:
	CheyetteDD_Model_PTR	cheyetteModel_PTR_ ;
	VanillaSwaption			vanillaSwaption_ ;
	RNGenerator_PTR			rnGenerator_ ;				//classe abstraite
	

public:
	qG_VanillaSwaptionMCPricer(	CheyetteDD_Model_PTR cheyetteModel_PTR,
								VanillaSwaption vanillaSwaption,
								RNGenerator_PTR rnGenerator);
	~qG_VanillaSwaptionMCPricer(void);

	double TrajectoireEuler(int nbPas) const ;
	
	//! Pricing at time T0 = 0
	double price(const VanillaSwaption& vanillaSwaption, size_t nbSimulation, double nbPas)  const ;
};

typedef boost::shared_ptr<qG_VanillaSwaptionMCPricer> qG_VanillaSwaptionMCPricer_PTR;


