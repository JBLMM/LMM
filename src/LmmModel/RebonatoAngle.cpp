
#include <math.h>
#include <cassert>
#include <iostream>

#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/math/constants/constants.hpp>

#include <ql/math/optimization/endcriteria.hpp>
#include <ql/math/optimization/constraint.hpp>
#include <ql/math/optimization/problem.hpp>
#include <ql/math/optimization/simplex.hpp>
#include <ql/math/optimization/levenbergmarquardt.hpp>
#include <ql/math/optimization/conjugategradient.hpp>
#include <ql/math/matrixutilities/tapcorrelations.hpp>
#include <ql/math/matrixutilities/pseudosqrt.hpp>
//#include <ql/errors.hpp>

#include <LMM/LmmModel/PCA.h>
#include <LMM/LmmModel/RebonatoAngle.h>



using namespace QuantLib;

const double PI = boost::math::constants::pi<double>(); 

//! construct the matrix from Rebonato Angle
RebonatoAngle::RebonatoAngle(size_t matrixSize, size_t rank, const Array& angles) // Angle -> BMatrix, correlMatrix
	: matrixSize_(matrixSize), rank_(rank), angles_(angles), BMatrix_(matrixSize, rank)
{
	if(!checkAngle(angles))
		throw("Angles are not all in [0,PI]");

	calculateBMatrixFromAngle(angles_);
	correlMatrix_ = BMatrix_*transpose(BMatrix_);
}

//! construct the matrix from Rebonato Angle
RebonatoAngle::RebonatoAngle(const Matrix& BMatrix) // BMatrix -> Angle, correlMatrix
	: matrixSize_(BMatrix.rows()), rank_(BMatrix.columns()), angles_((rank_-1) * matrixSize_, 0.0), BMatrix_(BMatrix)
{
	if(matrixSize_< rank_ || rank_ <2)
		throw ("Error matrix size not permitted.");

	//! YY TDOO: ?????? need to also check that BMatrix is positively defined: require by the Rebonato angle representation.
	calculateAngleFromBMatrix(BMatrix_);

	correlMatrix_ = BMatrix_*transpose(BMatrix_);
}


//! given the originalCorrelMatrix then find the nearest approximation RebonatoAngle
RebonatoAngle::RebonatoAngle(const Matrix& originalCorrelMatrix, size_t reducedRank)
	: matrixSize_(originalCorrelMatrix.rows())
	, rank_(reducedRank)
	, angles_((rank_-1) * matrixSize_, 0.0)
	, correlMatrix_(originalCorrelMatrix) // need to change it latter to the approximated correlMatrix.
{
	//! PCA to find the BMatrix_
	bool normalizeDiagonal = true;
	BMatrix_ = PCA::doPCA(originalCorrelMatrix, reducedRank, normalizeDiagonal);

	std::string f1 = "E://1.csv";
	std::string f2 = "E://2.csv";
	std::string f3 = "E://3.csv";
	print_details(f1);
	//! using PCA result -> starting point of RebonatoAngle.
	calculateAngleFromBMatrix(BMatrix_);
	print_details(f2);

	//! Optimization: iterate the RebonatoAngle
	solveTheNearestCorrelProblem(originalCorrelMatrix);

	//! update BMatrix_ & construct correlMatrix_ (!= originalCorrelMatrix)
	calculateBMatrixFromAngle(angles_);
	print_details(f3);
	correlMatrix_ = BMatrix_*transpose(BMatrix_);
}

void RebonatoAngle::solveTheNearestCorrelProblem(const Matrix& target)
{
	//! test the dim of target matrix
	if(target.rows()!=matrixSize_ || target.columns()!=matrixSize_ )
		throw("target does not have proper dim.");

	//! correlMatrix_: correl matrix to approximater
	//! angles_: first guess

	//! To creat the optimization problem and then to solve it ... 
	Size maxIterations = 1000;
	Size minStatIterations = 500;
	Real rootEpsilon =1e-8;
	Real functionEpsilon =1e-9;
	Real gradientNormEpsilon =1e-5;

	EndCriteria myEndCrit ( maxIterations ,
		minStatIterations ,
		rootEpsilon ,
		functionEpsilon ,
		gradientNormEpsilon );
	boost::function<Disposable<Matrix>(const Array&, size_t, size_t)> f = QuantLib::triangularAnglesParametrization;
	RebonatoAngle::FrobeniusCostFunctionYY myFunc(target,
		RebonatoAngle::triangularAnglesParametrizationYY,// QuantLib::triangularAnglesParametrization,
		matrixSize_,
		rank_);

	NoConstraint constraint ;
	Problem myProb ( myFunc , constraint , angles_);
	//ConjugateGradient solver ; // give L2 norm, optimization result will be quite similar to PCA!
	LevenbergMarquardt solver ;

	EndCriteria :: Type solvedCrit1 = solver . minimize ( myProb , myEndCrit ); // angles_ update at each iteration ... 
	//YYYYYYYYYYYYYYYYYYY
	angles_ = myProb . currentValue ();

	//LevenbergMarquardt solver ;
	//EndCriteria :: Type solvedCrit = solver . minimize (myProb , myEndCrit );
}

