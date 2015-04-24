#pragma once

#include <LMM/instrument/VanillaSwap.h> 
#include <LMM/helper/LMMTenorStructure.h>

class LmmVanillaSwapPricer
{

public:
	//! constructor
	LmmVanillaSwapPricer(LMMTenorStructure_PTR lmmTenorStructure);
	//! destructor
	virtual ~LmmVanillaSwapPricer();

	//! To validate the result
	// initLibor[i] = L_i[T_0]
	double swapNPV_Analytical_1(const VanillaSwap& vanillaSwap, const std::vector<double>& liborsInitValue)  const;

	// initLibor[i] = L_i[T_0]
	double swapNPV_Analytical_2(const VanillaSwap& vanillaSwap, const std::vector<double>& liborsInitValue)  const;

	double swapRate_Analytical(const VanillaSwap& vanillaSwap,  const std::vector<double>& liborsInitValue)  const;

	virtual double annuity( const LMM::Index  indexValuationDate, // used for  MC pricing
		const VanillaSwap    & vanillaSwap,
		const std::vector<double> & numeraire
		) const;

	virtual double annuity0(const VanillaSwap& vanillaSwap,  // pricing at date 0, price is indp of model, only use liborsInitValue
							const std::vector<double>& liborsInitValue)  const;

	virtual double omega0(LMM::Index liborIndex, const double& annuity_T0, const std::vector<double>& bonds_T0) const;

protected: 

	virtual double forwardSwapRate( 
		const LMM::Index            indexValuationDate,
		const VanillaSwap    & vanillaSwap,
		const std::vector<double> & numeraire
		) const;

	virtual double pvFixedLeg(
		const LMM::Index            indexValuationDate,
		const VanillaSwap    & vanillaSwap,
		const std::vector<double> & numeraire
		)  const;

	//! YY TODO: maybe move it here, as the fixing leg ...
	//double pvFloatingLeg(LMM::Index indexValuationDate,
	//					const VanillaSwap& vanillaSwap,
	//					const std::vector<double>& numeraire, //numeraire[i]: numeraire value at T_i
	//					const matrix& liborMatrix) const;

protected:

	LMMTenorStructure_PTR pLMMTenorStructure_;	
};

typedef boost::shared_ptr<LmmVanillaSwapPricer> LmmVanillaSwapPricer_PTR;

