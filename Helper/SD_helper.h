#include "Arduino.h"
#include "SD.h"
#include "ds3231.h"

bool InitSD(HardwareSerial *output_log)	{
	
	Sd2Card card;
	SdVolume volume;
	SdFile root; 

	if(card.init(SPI_HALF_SPEED, PA4))	{
		output_log->print("\r\nSD initialized.\r\n");

		output_log->print("Card type: ");
		switch (card.type()) {
			case SD_CARD_TYPE_SD1:
				output_log->println("SD1");
			break;
			case SD_CARD_TYPE_SD2:
				output_log->println("SD2");
			break;
			case SD_CARD_TYPE_SDHC:
				output_log->println("SDHC");
			break;
			default:
				output_log->println("Unknown");
		}

		if (!volume.init(card)) {
			Serial.println("Could not find FAT16/FAT32 partition.\r\nMake sure you've formatted the card");
		}else	{
			uint32_t volumesize;
			output_log->print("Volume type is FAT");
			output_log->println(volume.fatType(), DEC);
			volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
			volumesize *= volume.clusterCount();       // we'll have a lot of clusters
			volumesize *= 512;    
			output_log->print("Volume size (Kbytes): ");
			volumesize /= 1024;
			output_log->println(volumesize);

			
			if(SD.begin(PA4))	{

				return true;
			}
			

			return false;
		}
	}else	{
		output_log->print("SD fail initialization.\r\n");
	}

	return false;
}

TimeFormat GetTime(){
    ts t;
    DS3231_get(&t); // Reading time from RTC

    TimeFormat retVal;
    retVal.year = (t.year);
    retVal.mounth = (t.mon);
    retVal.day = (t.mday);
    retVal.hour = (t.hour);
    retVal.minute = (t.min);
    retVal.second = (t.sec);

    retVal.end_hour = t.hour;
    retVal.end_minute = t.min;

    return retVal;
}

bool IsFinish(TimeFormat startTime, int* minuteLeft){
    TimeFormat realTime;
    realTime = GetTime();

    if(startTime.end_hour == realTime.hour){
        if( realTime.minute >= startTime.end_minute){
            return true;
        }
    }

    if(startTime.end_hour == startTime.hour){
        *minuteLeft = startTime.end_minute - realTime.minute;
    }else{
        if(startTime.end_hour == realTime.hour){
            *minuteLeft = startTime.end_minute - realTime.minute;
        }else{
            *minuteLeft = (60 - realTime.minute) + startTime.end_minute;
        }
    }

    return false;
}
 

void SetTime(char *cmd, int cmdsize){
   const int BUFF_MAX = 17u;  // extern
   struct ts t;         // extern
   int i;
   extern uint8_t time[8]; 
   uint8_t reg_val;
   char buff[BUFF_MAX]; 

   //snprintf(buff, BUFF_MAX, "cmd was '%s' %d\n", cmd, cmdsize);
   //Serial.print(buff);

   // TssmmhhWDDMMYYYY aka set time
   if (cmd[0] == 84 && cmdsize == 16) {
       //T355720619112011
       t.sec = inp2toi(cmd, 1);
       t.min = inp2toi(cmd, 3);
       t.hour = inp2toi(cmd, 5);
       t.wday = cmd[7] - 48;
       t.mday = inp2toi(cmd, 8);
       t.mon = inp2toi(cmd, 10);
       t.year = inp2toi(cmd, 12) * 100 + inp2toi(cmd, 14);
       DS3231_set(t);
       Serial.println("OK");
   } else if (cmd[0] == 49 && cmdsize == 1) {  // "1" get alarm 1
       DS3231_get_a1(&buff[0], 59);
       Serial.println(buff);
   } else if (cmd[0] == 50 && cmdsize == 1) {  // "2" get alarm 1
       DS3231_get_a2(&buff[0], 59);
       Serial.println(buff);
   } else if (cmd[0] == 51 && cmdsize == 1) {  // "3" get aging register
       Serial.print("aging reg is ");
       Serial.println(DS3231_get_aging(), DEC);
   } else if (cmd[0] == 65 && cmdsize == 9) {  // "A" set alarm 1
       DS3231_set_creg(DS3231_INTCN | DS3231_A1IE);
       //ASSMMHHDD
       for (i = 0; i < 4; i++) {
           time[i] = (cmd[2 * i + 1] - 48) * 10 + cmd[2 * i + 2] - 48; 
            // ss, mm, hh, dd
       }
       uint8_t flags[5] = { 0, 0, 0, 0, 0 };
       DS3231_set_a1(time[0], time[1], time[2], time[3], flags);
       DS3231_get_a1(&buff[0], 59);
       Serial.println(buff);
   } else if (cmd[0] == 66 && cmdsize == 7) {  // "B" Set Alarm 2
       DS3231_set_creg(DS3231_INTCN | DS3231_A2IE);
       //BMMHHDD
       for (i = 0; i < 4; i++) {
           time[i] = (cmd[2 * i + 1] - 48) * 10 + cmd[2 * i + 2] - 48; 
            // mm, hh, dd
       }
       uint8_t flags[5] = { 0, 0, 0, 0 };
       DS3231_set_a2(time[0], time[1], time[2], flags);
       DS3231_get_a2(&buff[0], 59);
       Serial.println(buff);
   } else if (cmd[0] == 67 && cmdsize == 1) {  
        // "C" - get temperature register
       Serial.print("temperature reg is ");
       Serial.println(DS3231_get_treg(), DEC);
   } else if (cmd[0] == 68 && cmdsize == 1) {  
        // "D" - reset status register alarm flags
       reg_val = DS3231_get_sreg();
       reg_val &= B11111100;
       DS3231_set_sreg(reg_val);
   } else if (cmd[0] == 70 && cmdsize == 1) {  // "F" - custom fct
       reg_val = DS3231_get_addr(0x5);
       Serial.print("orig ");
       Serial.print(reg_val,DEC);
       Serial.print("month is ");
       Serial.println(bcdtodec(reg_val & 0x1F),DEC);
   } else if (cmd[0] == 71 && cmdsize == 1) {  
        // "G" - set aging status register
       DS3231_set_aging(0);
   } else if (cmd[0] == 83 && cmdsize == 1) {  
        // "S" - get status register
       Serial.print("status reg is ");
       Serial.println(DS3231_get_sreg(), DEC);
   } else {
       Serial.print("unknown command prefix ");
       Serial.println(cmd[0]);
       Serial.println(cmd[0], DEC);
   }
}

