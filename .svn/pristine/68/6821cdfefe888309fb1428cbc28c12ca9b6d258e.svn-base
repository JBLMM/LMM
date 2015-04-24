//#include <iostream>
//

//#include <LMM/instrument/Tarn.h>
//Tarn::Tarn()
//	: strike_(0.), target_(0.), indexStart_(0), indexEnd_(0) ,exoticLegTenor_(0), liborLegTenor_(0), exoLiborTenorRatio_(0)
//{}
//
//Tarn::Tarn(double strike, double target, size_t indexStart, size_t indexEnd, size_t liborLegTenor, size_t exoticLegTenor)
//	:strike_(strike), target_(target), indexStart_(indexStart), indexEnd_(indexEnd), exoticLegTenor_(exoticLegTenor), liborLegTenor_(liborLegTenor)
//{
//	if(liborLegTenor_%exoticLegTenor_ != 0)
//		throw("wrong config of liborLegTenor_ & exoticLegTenor_");
//	exoLiborTenorRatio_ = liborLegTenor_/exoticLegTenor_;
//
//	if( (liborLegTenor_-indexStart_) % exoLiborTenorRatio_ != 0 )
//		throw("wrong config of liborLegTenor_ & exoticLegTenor_");
//
//	size_t floatingPaymentSize =  (indexEnd_ - indexStart_);
//	size_t fixingPaymentSize   =  floatingPaymentSize/exoLiborTenorRatio_;
//
//	//floating schedule
//	for(size_t i=0; i<floatingPaymentSize; ++i)
//	{
//		exoticPaymentIndexSchedule.push_back(indexStart_+i+1);
//	}
//
//	for(size_t i=0; i<fixingPaymentSize; ++i)
//	{
//		liborPaymentIndexSchedule.push_back(indexStart_+(i+1)*exoLiborTenorRatio_);
//	}
//}
//
//void Tarn::print_details()
//{
//	std::cout << "indexStart_ = " << indexStart_ << std::endl;
//	std::cout << "indexEnd_ = " << indexEnd_ << std::endl;
//
//	std::cout << "exoticPaymentIndexSchedule: " << std::endl;
//	for(size_t i=0; i<exoticPaymentIndexSchedule.size(); ++i)
//	{
//		std::cout << exoticPaymentIndexSchedule[i] << std::endl;
//	}
//
//	std::cout << "liborPaymentIndexSchedule: " << std::endl;
//	for(size_t i=0; i<liborPaymentIndexSchedule.size(); ++i)
//	{
//		std::cout << liborPaymentIndexSchedule[i] << std::endl;
//	}
//}
//
////-- Getters/Setters
//double Tarn::get_strike(){return strike_;}
//double & Tarn::strike(){ return strike_  ; }
//double Tarn::get_target(){return target_;}
//void Tarn::set_target(double target){target_ = target;}
//double Tarn::get_indexStart(){return indexStart_;}
//double Tarn::get_indexEnd(){return indexEnd_;}
//double Tarn::get_liborLegTenor(){return liborLegTenor_;}
//double Tarn::get_exoticLegTenor(){return exoticLegTenor_;}
//const std::vector<size_t>& Tarn::getExoticPaymentIndices(){return exoticPaymentIndexSchedule;}
//const std::vector<size_t>& Tarn::getLiborPaymentIndices(){return liborPaymentIndexSchedule;}
//
//
//
//void Tarn::showTarnStructure()
//{
//	double strike = 0.115;
//	size_t exoLegTenor = 6;
//	size_t liborLegTenor = 12;
//	double target = 0.03;
//	size_t indexStart = 2;
//	size_t indexEnd = 5;
//
//	Tarn tarn(strike,target,indexStart,indexEnd,liborLegTenor,exoLegTenor); // exo and libor legs share the same schedule
//	tarn.print_details();
//}