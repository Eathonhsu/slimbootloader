/** @file
Implementation for console input library

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Library/SerialPortLib.h>
#include <Library/ConsoleInLib.h>

/**
  Poll a console to see if there is any data waiting to be read.

  @retval FALSE                There is no data waiting to be read from the console.
  @retval TRUE                 Data is waiting to be read from the console.

**/
BOOLEAN
EFIAPI
ConsolePoll (
  VOID
  )
{

  if ((PcdGet32 (PcdConsoleInDeviceMask) & ConsoleInSerialPort) != 0) {
    if (SerialPortPoll ()) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Read data from a console device into a buffer.

  @param  Buffer           Pointer to the data buffer to store the data read from the console device.
  @param  NumberOfBytes    Number of bytes to read from the console device.

  @retval 0                NumberOfBytes is 0.
  @retval >0               The number of bytes read from the console device.
                           If this value is less than NumberOfBytes, then the read operation failed.
**/
UINTN
EFIAPI
ConsoleRead (
  OUT UINT8 *Buffer,
  IN UINTN  NumberOfBytes
  )
{
  UINTN    Count;
  UINTN    ReadCount;

  Count = 0;
  while (Count < NumberOfBytes) {
    if ((PcdGet32 (PcdConsoleInDeviceMask) & ConsoleInSerialPort) != 0) {
      if (SerialPortPoll ()) {
        ReadCount = SerialPortRead (Buffer, 1);
        Buffer += ReadCount;
        Count  += ReadCount;
      }
    }
  }

  return NumberOfBytes;
}

