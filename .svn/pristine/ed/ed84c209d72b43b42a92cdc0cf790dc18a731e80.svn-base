#include <LMM/helper/TenorType.h>

#include <cassert>

#include <boost/lexical_cast.hpp>

#define TenorType_INITIALIZER( tenortypeID , nbMonth ,tenorNameStr )   \
	template<> const std::string  Tenor::TenorType<tenortypeID>::NAME      = std::string(tenorNameStr) ;  \
	template<> const unsigned int Tenor::TenorType<tenortypeID>::NB_MONTH  = nbMonth                   ;  \
	template<> const double       Tenor::TenorType<tenortypeID>::YEAR_FRAC = nbMonth/12.               ;  \


// map diervs properties of TenorType
TenorType_INITIALIZER(0,  0 , "Non")
TenorType_INITIALIZER(1,  1 ,  "1M")
TenorType_INITIALIZER(2,  3 ,  "3M")
TenorType_INITIALIZER(3,  6 ,  "6M")
TenorType_INITIALIZER(4,  9 ,  "9M")
TenorType_INITIALIZER(5, 12 , "12M")
TenorType_INITIALIZER(6, 12 , "1YR")




const Tenor::TYPE_0 Tenor::_Non ;// initialization static variable
const Tenor::TYPE_1 Tenor::_1M  ;
const Tenor::TYPE_2 Tenor::_3M  ;
const Tenor::TYPE_3 Tenor::_6M  ;
const Tenor::TYPE_4 Tenor::_9M  ;
const Tenor::TYPE_5 Tenor::_12M ;
const Tenor::TYPE_6 Tenor::_1YR ;


Tenor & Tenor::operator=(const Tenor & other)
{
	this->name     = other.name     ;
	this->nbMonth  = other.nbMonth  ;
	this->yearFrac = other.yearFrac ;
	return *this;
}



bool Tenor::operator==(const Tenor& other) const { return (this->NbOfMonth() == other.NbOfMonth() ); }

bool Tenor::operator!=(const Tenor& other) const { return (this->NbOfMonth() != other.NbOfMonth() ); }

//YY TODO: change this shit code... 
bool                 Tenor::isValidTenor() const { return this->NbOfMonth() > 0;    }
const double &       Tenor::YearFraction() const { return *yearFrac ; }
const unsigned int & Tenor::NbOfMonth(   ) const { return *nbMonth  ; }
const std::string  & Tenor::Name(        ) const { return *name     ; }

//ctntodo implement operator/ that do the same thing
size_t Tenor::ratioTo(const Tenor& other) const
{
	assert( this->isValidTenor() ) ;
	assert( other.isValidTenor() ) ;

	unsigned int tenor1 = this->NbOfMonth() ;
	unsigned int tenor2 = other.NbOfMonth() ;
		
	assert( tenor1%tenor2==0 )  ; // to compute the ratio only when tenor1 is divisible by tenor2
	return ( tenor1/tenor2 );
}


std::ostream& operator<<(std::ostream& os, const Tenor& tenor)
{
	os<< std::string("Tenor")
	  << tenor.Name()
	  << std::string("[")
	  << boost::lexical_cast<std::string>( tenor.NbOfMonth() )
	  << std::string("MO|")
	  << boost::lexical_cast<std::string>( tenor.YearFraction() )
	  << std::string("YR]") ;
	return os;
}
