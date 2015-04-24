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

set(SRC_${LUNA_CALIBRATION}
	"${CMAKE_SOURCE_DIR}/LMM/calibration/ATMSwaptionMarketData.h"
	"${CMAKE_SOURCE_DIR}/LMM/calibration/ATMSwaptionStructuredData.h"
	"${CMAKE_SOURCE_DIR}/LMM/calibration/Calibration.h"
	"${CMAKE_SOURCE_DIR}/LMM/calibration/RebonatoVolatilityCostFunction.h"
	"${CMAKE_SOURCE_DIR}/LMM/calibration/TestCostFunction.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/ATMSwaptionMarketData.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/ATMSwaptionStructuredData.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Calibration.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/RebonatoVolatilityCostFunction.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/TestCostFunction.cpp"
)

## Catch local variable for the global scope
set(SRC_${LUNA_CALIBRATION} ${SRC_${LUNA_CALIBRATION}} PARENT_SCOPE)

if(NOT LUNA_BUILD_FOR_YUANLI)
	add_library(${LUNA_CALIBRATION} ${SRC_${LUNA_CALIBRATION}})
	target_link_libraries(${LUNA_CALIBRATION} ${LUNA_PRICER} ${LUNA_MODEL} ${LUNA_INSTRUMENT} ${LUNA_NUMERIC} ${LUNA_HELPER})
endif()