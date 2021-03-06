/*=Plus=header=begin======================================================
  Program: Plus
  Copyright (c) Laboratory for Percutaneous Surgery. All rights reserved.
  See License.txt for details.
=========================================================Plus=header=end*/


#ifndef _RAWSERIALLINE_H_
#define _RAWSERIALLINE_H_

#ifdef _WIN32
#include <windows.h>
#else
#define INVALID_HANDLE_VALUE (-1)
#endif

#include <string>

/*!
\class SerialLine
\brief Class for reading and writing data through the serial (RS-232) port

The class currently only works on Windows. If serial communication is needed on other platforms then
sample code can be found in src\Utilities\ndicapi\ndicapi_serial.* or at https://github.com/wjwwood/serial.

\ingroup PlusLibDataCollection
*/

class SerialLine  
{
public:
  typedef unsigned long DWORD;
  typedef unsigned int UINT;
  typedef unsigned char BYTE;

#ifdef _WIN32
  // HANDLE is defined for Windows already
#else
  typedef int HANDLE;
#endif
  
  SerialLine();
  virtual ~SerialLine();

  /*! Open serial port */ 
  bool Open();

  /*! Close serial port */ 
  void Close();

  /*! Write data to the serial port. Returns the number of writes actually written. */
  int Write(const BYTE* data, int numberOfBytesToWrite);

  /*! Write a single byte to the serial port. Returns true if successful. */
  bool Write(const BYTE data);

  /*! Read data from the serial port. Returns the number of writes actually read. */
  int Read(BYTE *data, int maxNumberOfBytesToRead);

  /*! Read a single byte from the serial port. Returns true if successful. */ 
  bool Read(BYTE &data);

  /*! Set the serial port name e.g. COM1 */ 
  void SetPortName(const std::string &name) { m_PortName=name; };

  /*! Get the serial port name */ 
  std::string GetPortName() const { return m_PortName; };

  /*! Set the serial port speed */ 
  void SetSerialPortSpeed(DWORD speed) { m_SerialPortSpeed=speed; };

  /*! Set the serial port max reply time */ 
  void SetMaxReplyTime(int maxreply) { m_MaxReplyTime=maxreply; };

  /*! Get the serial port max reply time */ 
  int GetMaxReplyTime() const { return m_MaxReplyTime; };

  /*! Check the handle alive status */ 
  bool IsHandleAlive() const { return (m_CommHandle != INVALID_HANDLE_VALUE); };

  /*! Check the handle alive status */ 
  unsigned int GetNumberOfBytesAvailableForReading() const;

  /*! Clears the device's error flag to enable additional input and output (I/O) operations  */ 
  DWORD ClearError();

private:
  HANDLE m_CommHandle;
  std::string m_PortName;
  DWORD m_SerialPortSpeed;
  int m_MaxReplyTime;
  int UpdateSerialBuffer();
};

#endif 
