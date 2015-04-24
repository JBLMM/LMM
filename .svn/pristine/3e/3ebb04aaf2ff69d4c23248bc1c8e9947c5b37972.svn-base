//#pragma once
//
//#include "Product.h"
//#include "DeterministicVol.h"
//#include "SwapModelMc.h"
//
//#include <vector>
////using namespace std;
//// Enum type to select the probability under which computing LMM rates
//enum Proba {spot, terminal};
//
//// Enum type for vol approximation method 
//enum VolApproxType { rebonato, andersen };
//
//**
// * @brief Swaption class
// */
//class Swaption_ : public Product {
//
//private:
//
//
//	/// Swaption's last tenor index
//	size_t N;              // TN
//
//	/// Swaption's maturity (in years)
//	size_t maturity_index; // T0
//
//	/// Underlying swap's tenor length (in years)
//	size_t tenor;
//
//	/// The underlying swap's reset dates
//	numeric::ublas::vector<Time> tenorDates; // TODO : Replace by dates given by tenor structure
//
//	// TODO : replace tenor & tenorDates by tenor structure
//	SwapModelMc tenorStruct;
//
//	/// fraction between fixing and floating periods 
//	size_t diffPer;
//
//	/// The underlying's strike
//	QuantLib::Real K; 
//	
//	/// The volatility structure employed for Libor rates
//	DeterministicVol *myVolMod;
//
//	std::vector<double> fixingSchedule;
//	std::vector<double> floatingSchedule;
//
//public:
//
//	/**
//	 * @brief Swaption constructor with parameters
//	 * @param swapRate_T0 swap rate value at first tenor date
//	 * @param libor_T0 (known) vector of libor rates at first tenor dates
//	 * @param myLmm LMM model used to compute swaption's prices
//	 * @param K swpation's strike
//	 * @param G a gaussian vector (Brownian motion under Swap measure)
//	 * @param absAcc absolute accuracy to determine when to stop integration
//	 * @param maxEval number of maximum evaluations above which the integration has to stop
//	 */
//	Swaption_(size_t maturity, size_t last_index, size_t tenor, size_t fixPer, size_t floatPer, Real K, numeric::ublas::vector<Time> tenorDates, size_t nbFactors, matrix<Real> B, const std::vector<Real> &abcd);
//	
//	Swaption_(const Swaption_ & prod);
//
//	~Swaption_();
//
//
//	/**
//	 * @brief Compute a T-zero-coupon bond price at time t
//	 * @param r instantaneous spot rate
//	 * @param t date at which we compute the T-bond (t <= T)
//	 * @param T the bond's maturity
//	 * @returns the T-bond's price at time t
//	 */
//	Real bondPrice(Rate r, Time t, Time T);
//
//
//	Real bondPrice_0(Rate r, Time T);
//
//	/**
//	 * @brief Compute a Ti libor rate at time t=0
//	 */
//	Real libor_0(size_t i);
//
//	Real annuity_0();
//
//	Real swapRate_0();
//
//	// k is the index such that T_k is the bond's maturity
//	Real bondPrice_T0(size_t k, const matrix<Rate> &lmm);
//
//	// bond price computed with fixing tenor dates (for fixing leg)
//	Real bondPriceFixing_T0(size_t k, const matrix<Rate> &lmm);
//
//	Real pvFixing(const numeric::ublas::vector<Real> &bonds);
//
//	// subLmm is a submatrix of the Libor matrix, starting at T0 = maturity date
//	Real pvFloatting(const matrix<Rate> &subLmm, const numeric::ublas::vector<Real> &bonds);
//
//	/**
//	 * @brief Swaption payoff
//	 * @returns the swaption's payoff
//	 */
//	virtual Real payoff(const matrix<Rate> &lmm, const numeric::ublas::vector<Real> &bonds);
//
//	/**
//	 * @brief Compute Swap rate at time T0 (first tenor date)
//	 */
//	Rate swapRate_T0();
//
//	/**
//	 * @brief Compute the i-th omega function for Rebonato's approximation
//	 * @param i the index of the omega coefficient
//	 * @returns the i-th omega coefficient
//	 */
//	Real omega0(size_t i);
//
//	Real omegaAndersen0(size_t i);
//
//	/**
//	 * @brief Computes volatility of the swap rate dynamics
//	 * @param t date at which we compute the volatility
//	 * @param S0 the swap rate's value at time t=0 (take 2 or 2.5%)
//	 * @param G au gaussian variable
//	 * @returns the swap rate's volatility
//	 */
//	Volatility sigmaARebonato(Time t, Rate S0, Real G, Real a, Real b, Real c, Real d);
//
//
//	Real ReboVolInt(Time S, Time T, Rate S0, Real a, Real b, Real c, Real d);
//
//	/**
//	 * @brief Compute the swap rate's value at time t
//	 * @param t the date at which we consider the swap rate
//	 * @param G a gaussian variable
//	 * @returns the swap rate at time t
//	 */
//	Rate swapRate(Time t, Real G, Real a, Real b, Real c, Real d);
//	
//	/**
//	 * @brief Compute omega coefficients at time 0 for Andersen's approximation
//	 * @param bondPrices_0 bond prices at time 0
//	 * @returns Rebonato's omega coefficients at time 0
//	 */
//	numeric::ublas::vector<Real> omegaA_0(numeric::ublas::vector<Real> bondPrices_0);
//
//
//	/** 
//	 * @brief Compute derivative of a bond price with maturity Tk in regards with Li, at time t=0
//	 * @param k index of the bond's maturity date
//	 * @param i index of the rate with respect to which we derivate the bond price 
//	 * @param bond_T0 price of the bond with first tenor date as maturity, at time T_index
//	 * @returns the derivative of the Tk-bond 
//	 */
//	Real bondPriceDerivative_0(size_t k, size_t i, Real bond_T0);
//	
//
//	/**
//	 * @brief Compute derivative of omega_k with respect to the i-th LIBOR rate, at time t=0 
//	 * @param k index of omega
//	 * @param i index of the LIBOR rate with respect to which we derivate
//	 * @param index index of the tenor date at which we compute the derivative
//	 * @returns omega-k's derivative
//	 */
//	Real omega_k_deriv_0(size_t k, size_t i, Real bond_i);
//
//	Real annuity(Time t);
//
//	// getters //
//	Real getK();
//
//	// returns the fraction between fixing and floating periods
//	size_t getDiffPer();
//
//	// returns number of fixing dates
//	size_t getFixingDateSize();
//
//	// returns number of floating dates
//	size_t getFloatingDateSize();
//
//	const numeric::ublas::vector<Real> &getFixingDates();
//
//	const numeric::ublas::vector<Real> &getFloatingDates();
//};
