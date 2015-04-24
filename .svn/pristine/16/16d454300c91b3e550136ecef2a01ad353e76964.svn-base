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

## List all sources files for ${LUNA_RNGENERATOR} package
set(SRC_${LUNA_RNGENERATOR} ${SRC_${LUNA_RNGENERATOR}}
	"${CMAKE_SOURCE_DIR}/LMM/RNGenerator/McGenerator.h"
	"${CMAKE_SOURCE_DIR}/LMM/RNGenerator/QMcGenerator.h"
	"${CMAKE_SOURCE_DIR}/LMM/RNGenerator/RNGenerator.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/McGenerator.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/QMcGenerator.cpp"	
)

## Catch local variable for the global scope
set(SRC_${LUNA_RNGENERATOR} ${SRC_${LUNA_RNGENERATOR}} PARENT_SCOPE)

if(NOT LUNA_BUILD_FOR_YUANLI) 
	add_library(${LUNA_RNGENERATOR} ${SRC_${LUNA_RNGENERATOR}})
endif()