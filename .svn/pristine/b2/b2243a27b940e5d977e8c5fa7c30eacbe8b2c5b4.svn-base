#pragma once


#include <string>


/*! \class Tenor
*  This class model the Tenor Type 1M,2M ... 12M
*  Its design is an advanced of and enumeration, who can have instance and method
* User can do
* Tenor m_tenor=Tenor::3M;
* and use all method of Tenor 
*/
class Tenor
{
public:

	/*! To define a new TenorType, user has to define
	* - TYPEID   of tenor type
	* - NAME     of tenor type
	* - NB_MONTH number of month of tenor type
	* by specializing this template
	*/
	template<unsigned int TYPEID> class TenorType 
	{

	public :
		TenorType(){};
		static const std::string  NAME      ;
		static const unsigned int NB_MONTH  ;
		static const double       YEAR_FRAC ;
	};

	static const TenorType<0>  _Non;typedef TenorType<0> TYPE_0;
	static const TenorType<1>   _1M;typedef TenorType<1> TYPE_1;
	static const TenorType<2>   _3M;typedef TenorType<2> TYPE_2;
	static const TenorType<3>   _6M;typedef TenorType<3> TYPE_3;
	static const TenorType<4>   _9M;typedef TenorType<4> TYPE_4;
	static const TenorType<5>  _12M;typedef TenorType<5> TYPE_5;
	static const TenorType<6>  _1YR;typedef TenorType<6> TYPE_6;


	//! Constructor build a tenor by giving its type
	template<unsigned int NBMONTH>
	Tenor(const TenorType<NBMONTH>&):name(&TenorType<NBMONTH>::NAME),nbMonth(&TenorType<NBMONTH>::NB_MONTH), yearFrac(&TenorType<NBMONTH>::YEAR_FRAC){}

	//! assignment operator
	Tenor & operator=(const Tenor & other);	

	//! check if two tenor is equal or not
	bool operator==(const Tenor& other) const;
	bool operator!=(const Tenor& other) const;

	bool isValidTenor()     const ;

	//! get the year fraction of the Tenor
	const double       & YearFraction() const ;

	//! get the year fraction of the Tenor
	const unsigned int & NbOfMonth(   ) const ;

	//! get the name of Tenor
	const std::string  & Name(        ) const ;

	// compute the euclidean quotient of this Tenor to other Tenor 
	size_t ratioTo(const Tenor& other) const;

private :
	const std::string  * name     ;
	const unsigned int * nbMonth  ;
	const double       * yearFrac ;
};

std::ostream& operator<<(std::ostream& os, const Tenor& tenor);
