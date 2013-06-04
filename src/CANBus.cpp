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

#include <leo_can/CANBus.h>

using namespace leo_can;

// default constructor of a can device
CANBus::CANBus(
    CANBus::Rate rate,
    CANBus::Loopback loopback ) :
  rate_(rate),
  loopback_( loopback )
{ 
}

CANBus::~CANBus() { }
