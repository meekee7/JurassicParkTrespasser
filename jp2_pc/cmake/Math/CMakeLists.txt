cmake_minimum_required(VERSION 3.5)
project(Math)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND Math_Inc
    ${CMAKE_SOURCE_DIR}/source/Lib/Math/FastInverse.hpp
    ${CMAKE_SOURCE_DIR}/source/Lib/Math/FastSqrt.hpp
    ${CMAKE_SOURCE_DIR}/source/Lib/Math/FastTrig.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Math/FloatDef.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Math/FloatTable.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Math/MathUtil.hpp
    ${CMAKE_SOURCE_DIR}/source/Lib/Transform/Matrix.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Transform/Matrix2.hpp
    ${CMAKE_SOURCE_DIR}/source/Lib/Transform/Presence.hpp
    ${CMAKE_SOURCE_DIR}/source/Lib/Transform/Rotate.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Transform/Scale.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Transform/Shear.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Transform/Transform.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Transform/TransformIO.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Transform/Translate.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Transform/TransLinear.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Transform/Vector.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Transform/VectorRange.hpp
)

list(APPEND Math_Src
    ${CMAKE_SOURCE_DIR}/Source/Lib/Math/FastInverse.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Math/FastSqrt.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Math/FastTrig.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Transform/Matrix.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Transform/Presence.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Transform/Rotate.cpp
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

add_library(${PROJECT_NAME} STATIC ${Math_Inc} ${Math_Src} )

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Lib/Util)
