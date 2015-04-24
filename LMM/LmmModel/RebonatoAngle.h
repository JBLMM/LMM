#pragma once

//! YY shit should not include so much file in .h, should not use namespace QuantLib in .h !!!! 


//#include <ql/errors.hpp>  // for QL_REQUIRE


//"Parameterizing correlations: a geometric interpretation"
#include <string>
#include <fstream>
#include <math.h>
#include <cassert>

#include <boost/function.hpp>

#include <ql/math/optimization/costfunction.hpp>
#include <ql/math/array.hpp>
#include <ql/math/matrix.hpp>
#include <ql/utilities/disposable.hpp>
//ctnrefactor move to numeric
//using namespace QuantLib;

//! YY not efficient at all, memory usage or algorithm. need to improve.
//! all angles in [0,pi] expect esp angles \in [0,2*pi]
class RebonatoAngle
{
	size_t   matrixSize_;   // fullRank
	size_t   rank_;         // reducecRank: >=2
	QuantLib::Array  angles_;       // valide range [0,PI] and for specailAngle [0,2*PI]
	QuantLib::Matrix BMatrix_;      // size = (matrixSize_, rank_)
	QuantLib::Matrix correlMatrix_; // correlMatrix_ = BMatrix_*BMatrix_^T

public:
	//! default constructor: does nothing. 
	RebonatoAngle(){};

	//! construct the matrix from Rebonato Angle
	RebonatoAngle(size_t matrixSize, size_t rank, const QuantLib::Array& angles); // Angle -> BMatrix, correlMatrix

	//! construct the matrix from Rebonato Angle
	RebonatoAngle(const QuantLib::Matrix& BMatrix);// BMatrix -> Angle, correlMatrix

	//! given the originalCorrelMatrix then find the nearest approximation RebonatoAngle
	RebonatoAngle(const QuantLib::Matrix& originalCorrelMatrix, size_t reducedRank);  // the finally saved correlMatrix_ is not the originalCorrelMatrix.


	void calculateBMatrixFromAngle(const QuantLib::Array& angles);
	void calculateAngleFromBMatrix(const QuantLib::Matrix& BMatrix);

	QuantLib::Array  get_angles() const {return angles_;}
	QuantLib::Matrix get_BMatrix() const {return BMatrix_;}
	QuantLib::Matrix get_correlMatrix() const {return correlMatrix_;}

	bool isSpecialAnlge(size_t index_i) const;

	bool checkAngle(const QuantLib::Array& angles) const;

	//! all angles in [0,pi] expect esp angles \in [0,2*pi]
	static bool isSpecialAnlge(size_t index_i, size_t rank);


	//! nearest Matrix to originalCorrelMatrix
	void solveTheNearestCorrelProblem(const QuantLib::Matrix& target); // given: correlMatrix_ -> try to find its nearest approximation by RebonatoAngle

	// YY Duplicate: RebonatoAngle::calculateBMatrixFromAngle(const Array& angles)
	static QuantLib::Disposable<QuantLib::Matrix> triangularAnglesParametrizationYY( const QuantLib::Array& angles,
															   size_t matrixSize,
															   size_t rank);


	
    //! for optimization
    // Cost function associated with Frobenius norm.
    // <http://en.wikipedia.org/wiki/Matrix_norm>
    class FrobeniusCostFunctionYY : public QuantLib::CostFunction
	{
      public:
        FrobeniusCostFunctionYY(const QuantLib::Matrix& target,
						        const boost::function<QuantLib::Disposable<QuantLib::Matrix>(const QuantLib::Array&,
																	   size_t,
																	   size_t)>& f,
							  size_t matrixSize,
							  size_t rank)
        : target_(target), f_(f), matrixSize_(matrixSize), rank_(rank) {}
        double value (const QuantLib::Array &x) const;
        QuantLib::Disposable<QuantLib::Array> values (const QuantLib::Array &x) const;
      private:
        QuantLib::Matrix target_;
        boost::function<QuantLib::Disposable<QuantLib::Matrix>(const QuantLib::Array&, size_t, size_t)> f_;
        size_t matrixSize_;
        size_t rank_;
    };

	void print_details(std::string& fileName) const;
};