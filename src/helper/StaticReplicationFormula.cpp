#include <boost/bind.hpp>

#include <ql/math/integrals/simpsonintegral.hpp>

#include <LMM/helper/StaticReplicationFormula.h>

double StaticReplicationFormula::evaluate() const // do integration
{

	QuantLib::SimpsonIntegral numInt(absoluteAccuracy_, maxEvaluations_);

	boost::function<double (double)> integrantLower = boost::bind(&StaticReplicationFormula::integrantLowerFunc,this,_1);
	boost::function<double (double)> integrantUpper = boost::bind(&StaticReplicationFormula::integrantUpperFunc,this,_1);

	return f_(F_) 
		   + numInt(integrantLower, boundLower_, F_) 
		   + numInt(integrantUpper, F_, boundUpper_);
}
