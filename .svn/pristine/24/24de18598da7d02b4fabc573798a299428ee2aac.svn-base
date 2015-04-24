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

add_executable(smallTest_DerivatibleFunction "Test_DerivatibleFunction.cpp")
target_link_libraries(smallTest_DerivatibleFunction ${LUNA_HELPER})
set_target_properties(smallTest_DerivatibleFunction PROPERTIES FOLDER smallTest)
add_test(smallTest_DerivatibleFunction ${LUNA_ALLIN1_TARGET_DIR}/smallTest_DerivatibleFunction "--log_level=message")

add_executable(smallTest_BlackGreek "Test_BlackGreek.cpp")
target_link_libraries(smallTest_BlackGreek ${LUNA_HELPER} ${LUNA_NUMERIC})
set_target_properties(smallTest_BlackGreek PROPERTIES FOLDER smallTest)
add_test(smallTest_BlackGreek ${LUNA_ALLIN1_TARGET_DIR}/smallTest_BlackGreek "--log_level=message")

add_executable(smallTest_McTerminalLmm "Test_McTerminalLmm.cpp")
target_link_libraries(smallTest_McTerminalLmm ${LUNA_MODEL} ${LUNA_HELPER} ${LUNA_RNGENERATOR})
set_target_properties(smallTest_McTerminalLmm PROPERTIES FOLDER smallTest)
add_test(smallTest_McTerminalLmm ${LUNA_ALLIN1_TARGET_DIR}/smallTest_McTerminalLmm "--log_level=message")

add_executable(smallTest_VanillaSwapPricer "Test_VanillaSwapPricer.cpp")
target_link_libraries(smallTest_VanillaSwapPricer ${LUNA_PRICER} ${LUNA_MODEL} ${LUNA_HELPER})
set_target_properties(smallTest_VanillaSwapPricer PROPERTIES FOLDER smallTest)
add_test(smallTest_VanillaSwapPricer ${LUNA_ALLIN1_TARGET_DIR}/smallTest_VanillaSwapPricer "--log_level=message")

add_executable(smallTest_VanillaSwaptionPricer "Test_VanillaSwaptionPricer.cpp")
target_link_libraries(smallTest_VanillaSwaptionPricer ${LUNA_PRICER} ${LUNA_MODEL} ${LUNA_HELPER})
set_target_properties(smallTest_VanillaSwaptionPricer PROPERTIES FOLDER smallTest)
add_test(smallTest_VanillaSwaptionPricer ${LUNA_ALLIN1_TARGET_DIR}/smallTest_VanillaSwaptionPricer "--log_level=message")