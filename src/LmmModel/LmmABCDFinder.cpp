#include <LMM/LmmModel/LmmABCDFinder.h>
#include <LMM/helper/GenericPath.h>


QuantLib::Disposable<QuantLib::Array> comp_blackVol(const std::vector<double>& timeline, const QuantLib::Array& abcd) 
{
	size_t nbPoints = timeline.size() ;

	QuantLib::Array mdl_black_values(nbPoints,0.);

	bool isAllowableABCD = true;

	// check abcd condition, if not allowable, put a absurd value into computation in order to block the minimizator go to this direction
	if(    abcd[0]+abcd[3]<=0 // a+d > 0
		|| abcd[3]<=0      // d   > 0
		|| abcd[2]<=0      // c   > 0
		){
			isAllowableABCD =false;
	}

		if(isAllowableABCD)
	{
		QuantLib::AbcdFunction  abcdfunc(abcd[0],abcd[1],abcd[2],abcd[3]);
		
		// compute abcd model value square black vol multiplying by maturity
		for(size_t maturityIndex=0;maturityIndex<nbPoints;++maturityIndex)
		{
			const double & maturity_date = timeline[maturityIndex]  ;	assert(maturity_date>0);

			if(maturityIndex==0)
			{
				const double & time_interval = timeline[0];
				mdl_black_values[maturityIndex] += abcdfunc(maturity_date)*abcdfunc(maturity_date) * time_interval ;
			}
			else
			{
				// build piecewise vol for maturity T_i
				QuantLib::Array abcd_piecewise_vol(maturityIndex+1,0.);
								
				for(size_t iTime=0;iTime<abcd_piecewise_vol.size();++iTime)
				{
					const double time_to_maturity = maturity_date - timeline[iTime] ; assert(time_to_maturity>=0);
					abcd_piecewise_vol[iTime] = abcdfunc(time_to_maturity);
				}

				// accumulate square sum 
				mdl_black_values[maturityIndex]+= timeline[0]*abcd_piecewise_vol[0]*abcd_piecewise_vol[0];
				for(size_t iTime=1;iTime<abcd_piecewise_vol.size();++iTime)
				{
					const double time_interval = timeline[iTime] - timeline[iTime-1] ; assert(time_interval>0);
					
					mdl_black_values[maturityIndex] += time_interval*abcd_piecewise_vol[iTime]*abcd_piecewise_vol[iTime];				
				}

			}
		}

		// compute model black vol
		for(size_t maturityIndex=0;maturityIndex<nbPoints;++maturityIndex)
		{
			const double & maturity_date = timeline[maturityIndex];
			mdl_black_values[maturityIndex]/=maturity_date;			
			mdl_black_values[maturityIndex] = std::sqrt(mdl_black_values[maturityIndex]);
		}
	}
	else
	{
		for(size_t i=0;i<mdl_black_values.size();++i) mdl_black_values[i] = 10000000;// mkt_vol_values_[i];//
	}

	return mdl_black_values;
}


ABCDSimpleCostFunction::ABCDSimpleCostFunction( const std::vector<double>& time_line, const std::vector<double>& vol_values)
	: mkt_timeline_(time_line.size(),-1000)
	, mkt_vol_values_(vol_values.size(),-1000)
{
	size_t nbPoints = time_line.size();
	assert(nbPoints == vol_values.size() );

	for(size_t i=0;i<nbPoints;++i)
	{
		mkt_timeline_[i]   = time_line[i];	
		mkt_vol_values_[i] = vol_values[i];
	}	
}

QuantLib::Real ABCDSimpleCostFunction::value(const QuantLib::Array & x) const 
{
	QL_REQUIRE (x. size ()==4 , " abdc params is 4-dim");

	double sum_squares = 0;

	QuantLib::Array vol_diff = values(x);
	for(size_t i=0;i<vol_diff.size();++i)
	{
		sum_squares+=vol_diff[i]*vol_diff[i];
	}

	return sum_squares;
}	

QuantLib::Disposable<QuantLib::Array> ABCDSimpleCostFunction::values(const QuantLib::Array& x) const 
{
	QL_REQUIRE (x. size ()==4 , " abdc params is 4-dim");

	QuantLib::AbcdFunction  abcdfunc(x[0],x[1],x[2],x[3]);

	size_t nbPoints = mkt_timeline_.size(); 

	QuantLib::Array vol_diff(nbPoints);

	for(size_t i=0;i<nbPoints;++i)
	{
		double abcd_vol_value_i = abcdfunc(mkt_timeline_[i] );

		vol_diff[i]=mkt_vol_values_[i] - abcd_vol_value_i;
	}

	std::cout<<"abcd : "<<x<<std::endl<<"cost :"<<vol_diff<<std::endl<<std::endl;

	return vol_diff;
}



