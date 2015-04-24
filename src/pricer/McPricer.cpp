#include <LMM/pricer/McPricer.h>
//
//// Function computing a random gaussian vector
//template<class T>
//void gen_normal(T & generator, numeric::ublas::vector<Real> &res) {
//	for(size_t i = 0; i < res.size(); ++i)
//		res(i) = generator();
//}
//
//McPricer::McPricer(const Swaption_ &prod) {
//	this->mySwaption = new Swaption_(prod);
//}
//
//McPricer::~McPricer() {
//	delete [] mySwaption;
//}
//
//
//Real McPricer::priceSwaption(Time t, Real G, Real a, Real b, Real c, Real d) {
//
//	/*Real swaprate = mySwaption->swapRate(t,G,a,b,c,d);
//	Volatility sigmaA = mySwaption->sigmaARebonato(t,0.025,G,a,b,c,d);
//	Real A = mySwaption->annuity(t);
//	Real price = A * blackFormula(Option::Call,mySwaption->getK(), swaprate, sigmaA);
//	
//	return price;*/
//	return 0;
//}
//
//
//void McPricer::price(
//	size_t index, size_t M, RNG_Type &generator, 
//	numeric::ublas::vector<Real> bondPrices_0, Real swapRate_0, 
//	VolApproxType vat,
//	Real &prodPrice, Real &mcError) {
//
//		//Real tmp, A;
//		//size_t N = (*this->mySwaption).getLiborRates().size1() - 1;
//		//numeric::ublas::vector<Rate> myDates(N+2);
//		//Real tmpPrice = 0;
//		//Real tmpMcErr = 0;
//		//numeric::ublas::vector<Real> rngVector(N+1);
//
//		//// Monte-Carlo simulation
//		//for (size_t i = 0; i < M; ++i) {
//		//	gen_normal(generator, rngVector); // compute a gaussian random vector
//		//	(*this->mySwaption).swapRateDynamics(bondPrices_0, swapRate_0, vat, rngVector); // compute swap rates at tenor dates
//
//		//	std::cout << "swap rate at T0 : " << (*this->mySwaption).getSwapRates()(0) << std::endl;
//		//	std::cout << "strike : " << (*this->mySwaption).getK() << std::endl << std::endl;
//
//		//	tmp = std::max((*this->mySwaption).getSwapRates()(0) - (*this->mySwaption).getK(),0.); // compute payoff
//
//		//	tmpPrice += tmp;
//		//	tmpMcErr += tmp * tmp;
//		//}
//
//		//tmpPrice *= 1./M; 
//
//		//tmpMcErr *= 1./(M-1);
//		//tmpMcErr -= tmpPrice*tmpPrice; 
//		//tmpMcErr *= ((Real)M)/(M-1); 
//		//tmpMcErr = sqrt(tmpMcErr);
//
//		//myDates = (*this->mySwaption).getLmm().getTenorDates();
//		//A = (*this->mySwaption).annuity(index, N+1, myDates, bondPrices_0(index));
//
//		//tmpPrice *= A; 
//		//tmpMcErr *= (A*A);
//		//prodPrice = tmpPrice; // MC estimator
//		//mcError = tmpMcErr; // MC error
//}