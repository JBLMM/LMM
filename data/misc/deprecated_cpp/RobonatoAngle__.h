//#pragma once
//
////! YY shit should not include so much file in .h, should not use namespace QuantLib in .h !!!! 
//#include <ql/math/matrixutilities/tapcorrelations.hpp>
//#include <boost/random/uniform_real_distribution.hpp>
//#include <ql/math/matrixutilities/pseudosqrt.hpp>
//#include <boost/math/constants/constants.hpp>
//#include <boost/random/variate_generator.hpp>
//#include <boost/random/mersenne_twister.hpp>
//
////"Parameterizing correlations: a geometric interpretation"
//#include <string>
//#include <fstream>
//#include <math.h>
//
//using namespace QuantLib;
//
////! YY not efficient at all, memory usage or algorithm. need to improve.
////! all angles in [0,pi] expect esp angles \in [0,2*pi]
//class RobonatoAngle
//{
//	Size   matrixSize_;   // fullRank
//	Size   rank_;         // reducecRank: >=2
//	Array  angles_;       // valide range [0,PI] and for specailAngle [0,2*PI]
//	Matrix BMatrix_;      // size = (matrixSize_, rank_)
//	Matrix correlMatrix_; // correlMatrix_ = BMatrix_*BMatrix_^T
//
//public:
//	//! default constructor: does nothing. 
//	RobonatoAngle(){};
//
//	//! construct the matrix from Robonato Angle
//	RobonatoAngle(Size matrixSize, Size rank, const Array& angles); // Angle -> BMatrix, correlMatrix
//
//	//! construct the matrix from Robonato Angle
//	RobonatoAngle(const Matrix& BMatrix);// BMatrix -> Angle, correlMatrix
//
//	//! given the originalCorrelMatrix then find the nearest approximation RobonatoAngle
//	RobonatoAngle(const Matrix& originalCorrelMatrix, Size reducedRank);  // the finally saved correlMatrix_ is not the originalCorrelMatrix.
//
//
//	void calculateBMatrixFromAngle(const Array& angles);
//	void calculateAngleFromBMatrix(const Matrix& BMatrix);
//
//	Array  get_angles() const {return angles_;}
//	Matrix get_BMatrix() const {return BMatrix_;}
//	Matrix get_correlMatrix() const {return correlMatrix_;}
//
//	bool isSpecialAnlge(Size index_i) const;
//
//	bool checkAngle(const Array& angles) const;
//
//	//! all angles in [0,pi] expect esp angles \in [0,2*pi]
//	static bool isSpecialAnlge(Size index_i, Size rank)
//	{
//		if(rank<2)
//			throw("rank too small, not permitted");
//		if(index_i ==0) 
//			return false;
//		size_t r = rank-1; 
//
//	    size_t pivotIndex = (r+1)*r/2-1;
//
//		if(index_i < pivotIndex)
//		{
//		    for(size_t k=2; k<=r; ++k)
//			{
//			    size_t criticIndex = (k+1)*k/2-1;
//				if(index_i == criticIndex)
//					return true;
//				if(index_i < criticIndex)
//					return false;
//			}
//		}
//		else
//		{
//		    if((index_i-pivotIndex)%r==0)
//				return true;
//			else 
//				return false;
//		}
//	}
//
//
//	//! nearest Matrix to originalCorrelMatrix
//	void solveTheNearestCorrelProblem(const Matrix& target); // given: correlMatrix_ -> try to find its nearest approximation by RobonatoAngle
//
//
//	void print_details(std::string& fileName) const;
//
//	
//    //! for optimization
//    // Cost function associated with Frobenius norm.
//    // <http://en.wikipedia.org/wiki/Matrix_norm>
//    class FrobeniusCostFunctionYY : public CostFunction
//	{
//      public:
//        FrobeniusCostFunctionYY(const Matrix& target,
//						      const boost::function<Disposable<Matrix>(const Array&,
//																	   Size,
//																	   Size)>& f,
//							  Size matrixSize,
//							  Size rank)
//        : target_(target), f_(f), matrixSize_(matrixSize), rank_(rank) {}
//        Real value (const Array &x) const;
//        Disposable<Array> values (const Array &x) const;
//      private:
//        Matrix target_;
//        boost::function<Disposable<Matrix>(const Array&, Size, Size)> f_;
//        Size matrixSize_;
//        Size rank_;
//    };
//
//};
//
//
//
////! function of test
//void testSpecialAngles();
//
//void testRobonatoAngle();
