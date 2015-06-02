//#include "CheyetteDD_VanillaSwaptionMCPricer.h"
//
//
//CheyetteDD_VanillaSwaptionMCPricer::CheyetteDD_VanillaSwaptionMCPricer(	CheyetteDD_Model_PTR cheyetteModel_PTR,
//																		RNGenerator_PTR rnGenerator)
//					: cheyetteModel_PTR_(cheyetteModel_PTR), rnGenerator_(rnGenerator)
//{}
//
//
//
////generation d'une seule trajectoire
//double CheyetteDD_VanillaSwaptionMCPricer::TrajectoireEuler(size_t nbPas) const
//{
//	std::vector<double>   gaussian_tmp(1,0.); 
//	rnGenerator_->generate(gaussian_tmp);  // generate Gaussian.
//
//	//T0 : date d'entrée dans le swap
//	//double deltaT = T0 / nbPas ;
//	double x(0.), y(0.) ;  //x0, y0
//
//	double k = cheyetteModel_PTR_->get_CheyetteDD_Parameter().k_ ;
//	piecewiseconst_RR_Function m		= cheyetteModel_PTR_->get_CheyetteDD_Parameter().m_ ;
//	piecewiseconst_RR_Function sigma	= cheyetteModel_PTR_->get_CheyetteDD_Parameter().sigma_ ;
//
//	//double sigma_r_t, m_t ;
//	double r0 = 0.03 ;
//
//	//on a x = x(t), y = y(t), on calcule x(t + dt), y(t + dt)
//	for (size_t pas = 1; pas <= nbPas ; ++pas)
//	{ 
//		//m_t = m.evaluate(pas - 1) ;
//		//sigma_r_t = sigma.evaluate(pas-1) * (m_t * x + (1-m_t) * r0) ;
//		//x += (y - k * x) * deltaT + sigma_r_t * sqrt(deltaT) * (gaussian_tmp[pas] - gaussian_tmp[pas - 1]) ;  
//		//y += y + (sigma_r_t * sigma_r_t - 2 * k * y) * deltaT ;
//
//		//... 
//	}
//	std::cout << "f(0,t) non pris en compte dans le r(t) = x(t) + f(0,t)" << std::endl ;
//
//	return 0 ;
//}
//
//double CheyetteDD_VanillaSwaptionMCPricer::price(const VanillaSwaption& vanillaSwaption, size_t nbSimulation, size_t nbPas)  const
//{
//	double result = 0.0;
//	//double variance = 0.0;
//
//	//rnGenerator_->resetGeneratorToinitSeed() ;
//
//	//LMM::Index indexValuationDate = 0;
//	//for(size_t itrSimulation=0; itrSimulation<nbSimulation; ++itrSimulation)
//	//{
//	//	mcLmm_->TrajectoireEuler();  
//
//	//	result				 += npvSwap;
//	//	variance += npvSwap*npvSwap;
//	//}
//	//result   /=nbSimulation; 
//
//
//		//variance = (variance/nbSimulation-result*result);	
//		//	      << " mcScheme=" << MCSchemeType::mcSchemeType2String(mcLmm_->get_mcSchemeType())
//		//	      << ", SwapPriceMC=" << result
//		//		  << ", nbSimualtion=" << nbSimulation 
//		//	      << ",  99% confidential interval = " << 2.57*std::sqrt(variance / nbSimulation) << std::endl;
//	
//
//	return result;
//}
//
