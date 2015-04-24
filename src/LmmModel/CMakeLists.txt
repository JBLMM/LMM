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

set(SRC_${LUNA_MODEL}
	"${CMAKE_SOURCE_DIR}/LMM/LmmModel/Correlation.h"
	"${CMAKE_SOURCE_DIR}/LMM/LmmModel/Dispersion.h"
	"${CMAKE_SOURCE_DIR}/LMM/LmmModel/HGVolatilityFunction.h"
	"${CMAKE_SOURCE_DIR}/LMM/LmmModel/Lmm.h"
	"${CMAKE_SOURCE_DIR}/LMM/LmmModel/McLmm.h"
	"${CMAKE_SOURCE_DIR}/LMM/LmmModel/McSpotLmm.h"
	"${CMAKE_SOURCE_DIR}/LMM/LmmModel/McTerminalLmm.h"
	"${CMAKE_SOURCE_DIR}/LMM/LmmModel/PCA.h"
	"${CMAKE_SOURCE_DIR}/LMM/LmmModel/RobonatoAngle.h"
	"${CMAKE_SOURCE_DIR}/LMM/LmmModel/VolatilityFunction.h"
	"${CMAKE_SOURCE_DIR}/LMM/LmmModel/VolatilityGridBuilder.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Correlation.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Dispersion.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/HGVolatilityFunction.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Lmm.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/McLmm.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/McSpotLmm.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/McTerminalLmm.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/PCA.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/RobonatoAngle.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/VolatilityGridBuilder.cpp"
)

## Catch local variable for the global scope
set(SRC_${LUNA_MODEL} ${SRC_${LUNA_MODEL}} PARENT_SCOPE)

if(NOT LUNA_BUILD_FOR_YUANLI) 
	add_library(${LUNA_MODEL} ${SRC_${LUNA_MODEL}})
	target_link_libraries(${LUNA_MODEL} ${LUNA_INSTRUMENT} ${LUNA_HELPER} ${QuantLib_LIBRARY})
endif()