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
add_executable(test_apath "test_apath.cpp")
target_link_libraries(test_apath ${Boost_LIBRARIES})
set_target_properties(test_apath PROPERTIES FOLDER unitTest)

add_executable(unitTest_BoostUblasMatrix "test_BoostUblasMatrix.cpp")
target_link_libraries(unitTest_BoostUblasMatrix ${QuantLib_LIBRARY})
set_target_properties(unitTest_BoostUblasMatrix PROPERTIES FOLDER unitTest)
add_test(unitTest_BoostUblasMatrix ${LUNA_ALLIN1_TARGET_DIR}/unitTest_BoostUblasMatrix "--log_level=message")

add_executable(unitTest_calibration_ATMSwaptionMarketData "test_calibration_ATMSwaptionMarketData.cpp")
target_link_libraries(unitTest_calibration_ATMSwaptionMarketData ${LUNA_CALIBRATION})
set_target_properties(unitTest_calibration_ATMSwaptionMarketData PROPERTIES FOLDER unitTest)
add_test(unitTest_calibration_ATMSwaptionMarketData ${LUNA_ALLIN1_TARGET_DIR}/unitTest_calibration_ATMSwaptionMarketData "--log_level=message")

add_executable(unitTest_calibration_ATMSwaptionStructuredData "test_calibration_ATMSwaptionStructuredData.cpp")
target_link_libraries(unitTest_calibration_ATMSwaptionStructuredData ${LUNA_CALIBRATION})
set_target_properties(unitTest_calibration_ATMSwaptionStructuredData PROPERTIES FOLDER unitTest)
add_test(unitTest_calibration_ATMSwaptionStructuredData ${LUNA_ALLIN1_TARGET_DIR}/unitTest_calibration_ATMSwaptionStructuredData "--log_level=message")

add_executable(unittest_calibration_RebonatoVolatilityCostFunction "test_calibration_RebonatoVolatilityCostFunction.cpp")
target_link_libraries(unittest_calibration_RebonatoVolatilityCostFunction ${LUNA_CALIBRATION} ${LUNA_MODEL} ${QuantLib_LIBRARY})
set_target_properties(unittest_calibration_RebonatoVolatilityCostFunction PROPERTIES FOLDER unitTest)
add_test(unittest_calibration_RebonatoVolatilityCostFunction ${LUNA_ALLIN1_TARGET_DIR}/unittest_calibration_RebonatoVolatilityCostFunction "--log_level=message")

add_executable(unitTest_numeric_Interpolation "test_numeric_Interpolation.cpp")
target_link_libraries(unitTest_numeric_Interpolation ${LUNA_NUMERIC})
set_target_properties(unitTest_numeric_Interpolation PROPERTIES FOLDER unitTest)
add_test(unitTest_numeric_Interpolation ${LUNA_ALLIN1_TARGET_DIR}/unitTest_numeric_Interpolation "--log_level=message")

add_executable(unitTest_numeric_NumericalMethods "test_numeric_NumericalMethods.cpp")
target_link_libraries(unitTest_numeric_NumericalMethods ${LUNA_NUMERIC})
set_target_properties(unitTest_numeric_NumericalMethods PROPERTIES FOLDER unitTest)
add_test(unitTest_numeric_NumericalMethods ${LUNA_ALLIN1_TARGET_DIR}/unitTest_numeric_NumericalMethods "--log_level=message")

add_executable(unitTest_helper_LMMTenorStructure "test_helper_LMMTenorStructure.cpp")
target_link_libraries(unitTest_helper_LMMTenorStructure ${LUNA_HELPER})
set_target_properties(unitTest_helper_LMMTenorStructure PROPERTIES FOLDER unitTest)
add_test(unitTest_helper_LMMTenorStructure ${LUNA_ALLIN1_TARGET_DIR}/unitTest_helper_LMMTenorStructure "--log_level=message")

#add_executable(unitTest_helper_QMcGenerator "test_helper_QMcGenerator.cpp")
#target_link_libraries(unitTest_helper_QMcGenerator ${LUNA_HELPER})
#set_target_properties(unitTest_helper_QMcGenerator PROPERTIES FOLDER unitTest)
#add_test(unitTest_helper_QMcGenerator ${LUNA_ALLIN1_TARGET_DIR}/unitTest_helper_QMcGenerator "--log_level=message")

add_executable(unitTest_helper_Tensor "test_helper_Tensor.cpp")
target_link_libraries(unitTest_helper_Tensor ${LUNA_HELPER})
set_target_properties(unitTest_helper_Tensor PROPERTIES FOLDER unitTest)
add_test(unitTest_helper_Tensor ${LUNA_ALLIN1_TARGET_DIR}/unitTest_helper_Tensor "--log_level=message")

