cmake_minimum_required(VERSION 3.5)
project(MathTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND MathTest_Inc    
    ${CMAKE_SOURCE_DIR}/Source/Test/TestMath.hpp
)

list(APPEND MathTest_Src
    ${CMAKE_SOURCE_DIR}/Source/Test/TestMath.cpp
)

list(APPEND MathTest_Rsc
    ${CMAKE_SOURCE_DIR}/Source/Shell/WinShell.rc

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

add_executable(${PROJECT_NAME} WIN32 ${MathTest_Inc} ${MathTest_Src} ${MathTest_Rsc} )

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Tests)
