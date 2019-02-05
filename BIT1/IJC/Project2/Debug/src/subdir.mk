################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/htab_bucket_count.c \
../src/htab_clear.c \
../src/htab_find.c \
../src/htab_foreach.c \
../src/htab_free.c \
../src/htab_init.c \
../src/htab_lookup_add.c \
../src/htab_move.c \
../src/htab_remove.c \
../src/htab_size.c \
../src/io.c \
../src/tail.c \
../src/wordcount.c 

O_SRCS += \
../src/htab_bucket_count-shared.o \
../src/htab_bucket_count-static.o \
../src/htab_clear-shared.o \
../src/htab_clear-static.o \
../src/htab_find-shared.o \
../src/htab_find-static.o \
../src/htab_foreach-shared.o \
../src/htab_foreach-static.o \
../src/htab_free-shared.o \
../src/htab_free-static.o \
../src/htab_init-shared.o \
../src/htab_init-static.o \
../src/htab_lookup_add-shared.o \
../src/htab_lookup_add-static.o \
../src/htab_move-shared.o \
../src/htab_move-static.o \
../src/htab_remove-shared.o \
../src/htab_remove-static.o \
../src/htab_size-shared.o \
../src/htab_size-static.o \
../src/io.o \
../src/tail.o \
../src/tail2.o \
../src/wordcount.o 

OBJS += \
./src/htab_bucket_count.o \
./src/htab_clear.o \
./src/htab_find.o \
./src/htab_foreach.o \
./src/htab_free.o \
./src/htab_init.o \
./src/htab_lookup_add.o \
./src/htab_move.o \
./src/htab_remove.o \
./src/htab_size.o \
./src/io.o \
./src/tail.o \
./src/wordcount.o 

C_DEPS += \
./src/htab_bucket_count.d \
./src/htab_clear.d \
./src/htab_find.d \
./src/htab_foreach.d \
./src/htab_free.d \
./src/htab_init.d \
./src/htab_lookup_add.d \
./src/htab_move.d \
./src/htab_remove.d \
./src/htab_size.d \
./src/io.d \
./src/tail.d \
./src/wordcount.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -std=c99 -O0 -g3 -pedantic -Wall -Wextra -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


