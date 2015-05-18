#include "courbeInput.h"


courbeInput::courbeInput(void)
{
	listeMatu_.clear() ; tauxZC_.clear() ;
	listeMatu_.push_back(0) ; listeMatu_.push_back(1) ; listeMatu_.push_back(2) ; listeMatu_.push_back(3) ; 
	listeMatu_.push_back(4) ; listeMatu_.push_back(5) ; listeMatu_.push_back(10) ; listeMatu_.push_back(15) ; 
	listeMatu_.push_back(20) ; listeMatu_.push_back(25) ;
	tauxZC_.push_back(0.8/100) ; tauxZC_.push_back(0.85/100) ; tauxZC_.push_back(0.9/100) ; tauxZC_.push_back(0.92/100) ; 
	tauxZC_.push_back(0.95/100) ; tauxZC_.push_back(1.00/100) ; tauxZC_.push_back(1.5/100) ; tauxZC_.push_back(2/100) ; 
	tauxZC_.push_back(2.5/100) ; tauxZC_.push_back(2.3/100) ; 
}

courbeInput::courbeInput(std::vector<double> listeMatu, std::vector<double> tauxZC)
	: listeMatu_(listeMatu), tauxZC_(tauxZC)
{
	assert(listeMatu_.size() == tauxZC_.size()) ;
}


courbeInput::~courbeInput(void)
{
}

double courbeInput::get_ZC_maturite_T(double T)
{
	int N = listeMatu_.size() ;
	assert (N > 0) ;
	assert (0 <= T && T <= listeMatu_[N-1]) ;
	int i=0 ;
	while (T > listeMatu_[i+1] && i < N-2){++i ;}
	
	return ( tauxZC_[i] + (tauxZC_[i+1] - tauxZC_[i])/(listeMatu_[i+1] - listeMatu_[i])*(T - listeMatu_[i]) ) ;
}

void courbeInput::showCourbeInput()
{
	int N = listeMatu_.size() ;
	std::cout << "listeMatu   |   tauxZC" << std::endl ;
	for (int i = 0 ; i < N ; ++i)
	{
		std::cout << listeMatu_[i] << "  |  " << tauxZC_[i] << std::endl ; 
	}
}
