cmake_minimum_required(VERSION 3.5)
project(WaveTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)


list(APPEND WaveTest_Src
    ${CMAKE_SOURCE_DIR}/Source/Test/Physics/WaveTest.cpp
    ${CMAKE_SOURCE_DIR}/Source/Shell/WinShell.rc
)

set(CMAKE_MFC_FLAG 2)
add_compile_definitions( 
    #WIN32
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:_RELEASE>
    #_WINDOWS
)

add_executable(${PROJECT_NAME} WIN32 ${WaveTest_Src})

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Tests)
