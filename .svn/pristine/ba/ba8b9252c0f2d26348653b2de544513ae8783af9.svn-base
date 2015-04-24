#include <LMM/helper/TypeConverter.h>

boost::numeric::ublas::matrix<double> QLMatrix2BoostMatrix(const QuantLib::Matrix& m)
{
	boost::numeric::ublas::matrix<double> m2(m.rows(), m.columns());
	for(size_t i=0; i<m2.size1(); ++i)
	{
		for(size_t j=0; j<m2.size2(); ++j)
		{
			m2(i,j) = m[i][j];
		}
	}
	return m2;
}