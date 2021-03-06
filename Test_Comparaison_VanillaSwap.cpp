#include <LMM/Test/Tests.h>

#include <vector>

#include <LMM/helper/LMMTenorStructure.h>

#include <LMM/pricer/LmmVanillaSwapPricer.h>
#include <GeneticSwap.h>
#include <GeneticVanillaSwapPricer.h>
#include <Coupon.h>
#include <CappedFlooredCoupon.h>
#include <InstrumentFactory.h>
#include <CouponLeg.h>



void vanillaSwapComparaisonExemple()
{

	double strike=1.6;
	size_t indexStart=3;
	size_t indexEnd=19;
	Tenor tenorStruture=Tenor::_6M;
	Tenor floatingTenor=Tenor::_6M;
	Tenor fixedTenor=Tenor::_12M;
	LMMTenorStructure_PTR lmmTenorStructure(new LMMTenorStructure(tenorStruture, 10));
	size_t liborIndex=lmmTenorStructure->get_horizon()+1;
	std::vector<double> myInitialLibor(liborIndex);
	for (size_t i = 0; i <myInitialLibor.size(); i++)
	{
		myInitialLibor[i]=0.02;//+((double)i)*0.01;
	}


	//VaniSwap_Chi_Trang
	VanillaSwap myVS(strike, indexStart , indexEnd, floatingTenor, fixedTenor, lmmTenorStructure);
	myVS.print("test_beginer_VanillaSwap.txt");
	LmmVanillaSwapPricer myVSP(lmmTenorStructure);
	double prix_swap=myVSP.swapNPV_Analytical_1(myVS, myInitialLibor);
	cout << "FirstVersionSwapPrice: "<< prix_swap << endl;

	//GeneticSwap test
	GeneticSwap_CONSTPTR vanillaSwap_Genetic=InstrumentFactory::createVanillaSwap(
		strike,indexStart,indexEnd,floatingTenor,fixedTenor,lmmTenorStructure,1.0);
	GeneticVanillaSwapPricer_PTR geneticVanillaSwapPricer(new GeneticVanillaSwapPricer());
	double geneticPrice=geneticVanillaSwapPricer->geneticVanillaSwap_Analytical(vanillaSwap_Genetic, myInitialLibor);
	cout << "GeneticSwapTest: "<<geneticPrice << endl;

	cout << "Difference:" << geneticPrice-prix_swap<< endl;
}
