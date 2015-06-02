#include <Cheyette/Fonction.h>
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
	IncrementalIntegrator1D_Riemann::IncrementalIntegrator1D_Riemann(double start, double end, size_t nbPoints)
		:grids_(nbPoints), values_(nbPoints)
	{
		assert(end >= start) ;
		if(nbPoints < 2)
				throw("Error in IncrementalIntegrator1D_Riemann, size should be >=2 ");

		double delta = (end-start)/(nbPoints-1);

		grids_[0] = start;
		values_[0] = 0 ;

		for(size_t i=1; i<nbPoints; ++i)
		{
			grids_[i] = grids_[i-1] + delta;
		}
	}


	void IncrementalIntegrator1D_Riemann::vecteur_integrate( const boost::function<double(double)>& func) const
	{
	   double res = 0;
	
	   values_[0] = 0.0;
	   for(size_t i=0; i<grids_.size()-1; ++i)  //securité si double appel de vecteur integrate
	   {  
		   values_[i]  = 0 ;
	   }
	   for(size_t i=0; i<grids_.size()-1; ++i)
	   {
		   double t1 = grids_[i];
		   double t2 = grids_[i+1];

		   double t_mid = 1/2. * (t1+t2);
		   double current_result = func(t_mid)*(t2-t1);   // simplest one :)
		  
		   values_[i+1]  += values_[i] + current_result ;
	   }
	}

	IncrementalIntegrator2D_Riemann::IncrementalIntegrator2D_Riemann(double start, double end, size_t nbPoints)
		:incrementalIntegrator_inner_(start,end,nbPoints),
		incrementalIntegrator_outer_(start,end,nbPoints)
	{
	
	}

	double IncrementalIntegrator2D_Riemann::integrate( const boost::function<double(double)>& func_outer, 
		                                               const boost::function<double(double)>& func_inner) const
	{
		 incrementalIntegrator_inner_.vecteur_integrate(func_inner);

		 std::vector<double> x_grid = incrementalIntegrator_inner_.get_grids();
		 std::vector<double> value_grid = incrementalIntegrator_inner_.get_values();
		 Interpolation_RR_Function incrementalFunction_inner (x_grid, value_grid);
		 Boost_RR_Function func_outer2(func_outer);
		 //RR_Function_CONSTPTR func_outer2(new Boost_RR_Function(func_outer));

		RR_Function_ComposeByProduct func(func_outer2, incrementalFunction_inner);

		 return incrementalIntegrator_outer_.integrate(func);
	}
} 