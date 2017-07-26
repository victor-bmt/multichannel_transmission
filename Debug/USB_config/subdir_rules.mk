################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
USB_config/UsbIsr.obj: ../USB_config/UsbIsr.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs_v7/ccsv7/tools/compiler/ti-cgt-msp430_16.9.1.LTS/bin/cl430" -vmspx --abi=eabi --data_model=restricted -Ooff --include_path="C:/ti/ccs_v7/ccsv7/ccs_base/msp430/include" --include_path="C:/ti/ccs_v7/ccsv7/tools/compiler/ti-cgt-msp430_16.9.1.LTS/include" --include_path="C:/Users/user1/workspace_v6_0/SimpleUsbBackchannel/driverlib/MSP430F5xx_6xx" --include_path="C:/Users/user1/workspace_v6_0/SimpleUsbBackchannel/" --include_path="C:/Users/user1/workspace_v6_0/SimpleUsbBackchannel/USB_config" --advice:power=all -g --define=__MSP430F5529__ --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="USB_config/UsbIsr.d" --obj_directory="USB_config" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

USB_config/descriptors.obj: ../USB_config/descriptors.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs_v7/ccsv7/tools/compiler/ti-cgt-msp430_16.9.1.LTS/bin/cl430" -vmspx --abi=eabi --data_model=restricted -Ooff --include_path="C:/ti/ccs_v7/ccsv7/ccs_base/msp430/include" --include_path="C:/ti/ccs_v7/ccsv7/tools/compiler/ti-cgt-msp430_16.9.1.LTS/include" --include_path="C:/Users/user1/workspace_v6_0/SimpleUsbBackchannel/driverlib/MSP430F5xx_6xx" --include_path="C:/Users/user1/workspace_v6_0/SimpleUsbBackchannel/" --include_path="C:/Users/user1/workspace_v6_0/SimpleUsbBackchannel/USB_config" --advice:power=all -g --define=__MSP430F5529__ --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="USB_config/descriptors.d" --obj_directory="USB_config" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


