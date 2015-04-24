#include <LMM/helper/Functional.h>
#include <iostream>

double func_f1(double x){return x;}
double func_f2(double x){return x*x;}

void test_Functional()
{
	RRFunction f1 = func_f1;
	RRFunction f2= func_f2;

	RRFunction f_compos = compos(f1,f2);
	RRFunction f_add    = f1 + f2;
	RRFunction f_sub    = f1 - f2;
	RRFunction f_mul    = f1 * f2;
	RRFunction f_div    = f1 / f2;

	double x = 2;

	std::cout << "x= " << x << std::endl;
	std::cout << "f_compos = " << f_compos(x) << std::endl;
	std::cout << "f_add = " << f_add(x) << std::endl;
	std::cout << "f_sub = " << f_sub(x) << std::endl;
	std::cout << "f_mul = " << f_mul(x) << std::endl;
	std::cout << "f_div = " << f_div(x) << std::endl;
}