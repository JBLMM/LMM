//#pragma once 
//
//#include "Swaption.h"
//
///**
// * @brief Derivative pricing class, using Monte-Carlo method
// */
//class McPricer {
//
////private:
////
////	Swaption_ *mySwaption;
////	
////public:
////
////	McPricer(const Swaption_ &prod);
////
////	~McPricer();
////	
////	Real priceSwaption(Time t, Real G, Real a, Real b, Real c, Real d);
////	
////	/** 
////	 * @brief MC derivative pricer with underlyings modelled after LMM
////	 * @param index index of tenor date at which we price (0 means T0 = today)
////	 * @param M number of MC simulations
////	 * @param generator a gaussian random vector generator
////	 * @param bondPrices_0 bond prices at time 0
////	 * @param swapRate_0 swap rates' values at time 0
////	 * @param prodPrice (out) MC price of the derivative
////	 * @param mcError (out) MC estimated error 
////	 */
////	void price(
////		size_t index, size_t M, RNG_Type &generator, 
////		numeric::ublas::vector<Real> bondPrices_0, Real swapRate_0, VolApproxType vat,
////		Real &prodPrice, Real &mcError);
//};
