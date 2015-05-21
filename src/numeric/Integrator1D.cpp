#include <LMM/numeric/Integrator1D.h>
#include <vector>
#include <iostream>

namespace numeric 
{
	double Integrator1D_Riemann::integrate(double start, double end, size_t nbPoints,
								           const boost::function<double(double)>& func) const
	{
		//! construct uniform grid, not efficient at all, because it's only a test algorithm
		std::vector<double> grid(nbPoints);
		if(nbPoints < 2)
				throw("Error in Integrator1D_UniformGrid, size should be >=2 ");

		double delta = (end-start)/(nbPoints-1);
		grid[0] = start;
		for(size_t i=1; i<nbPoints; ++i)
		{
			grid[i] = grid[i-1] + delta;
		}
		

	   //! Riemann sum
       double result = 0.0;

	   if(grid.size()<2)
		   throw("Error in Integrator1D::integrate, grid.size() should >= 2");

	 
	   for(size_t i=0; i<grid.size()-1; ++i)
	   {
		   double t1 = grid[i];
		   double t2 = grid[i+1];
		   double t_mid = (t1+t2)/2;
		   result += func(t_mid)*(t2-t1);   // simplest one :)
	   }
	   return result;
	}
} 