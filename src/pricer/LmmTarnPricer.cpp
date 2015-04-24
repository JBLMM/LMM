//#include "LmmTarnPricer.h"
//
//LmmTarnPricer::LmmTarnPricer(const Tarn& tarn, McLmm& lmmModel)
//{
//	tarn_ = tarn;
//	lmmModel_ = &lmmModel;
//}
//
//double LmmTarnPricer::structuredCoupon(size_t index_libor,const matrix_& libors)
//{
//	return std::max(tarn_.get_strike() - 2*libors[index_libor][index_libor],0.);
//}
//
////-- Not sure about the indices, must test them!!
//double LmmTarnPricer::currentTotalReturn(size_t index_current_date,const matrix_& libors)
//{
//	size_t indexFirstTenorDate = tarn_.getExoticPaymentIndices()[0];
//	double tenor = ((double)tarn_.get_exoticLegTenor())/12;
//	double couponSum = 0.;
//
//	for (size_t i = indexFirstTenorDate-1; i <= index_current_date; ++i)
//		couponSum += tenor * structuredCoupon(i,libors);
//
//	return couponSum;
//}
//
////-- TODO: Compute cumulated coupons inside, rather 
//double LmmTarnPricer::pvExoticLeg(const std::vector<double>& numeraires,
//		                          const matrix_& libors)
//{
//	double exoTenorPerYear = ((double)tarn_.get_exoticLegTenor())/12;
//	size_t nbFloatingPayments = tarn_.getExoticPaymentIndices().size();
//
//	double pvExoticLeg = 0.;
//	for each (size_t indexPayment in  tarn_.getExoticPaymentIndices())
//	{
//		if (currentTotalReturn(indexPayment-1,libors) >= tarn_.get_target())
//			//return pvExoticLeg * numeraires[tarn_.get_indexStart()];
//			break;
//		
//		pvExoticLeg += exoTenorPerYear * structuredCoupon(indexPayment-1,libors)*(1./numeraires[indexPayment]); 
//	}
//
//	pvExoticLeg *= numeraires[tarn_.get_indexStart()];
//
//	return pvExoticLeg;
//}
//
//double LmmTarnPricer::pvLiborLeg(const std::vector<double>& numeraires,
//		                         const matrix_& libors)
//{
//	double floatTenorPerYear = ((double)tarn_.get_liborLegTenor())/12;
//	size_t nbFloatingPayments = tarn_.getLiborPaymentIndices().size();
//
//	double pvLiborLeg = 0.; 
//	for each (size_t indexPayment in tarn_.getLiborPaymentIndices())
//	{
//		if (currentTotalReturn(indexPayment-1,libors) >= tarn_.get_target())
//			//return pvLiborLeg * numeraires[tarn_.get_indexStart()];
//			break;
//
//		pvLiborLeg += floatTenorPerYear * libors[indexPayment-1][indexPayment-1] / numeraires[indexPayment];
//	}
//
//	pvLiborLeg *= numeraires[tarn_.get_indexStart()];
//
//	return pvLiborLeg;
//}
//
//double LmmTarnPricer::npvTarn(const std::vector<double>& numeraires,
//		                      const matrix_& libors)
//{
//	return pvExoticLeg(numeraires,libors) - pvLiborLeg(numeraires,libors);
//}
//
////-- Not sure about the start index
//void LmmTarnPricer::McTarnPrice(size_t nbSimulations,
//		                        Real &mcPrice, 
//					            Real &mcStdErr, 
//					            const std::vector<double>& libors_T0,
//					            ApproxType approx)
//{
//	size_t N = lmmModel_->getHorizon();
//
//	//-- MC Loop
//	mcPrice = 0.; 
//	mcStdErr = 0.;
//	for (size_t nbSim = 0; nbSim < nbSimulations; ++nbSim)
//	{
//		//-- libor simulation
//		lmmModel_->LmmSimulation(approx);
//
//		//-- numeraire computations
//		lmmModel_->computeNumeraires();
//
//		matrix_ liborsMatrix = lmmModel_->getLiborMatrix();
//		std::vector<double> numeraires = lmmModel_->getNumeraire();
//
//		Real numeraire_pricing_date = lmmModel_->getNumeraire(0);
//		Real numeraire_maturity_date = lmmModel_->getNumeraire(tarn_.get_indexStart());
//
//		double tarnPayoff = (numeraire_pricing_date/numeraire_maturity_date)*npvTarn(numeraires,liborsMatrix);
//
//		mcPrice += tarnPayoff;
//		mcStdErr += tarnPayoff*tarnPayoff;
//	}
//
//	mcPrice /= nbSimulations;
//
//	mcStdErr /= (nbSimulations-1);
//	mcStdErr -= (((Real)nbSimulations)/(nbSimulations-1))*mcPrice*mcPrice;
//	mcStdErr = sqrt(mcStdErr);
//}
