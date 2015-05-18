#include "InverseFunction.h"


//InverseFunction::InverseFunction(boost::function<double(double)> func, std::vector<double> tableauX)
InverseFunction::InverseFunction(std::vector<double> tableauX)
{
	tableauX_ = tableauX ;
	double x, f_x ;
	tableauY_.clear() ;
	for (int i = 0 ; i < tableauX.size() ; ++i)
	{
		x = tableauX_[i] ;
		f_x = 3 * x * x + 8 * x ; 
		tableauY_.push_back(f_x) ; 
		std::cout << f_x << std::endl ;
	}
}


InverseFunction::~InverseFunction(void){}

double InverseFunction::fMoinsUn(double y)
{
	int N = tableauY_.size() ;
	assert (N > 0) ;
	assert (tableauY_[0] <= y && y <= tableauY_[N-1]) ;
	int i=0 ;
	while (y > tableauY_[i+1] && i < N-2){++i ;}
	
	return ( tableauX_[i] + (y - tableauY_[i])/(tableauY_[i+1] - tableauY_[i])*(tableauX_[i+1] - tableauX_[i]) ) ;
}