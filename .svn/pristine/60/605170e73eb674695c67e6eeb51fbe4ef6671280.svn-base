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

set(SRC_${LUNA_NUMERIC}
    "${CMAKE_SOURCE_DIR}/LMM/numeric/BilinearInterpolation.h"
	"${CMAKE_SOURCE_DIR}/LMM/numeric/Interpolation.h"
    "${CMAKE_SOURCE_DIR}/LMM/numeric/NumericalMethods.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/BilinearInterpolation.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Interpolation.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/NumericalMethods.cpp"	
)

## Catch local variable for the global scope
set(SRC_${LUNA_NUMERIC} ${SRC_${LUNA_NUMERIC}} PARENT_SCOPE)

if(NOT LUNA_BUILD_FOR_YUANLI) 
	add_library(${LUNA_NUMERIC} ${SRC_${LUNA_NUMERIC}})
	target_link_libraries(${LUNA_NUMERIC} ${QuantLib_LIBRARY})
endif()