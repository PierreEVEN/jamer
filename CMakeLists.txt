cmake_minimum_required(VERSION 3.8)

project(Jamer)


set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT jamerheadless)

# Set project constants
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}) # Project dir
set(BINARIES_DIR ${PROJECT_ROOT}/bin) # Binaries dir

# Setup target
function(construct_target TARGET_NAME TARGET_PATH SOURCES)
    set_target_properties(${TARGET_NAME} PROPERTIES 
        ARCHIVE_OUTPUT_DIRECTORY "${BINARIES_DIR}/Lib"
        RUNTIME_OUTPUT_DIRECTORY "${BINARIES_DIR}/Bin"
        LIBRARY_OUTPUT_DIRECTORY "${BINARIES_DIR}/Bin"
        VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_ROOT}")
    target_include_directories(${TARGET_NAME} PUBLIC ${TARGET_PATH})
    target_compile_features(${TARGET_NAME} PUBLIC cxx_std_17)
    set_target_properties(${TARGET_NAME} PROPERTIES CXX_EXTENSIONS OFF)
    IF (WIN32)
    ELSE ()
        target_link_libraries(${TARGET_NAME} -ldl -lpthread stdc++fs)
    ENDIF()
endfunction()

# windows only

# asio
set(ASIOPATH ./third_party/asiosdk/)
file(GLOB_RECURSE SOURCES ${ASIOPATH}/common/asio.cpp ${ASIOPATH}/host/pc/asiolist.cpp ${ASIOPATH}/host/asiodrivers.cpp)
add_library(asiosdk ./third_party/asiosdk/host/ ${SOURCES})
target_include_directories(asiosdk PUBLIC ./third_party/asiosdk/host/pc)
target_include_directories(asiosdk PUBLIC ./third_party/asiosdk/host)
target_include_directories(asiosdk PUBLIC ./third_party/asiosdk/common)
construct_target(asiosdk ./third_party/asiosdk/common ${SOURCES})



# main lib
file(GLOB_RECURSE SOURCES ./src/jamerlib/*)
add_library(jamerlib ${SOURCES})
construct_target(jamerlib ./src/jamerlib/public ${SOURCES})
target_link_libraries(jamerlib asiosdk)

# ui
file(GLOB_RECURSE SOURCES ./src/jamerui/*)
add_executable(jamerui ${SOURCES})
target_link_libraries(jamerui jamerlib)
construct_target(jamerui ./src/jamerui/public ${SOURCES})
target_link_libraries(jamerui asiosdk)

# headless
file(GLOB_RECURSE SOURCES ./src/jamerheadless/*)
add_executable(jamerheadless ${SOURCES})
target_link_libraries(jamerheadless jamerlib)
construct_target(jamerheadless ./src/jamerheadless/public ${SOURCES})
target_link_libraries(jamerheadless asiosdk)