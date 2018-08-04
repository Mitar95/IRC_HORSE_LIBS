#include <Arduino.h>

String static ConvertIntToString(int someInt){
    char str[12];
    sprintf(str, "%d", someInt);

    return String(str);
}

String static ConvertVolatileCharToString(volatile char buf[]){
    String retVal = "";

    for(int i = 0; i < 101 && buf[i] != 0; i++){
        retVal += buf[i];
    }

    return retVal;
}

void static ConvertVolatileStringToChar(String buf, volatile char* arr){

    for(unsigned int i = 0; i < buf.length(); i++){
        arr[i] = buf.charAt(i);
    }

    arr[buf.length()] = 0;
}