//#pragma once
/* *****************************************************************
* 4Rose_V007 main entry
* Author: Edward French
******************************************************************/
#include "math.h"
#include <string>
#include <EEPROM.h>
#include "4RoseDefines.h"
#include "SD.h"
#include "SPI.h"
#include "AccelStepper.h"
#include "TeensyStep.h"

//==================================================================
// Pin assignments:  See 4RoseDefines.h
//==================================================================
	// Initialize AccelStepper object
AccelStepper accelStep_Spindle(AccelStepper::DRIVER, PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
AccelStepper accelStep_Axis_Z(AccelStepper::DRIVER, PIN_AXIS_Z_STEP, PIN_AXIS_Z_DIR);

// TeensyStep initialization
Stepper stepperSpindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
Stepper stepperAxis_Z(PIN_AXIS_Z_STEP, PIN_AXIS_Z_DIR);
Stepper stepperAxis_X(PIN_AXIS_X_STEP, PIN_AXIS_X_DIR);
Stepper stepperAxis_B(PIN_AXIS_B_STEP, PIN_AXIS_B_DIR);

// TeensyStep controllers
RotateControl rotateController1;

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

	// Initialize Nextion LCD for 115200 baud
	// Note: Nextion requires 3 0xFF bytes to signal end of transmission
	Serial3.begin(115200); //Nextion Serial baud rate set in Nextion pageSpB Preinitialize Event tab
	Serial3.print("bauds=115200");
	Serial3.write(0xFF);
	Serial3.write(0xFF);
	Serial3.write(0xFF);
	delay(50);
	Serial3.print("bauds=115200");
	Serial3.write(0xFF);
	Serial3.write(0xFF);
	Serial3.write(0xFF);
	delay(50);
	Serial3.print("bkcmd=0");  // Set Nextion to return NO replies to each command
	Serial3.write(0xFF);
	Serial3.write(0xFF);
	Serial3.write(0xFF);
	delay(50);


	// Update with values from EEProm
	EEPROM.get(eePromAddress_Setup, configMain);

	EEPROM.get(eePromAddress_SpZ, configSpZ);
	EEPROM.get(eePromAddress_SpX, configSpX);
	EEPROM.get(eePromAddress_SpB, configSpB);
	EEPROM.get(eePromAddress_SyncZ, configSyncZ);
	EEPROM.get(eePromAddress_SyncX, configSyncX);
	EEPROM.get(eePromAddress_Sp, configSp);
	EEPROM.get(eePromAddress_Index1, configIndex1);
	EEPROM.get(eePromAddress_Index2, configIndex2);
	EEPROM.get(eePromAddress_MoveZ, configMoveZ);
	EEPROM.get(eePromAddress_MoveX, configMoveX);
	EEPROM.get(eePromAddress_Rose, configRose);
	EEPROM.get(eePromAddress_Reci, configReci);

	// Config as well as all other EEProm settings must be run from Nextion whenever Teensy is updated.  
	// EEProm may contain invalid settings otherwise.

	pinMode(PIN_SPINDLE_ENABLE, OUTPUT);
	pinMode(PIN_AXIS_Z_ENABLE, OUTPUT);
	pinMode(PIN_AXIS_X_ENABLE, OUTPUT);

	digitalWrite(PIN_SPINDLE_ENABLE, HIGH);  // Disable 
	digitalWrite(PIN_AXIS_Z_ENABLE, HIGH);  // Disable 
	digitalWrite(PIN_AXIS_X_ENABLE, HIGH);  // Disable 

	// Set Microstepping mode
	pinMode(PIN_SPINDLE_MS0, OUTPUT);
	pinMode(PIN_SPINDLE_MS1, OUTPUT);
	pinMode(PIN_SPINDLE_MS2, OUTPUT);
	pinMode(PIN_AXIS_Z_MS0, OUTPUT);
	pinMode(PIN_AXIS_Z_MS1, OUTPUT);
	pinMode(PIN_AXIS_Z_MS2, OUTPUT);
	SetMicrosteppingMode(configMain.microsteps_Spindle, PIN_SPINDLE_MS0, PIN_SPINDLE_MS1, PIN_SPINDLE_MS2);
	SetMicrosteppingMode(configMain.microsteps_Axis_Z, PIN_AXIS_Z_MS0, PIN_AXIS_Z_MS1, PIN_AXIS_Z_MS2);
	///////////////////////////////////////////////////
	// Configure AccelStepper for Z axis bug
		// Set the enable pin for the stepper motor
	accelStep_Spindle.setEnablePin(PIN_SPINDLE_ENABLE);

	// Set pins "inverted", HIGH == off
	accelStep_Spindle.setPinsInverted(false, false, true);

	// Disable the spindle stepper motor
	accelStep_Spindle.disableOutputs();

	// Z Axis stepper motor initialization
	// Configure Enable Z Axis stepper
	accelStep_Axis_Z.setEnablePin(PIN_AXIS_Z_ENABLE);
	accelStep_Axis_Z.setPinsInverted(false, false, true);

	// Disable the Z Axis stepper motor (enable in run method)
	accelStep_Axis_Z.disableOutputs();
	///////////////////////////////////////////////////
#ifdef FOUR_AXES
	pinMode(PIN_AXIS_B_ENABLE, OUTPUT);
	digitalWrite(PIN_AXIS_B_ENABLE, HIGH);  // Disable 

	// Set Microstepping mode
	SetMicrosteppingMode(configMain.microsteps_Axis_B, PIN_AXIS_B_MS0, PIN_AXIS_B_MS1, PIN_AXIS_B_MS2);
	stepperAxis_B.setStepPinPolarity(configMain.polarity_Axis_B ? (LOW) : (HIGH)); // driver expects active low pulses
#endif //FOUR_AXES
#ifndef TWO_AXES_V2
	pinMode(PIN_AXIS_Z_MS2, OUTPUT);
	pinMode(PIN_AXIS_X_MS0, OUTPUT);
	pinMode(PIN_AXIS_X_MS1, OUTPUT);
	SetMicrosteppingMode(configMain.microsteps_Axis_X, PIN_AXIS_X_MS0, PIN_AXIS_X_MS1, PIN_AXIS_X_MS2);

	// Configure TeensyStep motors
	stepperAxis_X.setStepPinPolarity(configMain.polarity_Axis_X ? (LOW) : (HIGH)); // driver expects active low pulses

#endif // TWO_AXES_V2
	stepperSpindle.setStepPinPolarity(configMain.polarity_Spindle ? (LOW) : (HIGH)); // DRV8825 driver expects active low pulses
	stepperAxis_Z.setStepPinPolarity(configMain.polarity_Axis_Z ? (LOW) : (HIGH)); // driver expects active low pulses

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
	//bool sdValid = false;
#ifdef FOUR_AXES
	SD.begin(BUILTIN_SDCARD);
#else
	SD.begin(PIN_SPI_CS);
#endif // FOUR_AXES

	for (int i = 0; i < 3; i++) // Verify Teensy is operational
	{
		digitalWrite(LED_BUILTIN, HIGH);
		delay(50);
		digitalWrite(LED_BUILTIN, LOW);
		delay(300);
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
	// Select serial port: Serial = USB connected, Serial3 = Nextion LCD connected
	if (Serial.available() > 0)
	{
		// USB connected 
		serialId = 0;
	}
	else if (Serial3.available() > 0)
	{
		//Nextion connected
		serialId = 1;
	}
	serialId = 1;

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
#ifdef DEBUG
			Serial.print("pageCallerId:");
			Serial.println(pageCallerId);
#endif // DEBUG
			switch (pageCallerId)
			{
				case PAGE_SPZ:
				{
					configSpZ.maxSpd_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SpZ, configSpZ);
#ifdef DEBUG
					Serial.print("maxSpd:");
					Serial.println(configSpZ.maxSpd_Spindle);
#endif // DEBUG
					break;
				}
				case PAGE_SPX:
				{
					configSpX.maxSpd_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SpX, configSpX);
#ifdef DEBUG
					Serial.print("maxSpd:");
					Serial.println(configSpX.maxSpd_Spindle);
#endif // DEBUG
					break;
				}
				case PAGE_SPB:
				{
					configSpB.maxSpd_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SpB, configSpB);
#ifdef DEBUG
					Serial.print("maxSpd:");
					Serial.println(configSpB.maxSpd_Spindle);
#endif // DEBUG
					break;
				}
				case PAGE_SYNCZ:
				{
					configSyncZ.maxSpd_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SyncZ, configSyncZ);
