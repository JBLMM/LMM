#include <iostream>

#include <boost/shared_ptr.hpp>

#include <LMM/helper/Name.h>
#include <LMM/helper/GenericPath.h>
#include <LMM/helper/Printer.h>

#include <LMM/helper/TenorType.h>
#include <LMM/RNGenerator/McGenerator.h>
#include <LMM/helper/LMMTenorStructure.h>
#include <LMM/instrument/VanillaSwap.h>
#include <LMM/instrument/VanillaSwaption.h>
#include <LMM/ModelMCLMM/Correlation.h>
#include <LMM/ModelMCLMM/Dispersion.h>
#include <LMM/ModelMCLMM/HGVolatilityFunction.h>
#include <LMM/ModelMCLMM/McTerminalLmm.h>
#include <LMM/ModelMCLMM/McSpotLmm.h>
#include <LMM/pricer/McLmmVanillaSwapPricer.h>
#include <LMM/pricer/McLmmVanillaSwaptionPricer.h>
#include <LMM/pricer/LmmApproxVanillaSwaptionPricer.h>


/*! \class PricingConfig
* Use for configure a simulation of pricing swap and swaption
* All variable membre are configuration
* After resetting something, user has to call resetAllLmmComponent() in order to take in account of change
*/
struct PricingConfig
{
	PricingConfig() 
		: shifted_value(0.)                                              // Commons input
		, fwdRate(0.04)                                                  //
		, horizonYear(20)                                                //
		, floatLegTenorType(Tenor::_6M)	, fixedLegTenorType(Tenor::_6M)  // Swap, Swaptions
		, pLMMTenorStructure(new LMMTenorStructure(fixedLegTenorType, horizonYear) ) 
		, strike(0.01) , indexStart(16)	, indexEnd(30)                   //
		// [a+b.tau]e^{-c.tau + d} //
		, a(0.0),b(0.0),c(0.0),d(0.02), abcd_params(a,b,c,d, pLMMTenorStructure)         // Vol
		//, a(-0.06),b(0.17),c(0.54),d(0.10), abcd_params(a,b,c,d, pLMMTenorStructure)         // Vol
		, corr_nbFactor(3)                                                    //
		, XY_corr_alpha ( 0.3)                                             // Correlation
		, XY_corr_beta  ( 0.5)                                            //	
		, corr_ReductionType(CorrelationReductionType::RA) //PCA       //
		, seed(47)                                                       // Simulations
		, useTerminalProba(true)                                         //
		, mcSchemeType(MCSchemeType::EULER) // PC // IPC                 //
		, minNbSimu (2000)                                          //
		, maxNbSimu (100000)                                         //
		, bnSimualtionPowStep(2)                                         //		
	{ 

		// setting all LMM classes used for tests 
		this->resetAllLmmComponent();
	}

	// common config
	double shifted_value;
	double fwdRate;

	size_t horizonYear;
	Tenor  floatLegTenorType;
	Tenor  fixedLegTenorType;

	LMMTenorStructure_PTR pLMMTenorStructure;

	std::vector<double> shifts;	
	std::vector<double> liborsInitValue;
	std::vector<double> zerocoupond;
	std::vector<double> numeraire;
	double              annuity;
	double              swaprate;
	std::vector<double> omegazero;

	// configure for Swap
	double      strike;  
	LMM::Index  indexStart;  
	LMM::Index  indexEnd;

	LMM::VanillaSwap_PTR  pVanillaSwap;
	VanillaSwaption_PTR   pVanillaSwaption;

	// configure for volatility
	double a,b,c,d;
	DefaultHGVolatilityParam abcd_params;	
	VolatilityFunction_PTR pHGVolatilityFunction; 


	// configure for correlation
	size_t corr_nbFactor;  // nb factor of the model (nb Brownians)
	double XY_corr_alpha;
	double XY_corr_beta;
	CorrelationReductionType::CorrelationReductionType corr_ReductionType;
	Correlation_PTR pCorrelation;

	Dispersion_PTR pDispersion;

	// configure for simulations
	unsigned long seed;
	RNGenerator_PTR pRNGenerator;

	bool useTerminalProba;
	MCSchemeType::MCSchemeType mcSchemeType;

	size_t minNbSimu;
	size_t maxNbSimu; 
	double bnSimualtionPowStep;

