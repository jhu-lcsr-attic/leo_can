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

set( INCLUDE_DIR "include/leoCAN" )
set( SRC_DIR "src" )

set( HEADER_FILES 
  ${INCLUDE_DIR}/osaCANBus.h 
  ${INCLUDE_DIR}/osaCANBusFrame.h )

set( SOURCE_FILES 
  ${SRC_DIR}/CANBus.cpp
  ${SRC_DIR}/CANBusFrame.cpp )

# Add path to cmake modules in this package
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${leoCAN_SOURCE_DIR}/cmake")

# Check for RT Socket CAN 
find_package( RTSocketCAN )
if( RTSocketCANFound )
  message("-- Building leoCAN with RTSocketCAN support.") 
  set( HEADER_FILES ${HEADER_FILES} 
    ${INCLUDE_DIR}/RTSocketCAN.h )

  set( SOURCE_FILES ${SOURCE_FILES} 
    ${SRC_DIR}/RTSocketCAN.cpp )

  set( leoCAN_LIBRARIES_PATH ${RTDM_LIBRARIES_PATH} )
  set( leoCAN_LIBRARIES ${RTDM_LIBRARIES} )
  set( leoCAN_DEFINITIONS ${RTDM_DEFINITIONS} )

  include_directories( ${RTDM_INCLUDE_DIR} )
else()
  message("-- Not building leoCAN with RTSocketCAN support.") 
endif()

# Create target for library
add_library( leoCAN ${HEADER_FILES} ${SOURCE_FILES} )
target_link_libraries( leoCAN ${leoCAN_LIBRARIES} )
