#pragma once

#include <boost/shared_ptr.hpp>

#include <LMM/helper/Name.h>
#include <LMM/instrument/VanillaSwap.h>

/*! ctn todo VanillaSwaption should be an inherited of VanillaSwap
 *
 **/
class VanillaSwaption
{
private:
	VanillaSwap vanillaSwap_;                     // The underlying swap, indexStart is the payment date.
	OptionType::OptionType vanillaSwaptionType_;  // YY TODO: this should be moved to swap class ! 
	
public:

	//! Default constructor is to avoid. It is used only when building a container of Swaptions
	//! The container initialization require a default constructor of elements type
	VanillaSwaption(){}; 

	VanillaSwaption(const VanillaSwap& vanillaSwap , OptionType::OptionType vanillaSwaptionType);

	double get_strike() const {return vanillaSwap_.get_strike();}
	void   set_strike(double strike) {vanillaSwap_.set_strike(strike);}   
	double& get_strikeRef() {return vanillaSwap_.get_strikeRef();} // YY ask chi-tan, to don't use this function ...

	LMM::Index         get_indexMaturity() const {return vanillaSwap_.get_indexStart();}	
	const VanillaSwap& getUnderlyingSwap() const {return vanillaSwap_;}
	VanillaSwap& getUnderlyingSwap_RefNonConst() {return vanillaSwap_;}  // for skew calculation: need to bump strike ...

	double payoff(const double& pvVloatingLeg, const double& pvFixedLeg) const;	
	Tenor  get_lmmTenorStructureTenorType() const {return getUnderlyingSwap().get_lmmTenorStructureTenorType();} // to compare with the pricer's lmm's strucutreTenor.
};

typedef boost::shared_ptr<VanillaSwaption> VanillaSwaption_PTR;


