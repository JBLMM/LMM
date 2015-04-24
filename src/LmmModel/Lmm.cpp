#include <LMM/LmmModel/Lmm.h>
#include <cassert>

//Lmm::Lmm(const Dispersion& dispersion, const std::vector<double>& shifts) // size = N+1, horizon = N
Lmm::Lmm(const Dispersion& dispersion) // size = N+1, horizon = N
		 : dispersion_(dispersion)
		 //, shifts_(shifts)
		 , covarianceTensor_         (dispersion_.get_horizon()+1, dispersion_.get_horizon()+1,dispersion_.get_horizon()+1)
		 , cumulatedCovarianceTensor_(dispersion_.get_horizon()+1, dispersion_.get_horizon()+1,dispersion_.get_horizon()+1)
{
	//assert(shifts.size() == dispersion_.get_horizon()+1);

	initCovarianceTensor();
	initCumulatedCovarianceTensor();
}

void Lmm::update_VolatilityParam(VolatilityParam_PTR vol_param_ptr) 
{
	dispersion_.update_VolatilityParam(vol_param_ptr);

	initCovarianceTensor();
	initCumulatedCovarianceTensor();
}

void Lmm::update_Correlation() 
{
	dispersion_.update_Correlation(); 
	initCovarianceTensor();
	initCumulatedCovarianceTensor();
}

void Lmm::initCovarianceTensor()
{
	//! YY TODO: make the not initialized value to #IND

	// tensor_:  k,i,j
	// L_i, L_j 's integral of vol in [T_{k-1},T_k]: i,j >= k, horizon = N 
	size_t horizon = dispersion_.get_horizon();
	for(size_t indexTime = 1; indexTime <= horizon; ++indexTime)
	{
		for(size_t indexLibor_i = indexTime; indexLibor_i <= horizon; ++indexLibor_i)
		{
			for(size_t indexLibor_j = indexLibor_i; indexLibor_j <= horizon; ++indexLibor_j)
			{
				const double covInt_ij = dispersion_.covIntegral(indexTime-1, indexTime, indexLibor_i, indexLibor_j );

				//! because of the symmetric of the tensor
				covarianceTensor_(indexTime, indexLibor_i, indexLibor_j) = covInt_ij ;
				covarianceTensor_(indexTime, indexLibor_j, indexLibor_i) = covInt_ij ;
			}
		}
	}
}

void Lmm::initCumulatedCovarianceTensor()
{
	// tensor_:  k,i,j
	// L_i, L_j 's integral of vol in [T_0,T_k]: i,j >= k
	size_t horizon = dispersion_.get_horizon();
	for(size_t indexTime = 1; indexTime <= horizon; ++indexTime)
	{
		for(size_t indexLibor_i = indexTime; indexLibor_i <= horizon; ++indexLibor_i)
		{
			for(size_t indexLibor_j = indexLibor_i; indexLibor_j <= horizon; ++indexLibor_j)
			{
				if(indexTime ==1)
				{
					cumulatedCovarianceTensor_(indexTime, indexLibor_i, indexLibor_j) = covarianceTensor_(indexTime, indexLibor_i, indexLibor_j);
				}
				else
				{
					cumulatedCovarianceTensor_(indexTime, indexLibor_i, indexLibor_j) =   cumulatedCovarianceTensor_(indexTime-1, indexLibor_i, indexLibor_j) 
																					    + covarianceTensor_(indexTime, indexLibor_i, indexLibor_j);
				}
				//! because of the symmetric of the tensor
				cumulatedCovarianceTensor_(indexTime, indexLibor_j, indexLibor_i) = cumulatedCovarianceTensor_(indexTime, indexLibor_i, indexLibor_j);
			}

		}
	}

}


LMM::Index Lmm::get_horizon() const { return dispersion_.get_horizon() ; }
LMM::Index Lmm::get_nbLIBOR() const { return dispersion_.get_nbLIBOR() ; }

LMMTenorStructure_PTR Lmm::get_LMMTenorStructure () const {return dispersion_.get_LMMTenorStructure_PTR();}

//const std::vector<double>& Lmm::get_shifts() const {return shifts_;}
//double Lmm::get_shift(LMM::Index indexLibor) const {return shifts_[indexLibor];}

double Lmm::get_covarianceTensor(size_t indexTime, size_t indexLibor_i, size_t indexLibor_j) const
{
	assert(indexTime <= indexLibor_i && indexTime <= indexLibor_j);
	return covarianceTensor_(indexTime, indexLibor_i, indexLibor_j);
}

double Lmm::get_cumulatedcovarianceTensor(size_t indexTime, size_t indexLibor_i, size_t indexLibor_j) const
{
	assert(indexTime <= indexLibor_i && indexTime <= indexLibor_j);
	return cumulatedCovarianceTensor_(indexTime, indexLibor_i, indexLibor_j);
}


void Lmm::write_to_stream(std::ostream& outputstream)const
{
	outputstream<<"covarianceTensor_"<<std::endl;
	covarianceTensor_.write_to_stream(outputstream);

	outputstream<<"cumulatedCovarianceTensor_"<<std::endl;
	cumulatedCovarianceTensor_.write_to_stream(outputstream);
}

std::ostream& operator<<(std::ostream& os, const Lmm& lmm)
{
	lmm.write_to_stream(os);
	return os;
}