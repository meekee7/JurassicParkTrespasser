cmake_minimum_required(VERSION 3.5)
project(QuantizerToolCLI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND QuantizerToolCLI_Src
    ${CMAKE_SOURCE_DIR}/Source/Tools/QuantizerTool/QuantMain.c
    ${CMAKE_SOURCE_DIR}/Source/Tools/QuantizerTool/NEUQUANT.C
)


set(CMAKE_MFC_FLAG 2)
add_compile_definitions( 
    #WIN32
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:_RELEASE>
    #_WINDOWS
)


add_executable(${PROJECT_NAME} ${QuantizerToolCLI_Src} )

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Tools)