#ifdef DEBUG
					Serial.print("maxSpd:");
					Serial.println(configSyncZ.maxSpd_Spindle);
#endif // DEBUG
					break;
				}
				case PAGE_SYNCX:
				{
					configSyncX.maxSpd_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SyncX, configSyncX);
#ifdef DEBUG
					Serial.print("maxSpd:");
					Serial.println(configSyncX.maxSpd_Spindle);
#endif // DEBUG
					break;
				}
				case PAGE_RECI_AXIAL_Z:
				case PAGE_RECI_RADIAL_Z:
				case PAGE_RECI_AXIAL_X:
				case PAGE_RECI_RADIAL_X:
				{
					configReci.maxSpd_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Reci, configReci);
#ifdef DEBUG
					Serial.print("maxSpd:");
					Serial.println(configReci.maxSpd_Spindle);
#endif // DEBUG
					break;
				}
				case PAGE_SP:
				{
					switch (configSp.activeAxis)
					{
						case ID_AXIS_Z:
						{
							configSp.maxSpd_Axis_Z = GetSerialFloat(serialId);
#ifdef DEBUG
							Serial.print("maxSpd:");
							Serial.println(configSp.maxSpd_Axis_Z);
#endif // DEBUG
							break;
						}
						case ID_AXIS_X:
						{
							configSp.maxSpd_Axis_X = GetSerialFloat(serialId);
#ifdef DEBUG
							Serial.print("maxSpd:");
							Serial.println(configSp.maxSpd_Axis_X);
#endif // DEBUG
							break;
						}
						case ID_AXIS_B:
						{
							configSp.maxSpd_Axis_B = GetSerialFloat(serialId);
#ifdef DEBUG
							Serial.print("maxSpd:");
							Serial.println(configSp.maxSpd_Axis_B);
#endif // DEBUG
							break;
						}
						case ID_SPINDLE:
						{
							configSp.maxSpd_Spindle = GetSerialFloat(serialId);
#ifdef DEBUG
							Serial.print("maxSpd:");
							Serial.println(configSp.maxSpd_Spindle);
#endif // DEBUG
							break;
						}
					}

					
					EEPROM.put(eePromAddress_Sp, configSp);

					break;
				}
				case PAGE_INDEX1:
				{
					configIndex1.maxSpd = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Index1, configIndex1);
#ifdef DEBUG
					Serial.print("maxSpd:");
					Serial.println(configIndex1.maxSpd);
#endif // DEBUG
					break;
				}
				case PAGE_INDEX2:
				{
					configIndex2.maxSpd = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Index2, configIndex2);
#ifdef DEBUG
					Serial.print("maxSpd:");
					Serial.println(configIndex2.maxSpd);
#endif // DEBUG
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
#ifdef DEBUG
					Serial.print("accel_Spindle:");
					Serial.println(configSpZ.accel_Spindle);
#endif // DEBUG
					break;
				}
				case PAGE_SPX:
				{
					configSpX.accel_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SpX, configSpX);
#ifdef DEBUG
					Serial.print("accel_Spindle:");
					Serial.println(configSpX.accel_Spindle);
#endif // DEBUG
					break;
				}
				case PAGE_SPB:
				{
					configSpB.accel_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SpB, configSpB);
#ifdef DEBUG
					Serial.print("accel_Spindle:");
					Serial.println(configSpB.accel_Spindle);
#endif // DEBUG
					break;
				}
				case PAGE_SYNCZ:
				{
					configSyncZ.accel_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SyncZ, configSyncZ);
#ifdef DEBUG
					Serial.print("accel_Spindle:");
					Serial.println(configSyncZ.accel_Spindle);
#endif // DEBUG
					break;
				}
				case PAGE_SYNCX:
				{
					configSyncX.accel_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SyncX, configSyncX);
#ifdef DEBUG
					Serial.print("accel_Spindle:");
					Serial.println(configSyncX.accel_Spindle);
#endif // DEBUG
					break;
				}
				case PAGE_RECI_AXIAL_Z:
				case PAGE_RECI_RADIAL_Z:
				case PAGE_RECI_AXIAL_X:
				case PAGE_RECI_RADIAL_X:
				{
					configReci.accel_Spindle = GetSerialFloat(serialId);
					Serial.println(configReci.accel_Spindle);
					EEPROM.put(eePromAddress_Reci, configReci);
#ifdef DEBUG
					Serial.print("accel_Spindle:");
					Serial.println(configReci.accel_Spindle);
#endif // DEBUG
					break;
				}
				case PAGE_SP:
				{
					switch (configSp.activeAxis)
					{
						case ID_AXIS_Z:
						{
							configSp.accel_Axis_Z = GetSerialFloat(serialId);
							break;
						}
						case ID_AXIS_X:
						{
							configSp.accel_Axis_X = GetSerialFloat(serialId);
							break;
						}
						case ID_AXIS_B:
						{
							configSp.accel_Axis_B = GetSerialFloat(serialId);
							break;
						}
						case ID_SPINDLE:
						{
							configSp.accel_Spindle = GetSerialFloat(serialId);
	#ifdef DEBUG

							Serial.print("accel_Spindle:");
							Serial.println(configSp.accel_Spindle);

	#endif // DEBUG
							break;
						}
					}

					EEPROM.put(eePromAddress_Sp, configSp);
#ifdef DEBUG
					Serial.print("accel:");
					Serial.println(configSp.accel_Spindle);
#endif // DEBUG
					break;
				}
				case PAGE_INDEX1:
				{
					configIndex1.accel = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Index1, configIndex1);
