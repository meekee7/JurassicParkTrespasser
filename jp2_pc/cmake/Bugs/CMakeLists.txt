cmake_minimum_required(VERSION 3.5)
project(Bugs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND Bugs_Src
    ${CMAKE_SOURCE_DIR}/Source/Bugs/BogusNestedType.cpp
    ${CMAKE_SOURCE_DIR}/Source/Bugs/BogusTemplateDefinition.cpp
    ${CMAKE_SOURCE_DIR}/Source/Bugs/BogusTemplateType.cpp
    ${CMAKE_SOURCE_DIR}/Source/Bugs/Complaints.cpp
    ${CMAKE_SOURCE_DIR}/Source/Bugs/DynamicCastConst.cpp
    ${CMAKE_SOURCE_DIR}/Source/Bugs/FriendOperator.cpp
    ${CMAKE_SOURCE_DIR}/Source/Bugs/FriendSpecialise.cpp
    ${CMAKE_SOURCE_DIR}/Source/Bugs/NoConvert.cpp
    ${CMAKE_SOURCE_DIR}/Source/Bugs/PartialSpecialise.cpp
    ${CMAKE_SOURCE_DIR}/Source/Bugs/TemplateAmbiguity.cpp
    ${CMAKE_SOURCE_DIR}/Source/Bugs/TemplateDefaultError.cpp
    ${CMAKE_SOURCE_DIR}/Source/Bugs/TemplateDefaultRedef.cpp
    ${CMAKE_SOURCE_DIR}/Source/Bugs/TemplateNested.cpp
)

add_executable(${PROJECT_NAME} ${Bugs_Src} )

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Lib/Other)
