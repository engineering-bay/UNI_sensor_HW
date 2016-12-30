################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
hal.obj: ../hal.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.3.0.STS/bin/cl430" -vmsp --use_hw_mpy=none --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/Work/Projects/02-SmartHome/03_unified_sensor/04-Embedded/uni_sensor_hwr_1_2" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.3.0.STS/include" --advice:power_severity=suppress -g --define=__MSP430G2553__ --display_error_number --diag_wrap=off --diag_warning=225 --abi=eabi --printf_support=minimal --preproc_with_compile --preproc_dependency="hal.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

init.obj: ../init.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.3.0.STS/bin/cl430" -vmsp --use_hw_mpy=none --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/Work/Projects/02-SmartHome/03_unified_sensor/04-Embedded/uni_sensor_hwr_1_2" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.3.0.STS/include" --advice:power_severity=suppress -g --define=__MSP430G2553__ --display_error_number --diag_wrap=off --diag_warning=225 --abi=eabi --printf_support=minimal --preproc_with_compile --preproc_dependency="init.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.3.0.STS/bin/cl430" -vmsp --use_hw_mpy=none --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/Work/Projects/02-SmartHome/03_unified_sensor/04-Embedded/uni_sensor_hwr_1_2" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.3.0.STS/include" --advice:power_severity=suppress -g --define=__MSP430G2553__ --display_error_number --diag_wrap=off --diag_warning=225 --abi=eabi --printf_support=minimal --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

rf_module.obj: ../rf_module.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.3.0.STS/bin/cl430" -vmsp --use_hw_mpy=none --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/Work/Projects/02-SmartHome/03_unified_sensor/04-Embedded/uni_sensor_hwr_1_2" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.3.0.STS/include" --advice:power_severity=suppress -g --define=__MSP430G2553__ --display_error_number --diag_wrap=off --diag_warning=225 --abi=eabi --printf_support=minimal --preproc_with_compile --preproc_dependency="rf_module.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


