#include <cassert>

#include <LMM/instrument/VanillaSwap.h>

#include <fstream>

#include <LMM/helper/GenericPath.h>
#include <LMM/helper/Name.h>
#include <LMM/helper/BuildVariable.h>

VanillaSwap::VanillaSwap(){}

//! Suppose there is a LMM Tenor structure: {T_k}_{k=0}^{N}, and forall k, T_{k+1} - T_k = liborTenor, the unity is in Month. 
//! a swap begins at: T_i, end at T_j.
VanillaSwap::VanillaSwap(const double& strike,
						 LMM::Index indexStart, 
						 LMM::Index indexEnd, 
						 const Tenor& floatingLegTenorType,
						 const Tenor& fixedLegTenorType,
						 LMMTenorStructure_PTR lmmTenorStructure)
						 : strike_(strike)
						 , indexStart_(indexStart)
						 , indexEnd_(indexEnd)
						 , floatingLegTenorType_(floatingLegTenorType)
						 , fixedLegTenorType_(fixedLegTenorType)
						 , lmmTenorStructure_(lmmTenorStructure)						 
{
	floatingVsLiborTenorTypeRatio_ = floatingLegTenorType_.ratioTo(lmmTenorStructure_->get_tenorType() ) ;
	fixedVsLiborTenorTypeRatio_    = fixedLegTenorType_.ratioTo( lmmTenorStructure_->get_tenorType() )   ;

	assert(floatingLegTenorType_ == lmmTenorStructure_->get_tenorType() ) ;  // floatingTenor == lmmTenor!

	assert( indexEnd_ > indexStart_ ); assert( indexStart_ >=0 );
	assert( (indexEnd_ - indexStart_)%floatingVsLiborTenorTypeRatio_ ==0  );
	assert( (indexEnd_ - indexStart_)%fixedVsLiborTenorTypeRatio_    ==0  );

	size_t nbFloatLeg =  (indexEnd_ - indexStart_)/floatingVsLiborTenorTypeRatio_;

	for(size_t i=0; i<nbFloatLeg ; ++i)
	{
		floatingLegPaymentIndexSchedule_.push_back(indexStart_+(i+1)*floatingVsLiborTenorTypeRatio_);
	}

	size_t nbFixedLeg = (indexEnd_ - indexStart_)/fixedVsLiborTenorTypeRatio_;
	for(size_t i=0; i< nbFixedLeg ; ++i)
	{
		fixedLegPaymentIndexSchedule_.push_back(indexStart_+(i+1)*fixedVsLiborTenorTypeRatio_);
	}

	//precalculate deltaT for fixed and float legs, moved from LmmVanillaSwapPricer (30.05.2014)
	assert( lmmTenorStructure_->get_horizon()   >= this->get_indexEnd() );// if not cannot price this swap;

	//! floatingLeg
	const std::vector<LMM::Index>& floatingLegPaymentIndexSchedule = this->get_floatingLegPaymentIndexSchedule();
	deltaTFloatingLeg_.resize( floatingLegPaymentIndexSchedule.size() );
	for(size_t itr = 0; itr<deltaTFloatingLeg_.size(); ++itr)
	{
		size_t index = floatingLegPaymentIndexSchedule[itr];
		deltaTFloatingLeg_[itr] = lmmTenorStructure_->get_deltaT(index-1); // T[index] - T[index-1]
	}	

	//! fixedLeg
	const std::vector<LMM::Index>& fixedLegPaymentIndexSchedule    = this->get_fixedLegPaymentIndexSchedule();
	deltaTFixedLeg_.resize(fixedLegPaymentIndexSchedule.size());
	for(size_t itr = 0; itr<deltaTFixedLeg_.size(); ++itr)
	{
		size_t index = fixedLegPaymentIndexSchedule[itr];
		double t2 = lmmTenorStructure_->get_tenorDate(index);
		double t1 = lmmTenorStructure_->get_tenorDate(index - this->get_fixedLegTenorLmmTenorRatio() );
		deltaTFixedLeg_[itr] = t2-t1;
	}
}

void VanillaSwap::write_to_stream(std::ostream& outputstream)const 
{
	if(lmmTenorStructure_ != NULL)// if lmmTenorStructure_ is not an empty pointer
	{
		outputstream << "--- VanillaSwap Info ---" <<std::endl;
		outputstream << "floatingLegTenorType_ ; "    <<" fixedLegTenorType_ ; "   <<" simulationTenorType_ ; "                    <<  std::endl;
		outputstream <<  floatingLegTenorType_ <<" ; "<<  fixedLegTenorType_<<" ; "<<  lmmTenorStructure_->get_tenorType()<<" ; "<< std::endl;

		outputstream << "strike_ ; "    <<" indexStart_ ; "   <<" indexEnd_ ; " <<        std::endl;
		outputstream <<  strike_ <<" ; "<<  indexStart_<<" ; "<<  indexEnd_     <<" ; "<< std::endl;

		outputstream << "     ; "    <<" Date Start ; "             <<" Date End; "       <<      std::endl ;
		outputstream << "     ; "    <<this->get_StartDate()<<" ; " <<this->get_EndDate() <<";"<< std::endl;


		const std::vector<double> & lmm_dates = lmmTenorStructure_->get_tenorDate();
		outputstream << ";;;;LMM Dates ; ";
		for(size_t i=0; i<lmm_dates.size(); ++i)
		{
			outputstream << lmm_dates[i] <<";" ;
		}outputstream <<std::endl;

		outputstream << ";;;;floatingPaymentIndexSchedule_ ; ";
		for(size_t i=0; i<floatingLegPaymentIndexSchedule_.size(); ++i)
		{
			outputstream << floatingLegPaymentIndexSchedule_[i] <<";" ;
		}outputstream <<std::endl;

		outputstream << ";;;;fixedLegPaymentIndexSchedule_ ; ";
		for(size_t i=0; i<fixedLegPaymentIndexSchedule_.size(); ++i)
		{
			outputstream <<fixedLegPaymentIndexSchedule_[i]<<";";
		}outputstream <<std::endl;

		outputstream << ";;;;deltaTFloatingLeg_ ;";
		for(size_t i=0; i<deltaTFloatingLeg_.size(); ++i)
		{
			outputstream << deltaTFloatingLeg_[i]<<";" ;
		}outputstream <<std::endl;

		outputstream << ";;;;deltaTFixedLeg_ ;";
		for(size_t i=0; i<deltaTFixedLeg_.size(); ++i)
		{
			outputstream <<deltaTFixedLeg_[i]<<";";
		}outputstream <<std::endl;
	}
	else
	{
		if(LMM::WARNLMM ())
			outputstream <<LMM::WARN_MSG<< " VanillaSwap is empty " <<std::endl;	
	}
}

void VanillaSwap::print(const std::string& filename) const 
{
	std::string path_OutPut = LMMPATH::get_output_path() + filename;

	std::ofstream outputstream;
	outputstream.open(path_OutPut.c_str());
	outputstream<<*this<<std::endl;
	outputstream.close();
}




std::ostream& operator<<(std::ostream& os, const VanillaSwap& swap){ swap.write_to_stream(os) ; return os; }