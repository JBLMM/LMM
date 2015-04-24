#include <LMM/LmmModel/LmmCascade_gCalibrator.h>

#include <LMM/LmmModel/LmmCascade_gCostFunction.h>

#include <LMM/helper/GenericPath.h>

#include <fstream>
#include <cassert>
#include <string.h>
#include <ctime>


void LmmCascade_gCalibrator::solve()
{
	double local_minimization_time = 0;
	QuantLib::Integer local_minimization_stopInfo = -10000;

	const UpperTriangularDoubleMatrix& init_gDelegate_matrix = gMatrixMappingInitiate_.get_gDelegate_Ref();

	size_t max_DelegateRowIndex = init_gDelegate_matrix.size1()-2;

	UpperTriangularDoubleMatrix quoted_matrix = pLmmBaseCostFunction_->get_MarketQuoteValuesMatrix();

	boost::shared_ptr<LmmCascade_gCostFunction> pLmmCascade_gCostFunction = boost::static_pointer_cast<LmmCascade_gCostFunction>(pLmmBaseCostFunction_);
	for(size_t iExperity=1;iExperity <= max_DelegateRowIndex ;++iExperity)
	{
		size_t max_DelegateColIndex = gMatrixMappingInitiate_.get_gDelegate_Ref().size2() - (iExperity+1);

		for(size_t jTenor=1;jTenor <= max_DelegateColIndex ;++jTenor)
		{
			pLmmBaseCostFunction_->reset_SwaptionCell(iExperity,jTenor);

			QuantLib::LevenbergMarquardt minimizationSolver(functionEpsilon(), rootEpsilon(), 1e-16);

			//if(iExperity==7 && jTenor==1){ test_ctndebug_costFunction(iExperity,jTenor,-2,2,200) ;}			
			//if(iExperity==8 && jTenor==1){ test_ctndebug_costFunction(iExperity,jTenor,-2,2,200) ;}
			//if(iExperity==9 && jTenor==4){ test_ctndebug_costFunction(iExperity,jTenor,-6,2,200) ;}

			//if(iExperity==10 && jTenor==1){ test_ctndebug_costFunction(iExperity,jTenor,-20,20,200) ;}
			//if(iExperity==10 && jTenor==5){ test_ctndebug_costFunction(iExperity,jTenor,-20,20,200) ;}
			//if(iExperity==10 && jTenor==6){ test_ctndebug_costFunction(iExperity,jTenor,-20,20,200) ;}

			//if(iExperity==11 && jTenor==1){ test_ctndebug_costFunction(iExperity,jTenor,-0.5,0.5,200) ;}
			//if(iExperity==11 && jTenor==2){ test_ctndebug_costFunction(iExperity,jTenor,-20,20,200) ;}
			//if(iExperity==11 && jTenor==4){ test_ctndebug_costFunction(iExperity,jTenor,-20,20,200) ;}
			//if(iExperity==11 && jTenor==5){ test_ctndebug_costFunction(iExperity,jTenor,-20,20,200) ;}

			//if(iExperity==12 && jTenor==1){ test_ctndebug_costFunction(iExperity,jTenor,-20,20,200) ;}
			//if(iExperity==12 && jTenor==2){ test_ctndebug_costFunction(iExperity,jTenor,-20,20,200) ;}
			//if(iExperity==12 && jTenor==3){ test_ctndebug_costFunction(iExperity,jTenor,-20,20,200) ;}

			//if(iExperity==13 && jTenor==1){ test_ctndebug_costFunction(iExperity,jTenor,-20,20,200) ;}
			//if(iExperity==13 && jTenor==2){ test_ctndebug_costFunction(iExperity,jTenor,-20,20,200) ;}
			//if(iExperity==13 && jTenor==3){ test_ctndebug_costFunction(iExperity,jTenor,-20,20,200) ;}

			//if(iExperity==14 && jTenor==1){ test_ctndebug_costFunction(iExperity,jTenor,-20,20,200) ;}
			//if(iExperity==14 && jTenor==2){ test_ctndebug_costFunction(iExperity,jTenor,-20,20,200) ;}
			//if(iExperity==15 && jTenor==1){ test_ctndebug_costFunction(iExperity,jTenor,-20,20,200) ;}


			QuantLib::Array start_point(1);
			start_point[0] = init_gDelegate_matrix(iExperity,jTenor);

			QuantLib::Problem optimizationProblem(*pLmmBaseCostFunction_.get(), *pConstraint_.get() , start_point);  

			clock_t start_minimizer = std::clock();

			endConvergenceType_ = minimizationSolver.minimize( optimizationProblem, stopCriteria_ );	

			clock_t end_minimizer = std::clock();

			local_minimization_time      = double(end_minimizer - start_minimizer) / CLOCKS_PER_SEC;
			local_minimization_stopInfo  = minimizationSolver.getInfo();

			total_number_called_ += pLmmBaseCostFunction_->get_nbCalled();
			total_minimization_time_ += local_minimization_time ;
			infoStopSolverMatrix_(iExperity,jTenor) = local_minimization_stopInfo ;

			

			const double& found_param = optimizationProblem.currentValue()[0];
			const double& last_cost_funciton = pLmmCascade_gCostFunction->get_current_diff_Value() ;
			const double& last_mdl_value = pLmmCascade_gCostFunction->get_current_mdl_Value();
			const double& f_error = 100* last_cost_funciton / quoted_matrix(iExperity,jTenor);
			std::cout<<"Cascade Calibration ["<<iExperity <<","<<jTenor<<"]	f_error(%)."<<f_error;
			if(!isVirtualCalibration_){
				std::cout<<"	found_param."<<found_param ;
			}
			else
			{
				const double & true_param = true_gDelegate_matrix_(iExperity,jTenor);
				const double calib_diff  = found_param - true_param;
				const double param_error = calib_diff / true_param;
				std::cout<<"	calib_diff."<<calib_diff <<"	param_error(%)."<<param_error;
			}
			std::cout<<std::endl<<std::endl;

			if(found_param<1e-4)// reached param bound
			{
				std::pair<size_t, size_t> pair_indices(iExperity,jTenor);
				bad_swaptions_indices_.push_back( pair_indices );
				bad_swaption_prices_.push_back(last_mdl_value);
				bad_swaption_errors_.push_back(f_error);
				bad_gDelegate_found_.push_back(found_param);
			}
		}
	}
	
	retreive_calib_info();
}

