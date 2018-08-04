# TWCL Communication Handler #

> One Packet Format: ABGCGDGD1GHGEnF

---

## Packet Description ##

NAME | DESCRIPTION                      | VALUE                      | REQUIRED
---- | -------------------------------  | -------------------------- | --------
A    | Packet start byte.               | 0x1F                       | TRUE
B    | Packet direction.                | 0x21-0x26.                 | TRUE
C    | Packet message format            | 0x64-0x6C-Not yet defined. | TRUE
D    | Packet message type              | 0x25-0x26-Not yet defined. | TRUE
D1   | Packet message type additional.  | 0x25-0xFFFE                | TRUE
En   | Packet message.                  | Max. number: 1024 bytes    | TRUE
F    | Packet end.                      | 0x17                       | TRUE
G    | Packet information deliminator.  | 0x1E                       | TRUE
H    | Address of unit that is sending. | 0x21-0x25.                 | TRUE

## B byte ##
- 0x21 - LLHCU
- 0x22 - CLU
- 0x23 - BLDCCU
- 0x24 - STEPCU
- 0x25 - UICU
- 0x26 - BROADCAST

## C byte ##
- 0x64 - UINT8
- 0x65 - UINT16
- 0x66 - UINT32
- 0x67 - Integer
- 0x68 - Float
- 0x69 - Char
- 0x6A - Char Array
- 0x6B - String

## D bytes ##
- 0x25 - Generic
- 0x26 - System Booted
- 0x27 - Boot
- 0x28 - Error
- 0x2A - Ping
- 0x2B - Switch
- 0x2C - Heater
- 0x2D - Temperature
- 0x2E - Motor
- 0x2F - Fan

## D1 bytes ##
- If D = Temperature
	- 0x25 - DS3
	- 0x26 - DS4
	- 0x27 - MS1
	- 0x28 - MS2
	- 0x29 - AS1
	- 0x2A - AS2
	- 0x2B - AS3
	- 0x2C - HS1
	- 0x2D - HS2
	- 0x2E - HS6
	- 0x2F - HS7
	- 0x30 - HS8
	- 0x31 - HS9
	- 0x32 - HS10
	- 0x33 - HS11
	- 0x34 - HS12
	- 0x35 - HS13
- If D = Switch
	- 0x25 - DS1
	- 0x26 - DS2
	- 0x27 - HS3
	- 0x28 - HS4
	- 0x29 - HS5
- If D = Heater
	- 0x25 - DHT1
	- 0x26 - MHT1
	- 0x27 - AHT1
	- 0x28 - HHT1
- If D = Motor
	- 0x25 - DM1
	- 0x26 - MM1
- If D = Fan
	- 0x25 - DFN1
	- 0x26 - MFN1
	- 0x27 - AFN1
	- 0x28 - HFN1
	- 0x29 - HFN2
	- 0x2A - HFN3
- If D = Generic
	- 0x25 - DM1
	- 0x26 - DM2
	- 0x27 - _Not defined_

## H byte ##
- 0x21 - LLHCU
- 0x22 - CLU
- 0x23 - BLDCCU
- 0x24 - STEPCU
- 0x25 - UICU

**A, C, G, H and F bytes are set automatically.**

## Methods - TwclHandler##

METHOD                                                                                              | TYPE       | Description
----------------------------------------------------------------------------------------------------| ---------- | ------------------------------------------------
TwclHandler()                                                                                       | void       | Default constructor. Do not use!
TwclHandler(uint8_t thisMcuId)                                                                      | void       | Name/ID of MCU that is using this object.
init(HardwareSerial *hardwareSerial, unsigned long baudRate, uint8_t busyInPin, uint8_t busyOutPin) | void       | Initialize handler for slave instance.
init(HardwareSerial *hardwareSerial, unsigned long baudRate)                                        | void       | Initialize handler for master instance.
send(uint16_t b, uint16_t d, uint16_t d1, <Generic> e)                                              | bool       | Returns 1 if message e was send, returns 0 otherwise.
listen()                                                                                            | bool       | Returns 1 if packet is received, returns 0 otherwise.
getMyId()                                                                                           | uint8_t    | Returns value set in constructor.
getPacket()                                                                                         | TwclPacket | Returns last packet received.
getBuffer()                                                                                         | String     | Returns last packet received formated as string.

