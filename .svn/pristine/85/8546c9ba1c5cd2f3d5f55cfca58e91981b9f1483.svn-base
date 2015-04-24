#pragma once

#include <boost/numeric/ublas/triangular.hpp>  // Triangular matrix
#include <boost/numeric/ublas/io.hpp>

#include <ql/termstructures/volatility/abcd.hpp>

#include <LMM/helper/LMMTenorStructure.h>
#include <LMM/LmmModel/VolatilityFunction.h>

#include <LMM/LmmModel/Correlation.h>


/*! \class HGVolatilityParam
 *
 *  This is a class that hold volatility parameters  for HGVolatilityFunction
 *  This class hold all volatility parameters, and the also define the h,g functions as 'parameters'
 *  If one need to create an other HGVolatility, then just defining an other HGVolatilityParam by
 *  inheriting from this class
 *
 *  and define the 4 pure virtual methods
 *   - h
 *   - g
 *   - reset_FromArray // VolatilityParam pure virtual
 *   - write_to_stream // VolatilityParam pure virtual
 */

//! YY change the name to Shifted_HGVolatilityFunction
class Shifted_HGVolatilityParam : public VolatilityParam
{
	
public:
	struct ABCDParameter
	{
		double a_;
		double b_;
		double c_;
		double d_;
		ABCDParameter(double a, double b, double c, double d):a_(a),b_(b),c_(c),d_(d){};
	};


	typedef boost::numeric::ublas::triangular_matrix<double, boost::numeric::ublas::lower> LowerTriangularMatrix;
	typedef std::vector< std::pair<size_t, size_t> > VectorOfMatrixIndices;

	/*! vector of pair indices Libor and Time
	 *  Do not forget this order of pair when storing indices
	 *  Do not forget they are Libor, Time indices and not matrix indices
	 *  HGVolatilityParam has to convert these indices into matrix indices 
	 *  Then modify the appropriated values
	 */


	//! YY TODO: to delete latter
	Shifted_HGVolatilityParam( LMMTenorStructure_PTR pLMMTenorStructure, 
							   ABCDParameter		 h_abcdParameter,
							   double				 g_constParameter,
							   double				 shift_constParameter)
				: VolatilityParam()
				, pLMMTenorStructure_(pLMMTenorStructure)
				, abcd_param_(h_abcdParameter)
				, h_matrix_(pLMMTenorStructure_->get_nbLIBOR() , pLMMTenorStructure_->get_nbLIBOR())
				, g_matrix_(pLMMTenorStructure_->get_nbLIBOR() , pLMMTenorStructure_->get_nbLIBOR())
				, shift_matrix_(pLMMTenorStructure_->get_nbLIBOR() , pLMMTenorStructure_->get_nbLIBOR())
			{
				init_h_matrix(abcd_param_);	
				init_g_matrix(g_constParameter);
				init_shift_matrix(shift_constParameter);	
			}


	Shifted_HGVolatilityParam(LMMTenorStructure_PTR pLMMTenorStructure, 
							  ABCDParameter		  h_abcdParameter,
							  double				  g_constParameter,
							  const vector<double>& shift_constParameter)
		: VolatilityParam()
		, pLMMTenorStructure_(pLMMTenorStructure)
		, abcd_param_(h_abcdParameter)
		, h_matrix_(pLMMTenorStructure_->get_nbLIBOR() , pLMMTenorStructure_->get_nbLIBOR())
		, g_matrix_(pLMMTenorStructure_->get_nbLIBOR() , pLMMTenorStructure_->get_nbLIBOR())
		, shift_matrix_(pLMMTenorStructure_->get_nbLIBOR() , pLMMTenorStructure_->get_nbLIBOR())
	{
		init_h_matrix(abcd_param_);	
		init_g_matrix(g_constParameter);
		init_shift_matrix(shift_constParameter);	
	}

	
	//! YY TODO: real constructor
	Shifted_HGVolatilityParam(  LMMTenorStructure_PTR        pLMMTenorStructure, 
								ABCDParameter		         h_abcdParameter,
								const LowerTriangularMatrix& g_matrix,
								const LowerTriangularMatrix& shift_matrix)
		: VolatilityParam()
		, pLMMTenorStructure_(pLMMTenorStructure)
		, abcd_param_(h_abcdParameter)
		, h_matrix_(pLMMTenorStructure_->get_nbLIBOR() , pLMMTenorStructure_->get_nbLIBOR())
		, g_matrix_(g_matrix)
		, shift_matrix_(shift_matrix)
	{
		init_h_matrix(abcd_param_);	        
		assert(h_matrix_.size1()     == pLMMTenorStructure_->get_nbLIBOR() && h_matrix_.size2() == pLMMTenorStructure_->get_nbLIBOR());
		assert(g_matrix_.size1()     == pLMMTenorStructure_->get_nbLIBOR() && g_matrix_.size2() == pLMMTenorStructure_->get_nbLIBOR());
		assert(shift_matrix_.size1() == pLMMTenorStructure_->get_nbLIBOR() && shift_matrix_.size2() == pLMMTenorStructure_->get_nbLIBOR());

		//YY TODO: to force h,g,shift's the first line & row = 1.0e100 ! 
	}

