#include <LMM/Test/Tests.h>
#include <LMM/helper/Name.h>
#include <LMM/helper/TenorType.h>

#include <LMM/instrument/VanillaSwap.h>

#include <LMM/LmmModel/Correlation.h>

#include <LMM/LmmModel/Lmm.h>

#include <LMM/LmmModel/McTerminalLmm.h>

#include <LMM/LmmModel/McLmm.h>

#include <LMM/RNGenerator/RNGenerator.h>
#include <LMM/RNGenerator/McGenerator.h>

void test_beginner()
{
	LMMTenorStructure_PTR myTS(new LMMTenorStructure(Tenor::_6M, 10));
	VanillaSwap myVS(1.25, 0 , 2, Tenor::_6M, Tenor::_12M, myTS);
	myVS.print("test_beginer_VanillaSwap.txt");
	int liborLength=myTS->get_horizon();
	std::vector<double> myInitialLibor(liborLength+1);
	cout << 1<< endl;
	for (size_t i = 0; i <myInitialLibor.size(); i++)
	{
		myInitialLibor[i]=1.0;
	}
	
	LmmVanillaSwapPricer myVSP(myTS);
	double prix_swap=myVSP.swapNPV_Analytical_1(myVS, myInitialLibor);
	cout << prix_swap << endl;


	size_t fullrank=myTS->get_horizon()+1;
	size_t reducedrank=5;
	CorrelationReductionType::CorrelationReductionType correlationreductiontype = CorrelationReductionType::PCA;
	Correlation_PTR cor(new Rebonato2_Correlation(fullrank,reducedrank,correlationreductiontype,0.1,0.5));
	cor->calculate();
	//cor->print("test_beginner_cor.txt");

	double a=-0.1;
	double b=0.1;	
	double c=0.1;
	double d=0.1;	
	Shifted_HGVolatilityParam::ABCDParameter HabcdParameters(a,b,c,d);

	double g_constParam = 1.0;
	double shift_constParam = -0.01;

	ConstShifted_HGVolatilityParam_PTR hgParam( new ConstShifted_HGVolatilityParam(myTS,HabcdParameters,g_constParam,shift_constParam));

	double fwdrate=0.02;
	std::vector<double> liborsInitValue(fullrank, fwdrate);

	ConstShifted_HGVolatilityFunction_PTR hgVolatilityFunciton(new ConstShifted_HGVolatilityFunction(myTS, cor, hgParam));
	hgParam->print("test_biginner_hgparameters.txt");
	Dispersion dispersion(hgVolatilityFunciton);

	Lmm_PTR lmm(new Lmm(dispersion));
	
	RNGenerator_PTR rnGenerator(new McGenerator(fullrank));
	McTerminalLmm mcTerminallmm(lmm, liborsInitValue,rnGenerator,MCSchemeType::EULER);
	mcTerminallmm.print("test_beginner_mclmmTerminal.csv");
}
