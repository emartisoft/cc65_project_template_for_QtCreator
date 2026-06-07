cmake_minimum_required(VERSION 3.20)
project(%{ProjectName} C)

# ============================================================================
# IMPORTANT: Set your VICE C64 emulator path here or via CMake GUI
# ============================================================================
# Option 1 (Recommended): Set via CMake GUI or command line:
#   cmake -DEMULATOR_PATH=/Applications/vice-arm64-gtk3-3.9/x64sc.app/Contents/MacOS/x64sc ..
#
# Option 2: Edit this line directly with your emulator path:
set(EMULATOR_PATH "" CACHE FILEPATH "Path to VICE C64 emulator executable")
#
# NOTE: If you're updating an existing project:
#   1. Delete the entire 'build' directory
#   2. Re-run cmake configuration
#   3. Then the Run button will work correctly with the emulator
# ============================================================================

set(TARGET %{ProjectName}.prg)
set(SOURCES %{CFileName})

# Find cc65 compiler with environment variable support
find_program(CC65 cl65 REQUIRED HINTS $ENV{CC65_DIR})

# Detect and configure cc65 include directories
include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/include)
if(WIN32)
    file(GLOB CC65_INCLUDE_DIRS 
        "C:/cc65/include"
        "$ENV{CC65_DIR}/include"
        "$ENV{PROGRAMFILES}/cc65/include"
    )
elseif(APPLE)
    file(GLOB CC65_INCLUDE_DIRS
        "/opt/homebrew/share/cc65/include"
        "/usr/local/share/cc65/include"
        "$ENV{HOME}/.homebrew/share/cc65/include"
    )
else()
    file(GLOB CC65_INCLUDE_DIRS
        "/usr/share/cc65/include"
        "/usr/local/share/cc65/include"
        "$ENV{HOME}/.local/share/cc65/include"
    )
endif()

list(FILTER CC65_INCLUDE_DIRS EXCLUDE REGEX ".*-NOTFOUND$")
if(CC65_INCLUDE_DIRS)
    list(GET CC65_INCLUDE_DIRS 0 CC65_INCLUDE_DIR)
    include_directories(SYSTEM ${CC65_INCLUDE_DIR})
endif()
add_compile_definitions(__C64__ __fastcall__=)

add_library(${PROJECT_NAME} OBJECT EXCLUDE_FROM_ALL ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    EXCLUDE_FROM_DEFAULT_BUILD TRUE
    LINKER_LANGUAGE C
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}
    COMMAND ${CC65} -t c64 -o ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}
            ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCES}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCES}
    COMMENT "Compiling for C64: ${TARGET}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    VERBATIM
)

add_custom_target(build ALL DEPENDS ${TARGET})

if(NOT EMULATOR_PATH OR EMULATOR_PATH STREQUAL "")
    add_custom_target(c64run
        COMMAND ${CMAKE_COMMAND} -E echo "ERROR: EMULATOR_PATH is not set!"
        COMMAND ${CMAKE_COMMAND} -E echo "Please set it before running:"
        COMMAND ${CMAKE_COMMAND} -E echo "  cmake -DEMULATOR_PATH=/path/to/x64sc .."
        COMMAND ${CMAKE_COMMAND} -E echo "Or use CMake GUI to configure the emulator path."
        COMMAND false
        DEPENDS build
        COMMENT "VICE emulator path not configured"
    )
else()
    if(WIN32)
        add_custom_target(c64run
            COMMAND cmd /c start "" "${EMULATOR_PATH}" "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}"
            DEPENDS build
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Launching C64 emulator: ${EMULATOR_PATH}"
        )
    elseif(APPLE)
        add_custom_target(c64run
            COMMAND "${EMULATOR_PATH}" "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}"
            DEPENDS build
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Launching C64 emulator on macOS: ${EMULATOR_PATH} ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}"
        )
    elseif(LINUX)
        add_custom_target(c64run
            COMMAND "${EMULATOR_PATH}" "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}"
            DEPENDS build
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Launching C64 emulator on Linux: ${EMULATOR_PATH}"
        )
    else()
        add_custom_target(c64run
            COMMAND ${CMAKE_COMMAND} -E echo "C64 emulator launch not supported on this platform"
            DEPENDS build
            COMMENT "C64 emulator launch not supported"
        )
    endif()
endif()

set_target_properties(c64run PROPERTIES
    FOLDER "qtc_runnable"
)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/clean-all.cmake
"file(GLOB _rm_files
    \"${CMAKE_CURRENT_BINARY_DIR}/*.o\"
    \"${CMAKE_CURRENT_BINARY_DIR}/*.prg\"
)
file(REMOVE \${_rm_files})
")
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/clean-all.cmake
)
