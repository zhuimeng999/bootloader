################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/x6818/clkgen.c \
../lib/x6818/clkpwr.c \
../lib/x6818/ddr3.c \
../lib/x6818/gpio.c \
../lib/x6818/iprst.c \
../lib/x6818/tieoff.c \
../lib/x6818/uart.c 

OBJS += \
./lib/x6818/clkgen.o \
./lib/x6818/clkpwr.o \
./lib/x6818/ddr3.o \
./lib/x6818/gpio.o \
./lib/x6818/iprst.o \
./lib/x6818/tieoff.o \
./lib/x6818/uart.o 

C_DEPS += \
./lib/x6818/clkgen.d \
./lib/x6818/clkpwr.d \
./lib/x6818/ddr3.d \
./lib/x6818/gpio.d \
./lib/x6818/iprst.d \
./lib/x6818/tieoff.d \
./lib/x6818/uart.d 


# Each subdirectory must supply rules for building sources it contributes
lib/x6818/%.o: ../lib/x6818/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-a53 -marm -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Werror -Wall -Wextra  -g -I"/home/lucius/Projects/eclipse-workspace/bootloader/include" -std=gnu11 -Wbad-function-cast -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


