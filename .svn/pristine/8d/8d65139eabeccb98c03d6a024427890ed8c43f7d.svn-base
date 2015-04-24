#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>

#include <boost/shared_ptr.hpp>
#include <ql/math/array.hpp>

#include <LMM/LmmModel/PCA.h>
#include <LMM/LmmModel/RebonatoAngle.h>

using namespace std;
using namespace QuantLib;

//! YY TODO: need to do the check: the finally approximated matrix: B*B^T is positively defined!
//           what happens if the matrix is not positively defined ? ----  it's not a correlation matrix.

// ------------------------------------------------------------------------------
//
//                             Correlation
//
// ------------------------------------------------------------------------------
namespace CorrelationReductionType
{
	enum CorrelationReductionType {RA, PCA, NON}; // RA: RebonatoAngle
}


class Correlation
{
protected:
	size_t fullRank_;                   // rank of correlation matrix.
	size_t reducedRank_;                // rank of the reduced correlation matrix 
	Matrix originalCorrelMatrix_;       // originalCorrelMatrix, size = (fullRank_, fullRank_)
	Matrix reducedCorrelMatrixB_;       // size = (fullRank_,reducedRank_)
	Matrix reducedCorrelMatrixApprox_ ; // approximate the originalCorrelMatrix, value = B*B^T, size = (rank_, rank_)
	CorrelationReductionType::CorrelationReductionType reductionType_;

	bool   flagApproxConstruced_;

public:
	Correlation(size_t fullRank, size_t reducedRank, CorrelationReductionType::CorrelationReductionType& reductionType); // YY: TODO don't like default parameter: use boost::defaultable<size_t> for reduceRank latter.

	virtual ~Correlation(){}

	void calculate();  // orignalCorrel -> B -> approxiCorrel = B*B^T

	virtual QuantLib::Array get_ArrayFrom_Correlation() const 
	{		
		throw("Correlation::get_ArrayFrom_Correlation not yet implemented ");	
		QuantLib::Array empty_array;return empty_array;
	}

	virtual void reset_FromArray(const QuantLib::Array& x) 
	{
		flagApproxConstruced_=false;
		throw("Correlation::reset_FromArray not yet implemented ");
	}

private:
	void calculateCorrelOriginal();
	void calculateReducedCorrelB();
	void calculateReducedCorrelApprox();

protected:
		
	virtual double correlOriginal(size_t i, size_t j) const = 0; // YY Question: can the pure virtual function be private ? 

	void printPlusPlus_matrices(const std::string& fileName) const ;

public:
	//! getter.
	size_t get_fullRank();
	size_t get_reducedRank();
	const  Matrix& get_originalCorrelMatrix() const;
	const  Matrix& get_reducedCorrelMatrixB() const;
	const  Matrix& get_reducedCorrelMatrixApprox() const;
	const  CorrelationReductionType::CorrelationReductionType& get_reductionType() const;
	const  std::string get_reductionTypeString() const;
	bool   get_flagApproxConstruced()const;

	//! for time period [timeIndex_k-1, timeIndex_k]
	double correlApprox(size_t liborIndex_i , size_t liborIndex_j, size_t  timeIndex_k) const ;

	//print
	virtual void print(const std::string& filename) const ;
	std::string nameCorrelationReductionType()      const ;
};
typedef boost::shared_ptr<Correlation> Correlation_PTR;


// ------------------------------------------------------------------------------
//
//                          Shosemaker2_Correlation
//
// ------------------------------------------------------------------------------
class Shosemaker2_Correlation : public Correlation
{
private:
	double eta_;
	double rhoInfty_;

public:
	Shosemaker2_Correlation(size_t fullRank, size_t reducedRank, CorrelationReductionType::CorrelationReductionType& reductionType, double eta, double rhoInfty); // // YY: TODO don't like default parameter: use boost::defaultable<size_t> for reduceRank latter.

	virtual ~Shosemaker2_Correlation(){}

	bool checkParams() const;

protected:
	double correlOriginal(size_t index_i, size_t index_j) const; 
};


