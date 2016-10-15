################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/EmbeddedKalman.c" \
"../Sources/Events.c" \
"../Sources/Functions.c" \
"../Sources/INS_GPS.c" \
"../Sources/main.c" \

C_SRCS += \
../Sources/EmbeddedKalman.c \
../Sources/Events.c \
../Sources/Functions.c \
../Sources/INS_GPS.c \
../Sources/main.c \

OBJS += \
./Sources/EmbeddedKalman_c.obj \
./Sources/Events_c.obj \
./Sources/Functions_c.obj \
./Sources/INS_GPS_c.obj \
./Sources/main_c.obj \

OBJS_QUOTED += \
"./Sources/EmbeddedKalman_c.obj" \
"./Sources/Events_c.obj" \
"./Sources/Functions_c.obj" \
"./Sources/INS_GPS_c.obj" \
"./Sources/main_c.obj" \

C_DEPS += \
./Sources/EmbeddedKalman_c.d \
./Sources/Events_c.d \
./Sources/Functions_c.d \
./Sources/INS_GPS_c.d \
./Sources/main_c.d \

OBJS_OS_FORMAT += \
./Sources/EmbeddedKalman_c.obj \
./Sources/Events_c.obj \
./Sources/Functions_c.obj \
./Sources/INS_GPS_c.obj \
./Sources/main_c.obj \

C_DEPS_QUOTED += \
"./Sources/EmbeddedKalman_c.d" \
"./Sources/Events_c.d" \
"./Sources/Functions_c.d" \
"./Sources/INS_GPS_c.d" \
"./Sources/main_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Sources/EmbeddedKalman_c.obj: ../Sources/EmbeddedKalman.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"Sources/EmbeddedKalman.args" -o "Sources/EmbeddedKalman_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/%.d: ../Sources/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/Events_c.obj: ../Sources/Events.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"Sources/Events.args" -o "Sources/Events_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Functions_c.obj: ../Sources/Functions.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"Sources/Functions.args" -o "Sources/Functions_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/INS_GPS_c.obj: ../Sources/INS_GPS.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"Sources/INS_GPS.args" -o "Sources/INS_GPS_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/main_c.obj: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"Sources/main.args" -o "Sources/main_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '


