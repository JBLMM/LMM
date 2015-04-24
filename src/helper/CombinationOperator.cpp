//#include <LMM/helper/CombinationOperator.h>
//#include <iostream>
//
//DerivatibleFunction CombinationOperator::combine(const DerivatibleFunction& derivatibleFunc1, 
//												 const DerivatibleFunction& derivatibleFunc2)
//{
//	derivatibleFunc1_ = derivatibleFunc1;
//	derivatibleFunc2_ = derivatibleFunc2;
//
//	boost::function<double (double)> combinedFunction		  = boost::bind(&CombinationOperator::function,this,_1);
//	boost::function<double (double)> combinedFirstDerivative  = boost::bind(&CombinationOperator::firstDerivative,this,_1);
//	boost::function<double (double)> combinedSecondDerivative = boost::bind(&CombinationOperator::secondDerivative,this,_1);
//
//	DerivatibleFunction dd(combinedFunction, combinedFirstDerivative, combinedSecondDerivative);
//	std::cout << "test = " << dd.func_(3.14); //  << std::end;
//	return dd;
//}
//
//
//AddCombinationOperator::AddCombinationOperator()
//	:CombinationOperator("+")
//{}
//
//double AddCombinationOperator::function(double x) const
//{
//	return derivatibleFunc1_.func_(x) + derivatibleFunc2_.func_(x);
//}
//
//double AddCombinationOperator::firstDerivative(double x) const
//{
//	return derivatibleFunc1_.func_1stDerivative_(x) + derivatibleFunc2_.func_1stDerivative_(x);
//}
//
//double AddCombinationOperator::secondDerivative(double x) const 
//{
//	return derivatibleFunc1_.func_2ndDerivative_(x) + derivatibleFunc2_.func_2ndDerivative_(x);
//}
//
////
////
////
////SubstractCombinationOperator::SubstractCombinationOperator()
////	:CombinationOperator("-")
////{}
////
////double SubstractCombinationOperator::function(double x) const
////{
////	return derivatibleFunc1_.func_(x) - derivatibleFunc2_.func_(x);
////}
////
////double SubstractCombinationOperator::firstDerivative(double x) const
////{
////	return derivatibleFunc1_.func_1stDerivative_(x) - derivatibleFunc2_.func_1stDerivative_(x);
////}
////
////double SubstractCombinationOperator::secondDerivative(double x) const 
////{
////	return derivatibleFunc1_.func_2ndDerivative_(x) - derivatibleFunc2_.func_2ndDerivative_(x);
////}
////
////
////
////MultiplyCombinationOperator::MultiplyCombinationOperator()
////	:CombinationOperator("*")
////{}
////
////double MultiplyCombinationOperator::function(double x) const
////{
////	return derivatibleFunc1_.func_(x) * derivatibleFunc2_.func_(x);
////}
////
////double MultiplyCombinationOperator::firstDerivative(double x) const
////{
////	return derivatibleFunc1_.func_1stDerivative_(x) * derivatibleFunc2_.func_(x) 
////		+  derivatibleFunc1_.func_(x) * derivatibleFunc2_.func_1stDerivative_(x);
////}
////
////double MultiplyCombinationOperator::secondDerivative(double x) const 
////{
////	return derivatibleFunc1_.func_2ndDerivative_(x) * derivatibleFunc2_.func_(x)
////		+ 2 * derivatibleFunc1_.func_1stDerivative_(x) * derivatibleFunc2_.func_1stDerivative_(x)
////		+ derivatibleFunc1_.func_(x) * derivatibleFunc2_.func_2ndDerivative_(x);
////}
////
////
////
////
////DivideCombinationOperator::DivideCombinationOperator()
////	:CombinationOperator("/")
////{}
////
////double DivideCombinationOperator::function(double x) const
////{
////	return derivatibleFunc1_.func_(x) / derivatibleFunc2_.func_(x);
////}
////
////double DivideCombinationOperator::firstDerivative(double x) const
////{
////	double numerator = derivatibleFunc1_.func_1stDerivative_(x) * derivatibleFunc2_.func_(x) 
////		-  derivatibleFunc1_.func_(x) * derivatibleFunc2_.func_1stDerivative_(x);
////	double denominator = derivatibleFunc2_.func_(x)*derivatibleFunc2_.func_(x);  
////	return numerator/denominator;
////}
////
//////! (f/g)'  = (f'g-fg')/g^2 = F/G, where: F= f'g-fg', G = g^2
//////! (f/g)'' = (F'G-FG')/G^2, where F' = f''g - 2f'g' + fg'', G' = 2*g*g'
////double DivideCombinationOperator::secondDerivative(double x) const 
////{
////	double F       =    derivatibleFunc1_.func_1stDerivative_(x) * derivatibleFunc2_.func_(x) 
////		- derivatibleFunc1_.func_(x) * derivatibleFunc2_.func_1stDerivative_(x);
////
////	double F_prime =   derivatibleFunc1_.func_2ndDerivative_(x)*derivatibleFunc2_.func_(x) 
////		- 2 * derivatibleFunc1_.func_1stDerivative_(x)*derivatibleFunc2_.func_1stDerivative_(x) 
////		+ derivatibleFunc1_.func_(x)*derivatibleFunc2_.func_2ndDerivative_(x);
////
////	double G       =  derivatibleFunc2_.func_(x) * derivatibleFunc2_.func_(x);
////	double G_prime =  2 * derivatibleFunc2_.func_(x) * derivatibleFunc2_.func_2ndDerivative_(x);
////
////	double numerator   = F_prime*G - F*G_prime;
////	double denominator = G*G;
////	return numerator/denominator;
////}
