#pragma once

#include <vector>
#include <ql/math/array.hpp>
#include <boost/numeric/ublas/triangular.hpp>


typedef boost::numeric::ublas::matrix<double> UpperTriangularDoubleMatrix;//to change
typedef boost::numeric::ublas::matrix<std::pair<size_t, size_t> > UpperTriangularIndexPairMatrix;

/*! \class UpperTriangularMatrixBlockInterpolator
*
* This class implement specially for the upper anti triangular matrix, used for gTransformed interpolation
* 
* - The matrix to be interpolated will note count first column and row
* - Every use of cell out of upper part will break the interpolator
*
* The Interpolator implement linear interpolation for "primitives"
*
* - row segment
* - col segment
* - diag segment
* - rectangle block
* - sub upper triangular block
* - extrapolate in diagonal direction of top margin, left margin and the block top-left 
*
* NOTE : these interpolation, extrapolation algorithm implement is associated with the "cascade calibration" algorithm.
*        That provide all smallest primitive for interpolating the gTransformed matrix in the GMatrixMapping
*/
class UpperTriangularMatrixBlockInterpolator
{
public:

	//! constructor
	UpperTriangularMatrixBlockInterpolator(UpperTriangularDoubleMatrix & matrix) : matrix_(matrix) { assert(matrix_.size1() == matrix_.size2()); }

	UpperTriangularDoubleMatrix & get(){ return matrix_;}

	//! extrapolate all constant from the top-left corner cell
	void extrapolate_TopLeftCorner(const size_t rowNODE, const size_t colNODE);

	//! extrapolate in the diagonal direction the top margin part
	void extrapolate_Top(const size_t rowNODE, const size_t leftColNODE, const size_t rightColNODE);

	//! extrapolate in the diagonal direction the left margin part
	void extrapolate_Left(const size_t colNODE, const size_t topRowNODE , const size_t bottomRowNODE);

	//! an sub upper triangle is totally determined by a square, the diag is bottomleft to topright of square
	void interpolate_SubUpperTriangle(const size_t topRowNODE, const size_t bottomRowNODE, const size_t leftColNODE, const size_t rightColNODE);

	//! an sub upper triangle is totally determined by a square, the diag is bottomleft to topright of square
	void interpolate_SubLowerTriangle(const size_t topRowNODE, const size_t bottomRowNODE, const size_t leftColNODE, const size_t rightColNODE);

	//! an sub regtangle is totally determined  bottomleft to topright
	void interpolate_SubRectangle(const size_t topRowNODE, const size_t bottomRowNODE, const size_t leftColNODE, const size_t rightColNODE);

	//! and parallelogram is determined by 6 values
	void interpolate_Parallelogram( const size_t topRowNODE, const size_t bottomRowNODE
		, const size_t leftleftColNODE , const size_t leftColNODE
		, const size_t rightColNODE, const size_t rightrightColNODE);

	void interpolate_ROW(const size_t rowNODE, const size_t leftColNODE, const size_t rightColNODE);
	void interpolate_COL(const size_t colNODE, const size_t topRowNODE , const size_t bottomRowNODE);

	//! an diag segment is totally determined by a square, the diag is bottomleft to topright of square
	void interpolate_SubDiag(const size_t topRowNODE, const size_t bottomRowNODE, const size_t leftColNODE, const size_t rightColNODE);

	void interpolate(const UpperTriangularIndexPairMatrix& indices_mapper);

protected :

	UpperTriangularDoubleMatrix & matrix_;

	//! diag count from small to large
	//! the largest diag is the last diag	//! diag i has i Elements	//! there are no diag[0]
	size_t get_NB_SubDiag() const { return matrix_.size1()-2; }

	//! give a cell, get the diag ID
	//! NOTE : number of element on a sub diag equal to diag ID
	size_t get_SubDiag_ID(const size_t rowNODE, const size_t colNODE) const;

	double segment_interplation(const double & lowvalue, const double& highvalue, const size_t lowIndex, const size_t highIndex, const size_t interpolIndex) const;	
};


/*! \class UpperTriangularMatrixColRowInterpolator
*
*/
class UpperTriangularMatrixColRowInterpolator : public UpperTriangularMatrixBlockInterpolator
{
public:
	UpperTriangularMatrixColRowInterpolator(UpperTriangularDoubleMatrix & matrix):UpperTriangularMatrixBlockInterpolator(matrix){}

	/*! interpolate upper triangular matrix
	*  Ignore first row and first column
	*
	*/
	void interpolate( const std::vector<size_t> & row_indices_missed
					, const std::vector<size_t> & col_indices_missed);
};