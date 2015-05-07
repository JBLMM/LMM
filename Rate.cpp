#include "Rate.h"
#include <boost/shared_ptr.hpp>

typedef boost::shared_ptr<Rate> Rate_PTR;
typedef boost::shared_ptr<const Rate> Rate_CONSTPTR;

Rate::~Rate(){};

//Rate_PTR Rate::clone_Rate_PTR()const{return Rate_PTR(new Rate(*this));}


