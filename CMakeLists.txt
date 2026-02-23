# Reference:
# https://github.com/pybind/scikit_build_example/blob/master/CMakeLists.txt

# Require CMake 3.15+ (matching scikit-build-core) Use new versions of all
# policies up to CMake 3.27
cmake_minimum_required(VERSION 3.15...3.27)

# Scikit-build-core sets these values for you, or you can just hard-code the
# name and version.
project(
  ${SKBUILD_PROJECT_NAME}
  VERSION ${SKBUILD_PROJECT_VERSION}
  LANGUAGES CXX)

# Find the module development requirements (requires FindPython from 3.17 or
# scikit-build-core's built-in backport)
include(FetchContent)
find_package(Python REQUIRED COMPONENTS Interpreter Development.Module)
find_package(pybind11 CONFIG REQUIRED)
find_package(Eigen3 3.3 QUIET NO_MODULE)
if (NOT Eigen3_FOUND)
    message(STATUS "Eigen3 not found, fetching 5.0.0 via FetchContent...")
    FetchContent_Declare(
            eigen_default
            GIT_REPOSITORY "https://gitlab.com/libeigen/eigen.git"
            GIT_TAG        "5.0.0"
    )
    FetchContent_MakeAvailable(eigen_default)
endif()

if (NOT TARGET Eigen3::Eigen)
    message(FATAL_ERROR "Failed to fetch Eigen dependency.")
endif()

# compiler flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

# include path
include_directories(
    src
)

# Add a library using FindPython's tooling (pybind11 also provides a helper like
# this)
python_add_library(_core MODULE python/src/polypartition_py.cpp 
    src/polypartition.cpp
    src/hm_eigen.cpp
    WITH_SOABI)

# link libraries
target_link_libraries(_core PRIVATE pybind11::headers)
target_link_libraries(_core PUBLIC Eigen3::Eigen)

# This is passing in the version as a define just as an example
target_compile_definitions(_core PRIVATE VERSION_INFO=${PROJECT_VERSION})

# The install directory is the output (wheel) directory
install(TARGETS _core DESTINATION polypartition)