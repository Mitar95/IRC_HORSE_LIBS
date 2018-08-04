// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// File: TwclHandler.h
// Description: Handler Class for twcl communication packets.
// Date: 13/06/2018
// Author: Branislav Vajagic
// Company: IRC - Interactive Robotics Center
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifndef TwclHandler_h
#define TwclHandler_h

#include "Arduino.h"
// Object/Entity that defines one twcl packet.
#include "TwclPacket.h"

class TwclHandler	{
	public:
		TwclHandler();
		TwclHandler(uint8_t thisMcuId);
		// @param hardwareSerial - pointer to HardwareSerial UART Object.
		// @param baudRate - at which speed will port be opened.
		// @param busyInPin - pin for checking if twcl bus is busy.
		// @param busyOutPin - pin for setting twcl bus to busy.
		void init(HardwareSerial *hardwareSerial, unsigned long baudRate,
				uint8_t busyInPin, uint8_t busyOutPin);
		void init(HardwareSerial *hardwareSerial, unsigned long baudRate);

		// All params are described in table in readme.md
		bool send(uint16_t b, uint16_t d, uint16_t d1, uint8_t e);
		bool send(uint16_t b, uint16_t d, uint16_t d1, uint16_t e);
		bool send(uint16_t b, uint16_t d, uint16_t d1, uint32_t e);
		bool send(uint16_t b, uint16_t d, uint16_t d1, int e);
		bool send(uint16_t b, uint16_t d, uint16_t d1, float e);
		bool send(uint16_t b, uint16_t d, uint16_t d1, char e);
		bool send(uint16_t b, uint16_t d, uint16_t d1, char *e);
		bool send(uint16_t b, uint16_t d, uint16_t d1, String e);

		// Every time this method is called, it will check serial buffer, empty it
		// and if the full packet is received, it will return true.
		bool listen();

		uint8_t getMyId();

		// Return twcl packet object created from data in buffer.
		// It is recommended to call this function only after listen()
		// returns true. 
		TwclPacket getPacket();
		String getBuffer();

		// B bytes
		#define LLHCU 		0x21
		#define CLU 		0x22
		#define BLDCCU 		0x23
		#define STEPCU 		0x24
		#define UICU 		0x25
		#define BROADCAST	0x26

		// C bytes
		#define UINT8		0x64
		#define UINT16		0x65
		#define UINT32		0x66
		#define INTEGER		0x67
		#define FLOAT		0x68
		#define CHAR		0x69
		#define CHAR_ARRAY	0x6A
		#define STRING		0x6B

		// D bytes
		#define GENERIC		0x25
		#define SYSTEM_BOOT	0x26
		#define BOOT		0x27
		#define ERROR		0x28
		#define PING		0x2A
		#define SWITCH		0x2B
		#define HEATER		0x2C
		#define TEMPERATURE	0x2D
		#define MOTOR		0x2E
		#define FAN			0x2F
		#define SYSTEM_STOP	0x30

		// D1 byte
		#define DS3		0x25
		#define DS4		0x26
		#define MS1		0x27
		#define MS2		0x28
		#define AS1		0x29
		#define AS2		0x2A
		#define AS3		0x2B
		#define HS1		0x2C
		#define HS2		0x2D
		#define HS6		0x2E
		#define HS7		0x2F
		#define HS8		0x30
		#define HS9		0x31
		#define HS10	0x32
		#define HS11	0x33
		#define HS12	0x34
		#define HS13	0x35

		#define DS1	0x25
		#define DS2	0x26
		#define HS3	0x27
		#define HS4	0x28
		#define HS5	0x29

		#define DHT1	0x25
		#define MHT1	0x26
		#define AHT1	0x27
		#define HHT1	0x28

		#define DM1	0x25
		#define MM1	0x26

		#define DFN1		0x25
		#define MFN1		0x26
		#define AFN1		0x27
		#define HFN1		0x28
		#define HFN2		0x29
		#define HFN3		0x2A

		// D = GENERIC
		#define NEW_TREATMENT	0x27
		#define MINUTE_LEFT		0x28
		
		// D = SYSTEM_STOP
		#define STOP_OK			0x30
		#define STOP_USER		0x31

		const uint8_t a_vec = 0x1F; // Start byte
		const uint8_t f_vec = 0x17; // End byte
		const uint8_t delim_vec = 0x1E; // Deliminator byte
		#define c_vec_size 8
		#define d_vec_size 6
		const uint16_t c_vec[c_vec_size] = {0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B};
		const uint16_t d_vec[d_vec_size] = {0x25, 0x26, 0x27, 0x28, 0x29, 0x2A};

	private:
		String buffer;
		char rollingBufffer;
		bool packetStart, isMaster;
		uint8_t bip, bop, myID;
		HardwareSerial *serial;
		TwclPacket packet;

		void startSend(uint16_t *b, uint16_t c, uint16_t *d, uint16_t *d1);
		void endSend();
};
#endif
