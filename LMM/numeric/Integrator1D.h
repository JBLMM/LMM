#pragma once


#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <vector>
#include <iostream>

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
		const std::vector<double>& get_grids() const {return grids_;} // not efficient at all ! 
		virtual ~Integrator1D_Riemann(){};
		//! uniform grid integral

		Integrator1D_Riemann(double start, double end, size_t nbPoints);

		double integrate(const boost::function<double(double)>& func) const;	
	};


	//mise en mémoire et utilisation de \int_0^T_i f(s) ds pour calculer \int_0^T_j f(s) ds, avec T_j > T_i
	class IncrementalIntegrator1D_Riemann : public Integrator1D
	{
	private:
		std::vector<double> grids_;					//T_1, T_2, ..., T_N
		mutable std::vector<double> values_;		// \int_0^T1 f(s) ds , \int_0^T2 f(s) ds, ..., \int_0^TN f(s) ds 

	public:
		//ctor  //! uniform grid integral
		IncrementalIntegrator1D_Riemann(double start, double end, size_t nbPoints);  
		
		//dtor
		virtual ~IncrementalIntegrator1D_Riemann(){};
		 
		//remplit le vecteur values_ qui contient les integrales de 0_T1, 0_T2, ... 0_TN
		 void vecteur_integrate( const boost::function<double(double)>& func) const;

		//getters
		const std::vector<double>& get_grids() const {return grids_;}
		const std::vector<double>& get_values() const {return values_;}

		double get_value(size_t index) const
		{
			assert(index>=0 && index <= grids_.size());
			return values_[index];
		}

		double integrate(const boost::function<double(double)>& func) const
		{
			std::cout << "utiliser get_values a la place de integrate" << std::endl ;
			return values_[values_.size() - 1] ;
		}
		 		 
	};
	typedef boost::shared_ptr<IncrementalIntegrator1D_Riemann> IncrementalIntegrator1D_Riemann_PTR;
	typedef boost::shared_ptr<const IncrementalIntegrator1D_Riemann> IncrementalIntegrator1D_Riemann_CONSTPTR;






	class Integrator2D
	{		
	public:
		 
		virtual ~Integrator2D(){};

		virtual double integrate(const boost::function<double(double)>& func1, const boost::function<double(double)>& func2) const = 0;

	};
	typedef boost::shared_ptr<Integrator1D> Integrator1D_PTR;
	typedef boost::shared_ptr<const Integrator1D> Integrator1D_CONSTPTR;
//IncrementalIntegrator1D_Riemann : 
	//calcul d'intégrales du type \int_0^t f(u) (\int_0^u g(s) ds) du
//pour le time averaging (Cheyette) :
	//calcul intermédiaire de (\int_0^u l2(s) ds) et interpolation 
	//pour le calcul de \int_0^t l2(u) b(u) (\int_0^u l2(s) ds) du 
	class IncrementalIntegrator2D_Riemann : public Integrator2D
	{
	private:
		IncrementalIntegrator1D_Riemann incrementalIntegrator_inner_;
		Integrator1D_Riemann            incrementalIntegrator_outer_;

	public:
		//ctor  //! uniform grid integral
		IncrementalIntegrator2D_Riemann(double start, double end, size_t nbPoints);  
		
		//dtor
		virtual ~IncrementalIntegrator2D_Riemann(){};
		 
		double integrate( const boost::function<double(double)>& func_outer, const boost::function<double(double)>& func_inner) const;	 		 
	};
	typedef boost::shared_ptr<IncrementalIntegrator1D_Riemann> IncrementalIntegrator1D_Riemann_PTR;
	typedef boost::shared_ptr<const IncrementalIntegrator1D_Riemann> IncrementalIntegrator1D_Riemann_CONSTPTR;


} 


