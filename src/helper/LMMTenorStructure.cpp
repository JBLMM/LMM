//#include <LMM/LmmModel/LMMTenorStructure.h>
#include <LMM/helper/LMMTenorStructure.h>

#include <LMM/helper/GenericPath.h>
#include <LMM/helper/Printer.h>
#include <LMM/helper/BuildVariable.h>
#include <LMM/helper/EqualOperator.h>


LMMTenorStructure::LMMTenorStructure(const Tenor&  tenorType, int max_nbOfYear)
: tenorType_(tenorType)
{
	assert( max_nbOfYear > 1 ) ; 	

	if(!tenorType.isValidTenor()) 
		throw("Error: is not valid tenorType.");

	double tenorTime = tenorType.YearFraction();
	horizon_  = max_nbOfYear * ( 12/tenorType.NbOfMonth() );

	tenorDates_ = std::vector<double>(horizon_+2);  // size=N+2,  [T_0, T_1, T_2, ..., T_N, T_{N+1}]
	for(size_t i=0; i<tenorDates_.size(); ++i)
	{
		tenorDates_[i] = i*tenorTime;
	}

	tenorDeltaT_ = std::vector<double>(horizon_+1); // size = N+1
	for(size_t i=0; i<tenorDeltaT_.size(); ++i)
	{
		tenorDeltaT_[i] = tenorDates_[i+1] - tenorDates_[i];
	}		
}


Tenor LMMTenorStructure::get_tenorType() const {return tenorType_;}

Tenor LMMTenorStructure::get_tenorType(const double& year_fraction) const 
{
	const double eps = 1e-8;
	if     (std::abs(year_fraction - 1./12. ) < eps ) return Tenor::_1M ;
	else if(std::abs(year_fraction - 3./12. ) < eps ) return Tenor::_3M ;
	else if(std::abs(year_fraction - 6./12. ) < eps ) return Tenor::_6M ;
	else if(std::abs(year_fraction - 9./12. ) < eps ) return Tenor::_9M ;
	else if(std::abs(year_fraction - 1.     ) < eps ) return Tenor::_1YR;
	else return Tenor::_Non ;
}

LMM::Index LMMTenorStructure::get_Index(const double& date) const 
{
	LMM::Index result=0;
	bool indexfound=false;

	LMM::Index indexEnd = tenorDates_.size();

	// find the index of the nearest LMMTenorStructure's date
	for(LMM::Index index=0 ; index<indexEnd ; ++index)
	{
		if(std::abs(tenorDates_[index] - date ) < LMM::EPSILON ) 
		{
			result = index; 
			indexfound = true;
			break;
		}
	}

	if(!indexfound)
	{
		if(LMM::WARNLMM ())
			std::cout<< LMM::WARN_MSG << "LMMTenorStructure::getIndex() Index not found for date : "<<date<<std::endl;		
	}

	return result;
}


LMM::Index LMMTenorStructure::get_horizon() const { return horizon_ ;}   // horizon = N
LMM::Index LMMTenorStructure::get_nbLIBOR() const { return horizon_ +1;} // nbLibor = N+1

double              LMMTenorStructure::get_EndDate()            const { return tenorDates_.back(); }

double              LMMTenorStructure::get_deltaT(size_t index) const { return tenorDeltaT_[index]; }
const std::vector<double>& LMMTenorStructure::get_deltaT(            ) const { return tenorDeltaT_;        }

double              LMMTenorStructure::get_tenorDate(size_t index) const { return tenorDates_[index]; }
const std::vector<double>& LMMTenorStructure::get_tenorDate(            ) const { return tenorDates_;        }


bool LMMTenorStructure::is_TenorDate(const double& dateT) const 
{
	//this is a find exact value, maybe a search with tolerence of 10e-9 should be more appropriated
	return ( std::find(tenorDates_.begin(), tenorDates_.end(), dateT)!=tenorDates_.end() ) ;
}

//! equal operator: when modify the class, don't forget to adjust == operator
bool LMMTenorStructure::operator == (const LMMTenorStructure& lmmTenorStructure) const
{
	if(horizon_ != lmmTenorStructure.get_horizon())	return false;

	if(tenorType_ != lmmTenorStructure.tenorType_) return false;

	if(!vectorEqual(tenorDates_,lmmTenorStructure.get_tenorDate(), 1.0e-16)) return false;

	if(!vectorEqual(tenorDeltaT_,lmmTenorStructure.get_deltaT(), 1.0e-16)) return false;

	return true;
}