QuantLib::Disposable<QuantLib::Array> ABCDCumulativeCostFunction::values(const QuantLib::Array& x) const 
{
	QL_REQUIRE (x. size ()==4 , " abdc params is 4-dim");

	QuantLib::Array mdl_black_values = comp_blackVol(mkt_timeline_,x) ; 

	size_t nbPoints = mkt_vol_values_.size() ;
	assert(mdl_black_values.size() == nbPoints );

	QuantLib::Array vol_diff(nbPoints);

	// first element in the cost diff converg too which, we add here some weight in order to have balance of convergence speed

	for(size_t i=0;i<nbPoints;++i)
	{
		vol_diff[i]= mdl_black_values[i] - mkt_vol_values_[i];
	}

	std::cout<<"abcd : "<<x<<std::endl<<"cost :"<<vol_diff<<std::endl<<std::endl;

	return vol_diff;
}



void LmmABCDFinder::solve()
{
	QuantLib::NoConstraint empty_constraint;	

	QuantLib::LevenbergMarquardt minimizationSolver(stopCriteria_.functionEpsilon(), stopCriteria_.rootEpsilon(), 1e-16);

	QuantLib::Array start_abcd (4);
	start_abcd[0]=0.156147;
	start_abcd[1]=0.363384;
	start_abcd[2]=0.407813;
	start_abcd[3]=1.;

	QuantLib::Problem optimizationProblem(abdcCostFunc_, empty_constraint, start_abcd);  

	endConvergenceType_ = minimizationSolver.minimize( optimizationProblem, stopCriteria_ );	

	found_abcd = optimizationProblem.currentValue();
}

void LmmABCDFinder::print( const QuantLib::Array & found_abcd
					     , const std::vector<double>& time_line
					     , const std::vector<double>& black_vol
					     , const std::string & filename)
{
	assert(time_line.size() == black_vol.size() );
	size_t nbQuote = time_line.size();
	
	std::string path_OutPut = LMMPATH::get_output_path() + filename;
	std::ofstream final_result ;
	final_result.open(path_OutPut.c_str());
	
	final_result<<"a, b, c, d,"<<std::endl;
	final_result<<found_abcd[0]<<","<<found_abcd[1]<<","<<found_abcd[2]<<","<<found_abcd[3]<<","<<std::endl<<std::endl;
	
	final_result<<"Quote Black Vol analysis ==== ,"<<std::endl;
	final_result<<"Time,";for(size_t i=0;i<nbQuote;++i){final_result<< time_line[i] <<",";} final_result<<std::endl;
	final_result<<"Quoted Vol ,";for(size_t i=0;i<nbQuote;++i){final_result<< black_vol[i] <<",";} final_result<<std::endl;
	final_result<<"Vol Square Mul T,";for(size_t i=0;i<nbQuote;++i){final_result<< time_line[i]*black_vol[i]*black_vol[i] <<",";} final_result<<std::endl<<std::endl<<std::endl;


	QuantLib::Array abcd_BlackVol = comp_blackVol(time_line,found_abcd) ; 
	final_result<<"Comparison ==== ,"<<std::endl;
	final_result<<"Time,";for(size_t i=0;i<nbQuote;++i){final_result<< time_line[i] <<",";} final_result<<std::endl;
	final_result<<"Quoted Vol ,";for(size_t i=0;i<nbQuote;++i){final_result<< black_vol[i] <<",";} final_result<<std::endl;
	final_result<<"abcd Black Vol ,";for(size_t i=0;i<nbQuote;++i){final_result<< abcd_BlackVol[i] <<",";} final_result<<std::endl;
	final_result<<"Error(%),";for(size_t i=0;i<nbQuote;++i){final_result<< 100*( black_vol[i] - abcd_BlackVol[i])/black_vol[i]  <<",";} final_result<<std::endl<<std::endl;


	final_result.close();
}

void LmmABCDFinder::print(const std::string & filename)
{
	LmmABCDFinder::print(found_abcd,abdcCostFunc_.get_mkt_timeline(),abdcCostFunc_.get_mkt_vol_values(),filename);

}