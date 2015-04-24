#pragma once

#include <boost/shared_ptr.hpp>

#include <LMM/numeric/Interpolation.h>

#include <LMM/helper/Printer.h>
#include <LMM/helper/GenericPath.h>

#include <LMM/instrument/VanillaSwaption.h>
#include <LMM/LmmModel/UpperTriangleVanillaSwaptionQuotes.h>
#include <LMM/numeric/UpperTriangularMatrixInterpolator.h>

typedef boost::numeric::ublas::triangular_matrix<double, boost::numeric::ublas::lower> LowerTriangularDoubleMatrix; // LTDM

typedef boost::numeric::ublas::matrix<bool> UpperTriangularBoolMatrix;

//TODO: 1. UpperTriangularDoubleMatrix <--> quantilib array (attetion: il ne faut pas faire le pour lowerTriangleMatrix)
//	  il faut l'implementer dans Helper class 	   
//	  2. implement Class interpolator in Helper, il ne faut pas implement dans cette class. 

/*! \class GMatrixMapping  
*
* gDelegate_    is a upper triangular matrix
* gTransformed_ is a upper triangular matrix (interpolated by gDelegate)
* g_            is a lower triangular matrix (g matrix in hg volatility matrix), that is transformed from gTransformed_
*
* The routine is 
*		gDelegate_ (fetching + interpolating) --> gTransformed (update_g_from_gTransforme) -> g
*/

class GMatrixMapping  
{
	UpperTriangularBoolMatrix gDelegateFlags_; //ctntodo use this matrix as flag indicating cells to be truncated or not
	LowerTriangularDoubleMatrix  g_;            // libor's vol g, 1st row & col ignored lower triangle: size = 2*year
	UpperTriangularDoubleMatrix gTransformed_; // libor's vol g: transformed , 1st row & col ignored uppertriangle: size = 2*year
	UpperTriangularDoubleMatrix gDelegate_;                     // swaption's size, 1st row & col ignored  size = year + 1

	UpperTriangularIndexPairMatrix gDelegate_IndexMappping_gTransformed_; // helper swaption's size, 1st row & col ignored  size = year  + 1

	std::vector<std::pair<size_t,size_t> > truncated_cell_;

	std::vector<size_t> full_rows_interpolations_indices_;
	std::vector<size_t> full_cols_interpolations_indices_;

	// Rows and Cols of gDelegate that will not be take in account
	std::vector<size_t> truncated_gDelegate_Rows_;
	std::vector<size_t> truncated_gDelegate_Cols_;
public:

	//! ctntodo move into test DO NOT leave in this class
	static size_t get_gDelegateSizeFromHorizon(size_t horizon, size_t ratioFixedFloat);
	static size_t get_gSizeFromNbYear(size_t nbYear, size_t ratioFixedFloat)      ;

	GMatrixMapping(size_t full_g_size,
		           const UpperTriangularDoubleMatrix&    gDelegate, // used as calibration input
		           const UpperTriangularIndexPairMatrix& gDelegate_IndexMappping_gTransformed);

	// convert gDelegate format: matrix -> QuantLib::Array
	QuantLib::Array get_DelegateArray() const;
	//! = put all elements from a row of gDelegate into in array and return here
	QuantLib::Array get_DelegateArray(const size_t iRowDelegate) const;

	//! reset from a QuantLib array to gDelegate and process all the routine.
	void reset_gDelegate( const QuantLib::Array & qlArray );
	//! reset from a QuantLib array to a specific gDelegate's row and process locally the routine.
	void reset_gDelegate( const QuantLib::Array & qlArray , const size_t iRowDelegate);

	//! reset a cellule for gDelegate
	void reset_gDelegate( const double& cell_value, const std::pair<size_t,size_t>& gDelegateCELL);


	//! reset rows and columns of gDelegate that will not be used
	void add_Truncated_gDelegate_Cells(const std::pair<size_t,size_t>& gDelegateCELL);
	void reset_Truncated_gDelegate_Rows(const std::vector<size_t> & gDelegate_rows_indices);
	void reset_Truncated_gDelegate_Cols(const std::vector<size_t> & gDelegate_cols_indices);
	
	//! getter: 
	const LowerTriangularDoubleMatrix& get_g_Ref()            const {return g_;}
	const UpperTriangularDoubleMatrix& get_gTransformed_Ref() const {return gTransformed_;}
	const UpperTriangularDoubleMatrix& get_gDelegate_Ref()    const {return gDelegate_;}
	//! method retransform the gDelegate matrix in natural format, coherent to the gMatrix
	//! NOTE : this method is only used for a print propose, 
	LowerTriangularDoubleMatrix get_gDelegateInNaturalFormat() const;

	//! getter of informations about delegate matrix
	size_t get_TotalNbDelegateElement() const ;
	size_t get_NbDelegateElementOnRow(const size_t iRowDelegate) const;
	size_t get_MaxDelegateRowIndex()    const { return gDelegate_.size1() - 2 ; }
	size_t get_MaxDelegateColIndex()    const { return gDelegate_.size2() - 2 ; }	
	
	const std::vector<std::pair<size_t,size_t> >& get_truncated_gDelegate_CELL() const{ return truncated_cell_;}
	const std::vector<size_t>& get_truncated_gDelegate_Rows() const { return truncated_gDelegate_Rows_; }
	const std::vector<size_t>& get_truncated_gDelegate_Cols() const { return truncated_gDelegate_Cols_; }

	void print(const std::string& filename) const;

private:

	void re_initiate_FlagMatrix();

	UpperTriangularMatrixBlockInterpolator interpolator_;

	void initiate_interpolation_indices();
	 
	//! globally processing the routine gDelegate_ (interpolates) --> gTransformed (update_g_from_gTransforme) -> g
	void global_proccess_transform();
	
	//! fetch values from gDelegate to gTransformed
	void fetch_gDelegate_to_gTransformd(const size_t iRowDelegate);

	//! interpolation gTransformed
	void rowDelegate_proccess_transform(const size_t iRowDelegate);
	
	//! transforming gTransformed to g matrix by giving row intervals indices
	void update_g_from_gTransformed(const size_t iRow_gTransformed, const size_t iiRow_gTransformed) ;		
	
	//! from a iRowDelegate, return rows and columns interpolations indices
	std::vector<size_t> get_interpolationRowsIndices_gTransformed(const size_t iRowDelegate, const size_t iiRowDelegate) const;
	
	//! a cell indices is <row,col> couple
	std::pair<size_t,size_t> get_lowerMatrixCell_From_upperMatrixCell(const std::pair<size_t,size_t>& gTransformedCellIndice) const;
	//std::pair<size_t,size_t> get_upperMatrixCell_From_lowerMatrixCell(const std::pair<size_t,size_t>& gCellIndice) const;

	void interpolate_gDelegate();
};

typedef boost::shared_ptr<GMatrixMapping> GMatrixMapping_PTR;