cmake_minimum_required(VERSION 3.5)
project(AITest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND AITest_Inc
    ${CMAKE_SOURCE_DIR}/Source/Test/AI/AITest.hpp
    ${CMAKE_SOURCE_DIR}/Source/Test/AI/AITestShell.hpp
    ${CMAKE_SOURCE_DIR}/Source/Test/AI/AStarTest.hpp
    ${CMAKE_SOURCE_DIR}/Source/Test/AI/FakePhysics.hpp
    ${CMAKE_SOURCE_DIR}/Source/Test/AI/FakeShape.hpp
    ${CMAKE_SOURCE_DIR}/Source/GUIApp/PredefinedShapes.hpp
    ${CMAKE_SOURCE_DIR}/Source/Test/AI/TestAnimal.hpp
    ${CMAKE_SOURCE_DIR}/Source/Test/AI/TestBrains.hpp
    ${CMAKE_SOURCE_DIR}/Source/Test/AI/UIModes.hpp
)

list(APPEND AITest_Src
    ${CMAKE_SOURCE_DIR}/Source/Test/AI/AITest.cpp
    ${CMAKE_SOURCE_DIR}/Source/Test/AI/AITestShell.cpp
    ${CMAKE_SOURCE_DIR}/Source/Test/AI/AStarTest.cpp
    ${CMAKE_SOURCE_DIR}/Source/Test/AI/FakePhysics.cpp
    ${CMAKE_SOURCE_DIR}/Source/Test/AI/FakeShape.cpp
    ${CMAKE_SOURCE_DIR}/Source/GUIApp/PreDefinedShapes.cpp
    ${CMAKE_SOURCE_DIR}/Source/Test/AI/TestAnimal.cpp
    ${CMAKE_SOURCE_DIR}/Source/Test/AI/TestBrains.cpp
    ${CMAKE_SOURCE_DIR}/Source/Test/AI/UIModes.cpp
)

list(APPEND AITest_Rsc
    ${CMAKE_SOURCE_DIR}/Source/Test/AI/AI.rc
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

add_executable(${PROJECT_NAME} WIN32 ${AITest_Inc} ${AITest_Src} ${AITest_Rsc} )
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Tests)
