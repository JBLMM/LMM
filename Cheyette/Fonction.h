#pragma once

#include <vector>
#include <iostream>

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include <LMM\numeric\NumericalMethods.h>

/***********************************************************
******* fonctions de R dans R 
***********************************************************/

class RR_Function  // fonction de R dans R
{
public:
	virtual double operator ()(double x) const = 0;
};
typedef boost::shared_ptr<RR_Function>       RR_Function_PTR;
typedef boost::shared_ptr<const RR_Function> RR_Function_CONSTPTR;

class Boost_RR_Function : public  RR_Function 
{
private:
	boost::function<double(double)> func_;
public:
	Boost_RR_Function(const boost::function<double(double)>& func):func_(func){}
	virtual ~Boost_RR_Function(){}
	
	double operator ()(double x) const {return func_(x);}
};
typedef boost::shared_ptr<Boost_RR_Function>       Boost_RR_Function_PTR;
typedef boost::shared_ptr<const Boost_RR_Function> Boost_RR_Function_CONSTPTR;


class RR_Function_Composed : public RR_Function
{
protected:
	RR_Function_CONSTPTR func1_;
	RR_Function_CONSTPTR func2_;
public:
	//convertisseur de boost::function<double(double)> en RR_Function_CONSTPTR
	RR_Function_CONSTPTR conv(const boost::function<double(double)>& func1) const
	{
		RR_Function_CONSTPTR f(new Boost_RR_Function(func1));
		return f ;
	}
	RR_Function_Composed(const boost::function<double(double)>& func1, const boost::function<double(double)>& func2)
		:func1_(conv(func1)), func2_(conv(func2)){}
	RR_Function_Composed(RR_Function_CONSTPTR func1, RR_Function_CONSTPTR func2)
		:func1_(func1), func2_(func2){}
};
typedef boost::shared_ptr<RR_Function_Composed>       RR_Function_Composed_PTR;
typedef boost::shared_ptr<const RR_Function_Composed> RR_Function_Composed_CONSTPTR;


class RR_Function_ComposeByProduct : public RR_Function_Composed
{
public:
	RR_Function_ComposeByProduct(const boost::function<double(double)>& func1, const boost::function<double(double)>& func2)
		:RR_Function_Composed(func1, func2){}
	RR_Function_ComposeByProduct(RR_Function_CONSTPTR func1, RR_Function_CONSTPTR func2)
		:RR_Function_Composed(func1, func2){}

	double operator ()(double x) const {return func1_->operator()(x) * func2_->operator()(x) ; } 

};
typedef boost::shared_ptr<RR_Function_ComposeByProduct>       RR_Function_ComposeByProduct_PTR;
typedef boost::shared_ptr<const RR_Function_ComposeByProduct> RR_Function_ComposeByProduct_CONSTPTR;


//interpolation lineaire
class Interpolation_RR_Function : public  RR_Function 
{
private:
	 std::vector<double> grid_ ;   //plus de reference, cf le constructeur
	 std::vector<double> value_ ;
     //Interpolator interpolator_;  //pas de classe Interpolator
public:
	Interpolation_RR_Function()
	{
		std::vector<double> grid, value ;
		grid.push_back(0) ; value.push_back(0) ; 
		grid_= grid ; value_ = value ;
	}
	Interpolation_RR_Function(std::vector<double>& grid, std::vector<double>& value)
		:grid_(grid), value_(value) //:interpolator_(){}
		{
			try{
				if (grid_.size() != value_.size())
				{
					std::cout << "grid_.size() : " << grid_.size() << " , value_.size() : " << value_.size() << std::endl ;
					throw std::string("Exception Interpolation RR Function : vecteurs grid et value doivent avoir meme longueur");
				}
			}
			catch(std::string const& chaine) {std::cerr << chaine << std::endl;}
		}
	Interpolation_RR_Function(Interpolation_RR_Function const& f_toCopy)
		:grid_(f_toCopy.grid_), value_(f_toCopy.value_){}

	virtual ~Interpolation_RR_Function(){}

	std::vector<double> get_grid_() const {return grid_ ;}    //plus de referecne pb de constructeur par defaut sinon
	std::vector<double> get_value_() const {return value_ ;}

	void set_grid_(std::vector<double> grid) {grid_ = grid ;}
	void set_value_(std::vector<double> value) {value_ = value ;}

	double operator ()(double x) const {return  NumericalMethods::linearInterpolation2(x, grid_, value_);}

	Interpolation_RR_Function& operator=(Interpolation_RR_Function const& f2)
	{
		grid_ = f2.get_grid_() ;
		value_ = f2.get_value_() ;
		return *this ;
	}
};

typedef boost::shared_ptr<Interpolation_RR_Function>       Interpolation_RR_Function_PTR;
typedef boost::shared_ptr<const Interpolation_RR_Function> Interpolation_RR_Function_CONSTPTR;

//fonction constante
class constant_RR_Function : public  RR_Function // R->R
{
private :
	double constValue_ ;
public:
	constant_RR_Function(double constValue) : constValue_(constValue) {std::cout << "creation d'une fonction constante" << std::endl ;}
	boost::function<double(double)> func_;

	virtual ~constant_RR_Function(){};

	double operator ()(double x) const {return constValue_;}
};

//fonction constante par morceaux avec convention cadlag
class piecewiseconst_RR_Function : public RR_Function 
{
private :
	//si x est de longueur N, y doit être de longueur N-1
	std::vector<double> x_; //abscisses 
	std::vector<double> y_; //ordonnees

