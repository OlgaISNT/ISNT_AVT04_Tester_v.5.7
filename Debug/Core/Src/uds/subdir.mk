################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/uds/extras.c \
../Core/Src/uds/uds.c 

OBJS += \
./Core/Src/uds/extras.o \
./Core/Src/uds/uds.o 

C_DEPS += \
./Core/Src/uds/extras.d \
./Core/Src/uds/uds.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/uds/%.o: ../Core/Src/uds/%.c Core/Src/uds/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -D__FPU_PRESENT -DARM_MATH_CM4 -c -I"C:/Users/olgas/projects/chery/ISNT_AVT04_Tester_v.5.7_git/Cherry_Project FreeRTOS/Drivers/CMSIS/DSP/Include" -I"C:/Users/olgas/projects/chery/ISNT_AVT04_Tester_v.5.7_git/Cherry_Project FreeRTOS/Core/Src/uds" -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