	//! for BetaShifted_HGVolatilityFunction
	Shifted_HGVolatilityParam(LMMTenorStructure_PTR        pLMMTenorStructure, 
							  ABCDParameter		           h_abcdParameter,
							  const LowerTriangularMatrix& g_matrix)
		: VolatilityParam()
		, pLMMTenorStructure_(pLMMTenorStructure)
		, abcd_param_(h_abcdParameter)
		, h_matrix_(pLMMTenorStructure_->get_nbLIBOR() , pLMMTenorStructure_->get_nbLIBOR())
		, g_matrix_(g_matrix)
		//, shift_matrix_(0,0)
	{
		init_h_matrix(abcd_param_);	        
		assert(h_matrix_.size1()     == pLMMTenorStructure_->get_nbLIBOR() && h_matrix_.size2() == pLMMTenorStructure_->get_nbLIBOR());
		assert(g_matrix_.size1()     == pLMMTenorStructure_->get_nbLIBOR() && g_matrix_.size2() == pLMMTenorStructure_->get_nbLIBOR());
	}

	virtual ~Shifted_HGVolatilityParam(){}

	double h(size_t indexLibor, size_t indexTime) const 
	{
		assert(indexLibor>0 && indexTime>0 && indexLibor>=indexTime);
		return h_matrix_(indexLibor,indexTime);
	}
	
	double g(size_t indexLibor, size_t indexTime) const 
	{
		assert(indexLibor>0 && indexTime>0 && indexLibor>=indexTime);
		return g_matrix_(indexLibor,indexTime);	
	}

	double shift(size_t indexLibor, size_t indexTime) const  
	{
		assert(indexLibor>=0 && indexTime>=0 && indexLibor>=indexTime); // Attention: shift is different from h,g, 0 element should be initialized!
		return shift_matrix_(indexLibor,indexTime);	
	}
	
	
	void reset_g_matrix(const LowerTriangularMatrix& other_g);

	size_t get_horizon() const ;
		
	virtual  QuantLib::Array get_ArrayFrom_Shift() const = 0;

	virtual void reset_Shift_FromArray(const QuantLib::Array& x) = 0;
	
	ABCDParameter get_ABCD() const { return abcd_param_; }

	virtual void reset_ABCD(const QuantLib::Array& x) = 0;

	virtual void write_to_stream(std::ostream & os) const  ;

protected :
	
	LMMTenorStructure_PTR pLMMTenorStructure_ ;
	ABCDParameter abcd_param_;

	LowerTriangularMatrix g_matrix_;
	LowerTriangularMatrix h_matrix_;
	LowerTriangularMatrix shift_matrix_;		

private :
	//! YY To delete	
	void init_h_matrix(ABCDParameter h_abcdParameter); // not initialized 1st row & column
	void init_g_matrix(double g_constParameter);       // not initialized 1st row & column
	void init_shift_matrix(double shift_constParameter); // shift need to initialized all matrix, not like h,g
	void init_shift_matrix(const std::vector<double>& shift_constParameter); // shift need to initialized all matrix, not like h,g
};
typedef boost::shared_ptr<Shifted_HGVolatilityParam> Shifted_HGVolatilityParam_PTR;

//----------------------------------------------------------------------------------------
//
//					Shifted_HGVolatilityFunction
//
//----------------------------------------------------------------------------------------
class Shifted_HGVolatilityFunction : public VolatilityFunction
{
	typedef boost::numeric::ublas::triangular_matrix<double, boost::numeric::ublas::lower> LowerTriangularMatrix;

public:
	 // horizon = N, total number of Libor: L_k, k = [0,N]

	//! Constructor
	Shifted_HGVolatilityFunction(LMMTenorStructure_PTR pLMMTenorStructure,
								 Correlation_PTR       pCorrelation,
								 Shifted_HGVolatilityParam_PTR pShifted_HGVolatilityParam)
								 :VolatilityFunction(pLMMTenorStructure,pCorrelation)
								 ,pShifted_HGVolatilityParam_(pShifted_HGVolatilityParam){}


	virtual ~Shifted_HGVolatilityFunction(){}

    //! \int_{T_i}^{T_j} sigma_i(u)*sigma_j(u)*rho_ij(u) du
	double covIntegral( LMM::Index indexTime_i  ,
						LMM::Index indexTime_j  ,
						LMM::Index indexLibor_i ,
						LMM::Index indexLibor_j ) const;

	//virtual double equivalentShift(LMM::Index liborIndex, LMM::Index timeIndex) const
	//{
	//	return pShifted_HGVolatilityParam_->shift(liborIndex,timeIndex);
	//}

	virtual double get_shift_timeIndexVersion      (LMM::Index liborIndex, LMM::Index timeIndex) const
	{
		return pShifted_HGVolatilityParam_->shift(liborIndex,timeIndex);
	}
	virtual double get_shift_timeContinuousVersion (LMM::Index liborIndex, double t) const
	{
		const std::vector<double>& tenor_dates = pLMMTenorStructure_->get_tenorDate();

		assert(liborIndex < pLMMTenorStructure_->get_nbLIBOR() );
		assert(t <= tenor_dates[liborIndex] ); // shift_i(...) function die after time T_i 

		//! ChiThan: search the nearest time index after t
		if(t == 0.0)
			return Shifted_HGVolatilityFunction::get_shift_timeIndexVersion(liborIndex,1);

		size_t timeIndex=0;
		while (t> tenor_dates[timeIndex])
		{
			++timeIndex;
		}
		return Shifted_HGVolatilityFunction::get_shift_timeIndexVersion(liborIndex,timeIndex) ;
	}



	//! use for calibration
	virtual void update_VolatilityParam(VolatilityParam_PTR vol_param_ptr);

	Shifted_HGVolatilityParam_PTR get_ShiftedHGVolatilityParam_PTR() const {return pShifted_HGVolatilityParam_;}

protected:

	Shifted_HGVolatilityParam_PTR pShifted_HGVolatilityParam_;

public:
	//! print to fil
	virtual void print(const std::string& filename) const;
};
typedef boost::shared_ptr<Shifted_HGVolatilityFunction> Shifted_HGVolatilityFunction_PTR;