	MCLmm_PTR pMCLmm;


	// configure for pricers
	boost::shared_ptr< MCLmmVanillaSwapPricer > pMCLmmVanillaSwapPricer;
	boost::shared_ptr< MCLmmVanillaSwaptionPricer >	pMCLmmVanillaSwaptionPricer;
	boost::shared_ptr< LmmApproxVanillaSwaptionPricer> pLmmApproxVanillaSwaptionPricer;

	std::vector<size_t> result_nbSimulationVector;
	std::vector<double> simuPrice_Vector;
	double              result_analyticalPrice;

	// setting all LMM classes used for tests 
	void resetAllLmmComponent();

	//print the result into csv file by giving the file name
	void print_test_result( const std::string & filename ) const;

	~PricingConfig() {}

private :
	void precomputeZeroCouponAnnuityAndOmega();
};




int main(int arvc, char* arvg[])
{
	PricingConfig config;

	config.result_nbSimulationVector.clear();
	config.simuPrice_Vector.clear();

	double blackVol = config.pLmmApproxVanillaSwaptionPricer->volBlack(*config.pVanillaSwaption,config.liborsInitValue);
	//double RebonatoVol = pLmmApproxVanillaSwaptionPricer->computeRebonatoVolatility(*pVanillaSwaption,liborsInitValue,shifts,omegazero,swaprate);
	
	double swapRate = config.pLmmApproxVanillaSwaptionPricer->swapRate_Analytical(config.pVanillaSwaption->getUnderlyingSwap(), config.liborsInitValue); // swapRate at time 0
	double annuity0 = config.pLmmApproxVanillaSwaptionPricer->annuity0(config.pVanillaSwaption->getUnderlyingSwap(), config.liborsInitValue); // swapRate at time 0
	double swaptionPrice = annuity0*NumericalMethods::Black_Price(swapRate, config.strike, blackVol, 1);
	std::cout<<"swaption_blackVol : "<<blackVol << "swaptionPrice = " << swaptionPrice << std::endl; 

	//result_analyticalPrice = pLmmApproxVanillaSwaptionPricer->price(*pVanillaSwaption,liborsInitValue);
	//std::cout<<"result_analyticalPrice : "<<result_analyticalPrice; // << "swaptionPrice = " << swaptionPrice << std::endl; 

	size_t nbSimulation ;
	for(size_t i=0;; ++i)
	{
		nbSimulation = (size_t)std::pow(config.bnSimualtionPowStep,(int)i)*config.minNbSimu;
		if(nbSimulation>config.maxNbSimu)
			break;
		config.result_nbSimulationVector.push_back(nbSimulation);
		config.simuPrice_Vector.push_back(config.pMCLmmVanillaSwaptionPricer->price(*config.pVanillaSwaption, nbSimulation));
		config.pMCLmmVanillaSwaptionPricer->resetGeneratorToinitSeed(); // for the next generation begin with the same seed as this one...

		std::cout<<"LmmApproxVanillaSwaptionPricer::price : "<<config.result_analyticalPrice 
			<<"      MCLmmVanillaSwaptionPricer nbSimu."<<nbSimulation<<"   price."<<config.simuPrice_Vector.back()<<std::endl;
	}

	config.print_test_result("simuTest_swapion_pricing_MC_results.csv");
		
	return 0;
}










