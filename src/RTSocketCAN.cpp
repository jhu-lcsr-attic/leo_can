/*

  Author(s): Simon Leonard
  Created on: Dec 02 2009

  (C) Copyright 2009 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/

#include <cstring>
#include <cstdio>

#include <leo_can/RTSocketCAN.h>

#ifdef __XENO__ // Only compile if xenomai

using namespace leo_can;

RTSocketCAN::RTSocketCAN(
    const std::string& device_name, 
    CANBus::Rate rate,
    CANBus::Loopback loopback ) : 
  CANBus( rate, loopback ),
  device_name_( device_name ),
  canfd_(-1),
  n_filters_( 0 )
{
  // Check if the device name is empty
  if( device_name_.empty() ) {
    std::cerr << "No device name." << std::endl;
  }
}

RTSocketCAN::~RTSocketCAN() { 
}

CANBus::Errno RTSocketCAN::Open()
{
  int errno;

  ifreq ifr;

  // create a CAN socket
  canfd_ = rt_dev_socket( PF_CAN, SOCK_RAW, CAN_RAW );
  if( canfd_ < 0 ) {
    std::cerr << "Couldn't create a CAN socket." << std::endl;
    return CANBus::EFAILURE;
  }

  if( loopback_ == CANBus::LOOPBACK_ON ) {
    errno = rt_dev_setsockopt(
        canfd_, 
        SOL_CAN_RAW, 
        CAN_RAW_LOOPBACK,
        &loopback_, 
        sizeof(loopback_) );

    if( errno != 0) {
      std::cerr << "Couldn't set loopback mode for " << device_name_
        << ". Error code was: " << errno
        << std::endl;
      return CANBus::EFAILURE;
    }
  }

  // Get CAN interface index by name
  strncpy(ifr.ifr_name, device_name_.data(), IFNAMSIZ);
  errno = rt_dev_ioctl( canfd_, SIOCGIFINDEX, &ifr );
  if( errno != 0 ){
    std::cerr << "Couldn't get the CAN interface index by name."
      << "Error code was: " << errno
      << std::endl;
    return CANBus::EFAILURE;
  }

  // Bind the socket to the local address
  memset(&addr_, 0, sizeof(addr_));    // clear the address structure
  addr_.can_ifindex = ifr.ifr_ifindex; // ifr_ifindex was set from SIOCGIFINDEX
  addr_.can_family = AF_CAN;           // Address Family CAN

  errno = rt_dev_bind(
      canfd_, 
      (struct sockaddr*)&addr_, 
      sizeof(struct sockaddr_can) );
  if( errno != 0 ) {
    std::cerr << "Couldn't bind the socket. Error code was: " 
      << errno << std::endl;
    return EFAILURE;
  }

  // set the baud rate
  can_baudrate_t* can_baudrate = (can_baudrate_t*)&ifr.ifr_ifru;
  *can_baudrate = rate_;
  if( rt_dev_ioctl( canfd_, SIOCSCANBAUDRATE, &ifr ) ){
    std::cerr << "Couldn't set the rate." << std::endl;
    return EFAILURE;
  }

  // Set the mode 
  CAN_MODE* mode = (CAN_MODE*)&ifr.ifr_ifru;
  *mode = CAN_MODE_START;
  if( rt_dev_ioctl(canfd_, SIOCSCANMODE, &ifr) ){
    std::cerr << "Couldn't set the operation mode." << std::endl;
    return EFAILURE;
  }

  nanosecs_rel_t timeout = 0;
  if (rt_dev_ioctl(canfd_, RTCAN_RTIOC_SND_TIMEOUT, &timeout) ){
    perror("RTSocketCAN::open: Couldn't set the send timeout: ");
    return EFAILURE;
  }

  timeout = 100000000;
  if( rt_dev_ioctl(canfd_, RTCAN_RTIOC_RCV_TIMEOUT, &timeout) ){
    perror("RTSocketCAN::open: Couldn't set the recv timeout: ");
    return EFAILURE;
  }

  return ESUCCESS;
}

CANBus::Errno RTSocketCAN::Close(){
  // close the socket
  if(canfd_ >=0 ) {
    int rt_error = 0;
    if( rt_error = rt_dev_setsockopt(
          canfd_, 
          SOL_CAN_RAW, 
          CAN_RAW_FILTER, 
          filters_, 
          0 ) )
    {
      std::cerr << "Couldn't clear the socket filters on socket descriptor "<<canfd_<<": " <<strerror(-rt_error)<< std::endl;
    }

    if( rt_dev_close( canfd_ ) ){
      std::cerr << "Couldn't close the socket." << std::endl;
      return EFAILURE;
    }

    canfd_ = -1;
  }

  return ESUCCESS;
}

// Send a can frame
// Note that block is useless for Socket CAN
CANBus::Errno RTSocketCAN::Send(
    const CANBusFrame& canframe, 
    CANBus::Flags )
{

  // copy the data in to a RTSocket CAN frame
  // can_frame_t is defined in xenomai/include/rtdm/rtcan.h
  can_frame_t frame;
  frame.can_id = (can_id_t)canframe.GetID();
  frame.can_dlc = (uint8_t)canframe.GetLength();  

  const uint8_t* data = (const uint8_t*)canframe.GetData();
  std::copy(data, data+CANBusFrame::DATA_FIELD_LEN, frame.data);

  // send the frame
  int error = rt_dev_send(
      canfd_, 
      (void*)&frame, 
      sizeof(can_frame_t), 
      0 );

  if( error < 0 ){
    std::cerr << "Failed to send CAN frame " << error << std::endl;
    return EFAILURE;
  }

  return ESUCCESS;
}

// Receive a CAN frame
CANBus::Errno RTSocketCAN::Recv(CANBusFrame& canframe, CANBus::Flags){

  struct can_frame frame;            // the RT Socket CAN frame
  memset(&frame, 0, sizeof(frame));  // clear the frame

  int error =  rt_dev_recv(
      canfd_, 
      (void*)&frame, 
      sizeof(can_frame_t), 
      0 );

  if( error < 0 ){
    std::cerr << "Failed to receive the CAN frame. Error: " << strerror(-error) 
      << std::endl;
    return EFAILURE;
  }

  // create a CANBusFrame
  canframe = CANBusFrame( frame.can_id, frame.data, frame.can_dlc );
  //std::cout << "RECV: " << std::endl << canframe << std::endl;

  return ESUCCESS;
}

CANBus::Errno RTSocketCAN::AddFilter( const CANBus::Filter& filter ) {

  //std::cout << "RTSocketCAN::AddFilter" << std::endl;

  if( n_filters_ < RTSocketCAN::MAX_NUM_FILTERS ){

    /*
    // Avoid duplicates
    for( size_t i=0; i<n_filters_; i++ ){
    if( filters[i].can_mask == filter.mask && filters[i].can_id == filter.id )
    { return CANBus::ESUCCESS; }
    }
    */

    filters_[n_filters_].can_mask = filter.mask_;
    filters_[n_filters_].can_id   = filter.id_;
    n_filters_++;

    // Set the filter to the socket
    int rt_error = 0;
    if( rt_error = rt_dev_setsockopt(
          canfd_, 
          SOL_CAN_RAW, 
          CAN_RAW_FILTER, 
          filters_, 
          n_filters_*sizeof(struct can_filter) ) )
    {
      std::cerr << "Couldn't set the socket filter ("<<filter.mask_<<", "<<filter.id_<<") on socket descriptor "<<canfd_<<": " <<strerror(-rt_error)<< std::endl;
      return CANBus::EFAILURE;
    }

    return CANBus::ESUCCESS;
  } else {
    std::cerr << "Reached maximum number of filters." << std::endl;
    return CANBus::EFAILURE;
  }

  return CANBus::EFAILURE;

}

#endif // ifdef __XENO__
