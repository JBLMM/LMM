#pragma once

#include <deque>

#include <ql/types.hpp>
#include <ql/math/array.hpp>
#include <ql/math/optimization/costfunction.hpp>

#include <LMM/instrument/VanillaSwaption.h>
#include <LMM/LmmModel/GMatrixMapping.h>
#include <LMM/LmmModel/UpperTriangleVanillaSwaptionQuotes.h>
#include <LMM/LmmModel/LmmPenalty.h>
#include <LMM/pricer/LmmVanillaSwaptionApproxPricer_Rebonato.h>

#include <LMM/LmmModel/Shifted_HGVolatilityFunction.h>


/*! \class LmmBaseCostFunction
*
*
*
*/

class LmmBaseCostFunction : public CostFunction
{
public:

	//! constructor 
	LmmBaseCostFunction( LiborQuotes_ConstPTR pLiborQuotes 
		, UpperTriangleVanillaSwaptionQuotes_ConstPTR pUpperTriangleVanillaSwaptionQuotes )
		: CostFunction()
		, pLiborQuotes_(pLiborQuotes)
		, pUpperTriangleVanillaSwaptionQuotes_(pUpperTriangleVanillaSwaptionQuotes)
		, upperTriangleVanillaSwaptionMdlValues_(pUpperTriangleVanillaSwaptionQuotes_->size1(), pUpperTriangleVanillaSwaptionQuotes_->size2() )
		, upperTriangleVanillaSwaptionWeights_(pUpperTriangleVanillaSwaptionQuotes_->size1(), pUpperTriangleVanillaSwaptionQuotes_->size2() )
		, nbCalled(0)
	{
		//default initiate swaption weights
		size_t nbWeightsRow = upperTriangleVanillaSwaptionWeights_.size1();
		size_t nbWeightsCol = upperTriangleVanillaSwaptionWeights_.size2();
		for(size_t iRow=1;iRow<nbWeightsRow;++iRow)
		{
			for(size_t jCol=1;jCol<nbWeightsCol-iRow;++jCol)
			{
				upperTriangleVanillaSwaptionWeights_(iRow,jCol)= 1.;
			}
		}
	}

	size_t size1() const { return pUpperTriangleVanillaSwaptionQuotes_->size1() ; }
	size_t size2() const { return pUpperTriangleVanillaSwaptionQuotes_->size2() ; }

	// name of cost function type
	virtual std::string Name() const = 0 ;

	// specific row for local calib
	virtual void reset_SwaptionRow(const size_t iDelegate) = 0 ;

	// specific cell for cascade calib
	virtual void reset_SwaptionCell(const size_t iDelegate, const size_t jDelegate) = 0 ; 

	virtual void reset_CalibrationParams(const QuantLib::Array & params) const = 0 ;

	virtual Real value(const Array & x) const 
	{
		Array diff_cost = values(x);

		Real res = 0;
		for (size_t i = 0; i < diff_cost.size(); ++i)
		{
			res += diff_cost[i]*diff_cost[i];
		}

		res = sqrt(res);
		return res;	
	}

	//const Array& param_array
	virtual Disposable<Array> values(const Array& x) const = 0 ;

	virtual void print(const std::string& filename) const = 0 ;

	// get and set for swaption weights matrix
	UpperTriangularDoubleMatrix& get_SwaptionWeightMatrix(){ return upperTriangleVanillaSwaptionWeights_ ; }
	void reset_SwaptionWeightMatrix(const UpperTriangularDoubleMatrix& weight_matrix)
	{
		assert(upperTriangleVanillaSwaptionWeights_.size1() == weight_matrix.size1() );
		assert(upperTriangleVanillaSwaptionWeights_.size2() == weight_matrix.size2() );
		upperTriangleVanillaSwaptionWeights_ = weight_matrix;	
	}

	// Getter!
	// attention, methods returning matrix copies, do not use in a loop
	UpperTriangularDoubleMatrix get_MarketQuoteValuesMatrix() const {return pUpperTriangleVanillaSwaptionQuotes_->get_UpperTriangularQuoteValues();	}	
	UpperTriangularDoubleMatrix get_ModelQuoteValuesMatrix()  const { fully_UpdateSwaptionMdlValues() ; return upperTriangleVanillaSwaptionMdlValues_ ; }

	LMMTenorStructure_PTR get_LMMTenorStructure_PTR() { return pLiborQuotes_->get_LMMTenorStructure_PTR(); }
	LiborQuotes_ConstPTR get_LiborQuotes() const { return pLiborQuotes_ ; } 
	UpperTriangleVanillaSwaptionQuotes_ConstPTR get_UpperTriangleVanillaSwaptionQuotes() const { return pUpperTriangleVanillaSwaptionQuotes_;}
	const UpperTriangularDoubleMatrix& get_SwaptionWeightMatrix() const{ return upperTriangleVanillaSwaptionWeights_;}

	//! use only for print calib evolution in the case of virtual calibration test
	//! in this case, we know the "true parameters" which is used for generating market data
	virtual void reset_TrueParam(const QuantLib::Array & true_param) = 0 ; 

	virtual void fully_UpdateSwaptionMdlValues() const = 0 ;

	size_t get_nbCalled() const { return nbCalled; }

protected :

	//! pricing 
	
	virtual void partially_UpdateSwaptionMdlValues() const = 0 ;
	

	LiborQuotes_ConstPTR pLiborQuotes_;
	UpperTriangleVanillaSwaptionQuotes_ConstPTR pUpperTriangleVanillaSwaptionQuotes_;

	UpperTriangularDoubleMatrix upperTriangleVanillaSwaptionWeights_;

	//values change during cost calculation
	mutable unsigned int nbCalled;
	mutable UpperTriangularDoubleMatrix upperTriangleVanillaSwaptionMdlValues_;	

	virtual bool breakForPrintOut() const {return (nbCalled%20) ==0;}
};

typedef boost::shared_ptr<LmmBaseCostFunction> LmmBaseCostFunction_PTR;

