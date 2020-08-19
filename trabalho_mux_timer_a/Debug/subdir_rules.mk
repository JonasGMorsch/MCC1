################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/Development/CCS930/ccs/tools/compiler/ti-cgt-msp430_18.12.4.LTS/bin/cl430" -vmsp --use_hw_mpy=none --include_path="C:/Development/CCS930/ccs/ccs_base/msp430/include" --include_path="C:/Development/CCS930/workspace/trabalho_mux_timer_a" --include_path="C:/Development/CCS930/ccs/tools/compiler/ti-cgt-msp430_18.12.4.LTS/include" --advice:power="all" --define=__MSP430F2132__ -g --c99 --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU19 --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


