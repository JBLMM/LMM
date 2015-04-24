//#include <iostream>
//#include <fstream>
//#include "LmmCapletPricer.h"
//#include "NumericalMethods.h"
//
//using namespace std;
//
//
//template<class T>
//void gen_normal(T & generator, std::vector<Real> &res)
//{
//	for(size_t i = 0; i < res.size(); ++i)
//		res[i] = generator();
//}
//
//
//
//LmmCapletPricer::LmmCapletPricer(const Caplet& caplet, McLmm& lmmModel)
//{
//	caplet_ = caplet;
//	lmmModel_ = &lmmModel;
//}
//
//
//double LmmCapletPricer::capletNpv(const std::vector<std::vector<double>> &libors)
//{	
//	double tenor = caplet_.get_tenor();
//	size_t index_caplet = caplet_.get_liborIndex();
//	double libor = libors[index_caplet][index_caplet];
//	double strike = caplet_.get_strike();
//
//	return tenor*std::max(libor-strike,0.);
//}
//
////void LmmCapletPricer::McCapletPrice_Terminal_Euler(
////	                                               size_t nbSimulations,
////												   double& out_mcPrice, 
////											       double& out_mcStdErr,
////												   RNG_Type & generator, 
////												   const std::vector<double>& libors_T0,
////												   const std::vector<std::vector<std::vector<double>>>& covTensor)
////{
////
////	size_t N = lmmModel_->getHorizon();
////
////	//-- MC loop
////	out_mcPrice = 0.; 
////	out_mcStdErr = 0.;
////	for (size_t simulIndex = 0; simulIndex < nbSimulations; ++simulIndex) 
////	{
////		//-- Shifted Libor simulation
////		//lmmModel_.TerminalLmmModel_Euler();
////		lmmModel_->TerminalLmmSimulation(euler);
////
////		matrix_ liborsMatrix = lmmModel_->getLiborMatrix();
////
////		//-- Compute numeraire denominator (P(Ti+1,Tn+1))
////		size_t index_caplet = caplet_.get_liborIndex();
////		double numeraire_denom = lmmModel_->bondPrice_i_j(index_caplet+1,N+1);
////		
////		double x = capletNpv(liborsMatrix)/numeraire_denom;
////		out_mcPrice += x;
////		out_mcStdErr += x*x;
////	}
////
////	double numeraire_numer = lmmModel_->bondPrice_i_j(0,N+1);
////	out_mcPrice *= numeraire_numer/nbSimulations;
////
////	out_mcStdErr /= (nbSimulations-1);
////	out_mcStdErr *= (numeraire_numer * numeraire_numer);
////	out_mcStdErr -= (((Real)nbSimulations)/(nbSimulations-1))*out_mcPrice*out_mcPrice;
////	out_mcStdErr = sqrt(out_mcStdErr);
////}
////
////void LmmCapletPricer::McCapletPrice_Terminal_Pc(
////							                    size_t nbSimulations,
////							                    double& out_mcPrice, 
////							                    double& out_mcStdErr,
////							                    RNG_Type & generator, 
////							                    const std::vector<double>& libors_T0,
////							                    const std::vector<std::vector<std::vector<double>>>& covTensor)
////{
////	size_t N = lmmModel_->getHorizon();
////
////	//-- MC loop
////	out_mcPrice = 0.; 
////	out_mcStdErr = 0.;
////
////	for (size_t simulIndex = 0; simulIndex < nbSimulations; ++simulIndex) 
////	{
////		//-- Libor simulation
////		//lmmModel_.TerminalLmmModel_Pc();
////		lmmModel_->TerminalLmmSimulation(ipc);
////
////		matrix_ liborsMatrix = lmmModel_->getLiborMatrix();
////
////		//-- Compute numeraire denominator (P(Ti+1,Tn+1))
////		size_t index_caplet = caplet_.get_liborIndex();
////		double numeraire_denom = lmmModel_->bondPrice_i_j(index_caplet+1,N+1);
////	
////		double x = capletNpv(liborsMatrix)/numeraire_denom;
////		out_mcPrice += x;
////		out_mcStdErr += x*x;
////	}
////
////	double numeraire_numer = lmmModel_->bondPrice_i_j(0,N+1);
////	out_mcPrice *= numeraire_numer/nbSimulations;
////
////	out_mcStdErr /= (nbSimulations-1);
////	out_mcStdErr *= (numeraire_numer * numeraire_numer);
////	out_mcStdErr -= (((Real)nbSimulations)/(nbSimulations-1))*out_mcPrice*out_mcPrice;
////	out_mcStdErr = sqrt(out_mcStdErr);
////
////}
////
////void LmmCapletPricer::McCapletPrice_Spot_Euler(
////							                   size_t nbSimulations,
////							                   double& out_mcPrice, 
////							                   double& out_mcStdErr,
////							                   RNG_Type & generator, 
////							                   const std::vector<double>& libors_T0,
////							                   const std::vector<std::vector<std::vector<double>>>& covTensor)
////{
////
////	size_t N = lmmModel_->getHorizon();
////	
////	//-- MC loop
////	out_mcPrice = 0.; 
////	out_mcStdErr = 0.;
////
////	for (size_t simulIndex = 0; simulIndex < nbSimulations; ++simulIndex) 
////	{
////		//-- Libor simulation
////		//lmmModel_.SpotLmmModel_euler();
////		lmmModel_->SpotLmmSimulation(euler);
////		
////		matrix_ liborsMatrix = lmmModel_->getLiborMatrix();
////
////		//-- Compute numeraire Bd(Ti+1)
////		size_t index_caplet = caplet_.get_liborIndex(); 
////		double numeraire = lmmModel_->Spot_numeraire(index_caplet+1,liborsMatrix);
////		
////		double x = capletNpv(liborsMatrix)/numeraire;
////		out_mcPrice += x;
////		out_mcStdErr += x*x;
////	}
////
////	out_mcPrice /= nbSimulations;
////
////	out_mcStdErr /= (nbSimulations-1);
////	out_mcStdErr -= (((Real)nbSimulations)/(nbSimulations-1))*out_mcPrice*out_mcPrice;
////	out_mcStdErr = sqrt(out_mcStdErr);
////}
////
////void LmmCapletPricer::McCapletPrice_Spot_Pc(
////						                    size_t nbSimulations,
////						                    double& out_mcPrice, 
////						                    double& out_mcStdErr,
////						                    RNG_Type & generator, 
////						                    const std::vector<double>& libors_T0,
////						                    const std::vector<std::vector<std::vector<double>>>& covTensor)
////{
////	size_t N = lmmModel_->getHorizon();
////	
////	//-- MC loop
////	out_mcPrice = 0.; 
////	out_mcStdErr = 0.;
////
////	for (size_t simulIndex = 0; simulIndex < nbSimulations; ++simulIndex) 
////	{
////		//-- Libor simulation
////		//lmmModel_.SpotLmmModel_pc();
////		lmmModel_->SpotLmmSimulation(ipc);
////
////		matrix_ liborsMatrix = lmmModel_->getLiborMatrix();
////
////		//-- Compute numeraire Bd(Ti+1)
////		size_t index_caplet = caplet_.get_liborIndex(); 
////		double numeraire = lmmModel_->Spot_numeraire(index_caplet+1,liborsMatrix);
////
////		double x = capletNpv(liborsMatrix)/numeraire;
////		out_mcPrice += x;
////		out_mcStdErr += x*x;
////	}
////
////	out_mcPrice /= nbSimulations;
////
////	out_mcStdErr /= (nbSimulations-1);
////	out_mcStdErr -= (((Real)nbSimulations)/(nbSimulations-1))*out_mcPrice*out_mcPrice;
////	out_mcStdErr = sqrt(out_mcStdErr);
////}
//
//
//void LmmCapletPricer::McCapletPrice(size_t nbSimulations,
//								    double& out_mcPrice, 
//								    double& out_mcStdErr,
//								    const std::vector<double>& libors_T0,
//								    ApproxType approx)
//{
//	size_t N = lmmModel_->getHorizon();
//	
//	out_mcPrice = 0.; 
//	out_mcStdErr = 0.;
//
//	//-- MC loop
//	for (size_t simulIndex = 0; simulIndex < nbSimulations; ++simulIndex) 
//	{
//		//-- Libor simulation
//		lmmModel_->LmmSimulation(approx);
//		lmmModel_->computeNumeraires();
//
//		matrix_ liborsMatrix = lmmModel_->getLiborMatrix();
//
//		//-- Compute numeraire Bd(Ti+1)
//		size_t index_caplet = caplet_.get_liborIndex(); 
//
//		Real numeraire_pricing_date = lmmModel_->getNumeraire(0);
//		Real numeraire_maturity_date = lmmModel_->getNumeraire(index_caplet+1);
//
//		double showNpv = capletNpv(liborsMatrix); // TEST
//
//		double x = (numeraire_pricing_date/numeraire_maturity_date) * capletNpv(liborsMatrix);
//		out_mcPrice += x;
//		out_mcStdErr += x*x;
//	}
//
//	out_mcPrice /= nbSimulations;
//
//	out_mcStdErr /= (nbSimulations-1);
//	out_mcStdErr -= (((Real)nbSimulations)/(nbSimulations-1))*out_mcPrice*out_mcPrice;
//	out_mcStdErr = sqrt(out_mcStdErr);
//}
//
//
//Caplet& LmmCapletPricer::get_caplet() {return caplet_;}
//
//void LmmCapletPricer::set_caplet(const Caplet& caplet) {caplet_=caplet;}
//
//
////-------------------------------------------------------------//
////                            TESTS                            //
////-------------------------------------------------------------//
//
//void LmmCapletPricer::test_computeCapletPrices()
//{
//	size_t nbSimulations = 50000;
//
//	size_t N_libor = lmmModel_->getHorizon();
//	std::vector<double> tenorDates = lmmModel_->getTenorDates();
//	std::vector<double> tenorPeriods = lmmModel_->getDeltaT();
//	std::vector<double> liborShifts = lmmModel_->getShifts();
//
//	//-- Get libors a time 0
//	std::vector<double> libors_0(N_libor+1);
//	for (size_t i = 0; i < N_libor+1; ++i)
//		libors_0[i] = lmmModel_->getLiborMatrix()[i][0];
//	
//	double mcPrice = 9999999.99999;
//	double mcErr = 9999999.99999;
//
//	ofstream outputFile;
//	outputFile.open("caplet_spot.csv");
//
//	outputFile << "Caplet prices under terminal measure - euler approximation" << endl << endl;
//	outputFile << "Number of simulations;" << nbSimulations << ";" << endl << endl;
//	outputFile << "index" << ";" << "Black Price;";
//	outputFile << "MC Price (Euler);" << "CI width (Euler);" << "Relative error (Euler);";
//	outputFile << "MC Price (PC);" << "CI width (PC);" << "Relative error (PC);" << endl;
//
//	//-- Init Black pricer
//	NumericalMethods blackPricer;
//
//	//-- Creates every caplet on the time grid
//	for (size_t i = 1; i < N_libor+1; ++i)
//	{
//		double K = libors_0[i]; // ATM Strike
//		double tenorLibor = tenorPeriods[i];
//		Caplet ith_caplet(tenorDates[i+1],tenorLibor,K,i); 
//		set_caplet(ith_caplet);
//
//		McCapletPrice(nbSimulations,mcPrice,mcErr,libors_0,euler); // Simulations with Euler stepping
//
//		double ciWidth = mcErr*3/sqrt((double)nbSimulations);
//
//		//------------------------------------------------------//
//		//                       BLACK                          //
//		//------------------------------------------------------//
//
//		double sigma = (lmmModel_->getVolMod())->varIntegral(i,tenorDates[i],0,tenorDates[i]);
//
//		//-- Compute numeraire P(0,T_index_caplet+1) (numeraire of the T_index_caplet_+1 forward measure)
//		double numeraire_0 = 1.;
//		for (size_t k = 0; k < i+1; ++k)
//			numeraire_0 /= (1 + tenorPeriods[k]*libors_0[k]);
//
//		double discount = tenorPeriods[i]*numeraire_0;
//
//		double shifted_libor_0 = libors_0[i] + liborShifts[i];
//		double shifted_strike = K + liborShifts[i];
//
//		double blackCapletPrice = blackPricer.BlackFormula(shifted_libor_0,shifted_strike,sigma,discount);
//
//		//------------------------------------------------------//
//		//------------------------------------------------------//
//
//		outputFile << i << ";";
//		outputFile << blackCapletPrice << ";";
//		outputFile << mcPrice << ";";
//		outputFile << ciWidth << ";";
//		outputFile <<  abs(blackCapletPrice-mcPrice)/blackCapletPrice << ";";
//
//		McCapletPrice(nbSimulations,mcPrice,mcErr,libors_0,ipc); // Simulations with predictor-corrector
//		ciWidth = mcErr*3/sqrt((double)nbSimulations);
//		outputFile << mcPrice << ";";
//		outputFile << ciWidth << ";";
//		outputFile <<  abs(blackCapletPrice-mcPrice)/blackCapletPrice << ";" << endl;
//	}
//
//	outputFile.close();
//	cout << "END OF WRITING" << endl;
//}
//
