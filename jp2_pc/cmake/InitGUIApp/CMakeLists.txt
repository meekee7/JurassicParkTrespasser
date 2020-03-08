cmake_minimum_required(VERSION 3.5)
project(InitGUIApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND InitGUIApp_Inc    
    ${CMAKE_SOURCE_DIR}/Source/InitGUIApp/DDDevice.hpp
    ${CMAKE_SOURCE_DIR}/Source/InitGUIApp/resource.h
    ${CMAKE_SOURCE_DIR}/Source/InitGUIApp/StdAfx.h
    ${CMAKE_SOURCE_DIR}/Source/InitGUIApp/InitGUIApp.h
    ${CMAKE_SOURCE_DIR}/Source/InitGUIApp/InitGUIAppDlg.h
)

list(APPEND InitGUIApp_Src
    ${CMAKE_SOURCE_DIR}/Source/InitGUIApp/DDDevice.cpp
    ${CMAKE_SOURCE_DIR}/Source/InitGUIApp/StdAfx.cpp
    ${CMAKE_SOURCE_DIR}/Source/InitGUIApp/InitGUIApp.cpp
    ${CMAKE_SOURCE_DIR}/Source/InitGUIApp/InitGUIAppDlg.cpp
)

list(APPEND InitGUIApp_Rsc
    ${CMAKE_SOURCE_DIR}/Source/InitGUIApp/InitGUIApp.rc
    ${CMAKE_SOURCE_DIR}/Source/InitGUIApp/res/InitGUIApp.rc2
    ${CMAKE_SOURCE_DIR}/Source/InitGUIApp/res/InitGUIApp.ico
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

add_executable(${PROJECT_NAME} WIN32 ${InitGUIApp_Inc} ${InitGUIApp_Src} ${InitGUIApp_Rsc} )

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Tools)