#ifdef DEBUG
					Serial.print("accel:");
					Serial.println(configIndex1.accel);
#endif // DEBUG
					break;
				}
				case PAGE_INDEX2:
				{
					configIndex2.accel = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Index2, configIndex2);
#ifdef DEBUG
					Serial.print("accel:");
					Serial.println(configIndex2.accel);
#endif // DEBUG
					break;
				}
			}

			break;
		}
		case 60: // < Less than - Rose: Axis MaxSpd
		{
			configRose.maxSpd_Axis_Z = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rose, configRose);
#ifdef DEBUG
			Serial.print("maxSpd_Axis_Z:");
			Serial.println(configRose.maxSpd_Axis_Z);
#endif // DEBUG
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
			configMain.speedPercent_MoveZ = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("speedPercent_MoveZ:");
			Serial.println(configMain.speedPercent_MoveZ);
#endif // DEBUG
			break;
		}
		case 63: // ? Question mark - moveZ_Distance (Text)
		{
			// Set Distance
			float newDistance = GetSerialFloat(serialId);
			configMain.distance_MoveZ = newDistance;
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("distance_MoveZ:");
			Serial.println(configMain.distance_MoveZ);
#endif // DEBUG
			break;
		}
		case 64: // @ - Rose: Spindle RPM
		{
			configRose.speedPercent_Spindle = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rose, configRose);
#ifdef DEBUG
			Serial.print("speedPercent_Spindle:");
			Serial.println(configRose.speedPercent_Spindle);
#endif // DEBUG
			break;
		}
		case 65: // A - Enable spindle stepper
		{

			digitalWrite(PIN_SPINDLE_ENABLE, LOW);
			break;
		}
		case 66: //B - Disable spindle stepper
		{
			digitalWrite(PIN_SPINDLE_ENABLE, HIGH);
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
			configMain.speedPercentSpindle_SyncZ = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("speedPercentSpindle_SyncZ:");
			Serial.println(configMain.speedPercentSpindle_SyncZ);
#endif // DEBUG
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
				Serial3.print("pageIndex2.t7.txt="); // Nextion may not get the first packet
				Serial3.write(0x22);
				Serial3.print(newIndexSize);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				delay(100);
				Serial3.print("pageIndex2.t7.txt=");
				Serial3.write(0x22);
				Serial3.print(newIndexSize);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
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
				Serial3.print("pageIndex2.t7.txt=");
				Serial3.write(0x22);
				Serial3.print(newIndexSize);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				delay(100);
				Serial3.print("pageIndex2.t7.txt=");
				Serial3.write(0x22);
				Serial3.print(newIndexSize);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
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
			configMain.speedPercent_Index2 = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("speedPercent_Index2:");
			Serial.println(configMain.speedPercent_Index2);
#endif // DEBUG
			break;
		}
		case 73: // I-SyncZ Helix Type
		{
			configMain.helixType_SyncZ = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("helixType_SyncZ:");
			Serial.println(configMain.helixType_SyncZ);
#endif // DEBUG
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

			//Sync_SpindleZ(directionSpindle, directionAxis);
			Sync(directionSpindle, directionAxis, ID_AXIS_Z);
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

			Sync(directionSpindle, directionAxis, ID_AXIS_Z);
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
#ifdef DEBUG
			Serial.print("size_Index2:");
			Serial.println(configMain.size_Index2);
#endif // DEBUG
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
			configMain.speedPercentAxis_SyncZ = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("speedPercentAxis_SyncZ:");
			Serial.println(configMain.speedPercentAxis_SyncZ);
#endif // DEBUG
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
			configMain.speedPercent_Index1 = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("speedPercent_Index1:");
			Serial.println(configMain.speedPercent_Index1);
#endif // DEBUG
			break;
		}
		case 84: // T - Rose: n
		{
			configRose.n = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rose, configRose);
#ifdef DEBUG
			Serial.print("n:");
			Serial.println(configRose.n);
#endif // DEBUG
			break;
		}
		case 85: //U - MoveZ total distance
		{
			// Total distance only valid for current session
			distanceTotal_MoveZ = GetSerialFloat(serialId);

			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("distanceTotal_MoveZ:");
			Serial.println(distanceTotal_MoveZ);
#endif // DEBUG
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
#ifdef DEBUG
			Serial.print("newIndexSize_Index1:");
			Serial.println(newIndexSize);
			Serial.print("size_Index1:");
			Serial.println(configMain.size_Index1);
#endif // DEBUG
			break;
		}
		case 89:  // Y - Rose: d
		{
			configRose.d = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rose, configRose);
#ifdef DEBUG
			Serial.print("d:");
			Serial.println(configRose.d);
