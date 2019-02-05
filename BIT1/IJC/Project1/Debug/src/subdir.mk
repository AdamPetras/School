################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/eratosthenes.c \
../src/error.c \
../src/ppm.c \
../src/primes.c \
../src/steg-decode.c 

O_SRCS += \
../src/eratosthenes.o \
../src/error.o \
../src/ppm.o \
../src/primes.o \
../src/steg-decode.o 

OBJS += \
./src/eratosthenes.o \
./src/error.o \
./src/ppm.o \
./src/primes.o \
./src/steg-decode.o 

C_DEPS += \
./src/eratosthenes.d \
./src/error.d \
./src/ppm.d \
./src/primes.d \
./src/steg-decode.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -std=c99 -O2 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


