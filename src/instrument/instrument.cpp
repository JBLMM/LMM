//#include <cassert>
//
//
//#include <LMM/instrument/nstrument.h>
//
//using namespace Instruments;
//
//Fixed::Fixed(double K): K_(K) {}
//
//Libor::Libor():tenor_(0.), T1_(0.), T2_(0.) {}
//
//Libor::Libor(double T1, double T2, double tenor) {
//	assert(T2-T1 == tenor);
//	T1_ = T1;
//	T2_ = T2;
//	tenor_ = tenor;
//}
//
//Caplet::Caplet(double T_expiry, double tenor, double strike) {
//	T_expiry_ = T_expiry;
//	tenor_ = tenor;
//	strike_ = strike;
//	liborIndex_ = Libor(T_expiry_,T_expiry_+tenor_,tenor_);
//}
//
//double Caplet::payoff() {
//	return std::max(1. - strike_,0.);
//}
//
//Swap::Swap(double startDate, 
//		  double endDate, 
//		  const InstrumentIndex & leg1, 
//		  const InstrumentIndex & leg2, 
//		  double leg1_payment_freq, 
//		  double leg2_payment_freq)
//{
//	startDate_ = startDate;
//	endDate_ = endDate;
//	Leg1_index_ = new InstrumentIndex(leg1);
//	Leg2_index_ = new InstrumentIndex(leg2);
//	Leg1_payementFrequency_ = leg1_payment_freq;
//	Leg2_payementFrequency_ = leg2_payment_freq;
//}
//
//Swap::Swap(const Swap &swap) {
//	startDate_ = swap.startDate_;
//	endDate_ = swap.endDate_;
//	Leg1_payementFrequency_ = swap.Leg1_payementFrequency_;
//	Leg2_payementFrequency_ = swap.Leg2_payementFrequency_;
//	Leg1_index_ = new InstrumentIndex(*(swap.Leg1_index_));
//	Leg2_index_ = new InstrumentIndex(*(swap.Leg2_index_));
//}
//
//Swap::~Swap() {
//	delete [] Leg1_index_;
//	delete [] Leg2_index_;
//}
//
//double Swap::pvFloating(double T) {
//	// Compute discount factors at double T
//	// Use libor index class and sum it all
//	return 0.;
//}
//
//double Swap::pvFixing(double T) {
//	// Compute discount factors at double T
//	return 0.;
//}