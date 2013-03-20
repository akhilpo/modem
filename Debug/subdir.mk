################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../gprs.c \
../mircam_logic.c \
../modem.c \
../test.c \
../uart.c 

OBJS += \
./gprs.o \
./mircam_logic.o \
./modem.o \
./test.o \
./uart.o 

C_DEPS += \
./gprs.d \
./mircam_logic.d \
./modem.d \
./test.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


