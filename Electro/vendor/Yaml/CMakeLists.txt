# Copyright(c) 2021 - Electro Team - All rights reserved

include(${CMAKE_SOURCE_DIR}/scripts/CMakeUtils.cmake)

file(GLOB_RECURSE SOURCE_FILES "src/*.h" "src/*.cpp" "include/*.h")
set(INCLUDE_DIRS include)

add_library(Yaml ${SOURCE_FILES})
include_directories(${INCLUDE_DIRS})

GroupSourcesByFolder(Yaml)
set_target_properties (Yaml PROPERTIES FOLDER Dependencies)

message(STATUS "Configuring Yaml... done!")