// ------------------------------------------------------------------------------
//
//                          Shosemaker3_Correlation
//
// ------------------------------------------------------------------------------
//! YY  the formula is not sure correct: result is different from the X-report ??? !!! ---- TOCHECK 
class Shosemaker3_Correlation : public Correlation
{
private:
	double eta1_;
	double eta2_;
	double rhoInfty_;

public:
	Shosemaker3_Correlation(size_t fullRank, size_t reducedRank, CorrelationReductionType::CorrelationReductionType& reductionType, double eta1, double eta2, double rhoInfty);

	virtual ~Shosemaker3_Correlation(){}

	bool checkParams() const;

protected:
	double correlOriginal(size_t index_i, size_t index_j) const; 
};	



// ------------------------------------------------------------------------------
//
//                          Rebonato2_Correlation
//
// ------------------------------------------------------------------------------
class Rebonato2_Correlation : public Correlation
{
private:
	double beta_;
	double rhoInfty_;

public:
	Rebonato2_Correlation(size_t fullRank, size_t reducedRank, CorrelationReductionType::CorrelationReductionType& reductionType, double beta, double rhoInfty);

	virtual ~Rebonato2_Correlation(){}

	bool checkParams() const;

protected:
	double correlOriginal(size_t index_i, size_t index_j) const; 
};	



// ------------------------------------------------------------------------------
//
//                          Rebonato3_Correlation
//
// ------------------------------------------------------------------------------
class Rebonato3_Correlation : public Correlation
{
private:
	double alpha_;
	double beta_;
	double rhoInfty_;

public:
	Rebonato3_Correlation(size_t fullRank, size_t reducedRank, CorrelationReductionType::CorrelationReductionType& reductionType, double alpha, double beta, double rhoInfty);

	virtual ~Rebonato3_Correlation(){}

	bool checkParams() const;

protected:
	double correlOriginal(size_t index_i, size_t index_j)const; 
};	




// ------------------------------------------------------------------------------
//
//                          XY_beta_Correlation
//
// ------------------------------------------------------------------------------
class XY_beta_Correlation : public Correlation
{
private:
	double alpha_;
	double beta_;

public:
	XY_beta_Correlation  (size_t fullRank, 
		size_t reducedRank, 
		CorrelationReductionType::CorrelationReductionType& reductionType,
		double alpha, 
		double beta);

	virtual QuantLib::Array get_ArrayFrom_Correlation() const 
	{		
		QuantLib::Array alpha_beta(2);
		alpha_beta[0]=alpha_;
		alpha_beta[1]=beta_;
		return alpha_beta;
	}

	virtual void reset_FromArray(const QuantLib::Array& x)
	{
		QL_REQUIRE(x.size()==2 , "XY_beta_Correlation has exactly 2 parameters ");
		alpha_=x[0];
		beta_ =x[1];
		// correlation approximation matrix need to be refactorized
		flagApproxConstruced_=false; 
	}

	virtual void print(const std::string& filename) const ;

	virtual ~XY_beta_Correlation(){}

	bool checkParams() const;

protected:
	double correlOriginal(size_t index_i, size_t index_j) const; 
};	


// ------------------------------------------------------------------------------
//
//                          Const_Correlation
//
// ------------------------------------------------------------------------------
//! used for perfectly correlated & independent correlation structure.
//! this correlation will not do the reduce rank
class Const_Correlation : public Correlation
{
private:
	double correlConst_;

public:
	Const_Correlation  (size_t fullRank, 
		size_t reducedRank, 
		CorrelationReductionType::CorrelationReductionType& reductionType,
		double correlConst);

	virtual ~Const_Correlation(){}

	virtual QuantLib::Array get_ArrayFrom_Correlation() const 
	{		
		QuantLib::Array const_correl(1);
		const_correl[0]=correlConst_;
		return const_correl;
	}

	virtual void reset_FromArray(const QuantLib::Array& x) 
	{
		QL_REQUIRE(x.size()==1 , "Const_Correlation has exactly 1 parameter");
		correlConst_=x[0];
		// correlation approximation matrix need to be refactorized
		flagApproxConstruced_=false; 
	}

	virtual void print(const std::string& filename) const ;

	bool checkParams() const;

protected:
	double correlOriginal(size_t index_i, size_t index_j) const; 

};	



