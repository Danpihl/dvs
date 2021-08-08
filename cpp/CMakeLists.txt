cmake_minimum_required(VERSION 3.14.3 FATAL_ERROR)

project(dvs CXX)

set(REPO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

if(UNIX AND NOT APPLE)
    set(PLATFORM_LINUX TRUE)
    add_compile_definitions(PLATFORM_LINUX_M)
elseif(APPLE)
    set(PLATFORM_APPLE TRUE)
    add_compile_definitions(PLATFORM_APPLE_M)
elseif(WIN32)
    set(PLATFORM_WINDOWS TRUE)
    add_compile_definitions(PLATFORM_WINDOWS_M)
else()
    message(FATAL_ERROR "Unknown platform!" )
endif()

if(PLATFORM_LINUX)
    include(cmake/linux.cmake)
else()
    include(cmake/macos.cmake)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-write-strings -Wno-deprecated \
    -Wall -Wextra -pedantic -fdiagnostics-color")

# if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 --coverage -fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -std=c++17")
# endif()

include_directories(${REPO_DIR}/cpp/)


# Googletest
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/externals/googletest/build/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/externals/googletest/googletest/include)

# Nlohmann
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/externals/nlohmann/include)

# ****** wxWidgets ******
# find_package(wxWidgets COMPONENTS core base gl adv)

set(WX_CUSTOM_INC_PATH ${REPO_DIR}/cpp/externals/wxwidgets/include)
set(WX_CUSTOM_LIB_PATH ${REPO_DIR}/cpp/externals/wxwidgets/lib)

include_directories(${WX_CUSTOM_INC_PATH})
link_directories(${WX_CUSTOM_LIB_PATH})

# set(wxWidgets_LIBRARIES -lwx_osx_cocoau_core-3.1
#                         -lwx_baseu-3.1
#                         -lwx_osx_cocoau_gl-3.1
#                         -lwx_osx_cocoau_adv-3.1)

set(GTEST_LIB_FILES libgtest.a
                    libgtest_main.a
                    pthread)

# ****** OpenGL ******
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})

# Adding subdirectories
add_subdirectory(applications/test_app)
add_subdirectory(applications/test_app_c)
add_subdirectory(test)
add_subdirectory(main_application)
