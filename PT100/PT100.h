#ifndef PT100_h
#define PT100_h

#include "Arduino.h"
#include "SoftwareTimer.h"

class PT100	{
	public:
		void init(uint8_t input_pin, double *points, double R1, double voltage);

		// --- Simple ---
		double get();
		double getRawVoltage();
		double getRawAD();
		double getResistance();
		double getAmplification(double *voltage);
		double getCalculatedVa(double *vMeasured);

		// --- Advanced ---
		double getTemperature();
		void getSample();
		void requestTemperature();
		bool isRequested();
		bool isReady();
		bool calibrate(uint16_t maxTimeInMs);

		double getLastTemperature();
		double getLastAD();
		double getLastVoltageA();
		double getLastVoltageD();
		double getLastAmplification();
		double getLastResistance();

	private:
		#define adOffs 1

		uint8_t pin;

		double vMeasured;
		double aCalculated;
		double vaCalculated;
		double rx, temperature;
		double adK;
		double a[2], rttk[2];
		double r1, vcc;
		double ad, adNew, adOld;
		double acceleration;

		double atv();
		double vdtva(double *voltage);
		double vtr(double *voltage);
		double rtt(double *resistance);

		const static uint16_t maxNumOfSamples = 10;
		uint16_t currSample;

		bool requested;
		bool ready;

		SoftwareTimer dampTimer;
};

#endif