#endif // DEBUG
			break;
		}
		case 90:  // Z - Main Axis Clockwise
		{
			RunTwoSteppers_SpZ(DIR_CW, DIR_CW, 0);
			// BugBug: Z axis only runs one direction in following.
			//RunTwoSteppers_Sp_Axis(DIR_CW, DIR_CW, ID_AXIS_Z, ID_AXIS_Z);
			break;
		}
		case 91:  // [ - Z Axis Counterclockwise
		{
			RunTwoSteppers_SpZ(DIR_CCW, DIR_CCW, 0);
			// BugBug: Z axis only runs one direction in following.
			//RunTwoSteppers_Sp_Axis(DIR_CCW, DIR_CCW, ID_AXIS_Z, ID_AXIS_Z);
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


#ifdef DEBUG
			Serial.print("Loop-IncomingByte:");
			Serial.println(incomingByte);
			Serial.print("axisId:");
			Serial.println(axisId);
#endif // DEBUG

			switch (axisId)
			{
				case 1:
				{
					digitalWrite(PIN_AXIS_Z_ENABLE, LOW);
					break;
				}
				case 2:
				{
					digitalWrite(PIN_AXIS_X_ENABLE, LOW);

					break;
				}
				case 3:
				{
					digitalWrite(PIN_AXIS_B_ENABLE, LOW);
					Serial.print("PIN_AXIS_B_ENABLE:");
					Serial.println(LOW);
					break;
				}
			}
			break;
		}
		case 95: //_ - Disable Z axis stepper
		{
			int axisId = GetSerialInteger();
#ifdef DEBUG
			Serial.print("axisId:");
			Serial.println(axisId);
			Serial.print("Loop-IncomingByte:");
			Serial.println(incomingByte);
#endif // DEBUG
			switch (axisId)
			{
				case 1:
				{
					digitalWrite(PIN_AXIS_Z_ENABLE, HIGH);
					break;
				}
				case 2:
				{
					digitalWrite(PIN_AXIS_X_ENABLE, HIGH);
					break;
				}
				case 3:
				{
					digitalWrite(PIN_AXIS_B_ENABLE, HIGH);
					Serial.print("PIN_AXIS_B_ENABLE:");
					Serial.println(HIGH);
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
			configMain.speedPercent_Axis_Z = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("speedPercent_Axis_Z:");
			Serial.println(configMain.speedPercent_Axis_Z);
#endif // DEBUG
			break;
		}
		case 98: // b - Test EEPROM settings Setup screen
		{
			TestEEPROMSetup();
			break;
		}
		case 99: // c - Z Axis Stop
		{
			Serial3.print("pageSpZ.va2.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			break;
		}
		case 100: // d - Sp2 Clockwise
		{
			RunOneStepper(DIR_CW);
			break;
		}
		case 101: // e - Sp2 CounterClockwise
		{
			RunOneStepper(DIR_CCW);
			break;
		}
		case 102: // f - Set Sp2 Speed
		{
#ifdef DEBUG
			Serial.print("configSp.activeAxis:");
			Serial.println(configSp.activeAxis);
			Serial.print("speedPercent_Sp2:");
#endif // DEBUG
			switch (configSp.activeAxis)
			{
				case ID_AXIS_Z:
				{
					configSp.speedPercent_Axis_Z = (int)GetSerialFloat(serialId);
#ifdef DEBUG

					Serial.println(configSp.speedPercent_Axis_Z);
#endif // DEBUG
					break;
				}
				case ID_AXIS_X:
				{
					configSp.speedPercent_Axis_X = (int)GetSerialFloat(serialId);
#ifdef DEBUG

					Serial.println(configSp.speedPercent_Axis_X);
#endif // DEBUG
					break;
				}
				case ID_AXIS_B:
				{
					configSp.speedPercent_Axis_B = (int)GetSerialFloat(serialId);
#ifdef DEBUG

					Serial.println(configSp.speedPercent_Axis_B);
#endif // DEBUG
					break;
				}
				case ID_SPINDLE:
				{
					configSp.speedPercent_Spindle = (int)GetSerialFloat(serialId);
#ifdef DEBUG

					Serial.println(configSp.speedPercent_Spindle);
#endif // DEBUG
					break;
				}
			}
			EEPROM.put(eePromAddress_Setup, configSp);

			break;
		}
		case 103: // g - Rose: Spindle MaxSpd
		{
			configRose.maxSpd_Spindle = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rose, configRose);
#ifdef DEBUG
			Serial.print("maxSpd_Spindle:");
			Serial.println(configRose.maxSpd_Spindle);
#endif // DEBUG
			break;
		}
		case 104: // h - Rose: Spindle Accel
		{
			configRose.accel_Spindle = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rose, configRose);
#ifdef DEBUG
			Serial.print("accel_Spindle:");
			Serial.println(configRose.accel_Spindle);
#endif // DEBUG
			break;
		}
		case 105: // i - Rose pattern CCW
		{
			int selection = GetSerialInteger();

			switch (selection)
			{
				case 0:
				case 30:
				{
					RoseRadial_Z(DIR_CCW);
					break;
				}
				case 1:
				case 31:
				{
					RoseRadial_X(DIR_CCW);
					break;
				}
				case 2:
				case 32:
				{
					RosePattern_Axial_Z(DIR_CCW);
					break;
				}
				case 3:
				case 33:
				{
					//DoRosePattern_Axial_X(DIR_CCW);
					break;
				}
			}

			break;
		}
		case 106: // j - Move Z clockwise
		{
			MoveAxis(ID_AXIS_Z, DIR_CW);
			break;
		}
		case 107: // k - Z Spindle Clockwise
		{
#ifdef DEBUG
			Serial.print("1:spindle CW:");
#endif // DEBUG
			RunTwoSteppers_SpZ(DIR_CW, DIR_CW, 3);
			// BugBug: 
			//RunTwoSteppers_Sp_Axis(DIR_CW, DIR_CW, ID_SPINDLE, ID_AXIS_Z);
			break;
		}
		case 108: // l - Z spindle counter clockwise
		{
#ifdef DEBUG
			Serial.print("1:spindle CCW:");
#endif // DEBUG
			RunTwoSteppers_SpZ(DIR_CCW, DIR_CCW, 3);
			// BugBug:
			//RunTwoSteppers_Sp_Axis(DIR_CCW, DIR_CCW, ID_SPINDLE, ID_AXIS_Z);
			break;
		}
		case 109: // m - 
		{
			break;
		}
		case 110: // n - Set SpZ (Main) Spindle Speed
		{
			configMain.speedPercentSpindle_SpZ = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("speedPercentSpindle_SpZ:");
			Serial.println(configMain.speedPercentSpindle_SpZ);
#endif // DEBUG
			break;
		}
		case 111: // o - Rose: Spindle Steps per rev
		{
			configRose.speedPercent_Axis = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rose, configRose);
#ifdef DEBUG
			Serial.print("speedPercent_Axis:");
			Serial.println(configRose.speedPercent_Axis);
#endif // DEBUG
			break;
		}
		case 112: // p - Z/Sync Spindle Microsteps
		{
			//ToDo:
			configMain.microsteps_Spindle = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
			SetMicrosteppingMode(configMain.microsteps_Spindle, PIN_SPINDLE_MS0, PIN_SPINDLE_MS1, PIN_SPINDLE_MS2);
#ifdef DEBUG
			Serial.print("microsteps_Spindle:");
			Serial.println(configMain.microsteps_Spindle);
#endif // DEBUG
			break;
		}
		case 113: // q - Move Z counterclockwise
		{
			MoveAxis(ID_AXIS_Z,DIR_CCW);
			break;
		}
		case 114: // r - Main/Sync/Sp2 Spindle FullSteps
		{
			configMain.steps360_Spindle = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("steps360_Spindle:");
			Serial.println(configMain.steps360_Spindle);
#endif // DEBUG
			break;
		}
		case 115: // s - Main/Sync/Sp2 Spindle Gear Ratio
		{
			configMain.gearRatio_Spindle = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("gearRatio_Spindle:");
			Serial.println(configMain.gearRatio_Spindle);
#endif // DEBUG
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
					configSpB.accel_Axis = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SpB, configSpB);
#ifdef DEBUG
					Serial.print("accel_Axis:");
					Serial.println(configSpB.accel_Axis);
#endif // DEBUG
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
					configSpB.maxSpd_Axis = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SpB, configSpB);
