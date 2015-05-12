#pragma once

#include <vector>
#include "assert.h"
#include <boost/shared_ptr.hpp>
#include <iostream>

//c'est la courbe spot des taux ZC

//Singleton...
class courbeInput
{
private:
	std::vector<double> listeMatu_ ;
	std::vector<double> tauxZC_ ;

public:
	courbeInput(void);
	courbeInput(std::vector<double> listeMatu, std::vector<double> tauxZC);
	~courbeInput(void);

	double get_ZC_maturite_T(double T) ;
	void showCourbeInput() ;
};

typedef boost::shared_ptr<courbeInput> courbeInput_PTR;
typedef boost::shared_ptr<const courbeInput> courbeInput_CONSTPTR;