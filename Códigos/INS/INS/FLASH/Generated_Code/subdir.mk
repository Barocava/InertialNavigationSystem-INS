################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Generated_Code/AS1.c" \
"../Generated_Code/AS2.c" \
"../Generated_Code/Bit1.c" \
"../Generated_Code/Bit2.c" \
"../Generated_Code/Bit3.c" \
"../Generated_Code/CI2C1.c" \
"../Generated_Code/CI2C2.c" \
"../Generated_Code/Cpu.c" \
"../Generated_Code/IO_Map.c" \
"../Generated_Code/TI1.c" \
"../Generated_Code/Vectors.c" \

C_SRCS += \
../Generated_Code/AS1.c \
../Generated_Code/AS2.c \
../Generated_Code/Bit1.c \
../Generated_Code/Bit2.c \
../Generated_Code/Bit3.c \
../Generated_Code/CI2C1.c \
../Generated_Code/CI2C2.c \
../Generated_Code/Cpu.c \
../Generated_Code/IO_Map.c \
../Generated_Code/TI1.c \
../Generated_Code/Vectors.c \

OBJS += \
./Generated_Code/AS1_c.obj \
./Generated_Code/AS2_c.obj \
./Generated_Code/Bit1_c.obj \
./Generated_Code/Bit2_c.obj \
./Generated_Code/Bit3_c.obj \
./Generated_Code/CI2C1_c.obj \
./Generated_Code/CI2C2_c.obj \
./Generated_Code/Cpu_c.obj \
./Generated_Code/IO_Map_c.obj \
./Generated_Code/TI1_c.obj \
./Generated_Code/Vectors_c.obj \

OBJS_QUOTED += \
"./Generated_Code/AS1_c.obj" \
"./Generated_Code/AS2_c.obj" \
"./Generated_Code/Bit1_c.obj" \
"./Generated_Code/Bit2_c.obj" \
"./Generated_Code/Bit3_c.obj" \
"./Generated_Code/CI2C1_c.obj" \
"./Generated_Code/CI2C2_c.obj" \
"./Generated_Code/Cpu_c.obj" \
"./Generated_Code/IO_Map_c.obj" \
"./Generated_Code/TI1_c.obj" \
"./Generated_Code/Vectors_c.obj" \

C_DEPS += \
./Generated_Code/AS1_c.d \
./Generated_Code/AS2_c.d \
./Generated_Code/Bit1_c.d \
./Generated_Code/Bit2_c.d \
./Generated_Code/Bit3_c.d \
./Generated_Code/CI2C1_c.d \
./Generated_Code/CI2C2_c.d \
./Generated_Code/Cpu_c.d \
./Generated_Code/IO_Map_c.d \
./Generated_Code/TI1_c.d \
./Generated_Code/Vectors_c.d \

OBJS_OS_FORMAT += \
./Generated_Code/AS1_c.obj \
./Generated_Code/AS2_c.obj \
./Generated_Code/Bit1_c.obj \
./Generated_Code/Bit2_c.obj \
./Generated_Code/Bit3_c.obj \
./Generated_Code/CI2C1_c.obj \
./Generated_Code/CI2C2_c.obj \
./Generated_Code/Cpu_c.obj \
./Generated_Code/IO_Map_c.obj \
./Generated_Code/TI1_c.obj \
./Generated_Code/Vectors_c.obj \

C_DEPS_QUOTED += \
"./Generated_Code/AS1_c.d" \
"./Generated_Code/AS2_c.d" \
"./Generated_Code/Bit1_c.d" \
"./Generated_Code/Bit2_c.d" \
"./Generated_Code/Bit3_c.d" \
"./Generated_Code/CI2C1_c.d" \
"./Generated_Code/CI2C2_c.d" \
"./Generated_Code/Cpu_c.d" \
"./Generated_Code/IO_Map_c.d" \
"./Generated_Code/TI1_c.d" \
"./Generated_Code/Vectors_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Generated_Code/AS1_c.obj: ../Generated_Code/AS1.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"Generated_Code/AS1.args" -o "Generated_Code/AS1_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/%.d: ../Generated_Code/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Generated_Code/AS2_c.obj: ../Generated_Code/AS2.c
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"Generated_Code/AS2.args" -o "Generated_Code/AS2_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/Bit1_c.obj: ../Generated_Code/Bit1.c
	@echo 'Building file: $<'
	@echo 'Executing target #9 $<'
	@echo 'Invoking: ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"Generated_Code/Bit1.args" -o "Generated_Code/Bit1_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/Bit2_c.obj: ../Generated_Code/Bit2.c
	@echo 'Building file: $<'
	@echo 'Executing target #10 $<'
	@echo 'Invoking: ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"Generated_Code/Bit2.args" -o "Generated_Code/Bit2_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/Bit3_c.obj: ../Generated_Code/Bit3.c
	@echo 'Building file: $<'
	@echo 'Executing target #11 $<'
	@echo 'Invoking: ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"Generated_Code/Bit3.args" -o "Generated_Code/Bit3_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/CI2C1_c.obj: ../Generated_Code/CI2C1.c
	@echo 'Building file: $<'
	@echo 'Executing target #12 $<'
	@echo 'Invoking: ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"Generated_Code/CI2C1.args" -o "Generated_Code/CI2C1_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/CI2C2_c.obj: ../Generated_Code/CI2C2.c
	@echo 'Building file: $<'
	@echo 'Executing target #13 $<'
	@echo 'Invoking: ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"Generated_Code/CI2C2.args" -o "Generated_Code/CI2C2_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/Cpu_c.obj: ../Generated_Code/Cpu.c
	@echo 'Building file: $<'
	@echo 'Executing target #14 $<'
	@echo 'Invoking: ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"Generated_Code/Cpu.args" -o "Generated_Code/Cpu_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/IO_Map_c.obj: ../Generated_Code/IO_Map.c
	@echo 'Building file: $<'
	@echo 'Executing target #15 $<'
	@echo 'Invoking: ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"Generated_Code/IO_Map.args" -o "Generated_Code/IO_Map_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/TI1_c.obj: ../Generated_Code/TI1.c
	@echo 'Building file: $<'
	@echo 'Executing target #16 $<'
	@echo 'Invoking: ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"Generated_Code/TI1.args" -o "Generated_Code/TI1_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/Vectors_c.obj: ../Generated_Code/Vectors.c
	@echo 'Building file: $<'
	@echo 'Executing target #17 $<'
	@echo 'Invoking: ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"Generated_Code/Vectors.args" -o "Generated_Code/Vectors_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '


