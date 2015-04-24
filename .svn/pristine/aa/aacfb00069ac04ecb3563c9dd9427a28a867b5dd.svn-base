#pragma once
#include <string>

#include <sstream>
#include <string>

#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include <ql/math/matrix.hpp>

template <typename T>
std::string Number2String (const T& Number )
{
	std::ostringstream ss;
	ss << Number;
	return ss.str();
}


//! very slow function: too much copy-coller.
boost::numeric::ublas::matrix<double> QLMatrix2BoostMatrix(const QuantLib::Matrix& m);

//template <typename T>
//T StringToNumber ( const string& Text )
//{
//	istringstream ss(Text);
//	T result;
//	ss >> result;
//	assert(result)
//	return result;
//}
