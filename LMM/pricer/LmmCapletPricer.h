//#pragma once
//
//#include "McLmm.h"
//#include "Caplet.h"
//
//class LmmCapletPricer
//{
//private:
//	Caplet caplet_;
//	McLmm* lmmModel_;  // YY shit code (a pointer here ?) you are joking me ? 
//
//public:
//	LmmCapletPricer(const Caplet& caplet, McLmm& lmmModel);
//	double capletNpv(const std::vector<std::vector<double>> &libors);
//	
////void McCapletPrice_Terminal_Euler(
////								  size_t nbSimulations,
////								  double& out_mcPrice, 
////								  double& out_mcStdErr,
////								  RNG_Type & generator, 
////								  const std::vector<double>& libors_T0,
////								  const std::vector<std::vector<std::vector<double>>>& covTensor);
////
////void McCapletPrice_Terminal_Pc(
////							   size_t nbSimulations,
////							   double& out_mcPrice, 
////							   double& out_mcStdErr,
////							   RNG_Type & generator, 
////							   const std::vector<double>& libors_T0,
////							   const std::vector<std::vector<std::vector<double>>>& covTensor);
////
////void McCapletPrice_Spot_Euler(
////							  size_t nbSimulations,
////							  double& out_mcPrice, 
////							  double& out_mcStdErr,
////							  RNG_Type & generator, 
////							  const std::vector<double>& libors_T0,
////							  const std::vector<std::vector<std::vector<double>>>& covTensor);
////
////void McCapletPrice_Spot_Pc(
////						   size_t nbSimulations,
////						   double& out_mcPrice, 
////						   double& out_mcStdErr,
////						   RNG_Type & generator, 
////						   const std::vector<double>& libors_T0,
////						   const std::vector<std::vector<std::vector<double>>>& covTensor);
//
//void McCapletPrice(size_t nbSimulations,
//			       double& out_mcPrice, 
//				   double& out_mcStdErr,
//				   const std::vector<double>& libors_T0,
//				   ApproxType approx);
//
//
//Caplet& get_caplet();
//void set_caplet(const Caplet& caplet);
//
//void test_computeCapletPrices();
//
//};
