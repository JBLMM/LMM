#include "TestPricing.h"
//#include "MC.h"
//#include <boost/math/distributions.hpp>
//#include <boost/numeric/ublas/matrix_proxy.hpp>
//#include <ql/pricingengines/blackformula.hpp>
//#include <ql/math/distributions/normaldistribution.hpp>
//#include <ctime>
//
//using namespace std;
//
//
//template<class T>
//void gen_normal(T & generator, numeric::ublas::vector<Real> &res)
//{
//	for(size_t i = 0; i < res.size(); ++i)
//		res(i) = generator();
//}
//
//
//
//TestPricing::TestPricing( // Libor params
//						  size_t N_libor,
//						  size_t nbFactors,
//						  double tenor_libor, 
//						  const matrix<Real>& B,
//						  // Vol params
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
//						  Rate K ) {
//	
//	// Swap Simulation parameters//
//	this->maturity = maturity;
//	this->tenor = tenors;
//	this->maturityPeriod = maturityPeriod; // Defines the discretization step
//	this->tenorPeriod = floatPer/12.;
//
//	// MC 
//	this->M = M;
//	this->tenorLibor = tenor_libor; // 6M
//	size_t diff = fixPer/floatPer;
//
//	this->maturityIndex = maturity / tenorLibor; 
//
//	// LMM 
//	size_t nbLiborMat = maturity/tenorLibor; //  TODO : create enum type for 6M, 3M
//	this->N = N_libor; 
//
//	// Model parameters //
//
//	numeric::ublas::vector<Time> discPeriods(N+1);
//	for (size_t i = 0; i < N_libor+1; ++i) // Here, change maturity to maturity * discretStep
//		discPeriods(i) = tenorLibor;
//
//	this->abcdParams = abcd;
//	this->B = B;
//
//	//-- Constant parameters for abcd vol structure
//	std::vector<double> g(N+1);
//	for (size_t i = 0; i < N+1; ++i)
//		g[i] = 1.;
//
//	DeterministicVol myVol(nbFactors,N+1,abcdParams,g);
//
//	//-- Shifts for each libor
//	// TODO : Put shifts as parameters
//	std::vector<double> liborShifts(N+1,0.);
//
//	this->model = McLmm(this->N, discPeriods, this->B, myVol,liborShifts);
//
//	matrix<Rate> liborRates(N+1, N+2, 0.);
//
//
//	// !!!!! Careful! We will get rid of that as soon as swap structure is ok
//	swapTenors = numeric::ublas::vector<Time>(tenor+1);
//	for (size_t i = 0; i < tenor+1; ++i) 
//		swapTenors(i) = this->model.getTenorDates()(i+maturityIndex);
// 		//swapTenors(i) = this->model.getTenorDates()(i+maturity);
//
//	// Here, we also have initialized our swap structure
//	this->product = new Swaption_(this->maturityIndex, this->N+1, this->tenor, fixPer, floatPer, K, swapTenors, nbFactors, B, abcdParams);
//}
//
//TestPricing::~TestPricing(){
//	delete [] product;
//}
//
//
//Real TestPricing::bondPrice(size_t i, size_t k, const matrix<Rate> &lmm) {
//	Real res = 1;
//	for (size_t j = i; j < k; ++j) {
//		Time per = model.getTenorDates()(j+1) - model.getTenorDates()(j);
//		res *= 1./(1 + per*lmm(j,i));
//	}
//	return res;
//}
//
//
//Real TestPricing::Spot_numeraire(size_t k, const matrix<Rate>& lmm) // spot numeraire, B(T_0=0) = 1.0
//{
//	Real res = 1;
//	Time per = model.getTenorDates()(k) - model.getTenorDates()(k-1); // YY bug, per should be different!
//	for (size_t j = 0; j < k; ++j) {
//		res *= (1 + per*lmm(j,j)); // YY not sure of the index, need to check...
//	}
//	return res;
//}
//
//// N = index of the last tenor date
//// M = nb of MC simulations
//void TestPricing::McEulerTerminalPrice(Real &mcPrice, Real &mcErr, RNG_Type & generator) {
//
//	mcPrice = 0; mcErr = 0; 
//	Real num = 0, tmp = 0;
//
//	Real a = abcdParams(0);
//	Real b = abcdParams(1);
//	Real c = abcdParams(2);
//	Real d = abcdParams(3);
//
//	Time T_terminal = (this->model).getTenorDates()(N+1); // terminal date 
//	Real bond_0 = (*product).bondPrice_0(0.02,T_terminal); // price at time 0 of the terminal maturity bond
//
//	size_t nbFixDates = (*product).getFixingDateSize();
//	size_t nbFloatDates = (*product).getFloatingDateSize();
//
//	size_t diff = (*product).getDiffPer();
//
//	//numeric::ublas::vector<Real> bonds(nbFloatDates,0.); // a bond price vector 
//	numeric::ublas::vector<Real> bonds(N+2-maturityIndex,0.);
//	numeric::ublas::vector<Real> G(3,0.);
//	matrix<Real> GaussianMatrix(3,N,0.); 
//
//	matrix<Rate> lmm(N+1,N+2,0.);
//
//	// initialize lmm matrix with libors at time t=0
//	for (size_t k = 0; k < N+1; ++k)
//			lmm(k,0) = model.libor_0(k);
//
//
//	// Creation of a vector of matrices, for vol integrals
//	// The vector's k-th component is a matrix of integrals, computed between T_k and T_{k+1}
//	numeric::ublas::vector<matrix<Real>> covTensor(N+1);
//	
//	for (size_t k = 0; k < N+1; ++k) { // time /int_{T_k}^{T_{k+1}} 
//
//		Time S = model.getTenorDates()(k);
//		Time T = model.getTenorDates()(k+1);
//		covTensor(k) = matrix<Real>(N+1,N+1,0.);
//
//		for (size_t i = k+1; i < N+1; ++i) { 
//
//			Time Ti = model.getTenorDates()(i);
//
//			for (size_t j = i; j < N+1; ++j) {
//
//				Time Tj = model.getTenorDates()(j);
//				covTensor(k)(i,j) = model.getVolMod().covIntegral(i,j,Ti,Tj,S,T);
//			}
//		}
//	}
//
//
//	//**** MC Simulations *****//
//	for (size_t i = 0; i < M; ++i) {
//
//		// Fill Gaussian matrix
//		for (size_t k = 0; k < N; ++k) {
//			gen_normal(generator,G); // Create a 3 dim gaussian vector
//			for (size_t i = 0; i < 3; ++i)
//				GaussianMatrix(i,k) = G(i);
//		}
//		
//
//	/*	for (size_t k = 0; k < N; ++k) {
//			for (size_t i = 0; i < 3; ++i)
//				cout << GaussianMatrix(i,k) << " ";
//
//			cout << endl;
//		}
//		cout << endl;*/
//
//
//		//model.TerminalLmmModel(GaussianMatrix,lmm, covTensor,euler); // Generate LIBOR rates under terminal measure		
//
//		model.TerminalLmmModel_Pc(GaussianMatrix, lmm, covTensor); 
//
//		/*for (size_t i = 0; i < lmm.size1(); ++i) {
//			for (size_t j = 0; j < lmm.size2(); ++j)
//				cout << lmm(i,j) << " ";
//
//			cout << endl;
//		}
//		cout << endl;*/
//
//
//		for (size_t k = 0; k < N+2-maturityIndex; ++k)     // !!!! HERE : we use maturity index
//			bonds(k) = model.bondPrice_i_j(maturityIndex,k+1,lmm);
//			//bonds(k) = model.bondPrice_i(maturity,k+1,lmm);
//			
//		/*for (size_t k = 0; k < N+2-maturityIndex; ++k)
//			cout << "Bond " << k << " : " << bonds(k) << " -- ";*/
//
//		//cout << endl << endl;
//
//		num = 1/ bonds(N+1-maturityIndex);
//
//		tmp = num * (*product).payoff(lmm, bonds);
//
//		mcPrice += tmp; // MC sum
//		mcErr += tmp*tmp;
//		//time_t t5 = clock();
//		/*cout << "simulation number " << i << endl;
//		cout << (double)t2-(double)t1 << endl;
//		cout << (double)t3-(double)t2 << endl;
//		cout << (double)t4-(double)t3 << endl;
//		cout << (double)t5-(double)t4 << endl;*/
//
//	}
//
//	mcPrice /= M;
//	mcPrice *= bond_0;
//
//	mcErr /= (M-1);
//	mcErr *= (bond_0 * bond_0);
//	mcErr -= (((Real)M)/(M-1))*mcPrice*mcPrice;
//	mcErr = sqrt(mcErr);
//	
//}
//
//void TestPricing::McIpcTerminalPrice(Real &mcPrice, Real &mcErr, RNG_Type & generator) {
//
//	mcPrice = 0; mcErr = 0; 
//	Real num = 0, tmp = 0;
//
//	Real a = abcdParams(0);
//	Real b = abcdParams(1);
//	Real c = abcdParams(2);
//	Real d = abcdParams(3);
//
//	size_t nbFixDates = (*product).getFixingDateSize();
//	size_t nbFloatDates = (*product).getFloatingDateSize();
//
//	size_t diff = (*product).getDiffPer();
//
//	numeric::ublas::vector<Real> bonds(nbFloatDates,0.); // a bond price vector 
//
//	Real bond_0 = (*product).bondPrice_0(0.02,swapTenors(tenor));
//
//	matrix<Rate> lmm(N+1,N+2,0.);
//	// initialize lmm matrix with libors at time t=0
//	for (size_t k = 0; k < N+1; ++k)
//		lmm(k,0) = model.libor_0(k);
//
//	numeric::ublas::vector<Real> G(3,0);
//	matrix<Real> GaussianMatrix(3,N+1,0.);
//
//	numeric::ublas::vector<matrix<Real>> covTensor(N+1);
//
//	//**** MC Simulations *****//
//	for (size_t i = 0; i < M; ++i) {
//
//		// initialize lmm matrix with libors at time t=0
//		for (size_t k = 0; k < N+1; ++k)
//			lmm(k,0) = model.libor_0(k);
//
//		// Fill Gaussian matrix
//		for (size_t k = 0; k < N+1; ++k) {
//			gen_normal(generator,G); // Create a 3 dim gaussian vector
//			for (size_t i = 0; i < 3; ++i)
//				GaussianMatrix(i,k) = G(i);
//		}
//		
//		model.TerminalLmmModel_Euler(GaussianMatrix,lmm,covTensor); // Generate LIBOR rates	
//
//		//num = 1 / bonds(N+1-maturity);
//		num = 1 / bonds(N+1-maturityIndex);
//		tmp = num * (*product).payoff(lmm,bonds);
//
//		mcPrice += tmp; // MC sum
//		mcErr += tmp*tmp;
//	}
//
//	mcPrice /= M;
//	mcPrice *= bond_0;
//
//	mcErr /= (M-1);
//	mcErr *= (bond_0 * bond_0);
//	mcErr -= (((Real)M)/(M-1))*mcPrice*mcPrice;
//	mcErr = sqrt(mcErr);
//}
//
//void TestPricing::McEulerSpotPrice(Real &mcPrice, Real &mcErr, RNG_Type & generator) {
//
//	mcPrice = 0; mcErr = 0; 
//	Real num = 1, tmp = 0, per = 0;
//
//	Real a = abcdParams(0);
//	Real b = abcdParams(1);
//	Real c = abcdParams(2);
//	Real d = abcdParams(3);
//
//	size_t nbFixDates = (*product).getFixingDateSize();
//	size_t nbFloatDates = (*product).getFloatingDateSize();
//
//	size_t diff = (*product).getDiffPer();
//
//	numeric::ublas::vector<Real> bonds(nbFloatDates,0.); // a bond price vector 
//
//	numeric::ublas::vector<matrix<Real>> covTensor(N+1);
//
//	matrix<Rate> lmm(N+1,N+2,0.);
//
//	// initialize lmm matrix with libors at time t=0
//	for (size_t k = 0; k < N+1; ++k)
//		lmm(k,0) = model.libor_0(k);
//
//	numeric::ublas::vector<Real> G(3,0.);
//	
//	//**** MC Simulations *****//
//	for (size_t i = 0; i < M; ++i) {
//
//		gen_normal(generator,G); // Create a 3 dim gaussian vector
//		model.SpotLmmModel(G,lmm,covTensor,euler); // Generate LIBOR rates under spot measure	
//
//		for (size_t k = 0; k < nbFloatDates; ++k) 
//			bonds(k) = (*product).bondPrice_T0(k+maturityIndex,lmm);
//			//bonds(k) = (*product).bondPrice_T0(k+maturity,lmm);
//
//		// Compute spot numeraire at time T0 (swaption's maturity)
//		for (size_t j = 0; j < maturityIndex; ++j) {
//			per = model.getTenorDates()(j+1) - model.getTenorDates()(j); // TODO: return ref
//			num *= (1 + per*lmm(j,j));
//		}	
//		num *= bonds(1);
//
//		tmp = (*product).payoff(lmm,bonds) / num;
//		
//		mcPrice += tmp; // MC sum
//		mcErr += tmp*tmp;
//
//		num = 1;
//	}
//
//	mcPrice /= M;
//
//	mcErr /= (M-1);
//	mcErr -= (((Real)M)/(M-1))*mcPrice*mcPrice;
//	mcErr = sqrt(mcErr);
//}
//
//void TestPricing::McIpcSpotPrice(Real &mcPrice, Real &mcErr, RNG_Type & generator) {
//
//	mcPrice = 0; mcErr = 0; 
//	Real num = 1, tmp = 0, per = 0;
//
//	Real a = abcdParams(0);
//	Real b = abcdParams(1);
//	Real c = abcdParams(2);
//	Real d = abcdParams(3);
//
//	size_t nbFixDates = (*product).getFixingDateSize();
//	size_t nbFloatDates = (*product).getFloatingDateSize();
//
//	size_t diff = (*product).getDiffPer();
//
//	numeric::ublas::vector<Real> bonds(nbFloatDates,0.); // a bond price vector 
//	//numeric::ublas::vector<Real> bonds(N+2-maturityIndex,0.);
//	numeric::ublas::vector<matrix<Real>> covTensor(N+1);
//	matrix<Rate> lmm(N+1,N+2,0.);
//
//	// initialize lmm matrix with libors at time t=0
//	for (size_t k = 0; k < N+1; ++k)
//		lmm(k,0) = model.libor_0(k);
//
//	numeric::ublas::vector<Real> G(3,0.);
//	
//	//**** MC Simulations *****//
//	for (size_t i = 0; i < M; ++i) {
//
//		// initialize lmm matrix with libors at time t=0
//		for (size_t k = 0; k < N+1; ++k)
//			lmm(k,0) = model.libor_0(k);
//
//		gen_normal(generator,G); // Create a 3 dim gaussian vector
//		model.SpotLmmModel(G,lmm,covTensor,ipc); // Generate LIBOR rates under spot measure	
//
//		for (size_t k = 0; k < N-maturityIndex+2; ++k) 
//			bonds(k) = (*product).bondPrice_T0(k+maturityIndex,lmm);
//
//		// Compute spot numeraire at time T0 (swaption's maturity)
//		for (size_t j = 0; j < maturityIndex; ++j) {
//			per = model.getTenorDates()(j+1) - model.getTenorDates()(j);
//			num *= (1 + per*lmm(j,j));
//		}	
//		num *= bonds(1);
//
//		tmp = (*product).payoff(lmm,bonds) / num;
//
//		mcPrice += tmp; // MC sum
//		mcErr += tmp*tmp;
//
//		num = 1;
//	}
//
//	mcPrice /= M;
//
//	mcErr /= (M-1);
//	mcErr -= (((Real)M)/(M-1))*mcPrice*mcPrice;
//	mcErr = sqrt(mcErr);
//} 
//
//void TestPricing::RebonatoPrice(Real &price){
//
//	Real a = abcdParams(0);
//	Real b = abcdParams(1);
//	Real c = abcdParams(2);
//	Real d = abcdParams(3);
//
//	Real varTmp = 0, corrTmp = 0;
//	Real K = (*product).getK();
//	Time T_maturity = (this->model).getTenorDates()(maturityIndex);
//	
//	boost::math::normal_distribution<> nd(0,1); 
//
//
//	// Compute Rebonato's vol approximation
//	for (size_t i = maturityIndex; i < this->N+1; ++i) {
//
//		Time Ti = (this->model).getTenorDates()(i);
//
//		for (size_t j = maturityIndex; j < this->N+1; ++j) {
//	
//			Time Tj = (this->model).getTenorDates()(j);
//			Real volTmp = this->model.getVolMod().covIntegral(i,j,Ti,Tj,0,T_maturity);
//
//			varTmp += (*product).omega0(i) * (*product).omega0(j) * 
//				(*product).libor_0(i-1) * (*product).libor_0(j-1) * volTmp;
//		}
//	}
//
//	Real S0 = (*product).swapRate_0();
//	varTmp *= 1/(S0*S0);
//
//	Real A0 = (*product).annuity_0();
//	Real d1 = (log(S0/K) + varTmp/2)/sqrt(varTmp);
//	Real d2 = d1 - sqrt(varTmp);
//
//	Real N1 = cdf(nd,d1);
//	Real N2 = cdf(nd,d2); 
//	
//	//N1 = 1; N2 = 1;
//
//	price = A0*(S0*N1-K*N2);
//}
//
//void TestPricing::AndersenPrice(Real &price) {
//
//	Real a = abcdParams(0);
//	Real b = abcdParams(1);
//	Real c = abcdParams(2);
//	Real d = abcdParams(3);
//
//	Real d1, d2, S0, A0;
//	Real N1, N2;
//	Real varTmp = 0, corrTmp = 0, volTmp = 0;
//	Real K = (*product).getK();
//
//	Time T0 = (this->model).getTenorDates()(maturityIndex);
//	Time Ti = 0,Tj = 0;
//
//	boost::math::normal_distribution<> nd(0,1); 
//
//	for (size_t i = 0; i < tenor; ++i) {
//
//		Ti = (this->model).getTenorDates()(maturityIndex + i);
//		for (size_t j = 0; j < tenor; ++j) {
//	
//			Tj = (this->model).getTenorDates()(maturityIndex + j);
//			volTmp = (this->model.getVolMod()).covIntegral(i,j,Ti,Tj,0,T0);
//
//			varTmp += (*product).omegaAndersen0(i) * (*product).omegaAndersen0(j) * 
//				(*product).libor_0(i) * (*product).libor_0(j) * volTmp;
//		}
//	}
//
//	S0 = (*product).swapRate_0();
//	varTmp *= 1/(S0*S0);
//
//	A0 = (*product).annuity_0();
//	d1 = (log(S0/K) + varTmp/2)/sqrt(varTmp);
//	d2 = d1 - sqrt(varTmp);
//	N1 = cdf(nd,d1);
//	N2 = cdf(nd,d2); 
//
//	price = A0*(S0*N1-K*N2);
//
//}
//
//
//Real TestPricing::BlackCaplet(size_t i) {
//
//	Real a = abcdParams(0);
//	Real b = abcdParams(1);
//	Real c = abcdParams(2);
//	Real d = abcdParams(3);
//
//	Real K = product->getK();
//	Time Ti = (this->model).getTenorDates()(i);
//	Time Tk = (this->model).getTenorDates()(i+1);
//	Real per = Tk - Ti;
//	Real bond = product->bondPrice_0(0.02,Tk);
//	Real libor0 = model.libor_0(i);
//	Real varTmp = model.getVolMod().varIntegral(i,Ti,0,Ti);
//	
//	cout << "Black Formula : variance : " << varTmp << endl;
//
//	Real d1 = log(libor0/K)/sqrt(varTmp) + 0.5*sqrt(varTmp);
//	Real d2 = d1 - sqrt(varTmp);
//
//	CumulativeNormalDistribution nd;
//	Real N1 = nd(d1);
//	Real N2 = nd(d2);
//
//	Real price = bond*per*(libor0*N1 - K*N2);
//	
//	return price;
//}
//
//void TestPricing::McForwardCaplet(size_t i, Real & mcPrice, Real & mcErr, RNG_Type & generator) {
//	
//	Real a = abcdParams(0);
//	Real b = abcdParams(1);
//	Real c = abcdParams(2);
//	Real d = abcdParams(3);
//
//	Real libor = 0.;
//	Time Ti = (this->model).getTenorDates()(i);
//	Time Tk = (this->model).getTenorDates()(i+1);
//	Real numeraire = product->bondPrice_0(0.02,Tk);
//	Real varTmp = (model.getVolMod()).varIntegral(i,Ti,0,Ti);
//	Real per = Tk - Ti;
//	Real tmp = per * numeraire;
//
//	mcPrice = 0.;
//	mcErr = 0.;
//	numeric::ublas::vector<Real> G(1,0.); // Gaussian random variable
//
//	for (size_t j = 0; j < M; ++j) {
//
//		gen_normal(generator,G); 
//		// Compute Li(Ti) under the Ti+1 forward measure => drfitless
//		libor = exp( sqrt(varTmp)*G(0)-0.5*varTmp );
//		libor *= model.libor_0(i);
//
//		Real x = max(libor-product->getK(), 0.);
//		mcPrice += x;
//		mcErr += x*x;
//		
//	}
//
//	mcPrice *= tmp/M;
//
//	mcErr *= (tmp*tmp)/(M-1);
//	mcErr -= (((Real)M)/(M-1))*mcPrice*mcPrice;
//	mcErr = sqrt(mcErr);
//}
//
//void TestPricing::McTerminalCaplet(Real & mcPrice, Real & mcErr, size_t index_libor, RNG_Type & generator) {
//
//	Real a = abcdParams(0);
//	Real b = abcdParams(1);
//	Real c = abcdParams(2);
//	Real d = abcdParams(3);
//
//	Real libor = 0.;
//	Time Ti = (this->model).getTenorDates()(N);
//	Time T_terminal = (this->model).getTenorDates()(N+1);
//	Real numeraire = product->bondPrice_0(0.02,T_terminal);
//	
//	Real per = T_terminal - Ti;
//	Real tmp = per * numeraire;
//
//	mcPrice = 0.;
//	mcErr = 0.;
//
//	size_t nbFactors = this->model.getVolMod().getNbFactors();
//	numeric::ublas::vector<Real> G(nbFactors,0.); // Gaussian random variable
//	matrix<Real> GaussianMatrix(nbFactors,N+1,0.); // A gaussian matrix, one column per time step
//
//	matrix<Real> lmm(N+1,N+2,0.);
//	numeric::ublas::vector<Real> bonds(N-maturityIndex+2,0.); //maturity = T0
//
//	for (size_t j = 0; j < N+1; ++j)
//		lmm(j,0) = model.libor_0(j);
//
//	numeric::ublas::vector<matrix<Real>> covTensor(N+1);
//	
//	for (size_t k = 0; k < N+1; ++k) { // time /int_{T_k}^{T_{k+1}} 
//
//		Time S = model.getTenorDates()(k);
//		Time T = model.getTenorDates()(k+1);
//		covTensor(k) = matrix<Real>(N+1,N+1,0.);
//
//		for (size_t i = k+1; i < N+1; ++i) { 
//
//			Ti = model.getTenorDates()(i);
//
//			for (size_t j = i; j < N+1; ++j) {
//				Time Tj = model.getTenorDates()(j);
//				covTensor(k)(i,j) = model.getVolMod().covIntegral(i,j,Ti,Tj,S,T);
//			}
//		}
//	}
//	
//	time_t t1 = clock();
//	for (size_t j = 0; j < M; ++j) {
//		
//		// Fill the gaussian matrix
//		for (size_t k = 0; k < N+1; ++k) {
//			gen_normal(generator,G);
//			for (size_t i = 0; i < nbFactors; ++i)
//				GaussianMatrix(i,k) = G(i);
//		}
//
//		model.TerminalLmmModel_Pc(GaussianMatrix,lmm,covTensor); // Generate LIBOR rates under terminal measure		
//
//		libor = lmm(index_libor,index_libor);
//
//		//discount(i)
//		Real discount_i = model.bondPrice_i_j(index_libor+1,N+1,lmm);
//
//		Real x = max(libor-product->getK(), 0.)/discount_i;
//		mcPrice += x;
//		mcErr += x*x;
//	}
//
//	//time_t t2 = clock();
//
//	mcPrice *= tmp/M;
//
//	mcErr *= (tmp*tmp)/(M-1);
//	mcErr -= (((Real)M)/(M-1))*mcPrice*mcPrice;
//	mcErr = sqrt(mcErr);
//}
//
//void TestPricing::McTerminalCaplet2(Real & mcPrice, Real & mcErr, size_t index_libor, RNG_Type & generator) {
//
//	Real a = abcdParams(0);
//	Real b = abcdParams(1);
//	Real c = abcdParams(2);
//	Real d = abcdParams(3);
//
//	Real libor = 0.;
//	Time Ti = (this->model).getTenorDates()(N);
//	Time T_terminal = (this->model).getTenorDates()(N+1);
//	Real numeraire = product->bondPrice_0(0.02,T_terminal);
//	
//	Real per = T_terminal - Ti;
//	Real tmp = per * numeraire;
//
//	mcPrice = 0.;
//	mcErr = 0.;
//	numeric::ublas::vector<Real> G(3,0.); // Gaussian random variable
//	matrix<Real> GaussianMatrix(3,N+1,0.); // A gaussian matrix, one column per time step
//
//	matrix<Real> lmm(N+1,N+2,0.);
//	numeric::ublas::vector<Real> bonds(N-maturityIndex+2,0.); //maturity = T0
//
//	for (size_t j = 0; j < N+1; ++j)
//		lmm(j,0) = model.libor_0(j);
// 
//	 size_t N_discretization = 15;  
//
//	
//	MC mc(generator, 
//	   model, 
//	   1, 
//	   N_discretization); 
//	
//	time_t t1 = clock();
//	for (size_t j = 0; j < M; ++j) {
//		
//			
//		
//		// Simulations
//		mc.simulation();
//		matrix<Real> liborMatrix = mc.getPath();
//
//		//libor = lmm(index_libor,index_libor);
//		libor = liborMatrix(index_libor,index_libor);
//		//discount(i)
//		Real discount_i = model.bondPrice_i_j(index_libor+1,N+1,lmm);
//
//		Real x = max(libor-product->getK(), 0.)/discount_i;
//		mcPrice += x;
//		mcErr += x*x;
//	}
//
//	time_t t2 = clock();
//	//cout << t2-t1 << endl;
//	mcPrice *= tmp/M;
//
//	mcErr *= (tmp*tmp)/(M-1);
//	mcErr -= (((Real)M)/(M-1))*mcPrice*mcPrice;
//	mcErr = sqrt(mcErr);
//}
//
//void TestPricing::McSpotCaplet(Real & mcPrice, Real & mcErr, size_t index_libor, RNG_Type & generator) { // euler
//
//	Real a = abcdParams(0);
//	Real b = abcdParams(1);
//	Real c = abcdParams(2);
//	Real d = abcdParams(3);
//
//	Real libor = 0.;
//	Time Ti = (this->model).getTenorDates()(N);
//	Time T_terminal = (this->model).getTenorDates()(N+1);
//	Real numeraire = product->bondPrice_0(0.02,T_terminal);
//	
//	Real per = T_terminal - Ti;
//	Real tmp = per; // B(T_0=0) = 1.0
//
//	mcPrice = 0.;
//	mcErr = 0.;
//
//	size_t nbFactors = this->model.getVolMod().getNbFactors();
//	numeric::ublas::vector<Real> G(nbFactors,0.); // Gaussian random variable
//	matrix<Real> GaussianMatrix(nbFactors,N+1,0.); // A gaussian matrix, one column per time step
//
//	matrix<Real> lmm(N+1,N+2,0.);
//	numeric::ublas::vector<Real> bonds(N-maturityIndex+2,0.); //maturity = T0
//
//	for (size_t j = 0; j < N+1; ++j)
//		lmm(j,0) = model.libor_0(j);
//
//	// Creation of a vector of matrices, for vol integrals
//	// The vector's k-th component is a matrix of integrals, computed between T_k and T_{k+1}
//	numeric::ublas::vector<matrix<Real>> covTensor(N+1);
//	
//	for (size_t k = 0; k < N+1; ++k) { // time /int_{T_k}^{T_{k+1}} 
//
//		Time S = model.getTenorDates()(k);
//		Time T = model.getTenorDates()(k+1);
//		covTensor(k) = matrix<Real>(N+1,N+1,0.);
//
//		for (size_t i = k+1; i < N+1; ++i) { 
//
//			Ti = model.getTenorDates()(i);
//
//			for (size_t j = i; j < N+1; ++j) {
//				Time Tj = model.getTenorDates()(j);
//				covTensor(k)(j,i) = model.getVolMod().covIntegral(i,j,Ti,Tj,S,T);
//			}
//		}
//	}
//	
//	time_t t1 = clock();
//	for (size_t j = 0; j < M; ++j) {
//		
//		// Fill the gaussian matrix
//		for (size_t k = 0; k < N+1; ++k) {
//			gen_normal(generator,G);
//			for (size_t i = 0; i < nbFactors; ++i)
//				GaussianMatrix(i,k) = G(i);
//		}
//
//		//model.SpotLmmModel_euler(GaussianMatrix,lmm,covTensor);     // YY euler
//		model.SpotLmmModel_pc(GaussianMatrix,lmm,covTensor);
//
//		libor = lmm(index_libor,index_libor);
//		Real B_i = Spot_numeraire(index_libor+1,lmm); // YY not sure of the index need to check! 
//		
//		Real x = max(libor-product->getK(), 0.)/B_i;
//		mcPrice += x;
//		mcErr += x*x;
//	}
//
//	time_t t2 = clock();
//	//cout << t2-t1 << endl;
//	mcPrice *= tmp/M;
//
//	mcErr *= (tmp*tmp)/(M-1);
//	mcErr -= (((Real)M)/(M-1))*mcPrice*mcPrice;
//	mcErr = sqrt(mcErr);
//} 
//
//
//
//void TestPricing::McTerminalEulerSwaptionPrice(Real &mcPrice, 
//											   Real &mcStdErr, 
//											   SwapInstruments::VanillaSwap& swap,
//											   SwapInstruments::VanillaSwaption& swaption,
//											   RNG_Type & generator, 
//											   const std::vector<double>& libors_T0,
//											   const numeric::ublas::vector<matrix<Real>>& covTensor) {
//
//	matrix<Real> libors(N+1,N+2,0.);
//
//	//-- initialize the libor matrix
//	for (size_t i = 0; i < N+1; ++i)
//		libors(i,0) = libors_T0[i];
//
//	
//	//-- MC loop
//	mcPrice = 0.; 
//	mcStdErr = 0.;
//	size_t nbFactors = this->model.getVolMod().getNbFactors();
//
//	for (size_t simulIndex = 0; simulIndex < M; ++simulIndex) 
//	{
//		//-- Fill the gaussian matrix (one column for each time step)
//		matrix<Real> gaussianMatrix(nbFactors,N+1);
//		numeric::ublas::vector<Real> G(nbFactors,0.);
//		for (size_t k = 0; k < N+1; ++k) {
//			gen_normal(generator,G);
//			for (size_t i = 0; i < nbFactors; ++i)
//				gaussianMatrix(i,k) = G(i);
//		}
//
//		//-- Libor simulation
//		model.TerminalLmmModel_Euler(gaussianMatrix,libors,covTensor);
//		
//
//		//-- TODO : Replace matrix<Real> by vector<vector> everywhere 
//		std::vector<std::vector<double>> liborsMatrix(libors.size1());
//		for (size_t i = 0; i < libors.size1(); ++i) 
//		{
//			for (size_t j = 0; j < libors.size2(); ++j)
//				liborsMatrix[i].push_back(libors(i,j));
//		}
//
//		//-- Compute ZC bonds with simulated libors
//		std::vector<double> bonds;
//		swap.computeBonds_T0(N,bonds, liborsMatrix);
//
//		double swaptionPayoff = swaption.payoff(bonds,liborsMatrix);
//		double numeraire = model.bondPrice_i_j(swap.get_indexStart(),N+1,libors); // P(T0,Tn+1) // P(T_start,Tn+1)
//		double x = swaptionPayoff/numeraire; 
//		mcPrice += x;
//		mcStdErr += x*x;
//	}
//
//	mcPrice /= M;
//	double bond_0 = model.bondPrice_i_j(0,N+1,libors);
//	mcPrice *= bond_0;
//
//	mcStdErr /= (M-1);
//	mcStdErr *= (bond_0 * bond_0);
//	mcStdErr -= (((Real)M)/(M-1))*mcPrice*mcPrice;
//	mcStdErr = sqrt(mcStdErr);
//}
//
//
//void TestPricing::McTerminalPcSwaptionPrice(Real &mcPrice, 
//											Real &mcStdErr, 
//											SwapInstruments::VanillaSwap& swap,
//											SwapInstruments::VanillaSwaption& swaption,
//											RNG_Type & generator, 
//											const std::vector<double>& libors_T0,
//											const numeric::ublas::vector<matrix<Real>>& covTensor) {
//
//	matrix<Real> libors(N+1,N+2,0.);
//
//	//-- initialize the libor matrix
//	for (size_t i = 0; i < N+1; ++i)
//		libors(i,0) = libors_T0[i];
//
//	
//	//-- MC loop
//	mcPrice = 0.; 
//	mcStdErr = 0.;
//	size_t nbFactors = this->model.getVolMod().getNbFactors();
//
//	for (size_t simulIndex = 0; simulIndex < M; ++simulIndex) 
//	{
//		//-- Fill the gaussian matrix (one column for each time step)
//		matrix<Real> gaussianMatrix(nbFactors,N+1);
//		numeric::ublas::vector<Real> G(nbFactors,0.);
//		for (size_t k = 0; k < N+1; ++k) {
//			gen_normal(generator,G);
//			for (size_t i = 0; i < nbFactors; ++i)
//				gaussianMatrix(i,k) = G(i);
//		}
//
//		//-- Libor simulation
//		model.TerminalLmmModel_Pc(gaussianMatrix,libors,covTensor);
//		
//
//		//-- TODO : Replace matrix<Real> by vector<vector> everywhere 
//		std::vector<std::vector<double>> liborsMatrix(libors.size1());
//		for (size_t i = 0; i < libors.size1(); ++i) 
//		{
//			for (size_t j = 0; j < libors.size2(); ++j)
//				liborsMatrix[i].push_back(libors(i,j));
//		}
//
//		//-- Compute ZC bonds with simulated libors
//		std::vector<double> bonds;
//		swap.computeBonds_T0(N,bonds, liborsMatrix);
//
//		double swaptionPayoff = swaption.payoff(bonds,liborsMatrix);
//		double numeraire = model.bondPrice_i_j(swap.get_indexStart(),N+1,libors); // P(T0,Tn+1) // P(T_start,Tn+1)
//		double x = swaptionPayoff/numeraire; 
//		mcPrice += x;
//		mcStdErr += x*x;
//	}
//
//	mcPrice /= M;
//	double bond_0 = model.bondPrice_i_j(0,N+1,libors);
//	mcPrice *= bond_0;
//
//	mcStdErr /= (M-1);
//	mcStdErr *= (bond_0 * bond_0);
//	mcStdErr -= (((Real)M)/(M-1))*mcPrice*mcPrice;
//	mcStdErr = sqrt(mcStdErr);
//}
//
//
//void TestPricing::McSpotEulerSwaptionPrice(Real &mcPrice, 
//										   Real &mcStdErr, 
//										   SwapInstruments::VanillaSwap& swap,
//										   SwapInstruments::VanillaSwaption& swaption,
//										   RNG_Type & generator, 
//										   const std::vector<double>& libors_T0,
//										   const numeric::ublas::vector<matrix<Real>>& covTensor) {
//
//	matrix<Real> libors(N+1,N+2,0.);
//
//	//-- initialize the libor matrix
//	for (size_t i = 0; i < N+1; ++i)
//		libors(i,0) = libors_T0[i];
//
//	
//	//-- MC loop
//	mcPrice = 0.; 
//	mcStdErr = 0.;
//	size_t nbFactors = this->model.getVolMod().getNbFactors();
//
//	for (size_t simulIndex = 0; simulIndex < M; ++simulIndex) 
//	{
//		//-- Fill the gaussian matrix (one column for each time step)
//		matrix<Real> gaussianMatrix(nbFactors,N+1);
//		numeric::ublas::vector<Real> G(nbFactors,0.);
//		for (size_t k = 0; k < N+1; ++k) {
//			gen_normal(generator,G);
//			for (size_t i = 0; i < nbFactors; ++i)
//				gaussianMatrix(i,k) = G(i);
//		}
//
//		//-- Libor simulation
//		model.SpotLmmModel_euler(gaussianMatrix,libors,covTensor);
//
//		//-- TODO : Replace matrix<Real> by vector<vector> everywhere 
//		std::vector<std::vector<double>> liborsMatrix(libors.size1());
//		for (size_t i = 0; i < libors.size1(); ++i) 
//		{
//			for (size_t j = 0; j < libors.size2(); ++j)
//				liborsMatrix[i].push_back(libors(i,j));
//		}
//
//		//-- Compute ZC bonds with simulated libors
//		std::vector<double> bonds;
//		swap.computeBonds_T0(N,bonds, liborsMatrix);
//
//		double swaptionPayoff = swaption.payoff(bonds,liborsMatrix);
//		
//		Real B_i = Spot_numeraire(swap.get_indexStart(),libors);
//		
//		double x = swaptionPayoff/B_i; 
//		mcPrice += x;
//		mcStdErr += x*x;
//	}
//
//	mcPrice /= M;
//
//	mcStdErr /= (M-1);
//	mcStdErr -= (((Real)M)/(M-1))*mcPrice*mcPrice;
//	mcStdErr = sqrt(mcStdErr);
//}
//
//
//
//void TestPricing::McSpotPcSwaptionPrice(Real &mcPrice, 
//										Real &mcStdErr, 
//										SwapInstruments::VanillaSwap& swap,
//										SwapInstruments::VanillaSwaption& swaption,
//										RNG_Type & generator, 
//										const std::vector<double>& libors_T0,
//										const numeric::ublas::vector<matrix<Real>>& covTensor) {
//
//	matrix<Real> libors(N+1,N+2,0.);
//
//	//-- initialize the libor matrix
//	for (size_t i = 0; i < N+1; ++i)
//		libors(i,0) = libors_T0[i];
//
//	
//	//-- MC loop
//	mcPrice = 0.; 
//	mcStdErr = 0.;
//	size_t nbFactors = this->model.getVolMod().getNbFactors();
//
//	for (size_t simulIndex = 0; simulIndex < M; ++simulIndex) 
//	{
//		//-- Fill the gaussian matrix (one column for each time step)
//		matrix<Real> gaussianMatrix(nbFactors,N+1);
//		numeric::ublas::vector<Real> G(nbFactors,0.);
//		for (size_t k = 0; k < N+1; ++k) {
//			gen_normal(generator,G);
//			for (size_t i = 0; i < nbFactors; ++i)
//				gaussianMatrix(i,k) = G(i);
//		}
//
//		//-- Libor simulation
//		model.SpotLmmModel_pc(gaussianMatrix,libors,covTensor);
//
//		//-- TODO : Replace matrix<Real> by vector<vector> everywhere 
//		std::vector<std::vector<double>> liborsMatrix(libors.size1());
//		for (size_t i = 0; i < libors.size1(); ++i) 
//		{
//			for (size_t j = 0; j < libors.size2(); ++j)
//				liborsMatrix[i].push_back(libors(i,j));
//		}
//
//		//-- Compute ZC bonds with simulated libors
//		std::vector<double> bonds;
//		swap.computeBonds_T0(N,bonds, liborsMatrix);
//
//		double swaptionPayoff = swaption.payoff(bonds,liborsMatrix);
//		Real B_i = Spot_numeraire(swap.get_indexStart(),libors);
//		//double numeraire = model.bondPrice_i(swap.get_indexStart(),N+1,libors); // P(T0,Tn+1) // P(T_start,Tn+1)
//		//double x = swaptionPayoff/numeraire; 
//		double x = swaptionPayoff/B_i; 
//		mcPrice += x;
//		mcStdErr += x*x;
//	}
//
//	mcPrice /= M;
//	//double bond_0 = model.bondPrice_i(0,N+1,libors);
//	//mcPrice *= bond_0;
//
//	mcStdErr /= (M-1);
//	//mcStdErr *= (bond_0 * bond_0);
//	mcStdErr -= (((Real)M)/(M-1))*mcPrice*mcPrice;
//	mcStdErr = sqrt(mcStdErr);
//}