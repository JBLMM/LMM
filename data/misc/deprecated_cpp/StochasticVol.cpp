//#include "StochasticVol.h"
//
//StochasticVol::StochasticVol() {} 
//
//StochasticVol::~StochasticVol() {} 
//
////
////numeric::ublas::vector<Real> StochasticVol::computeI(Time S, Time T, AbcdFunction func, numeric::ublas::vector<Time> tenorDates, Real absAcc, Size maxEval) {
////	numeric::ublas::vector<Real> tmp(0);
////	return tmp;	
////}
////
////numeric::ublas::vector<Real> StochasticVol::computeY(Time S, Time T, AbcdFunction func, numeric::ublas::vector<Time> tenorDates, Real absAcc, Size maxEval, matrix<Real> G) {
////	numeric::ublas::vector<Real> tmp(0);
////	return tmp;	
////}
////
////void StochasticVol::computeCov(matrix<Real> &cov_mat, size_t N, Time S, Time T, AbcdFunction func, numeric::ublas::vector<Time> tenorDates, Real absAcc, Size maxEval) {
////
////}
//
//const matrix<Real> & StochasticVol::getCorr() {
//	return this->corrMatrix;
//}
//
//
//Real StochasticVol::computeIntSto(size_t i, size_t j,
//		const numeric::ublas::vector<matrix<Real>> &covTensor, numeric::ublas::vector<Real> &G) {
//			return 0;
//}
//
//Real StochasticVol::abcdCov(size_t i, size_t j, Time Ti, Time Tj, Time t, Real a, Real b, Real c, Real d) {
//	return 0;
//}
//
//Real StochasticVol::covIntegral(size_t i, size_t j, Time Ti, Time Tj, Time S, Time T) {
//	return 0;
//}
//
//Real StochasticVol::varIntegral(size_t i, Time Ti, Time S, Time T) {
//	return 0;
//}
//
//const matrix<Real> &StochasticVol::getterB() { return this->B; }