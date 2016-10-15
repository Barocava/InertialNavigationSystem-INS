################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Project_Settings/Startup_Code/startcf.c" \

C_SRCS += \
../Project_Settings/Startup_Code/startcf.c \

OBJS += \
./Project_Settings/Startup_Code/startcf_c.obj \

OBJS_QUOTED += \
"./Project_Settings/Startup_Code/startcf_c.obj" \

C_DEPS += \
./Project_Settings/Startup_Code/startcf_c.d \

OBJS_OS_FORMAT += \
./Project_Settings/Startup_Code/startcf_c.obj \

C_DEPS_QUOTED += \
"./Project_Settings/Startup_Code/startcf_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Project_Settings/Startup_Code/startcf_c.obj: ../Project_Settings/Startup_Code/startcf.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"Project_Settings/Startup_Code/startcf.args" -o "Project_Settings/Startup_Code/startcf_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Project_Settings/Startup_Code/%.d: ../Project_Settings/Startup_Code/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


