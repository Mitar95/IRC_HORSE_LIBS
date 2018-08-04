#include "TwclHandler.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TWCL HANDLER
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TwclHandler::TwclHandler(){};

TwclHandler::TwclHandler(uint8_t thisID)	{
	myID = thisID;
}

void TwclHandler::init(HardwareSerial *hardwareSerial, unsigned long baudRate,
		uint8_t busyInPin, uint8_t busyOutPin)	{
	isMaster = false;
	serial = hardwareSerial;
	serial->begin(baudRate);

	bip = busyInPin;
	bop = busyOutPin;

	pinMode(bip, INPUT);
	pinMode(bop, OUTPUT);
	digitalWrite(bop, 0);

	packetStart = false;
	rollingBufffer = 0x00;
}

void TwclHandler::init(HardwareSerial *hardwareSerial, unsigned long baudRate)	{
	isMaster = true;
	serial = hardwareSerial;
	serial->begin(baudRate);
	
	packetStart = false;
	rollingBufffer = 0x00;
}

bool TwclHandler::listen()	{
	if(serial->available())	{
		if(packetStart)	{
			while(serial->available())	{
				rollingBufffer = char(serial->read());
				buffer += rollingBufffer;

				if(rollingBufffer == f_vec)	{
					packetStart = false;

					if(packet.build(buffer))	{
						if(packet.getD() == BOOT && packet.getB() == myID)	{
							while(!send(myID, packet.getD(), packet.getD1(), packet.getE()));
						}else if(packet.getD() == PING && packet.getB() == myID)	{
							while(!send(myID, packet.getD(), packet.getD1(), packet.getE()));
						}

						if(isMaster)	return true;
						else	{
							if(packet.getB() == myID || packet.getB() == BROADCAST)	{
								return true;
							}
						}
					}
				}
			}
		}else	{
			while(serial->available())	{
				rollingBufffer = char(serial->read());
				if(rollingBufffer == a_vec)	{
					buffer = rollingBufffer;
					packetStart = true;
					break;
				}
			}
		}
	}

	return false;
}

TwclPacket TwclHandler::getPacket()	{
	return packet;
}

String TwclHandler::getBuffer()	{
	return buffer;
}

bool TwclHandler::send(uint16_t b, uint16_t d, uint16_t d1, uint8_t e)	{
	if(isMaster)	{
		startSend(&b, UINT8, &d, &d1);
		serial->print(e);
		endSend();
		return true;
	}else	{
		if(digitalRead(bip))	{
			digitalWrite(bop, 1);
			startSend(&b, UINT8, &d, &d1);
			serial->print(e);
			endSend();

			digitalWrite(bop, 0);
			return true;
		}
	}
	return false;
}

bool TwclHandler::send(uint16_t b, uint16_t d, uint16_t d1, uint16_t e)	{
	if(isMaster)	{
		startSend(&b, UINT16, &d, &d1);
		serial->print(e);
		endSend();
		return true;
	}else	{
		if(digitalRead(bip))	{
			digitalWrite(bop, 1);
			startSend(&b, UINT16, &d, &d1);
			serial->print(e);
			endSend();

			digitalWrite(bop, 0);
			return true;
		}
	}
	return false;
}

bool TwclHandler::send(uint16_t b, uint16_t d, uint16_t d1, uint32_t e)	{
	if(isMaster)	{
		startSend(&b, UINT32, &d, &d1);
		serial->print(e);
		endSend();
		return true;
	}else	{
		if(digitalRead(bip))	{
			digitalWrite(bop, 1);
			startSend(&b, UINT32, &d, &d1);
			serial->print(e);
			endSend();

			digitalWrite(bop, 0);
			return true;
		}
	}
	return false;
}

bool TwclHandler::send(uint16_t b, uint16_t d, uint16_t d1, int e)	{
	if(isMaster)	{
		startSend(&b, INTEGER, &d, &d1);
		serial->print(e);
		endSend();
		return true;
	}else	{
		if(digitalRead(bip))	{
			digitalWrite(bop, 1);
			startSend(&b, INTEGER, &d, &d1);
			serial->print(e);
			endSend();

			digitalWrite(bop, 0);
			return true;
		}
	}
	return false;
}

