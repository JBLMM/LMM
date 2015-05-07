#include "CappedFlooredCoupon.h"

Coupon_PTR CappedFlooredCoupon::clone_Coupon_PTR()const{return Coupon_PTR(new CappedFlooredCoupon(*this));}