#include <LMM/numeric/Integrator1D.h>
#include <boost/bind.hpp>
#include <iostream>
#include <math.h>

double func_1(double x){return std::sin(x);}

class Functor 
{
public:
	double f(double x)
	{
        return std::sin(x);
	}
};


// boost::function<void (int)> f2( boost::bind( &myclass::fun2, this, _1 ) );

void test_Integrator1D()
{
	/*
	 numeric::Integrator1D_Riemann integral_Riemnan; 

	 double gridStart = 0.0;
	 double gridEnd   = 3.14159265358979;
	 size_t gridSize  = 100;

	 boost::function<double(double)> func1 = func_1;
	 double result1 = integral_Riemnan.integrate(gridStart, gridEnd, gridSize, func1);
   
	 Functor functorObj;
	 boost::function<double(double)> func2 = boost::bind(&Functor::f, functorObj, _1);
	 double result2 = integral_Riemnan.integrate(gridStart, gridEnd, gridSize, func2);

	 std::cout << result1 << " vs " << result2 << std::endl;
	 */
}

double f_test (double x)
{
	return x ;
}

// \int_0^1 u \int_0^u s ds du = 1/8
void test_incremental_integrale2()
{
	double start = 0 ;
	double end = 1 ;
	size_t nbPoints = 100 + 1 ; //delta t = 1/10
	std::vector<double> f_grids ;
	for (size_t i = 0 ; i < nbPoints ; ++i)
	{
		f_grids.push_back(i/100.) ;		//modifier ici aussi le nb de points
	}
	numeric::IncrementalIntegrator1D_Riemann incr(start, end, nbPoints, f_grids); 
	//for (size_t i = 0 ; i < nbPoints ; ++i)			//debogage
	//{
	//	std::cout << incr.get_value(i) << std::endl ;
	//}
	boost::function<double(double)> func1 = f_test;
	std::cout << incr.integrate(func1) << std::endl ;
	std::cout << 1/8. << std::endl ;
}