#ifdef DEBUG
					Serial.print("maxSpd_Axis:");
					Serial.println(configSpB.maxSpd_Axis);
#endif // DEBUG
					break;
				}
			}
			break;
		}
		case 118: // v - B axis microsteps
		{
			configMain.microsteps_Axis_B = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef FOUR_AXES
			SetMicrosteppingMode(configMain.microsteps_Axis_B, PIN_AXIS_B_MS0, PIN_AXIS_B_MS1, PIN_AXIS_B_MS2);
#endif //FOUR_AXES
#ifdef DEBUG
			Serial.print("microsteps_Axis_B:");
			Serial.println(configMain.microsteps_Axis_B);
#endif // DEBUG
			break;
		}
		case 119: // w - B axis Steps/360 
		{
			configMain.steps360_Axis_B = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("steps360_Axis_B:");
			Serial.println(configMain.steps360_Axis_B);
#endif // DEBUG
			break;
		}
		case 120: // x - B axis Gear Ratio
		{
			configMain.gearRatio_AxisB = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("gearRatio_AxisB:");
			Serial.println(configMain.gearRatio_AxisB);
#endif // DEBUG
			break;
		}
//#endif // FOUR_AXES
		case 121: // y - Z Axis Microsteps
		{
			configMain.microsteps_Axis_Z = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
			SetMicrosteppingMode(configMain.microsteps_Axis_Z, PIN_AXIS_Z_MS0, PIN_AXIS_Z_MS1, PIN_AXIS_Z_MS2);
#ifdef DEBUG
			Serial.print("microsteps_Axis_Z:");
			Serial.println(configMain.microsteps_Axis_Z);
#endif // DEBUG
			break;
		}
		case 122: // z - Z Axis FullSteps
		{
			configMain.steps360_Axis_Z = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("steps360_Axis_Z:");
			Serial.println(configMain.steps360_Axis_Z);
#endif // DEBUG
			break;
		}
		case 123: // { - Z Distance per revolution
		{
			configMain.distancePerRev_AxisZ = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("distancePerRev_AxisZ:");
			Serial.println(configMain.distancePerRev_AxisZ);
#endif // DEBUG
			break;
		}
		case 124: // | - Z Axis Acceleration
		{
			pageCallerId = GetSerialIntegerOnly();
			switch (pageCallerId)
			{
				case PAGE_SPZ:
				{
					configSpZ.accel_Axis = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SpZ, configSpZ);
#ifdef DEBUG
					Serial.print("accel_Axis:");
					Serial.println(configSpZ.accel_Axis);
#endif // DEBUG
					break;
				}
				case PAGE_SYNCZ:
				{
					configSyncZ.accel_Axis = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SyncZ, configSyncZ);
#ifdef DEBUG
					Serial.print("accel_Axis:");
					Serial.println(configSyncZ.accel_Axis);
#endif // DEBUG
					break;
				}
				case PAGE_RECI_AXIAL_Z:
				case PAGE_RECI_RADIAL_Z:
				{
					configReci.accel_Axis_Z = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Reci, configReci);
#ifdef DEBUG
					Serial.print("accel_Axis_Z:");
					Serial.println(configReci.accel_Axis_Z);
#endif // DEBUG
					break;
				}
				case PAGE_RECI_AXIAL_X:
				case PAGE_RECI_RADIAL_X:
				{
					configReci.accel_Axis_X = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Reci, configReci);
#ifdef DEBUG
					Serial.print("accel_Axis_X:");
					Serial.println(configReci.accel_Axis_X);
#endif // DEBUG
					break;
				}
				case PAGE_MOVEZ:
				{
					configMoveZ.accel = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_MoveZ, configMoveZ);
#ifdef DEBUG
					Serial.print("accel:");
					Serial.println(configMoveZ.accel);
#endif // DEBUG
					break;
				}
			}

			break;
		}
		case 125: // } - Rose: Spindle Axis Accel
		{
			configRose.accel_Axis_Z = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rose, configRose);
#ifdef DEBUG
			Serial.print("accel_Axis_Z:");
			Serial.println(configRose.accel_Axis_Z);
#endif // DEBUG
			break;
		}
		case 126: // ~ - Test EEPROM settings Config screen
		{
			SerialPrint("pageConfig.t19.txt=");
			SerialWrite(0x22);
			SerialPrint("Config");
			SerialWrite(0x22);
			SerialWrite(0xff);
			SerialWrite(0xff);
			SerialWrite(0xff);
			delay(2000);
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
			configMain.speedPercentSpindle_SpX = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("speedPercentSpindle_SpX:");
			Serial.println(configMain.speedPercentSpindle_SpX);
#endif // DEBUG
			break;
		}
		case 162: // ¢ - Sp-X axis speed
		{
			configMain.speedPercent_Axis_X = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("speedPercent_Axis_X:");
			Serial.println(configMain.speedPercent_Axis_X);
#endif // DEBUG
			break;
		}
		case 163: // £ - Sp-X spindle CCW
		{
			RunTwoSteppers_Sp_Axis(DIR_CCW, DIR_CCW, ID_SPINDLE, ID_AXIS_X);
			break;
		}
		case 164: // ¤ - Sp-X spindle CW
		{
			RunTwoSteppers_Sp_Axis(DIR_CW, DIR_CW, ID_SPINDLE, ID_AXIS_X);
			break;
		}
		case 165: // ¥ - Sp-X Axis CCW
		{
			RunTwoSteppers_Sp_Axis(DIR_CCW, DIR_CCW, ID_AXIS_X, ID_AXIS_X);
			break;
		}
		case 166: // ¦ - Sp-X Axis CW
		{
			RunTwoSteppers_Sp_Axis(DIR_CW, DIR_CW, ID_AXIS_X, ID_AXIS_X);
			break;
		}
		case 167: // § - Sp-B spindle speed
		{
			configMain.speedPercentSpindle_SpB = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("speedPercentSpindle_SpB:");
			Serial.println(configMain.speedPercentSpindle_SpB);
#endif // DEBUG
			break;
		}
		case 168: // ¨ - Sp-B axis speed
		{
			configMain.speedPercent_Axis_B = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("speedPercent_Axis_B:");
			Serial.println(configMain.speedPercent_Axis_B);
#endif // DEBUG
			break;
		}
		case 169: // © - Sp-B spindle CCW
		{
			RunTwoSteppers_Sp_Axis(DIR_CCW, DIR_CCW, ID_SPINDLE, ID_AXIS_B);
			break;
		}
		case 170: // ª - Sp-B spindle CW
		{
			RunTwoSteppers_Sp_Axis(DIR_CW, DIR_CW, ID_SPINDLE, ID_AXIS_B);
			break;
		}
		case 171: // « - Sp-B Axis CCW
		{
			RunTwoSteppers_Sp_Axis(DIR_CCW, DIR_CCW, ID_AXIS_B, ID_AXIS_B);
			break;
		}
		case 172: // ¬ - Sp-B Axis CW
		{
			RunTwoSteppers_Sp_Axis(DIR_CW, DIR_CW, ID_AXIS_B, ID_AXIS_B);
			break;
		}
		//case 173: // Don't use 
		//{
		//	break;
		//}
		case 174: // ® - SyncX axis speed
		{
			configMain.speedPercentAxis_SyncX = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("speedPercentAxis_SyncX:");
			Serial.println(configMain.speedPercentAxis_SyncX);
#endif // DEBUG
			break;
		}
		case 175: // ¯ - SyncX spindle speed
		{
			configMain.speedPercentSpindle_SyncX = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("speedPercentSpindle_SyncX:");
			Serial.println(configMain.speedPercentSpindle_SyncX);
#endif // DEBUG
			break;
		}
		case 176: // ° - SyncX Helix direction
		{
			configMain.helixType_SyncX = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("helixType_SyncX:");
			Serial.println(configMain.helixType_SyncX);
#endif // DEBUG
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

			Sync(directionSpindle, directionAxis, ID_AXIS_X);
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
			
			Sync(directionSpindle, directionAxis, ID_AXIS_X);
			break;
		}
		case 179: // ³ - MvX Speed 
		{
			configMain.speedPercent_MoveX = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("speedPercent_MoveX:");
			Serial.println(configMain.speedPercent_MoveX);
#endif // DEBUG
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
#ifdef DEBUG
			Serial.print("distance_MoveX:");
			Serial.println(configMain.distance_MoveX);
#endif // DEBUG
			break;
		}
		case 182: // ¶ - 
		{

			break;
		}
		case 183: // · - MvX CCW
		{
			// Run
			MoveAxis(ID_AXIS_X, DIR_CCW);
			break;
		}
		case 184: // ¸ - MvX CW
		{
			// Run
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
					configSpX.maxSpd_Axis = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SpX, configSpX);
