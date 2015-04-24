//#include "CapletTests.h"
//#include <vector>
//
//using namespace std;
//
//void pricingCapletTest() 
//{
//	#pragma region Parameters
//
//	Real a = -0.06; 
//	Real b = 0.17; 
//	Real c = 0.54; 
//	Real d = 0.17;
//
//	std::vector<Real> abcdParams(4);
//	abcdParams[0] = a;
//	abcdParams[1] = b;
//	abcdParams[2] = c;
//	abcdParams[3] = d;
//
//	Rate r = 0.02;
//
//	size_t floatPer = 6;
//	size_t fixPer = 12;
//	Real tenorLibor = 0.5; // = 6M per LIBOR
//
//	size_t N = 19; // Index of the last libor 
//	size_t M = 50000; // Number of MC simulations
//	size_t nbFactors = 3; // number of factors for LMM
//
//	//-- Constant parameters for abcd vol structure
//	std::vector<double> g(N+1);
//	for (size_t i = 0; i < N+1; ++i)
//		g[i] = 1.;
//
//	//-- Set tenor periods (in years)
//	Time firstTenor = 1;
//	std::vector<Time> tenorPeriods(N+1);
//	for (size_t i = 0; i < N+1; ++i) 
//		tenorPeriods[i] = tenorLibor;
//
//	//-- Set tenor dates (in years)
//	std::vector<Time> tenorDates(N+2);
//	tenorDates[0] = 0.;
//	for (size_t i = 1; i < N+2; ++i)
//		tenorDates[i] = tenorDates[i-1] + tenorPeriods[i-1];
//
//
//	//-- Set volatility model
//	DeterministicVol vol(nbFactors,N+1,abcdParams,g);
//
//	//-- Random number generator
//	boost::variate_generator<boost::mt19937, boost::normal_distribution<> >
//	generator(boost::mt19937(time(0)),
//	boost::normal_distribution<>());
//
//	//-- Shifts for each libor
//	std::vector<double> liborShifts(N+1,0.);
//
//#pragma endregion
//
//	std::vector<double> libors_init(N+1);
//	for (size_t i = 0; i < N+1; ++i)
//		libors_init[i] = 0.02013;
//
//	//-- LMM model
//	McTerminalLmm model(N,tenorPeriods,vol,liborShifts,libors_init);
//
//	//-- variances/Covariances
//	std::vector<std::vector<std::vector<double>>> covTensor(N+1);
//	covTensor = vol.computeAllCovariances(N,model.getTenorDates());
//	
//	#pragma region Caplet
//
//	size_t index_caplet = 10; 
//	//double strike = model.libor_0(index_caplet,r); // ATM strike
//	double strike = model.getLiborMatrix()[index_caplet][0];
//
//	//-- Caplet
//	Caplet caplet(tenorDates[index_caplet+1],tenorLibor,strike,index_caplet);
//
//	#pragma endregion
//
//	#pragma region MC pricing
//
//	double mcPrice = 0., mcErr = 0.;
//
//	//-- Set libors at time 0
//	std::vector<double> libors_0;
//	for (size_t i = 0; i < N+1; ++i)
//		libors_0.push_back(model.getLiborMatrix()[i][0]);
//
//	//-- Caplet pricer
//	LmmCapletPricer capletPricer(caplet,model);
////	capletPricer.McCapletPrice_Terminal_Pc(M,mcPrice,mcErr,generator,libors_0,covTensor);
//	capletPricer.McCapletPrice(M,mcPrice,mcErr,libors_0,euler);
//
//	Real lowerIc = mcPrice - 3*mcErr/sqrt((Real)M);
//	Real upperIc = mcPrice + 3*mcErr/sqrt((Real)M);
//	cout << "Monte-Carlo price estimate with "<< M << " simulations : " << mcPrice << endl;
//	cout << "99% Confidence Interval : " << "[" << lowerIc << " ; " << upperIc << "]" << " X = " << 3*mcErr/sqrt((Real)M) << endl << endl;
//
//	#pragma endregion
//}
//
//
