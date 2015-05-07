#pragma once
#include <boost/shared_ptr.hpp>



class Rate1
{
public:
	virtual ~Rate1(){}
	virtual boost::shared_ptr<Rate1> clone()const{return boost::shared_ptr<Rate1>(new Rate1(*this));}
};
typedef boost::shared_ptr<Rate1> Rate_PTR;
typedef boost::shared_ptr<const Rate1> Rate_CONSTPTR;



