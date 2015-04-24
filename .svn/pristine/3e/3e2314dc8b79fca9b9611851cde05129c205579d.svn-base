
#include <iostream>

using namespace QuantLib;

void test_DateConvention()
{
	QuantLib::DayCounter myCounter = QuantLib::Thirty360();

	Date dateStart = Date(02,Jan,2014);
	Date dateEnd   = Date(01,Aug,2015);

	Time yearFractionValue = myCounter.yearFraction(dateStart, dateEnd);

	std::cout << "yearFractionValue = " << yearFractionValue << std::endl;
}