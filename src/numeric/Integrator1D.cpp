#include <LMM/numeric/Integrator1D.h>
#include <vector>
#include <iostream>

namespace numeric 
{
//
//Riemann
//
	Integrator1D_Riemann::Integrator1D_Riemann(double start, double end, size_t nbPoints)
		:grids_(nbPoints)
	{
		if(nbPoints < 2)
				throw("Error in Integrator1D_UniformGrid, size should be >=2 ");

		double delta = (end-start)/(nbPoints-1);
		grids_[0] = start;
		for(size_t i=1; i<nbPoints; ++i)
		{
			grids_[i] = grids_[i-1] + delta;
		}
	}

	double Integrator1D_Riemann::integrate( const boost::function<double(double)>& func) const
	{
		double res = 0 ;
		for(size_t i=0; i<grids_.size()-1; ++i)
		{
			double t1 = grids_[i];
			double t2 = grids_[i+1];
			double t_mid = (t1+t2)/2;
			double current_result = func(t_mid)*(t2-t1);   // simplest one :)
		   
			res += current_result;
		}
		return res ;
	}

//
//Incremental Riemann
//
	IncrementalIntegrator1D_Riemann::IncrementalIntegrator1D_Riemann(double start, double end, size_t nbPoints, std::vector<double> f_grids)
		:grids_(nbPoints), values_(nbPoints), f_grids_(f_grids)
	{
		assert(end >= start) ;
		assert(f_grids.size() == nbPoints);
		if(nbPoints < 2)
				throw("Error in IncrementalIntegrator1D_Riemann, size should be >=2 ");

		double delta = (end-start)/(nbPoints-1);

		grids_[0] = start;
		values_[0] = 0 ;

		for(size_t i=1; i<nbPoints; ++i)
		{
			grids_[i] = grids_[i-1] + delta;
			values_[i] = values_[i-1] + 1/2. * (f_grids_[i-1]+f_grids_[i]) * delta ;  //grille uniforme 
		}
	}


	double IncrementalIntegrator1D_Riemann::integrate( const boost::function<double(double)>& func) const
	{
	   double res = 0;
	   for(size_t i=0; i<grids_.size()-1; ++i)
	   {
		   double t1 = grids_[i];
		   double t2 = grids_[i+1];

		   double t_mid = 1/2. * (t1+t2);
		   double current_result = func(t_mid)*(t2-t1);   // simplest one :)
		   
		   //approx de l'integrale \int_0^u g(s) ds pour u entre T_i et T_{i+1}
		   double approx_integrale = 1/2. * (values_[i] + values_[i+1])	 ; //    *(t2-t1);   
		   res += current_result * approx_integrale ;
	   }
	   return res ;
	}
} 