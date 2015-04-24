#include <LMM/helper/CalibrationType.h>

#include <cassert>

#include <boost/lexical_cast.hpp>


const CalibrationType CalibrationType::GLOBAL (0,"Global");
const CalibrationType CalibrationType::LOCAL  (1,"Local");
const CalibrationType CalibrationType::CASCADE(2,"Cascade");


CalibrationType & CalibrationType::operator=(const CalibrationType & other)
{
	this->type_ID_ = other.type_ID_;
	this->name_    = other.name_;
	return *this;
}

bool CalibrationType::operator==(const CalibrationType& other) const 
{
	return (this->type_ID_ == other.type_ID_ && this->name_ == other.name_);
}

bool CalibrationType::operator!=(const CalibrationType& other) const 
{
	return !(this->type_ID_ == other.type_ID_ && this->name_ == other.name_);
}

const std::string  & CalibrationType::Name() const { return name_ ; }

std::ostream& operator<<(std::ostream& os, const CalibrationType& tenor)
{
	os<< std::string("CalibrationType")
	  << tenor.Name();
	return os;
}