//void RebonatoAngle::calculateBMatrixFromAngle(const Array& angles)
//{
//	if(!checkAngle(angles))
//		throw("Angles are not all in [0,PI]");
//	Array anglesCopy(angles);
//	Matrix m = QuantLib::triangularAnglesParametrization(anglesCopy, matrixSize_, rank_);  //YY : shit QuantLib algo, return some shit matrix, need to modify it.
//		
//	// taking only the first rank clomuns 
//	for(size_t_t i=0; i<BMatrix_.rows(); ++i)
//	{
//		for(size_t_t j=0; j<BMatrix_.columns(); ++j)
//		{
//			BMatrix_[i][j] = m[i][j];
//		}
//	}
//}


void RebonatoAngle::calculateBMatrixFromAngle(const Array& angles)
{
	// what if rank == 1?
	//QL_REQUIRE((rank_-1) * (matrixSize_ ) == angles.size(),
	//            "rank-1) * (matrixSize ) == angles.size()");

	assert((rank_-1) * (matrixSize_ ) == angles.size());

	size_t k = 0; //angles index
	for (size_t i=0; i<matrixSize_; ++i) {
		Real sinProduct = 1.0;
		for (size_t j=0; j<rank_-1; ++j) {
			BMatrix_[i][j] = std::cos(angles[k]);
			BMatrix_[i][j] *= sinProduct;
			sinProduct *= std::sin(angles[k]);
			++k;
		}
		BMatrix_[i][rank_-1] = sinProduct;
	}
}

void RebonatoAngle::calculateAngleFromBMatrix(const Matrix& BMatrix)
{
	if(BMatrix.rows() != matrixSize_ || BMatrix.columns() != rank_)
		throw("Error matrix size dismatch.");

	Matrix m(BMatrix);
	size_t k = 0; // angle index
	for(size_t i=0; i<matrixSize_; ++i)    
	{
		Real sinProduct = 1.0;
		for(size_t j=0; j<rank_-1; ++j)    
		{
			m[i][j] /= sinProduct;
			angles_[k] = std::acos(m[i][j]);				
			sinProduct *= std::sin(angles_[k]);
			++k;
		}
		//! last one need to check the sign				
		if(m[i][rank_-1]*sinProduct < 0) // i.e test if original sinProduct and this->sinProduct () has the same sign! ---- their differenc is the last angle!
			angles_[k-1] += 2*(PI-angles_[k-1]);
	}

	//! check the result.
	if(!checkAngle(angles_))
		throw("Angles are not all in [0,PI]");
}

bool RebonatoAngle::isSpecialAnlge(size_t index_i) const
{
	return isSpecialAnlge(index_i, rank_);
}

bool RebonatoAngle::checkAngle(const Array& angles) const
{
	double PI = boost::math::constants::pi<double>();
	for(size_t i=0; i<angles.size(); ++i)
	{
		if(!isSpecialAnlge(i))
		{
			if(angles[i]<0 || angles[i]>PI)
				return false;
		}
		else
		{
			if(angles[i]<0 || angles[i]>2*PI)
				return false;
		}
	}
	return true;
}

bool RebonatoAngle::isSpecialAnlge(size_t index_i, size_t rank)
{
	if(rank<2)
		throw("rank too small, not permitted");
	if(index_i ==0) 
		return false;
	size_t r = rank-1; 

	if((index_i+1)%r==0)
		return true;
	else
		return false;
}




////! all angles in [0,pi] expect esp angles \in [0,2*pi]
//static bool isSpecialAnlge(Size index_i, Size rank)
//{
//	if(rank<2)
//		throw("rank too small, not permitted");
//	if(index_i ==0) 
//		return false;
//	size_t r = rank-1; 

//    size_t pivotIndex = (r+1)*r/2-1;

