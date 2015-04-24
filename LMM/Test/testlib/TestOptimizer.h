//#pragma once
//
//#include "RobonatoAngle.h" // YY stupid only for the QuantLib Type 
//#include <ql/pricingengines/blackscholescalculator.hpp>
//#include <boost/random/normal_distribution.hpp>
//#include <boost/math/distributions.hpp> // boost::math::cdf, pdf, ... 
//#include <boost/math/distributions/normal.hpp>
//#include <ql/types.hpp>
//#include <cassert>
//
//using namespace QuantLib;
//
//class TestOptimizer
//{
//public:
//
//
//};
//
////! TODO:
////! 1. binding to a member function 
////! 2. set constrain.
//
//
//class RosenBrockFunction : public CostFunction {
//public :
//	Real value ( const Array & x) const {
//		//QL_REQUIRE (x. size ()==2 , " Rosenbrock function is 2- dim .");
//		assert(x.size()==2);
//		Real res =(1 -x [0])*(1 - x [0]);
//		res +=100.0*( x[1] - x [0]* x [0])*( x[1] -x [0]* x [0]);
//		return res;
//	}
//
//	Disposable <Array> values ( const Array & x) const
//	{
//		//QL_REQUIRE (x. size ()==2 , " Rosenbrock function is 2- dim .");
//		assert(x.size()==2);
//		// irrelevant what you write in res for most of the optimizers
//		// most of them are using value anyways . try with res [0]=100.0
//		Array res (1); res [0]= value (x);
//		return res;
//	}
//};
//
//void testOptimizer1 (){
//	Size maxIterations =1000;
//	Size minStatIterations =100;
//	Real rootEpsilon =1e-8;
//	Real functionEpsilon =1e-9;
//	Real gradientNormEpsilon =1e-5;
//
//	EndCriteria myEndCrit ( maxIterations ,
//							minStatIterations ,
//							rootEpsilon ,
//							functionEpsilon ,
//							gradientNormEpsilon );
//
//	RosenBrockFunction myFunc ;
//	NoConstraint constraint ;
//	Problem myProb1 ( myFunc , constraint , Array (2 ,0.1));
//	Problem myProb2 ( myFunc , constraint , Array (2 ,0.1));
//	Simplex solver1 (0.1);
//	ConjugateGradient solver2 ;
//
//	EndCriteria :: Type solvedCrit1 = solver1 . minimize ( myProb1 , myEndCrit );
//	EndCriteria :: Type solvedCrit2 = solver2 . minimize ( myProb2 , myEndCrit );
//
//	std :: cout << " ---- ----  1st test ---- ----  :" << std :: endl ;
//	std :: cout << " Criteria Simplex :" << solvedCrit1 << std :: endl ;
//	std :: cout << " Root Simplex :" << myProb1 . currentValue () << std :: endl ;
//	std :: cout << " Min F Value Simplex :" << myProb1 . functionValue () << std :: endl ;
//	std :: cout << " Criteria CG:" << solvedCrit2 << std :: endl ;
//	std :: cout << " Root CG:" << myProb2 . currentValue () << std :: endl ;
//	std :: cout << " Min F Value CG :" << myProb2 . functionValue () << std :: endl ;
//}
//
//
////------------------------------------------------------------------------------------
////
////                                     2ND problem
////
////------------------------------------------------------------------------------------
//
//Real blackScholesPrice ( const Real & spot ,
//						const Real & strike ,
//						const Rate & rd ,
//						const Rate & rf ,
//						const Volatility & vol ,
//						const Time & tau ,
//						const size_t & phi )
//{
//	boost::math::normal d(0.0 ,1.0); // shit quantlib doc, bug everywhere...
//	Real dp ,dm , fwd , stdDev , res , domDf , forDf ;
//	domDf =std :: exp (-rd* tau ); forDf = std :: exp(-rf* tau );
//	fwd = spot * forDf / domDf ;
//	stdDev = vol* std :: sqrt (tau );
//	dp =( std :: log ( fwd / strike )+0.5* stdDev * stdDev )/ stdDev ;
//	dm =( std :: log ( fwd / strike ) -0.5* stdDev * stdDev )/ stdDev ;
//	res = phi * domDf *( fwd* boost::math::cdf(d,phi *dp) - strike * boost::math::cdf (d, phi *dm ));
//	return res ;
//}
//
//class CallProblemFunction : public CostFunction 
//{
//private :
//	Real C1_ ,C2_ ,C3_ ,C4_ ,K1_ ,K2_ ,K3_ , K4_;
//	Rate rd_ , rf_ ;
//	size_t phi_ ;
//	Time tau_ ;
//public :
//	CallProblemFunction ( const Rate & rd , const Rate & rf , const Time & tau , const size_t & phi ,
//						  const Real & K1 , const Real & K2 , const Real & K3 , const Real & K4 ,
//						  const Real & C1 , const Real & C2 , const Real & C3 , const Real & C4)
//	: rd_ (rd), rf_ (rf), phi_ (phi), tau_ ( tau ),
//	C1_ (C1), C2_ (C2), C3_ (C3), C4_(C4),
//	K1_ (K1), K2_ (K2), K3_ (K3), K4_(K4 ){}
//
//	Real value ( const Array & x) const 
//	{
//		Array tmpRes = values (x);
//		Real res = tmpRes [0]* tmpRes [0];
//		res += tmpRes [1]* tmpRes [1];
//		res += tmpRes [2]* tmpRes [2];
//		res += tmpRes [3]* tmpRes [3];
//		return res;
//	}
//
//	Disposable <Array> values ( const Array & x) const 
//	{
//		Array res (4);
//		res [0]= blackScholesPrice (x[0] , K1_ ,rd_ ,rf_ ,x[1] , tau_ , phi_ )- C1_ ;
//		res [1]= blackScholesPrice (x[0] , K2_ ,rd_ ,rf_ ,x[1] , tau_ , phi_ )- C2_ ;
//		res [2]= blackScholesPrice (x[0] , K3_ ,rd_ ,rf_ ,x[1] , tau_ , phi_ )- C3_ ;
//		res [3]= blackScholesPrice (x[0] , K4_ ,rd_ ,rf_ ,x[1] , tau_ , phi_ )- C4_ ;
//		return res ;
//	}
//};
//
//
//void testOptimizer2 (){
//	// setup of market parameters
//	Real spot =98.51;
//	Volatility vol =0.134;
//	Real K1 =87.0 , K2 =96.0 , K3 =103.0 , K4 =110.0;
//	Rate rd =0.002 , rf =0.01;
//	size_t phi =1;
//	Time tau =0.6;
//	// calculate Black Scholes prices
//	Real C1= blackScholesPrice (spot ,K1 ,rd ,rf ,vol ,tau , phi );
//	Real C2= blackScholesPrice (spot ,K2 ,rd ,rf ,vol ,tau , phi );
//	Real C3= blackScholesPrice (spot ,K3 ,rd ,rf ,vol ,tau , phi );
//	Real C4= blackScholesPrice (spot ,K4 ,rd ,rf ,vol ,tau , phi );
//	CallProblemFunction optFunc (rd , rf , tau , phi ,K1 , K2 , K3 , K4 , C1 , C2 , C3 , C4 );
//
//	Size maxIterations =1000;
//	Size minStatIterations =100;
//	Real rootEpsilon =1e-5;
//	Real functionEpsilon =1e-5;
//	Real gradientNormEpsilon =1e-5;
//	EndCriteria myEndCrit ( maxIterations , minStatIterations , rootEpsilon ,
//	functionEpsilon , gradientNormEpsilon );
//	Array startVal (2); startVal [0]=80.0; startVal [1]=0.20;
//	NoConstraint constraint ;
//	Problem myProb ( optFunc , constraint , startVal );
//	LevenbergMarquardt solver ;
//	EndCriteria :: Type solvedCrit = solver . minimize (myProb , myEndCrit );
//
//	std :: cout << " ---- ----  2nd test ---- ----  :" << solvedCrit << std :: endl ;
//	std :: cout << " Criteria :" << solvedCrit << std :: endl ;
//	std :: cout << " Root :" << myProb . currentValue () << std :: endl ;
//std :: cout << " Min Function Value :" << myProb . functionValue () << std :: endl ;
//}