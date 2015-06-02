//#pragma once
//#include <boost/shared_ptr.hpp>
//
//#include "CheyetteDD_Model.h"
//#include <LMM\instrument\VanillaSwaption.h>  
//#include <LMM\RNGenerator\RNGenerator.h>
//
//
//class CheyetteDD_VanillaSwaptionMCPricer
//{
//private:
//	//pricer : ne pas mettre le produit en attribut (pas de swaption en attribut)
//	CheyetteDD_Model_PTR	cheyetteModel_PTR_ ;
//	RNGenerator_PTR			rnGenerator_ ;				//classe abstraite
//	
//
//public:
//	CheyetteDD_VanillaSwaptionMCPricer(	CheyetteDD_Model_PTR cheyetteModel_PTR,
//										RNGenerator_PTR rnGenerator);
//
//	virtual ~CheyetteDD_VanillaSwaptionMCPricer(void){}
//
//	//! Pricing at time T0 = 0
//	double price(const VanillaSwaption& vanillaSwaption, size_t nbSimulation, size_t nbPas)  const ;
//	
//};
//
//typedef boost::shared_ptr<CheyetteDD_VanillaSwaptionMCPricer> CheyetteDD_VanillaSwaptionMCPricer_PTR;
//typedef boost::shared_ptr<const CheyetteDD_VanillaSwaptionMCPricer> CheyetteDD_VanillaSwaptionMCPricer_CONSTPTR;
//
//
