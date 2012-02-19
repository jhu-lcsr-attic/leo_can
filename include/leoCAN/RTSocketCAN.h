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


#ifndef _LEOCAN_RTSOCKETCAN_H
#define _LEOCAN_RTSOCKETCAN_H

#ifdef __XENO__ // Only compile if xenomai

#include <rtdm/rtcan.h>            // Defines for the RT CAN socket

#include <leoCAN/CANBus.h>       // the CAN base class

//! A Real Time Socket CAN device
/**
   Most harware in Linux use the /dev interface. Typically, your device (usb,
   serial port, ...) will be represented by a file in /dev. CAN hardware is no
   exception except for the socket CAN module. SocketCAN implements a CAN stack
   and extend the BSD sockets just like you have a ethernet stack and sockets. 
   RT SocketCAN is the "real time" version for the Xenomai framework.
   To use RTSocketCAN, you must have a Xenomai patched Linux kernel with 
   the RT CAN module enabled
*/

namespace leoCAN {

  class RTSocketCAN : public CANBus {

   private:
    
    //! The name of the CAN device (rtcan0, rtcan1, ...)
    std::string device_name_;
    
    //! The file descriptor of the socket
    int canfd_;

    //! The socket address for the CAN address family
    sockaddr_can addr_;

    //! CAN filters
    /**
       The default maximum number of CAN filter in Xenomai is 16. Howerver, this
       limit can be increased when configuring the kernel.
     */
    static const size_t MAX_NUM_FILTERS = 32;
    can_filter filters_[MAX_NUM_FILTERS];
    size_t n_filters_;

  public:

    //! Constructor
    /**
       Initialize the device name and the rate of the CAN device
       \param devicename The name of the device (rtcan0, rtcan1, ...)
       \param rate The CAN rate (RATE_150, RATE_300, RATE_1000)
    */
    RTSocketCAN(
        const std::string& devicename, 
        CANBus::Rate rate,
        CANBus::Loopback loopback = CANBus::LOOPBACK_OFF );
    
    //! Destructor
    ~RTSocketCAN();

    //! Open and configure the CAN socket
    /**
       Creates and configure a RT CAN socket.
    */
    CANBus::Errno Open();

    //! Close the CAN socket
    CANBus::Errno Close();

    //! Send a CAN frame on the bus
    /**
       Call this method to send a CAN frame on the bus.
       \param frame[in] The CAN frame to send on the bus
       \param flags Block the device until the operation is completed. This
                    parameter is irrelevant for RTSocketCAN.
    */
    CANBus::Errno Send(
        const CANBusFrame& frame, 
        CANBus::Flags flags = CANBus::MSG_NOFLAG );

    //! Receive a CAN frame
    /**
       Call this method to receive a CAN frame.
       \param frame[out] The CAN frame received from the bus
       \param flags Block the device until a CAN frame is received. This
                    parameter is irrelevant for RTSocketCAN.
    */
    CANBus::Errno Recv(
        CANBusFrame& frame, 
        CANBus::Flags flags = CANBus::MSG_NOFLAG );
    
    //! Add a filter to the RTSocketCAN device
    CANBus::Errno AddFilter( const CANBus::Filter& filter );

  };
}

#endif // ifdef __XENO__

#endif // ifndef _LEOCAN_RTSOCKETCAN_H
