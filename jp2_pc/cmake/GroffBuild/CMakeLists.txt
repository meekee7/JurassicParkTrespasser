cmake_minimum_required(VERSION 3.5)
project(GroffBuild)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND GroffBuild_Inc    
    ${CMAKE_SOURCE_DIR}/Source/GroffBuild/dialogs.h
    ${CMAKE_SOURCE_DIR}/Source/GroffBuild/gbuilder.h
    ${CMAKE_SOURCE_DIR}/Source/GroffBuild/main.h
    ${CMAKE_SOURCE_DIR}/Source/GroffBuild/reg.h
    ${CMAKE_SOURCE_DIR}/Source/GroffBuild/utils.h
)

list(APPEND GroffBuild_Src
    ${CMAKE_SOURCE_DIR}/Source/GroffBuild/Dialogs.cpp
    ${CMAKE_SOURCE_DIR}/Source/GroffBuild/gbuilder.cpp
    ${CMAKE_SOURCE_DIR}/Source/GroffBuild/main.cpp
    ${CMAKE_SOURCE_DIR}/Source/GroffBuild/maindlg.cpp
    ${CMAKE_SOURCE_DIR}/Source/GroffBuild/reg.cpp
    ${CMAKE_SOURCE_DIR}/Source/GroffBuild/Utils.cpp
)

list(APPEND GroffBuild_Rsc
    ${CMAKE_SOURCE_DIR}/Source/GroffBuild/GroffBuild.rc
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

add_executable(${PROJECT_NAME} WIN32 ${GroffBuild_Inc} ${GroffBuild_Src} ${GroffBuild_Rsc} )

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Tools)
