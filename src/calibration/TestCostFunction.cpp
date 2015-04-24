#include <LMM/calibration/TestCostFunction.h>
#include <cassert>

TestCostFunction::TestCostFunction(const double& a, const double& b, const double& c)
: a_(a),b_(b),c_(c)
{}

TestCostFunction::~TestCostFunction() {}

QuantLib::Real TestCostFunction::value(const QuantLib::Array & x) const
{
	return a_ + b_*x[0] + c_*x[0]*x[0];
}

QuantLib::Disposable<QuantLib::Array> TestCostFunction::values(const QuantLib::Array& x) const
{
	//QL_REQUIRE (x. size ()==1, "x is a one dimension variable");
	assert(x.size ()==1);
	QuantLib::Array res(1);
	res[0] = value(x);
	return res;
}

const double& TestCostFunction::get_a() const {return a_;}
const double& TestCostFunction::get_b() const {return b_;}
const double& TestCostFunction::get_c() const {return c_;}