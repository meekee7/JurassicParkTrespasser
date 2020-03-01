cmake_minimum_required(VERSION 3.5)
project(ScreenRenderDWI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND ScreenRenderDWI_Inc
    ${CMAKE_SOURCE_DIR}/Source/gblinc/AsmSupport.hpp
    ${CMAKE_SOURCE_DIR}/source/Lib/Renderer/Primitives/DrawSubTriangle.hpp
    ${CMAKE_SOURCE_DIR}/source/Lib/Renderer/Primitives/DrawTriangle.hpp
    ${CMAKE_SOURCE_DIR}/source/Lib/Renderer/Primitives/FastBump.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Renderer/Primitives/P6/FastBumpEx.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Renderer/Primitives/FastBumpMath.hpp
    ${CMAKE_SOURCE_DIR}/source/Lib/Renderer/Primitives/FastBumpTable.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Renderer/Primitives/GouraudT.hpp
    ${CMAKE_SOURCE_DIR}/source/Lib/Renderer/RenderCache.hpp
    ${CMAKE_SOURCE_DIR}/source/Lib/Renderer/RenderCacheHelp.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Renderer/RenderCachePriv.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Renderer/Primitives/P6/ScanlineAsmMacros.hpp
    ${CMAKE_SOURCE_DIR}/source/Lib/Renderer/ScreenRenderAuxD3D.hpp
    ${CMAKE_SOURCE_DIR}/source/Lib/Renderer/ScreenRenderAuxD3DBatch.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Renderer/screenrenderdwi.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Renderer/Primitives/Walk.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Renderer/Primitives/P6/WalkEx.hpp
)

list(APPEND ScreenRenderDWI_Src
    ${CMAKE_SOURCE_DIR}/Source/Lib/Renderer/Primitives/DrawSubTriangle.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Renderer/Primitives/DrawTriangle.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Renderer/Primitives/DrawTriangleEx.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Renderer/Primitives/FastBump.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Renderer/Primitives/FastBumpMath.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Renderer/Primitives/FastBumpTable.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Renderer/RenderCache.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Renderer/RenderCacheHelp.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Renderer/RenderCachePriv.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Renderer/ScreenRenderAuxD3D.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Renderer/ScreenRenderAuxD3DBatch.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Renderer/ScreenRenderDWI.cpp
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

add_library(${PROJECT_NAME} STATIC ${ScreenRenderDWI_Inc} ${ScreenRenderDWI_Src} )

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Lib/Render)
