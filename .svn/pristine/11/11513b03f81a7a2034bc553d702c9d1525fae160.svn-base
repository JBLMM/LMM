//#pragma once
//#include <iostream>
//#include "McLmm.h"
//#include "Swaption.h"
//#include "Swap.h"
//
//class TestPricing {
//
//private:
//
//	Real   maturity; // Swaption maturity (in years)
//	size_t tenor; // Swap tenor lenght (in years)
//	size_t maturityIndex; // index of the date corresponding to the swaption's maturity
//	Time   maturityPeriod; // period length for swaption maturity
//	Time   tenorPeriod; // period length for swap tenor
//	size_t N; // index of the last tenor date (taking into account ALL dates)
//	size_t M; // Number of MC simulations
//	
//	// Volatility model 
//	numeric::ublas::vector<Real> abcdParams;
//	matrix<Real> B; // matrix obtained by correlation matrix decomposition
//
//	Real tenorLibor;
//
//	McLmm* model; // the Forward LIBOR model
//
//	Swaption_ *product; // the swaption to price
//	numeric::ublas::vector<Time> swapTenors; // underlying swap's tenor dates
//	matrix<Real> eulerConvergenceTable; 
//	matrix<Real> ipcConvergenceTable;
//
//
//public:
//
//	TestPricing(		// Libor params
//						  size_t N_libor,
//						  size_t nbFactors,
//						  double tenor_libor, 
//						  const matrix<Real>& B,
//						  // vol params
//						  const numeric::ublas::vector<Real>& abcd,
//						  // Swap params 
//						  Real maturity, 
//						  size_t tenors, 
//						  size_t fixPer,
//						  size_t floatPer,
//						  //Time maturityPeriod, 
//						  // MC 
//						  size_t M, 
//						  // payoff 
//						  Rate K );
//	~TestPricing();
//
//	// t_k Bond price at time t_i
//	Real bondPrice(size_t i, size_t k, const matrix<Rate> &lmm);
//	Real Spot_numeraire(size_t k, const matrix<Rate>& lmm);
//
//	void McEulerTerminalPrice(Real &mcPrice, Real &mcErr, RNG_Type & generator);
//	void McIpcTerminalPrice(Real &mcPrice, Real &mcErr, RNG_Type & generator);
//
//	void McEulerSpotPrice(Real &mcPrice, Real &mcErr, RNG_Type & generator);
//	void McIpcSpotPrice(Real &mcPrice, Real &mcErr, RNG_Type & generator);
//
//	void RebonatoPrice(Real &Price);
//	void AndersenPrice(Real &Price);
//
//	Real BlackCaplet(size_t i);
//	void McForwardCaplet(size_t i, Real & mcPrice, Real & mcErr, RNG_Type & generator);
//	void McTerminalCaplet(Real & mcPrice, Real & mcErr, size_t index_libor, RNG_Type & generator);
//	void McTerminalCaplet2(Real & mcPrice, Real & mcErr, size_t index_libor, RNG_Type & generator);
//	void McSpotCaplet(Real & mcPrice, Real & mcErr, size_t index_libor, RNG_Type & generator);
//
//
//	void McTerminalEulerSwaptionPrice(Real &mcPrice, 
//		                              Real &mcStdErr, 
//									  SwapInstruments::VanillaSwap& swap,
//									  SwapInstruments::VanillaSwaption& swaption,
//									  RNG_Type & generator,
//									  const std::vector<double>& libors_T0,
//									  const numeric::ublas::vector<matrix<Real>>& covTensor);
//
//	void McTerminalPcSwaptionPrice(Real &mcPrice, 
//		                           Real &mcStdErr, 
//								   SwapInstruments::VanillaSwap& swap,
//								   SwapInstruments::VanillaSwaption& swaption,
//								   RNG_Type & generator,
//								   const std::vector<double>& libors_T0,
//								   const numeric::ublas::vector<matrix<Real>>& covTensor);
//
//	void McSpotEulerSwaptionPrice(Real &mcPrice, 
//		                          Real &mcStdErr, 
//								  SwapInstruments::VanillaSwap& swap,
//								  SwapInstruments::VanillaSwaption& swaption,
//								  RNG_Type & generator,
//								  const std::vector<double>& libors_T0,
//								  const numeric::ublas::vector<matrix<Real>>& covTensor);
//
//	void McSpotPcSwaptionPrice(Real &mcPrice, 
//		                       Real &mcStdErr, 
//							   SwapInstruments::VanillaSwap& swap,
//							   SwapInstruments::VanillaSwaption& swaption,
//							   RNG_Type & generator,
//							   const std::vector<double>& libors_T0,
//							   const numeric::ublas::vector<matrix<Real>>& covTensor);
//};
//
