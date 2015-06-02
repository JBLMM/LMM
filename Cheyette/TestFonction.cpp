#include "TestFonction.h"
#include <iostream>


void TestFonctionConstanteMorceaux(){
	std::cout << "TestFonction()" << std::endl ;
	constant_RR_Function f = constant_RR_Function(3);
	std::cout << f.evaluate(0) << std::endl ;

	std::vector<double> x ; x.push_back(0) ; x.push_back(1) ; x.push_back(2) ;
	std::vector<double> y ; y.push_back(3) ; y.push_back(4) ;
			
	piecewiseconst_RR_Function f_CM = piecewiseconst_RR_Function(x, y) ;
	double res1 = f_CM.evaluate(1) ;
	std::cout << "res1 " << res1 << std::endl ;
	std::cout << f_CM.evaluate(1.5) << std::endl ;
}

void TestFonctionConstanteMorceaux_R2R(){
	//vecteur des abscisses
	std::vector<double> t ; t.push_back(0) ; t.push_back(1) ; t.push_back(2) ;

	//vecteur des fonctions constantes par morceaux
	piecewiseconst_RR_Function f1 ;
	std::vector<double> x ; x.push_back(0) ; x.push_back(0.5) ; x.push_back(1) ;
	std::vector<double> y ; y.push_back(3) ; y.push_back(4) ;
	piecewiseconst_RR_Function f2 = piecewiseconst_RR_Function(x, y) ;

	std::vector<piecewiseconst_RR_Function> f ; f.push_back(f1) ; f.push_back(f2) ;
	
	piecewiseconst_R2R_Function f_R2R = piecewiseconst_R2R_Function(t, f) ;
	double res1, res2, res3, res4, res5, res6 ;
	res1 = f_R2R.evaluate(0.1, 0.5) ; //1
	res2 = f_R2R.evaluate(1.1, 0.2) ; //3
	res3 = f_R2R.evaluate(1.1, 0.8) ; //4
	res4 = f_R2R.evaluate(0, 0.5) ; //1
	res5 = f_R2R.evaluate(1, 0.2) ; //3
	res6 = f_R2R.evaluate(2, 0.8) ; //4

	std::cout << "res1 " << res1 << ", res2 : " << res2 << ", res3 : " << res3 << std::endl ;
	std::cout << "res4 " << res4 << ", res5 : " << res5 << ", res6 : " << res6 << std::endl ;
}



void TestInverse(){

	std::vector<double> tableauX ; 
	for (int i = 0 ; i < 11 ; ++i)
	{
		tableauX.push_back(i) ;
	}
	InverseFunction i = InverseFunction(tableauX) ;
	std::cout << "i0   " << i.fMoinsUn(0) << std::endl ;
	std::cout << "i115   " << i.fMoinsUn(115) << std::endl ;
	std::cout << "i200   " << i.fMoinsUn(200) << std::endl ;
	std::cout << "i380  " << i.fMoinsUn(380) << std::endl ;

}