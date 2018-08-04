#include "Converter.h"

class LogFormat {     
    public:
        
        LogFormat();

        void SetUser(String);

        String GetUser() const;

        void SetHorse(String);
        String GetHorse() const;

        void SetDateTime(String );
        String GetDateTime() const;

        void SetDuration(int );
        int GetDuration() const;

        void SetCapacity(int );
        int GetCapacity() const;

        void SetError(String );
        String GetError() const;

    //    String ErrorTable[];

        String ToString();
        void ToModel(String);
        void ToModelFromMessage(String);

        void Print(HardwareSerial*);


    private:
        String _user;
        String _horse;
        String _dateTime;
        int _duration;
        int _capacity;
        String _error;
};