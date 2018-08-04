#include "LogFormat.h"


char splitChar = '@';

LogFormat::LogFormat(){
    _user = "";
    _horse = "";
    _dateTime = "";
    _duration = 0;
    _capacity = 0;
    _error = "";
}


// setters

void LogFormat::SetUser(String value){_user = value;}

void LogFormat::SetHorse(String value){_horse = value;}

void LogFormat::SetDateTime(String value){_dateTime = value;}

void LogFormat::SetDuration(int value){_duration = value;}

void LogFormat::SetCapacity(int value){_capacity = value;}

void LogFormat::SetError(String value){_error = value;}


// Getters

String LogFormat::GetUser() const{return _user;}

String LogFormat::GetHorse() const{return _horse;}

String LogFormat::GetDateTime() const{return _dateTime;}

int LogFormat::GetDuration() const{return _duration;}

int LogFormat::GetCapacity() const{return _capacity;}

String LogFormat::GetError() const{return _error;}


// methods

String LogFormat::ToString(){
    return _user + splitChar +
        _horse + splitChar +
        _dateTime + splitChar +
        ConvertIntToString(_duration) + splitChar +
        ConvertIntToString(_capacity) + splitChar +
        _error;
}

void LogFormat::ToModel(String line){

    unsigned char splitPossition = 0;
    String user = "";
    String horse = "";
    String dateTime = "";
    String duration = "";
    String capacity = "";
    String error = "";

    for(unsigned int i = 0; i < line.length(); i++){
        if(line.charAt(i) == splitChar){
            splitPossition++;
            continue;
        }

        switch(splitPossition){
            case 0:
                user += line.charAt(i);
                break;
            case 1:
                horse += line.charAt(i);
                break;
            case 2:
                dateTime += line.charAt(i);
                break;
            case 3:
                duration += line.charAt(i);
                break;
            case 4:
                capacity += line.charAt(i);
                break;
            case 5:
                error += line.charAt(i);
                break;
            default:
                break;
        }
    }

    _user = user;
    _horse = horse;
    _dateTime = dateTime;
    _duration = duration.toInt();
    _capacity = capacity.toInt();
    _error = error;
}

void LogFormat::ToModelFromMessage(String line){

    unsigned char splitPossition = 0;
    String user = "";
    String horse = "";
    String dateTime = "";
    String duration = "";
    String capacity = "";
    String error = "";

    for(unsigned int i = 0; i < line.length(); i++){
        if(line.charAt(i) == splitChar){
            splitPossition++;
            continue;
        }

        switch(splitPossition){
            case 0:
                user += line.charAt(i);
                break;
            case 1:
                horse += line.charAt(i);
                break;
            case 2:
                duration += line.charAt(i);
                break;
            case 3:
                capacity += line.charAt(i);
                break;
            default:
                break;
        }
    }

    _user = user;
    _horse = horse;
    _duration = duration.toInt();
    _capacity = capacity.toInt();
    _error = "0";
}

void LogFormat::Print(HardwareSerial* console){
    console->println("User:\t\t" + _user);
    console->println("Horse:\t\t" + _horse);
    console->println("DateTime:\t" + _dateTime);
    console->println("Duration:\t" + ConvertIntToString(_duration));
    console->println("Capacity:\t" + ConvertIntToString(_capacity));
    console->println("Error:\t\t" + _error);
}