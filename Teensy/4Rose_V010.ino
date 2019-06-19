//#pragma once
/* *****************************************************************
* 4Rose_V010 main entry
* Author: Edward French
******************************************************************/
#include "math.h"
#include <string>
#include <EEPROM.h>
#include "4RoseDefines.h"
#include "SD.h"
#include "SPI.h"
#include "TeensyStep.h" //  https://github.com/luni64/TeensyStep
#include "AccelStepper.h"

//==================================================================
// Pin assignments:  See 4RoseDefines.h
//==================================================================
	// Initialize AccelStepper object (Used in Greek Key.  TeensyStep RotateControl doesn't honor SetTargetRel or SetTargetAbs.)
AccelStepper accelStep_Spindle(AccelStepper::DRIVER, PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
AccelStepper accelStep_Axis_Z(AccelStepper::DRIVER, PIN_AXIS_Z_STEP, PIN_AXIS_Z_DIR);
AccelStepper accelStep_Axis_X(AccelStepper::DRIVER, PIN_AXIS_X_STEP, PIN_AXIS_X_DIR);

// TeensyStep  initialization
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
	pinMode(LED_BUILTIN, OUTPUT);

	// Initialize Serial0 (USB port)
	Serial.begin(115200);

	// Initialize Nextion LCD for 115200 baud
	// Note: Nextion requires 3 0xFF bytes to signal end of transmission
	Serial3.begin(115200); //Nextion Serial baud rate set in Nextion pageMain Preinitialize Event tab
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
	EEPROM.get(eePromAddress_pageMain, configPageMain);

	EEPROM.get(eePromAddress_SyncZ, configSyncZ);
	EEPROM.get(eePromAddress_SyncX, configSyncX);
	EEPROM.get(eePromAddress_pageOne, configOne);
	EEPROM.get(eePromAddress_Index_Main, configIndex_Main);
	EEPROM.get(eePromAddress_Index_1, configIndex_1);
	EEPROM.get(eePromAddress_Index_2, configIndex_2);
	EEPROM.get(eePromAddress_Index_3, configIndex_3);
	EEPROM.get(eePromAddress_MoveZ, configMoveZ);
	EEPROM.get(eePromAddress_MoveX, configMoveX);
	EEPROM.get(eePromAddress_Rose, configRose);
	EEPROM.get(eePromAddress_Rec, configReci);
	EEPROM.get(eePromAddress_GreekKey_Main, configGreekKey_Main);
	EEPROM.get(eePromAddress_GreekKey_Z, configGreekKey_Z);
	EEPROM.get(eePromAddress_GreekKey_X, configGreekKey_X);

	// Config as well as all other EEProm settings must be run from Nextion whenever Teensy is updated.  
	// EEProm may contain invalid settings otherwise.

	pinMode(PIN_SPINDLE_ENABLE, OUTPUT);
	pinMode(PIN_AXIS_Z_ENABLE, OUTPUT);
	pinMode(PIN_AXIS_X_ENABLE, OUTPUT);

	SetEnable(ID_SPINDLE, false);

	SetEnable(ID_AXIS_Z, false);
	SetEnable(ID_AXIS_X, false);
	// Set Microstepping mode (Microsteps configured on hardware.)
	/*pinMode(PIN_SPINDLE_MS0, OUTPUT);
	pinMode(PIN_SPINDLE_MS1, OUTPUT);
	pinMode(PIN_SPINDLE_MS2, OUTPUT);
	pinMode(PIN_AXIS_Z_MS0, OUTPUT);
	pinMode(PIN_AXIS_Z_MS1, OUTPUT);
	pinMode(PIN_AXIS_Z_MS2, OUTPUT);*/
	// Microsteps set on PCB
	//SetMicrosteppingMode(configMain.microsteps_Spindle, PIN_SPINDLE_MS0, PIN_SPINDLE_MS1, PIN_SPINDLE_MS2);
	// Microsteps set on PCB
	//SetMicrosteppingMode(configMain.microsteps_Axis_Z, PIN_AXIS_Z_MS0, PIN_AXIS_Z_MS1, PIN_AXIS_Z_MS2);

#ifdef FOUR_AXES
	pinMode(PIN_AXIS_B_ENABLE, OUTPUT);
	SetEnable(ID_AXIS_B, false);


	// Set Microstepping mode (Microsteps configured on hardware.)
	//pinMode(PIN_AXIS_B_MS2, OUTPUT);
	//pinMode(PIN_AXIS_B_MS0, OUTPUT);
	//pinMode(PIN_AXIS_B_MS1, OUTPUT);
	// Microsteps set on PCB
	//SetMicrosteppingMode(configMain.microsteps_Axis_B, PIN_AXIS_B_MS0, PIN_AXIS_B_MS1, PIN_AXIS_B_MS2);
	// Microsteps set on PCB

#endif //FOUR_AXES
#ifndef TWO_AXES_V2
	// Set Microstepping mode (Microsteps configured on hardware.)
	//pinMode(PIN_AXIS_X_MS2, OUTPUT);
	//pinMode(PIN_AXIS_X_MS0, OUTPUT);
	//pinMode(PIN_AXIS_X_MS1, OUTPUT);
	// Microsteps set on PCB
	//SetMicrosteppingMode(configMain.microsteps_Axis_X, PIN_AXIS_X_MS0, PIN_AXIS_X_MS1, PIN_AXIS_X_MS2);

	// Configure TeensyStep motors

#endif // TWO_AXES_V2

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
#ifdef FOUR_AXES
	SD.begin(BUILTIN_SDCARD);
#else
	SD.begin(PIN_SPI_CS);
#endif // FOUR_AXES

	// Configure Accelsteppers
	accelStep_Spindle.setEnablePin(PIN_SPINDLE_ENABLE);

	// Accelstepper default for Enable is HIGH.  setPinsInverted(false, false,true) for Enable LOW
	if(!configMain.polarity_Spindle)
	{ 
		accelStep_Spindle.setPinsInverted(false, false, true);
	}

	SetEnable(ID_SPINDLE, false);

	accelStep_Axis_Z.setEnablePin(PIN_AXIS_Z_ENABLE);
	if (!configMain.polarity_Axis_Z)
	{
		accelStep_Axis_Z.setPinsInverted(false, false, true);
	}

	SetEnable(ID_AXIS_Z, false);

#ifdef FOUR_AXES
	accelStep_Axis_X.setEnablePin(PIN_AXIS_X_ENABLE);
	if (!configMain.polarity_Axis_X)
	{
		accelStep_Axis_X.setPinsInverted(false, false, true);
	}
	SetEnable(ID_AXIS_X, false);

#endif // FOUR_AXES

	for (int i = 0; i < 3; i++) // Verify Teensy is operational
	{
		digitalWrite(LED_BUILTIN, HIGH);
		delay(50);
		digitalWrite(LED_BUILTIN, LOW);
		delay(300);
	}
#ifdef DEBUG
	Serial.println("Initialized...");
#endif // DEBUG

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
	// ToDo: Auto select serial port
	// Select serial port: Serial = USB connected, Serial3 = Nextion LCD connected
	if (Serial.available() > 0)
	{
		// USB connected 
		serialId = 0;
	}
	else if (Serial3.available() > 0)//***Hardcoded to Serial3*** See SerialAvailable() method.
	{
		//Nextion connected
		serialId = 1;
	}
	serialId = 1; //***Hardcoded to Serial3*** 

	// All Nextion incoming data packets are terminated with one 0xFF byte
	if (serialId <9) // If serial data is available, parse the data
	{
		incomingByte = SerialRead(serialId);
		delay(100);

		switch (incomingByte)
		{
		case 41: // ) - 
		{
			break;
		}
		case 42: // * - 
		{

			break;
		}
		case 43: // + - Greek Key: Pattern radial or axial
		{
			configGreekKey_Main.radialOrAxial_Pattern = GetSerialInteger();
			EEPROM.put(eePromAddress_GreekKey_Main, configGreekKey_Main);
#ifdef DEBUG
			Serial.print("configGreekKey_Main.radialOrAxial_Pattern:");
			Serial.println(configGreekKey_Main.radialOrAxial_Pattern);
#endif // DEBUG

			break;
		}
		case 44: // , - Greek Key: File radial or axial
		{
			configGreekKey_Main.radialOrAxial_File = GetSerialInteger();
			EEPROM.put(eePromAddress_GreekKey_Main, configGreekKey_Main);
			break;
		}
		case 45: // - - Return Spindle and B axis to start positions
		{
			pageCallerId = GetSerialInteger();
			ReturnToStartPosition(ID_AXIS_B);
			break;
		}
		case 46: // . - Greek Key: Pattern count(pattern repeats)
		{
			configGreekKey_Main.countPattern = GetSerialInteger();
#ifdef DEBUG
			Serial.print("countPattern:");
			Serial.println(configGreekKey_Main.countPattern);
#endif // DEBUG

			EEPROM.put(eePromAddress_GreekKey_Main, configGreekKey_Main);
			break;
		}
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
				case PAGE_MAIN:
				{
					configPageMain.maxSpd_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_pageMain, configPageMain);
#ifdef DEBUG
					Serial.print("maxSpd:");
					Serial.println(configPageMain.maxSpd_Spindle);
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
				case PAGE_REC:
				{
					configReci.maxSpd_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Rec, configReci);
#ifdef DEBUG
					Serial.print("maxSpd:");
					Serial.println(configReci.maxSpd_Spindle);
#endif // DEBUG
					break;
				}
				case PAGE_ONE:
				{
					switch (configOne.activeAxis)
					{
						case ID_AXIS_Z:
						{
							configOne.maxSpd_Axis_Z = GetSerialFloat(serialId);
#ifdef DEBUG
							Serial.print("maxSpd:");
							Serial.println(configOne.maxSpd_Axis_Z);
#endif // DEBUG
							break;
						}
						case ID_AXIS_X:
						{
							configOne.maxSpd_Axis_X = GetSerialFloat(serialId);
#ifdef DEBUG
							Serial.print("maxSpd:");
							Serial.println(configOne.maxSpd_Axis_X);
#endif // DEBUG
							break;
						}
						case ID_AXIS_B:
						{
							configOne.maxSpd_Axis_B = GetSerialFloat(serialId);
#ifdef DEBUG
							Serial.print("maxSpd:");
							Serial.println(configOne.maxSpd_Axis_B);
#endif // DEBUG
							break;
						}
						case ID_SPINDLE:
						{
							configOne.maxSpd_Spindle = GetSerialFloat(serialId);
#ifdef DEBUG
							Serial.print("maxSpd:");
							Serial.println(configOne.maxSpd_Spindle);
#endif // DEBUG
							break;
						}
					}

					EEPROM.put(eePromAddress_pageOne, configOne);

					break;
				}
				case PAGE_INDEX1:
				{
					configIndex_Main.maxSpd = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Index_Main, configIndex_Main);
#ifdef DEBUG
					Serial.print("maxSpd:");
					Serial.println(configIndex_Main.maxSpd);
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
				case PAGE_MAIN:
				{
					configPageMain.accel_Spindle = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_pageMain, configPageMain);
#ifdef DEBUG
					Serial.print("accel_Spindle:");
					Serial.println(configPageMain.accel_Spindle);
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
				case PAGE_REC:
				{
					configReci.accel_Spindle = GetSerialFloat(serialId);
					Serial.println(configReci.accel_Spindle);
					EEPROM.put(eePromAddress_Rec, configReci);
#ifdef DEBUG
					Serial.print("accel_Spindle:");
					Serial.println(configReci.accel_Spindle);
#endif // DEBUG
					break;
				}
				case PAGE_ONE:
				{
					switch (configOne.activeAxis)
					{
						case ID_AXIS_Z:
						{
							configOne.accel_Axis_Z = GetSerialFloat(serialId);
							break;
						}
						case ID_AXIS_X:
						{
							configOne.accel_Axis_X = GetSerialFloat(serialId);
							break;
						}
						case ID_AXIS_B:
						{
							configOne.accel_Axis_B = GetSerialFloat(serialId);
							break;
						}
						case ID_SPINDLE:
						{
							configOne.accel_Spindle = GetSerialFloat(serialId);
	#ifdef DEBUG
							Serial.print("accel_Spindle:");
							Serial.println(configOne.accel_Spindle);
	#endif // DEBUG
							break;
						}
					}

					EEPROM.put(eePromAddress_pageOne, configOne);
#ifdef DEBUG
					Serial.print("accel:");
					Serial.println(configOne.accel_Spindle);
#endif // DEBUG
					break;
				}
				case PAGE_INDEX1:
				{
					configIndex_Main.accel = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Index_Main, configIndex_Main);
#ifdef DEBUG
					Serial.print("accel:");
					Serial.println(configIndex_Main.accel);
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
		case 61: // = Do Greek Key from file
		{

#ifdef DEBUG
			Serial.println("Enter Case 61:Direction");
#endif
			reverseDirection = GetSerialInteger();
			if (reverseDirection == 0)
			{
				reverseDirection = DIR_CCW; // Nextion can't send negative number
			}
			else
			{
				reverseDirection = DIR_CW;
			}
#ifdef DEBUG
			Serial.print("configGreekKey_Main.axisId:");
			Serial.println(configGreekKey_Main.axisId);
			Serial.print("reverseDirection:");
			Serial.println(reverseDirection);
			Serial.println("++++++++++++++++++");
#endif // DEBUG
			GreekKeyFromFile(reverseDirection);

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
		case 65: // A - Not Used
		{
			break;
		}
		case 66: //B - Not Used
		{
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
		case 69: //E - Index by divisions or degrees
		{
			switch (configIndex_Main.indexId)
			{
				case 1:
				{
					// Set degreeOrDivision
					configIndex_1.degreeOrDivision = GetSerialIntegerOnly();
#ifdef DEBUG
					Serial.print("Index1-DegreeOrDivision(69):");
					Serial.println(configIndex_1.degreeOrDivision);
#endif // DEBUG
					EEPROM.put(eePromAddress_Index_1, configIndex_1);
					break;
				}
				case 2:
				{
					// Set degreeOrDivision
					configIndex_2.degreeOrDivision = GetSerialIntegerOnly();
#ifdef DEBUG
					Serial.print("Index2-DegreeOrDivision(69):");
					Serial.println(configIndex_2.degreeOrDivision);
#endif // DEBUG
					EEPROM.put(eePromAddress_Index_2, configIndex_2);
					break;
				}
				case 3:
				{
					// Set degreeOrDivision
					configIndex_3.degreeOrDivision = GetSerialIntegerOnly();
#ifdef DEBUG
					Serial.print("Index3-DegreeOrDivision(69):");
					Serial.println(configIndex_3.degreeOrDivision);
#endif // DEBUG
					EEPROM.put(eePromAddress_Index_3, configIndex_3);
					break;
				}
			}

			break;
		}
		case 70: // F - Rose: Radial or axial
		{
			configRose.radial_Axial = GetSerialInteger();
			EEPROM.put(eePromAddress_Rose, configRose);
			break;
		}
		case 71: // G - Greek key axis maxSpeed
		{	
			int maxSpeed = (int)GetSerialFloat(serialId);
#ifdef DEBUG
			Serial.print("maxSpeed(71):");
			Serial.println(maxSpeed);
#endif // DEBUG

			switch(configGreekKey_Main.axisId)
			{ 
				case ID_AXIS_Z: // Greek Key Z: Z Axis MaxSpd
				{
					configGreekKey_Z.maxSpd_Axis = maxSpeed;
					EEPROM.put(eePromAddress_GreekKey_Z, configGreekKey_Z);
					break;
				}
				case ID_AXIS_X: // Greek Key X: X Axis MaxSpd
				{
					configGreekKey_X.maxSpd_Axis = maxSpeed;
					EEPROM.put(eePromAddress_GreekKey_X, configGreekKey_X);
					break;
				}
			}
			break;
		}
		case 72: // H - Rose: AxisId
		{
			configRose.axisId = GetSerialInteger();
			EEPROM.put(eePromAddress_Rose, configRose);
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
					directionAxis = DIR_CCW;// 0; // CCW
					directionSpindle = DIR_CCW;// 0; //CCW
					break;
				}
				case 1: //Right for right handed Z axis leadscrew, Left for left handed Z axis leadscrew.
				case 49:
				{
					directionAxis = DIR_CCW; // CCW
					directionSpindle = DIR_CW; //CW
					break;
				}
			}
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
					directionAxis = DIR_CW; // CW
					directionSpindle = DIR_CW; // CW
					break;
				}
				case 1: //Right for right handed Z axis leadscrew, Left for left handed Z axis leadscrew.
				case 49:
				{
					directionAxis = DIR_CW; // CW
					directionSpindle = DIR_CCW;//0; // CCW
					break;
				}
			}

			Sync(directionSpindle, directionAxis, ID_AXIS_Z);
			break;
		}
		case 76: // L - Greek Key File: Segment or Actual
		{
			configGreekKey_Main.segmentOrActual = GetSerialInteger();// (int)GetSerialFloat(serialId);
#ifdef DEBUG
			Serial.print("segmentOrActual");
			Serial.println(configGreekKey_Main.segmentOrActual);
#endif // DEBUG
			EEPROM.put(eePromAddress_GreekKey_Main, configGreekKey_Main);
			break;
		}
		case 77: // M - Greek Key File
		{
			configGreekKey_Main.countPatternFile = (int)GetSerialFloat(serialId);//  GetSerialInteger();
#ifdef DEBUG
			Serial.print("countPatternFile: ");
			Serial.println(configGreekKey_Main.countPatternFile);
#endif // DEBUG
			EEPROM.put(eePromAddress_GreekKey_Main, configGreekKey_Main);
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
		case 80: // P - Greek Key Source: Pattern or file
		{
			configGreekKey_Main.fileOrPattern = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_GreekKey_Main, configGreekKey_Main);
			break;
		}
		case 81: // Q - Index1 counter clockwise
		{
			double newIndexSize = 0;
			Serial.println("");
			Serial.print("configIndex_Main.indexId(81-1):");
			Serial.println(configIndex_Main.indexId);
			badFilename = false;

			switch (configIndex_Main.indexId)
			{
				case 1:
				{
					Serial.print("configIndex_1.fileOrFixed(81-1):");
					Serial.println(configIndex_1.fileOrFixed);
					if (configIndex_1.fileOrFixed == FILE_SD)
					{
						int lineNumber = GetSerialInteger();
						if (lineNumber == 255)
						{
							lineNumber = 0;
						}

						newIndexSize = GetIndexDataFromSD(lineNumber);
						configIndex_1.size = (configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) * (newIndexSize / 360);
#ifdef DEBUG
						Serial.println("");
						Serial.print("lineNumber(81-1):");
						Serial.println(lineNumber);
						Serial.print("newIndexSize(81-1):");
						Serial.println(newIndexSize);
						Serial.print("configIndex_1.size(81-1):");
						Serial.println(configIndex_1.size);
						Serial.println("");
#endif // DEBUG
						Serial3.print("pageIndex1.t7.txt="); // Nextion may not get the first packet
						Serial3.write(0x22);
						Serial3.print(newIndexSize);
						Serial3.write(0x22);
						Serial3.write(0xff);
						Serial3.write(0xff);
						Serial3.write(0xff);
						delay(100);
						Serial3.print("pageIndex1.t7.txt=");
						Serial3.write(0x22);
						Serial3.print(newIndexSize);
						Serial3.write(0x22);
						Serial3.write(0xff);
						Serial3.write(0xff);
						Serial3.write(0xff);
						delay(100);
					}

					break;
				}
				case 2:
				{
#ifdef DEBUG
					Serial.println("++++++++++++++++++++configIndex_2.fileOrFixed(81-2):");
					Serial.println(configIndex_2.fileOrFixed);
#endif // DEBUG
					if (configIndex_2.fileOrFixed == FILE_SD)
					{
						int lineNumber = GetSerialInteger();
						if (lineNumber == 255)
						{
							lineNumber = 0;
						}

						newIndexSize = GetIndexDataFromSD(lineNumber);
						configIndex_2.size = (configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) * (newIndexSize / 360);
						Serial.println("");
						Serial.print("lineNumber(81-1):");
						Serial.println(lineNumber);
						Serial.print("newIndexSize(81-1):");
						Serial.println(newIndexSize);
						Serial.print("configIndex_2.size(81-1):");
						Serial.println(configIndex_2.size);
						Serial.println("");
						Serial3.print("pageIndex1.t7.txt="); // Nextion may not get the first packet
						Serial3.write(0x22);
						Serial3.print(newIndexSize);
						Serial3.write(0x22);
						Serial3.write(0xff);
						Serial3.write(0xff);
						Serial3.write(0xff);
						delay(100);
						Serial3.print("pageIndex1.t7.txt=");
						Serial3.write(0x22);
						Serial3.print(newIndexSize);
						Serial3.write(0x22);
						Serial3.write(0xff);
						Serial3.write(0xff);
						Serial3.write(0xff);
						delay(100);
					}

					break;
				}
				case 3:
				{
					Serial.print("configIndex_3.fileOrFixed(81-3):");
					Serial.println(configIndex_3.fileOrFixed);
					if (configIndex_3.fileOrFixed == FILE_SD)
					{
						int lineNumber = GetSerialInteger();
						if (lineNumber == 255)
						{
							lineNumber = 0;
						}
						newIndexSize = GetIndexDataFromSD(lineNumber);
						configIndex_3.size = (configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) * (newIndexSize / 360);
						Serial.println("");
						Serial.print("lineNumber(81-1):");
						Serial.println(lineNumber);
						Serial.print("newIndexSize(81-1):");
						Serial.println(newIndexSize);
						Serial.print("configIndex_3.size(81-1):");
						Serial.println(configIndex_3.size);
						Serial.println("");
						Serial3.print("pageIndex1.t7.txt="); // Nextion may not get the first packet
						Serial3.write(0x22);
						Serial3.print(newIndexSize);
						Serial3.write(0x22);
						Serial3.write(0xff);
						Serial3.write(0xff);
						Serial3.write(0xff);
						delay(100);
						Serial3.print("pageIndex1.t7.txt=");
						Serial3.write(0x22);
						Serial3.print(newIndexSize);
						Serial3.write(0x22);
						Serial3.write(0xff);
						Serial3.write(0xff);
						Serial3.write(0xff);
						delay(100);
					}

					break;
				}
			}

			if (!badFilename)
			{
				IndexSpindle(DIR_CCW);
			}
			else
			{
				// Update Index flag on Nextion. 
				Serial3.print("pageIndex1.bt3.pco=59391");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.print("pageIndex1.va0.val=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.print("pageIndex1.bt3.val=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.print("pageIndex1.bt2.val=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.print("pageIndex1.bt1.val=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.print("pageSync.b6.pco=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.print("pageSync.b5.pco=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.print("pageSyncX.b6.pco=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.print("pageSyncX.b5.pco=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
			}

			break;
		}
		case 82: // R - Index1 Clockwise
		{
			double newIndexSize = 0;
			badFilename = false;
			Serial.println("");
			Serial.print("configIndex_Main.axisId(82-1):");
			Serial.println(configIndex_Main.indexId);
			Serial.println("");
			Serial.print("configIndex_1.fileOrFixed(82-1):");
			Serial.println(configIndex_1.fileOrFixed);
			switch (configIndex_Main.indexId)
			{
				case 1:
				{
					if (configIndex_1.fileOrFixed == FILE_SD)
					{
						int lineNumber = GetSerialInteger();
						if (lineNumber == 255)
						{
							lineNumber = 0;
						}

						newIndexSize = GetIndexDataFromSD(lineNumber);
						configIndex_1.size = (configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) * (newIndexSize / 360);
	#ifdef DEBUG
						Serial.println("");
						Serial.print("lineNumber(81-1):");
						Serial.println(lineNumber);
						Serial.print("newIndexSize(81-1):");
						Serial.println(newIndexSize);
						Serial.print("configIndex_1.size(81-1):");
						Serial.println(configIndex_1.size);
						Serial.println("");
	#endif // DEBUG
						Serial3.print("pageIndex1.t7.txt="); // Nextion may not get the first packet
						Serial3.write(0x22);
						Serial3.print(newIndexSize);
						Serial3.write(0x22);
						Serial3.write(0xff);
						Serial3.write(0xff);
						Serial3.write(0xff);
						delay(100);
						Serial3.print("pageIndex1.t7.txt=");
						Serial3.write(0x22);
						Serial3.print(newIndexSize);
						Serial3.write(0x22);
						Serial3.write(0xff);
						Serial3.write(0xff);
						Serial3.write(0xff);
						delay(100);
					}

					break;
				}
				case 2:
				{
					if (configIndex_2.fileOrFixed == FILE_SD)
					{
						int lineNumber = GetSerialInteger();
						if (lineNumber == 255)
						{
							lineNumber = 0;
						}

						newIndexSize = GetIndexDataFromSD(lineNumber);
						configIndex_2.size = (configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) * (newIndexSize / 360);
	#ifdef DEBUG
						Serial.println("");
						Serial.print("lineNumber(81-2):");
						Serial.println(lineNumber);
						Serial.print("newIndexSize(81-2):");
						Serial.println(newIndexSize);
						Serial.print("configIndex_2.size(81-2):");
						Serial.println(configIndex_2.size);
						Serial.println("");
	#endif // DEBUG
						Serial3.print("pageIndex1.t7.txt="); // Nextion may not get the first packet
						Serial3.write(0x22);
						Serial3.print(newIndexSize);
						Serial3.write(0x22);
						Serial3.write(0xff);
						Serial3.write(0xff);
						Serial3.write(0xff);
						delay(100);
						Serial3.print("pageIndex1.t7.txt=");
						Serial3.write(0x22);
						Serial3.print(newIndexSize);
						Serial3.write(0x22);
						Serial3.write(0xff);
						Serial3.write(0xff);
						Serial3.write(0xff);
						delay(100);
					}

					break;
				}
				case 3:
				{
					if (configIndex_3.fileOrFixed == FILE_SD)
					{
						int lineNumber = GetSerialInteger();
						if (lineNumber == 255)
						{
							lineNumber = 0;
						}
						newIndexSize = GetIndexDataFromSD(lineNumber);
						configIndex_3.size = (configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) * (newIndexSize / 360);
	#ifdef DEBUG
						Serial.println("");
						Serial.print("lineNumber(81-3):");
						Serial.println(lineNumber);
						Serial.print("newIndexSize(81-3):");
						Serial.println(newIndexSize);
						Serial.print("configIndex_3.size(81-3):");
						Serial.println(configIndex_3.size);
						Serial.println("");
	#endif // DEBUG
						Serial3.print("pageIndex1.t7.txt="); // Nextion may not get the first packet
						Serial3.write(0x22);
						Serial3.print(newIndexSize);
						Serial3.write(0x22);
						Serial3.write(0xff);
						Serial3.write(0xff);
						Serial3.write(0xff);
						delay(100);
						Serial3.print("pageIndex1.t7.txt=");
						Serial3.write(0x22);
						Serial3.print(newIndexSize);
						Serial3.write(0x22);
						Serial3.write(0xff);
						Serial3.write(0xff);
						Serial3.write(0xff);
						delay(100);
					}

					break;
				}
			}

			if (!badFilename)
			{
				IndexSpindle(DIR_CW);
			}
			else
			{
				// Update Index flag on Nextion. 
				Serial3.print("pageIndex1.bt3.pco=59391");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.print("pageIndex1.va0.val=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.print("pageIndex1.bt3.val=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.print("pageIndex1.bt2.val=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.print("pageIndex1.bt1.val=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.print("pageSync.b6.pco=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.print("pageSync.b5.pco=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.print("pageSyncX.b6.pco=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.print("pageSyncX.b5.pco=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
			}
			break;
		}
		case 83: // S - Set Index speed
		{
			configIndex_Main.speedPercent = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Index_Main, configIndex_Main);
#ifdef DEBUG
			Serial.print("speedPercent_Index:");
			Serial.println(configIndex_Main.speedPercent);
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
		case 85: //U - Greek Key File: Axis Segment Length
		{
			configGreekKey_Main.segmentLengthForFile = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_GreekKey_Main, configGreekKey_Main);
#ifdef DEBUG
			Serial.print("configGreekKey_Main.segmentLengthFile:");
			Serial.println(configGreekKey_Main.segmentLengthForFile);
			Serial.print("configGreekKey_Main.segmentLengthFile:");
			Serial.println(configGreekKey_Main.segmentLengthForFile);
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
		case 87: // W - Set Index1 type to Degrees or Divisions
		{
			configReci.radial_axial = GetSerialInteger();
			EEPROM.put(eePromAddress_Rec, configReci);
			break;
		}
		case 88:  // X - Return to start: Spindle
		{
			pageCallerId = GetSerialInteger();
			ReturnToStartPosition(ID_SPINDLE);
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
			RunTwoSteppers_All(DIR_CW, DIR_CW, ID_AXIS_Z);
			break;
		}
		case 91:  // [ - Z Axis Counterclockwise
		{
			RunTwoSteppers_All(DIR_CCW, DIR_CCW, ID_AXIS_Z);
			break;
		}
		case 92: // \ - *** Don't Use***
		{
			break;
		}
		case 93: // ] - Not Used
		{
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
				case ID_AXIS_Z:
				{
					SetEnable(ID_AXIS_Z, true);
					break;
				}
				case ID_AXIS_X:
				{
					SetEnable(ID_AXIS_X, true);
					break;
				}
				case ID_AXIS_B:
				{
					SetEnable(ID_AXIS_B, true);
					break;
				}
				case ID_SPINDLE:
				{
					SetEnable(ID_SPINDLE, true);
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
				case ID_AXIS_Z:
				{
					SetEnable(ID_AXIS_Z, false);
					break;
				}
				case ID_AXIS_X:
				{
					SetEnable(ID_AXIS_X, false);
					break;
				}
				case ID_AXIS_B:
				{
					SetEnable(ID_AXIS_B, false);
					break;
				}
				case ID_SPINDLE:
				{
					SetEnable(ID_SPINDLE, false);
					break;
				}
			}
			break;
		}
		case 96: // ' - *** Don't Use***
		{
			break;
		}
		case 97: // a - Z Axis Speed Percent
		{
			int newSpeedPercentage = (int)GetSerialFloat(serialId);
#ifdef DEBUG
			Serial.print("0-newSpeedPercentage:");
			Serial.println(newSpeedPercentage);
#endif // DEBUG
			switch (configPageMain.axisId)
			{
				case ID_AXIS_Z:
				{
					configMain.speedPercent_Axis_Z = newSpeedPercentage;
#ifdef DEBUG
					Serial.print("speedPercent_Axis_Z:");
					Serial.println(configMain.speedPercent_Axis_Z);
#endif // DEBUG
					EEPROM.put(eePromAddress_Setup, configMain);
					break;
				}
				case ID_AXIS_X:
				{
					configMain.speedPercent_Axis_X = newSpeedPercentage;
					EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
					Serial.print("speedPercent_Axis_X:");
					Serial.println(configMain.speedPercent_Axis_X);
#endif // DEBUG
					break;
				}
				case ID_AXIS_B:
				{
					configMain.speedPercent_Axis_B = newSpeedPercentage;
					EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
					Serial.print("speedPercent_Axis_B:");
					Serial.println(configMain.speedPercent_Axis_B);
#endif // DEBUG
					break;
				}
			}
#ifdef DEBUG
				Serial.print("3-newSpeedPercentage:");
				Serial.println(newSpeedPercentage);
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
			// Implemented in individual methods
			Serial3.print("pageMain.va2.val=0");
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
			Serial.println(configOne.activeAxis);
			Serial.print("speedPercent_Sp2:");
#endif // DEBUG
			switch (configOne.activeAxis)
			{
				case ID_AXIS_Z:
				{
					configOne.speedPercent_Axis_Z = (int)GetSerialFloat(serialId);
#ifdef DEBUG
					Serial.println(configOne.speedPercent_Axis_Z);
#endif // DEBUG
					break;
				}
				case ID_AXIS_X:
				{
					configOne.speedPercent_Axis_X = (int)GetSerialFloat(serialId);
#ifdef DEBUG
					Serial.println(configOne.speedPercent_Axis_X);
#endif // DEBUG
					break;
				}
				case ID_AXIS_B:
				{
					configOne.speedPercent_Axis_B = (int)GetSerialFloat(serialId);
#ifdef DEBUG
					Serial.println(configOne.speedPercent_Axis_B);
#endif // DEBUG
					break;
				}
				case ID_SPINDLE:
				{
					configOne.speedPercent_Spindle = (int)GetSerialFloat(serialId);
#ifdef DEBUG
					Serial.println(configOne.speedPercent_Spindle);
#endif // DEBUG
					break;
				}
			}
			EEPROM.put(eePromAddress_pageOne, configOne);

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

#ifdef DEBUG
			Serial.print("configRose.axisId:");
			Serial.println(configRose.axisId);
#endif // DEBUG

			switch (configRose.axisId)
			{
				case ID_AXIS_Z: // Z axis
				{
					RoseRadial_Z(DIR_CCW);
					break;
				}
				case ID_AXIS_X: // X axis
				{
					RoseRadial_X(DIR_CCW);
					break;
				}
			}

			break;
		}
		case 106: // j - Move Z counter clockwise
		{

			MoveAxis(ID_AXIS_Z, DIR_CCW);
			break;
		}
		case 107: // k - Z Spindle Clockwise
		{
			RunTwoSteppers_All(DIR_CW, DIR_CW, ID_SPINDLE);
			break;
		}
		case 108: // l - Z spindle counter clockwise
		{
			RunTwoSteppers_All(DIR_CCW, DIR_CCW, ID_SPINDLE);
			break;
		}
		case 109: // m - //Not used
		{
			break;
		}
		case 110: // n - Set SpZ (Main) Spindle Speed
		{
			configMain.speedPercentSpindle_Main = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
			break;
		}
		case 111: // o - Rose: Speed Percentage Z Axis 
		{
			if (configRose.axisId == ID_AXIS_Z)
			{
				configRose.speedPercent_Axis_Z = (int)GetSerialFloat(serialId);
			}
			else
			{
				configRose.speedPercent_Axis_X = (int)GetSerialFloat(serialId);
			}
			
			EEPROM.put(eePromAddress_Rose, configRose);
			break;
		}
		case 112: // p - Z/Sync Spindle Microsteps
		{	
			configMain.microsteps_Spindle = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
			// Microsteps set on PCB
			//SetMicrosteppingMode(configMain.microsteps_Spindle, PIN_SPINDLE_MS0, PIN_SPINDLE_MS1, PIN_SPINDLE_MS2);
#ifdef DEBUG
			Serial.print("microsteps_Spindle:");
			Serial.println(configMain.microsteps_Spindle);
#endif // DEBUG
			break;
		}
		case 113: // q - Move Z counterclockwise
		{
			MoveAxis(ID_AXIS_Z,DIR_CW);
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
				case PAGE_MAIN:
				{
					configPageMain.accel_Axis_B = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_pageMain, configPageMain);
#ifdef DEBUG
					Serial.print("accel_Axis_B:");
					Serial.println(configPageMain.accel_Axis_B);
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
				case PAGE_MAIN:
				{
					configPageMain.maxSpd_Axis_B = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_pageMain, configPageMain);
#ifdef DEBUG
					Serial.print("maxSpd_Axis:");
					Serial.println(configPageMain.maxSpd_Axis_B);
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
			// Microsteps set on PCB
			//SetMicrosteppingMode(configMain.microsteps_Axis_B, PIN_AXIS_B_MS0, PIN_AXIS_B_MS1, PIN_AXIS_B_MS2);
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
			// Microsteps set on PCB
			//SetMicrosteppingMode(configMain.microsteps_Axis_Z, PIN_AXIS_Z_MS0, PIN_AXIS_Z_MS1, PIN_AXIS_Z_MS2);
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
#ifdef DEBUG
			Serial.print("distancePerRev_AxisZ:");
			Serial.println(configMain.distancePerRev_AxisZ);
#endif // DEBUG
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
				case PAGE_MAIN:
				{
					configPageMain.accel_Axis_Z = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_pageMain, configPageMain);
#ifdef DEBUG
					Serial.print("accel_Axis:");
					Serial.println(configPageMain.accel_Axis_Z);
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
				case PAGE_REC:
				{
					switch (configReci.axisId)
					{
						case ID_AXIS_Z:
						{
							configReci.accel_Axis_Z = GetSerialFloat(serialId);
							EEPROM.put(eePromAddress_Rec, configReci);
	#ifdef DEBUG
							Serial.print("accel_Axis_Z:");
							Serial.println(configReci.accel_Axis_Z);
	#endif // DEBUG
							break;
						}
						case ID_AXIS_X:
						{
							configReci.accel_Axis_X = GetSerialFloat(serialId);
							EEPROM.put(eePromAddress_Rec, configReci);
	#ifdef DEBUG
							Serial.print("accel_Axis_X:");
							Serial.println(configReci.accel_Axis_X);
	#endif // DEBUG
							break;
						}
					}

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
		case 125: // } - Rose: Z Axis Accel
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
			configMain.speedPercentSpindle_Main = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("speedPercentSpindle_SpX:");
			Serial.println(configMain.speedPercentSpindle_Main);
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
			float nextSpeed_AxisX = configMain.speedPercent_Axis_X  * configPageMain.maxSpd_Axis_X * .01;
			Serial.print("nextSpeed_AxisX:");
			Serial.println(nextSpeed_AxisX);
#endif // DEBUG
			break;
		}
		case 163: // £ - Not used
		{
			break;
		}
		case 164: // ¤ - Not used
		{
			break;
		}
		case 165: // ¥ - Sp-X Axis CCW
		{
			RunTwoSteppers_All(DIR_CCW, DIR_CCW, ID_AXIS_X);
			break;
		}
		case 166: // ¦ - Sp-X Axis CW
		{
			RunTwoSteppers_All(DIR_CW, DIR_CW, ID_AXIS_X);

			break;
		}
		case 167: // § - SpB spindle speed
		{
			configMain.speedPercentSpindle_Main = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("speedPercentSpindle_SpB:");
			Serial.println(configMain.speedPercentSpindle_Main);
#endif // DEBUG
			break;
		}
		case 168: // ¨ - SpB axis speed
		{
			configMain.speedPercent_Axis_B = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("speedPercent_Axis_B:");
			Serial.println(configMain.speedPercent_Axis_B);
#endif // DEBUG
			break;
		}
		case 169: // © - Not used
		{
			break;
		}
		case 170: // ª - Not used
		{
			break;
		}
		case 171: // « - Sp-B Axis CCW
		{
			RunTwoSteppers_All(DIR_CCW, DIR_CCW, ID_AXIS_B);
			break;
		}
		case 172: // ¬ - Sp-B Axis CW
		{
			RunTwoSteppers_All(DIR_CW, DIR_CW, ID_AXIS_B);
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
		case 182: // ¶ - Greek Key: Pattern Type  (2 through 5)
		{
			int pattern = GetSerialInteger();// 

			configGreekKey_Main.patternId = pattern;
			EEPROM.put(eePromAddress_GreekKey_Main, configGreekKey_Main);

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
				case PAGE_MAIN:
				{
					configPageMain.maxSpd_Axis_X = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_pageMain, configPageMain);
#ifdef DEBUG
					Serial.print("maxSpd_Axis:");
					Serial.println(configPageMain.maxSpd_Axis_X);
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
				case PAGE_MAIN:
				{
					configPageMain.accel_Axis_X = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_pageMain, configPageMain);
#ifdef DEBUG
					Serial.print("accel_Axis:");
					Serial.println(configPageMain.accel_Axis_X);
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
			// Microsteps set on PCB
			//SetMicrosteppingMode(configMain.microsteps_Axis_X, PIN_AXIS_X_MS0, PIN_AXIS_X_MS1, PIN_AXIS_X_MS2);
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
		case 190: // ¾ - Index Source:Fixed or File
		{
			GetFilenameFromSerial();
			break;
		}
		case 191: // ¿ - Index: Size
		{
			double newIndexSize = GetSerialFloat(serialId);
			int stepsPerRevolution = (int)round(configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle);
#ifdef DEBUG
			Serial.print("---Index-indexId: ");
			Serial.println(configIndex_Main.indexId);
			Serial.print("----newIndexSize: ");
			Serial.println(newIndexSize);
#endif // DEBUG
			switch (configIndex_Main.indexId)
			{
				case 1:
				{
					// Default: Divisions
					if (configIndex_1.degreeOrDivision == BY_DEGREES) // Degrees
					{
						configIndex_1.size = stepsPerRevolution * (newIndexSize / 360);
					}
					else
					{
						configIndex_1.size = stepsPerRevolution / (newIndexSize);
					}
	#ifdef DEBUG
					Serial.print("Index1-Size(191):");
					Serial.println(configIndex_1.size);
	#endif // DEBUG
					EEPROM.put(eePromAddress_Index_1, configIndex_1);
					break;
				}
				case 2:
				{
					// Default: Divisions
					if (configIndex_2.degreeOrDivision == BY_DEGREES) // Degrees
					{
						configIndex_2.size = stepsPerRevolution * (newIndexSize / 360);
					}
					else
					{
						configIndex_2.size = stepsPerRevolution / (newIndexSize);
					}
	#ifdef DEBUG
					Serial.print("Index2-Size(191):");
					Serial.println(configIndex_2.size);
	#endif // DEBUG
					EEPROM.put(eePromAddress_Index_2, configIndex_2);
					break;
				}
				case 3:
				{
					// Default: Divisions
					if (configIndex_3.degreeOrDivision == BY_DEGREES) // Degrees
					{
						configIndex_3.size = stepsPerRevolution * (newIndexSize / 360);
					}
					else
					{
						configIndex_3.size = stepsPerRevolution / (newIndexSize);
					}

	#ifdef DEBUG
					Serial.print("Index3-Size(191):");
					Serial.println(configIndex_3.size);
	#endif // DEBUG
					EEPROM.put(eePromAddress_Index_3, configIndex_3);
					break;
				}
			}

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
		case 196: // Ä - Index:SourceFixed
		{
			int newSource = GetSerialInteger();
#ifdef DEBUG
			Serial.print("newSource(196):");
			Serial.println(newSource);
#endif // DEBUG
			switch (configIndex_Main.indexId)
			{
				case 1:
				{
					configIndex_1.fileOrFixed = newSource;
					if (configIndex_1.fileOrFixed == 49)
					{
						configIndex_1.fileOrFixed = 1;
					}
					else if (configIndex_1.fileOrFixed == 48)
					{
						configIndex_1.fileOrFixed = 0;
					}
					EEPROM.put(eePromAddress_Setup, configMain);
	#ifdef DEBUG
					Serial.print("source_Index1(196):");
					Serial.println(configIndex_1.fileOrFixed);
	#endif // DEBUG
					break;
				}
				case 2:
				{
					configIndex_2.fileOrFixed = newSource;
					if (configIndex_2.fileOrFixed == 49)
					{
						configIndex_2.fileOrFixed = 1;
					}
					else if (configIndex_2.fileOrFixed == 48)
					{
						configIndex_2.fileOrFixed = 0;
					}
					EEPROM.put(eePromAddress_Setup, configMain);
	#ifdef DEBUG
					Serial.print("source_Index2(196):");
					Serial.println(configIndex_2.fileOrFixed);
	#endif // DEBUG
					break;
				}
				case 3:
				{
					configIndex_3.fileOrFixed = newSource;
					if (configIndex_3.fileOrFixed == 49)
					{
						configIndex_3.fileOrFixed = 1;
					}
					else if (configIndex_3.fileOrFixed == 48)
					{
						configIndex_3.fileOrFixed = 0;
					}
					EEPROM.put(eePromAddress_Setup, configMain);
	#ifdef DEBUG
					Serial.print("source_Index3(196):");
					Serial.println(configIndex_3.fileOrFixed);
	#endif // DEBUG
					break;
				}
			}

			break;
		}
		case 197: // Å - pageMain: Set axisId
		{
			configPageMain.axisId = GetSerialInteger();
#ifdef DEBUG
			Serial.print("configPageMain.axisId:");
			Serial.println(configPageMain.axisId);
#endif // DEBUG
			EEPROM.put(eePromAddress_pageMain, configPageMain);
#ifdef DEBUG
			Serial.print("configPageMain.axisId:");
			Serial.println(configPageMain.axisId);
#endif // DEBUG
			break;
		}
		case 198: // Æ - Do Rec AxialZ 
		{
			int waveDir = GetSerialFloat(serialId);
#ifdef DEBUG
			Serial.print("waveDir(197: ");
			Serial.println(waveDir);
#endif // DEBUG

			// In: -1  Out: 1
			if (waveDir != WAVE_OUT)
			{
				waveDir = WAVE_IN;
			}

#ifdef DEBUG
			Serial.print("waveDir(197: ");
			Serial.println(waveDir);
			Serial.print("configReci.axisId(197: ");
			Serial.println(configReci.axisId);
			Serial.print("configReci.radial_axial(197: ");
			Serial.println(configReci.radial_axial);
#endif // DEBUG

			switch (configReci.axisId)
			{
				case ID_AXIS_Z: // Z Axis
				{
					switch (configReci.radial_axial)
					{
						case RADIAL: // Radial
						{
							Reciprocate_RadialZ(waveDir);
							break;
						}
						case AXIAL: // Axial
						{
							Reciprocate_AxialZ(waveDir);
							break;
						}
					}
					break;
				}
				case ID_AXIS_X: // X Axis
				{
					switch (configReci.radial_axial)
					{
						case RADIAL: // Radial
						{
							Reciprocate_RadialX(waveDir);
							break;
						}
						case AXIAL: // Axial
						{
							Reciprocate_AxialX(waveDir);
							break;
						}
					}
					break;
				}
			}

			break;
		}
		case 199: // Ç - Rose: Z axis Radial Amplitude
		{
			configRose.amplitude_Radial_Z = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rose, configRose);
#ifdef DEBUG
			Serial.print("amplitude_Axis_Z:");
			Serial.println(configRose.amplitude_Radial_Z);
#endif // DEBUG
			break;
		}
		case 200: // È - Greek Key: Spindle Accel
		{
			//int speedBarId = GetSerialIntegerOnly();
			int accel = (int)GetSerialFloat(serialId);
			Serial.print("accel:");
			Serial.println(accel);
			switch (configGreekKey_Main.axisId)
			{
				case ID_AXIS_Z: // Greek Key Z: Spindle accel
				{
					configGreekKey_Z.accel_Spindle = accel;
					EEPROM.put(eePromAddress_GreekKey_Z, configGreekKey_Z);
					break;
				}
				case ID_AXIS_X: // Greek Key X: Spindle accel
				{
					configGreekKey_X.accel_Spindle = accel;
					EEPROM.put(eePromAddress_GreekKey_X, configGreekKey_X);
					break;
				}
			}
			break;
		}
		case 201: // É - DoRosePattern_X
		{
#ifdef DEBUG
			Serial.print("configRose.axisId:");
			Serial.println(configRose.axisId);
#endif // DEBUG

			switch (configRose.axisId)
			{
				case ID_AXIS_Z: // Z axis
				{
					RoseRadial_Z(DIR_CW);
					break;
				}
				case ID_AXIS_X: // X axis
				{
					RoseRadial_X(DIR_CW);
					break;
				}
			}

			break;
		}
		case 202: // Ê - Axis MaxSpeed 
		{
			pageCallerId = GetSerialIntegerOnly();
			switch (pageCallerId)
			{
				case PAGE_MAIN:
				{
					configPageMain.maxSpd_Axis_Z = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_pageMain, configPageMain);
#ifdef DEBUG
					Serial.print("maxSpd:");
					Serial.println(configPageMain.maxSpd_Axis_Z);
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
				case PAGE_REC:
				{
					switch (configReci.axisId)
					{
						case ID_AXIS_Z:
						{
							configReci.maxSpd_Axis_Z = GetSerialFloat(serialId);
							EEPROM.put(eePromAddress_Rec, configReci);
	#ifdef DEBUG
							Serial.print("maxSpd_Axis_Z:");
							Serial.println(configReci.maxSpd_Axis_Z);
	#endif // DEBUG
							break;
						}
						case ID_AXIS_X:
						{
							configReci.maxSpd_Axis_X = GetSerialFloat(serialId);
							EEPROM.put(eePromAddress_Rec, configReci);
	#ifdef DEBUG
							Serial.print("maxSpd_Axis_X:");
							Serial.println(configReci.maxSpd_Axis_X);
	#endif // DEBUG
							break;
						}
					}

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
		case 203: // Ì - Rec1_Z Axial Waves (Count)
		{
			configReci.waves_AxialZ = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("waves_AxialZ:");
			Serial.println(configReci.waves_AxialZ);
#endif // DEBUG
			break;
		}
		case 204: // Ë - Rec1_Z Axial Amplitude
		{
			configReci.amplitude_AxialZ = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rec, configReci);
#ifdef DEBUG
			Serial.print("amplitude_AxialZ:");
			Serial.println(configReci.amplitude_AxialZ);
#endif // DEBUG
			break;
		}
		case 205: // Í - Rec_Z Axial Distance
		{
			configReci.distance_AxialZ = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rec, configReci);
#ifdef DEBUG
			Serial.print("distance_AxialZ:");
			Serial.println(configReci.distance_AxialZ);
#endif // DEBUG
			break;
		}
		case 206: // Î -  Return Spindle and Z axis to start positions
		{
			pageCallerId = GetSerialInteger();
			ReturnToStartPosition(ID_AXIS_Z);
			break;
		}
		case 207: // Ï - Do Rec1_S
		{
			int waveDir = GetSerialFloat(serialId);

			// In: -1  Out: 1
			if (waveDir != WAVE_OUT)
			{
				waveDir = WAVE_IN;
			}
			Reciprocate_RadialZ(waveDir);
			break;
		}
		case 208: // Ð - SP: Active Stepper
		{
			configOne.activeAxis = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_pageOne, configOne);
			break;
		}
		case 209: // Ñ - Reci AxialX and RadialX Axis Speed percentage
		{
			configReci.speedPercent_Axis_X = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rec, configReci);
#ifdef DEBUG
			Serial.print("speedPercent_Axis_X:");
			Serial.println(configReci.speedPercent_Axis_X);
#endif // DEBUG
			break;
		}
		case 210: // Ò - Rec1_Spindle Spindle Speed percentage
		{
			configReci.speedPercent_Spindle = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rec, configReci);
#ifdef DEBUG
			Serial.print("speedPercentSpindle:");
			Serial.println(configReci.speedPercent_Spindle);
#endif // DEBUG
			break;
		}
		case 211: // Ó - Rec AxialZ and RadialZ Axis Speed percentage 
		{
			configReci.speedPercent_Axis_Z = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rec, configReci);
#ifdef DEBUG
			Serial.print("speedPercentAxis:");
			Serial.println(configReci.speedPercent_Axis_Z);
#endif // DEBUG
			break;
		}
		case 212: // Ô - Rec Z radial Waves (Count)
		{
			configReci.waves_RadialZ = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rec, configReci);
#ifdef DEBUG
			Serial.print("waves_RadialZ:");
			Serial.println(configReci.waves_RadialZ);
#endif // DEBUG
			break;
		}
		case 213: // Õ - Rec Z radial Degrees
		{
			configReci.degrees_RadialZ = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rec, configReci);
#ifdef DEBUG
			Serial.print("degrees_RadialZ:");
			Serial.println(configReci.degrees_RadialZ);
#endif // DEBUG
			break;
		}
		case 214: // Ö - Rec1 Z radial Amplitude Axis
		{
			configReci.amplitude_RadialZ = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rec, configReci);
#ifdef DEBUG
			Serial.print("amplitude_RadialZ:");
			Serial.println(configReci.amplitude_RadialZ);
#endif // DEBUG
			break;
		}
		case 215: // × - Return Spindle and X axis to start positions
		{
			pageCallerId = GetSerialInteger();
			ReturnToStartPosition(ID_AXIS_X);
			break;
		}
		case 216: // Ø - Polarity Spindle (High or Low)
		{
			int polaritySpindle = GetSerialInteger();
#ifdef DEBUG
			Serial.print("polaritySpindle:");
			Serial.println(polaritySpindle);
#endif // DEBUG
			// HIGH = 1, LOW = 0
			polaritySpindle >= 1 ? (configMain.polarity_Spindle = true) : (configMain.polarity_Spindle = false);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("configMain.polarity_Spindle:");
			Serial.println(configMain.polarity_Spindle);
#endif // DEBUG
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
			configMain.limit_Min_Z = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("limit_Min_Z:");
			Serial.println(configMain.limit_Min_Z);
#endif // DEBUG
			break;
		}
		case 221: // Ý - Limit switch Z Max
		{
			configMain.limit_Max_Z = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("limit_Max_Z:");
			Serial.println(configMain.limit_Max_Z);
#endif // DEBUG
			break;
		}
		case 222: // Þ - Limit switch X Min
		{
			configMain.limit_Min_X = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("limit_Min_X:");
			Serial.println(configMain.limit_Min_X);
#endif // DEBUG
			break;
		}
		case 223: // ß - Limit switch X Max
		{
			configMain.limit_Max_X = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("limit_Max_X:");
			Serial.println(configMain.limit_Max_X);
#endif // DEBUG
			break;
		}
		case 224: // à - Limit switch B Min
		{
			configMain.limit_Min_B = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("limit_Min_B:");
			Serial.println(configMain.limit_Min_B);
#endif // DEBUG
			break;
		}
		case 225: // á - Limit switch B Max
		{
			configMain.limit_Max_B = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("limit_Max_B:");
			Serial.println(configMain.limit_Max_B);
#endif // DEBUG
			break;
		}
		case 226: // â - Rose: X Axis MaxSpd
		{
			configRose.maxSpd_Axis_X = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rose, configRose);
#ifdef DEBUG
			Serial.print("maxSpd_Axis_X:");
			Serial.println(configRose.maxSpd_Axis_X);
#endif // DEBUG
			break;
		}
		case 227: // ã - Rose: X Axis Accel
		{
			configRose.accel_Axis_X = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rose, configRose);
#ifdef DEBUG
			Serial.print("accel_Axis_X:");
			Serial.println(configRose.accel_Axis_X);
#endif // DEBUG
			break;
		}
		case 228: // ä - Rose: X Axis Radial Amplitude
		{
			configRose.amplitude_Radial_X= GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rose, configRose);
#ifdef DEBUG
			Serial.print("amplitude_Axis_X:");
			Serial.println(configRose.amplitude_Radial_X);
#endif // DEBUG
			break;
		}
		case 229: // å - Rose: Spindle Amplitude
		{
			configRose.amplitude_Axial_Z = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rose, configRose);
#ifdef DEBUG
			Serial.print("amplitude_Axial Z:");
			Serial.println(configRose.amplitude_Axial_Z);
#endif // DEBUG
			break;
		}
		case 230: // å - Greek Key File: Pattern Count/360
		{
			configGreekKey_Main.countPatternFile360 = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_GreekKey_Main, configGreekKey_Main);
#ifdef DEBUG
			Serial.print("configGreekKey_Main.countPatternFile360:");
			Serial.println(configGreekKey_Main.countPatternFile360);
#endif // DEBUG
			break;
		}
		case 231: // ç - Rose: Return Z and Spindle
		{
			int axisId = GetSerialInteger();
			pageCallerId = PAGE_GEO;
			ReturnToStartPosition(axisId);
			break;
		}
		case 232: // è - Return Spindle MaxSpd
		{
			configMain.maxSpd_Return_Spindle = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("maxSpd_Return_Spindle:");
			Serial.println(configMain.maxSpd_Return_Spindle);
#endif // DEBUG
			break;
		}
		case 233: // é - Return Spindle Accel
		{
			configMain.accel_Return_Spindle = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("accel_Return_Spindle:");
			Serial.println(configMain.accel_Return_Spindle);
#endif // DEBUG
			break;
		}
		case 234: // ê - Return Spindle Accel
		{
			configMain.maxSpd_Return_Axis = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("maxSpd_Return_Axis:");
			Serial.println(configMain.maxSpd_Return_Axis);
#endif // DEBUG
			break;
		}
		case 235: // ë - Return Spindle Accel
		{
			configMain.accel_Return_Axis = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configMain);