void PricingConfig::resetAllLmmComponent()
{
	shifts.resize(pLMMTenorStructure->get_horizon()+1) ; std::fill(shifts.begin(), shifts.end(), shifted_value);

	liborsInitValue.resize(pLMMTenorStructure->get_horizon()+1) ; std::fill(liborsInitValue.begin(), liborsInitValue.end(), fwdRate);
	
	pVanillaSwap.reset( new LMM::VanillaSwap(strike, indexStart, indexEnd, floatLegTenorType, fixedLegTenorType, pLMMTenorStructure) );
	pVanillaSwaption.reset(new VanillaSwaption(*pVanillaSwap,OptionType::CALL) );
		
	pHGVolatilityFunction.reset( new HGVolatilityFunction(abcd_params ,  pLMMTenorStructure) );
	
	size_t correlFullRank = pLMMTenorStructure->get_horizon()+1;
	size_t correlReducedRank = corr_nbFactor;

	CorrelationReductionType::CorrelationReductionType non_reduction=CorrelationReductionType::NON;
	//pCorrelation.reset(new Const_Correlation(correlFullRank,correlReducedRank, non_reduction , 0. ) );
	pCorrelation.reset(new XY_beta_Correlation(correlFullRank,correlReducedRank, corr_ReductionType,XY_corr_alpha,XY_corr_beta) );
	pCorrelation->calculate();
	pCorrelation->print("simuTest_MCvsApprox_SwaptionPricer_Correlation.csv");

	pDispersion.reset(new Dispersion(pCorrelation , pHGVolatilityFunction) );

	pRNGenerator.reset( new McGenerator(seed) );

	if(useTerminalProba)
	{
		pMCLmm.reset(new McTerminalLmm(*pDispersion, shifts, liborsInitValue, pRNGenerator, mcSchemeType) ) ;
	}
	else
	{
		pMCLmm.reset(new McSpotLmm(*pDispersion, shifts, liborsInitValue, pRNGenerator, mcSchemeType) ) ;
	}

	pMCLmmVanillaSwapPricer.reset(     new MCLmmVanillaSwapPricer(pMCLmm)     ) ;
	pMCLmmVanillaSwaptionPricer.reset( new MCLmmVanillaSwaptionPricer(pMCLmm) ) ;
	pLmmApproxVanillaSwaptionPricer.reset( new LmmApproxVanillaSwaptionPricer(pMCLmm) );

	this->precomputeZeroCouponAnnuityAndOmega();

}




void PricingConfig::print_test_result( const std::string & filename ) const
{
	std::string path_FileOut = LMM::get_output_path() + filename;
	std::ofstream outputFile  ;  

	outputFile.open(path_FileOut.c_str() );

	size_t result_size = result_nbSimulationVector.size(); assert(result_size == simuPrice_Vector.size() );

	// print labels
	outputFile<< "NBr Simulations ; " << "Simu Price ; " << " Approx Price ; " << ";; Other Info "
		<< " ; MCSchemeType::" <<MCSchemeType::mcSchemeType2String(mcSchemeType)
		//<< " ; CorrelationReductionType::" <<pCorrelation->nameCorrelationReductionType()
		<<std::endl;

	//print from end to begin
	if(result_size > 0)	
	{ 
		outputFile<< result_nbSimulationVector[result_size-1] << " ; " <<simuPrice_Vector[result_size-1] <<" ; ";
	}
	outputFile<<result_analyticalPrice<<" ; "<<std::endl;

	size_t last_item = result_size-2;
	while(last_item >0)
	{
		outputFile<< result_nbSimulationVector[last_item] << " ; " <<simuPrice_Vector[last_item] <<" ; "<<std::endl;
		--last_item;
	}
	outputFile.close();
}


void PricingConfig::precomputeZeroCouponAnnuityAndOmega()
{
	zerocoupond.resize(liborsInitValue.size() + 1);     
	numeraire.resize(zerocoupond.size());

	const std::vector<double>& deltaT = pLMMTenorStructure->get_deltaT();

	// compute of ZC and numeraire
	zerocoupond[0] = 1.0;
	numeraire[0]   = 1.0;
	for(size_t i=1; i<zerocoupond.size(); ++i)
	{
		size_t indexLibor = i-1;
		// P(0,T_{i-1}) / P(0,T_i)   = 1 + (T_{i}-T_{i-1}) L(0,T_{i-1},T_i)
		zerocoupond[i] = zerocoupond[i-1]/(1+deltaT[indexLibor]*liborsInitValue[indexLibor]);
		numeraire[i] = 1/zerocoupond[i];
	}

	// compute of annuity
	const LMM::VanillaSwap & swap = pVanillaSwaption->getUnderlyingSwap();
	annuity  = pLmmApproxVanillaSwaptionPricer->annuity( 0, swap , numeraire);
	swaprate = pLmmApproxVanillaSwaptionPricer->swapRate_Analytical(swap ,liborsInitValue);

	// compute omegazero
	for(size_t kLibor=0 ; kLibor< pLMMTenorStructure->get_nbLIBOR() ; ++kLibor)
	{
		omegazero.push_back( pLmmApproxVanillaSwaptionPricer->omega0(kLibor, annuity, zerocoupond ) ) ;
	}
}