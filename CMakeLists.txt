# http://ros.org/doc/groovy/api/catkin/html/user_guide/supposed.html
cmake_minimum_required(VERSION 2.8.3)
project(leo_can)

# Load catkin and all dependencies required for this package
# TODO: remove all from COMPONENTS that are not catkin packages.
find_package(catkin REQUIRED )

# Set the include and source paths
set( INCLUDE_DIR "include/leo_can" )
set( SRC_DIR "src" )

# Set the executable and library output paths
set( leo_can_INCLUDE_DIRS "${leo_can_SOURCE_DIR}/include" )
set( leo_can_LIBRARY_DIRS )
set( leo_can_LIBRARIES )
set( leo_can_DEFINITIONS )
set( leo_can_HEADER_FILES )
set( leo_can_SOURCE_FILES )

# Add path to cmake modules in this package
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${leo_can_SOURCE_DIR}/cmake")

# Check for RT Socket CAN 
find_package( RTSocketCAN )
if( RTSocketCAN_FOUND )
  message("-- Building leo_can with RTSocketCAN support.") 
  list(APPEND leo_can_HEADER_FILES
    ${INCLUDE_DIR}/RTSocketCAN.h )

  list(APPEND leo_can_SOURCE_FILES
    ${SRC_DIR}/RTSocketCAN.cpp )

  list(APPEND leo_can_INCLUDE_DIRS ${RTDM_INCLUDE_DIRS} )
  list(APPEND leo_can_LIBRARY_DIRS ${RTDM_LIBRARY_DIRS} )
  list(APPEND leo_can_DEFINITIONS ${RTDM_DEFINITIONS} )
  list(APPEND leo_can_LIBRARIES ${RTDM_LIBRARIES} )

else()
  message("-- Not building leo_can with RTSocketCAN support.") 
endif()

# Add include directories
include_directories( ${leo_can_INCLUDE_DIRS} )
# Add linker directories
link_directories( ${leo_can_LIBRARY_DIRS} )
# Add definitions
add_definitions( ${leo_can_DEFINITIONS} )

# Create leo_can library
list(APPEND leo_can_HEADER_FILES 
  ${INCLUDE_DIR}/CANBus.h 
  ${INCLUDE_DIR}/CANBusFrame.h )

list(APPEND leo_can_SOURCE_FILES 
  ${SRC_DIR}/CANBus.cpp
  ${SRC_DIR}/CANBusFrame.cpp )

add_library( leo_can SHARED ${leo_can_HEADER_FILES} ${leo_can_SOURCE_FILES} )
target_link_libraries( leo_can ${leo_can_LIBRARIES} )

# Create configuration file
list(APPEND leo_can_INCLUDE_DIRS )
list(APPEND leo_can_LIBRARY_DIRS ${LIBRARY_OUTPUT_PATH} )
list(APPEND leo_can_LIBRARIES leo_can )

# TODO: fill in what other packages will need to use this package
## DEPENDS: system dependencies of this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## INCLUDE_DIRS: 
## LIBRARIES: libraries you create in this project that dependent projects also need
catkin_package(
    DEPENDS  # TODO
    CATKIN_DEPENDS # TODO
    INCLUDE_DIRS # TODO include
    LIBRARIES # TODO
    CFG_EXTRAS leo_can-config.cmake.in
)

# Install the cmake config
install(FILES 
  cmake/FindRTSocketCAN.cmake
  cmake/FindBitCtrl.cmake
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/cmake
  USE_SOURCE_PERMISSIONS
)