void LmmCascade_gCalibrator::retreive_calib_info()
{
	retreive_calib_global_error();

	boost::shared_ptr<LmmCascade_gCostFunction> pLmmCascade_gCostFunction = boost::static_pointer_cast<LmmCascade_gCostFunction>(pLmmBaseCostFunction_);

	calibrated_gDelegate_matrix_ = pLmmCascade_gCostFunction->get_GMatrixMapping()->get_gDelegate_Ref();

	const size_t swaption_matrix_size = calibrated_gDelegate_matrix_.size1();

	size_t nb_errors_counter=0;
	for(size_t iExperity=1;iExperity<swaption_matrix_size;++iExperity)
	{
		const size_t maxTenorBound =  swaption_matrix_size - iExperity;

		for(size_t jTenor=1;jTenor<maxTenorBound;++jTenor)
		{
			// bad calibrated param
			if(calibrated_gDelegate_matrix_(iExperity,jTenor) <1e-4
				//&& calibrated_gDelegate_matrix_(iExperity,jTenor) > -5000 // case truncated, values are setted  
					)
			{
				std::pair<size_t,size_t> bad_param_indices(iExperity,jTenor);
				bad_swaptions_indices_.push_back(bad_param_indices);
			}

			// Compute param error
			if(isVirtualCalibration_)
			{				
				const double relative_param_error = 100*(calibrated_gDelegate_matrix_(iExperity,jTenor) - true_gDelegate_matrix_(iExperity,jTenor))/true_gDelegate_matrix_(iExperity,jTenor);
				rel_error_gDelegate_matrix_(iExperity,jTenor) = relative_param_error;
				if(std::abs(relative_param_error) > max_param_rel_error_ ) max_param_rel_error_ = std::abs(relative_param_error);
			}
		}
	}

	// ---------------- PRINT GENERAL RESULT INFO OF CALBRATION  ---------------------------
	std::stringstream general_info;

	Shifted_HGVolatilityParam::ABCDParameter abcdParam = pLmmCascade_gCostFunction->get_Shifted_HGVolatilityParam()->get_ABCD();
	general_info<<"a,b,c,d"<<std::endl;
	general_info<<abcdParam.a_<<","<<abcdParam.b_<<","<<abcdParam.c_<<","<<abcdParam.d_<<","<<std::endl;
	
	// ------ print BAD SWAPTIONS
	if(	!bad_swaptions_indices_.empty() )
	{
		general_info<<"BAD Calibrated ,,";
		for(size_t i=0;i<bad_swaptions_indices_.size();++i)
		{
			general_info<<"("<<bad_swaptions_indices_[i].first<<";"<<bad_swaptions_indices_[i].second<<")  ,";
		}
		general_info<<std::endl;

		general_info<<"BAD calibrated Value ,,";
		for(size_t i=0;i<bad_gDelegate_found_.size();++i)
		{
			general_info<<bad_gDelegate_found_[i]<<" ,";
		}
		general_info<<std::endl;

		general_info<<"Price Error(%) ,,";
		for(size_t i=0;i<bad_swaption_errors_.size();++i)
		{
			general_info<<bad_swaption_errors_[i]<<",";
		}
		general_info<<std::endl;
	}
	general_info<<std::endl;
	
	if(isVirtualCalibration_)
	{
		general_info	<<max_param_rel_error_ <<", Max Param Error(%),"<<std::endl;
	}

	base_general_result_info_ += general_info.str();
}

