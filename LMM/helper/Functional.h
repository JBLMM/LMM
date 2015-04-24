#include <boost/function.hpp>

//TODO: to use template: without template it seems very slow.
//TODO: active namespace latter

typedef boost::function<double (double)> RRFunction;

//namespace Functional
//{
	RRFunction compos  (const RRFunction& f, const RRFunction& g);
	RRFunction operator+(const RRFunction& f, const RRFunction& g);
	RRFunction operator-(const RRFunction& f, const RRFunction& g);
	RRFunction operator*(const RRFunction& f, const RRFunction& g);
	RRFunction operator/(const RRFunction& f, const RRFunction& g);
//}