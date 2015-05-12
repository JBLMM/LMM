#include "Coupon.h"


boost::shared_ptr<Coupon> Coupon::clone()const{return boost::shared_ptr<Coupon>(new Coupon(*this));}