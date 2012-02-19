
#ifndef _LEOCAN_CANBUSFRAME_H
#define _LEOCAN_CANBUSFRAME_H

#include <vector>
#include <iostream>
#include <iomanip>

namespace leoCAN {
  //! CAN frame implementation
  /**
    This defines a basic CAN frame. It does not support the extended frame 
    format. The frame is composed of an identifier, 8 bytes of data and a data 
    length. A CAN frame contain an 11 bits identifier, 0-8 bytes of data and a 
    data length code (0-8).
    This class does not support the extended frame format.
    */
  class CANBusFrame {

  public:

    //! The id of a CAN frame
    /**
      A CAN ID has 11 bits so we use 16 bits to represent a CAN id of which 
      only the 11 LSB are used.
      */
    typedef unsigned short id_t; // FIXME: formerly ID

    typedef unsigned short mask_t; // FIXME: formerly Mask

    //! The data type
    typedef unsigned char data_t; // FIXME formerly Data

    static const unsigned char DATA_FIELD_LEN = 8;

    //! The field of data
    typedef data_t data_field_t[DATA_FIELD_LEN]; // FIXME formerly DataField

    //! The data length type
    typedef unsigned char data_len_t; // FIXME: formerly DataLength

  protected:

    //! The ID of the node origin
    id_t id_;

    //! The message (8 bytes)
    data_field_t data_;

    //! The lenght of the message in bytes
    data_len_t data_len_;

  public:


    //! Default constructor
    /**
      Set the IDs to "0" the message length to "0" and all the message bytes to
      "0"
      */
    CANBusFrame();

    //! Overloaded constructor
    /**
      Set the id of the CAN frame and the message
      */
    CANBusFrame( id_t canid, data_field_t data, data_len_t data_len );

    CANBusFrame( id_t canid, const std::vector<CANBusFrame::data_t>& data );

    //! Return the identifier of the frame
    inline id_t GetID() const { return id_; }

    //! Return the length in bytes of the data
    inline data_len_t GetLength() const { return data_len_; }

    //! Return a pointer to the data
    inline data_t* GetData() { return  &(data_[0]); }

    //! Return a const pointer to the data
    inline const data_t* GetData() const { return  &(data_[0]); }

    //! Output the can frame
    /**
      Prints out the frame's identifier, data length and data
      \param os[in] An ouptput stream
      \param cf[in] A CAN frame
      */
    friend std::ostream& operator<<(std::ostream& os, const CANBusFrame& frame){
      os <<"ID: 0x" 
        << std::hex << std::setfill('0') << std::setw(4) 
        << (int)frame.GetID() << std::endl
        << "Length: " << (int)frame.GetLength() << std::endl 
        << "Data: ";
      for( data_len_t i=0; i<frame.GetLength(); i++ ) {
        os << "0x" << std::hex << std::setfill('0')
          << std::setw(2) 
          << (int)(frame.data_[i]) << " ";
      }
      os << std::dec;
      return os;
    }

  }; // Frame
}

#endif // ifndef _LEOCAN_CANBUSFRAME_H
