#pragma once

#include <JBLMM/Instrument/GeneticSwap.h> 

class GeneticVanillaSwapPricer
{
	//GeneticSwap_PTR geneticVanillaSwap_;

public:
	//! constructor
	//GeneticVanillaSwapPricer(GeneticSwap_PTR geneticVanillaSwap):geneticVanillaSwap_(geneticVanillaSwap){}
	//! destructor
	virtual ~GeneticVanillaSwapPricer(){}	

	//! To validate the result
	// initLibor[i] = L_i[T_0]
	double geneticVanillaSwap_Analytical(GeneticSwap_CONSTPTR geneticVanillaSwap, const std::vector<double>& liborInitValue)const;
	//double geneticVanillaSwap_MC(GeneticSwap_CONSTPTR geneticVanillaSwap, const std::vector<double>& liborInitValue)const;

	//getter
	//GeneticSwap_PTR getGeneticVanillaSwap()const{return geneticVanillaSwap_;}

	//size_t find(double paymentDay, std::vector<double> dateLibor)const;



//protected: 
//
//	virtual double forwardSwapRate( 
//		const LMM::Index            indexValuationDate,
//		const VanillaSwap    & vanillaSwap,
//		const std::vector<double> & numeraire
//		) const;
//
//	virtual double pvFixedLeg(
//		const LMM::Index            indexValuationDate,
//		const VanillaSwap    & vanillaSwap,
//		const std::vector<double> & numeraire
//		)  const;
//
	//! YY TODO: maybe move it here, as the fixing leg ...
	//double pvFloatingLeg(LMM::Index indexValuationDate,
	//					const VanillaSwap& vanillaSwap,
	//					const std::vector<double>& numeraire, //numeraire[i]: numeraire value at T_i
	//					const matrix& liborMatrix) const;

};

typedef boost::shared_ptr<GeneticVanillaSwapPricer> GeneticVanillaSwapPricer_PTR;



