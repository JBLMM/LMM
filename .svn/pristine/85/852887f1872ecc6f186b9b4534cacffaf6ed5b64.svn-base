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

set(SRC_${LUNA_INSTRUMENT}
	"${CMAKE_SOURCE_DIR}/LMM/instrument/Caplet.h"
	"${CMAKE_SOURCE_DIR}/LMM/instrument/CMS.h"
	"${CMAKE_SOURCE_DIR}/LMM/instrument/instrument.h"
	"${CMAKE_SOURCE_DIR}/LMM/instrument/Product.h"
	"${CMAKE_SOURCE_DIR}/LMM/instrument/Tarn.h"
	"${CMAKE_SOURCE_DIR}/LMM/instrument/VanillaSwap.h"
	"${CMAKE_SOURCE_DIR}/LMM/instrument/VanillaSwaption.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Caplet.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/CMS.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/instrument.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Tarn.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/VanillaSwap.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/VanillaSwaption.cpp"
)

## Catch local variable for the global scope
set(SRC_${LUNA_INSTRUMENT} ${SRC_${LUNA_INSTRUMENT}} PARENT_SCOPE)

if(NOT LUNA_BUILD_FOR_YUANLI) 
	add_library(${LUNA_INSTRUMENT} ${SRC_${LUNA_INSTRUMENT}})
	target_link_libraries(${LUNA_INSTRUMENT} ${LUNA_HELPER})
endif()