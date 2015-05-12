#include "CappedFlooredCoupon.h"

CappedFlooredCoupon::CappedFlooredCoupon(double paymentDate, double nominal, double period,  double floor,  double cap, Rate_PTR rate, double multiFactor,double addFactor, double valuationDate)
{
		paymentDate_=paymentDate;
		nominal_=nominal;
		period_=period;
		//ifFloored_=ifFloored;
		floor_=floor;
		//ifCapped_=ifCapped;
		cap_=cap;
		rate_=rate;
		multiFactor_=multiFactor;
		addFactor_=addFactor;
		valuationDate_=valuationDate;
}


CappedFlooredCoupon::CappedFlooredCoupon(const CappedFlooredCoupon& c)
{		
		paymentDate_=c.getPaymentDate();
		nominal_=c.getNominal();
		period_=c.getPeriod();
		//ifFloored_=c.getIfFloored();
		floor_=c.getFloor();
		//ifCapped_=c.getIfCapped();
		cap_=c.getCap();
		rate_=(c.getRate()->clone());
		multiFactor_=c.getMultiFactor();
		addFactor_=c.getAddFactor();
		valuationDate_=c.getValuationDate();
}


Coupon_PTR CappedFlooredCoupon::clone()const{return Coupon_PTR(new CappedFlooredCoupon(*this));}