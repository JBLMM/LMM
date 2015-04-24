#include "MC.h"
//
//#include <boost\numeric\ublas\matrix_proxy.hpp>
//#include <boost/random/normal_distribution.hpp>
//#include <boost/random/mersenne_twister.hpp>
//#include <boost/random/variate_generator.hpp>
//
//template<class T>
//void gen_normal(T & generator, numeric::ublas::vector<Real> &res)
//{
//	for(size_t i = 0; i < res.size(); ++i)
//		res(i) = generator();
//}
//
//
//MC::MC(const RNG &rng, 
//	   const McLmm &lmmModel, 
//	   size_t N_simulation, 
//	   size_t N_discretization) 
//{
//	this->rng                  = new RNG(rng);
//	this->lmmModel             = McLmm(lmmModel);
//	size_t N_libors               = this->lmmModel.getHorizon();
//	this->path                 = matrix<Rate>(N_libors+1,N_libors+1,0.);
//	this->libors_temp          = numeric::ublas::vector<Rate>(N_libors+1,0.);
//
//	this->N_discretization     = N_discretization;
//	this->N_simulation         = N_simulation;
//	
//	Time tenorPer              = this->lmmModel.getDeltaT()(0); // TODO: create a vector of periods
//	this->delta_discretization = tenorPer/N_discretization;
//}
//
//MC::~MC() 
//{
//	delete [] rng;
//}
//
//void MC::simulation()
//{
//	size_t N_libor = lmmModel.getHorizon();
//	size_t nbFactors = lmmModel.getVolMod().getNbFactors();
//
//	// Initialize libors a time 0
//	initialize_libors();
//
//
//	numeric::ublas::vector<matrix<Real>> covTensor(N_discretization);
//	for (size_t i = 0; i < N_discretization; ++i)
//		covTensor(i) = matrix<Real>(N_libor+1,N_libor+1,0.);
//
//	
//	//matrix<Real> G(nbFactors, N_discretization,0.);
//	numeric::ublas::vector<Real> Gaussian_tmp(nbFactors,0.);
//
//	Real mcPrice = 0.;
//	Real mcStdDev = 0.;
//
//	for(size_t l = 0; l < N_simulation; ++l)
//	{
//		// Compute a trajectory for each libor
//		for(size_t index_T = 0; index_T < N_libor; ++index_T) // loop on T_i
//		{
//			size_t index_T1 = index_T;
//			size_t index_T2 = index_T+1;
//
//			matrix_column<matrix<Rate>> lmmColumn(path,index_T1); // Extract the column of libor matrix corresponding to date T_{index_T}
//			libors_temp = lmmColumn; 
//
//			Time t1 = lmmModel.getTenorDates()(index_T1);
//
//			/*numeric::ublas::vector<matrix<Real>>*/ 
//			covTensor = computeCovarianceTensor(N_discretization,index_T1); 
//							
//			for(size_t j = 0; j < N_discretization; ++j) // T_i ->T_{i+1}
//			{
//			    t1 += j*delta_discretization;
//				Time t2 = t1 + delta_discretization;
//
//				gen_normal(*rng,Gaussian_tmp);
//
//				YY_euler_Terminal(index_T+1,t1,t2,Gaussian_tmp,covTensor(j)); // simulation libor [i,i+1,...,N] in vector libors_temp
//				//YY_euler_spot(index_T+1,t1,t2,Gaussian_tmp,covTensor(j));
//				//YY_predictor_corrector_terminal(index_T+1,t1,t2,Gaussian_tmp,covTensor(j));
//			}
//
//			for (size_t index_libor = index_T2; index_libor < libors_temp.size(); ++index_libor)
//				path(index_libor,index_T2) = libors_temp(index_libor);
//		}
//
//		// Here : Compute a price
//		
//	}
//} 
//
//// Set all libors values at time t=0
//void MC::initialize_libors() {
//	size_t N_libor = this->lmmModel.getHorizon();
//	for (size_t index_libor = 0; index_libor < N_libor+1; ++index_libor) {
//		this->path(index_libor,0) = this->lmmModel.libor_0(index_libor);
//	}
//}
//
//// Compute all vol covariances for every discretization steps in ONE tenor ([S;T])
//numeric::ublas::vector<matrix<Real>> MC::computeCovarianceTensor(size_t N_discretization, size_t index_T) 
//{
//	size_t N_libor = lmmModel.getHorizon();
//	numeric::ublas::vector<matrix<Real>> covTensor(N_discretization);
//	Time t1 = lmmModel.getTenorDates()(index_T); // Set t1 at the right date
//
//	// Loop on time discretization
//	for (size_t index_disc = 0; index_disc < N_discretization; ++index_disc) {
//
//		covTensor(index_disc) = matrix<Real>(N_libor+1,N_libor+1,0.);
//		 t1 += index_disc*delta_discretization;
//		 Time t2 = t1 + delta_discretization;
//
//		for (size_t index_libor = index_T; index_libor < N_libor+1; ++index_libor) {
//			Time Ti = lmmModel.getTenorDates()(index_libor);
//
//			for (size_t k = index_libor; k < N_libor+1; ++k) {
//				Time Tk = lmmModel.getTenorDates()(k);
//				covTensor(index_disc)(index_libor,k) = lmmModel.getVolMod().covIntegral(index_libor,k,Ti,Tk,t1,t2);
//				covTensor(index_disc)(k,index_libor) = covTensor(index_disc)(index_libor,k);
//			}
//		}
//	}	
//	return covTensor;
//}
//
//
//void MC::YY_euler_Terminal(size_t index_T, 
//	                       double t1, 
//						   double t2, 
//						   const numeric::ublas::vector<Real> &G,
//						   matrix<Real> &covariances_t1_t2)  // dX_t = dirft(t,X_t)dt + diffusion(t,X_t)dW_t
//{
//	size_t N_libor = lmmModel.getHorizon();
//	numeric::ublas::vector<Real> var_temp(lmmModel.getHorizon()+1, 0.);
//
//	for(size_t index_libor = N_libor; index_libor >= index_T; --index_libor) {
//		var_temp(index_libor) = covariances_t1_t2(index_libor,index_libor);
//	}
//
//	numeric::ublas::vector<Real> drift       = lmmModel.drifts(libors_temp,index_T,t1,t2,covariances_t1_t2);     // dim = N, some are empty(index_T)   --YY You need to save drift in a vector for each libor
//	numeric::ublas::vector<Real> diffusion   = lmmModel.diffusions(var_temp,G,index_T,t1,t2); // dim = N*3, some are empty(index_T)  --YY same as drift
//
//	/*std::cout << "DRIFTs : " << std::endl;
//	for (size_t i = 0; i < diffusion.size(); ++i)
//		std::cout << diffusion(i) << " ";
//
//	std::cout << std::endl << std::endl;*/
//
//	size_t index_first_libor = index_T; // we don't compute libors which indices are under the time index
//
//	for(size_t index_libor = N_libor; index_libor >= index_first_libor; --index_libor)
//	{
//		libors_temp(index_libor)  =  libors_temp(index_libor) * exp( drift(index_libor) - 0.5*var_temp(index_libor) + diffusion(index_libor) );
//	}
//}
//
//
//
//void MC::YY_euler_spot(size_t index_T, 
//	                       double t1, 
//						   double t2, 
//						   const numeric::ublas::vector<Real> &G,
//						   matrix<Real> &covariances_t1_t2)  // dX_t = dirft(t,X_t)dt + diffusion(t,X_t)dW_t
//{
//	size_t N_libor = lmmModel.getHorizon();
//	numeric::ublas::vector<Real> var_temp(lmmModel.getHorizon()+1, 0.);
//
//	for(size_t index_libor = index_T; index_libor <= N_libor; ++index_libor) {
//		Time Ti = lmmModel.getTenorDates()(index_libor);
//		var_temp(index_libor) = lmmModel.getVolMod().covIntegral(index_libor,index_libor,Ti,Ti,t1,t2);
//	}
//
//	numeric::ublas::vector<Real> drift           = lmmModel.spot_drifts(libors_temp,index_T,t1,t2,covariances_t1_t2);     // dim = N, some are empty(index_T)   --YY You need to save drift in a vector for each libor
//	numeric::ublas::vector<Real> diffusion       = lmmModel.diffusions(var_temp,G,index_T,t1,t2); // dim = N*3, some are empty(index_T)  --YY same as drift
//
//	size_t index_first_libor = index_T; // we don't compute libors which indices are under the time index
//
//	for(size_t index_libor = index_first_libor; index_libor <= N_libor; ++index_libor)
//	{
//		libors_temp(index_libor)  =  libors_temp(index_libor) * exp( drift(index_libor) - 0.5*var_temp(index_libor) + diffusion(index_libor) );
//	}
//}
//
//
//
//void MC::YY_predictor_corrector_terminal(size_t index_T, double t1, double t2, const numeric::ublas::vector<Real> &G, matrix<Real> &covariances_t1_t2)  // dX_t = dirft(t,X_t)dt + diffusion(t,X_t)dW_t
//
//{
//
//	size_t N_libor = lmmModel.getHorizon();
//	size_t index_first_libor = index_T; // we don't compute libors which indices are under the time index
//	numeric::ublas::vector<Real> var_temp(lmmModel.getHorizon()+1, 0.);
//
//	for(size_t index_libor = N_libor; index_libor >= index_T; --index_libor) {
//		Time Ti = lmmModel.getTenorDates()(index_libor);
//		var_temp(index_libor) = lmmModel.getVolMod().covIntegral(index_libor,index_libor,Ti,Ti,t1,t2);
//	}
//
//	numeric::ublas::vector<Real> diffusion = lmmModel.diffusions(var_temp,G,index_T,t1,t2); 
//
//	// before erasing libors at time t1 by those at time t2, we save them inside a new vector (for Euler/IPC)
//	numeric::ublas::vector<Real> libors_t1 = libors_temp; 
//
//	// Compute all libors at time t2, without any drift at first
//	for(size_t index_libor = N_libor; index_libor >= index_first_libor; --index_libor)
//	{
//		libors_temp(index_libor)  =  libors_temp(index_libor) * exp(- 0.5*var_temp(index_libor) + diffusion(index_libor) );
//
//		if (index_libor == N_libor || index_libor == index_first_libor) // Then, there is no drift
//			continue;
//
//		Real drift_index_libor = lmmModel.drifts_ipc(index_libor, libors_t1, libors_temp,index_T,t1,t2,covariances_t1_t2); 
//		libors_temp(index_libor)  *=  exp( drift_index_libor );
//	}
//}
//
//// TODO : change the loops' order and also use the GOOD DRIFT METHOD
//void MC::YY_predictor_corrector_spot(size_t index_T, double t1, double t2, const numeric::ublas::vector<Real> &G, matrix<Real> &covariances_t1_t2) {
//
//	size_t N_libor = lmmModel.getHorizon();
//	size_t index_first_libor = index_T; // we don't compute libors which indices are under the time index
//	numeric::ublas::vector<Real> var_temp(lmmModel.getHorizon()+1, 0.);
//
//	for(size_t index_libor = N_libor; index_libor >= index_T; --index_libor) {
//		Time Ti = lmmModel.getTenorDates()(index_libor);
//		var_temp(index_libor) = lmmModel.getVolMod().covIntegral(index_libor,index_libor,Ti,Ti,t1,t2);
//	}
//
//	numeric::ublas::vector<Real> diffusion = lmmModel.diffusions(var_temp,G,index_T,t1,t2); 
//
//	// before erasing libors at time t1 by those at time t2, we save them inside a new vector (for Euler/IPC)
//	numeric::ublas::vector<Real> libors_t1 = libors_temp; 
//
//	// Compute all libors at time t2, without any drift at first
//	for(size_t index_libor = N_libor; index_libor >= index_first_libor; --index_libor)
//	{
//		libors_temp(index_libor)  =  libors_temp(index_libor) * exp(- 0.5*var_temp(index_libor) + diffusion(index_libor) );
//
//		if (index_libor == N_libor || index_libor == index_first_libor) // Then, there is no drift
//			continue;
//
//		Real drift_index_libor = lmmModel.drifts_ipc(index_libor, libors_t1, libors_temp,index_T,t1,t2,covariances_t1_t2); 
//		libors_temp(index_libor)  *=  exp( drift_index_libor );
//	}
//}
//
//const matrix<Rate> & MC::getPath() {
//	return this->path;
//}