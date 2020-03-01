cmake_minimum_required(VERSION 3.5)
project(ProcessorDetect)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND ProcessorDetect_Inc
    ${CMAKE_SOURCE_DIR}/Source/Lib/Sys/Processor.hpp
)

list(APPEND ProcessorDetect_Src
    "${CMAKE_SOURCE_DIR}/Source/Tools/Processor Detect/Detect.cpp"
)

include_directories(
    ${CMAKE_SOURCE_DIR}/Source
    ${CMAKE_SOURCE_DIR}/Source/gblinc
)

set(CMAKE_MFC_FLAG 2)
add_compile_definitions( 
    #WIN32
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Debug>:BUILDVER_MODE=MODE_DEBUG>
    $<$<CONFIG:Release>:BUILDVER_MODE=MODE_RELEASE>
    $<$<CONFIG:Final>:BUILDVER_MODE=MODE_FINAL>
    $<$<CONFIG:Release>:_RELEASE>
    $<$<CONFIG:Final>:_RELEASE>
    
    $<$<CONFIG:Debug>:TARGET_PROCESSOR=PROCESSOR_PENTIUMPRO
    $<$<CONFIG:Release>:TARGET_PROCESSOR=PROCESSOR_PENTIUMPRO
    $<$<CONFIG:Final>:TARGET_PROCESSOR=PROCESSOR_PENTIUMPRO
    #_WINDOWS
)

add_library(${PROJECT_NAME} SHARED ${ProcessorDetect_Inc} ${ProcessorDetect_Src} )

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Lib/Other)
