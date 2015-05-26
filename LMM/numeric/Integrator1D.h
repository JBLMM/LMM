#pragma once


#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <vector>

namespace numeric 
{
	// ------------------------------------------------------
	//                     Integrator
	// ------------------------------------------------------
	class Integrator1D
	{		
		 
	public:
		 
		virtual ~Integrator1D(){};

		virtual double integrate(const boost::function<double(double)>& func) const = 0;

	};
	typedef boost::shared_ptr<Integrator1D> Integrator1D_PTR;
	typedef boost::shared_ptr<const Integrator1D> Integrator1D_CONSTPTR;


	class Integrator1D_Riemann : public Integrator1D
	{
	private:
		std::vector<double> grids_;

	public:
		 virtual ~Integrator1D_Riemann(){};
		 //! uniform grid integral

		 Integrator1D_Riemann(double start, double end, size_t nbPoints);

		 double integrate( const boost::function<double(double)>& func) const;	
	};

//IncrementalIntegrator1D_Riemann : 
	//calcul d'intégrales du type \int_0^t f(u) (\int_0^u g(s) ds) du
//pour le time averaging (Cheyette) :
	//calcul intermédiaire de (\int_0^u l2(s) ds) et interpolation 
	//pour le calcul de \int_0^t l2(u) b(u) (\int_0^u l2(s) ds) du 
	class IncrementalIntegrator1D_Riemann : public Integrator1D
	{
		std::vector<double> grids_;					//T_1, T_2, ..., T_N
		std::vector<double> f_grids_;				//g(T_1), g(T_2), ..., g(T_N)
		//calculs intermediaires stockes :
		std::vector<double> values_;		// \int_0^T1 g(s) ds , \int_0^T2 g(s) ds, ..., \int_0^TN g(s) ds 

	public:
		//ctor  //! uniform grid integral
		IncrementalIntegrator1D_Riemann(double start, double end, size_t nbPoints, std::vector<double> f_grids);  
		
		//dtor
		virtual ~IncrementalIntegrator1D_Riemann(){};
		 
		 double integrate( const boost::function<double(double)>& func) const;

		 const std::vector<double>& get_values() const {return values_;}

		 double get_value(size_t index) const
		 {
			 assert(index>=0 && index <= grids_.size());
			 return values_[index];
		 }
		 		 
	};
	typedef boost::shared_ptr<IncrementalIntegrator1D_Riemann> IncrementalIntegrator1D_Riemann_PTR;
	typedef boost::shared_ptr<const IncrementalIntegrator1D_Riemann> IncrementalIntegrator1D_Riemann_CONSTPTR;
} 