//	if(index_i < pivotIndex)
//	{
//	    for(size_t k=2; k<=r; ++k)
//		{
//		    size_t criticIndex = (k+1)*k/2-1;
//			if(index_i == criticIndex)
//				return true;
//			if(index_i < criticIndex)
//				return false;
//		}
//	}
//	else
//	{
//	    if((index_i-pivotIndex)%r==0)
//			return true;
//		else 
//			return false;
//	}
//}

QuantLib::Disposable<QuantLib::Matrix> RebonatoAngle::triangularAnglesParametrizationYY( const QuantLib::Array& angles,
																						size_t matrixSize,
																						size_t rank)
{
	// what if rank == 1?
	//QL_REQUIRE((rank-1) * (matrixSize ) == angles.size(),
	//			"rank-1) * (matrixSize ) == angles.size()");

	assert((rank-1) * (matrixSize ) == angles.size());

	QuantLib::Matrix BMatrix(matrixSize, rank);

	size_t k = 0; //angles index
	for (size_t i=0; i<matrixSize; ++i) {
		double sinProduct = 1.0;
		for (size_t j=0; j<rank-1; ++j) {
			BMatrix[i][j] = std::cos(angles[k]);
			BMatrix[i][j] *= sinProduct;
			sinProduct *= std::sin(angles[k]);
			++k;
		}
		BMatrix[i][rank-1] = sinProduct;
	}
	return BMatrix;
}


//Real RebonatoAngle::FrobeniusCostFunctionYY::value(const Array& x) const {
//    Array temp = values(x);
//    return DotProduct(temp, temp);
//}
//
//
//Disposable<Array> RebonatoAngle::FrobeniusCostFunctionYY::values(const Array& x) const {
//    Array result((target_.rows()*(target_.columns()-1))/2);
//    Matrix pseudoRoot = f_(x, matrixSize_, rank_);
//    Matrix differences = pseudoRoot * transpose(pseudoRoot) - target_;
//    Size k = 0;
//    // then we store the elementwise differences in a vector.
//    for (Size i=0; i<target_.rows(); ++i) {
//        for (Size j=0; j<i; ++j){
//            result[k] = differences[i][j];
//            ++k;
//        }
//    }
//    return result;
//}


//! used by Conjugate gradiant
Real RebonatoAngle::FrobeniusCostFunctionYY::value(const Array& x) const {
	Array temp = values(x);
	double error = DotProduct(temp, temp);
	std::cout << error << std::endl;
	return error;
}

//! used by Levenberg Marquart
Disposable<Array> RebonatoAngle::FrobeniusCostFunctionYY::values(const Array& x) const 
{
	//! because the matrix is symetric, only need to do half of them :) 
	Array result((target_.rows()*(target_.columns()-1))/2);
	Matrix pseudoRoot = f_(x, matrixSize_, rank_);
	Matrix correl = pseudoRoot * transpose(pseudoRoot);
	Matrix differences = correl - target_;
	Size k = 0;
	// then we store the elementwise differences in a vector.
	for (Size i=0; i<target_.rows(); ++i) {
		for (Size j=0; j<i; ++j)
		{			
			//double weight = (std::abs((double)i-(double)j)+1);
			//if(i-j<20 || j-i<20)
			//	weight = 0;
			result[k] = std::abs(differences[i][j])/correl[i][j];//*weight*weight;
			++k;
		}
	}
	return result;
}

void RebonatoAngle::print_details(std::string& fileName) const
{
	std::ofstream myfile;
	myfile.open(fileName.c_str());

	myfile << "Angles" << std::endl;
	for(size_t i=0; i<angles_.size(); ++i)
	{
		myfile << angles_[i] << std::endl;
	}
	myfile << std::endl << std::endl;


	myfile << "BMatrix" << std::endl;
	for(size_t i=0; i<BMatrix_.rows(); ++i)
	{
		for(size_t j=0; j<BMatrix_.columns(); ++j)
		{
			myfile << BMatrix_[i][j] << ",";
		}
		myfile << std::endl;
	}
	myfile << std::endl << std::endl;


	myfile << "correlMatrix = BMatrix*BMatrix^T" << std::endl;
	for(size_t i=0; i<correlMatrix_.rows(); ++i)
	{
		for(size_t j=0; j<correlMatrix_.columns(); ++j)
		{
			myfile << correlMatrix_[i][j] << ",";
		}
		myfile << std::endl;
	}
	myfile << std::endl << std::endl; 
}