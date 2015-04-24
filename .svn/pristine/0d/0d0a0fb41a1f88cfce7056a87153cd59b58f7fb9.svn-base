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
#include <LMM/LmmModel/Correlation.h>
#include <LMM/LmmModel/Dispersion.h>
#include <LMM/LmmModel/ConstShifted_HGVolatilityFunction.h>
#include <LMM/LmmModel/McTerminalLmm.h>
#include <LMM/LmmModel/McSpotLmm.h>
#include <LMM/pricer/McLmmVanillaSwapPricer.h>
#include <LMM/pricer/McLmmVanillaSwaptionPricer.h>
#include <LMM/pricer/LmmVanillaSwaptionApproxPricer_Rebonato.h>
#include <LMM/pricer/LmmVanillaSwaptionApproxPricer_Piterbarg.h>
#include <LMM/LmmModel/Lmm.h>
#include <LMM/LmmModel/ConstShifted_HGVolatilityFunction.h>

/*! \class PricingConfig
* Use for configure a simulation of pricing swap and swaption
* All variable membre are configuration
* After resetting something, user has to call resetAllLmmComponent() in order to take in account of change
*/
struct PricingConfig
{
	PricingConfig() 
		: shifted_value(0.0)                                             // TODO: test shift = .01 
		, fwdRate(0.04)                                                   //
		, horizonYear(30)                                                 //
		, floatLegTenorType(Tenor::_6M)	, fixedLegTenorType(Tenor::_12M)  // Swap, Swaptions
		, pLMMTenorStructure(new LMMTenorStructure(floatLegTenorType, horizonYear) ) 
		, strike(0.04) , indexStart(20)	, indexEnd(40)                   //
		// [a+b.tau]e^{-c.tau + d} //
		, a(0.0),b(0.0),c(0.0),d(0.2), abcd_params_ptr (new ConstShifted_HGVolatilityParam(pLMMTenorStructure, Shifted_HGVolatilityParam::ABCDParameter(a,b,c,d),1.0,0.0))         // Vol
		//, a(-0.06),b(0.17),c(0.54),d(0.10), abcd_params(pLMMTenorStructure,a,b,c,d)         // Vol
		, corr_nbFactor(3)                                                 //
		, XY_corr_alpha ( 0.3)                                             // Correlation
		, XY_corr_beta  ( 0.5)                                             //	
		, corr_ReductionType(CorrelationReductionType::RA)				   //PCA       
		, seed(47)                                                         // Simulations
		, useTerminalProba(true)                                           //
		, mcSchemeType(MCSchemeType::PC) // PC // IPC                      //
		, minNbSimu (2000)                                          
		, maxNbSimu (50000)                                        
		, bnSimualtionPowStep(1.5)                                    
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

	// configure for Swap
	double      strike;  
	LMM::Index  indexStart;  
	LMM::Index  indexEnd;

	VanillaSwap_PTR  pVanillaSwap;
	VanillaSwaption_PTR   pVanillaSwaption;

	// configure for volatility
	double a,b,c,d;
	ConstShifted_HGVolatilityParam_PTR abcd_params_ptr;	
	VolatilityFunction_PTR pHGVolatilityFunction; 


	// configure for correlation
	size_t corr_nbFactor;  // nb factor of the model (nb Brownians)
	double XY_corr_alpha;
	double XY_corr_beta;
	CorrelationReductionType::CorrelationReductionType corr_ReductionType;
	Correlation_PTR pCorrelation;

	Dispersion_PTR pDispersion;

	// configure for simulations
	unsigned long   seed;
	RNGenerator_PTR pRNGenerator;

	bool useTerminalProba;
	MCSchemeType::MCSchemeType mcSchemeType;

	size_t minNbSimu;
	size_t maxNbSimu; 
	double bnSimualtionPowStep;

	Lmm_PTR         pLmm;
	McLmm_PTR		pMCLmm;
	

	// configure for pricers
	
	McLmmVanillaSwapPricer_PTR      pMCLmmVanillaSwapPricer;
	McLmmVanillaSwaptionPricer_PTR  pMCLmmVanillaSwaptionPricer;
	LmmVanillaSwaptionApproxPricer_Rebonato_PTR pLmmApproxVanillaSwaptionPricer;

	std::vector<size_t> result_nbSimulationVector;
	std::vector<double> simuPrice_Vector;
	double              result_analyticalPrice;

	// setting all LMM classes used for tests 
	void resetAllLmmComponent();

	//print the result into csv file by giving the file name
	void print_test_result( const std::string & filename ) const;

	~PricingConfig() {}

private :
};

