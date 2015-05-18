#include <Cheyette/courbeInput.h>

CourbeInput::CourbeInput(void){}


CourbeInput::CourbeInput(std::vector<double> listeMatu, std::vector<double> tauxZC)
	: listeMatu_(listeMatu), tauxZC_(tauxZC)
{
	assert(listeMatu_.size() == tauxZC_.size()) ;
}


CourbeInput::~CourbeInput(void)
{
}

double CourbeInput::get_ZC0(double T)
{
	int N = listeMatu_.size() ;
	assert (N > 0) ;
	assert (0 <= T && T <= listeMatu_[N-1]) ;
	int i=0 ;
	while (T > listeMatu_[i+1] && i < N-2){++i ;}
	
	return ( tauxZC_[i] + (tauxZC_[i+1] - tauxZC_[i])/(listeMatu_[i+1] - listeMatu_[i])*(T - listeMatu_[i]) ) ;
}

void CourbeInput::showCourbeInput()
{
	int N = listeMatu_.size() ;
	std::cout << "listeMatu   |   tauxZC" << std::endl ;
	for (int i = 0 ; i < N ; ++i)
	{
		std::cout << listeMatu_[i] << "  |  " << tauxZC_[i] << std::endl ; 
	}
}
