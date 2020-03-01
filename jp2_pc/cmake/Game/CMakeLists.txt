cmake_minimum_required(VERSION 3.5)
project(Game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND Game_Inc
    ${CMAKE_SOURCE_DIR}/Source/Lib/Trigger/Action.hpp
    ${CMAKE_SOURCE_DIR}/Source/Game/DesignDaemon/BloodSplat.hpp
    ${CMAKE_SOURCE_DIR}/source/Game/DesignDaemon/Daemon.hpp
    ${CMAKE_SOURCE_DIR}/Source/Game/DesignDaemon/DaemonScript.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Trigger/ExpressionEvaluate.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Trigger/GameActions.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/EntityDBase/GameLoop.hpp
    ${CMAKE_SOURCE_DIR}/source/Game/DesignDaemon/Gun.hpp
    ${CMAKE_SOURCE_DIR}/source/Game/DesignDaemon/HitSpang.hpp
    ${CMAKE_SOURCE_DIR}/Source/Game/DesignDaemon/Player.hpp
    ${CMAKE_SOURCE_DIR}/Source/Game/DesignDaemon/PlayerSettings.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/EntityDBase/TextOverlay.hpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Trigger/Trigger.hpp
)

list(APPEND Game_Src
    ${CMAKE_SOURCE_DIR}/Source/Lib/File/Image.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Trigger/Action.cpp
    ${CMAKE_SOURCE_DIR}/Source/Game/DesignDaemon/BloodSplat.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Trigger/BooleanTrigger.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Trigger/CollisionTrigger.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Trigger/CreatureTrigger.cpp
    ${CMAKE_SOURCE_DIR}/Source/Game/DesignDaemon/Daemon.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Trigger/ExpressionEvaluate.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Trigger/GameActions.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/EntityDBase/GameLoop.cpp
    ${CMAKE_SOURCE_DIR}/Source/Game/DesignDaemon/Gun.cpp
    ${CMAKE_SOURCE_DIR}/Source/Game/DesignDaemon/HitSpang.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Trigger/LocationTrigger.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Trigger/MagnetTrigger.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Trigger/MoreMassTrigger.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Trigger/ObjectTrigger.cpp
    ${CMAKE_SOURCE_DIR}/Source/Game/DesignDaemon/Player.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Trigger/SequenceTrigger.cpp
    ${CMAKE_SOURCE_DIR}/Source/Game/DesignDaemon/Socket.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Trigger/StartTrigger.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/EntityDBase/TextOverlay.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Trigger/TimerTrigger.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Trigger/TriggerBase.cpp
    ${CMAKE_SOURCE_DIR}/Source/Lib/Trigger/VariableTrigger.cpp
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

add_library(${PROJECT_NAME} STATIC ${Game_Inc} ${Game_Src} )

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Lib/Game)
