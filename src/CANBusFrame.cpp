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

#include <algorithm>

#include <leo_can/CANBusFrame.h>

using namespace leo_can;

// Default initialization of a CAN frame
CANBusFrame::CANBusFrame() : 
  id_(0),     // default ID
  data_len_(0)  // no data
{ 
  // Initialize the packet to zero
  std::fill(data_,data_+CANBusFrame::DATA_FIELD_LEN,0);
}

CANBusFrame::CANBusFrame(
    CANBusFrame::id_t id, 
    CANBusFrame::data_field_t data,
    CANBusFrame::data_len_t data_len ) :
  id_(0),
  data_len_(0)
{
  // Initialize the packet to zero before starting
  std::fill(data_,data_+CANBusFrame::DATA_FIELD_LEN,0);

  // A can ID has 11 bits. Ensure that only 11 bits are used
  if( (~0x07FF) & id ) {
    std::cerr << __FILE__ << __LINE__ << std::endl
			<< ": Illegal CAN id: " << id 
			<< std::endl;
  } else {
    // Raise an error if more than 8 bytes are given
    if( data_len > DATA_FIELD_LEN) {
      std::cerr << __FILE__ << __LINE__ << std::endl
			  << ": Illegal message length: " << data_len
			  << std::endl;
    } else {
      id_ = id;                                   // Copy the CAN ID
      data_len_ = data_len;                       // Copy the data length
      std::copy(data,data+DATA_FIELD_LEN,data_);  // Copy the data
    }
  }
}

CANBusFrame::CANBusFrame(
    CANBusFrame::id_t id, 
    const std::vector<CANBusFrame::data_t>& data) :
  id_(0),
  data_len_(0)
{
  // Initialize the packet to zero before starting
  std::fill(data_,data_+CANBusFrame::DATA_FIELD_LEN,0);

  // A can ID has 11 bits. Ensure that only 11 bits are used
  if( (~0x07FF) & id ) {
    std::cerr << __FILE__ << __LINE__ << std::endl
			<< ": Illegal CAN id: " << id 
			<< std::endl;
  } else {
    // Raise an error if more than 8 bytes are given
    if(data.size()  > DATA_FIELD_LEN) {
      std::cerr << __FILE__ << __LINE__ << std::endl
			  << ": Illegal message length: " << data.size()
			  << std::endl;
    } else {
      id_ = id;                                 // Copy the CAN ID
      data_len_ = data.size();                     // Copy the data length
      std::copy(data.begin(),data.end(),data_); // Copy the data
    }
  }
}

