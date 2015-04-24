//#pragma once
//
//#include "Approximation.h"
//
//#include <ql\types.hpp>
//#include <ql\math\array.hpp>
//#include <ql\math\optimization\costfunction.hpp>
//
//typedef std::vector<std::vector<double>> matrix_;
//typedef std::vector<std::vector<std::vector<size_t>>> indexMatrix;
//
//using namespace QuantLib;
//
//class SwaptionCostFunction: public CostFunction
//{
//private:
//	
//	matrix_ swaptionMatrix_Mkt_; // Matrix of market swaption vol
//	matrix_ swaptionStrikes_;
//
//	indexMatrix floatingIndexMatrix_; // each component of the matrix has a specific set of floating indices
//	matrix_ swapRates_; // Matrix of swap rates at time 0
//	matrix_ annuities_; // Matrix of annuities at time 0 -- not used
//	std::vector<double> bonds_0_; // vector of ZC bonds at time 0
//	std::vector<double> libors_0_; // Libors a time 0
//	std::vector<double> libor_shifts_; // ATTENTION : si shift not 0, traiter les vol de marché d'abord
//	mutable Approximation approximation_; // Class implementing Rebonato's formula 
//	mutable NumericalMethods NM_;
//
//	matrix_ weights_; // weights for regulation 
//	matrix_ weights_maturity_; // weights for regulation (1st order derivative with respect to maturity)
//	matrix_ weights_tenor_; // weights for regulation (1st order derivative with respect to tenor)
//	matrix_ weights_maturity_2_; // weights for regulation (2nd order derivative with respect to maturity)
//	matrix_ weights_tenor_2_; // weights for regulation (2nd order derivative with respect to tenor)
//	
//public:
//	SwaptionCostFunction(const matrix_& swaptionMatrix,
//		                 const matrix_& swaptionStrikes,			
//						 const indexMatrix& floatingIndexMatrix,
//						 matrix_ swapRates,
//					     matrix_ annuities,
//					     std::vector<double> bonds_0,
//					     std::vector<double> libors_0,
//						 std::vector<double> libor_shifts,
//						 const Approximation& approximation,
//						 
//						 matrix_ weights,
//						 matrix_ weights_maturity,
//						 matrix_ weights_tenor,
//						 matrix_ weights_maturity_2,
//						 matrix_ weights_tenor_2);
//
//	~SwaptionCostFunction();
//
//	Real value(const Array & x) const; 
//	Disposable<Array> values(const Array& x) const; 
//
//	//-- Compute regulation term 
//	//-- coefficients ci control regulation terms
//	Real regularisation(const Array& x, Real c1, Real c2, Real c3, Real c4) const;  // TODO: changer nom coef 
//	Real sum_all_weights_regularisation(const matrix_& weights) const;
//	//Real sum_all_derivatives_regularisation(const matrix_& weights, const matrix_& derivatives);
//
//	void getMarketInfo(string fileName);
//	matrix_ map_ArrayToMatrix(const Array& x) const;
//	Array map_MatrixtoArray(const matrix_& mat) const;
//
//	//-- Getters
//	size_t get_swaptionMatrixRows();
//};