#ifdef DEBUG
			Serial.print("accel_Return_Axis:");
			Serial.println(configMain.accel_Return_Axis);
#endif // DEBUG
			break;
		}
		case 236: //ì- Clear Stepper positions and set pageCallerId
		{
#ifdef DEBUG
			Serial.println("enter set pageCallerId.");
#endif // DEBUG
			returnSteps_Axis = 0;
			returnSteps_Spindle = 0;
			endPosition_Axis = 0;
			endPosition_Spindle = 0;

			pageCallerId = GetSerialInteger();
#ifdef DEBUG
			Serial.print("pageCallerId:");
			Serial.println(pageCallerId);
#endif // DEBUG
			break;
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
		case 238: // î - Rec1 axisId
		{
			configReci.axisId = GetSerialInteger();
			EEPROM.put(eePromAddress_Rec, configReci);
			break;
		}
		case 239: // ï - Rec1 AxialX Waves
		{
			configReci.waves_AxialX = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rec, configReci);
#ifdef DEBUG
			Serial.print("waves_AxialX:");
			Serial.println(configReci.waves_AxialX);
#endif // DEBUG
			break;
		}
		case 240: // ð - Rec1 AxialX Amplitude
		{
			configReci.amplitude_AxialX = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rec, configReci);
#ifdef DEBUG
			Serial.print("amplitude_AxialX:");
			Serial.println(configReci.amplitude_AxialX);
