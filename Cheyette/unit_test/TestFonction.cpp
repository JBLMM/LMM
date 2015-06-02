#include <Cheyette\unit_test\TestFonction.h>
#include <iostream>


void TestFonctionConstanteMorceaux(){
	std::cout << "TestFonction()" << std::endl ;
//fonction constante
	constant_RR_Function f = constant_RR_Function(3);
	std::cout << f(0) << std::endl ;
//fonction constante par morceaux
	std::vector<double> x ; x.push_back(0) ; x.push_back(1) ; x.push_back(2) ;
	std::vector<double> y ; y.push_back(3) ; y.push_back(4) ;
			
	piecewiseconst_RR_Function f_CM = piecewiseconst_RR_Function(x, y) ;
	std::cout << "f(0) = " << f_CM(0) << "vs : 3" << std::endl ;
	std::cout << "f(1) = " << f_CM(1) << "vs : 4" << std::endl ;
	std::cout << "f(2) = " << f_CM(2) << "vs : 4" << std::endl ;
//doit renvoyer message d'erreur : extrapolation
	std::cout << "non defini en -1 : f(-1) = " << f_CM(-1) << std::endl ;
	std::cout << "non defini en 4 : f(4) = " << f_CM(4) << std::endl ;
//doit renvoyer message d'erreur : vecteur pas de bonne taille    
	y.push_back(5) ;
	f_CM = piecewiseconst_RR_Function(x, y) ;
	std::cout << "pb de taille de vecteur : f(0) = " << f_CM(0) << std::endl ;
	std::cout << "pb de taille de vecteur : f(2) = " << f_CM(2) << std::endl ;
}

	double f123(double x){return x ;}
	double g123(double x){return x ;}

void TestCompositionMultiplicationFonction()
{

	boost::function<double(double)> f_boost = boost::bind(f123, _1);
	boost::function<double(double)> g_boost = boost::bind(g123, _1);
	RR_Function_ComposeByProduct f_mult(f_boost, g_boost) ;
	std::cout << "f_mult(1) = f(1)*g(1) = " << f_mult(1) << " vs 1" << std::endl ;
	std::cout << "f_mult(2) = f(2)*g(2) = " << f_mult(2) << " vs 4" << std::endl ;
}

void TestInterpolation_RR_Function()
{
	std::vector<double> grid ;
	std::vector<double> value ;

	grid.push_back(0) ; grid.push_back(1) ; grid.push_back(2) ;
	value.push_back(3) ; value.push_back(4) ; 
	Interpolation_RR_Function f_interp(grid, value) ;
	//pb de taille de vecteur
	std::cout << "f_interp(0) = " << f_interp(0) << " vs 3" << std::endl ; //OK
	//bonne taille 
	value.push_back(5) ;  //f_interp defini par reference 
	std::cout << "f_interp(0) = " << f_interp(0) << " vs 3" << std::endl ;
	std::cout << "f_interp(1) = " << f_interp(1) << " vs 4" << std::endl ;
	std::cout << "f_interp(2) = " << f_interp(2) << " vs 5" << std::endl ;
	std::cout << "f_interp(0.5) = " << f_interp(0.5) << " vs 3.5" << std::endl ;
	//extrapolation ?
	std::cout << "Extrapolation f_interp(6) = " << f_interp(6) << std::endl ;
}


//void TestFonctionConstanteMorceaux_R2R(){
//	//vecteur des abscisses
//	std::vector<double> t ; t.push_back(0) ; t.push_back(1) ; t.push_back(2) ;
//
//	//vecteur des fonctions constantes par morceaux
//	piecewiseconst_RR_Function f1 ;
//	std::vector<double> x ; x.push_back(0) ; x.push_back(0.5) ; x.push_back(1) ;
//	std::vector<double> y ; y.push_back(3) ; y.push_back(4) ;
//	piecewiseconst_RR_Function f2 = piecewiseconst_RR_Function(x, y) ;
//
//	std::vector<piecewiseconst_RR_Function> f ; f.push_back(f1) ; f.push_back(f2) ;
//	
//	piecewiseconst_R2R_Function f_R2R = piecewiseconst_R2R_Function(t, f) ;
//	double res1, res2, res3, res4, res5, res6 ;
//	res1 = f_R2R.evaluate(0.1, 0.5) ; //1
//	res2 = f_R2R.evaluate(1.1, 0.2) ; //3
//	res3 = f_R2R.evaluate(1.1, 0.8) ; //4
//	res4 = f_R2R.evaluate(0, 0.5) ; //1
//	res5 = f_R2R.evaluate(1, 0.2) ; //3
//	res6 = f_R2R.evaluate(2, 0.8) ; //4
//
//	std::cout << "res1 " << res1 << ", res2 : " << res2 << ", res3 : " << res3 << std::endl ;
//	std::cout << "res4 " << res4 << ", res5 : " << res5 << ", res6 : " << res6 << std::endl ;
//}



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