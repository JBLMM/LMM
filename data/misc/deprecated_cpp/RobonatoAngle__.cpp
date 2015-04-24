//
//#include "RobonatoAngle.h"

////#include <ql/types.hpp>
//
////#include <ql/math/matrixutilities/tapcorrelations.hpp>
////#include <boost/random/uniform_real_distribution.hpp>
////#include <ql/math/matrixutilities/pseudosqrt.hpp>
////#include <boost/math/constants/constants.hpp>
////
//////"Parameterizing correlations: a geometric interpretation"
////#include <string>
////#include <fstream>
////#include <math.h>
////
////using namespace QuantLib;
//
//
//#include <ql\math\optimization\endcriteria.hpp>
//#include <ql\math\optimization\constraint.hpp>
//#include <ql\math\optimization\problem.hpp>
//#include <ql\math\optimization\simplex.hpp>
//#include <ql\math\optimization\levenbergmarquardt.hpp>
//#include <ql\math\optimization\conjugategradient.hpp>
////
////#include <ql/pricingengines/blackscholescalculator.hpp>  // for the optimization problem #include
////#include <boost/random/normal_distribution.hpp>
////#include <boost/math/distributions.hpp> // boost::math::cdf, pdf, ... 
////#include <boost/math/distributions/normal.hpp>
////#include <ql/types.hpp>
//
//#include "PCA.h"
//
//
//const double PI = boost::math::constants::pi<double>(); 
//
////! construct the matrix from Robonato Angle
//RobonatoAngle::RobonatoAngle(Size matrixSize, Size rank, const Array& angles) // Angle -> BMatrix, correlMatrix
//	:matrixSize_(matrixSize), rank_(rank), angles_(angles), BMatrix_(matrixSize, rank)
//{
//	if(!checkAngle(angles))
//		throw("Angles are not all in [0,PI]");
//
//	calculateBMatrixFromAngle(angles_);
//	correlMatrix_ = BMatrix_*transpose(BMatrix_);
//}
//
////! construct the matrix from Robonato Angle
//RobonatoAngle::RobonatoAngle(const Matrix& BMatrix) // BMatrix -> Angle, correlMatrix
//	:matrixSize_(BMatrix.rows()), rank_(BMatrix.columns()), angles_((rank_-1) * (2*matrixSize_ - rank_)/2, 0.0), BMatrix_(BMatrix)
//{
//	if(matrixSize_< rank_ || rank_ <2)
//		throw ("Error matrix size not permitted.");
//
//	//! YY TDOO: ?????? need to also check that BMatrix is positively defined: require by the Robonato angle representation.
//	calculateAngleFromBMatrix(BMatrix_);
//
//	correlMatrix_ = BMatrix_*transpose(BMatrix_);
//}
//
//
////! given the originalCorrelMatrix then find the nearest approximation RobonatoAngle
//RobonatoAngle::RobonatoAngle(const Matrix& originalCorrelMatrix, Size reducedRank)
//	:matrixSize_(originalCorrelMatrix.rows()),
//	 rank_(reducedRank), 
//	 angles_((rank_-1) * (2*matrixSize_ - rank_)/2, 0.0), 
//	 correlMatrix_(originalCorrelMatrix) // need to change it latter to the approximated correlMatrix.
//{
//     //! PCA to find the BMatrix_
//	 bool normalizeDiagonal = true;
//	 PCARETURNTYPE PCAReturnType = PCARETURNTYPE::MatrixB;
//	 BMatrix_ = PCA::doPCA(originalCorrelMatrix, reducedRank, normalizeDiagonal, PCAReturnType);
//
//	 std::string f1 = "E://1.csv";
//	 std::string f2 = "E://2.csv";
//	 std::string f3 = "E://3.csv";
//	 print_details(f1);
//	 //! using PCA result -> starting point of RobonatoAngle.
//	 //YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY
//	 calculateAngleFromBMatrix(BMatrix_);
//	 print_details(f2);
//	 //! Optimization: iterate the RobonatoAngle
//	 //solveTheNearestCorrelProblem(originalCorrelMatrix);
//
//	 //! update BMatrix_ & construct correlMatrix_ (!= originalCorrelMatrix)
//     calculateBMatrixFromAngle(angles_);
//	 print_details(f3);
//	 correlMatrix_ = BMatrix_*transpose(BMatrix_);
//}
//
//void RobonatoAngle::solveTheNearestCorrelProblem(const Matrix& target)
//{
//	 //! test the dim of target matrix
//	if(target.rows()!=matrixSize_ || target.columns()!=matrixSize_ )
//		throw("target does not have proper dim.");
//
//	 //! correlMatrix_: correl matrix to approximater
//	 //! angles_: first guess
//
//	 //! To creat the optimization problem and then to solve it ... 
//	Size maxIterations = 200;
//	Size minStatIterations = 100;
//	Real rootEpsilon =1e-8;
//	Real functionEpsilon =1e-9;
//	Real gradientNormEpsilon =1e-5;
//
//	EndCriteria myEndCrit ( maxIterations ,
//							minStatIterations ,
//							rootEpsilon ,
//							functionEpsilon ,
//							gradientNormEpsilon );
//	//boost::function<Disposable<Matrix>(const Array&, Size, Size)> f = QuantLib::triangularAnglesParametrization;
//	RobonatoAngle::FrobeniusCostFunctionYY myFunc(target,
//												  QuantLib::triangularAnglesParametrization,// QuantLib::triangularAnglesParametrization,
//												  matrixSize_,
//												  rank_);
//		                         
//	NoConstraint constraint ;
//	Problem myProb ( myFunc , constraint , angles_);
//	ConjugateGradient solver ;
//	//LevenbergMarquardt solver ;
//
//	EndCriteria :: Type solvedCrit1 = solver . minimize ( myProb , myEndCrit ); // angles_ update at each iteration ... 
//
//	//LevenbergMarquardt solver ;
//	//EndCriteria :: Type solvedCrit = solver . minimize (myProb , myEndCrit );
//}
//
//void RobonatoAngle::calculateBMatrixFromAngle(const Array& angles)
//{
//	if(!checkAngle(angles))
//		throw("Angles are not all in [0,PI]");
//	Array anglesCopy(angles);
//	Matrix m = QuantLib::triangularAnglesParametrization(anglesCopy, matrixSize_, rank_);  //YY : shit QuantLib algo, return some shit matrix, need to modify it.
//		
//	// taking only the first rank clomuns 
//	for(size_t i=0; i<BMatrix_.rows(); ++i)
//	{
//		for(size_t j=0; j<BMatrix_.columns(); ++j)
//		{
//			BMatrix_[i][j] = m[i][j];
//		}
//	}
//}
//
//void RobonatoAngle::calculateAngleFromBMatrix(const Matrix& BMatrix)
//{
//	if(BMatrix.rows() != matrixSize_ || BMatrix.columns() != rank_)
//		throw("Erro matrix size dismatch.");
//
//	Matrix m(BMatrix);
//	Size k = 0; // angle index
//	for(Size i=1; i<m.rows(); ++i)    
//	{
//		Real sinProduct = 1.0;
//		Size bound = std::min(i,rank_-1);
//		for(Size j=0; j<bound; ++j)    
//		{
//			m[i][j] /= sinProduct;
//			angles_[k] = std::acos(m[i][j]);				
//			sinProduct *= std::sin(angles_[k]);
//			++k;
//		}
//		//! last one need to check the sign				
//		if(m[i][bound]*sinProduct < 0) // i.e test if original sinProduct and this->sinProduct () has the same sign! ---- their differenc is the last angle!
//			angles_[k-1] += 2*(PI-angles_[k-1]);
//	}
//
//	//! check the result.
//	if(!checkAngle(angles_))
//		throw("Angles are not all in [0,PI]");
//}
//
//bool RobonatoAngle::isSpecialAnlge(Size index_i) const
//{
//	return isSpecialAnlge(index_i, rank_);
//}
//
//bool RobonatoAngle::checkAngle(const Array& angles) const
//{
//	double PI = boost::math::constants::pi<double>();
//	for(Size i=0; i<angles.size(); ++i)
//	{
//		if(!isSpecialAnlge(i))
//		{
//			if(angles[i]<0 || angles[i]>PI)
//				return false;
//		}
//		else
//		{
//			if(angles[i]<0 || angles[i]>2*PI)
//				return false;
//		}
//	}
//	return true;
//}
//
//
//
//
//
//////! all angles in [0,pi] expect esp angles \in [0,2*pi]
////static bool isSpecialAnlge(Size index_i, Size rank)
////{
////	if(rank<2)
////		throw("rank too small, not permitted");
////	if(index_i ==0) 
////		return false;
////	size_t r = rank-1; 
//
////    size_t pivotIndex = (r+1)*r/2-1;
//
////	if(index_i < pivotIndex)
////	{
////	    for(size_t k=2; k<=r; ++k)
////		{
////		    size_t criticIndex = (k+1)*k/2-1;
////			if(index_i == criticIndex)
////				return true;
////			if(index_i < criticIndex)
////				return false;
////		}
////	}
////	else
////	{
////	    if((index_i-pivotIndex)%r==0)
////			return true;
////		else 
////			return false;
////	}
////}
//
//void RobonatoAngle::print_details(std::string& fileName) const
//{
//	std::ofstream myfile;
//	myfile.open(fileName.c_str());
//
//	myfile << "Angles" << std::endl;
//	for(size_t i=0; i<angles_.size(); ++i)
//	{
//		myfile << angles_[i] << std::endl;
//	}
//	myfile << std::endl << std::endl;
//
//
//	myfile << "BMatrix" << std::endl;
//	for(size_t i=0; i<BMatrix_.rows(); ++i)
//	{
//		for(size_t j=0; j<BMatrix_.columns(); ++j)
//		{
//			myfile << BMatrix_[i][j] << ",";
//		}
//		myfile << std::endl;
//	}
//	myfile << std::endl << std::endl;
//
//
//	myfile << "correlMatrix = BMatrix*BMatrix^T" << std::endl;
//	for(size_t i=0; i<correlMatrix_.rows(); ++i)
//	{
//		for(size_t j=0; j<correlMatrix_.columns(); ++j)
//		{
//			myfile << correlMatrix_[i][j] << ",";
//		}
//		myfile << std::endl;
//	}
//	myfile << std::endl << std::endl; 
//}
//
//
//
////Real RobonatoAngle::FrobeniusCostFunctionYY::value(const Array& x) const {
////    Array temp = values(x);
////    return DotProduct(temp, temp);
////}
////
////
////Disposable<Array> RobonatoAngle::FrobeniusCostFunctionYY::values(const Array& x) const {
////    Array result((target_.rows()*(target_.columns()-1))/2);
////    Matrix pseudoRoot = f_(x, matrixSize_, rank_);
////    Matrix differences = pseudoRoot * transpose(pseudoRoot) - target_;
////    Size k = 0;
////    // then we store the elementwise differences in a vector.
////    for (Size i=0; i<target_.rows(); ++i) {
////        for (Size j=0; j<i; ++j){
////            result[k] = differences[i][j];
////            ++k;
////        }
////    }
////    return result;
////}
//
//
//Real RobonatoAngle::FrobeniusCostFunctionYY::value(const Array& x) const {
//	Array temps = values(x);
//	return temps[0];
//}
//
//
//Disposable<Array> RobonatoAngle::FrobeniusCostFunctionYY::values(const Array& x) const 
//{
//    Matrix pseudoRoot = f_(x, matrixSize_, rank_);
//    Matrix differences = pseudoRoot * transpose(pseudoRoot) - target_;
//	size_t N = matrixSize_*(matrixSize_-1)/2;
//	double error = 0.0;
//    // then we store the elementwise differences in a vector.
//
//    for (Size i=0; i<target_.rows(); ++i) {
//        for (Size j=0; j<i; ++j){
//            error  += differences[i][j]*differences[i][j];
//        }
//    }
//	error += sqrt(error)/matrixSize_;
//
//	Array errorArray(1,error); 
//    return errorArray;
//}
//
//
//
//
//
////--------------------------------------------------------------
////
////                          Test
////
////--------------------------------------------------------------
//
//void testSpecialAngles()
//{
//	size_t matrixSize = 40;
//	size_t rank = 3;
//	size_t angleSize = (rank-1) * (2*matrixSize - rank)/2;
//	Array angles(angleSize,0.25);
//    RobonatoAngle ra(matrixSize, rank, angles);
//	
//	for(size_t i=0; i<angleSize; ++i)
//	{
//	    std::cout << "index [" << i << "] is special ? " << ra.isSpecialAnlge(i) << std::endl;
//	}
//}
//
//
//void test4RandRA()
//{
//	Matrix B(4,2);
//
//
//	//B[0][0] = 1.0;
//	//B[0][1] = 0.125777;
//	//B[0][2] = 0.0445788;
//	//B[0][3] = 0.0158;
//
//	//B[1][0] = 0.125777;
//	//B[1][1] = 1.0;
//	//B[1][2] = 0.354429;
//	//B[1][3] = 0.12562;
//
//	//B[2][0] = 0.0445788;
//	//B[2][1] = 0.354429;
//	//B[2][2] = 1.0;
//	//B[2][3] = 0.354429;
//
//	//B[3][0] = 0.0158;
//	//B[3][1] = 0.12562;
//	//B[3][2] = 0.354429;
//	//B[3][3] = 1.0;
//
//		//1	0.125777	0.0445788	0.0158
//	//0.125777	1	0.354429	0.12562
//	//0.0445788	0.354429	1	0.354429
//	//0.0158	0.12562	0.354429	1
//	
//	B[0][0] = 1.0;
//	B[0][1] = 0;
//
//	B[1][0] = 0.930984;
//	B[1][1] = 0.36506;
//
//	B[2][0] = 0.981581;
//	B[2][1] = -0.191045;
//
//	B[3][0] = 0.85686;
//	B[3][1] = -0.51555;
//
//
//
//	//1	0
//	//0.930984	0.36506
//	//0.981581	-0.191045
//	//0.85686	-0.51555
//
//
//	
//	RobonatoAngle rbAngle1(B);
//	std::string fileName1 = "E:\\4Rank_RobonatoAngle_1.csv";
//	rbAngle1.print_details(fileName1);
//
//	
//	RobonatoAngle rbAngle2(4, 2, rbAngle1.get_angles());
//	std::string fileName2 = "E:\\4Rank_RobonatoAngle_2.csv";
//	rbAngle2.print_details(fileName2);
//
//}
//
//void testRobonatoAngle()
//{
//	//testSpecialAngles();
//
//	test4RandRA();
//
//	//! angles -> BMatrix, correlMatrix
//	size_t matrixSize = 40;
//	size_t rank = 3;
//	size_t angleSize = (rank-1) * (2*matrixSize - rank)/2;
//
//	Array angles(angleSize);
//	boost::variate_generator<boost::mt19937, boost::random::uniform_real_distribution<> >
//		generator(boost::mt19937(0.5),boost::random::uniform_real_distribution<>()); // uniform generator [0, 1]
//
//
//	double PI = boost::math::constants::pi<double>();
//
//	for(size_t i=0; i<angleSize; ++i)
//	{
//		if(RobonatoAngle::isSpecialAnlge(i,rank))
//			angles[i] = generator()*2*PI; // [0,2*PI]
//		else
//			angles[i] = generator()*PI;   // [0,PI]
//	}
//
//	RobonatoAngle rbAngle1(matrixSize, rank, angles);
//	std::string fileName1 = "E:\\Random_RobonatoAngle1.csv";
//	rbAngle1.print_details(fileName1);
//
//
//	//! BMAtrix -> angles, correlMatrix  // Test is passed when the input and output angle are the same.
//	RobonatoAngle rbAngle2(rbAngle1.get_BMatrix());
//	std::string fileName2 = "E:\\Random_RobonatoAngle2.csv";
//	rbAngle2.print_details(fileName2);
//
//	//! BMAtrix -> angles, correlMatrix  // Test is passed when the input and output angle are the same.
//	RobonatoAngle rbAngle3(matrixSize, rank, rbAngle2.get_angles());
//	std::string fileName3 = "E:\\Random_RobonatoAngle3.csv";
//	rbAngle3.print_details(fileName3);
//}
