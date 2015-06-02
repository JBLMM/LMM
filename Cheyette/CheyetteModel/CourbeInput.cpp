#include <Cheyette\CheyetteModel\CourbeInput.h>

const double epsilon = 1.e-6;  // bad implementation 

CourbeInput::CourbeInput(std::vector<double> listeMatu, std::vector<double> tauxZC)
	: listeMatu_(listeMatu), tauxZC_(tauxZC)
{
	assert(listeMatu_.size() == tauxZC_.size()) ;
}

double CourbeInput::get_tauxZC0(double T) const
{
	return NumericalMethods::linearInterpolation2(T, listeMatu_, tauxZC_) ;
}

double CourbeInput::get_f_0_t(double t) const
{
	double yieldT = get_tauxZC0(t)*t;

	double t_bump = t + epsilon;
	double yieldT_bump = get_tauxZC0(t_bump)*t_bump;

	return (yieldT_bump-yieldT)/epsilon;

}

void CourbeInput::showCourbeInput() const
{
	int N = listeMatu_.size() ;
	std::cout << "listeMatu   |   tauxZC" << std::endl ;
	for (int i = 0 ; i < N ; ++i)
	{
		std::cout << listeMatu_[i] << "  |  " << tauxZC_[i] << std::endl ; 
	}
}
