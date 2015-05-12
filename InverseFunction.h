#pragma once

#include <vector>
#include <boost/function.hpp>
#include <iostream>


class InverseFunction
{
private :
	std::vector<double> tableauX_ ;
	std::vector<double> tableauY_ ;
	//boost::function<double(double)> func_;

public:
	//le constructeur crée les images du tableauX dans tableauY
	//InverseFunction(boost::function<double(double)> func, std::vector<double> tableauX);  
	InverseFunction(std::vector<double> tableauX);  

	~InverseFunction(void);

	double fMoinsUn(double y) ;
};

