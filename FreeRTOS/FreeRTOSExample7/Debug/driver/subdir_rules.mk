################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
driver/buttons.obj: ../driver/buttons.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_17.9.0.STS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/BAO/EmbeddedProgExample/TVALab/FreeRTOSExample7" --include_path="D:/BAO/EmbeddedProgExample/TVALab/Example1FreeRTOS/Application" --include_path="D:/BAO/EmbeddedProgExample/utils" --include_path="D:/BAO/EmbeddedProgExample/TVALab/FreeRTOS/Application" --include_path="D:/BAO/EmbeddedProgExample/drivers" --include_path="D:/BAO/EmbeddedProgExample" --include_path="C:/ti/TivaWare_C_Series-2.1.4.178" --include_path="D:/BAO/EmbeddedProgExample/FreeRTOS/FreeRTOSv10.0.1/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="D:/BAO/EmbeddedProgExample/FreeRTOS/FreeRTOSv10.0.1/FreeRTOS/Source/include" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_17.9.0.STS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_BLIZZARD_RB1 -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="driver/buttons.d_raw" --obj_directory="driver" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driver/led.obj: ../driver/led.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_17.9.0.STS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/BAO/EmbeddedProgExample/TVALab/FreeRTOSExample7" --include_path="D:/BAO/EmbeddedProgExample/TVALab/Example1FreeRTOS/Application" --include_path="D:/BAO/EmbeddedProgExample/utils" --include_path="D:/BAO/EmbeddedProgExample/TVALab/FreeRTOS/Application" --include_path="D:/BAO/EmbeddedProgExample/drivers" --include_path="D:/BAO/EmbeddedProgExample" --include_path="C:/ti/TivaWare_C_Series-2.1.4.178" --include_path="D:/BAO/EmbeddedProgExample/FreeRTOS/FreeRTOSv10.0.1/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="D:/BAO/EmbeddedProgExample/FreeRTOS/FreeRTOSv10.0.1/FreeRTOS/Source/include" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_17.9.0.STS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_BLIZZARD_RB1 -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="driver/led.d_raw" --obj_directory="driver" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driver/rgb.obj: ../driver/rgb.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_17.9.0.STS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/BAO/EmbeddedProgExample/TVALab/FreeRTOSExample7" --include_path="D:/BAO/EmbeddedProgExample/TVALab/Example1FreeRTOS/Application" --include_path="D:/BAO/EmbeddedProgExample/utils" --include_path="D:/BAO/EmbeddedProgExample/TVALab/FreeRTOS/Application" --include_path="D:/BAO/EmbeddedProgExample/drivers" --include_path="D:/BAO/EmbeddedProgExample" --include_path="C:/ti/TivaWare_C_Series-2.1.4.178" --include_path="D:/BAO/EmbeddedProgExample/FreeRTOS/FreeRTOSv10.0.1/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="D:/BAO/EmbeddedProgExample/FreeRTOS/FreeRTOSv10.0.1/FreeRTOS/Source/include" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_17.9.0.STS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_BLIZZARD_RB1 -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="driver/rgb.d_raw" --obj_directory="driver" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driver/switches.obj: ../driver/switches.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_17.9.0.STS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/BAO/EmbeddedProgExample/TVALab/FreeRTOSExample7" --include_path="D:/BAO/EmbeddedProgExample/TVALab/Example1FreeRTOS/Application" --include_path="D:/BAO/EmbeddedProgExample/utils" --include_path="D:/BAO/EmbeddedProgExample/TVALab/FreeRTOS/Application" --include_path="D:/BAO/EmbeddedProgExample/drivers" --include_path="D:/BAO/EmbeddedProgExample" --include_path="C:/ti/TivaWare_C_Series-2.1.4.178" --include_path="D:/BAO/EmbeddedProgExample/FreeRTOS/FreeRTOSv10.0.1/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="D:/BAO/EmbeddedProgExample/FreeRTOS/FreeRTOSv10.0.1/FreeRTOS/Source/include" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_17.9.0.STS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_BLIZZARD_RB1 -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="driver/switches.d_raw" --obj_directory="driver" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driver/uartstdio.obj: ../driver/uartstdio.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_17.9.0.STS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/BAO/EmbeddedProgExample/TVALab/FreeRTOSExample7" --include_path="D:/BAO/EmbeddedProgExample/TVALab/Example1FreeRTOS/Application" --include_path="D:/BAO/EmbeddedProgExample/utils" --include_path="D:/BAO/EmbeddedProgExample/TVALab/FreeRTOS/Application" --include_path="D:/BAO/EmbeddedProgExample/drivers" --include_path="D:/BAO/EmbeddedProgExample" --include_path="C:/ti/TivaWare_C_Series-2.1.4.178" --include_path="D:/BAO/EmbeddedProgExample/FreeRTOS/FreeRTOSv10.0.1/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="D:/BAO/EmbeddedProgExample/FreeRTOS/FreeRTOSv10.0.1/FreeRTOS/Source/include" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_17.9.0.STS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_BLIZZARD_RB1 -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="driver/uartstdio.d_raw" --obj_directory="driver" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


