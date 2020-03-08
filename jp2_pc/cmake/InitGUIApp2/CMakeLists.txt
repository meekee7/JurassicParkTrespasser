cmake_minimum_required(VERSION 3.5)
project(InitGUIApp2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND InitGUIApp2_Inc    
    ${CMAKE_SOURCE_DIR}/Source/InitGUIApp2/resource.h
    ${CMAKE_SOURCE_DIR}/Source/InitGUIApp2/StdAfx.h
    ${CMAKE_SOURCE_DIR}/Source/InitGUIApp2/InitGUIApp2.h
    ${CMAKE_SOURCE_DIR}/Source/InitGUIApp2/InitGUIApp2Dlg.h
)

list(APPEND InitGUIApp2_Src
    ${CMAKE_SOURCE_DIR}/Source/InitGUIApp2/StdAfx.cpp
    ${CMAKE_SOURCE_DIR}/Source/InitGUIApp2/InitGUIApp2.cpp
    ${CMAKE_SOURCE_DIR}/Source/InitGUIApp2/InitGUIApp2Dlg.cpp
)

list(APPEND InitGUIApp2_Rsc
    ${CMAKE_SOURCE_DIR}/Source/InitGUIApp2/InitGUIApp2.rc
    ${CMAKE_SOURCE_DIR}/Source/InitGUIApp2/res/InitGUIApp2.rc2
    ${CMAKE_SOURCE_DIR}/Source/InitGUIApp2/res/InitGUIApp2.ico
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
    
    $<$<CONFIG:Debug>:TARGET_PROCESSOR=PROCESSOR_PENTIUMPRO>
    $<$<CONFIG:Release>:TARGET_PROCESSOR=PROCESSOR_PENTIUMPRO>
    $<$<CONFIG:Final>:TARGET_PROCESSOR=PROCESSOR_PENTIUMPRO>
    #_WINDOWS
)

add_executable(${PROJECT_NAME} WIN32 ${InitGUIApp2_Inc} ${InitGUIApp2_Src} ${InitGUIApp2_Rsc} )

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Tools)

target_link_libraries(${PROJECT_NAME}
    System
    GeomDBase
)