#ifdef DEBUG
					Serial.print("maxSpd_Axis:");
					Serial.println(configSpX.maxSpd_Axis);
#endif // DEBUG
					break;
				}
				case PAGE_SYNCX:
				{
					configSyncX.maxSpd_Axis = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SyncX, configSyncX);
#ifdef DEBUG
					Serial.print("maxSpd_Axis:");
					Serial.println(configSyncX.maxSpd_Axis);
#endif // DEBUG
					break;
				}
				case PAGE_MOVEX:
				{
					configMoveX.maxSpd = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_MoveX, configMoveX);
#ifdef DEBUG
					Serial.print("maxSpd:");
					Serial.println(configMoveX.maxSpd);
#endif // DEBUG
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
					configSpX.accel_Axis = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SpX, configSpX);
#ifdef DEBUG
					Serial.print("accel_Axis:");
					Serial.println(configSpX.accel_Axis);
#endif // DEBUG
					break;
				}
				case PAGE_SYNCX:
				{
					configSyncX.accel_Axis = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SyncX, configSyncX);
#ifdef DEBUG
					Serial.print("accel_Axis:");
					Serial.println(configSyncX.accel_Axis);
#endif // DEBUG
					break;
				}
				case PAGE_MOVEX:
				{
					configMoveX.accel = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_MoveX, configMoveX);
#ifdef DEBUG
					Serial.print("microsteps_Axis_X:");
					Serial.println(configMoveX.accel);
#endif // DEBUG
					break;
				}
			}
			break;
		}
		case 187: // » - X microsteps
		{
			configMain.microsteps_Axis_X = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifndef TWO_AXES_V2
			SetMicrosteppingMode(configMain.microsteps_Axis_X, PIN_AXIS_X_MS0, PIN_AXIS_X_MS1, PIN_AXIS_X_MS2);
#endif // !TWO_AXES_V2
#ifdef DEBUG
			Serial.print("microsteps_Axis_X:");
			Serial.println(configMain.microsteps_Axis_X);
#endif // DEBUG
			break;
		}
		case 188: // ¼ - X Full Steps
		{
			configMain.steps360_Axis_X = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("steps360_Axis_X:");
			Serial.println(configMain.steps360_Axis_X);
#endif // DEBUG
			break;
		}
		case 189: // ½ - X Distance per revolution (Text)
		{
			configMain.distancePerRev_AxisX = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("distancePerRev_AxisX:");
			Serial.println(configMain.distancePerRev_AxisX);
#endif // DEBUG
			break;
		}

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
#ifdef DEBUG
			Serial.print("revolutionsSyncZ_Spindle:");
			Serial.println(configMain.revolutionsSyncZ_Spindle);
#endif // DEBUG
			break;
		}
		case 193: // Á - Distance_SyncZ
		{
			configMain.distanceSyncZ = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("distanceSyncZ:");
			Serial.println(configMain.distanceSyncZ);
#endif // DEBUG
			break;
		}
		case 194: // À - Revolutions_SyncX
		{
			configMain.revolutionsSyncX_Spindle = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("revolutionsSyncX_Spindle:");
			Serial.println(configMain.revolutionsSyncX_Spindle);
#endif // DEBUG
			break;
		}
		case 195: // Á - Distance_SyncZ
		{
			configMain.distanceSyncX = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("distanceSyncX:");
			Serial.println(configMain.distanceSyncX);
#endif // DEBUG
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
#ifdef DEBUG
			Serial.print("source_Index2:");
			Serial.println(configMain.source_Index2);
#endif // DEBUG
			break;
		}
		case 197: // Å - Index2_SourceFile
		{
			GetIndex2FilenameFromSerial1();
			configMain.source_Index2 = FILE_SD;
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("source_Index2:");
			Serial.println(configMain.source_Index2);
#endif // DEBUG
			break;
		}
		case 198: // Æ - Do Reci AxialZ 
		{
			int waveDir = GetSerialFloat(serialId);

			// In: -1  Out: 1
			if (waveDir != 1)
			{
				waveDir = -1;
			}

			switch (pageCallerId)
			{
			case PAGE_RECI_AXIAL_Z:
			{
				Reciprocate_AxialZ(waveDir);
				break;
			}
			case PAGE_RECI_RADIAL_Z:
			{
				Reciprocate_RadialZ(waveDir);
				break;
			}
			case PAGE_RECI_AXIAL_X:
			{
				Reciprocate_AxialX(waveDir);
				break;
			}
			case PAGE_RECI_RADIAL_X:
			{
				Reciprocate_RadialX(waveDir);
				break;
			}
			}

			
			break;
		}
		case 199: // Ç - Rose: Spindle Axis Amplitude
		{
			configRose.amplitude_Axis_Z = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rose, configRose);
#ifdef DEBUG
			Serial.print("amplitude_Axis_Z:");
			Serial.println(configRose.amplitude_Axis_Z);
