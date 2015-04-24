#define BOOST_TEST_MODULE LMM_unit_test_model_PCA
#include <boost/test/included/unit_test.hpp>

#include <iostream>

#include <ql/math/matrixutilities/symmetricschurdecomposition.hpp>

#include <LMM/ModelMCLMM/PCA.h>

using namespace QuantLib; // for Matrix

BOOST_AUTO_TEST_SUITE(lmm_test_model_PCA)

BOOST_AUTO_TEST_CASE(test_SymmetricSchurDecomposition)
{
	
	size_t size = 3;

	Matrix m(size,size);
	m[0][0] = 1;
	m[1][1] = 1;
	m[2][2] = 1;

	m[0][1] = 0.5;
	m[1][0] = 0.5;

	m[0][2] = 0.25;
	m[2][0] = 0.25;

	m[1][2] = 0.75;
	m[2][1] = 0.75;

	std::cout << "original matrix :" << std::endl;
	for(size_t i=0; i<m.rows(); ++i)
	{
		for(size_t j=0; j<m.columns(); ++j)
		{
			std::cout << m[i][j] << " - ";
		}
		std::cout << std::endl;
	}

	//if(originalMatrix.)
	SymmetricSchurDecomposition ssd(m);

	//! eigenvalue
	Array eigenvalues = ssd.eigenvalues();
	std::cout << "print eigenvalues:" << std::endl;
	for(size_t i=0; i<eigenvalues.size(); ++i)
	{
		std::cout << eigenvalues[i] << std::endl;
	}

	//! eigenvector
	Matrix eigenvectors = ssd.eigenvectors();
	std::cout << "print eigenvectors:" << std::endl;
	for(size_t i=0; i<eigenvectors.rows(); ++i)
	{
		for(size_t j=0; j<eigenvectors.columns(); ++j)
		{
			std::cout << eigenvectors[i][j] << "  ";
		}
		std::cout << std::endl;
	}

	//! reproduce original matrix
	Matrix U = eigenvectors;
	Matrix U_transpose = transpose(eigenvectors);
	Matrix D(U.rows(), U.columns(), 0.0);
	for(size_t i=0; i<D.rows(); ++i)
		D[i][i] = eigenvalues[i];

	Matrix finalMatrix = U*D*U_transpose;

	std::cout << "reconstruct the matrix :" << std::endl;
	for(size_t i=0; i<finalMatrix.rows(); ++i)
	{
		for(size_t j=0; j<finalMatrix.columns(); ++j)
		{
			std::cout << finalMatrix[i][j] << "  ";
		}
		std::cout << std::endl;
	}

	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(test_PCA)
{
	
	size_t fullRank = 3;
	size_t reducedRank = 2;

	Matrix m(fullRank,fullRank);
	m[0][0] = 1;
	m[1][1] = 1;
	m[2][2] = 1;

	m[0][1] = 0.5;
	m[1][0] = 0.5;

	m[0][2] = 0.25;
	m[2][0] = 0.25;

	m[1][2] = 0.75;
	m[2][1] = 0.75;

	std::cout << "original matrix :" << std::endl;
	for(size_t i=0; i<m.rows(); ++i)
	{
		for(size_t j=0; j<m.columns(); ++j)
		{
			std::cout << m[i][j] << "  ";
		}
		std::cout << std::endl;
	}


	bool useNormalizeDiagonal = true;
	Matrix BMatrix = PCA::doPCA(m, fullRank, useNormalizeDiagonal);
	Matrix reducedMatrix = BMatrix*transpose(BMatrix);

	std::cout << "reconstruct the matrix with full rank:" << std::endl;
	for(size_t i=0; i<reducedMatrix.rows(); ++i)
	{
		for(size_t j=0; j<reducedMatrix.columns(); ++j)
		{
			std::cout << reducedMatrix[i][j] << "  ";
		}
		std::cout << std::endl;
	}

	bool useNormalizeDiagonal2 = false;
	Matrix BMatrix2 = PCA::doPCA(m, reducedRank, useNormalizeDiagonal2);
	Matrix reducedMatrix2 = BMatrix2*transpose(BMatrix2);

	std::cout << "reconstruct the matrix with reduced rank = 2:" << std::endl;
	for(size_t i=0; i<reducedMatrix2.rows(); ++i)
	{
		for(size_t j=0; j<reducedMatrix2.columns(); ++j)
		{
			std::cout << reducedMatrix2[i][j] << "  ";
		}
		std::cout << std::endl;
	}

	bool useNormalizeDiagonal3 = true;
	Matrix BMatrix3 = PCA::doPCA(m, reducedRank, useNormalizeDiagonal3);
	Matrix reducedMatrix3 = BMatrix3*transpose(BMatrix3);

	std::cout << "reconstruct the matrix with reduced rank = 3 and normalize the diagonal:" << std::endl;
	for(size_t i=0; i<reducedMatrix3.rows(); ++i)
	{
		for(size_t j=0; j<reducedMatrix3.columns(); ++j)
		{
			std::cout << reducedMatrix3[i][j] << "  ";
		}
		std::cout << std::endl;
	}

	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_SUITE_END()