void LmmCascade_gCalibrator::printPlusPlus(const std::string& base_filename) const
{
	std::string path_OutPut = LMMPATH::get_output_path() + base_filename;

	std::ofstream final_result ;

	final_result.open(path_OutPut.c_str(), std::ios::app);

	final_result<<"==============  General Results  ==============="<<std::endl;

	final_result<<base_general_result_info_;

	final_result<<std::endl;

	final_result<<std::endl<<"==================== Quotation ERROR Matrix Results  ==================="<<std::endl;

	final_result<<std::endl<<"## absolute error , "<<std::endl;
	for(size_t i=1;i<abs_quote_error_matrix_.size1();++i)
	{
		for(size_t j=1;j<abs_quote_error_matrix_.size1()-i;++j)
		{
			final_result<<abs_quote_error_matrix_(i,j)<<",";
		}
		final_result<<std::endl;
	}

	final_result<<std::endl<<"## rel error (%), "<<std::endl;
	for(size_t i=1;i<rel_quote_error_matrix_.size1();++i)
	{
		for(size_t j=1;j<rel_quote_error_matrix_.size1()-i;++j)
		{
			final_result<<rel_quote_error_matrix_(i,j)<<",";
		}
		final_result<<std::endl;
	}

	///////  PRINT model quote and market quotes
	UpperTriangularDoubleMatrix mkt_black_vol_matrix = pLmmBaseCostFunction_->get_MarketQuoteValuesMatrix();
	UpperTriangularDoubleMatrix mdl_black_vol_matrix = pLmmBaseCostFunction_->get_ModelQuoteValuesMatrix();

	final_result<<std::endl<<"## Model Black Vol, "<<std::endl;
	for(size_t i=1;i<mdl_black_vol_matrix.size1();++i)
	{
		for(size_t j=1;j<mdl_black_vol_matrix.size1()-i;++j)
		{
			final_result<<mdl_black_vol_matrix(i,j)<<",";
		}
		final_result<<std::endl;
	}

	final_result<<std::endl<<"## Market Black Vol, "<<std::endl;
	for(size_t i=1;i<mkt_black_vol_matrix.size1();++i)
	{
		for(size_t j=1;j<mkt_black_vol_matrix.size1()-i;++j)
		{
			final_result<<mkt_black_vol_matrix(i,j)<<",";
		}
		final_result<<std::endl;
	}

	final_result<<std::endl<<"## Info Cascade solver stop, "<<std::endl;
	for(size_t i=1;i<infoStopSolverMatrix_.size1();++i)
	{
		for(size_t j=1;j<infoStopSolverMatrix_.size1()-i;++j)
		{
			final_result<<infoStopSolverMatrix_(i,j)<<",";
		}
		final_result<<std::endl;
	}

	final_result<<std::endl<<"==================== Quotation ERROR Vector Results  ==================="<<std::endl;
	final_result<<"(Expir;Tenor) , , MKT Quotes, MDL Quotes, , Abs Error, Rel Error (%)"<<std::endl;


	for(size_t iExpirity = 1; iExpirity<mkt_black_vol_matrix.size1()-1; ++iExpirity) // row
	{
		size_t nb_jSwaption = mkt_black_vol_matrix.size2()- iExpirity;
		for(size_t jTenor = 1;jTenor< nb_jSwaption; ++jTenor)
		{
			final_result<<"("<<iExpirity<<";"<<jTenor<<"), ,";
			final_result<<mkt_black_vol_matrix(iExpirity,jTenor) <<","
				<<mdl_black_vol_matrix(iExpirity,jTenor) <<", ,"
				<<abs_quote_error_matrix_(iExpirity,jTenor) <<","
				<<rel_quote_error_matrix_(iExpirity,jTenor) <<",";

			if(std::abs(rel_quote_error_matrix_(iExpirity,jTenor))>30)
			{
				final_result<<", BAD BAD,"<<std::endl;
			}
			else if(std::abs(rel_quote_error_matrix_(iExpirity,jTenor))>10)
			{
				final_result<<", BAD,"<<std::endl;
			}
			else
			{
				final_result<<","<<std::endl;
			}			
		}
	}
	final_result<<std::endl
		<<", , , , Max Error ,"<< max_quote_abs_error_ <<","<< max_quote_rel_error_        <<","<<std::endl<<std::endl;

	if(isVirtualCalibration_)
	{
		final_result<<std::endl<<"==================== Param ERROR Vector Results for Virtual Test gDelegate ==================="<<std::endl;
		final_result<<"(Expir;Tenor) , , TrueParam, Calibrated Param, Error (%)"<<std::endl;


		for(size_t iExpirity = 1; iExpirity<mkt_black_vol_matrix.size1()-1; ++iExpirity) // row
		{
			size_t nb_jSwaption = mkt_black_vol_matrix.size2()- iExpirity;
			for(size_t jTenor = 1;jTenor< nb_jSwaption; ++jTenor)
			{
				final_result<<"("<<iExpirity<<";"<<jTenor<<"), ,";
				final_result<<true_gDelegate_matrix_(iExpirity,jTenor) <<","
					<<calibrated_gDelegate_matrix_(iExpirity,jTenor) <<", ,"
					<<rel_error_gDelegate_matrix_(iExpirity,jTenor) <<",";

				if(std::abs(rel_error_gDelegate_matrix_(iExpirity,jTenor))>15)
				{
					final_result<<", BAD BAD,"<<std::endl;
				}
				else if(std::abs(rel_error_gDelegate_matrix_(iExpirity,jTenor))>5)
				{
					final_result<<", BAD,"<<std::endl;
				}
				else
				{
					final_result<<","<<std::endl;
				}			
			}
		}
		final_result<<std::endl
			<<", , , ,Max Error(%) ,"<< max_param_rel_error_        <<","<<std::endl<<std::endl;
	}

	final_result.close();
}

void LmmCascade_gCalibrator::test_ctndebug_costFunction(size_t iExperity, size_t  jTenor, const double minX, const double maxX, size_t nbSeg) const
{
	assert(maxX>minX);
	const double seg = (maxX-minX)/nbSeg;

	std::ostringstream cost_function_debug_file;
	cost_function_debug_file<<"CascadeCostFunctionDebug_i"<<iExperity<<"_j"<<jTenor<<"_.csv";
	std::string cost_function_debug_file_name(cost_function_debug_file.str());

	std::string path_OutPut = LMMPATH::get_output_path() + cost_function_debug_file_name;
	std::ofstream out_stream ; out_stream.open( path_OutPut.c_str() );

	out_stream<<"x, f(x) ,, cost function,, iExperity,"<<iExperity<<",,jTenor,"<<jTenor <<std::endl;

	QuantLib::Array x(1);
	for(size_t iPoint=0;iPoint<=nbSeg;++iPoint)
	{
		const double point_value = minX + iPoint*seg;
		x[0] = point_value;
		QuantLib::Array f = pLmmBaseCostFunction_->values(x);
		assert(x.size()==1 && f.size() == 1);
		out_stream<<x[0]<<","<<f[0]<<std::endl;
	}

	out_stream.close();
}