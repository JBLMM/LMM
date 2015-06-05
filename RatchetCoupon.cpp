#include "RatchetCoupon.h"

RatchetCoupon::RatchetCoupon(	LMM::Index paymentIndex,
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
								double leverage,
								std::string dependentCoupon)
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
	leverage_(leverage),
	dependentCoupon_(dependentCoupon)
{
}

