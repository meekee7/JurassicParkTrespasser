cmake_minimum_required(VERSION 3.5)
project(Physics)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND Physics_Inc
    ${CMAKE_SOURCE_DIR}/source/Lib/Loader/ASyncLoader.hpp
    ${CMAKE_SOURCE_DIR}/source/Lib/Physics/Arms.h
    ${CMAKE_SOURCE_DIR}/Source/Lib/Physics/BioModel.h
    ${CMAKE_SOURCE_DIR}/Source/Lib/Physics/BioStructure.h
    ${CMAKE_SOURCE_DIR}/Source/Lib/Physics/dino_biped.h
    ${CMAKE_SOURCE_DIR}/Source/Lib/Physics/futil.h
    ${CMAKE_SOURCE_DIR}/Source/Lib/Physics/Human.h
    ${CMAKE_SOURCE_DIR}/source/Lib/Physics/InfoBox.hpp
    ${CMAKE_SOURCE_DIR}/source/Lib/Physics/InfoCompound.hpp
    ${CMAKE_SOURCE_DIR}/source/Lib/Physics/InfoPlayer.hpp
    ${CMAKE_SOURCE_DIR}/source/Lib/Physics/InfoSkeleton.hpp
    ${CMAKE_SOURCE_DIR}/source/Lib/Physics/Magnet.hpp
    ${CMAKE_SOURCE_DIR}/source/Lib/Physics/Pelvis.h
    ${CMAKE_SOURCE_DIR}/Source/Lib/Physics/pelvis_def.h
    ${CMAKE_SOURCE_DIR}/Source/Lib/Physics/PhysicsHelp.hpp
    ${CMAKE_SOURCE_DIR}/source/Lib/Physics/PhysicsImport.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/EntityDBase/PhysicsInfo.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Physics/PhysicsStats.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Physics/PhysicsSystem.hpp
    ${CMAKE_SOURCE_DIR}/source/Lib/Physics/WaterDisturbance.hpp
    ${CMAKE_SOURCE_DIR}/source/Lib/Physics/Waves.hpp
    ${CMAKE_SOURCE_DIR}/source/Lib/Physics/Xob_bc.hpp
)

list(APPEND Physics_Src
    ${CMAKE_SOURCE_DIR}/Source/Lib/Loader/AsyncLoader.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Physics/Arms.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Physics/BioModel.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Physics/dino_biped.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Physics/dino_quad.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Physics/foot.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Physics/futil.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Physics/Human.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Physics/InfoBox.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Physics/InfoCompound.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Physics/InfoPlayer.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Physics/InfoSkeleton.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Physics/Magnet.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Physics/Pelvis.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Physics/PhysicsImport.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Physics/PhysicsInfo.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Physics/PhysicsSystem.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Physics/WaterDisturbance.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Physics/waves.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Physics/Xob_bc.cpp
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

add_library(${PROJECT_NAME} STATIC ${Physics_Inc} ${Physics_Src} )

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Lib/Game)
