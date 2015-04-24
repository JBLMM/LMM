#ifndef LMM_NUMERIC_INTERPOLATION_H
#define LMM_NUMERIC_INTERPOLATION_H
#pragma once

#include <vector>
#include <ql/math/array.hpp>
#include <boost/numeric/ublas/triangular.hpp>



namespace numeric //ctntodo change this namespace to interpolation, and remove class interpolation, use free method instead
{
	typedef boost::numeric::ublas::matrix<double> FullMatrix;
	typedef boost::numeric::ublas::matrix<double> UpperTriangularDoubleMatrix;//to change

	class Interpolation
	{
	public:
		//-- Interpolation on y - find value corresponding to given maturity date
		// Insert a point in the appropriated position in x vector
		// Compute the coherent point in vector y, and insert it into the appropriated position 
		void linearInterpolation
			(
			std::vector<double>& y        ,
			std::vector<double>& x        ,
			const double&        x_interp ,
			const double epsilon=10e-8    // if x_interp is too close to a node on x vector, we will not interpolate
			);

		double linearInterpolation
			(
			const double& x1, 
			const double& x2, 
			const double& y1, 
			const double& y2, 
			const double& x_interp
			);

		/*! interpolation for vectors, that can be used for columns and rows in matrix
		 *
		 *  Giving nodes values and its correspondant nodes indices, interpolator return a full vector interpolated
		 *		- interpolate values between nodes
		 *		- extrapolate values out of nodes rank
		 *
		 *  NOTE : the first case of the vector is setted to an absurb value
		 *         that is coherent with the convention of gMatrix, or gTransformed matrix
		 */
		QuantLib::Array interpolateVector( const QuantLib::Array & node_values
			                             , const std::vector<size_t> & node_indices
										 , const size_t vector_size) const ;



		//!
		/*! interpolation helper for matrice.
		 *  giving two extremun value and its corresponding indices, interpolate value of a index between them
		 */
		double columnIntepolation(const double & lowvalue, const double& highvalue,
			                      size_t lowIndex, size_t highIndex, 
								  size_t interpolIndex);	



		/*!
		 *  This not allow extrapolation here, every missed indices has to be in the interior of the matrix
		 *
		 */
		void fullMatrixInterpolate( FullMatrix& matrix
			                      , const std::vector<size_t> & row_indices_missed
								  , const std::vector<size_t> & col_indices_missed);
	};
}
#endif /* LMM_NUMERIC_INTERPOLATION_H */