void test_VanialSwaptionPricer_MCvsApprox()
{
	PricingConfig config;

	config.result_nbSimulationVector.clear();
	config.simuPrice_Vector.clear();

	double blackVol = config.pLmmApproxVanillaSwaptionPricer->volBlack(*config.pVanillaSwaption,config.liborsInitValue);
	//double RebonatoVol = pLmmApproxVanillaSwaptionPricer->computeRebonatoVolatility(*pVanillaSwaption,liborsInitValue,shifts,omegazero,swaprate);
	
	//double swapRate = config.pLmmApproxVanillaSwaptionPricer->swapRate_Analytical(config.pVanillaSwaption->getUnderlyingSwap(), config.liborsInitValue); // swapRate at time 0
	//double annuity0 = config.pLmmApproxVanillaSwaptionPricer->annuity0(config.pVanillaSwaption->getUnderlyingSwap(), config.liborsInitValue); // swapRate at time 0
	//double swaptionPrice = annuity0*NumericalMethods::Black_Price(swapRate, config.strike, blackVol, 1);
	std::cout<<"swaption_blackVol : "<<blackVol << std::endl;
	double blackPrice = config.pLmmApproxVanillaSwaptionPricer->price(*config.pVanillaSwaption,config.liborsInitValue);
	std::cout<<"blackPrice : "<< blackPrice << std::endl; 

	size_t nbSimulation ;
	//size_t nbLastSimulation = 10;
	for(size_t i=0;; ++i)
	{		
		nbSimulation = (size_t)std::pow(config.bnSimualtionPowStep,(int)i)*config.minNbSimu;
		//nbLastSimulation += i*10;
		//nbSimulation = (size_t)(i*i*i);
		if(nbSimulation>config.maxNbSimu)
			break;
		config.result_nbSimulationVector.push_back(nbSimulation);

		std::cout << "start pricing ---- ---- simulation = " << nbSimulation <<  std::endl;
		double price = config.pMCLmmVanillaSwaptionPricer->price(*config.pVanillaSwaption, nbSimulation);
		std::cout <<"MCLmmVanillaSwaptionPricer nbSimu="<< nbSimulation<<", price="<< price <<std::endl;
		std::cout << "end pricing ---- ---- " << std::endl << std::endl;;

		config.simuPrice_Vector.push_back(price);
		config.pMCLmmVanillaSwaptionPricer->resetGeneratorToinitSeed(); // for the next generation begin with the same seed as this one...		
	}

	config.print_test_result("simuTest_swapion_pricing_MC_results.csv");		
	getchar();  // why I cannot use this trick ?? still a problem of project ??
}



void PricingConfig::resetAllLmmComponent()
{
	shifts.resize(pLMMTenorStructure->get_horizon()+1) ; std::fill(shifts.begin(), shifts.end(), shifted_value);

	liborsInitValue.resize(pLMMTenorStructure->get_horizon()+1) ; std::fill(liborsInitValue.begin(), liborsInitValue.end(), fwdRate);
	
	pVanillaSwap.reset( new VanillaSwap(strike, indexStart, indexEnd, floatLegTenorType, fixedLegTenorType, pLMMTenorStructure) );
	pVanillaSwaption.reset(new VanillaSwaption(*pVanillaSwap,OptionType::CALL) );

	size_t correlFullRank = pLMMTenorStructure->get_horizon()+1;
	size_t correlReducedRank = corr_nbFactor;

	CorrelationReductionType::CorrelationReductionType non_reduction=CorrelationReductionType::NON;
	//pCorrelation.reset(new Const_Correlation(correlFullRank,correlReducedRank, non_reduction , 0. ) );
	pCorrelation.reset(new XY_beta_Correlation(correlFullRank,correlReducedRank, corr_ReductionType,XY_corr_alpha,XY_corr_beta) );
	pCorrelation->calculate();
	//pCorrelation->print("simuTest_MCvsApprox_SwaptionPricer_Correlation.csv");


	pHGVolatilityFunction.reset( new ConstShifted_HGVolatilityFunction(pLMMTenorStructure, pCorrelation, abcd_params_ptr) );
	

	pDispersion.reset(new Dispersion(pHGVolatilityFunction) );

	pRNGenerator.reset( new McGenerator(seed) );

	pLmm.reset(new Lmm(*pDispersion));

	if(useTerminalProba)
	{
		pMCLmm.reset(new McTerminalLmm(pLmm, liborsInitValue, pRNGenerator, mcSchemeType) ) ;
	}
	else
	{
		pMCLmm.reset(new McSpotLmm(pLmm, liborsInitValue, pRNGenerator, mcSchemeType) ) ;
	}

	pMCLmmVanillaSwapPricer.reset(     new McLmmVanillaSwapPricer(pMCLmm)     ) ;
	pMCLmmVanillaSwaptionPricer.reset( new McLmmVanillaSwaptionPricer(pMCLmm) ) ;
	pLmmApproxVanillaSwaptionPricer.reset( new LmmVanillaSwaptionApproxPricer_Rebonato(pLmm) ); //YY: wrong!
}




void PricingConfig::print_test_result( const std::string & filename ) const
{
	std::string path_FileOut = LMMPATH::get_output_path() + filename;
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

