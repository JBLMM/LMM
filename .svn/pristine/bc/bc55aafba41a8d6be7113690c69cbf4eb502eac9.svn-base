#define BOOST_TEST_MODULE LMM_unit_test_model

#include <string>

#include <boost/test/included/unit_test.hpp>

#include <LMM/ModelMCLMM/HGVolatilityFunction.h>


/*! an example of creating a HGVolatilityParam
 */
class EmptyHGVolatilityParam : public HGVolatilityParam
{
public:

	EmptyHGVolatilityParam(LMMTenorStructure_PTR _pLMMTenorStructure):HGVolatilityParam(_pLMMTenorStructure){}

	virtual ~EmptyHGVolatilityParam(){}

	virtual void reset_FromArray(const QuantLib::Array& x) {}

	virtual void write_to_stream(std::ostream & os) const  {os<<" I am an empty Volatility Param"<<std::endl;}

	virtual double h(size_t indexLibor, size_t indexTime) const{ return 0 ; }
	virtual double g(size_t indexLibor, size_t indexTime) const{ return 0 ; }
};


struct ABCDFixture // use for test
{
	ABCDFixture() //initialize test fixture
	: a( -0.06 )
	, b( 0.17  )
	, c( 0.54  )
	, d( 0.17  )
	, tenor(Tenor::_3M)
	, maxTime( 2 )
	, pLMMTenorStructure(new LMMTenorStructure(tenor, maxTime))
	, emptyVolParam(pLMMTenorStructure)
	{}

	double a ;
	double b ;
	double c ;
	double d ;

	Tenor tenor;
	double maxTime;
	LMMTenorStructure_PTR pLMMTenorStructure;
	EmptyHGVolatilityParam emptyVolParam    ;
};



BOOST_FIXTURE_TEST_CASE(test_EmptyVolatility_constructor , ABCDFixture)
{
	HGVolatilityFunction hgVolatilityFunction(emptyVolParam,pLMMTenorStructure);

	std::cout<<emptyVolParam<<std::endl;

	BOOST_CHECK(true);
}



BOOST_FIXTURE_TEST_CASE(test_ABCDVolatility_constructor , ABCDFixture)
{

	DefaultHGVolatilityParam abcdParams(a,b,c,d, pLMMTenorStructure);

	HGVolatilityFunction hgVolatilityFunction(abcdParams,pLMMTenorStructure);
		
	hgVolatilityFunction.print("unitTest_DefaultHGVolatilityParam.csv");

	DefaultHGVolatilityParam abcdParams1(0.,0.,0.,0., pLMMTenorStructure);

	hgVolatilityFunction.reset_VolatilityParam(abcdParams1);

	//toto check if the reset is taking in account
	BOOST_CHECK(true);
}



