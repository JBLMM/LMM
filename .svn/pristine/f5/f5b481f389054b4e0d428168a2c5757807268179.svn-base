#include <LMM/LmmModel/LmmABCDCostFunction.h>

LmmABCDCostFunction::LmmABCDCostFunction( LmmVanillaSwaptionApproxPricer_Rebonato_PTR lmmRobonato_ptr                // pricer  
										 , LiborQuotes_ConstPTR liborQuotes_ptr
										 , UpperTriangleVanillaSwaptionQuotes_ConstPTR pUpperTriangleVanillaSwaptionQuotes // instrument to calibrate 
										 , Shifted_HGVolatilityParam_PTR pShifted_HGVolatilityParam
										 )
										 : LmmBaseCostFunction(liborQuotes_ptr , pUpperTriangleVanillaSwaptionQuotes)
										 , pLmmVanillaSwaptionApproxPricer_Rebonato_(lmmRobonato_ptr)
										 , buffer_Shifted_HGVolatilityParam_(pShifted_HGVolatilityParam)
{}

void LmmABCDCostFunction::reset_CalibrationParams(const QuantLib::Array & params) const 
{
	buffer_Shifted_HGVolatilityParam_->reset_ABCD( params );
	pLmmVanillaSwaptionApproxPricer_Rebonato_->update_VolatilityParam( buffer_Shifted_HGVolatilityParam_ );
}

//const Array& param_array
Disposable<Array> LmmABCDCostFunction::values(const Array& param_array) const
{
	// NOTE : 
	// Costfunction::values has to compute only values, not absolute values, nor squared values
	// see Introduction to Selected Classes of QuantLib II _ Dimitri Reiswich 2010 p.57/148

	reset_CalibrationParams(param_array);
	partially_UpdateSwaptionMdlValues();

	size_t nbSwaption = calc_nbSwaptions() ;

	//! diff upperTriangleVanillaSwaptionMktQuotes_ - upperTriangleVanillaSwaptionMdlValues_
	Array diff_cost( nbSwaption );
	size_t vector_counter=0;

	const UpperTriangularVanillaSwaptionQuotes & mkt_swaption_quote = pUpperTriangleVanillaSwaptionQuotes_->get_UpperTriangularVanillaSwaptionQuotes() ;

	if(calib_constraint.empty() )
	{
		for(size_t iExperity=1;iExperity<pUpperTriangleVanillaSwaptionQuotes_->size1()-1;++iExperity)
		{
			size_t nb_jSwaption = pUpperTriangleVanillaSwaptionQuotes_->size2()- iExperity;
			for(size_t jTenor = 1;jTenor< nb_jSwaption; ++jTenor)
			{
				const double diff = mkt_swaption_quote(iExperity,jTenor).second - upperTriangleVanillaSwaptionMdlValues_(iExperity,jTenor);
				diff_cost[vector_counter]= diff;//*diff; // To check with quantLib. --> checked
				++vector_counter;
			}
		}
	}
	else
	{
		assert( calib_constraint.size() >3 );

		for(size_t iCell=0;iCell<calib_constraint.size();++iCell)
		{
			const std::pair<size_t,size_t> & cell = calib_constraint[iCell];
			size_t iExperity = cell.first;
			size_t jTenor    = cell.second;

			const double diff = mkt_swaption_quote(iExperity,jTenor).second - upperTriangleVanillaSwaptionMdlValues_(iExperity,jTenor);
			diff_cost[vector_counter]= diff;//*diff; // To check with quantLib. --> checked
			++vector_counter;
		}
	}
	// check if penalties fully fill the cost vector
	assert(vector_counter == diff_cost.size() ); 

	if(LMM::DEUBGLMM())
	{
		if( breakForPrintOut(nbCalled) ) 
		{
			std::cout<<std::endl<<LMM::NOTIF_MSG<<" iter."<<nbCalled<<std::endl;

			if(buffer_calib_reference.empty() )
			{
				std::cout <<"      x = "<<param_array<<std::endl<<std::endl;
			}
			else
			{
				QuantLib::Array error = error_calib(param_array);
				std::cout <<"diff   = "<<error<<std::endl<<std::endl;
			}
			std::cout <<"f(x,p) = "<<diff_cost<<std::endl;			
		}
	}

	++nbCalled;
	return diff_cost;
}