## Methods - TwclPacket##

METHOD                         | TYPE     | DESCRIPTION
------------------------------ | -------- | ----------------------------------------------------------
build(String s)                | bool     | Creates TwclPacket Object from twcl packet formated string.
isCreated()                    | bool     | Returns 1 if build was successful.
getB()                         | uint16_t | Returns B byte of packet. 
getC()                         | uint16_t | Returns C byte of packet. 
getD()                         | uint16_t | Returns D byte of packet. 
getD1()                        | uint16_t | Returns D1 byte of packet. 
getE()                         | String   | Returns message of the packet. 
getH()                         | uint16_t | Returns H byte of packet. 
setB(uint16_t value)           | void     | Set B byte of packet.
setC(uint16_t value)           | void     | Set C byte of packet.
setD(uint16_t value)           | void     | Set D byte of packet.
setD1(uint16_t value)          | void     | Set D1 byte of packet.
setE(String value)             | void     | Set message of packet.
setH(uint16_t value)           | void     | Set H byte of packet.
toJSON()                       | String   | Returns object as JSON formated string.
log(HardwareSerial *serial)    | void     | Prints contents of the packet to selected serial port.
logDec(HardwareSerial *serial) | void     | Prints contents of the packet to selected serial port.

# How To Use #

## Example 1 ##
		#include "avr/io.h"
		#include "HardwareSerial.h"
		#include "TwclHandler.h"

		int main(void)	{
			TwclHandler twclHandler(CLU); // Create instance of twcl handler object
			TwclPacket twclPacket;	// Create instance of twcl packet object

			// Initialize handler
			twclHandler.init(&Serial1, 115200);

			while(1)	{
				// Listen for incoming packets
				if(twclHandler.listen())	{
					// Packet is in buffer
					twclPacket = twclHandler.getPacket();

					// Check if packet was created successfully
					if(twclPacket.isCreated())	{
						// Do something with data
						// .......
					}
				}

				if(<some statement>)	{
					// Send temperature data of sensor 2 to LLHCU
					if(twclHandler.send(LLHCU, TEMPERATURE, 0x26, 24.67))	{
						<some statement> = false;
					}
				}
			}
		}

## Example 2
		#include "Arduino.h"
		#include "TwclHandler.h"

		void setup(){}
		void loop()	{
			TwclHandler twclHandler(CLU); // Create instance of twcl handler object
			TwclPacket twclPacket;	// Create instance of twcl packet object

			// Initialize handler
			twclHandler.init(&Serial1, 115200);

			while(1)	{
				// Listen for incoming packets
				if(twclHandler.listen())	{
					// Packet is in buffer
					twclPacket = twclHandler.getPacket();

					// Check if packet was created successfully
					if(twclPacket.isCreated())	{
						// Do something with data
						// .......
					}
				}

				if(<some statement>)	{
					// Send temperature data of sensor 2 to LLHCU
					if(twclHandler.send(LLHCU, TEMPERATURE, 0x26, 24.67))	{
						<some statement> = false;
					}
				}
			}
		}

# Send Generic Data - Error Object

		#include "Arduino.h"
		#include "TwclHandler.h"

		void setup(){}
		void loop()	{
			TwclHandler twclHandler(LLHCU); // Create instance of twcl handler object
			TwclPacket twclPacket;	// Create instance of twcl packet object

			// Initialize handler
			twclHandler.init(&Serial1, 115200);

			while(1)	{
				// Listen for incoming packets
				if(twclHandler.listen())	{
					// Packet is in buffer
					twclPacket = twclHandler.getPacket();

					// Check if packet was created successfully
					if(twclPacket.isCreated())	{
						// Do something with data
						// .......
					}
				}

				if(<some statement>)	{
					// Send error message to CLU
					Error error(ERROR_WARNING, "Some warning message.")
					if(twclHandler.send(CLU, ERROR, 0x25, error.toString()))	{
						<some statement> = false;
					}
				}
			}
		}