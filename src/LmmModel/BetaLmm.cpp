//#include <LMM/LmmModel/BetaLmm.h>
//#include <cassert>
//
//BetaLmm::BetaLmm(const Dispersion& dispersion, const LowerTriangularMatrix& betas) // size = N+1, horizon = N
//		 : Lmm(dispersion)
//		 , betas_(betas)
//{
//	assert(betas_.size1()== dispersion_.get_horizon()+1 && betas_.size2() == dispersion_.get_horizon()+1);
//}
//
//double BetaLmm::get_beta(LMM::Index liborIndex, LMM::Index timeIndex) const
//{
//	assert(liborIndex !=0 && liborIndex >= timeIndex);
//	return betas_(liborIndex,timeIndex);
//}
//
//double BetaLmm::equivalentShift(LMM::Index liborIndex, LMM::Index timeIndex, const std::vector<double>& liborsInitValue) const // for Mc simulation: beta for period [timeIndex-1, timeIndex]
//{
//	double beta = get_beta(liborIndex,timeIndex);
//	assert(abs(beta) >= 1.0e-8); // beta should not be zero! if not the equivalentShift will not validate.
//
//	return (1-beta)/beta*liborsInitValue[liborIndex];
//}