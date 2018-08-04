#include "TwclPacket.h"
#include "TwclHandler.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TWCL PACKET
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool TwclPacket::build(String s)	{
	created = false;
	if(s.charAt(0) == TwclHandler().a_vec)	{
		if(s.charAt(s.length()-1) == TwclHandler().f_vec)	{
			String s2[6];
			uint8_t j = 0;
			s2[j] = "";
			for(uint16_t i = 1; i < s.length()-1; i++)	{
				if(s.charAt(i) == TwclHandler().delim_vec)	{
					j++;
					if(j > 5)	{
						return false;
					}
					s2[j] = "";
				}else	{
					s2[j] += s.charAt(i);
				}
			}

			if(j == 5)	{
				for(j = 0; j < 5; j++)	{
					if(s2[j].length() == 2)	{
						bcdd1[j] = (s2[j].charAt(0) << 8) + s2[j].charAt(1);
					}else if(s2[j].length() == 1)	{
						bcdd1[j] = s2[j].charAt(0);
					}else	{
						return false;
					}
				}
				e = s2[5];
				created = true;
				return true;
			}
		}
	}
	return false;
}

String TwclPacket::toJSON()	{
	String s = "{\"B\":";
	s += bcdd1[0];
	s += ",";
	s += "\"C\":";
	s += bcdd1[1];
	s += ",";
	s += "\"D\":";
	s += bcdd1[2];
	s += ",";
	s += "\"D1\":";
	s += bcdd1[3];
	s += ",";
	s += "\"H\":";
	s += bcdd1[4];
	s += ",";
	s += "\"E\":\"";
	s += e;
	s += "\"";
	s += "}";

	return s;
}

// String TwclPacket::toCSV()	{
// 	String s = "B,C,D,D1,H,E\r\n";
// 	s += bcdd1[0];
// 	s += ",";
// 	s += bcdd1[1];
// 	s += ",";
// 	s += bcdd1[2];
// 	s += ",";
// 	s += bcdd1[3];
// 	s += ",";
// 	s += bcdd1[4];
// 	s += ",";
// 	s += e;

// 	return s;
// }

void TwclPacket::log(HardwareSerial *serial)	{
	serial->print("- - - - - - - - - - - - - - -\r\n");
	serial->print("B: 0x"); 	serial->println(bcdd1[0], HEX);
	serial->print("C: 0x");		serial->println(bcdd1[1], HEX);
	serial->print("D: 0x");		serial->println(bcdd1[2], HEX);
	serial->print("D1: 0x");	serial->println(bcdd1[3], HEX);
	serial->print("H: 0x");		serial->println(bcdd1[4], HEX);
	serial->print("E: ");		serial->println(e);
	serial->print("- - - - - - - - - - - - - - -\r\n");
}

/*void TwclPacket::log(Monitor *monitor)	{
	monitor->print("- - - - - - - - - - - - - - -\r\n");
	monitor->print("B: 0x");
	monitor->println(bcdd1[0], HEX);
	monitor->print("C: 0x");
	monitor->println(bcdd1[1], HEX);
	monitor->print("D: 0x");
	monitor->println(bcdd1[2], HEX);
	monitor->print("D1: 0x");
	monitor->println(bcdd1[3], HEX);
	monitor->print("E: ");
	monitor->println(e);
	monitor->print("- - - - - - - - - - - - - - -\r\n");
}*/

void TwclPacket::logDec(HardwareSerial *serial)	{
	serial->print("- - - - - - - - - - - - - - -\r\n");
	serial->print("B: ");		serial->println(bcdd1[0]);
	serial->print("C: ");		serial->println(bcdd1[1]);
	serial->print("D: ");		serial->println(bcdd1[2]);
	serial->print("D1: ");		serial->println(bcdd1[3]);
	serial->print("H: ");		serial->println(bcdd1[4]);
	serial->print("E: ");		serial->println(e);
	serial->print("- - - - - - - - - - - - - - -\r\n");
}

bool TwclPacket::isCreated()	{
	return created;
}

uint16_t TwclPacket::getB()	{return bcdd1[0];}
void TwclPacket::setB(uint16_t B)	{bcdd1[0] = B;}

uint16_t TwclPacket::getC()	{return bcdd1[1];}
void TwclPacket::setC(uint16_t C)	{bcdd1[1] = C;}

uint16_t TwclPacket::getD()	{return bcdd1[2];}
void TwclPacket::setD(uint16_t D)	{bcdd1[2] = D;}

uint16_t TwclPacket::getD1()	{return bcdd1[3];}
uint16_t TwclPacket::getD1WithOffs()	{return (bcdd1[3] - d1_dec_offs);}
void TwclPacket::setD1(uint16_t D1)	{bcdd1[3] = D1;}

uint16_t TwclPacket::getH()	{return bcdd1[4];}
void TwclPacket::setH(uint16_t H)	{bcdd1[4] = H;}

String TwclPacket::getE()	{return e;}
void TwclPacket::setE(String E)	{e = E;}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
