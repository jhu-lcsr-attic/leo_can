cmake_minimum_required(VERSION 2.4.6)

if(False)
  include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)

  # Set the build type.  Options are:
  #  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
  #  Debug          : w/ debug symbols, w/o optimization
  #  Release        : w/o debug symbols, w/ optimization
  #  RelWithDebInfo : w/ debug symbols, w/ optimization
  #  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
  #set(ROS_BUILD_TYPE RelWithDebInfo)

  rosbuild_init()

  #set the default path for built executables to the "bin" directory
  set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
  #set the default path for built libraries to the "lib" directory
  set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

  #uncomment if you have defined messages
  #rosbuild_genmsg()
  #uncomment if you have defined services
  #rosbuild_gensrv()

  #common commands for building c++ executables and libraries
  #rosbuild_add_library(barrett_direct src/osaGroup.cpp src/osaPuck.cpp src/osaWAM.cpp src/osaBH8_280.cpp)
  #target_link_libraries(${PROJECT_NAME} another_library)
  #rosbuild_add_boost_directories()
  #rosbuild_link_boost(${PROJECT_NAME} thread)
  #rosbuild_add_executable(example examples/example.cpp)
  #target_link_libraries(example ${PROJECT_NAME})
endif()

project(leoCAN)

set( INCLUDE_DIR "include/leoCAN" )
set( SRC_DIR "src" )

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

set( leoCAN_INCLUDE_DIRS "${leoCAN_SOURCE_DIR}/include" )
set( leoCAN_LIBRARY_DIRS )
set( leoCAN_LIBRARIES )
set( leoCAN_DEFINITIONS )

set( HEADER_FILES 
  ${INCLUDE_DIR}/CANBus.h 
  ${INCLUDE_DIR}/CANBusFrame.h )

set( SOURCE_FILES 
  ${SRC_DIR}/CANBus.cpp
  ${SRC_DIR}/CANBusFrame.cpp )

# Add path to cmake modules in this package
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${leoCAN_SOURCE_DIR}/cmake")

# Check for RT Socket CAN 
find_package( RTSocketCAN )
if( RTSocketCAN_FOUND )
  message("-- Building leoCAN with RTSocketCAN support.") 
  set( HEADER_FILES ${HEADER_FILES} 
    ${INCLUDE_DIR}/RTSocketCAN.h )

  set( SOURCE_FILES ${SOURCE_FILES} 
    ${SRC_DIR}/RTSocketCAN.cpp )

  list(APPEND leoCAN_INCLUDE_DIRS ${RTDM_INCLUDE_DIRS} )
  list(APPEND leoCAN_LIBRARY_DIRS ${RTDM_LIBRARY_DIRS} )
  list(APPEND leoCAN_LIBRARIES ${RTDM_LIBRARIES} )
  list(APPEND leoCAN_DEFINITIONS ${RTDM_DEFINITIONS} )

else()
  message("-- Not building leoCAN with RTSocketCAN support.") 
endif()

# Add include directories
include_directories( ${leoCAN_INCLUDE_DIRS} )

# Add definitions
add_definitions( ${leoCAN_DEFINITIONS} )

# Create target for library
add_library( leoCAN SHARED ${HEADER_FILES} ${SOURCE_FILES} )
target_link_libraries( leoCAN ${leoCAN_LIBRARIES} )

# Create configuration file
list(APPEND leoCAN_INCLUDE_DIRS )
list(APPEND leoCAN_LIBRARY_DIRS ${LIBRARY_OUTPUT_PATH} )
list(APPEND leoCAN_LIBRARIES leoCAN )

configure_file(
  "${leoCAN_SOURCE_DIR}/cmake/leoCANConfig.cmake.in"
  "${leoCAN_BINARY_DIR}/share/cmake/leoCAN/leoCANConfig.cmake"
  @ONLY@ )
