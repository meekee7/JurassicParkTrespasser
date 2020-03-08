cmake_minimum_required(VERSION 3.5)
project(TweakTrespass)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND TweakTrespass_Inc    
    ${CMAKE_SOURCE_DIR}/Source/TrespassAdv/DialogName.hpp
    ${CMAKE_SOURCE_DIR}/Source/TrespassAdv/resource.h
    ${CMAKE_SOURCE_DIR}/Source/TrespassAdv/StdAfx.h
    ${CMAKE_SOURCE_DIR}/Source/TrespassAdv/TrespassAdv.h
    ${CMAKE_SOURCE_DIR}/Source/TrespassAdv/TrespassAdvDlg.h
)

list(APPEND TweakTrespass_Src
    ${CMAKE_SOURCE_DIR}/Source/TrespassAdv/DialogName.cpp
    ${CMAKE_SOURCE_DIR}/Source/TrespassAdv/StdAfx.cpp
    ${CMAKE_SOURCE_DIR}/Source/TrespassAdv/TrespassAdv.cpp
    ${CMAKE_SOURCE_DIR}/Source/TrespassAdv/TrespassAdvDlg.cpp
)

list(APPEND TweakTrespass_Rsc
    ${CMAKE_SOURCE_DIR}/Source/TrespassAdv/TrespassAdv.rc
    ${CMAKE_SOURCE_DIR}/Source/TrespassAdv/res/TrespassAdv.rc2
    ${CMAKE_SOURCE_DIR}/Source/TrespassAdv/res/TrespassAdv.ico
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

add_executable(${PROJECT_NAME} WIN32 ${TweakTrespass_Inc} ${TweakTrespass_Src} ${TweakTrespass_Rsc} )

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Tools)
