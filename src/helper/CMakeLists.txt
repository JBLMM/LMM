#################################################
#   chithanhnguyen.math@gmail.com               #
#                                               #
#   Lunalogic intership project 2014            #
#   LIBOR Market Model                          #
#   Technologies:                               #
#   - cmake                                     #
#   - boost                                     #
#   - QuantLib                                  #
#                                               #
#################################################

set(SRC_${LUNA_HELPER}
    "${CMAKE_SOURCE_DIR}/LMM/helper/BuildVariable.h"
	"${CMAKE_SOURCE_DIR}/LMM/helper/CombinationOperator.h"
    "${CMAKE_SOURCE_DIR}/LMM/helper/DerivatibleFunction.h"
	"${CMAKE_SOURCE_DIR}/LMM/helper/EqualOperator.h"
	"${CMAKE_SOURCE_DIR}/LMM/helper/GenericPath.h"
	"${CMAKE_SOURCE_DIR}/LMM/helper/LMMTenorStructure.h"
	"${CMAKE_SOURCE_DIR}/LMM/helper/Name.h"	
	"${CMAKE_SOURCE_DIR}/LMM/helper/Printer.h"
	"${CMAKE_SOURCE_DIR}/LMM/helper/StaticReplicationFormula.h"
	"${CMAKE_SOURCE_DIR}/LMM/helper/TenorType.h"
	"${CMAKE_SOURCE_DIR}/LMM/helper/Tensor.h"
	"${CMAKE_SOURCE_DIR}/LMM/helper/TypeConverter.h"	
	"${CMAKE_CURRENT_SOURCE_DIR}/CombinationOperator.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DerivatibleFunction.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/LMMTenorStructure.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Printer.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/StaticReplicationFormula.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/TenorType.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Tensor.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/TypeConverter.cpp"	
)

## Catch local variable for the global scope
set(SRC_${LUNA_HELPER} ${SRC_${LUNA_HELPER}} PARENT_SCOPE)

if(NOT LUNA_BUILD_FOR_YUANLI) 
	add_library(${LUNA_HELPER} ${SRC_${LUNA_HELPER}})
	target_link_libraries(${LUNA_HELPER} ${LUNA_NUMERIC})
endif()