#endif // DEBUG
			break;
		}
		case 241: // ñ - Rec1 AxialX Distance
		{
			configReci.distance_AxialX = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rec, configReci);
#ifdef DEBUG
			Serial.print("distance_AxialX:");
			Serial.println(configReci.distance_AxialX);
#endif // DEBUG
			break;
		}
		case 242: // ò - Rec1 RadialX Waves
		{
			configReci.waves_RadialX = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rec, configReci);
#ifdef DEBUG
			Serial.print("waves_RadialX:");
			Serial.println(configReci.waves_RadialX);
#endif // DEBUG
			break;
		}
		case 243: // ó - Rec1 RadialX Degrees
		{
			configReci.degrees_RadialX = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rec, configReci);
#ifdef DEBUG
			Serial.print("degrees_RadialX:");
			Serial.println(configReci.degrees_RadialX);
#endif // DEBUG
			break;
		}
		case 244: // ô - Rec1 RadialX Amplitude
		{
			configReci.amplitude_RadialX = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rec, configReci);
#ifdef DEBUG
			Serial.print("amplitude_RadialX:");
			Serial.println(configReci.amplitude_RadialX);
#endif // DEBUG
			break;
		}
		case 245: // õ - Rose X Axial Amplitude
		{
			configRose.amplitude_Axial_X = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rose, configRose);
