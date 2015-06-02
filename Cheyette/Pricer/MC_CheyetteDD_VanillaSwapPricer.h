//#pragma once
//#include <boost/shared_ptr.hpp>
//#include <Cheyette/CheyetteDD_Model.h>
//#include <Cheyette/MC_Cheyette.h>		// Model
//#include <LMM/instrument/VanillaSwap.h>  // Instrument
//#include <LMM/pricer/LmmVanillaSwapPricer.h>
//
//
//class MC_CheyetteDD_VanillaSwapPricer : public LmmVanillaSwapPricer
//{
//protected:
//	MC_Cheyette_PTR mcCheyette_; 
//	LMMTenorStructure_PTR lmmTenorStructure_ ;
//public:
//	MC_CheyetteDD_VanillaSwapPricer(const MC_Cheyette_PTR& mcCheyette, LMMTenorStructure_PTR lmmTenorStructure)
//		: LmmVanillaSwapPricer(lmmTenorStructure), mcCheyette_(mcCheyette){}
//
//	virtual ~MC_CheyetteDD_VanillaSwapPricer(void){}
//
//	void resetGeneratorToinitSeed(){mcCheyette_->get_RNGenerator()->resetGeneratorToinitSeed();}
//
//	//! one simulation
//	//const std::vector<double>& numeraire, //numeraire[i]: numeraire value at T_i
//	double pvFloatingLeg(size_t indexValuationDate,
//						const VanillaSwap& vanillaSwap,
//						const std::vector<double>& xt_Cheyette, 
//						const std::vector<double>& yt_Cheyette) const ;
//
//	double pvFixedLeg(size_t indexValuationDate,
//						const VanillaSwap& vanillaSwap,
//						const std::vector<double>& xt_Cheyette, 
//						const std::vector<double>& yt_Cheyette) const ;
//
//	//! Pricing at time T0=0
//	double swapNPV (const VanillaSwap& vanillaSwap, size_t nbPas, size_t nbSimulation)  const;
//
//	double swapRate(LMM::Index indexValuationDate,
//					const VanillaSwap& vanillaSwap,
//					const std::vector<double>& numeraire, 
//					const std::vector<double>& xtCheyette, 
//					const std::vector<double>& yt_Cheyette) const;
//
//};
//
