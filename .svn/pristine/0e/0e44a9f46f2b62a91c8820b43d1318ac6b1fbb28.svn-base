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

add_executable(prog_MCvsApprox_SwaptionPricer "prog_MCvsApprox_SwaptionPricer.cpp")
target_link_libraries(prog_MCvsApprox_SwaptionPricer ${LUNA_NUMERIC} ${LUNA_HELPER} ${LUNA_INSTRUMENT} ${LUNA_MODEL} ${LUNA_PRICER})

add_executable(prog_RebonatoVolatilityCalibrator "prog_RebonatoVolatilityCalibrator.cpp")
target_link_libraries(prog_RebonatoVolatilityCalibrator ${LUNA_CALIBRATION} ${LUNA_NUMERIC} ${LUNA_HELPER} ${LUNA_INSTRUMENT} ${LUNA_MODEL} ${LUNA_PRICER})