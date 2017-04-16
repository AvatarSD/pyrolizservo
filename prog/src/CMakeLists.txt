
set(CMAKE_SYSTEM_NAME Generic)
set(TOOLCHAIN_PATH "D:/Win\ Programs/Atmel/Studio/7.0/toolchain/avr8/avr8-gnu-toolchain")
set(CMAKE_MAKE_PROGRAM "${TOOLCHAIN_PATH}/bin/make.exe")
set(CMAKE_C_COMPILER    "${TOOLCHAIN_PATH}/bin/avr-gcc.exe")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PATH}/bin/avr-g++.exe")
set(CMAKE_FIND_ROOT_PATH "${TOOLCHAIN_PATH}/avr")
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE ON)


project("AvtomotiveServo")

SET(flags "-mmcu=atmega168p -D__AVR_ATmega168P__ -DF_CPU=8000000UL \
	-Os -Wall -Wl,--gc-sections -fdata-sections -ffunction-sections \
         -flto -fshort-enums ") #-fno-threadsafe-statics
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${flags}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flags}")


include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/)



file(GLOB_RECURSE EXE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
add_executable(${PROJECT_NAME} ${EXE_SRC})


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND
        ${TOOLCHAIN_PATH}/bin/avr-nm.exe --print-size -t d -C --size-sort ${PROJECT_NAME})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND
        ${TOOLCHAIN_PATH}/bin/avr-size.exe -A -t ${PROJECT_NAME})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND
        ${TOOLCHAIN_PATH}/bin/avr-objcopy.exe -j .text -j .data -O ihex ${PROJECT_NAME} ${PROJECT_NAME}.hex)
#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND
#        ${TOOLCHAIN_PATH}/bin/avr-objcopy.exe -j .eeprom --change-section-lma .eeprom=0 -O ihex ${PROJECT_NAME} ${PROJECT_NAME}_eeprom.hex)

#program to board
#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND
#	avrdude -v -e -p m168p -c dragon_isp -Uflash:w:${PROJECT_NAME}.hex:i)
