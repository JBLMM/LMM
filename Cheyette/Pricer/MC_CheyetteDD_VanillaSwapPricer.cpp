//#include <cassert>
//#include <iostream>
//#include <cmath>
//
//#include <Cheyette\MC_CheyetteDD_VanillaSwapPricer.h>
//#include <LMM/helper/LMMTenorStructure.h>
//
//
//
////passage de x_t, y_t à P(t, T, xt, yt), puis Libor(..)
////const std::vector<double>& numeraire, //numeraire[i]: numeraire value at T_i
//double MC_CheyetteDD_VanillaSwapPricer::pvFloatingLeg(size_t indexValuationDate,
//											 const VanillaSwap& vanillaSwap,
//									         const std::vector<double>& xt_Cheyette, 
//											 const std::vector<double>& yt_Cheyette) const
//{
//	assert(indexValuationDate <= vanillaSwap.get_indexStart()); 
//	assert(lmmTenorStructure_->get_horizon() >= vanillaSwap.get_indexEnd());  // if not cannot price this swap;
//
//	double price = 0.0;
//	double P_t_Ti, P_t_Tip1, libor ;
//	const std::vector<LMM::Index>& floatingLegPaymentIndexSchedule = vanillaSwap.get_floatingLegPaymentIndexSchedule();
//	for(size_t itr = 0; itr < floatingLegPaymentIndexSchedule.size(); ++itr)
//	{
//		//! At time T_{i+1}, pay: L_i(T_i)
//		size_t floatingLegPaymentIndex = floatingLegPaymentIndexSchedule[itr]; // i+1
//		
//		//
//		// Attention: this line suppose: MCLMM simulation use the same lmmTenor to simulate libor !!!
//		// condition: floatingTenor = lmmTenor
//		size_t indexLibor = floatingLegPaymentIndex-1; 
//		//size_t indexT     = indexLibor;                                        // = i
//		
//		double x_t		= xt_Cheyette[indexLibor] ;
//		double y_t		= yt_Cheyette[indexLibor] ;
//		P_t_Ti			= P(indexValuationDate, 
//		P_t_Tip1		= P(
////		double libor	= mcCheyette_->get_Cheyette()->Libor(indexValuationDate, T1, T2, x_t, y_t) ;
////		double delta_T	= vanillaSwap.get_DeltaTFloatLeg(itr);
//		price			+= 
//		//price			+= delta_T*libor*numeraire[indexValuationDate]/numeraire[floatingLegPaymentIndex];		
//	}
//	return price;
//}
//
////passage de x_t, y_t à P(t, T, xt, yt), puis Libor(..)
////const std::vector<double>& numeraire, //numeraire[i]: numeraire value at T_i
//double MC_CheyetteDD_VanillaSwapPricer::pvFixedLeg(size_t indexValuationDate,
//											 const VanillaSwap& vanillaSwap,
//									         const std::vector<double>& xt_Cheyette, 
//											 const std::vector<double>& yt_Cheyette) const
//{
//	assert(indexValuationDate <= vanillaSwap.get_indexStart()); 
//	assert(lmmTenorStructure_->get_horizon() >= vanillaSwap.get_indexEnd());  // if not cannot price this swap;
//
//	double price = 0.0;
//	double P_t_Ti, P_t_Tip1, libor ;
//	const std::vector<LMM::Index>& fixedLegPaymentIndexSchedule = vanillaSwap.get_fixedLegPaymentIndexSchedule() ;
//	for(size_t itr = 0; itr < fixedLegPaymentIndexSchedule.size(); ++itr)
//	{
//		//! At time T_{i+1}, pay: L_i(T_i)
//		size_t fixedLegPaymentIndex = fixedLegPaymentIndexSchedule[itr]; // i+1
//		
//		//
//		// Attention: this line suppose: MCLMM simulation use the same lmmTenor to simulate libor !!!
//		// condition: floatingTenor = lmmTenor
//		size_t indexLibor = fixedLegPaymentIndex - 1; 
//		//size_t indexT     = indexLibor;                                        // = i
//		
//		double x_t		= xt_Cheyette[indexLibor] ;
//		double y_t		= yt_Cheyette[indexLibor] ;
//		P_t_T			= P(indexValuationDate, ) ;
//		double delta_T	= vanillaSwap.get_DeltaTFixedLeg() ;
//		price			+= delta_T * P_t_T * K ; 
//		//price			+= delta_T*libor*numeraire[indexValuationDate]/numeraire[floatingLegPaymentIndex];		
//	}
//	return price;
//}
//
//
//double MC_CheyetteDD_VanillaSwapPricer::swapNPV(const VanillaSwap& vanillaSwap, size_t nbPas, size_t nbSimulation)  const
//{
//	double result = 0.0;
////	double variance = 0.0;
//
//	size_t indexValuationDate = 0;
//	std::vector<double> xt_vect = mcCheyette_->get_xt_Cheyette() ;
//	std::vector<double> yt_vect = mcCheyette_->get_yt_Cheyette() ;
//	double xt, yt ;
//
//	for(size_t itrSimulation=0; itrSimulation<nbSimulation; ++itrSimulation)
//	{
//		mcCheyette_->simulateCheyette(nbPas) ;  
//		xt = xt_vect[] ;
//		yt = yt_vect[] ;
//		double npvFloatingLeg = pvFloatingLeg(indexValuationDate, vanillaSwap, mcCheyette_->get_numeraire(), xt, yt);
//		double npvFixedLeg    = pvFixedLeg   (indexValuationDate, vanillaSwap, mcCheyette_->get_numeraire(), xt, yt);
//		double npvSwap		  = npvFloatingLeg - npvFixedLeg;
//		result				 += npvSwap;
////		variance += npvSwap*npvSwap;
//	}
//	result   /=nbSimulation; 
//
///*	if(LMM::DEUBGLMM())
//	{
//		variance = (variance/nbSimulation-result*result);	
//	    std::cout << LMM::NOTIF_MSG 
//			      << " mcScheme=" << MCSchemeType::mcSchemeType2String(mcCheyette_->get_mcSchemeType())
//			      << ", SwapPriceMC=" << result
//				  << ", nbSimualtion=" << nbSimulation 
//			      << ",  99% confidence interval = " << 2.57*std::sqrt(variance / nbSimulation) << std::endl;
//	}*/	
//
//	return result;
//}
//
//
//double MC_CheyetteDD_VanillaSwapPricer:: swapRate(LMM::Index indexValuationDate,
//										 const VanillaSwap& vanillaSwap,
//										 const std::vector<double>& numeraire, 
//										 const std::vector<double>& xt_Cheyette,
//										 const std::vector<double>& yt_Cheyette) const
//{
//	double pvFloating = pvFloatingLeg( indexValuationDate,vanillaSwap,numeraire, xt_Cheyette, yt_Cheyette);
//	double annuity = pvFixedLeg   (indexValuationDate, vanillaSwap, numeraire);
//	return pvFloating / annuity;
//}
