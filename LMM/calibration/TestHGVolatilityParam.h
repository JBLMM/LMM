#pragma once

#include <boost/numeric/ublas/triangular.hpp>  // Triangular matrix
#include <boost/numeric/ublas/io.hpp>

#include <ql/termstructures/volatility/abcd.hpp>

#include <LMM/helper/LMMTenorStructure.h>
#include <LMM/LmmModel/ConstShifted_HGVolatilityFunction.h>


//==================  TEST HGABCD VOLATILITY PARAMETER ========================
//=======================================================================

/*! \class TestHGVolatilityParam
 *  use abcd for generating h function
 *  use of g function
 *
 *  Concerning parameters are 
 *  g_ij for each LIBOR L_i, there are i-1 g
 *  g_ij is a [nbLibor-1]x[nbLibor-1] matrix
 *
 *  Number of parameters are [nbLibor-1]x[nbLibor]/2 
 */
class TestHGVolatilityParam : public ConstShifted_HGVolatilityParam
{
public:
	typedef Shifted_HGVolatilityParam::VectorOfMatrixIndices VectorOfMatrixIndices;

	TestHGVolatilityParam(LMMTenorStructure_PTR _pLMMTenorStructure,double a, double b, double c, double d, 
						  double g_constParam, double shift_constParam);

	virtual ~TestHGVolatilityParam(){} 

	virtual  QuantLib::Array get_ArrayFrom_G(const VectorOfMatrixIndices& indices_vector) const ;

	virtual void reset_H_FromArray(const QuantLib::Array& x, const VectorOfMatrixIndices& indices_vector, bool needInterpolated=false);
	 
	virtual void reset_G_FromArray(const QuantLib::Array& x, const VectorOfMatrixIndices& indices_vector, bool needInterpolated=false);

	virtual  QuantLib::Array get_ArrayFrom_Shift() const;

	virtual void reset_Shift_FromArray(const QuantLib::Array& x);

	// This method printout elements of h, g and h*g matrices 
	// And also print the diag of g for easily debugging in calibration 
	virtual void write_to_stream(std::ostream & os) const  ;
protected:
	//virtual double calc_h(size_t indexLibor, size_t indexTime) const;
	//virtual double calc_g(size_t indexLibor, size_t indexTime) const;

private :

	////ctntodo giving an arbitary indices set, interpolate the rest of the g matrix
	void g_interpolate(size_t sparse_step); 


	/*! giving indices usually for a fixed column in g matrix
	 *  assuming every precedent sparse columns are fixed,   
	 *  g_interpolate_column interpolate and extrapolate
	 */
	void g_interpolate_column(const VectorOfMatrixIndices& indices_column);
	void g_diagonal_extrapolate_low_margin(size_t column_index, size_t row_offset);
	void g_extrapolate_left_margin(size_t first_sparse_col_index, size_t sparse_step);
	void g_vanish_right_of_column(size_t column_index);

	//! asuming every edge of a triangle in a matrix block all has determined values
	//  This block is a lower sub diagonale block, that can be determined by giving only two diagonale item
	//  This method interpolate only the strict interior of the triangle 
	void g_interpolate_sub_diagnal_block(size_t low_k, size_t high_k);
};