bool TwclHandler::send(uint16_t b, uint16_t d, uint16_t d1, float e)	{
	if(isMaster)	{
		startSend(&b, FLOAT, &d, &d1);
		serial->print(e);
		endSend();
		return true;
	}else	{
		if(digitalRead(bip))	{
			digitalWrite(bop, 1);
			startSend(&b, FLOAT, &d, &d1);
			serial->print(e);
			endSend();

			digitalWrite(bop, 0);
			return true;
		}
	}
	return false;
}

bool TwclHandler::send(uint16_t b, uint16_t d, uint16_t d1, char e)	{
	if(isMaster)	{
		startSend(&b, CHAR, &d, &d1);
		serial->print(e);
		endSend();
		return true;
	}else	{
		if(digitalRead(bip))	{
			digitalWrite(bop, 1);
			startSend(&b, CHAR, &d, &d1);
			serial->print(e);
			endSend();

			digitalWrite(bop, 0);
			return true;
		}
	}
	return false;
}

bool TwclHandler::send(uint16_t b, uint16_t d, uint16_t d1, char *e)	{
	if(isMaster)	{
		startSend(&b, CHAR_ARRAY, &d, &d1);
		uint16_t eSize = String(e).length();
		for(uint8_t i = 0; i < eSize; i++)	{
			serial->print(e[i]);
		}
		endSend();
		return true;
	}else	{
		if(digitalRead(bip))	{
			digitalWrite(bop, 1);
			startSend(&b, CHAR_ARRAY, &d, &d1);
			uint16_t eSize = String(e).length();
			for(uint8_t i = 0; i < eSize; i++)	{
				serial->print(e[i]);
			}
			endSend();

			digitalWrite(bop, 0);
			return true;
		}
	}
	return false;
}

bool TwclHandler::send(uint16_t b, uint16_t d, uint16_t d1, String e)	{
	if(isMaster)	{
			startSend(&b, STRING, &d, &d1);
			for(uint8_t i = 0; i < e.length(); i++)	{
				serial->print(e[i]);
			}
			endSend();
			return true;
	}else	{
		if(digitalRead(bip))	{
			digitalWrite(bop, 1);
			startSend(&b, STRING, &d, &d1);
			for(uint8_t i = 0; i < e.length(); i++)	{
				serial->print(e[i]);
			}
			endSend();

			digitalWrite(bop, 0);
			return true;
		}
	}
	return false;
}

uint8_t TwclHandler::getMyId()	{
	return myID;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// PRIVATE METHODS
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void TwclHandler::startSend(uint16_t *b, uint16_t c, uint16_t *d, uint16_t *d1)	{
	serial->write(a_vec);

	if((*b & 0xFF00) > 0)	{
		uint8_t thisByte = (*b & 0xFF00) >> 8;
		serial->write(thisByte);
		thisByte = *b & 0xFF;
		serial->write(thisByte);
	}else	{
		serial->write(*b);
	}
	serial->write(delim_vec);
	
	if((c & 0xFF00) > 0)	{
		uint8_t thisByte = c >> 8;
		serial->write(thisByte);
		thisByte = c & 0xFF;
		serial->write(thisByte);
	}else	{
		serial->write(c);
	}
	serial->write(delim_vec);

	if((*d & 0xFF00) > 0)	{
		uint8_t thisByte = *d >> 8;
		serial->write(thisByte);
		thisByte = *d & 0xFF;
		serial->write(thisByte);
	}else	{
		serial->write(*d);
	}
	serial->write(delim_vec);
	
	if((*d1 & 0xFF00) > 0)	{
		uint8_t thisByte = *d1 >> 8;
		serial->write(thisByte);
		thisByte = *d1 & 0xFF;
		serial->write(thisByte);
	}else	{
		serial->write(*d1);
	}
	serial->write(delim_vec);

	if((myID & 0xFF00) > 0)	{
		uint8_t thisByte = myID >> 8;
		serial->write(thisByte);
		thisByte = myID & 0xFF;
		serial->write(thisByte);
	}else	{
		serial->write(myID);
	}
	serial->write(delim_vec);
}

void TwclHandler::endSend()	{
	serial->write(f_vec);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -