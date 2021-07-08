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

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-write-strings -Wno-deprecated \
    -Wall -Wextra -pedantic -fdiagnostics-color")

# if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 --coverage -fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -std=c++17")
# endif()

if(PLATFORM_LINUX)
    include(cmake/linux.cmake)
else()
    include(cmake/mac.cmake)
endif()


# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/Users/annotelldaniel/work/repos/dvs/cpp/externals/wxwidgets/lib/wx/include/osx_cocoa-unicode-static-3.1 -I/Users/annotelldaniel/work/repos/dvs/cpp/externals/wxwidgets/include -D_FILE_OFFSET_BITS=64 -D__WXMAC__ -D__WXOSX__ -D__WXOSX_COCOA__")

include_directories(${REPO_DIR}/cpp/)


# Googletest
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/externals/googletest/build/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/externals/googletest/googletest/include)

# Nlohmann
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/externals/nlohmann/include)

# ****** wxWidgets ******
find_package(wxWidgets COMPONENTS core base gl adv)
include(${wxWidgets_USE_FILE})

# include_directories(${WX_CUSTOM_INC_PATH})
# link_directories(${WX_CUSTOM_LIB_PATH})


set(GTEST_LIB_FILES libgtest.a
                    libgtest_main.a
                    pthread)

# ****** OpenGL ******
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})

# Adding subdirectories
add_subdirectory(applications/test_app)
add_subdirectory(dvs)
add_subdirectory(main_application)
