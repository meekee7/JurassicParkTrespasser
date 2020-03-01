cmake_minimum_required(VERSION 3.5)
project(CollisionEditor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CollisionEditor_Inc
    ${CMAKE_SOURCE_DIR}/Source/Tools/CollisionEditor/CollisionEditor.h
    ${CMAKE_SOURCE_DIR}/Source/Tools/CollisionEditor/CollisionEditorDlg.h
    ${CMAKE_SOURCE_DIR}/Source/Tools/CollisionEditor/Database.h
    ${CMAKE_SOURCE_DIR}/Source/Tools/CollisionEditor/EditMaterialDlg.h
    ${CMAKE_SOURCE_DIR}/Source/Tools/CollisionEditor/resource.h
    ${CMAKE_SOURCE_DIR}/Source/Tools/CollisionEditor/SampleListDlg.h
    ${CMAKE_SOURCE_DIR}/Source/Tools/CollisionEditor/stdafx.h
)

list(APPEND CollisionEditor_Src
    ${CMAKE_SOURCE_DIR}/Source/Tools/CollisionEditor/CollisionEditor.cpp
    ${CMAKE_SOURCE_DIR}/Source/Tools/CollisionEditor/CollisionEditorDlg.cpp
    ${CMAKE_SOURCE_DIR}/Source/Tools/CollisionEditor/Database.cpp
    ${CMAKE_SOURCE_DIR}/Source/Tools/CollisionEditor/EditMaterialDlg.cpp
    ${CMAKE_SOURCE_DIR}/Source/Tools/CollisionEditor/SampleListDlg.cpp
    ${CMAKE_SOURCE_DIR}/Source/Tools/CollisionEditor/StdAfx.cpp
)

list(APPEND CollisionEditor_Rsc
    ${CMAKE_SOURCE_DIR}/Source/Tools/CollisionEditor/CollisionEditor.rc
    ${CMAKE_SOURCE_DIR}/Source/Tools/CollisionEditor/res/CollisionEditor.rc2
    ${CMAKE_SOURCE_DIR}/Source/Tools/CollisionEditor/res/CollisionEditor.ico
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

add_executable(${PROJECT_NAME} WIN32 ${CollisionEditor_Inc} ${CollisionEditor_Src} ${CollisionEditor_Rsc} )

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Tools)