add_executable(unitTest_helper_Tenor "test_helper_Tenor.cpp")
target_link_libraries(unitTest_helper_Tenor ${LUNA_HELPER})
set_target_properties(unitTest_helper_Tenor PROPERTIES FOLDER unitTest)
add_test(unitTest_helper_Tenor ${LUNA_ALLIN1_TARGET_DIR}/unitTest_helper_Tenor "--log_level=message")

add_executable(unitTest_instrument_VanillaSwap "test_instrument_VanillaSwap.cpp")
target_link_libraries(unitTest_instrument_VanillaSwap ${LUNA_INSTRUMENT})
set_target_properties(unitTest_instrument_VanillaSwap PROPERTIES FOLDER unitTest)
add_test(unitTest_instrument_VanillaSwap ${LUNA_ALLIN1_TARGET_DIR}/unitTest_instrument_VanillaSwap "--log_level=message")

add_executable(unitTest_ModelMCLMM_Correlation "test_ModelMCLMM_Correlation.cpp")
target_link_libraries(unitTest_ModelMCLMM_Correlation ${LUNA_MODEL} ${QuantLib_LIBRARY})
set_target_properties(unitTest_ModelMCLMM_Correlation PROPERTIES FOLDER unitTest)
add_test(unitTest_ModelMCLMM_Correlation ${LUNA_ALLIN1_TARGET_DIR}/unitTest_ModelMCLMM_Correlation "--log_level=message")

add_executable(unitTest_ModelMCLMM_Dispersion "test_ModelMCLMM_Dispersion.cpp")
target_link_libraries(unitTest_ModelMCLMM_Dispersion ${LUNA_MODEL} ${QuantLib_LIBRARY})
set_target_properties(unitTest_ModelMCLMM_Dispersion PROPERTIES FOLDER unitTest)
add_test(unitTest_ModelMCLMM_Dispersion ${LUNA_ALLIN1_TARGET_DIR}/unitTest_ModelMCLMM_Dispersion "--log_level=message")

add_executable(unitTest_ModelMCLMM_HGVoltatilityFunction "test_ModelMCLMM_HGVolatilityFunction.cpp")
target_link_libraries(unitTest_ModelMCLMM_HGVoltatilityFunction ${LUNA_MODEL} ${QuantLib_LIBRARY})
set_target_properties(unitTest_ModelMCLMM_HGVoltatilityFunction PROPERTIES FOLDER unitTest)
add_test(unitTest_ModelMCLMM_HGVoltatilityFunction ${LUNA_ALLIN1_TARGET_DIR}/unitTest_ModelMCLMM_HGVoltatilityFunction "--log_level=message")

#add_executable(unitTest_ModelMCLMM_PCA "test_ModelMCLMM_PCA.cpp")
#target_link_libraries(unitTest_ModelMCLMM_PCA ${LUNA_MODEL} ${QuantLib_LIBRARY})
#set_target_properties(unitTest_ModelMCLMM_PCA PROPERTIES FOLDER unitTest)
#add_test(unitTest_ModelMCLMM_PCA ${LUNA_ALLIN1_TARGET_DIR}/unitTest_ModelMCLMM_PCA "--log_level=message")

add_executable(unitTest_ModelMCLMM_RobonatoAngle "test_ModelMCLMM_RobonatoAngle.cpp")
target_link_libraries(unitTest_ModelMCLMM_RobonatoAngle ${LUNA_MODEL} ${QuantLib_LIBRARY})
set_target_properties(unitTest_ModelMCLMM_RobonatoAngle PROPERTIES FOLDER unitTest)
add_test(unitTest_ModelMCLMM_RobonatoAngle ${LUNA_ALLIN1_TARGET_DIR}/unitTest_ModelMCLMM_RobonatoAngle "--log_level=message")

add_executable(unitTest_pricer_LmmApproxVanillaSwaptionPricer "test_pricer_LmmApproxVanillaSwaptionPricer.cpp")
target_link_libraries(unitTest_pricer_LmmApproxVanillaSwaptionPricer ${LUNA_PRICER} ${QuantLib_LIBRARY})
set_target_properties(unitTest_pricer_LmmApproxVanillaSwaptionPricer PROPERTIES FOLDER unitTest)
add_test(unitTest_pricer_LmmApproxVanillaSwaptionPricer ${LUNA_ALLIN1_TARGET_DIR}/unitTest_pricer_LmmApproxVanillaSwaptionPricer "--log_level=message")

add_executable(unitTest_QuantLibIntegrationMethod "test_QuantLibIntegrationMethod.cpp")
target_link_libraries(unitTest_QuantLibIntegrationMethod ${QuantLib_LIBRARY})
set_target_properties(unitTest_QuantLibIntegrationMethod PROPERTIES FOLDER unitTest)
add_test(unitTest_QuantLibIntegrationMethod ${LUNA_ALLIN1_TARGET_DIR}/unitTest_QuantLibIntegrationMethod "--log_level=message")