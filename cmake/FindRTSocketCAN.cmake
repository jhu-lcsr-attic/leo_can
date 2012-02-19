
set( RTSocketCAN_FOUND FALSE )

# Find if RTSocketCAN is supported by the driver by searching /proc/rtcan/rtcan0/info.
# This ensures that at least one RTSocketCAN device has been detected.
find_file( RTCAN0_INFO "info" "/proc/rtcan/rtcan0" NO_DEFAULT_PATH )
if(${RTCAN0_INFO} STREQUAL "RTCAN0_INFO-NOTFOUND")
  message("-- WARNING: could not find rtcan device.")
endif()

# set the search paths
set( XENOMAI_SEARCH_PATH /usr/xenomai /usr/local/xenomai /usr/include/xenomai)
  
# find xeno-config.h
find_path( 
  XENOMAI_DIR
  NAMES include/xeno_config.h xeno_config.h
  PATHS ${XENOMAI_SEARCH_PATH} )

# Search for the rtdm.h
find_path( 
  RTDM_DIR
  NAMES include/rtdm/rtdm.h rtdm.h
  PATHS ${XENOMAI_SEARCH_PATH} )

# Search for the rtdm library
find_library( XENOMAI_LIBRARY_RTDM    rtdm    ${XENOMAI_DIR}/lib )
find_library( XENOMAI_LIBRARY_XENOMAI xenomai ${XENOMAI_DIR}/lib )

message("XENOMAI_LIBRARY_RTDM ${XENOMAI_LIBRARY_RTDM}")
message("XENOMAI_LIBRARY_XENOMAI ${XENOMAI_LIBRARY_XENOMAI}")
  
if( RTDM_DIR AND 
    XENOMAI_LIBRARY_RTDM AND 
    XENOMAI_LIBRARY_XENOMAI )

  set( RTSocketCAN_FOUND TRUE)
  set( RTDM_INCLUDE_DIR ${RTDM_DIR}/include )
  set( RTDM_DEFINITIONS "-D_GNU_SOURCE -D_REENTRANT -D__XENO__" )
  set( RTDM_LIBRARIES ${XENOMAI_LIBRARY_RTDM} )

endif()

mark_as_advanced( 
  RTCAN0_INFO 
  XENOMAI_DIR
  RTDM_DIR 
  XENOMAI_LIBRARY_RTDM
  XENOMAI_LIBRARY_XENOMAI )
