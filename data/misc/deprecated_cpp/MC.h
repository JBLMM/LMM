//#pragma once
//
//#include "McLmm.h"
//
//
//typedef boost::variate_generator<boost::mt19937, boost::normal_distribution<> > RNG;
//typedef std::vector<std::vector<double>> matrix_;
//
//class MC
//{
//
////private :
////
////	/// A random number generator for gaussian vectors
////	RNG *rng; // Gausian(0,1), Dim = dim 
////
////	/// The LMM model
////	McLmm* lmmModel; 
////
////	/// A matrix to be filled with LIBORs
////	matrix_ path; // 1 simulation: matrix 1/2*(N_libor, N_libor)
////	            // [index_libor][Ti]
////
////	/// temporary memory, starting point of Euler 
////	std::vector<double> libors_temp; 
////
////	/// Discretization time step per tenor
////	double delta_discretization; //= lmmModel/N_discretization; // constructor
////
////	/// Number of MC simulations
////	size_t N_simulation; 
////
////	/// Number of time steps per tenor
////	size_t N_discretization; // par tenor = 6M de libor 
////
////
////public:
////
////	MC(const RNG &rng, const McLmm &lmmModel, size_t N_simulation, size_t N_discretization);
////
////	~MC();
////
////	void simulation();
////
////	void initialize_libors();
////
////	numeric::ublas::vector<matrix<Real>> computeCovarianceTensor(size_t N_discretization, size_t index_T);
////
////	void YY_euler_Terminal(size_t index_T, double t1, double t2, const numeric::ublas::vector<Real> &G, matrix<Real> &covariances_t1_t2);
////	
////	void YY_euler_spot(size_t index_T, double t1, double t2, const numeric::ublas::vector<Real> &G, matrix<Real> &covariances_t1_t2);  // dX_t = dirft(t,X_t)dt + diffusion(t,X_t)dW_t
////
////	void YY_predictor_corrector_terminal(size_t index_T, double t1, double t2, const numeric::ublas::vector<Real> &G, matrix<Real> &covariances_t1_t2);  // dX_t = dirft(t,X_t)dt + diffusion(t,X_t)dW_t
////
////	void YY_predictor_corrector_spot(size_t index_T, double t1, double t2, const numeric::ublas::vector<Real> &G, matrix<Real> &covariances_t1_t2);  // dX_t = dirft(t,X_t)dt + diffusion(t,X_t)dW_t
////
////	const matrix<Rate> &getPath();
//};
