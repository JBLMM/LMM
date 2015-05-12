//#pragma once
//
//#include <cassert>
//
//#include <boost/shared_ptr.hpp>
//
//#include <LMM/numeric/NumericalMethods.h>
//#include <LMM/instrument/VanillaSwaption.h>
//#include <LMM/LmmModel/Lmm.h>
//#include <LMM/LmmModel/ShiftedLmm.h>
//#include <LMM/LmmModel/Dispersion.h>
//#include <LMM/pricer/LmmVanillaSwapPricer.h>
//#include <LMM/LmmModel/Shifted_HGVolatilityFunction.h>
//
//
///*! \class LmmApproxVanillaSwaptionPricer_Piterbarg
//*  Defining approx methods for swaption pricing
//*  to use the pre-calculation.
//*/
//
////YY: terminal prob => N+1
//class LmmVanillaSwaptionApproxPricer_Rebonato : public LmmVanillaSwapPricer  // YY: TODO this derivation is not good, need to change ... 
//{
//public:
//	ChyetteModel_ cheyetteModel_;
//	
//
//public:
//
//	double volBlack(const VanillaSwaption& vanillaSwaption, const std::vector<double>& liborsInitValue) const;
//	double price(const VanillaSwaption & swaption, const std::vector<double> & libor_init )const;
//
//	//! pre-calcule
//	void initializeCache();  // ?? not for first implementation. 
//
//private:
//
//	Lmm_PTR lmm_;
//	//Shifted_HGVolatilityFunction_PTR shifted_HGVolatilityFunction_; // Sorry, cannot do the casting
//
//	//TODO : delete every buffer and put them into functions parameters
//	mutable std::vector<double> buffer_liborAtExpensionInit;   // fixed L_i(t) for approximation purpose.
//	mutable std::vector<double> buffer_ZCAtExpensionInit;      // buffer_ZC[i] = P(T_0, T_i), so buffer_ZC[0] = 1.0
//	//mutable std::vector<double> buffer_numeraire		 ;     // buffer_numeraire[i] = 1/buffer_ZC[i]
//	mutable std::vector<double> buffer_omegaAtExpensionInit;   // omega_[i] = deltaT_floating[i] P(T_0,T_{i+1}) / annuity
//	mutable double              buffer_anuityAtExpensionInit;  // 
//	std::vector<double>         constShifts_;
//
//
//private:
//
//	//! pre-calculated has to do in this order : preCalculateLiborExpensionInit --> ZeroCoupon --> Anuities --> Omegas
//	//! liborExpensionInit can be different as initLibor: esp taken as: E^{Q^A} (L(t)) 
//
//	void preCalculateLiborAtExpensionInit  (const std::vector<double> & liborsInitValue, const VanillaSwaption& vanillaswaption ) const ;
//	void preCalculateZCAtExpensionInit     () const ;
//	void preCalculateAnuityAtExpensionInit (const VanillaSwaption& vanillaswaption ) const ;
//	void preCalculateOmegaAtExpensionInit  (const VanillaSwaption& vanillaswaption ) const ;	
//	void preCalculateALL                   (const std::vector<double> & liborsInitValue, const VanillaSwaption& vanillaswaption ) const ; //TODO this method to be public
//
//};
//
//typedef boost::shared_ptr<LmmVanillaSwaptionApproxPricer_Rebonato> LmmVanillaSwaptionApproxPricer_Rebonato_PTR;
//typedef boost::shared_ptr<const LmmVanillaSwaptionApproxPricer_Rebonato> LmmVanillaSwaptionApproxPricer_Rebonato_CONSTPTR;