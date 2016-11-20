################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ChatRoom.cpp \
../src/Dispatcher.cpp \
../src/MessengerServerCli.cpp \
../src/RegisterAndLogIn.cpp \
../src/Session.cpp \
../src/TCPMessengerServer.cpp \
../src/User.cpp \
../src/test.cpp 

OBJS += \
./src/ChatRoom.o \
./src/Dispatcher.o \
./src/MessengerServerCli.o \
./src/RegisterAndLogIn.o \
./src/Session.o \
./src/TCPMessengerServer.o \
./src/User.o \
./src/test.o 

CPP_DEPS += \
./src/ChatRoom.d \
./src/Dispatcher.d \
./src/MessengerServerCli.d \
./src/RegisterAndLogIn.d \
./src/Session.d \
./src/TCPMessengerServer.d \
./src/User.d \
./src/test.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/user/Desktop/workspace2/SocketUtils/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


