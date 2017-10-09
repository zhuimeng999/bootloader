################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c 

S_UPPER_SRCS += \
../src/start.S 

OBJS += \
./src/main.o \
./src/start.o 

S_UPPER_DEPS += \
./src/start.d 

C_DEPS += \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-a53 -marm -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Werror -Wall -Wextra  -g -I"/home/lucius/Projects/eclipse-workspace/bootloader/include" -std=gnu11 -Wbad-function-cast -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross Assembler'
	arm-none-eabi-gcc -mcpu=cortex-a53 -marm -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Werror -Wall -Wextra  -g -x assembler-with-cpp -I"/home/lucius/Projects/eclipse-workspace/bootloader/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


