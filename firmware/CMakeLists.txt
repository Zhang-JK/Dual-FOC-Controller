cmake_minimum_required(VERSION 3.15)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# re-create build directory if you want to change the default toolchain
# set(STM32_TOOLCHAIN_PATH "~/Software/arm-none-eabi-13.2")
set(STM32_TOOLCHAIN_PATH "~/Software/arm-none-eabi-12.3")
# set(STM32_TOOLCHAIN_PATH "~/Software/arm-none-eabi-11.3")
if(DEFINED ENV{STM32_TOOLCHAIN_PATH})
    message(STATUS "Detected STM32_TOOLCHAIN_PATH in environmental variables")
    set(STM32_TOOLCHAIN_PATH $ENV{STM32_TOOLCHAIN_PATH})
endif()
message(STATUS "STM32_TOOLCHAIN_PATH: " ${STM32_TOOLCHAIN_PATH})

set(STM32_TARGET_TRIPLET "arm-none-eabi")
if(DEFINED ENV{STM32_TARGET_TRIPLET})
    message(STATUS "Detected STM32_TARGET_TRIPLET in environmental variables")
    set(STM32_TARGET_TRIPLET $ENV{STM32_TARGET_TRIPLET})
endif()
message(STATUS "STM32_TARGET_TRIPLET: " ${STM32_TARGET_TRIPLET})

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(TOOLCHAIN_SYSROOT  "${STM32_TOOLCHAIN_PATH}/${STM32_TARGET_TRIPLET}")
set(TOOLCHAIN_BIN_PATH "${STM32_TOOLCHAIN_PATH}/bin")
set(TOOLCHAIN_INC_PATH "${STM32_TOOLCHAIN_PATH}/${STM32_TARGET_TRIPLET}/include")
set(TOOLCHAIN_LIB_PATH "${STM32_TOOLCHAIN_PATH}/${STM32_TARGET_TRIPLET}/lib")
set(CMAKE_SYSROOT ${TOOLCHAIN_SYSROOT})

find_program(CMAKE_C_COMPILER NAMES ${STM32_TARGET_TRIPLET}-gcc HINTS ${TOOLCHAIN_BIN_PATH})
find_program(CMAKE_CXX_COMPILER NAMES ${STM32_TARGET_TRIPLET}-g++ HINTS ${TOOLCHAIN_BIN_PATH})
find_program(CMAKE_ASM_COMPILER NAMES ${STM32_TARGET_TRIPLET}-gcc HINTS ${TOOLCHAIN_BIN_PATH})
find_program(CMAKE_OBJCOPY NAMES ${STM32_TARGET_TRIPLET}-objcopy HINTS ${TOOLCHAIN_BIN_PATH})
find_program(CMAKE_OBJDUMP NAMES ${STM32_TARGET_TRIPLET}-objdump HINTS ${TOOLCHAIN_BIN_PATH})
find_program(CMAKE_SIZE NAMES ${STM32_TARGET_TRIPLET}-size HINTS ${TOOLCHAIN_BIN_PATH})
find_program(CMAKE_DEBUGGER NAMES ${STM32_TARGET_TRIPLET}-gdb HINTS ${TOOLCHAIN_BIN_PATH})
find_program(CMAKE_CPPFILT NAMES ${STM32_TARGET_TRIPLET}-c++filt HINTS ${TOOLCHAIN_BIN_PATH})

# generate compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# project settings
project(Dual-FOC C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

add_compile_options(-mcpu=cortex-m3 -mthumb -mthumb-interwork)
add_compile_options(-Wall -ffunction-sections -fdata-sections -fno-common -fmessage-length=0)
add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-x$<SEMICOLON>assembler-with-cpp>)
add_compile_options(-Og -g -gdwarf-2)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F103C8Tx_FLASH.ld)
add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=cortex-m3 -mthumb -mthumb-interwork)
# add_link_options(-lc -lm -lnosys -specs=nosys.specs)
add_link_options(-T ${LINKER_SCRIPT})

add_definitions(-DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB)

## ----------------------- HAL -----------------------
file(GLOB hal_sources CONFIGURE_DEPENDS Drivers/STM32F1xx_HAL_Driver/Src/*.c)
add_library(hal STATIC ${hal_sources})
target_include_directories(hal PUBLIC 
    Core/Inc 
    Drivers/STM32F1xx_HAL_Driver/Inc 
    Drivers/CMSIS/Device/ST/STM32F1xx/Include 
    Drivers/CMSIS/Include
)

## ----------------------- FREERTOS -----------------------
add_library(freertos_config INTERFACE)
target_include_directories(freertos_config INTERFACE 
    INTERFACE 
    User
)
set(FREERTOS_PORT GCC_ARM_CM3 CACHE STRING "GCC_ARM_CM3")
add_subdirectory(FreeRTOS-Kernel)

## ----------------------- USER -----------------------
file(GLOB user_sources CONFIGURE_DEPENDS User/*.c User/*.cpp Diagnostic/*.c Diagnostic/*.cpp)
add_library(user STATIC ${user_sources})
target_include_directories(user PUBLIC 
    Core/Inc 
    User
    Drivers/STM32F1xx_HAL_Driver/Inc 
    Drivers/CMSIS/Device/ST/STM32F1xx/Include 
    Drivers/CMSIS/Include
    FreeRTOS-Kernel/include
    "$<TARGET_PROPERTY:freertos_kernel_port,INTERFACE_INCLUDE_DIRECTORIES>"
)

## ----------------------- MAIN -----------------------
file(GLOB MAIN_SOURCES CONFIGURE_DEPENDS "Core/Src/*.c" "Core/Src/*.cpp" startup_stm32f103xb.s)
add_executable(${PROJECT_NAME}.elf ${MAIN_SOURCES} ${LINKER_SCRIPT})
target_include_directories(${PROJECT_NAME}.elf PUBLIC 
    Core/Inc 
    User
    Drivers/STM32F1xx_HAL_Driver/Inc 
    Drivers/CMSIS/Device/ST/STM32F1xx/Include 
    Drivers/CMSIS/Include
    FreeRTOS-Kernel/include
    "$<TARGET_PROPERTY:freertos_kernel_port,INTERFACE_INCLUDE_DIRECTORIES>"
)
target_link_libraries(${PROJECT_NAME}.elf freertos_kernel hal user)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    # COMMENT "Building ${PROJECT_NAME}.hex and ${PROJECT_NAME}.bin"
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_SIZE} ${PROJECT_NAME}.elf
)