#endif // DEBUG
			break;
		}
		case 200: // È -
		{
			
			break;
		}
		case 201: // É - DoRosePattern_X
		{
			int selection = GetSerialInteger();

			switch (selection)
			{
				case 0:
				case 30:
				{
					RoseRadial_Z(DIR_CW);
					break;
				}
				case 1:
				case 31:
				{
					RoseRadial_X(DIR_CW);
					break;
				}
				case 2:
				case 32:
				{
					RosePattern_Axial_Z(DIR_CW);
					break;
				}
				case 3:
				case 33:
				{
					//DoRosePattern_Axial_X(DIR_CW);
					break;
				}
			}

			break;
		}
		case 202: // Ê - Rec1_Z Axis MaxSpeed 
		{
			pageCallerId = GetSerialIntegerOnly();
			switch (pageCallerId)
			{
				case PAGE_SPZ:
				{
					configSpZ.maxSpd_Axis = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SpZ, configSpZ);
#ifdef DEBUG
					Serial.print("maxSpd:");
					Serial.println(configSpZ.maxSpd_Axis);
#endif // DEBUG
					break;
				}
				case PAGE_SYNCZ:
				{
					configSyncZ.maxSpd_Axis = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_SyncZ, configSyncZ);
#ifdef DEBUG
					Serial.print("maxSpd:");
					Serial.println(configSyncZ.maxSpd_Axis);
#endif // DEBUG

					break;
				}
				case PAGE_RECI_AXIAL_Z:
				case PAGE_RECI_RADIAL_Z:
				{
					configReci.maxSpd_Axis_Z = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Reci, configReci);
#ifdef DEBUG
					Serial.print("maxSpd_Axis_Z:");
					Serial.println(configReci.maxSpd_Axis_Z);
#endif // DEBUG
					break;
				}
				case PAGE_RECI_AXIAL_X:
				case PAGE_RECI_RADIAL_X:
				{
					configReci.maxSpd_Axis_X = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Reci, configReci);
#ifdef DEBUG
					Serial.print("maxSpd_Axis_X:");
					Serial.println(configReci.maxSpd_Axis_X);
#endif // DEBUG
					break;
				}
				case PAGE_MOVEZ:
				{
					configMoveZ.maxSpd = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_MoveZ, configMoveZ);
#ifdef DEBUG
					Serial.print("maxSpd:");
					Serial.println(configMoveZ.maxSpd);
#endif // DEBUG
					break;
				}
			}

			break;
		}
		case 203: // Ì - Rec1_Z Waves (Count)
		{
			configReci.waves_AxialZ = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("waves_AxialZ:");
			Serial.println(configReci.waves_AxialZ);
#endif // DEBUG
			break;
		}
		case 204: // Ë - Rec1_Z Amplitude
		{
			configReci.amplitude_AxialZ = GetSerialFloat(serialId);
			
			EEPROM.put(eePromAddress_Setup, configReci);
#ifdef DEBUG
			Serial.print("amplitude_AxialZ:");
			Serial.println(configReci.amplitude_AxialZ);
#endif // DEBUG
			break;
		}
		case 205: // Í - Rec1_Z Distance
		{

			configReci.distance_AxialZ = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configReci);
#ifdef DEBUG
			Serial.print("distance_AxialZ:");
			Serial.println(configReci.distance_AxialZ);
#endif // DEBUG
			break;
		}
		case 206: // Î -  Return Spindle and Z axis
		{
			
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
			Reciprocate_RadialZ(waveDir);
			break;
		}
		case 208: // Ð - SP: Active Stepper
		{
			configSp.activeAxis = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Sp, configSp);
			break;
		}
		case 209: // Ñ - Reci AxialX and RadialX Axis Speed percentage
		{
			configReci.speedPercent_Axis_X = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configReci);
#ifdef DEBUG
			Serial.print("speedPercent_Axis_X:");
			Serial.println(configReci.speedPercent_Axis_X);
#endif // DEBUG
			break;
		}
		case 210: // Ò - Rec1_Spindle Spindle Speed percentage
		{
			configReci.speedPercent_Spindle = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configReci);
#ifdef DEBUG
			Serial.print("speedPercentSpindle:");
			Serial.println(configReci.speedPercent_Spindle);
#endif // DEBUG
			break;
		}
		case 211: // Ó - Reci AxialZ and RadialZ Axis Speed percentage 
		{
			configReci.speedPercent_Axis_Z = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configReci);
#ifdef DEBUG
			Serial.print("speedPercentAxis:");
			Serial.println(configReci.speedPercent_Axis_Z);
#endif // DEBUG
			break;
		}
		case 212: // Ô - Rec1_S Waves (Count)
		{
			configReci.waves_RadialZ = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configReci);
#ifdef DEBUG
			Serial.print("waves_RadialZ:");
			Serial.println(configReci.waves_RadialZ);
#endif // DEBUG
			break;
		}
		case 213: // Õ - Rec1_S Degrees
		{
			configReci.degrees_RadialZ = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configReci);
#ifdef DEBUG
			Serial.print("degrees_RadialZ:");
			Serial.println(configReci.degrees_RadialZ);
#endif // DEBUG
			break;
		}
		case 214: // Ö - Rec1_S Amplitude Axis
		{
			configReci.amplitude_RadialZ = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configReci);
#ifdef DEBUG
			Serial.print("amplitude_RadialZ:");
			Serial.println(configReci.amplitude_RadialZ);
