################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
tm4c123gh6pm_startup_ccs.obj: ../tm4c123gh6pm_startup_ccs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_17.9.0.STS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/BAO/EmbeddedProgExample/TVALab/FreeRTOSExample14/Application" --include_path="D:/BAO/EmbeddedProgExample/TVALab/FreeRTOSExample14" --include_path="D:/BAO/EmbeddedProgExample/utils" --include_path="D:/BAO/EmbeddedProgExample/TVALab/FreeRTOS/Application" --include_path="D:/BAO/EmbeddedProgExample/drivers" --include_path="D:/BAO/EmbeddedProgExample" --include_path="C:/ti/TivaWare_C_Series-2.1.4.178" --include_path="D:/BAO/EmbeddedProgExample/FreeRTOS/FreeRTOSv10.0.1/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="D:/BAO/EmbeddedProgExample/FreeRTOS/FreeRTOSv10.0.1/FreeRTOS/Source/include" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_17.9.0.STS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_BLIZZARD_RB1 -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="tm4c123gh6pm_startup_ccs.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