void LmmABCDCostFunction::partially_UpdateSwaptionMdlValues() const // pricing
{
	//! upperTriangle:

	const UpperTriangularVanillaSwaptionQuotes & mkt_swaption_quotes = pUpperTriangleVanillaSwaptionQuotes_->get_UpperTriangularVanillaSwaptionQuotes() ;
	const std::vector<double>& libor_init = pLiborQuotes_->get_InitLibor();

	if(calib_constraint.empty() )
	{
		fully_UpdateSwaptionMdlValues();	
	}
	else
	{
		assert( calib_constraint.size() >3 );

		for(size_t iCell=0;iCell<calib_constraint.size();++iCell)
		{
			const std::pair<size_t,size_t> & cell = calib_constraint[iCell];
			size_t iExperity = cell.first;
			size_t jTenor    = cell.second;

			const VanillaSwaption & swaption = mkt_swaption_quotes(iExperity,jTenor).first;

			const double blackVol_ij = pLmmVanillaSwaptionApproxPricer_Rebonato_->volBlack(swaption,libor_init);

			upperTriangleVanillaSwaptionMdlValues_(iExperity,jTenor)=blackVol_ij;
		}
	}
}

void LmmABCDCostFunction::fully_UpdateSwaptionMdlValues() const // YY TODO: move to somewhere else
{
	const UpperTriangularVanillaSwaptionQuotes & mkt_swaption_quotes = pUpperTriangleVanillaSwaptionQuotes_->get_UpperTriangularVanillaSwaptionQuotes() ;
	const std::vector<double>& libor_init = pLiborQuotes_->get_InitLibor();

	for(size_t iExperity = 1; iExperity<upperTriangleVanillaSwaptionMdlValues_.size1()-1; ++iExperity) // row
	{
		size_t nb_jSwaption = upperTriangleVanillaSwaptionMdlValues_.size2()- iExperity;
		for(size_t jTenor = 1;jTenor< nb_jSwaption; ++jTenor)
		{
			const VanillaSwaption & swaption = mkt_swaption_quotes(iExperity,jTenor).first;

			const double blackVol_ij = pLmmVanillaSwaptionApproxPricer_Rebonato_->volBlack(swaption,libor_init);

			upperTriangleVanillaSwaptionMdlValues_(iExperity,jTenor)=blackVol_ij;
		}
	}
}

QuantLib::Array LmmABCDCostFunction::error_calib(const QuantLib::Array & actual_param) const
{
	size_t nbParam = actual_param.size();
	QuantLib::Array error_calib(nbParam,0.);
	for(size_t i=0;i<nbParam;++i)
	{
		error_calib[i]  = actual_param[i] - buffer_calib_reference[i] ;
	}
	return error_calib;
}

size_t LmmABCDCostFunction::calc_nbSwaptions() const // YY TODO: ??? not here 
{
	size_t nbSwaption;

	if(calib_constraint.empty() )
	{
		size_t N = pUpperTriangleVanillaSwaptionQuotes_->size1() - 2 ;
		nbSwaption = (N+1)*N/2 ; 
	}
	else
	{
		assert( calib_constraint.size() >3 );
		nbSwaption = calib_constraint.size();
	}

	return nbSwaption;
}

void LmmABCDCostFunction::reset_TrueParam(const QuantLib::Array & true_param)
{
	const size_t nbParam = true_param.size();
	buffer_calib_reference.resize(nbParam);
	for(size_t i=0;i<nbParam;++i)
	{
		buffer_calib_reference[i]=true_param[i];
	}
}

void LmmABCDCostFunction::print(const std::string& filename) const 
{


}