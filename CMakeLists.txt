cmake_minimum_required(VERSION 3.26)

# Forteaza static linking inainte de orice subproiect (SFML, ImGui-SFML)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries" FORCE)

# NOTE: update executable name in .github/workflows/cmake.yml:25 when changing executable name in this file
# for now, the project name is used as the executable name
set(MAIN_PROJECT_NAME "oop")
set(MAIN_EXECUTABLE_NAME "${MAIN_PROJECT_NAME}")


project(${MAIN_PROJECT_NAME})
# set(CMAKE_PROJECT_VERSION_MAJOR 0)
# set(CMAKE_PROJECT_VERSION_MINOR 0)
# set(CMAKE_PROJECT_VERSION_PATCH 1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/Options.cmake)
include(cmake/CompilerFlags.cmake)
include(cmake/CopyHelper.cmake)

###############################################################################

# external dependencies with FetchContent
include(FetchContent)

set(FETCHCONTENT_QUIET OFF CACHE INTERNAL "Override verbose output" FORCE)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE INTERNAL "Override update strategy" FORCE)

# Proiectul foloseste doar Graphics/Window/System din SFML.
# Dezactivam modulele nefolosite ca sa evitam dependinte native inutile in CI.
set(SFML_BUILD_AUDIO OFF CACHE BOOL "Build SFML audio module" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "Build SFML network module" FORCE)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "Build SFML examples" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "Build SFML documentation" FORCE)
set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "Build SFML test suite" FORCE)

# NOTE: Also update env vars used for caching in
#     - .github/actions/configure-cmake/action.yml
#     - .github/workflows/cmake.yml

# SFML 2.6.x
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        2.6.x
    GIT_SHALLOW    1
    GIT_PROGRESS   ON
    SYSTEM
)

# Dear ImGui
FetchContent_Declare(
    ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.91.1
    GIT_SHALLOW    1
    GIT_PROGRESS   ON
    SYSTEM
)

# ImGui-SFML (official binding)
FetchContent_Declare(
    ImGuiSFML
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
    GIT_TAG        2.6.x
    GIT_SHALLOW    1
    GIT_PROGRESS   ON
    SYSTEM
)

# Populeaza SFML, ImGui si ImGui-SFML manual
# SFML se adauga via add_subdirectory -> expune targets (sfml-graphics etc.) direct
# ImGui-SFML se compileaza manual (un singur .cpp) linkand la targets SFML
FetchContent_GetProperties(SFML)
if(NOT sfml_POPULATED)
    FetchContent_Populate(SFML)
    add_subdirectory(${sfml_SOURCE_DIR} ${sfml_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_GetProperties(ImGui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(ImGui)
endif()

FetchContent_GetProperties(ImGuiSFML)
if(NOT imguisfml_POPULATED)
    FetchContent_Populate(ImGuiSFML)
    # Fix compatibilitate C++23/MSVC: sf::U8String -> auto
    file(READ "${imguisfml_SOURCE_DIR}/imgui-SFML.cpp" IMGUI_SFML_SRC)
    string(REPLACE
        "std::basic_string<std::uint8_t> tmp = sf::Clipboard::getString().toUtf8();"
        "auto tmp = sf::Clipboard::getString().toUtf8();"
        IMGUI_SFML_SRC "${IMGUI_SFML_SRC}")
    file(WRITE "${imguisfml_SOURCE_DIR}/imgui-SFML.cpp" "${IMGUI_SFML_SRC}")
endif()

# Creeaza o biblioteca ImGui-SFML manual (ocoleste find_package din CMakeLists.txt propriu)
add_library(ImGui-SFML STATIC
    ${imguisfml_SOURCE_DIR}/imgui-SFML.cpp
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
)
target_include_directories(ImGui-SFML SYSTEM PUBLIC
    ${imgui_SOURCE_DIR}
    ${imguisfml_SOURCE_DIR}
)
target_link_libraries(ImGui-SFML PUBLIC
    sfml-graphics
    sfml-window
    sfml-system
)
target_compile_definitions(ImGui-SFML PUBLIC IMGUI_USER_CONFIG="${imguisfml_SOURCE_DIR}/imconfig-SFML.h")
# Suprima warning-urile din bibliotecile externe
if(MSVC)
    target_compile_options(ImGui-SFML PRIVATE /W0)
else()
    target_compile_options(ImGui-SFML PRIVATE -w)
endif()
# ImGui-SFML.cpp are un bug cu C++23/MSVC: sf::U8String nu e convertibil la std::basic_string<uint8_t>
# Fix: patch direct in sursa (un reinterpret al tipului)
target_compile_definitions(ImGui-SFML PRIVATE _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING)

###############################################################################

# external dependencies with find_package

# find_package(Threads REQUIRED)

###############################################################################

# NOTE: update executable name in .github/workflows/cmake.yml:25 when changing name here
add_executable(${MAIN_EXECUTABLE_NAME}
    main.cpp
)
target_sources(${MAIN_EXECUTABLE_NAME}
    PRIVATE
        src/Example.cpp
        src/Locatie.cpp
        src/Resursa.cpp
        src/EchipaInterventie.cpp
        src/Incident.cpp
        src/CentruComanda.cpp
)

# NOTE: Add all defined targets (e.g. executables, libraries, etc. )
# NOTE: RUN_SANITIZERS is optional, if it's not present it will default to true
set_compiler_flags(RUN_SANITIZERS ${USE_ASAN} TARGET_NAMES ${MAIN_EXECUTABLE_NAME})
# set_compiler_flags(TARGET_NAMES ${MAIN_EXECUTABLE_NAME} ${FOO} ${BAR})
# where ${FOO} and ${BAR} represent additional executables or libraries
# you want to compile with the set compiler flags

###############################################################################

# use SYSTEM so cppcheck and clang-tidy do not report warnings from these directories
target_include_directories(${MAIN_EXECUTABLE_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)
target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE
    ext
)

###############################################################################

# GUI executable (SFML + ImGui)
set(GUI_EXECUTABLE_NAME "oop_gui")

add_executable(${GUI_EXECUTABLE_NAME}
    main_gui.cpp
)
target_sources(${GUI_EXECUTABLE_NAME}
    PRIVATE
        src/Locatie.cpp
        src/Resursa.cpp
        src/EchipaInterventie.cpp
        src/Incident.cpp
        src/CentruComanda.cpp
)
target_include_directories(${GUI_EXECUTABLE_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)
target_include_directories(${GUI_EXECUTABLE_NAME} SYSTEM PRIVATE
    ext
)
target_link_libraries(${GUI_EXECUTABLE_NAME} PRIVATE
    ImGui-SFML
)
set_compiler_flags(RUN_SANITIZERS OFF TARGET_NAMES ${GUI_EXECUTABLE_NAME})

###############################################################################

# copy binaries to "bin" folder; these are uploaded as artifacts on each release
# DESTINATION_DIR is set as "bin" in cmake/Options.cmake:6
install(TARGETS ${MAIN_EXECUTABLE_NAME} DESTINATION ${DESTINATION_DIR})
if(APPLE)
    install(FILES launcher.command DESTINATION ${DESTINATION_DIR})
endif()

copy_files(FILES tastatura.txt DIRECTORY assets COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})
# copy_files(FILES tastatura.txt config.json DIRECTORY images sounds COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})
# copy_files(DIRECTORY images sounds COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})
