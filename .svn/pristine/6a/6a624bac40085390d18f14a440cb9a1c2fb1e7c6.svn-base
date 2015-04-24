#include <lmm/helper/Functional.h>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>

//TODO: to use template, and RnR latter. 

//namespace Functional
//{
	//typedef boost::function<double (double)> RRFunction;

	double Identity (double x){return x;}
	double add (double x, double y) {return x+y;}
	double sub (double x, double y) {return x-y;}
	double mul (double x, double y) {return x*y;}
	double divide (double x, double y) {return x/y;}

	RRFunction IDENTITY = Identity;
	boost::function<double (double, double)> ADD = add;
	boost::function<double (double, double)> SUB = sub;
	boost::function<double (double, double)> MUL = mul;
	boost::function<double (double, double)> DIV = divide;

	RRFunction compos (const RRFunction& f, const RRFunction& g)
	{
		return boost::bind(f,boost::bind(g,_1));
	}
	RRFunction operator+(const RRFunction& f, const RRFunction& g)
	{
		return boost::bind(ADD, boost::bind(f,_1), boost::bind(g,_1));
	}
	RRFunction operator-(const RRFunction& f, const RRFunction& g)
	{
		return boost::bind(SUB, boost::bind(f,_1), boost::bind(g,_1));
	}
	RRFunction operator*(const RRFunction& f, const RRFunction& g)
	{
		return boost::bind(MUL, boost::bind(f,_1), boost::bind(g,_1));
	}
	RRFunction operator/(const RRFunction& f, const RRFunction& g)
	{
		return boost::bind(DIV, boost::bind(f,_1), boost::bind(g,_1));
	}
//}