################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/MessengerClientCli.cpp \
../src/TCPMessengerClient.cpp \
../src/UDPMessengerClient.cpp 

OBJS += \
./src/MessengerClientCli.o \
./src/TCPMessengerClient.o \
./src/UDPMessengerClient.o 

CPP_DEPS += \
./src/MessengerClientCli.d \
./src/TCPMessengerClient.d \
./src/UDPMessengerClient.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/user/Desktop/workspace2/SocketUtils/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


