#include "Coupon.h"

Coupon::~Coupon(){}
Coupon_PTR  Coupon::clone_Coupon_PTR()const{return Coupon_PTR(new Coupon(*this));}