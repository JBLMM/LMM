#include <LMM/Test/Tests.h>

#include <LMM/helper/LMMTenorStructure.h>
#include <LMM/instrument/VanillaSwap.h>
#include <LMM/instrument/VanillaSwaption.h>
#include <LMM/LmmModel/Lmm.h>
#include <LMM/LmmModel/Correlation.h>
#include <LMM/LmmModel/ConstShifted_HGVolatilityFunction.h>
//#include <LMM/calibration/TestHGVolatilityParam.h>

#include <LMM/LmmModel/UpperTriangleVanillaSwaptionQuotes.h>

void test_ResetSwap();

void test_GenerateLiborQuote();


UpperTriangularDoubleMatrix create_UpperTriangularDoubleMatrix(const size_t nbYear, const double& increment);

LmmVanillaSwaptionApproxPricer_Rebonato_PTR createLmmVanillaSwaptionApproxPricer(const LMMTenorStructure_PTR pLMMTenorStructure);

void test_UpperTriangleVanillaSwaptionQuotes()
{
	test_ResetSwap();

	test_GenerateLiborQuote();

	size_t nbYear = 5;
	Tenor tenorfixedleg = Tenor::_1YR;
	Tenor tenorfloatleg = Tenor::_6M;

	double constant_rate = 0.02;

	LMMTenorStructure_PTR pLmmTenorStructure( new LMMTenorStructure(tenorfloatleg, nbYear) );

	{
		UpperTriangularDoubleMatrix strike_matrix  = create_UpperTriangularDoubleMatrix(nbYear,0.001);
		UpperTriangularDoubleMatrix atm_vol_matrix = create_UpperTriangularDoubleMatrix(nbYear,0.01);

		UpperTriangleVanillaSwaptionQuotes_ConstPTR  swaption_quote_ptr (new UpperTriangleVanillaSwaptionQuotes(pLmmTenorStructure,nbYear,tenorfixedleg,tenorfloatleg,strike_matrix,atm_vol_matrix) ); 

		swaption_quote_ptr->print("test_UpperTriangleVanillaSwaptionQuotes.csv");

		const UpperTriangularIndexPairMatrix & index_matrix = swaption_quote_ptr->get_UpperTriangularIndexPairMatrix() ;

		for(size_t iDelegate = 1; iDelegate<index_matrix.size1();++iDelegate)
		{
			for(size_t jDelegate = 1; jDelegate<index_matrix.size1()-iDelegate;++jDelegate)
			{
				const std::pair<size_t,size_t> & pair_indices = index_matrix(iDelegate,jDelegate);

				assert(pair_indices.first  == 2* iDelegate);
				assert(pair_indices.second == 2* jDelegate);
			}
		}
	}

	{
		LmmVanillaSwaptionApproxPricer_Rebonato_PTR rebonato_ptr = createLmmVanillaSwaptionApproxPricer( pLmmTenorStructure );
		LiborQuotes_ConstPTR libor_quote_ptr = LiborQuotes::create_LiborInit(pLmmTenorStructure, constant_rate);

		UpperTriangleVanillaSwaptionQuotes_ConstPTR atm_swaption_implied_vol_ptr = UpperTriangleVanillaSwaptionQuotes::create_ATMSwaptionImpliedVol(
			libor_quote_ptr,
			tenorfixedleg,
			tenorfloatleg,
			rebonato_ptr
			);
		atm_swaption_implied_vol_ptr->print("test_UpperTriangleVanillaSwaptionQuotes_ATM.csv");
	}
}

void test_ResetSwap()
{
	size_t nbYear = 5;
	Tenor tenorfixedleg = Tenor::_1YR;
	Tenor tenorfloatleg = Tenor::_6M;
	LMMTenorStructure_PTR pLmmTenorStructure( new LMMTenorStructure(tenorfloatleg, nbYear) );

	VanillaSwap empty_swap;
	VanillaSwaption empty_swaption;

	std::cout<<" ====  Default constructor of Swap "<<std::endl;
	std::cout<<empty_swap<<std::endl;

	{
		VanillaSwap swap(0.01,2,6,tenorfloatleg,tenorfixedleg, pLmmTenorStructure);

		empty_swap = swap;

		VanillaSwaption swaption(swap , OptionType::CALL);

		empty_swaption =  swaption;
	}

	std::cout<<" ====  After Assignment of Swap "<<std::endl;
	std::cout<<empty_swap<<std::endl;
}

void test_GenerateLiborQuote()
{
	size_t nbYear = 50;
	Tenor tenorfloatleg = Tenor::_6M;
	const double constant_rate = 0.02;

	LMMTenorStructure_PTR pLmmTenorStructure( new LMMTenorStructure(tenorfloatleg, nbYear) );

	LiborQuotes libor(pLmmTenorStructure,constant_rate);
	assert( libor.check_data_consistency() );
	
	libor.print("test_GenerateLiborQuote.csv");
}

UpperTriangularDoubleMatrix create_UpperTriangularDoubleMatrix(const size_t nbYear, const double& increment)
{
	size_t matrix_size = nbYear + 1 ;
	UpperTriangularDoubleMatrix upper_triangle_matrix(matrix_size,matrix_size);

	for(size_t k=0;k<matrix_size;++k)
	{ 
		upper_triangle_matrix(0,k) = - 10000000; //first row is not used
		upper_triangle_matrix(k,0) = - 10000000; //first row is not used
	}

	double counter = increment;
	for(size_t i=1;i<matrix_size;++i)
	{
		for(size_t j=1;j<matrix_size-i;++j)
		{
			upper_triangle_matrix(i,j) = counter;		
			counter+=increment;
		}
	}

	return upper_triangle_matrix;
}

LmmVanillaSwaptionApproxPricer_Rebonato_PTR createLmmVanillaSwaptionApproxPricer(const LMMTenorStructure_PTR pLMMTenorStructure)
{
	//! Correlation 
	size_t nbFactor       = 3; // need to test nbFactor  = 3, and nbFactor = 
	size_t correlFullRank = pLMMTenorStructure->get_horizon()+1;
	size_t correlReducedRank = nbFactor;
	CorrelationReductionType::CorrelationReductionType correlReductionType = CorrelationReductionType::PCA;
	double correlAlpha = 0.0;
	double correlBeta  = 0.1;
	Correlation_PTR correlation(new XY_beta_Correlation(correlFullRank,correlReducedRank, correlReductionType,correlAlpha,correlBeta));
	correlation->calculate(); // for print.

	double a=0.22,b=0.4,c=0.6,d=0.15;
	double g_constParam = 1.;
	double shift_constParam = 0.0;

	Shifted_HGVolatilityParam::ABCDParameter abcdParam(a,b,c,d);
	ConstShifted_HGVolatilityParam_PTR pConstShifted_HGVolatilityParam( new ConstShifted_HGVolatilityParam(pLMMTenorStructure, abcdParam, g_constParam, shift_constParam));
	

	Shifted_HGVolatilityFunction_PTR pVolatilityFunction (new ConstShifted_HGVolatilityFunction(pLMMTenorStructure, correlation, pConstShifted_HGVolatilityParam)); 

	//! Dispersion
	Dispersion dispersion(pVolatilityFunction);

	Lmm_PTR lmm_ptr(new Lmm(dispersion) );

	LmmVanillaSwaptionApproxPricer_Rebonato_PTR pLmmApproxVanillaSwaptionPricer(new LmmVanillaSwaptionApproxPricer_Rebonato(lmm_ptr));	

	return pLmmApproxVanillaSwaptionPricer;
}