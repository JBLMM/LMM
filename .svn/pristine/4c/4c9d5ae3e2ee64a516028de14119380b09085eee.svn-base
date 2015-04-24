#pragma once

#include <boost/numeric/ublas/triangular.hpp>  // Triangular matrix
#include <boost/numeric/ublas/io.hpp>
#include <LMM/LmmModel/LmmCostFunction.h>
#include <LMM/LmmModel/LmmCalibrator.h>

typedef boost::numeric::ublas::matrix<double> UpperTriangularDoubleMatrix; // UTDM


class LmmVega
{
public:

	const vol_bump_epsilon = 1.0e-4; // YY TODO: test other value: 1.0e-3 ...

	LmmVega(LmmCalibrator_PTR lmmCalibrator)
	{		
		buffer_swaptionQuotes_ = lmmCalibrator_->get_UpperTriangleVanillaSwaptionQuotes();
		vegas_  = UpperTriangularDoubleMatrix(buffer_swaptionQuotes_.size1(), buffer_swaptionQuotes_.size2());        
	}

	virtual void compute_vega(LmmVanillaSwaptionApproxPricer_Rebonato_PTR pricer_) = 0;

private :

	UpperTriangularDoubleMatrix vegas_;	
	UpperTriangleVanillaSwaptionQuotes_ConstPTR buffer_swaptionQuotes_;
	LmmCalibrator_PTR lmmCalibrator_;	
};

class LmmVega_DF : public LmmVega
{
public:

	LmmVega_DF(LmmCalibrator_PTR lmmCalibrator) : LmmVega(lmmCalibrator){}

	virtual void compute_vega(LmmVanillaSwaptionApproxPricer_Rebonato_PTR exoOptionPricer,
		              VanillaSwaption_PTR exoOption)
	{
		UpperTriangleVanillaSwaptionQuotes_ConstPTR swaptionQuotes = lmmCalibrator_->get_UpperTriangleVanillaSwaptionQuotes();
		
		//! original calibrate + price
		lmmCalibrator_->solve();
		double original_exoPrice = exoOptionPricer->price(exoOption); 

		//! Dirac bump for Vega
		for(size_t i=1; i<swaptionQuotes.size1(); ++i)  // YY TODO. check index !
		{
			for(size_t j=1; i<swaptionQuotes.size2()-i; ++j)
			{			
				//! bump ieme quote: +epsilon
				const double original_quote = swaptionQuotes->get_OriginalQuote(i,j);
				const double bumped_quote = original_quote + vol_bump_epsilon ;
				swaptionQuotes->reset_Quote(i,j,bumped_quote);
				
				//! calibrate
				lmmCalibrator_->solve();

				//! reprice
				const double bumped_price = exoOptionPricer->price(exoOption);

				//! calucate Vega
				vegas_(i,j) = (bumped_price  - original_exoPrice) / bumped_quote;

				//! bump ieme quote: -epsilon
				swaptionQuotes->reset_Quote(i,j,original_quote);
			}
		}
	}
};




class LmmVega_ChainRule : public LmmVega
{
public:

	const double bumped_g_parameter = 1e-8;  // YY TODO: test other values ... 

	LmmVega_ChainRule(LmmCalibrator_PTR lmmCalibrator) : LmmVega(lmmCalibrator)
	{
		std::vector<double> vegas_vector;	
		UpperTriangleVanillaSwaptionQuotes_ConstPTR buffer_swaptionQuotes_vector;
		lmmParam_g_vector_  = lmmCalibrator->get_Calibrated_VectorFormat();
	}

	double calculate_dExo_dG(size_t i,  double original_price, 
		  LmmVanillaSwaptionApproxPricer_Rebonato_PTR exoOptionPricer,
		              VanillaSwaption_PTR exoOption)
	{
		const double original_g_parameter = lmmParam_g_vector_[i];
		lmmParam_g_vector_[i] += bumped_g_parameter;
		
		lmmCalibrator->reset_gParameter(lmmParam_g_vector_); // YY TODO: test it again please. 
		lmmCalibrator->solve();
		double bumped_price = exoOptionPricer->price(exoOption);

		const double derivate = (bumped_price - original_price)/ bumped_g_parameter;

		lmmParam_g_vector_[i] = original_g_parameter;

		return derivate;
	}

	virtual void compute_vega(LmmVanillaSwaptionApproxPricer_Rebonato_PTR exoOptionPricer,
		              VanillaSwaption_PTR exoOption)
	{
		//! original calibrate + price
		lmmCalibrator_->solve();
		double original_exoPrice = exoOptionPricer->price(exoOption); 


		//! Matrix multiplication for Vega
			// 1. Jacobian
			// \frac{\partial sigma}{\partial G}, sigma : vector Swaption vol quote, G: vector g,, i.e df/dx in cost function 
			// finite difference with dx ~ 1e-8 and test with others values 1e-7, 1e-6 ... 1.e-4
		Matrix jacobien = lmmCalibrator_->get_Jacobian(); 

		//! if jacobian inversable: 
		Matrix inverseJacobian = inv(jacobien); // use a approriated solver

			// 2. \frac{\partial exo}{\partial G}
		Matrix dExo_dG(n,1);

		for(size_t i=0;i<lmmParam_g_vector_.size();++i )
		{
			dExo_dG[i][0] = calculate_dExo_dG(i,original_exoPrice, pricer, exoOption );
		}

		vegas_vector_ = dExo_dG * inverseJacobian;
	}

private :

	std::vector<double> lmmParam_g_vector_;
	std::vector<double> vegas_vector_;	
	UpperTriangleVanillaSwaptionQuotes_ConstPTR buffer_swaptionQuotes_vector_;
};
