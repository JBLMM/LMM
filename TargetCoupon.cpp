#include "TargetCoupon.h"

TargetCoupon::TargetCoupon(	LMM::Index paymentIndex,
								double nominal,
								double period, 
								bool   ifFloored,
								double floorStrike,
								bool   ifCapped, 
								double capStrike, 
								Rate_CONSTPTR rate,
								double multiFactor,
								double addFactor, 
								LMM::Index valuationDateIndex,
								double target,
								std::string couponDependency)
	:
	CappedFlooredCoupon(paymentIndex,
		                nominal,
						period, 
						ifFloored,
						floorStrike,
						ifCapped, 
						capStrike, 
						rate,
						multiFactor,
						addFactor, 
						valuationDateIndex),
	target_(target),
	couponDependency_(couponDependency)
{
}