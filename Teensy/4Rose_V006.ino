//#pragma once
/* *****************************************************************
* 4Rose_V006 main entry
* Author: Edward French
******************************************************************/
#include "math.h"
#include <string>
#include <EEPROM.h>
#include "4RoseDefines.h"
#include "AccelStepper.h"
#include "SD.h"
#include "SPI.h"
#include "TeensyStep.h"

//==================================================================
// Pin assignments
//==================================================================
// See 4RoseDefines.h
// Initialize AccelStepper object
AccelStepper accelStep_Spindle(AccelStepper::DRIVER, PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
AccelStepper accelStep_Axis_Z(AccelStepper::DRIVER, PIN_AXIS_Z_STEP, PIN_AXIS_Z_DIR);
AccelStepper accelStep_Axis_X(AccelStepper::DRIVER, PIN_AXIS_X_STEP, PIN_AXIS_X_DIR);
AccelStepper accelStep_Axis_B(AccelStepper::DRIVER, PIN_AXIS_B_STEP, PIN_AXIS_B_DIR);

// TeensyStep initialization
Stepper teensyStep_Spindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
Stepper teensyStep_Axis_Z(PIN_AXIS_Z_STEP, PIN_AXIS_Z_DIR);
Stepper teensyStep_Axis_X(PIN_AXIS_X_STEP, PIN_AXIS_X_DIR);

// TeensyStep controllers
StepControl stepControllerI;
RotateControl controllerRose_Axis;

/// <summary>
/// Setup
/// </summary>
/// <comment>
/// Initial setup
/// </comment>
/// <returns></returns>
void setup()
{
	digitalWrite(1, HIGH);
	pinMode(LED_BUILTIN, OUTPUT);
	// Initialize Serial0 (USB port)
	Serial.begin(115200);
	//Serial.begin(9600);
	// Initialize Nextion LCD for 115200 baud
	// Note: Nextion requires 3 0xFF bytes to signal end of transmission
	// First run for Nextion must run at 9600.  Once the Nextion has been programmed for 115200, uncomment 115200 line and comment 9600 line.
	// Nextion may use Serial1 or Serial1.
	////Serialxx1.begin(115200);
	//Serial1.begin(9600);  
	Serial1.begin(115200); //Nextion Serial baud rate set in Nextion pageSpB Preinitialize Event tab
	Serial1.print("bauds=115200");
	Serial1.write(0xFF);
	Serial1.write(0xFF);
	Serial1.write(0xFF);
	delay(50);
	Serial1.print("bauds=115200");
	Serial1.write(0xFF);
	Serial1.write(0xFF);
	Serial1.write(0xFF);
	delay(50);
	Serial1.print("bkcmd=0");  // Set Nextion to return NO replies to each command
	Serial1.write(0xFF);
	Serial1.write(0xFF);
	Serial1.write(0xFF);
	delay(50);

	SerialPrint("pageSpZ.t0.txt=");
	SerialWrite(0x22);
	SerialPrint("Good");
	SerialWrite(0x22);
	SerialWrite(0xff);
	SerialWrite(0xff);
	SerialWrite(0xff);
	delay(50);
	SerialPrint("pageSpZ.t0.txt=");
	SerialWrite(0x22);
	SerialPrint("Good");
	SerialWrite(0x22);
	SerialWrite(0xff);
	SerialWrite(0xff);
	SerialWrite(0xff);
	delay(50);


	// Update with values from EEProm
	EEPROM.get(eePromAddress_Setup, configMain);

	EEPROM.get(eePromAddress_SpZ, configSpZ);
	EEPROM.get(eePromAddress_SpX, configSpX);
	EEPROM.get(eePromAddress_SpB, configSpB);
	EEPROM.get(eePromAddress_SyncZ, configSyncZ);
	EEPROM.get(eePromAddress_SyncX, configSyncX);
	EEPROM.get(eePromAddress_Rec1_Z, configRec1_Z);
	EEPROM.get(eePromAddress_Rec1_S, configRec1_S);
	EEPROM.get(eePromAddress_Sp, configSp);
	EEPROM.get(eePromAddress_Index1, configIndex1);
	EEPROM.get(eePromAddress_Index2, configIndex2);
	EEPROM.get(eePromAddress_MoveZ, configMoveZ);
	EEPROM.get(eePromAddress_MoveX, configMoveX);
	EEPROM.get(eePromAddress_Rose, configRose);

	// Config as well as all other EEProm settings must be run from Nextion whenever Teensy is updated.  
	// EEProm may contain invalid settings otherwise.

	//for (int i = 0; i < 3; i++) // Verify Teensy is operational
	//{
	//	digitalWrite(LED_BUILTIN, HIGH);
	//	delay(50);
	//	digitalWrite(LED_BUILTIN, LOW);
	//	delay(300);
	//}

#ifdef FOUR_AXES
	pinMode(PIN_SPINDLE_MS0, OUTPUT);
	pinMode(PIN_SPINDLE_MS1, OUTPUT);
	pinMode(PIN_SPINDLE_MS2, OUTPUT);
	pinMode(PIN_AXIS_Z_MS0, OUTPUT);
	pinMode(PIN_AXIS_Z_MS1, OUTPUT);
	pinMode(PIN_AXIS_Z_MS2, OUTPUT);
	pinMode(PIN_AXIS_X_MS0, OUTPUT);
	pinMode(PIN_AXIS_X_MS1, OUTPUT);
	pinMode(PIN_AXIS_X_MS2, OUTPUT);
	// Set Microstepping mode
	SetMicrosteppingMode(configMain.microsteps_Spindle, PIN_SPINDLE_MS0, PIN_SPINDLE_MS1, PIN_SPINDLE_MS2);
	SetMicrosteppingMode(configMain.microsteps_Axis_Z, PIN_AXIS_Z_MS0, PIN_AXIS_Z_MS1, PIN_AXIS_Z_MS2);
	//SetMicrosteppingMode(configMain.microsteps_Axis_B, PIN_AXIS_B_MS0, PIN_AXIS_B_MS1, PIN_AXIS_B_MS2);
#endif //FOUR_AXES
#ifndef TWO_AXES_V2
	SetMicrosteppingMode(configMain.microsteps_Axis_X, PIN_AXIS_X_MS0, PIN_AXIS_X_MS1, PIN_AXIS_X_MS2);
#endif // TWO_AXES_V2

	// Set the enable pin for the stepper motor
	accelStep_Spindle.setEnablePin(PIN_SPINDLE_ENABLE);

	// Set pins "inverted", HIGH == off
	accelStep_Spindle.setPinsInverted(false, false, configMain.enable_Spindle);

	// Disable the spindle stepper motor
	accelStep_Spindle.disableOutputs();

	//-------------------------------
	// Z Axis stepper motor initialization
	// Configure Enable Z Axis stepper
	accelStep_Axis_Z.setEnablePin(PIN_AXIS_Z_ENABLE);
	accelStep_Axis_Z.setPinsInverted(false, false, configMain.polarity_Axis_Z);

	// Disable the Z Axis stepper motor (enable in run method)
	accelStep_Axis_Z.disableOutputs();


	//-------------------------------
#ifdef FOUR_AXES
	// B Axis stepper motor initialization
	// Configure Enable B Axis stepper
	accelStep_Axis_B.setEnablePin(PIN_AXIS_B_ENABLE);
	accelStep_Axis_B.setPinsInverted(false, false, configMain.polarity_Axis_B);

	// Disable the B Axis stepper motor (enable in run method)
	accelStep_Axis_B.disableOutputs();

#endif // FOUR_AXES
#ifndef TWO_AXES_V2 // Three and Four axes boards
	// X Axis stepper motor initialization
	// Configure Enable X Axis stepper
	accelStep_Axis_X.setEnablePin(PIN_AXIS_X_ENABLE);
	accelStep_Axis_X.setPinsInverted(false, false, configMain.polarity_Axis_X);

	// Disable the X Axis stepper motor (enable in run method)
	accelStep_Axis_X.disableOutputs();


	//------------------------------
	// Configure TeensyStep motors
	teensyStep_Axis_X
		.setMaxSpeed(configSpX.maxSpd_Axis_X)       // steps/s
		.setAcceleration(configSpX.accel_Axis_X) // steps/s^2    
		.setStepPinPolarity(configMain.polarity_Axis_X ? (LOW) : (HIGH)); // driver expects active low pulses
#endif // TWO_AXES_V2

	// Configure TeensyStep motors

	teensyStep_Spindle
		.setMaxSpeed(configSyncZ.maxSpd_Spindle)       // steps/s
		.setAcceleration(configSyncZ.accel_Spindle) // steps/s^2 
		.setStepPinPolarity(configMain.enable_Spindle ? (LOW) : (HIGH)); // DRV8825 driver expects active low pulses

	teensyStep_Axis_Z
		.setMaxSpeed(configSyncZ.maxSpd_Axis_Z)       // steps/s
		.setAcceleration(configSyncZ.accel_Axis_Z) // steps/s^2 
		.setStepPinPolarity(configMain.polarity_Axis_Z ? (LOW) : (HIGH)); // driver expects active low pulses

	// Initialize Limit switches
	pinMode(configMain.limit_Min_Z, INPUT_PULLUP);
	pinMode(configMain.limit_Max_Z, INPUT_PULLUP);
	delayMicroseconds(10);
	digitalWrite(configMain.limit_Min_Z, HIGH);  // Enable
	digitalWrite(configMain.limit_Max_Z, HIGH);  // Enable
#ifndef TWO_AXES_V2 // Three and Four axes boards
	pinMode(configMain.limit_Min_X, INPUT_PULLUP);
	pinMode(configMain.limit_Max_X, INPUT_PULLUP);

	delayMicroseconds(10);
	digitalWrite(configMain.limit_Min_X, HIGH);  // Enable
	digitalWrite(configMain.limit_Max_X, HIGH);  // Enable
#endif
#ifdef FOUR_AXES
	pinMode(configMain.limit_Min_B, INPUT_PULLUP);
	pinMode(configMain.limit_Max_B, INPUT_PULLUP);
	digitalWrite(configMain.limit_Min_B, HIGH);  // Enable
	digitalWrite(configMain.limit_Max_B, HIGH);  // Enable
#endif
	delayMicroseconds(10);

	// Enable SD card reader
	bool sdValid = false;
#ifdef FOUR_AXES
	sdValid = SD.begin(BUILTIN_SDCARD);
#else
	sdValid = SD.begin(PIN_SPI_CS);
#endif // FOUR_AXES

	if (sdValid)
	{
	////	SerialPrint("pageIndex2.vaDisplayFile.val=1"); // Nextion may not get the first packet
	////	Serial1.write(0xff);
	////	Serial1.write(0xff);
	////	Serial1.write(0xff);
	////	delay(50);
	////	Serial1.print("pageIndex2.vaDisplayFile.val=1");
	////	Serial1.write(0xff);
	////	Serial1.write(0xff);
	////	Serial1.write(0xff);
	}
	else
	{
	////	SerialPrint("pageIndex2.vaDisplayFile.val=0"); // Nextion may not get the first packet
	////	Serial1.write(0xff);
	////	Serial1.write(0xff);
	////	Serial1.write(0xff);
	////	delay(50);
	////	Serial1.print("pageIndex2.vaDisplayFile.val=0");
	////	Serial1.write(0xff);
	////	Serial1.write(0xff);
	////	Serial1.write(0xff);
	}
}

/// <summary>
/// Loop
/// </summary>
/// <comment>
/// Main program loop
/// </comment>
/// <returns></returns>
void loop()
{
	// Select serial port: Serial = USB connected, Serial1 = Nextion LCD connected
	if (Serial.available() > 0)
	{
		// USB connected 
		serialId = 0;
	}
	////else if (Serialxx1.available() > 0)
	////{
	////	//Nextion connected
	////	serialId = 1;
	////}
	else if (Serial1.available() > 0)
	{
		//Nextion connected
		serialId = 1;
	}
	serialId = 3;
#ifdef DEBUG

	Serial1.print("pageSpZ.t0.txt="); // Nextion may not get the first packet
	Serial1.write(0x22);
	Serial1.print("Id:");
	Serial1.print(serialId);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(100);
	Serial1.print("pageSpZ.t0.txt=");
	Serial1.write(0x22);
	Serial1.print("Id:");
	Serial1.print(serialId);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(100);



#endif // DEBUG

	//for (int i = 0; i < 3; i++) // Verify Teensy is operational
	//{
	//	digitalWrite(LED_BUILTIN, HIGH);
	//	delay(50);
	//	digitalWrite(LED_BUILTIN, LOW);
	//	delay(300);
	//}
	// All Nextion incoming data packets are terminated with one 0xFF byte
	if (serialId <9) // If serial data is available, parse the data
	{
		incomingByte = SerialRead(serialId);
		delay(100);

		switch (incomingByte)
		{
			//case 48: // 0
			//case 49: // 1 - 
			//	case 50: // 2 - 
			//	case 51: // 3 - 
			//	case 52: // 4 - 
			//	case 53: // 5 - 
			//	case 54: // 6 - 
			//	case 55: // 7 - 
			//	case 56: // 8 - 
			//case 57: // 9 - 
			//{
			//	break;
			//}
		case 58: // : Colon - Sync Spindle MaxSpeed
		{
			pageCallerId = GetSerialIntegerOnly();
			switch (pageCallerId)
			{
				case PAGE_SPZ:
				{
					configSpZ.maxSpd_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SpZ, configSpZ);
					break;
				}
				case PAGE_SPX:
				{
					configSpX.maxSpd_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SpX, configSpX);
					break;
				}
				case PAGE_SPB:
				{
					configSpB.maxSpd_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SpB, configSpB);
					break;
				}
				case PAGE_SYNCZ:
				{
					configSyncZ.maxSpd_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SyncZ, configSyncZ);
					break;
				}
				case PAGE_SYNCX:
				{
					configSyncX.maxSpd_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SyncX, configSyncX);
					break;
				}
				case PAGE_REC1_Z:
				{
					configRec1_Z.maxSpd_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Rec1_Z, configRec1_Z);
					break;
				}
				case PAGE_REC1_S:
				{
					configRec1_S.maxSpd_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Rec1_S, configRec1_S);
					break;
				}
				case PAGE_SP:
				{
					configSp.maxSpd_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Sp, configSp);
					break;
				}
				case PAGE_INDEX1:
				{
					configIndex1.maxSpd_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Index1, configIndex1);
					break;
				}
				case PAGE_INDEX2:
				{
					configIndex2.maxSpd_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Index2, configIndex2);
					break;
				}
			}
			break;
		}
		case 59: // ; Semicolon - Sync Spindle Acceleration
		{
			pageCallerId = GetSerialIntegerOnly();
			switch (pageCallerId)
			{
				case PAGE_SPZ:
				{
					configSpZ.accel_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SpZ, configSpZ);
					break;
				}
				case PAGE_SPX:
				{
					configSpX.accel_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SpX, configSpX);
					break;
				}
				case PAGE_SPB:
				{
					configSpB.accel_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SpB, configSpB);
					break;
				}
				case PAGE_SYNCZ:
				{
					configSyncZ.accel_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SyncZ, configSyncZ);
					break;
				}
				case PAGE_SYNCX:
				{
					configSyncX.accel_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SyncX, configSyncX);
					break;
				}
				case PAGE_REC1_Z:
				{
					configRec1_Z.accel_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Rec1_Z, configRec1_Z);
					break;
				}
				case PAGE_REC1_S:
				{
					configRec1_S.accel_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Rec1_S, configRec1_S);
					break;
				}
				case PAGE_SP:
				{
					configSp.accel_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Sp, configSp);
					break;
				}
				case PAGE_INDEX1:
				{
					configIndex1.accel_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Index1, configIndex1);
					break;
				}
				case PAGE_INDEX2:
				{
					configIndex2.accel_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Index2, configIndex2);
					break;
				}
			}

			break;
		}
		case 60: // < Less than - Rose: Axis MaxSpd
		{
			configRose.maxSpd_Axis_Z = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rose, configRose);
			break;
		}
		case 61: // = Start microSD (Currently not used)
		{
#ifdef FOUR_AXES
			bool sdStarted = SD.begin(BUILTIN_SDCARD);
#else
			bool sdStarted = SD.begin(PIN_SPI_CS);
#endif
			if (!sdStarted)
			{
				// ToDo: Write to Nextion
			}

			break;
		}
		case 62: // > Greater than - Set moveZ_ speed
		{
			configMain.speedPercent_MoveZ = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 63: // ? Question mark - moveZ_Distance (Text)
		{
			// Set Distance
			float newDistance = GetSerialFloat(serialId);
			configMain.distance_MoveZ = newDistance;
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 64: // @ - Rose: Spindle RPM
		{
			configRose.speedPercent_Spindle = GetSerialInteger();
			EEPROM.put(eePromAddress_Rose, configRose);
			break;
		}
		case 65: // A - Enable spindle stepper
		{
			accelStep_Spindle.enableOutputs();
			break;
		}
		case 66: //B - Disable spindle stepper
		{
			accelStep_Spindle.disableOutputs();
			break;
		}
		// C: -> Cancel Stop Main/Sp2 Spindle
		// Implemented in Run methods
		case 67: // C - 
		{
		   break;
		}
		case 68: // D - SyncZ Spindle Speed
		{
			configMain.speedPercentSpindle_SyncZ = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 69: //E - Index2 by divisions
		{
			double newIndexSize = GetSerialFloat(serialId);
			configMain.size_Index2 = (configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) / (newIndexSize);
			configMain.type_Index2 = BY_DIVISIONS;
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 70: // F - Index2 counter clockwise
		{

			if (configMain.source_Index2 == FILE_SD)
			{
				int lineNumber = GetSerialInteger();
				if (lineNumber == 255)
				{
					lineNumber = 0;
				}
				double newIndexSize = GetDataFromSD(lineNumber);
				configMain.size_Index2 = (configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) * (newIndexSize / 360);
				Serial1.print("pageIndex2.t7.txt="); // Nextion may not get the first packet
				Serial1.write(0x22);
				Serial1.print(newIndexSize);
				Serial1.write(0x22);
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);
				delay(100);
				Serial1.print("pageIndex2.t7.txt=");
				Serial1.write(0x22);
				Serial1.print(newIndexSize);
				Serial1.write(0x22);
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);
				delay(100);
			}

			// Check for 0 size.  GetDataFromSD function will return 0 when it reaches end of file.
			if (configMain.size_Index2 > 0)
			{
				IndexSpindle(ID_INDEX_2, DIR_CCW);
			}
			break;
		}
		case 71: // G - Index2 Clockwise
		{	
			if (configMain.source_Index2 == FILE_SD)
			{
				int lineNumber = GetSerialInteger();
				if (lineNumber == 255)
				{
					lineNumber = 0;
				}

				// Get size from file
				double newIndexSize = GetDataFromSD(lineNumber);
				configMain.size_Index2 = (configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) * (newIndexSize / 360);
				Serial1.print("pageIndex2.t7.txt=");
				Serial1.write(0x22);
				Serial1.print(newIndexSize);
				Serial1.write(0x22);
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);
				delay(100);
				Serial1.print("pageIndex2.t7.txt=");
				Serial1.write(0x22);
				Serial1.print(newIndexSize);
				Serial1.write(0x22);
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);
				delay(100);
			}

			// Check for 0 size. GetDataFromSD function will return 0 when it reaches end of file.
			if (configMain.size_Index2 > 0)
			{
				IndexSpindle(ID_INDEX_2, DIR_CW); 
			}
			break;
		}
		case 72: // H - Set Index2 speed
		{
			configMain.speedPercent_Index2 = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 73: // I-SyncZ Helix Type
		{
			configMain.helixType_SyncZ = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 74: // J - SyncZ In
		{
			int helixType = GetSerialInteger();
			int directionSpindle = 1;
			int directionAxis = 1;

			switch (helixType)
			{
				case 0: // Left for right handed Z axis leadscrew, Right for left handed Z axis leadscrew.
				case 48:
				{
					directionAxis = -1;// 0; // CCW
					directionSpindle = -1;// 0; //CCW
					break;
				}
				case 1: //Right for right handed Z axis leadscrew, Left for left handed Z axis leadscrew.
				case 49:
				{
					directionAxis = -1;// 0; // CCW
					directionSpindle = 1; //CW
					break;
				}
			}

			Sync_SpindleZ(directionSpindle, directionAxis);
			break;
		}
		case 75: // K - SyncZ Out 
		{
			int helixType = GetSerialInteger();
			int directionSpindle = 1;
			int directionAxis = 1;

			switch (helixType)
			{
				case 0:// Left for right handed Z axis leadscrew, Right for left handed Z axis leadscrew.
				case 48:
				{
					directionAxis = 1; // CW
					directionSpindle = 1; // CW
					break;
				}
				case 1: //Right for right handed Z axis leadscrew, Left for left handed Z axis leadscrew.
				case 49:
				{
					directionAxis = 1; // CW
					directionSpindle = -1;//0; // CCW
					break;
				}
			}

			Sync_SpindleZ(directionSpindle, directionAxis);
			break;
		}
		case 76: // L - Set Index2 type to Degrees
		{
			double newIndexSize = GetSerialFloat(serialId);
			configMain.size_Index2 = (configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) * (newIndexSize / 360);
			configMain.type_Index2 = BY_DEGREES;
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 77: // M - Update Index2 size
		{
			double newIndexSize = GetSerialFloat(serialId);

			// Default: Divisions
			if (configMain.type_Index2 == BY_DEGREES) // Degrees
			{
				configMain.size_Index2 = (configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) * (newIndexSize / 360);
			}
			else
			{
				configMain.size_Index2 = (configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) / (newIndexSize);
			}

			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 78: // N - Sync Spindle Direction
		{
			int newDir = GetSerialInteger();
			switch (newDir)
			{
				case 0:
				case 48:
				{
					configMain.spindleDir_SyncZ = 0;
					break;
				}
				case 1:
				case 49:
				{
					configMain.spindleDir_SyncZ = 1;
					break;
				}
			}

			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 79: // O - Sync Axis Speed
		{
			configMain.speedPercentAxis_SyncZ = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 80: // P - Index1 by divisions
		{
			double newIndexSize = GetSerialFloat(serialId);
			configMain.size_Index1 = (configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) / (newIndexSize);
			configMain.type_Index1 = BY_DIVISIONS;
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 81: // Q - Index1 counter clockwise
		{
			// Check for 0 size
			if (configMain.size_Index1 > 0)
			{
				//DoIndex1();
				IndexSpindle(ID_INDEX_1, DIR_CCW);
			}
			break;
		}
		case 82: // R - Index1 Clockwise
		{
			// Check for 0 size
			if (configMain.size_Index1 > 0)
			{
				IndexSpindle(ID_INDEX_1, DIR_CW);
			}
			break;
		}
		case 83: // S - Set Index1 speed
		{
			configMain.speedPercent_Index1 = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 84: // T - Rose: n
		{
			configRose.n = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rose, configRose);
			break;
		}
		case 85: //U - MoveZ total distance
		{
			// Total distance only valid for current session
			distanceTotal_MoveZ = GetSerialFloat(serialId);

			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 86: // V - Sync Axis Direction
		{
			int newDir = GetSerialInteger();
			switch (newDir)
			{
				case 0:
				case 48:
				{
					configMain.axisDir_SyncZ = 0;
					break;
				}
				case 1:
				case 49:
				{
					configMain.axisDir_SyncZ = 1;
					break;
				}
			}

			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 87: // W - Set Index1 type to Degrees
		{
			double newIndexSize = GetSerialFloat(serialId);
			configMain.size_Index1 = (configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) * (newIndexSize / 360);
			configMain.type_Index1 = BY_DEGREES;
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 88:  // X - Update Index1 size
		{
			double newIndexSize = GetSerialFloat(serialId);

			// Default: Divisions
			if (configMain.type_Index1 == BY_DEGREES) // Degrees
			{
				configMain.size_Index1 = (configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) * (newIndexSize / 360);
			}
			else
			{
				configMain.size_Index1 = (configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) / (newIndexSize);
			}

			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 89:  // Y - Rose: d
		{
			configRose.d = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rose, configRose);
			break;
		}
		case 90:  // Z - Main Axis Clockwise
		{
			// Get current axis speed 
			configMain.speedPercent_Axis_Z = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);
			RunTwoSteppers_SpZ(
				DIR_CW,
				DIR_CW,
				0);
			break;
		}
		case 91:  // [ - Z Axis Counterclockwise
		{
			// Set Speed
			configMain.speedPercent_Axis_Z = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);

			// Run
			accelStep_Axis_Z.enableOutputs();
			RunTwoSteppers_SpZ(
				DIR_CCW,
				DIR_CW,
				0);
			break;
		}
		case 92: // \ - *** Don't Use***
		{
			break;
		}
		case 93: // ] - Sync Stop
		{
			//Implemented in Run methods
			break;
		}
		case 94: // ^ - Enable Z axis stepper
		{
			int axisId = GetSerialInteger();
			switch (axisId)
			{
				case 1:
				{
					accelStep_Axis_Z.enableOutputs();
					break;
				}
				case 2:
				{
	//#ifndef TWO_AXES_V2
					accelStep_Axis_X.enableOutputs();
	//#endif // TWO_AXES_V2
					break;
				}
				case 3:
				{
	//#ifdef FOUR_AXES
					accelStep_Axis_B.enableOutputs();
	//#endif // FOUR_AXES
					break;
				}
			}
			break;
		}
		case 95: //_ - Disable Z axis stepper
		{
			int axisId = GetSerialInteger();
			switch (axisId)
			{
				case 1:
				{
					accelStep_Axis_Z.disableOutputs();
					break;
				}
				case 2:
				{
	//#ifndef TWO_AXES_V2
					accelStep_Axis_X.disableOutputs();
	//#endif // TWO_AXES_V2
					break;
				}
				case 3:
				{
	//#ifdef FOUR_AXES
					accelStep_Axis_B.disableOutputs();
	//#endif // FOUR_AXES
					break;
				}
			}
			break;
		}
		case 96: // ' - *** Don't Use***
		{
			break;
		}
		case 97: // a - Z Axis Speed
		{
			configMain.speedPercent_Axis_Z = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 98: // b - Test EEPROM settings Setup screen
		{
			TestEEPROMSetup();
			break;
		}
		case 99: // c - Z Axis Stop
		{
			Serial1.print("pageSpZ.va2.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			break;
		}
		case 100: // d - Sp2 Clockwise
		{
			// Set Speed
			configMain.speedPercent_Sp2 = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);

			// Run
			RunStepper(DIR_CW, accelStep_Spindle, configSp.accel_Spindle, configMain.speedPercent_Sp2, configSp.maxSpd_Spindle);
			break;
		}
		case 101: // e - Sp2 CounterClockwise
		{
			// Set Speed
			configMain.speedPercent_Sp2 = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);

			// Run
			RunStepper(DIR_CCW, accelStep_Spindle, configSp.accel_Spindle, configMain.speedPercent_Sp2, configSp.maxSpd_Spindle);
			break;
		}
		case 102: // f - Set Sp2 Speed
		{
			configMain.speedPercent_Sp2 = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 103: // g - Rose: Spindle MaxSpd
		{
			configRose.maxSpd_Spindle = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rose, configRose);
			break;
		}
		case 104: // h - Rose: Spindle Accel
		{
			configRose.accel_Spindle = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rose, configRose);
			break;
		}
		case 105: // i - 
		{
			int selection = GetSerialInteger();

			switch (selection)
			{
				case 0:
				case 30:
				{
					DoRosePattern_Z();
					break;
				}
				case 1:
				case 31:
				{
					DoRosePattern_X();
					break;
				}
				case 2:
				case 32:
				{
					DoRosePattern_SpindleZ();
					break;
				}
				case 3:
				case 33:
				{
					//DoRosePattern_SpindleX();
					break;
				}
			}

			break;
		}
		case 106: // j - Move Z counter clockwise
		{
			configMain.speedPercent_MoveZ = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);

			// Run
			MoveAxis(ID_AXIS_Z, DIR_CCW);
			break;
		}
		case 107: // k - Z Spindle Clockwise
		{
			// Set Speed
			configMain.speedPercentSpindle_SpZ = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);

			RunTwoSteppers_SpZ(
				DIR_CW,
				DIR_CW,
				3);
			break;
		}
		case 108: // l - Z spindle counter clockwise
		{
			// Set Speed
			configMain.speedPercentSpindle_SpZ = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);

			// Run
			RunTwoSteppers_SpZ(
				DIR_CCW,
				DIR_CCW,
				3);

			break;
		}
		case 109: // m - 
		{
			break;
		}
		case 110: // n - Set SpZ (Main) Spindle Speed
		{
			configMain.speedPercentSpindle_SpZ = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 111: // o - Rose: Spindle Steps per rev
		{
			configRose.speedPercent_Axis = GetSerialInteger();
			EEPROM.put(eePromAddress_Rose, configRose);
			break;
		}
		case 112: // p - Z/Sync Spindle Microsteps
		{
			configMain.microsteps_Spindle = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
			SetMicrosteppingMode(configMain.microsteps_Spindle, PIN_SPINDLE_MS0, PIN_SPINDLE_MS1, PIN_SPINDLE_MS2);
			break;
		}
		case 113: // q - Move Z clockwise
		{
			configMain.speedPercent_MoveZ = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);

			// Run
			MoveAxis(ID_AXIS_Z,DIR_CW);

			break;
		}
		case 114: // r - Main/Sync/Sp2 Spindle FullSteps
		{
			configMain.steps360_Spindle = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 115: // s - Main/Sync/Sp2 Spindle Gear Ratio
		{
			configMain.gearRatio_Spindle = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
//#ifdef FOUR_AXES
		case 116: // t - B axis Acceleration
		{
			pageCallerId = GetSerialIntegerOnly();
			switch (pageCallerId)
			{
				case PAGE_SPB:
				{
					configSpB.accel_Axis_B = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SpB, configSpB);
					break;
				}
			}
			break;
		}
		case 117: // u - B axis Max Speed
		{
			pageCallerId = GetSerialIntegerOnly();
			switch (pageCallerId)
			{
				case PAGE_SPB:
				{
					configSpB.maxSpd_Axis_B = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SpB, configSpB);
					break;
				}
			}
			break;
		}
		case 118: // v - B axis microsteps
		{
			configMain.microsteps_Axis_B = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
			//SetMicrosteppingMode(configMain.microsteps_Axis_B, PIN_AXIS_B_MS0, PIN_AXIS_B_MS1, PIN_AXIS_B_MS2);
			break;
		}
		case 119: // w - B axis Steps/360 
		{
			configMain.steps360_Axis_B = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 120: // x - B axis Gear Ratio
		{
			configMain.gearRatio_AxisB = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
//#endif // FOUR_AXES
		case 121: // y - Z Axis Microsteps
		{
			configMain.microsteps_Axis_Z = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
			SetMicrosteppingMode(configMain.microsteps_Axis_Z, PIN_AXIS_Z_MS0, PIN_AXIS_Z_MS1, PIN_AXIS_Z_MS2);
			break;
		}
		case 122: // z - Z Axis FullSteps
		{
			configMain.steps360_Axis_Z = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 123: // { - Z Distance per revolution
		{
			configMain.distancePerRev_AxisZ = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 124: // | - Z Axis Acceleration
		{
			pageCallerId = GetSerialIntegerOnly();
			switch (pageCallerId)
			{
				case PAGE_SPZ:
				{
					configSpZ.accel_Axis_Z = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SpZ, configSpZ);
					break;
				}
				case PAGE_SYNCZ:
				{
					configSyncZ.accel_Axis_Z = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SyncZ, configSyncZ);
					break;
				}
				case PAGE_REC1_Z:
				{
					configRec1_Z.accel_Axis_Z = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Rec1_Z, configRec1_Z);
					break;
				}
				case PAGE_REC1_S:
				{
					configRec1_S.accel_Axis_Z = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Rec1_S, configRec1_S);
					break;
				}
				case PAGE_MOVEZ:
				{
					configMoveZ.accel_Axis_Z = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_MoveZ, configMoveZ);
					break;
				}
			}

			break;
		}
		case 125: // } - Rose: Spindle Axis Accel
		{
			configRose.accel_Axis_Z = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rose, configRose);
			break;
		}
		case 126: // ~ - Test EEPROM settings Config screen
		{
			TestEEPROMConfig();
			break;
		}
		case 127: // Don't use
		{
			break;
		}
		case 128: // Don't use
		{
			break;
		}
		case 129: // Don't use
		{
			break;
		}
		case 130: // Don't use
		{
			break;
		}
		case 131: // Don't use
		{
			break;
		}
		case 132: // Don't use
		{
			break;
		}
		case 133: // Don't use
		{
			break;
		}
		case 134: // Don't use
		{
			break;
		}
		case 135: // Don't use
		{
			break;
		}
		case 136: // Don't use
		{
			break;
		}
		case 137: // Don't use
		{
			break;
		}
		case 138: // Don't use
		{
			break;
		}
		case 139: // Don't use
		{
			break;
		}
		case 140: // Don't use
		{
			break;
		}
		case 141: // Don't use
		{
			break;
		}
		case 142: // Don't use
		{
			break;
		}
		case 143: // Don't use
		{
			break;
		}
		case 144: // Don't use
		{
			break;
		}
		case 145: // Don't use
		{
			break;
		}
		case 146: // Don't use
		{
			break;
		}
		case 147: // Don't use
		{
			break;
		}
		case 148: // Don't use
		{
			break;
		}
		case 149: // Don't use
		{
			break;
		}
		case 150: // Don't use
		{
			break;
		}
		case 151: // Don't use
		{
			break;
		}
		case 152: // Don't use
		{
			break;
		}
		case 153: // Don't use
		{
			break;
		}
		case 154: // Don't use
		{
			break;
		}
		case 155: // Don't use
		{
			break;
		}
		case 156: // Don't use
		{
			break;
		}
		case 157: // Don't use
		{
			break;
		}
		case 158: // Don't use
		{
			break;
		}
		case 159: // Don't use
		{
			break;
		}
		case 160: // Don't use
		{
			break;
		}
		case 161: // ¡ - Sp-X spindle speed
		{
			configMain.speedPercentSpindle_SpX = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 162: // ¢ - Sp-X axis speed
		{
			configMain.speedPercent_Axis_X = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 163: // £ - Sp-X spindle CCW
		{
			// Set Speed
			configMain.speedPercentSpindle_SpX = GetSerialInteger();
			accelStep_Spindle.setCurrentPosition(0);
			EEPROM.put(eePromAddress_Setup, configMain);

			RunTwoSteppers_SpX(
				DIR_CCW,
				DIR_CCW,
				3);
			break;
		}
		case 164: // ¤ - Sp-X spindle CW
		{
			// Set Speed
			configMain.speedPercentSpindle_SpX = GetSerialInteger();
			accelStep_Spindle.setCurrentPosition(0);
			EEPROM.put(eePromAddress_Setup, configMain);

			RunTwoSteppers_SpX(
				DIR_CW,
				DIR_CW,
				3);
			break;
		}
		case 165: // ¥ - Sp-X Axis CCW
		{
			// Set Speed
			configMain.speedPercent_Axis_X = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);

			// Run
			accelStep_Axis_X.enableOutputs();
			RunTwoSteppers_SpX(
				DIR_CCW,
				DIR_CW,
				0);
			break;
		}
		case 166: // ¦ - Sp-X Axis CW
		{
			// Set Speed
			configMain.speedPercent_Axis_X = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);

			// Run
			accelStep_Axis_X.enableOutputs();
			RunTwoSteppers_SpX(
				DIR_CW,
				DIR_CW,
				0);
			break;
		}
//#ifdef FOUR_AXES

		case 167: // § - Sp-B spindle speed
		{
			configMain.speedPercentSpindle_SpB = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 168: // ¨ - Sp-B axis speed
		{
			configMain.speedPercent_Axis_B = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 169: // © - Sp-B spindle CCW
		{
			// Set Speed
			configMain.speedPercentSpindle_SpB = GetSerialInteger();
			accelStep_Spindle.setCurrentPosition(0);
			EEPROM.put(eePromAddress_Setup, configMain);

			RunTwoSteppers_SpB(
				DIR_CCW,
				DIR_CCW,
				3);
			break;
		}
		case 170: // ª - Sp-B spindle CW
		{
			// Set Speed
			configMain.speedPercentSpindle_SpB = GetSerialInteger();
			accelStep_Spindle.setCurrentPosition(0);
			EEPROM.put(eePromAddress_Setup, configMain);

			RunTwoSteppers_SpB(
				DIR_CW,
				DIR_CW,
				3);
			break;
		}
		case 171: // « - Sp-B Axis CCW
		{
			// Set Speed
			configMain.speedPercent_Axis_B = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);

			// Run
			accelStep_Axis_B.enableOutputs();
			RunTwoSteppers_SpB(
				DIR_CCW,
				DIR_CW,
				0);
			break;
		}
		case 172: // ¬ - Sp-B Axis CW
		{
			// Set Speed
			configMain.speedPercent_Axis_B = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);

			// Run
			accelStep_Axis_B.enableOutputs();
			RunTwoSteppers_SpB(
				DIR_CW,
				DIR_CW,
				0);
			break;
		}
		//case 173: // Don't use 
		//{
		//	break;
		//}
//#endif // FOUR_AXES
//#ifndef TWO_AXES_V2
		case 174: // ® - SyncX axis speed
		{
			configMain.speedPercentAxis_SyncX = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 175: // ¯ - SyncX spindle speed
		{
			configMain.speedPercentSpindle_SyncX = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 176: // ° - SyncX Helix direction
		{
			configMain.helixType_SyncX = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 177: // ± - SyncX In
		{
			int helixType = GetSerialInteger();
			int directionSpindle = 1;
			int directionAxis = 1;

			switch (helixType)
			{
				case 0: // Left for right handed X axis leadscrew, Right for left handed X axis leadscrew.
				case 48:
				{
					directionAxis = -1; // CCW
					directionSpindle = -1; //CCW
					break;
				}
				case 1: //Right for right handed X axis leadscrew, Left for left handed X axis leadscrew.
				case 49:
				{
					directionAxis = -1; // CCW
					directionSpindle = 1; //CW
					break;
				}
			}

			Sync_SpindleX(directionSpindle, directionAxis);
			break;
		}
		case 178: // ² - SyncX Out
		{
			int helixType = GetSerialInteger();
			int directionSpindle = 1;
			int directionAxis = 1;

			switch (helixType)
			{
				case 0:// Left for right handed X axis leadscrew, Right for left handed X axis leadscrew.
				case 48:
				{
					directionAxis = 1; // CW
					directionSpindle = 1; // CW
					break;
				}
				case 1: //Right for right handed X axis leadscrew, Left for left handed X axis leadscrew.
				case 49:
				{
					directionAxis = 1; // CW
					directionSpindle = -1; // CCW
					break;
				}
			}
			
			Sync_SpindleX(directionSpindle, directionAxis);
			break;
		}
		case 179: // ³ - MvX Speed 
		{
			configMain.speedPercent_MoveX = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		//case 180: // Don't use
		//{
		//	break;
		//}
		case 181: // µ - MvX Distance (Text)
		{
			// Set Distance
			float newDistance = GetSerialFloat(serialId);
			configMain.distance_MoveX = newDistance;
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 182: // ¶ - MvX Total
		{
			// Total distance only valid for current session
			distanceTotal_MoveX = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 183: // · - MvX CCW
		{
			configMain.speedPercent_MoveX = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);

			// Run
			//MoveAxisX(DIR_CCW);
			MoveAxis(ID_AXIS_X, DIR_CCW);
			break;
		}
		case 184: // ¸ - MvX CW
		{
			configMain.speedPercent_MoveX = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);

			// Run
			//MoveAxisX(DIR_CW);
			MoveAxis(ID_AXIS_X, DIR_CW);
			break;
		}
		case 185: // ¹ - X Max speed
		{
			pageCallerId = GetSerialIntegerOnly();
			switch (pageCallerId)
			{
				case PAGE_SPX:
				{
					configSpX.maxSpd_Axis_X = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SpX, configSpX);
					break;
				}
				case PAGE_SYNCX:
				{
					configSyncX.maxSpd_Axis_X = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SyncX, configSyncX);
					break;
				}
				case PAGE_MOVEX:
				{
					configMoveX.maxSpd_Axis_X = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_MoveX, configMoveX);
					break;
				}
			}
			break;
		}
		case 186: // º - X Accel
		{
			pageCallerId = GetSerialIntegerOnly();
			switch (pageCallerId)
			{
				case PAGE_SPX:
				{
					configSpX.accel_Axis_X = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SpX, configSpX);
					break;
				}
				case PAGE_SYNCX:
				{
					configSyncX.accel_Axis_X = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SyncX, configSyncX);
					break;
				}
				case PAGE_MOVEX:
				{
					configMoveX.accel_Axis_X = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_MoveX, configMoveX);
					break;
				}
			}
			break;
		}
		case 187: // » - X microsteps
		{
			configMain.microsteps_Axis_X = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
			SetMicrosteppingMode(configMain.microsteps_Axis_X, PIN_AXIS_X_MS0, PIN_AXIS_X_MS1, PIN_AXIS_X_MS2);
			break;
		}
		case 188: // ¼ - X Full Steps
		{
			configMain.steps360_Axis_X = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 189: // ½ - X Distance per revolution (Text)
		{
			configMain.distancePerRev_AxisX = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);

			//configs dpr;
			//EEPROM.get(eePromAddress_Setup, dpr);
			////t7
			//Serial1.print("pageSetup.t7.txt=");
			//Serial1.write(0x22);
			//Serial1.print(dpr.distancePerRev_AxisX);
			//Serial1.write(0x22);
			//Serial1.write(0xff);
			//Serial1.write(0xff);
			//Serial1.write(0xff);
			//delay(50);
			//Serial1.print("pageSetup.t7.txt=");
			//Serial1.write(0x22);
			//Serial1.print(dpr.distancePerRev_AxisX);
			//Serial1.write(0x22);
			//Serial1.write(0xff);
			//Serial1.write(0xff);
			//Serial1.write(0xff);
			//delay(50);
			break;
		}
