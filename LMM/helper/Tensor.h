#ifndef LMM_HELPER_TENSOR_H
#define LMM_HELPER_TENSOR_H
#pragma once

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include <LMM/helper/GenericPath.h>


/*! The tensor is a 3D matrix storing for precomputed value of time integral in each intervall [T_{k-1},T_k]
 *  Each case stored 
 *
 * tensor(k,i,j) = \int_{T_{k-1}}^{T_k}  vol_i(t) * vol_j(t)  * \rho_{ij} dt
 *
 * Where
 * vol_i, vol_j are volatilities for i-th and j-th LIBOR path : L_i,L_j
 * \rho_{ij} is the reduced approximated correlation matrix (same dimension, but reduced for the nb factor)
 */

class Tensor
{
public:
	typedef boost::numeric::ublas::matrix<double> matrix;

	Tensor(size_t size1, size_t size2, size_t size3);

	void set_tensor(const std::vector<Tensor::matrix> & val) ;

	double       & operator()(size_t index_T, size_t indexLibor_i, size_t indexLibor_j)       ;
	const double & operator()(size_t index_T, size_t indexLibor_i, size_t indexLibor_j) const ;

	const std::vector<Tensor::matrix>& get_tensor()  const ;
	
	const Tensor::matrix& operator[](size_t indexT)  const ;		

	//! print in Excel	
	void write_to_stream(std::ostream& outputstream) const ;
	void print(const std::string& filename)          const ;
private:
	// tensor_(k,i,j): L_i, L_j 's integral of vol in [T_{k-1},T_k], k=[1,...,N], i,j=[k,...,N]
    std::vector<Tensor::matrix> tensor_; 
};

std::ostream& operator<<(std::ostream& os, const Tensor& swap);

#endif /* LMM_HELPER_TENSOR_H */