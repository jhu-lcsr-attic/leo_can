
@[if DEVELSPACE]@
list(APPEND CMAKE_MODULE_PATH "@(CMAKE_CURRENT_SOURCE_DIR)/cmake")
@[end if]@

find_package( RTSocketCAN )

if( RTSocketCAN_FOUND )
  message("-- Building leo_can with RTSocketCAN support.") 

  list(APPEND leo_can_INCLUDE_DIRS ${RTDM_INCLUDE_DIRS} )
  list(APPEND leo_can_LIBRARY_DIRS ${RTDM_LIBRARY_DIRS} )
  list(APPEND leo_can_DEFINITIONS ${RTDM_DEFINITIONS} )
  list(APPEND leo_can_LIBRARIES ${RTDM_LIBRARIES} )

else()
  message("-- Not building leo_can with RTSocketCAN support.") 
endif()
