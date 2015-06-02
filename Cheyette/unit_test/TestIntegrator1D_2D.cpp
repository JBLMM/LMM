#include "TestIntegrator1D_2D.h"


double Id(double x){return x ;}
double carre(double x){return x * x ;}

void TestIntegrator1D_Riemann()
{
	double start(0.), end(1.) ;
	size_t nbPoints(100) ;

	numeric::Integrator1D_Riemann inte(start, end, nbPoints);
	boost::function<double(double)> f = boost::bind(Id, _1);
	
	boost::function<double(double)> g = boost::bind(carre, _1);
	std::cout << inte.integrate(f) << " vs : 1/2" << std::endl ;
	std::cout << inte.integrate(f) << " vs : 1/2" << std::endl ;
	std::cout << inte.integrate(g) << " vs : 1/3" << std::endl ;
}

void TestIncrementalIntegrator1D_Riemann()
{
	double start(0.), end(10.) ;
	size_t nbPoints(11) ;
	numeric::IncrementalIntegrator1D_Riemann inte(start, end, nbPoints) ;
	boost::function<double(double)> f = boost::bind(Id, _1);

	inte.vecteur_integrate(f) ;
	for (size_t i = 0 ; i < inte.get_values().size() ; ++i)
	{
		std::cout << inte.get_value(i) << " vs : " << i*i/2. << std::endl ;
	}
	numeric::IncrementalIntegrator1D_Riemann inte2(start, end, nbPoints) ;
	boost::function<double(double)> g = boost::bind(carre, _1);

	inte2.vecteur_integrate(g) ;
	for (size_t i = 0 ; i < inte2.get_values().size() ; ++i)
	{
		std::cout << inte2.get_value(i) << " vs : " << i*i*i/3. << std::endl ;
	}
}


void TestIncrementalIntegrator2D_Riemann()
{
	double start(0.), end(1.) ;
	size_t nbPoints(101) ;
	numeric::IncrementalIntegrator2D_Riemann int2D(start, end, nbPoints) ;
	boost::function<double(double)> f_inner = boost::bind(Id, _1);
	boost::function<double(double)> f_outer = boost::bind(Id, _1);
	std::cout << "1er appel " << std::endl ;
	std::cout << int2D.integrate(f_outer, f_inner) << " vs 1/8 = " << 1/8. << std::endl ;
	
	std::cout << "2eme appel " << std::endl ;
	std::cout << int2D.integrate(f_outer, f_inner) << " vs 1/8 = " << 1/8. << std::endl ;

	boost::function<double(double)> f_outer2 = boost::bind(Id, _1);
	std::cout << int2D.integrate(f_outer2, f_inner) << " vs 1/10" << std::endl ;
}