#ifdef DEBUG
			Serial.print("amplitude_Axis_X:");
			Serial.println(configRose.amplitude_Axial_X);
#endif // DEBUG
			break;
		}
		case 246: // ö - IndexId 
		{
			configIndex_Main.indexId = GetSerialInteger();
#ifdef DEBUG
			Serial.print("configIndex_Main.axisId :");
			Serial.println(configIndex_Main.indexId);
#endif // DEBUG
			EEPROM.put(eePromAddress_Index_Main, configIndex_Main);
			break;
		}
		case 247: // ÷ - Do Greek Key (X and Z)(Left and Right 
		{
			reverseDirection = GetSerialInteger();
			if (reverseDirection == 0)
			{
				reverseDirection = -1; // Nextion won't send negative number
			}
#ifdef DEBUG
			Serial.print("configGreekKey_Main.axisId:");
			Serial.println(configGreekKey_Main.axisId);
			Serial.print("configGreekKey_Main.pattern:");
			Serial.println(configGreekKey_Main.patternId);
			Serial.print("reverseDirection:");
			Serial.println(reverseDirection);
#endif // DEBUG
			switch (configGreekKey_Main.patternId)
			{
				case 2:
				case 50:
				{
					GreekKey_Pattern_4a();
					break;
				}
				case 3:
				case 51:
				{
					GreekKey_Pattern_4b();
					break;
				}
				case 4:
				case 52:
				{
					GreekKey_Pattern_2a();
					break;
				}
				case 5:
				case 53:
				{
					GreekKey_Pattern_2b();
					break;
				}
				case 6:
				case 54:
				{
					GreekKey_Pattern_3a();
					break;
				}
				case 7:
				case 55:
				{
					GreekKey_Pattern_3b();
					break;
				}
			}

			break;
		}
		case 248: // õ - Greek Key MaxSpd
		{
			int maxSpeed = (int)GetSerialFloat(serialId);

#ifdef DEBUG
			Serial.print("maxSpeed:");
			Serial.println(maxSpeed);
#endif // DEBUG

			switch (configGreekKey_Main.axisId)
			{
				case 0: // Greek Key Z: Z Axis MaxSpd
				{
					configGreekKey_Z.maxSpd_Spindle = maxSpeed;
					EEPROM.put(eePromAddress_GreekKey_Z, configGreekKey_Z);
					break;
				}
				case 1: // Greek Key X: X Axis MaxSpd
				{
					configGreekKey_X.maxSpd_Spindle = maxSpeed;
					EEPROM.put(eePromAddress_GreekKey_X, configGreekKey_X);
					break;
				}
			}

			break;
		}
		case 249: // ù - Do Greek Key Axis Speed Percentage
		{
			int percentage = (int)GetSerialFloat(serialId);

#ifdef DEBUG
			Serial.print("configGreekKey_Main.axisId:");
			Serial.println(configGreekKey_Main.axisId);
			Serial.print("percentage:");
			Serial.println(percentage);
#endif // DEBUG

			if (configGreekKey_Main.axisId == 0)
			{
				configGreekKey_Z.speedPercent_Axis = percentage;
				EEPROM.put(eePromAddress_GreekKey_Z, configGreekKey_Z);
			}
			else
			{
				configGreekKey_X.speedPercent_Axis = percentage;
				EEPROM.put(eePromAddress_GreekKey_X, configGreekKey_X);
			}

			break;
		}
		case 250: // ù -Greek Key: Axis Accel
		{
			int accel = (int)GetSerialFloat(serialId);
			Serial.print("accel:");
			Serial.println(accel);
			switch (configGreekKey_Main.axisId)
			{
				case ID_AXIS_Z: // Greek Key Z: Z Axis accel
				case 49:
				{
					configGreekKey_Z.accel_Axis = accel;
					EEPROM.put(eePromAddress_GreekKey_Z, configGreekKey_Z);
					break;
				}
				case ID_AXIS_X: // Greek Key X: Spindle accel
				case 50:
				{
					configGreekKey_X.accel_Axis = accel;
					EEPROM.put(eePromAddress_GreekKey_X, configGreekKey_X);
					break;
				}
			}
			break;
		}
		case 251: // û - Greek Key Leg Length
		{
			configGreekKey_Main.segmentLengthPattern = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_GreekKey_Main, configGreekKey_Main);
