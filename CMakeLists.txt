project(dob)
cmake_minimum_required(VERSION 2.8.5)


###############################################################################
# Using Clang is awesome
###############################################################################
option(USE_CLANG "build application with clang" OFF)
if(USE_CLANG)
  SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")
  SET (CMAKE_CXX_FLAGS                "-Wall -O2 -std=c++11")
else(USE_CLANG)
  SET (CMAKE_CXX_COMPILER             "/usr/bin/g++")
  SET (CMAKE_CXX_FLAGS                "-Wall -O2 -std=c++11")
endif(USE_CLANG)



###############################################################################
# Headers
###############################################################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Inotify)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/FileStateDatabase)



###############################################################################
# Boost
###############################################################################
find_package(Boost 1.48.0 COMPONENTS system filesystem thread program_options REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
set(LIBS ${LIBS} ${Boost_LIBRARIES})



###############################################################################
# SQLite 3
###############################################################################
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lsqlite3 ")



###############################################################################
# NEON
###############################################################################
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lneon ")



###############################################################################
# pThreads
###############################################################################
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread ")



###############################################################################
# SigC
###############################################################################
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lsigc-2.0 ")



###############################################################################
# Build executables
###############################################################################
file(GLOB dob_SOURCES src/*)
add_executable(dob ${dob_SOURCES})

# emulate "make new"
add_custom_target(new 
  COMMAND make clean 
  COMMAND make)



###############################################################################
# Linking
###############################################################################
target_link_libraries(dob ${LIBS})



###############################################################################
# Doxygen (make doc)
###############################################################################
find_package(Doxygen)
if(DOXYGEN_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxygen.conf ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif(DOXYGEN_FOUND)