bool MakeDirection(String path, HardwareSerial* console){
if(SD.mkdir(path)){
        console->println("DIR OK");
        return true;
    }else{
        console->println("DIR ERROR");
        console->println("error opening " + path);
        return false;
    }
}

bool CreateFile(String path, String fileName, HardwareSerial* console){
    File myFile;

    String fullPath = path + "/" + fileName;

    if(! SD.exists(fullPath)){
        console->println("creating file " + fullPath);

        myFile = SD.open(fullPath, FILE_WRITE);

        if(!myFile){
            console->println("error opening " + fullPath);
            return false;
        }

        myFile.close();
    }

    return SD.exists(fullPath);
}

bool WriteToFile(String path, String fileName, LogFormat data, HardwareSerial* console){
    File myFile;

    String fullPath = path + "/" + fileName;

    if(! SD.exists(fullPath)){
        console->println("creating file " + fullPath);

        myFile = SD.open(fullPath, FILE_WRITE);

        if(!myFile){
            console->println("error opening " + fullPath);
            return false;
        }

        myFile.close();
    }

    myFile = SD.open(fullPath, FILE_WRITE);

    if(!myFile){
        console->println("error opening " + fullPath);
        return false;
    }

    myFile.println(data.ToString());

    myFile.close();

    return true;
}

int PrintFile(String path, String fileName, HardwareSerial* console){

    String fullPath = path + "/" + fileName;

    File myFile = SD.open(fullPath, FILE_READ);
    
    if(!myFile){
        console->println("error opening " + fullPath);
        return -1;
    }

    int numberOfLines = 0;

    while(myFile.available()){
        String line = myFile.readStringUntil('\n');
        
        console->println(line);

        numberOfLines++;
    }

    myFile.close();

    return numberOfLines;
}

bool DataToModel(String path, String fileName, LogFormat* retVal, HardwareSerial* console){
    
    String fullPath = path + "/" + fileName;

    File myFile = SD.open(fullPath, FILE_READ);
    
    if(!myFile){
        console->println("error opening " + fullPath);
        return false;
    }
    
    int i = 0;
    while(myFile.available()){
        String line = myFile.readStringUntil('\n');

        retVal[i].ToModel(line);

        i++;
    }

    myFile.close();

    return true;
}

/*   
    #pragma region Write treatment in log file

    String path = ConvertIntToString(startTime.year) + "/" + ConvertIntToString(startTime.mounth) + "/" + ConvertIntToString(startTime.day) + "/" + ConvertIntToString(startTime.hour);
    String fileName = "log.txt";

    if(! MakeDirection(path, console)){
        EndProgram();
    }
    
    if(! CreateFile(path, fileName, console)){
        EndProgram();
    }
    
    if(! WriteToFile(path, fileName, tretman, console)){
        EndProgram();
    }
   
    console->println("Successfully written data in log!");

    #pragma endregion
  
  
    int numberOfLines = PrintFile(path, fileName, console);

    if(numberOfLines == -1){
        console->println("Error while reading data.");
        EndProgram();
    }

    if(numberOfLines == 0){
        console->println("No data to read");
        EndProgram();
    }

    LogFormat dataSD[numberOfLines];
    
    console->print("Reading data...");

    if(!DataToModel(path, fileName, dataSD, console)){
        EndProgram();
    }

    console->println(" END");

 //   delay(3000);


    for(int i = 0; i < numberOfLines; i++){
        console->println("");
        console->println("Log\t\t[" + ConvertIntToString(i) + "]");
        
        dataSD[i].Print(console);

    //    delay(1000);
    }

    EndProgram();
    */