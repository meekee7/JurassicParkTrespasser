cmake_minimum_required(VERSION 3.5)
project(File)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND File_Inc
    ${CMAKE_SOURCE_DIR}/Source/Lib/File/Image.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/File/Section.hpp
)

list(APPEND File_Src
    ${CMAKE_SOURCE_DIR}/Source/Lib/File/Image.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/File/Section.cpp
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

add_library(${PROJECT_NAME} STATIC ${File_Inc} ${File_Src} )

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Lib/Util)
