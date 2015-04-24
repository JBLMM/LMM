#pragma once


#include <string>


/*! \class CalibrationType
 *  This class model the CalibrationType 
 *
 *  - Global
 *  - Local
 *  - Cascade
 *
 */
class CalibrationType
{
public:

	static const CalibrationType GLOBAL  ;
	static const CalibrationType CASCADE ;
	static const CalibrationType LOCAL   ;

	//! assignment operator
	CalibrationType & operator=(const CalibrationType & other);	

	//! check if the same calibration type
	bool operator==(const CalibrationType& other) const;
	bool operator!=(const CalibrationType& other) const;
	
	bool isGlobal() const  { return type_ID_ == 0 ;}
	bool isLocal() const   { return type_ID_ == 1 ;}
	bool isCascade() const { return type_ID_ == 2 ;}

	//! get the name of CalibrationType
	const std::string  & Name(        ) const ;
	
	CalibrationType(const unsigned int ID, const char* name) : type_ID_(ID), name_(name){}

private :
	unsigned int type_ID_ ;
	std::string  name_    ;
};

std::ostream& operator<<(std::ostream& os, const CalibrationType& tenor);