	//x0 = xmin ----------  x1 --------- x2 --------- x3 = xmax 
	//    [         y1       [     y2     [       y3    ]

public :
	//constructeur
	piecewiseconst_RR_Function(const std::vector<double>& x, const std::vector<double>& y) : x_(x), y_(y) {} 

	//getters
	std::vector<double> piecewiseconst_RR_Function::getx_() const {return x_ ;} 
	std::vector<double> piecewiseconst_RR_Function::gety_() const {return y_ ;}

	// find interval and return the corresponding y : convention càdlàg
	// traiter cas extreme: extrapolation ! 
	double piecewiseconst_RR_Function::evaluate(double t) const
	{
		//assert(x_[0] <= t || t <= x_[x_.size() - 1] ) ;
		//assert(y_.size() +  1 == x_.size()) ; 
	    try
		{
			bool OK(true) ;
			if(t < x_[0] || t > x_[x_.size() - 1] ){
				OK = false ;
				throw std::string("Exception : Evaluation de la fonction cste par morceaux hors du domaine");
			}
			if (y_.size() +  1 != x_.size())
			{
				OK = false ;
				throw std::string("Exception Fonction Constante par morceaux : vecteurs de taille non conforme");
			}
			if (OK){
				double res ; 
				if (t == x_[x_.size() - 1]){
					res = y_[y_.size() - 1] ;
				}else{
					size_t i = 0 ;
					bool boolean ; 
					boolean = (x_[i] <= t && t < x_[i+1]) ;
					while (!boolean && i < (x_.size() - 2)){
						++i ;
						boolean = (x_[i] < t && t < x_[i+1]) ;
					}
					res = y_[i] ;
				}
				return (res) ;		
			}
		}
		catch(std::string const& chaine) {std::cerr << chaine << std::endl;}

	}

	double operator ()(double x) const {return this->evaluate(x) ;}

	void show()
	{
		std::cout << "       for x in [" << x_[0] << ", " << x_[1] << "], y = " << y_[0] << std::endl ;
		for (size_t i = 1 ; i < x_.size() - 2 ; ++i)
		{
			std::cout << "       for x in [" << x_[i] << ", " << x_[i+1] << "[, y = " << y_[i] << std::endl ;
		}
		std::cout << "       for x in [" << x_[x_.size()-2] << ", " << x_[x_.size()-1] << "], y = " << y_[y_.size()-1] << std::endl ;
	}
};

/***********************************************************
******* fonctions de R^2 dans R 
***********************************************************/

class R2R_Function 
{
public:
	boost::function<double(double, double)> func_;

	virtual double evaluate(double x, double y) const = 0;
};

//
//class piecewiseconst_r2r_function : public r2r_function 
//{
//private :  //fonction de x à t fixé
//	//idem convention cadlag et longueur t_ = longueur f_ + 1
//	std::vector<double> t_;
//	std::vector<piecewiseconst_rr_function> f_; //fonction de x
//
//	//t0 ----------  t1 ----------- t2 ---------- t3 = t 
//	// [     f1(x)    [     f2(x)    [      f3(x)    ]
//
//public :
//	piecewiseconst_r2r_function(){}
//
//	piecewiseconst_r2r_function(std::vector<double> t, std::vector<piecewiseconst_rr_function> f) : t_(t), f_(f) {} 
//
//	// find interval and return the corresponding y : convention càdlàg
//	// traiter cas extreme: extrapolation ! 
//	double piecewiseconst_r2r_function::evaluate(double t, double x){
//		assert(t_[0] <= t || t <= t_[t_.size() - 1] ) ;
//		assert(f_.size() +  1 == t_.size()) ; 
//		double res ;
//		piecewiseconst_rr_function f_cm ;
//		if (t == t_[t_.size() - 1]){
//			f_cm = f_[f_.size() - 1] ;
//		}else{
//			size_t i = 0 ;
//			bool boolean ; 
//			boolean = (t_[i] <= t && t < t_[i+1]) ;
//			while (!boolean && i < (t_.size() - 2)){
//				++i ;
//				boolean = (t_[i] < t && t < t_[i+1]) ;
//			}
//			f_cm = f_[i] ;
//		}
//		res = f_cm.evaluate(x) ;
//		return (res) ;
//	}
//};
//
//
//class piecewiseconst_dd_rr_function : public piecewiseconst_rr_function
//{
//private :
//	piecewiseconst_rr_function m_;		//m(t) (skew)
//	piecewiseconst_rr_function sigma_;	//sigma(t) (niveau de la vol)
//	double l_ ;							//constante
//
//public :  //à t fixé, c'est une fonction de x
//	piecewiseconst_dd_rr_function(){}
//	piecewiseconst_dd_rr_function(piecewiseconst_rr_function m, piecewiseconst_rr_function sigma, double l) 
//		: m_(m), sigma_(sigma), l_(l)
//	{
//
//		// check pillar m & sigma is the same! :
//		assert(m.getx_() == m.gety_()) ;
//	} 
//
//	double piecewiseconst_dd_rr_function::sigma_r(double t, double x)
//	{
//		std::cout << "warning : fonction sigma r ne prend pas en compte le terme f(0,t)" << std::endl ;
//		return (m_.evaluate(t) * x + (1-m_.evaluate(t))*l_) * sigma_.evaluate(t);
//		//à améliorer avec l'opérateur ()
//	}
//
//	double piecewiseconst_dd_rr_function::d_sigma_r_dx(double t) //, double x)
//	{
//		return (m_.evaluate(t) *sigma_.evaluate(t));
//	}
//
//	piecewiseconst_rr_function get_m(){return m_ ;}
//	piecewiseconst_rr_function get_sigma(){return sigma_ ;}
//	double getl(){return l_ ;}
//
//};
