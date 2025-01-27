cmake_minimum_required(VERSION 3.22)

project(stm32cubemx)
add_library(stm32cubemx INTERFACE)

{{sr:Enable_CMake_lanuages}}

target_compile_definitions(stm32cubemx INTERFACE{{sr:symbols_c_SYMB}}
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(stm32cubemx INTERFACE{{sr:include_c_DIRS}}
)

target_sources(stm32cubemx INTERFACE{{sr:sources_SRCS}}
)

target_link_directories(stm32cubemx INTERFACE{{sr:link_DIRS}}
)

target_link_libraries(stm32cubemx INTERFACE{{sr:link_LIBS}}
)

# Validate that STM32CubeMX code is compatible with C standard
if(CMAKE_C_STANDARD LESS 11)
    message(ERROR "Generated code requires C11 or higher")
endif()

{{sr:Verify_CPP_STD}}
