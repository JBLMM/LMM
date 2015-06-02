#include "JBLMM/Element/CappedFlooredCoupon.h"
#include <LMM/helper/Name.h>

CappedFlooredCoupon::CappedFlooredCoupon(	LMM::Index paymentIndex,
											double nominal,
											double period,  
											bool ifFloored,
											double floorStrike,
											bool ifCapped,
											double capStrike,
											Rate_CONSTPTR rate,
											double multiFactor,
											double addFactor,
											LMM::Index valuationDateIndex)
										 : 
											Coupon(paymentIndex),
											nominal_(nominal),
											period_(period),
											ifFloored_(ifFloored),
											floorStrike_(floorStrike),
											ifCapped_(ifCapped),
											capStrike_(capStrike),
											rate_(rate),
											multiFactor_(multiFactor),
											addFactor_(addFactor),
											valuationDate_(valuationDateIndex)
{
}

//TODO?latter too long to write the code ... 
CappedFlooredCoupon::CappedFlooredCoupon(const CappedFlooredCoupon& c): Coupon(c.getPaymentIndex()),
																		nominal_(c.getNominal()),
																		period_(c.getPeriod()),
																		ifFloored_(c.getIfFloored()),
																		floorStrike_(c.getFloorStrike()),
																		ifCapped_(c.getIfCapped()),
																		capStrike_(c.getCapStrike()),	
																		multiFactor_(c.getMultiFactor()),
																		addFactor_(c.getAddFactor()),
																		valuationDate_(c.getValuationDate())
{		
		rate_=(c.getRate()->clone());
}


Coupon_PTR CappedFlooredCoupon::clone()const
{
	return Coupon_PTR(new CappedFlooredCoupon(*this));
}