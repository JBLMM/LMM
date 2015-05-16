#pragma once


#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

namespace numeric 
{
	// ------------------------------------------------------
	//                     Integrator
	// ------------------------------------------------------
	class Integrator1D
	{		
	public:
		virtual ~Integrator1D(){};

		virtual double integrate(double start, double end, size_t nbPoints, // integral interval [start, end]
			                     const boost::function<double(double)>& func) const = 0;

	};
	typedef boost::shared_ptr<Integrator1D> Integrator1D_PTR;
	typedef boost::shared_ptr<const Integrator1D> Integrator1D_CONSTPTR;



	class Integrator1D_Riemann : public Integrator1D
	{
	public:
		 virtual ~Integrator1D_Riemann(){};
		 //! uniform grid integral
		 double integrate(double start, double end, size_t nbPoints,
			              const boost::function<double(double)>& func) const;
		 		 
	};
	typedef boost::shared_ptr<Integrator1D_Riemann> Integrator1D_Riemann_PTR;
	typedef boost::shared_ptr<const Integrator1D_Riemann> Integrator1D_Riemann_CONSTPTR;
} 

