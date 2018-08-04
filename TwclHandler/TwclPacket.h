// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// File: TwclPacket.h
// Description: Object of type twcl packet
// Date: 13/06/2018
// Author: Branislav Vajagic
// Company: IRC - Interactive Robotics Center
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifndef TwclPacket_h
#define TwclPacket_h

#include "HardwareSerial.h"
#include "WString.h"

class TwclPacket	{
	public:
		// Will fill object variables and return true on success.
		bool build(String s);

		String toJSON();

		// Returns B byte
		uint16_t getB();
		void setB(uint16_t B);
		// Returns C byte
		uint16_t getC();
		void setC(uint16_t C);
		// Returns D byte
		uint16_t getD();
		void setD(uint16_t D);
		// Returns D1 bytes
		uint16_t getD1();
		void setD1(uint16_t D1);
		// Returns D1 with zero offset
		uint16_t getD1WithOffs();
		// Returns E bytes
		String getE();
		void setE(String E);
		// Returns H byte
		uint16_t getH();
		void setH(uint16_t H);

		// If call to build() method was successful or not
		bool isCreated();

		// Prints content of object on given UART port. Values are
		// printed in HEX format
		void log(HardwareSerial *serial);
		// void log(Monitor *monitor);
		// Prints content of object on given UART port. Values are
		// printed in DEC format
		void logDec(HardwareSerial *serial);

	private:
		#define d1_dec_offs 37

		String e;
		uint16_t bcdd1[5];
		bool created = false;
};
#endif
