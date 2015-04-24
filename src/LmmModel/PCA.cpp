#include <iostream>
#include <cassert>

#include <ql/math/matrixutilities/symmetricschurdecomposition.hpp>
#include <ql/math/array.hpp>

#include <LMM/LmmModel/PCA.h>


using namespace QuantLib; // for Matrix.

//! YY TODO: Now prefer to keep the algorithm clear, so it is highly inefficient. To improve latter. 
Matrix PCA::doPCA(const Matrix& originalMatrix, size_t reducedRank, bool normalizeDiagonal) // YY TODO: change it to boost::optional<bool>
{
	size_t fullRank = originalMatrix.rows(); 

	assert(fullRank > reducedRank);

	//! decompose the original matrix
	SymmetricSchurDecomposition ssd(originalMatrix);  // here it check if the matrix is squared, symetric.
	Array eigenvalues = ssd.eigenvalues();
	Matrix eigenvectors = ssd.eigenvectors();

	assert(checkEigenvalue(eigenvalues, reducedRank));

	//! construct the reducedRank matrix	   

	Array D(reducedRank);
	for(size_t i=0; i<D.size(); ++i)
	{
		D[i] = std::sqrt(eigenvalues[i]);
	}
	//! normalize the diagonal of the reducedRank matrix.
	Array diagonalNormalizeFactor(fullRank);
	if(normalizeDiagonal)
	{
		for(size_t i=0; i<fullRank; ++i)
		{
			diagonalNormalizeFactor[i] = 0.0;
			for(size_t j=0; j<reducedRank; ++j)
			{
				diagonalNormalizeFactor[i] += eigenvalues[j]*eigenvectors[i][j]*eigenvectors[i][j];
			}
			diagonalNormalizeFactor[i] = sqrt(diagonalNormalizeFactor[i]);
		}
	}
	else
	{
		for(size_t i=0; i<fullRank; ++i)
		{
			diagonalNormalizeFactor[i] = 1.0;
		}
	}

	Matrix U(fullRank,reducedRank,0.0);
	for(size_t i=0; i<fullRank; ++i)
	{
		for(size_t j=0; j<reducedRank; ++j)
		{
			U[i][j] = eigenvectors[i][j]*D[j]/diagonalNormalizeFactor[i];
		}
	}

	return U;
}

bool PCA::checkEigenvalue(Array& eigenvalues, size_t reducedRank)
{ 
	if(eigenvalues.size() < reducedRank)
	{
		return false;
	}

	//! check eigenvalues is in decreasing order.
	for(size_t i=1; i<eigenvalues.size(); ++i)
	{
		if(eigenvalues[i-1] < eigenvalues[i]) 
			return false;
	}

	//! check the first N=recudecRank eigenvalues are positive (=> reducedMatrix is postively defined)
	for(size_t i=0; i<reducedRank; ++i)
	{
		if(eigenvalues[i] < 0.0)
			return false;
	}

	return true;
}