//#endif // TWO_AXES_V2
		case 190: // ¾ - Index1 Source:Fixed
		{
			configMain.source_Index1 = FIXED;
			EEPROM.put(eePromAddress_Setup, configMain);

			break;
		}
		case 191: // ¿ - Index1 Source: Filename
		{
			// Not implemented 
			//filename_Index1 = GetSerial1String();
			//configMain.source_Index1 = FILE_SD;
			//EEPROM.put(eePromAddress_Main, configMain);
			break;
		}
		case 192: // À - Revolutions_SyncZ
		{
			configMain.revolutionsSyncZ_Spindle = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 193: // Á - Distance_SyncZ
		{
			configMain.distanceSyncZ = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 194: // À - Revolutions_SyncX
		{
			configMain.revolutionsSyncX_Spindle = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 195: // Á - Distance_SyncZ
		{
			configMain.distanceSyncX = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 196: // Ä - Index2_SourceFixed
		{
			configMain.source_Index2 = GetSerialInteger();
			if (configMain.source_Index2 == 49)
			{
				configMain.source_Index2 = 1;
			}
			else if (configMain.source_Index2 == 48)
			{
				configMain.source_Index2 = 0;
			}
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 197: // Å - Index2_SourceFile
		{
			GetIndex2FilenameFromSerial1();
			configMain.source_Index2 = FILE_SD;
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 198: // Æ - Do Rec1_Z 
		{
			int waveDir = GetSerialFloat(serialId);

			// In: -1  Out: 1
			if (waveDir != 1)
			{
				waveDir = -1;
			}

			DoRec1_Z(waveDir);
			break;
		}
		case 199: // Ç - Rose: Spindle Axis Amplitude
		{
			configRose.amplitude_Axis_Z = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rose, configRose);
			break;
		}
		case 200: // È - Rose: Show EEPROM settings
		{
			TestEEPROMRose();
			break;
		}
		case 201: // É - DoRosePattern_X
		{
			DoRosePattern_X();
			break;
		}
		case 202: // Ê - Rec1_Z Axis MaxSpeed 
		{
			pageCallerId = GetSerialIntegerOnly();
			switch (pageCallerId)
			{
				case PAGE_SPZ:
				{
					configSpZ.maxSpd_Axis_Z = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SpZ, configSpZ);
					break;
				}
				case PAGE_SYNCZ:
				{
					configSyncZ.maxSpd_Axis_Z = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SyncZ, configSyncZ);
					break;
				}

				case PAGE_REC1_Z:
				{
					configRec1_Z.maxSpd_Axis_Z = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Rec1_Z, configRec1_Z);
					break;
				}
				case PAGE_REC1_S:
				{
					configRec1_S.maxSpd_Axis_Z = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Rec1_S, configRec1_S);
					break;
				}
				case PAGE_MOVEZ:
				{
					configMoveZ.maxSpd_Axis_Z = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_MoveZ, configMoveZ);
					break;
				}
			}

			break;
		}
		case 203: // Ì - Rec1_Z Repeats (Count)
		{
			configMain.waves_Rec1_Z = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 204: // Ë - Rec1_Z Distance
		{
			configMain.distance_Rec1_Z = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 205: // Í - Rec1_Z Amplitude
		{
			configMain.amplitude_Rec1_Z = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 206: // Î -  Return
		{
			// Spindle and Z axis
			pageCallerId = GetSerialInteger();
			ReturnToStartPosition(0);
			break;
		}
		case 207: // Ï - Do Rec1_S
		{
			int waveDir = GetSerialFloat(serialId);

			// In: -1  Out: 1
			if (waveDir != 1)
			{
				waveDir = -1;
			}
			DoRec1_Spindle(waveDir);
			break;
		}
		case 208: // Ð - Rec1_Z Spindle Speed percentage
		{
			configMain.speedPercentSpindle_Rec1Z = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 209: // Ñ - Rec1_Z Axis Speed percentage
		{
			configMain.speedPercentAxis_Rec1Z = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 210: // Ò - Rec1_Spindle Spindle Speed percentage
		{
			configMain.speedPercentSpindle_Rec1S = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 211: // Ó - Rec1_Spindle Axis Speed percentage 
		{
			configMain.speedPercentAxis_Rec1S = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 212: // Ô - Rec1_S Repeats (Count)
		{
			configMain.waves_Rec1_S = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 213: // Õ - Rec1_S Distance
		{
			configMain.degrees_Rec1_S = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 214: // Ö - Rec1_S Amplitude
		{
			configMain.amplitude_Rec1_S = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 215: // × - Return
		{
			// Spindle and X axis
			pageCallerId = GetSerialInteger();
			ReturnToStartPosition(1);
			break;
		}
		case 216: // Ø - Enable Spindle (High or Low)
		{
			int enableSpindle = GetSerialInteger();

			// true (1): LOW  false (0): HIGH
			enableSpindle >= 1 ? (configMain.enable_Spindle = true) : (configMain.enable_Spindle = false);
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 217: // Ù - Enable Z (High or Low)
		{
			int enableZ = GetSerialInteger();

			// true (1): LOW  false (0): HIGH
			enableZ >= 1 ? (configMain.polarity_Axis_Z = true) : (configMain.polarity_Axis_Z = false);
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 218: // Ú - Enable X (High or Low)
		{
			int enableX = GetSerialInteger();

			// true (1): LOW  false (0): HIGH
			enableX >= 1 ? (configMain.polarity_Axis_X = true) : (configMain.polarity_Axis_X = false);
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 219: // Û - Enable B (High or Low)
		{
			int enableB = GetSerialInteger();

			// true (1): LOW  false (0): HIGH
			enableB >= 1 ? (configMain.polarity_Axis_B = true) : (configMain.polarity_Axis_B = false);
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 220: // Ü - Limit switch Z Min
		{
			configMain.limit_Min_Z = (int)GetSerial1Float();
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 221: // Ý - Limit switch Z Max
		{
			configMain.limit_Max_Z = (int)GetSerial1Float();
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 222: // Þ - Limit switch X Min
		{
			configMain.limit_Min_X = (int)GetSerial1Float();
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 223: // ß - Limit switch X Max
		{
			configMain.limit_Max_X = (int)GetSerial1Float();
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 224: // à - Limit switch B Min
		{
			configMain.limit_Min_B = (int)GetSerial1Float();
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 225: // á - Limit switch B Max
		{
			configMain.limit_Max_B = (int)GetSerial1Float();
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 226: // â - Rose: X Axis MaxSpd
		{
			configRose.maxSpd_Axis_X = (int)GetSerial1Float();
			EEPROM.put(eePromAddress_Rose, configRose);
			break;
		}
		case 227: // ã - Rose: X Axis Accel
		{
			configRose.accel_Axis_X = (int)GetSerial1Float();
			EEPROM.put(eePromAddress_Rose, configRose);
			break;
		}
		case 228: // ä - Rose: X Axis Amplitude
		{
			configRose.amplitude_Axis_X = GetSerial1Float();
			EEPROM.put(eePromAddress_Rose, configRose);
			break;
		}
		case 229: // å - Rose: Spindle Amplitude
		{
			configRose.amplitude_Spindle = GetSerial1Float();
			EEPROM.put(eePromAddress_Rose, configRose);
			break;
		}
		case 230: // å - Rose: DoRoseSpindle
		{
			DoRosePattern_SpindleZ();
			break;
		}
		case 231: // ç - Rose: Return Z and Spindle
		{
			int selection = GetSerialInteger();
			pageCallerId = PAGE_ROSE;
			ReturnToStartPosition(selection);
			break;
		}
		case 232: // è - Return Spindle MaxSpd
		{
			configMain.maxSpd_Return_Spindle = GetSerial1Float();
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 233: // é - Return Spindle Accel
		{
			configMain.accel_Return_Spindle = GetSerial1Float();
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 234: // ê - Return Spindle Accel
		{
			configMain.maxSpd_Return_Axis = GetSerial1Float();
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 235: // ë - Return Spindle Accel
		{
			configMain.accel_Return_Axis = GetSerial1Float();
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 236:
		{
			endPosition_Axis = 0;
			endPosition_Spindle = 0;
			teensyStep_Spindle.setPosition(0);
			teensyStep_Axis_Z.setPosition(0);
			teensyStep_Axis_X.setPosition(0);
		}
		default:
			SerialPrint("Input:");
			SerialPrintLn("N/A");
		}
		delay(50);
	}
} // End loop()
