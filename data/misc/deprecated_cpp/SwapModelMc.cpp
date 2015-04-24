//#include "SwapModelMc.h"
//
//
//SwapModelMc::SwapModelMc(): tenor(0), floatPer(0), fixPer(0){}
//
//SwapModelMc::SwapModelMc(size_t tenor, size_t beginDateIndex, size_t endDateIndex, size_t floatPer, size_t fixPer): tenor(tenor), floatPer(floatPer), fixPer(fixPer) {
//
//	/*this->beginDate = beginDate;
//	this->endDate = endDate;
//	this->beginDateIndex = (size_t)(this->beginDate/this->floatPer);
//	this->endDateIndex = (size_t)(this->endDate/this->floatPer);*/
//
//	this->beginDateIndex = beginDateIndex;
//	this->endDateIndex = endDateIndex;
//
//	/*size_t fixSize = tenor * 12/fixPer + 1;
//	size_t floatSize = tenor * 12/floatPer + 1;*/
//
//	size_t diff = fixPer/floatPer;
//	size_t floatSize = this->endDateIndex - this->beginDateIndex +1;
//
//	size_t fixSize = floatSize / diff;
//	
//	this->fixDates = numeric::ublas::vector<Time>(fixSize);
//	this->floatDates = numeric::ublas::vector<Time>(floatSize);
//
//	std::cout << " FLOAT DATES " << std::endl;
//	for (size_t i = 0; i < floatSize; ++i) {
//		this->floatDates(i) = (i+beginDateIndex) * floatPer/12.;
//		std::cout << this->floatDates(i) << std::endl;
//	}
//
//	std::cout << std::endl;
//
//	for (size_t i = 0; i < fixSize; ++i) 
//		this->fixDates(i) = this->floatDates(i*diff);
//}
//
//SwapModelMc::SwapModelMc(const SwapModelMc &swapStruct) {
//
//	size_t fixSize = tenor * 12/fixPer + 1;
//	size_t floatSize = (fixSize-1)*(fixPer/floatPer) + 1;
//	size_t diff = fixPer/floatPer;
//
//	this->floatPer = swapStruct.floatPer;
//	this->fixPer = swapStruct.fixPer;
//	this->fixDates = numeric::ublas::vector<Time>(fixSize);
//	this->floatDates = numeric::ublas::vector<Time>(floatSize);
//
//	for (size_t i = 0; i < floatSize; ++i)
//		this->floatDates(i) = swapStruct.floatDates(i);
//
//	for (size_t i = 0; i < fixSize; ++i) 
//		this->fixDates(i) = this->floatDates(i*diff);
//}
//
//size_t SwapModelMc::getTenor() { return this->tenor; }
//size_t SwapModelMc::getFloatPer() { return this->floatPer; }
//size_t SwapModelMc::getFixPer() { return this->fixPer; }
//const numeric::ublas::vector<Time> &SwapModelMc::getFloatDates() { return this->floatDates; }
//const numeric::ublas::vector<Time> &SwapModelMc::getFixDates() { return this->fixDates; }