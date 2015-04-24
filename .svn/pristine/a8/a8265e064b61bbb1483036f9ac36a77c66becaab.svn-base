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

add_subdirectory(numeric)
add_subdirectory(RNGenerator)
add_subdirectory(helper)
add_subdirectory(instrument)
add_subdirectory(LmmModel)
add_subdirectory(pricer)
add_subdirectory(calibration)

## Catch local variable for the global scope
set(SRC_${LUNA_RNGENERATOR} ${SRC_${LUNA_RNGENERATOR}} PARENT_SCOPE)
set(SRC_${LUNA_NUMERIC} ${SRC_${LUNA_NUMERIC}} PARENT_SCOPE)
set(SRC_${LUNA_HELPER} ${SRC_${LUNA_HELPER}} PARENT_SCOPE)
set(SRC_${LUNA_INSTRUMENT} ${SRC_${LUNA_INSTRUMENT}} PARENT_SCOPE)
set(SRC_${LUNA_MODEL} ${SRC_${LUNA_MODEL}} PARENT_SCOPE)
set(SRC_${LUNA_PRICER} ${SRC_${LUNA_PRICER}} PARENT_SCOPE)
set(SRC_${LUNA_CALIBRATION} ${SRC_${LUNA_CALIBRATION}} PARENT_SCOPE)