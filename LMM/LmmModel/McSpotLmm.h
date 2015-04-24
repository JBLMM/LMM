#pragma once

#include <LMM/LmmModel/McLmm.h>

class McSpotLmm : public McLmm
{
public:			   
	McSpotLmm(//const Dispersion&   dispersion,
		//const std::vector<double>&    shifts,
		Lmm_PTR lmm,
		const std::vector<double>&    liborsInitValue,
		RNGenerator_PTR               rnGenerator,
		MCSchemeType::MCSchemeType	  mcSchemeType);


	void simulateLMM();
	void computeNumeraires();

	//! forward simulation
	void SpotLmmModel_Euler();
	void SpotLmmModel_Pc();  
	void SpotLmmModel_Ipc(); 

private:
	double Spot_numeraire(size_t k, const matrix& lmm);
	//-- Tests
	//void testTerminalNumeraires();
};
