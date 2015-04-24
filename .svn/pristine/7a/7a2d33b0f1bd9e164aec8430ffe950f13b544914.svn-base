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

set(SRC_${LUNA_PRICER}
    "${CMAKE_SOURCE_DIR}/LMM/pricer/LmmApproxVanillaSwaptionPricer.h"
	"${CMAKE_SOURCE_DIR}/LMM/pricer/LmmCapletPricer.h"
	"${CMAKE_SOURCE_DIR}/LMM/pricer/LmmSwaptionPricer.h"
	"${CMAKE_SOURCE_DIR}/LMM/pricer/LmmTarnPricer.h"
	"${CMAKE_SOURCE_DIR}/LMM/pricer/McLmmVanillaSwapPricer.h"
	"${CMAKE_SOURCE_DIR}/LMM/pricer/McLmmVanillaSwaptionPricer.h"
	"${CMAKE_SOURCE_DIR}/LMM/pricer/McPricer.h"
	"${CMAKE_SOURCE_DIR}/LMM/pricer/StaticReplicationCMS.h"
	"${CMAKE_SOURCE_DIR}/LMM/pricer/VanillaSwapPricer.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/LmmApproxVanillaSwaptionPricer.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/LmmCapletPricer.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/LmmSwaptionPricer.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/LmmTarnPricer.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/McLmmVanillaSwapPricer.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/McLmmVanillaSwaptionPricer.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/McPricer.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/StaticReplicationCMS.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/VanillaSwapPricer.cpp"
)

## Catch local variable for the global scope
set(SRC_${LUNA_PRICER} ${SRC_${LUNA_PRICER}} PARENT_SCOPE)

if(NOT LUNA_BUILD_FOR_YUANLI)
	add_library(${LUNA_PRICER} ${SRC_${LUNA_PRICER}})
	target_link_libraries(${LUNA_PRICER} ${LUNA_MODEL} ${LUNA_INSTRUMENT} ${LUNA_HELPER} ${LUNA_RNGENERATOR})
endif()