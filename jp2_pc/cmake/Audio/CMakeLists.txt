cmake_minimum_required(VERSION 3.5)
project(Audio)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND Audio_Inc
    ${CMAKE_SOURCE_DIR}/Source/Lib/Audio/Audio.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Audio/AudioADPCM.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Audio/AudioDaemon.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Audio/AudioLoader.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Audio/AudioPCM.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Audio/AudioVOICE.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Audio/SoundDefs.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Audio/SoundTypes.hpp
)

list(APPEND Audio_Src
    ${CMAKE_SOURCE_DIR}/Source/Lib/Audio/Audio.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Audio/AudioADPCM.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Audio/AudioDaemon.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Audio/AudioLoad.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Audio/Material.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Audio/Sample.cpp
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

add_library(${PROJECT_NAME} STATIC ${Audio_Inc} ${Audio_Src} )

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Lib/Game)