#endif // DEBUG
			break;
		}
		case 215: // × - Return Spindle and X axis
		{
			pageCallerId = GetSerialInteger();
			ReturnToStartPosition(1);
			break;
		}
		case 216: // Ø - Polarity Spindle (High or Low)
		{
			int polaritySpindle = GetSerialInteger();
			polaritySpindle >= 1 ? (configMain.polarity_Spindle = true) : (configMain.polarity_Spindle = false);
			EEPROM.put(eePromAddress_Setup, configMain);

			break;
		}
		case 217: // Ù - Polarity Z (High or Low)
		{
			int polarityZ = GetSerialInteger();

			// true (1): LOW  false (0): HIGH
			polarityZ >= 1 ? (configMain.polarity_Axis_Z = true) : (configMain.polarity_Axis_Z = false);
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 218: // Ú - Polarity X (High or Low)
		{
			int polarityX = GetSerialInteger();

			// true (1): LOW  false (0): HIGH
			polarityX >= 1 ? (configMain.polarity_Axis_X = true) : (configMain.polarity_Axis_X = false);
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 219: // Û - Polarity B (High or Low)
		{
			int polarityB = GetSerialInteger();

			// true (1): LOW  false (0): HIGH
			polarityB >= 1 ? (configMain.polarity_Axis_B = true) : (configMain.polarity_Axis_B = false);
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 220: // Ü - Limit switch Z Min
		{
			configMain.limit_Min_Z = (int)GetSerial1Float();
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("limit_Min_Z:");
			Serial.println(configMain.limit_Min_Z);
#endif // DEBUG
			break;
		}
		case 221: // Ý - Limit switch Z Max
		{
			configMain.limit_Max_Z = (int)GetSerial1Float();
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("limit_Max_Z:");
			Serial.println(configMain.limit_Max_Z);
#endif // DEBUG
			break;
		}
		case 222: // Þ - Limit switch X Min
		{
			configMain.limit_Min_X = (int)GetSerial1Float();
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("limit_Min_X:");
			Serial.println(configMain.limit_Min_X);
#endif // DEBUG
			break;
		}
		case 223: // ß - Limit switch X Max
		{
			configMain.limit_Max_X = (int)GetSerial1Float();
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("limit_Max_X:");
			Serial.println(configMain.limit_Max_X);
#endif // DEBUG
			break;
		}
		case 224: // à - Limit switch B Min
		{
			configMain.limit_Min_B = (int)GetSerial1Float();
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("limit_Min_B:");
			Serial.println(configMain.limit_Min_B);
#endif // DEBUG
			break;
		}
		case 225: // á - Limit switch B Max
		{
			configMain.limit_Max_B = (int)GetSerial1Float();
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("limit_Max_B:");
			Serial.println(configMain.limit_Max_B);
#endif // DEBUG
			break;
		}
		case 226: // â - Rose: X Axis MaxSpd
		{
			configRose.maxSpd_Axis_X = (int)GetSerial1Float();
			EEPROM.put(eePromAddress_Rose, configRose);
#ifdef DEBUG
			Serial.print("maxSpd_Axis_X:");
			Serial.println(configRose.maxSpd_Axis_X);
#endif // DEBUG
			break;
		}
		case 227: // ã - Rose: X Axis Accel
		{
			configRose.accel_Axis_X = (int)GetSerial1Float();
			EEPROM.put(eePromAddress_Rose, configRose);
#ifdef DEBUG
			Serial.print("accel_Axis_X:");
			Serial.println(configRose.accel_Axis_X);
#endif // DEBUG
			break;
		}
		case 228: // ä - Rose: X Axis Amplitude
		{
			configRose.amplitude_Axis_X = GetSerial1Float();
			EEPROM.put(eePromAddress_Rose, configRose);
#ifdef DEBUG
			Serial.print("amplitude_Axis_X:");
			Serial.println(configRose.amplitude_Axis_X);
#endif // DEBUG
			break;
		}
		case 229: // å - Rose: Spindle Amplitude
		{
			configRose.amplitude_Spindle = GetSerial1Float();
			EEPROM.put(eePromAddress_Rose, configRose);
#ifdef DEBUG
			Serial.print("amplitude_Spindle:");
			Serial.println(configRose.amplitude_Spindle);
#endif // DEBUG
			break;
		}
		case 230: // å - Rose: DoRoseSpindle
		{
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
#ifdef DEBUG
			Serial.print("maxSpd_Return_Spindle:");
			Serial.println(configMain.maxSpd_Return_Spindle);
#endif // DEBUG
			break;
		}
		case 233: // é - Return Spindle Accel
		{
			configMain.accel_Return_Spindle = GetSerial1Float();
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("accel_Return_Spindle:");
			Serial.println(configMain.accel_Return_Spindle);
#endif // DEBUG
			break;
		}
		case 234: // ê - Return Spindle Accel
		{
			configMain.maxSpd_Return_Axis = GetSerial1Float();
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("maxSpd_Return_Axis:");
			Serial.println(configMain.maxSpd_Return_Axis);
#endif // DEBUG
			break;
		}
		case 235: // ë - Return Spindle Accel
		{
			configMain.accel_Return_Axis = GetSerial1Float();
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("accel_Return_Axis:");
			Serial.println(configMain.accel_Return_Axis);
#endif // DEBUG
			break;
		}
		case 236: //ì- Clear Stepper positions
		{
			returnSteps_Axis = 0;
			returnSteps_Spindle = 0;
			endPosition_Axis = 0;
			endPosition_Spindle = 0;
			//stepperSpindle.setPosition(0);
			//stepperAxis_Z.setPosition(0);
			//stepperAxis_X.setPosition(0);
			pageCallerId = GetSerialInteger();
		}
		case 237: // í - Track Positions
		{
			configMain.trackPositions = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("trackPositions:");
			Serial.println(configMain.trackPositions);
#endif // DEBUG
			break;
		}
		case 238: // î - Rec1 type Id
		{
			break;
		}
		case 239: // ï - Rec1 AxialX Waves
		{
			configReci.waves_AxialX = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configReci);
#ifdef DEBUG
			Serial.print("waves_AxialX:");
			Serial.println(configReci.waves_AxialX);
#endif // DEBUG
			break;
		}
		case 240: // ð - Rec1 AxialX Amplitude
		{
			configReci.amplitude_AxialX = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configReci);
#ifdef DEBUG
			Serial.print("amplitude_AxialX:");
			Serial.println(configReci.amplitude_AxialX);
#endif // DEBUG
			break;
		}
		case 241: // ñ - Rec1 AxialX Distance
		{
			configReci.distance_AxialX = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configReci);
#ifdef DEBUG
			Serial.print("distance_AxialX:");
			Serial.println(configReci.distance_AxialX);
#endif // DEBUG
			break;
		}
		case 242: // ò - Rec1 RadialX Waves
		{
			configReci.waves_RadialX = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configReci);
#ifdef DEBUG
			Serial.print("waves_RadialX:");
			Serial.println(configReci.waves_RadialX);
#endif // DEBUG
			break;
		}
		case 243: // ó - Rec1 RadialX Degrees
		{
			configReci.degrees_RadialX = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configReci);
#ifdef DEBUG
			Serial.print("degrees_RadialX:");
			Serial.println(configReci.degrees_RadialX);
#endif // DEBUG
			break;
		}
		case 244: // ô - Rec1 RadialX Amplitude
		{
			configReci.amplitude_RadialX = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configReci);
#ifdef DEBUG
			Serial.print("amplitude_RadialX:");
			Serial.println(configReci.amplitude_RadialX);
#endif // DEBUG
			break;
		}
		case 245: // õ - Rose X Axial Amplitude
		{

			configRose.amplitude_Axis_X = GetSerial1Float();
			EEPROM.put(eePromAddress_Rose, configRose);
#ifdef DEBUG
			Serial.print("amplitude_Axis_X:");
			Serial.println(configRose.amplitude_Axis_X);
#endif // DEBUG
			break;
		}
		//default:
		//{
		//	//SerialPrint("Input:");
		//	//SerialPrintLn("N/A");
		}
		delay(50);
	}
} // End loop()
