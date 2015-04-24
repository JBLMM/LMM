//#pragma once 
//
//#include "LmmSwaptionPricer.h"
//
////-- Init model parameters
//void setVolatility(size_t fullRank,
//	               size_t reducedRank,
//	               DeterministicVol& vol,
//	               std::vector<Real>& abcdParams,
//				   std::vector<Real>& g);
//
//void setModel(McLmm& model,
//	          double& tenorPeriod,
//			  size_t& floatingTenor,
//			  size_t& fixingTenor,
//			  std::vector<double>& tenorDates,
//			  std::vector<double>& deltaT,
//			  std::vector<double>& liborInit,
//			  std::vector<double>& liborShifts);
//
//void setSwapRate(SwapInstruments::VanillaSwap& swap, 
//	             McLmm& model, 
//			     double& swapRate,
//				 double& annuity,
//				 std::vector<double>& liborInit,
//				 std::vector<double>& bonds_0,
//				 size_t fixingTenor,
//				 size_t indexStart,
//				 size_t indexEnd);
//
//// Method in which we check the swap tenor structure
//void printSwapTenorStructure();
//
//
//// Compute MC swapion price and compare it with Rebonato's price
//// horizon = last libor's index
//// BM = size of the brownian vector
//// indexStart = index of the swap's first date
//// indexEnd = index of the swap's last date
//void pricingSwaptionTest(size_t& nbSimulations, 
//	                     size_t horizon, 
//	                     size_t BM, 
//						 std::vector<double>& tenorDates,
//						 std::vector<double>& liborInit, 
//						 size_t indexStart, 
//						 size_t indexEnd);
//
//// Compute a swaption equivalent to a caplet (one rate payed/received) and comapre it to
//// the equivalent caplet's price
//void compareSwaptionWithCaplet();
//
//void printMcResults(double& mcPrice, double& mcErr, size_t nbSimulations);
//
//void printRebonatoResult(double& rebonatoPrice, double& mcPrice);