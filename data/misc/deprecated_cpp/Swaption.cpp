//#include "Swaption.h"
//#include <cassert>
//#include <algorithm>
//
//
//
//#pragma region internal functions
//
//
//// Function computing a random gaussian vector
//template<class T>
//void gen_normal(T & generator, numeric::ublas::vector<Real> &res) {
//	for(size_t i = 0; i < res.size(); ++i)
//		res(i) = generator();
//}
//
//#pragma endregion
//
//
//#pragma region constructors
//
//Swaption_::Swaption_(
//	                 size_t maturity_index, 
//					 size_t last_index,
//					 // Swap params
//	                 size_t tenor, 
//					 size_t fixPer, 
//					 size_t floatPer, 
//					 
//					 Real K, 
//					 numeric::ublas::vector<Time> tenorDates, 
//					 size_t nbFactors,
//					 matrix<Real> B, 
//					 const std::vector<Real> &abcd) 
//{
//	
//	this->maturity_index = maturity_index;
//
//	//SwapModelMc tenorStruct(tenor, maturity_index, last_index, floatPer, fixPer);
//
//	// We now have two tenor date vectors : one for fixing, one for floating
//	this->tenorStruct = SwapModelMc(tenor,maturity_index, last_index, floatPer,fixPer);
//
//	this->tenor = tenorStruct.getTenor();
//	this->diffPer = tenorStruct.getFixPer()/tenorStruct.getFloatPer();
//
//	//this->N = maturity + tenor - 1;
//	size_t flSize = tenorStruct.getFloatDates().size(); // number of floating tenor dates
//
//	//this->N = maturity_index + flSize - 2; 
//	this->N = last_index;
//
//	this->K = K;
//	//this->tenorDates = numeric::ublas::vector<Time>(tenor+1);
//	//this->tenorDates = numeric::ublas::vector<Time>(flSize);
//	
//	size_t nbDates = tenorStruct.getFloatDates().size();
//	
//	/*this->tenorDates = numeric::ublas::vector<Time>(nbDates);
//	for (size_t index_date = 0; index_date < nbDates; ++index_date)
//		this->tenorDates(index_date) = tenorStruct.getFloatDates()(index_date);*/
//
//	this->tenorDates = tenorStruct.getFloatDates();
//
//	// TODO : add in cstr if that class is still employed
//	//-- Constant parameters for abcd vol structure
//	std::vector<double> g(N+1);
//	for (size_t i = 0; i < N+1; ++i)
//		g[i] = 1.;
//
//	this->myVolMod = new DeterministicVol(nbFactors,N+1,abcd,g);
//
//	for (size_t floating_index = 0; floating_index < floatingSchedule.size(); ++floating_index)
//		floatingSchedule[floating_index] = maturity_index + floating_index + 1; 
//	
//	for (size_t fixing_index = 0; fixing_index < fixingSchedule.size(); ++fixing_index)
//		floatingSchedule[fixing_index] = maturity_index + (fixing_index +1)*this->diffPer; 
//	
//}
//
//
//Swaption_::Swaption_(const Swaption_ & prod) {
//
//	this->maturity_index = prod.maturity_index;
//	SwapModelMc tenorStruct(prod.tenorStruct);
//
//	this->tenor = prod.tenor;
//	//this->N = this->maturity + this->tenor - 1;
//
//	size_t flSize = tenorStruct.getFloatDates().size();
//	this->N = maturity_index + flSize - 2;
//
//	this->K = prod.K;
//	//this->tenorDates = numeric::ublas::vector<Time>(tenor+1);
//
//	this->tenorDates = numeric::ublas::vector<Time>(flSize);
//	this->tenorDates = prod.tenorDates;
//
//	this->myVolMod = new DeterministicVol(*(prod.myVolMod));
//	this->tenorStruct = SwapModelMc(prod.tenorStruct);
//}
//
//
//Swaption_::~Swaption_() {}
//
//
//#pragma endregion
//
//
//#pragma region methods
//
//
//Real Swaption_::bondPrice_0(Rate r, Time T) {
//	return exp(-r*T);
//}
//
//Real Swaption_::libor_0(size_t i) {
//
//	Time Ti = tenorDates(i); // We take into account all floating dates, which contain fixing dates
//	Time Tj = tenorDates(i+1);
//	Real per = Tj - Ti;
//	Real res = (1./per)*(bondPrice_0(0.02,Ti)/bondPrice_0(0.02,Tj) - 1.);
//	return res;
//}
//
//// Annuity is computed with fixing dates
//Real Swaption_::annuity_0() {
//
//	size_t flSize = tenorStruct.getFloatDates().size();
//	Real per = tenorDates(1) - tenorDates(0);
//	Real tmp = 0;
//	
//	for (size_t k = 0; k < flSize; ++k) {
//		std::cout << "date " << k << " : " << tenorDates(k) << std::endl;
//		tmp += per * bondPrice_0(0.02,tenorDates(k));
//	}
//	
//	return tmp;
//}
//
//Real Swaption_::swapRate_0() {
//
//	size_t last_date_index = tenorDates.size();
//	Time last_date = tenorDates(last_date_index-1);
//	Real num = 1 - bondPrice_0(0.02,last_date);
//	Real den = annuity_0();
//
//	return num/den;
//}
//
//Real Swaption_::bondPrice_T0(size_t k, const matrix<Rate> &lmm) {  // TODO: only do once for each simulation.
//	Real res = 1;
//	Time per = tenorStruct.getFloatPer()/12.;
//
//	for (size_t j = maturity_index; j < k; ++j) { // maturity = T0 
//		/*per = tenorDates(j+1-maturity) - tenorDates(j-maturity);*/
//		res *= 1./(1 + per*lmm(j,maturity_index));
//	}
//	return res;
//}
// 
//// k's greatest value is nb fixing dates - 1
//Real Swaption_::bondPriceFixing_T0(size_t k, const matrix<Rate> &lmm) {  // TODO: only do once for each simulation.
//
//	Time per = tenorStruct.getFixPer()/12.;
//	Real res = 1./(1 + per*lmm(maturity_index,maturity_index));
//
//	// maturity = T0 
//	for (size_t j = maturity_index+1; j < k; ++j) 
//		res *= 1./(1 + per*lmm(j*diffPer,maturity_index));
//	
//	return res;
//}
//
//// TODO : bonds must be computed with bondPriceFixing_T0()
//Real Swaption_::pvFixing(const numeric::ublas::vector<Real> &bonds) {
//	
//	Real fixing = 0;
//	size_t fixSize = this->getFixingDateSize();
//	size_t per = this->tenorStruct.getFixPer()/12.;
//	/*
//	for (size_t k = 0; k < fixSize; ++k) 
//		fixing += per * bonds((k+1)*this->diffPer-1); 
//	*/
//
//	for (size_t k = 0; k < fixingSchedule.size(); ++k) {
//		size_t index = fixingSchedule[k];
//		fixing += this->K * per * bonds(index);
//	}
//	return fixing;
//}
//
//Real Swaption_::pvFloatting(const matrix<Rate> &lmm, const numeric::ublas::vector<Real> &bonds) {
//
//	Real floating = 0;
//	Time per = tenorStruct.getFloatPer()/12.;
//
//	//for (size_t k = maturity_index; k < N; ++k) 
//	//	floating += per * bonds(k) * lmm(k,k);
//	
//	//floatingSchedule
//
//	for (size_t k = 0; k < floatingSchedule.size(); ++k) {
//		size_t index = floatingSchedule[k];
//		floating += per * bonds(index) * lmm(index,index);
//	}
//
//	//	floating += per * bonds(k) * lmm(k,k);
//	
//
//	return floating;
//}
//
//Real Swaption_::payoff(const matrix<Rate> &subLmm, const numeric::ublas::vector<Real> &bonds) {
//
//	//std::cout << "PV Floating : " << pvFloatting(subLmm, bonds) << " -- Pv Fixing : " << pvFixing(bonds) << std::endl;
//
//	Real swapPayoff = pvFloatting(subLmm, bonds) - pvFixing(bonds);
//	return std::max(0.,swapPayoff);
//	return 0.;
//}
//
//Rate Swaption_::swapRate_T0() {
//
//	Time T0 = tenorDates(0);
//	Time TN = tenorDates(N+1);
//	Rate res = 1 - bondPrice_0(0.02,TN);
//	res /= annuity(T0);
//
//	return res;
//}
//
//
//Real Swaption_::omega0(size_t i) {
//
//	Real per = (tenorDates(1) - tenorDates(0));
//	Real res = per * bondPrice_0(0.02,tenorDates(i));
//	Real tmp = annuity_0();
//
//	return res/tmp;
//}
//
//
//Real Swaption_::omegaAndersen0(size_t i) {
//
//	Real res, omegaA;
//	Real num, den, den_tmp, per_k, per_j;
//	Real num_p, den_p, den_p_tmp;
//
//	size_t flSize = tenorStruct.getFloatDates().size();
//	Real per_i = tenorDates(i+1) - tenorDates(i);
//
//	omegaA = 0;
//
//	for (size_t k = 0; k < flSize-1; ++k) {
//
//		// Compute numerator
//		per_k = tenorDates(k+1) - tenorDates(k);
//		num = per_k;
//		for (size_t j = 0; j < k; ++j) {
//			per_j = tenorDates(j+1) - tenorDates(j);
//			num *= 1 / (1 + per_j * libor_0(j));
//		}
//
//		// Compute denominator
//		den = 0;
//		den_tmp = 1;
//		//for (size_t k = 0; k < tenor; ++k) {
//		for (size_t k = 0; k < flSize-1; ++k) {
//
//			per_k = tenorDates(k+1) - tenorDates(k);
//			for (size_t j = 0; j < k; ++j) {
//				per_j = tenorDates(j+1) - tenorDates(j);
//				den_tmp *= 1 / (1 + per_j * libor_0(j));
//			}
//			den += per_k * den_tmp;
//		}
//
//		// Compute denominator's derivative
//		den_p = 0;
//		den_p_tmp = 1;
//
//		for (size_t j = i; j < flSize-1; ++j) {
//
//			per_k = tenorDates(j+1) - tenorDates(j);
//			for (size_t l = 0; l < j; ++l) {
//				per_j = tenorDates(l+1) - tenorDates(l);
//				den_p_tmp *= 1 / (1 + per_j*libor_0(l));
//			}
//			den_p_tmp *= per_k;
//			den_p += den_p_tmp;
//		}
//
//		den_p *= (-per_i)/(1+per_i*libor_0(i));
//
//
//		if (k < i) { // Then numerator is constant
//			res = (-num) * den_p/(den*den);
//		}
//		else { // Compute numerator derivative
//			Real num_p;
//			per_k = tenorDates(k+1) - tenorDates(k);
//			den_p_tmp = per_k;
//
//			for (size_t j = 0; j < k; ++j) {
//				per_j = tenorDates(j+1) - tenorDates(j);
//				den_p_tmp *= 1 / (1+per_j*libor_0(j));
//			}
//
//			num_p = den_p_tmp * (-per_i)/(1+per_i*libor_0(i));
//
//			res  = (num_p * den - num * den_p)/(den*den);
//		}
//
//		res *= libor_0(k);
//		omegaA += res;
//	}
//
//	return omegaA + omega0(i);
//}
//
//Volatility Swaption_::sigmaARebonato(Time t, Rate S0, Real G, Real a, Real b, Real c, Real d) {
//
//	Real res = 0;
//	Real covTmp = 0;
//
//	for (size_t i = 0; i < N+1; ++i) {
//		for (size_t j = 0; j < N+1; ++j) {
//			/*covTmp = (*myVolMod).abcdCov(i,j,tenorDates(i),tenorDates(j),t,a,b,c,d);
//			res += omega0(i) * omega0(j) * libor_0(i) * libor_0(j) * covTmp;*/
//		}
//	}
//	res *= 1./(S0*S0);
//
//	return sqrt(res);
//}
//
//
//Real Swaption_::ReboVolInt(Time S, Time T, Rate S0, Real a, Real b, Real c, Real d) {
//
//	Real var = 0;
//	Real covTmp = 0;
//
//	for (size_t i = 0; i < N+1; ++i) {
//		for (size_t j = 0; j < N+1; ++j) {
//			covTmp = (*myVolMod).covIntegral(i,j,tenorDates(i),tenorDates(j),S,T);
//			var += omega0(i) * omega0(j) * libor_0(i) * libor_0(j) * covTmp;
//		}
//	}
//	var *= 1./(S0*S0); 
//	
//	return var;
//}
//
//
//Rate Swaption_::swapRate(Time t, Real G, Real a, Real b, Real c, Real d) {
//	
//	Real var = ReboVolInt(0,t,0.025,a,b,c,d);
//	//Real res = 0.025 * exp(sqrt(var)*G) * exp(-0.5*var);
//	return 0;	
//}
//
//
//********* EN CHANTIER *************/
//
//numeric::ublas::vector<Real> Swaption_::omegaA_0(numeric::ublas::vector<Real> bondPrices_0) {
//
//	Real period, tmp;
//	numeric::ublas::vector<Real> res(N+1);
//
//	/*res = omega0(bondPrices_0);
//
//	for (size_t i = 0; i < N+1; ++i) {
//		for (size_t k = 0; k < N+1; ++k) 
//			res(i) += omega_k_deriv_0(k,i,bondPrices_0(0)) * liborRates(k,0);
//	}*/
//
//	return res;
//}
//
//***************************************/
//
//Real Swaption_::bondPrice(Rate r, Time t, Time T) {
//	return exp(-r*(T-t));
//}
//
//
//
//
//
//Real Swaption_::bondPriceDerivative_0(size_t k, size_t i, Real bond_T0) {
//	
//	// A Tk-bond is a function of L0, L1, ..., Lk-1
//	/*if (i <= k-1 && i >= 0) { 
//		Real period = (*myLmm).getTenorDates()(i+1) - (*myLmm).getTenorDates()(i);
//		Real res = bondPrice(0,k,bond_T0);
//		res *= (-period) / (1 + period*liborRates(i,0));
//		return res;
//	}*/
//
//	return 0;
//}
//
//
//Real Swaption_::omega_k_deriv_0(size_t k, size_t i, Real bond_T0) {
//
//	/*Real u, v, res, period;
//
//	period = (*myLmm).getTenorDates()(k+1) - (*myLmm).getTenorDates()(k);
//	u = period * bondPriceDerivative_0(k+1,i,bond_T0);
//	v = annuity(0, N+1, (*myLmm).getTenorDates(), bond_T0);
//	res = u * v;
//
//	u = period * bondPrice(0,k+1,bond_T0);
//	v = 0;
//	for (size_t j = 0; j < N+1; ++j) {
//		period = (*myLmm).getTenorDates()(j+1) - (*myLmm).getTenorDates()(j);
//		v += period * bondPriceDerivative_0(j+1,i,bond_T0);
//	}
//
//	res -= u*v;
//	v = annuity(0, N+1, (*myLmm).getTenorDates(), bond_T0) * annuity(0, N+1, (*myLmm).getTenorDates(), bond_T0);
//	res /= v;
//	*/
//	return 0;
//}
//
//
//Real Swaption_::annuity(Time t) {
//
//	Real res = 0;
//	Real per = 0;
//	for (size_t i = 0; i < N+1; ++i) {
//		per = tenorDates(i+1) - tenorDates(i);
//		res += 1 * bondPrice(0.02, t, tenorDates(i+1));
//	}
//
//	return res;
//}
//
//
//
//
//#pragma endregion
//
//
//#pragma region getters
//
//Real Swaption_::getK() {
//	return this->K;
//}
//
//size_t Swaption_::getDiffPer() {
//	return this->diffPer;
//}
//
//size_t Swaption_::getFixingDateSize() {
//	return this->tenorStruct.getFixDates().size();
//}
//
//size_t Swaption_::getFloatingDateSize() {
//	return this->tenorStruct.getFloatDates().size();
//}
//
//const numeric::ublas::vector<Real> &Swaption_::getFixingDates() {
//	return this->tenorStruct.getFixDates();
//}
//
//const numeric::ublas::vector<Real> &Swaption_::getFloatingDates() {
//	return this->tenorStruct.getFloatDates();
//}
//
//#pragma endregion