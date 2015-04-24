#include <iostream>

#include <LMM/helper/Name.h>
#include <LMM/helper/GenericPath.h>
#include <LMM/helper/Printer.h>
#include <LMM/RNGenerator/McGenerator.h>
#include <LMM/LmmModel/ConstShifted_HGVolatilityFunction.h>
#include <LMM/LmmModel/BetaShifted_HGVolatilityFunction.h>
#include <LMM/LmmModel/McTerminalLmm.h>
#include <LMM/LmmModel/McSpotLmm.h>
#include <LMM/LmmModel/Lmm.h>
#include <LMM/helper/Tensor.h>

#include <stdlib.h>     // for C++ random number generator: srand, rand 
//----------------------------------------------------------------------------------------------
//
//                                       Test_Caplet
//
//----------------------------------------------------------------------------------------------
namespace TEST
{
	class TEST_LmmAndMcLmm
	{	
	protected:
		bool if_initialized_; 

		static const size_t        nbFactor_ = 3;     // for correlation reduction
		static const unsigned long seed_ = 47;     // MC

		Tenor tenorType_;     // lmmTenorStructure
		size_t horizonYear_;  // lmmTenorStructure		

		bool terminalOrSpotProb_;                  // MC
		MCSchemeType::MCSchemeType mcSchemeType_;  // MC 


		LMMTenorStructure_PTR  lmmTenorStructure_;

		std::vector<double> liborsInitValues_;	
		Correlation_PTR        correlation_;


		VolatilityFunction_PTR volatilityFunction_;
		Lmm_PTR   lmm_;
		McLmm_PTR mcLmm_;

	public:
		TEST_LmmAndMcLmm (  Tenor  tenorType, 
							size_t horizonYear,						   
							//const  std::vector<double>& liborsInitValues,
							double liborsInitValue,
							bool   terminalOrSpotProb,
							MCSchemeType::MCSchemeType mcSchemeType)
							: if_initialized_ (false),
							tenorType_(tenorType),
							horizonYear_(horizonYear),
							//liborsInitValues_(liborsInitValues),
							terminalOrSpotProb_(terminalOrSpotProb),
							mcSchemeType_(mcSchemeType)
		{			
			lmmTenorStructure_  = create_lmmTenorStructure(tenorType, horizonYear);			
			liborsInitValues_   = create_liborsInitValue(liborsInitValue);
			correlation_        = create_Correlation();			
		}

		virtual ~TEST_LmmAndMcLmm(){};

		void create()
		{
			volatilityFunction_ = create_volatilityFunction();
			lmm_                = create_lmm();
			mcLmm_              = create_McLmm();

			if_initialized_ = true;
		}

		LMMTenorStructure_PTR create_lmmTenorStructure(Tenor tenorType, size_t horizonYear)
		{
			LMMTenorStructure_PTR lmmTenorStructure(new LMMTenorStructure(tenorType, horizonYear));
			return lmmTenorStructure;
		}

		std::vector<double> create_liborsInitValue(double fwdRate)
		{		
			std::vector<double> liborsInitValue(lmmTenorStructure_->get_horizon()+1, fwdRate);
			return liborsInitValue;
		}

		Correlation_PTR create_Correlation()
		{
			//! Correlation 
			size_t nbFactor       = 3; // need to test nbFactor  = 3, and nbFactor = 
			size_t correlFullRank = lmmTenorStructure_->get_horizon()+1;
			size_t correlReducedRank = nbFactor;
			CorrelationReductionType::CorrelationReductionType correlReductionType = CorrelationReductionType::PCA;
			double correlAlpha = 0.0;
			double correlBeta  = 0.1;
			Correlation_PTR correlation(new XY_beta_Correlation(correlFullRank,correlReducedRank, correlReductionType,correlAlpha,correlBeta));
			correlation->calculate(); // for print.

			return correlation;
		}

		virtual VolatilityFunction_PTR create_volatilityFunction() = 0;

		virtual Lmm_PTR create_lmm() // need firstly create volatilityFunction
		{		
			Dispersion dispersion(volatilityFunction_);
			Lmm_PTR lmm (new Lmm(dispersion));
			return lmm;
		}

		McLmm_PTR create_McLmm() // need firstly create_lmm
		{
			RNGenerator_PTR  rnGenerator(new McGenerator(seed_));

			if(terminalOrSpotProb_)
			{
				McLmm_PTR mclmm(new McTerminalLmm (lmm_, liborsInitValues_, rnGenerator, mcSchemeType_));
				return mclmm;
			}
			else
			{
				McLmm_PTR mclmm(new McSpotLmm (lmm_, liborsInitValues_, rnGenerator, mcSchemeType_));
				return mclmm;
			}
		}

		Lmm_PTR get_lmm()
		{
			if(!if_initialized_)
				create();

			return lmm_;			
		}


		McLmm_PTR get_mcLmm()
		{
			if(!if_initialized_)
				create();

			return mcLmm_;			
		}

		std::vector<double> get_liborsInitValues() // copy not efficient.
		{
			return liborsInitValues_;
		}
	};


	class TEST_ShiftedLmmAndMcLmm : public TEST_LmmAndMcLmm
	{
		// Duplication :) so bad to change latter.
		//std::vector<double> shift_ConstVector_;
		double shift_constScalar_;

	public:
		TEST_ShiftedLmmAndMcLmm(Tenor  tenorType, 
			size_t horizonYear,						   
			//const  std::vector<double>& liborsInitValues,
			double liborsInitValue,
			bool   terminalOrSpotProb,
			MCSchemeType::MCSchemeType mcSchemeType,
			double shift_constScalar)
			:TEST_LmmAndMcLmm(tenorType, horizonYear, liborsInitValue, terminalOrSpotProb, mcSchemeType),
			shift_constScalar_(shift_constScalar)
		{}

		//TEST_ShiftedLmmAndMcLmm(Tenor  tenorType, 
		//	size_t horizonYear,						   
		//	//const  std::vector<double>& liborsInitValues,
		//	double liborsInitValue,
		//	bool   terminalOrSpotProb,
		//	MCSchemeType::MCSchemeType mcSchemeType,
		//	std::vector<double>& shift_ConstVector)
		//	:TEST_LmmAndMcLmm(tenorType, horizonYear, liborsInitValue, terminalOrSpotProb, mcSchemeType),
		//	shift_ConstVector_(shift_ConstVector)
		//{}

		VolatilityFunction_PTR create_volatilityFunction()
		{
			//! volatility function
			double a = -0.06;
			double b = 0.17;
			double c = 0.54;
			double d = 0.17;
			Shifted_HGVolatilityParam::ABCDParameter abcdParam (a,b,c,d);

			double g_constParam = 1.0;
			std::vector<double> shifts(lmmTenorStructure_->get_nbLIBOR(), shift_constScalar_);
			shifts[0] = -1.0e100;
			for(size_t i=1; i<shifts.size(); ++i)
			{
				shifts[i] =  (1 + 0.5*rand()/(double)RAND_MAX )*shift_constScalar_; // 20% of noise add to the shift
				std::cout << "shifts[" << i << "]= "<< shifts[i] << endl;
			}		

			ConstShifted_HGVolatilityParam_PTR hgParam( new ConstShifted_HGVolatilityParam(lmmTenorStructure_, abcdParam, g_constParam, shifts));

			ConstShifted_HGVolatilityFunction_PTR shifted_hgVolatilityFunction (
				new ConstShifted_HGVolatilityFunction(lmmTenorStructure_, correlation_, hgParam)
				); 
			return shifted_hgVolatilityFunction;
		}
	};

}



