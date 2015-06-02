#include "JBLMM/Element/Coupon.h"

Coupon::Coupon(const LMM::Index paymentIndex)
	:paymentIndex_(paymentIndex)
{
}

Coupon_PTR Coupon::clone()const
{
	return Coupon_PTR(new Coupon(*this));
}