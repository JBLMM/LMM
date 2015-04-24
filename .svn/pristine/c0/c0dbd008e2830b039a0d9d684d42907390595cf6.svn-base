#ifndef LMM_BUILDVARIABLE_H
#define LMM_BUILDVARIABLE_H
#pragma once

/*! Define common variables for build configuration
 *  Doing like this avoid rebuild the whole solution when these variable change
 */
 
namespace LMM
{
	//! set this variable to true when one want to print out all 
	//! LMM_WARN, LMM_NOTIF
	//! set this variable to false when turning a very longtime simulation, risk of printout a lot of msg
	bool  NOTIFLMM() ;
	bool  DEUBGLMM() ;
	bool  WARNLMM () ;
	
	static const char* TODO_MSG  = "LMM Todo : "         ;
	static const char* NOTIF_MSG = "	LMM Notification : " ;
	static const char* WARN_MSG  = "LMM Warning : "      ;
	static const char* ERROR_MSG = "LMM ERROR : "        ;

	static const double EPSILON = 1e-8                   ;
}

#endif /* LMM_BUILDVARIABLE_H */