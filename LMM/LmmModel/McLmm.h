#pragma once

#include <iostream>
#include <cassert>
#include <string>
#include <cassert>

#include <boost/numeric/ublas/matrix_proxy.hpp>

#include <ql/termstructures/volatility/abcd.hpp>

#include <LMM/helper/TypeConverter.h>
#include <LMM/RNGenerator/RNGenerator.h>
#include <LMM/LmmModel/Lmm.h>



namespace MCSchemeType
{
	enum MCSchemeType{ EULER, PC, IPC}; // Euler, predictor-corrector, improved predictor corrector (more fast)
	std::string mcSchemeType2String(MCSchemeType mcSchemeType);
}

typedef boost::numeric::ublas::matrix<double> matrix;


class McLmm
{
protected:
	Lmm_PTR lmm_;

	//! YY exist in Dispersion, but only for the simplicity of writing the code, put them here.
	size_t nbFactor_;   // = 3
	size_t horizon_;    // N: max Libor
	Matrix B_; 
	std::vector<double> liborsInitValue_; // used for equivalentShift for BetaLmm.

	//! comes from simulation
	std::vector<double> numeraires_; // numeraire[i] = P(T_i,T_{N+1})  , size = N+1  
	matrix liborMatrix_;             // liborMatrix_(i,j) = L_i(T_j)   , size = (N+1,N+1), first row&column not useful.
	RNGenerator_PTR rnGenerator_;
	MCSchemeType::MCSchemeType    mcSchemeType_;


	//! YY: TODO: for the moment suppose simulation step = delta_T, so: only need to pre-calculate 3D covarianceTensor.
	//            When need longStepping or more fine step, should modify this structure.
	//! tensor_(k,i,j): L_i, L_j 's integral of vol in [T_{k-1},T_k]
    //                  k = [1,...,N], i,j = [k,...,N]
	//void initCovarianceTensor();

	//! diffusion integration.
	//! int_{T_{k-1}}^{T_k} sigma_i(t) dW_t,   indexTime = k
	Real computeIntSto(size_t indexTime, size_t indexLibor, const std::vector<double>& G) const;

public:

	McLmm(Lmm_PTR lmm,
		  const std::vector<double>&         liborsInitValue,  // size = N+1
		  RNGenerator_PTR                    rnGenerator,      
		  MCSchemeType::MCSchemeType		 mcSchemeType);

	virtual void simulateLMM()         =0;
	virtual void computeNumeraires()   =0;
	
	

	//-- Getters 
	//size_t get_horizon() {return horizon_;}
	//const LMMTenorStructure& get_LMMTenorStructure () const {return dispersion_.get_LMMTenorStructure_PTR();}
	//const std::vector<double>& get_shifts()			  const {return shifts_;}
	const matrix& get_liborMatrix()                   const {return liborMatrix_;}
	const std::vector<double>& get_numeraire()		  const {return numeraires_;}
	double get_numeraire(size_t index)				  const {return numeraires_[index];}
	MCSchemeType::MCSchemeType  get_mcSchemeType()    const{return mcSchemeType_;}
	RNGenerator_PTR get_RNGenerator() {return rnGenerator_;}  //! needed when resetSeed

	Lmm_PTR get_lmm() const {return lmm_;}

	////-- Setters 
	//void set_shifts(const std::vector<double>& shifts);

	virtual void print(const std::string& filename) const;
	
private :

	void initLiborMatrix(const std::vector<double>& libors_init);


};

typedef boost::shared_ptr<McLmm> McLmm_PTR;
