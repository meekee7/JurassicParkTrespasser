cmake_minimum_required(VERSION 3.5)
project(WinShell)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND WinShell_Inc
    ${CMAKE_SOURCE_DIR}/Source/Shell/AppEvent.hpp
    ${CMAKE_SOURCE_DIR}/Source/Shell/AppShell.hpp
    ${CMAKE_SOURCE_DIR}/Source/Shell/WinShell.hpp
)

list(APPEND WinShell_Src
    ${CMAKE_SOURCE_DIR}/Source/Shell/AppEvent.cpp
    ${CMAKE_SOURCE_DIR}/Source/Shell/AppShell.cpp
    ${CMAKE_SOURCE_DIR}/Source/Shell/winshell.cpp
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

add_library(${PROJECT_NAME} STATIC ${WinShell_Inc} ${WinShell_Src} )

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Tests)