#ifdef DEBUG
			Serial.print("configGreekKey_Main.segmentLengthPattern:");
			Serial.println(configGreekKey_Main.segmentLengthPattern);
			Serial.print("configGreekKey_Main.segmentLengthPattern:");
			Serial.println(configGreekKey_Main.segmentLengthPattern);
#endif // DEBUG

			break;
		}
		case 252: // ü - Greek Key Count/360
		{
			configGreekKey_Main.countPattern360 = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_GreekKey_Main, configGreekKey_Main);
#ifdef DEBUG
			Serial.print("configGreekKey_Main.countPattern360:");
			Serial.println(configGreekKey_Main.countPattern360);
			Serial.print("configGreekKey_Main.countPattern360:");
			Serial.println(configGreekKey_Main.countPattern360);
#endif // DEBUG
			break;
		}
		case 253: // ü - Greek Key AxisId
		{
			configGreekKey_Main.axisId = GetSerialInteger();
			EEPROM.put(eePromAddress_GreekKey_Main, configGreekKey_Main);

#ifdef DEBUG
			Serial.print("configGreekKey_Main.axisId:");
			Serial.println(configGreekKey_Main.axisId);
			Serial.print("configGreekKey_Main.axisId:");
			Serial.println(configGreekKey_Main.axisId);
#endif // DEBUG
			break;
		}
		case 254: // þ - Greek Key Spindle Speed Percentage
		{
			int percentage = (int)GetSerialFloat(serialId);

			if (configGreekKey_Main.axisId == 0)
			{
				configGreekKey_Z.speedPercent_Spindle = percentage;
				EEPROM.put(eePromAddress_GreekKey_Z, configGreekKey_Z);
			}
			else
			{
				configGreekKey_X.speedPercent_Spindle = percentage;
				EEPROM.put(eePromAddress_GreekKey_X, configGreekKey_X);
			}
#ifdef DEBUG
			Serial.print("percentage:");
			Serial.println(percentage);
			Serial.print("configGreekKey_Z.speedPercent_Spindle:");
			Serial.println(configGreekKey_Z.speedPercent_Spindle);
			Serial.print("configGreekKey_X.speedPercent_Spindle:");
			Serial.println(configGreekKey_X.speedPercent_Spindle);
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