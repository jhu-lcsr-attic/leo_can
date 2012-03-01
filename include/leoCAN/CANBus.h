

#ifndef __LEOCAN_CANBUS_H
#define __LEOCAN_CANBUS_H

#include <leoCAN/CANBusFrame.h>

namespace leoCAN {
  //! Generic CAN bus
  /**
    The only thing this class does is to define the interface that must be
    implemented by a CAN bus. Any implementation of a CAN device depends on an 
    actual device driver.
    */
  class CANBus {

  public:

    //! The different CAN rates 
    enum Rate {
      RATE_150 =150000, 
      RATE_300 =300000, 
      RATE_1000=1000000
    };

    //! Error codes
    enum Errno {
      ESUCCESS, 
      EFAILURE
    };

    enum Flags {
      MSG_NOFLAG   = 0x00,
      MSG_CONFIRM  = 0x01,   // ask for a confirmation 
      MSG_DONTWAIT = 0x02    // enables non-blocking operation
    };

    enum Loopback {
      LOOPBACK_ON, 
      LOOPBACK_OFF
    };

  public:

    // CAN Filter
    /**
      CAN filters are used by the kernel to filter frames that are not aimed for
      a specific node id.
      */
    class Filter {
    public:
      CANBusFrame::mask_t mask_;
      CANBusFrame::id_t id_;

      Filter( CANBusFrame::mask_t mask, CANBusFrame::id_t id ) :
        mask_( mask ),
        id_( id )
      { }

    };

  protected:

    //! The rate of the device
    CANBus::Rate rate_;
    CANBus::Loopback loopback_;

  public:

    //! Default constructor
    /**
      Initialize a CAN device with the given rate
      \param rate The CAN rate
      */
    CANBus(
        CANBus::Rate rate, 
        CANBus::Loopback loopback = CANBus::LOOPBACK_OFF );

    //! Default destructor
    virtual ~CANBus();

    //! Open the CAN device
    virtual CANBus::Errno Open() = 0;

    //! Close the CAN device
    virtual CANBus::Errno Close() = 0;

    //! Send a CAN frame on the bus
    /**
      Call this method to send a CAN frame on the bus. The method can be blocking
      \param frame[in] The CAN frame to send on the bus
      \param block Block the device until the operation is completed
      */
    virtual CANBus::Errno Send(
        const CANBusFrame& frame, 
        CANBus::Flags flags=CANBus::MSG_NOFLAG ) = 0;

    //! Receive a CAN frame
    /**
      Call this method to receive a CAN frame. The method can be blocking.
      \param frame[out] The CAN frame received from the bus
      \param block Block the device until a CAN frame is received
      */
    virtual CANBus::Errno Recv(
        CANBusFrame& frame, 
        CANBus::Flags flags=CANBus::MSG_NOFLAG ) = 0;


    //! Add a CAN filter to the device
    /**
      Call this method to add a CAN filter to the device. This is OS and 
      dependant as it typically interacts with the driver to screen CAN frames.
      */
    virtual CANBus::Errno AddFilter( const CANBus::Filter& filter ) = 0;

  };
}

#endif // ifndef __LEOCAN_CANBUS_H
