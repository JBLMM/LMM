#include "Rate.h"
#include <boost/shared_ptr.hpp>

boost::shared_ptr<Rate1> Rate1::clone()const{return boost::shared_ptr<Rate1>(new Rate1(*this));}

//Rate_PTR Rate::clone_Rate_PTR()const{return Rate_PTR(new Rate(*this));}


