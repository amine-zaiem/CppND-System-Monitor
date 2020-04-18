################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/format.cpp \
../src/linux_parser.cpp \
../src/main.cpp \
../src/ncurses_display.cpp \
../src/process.cpp \
../src/processor.cpp \
../src/system.cpp 

OBJS += \
./src/format.o \
./src/linux_parser.o \
./src/main.o \
./src/ncurses_display.o \
./src/process.o \
./src/processor.o \
./src/system.o 

CPP_DEPS += \
./src/format.d \
./src/linux_parser.d \
./src/main.d \
./src/ncurses_display.d \
./src/process.d \
./src/processor.d \
./src/system.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


