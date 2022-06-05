#pragma once
/* *****************************************************************
* 4Rose main entry
* Author: Edward French
* Version: v3.0.f.beta - 05/09/22
******************************************************************/

#include "math.h"
#include <string>
#include <EEPROM.h>

#include "4RoseDefines.h"
#include "SD.h"
#include "SPI.h"
#include "TeensyStep.h" //  https://github.com/luni64/TeensyStep
#include "IniFile.h" // https://github.com/stevemarple/IniFile
//#include <MemoryFree.h>


//==================================================================
// Pin assignments:  See 4RoseDefines.h
//==================================================================
// TeensyStep  initialization
Stepper stepperSpindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
Stepper stepperAxis_Z(PIN_AXIS_Z_STEP, PIN_AXIS_Z_DIR);
Stepper stepperAxis_X(PIN_AXIS_X_STEP, PIN_AXIS_X_DIR);
Stepper stepperAxis_M3(PIN_AXIS_3_STEP, PIN_AXIS_3_DIR);
Stepper stepperAxis_M4(PIN_AXIS_4_STEP, PIN_AXIS_4_DIR);

//Stepper stepperAxis_MainX(PIN_AXIS_X_STEP, PIN_AXIS_X_DIR);

// TeensyStep controllers
RotateControl controllerAxis;

RotateControl controllerSpindle;

/// <summary>
/// Setup
/// </summary>
/// <comment>
/// Initial setup
/// </comment>
/// <returns></returns>
void setup()
{
	//const char* nextionEnd = "\xFF\xFF\xFF";
	pinMode(LED_BUILTIN, OUTPUT);

	// Initialize Serial0 (USB port) Baud rate is always at USB max.
	Serial.begin(115200);
	MilliDelay(15);
	uint32_t serialBaud = Serial.baud();

#ifdef DEBUG
	SerialPrint("Baud:");
	SerialPrintln(serialBaud);
#endif // DEBUG


	// Initialize Nextion LCD for 115200 baud and enable Serial1,2,or3.
	// Note: Nextion requires 3 0xFF bytes to signal end of transmission
	// Serial1

	//Serial1.begin(9600); //Nextion Serial baud rate set in Nextion pageMain Preinitialize Event tab
	//Serial1.print("bauds=9600");
	//Serial1.print(nextionEnd);
	//MilliDelay(50);
	//Serial1.print("bauds=9600");
	//Serial1.print(nextionEnd);
	//MilliDelay(50);


	Serial1.begin(921600); //Nextion Serial baud rate set in Nextion pageMain Preinitialize Event tab
	Serial1.print("bauds=921600");
	Serial1.print(nextionEnd);
	MilliDelay(50);
	Serial1.print("bauds=921600");
	Serial1.print(nextionEnd);
	MilliDelay(50);
	Serial1.print("bkcmd=0");  // Set Nextion to return NO replies to each command
	Serial1.print(nextionEnd);
	MilliDelay(50);

	if (Serial1.available() > 0)
	{
		serialId = 1;
#ifdef DEBUG
		Serial.print("1-serialId: ");
		Serial.println(serialId);
#endif // Debug
	}
	// Serial2
	Serial2.begin(921600); //Nextion Serial baud rate set in Nextion pageMain Preinitialize Event tab
	Serial2.print("bauds=921600");
	Serial2.print(nextionEnd);
	MilliDelay(50);
	Serial2.print("bauds=921600");
	Serial2.print(nextionEnd);
	MilliDelay(50);
	Serial2.print("bkcmd=0");  // Set Nextion to return NO replies to each command
	Serial2.print(nextionEnd);
	MilliDelay(50);
	if (Serial2.available() > 0)
	{
		serialId = 2;
#ifdef DEBUG
		Serial.print("2-serialId: ");
		Serial.println(serialId);
#endif // Debug
	}

	// Serial3
	//Serial3.begin(115200); //Nextion Serial baud rate set in Nextion pageMain Preinitialize Event tab
	Serial3.begin(921600); //Nextion Serial baud rate set in Nextion pageMain Preinitialize Event tab
	//Serial3.print("bauds=115200");
	Serial3.print("bauds=921600");
	Serial3.print(nextionEnd);
	MilliDelay(50);
	//Serial3.print("bauds=115200");
	Serial3.print("bauds=921600");
	Serial3.print(nextionEnd);
	MilliDelay(50);
	Serial3.print("bkcmd=0");  // Set Nextion to return NO replies to each command
	Serial3.print(nextionEnd);
	MilliDelay(50);
	if (Serial3.available() > 0)
	{
		serialId = 3;
#ifdef DEBUG
		Serial.print("3-serialId: ");
		Serial.println(serialId);
#endif // Debug
	}

	// Serial4
//
//	Serial4.begin(921600); //Nextion Serial baud rate set in Nextion pageMain Preinitialize Event tab
//	Serial4.print("bauds=921600");
//	Serial4.print(nextionEnd);
//	MilliDelay(50);
//	//Serial3.print("bauds=115200");
//	Serial4.print("bauds=921600");
//	Serial4.print(nextionEnd);
//	MilliDelay(50);
//	Serial4.print("bkcmd=0");  // Set Nextion to return NO replies to each command
//	Serial4.print(nextionEnd);
//	MilliDelay(50);
//	if (Serial4.available() > 0)
//	{
//		serialId = 4;
//#ifdef DEBUG
//		Serial.print("4-serialId: ");
//		Serial.println(serialId);
//#endif // Debug
//	}


#ifdef DEBUG
	Serial.print("serialId: ");
	Serial.println(serialId);
#endif // Debug
	// Update with values from EEProm
	EEPROM.get(eePromAddress_Setup, configSetup);
	EEPROM.get(eePromAddress_Main, configMain);
#ifdef DEBUG
	Serial.print("                                eePromAddress_Main: ");
	Serial.println(eePromAddress_Main);
#endif // DEBUG
	EEPROM.get(eePromAddress_Sync, configSync);

	EEPROM.get(eePromAddress_Multi, configMulti);
	EEPROM.get(eePromAddress_Ind_Prime, configIndex_Prime);
	EEPROM.get(eePromAddress_Ind_1, configIndex_1);
	EEPROM.get(eePromAddress_Ind_2, configIndex_2);
	EEPROM.get(eePromAddress_Ind_3, configIndex_3);
	EEPROM.get(eePromAddress_Ind_4, configIndex_4);
	EEPROM.get(eePromAddress_Ind_5, configIndex_5);
	EEPROM.get(eePromAddress_Mov, configMove);
	EEPROM.get(eePromAddress_Rose, configRose);
	EEPROM.get(eePromAddress_Rec, configRec);
	EEPROM.get(eePromAddress_Grk, configGreekKey);

#ifdef DEBUG
	Serial.print("configMain.axisId: ");
	Serial.println(configMain.axisId);
	Serial.print("configOne.axisId: ");
	Serial.println(configMulti.axisId);
	Serial.print("configIndex_Main.indexId: ");
	Serial.println(configIndex_Prime.indexId);
	Serial.print("configMove.axisId: ");
	Serial.println(configMove.axisId);
	Serial.print("configSync.axisId: ");
	Serial.println(configSync.axisId);
	Serial.print("configRec.axisId: ");
	Serial.println(configRec.axisId);
	Serial.print("configGreekKey.axisId: ");
	Serial.println(configGreekKey.axisId);
	Serial.print("configRose.axisId: ");
	Serial.println(configRose.axisId);
#endif // DEBUG

	// Config as well as all other EEProm settings should be run from Nextion whenever Teensy is updated.  
	// EEProm may contain invalid settings otherwise.

	pinMode(PIN_SPINDLE_ENABLE, OUTPUT);
	pinMode(PIN_AXIS_Z_ENABLE, OUTPUT);

	pinMode(PIN_AXIS_X_ENABLE, OUTPUT);
	pinMode(PIN_AXIS_3_ENABLE, OUTPUT);


	SetEnable(ID_SPINDLE, false, true);
	SetEnable(ID_AXIS_Z, false, true);
	//SetEnable(ID_AXIS_Z, true);
	SetEnable(ID_AXIS_X, false, true);
	SetEnable(ID_AXIS_3, false, true);

	//motorCount = configSetup.motorCount;
	//vendorId = configSetup.vendorId;
	//motorCount = 5;
	//vendorId = 4;


	if (configSetup.motorCount == 5)
	{
		pinMode(PIN_AXIS_4_ENABLE, OUTPUT);
		SetEnable(ID_AXIS_3, false, true);

		if (configSetup.limit_Min_M4 > 9 && configSetup.limit_Min_M4 < 40)
		{
			pinMode(configSetup.limit_Min_M4, INPUT_PULLUP);
			MilliDelay(10);
			digitalWrite(configSetup.limit_Min_M4, configSetup.limit_NCorNO);  // Enable
		}
		if (configSetup.limit_Max_M4 > 9 && configSetup.limit_Max_M4 < 40)
		{
			pinMode(configSetup.limit_Max_M4, INPUT_PULLUP);
			MilliDelay(10);
			digitalWrite(configSetup.limit_Max_M4, configSetup.limit_NCorNO);  // Enable
		}

	}

		// Initialize Limit switches	
	if(configSetup.limit_Min_Z >9 && configSetup.limit_Min_Z < 40)
	{ 
		pinMode(configSetup.limit_Min_Z, INPUT_PULLUP);
		MilliDelay(10);
		digitalWrite(configSetup.limit_Min_Z, configSetup.limit_NCorNO);  // Enable
	}
	if (configSetup.limit_Max_Z > 9 && configSetup.limit_Max_Z < 40)
	{ 
		pinMode(configSetup.limit_Max_Z, INPUT_PULLUP);
		MilliDelay(10);
		digitalWrite(configSetup.limit_Max_Z, configSetup.limit_NCorNO);  // Enable
	}

	if (configSetup.limit_Min_X > 9)
	{
		pinMode(configSetup.limit_Min_X, INPUT_PULLUP);
		MilliDelay(10);
		digitalWrite(configSetup.limit_Min_X, configSetup.limit_NCorNO);  // Enable
	}
	if (configSetup.limit_Max_X > 9)
	{
		pinMode(configSetup.limit_Max_X, INPUT_PULLUP);
		MilliDelay(10);
		digitalWrite(configSetup.limit_Max_X, configSetup.limit_NCorNO);  // Enable
	}

	if (configSetup.limit_Min_M3 > 9)
	{
		pinMode(configSetup.limit_Min_M3, INPUT_PULLUP);
		MilliDelay(10);
		digitalWrite(configSetup.limit_Min_M3, configSetup.limit_NCorNO);  // Enable
	}
	if (configSetup.limit_Max_M3 > 9)
	{
		pinMode(configSetup.limit_Max_M3, INPUT_PULLUP);
		MilliDelay(10);
		digitalWrite(configSetup.limit_Max_M3, configSetup.limit_NCorNO);  // Enable
	}

	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		pinMode(configSetup.cutterMotorPin, INPUT_PULLUP);
		digitalWrite(configSetup.cutterMotorPin, HIGH);  // Enable
	}

	MilliDelay(10);

	if (configSetup.eStop > 9 && configSetup.eStop < 40)
	{
		pinMode(configSetup.eStop, INPUT_PULLUP);
		digitalWrite(configSetup.eStop, configSetup.limit_NCorNO);  // Enable
	}

	MilliDelay(10);

	// Enable SD card reader
	//if(configSetup.motorCount<5)
	//{ 
	//	pinMode(PIN_SPI_CS_24, OUTPUT);
	//	digitalWrite(PIN_SPI_CS_24, HIGH);
	//	pinMode(PIN_SPI_CS_15, OUTPUT);
	//	digitalWrite(PIN_SPI_CS_15, HIGH);
	//	
	//}
    pinMode(PIN_SPI_CS_10, OUTPUT);
	digitalWrite(PIN_SPI_CS_10, HIGH);
	pinMode(PIN_SPI_CS_9, OUTPUT);
	digitalWrite(PIN_SPI_CS_9, HIGH);

	// Start SD
	BeginSD();

	SetEnable(ID_SPINDLE, false, true);

	for (int i = 0; i < 3; i++) // Verify Teensy is operational
	{
		digitalWrite(LED_BUILTIN, HIGH);
		MilliDelay(50);
		digitalWrite(LED_BUILTIN, LOW);
		MilliDelay(300);
	}

	// Set Spindle and M3 Synchro ratio
	synchro_Ratio = configSetup.gearRatio_AxisM3 / configSetup.gearRatio_Spindle;

	Serial.print("configSetup.gearRatio_Spindle: ");
	Serial.println(configSetup.gearRatio_Spindle,5);
	Serial.print("configSetup.gearRatio_AxisM3: ");
	Serial.println(configSetup.gearRatio_AxisM3,5);

	Serial.print("synchro_Ratio: ");
	Serial.println(synchro_Ratio,5);

#ifdef DEBUG
	const char* initialized = "Initialized...";
	Serial.println(initialized);
#endif // DEBUG


	// Uncomment to get size of defined structs.
	structSizeCalc();
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
	const char* pageCallerId_Char = "Page Caller ID:  ";
	//const char* axis_SpeedPercent_Char = "Axis Speed Percent:  ";
	//const char* spindle_SpeedPercent_Char = "Spindle Speed Percent:  ";
	//const char* x_SpeedPercent_Char = "X axis Speed Percent:  ";
	//const char* b_SpeedPercent_Char = "M3 axis Speed Percent:  ";
	//const char* z_SpeedPercent_Char = "Z axis Speed Percent:  ";
	const char* steps360_Char = "Steps/360: ";
	const char* gearRatio_Char = "Gear Ratio: ";
	const char* maxSpd_Char = "MaxSpeed:  ";
	const char* accel_Char = "Accel:  ";
	const char* polarity_Char = "Polarity:  ";
	const char* distance_Char = "Distance:  ";
	//const char * nextSpeed_Char = "Next Speed:";
	const char* axisId_Char = "Axis ID:  ";
	const char* indexId_Char = "Index ID:  ";
	const char* z_LimitMin_Char = "Limit Min Z:  ";
	const char* z_LimitMax_Char = "Limit Max Z:  ";
	//const char* z_Home_Char = "Home Z:  ";
	const char* x_LimitMin_Char = "Limit Min X:  ";
	const char* x_LimitMax_Char = "Limit Max X:  ";
	//const char* x_Home_Char = "Home X:  ";
	const char* m3_LimitMin_Char = "Limit Min M3:  ";
	const char* m3_LimitMax_Char = "Limit Max M3:  ";
	//const char* m3_Home_Char = "Home M3:  ";
	const char* m4_LimitMin_Char = "Limit Min M4:  ";
	const char* m4_LimitMax_Char = "Limit Max M4:  ";
	//const char* m4_Home_Char = "Home M4:  ";
	const char* index1_Char = "Index1:  ";
	const char* index2_Char = "Index2:  ";
	const char* index3_Char = "Index3:  ";
	const char* index4_Char = "Index4:  ";
	const char* index5_Char = "Index5:  ";
	//const char* newIndexSize_Char = "New Index Size:  ";
	//const char* indexSizeChar = " Size:";
	const char* degreeDivision_Char = " Degree or Division:  ";
	const char* radialOrAxial_Char = "Radial or Axial:  ";
	const char* helixType_Char = "Helix Type:  ";
	const char* amplitude_Char = "Amplitude:  ";
	const char* revolutions_Char = "Revolutions:  ";
	const char* waveDir_Char = "Wave Direction:  ";
	const char* waves_Char = "Waves:  ";
	const char* degrees_Char = "Degrees:  ";
	const char* segmentActual_Char = "Segment or Actual:  ";
	const char* segmentLength_Char = "Segment Length:  ";
	const char* patternCount_Char = "Pattern Count:  ";
	const char* pattern_Char = "Pattern:  ";
	const char* reverseDirection_Char = "Reverse Direction:  ";
	const char* fileOrFixed_Char = " File or Fixed:  ";
	//const char* lineNumber_Char = "Line Number:  ";
	//const char* pageIndex_t7_Char = "pageIndex.t7.txt=";
	//const char * pageIndex_bt3_pco_Char = "pageIndex.bt3.pco=59391";
	//const char* pageIndex_va0_Char = "pageIndex.va0.val=0";
	//const char* pageIndex_bt3_Char = "pageIndex.bt3.val=0";
	//const char* pageIndex_bt2_Char = "pageIndex.bt2.val=0";
	//const char* pageIndex_bt1_Char = "pageIndex.bt1.val=0";
	//const char* pageSync_b6_Char = "pageSync.b6.pco=0";
	//const char* pageSync_b5_Char = "pageSync.b5.pco=0";
	const char* rose_D_Char = "d: ";
	const char* rose_N_Char = "n: ";
	//const char * pageMain_va2_Char = "pageMain.va2.val=0";
	//const char* activeAxis_Char = "Active Axis:  ";

	//const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	//const char* nextionEnd = "\xFF\xFF\xFF";

	if (Serial1.available() > 0)
	{
		serialId = 1;
#ifdef DEBUG
		Serial.print("1.serialId: ");
#endif // Debug
	}
	else if (Serial2.available() > 0)
	{
		serialId = 2;
#ifdef DEBUG
		Serial.print("2.serialId: ");
#endif // Debug
	}
	else if (Serial3.available() > 0)
	{
		serialId = 3;
#ifdef DEBUG
		Serial.print("3.serialId: ");
#endif // Debug
	}
//	else if (Serial4.available() > 0)
//	{
//		serialId = 4;
//#ifdef DEBUG
//		Serial.print("3.serialId: ");
//#endif // Debug
//	}
	// All Nextion incoming data packets are terminated with one 0xFF byte
	if ((Serial1.available() > 0) || (Serial2.available() > 0) || (Serial3.available() > 0))// || (Serial4.available() > 0))
	{
		incomingByte = SerialRead(serialId);

#ifdef DEBUG
		Serial.print("serialId: ");
		Serial.println(serialId);
		//		Serial.print("incomingByte: ");
		//		Serial.println(incomingByte);
		//		Serial.println("MilliDelay");
#endif // DEBUG

		// Allow processing time for serial data
		MilliDelay(15);


		switch (incomingByte)
		{
			//case 33 through 42: Not Used.
			//{
			//	break;
			//}
		case 41: // ) - Not Used
		{

			break;
		}
		case 42: // * - 
		{

			break;
		}
		case 43: // + - Greek Key: Pattern radial or axial
		{
			configGreekKey.radialOrAxial_Pattern = GetSerialInteger();
			EEPROM.put(eePromAddress_Grk, configGreekKey);
			break;
		}
		case 44: // , - Greek Key: File radial or axial
		{
			configGreekKey.radialOrAxial_File = GetSerialInteger();
			EEPROM.put(eePromAddress_Grk, configGreekKey);
			break;
		}
		case 45: // - - Return Spindle and B axis to start positions
		{
			//pageCallerId = GetSerialInteger();
			if (pageCallerId == PAGE_MOVE)
			{
				ReturnToStartPosition_MovePage(configMove.axisId);
			}
			else
			{
				ReturnToStartPosition(ID_AXIS_3);
			}
			break;
		}
		case 46: // . - Greek Key: Pattern count(pattern repeats)
		{
			configGreekKey.patternCount_Pattern = GetSerialInteger();
			EEPROM.put(eePromAddress_Grk, configGreekKey);
			break;
		}
		//case 47 - 57: Don't Use
			//{
			//	break;
			//}
		case 58: // : Colon - Spindle MaxSpeed
		{
			int maxSpeed_Spindle = (int)GetSerialFloat(serialId);
#ifdef DEBUG
			const char* pageCallerId_Char = "pageCallerId:";
			Serial.print(pageCallerId_Char);
			Serial.println(pageCallerId);
#endif // DEBUG
			switch (pageCallerId)
			{
				case PAGE_MAIN:
				{

					configMain.maxSpd_Spindle = maxSpeed_Spindle;
					EEPROM.put(eePromAddress_Main, configMain);
	#ifdef DEBUG
					Serial.print(maxSpd_Char);
					Serial.println(configMain.maxSpd_Spindle);
	#endif // DEBUG
					break;
				}
				case PAGE_MULTI:
				case PAGE_ONE:
				{
					configMulti.maxSpd_Spindle = maxSpeed_Spindle;
					EEPROM.put(eePromAddress_Multi, configMulti);

					break;
				}
				case PAGE_INDEX:
				{
					configIndex_Prime.maxSpd_Spindle = maxSpeed_Spindle;
					EEPROM.put(eePromAddress_Ind_Prime, configIndex_Prime);
	#ifdef DEBUG
					Serial.print(maxSpd_Char);
					Serial.println(configIndex_Prime.maxSpd_Spindle);
	#endif // DEBUG
					break;
				}
				case PAGE_SYNC:
				case PAGE_REC:
				case PAGE_GRK:
				case PAGE_PROGRAM:
				{
					configSync.maxSpd_Spindle = maxSpeed_Spindle;
					configRec.maxSpd_Spindle = maxSpeed_Spindle;
					configGreekKey.maxSpd_Spindle = maxSpeed_Spindle;

					EEPROM.put(eePromAddress_Sync, configSync);
					EEPROM.put(eePromAddress_Rec, configRec);
					EEPROM.put(eePromAddress_Grk, configGreekKey);
	#ifdef DEBUG
					Serial.print(maxSpd_Char);
					Serial.println(configSync.maxSpd_Spindle);
	#endif // DEBUG
					break;
				}
				case PAGE_ROSE:
				{
					configRose.maxSpd_Spindle = maxSpeed_Spindle;
					EEPROM.put(eePromAddress_Rose, configRose);
	#ifdef DEBUG
					Serial.print(maxSpd_Char);
					Serial.println(configRose.maxSpd_Spindle);
	#endif // DEBUG
					break;
				}
			}
			break;
		}
		case 59: // ; Semicolon - Spindle Acceleration
		{
			int spindleAccel = (int)GetSerialFloat(serialId);
			switch (pageCallerId)
			{
				case PAGE_MAIN:
				{
					configMain.accel_Spindle = spindleAccel;
					EEPROM.put(eePromAddress_Main, configMain);
	#ifdef DEBUG
					Serial.print(accel_Char);
					Serial.println(configMain.accel_Spindle);
	#endif // DEBUG
					break;
				}

				case PAGE_MULTI:
				case PAGE_ONE:
				{

					configMulti.accel_Spindle = spindleAccel;
	#ifdef DEBUG
					Serial.print(accel_Char);
					Serial.println(configMulti.accel_Spindle);
	#endif // DEBUG
					EEPROM.put(eePromAddress_Multi, configMulti);
					break;
				}
				case PAGE_INDEX:
				{
					configIndex_Prime.accel_Spindle = spindleAccel;
					EEPROM.put(eePromAddress_Ind_Prime, configIndex_Prime);
	#ifdef DEBUG
					Serial.print(accel_Char);
					Serial.println(configIndex_Prime.accel_Spindle);
	#endif // DEBUG
					break;
				}
				case PAGE_SYNC:
				case PAGE_REC:
				case PAGE_GRK:
				case PAGE_PROGRAM:
				{
					configSync.accel_Spindle = spindleAccel;
					configRec.accel_Spindle = spindleAccel;
					configGreekKey.accel_Spindle = spindleAccel;

					EEPROM.put(eePromAddress_Sync, configSync);
					EEPROM.put(eePromAddress_Rec, configRec);
					EEPROM.put(eePromAddress_Grk, configGreekKey);
	#ifdef DEBUG
					Serial.print(accel_Char);
					Serial.println(configSync.accel_Spindle);
	#endif // DEBUG
					break;
				}
				case PAGE_ROSE:
				{
					configRose.accel_Spindle = spindleAccel;
					EEPROM.put(eePromAddress_Rose, configRose);
	#ifdef DEBUG
					Serial.print(accel_Char);
					Serial.println(configRose.accel_Spindle);
	#endif // DEBUG
					break;
				}
			} // end switch (pageCallerId)

			break;
		}
		case 60: // < Less than - Not Used
		{

			break;
		}
		case 61: // = Do Greek Key from file
		{

			reverseDirection = GetSerialInteger();
			if (reverseDirection == 0)
			{
				reverseDirection = DIR_CCW; // Nextion can't send negative number
			}
			else
			{
				reverseDirection = DIR_CW;
			}

			Serial.print("++++++++++++++++reverseDirection: ");
			Serial.println(reverseDirection);
			Program_FromFile(reverseDirection);

			break;
		}
		case 62: // > Greater than - Not Used
		{

			break;
		}
		case 63: // ? Question mark - Not Used
		{

			break;
		}
		case 64: // @ - Restart microSD port
		{
			Serial.println("Restart microSD port");
			lastFileIndex = -1;
			currentFileIndex = 0;
			BeginSD();
			break;
		}
		case 65: // A - Load Settings.ini
		{
			iniFileType = INI_4AXES;
			LoadSettings();
			break;
		}
		case 66: //B - AxisId
		{
			int axisId = GetSerialInteger();

			Serial.print("                   1234             pageCallerId:  ");
			Serial.println(pageCallerId);
			Serial.print("1.axisId:  ");
			Serial.println(axisId);
			switch (pageCallerId)
			{
				case PAGE_MAIN:
				{
					configMain.axisId = axisId;
				
					EEPROM.put(eePromAddress_Main, configMain);
					uint32_t myVal = 15;
	#ifdef DEBUG
				
					Serial.print("              567                   configMain.axisId:  ");
					Serial.println(configMain.axisId);

					EEPROM.get(600, myVal);
					Serial.print("maxSpd_Spindle--600:  ");
					Serial.println(myVal);

					EEPROM.get(604, myVal);
					Serial.print("maxSpd_Axis_Z--604:  ");
					Serial.println(myVal);

					EEPROM.get(608, myVal);
					Serial.print("maxSpd_Axis_X--608:  ");
					Serial.println(myVal);

					EEPROM.get(612, myVal);
					Serial.print("maxSpd_Axis_M3--612:  ");
					Serial.println(myVal);

					EEPROM.get(616, myVal);
					Serial.print("maxSpd_Axis_M4--616:  ");
					Serial.println(myVal);


					EEPROM.get(672, myVal);
					Serial.print("myVal--AxisId:  ");
					Serial.println(myVal);
	#endif // DEBUG
					break;
				}
				case PAGE_MULTI:
				case PAGE_ONE:
				{
					Serial.print("2.axisId:  ");
					Serial.println(configMulti.axisId);
					configMulti.axisId = axisId;
					EEPROM.put(eePromAddress_Multi, configMulti);

					Serial.print("3.axisId:  ");
					Serial.println(configMulti.axisId);
					break;
				}
				case PAGE_INDEX:
				{
					configIndex_Prime.axisId = axisId;
	#ifdef DEBUG
					Serial.print("Index axisId:  ");
					Serial.println(configIndex_Prime.axisId);
	#endif // DEBUG
					EEPROM.put(eePromAddress_Ind_Prime, configIndex_Prime);
					break;
				}
				case PAGE_MOVE:
				{
					configMove.axisId = axisId;
					EEPROM.put(eePromAddress_Mov, configMove);
					break;
				}
				case PAGE_BE:
				{
					break;
				}
				case PAGE_SYNC:
				case PAGE_REC:
				case PAGE_GRK:
				case PAGE_PROGRAM:
				{
					configSync.axisId = axisId;
					configRec.axisId = axisId;
					configGreekKey.axisId = axisId;
					configMove.axisId = axisId;

					EEPROM.put(eePromAddress_Sync, configSync);
					EEPROM.put(eePromAddress_Rec, configRec);
					EEPROM.put(eePromAddress_Grk, configGreekKey);
					EEPROM.put(eePromAddress_Mov, configMove);
	#ifdef DEBUG
					Serial.print(axisId_Char);
					Serial.println(configSync.axisId);
	#endif // DEBUG
					break;
				}

				case PAGE_ROSE:
				{
					configRose.axisId = axisId;
					EEPROM.put(eePromAddress_Rose, configRose);
					break;
				}


			}

#ifdef DEBUG
			Serial.print(axisId_Char);
			Serial.println(axisId);
#endif // DEBUG

			break;
		}
		case 67: // C - Used to rotate spindle in Greek Key 
		{
			break;
		}
		case 68: // D - Rec Style: Triangle, Square, Sawtooth
		{
			configRec.style = GetSerialInteger();
			EEPROM.put(eePromAddress_Rec, configRec);
			break;
		}
		case 69: //E - Index by divisions or degrees
		{
			switch (configIndex_Prime.indexId)
			{
				case INDEX_1:
				{
					// Set degreeOrDivision
					configIndex_1.degreeOrDivision = GetSerialIntegerOnly();
	#ifdef DEBUG
					Serial.print(index1_Char);
					Serial.print(degreeDivision_Char);
					Serial.println(configIndex_1.degreeOrDivision);
	#endif // DEBUG
					EEPROM.put(eePromAddress_Ind_1, configIndex_1);
					break;
				}
				case INDEX_2:
				{
					// Set degreeOrDivision
					configIndex_2.degreeOrDivision = GetSerialIntegerOnly();
	#ifdef DEBUG
					Serial.print(index2_Char);
					Serial.print(degreeDivision_Char);
					Serial.println(configIndex_2.degreeOrDivision);
	#endif // DEBUG
					EEPROM.put(eePromAddress_Ind_2, configIndex_2);
					break;
				}
				case INDEX_3:
				{
					// Set degreeOrDivision
					configIndex_3.degreeOrDivision = GetSerialIntegerOnly();
	#ifdef DEBUG
					Serial.print(index3_Char);
					Serial.print(degreeDivision_Char);
					Serial.println(configIndex_3.degreeOrDivision);
	#endif // DEBUG
					EEPROM.put(eePromAddress_Ind_3, configIndex_3);
					break;
				}
				case INDEX_4:
				{
					// Set degreeOrDivision
					configIndex_4.degreeOrDivision = GetSerialIntegerOnly();
	#ifdef DEBUG
					Serial.print(index4_Char);
					Serial.print(degreeDivision_Char);
					Serial.println(configIndex_4.degreeOrDivision);
	#endif // DEBUG
					EEPROM.put(eePromAddress_Ind_4, configIndex_4);
					break;
				}
				case INDEX_5:
				{
					// Set degreeOrDivision
					configIndex_5.degreeOrDivision = GetSerialIntegerOnly();
	#ifdef DEBUG
					Serial.print(index5_Char);
					Serial.print(degreeDivision_Char);
					Serial.println(configIndex_5.degreeOrDivision);
	#endif // DEBUG
					EEPROM.put(eePromAddress_Ind_5, configIndex_5);
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
		case 71: // G - EStop
		{
			configSetup.eStop = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
			Serial.print("eStop:  ");
			Serial.println(configSetup.eStop);
#endif // DEBUG
			break;
		}
		case 72: // H - Multi: Return multiple motors
		{
			ReturnToStartPosition_Multi();
			break;
		}
		case 73: // I-SyncZ Helix Type
		{
			configSync.helixType = GetSerialInteger();
			EEPROM.put(eePromAddress_Sync, configSync);
#ifdef DEBUG
			Serial.print(helixType_Char);
			Serial.println(configSync.helixType);
#endif // DEBUG
			break;
		}
		case 74: // J - Sync:Z In
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
			//Sync(directionSpindle, directionAxis, ID_AXIS_Z);
			Sync(directionSpindle, directionAxis);
			break;
		}
		case 75: // K - Sync: Z Out 
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

			Sync(directionSpindle, directionAxis);
			break;
		}
		case 76: // L - Greek Key File: Segment or Actual
		{
			configGreekKey.segmentOrActual = GetSerialInteger();// (int)GetSerialFloat(serialId);
#ifdef DEBUG
			Serial.print(segmentActual_Char);
			Serial.println(configGreekKey.segmentOrActual);
#endif // DEBUG
			EEPROM.put(eePromAddress_Grk, configGreekKey);
			break;
		}
		case 77: // M - Greek Key File: Pattern Count
		{
			configGreekKey.patternCount_File = (int)GetSerialFloat(serialId);//  GetSerialInteger();
#ifdef DEBUG
			Serial.print(patternCount_Char);
			Serial.println(configGreekKey.patternCount_File);
#endif // DEBUG
			EEPROM.put(eePromAddress_Grk, configGreekKey);
			break;
		}
		case 78: // N - GetFileListFromSD
		{
			Serial.print("Call_78:   ");
			currentFileIndex = GetSerialInteger();
#ifdef DEBUG				
			Serial.print("GetFileListFromSD_78: ");
			Serial.println(currentFileIndex);
#endif // DEBUG
			GetFileListFromSD(currentFileIndex);

			break;
		}
		case 79: // O - Setup: Alt X Distance per revolution (Text)
		{
			configSetup.distancePerRev_AxisXAlt = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
			Serial.print(distance_Char);
			Serial.println(configSetup.distancePerRev_AxisXAlt,5);
#endif // DEBUG
			break;
		}
		case 80: // P - Setup: Keep steppers enabled
		{
			configSetup.keepSteppersEnabled = GetSerialInteger();
			Serial.print("configSetup.keepSteppersEnabled:");
			Serial.println(configSetup.keepSteppersEnabled);
			// Enable or disable the steppers
			if (configSetup.keepSteppersEnabled == 1)
			{
				// Enable all steppers
				SetEnable(ID_SPINDLE, true, true);
				SetEnable(ID_AXIS_Z, true, true);
				SetEnable(ID_AXIS_X, true, true);
				SetEnable(ID_AXIS_3, true, true);
				SetEnable(ID_AXIS_4, true, true);
			}
			else
			{
				// Disable all steppers
				SetEnable(ID_SPINDLE, false, true);
				SetEnable(ID_AXIS_Z, false, true);
				SetEnable(ID_AXIS_X, false, true);
				SetEnable(ID_AXIS_3, false, true);
				SetEnable(ID_AXIS_4, false, true);
			}
			EEPROM.put(eePromAddress_Setup, configSetup);
			break;
		}
		case 81: // Q - Index1 counter clockwise
		{

			switch (configIndex_Prime.indexId)
			{
				case INDEX_1:
				{
					if (configIndex_1.fileOrFixed == FILE_SD)
					{
						IndexFromFile(DIR_CCW);
					}
					else
					{
						//IndexSpindle(DIR_CCW);
						Index(DIR_CCW);
					}

					break;
				}
				case INDEX_2:
				{
					if (configIndex_2.fileOrFixed == FILE_SD)
					{
						IndexFromFile(DIR_CCW);
					}
					else
					{
						//IndexSpindle(DIR_CCW);
						Index(DIR_CCW);
					}

					break;
				}
				case INDEX_3:
				{
					if (configIndex_3.fileOrFixed == FILE_SD)
					{
						IndexFromFile(DIR_CCW);
					}
					else
					{
						//IndexSpindle(DIR_CCW);
						Index(DIR_CCW);
					}

					break;
				}
				case INDEX_4:
				{
					if (configIndex_4.fileOrFixed == FILE_SD)
					{
						IndexFromFile(DIR_CCW);
					}
					else
					{
						//IndexSpindle(DIR_CCW);
						Index(DIR_CCW);
					}

					break;
				}
				case INDEX_5:
				{
					if (configIndex_5.fileOrFixed == FILE_SD)
					{
						IndexFromFile(DIR_CCW);
					}
					else
					{
						//IndexSpindle(DIR_CCW);
						Index(DIR_CCW);
					}

					break;
				}
			}


			break;
		}
		case 82: // R - Index1 Clockwise
		{

			badFilename = false;
#ifdef DEBUG
			Serial.print(indexId_Char);
			Serial.println(configIndex_Prime.indexId);
			Serial.print(index1_Char);
			Serial.print(fileOrFixed_Char);
			Serial.println(configIndex_1.fileOrFixed);
#endif // DEBUG
			switch (configIndex_Prime.indexId)
			{
				case INDEX_1:
				{
					if (configIndex_1.fileOrFixed == FILE_SD)
					{
						IndexFromFile(DIR_CW);
					}
					else
					{
						//IndexSpindle(DIR_CW);
						Index(DIR_CW);
					}
					break;
				}
				case INDEX_2:
				{
					if (configIndex_2.fileOrFixed == FILE_SD)
					{
						IndexFromFile(DIR_CW);
					}
					else
					{
						//IndexSpindle(DIR_CW);
						Index(DIR_CW);
					}
					break;
				}
				case INDEX_3:
				{
					if (configIndex_3.fileOrFixed == FILE_SD)
					{
						IndexFromFile(DIR_CW);
					}
					else
					{
						//IndexSpindle(DIR_CW);
						Index(DIR_CW);
					}

					break;
				}
				case INDEX_4:
				{
					if (configIndex_4.fileOrFixed == FILE_SD)
					{
						IndexFromFile(DIR_CW);
					}
					else
					{
						//IndexSpindle(DIR_CW);
						Index(DIR_CW);
					}

					break;
				}
				case INDEX_5:
				{
					if (configIndex_5.fileOrFixed == FILE_SD)
					{
						IndexFromFile(DIR_CW);
					}
					else
					{
						//IndexSpindle(DIR_CW);
						Index(DIR_CW);
					}

					break;
				}
			}

			break;
		}
		case 83: // S - Stop Steppers - Implemented in individual methods
		{

			break;
		}
		case 84: // T - Rose: n
		{
			configRose.n = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rose, configRose);
#ifdef DEBUG
			Serial.print(rose_N_Char);
			Serial.println(configRose.n);
#endif // DEBUG
			break;
		}
		case 85: //U - Greek Key File: Axis Segment Length
		{
			configGreekKey.segmentLength_File = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Grk, configGreekKey);
#ifdef DEBUG
			Serial.print(segmentLength_Char);
			Serial.println(configGreekKey.segmentLength_File);
#endif // DEBUG
			break;
		}
		case 86: // V - Stop spindle when limit switch triggered
		{
			configSetup.limit_StopSpindle = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configSetup);
			break;
		}
		case 87: // W - Shared: Radial or Axial
		{
			int radialOrAxial = GetSerialInteger();
			configRec.radial_axial = radialOrAxial;
			configGreekKey.radialOrAxial_File = radialOrAxial;
			configGreekKey.radialOrAxial_Pattern = radialOrAxial;
			EEPROM.put(eePromAddress_Rec, configRec);
			EEPROM.put(eePromAddress_Grk, configGreekKey);
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
			Serial.print(rose_D_Char);
			Serial.println(configRose.d);
#endif // DEBUG
			break;
		}
		case 90:  // Z - Main: Axis Clockwise
		{
			Main_TwoSteppers(DIR_CW, DIR_CW, configMain.axisId);

			break;
		}
		case 91:  // [ - Main: Axis Counterclockwise
		{
			Main_TwoSteppers(DIR_CCW, DIR_CCW, configMain.axisId);
			break;
		}
		case 92: // \ - *** Don't Use***
		{
			break;
		}
		case 93: // ] - Not used
		{

			break;
		}
		case 94: // ^ - Enable stepper
		{
			int axisId = GetSerialInteger();
#ifdef DEBUG
			Serial.print(axisId_Char);
			Serial.println(axisId);
#endif // DEBUG
			switch (axisId)
			{
				case ID_AXIS_Z:
				{
					SetEnable(ID_AXIS_Z, true, true);
					break;
				}
				case ID_AXIS_X:
				{
					SetEnable(ID_AXIS_X, true, true);
					break;
				}
				case ID_AXIS_3:
				{
					SetEnable(ID_AXIS_3, true, true);
					break;
				}
				case ID_AXIS_4:
				{
					SetEnable(ID_AXIS_4, true, true);
					break;
				}
				case ID_SPINDLE:
				{
					SetEnable(ID_SPINDLE, true, true);
					break;
				}
			}
			break;
		}
		case 95: //_ - Disable stepper
		{
			int axisId = GetSerialInteger();
			switch (axisId)
			{
			case ID_AXIS_Z:
			{
				SetEnable(ID_AXIS_Z, false, true);
				break;
			}
			case ID_AXIS_X:
			{
				SetEnable(ID_AXIS_X, false, true);
				break;
			}
			case ID_AXIS_3:
			{
				SetEnable(ID_AXIS_3, false, true);
				break;
			}
			case ID_AXIS_4:
			{
				SetEnable(ID_AXIS_4, false, true);
				break;
			}
			case ID_SPINDLE:
			{
				SetEnable(ID_SPINDLE, false, true);
				break;
			}
			}
			break;
		}
		case 96: // ' - *** Don't Use***
		{
			break;
		}
		case 97: // a - Rose: Spindle revolutions
		{
			configRose.spindleRevolutions = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rose, configRose);
#ifdef DEBUG
			Serial.print("Spindle Revolutions-a97: ");
			Serial.println(configRose.spindleRevolutions);
#endif // DEBUG
			break;
		}
		case 98: // b - Test EEPROM settings Setup screen
		{
			TestEEPROMSetup();
			break;
		}
		case 99: // c - Cancel/Stop Axis  
		{
			// Implemented in individual methods
			break;
		}
		case 100: // d - pageMulti Clockwise
		{
			Serial.print("case 100 CW axisId: ");
			Serial.println(configMulti.axisId);
			OneRunStepper(DIR_CW);

			break;
		}
		case 101: // e - pageMulti CounterClockwise
		{
			Serial.print("case 101 CCW axisId: ");
			Serial.println(configMulti.axisId);
			OneRunStepper(DIR_CCW);

			break;
		}
		case 102: // f - Not used
		{

			break;
		}
		case 103: // g - pageMulti: TargetDistance-Z
		{
			configMulti.targetDistance_Z = GetSerialFloat(serialId);
			break;
		}
		case 104: // h - pageMulti: TargetDistance-Z
		{
			configMulti.targetDistance_X = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Multi, configMulti);
			break;
		}
		case 105: // i - Rose pattern CCW
		{

#ifdef DEBUG
			Serial.print(axisId_Char);
			Serial.println(configRose.axisId);
#endif // DEBUG
			RoseRadial(DIR_CCW);
			break;
		}
		case 106: // j - pageMulti: TargetDistance-M3
		{
			configMulti.targetDistance_M3 = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Multi, configMulti);
			break;
		}
		case 107: // k - Main: Spindle Clockwise
		{
			Main_TwoSteppers(DIR_CW, DIR_CW, ID_SPINDLE);
			break;
		}
		case 108: // l - Main: Spindle counter clockwise
		{
			Main_TwoSteppers(DIR_CCW, DIR_CCW, ID_SPINDLE);
			break;
		}
		case 109: // m - Run Multi
		{
			Multi_SimultaneousTargets();
			break;
		}
		case 110: // n - Spindle Speed Percentage
		{
#ifdef DEBUG

			Serial.println("Enter case 110: >>>>>>>>>>>>>>>>>>>>>>>>>>");
			Serial.print("pageCallerId: ");
			Serial.println(pageCallerId);
#endif // DEBUG
			GetSpindleSpeed();

			break;
		}
		case 111: // o - Axis Speed Percentage
		{
#ifdef DEBUG

			Serial.println("Enter case 111: >>>>>>>>>>>>>>>>>>>>>>>>>>");
			Serial.print("pageCallerId: ");
			Serial.println(pageCallerId);
#endif // DEBUG
			GetAxisSpeed();

			break;
		}
		case 112: // p - Setup: Spindle Microsteps
		{
			configSetup.microsteps_Spindle = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configSetup);

			break;
		}
		case 113: // q - M4 axis microsteps
		{
			configSetup.microsteps_Axis_M4 = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configSetup);
			break;
		}
		case 114: // r - Setup: Spindle FullSteps
		{
			configSetup.steps360_Spindle = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
			Serial.print(steps360_Char);
			Serial.println(configSetup.steps360_Spindle);
#endif // DEBUG
			break;
		}
		case 115: // s - Setup: Spindle Gear Ratio
		{
			configSetup.gearRatio_Spindle = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configSetup);

			// Reset synchro_Ratio
			synchro_Ratio = configSetup.gearRatio_AxisM3 / configSetup.gearRatio_Spindle;
#ifdef DEBUG
			Serial.print(gearRatio_Char);
			Serial.println(configSetup.gearRatio_Spindle);
#endif // DEBUG
			break;
		}
		case 116: // t - Setup M3: Distance per revolution
		{
			configSetup.distancePerRev_AxisM3 = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
			Serial.print(distance_Char);
			Serial.println(configSetup.distancePerRev_AxisM3, 5);
#endif // DEBUG
			break;
		}
		case 117: // u - Setup: Polarity M4 (High or Low)
		{
			int polarityB = GetSerialInteger();

			// true (1): LOW  false (0): HIGH
			polarityB >= 1 ? (configSetup.polarity_Axis_M4 = true) : (configSetup.polarity_Axis_M4 = false);
			EEPROM.put(eePromAddress_Setup, configSetup);
			break;
		}
		case 118: // v - M3 axis microsteps
		{
			configSetup.microsteps_Axis_M3 = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configSetup);
			break;
		}
		case 119: // w - M3 axis Steps/360 
		{
			configSetup.steps360_Axis_M3 = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
			Serial.print(steps360_Char);
			Serial.println(configSetup.steps360_Axis_M3);
#endif // DEBUG
			break;
		}
		case 120: // x - M3 axis Gear Ratio
		{
			configSetup.gearRatio_AxisM3 = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configSetup);

			// Reset synchro_Ratio
			synchro_Ratio = configSetup.gearRatio_AxisM3 / configSetup.gearRatio_Spindle;
#ifdef DEBUG
			Serial.print(gearRatio_Char);
			Serial.println(configSetup.gearRatio_AxisM3);
#endif // DEBUG
			break;
		}

		case 121: // y - Z Axis Microsteps
		{
			configSetup.microsteps_Axis_Z = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configSetup);
			break;
		}
		case 122: // z - Z Axis FullSteps
		{
			configSetup.steps360_Axis_Z = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
			Serial.print(steps360_Char);
			Serial.println(configSetup.steps360_Axis_Z);
#endif // DEBUG
			break;
		}
		case 123: // { - Z Distance per revolution
		{
			configSetup.distancePerRev_AxisZ = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
			Serial.print(distance_Char);
			Serial.println(configSetup.distancePerRev_AxisZ, 5);
#endif // DEBUG
			break;
		}
		case 124: // | - Show all Teensy EEPROM values
		{
			TestAllTeensyEEPROMValues();
			break;
		}
		case 125: // } - Show Axis Id
		{

			Serial.print("configMain.axisId: ");
			Serial.println(configMain.axisId);
			Serial.print("configOne.axisId: ");
			Serial.println(configMulti.axisId);
			Serial.print("configIndex_Main.indexId: ");
			Serial.println(configIndex_Prime.indexId);
			Serial.print("configMove.axisId: ");
			Serial.println(configMove.axisId);
			Serial.print("configSync.axisId: ");
			Serial.println(configSync.axisId);
			Serial.print("configRec.axisId: ");
			Serial.println(configRec.axisId);
			Serial.print("configGreekKey.axisId: ");
			Serial.println(configGreekKey.axisId);
			Serial.print("configRose.axisId: ");
			Serial.println(configRose.axisId);

			break;
		}
		case 126: // ~ - Multi: target distance Spindle
		{
			configMulti.targetDistance_Spindle = GetSerialFloat(serialId);
#ifdef DEBUG
			Serial.print("targetDistance_Spindle:");
			Serial.println(configMulti.targetDistance_Spindle);
#endif // DEBUG
			EEPROM.put(eePromAddress_Multi, configMulti);
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
		case 161: // ¡ - pageLimits: NC or NO
		{
			configSetup.limit_NCorNO = GetSerialInteger();
#ifdef DEBUG
			Serial.print("configSetup.limit_NCorNO :");
			Serial.println(configSetup.limit_NCorNO);
#endif // DEBUG

			EEPROM.put(eePromAddress_Setup, configSetup);

			break;
		}
		case 162: // ¢ - Setup: M4: Radial or Lineal
		{
			configSetup.radialOrLinear_Axis_M4 = GetSerialInteger();
			// Radial = 0, Linear = 1

			EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
			Serial.print("radialOrLinear_Axis_M4: ");
			Serial.println(configSetup.radialOrLinear_Axis_M4);
#endif // DEBUG
			break;
		}
		case 163: // £ - TestEEPROM_Limits
		{
			TestEEPROM_Limits();
			break;
		}
		case 164: // ¤ - TestEEPROM_Returns
		{
			TestEEPROM_Returns();
			break;
		}
		case 165: // ¥ - Main: X Axis CCW
		{
			Main_TwoSteppers(DIR_CCW, DIR_CCW, ID_AXIS_X);
			break;
		}
		case 166: // ¦ - Main: X Axis CW
		{
			Main_TwoSteppers(DIR_CW, DIR_CW, ID_AXIS_X);
			break;
		}
		case 167: // § - Not used
		{
			break;
		}
		case 168: // ¨ - Setup M4: Distance per revolution
		{
			configSetup.distancePerRev_AxisM4 = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
			Serial.print(distance_Char);
			Serial.println(configSetup.distancePerRev_AxisM4, 5);
#endif // DEBUG
			break;
		}
		case 169: // © - M4 axis Gear Ratio
		{
			configSetup.gearRatio_AxisM4 = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
			Serial.print(gearRatio_Char);
			Serial.println(configSetup.gearRatio_AxisM4);
#endif // DEBUG
			break;
		}
		case 170: // ª - Setup: Cutter Motor Pin
		{
			configSetup.cutterMotorPin = GetSerialInteger();

			EEPROM.put(eePromAddress_Setup, configSetup);
			break;
		}
		case 171: // « - Main: M3 Axis CCW
		{
			Main_TwoSteppers(DIR_CCW, DIR_CCW, ID_AXIS_3);
			break;
		}
		case 172: // ¬ - Main: M3 Axis CW
		{
			Main_TwoSteppers(DIR_CW, DIR_CW, ID_AXIS_3);
			break;
		}
		case 173: // Don't use 
		{
			break;
		}
		case 174: // ® - Limit:  M4 Max
		{
			//configSetup.limit_Min_M4 = (int)GetSerialFloat(serialId);
			configSetup.limit_Max_M4 = GetSerialFloat(serialId);
			Serial.print(configSetup.limit_Max_M4);
			if (configSetup.limit_Max_M4 < 9)
			{
				configSetup.limit_Max_M4 = 56;
			}
			EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
			Serial.print(m4_LimitMax_Char);
			Serial.println(configSetup.limit_Max_Z);
#endif // DEBUG
			break;
		}
		case 175: // ¯ - Limit:  M4 Min
		{
			configSetup.limit_Min_M4 = GetSerialFloat(serialId);
			Serial.print(configSetup.limit_Min_M4);
			if (configSetup.limit_Min_M4 < 9)
			{
				configSetup.limit_Min_M4 = 55;
			}
			EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
			Serial.print(m4_LimitMin_Char);
			Serial.println(configSetup.limit_Min_Z);
#endif // DEBUG
			break;
		}
		case 176: // ° - All: M4 Radius
		{
			configSetup.radiusM4 = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
			Serial.print("radiusM4: ");
			Serial.println(configSetup.radiusM4);
#endif // DEBUG
			break;
		}
		case 177: // ± - Not Used
		{

			break;
		}
		case 178: // ² - Main: Synchro 
		{
			configMain.synchro_M3_Spindle = GetSerialInteger();
			EEPROM.put(eePromAddress_Main, configMain);
			break;
		}
		case 179: // ³ - Main: Synchro %
		{
			configMain.synchro_M3_Percentage = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Main, configMain);
			break;
		}
		case 180: // Don't use
		{
			break;
		}
		case 181: // µ - Move: Distance (Text)
		{
			float newDistance = GetSerialFloat(serialId);
#ifdef DEBUG
			Serial.print("newDistance: ");
			Serial.println(newDistance);
#endif // Debug
			switch (configMove.axisId)
			{
				case ID_MOVE_AXIS_Z1:
				{
					configMove.distance_MoveZ1 = newDistance;
					break;
				}
				case ID_MOVE_AXIS_Z2:
				{
					configMove.distance_MoveZ2 = newDistance;
	#ifdef DEBUG
					Serial.print("AxisId: ");
					Serial.println(configMove.axisId);
					Serial.print("distance_MoveZ2: ");
					Serial.println(configMove.distance_MoveZ2);
	#endif // Debug
					break;
				}
				case ID_MOVE_AXIS_X1:
				{
					configMove.distance_MoveX1 = newDistance;
					break;
				}
				case ID_MOVE_AXIS_X2:
				{
					configMove.distance_MoveX2 = newDistance;
	#ifdef DEBUG
					Serial.print("AxisId: ");
					Serial.println(configMove.axisId);
					Serial.print("distance_MoveX2: ");
					Serial.println(configMove.distance_MoveX2);
	#endif // Debug
					break;
				}
				case ID_MOVE_AXIS_M3_1:
				{
					configMove.distance_MoveM3_1 = newDistance;
					break;
				}
				case ID_MOVE_AXIS_M3_2:
				{
					configMove.distance_MoveM3_2 = newDistance;
					break;
				}
				case ID_MOVE_AXIS_M4_1:
				{
					configMove.distance_MoveM4_1 = newDistance;
					break;
				}
				case ID_MOVE_AXIS_M4_2:
				{
					configMove.distance_MoveM4_2 = newDistance;
					break;
				}
			}

			EEPROM.put(eePromAddress_Mov, configMove);
#ifdef DEBUG

			Serial.print("newDistance: ");
			Serial.println(newDistance);
#endif // DEBUG
			break;
		}
		case 182: // ¶ - Greek Key: Pattern Type  (2 through 7)
		{
			int pattern = GetSerialInteger();// 

			configGreekKey.patternId = pattern;
			EEPROM.put(eePromAddress_Grk, configGreekKey);

			break;
		}
		case 183: // · - Move:  CCW
		{
			MoveAxis(configMove.axisId, DIR_CCW);
			break;
		}
		case 184: // ¸ - Move: CW
		{
			MoveAxis(configMove.axisId, DIR_CW);
			break;
		}
		case 185: // ¹ - Axis Max speed
		{
			//pageCallerId = GetSerialIntegerOnly();
			Serial.print("case 185-pageCallerId: ");
			Serial.println(pageCallerId);

			int maxSpd_axis = GetSerialFloat(serialId);
			Serial.print("case 185-maxSpd_axis: ");
			Serial.println(maxSpd_axis);

			switch (pageCallerId)
			{
				case PAGE_MAIN:
				{
					switch (configMain.axisId)
					{
						case ID_AXIS_Z:
						{
							configMain.maxSpd_Axis_Z = maxSpd_axis;
							break;
						}
						case ID_AXIS_X:
						{
							configMain.maxSpd_Axis_X = maxSpd_axis;
							break;
						}
						case ID_AXIS_3:
						{
							configMain.maxSpd_Axis_M3 = maxSpd_axis;
							break;
						}
						case ID_AXIS_4:
						{
							configMain.maxSpd_Axis_M4 = maxSpd_axis;
							break;
						}
					}

					EEPROM.put(eePromAddress_Main, configMain);

					break;
				}
				case PAGE_MOVE:
				{
					switch (configMove.axisId)
					{
						case ID_MOVE_AXIS_Z1:
						case ID_MOVE_AXIS_Z2:
						{
							configMove.maxSpd_Axis_Z = maxSpd_axis;
							break;
						}
						case ID_MOVE_AXIS_X1:
						case ID_MOVE_AXIS_X2:
						{
							configMove.maxSpd_Axis_X = maxSpd_axis;
							break;
						}
						case ID_MOVE_AXIS_M3_1:
						case ID_MOVE_AXIS_M3_2:
						{
							configMove.maxSpd_Motor_3 = maxSpd_axis;
							break;
						}
						case ID_MOVE_AXIS_M4_1:
						case ID_MOVE_AXIS_M4_2:
						{
							configMove.maxSpd_Motor_4 = maxSpd_axis;
							break;
						}
					}

					EEPROM.put(eePromAddress_Mov, configMove);
					break;
				}
				case PAGE_INDEX:
				{

					switch (configIndex_Prime.axisId)
					{
						case ID_AXIS_3:
						{
							configIndex_Prime.maxSpd_Axis_M3 = maxSpd_axis;

							Serial.print("configIndex_Main.maxSpd_Axis_M3: ");
							Serial.println(configIndex_Prime.maxSpd_Axis_M3);
							break;
						}
						case ID_AXIS_4:
						{
							configIndex_Prime.maxSpd_Axis_M4 = maxSpd_axis;
							break;
						}
					}
					EEPROM.put(eePromAddress_Ind_Prime, configIndex_Prime);

					break;
				}
				case PAGE_MULTI:
				case PAGE_ONE:
				{
					Serial.print("pageMulti-configMulti.axisId: ");
					Serial.println(pageCallerId);
					switch (configMulti.axisId)
					{
						case ID_AXIS_Z:
						{
							configMulti.maxSpd_Axis_Z = maxSpd_axis;
		#ifdef DEBUG
							Serial.print(maxSpd_Char);
							Serial.println(configMulti.maxSpd_Axis_Z);
		#endif // DEBUG
							break;
						}
						case ID_AXIS_X:
						{
							configMulti.maxSpd_Axis_X = maxSpd_axis;
		#ifdef DEBUG
							Serial.print(maxSpd_Char);
							Serial.println(configMulti.maxSpd_Axis_X);
		#endif // DEBUG
							break;
						}
						case ID_AXIS_3:
						{
							configMulti.maxSpd_Axis_M3 = maxSpd_axis;
		#ifdef DEBUG
							Serial.print(maxSpd_Char);
							Serial.println(configMulti.maxSpd_Axis_M3);
		#endif // DEBUG
							break;
						}
						case ID_AXIS_4:
						{
							configMulti.maxSpd_Axis_M4 = maxSpd_axis;
	#ifdef DEBUG
							Serial.print(maxSpd_Char);
							Serial.println(configMulti.maxSpd_Axis_M4);
	#endif // DEBUG
							break;
						}

					} // end switch (configOne.axisId)

					EEPROM.put(eePromAddress_Multi, configMulti);

					break;
				}
				case PAGE_SYNC:
				case PAGE_REC:
				case PAGE_GRK:
				case PAGE_PROGRAM:
				{

					switch (configSync.axisId)
					{
					case ID_AXIS_Z:
					{
						configSync.maxSpd_Axis_Z = maxSpd_axis;
						configRec.maxSpd_Axis_Z = maxSpd_axis;
						configGreekKey.maxSpd_Axis_Z = maxSpd_axis;
						break;
					}
					case ID_AXIS_X:
					{
						configSync.maxSpd_Axis_X = maxSpd_axis;
						configRec.maxSpd_Axis_X = maxSpd_axis;
						configGreekKey.maxSpd_Axis_X = maxSpd_axis;
						Serial.print("configRec.maxSpd_Axis_X: ");
						Serial.print(configRec.maxSpd_Axis_X);
						break;
					}
					case ID_AXIS_3:
					{
						configSync.maxSpd_Axis_M3 = maxSpd_axis;
						configRec.maxSpd_Axis_M3 = maxSpd_axis;
						configGreekKey.maxSpd_Axis_M3 = maxSpd_axis;
						break;
					}
					case ID_AXIS_4:
					{
						configSync.maxSpd_Axis_M4 = maxSpd_axis;
						configRec.maxSpd_Axis_M4 = maxSpd_axis;
						configGreekKey.maxSpd_Axis_M4 = maxSpd_axis;
						break;
					}
					}
					EEPROM.put(eePromAddress_Sync, configSync);
					EEPROM.put(eePromAddress_Rec, configRec);
					EEPROM.put(eePromAddress_Grk, configGreekKey);

					configPageRec eePromRecPage;
					EEPROM.get(eePromAddress_Rec, eePromRecPage);
					Serial.print("eePromRecPage.maxSpd_Axis_X: ");
					Serial.println(eePromRecPage.maxSpd_Axis_X);

					break;
				}
				case PAGE_ROSE:
				{
					switch (configRose.axisId)
					{
						case ID_AXIS_Z:
						{
							configRose.maxSpd_Axis_Z = maxSpd_axis;
							break;
						}
						case ID_AXIS_X:
						{
							configRose.maxSpd_Axis_X = maxSpd_axis;
							break;
						}
						case ID_AXIS_3:
						{
							configRose.maxSpd_Axis_M3 = maxSpd_axis;
							break;
						}
						case ID_AXIS_4:
						{
							configRose.maxSpd_Axis_M4 = maxSpd_axis;
							break;
						}
					}

					EEPROM.put(eePromAddress_Rose, configRose);
					break;
				}
			}
			break;
		}
		case 186: // º - Axis Accel
		{
			//pageCallerId = GetSerialIntegerOnly();

			int accel_axis = GetSerialFloat(serialId);
			Serial.print("case 186-accel_axis: ");
			Serial.println(accel_axis);

			switch (pageCallerId)
			{
				case PAGE_MAIN:
				{
					switch (configMain.axisId)
					{
						case ID_AXIS_Z:
						{
							configMain.accel_Axis_Z = accel_axis;
							break;
						}
						case ID_AXIS_X:
						{
							configMain.accel_Axis_X = accel_axis;
							break;
						}
						case ID_AXIS_3:
						{
							configMain.accel_Axis_M3 = accel_axis;
		#ifdef DEBUG
							Serial.print("axisId:");
							Serial.println(configMain.axisId);
							Serial.print("....................accel_Axis_M3:");
							Serial.println(configMain.accel_Axis_M3);
		#endif // DEBUG
							break;
						}
						case ID_AXIS_4:
						{
							configMain.accel_Axis_M4 = accel_axis;
	#ifdef DEBUG
							Serial.print("axisId:");
							Serial.println(configMain.axisId);
							Serial.print("....................accel_Axis_M4:");
							Serial.println(configMain.accel_Axis_M4);
	#endif // DEBUG
							break;
						}
					}
					EEPROM.put(eePromAddress_Main, configMain);
					break;

				}

				case PAGE_MOVE:
				{
					switch (configMove.axisId)
					{
					case ID_MOVE_AXIS_Z1:
					case ID_MOVE_AXIS_Z2:
					{
						configMove.accel_Axis_Z = accel_axis;
						break;
					}
					case ID_MOVE_AXIS_X1:
					case ID_MOVE_AXIS_X2:
					{
						configMove.accel_Axis_X = accel_axis;
						break;
					}
					case ID_MOVE_AXIS_M3_1:
					case ID_MOVE_AXIS_M3_2:
					{
						configMove.accel_Motor_3 = accel_axis;
						break;
					}
					case ID_MOVE_AXIS_M4_1:
					case ID_MOVE_AXIS_M4_2:
					{
						configMove.accel_Motor_4 = accel_axis;
						break;
					}
					}

					EEPROM.put(eePromAddress_Mov, configMove);
					break;
				}
				case PAGE_INDEX:
				{
					switch (configIndex_Prime.axisId)
					{
					case ID_AXIS_3:
					{
						configIndex_Prime.accel_Axis_M3 = accel_axis;
						Serial.print("configIndex_Main.accel_Axis_M3: ");
						Serial.println(configIndex_Prime.accel_Axis_M3);
						break;
					}
					case ID_AXIS_4:
					{
						configIndex_Prime.accel_Axis_M4 = accel_axis;
						break;
					}
					}
					EEPROM.put(eePromAddress_Ind_Prime, configIndex_Prime);

					break;
				}
				case PAGE_MULTI:
				case PAGE_ONE:
				{
					switch (configMulti.axisId)
					{
						case ID_AXIS_Z:
						{
							configMulti.accel_Axis_Z = accel_axis;
							break;
						}
						case ID_AXIS_X:
						{
							configMulti.accel_Axis_X = accel_axis;
							break;
						}
						case ID_AXIS_3:
						{
							configMulti.accel_Axis_M3 = accel_axis;
		#ifdef DEBUG
							Serial.print(accel_Char);
							Serial.println(configMulti.accel_Axis_M3);
		#endif // DEBUG
							break;
						}
						case ID_AXIS_4:
						{
							configMulti.accel_Axis_M4 = accel_axis;
	#ifdef DEBUG
							Serial.print(accel_Char);
							Serial.println(configMulti.accel_Axis_M4);
	#endif // DEBUG
							break;
						}
					} // end switch (pageCallerId)

					EEPROM.put(eePromAddress_Multi, configMulti);
	#ifdef DEBUG
					Serial.print(accel_Char);
					Serial.println(configMulti.accel_Spindle);
	#endif // DEBUG
					break;

				}
				case PAGE_SYNC:
				case PAGE_REC:
				case PAGE_GRK:
				case PAGE_PROGRAM:
				{
					switch (configSync.axisId)
					{
					case ID_AXIS_Z:
					{
						configSync.accel_Axis_Z = accel_axis;
						configRec.accel_Axis_Z = accel_axis;
						configGreekKey.accel_Axis_Z = accel_axis;
						break;
					}
					case ID_AXIS_X:
					{
						configSync.accel_Axis_X = accel_axis;
						configRec.accel_Axis_X = accel_axis;
						configGreekKey.accel_Axis_X = accel_axis;
						break;
					}
					case ID_AXIS_3:
					{
						configSync.accel_Axis_M3 = accel_axis;
						configRec.accel_Axis_M3 = accel_axis;
						configGreekKey.accel_Axis_M3 = accel_axis;
						break;
					}
					case ID_AXIS_4:
					{
						configSync.accel_Axis_M4 = accel_axis;
						configRec.accel_Axis_M4 = accel_axis;
						configGreekKey.accel_Axis_M4 = accel_axis;
						break;
					}
					}
					EEPROM.put(eePromAddress_Sync, configSync);
					EEPROM.put(eePromAddress_Rec, configRec);
					EEPROM.put(eePromAddress_Grk, configGreekKey);
					break;

				}
				case PAGE_ROSE:
				{
					switch (configRose.axisId)
					{
						case ID_AXIS_Z:
						{
							configRose.accel_Axis_Z = accel_axis;
							break;
						}
						case ID_AXIS_X:
						{
							configRose.accel_Axis_X = accel_axis;
							break;
						}
						case ID_AXIS_3:
						{
							configRose.accel_Axis_M3 = accel_axis;
							break;
						}
						case ID_AXIS_4:
						{
							configRose.accel_Axis_M4 = accel_axis;
							break;
						}
					}

					EEPROM.put(eePromAddress_Rose, configRose);
					break;
				}
			}
			break;
		}
		case 187: // » - Setup: X microsteps
		{
			configSetup.microsteps_Axis_X = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configSetup);
			break;
		}
		case 188: // ¼ - Setup: X Full Steps
		{
			configSetup.steps360_Axis_X = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
			Serial.print(steps360_Char);
			Serial.println(configSetup.steps360_Axis_X);
#endif // DEBUG
			break;
		}
		case 189: // ½ - Setup: X Distance per revolution (Text)
		{
			configSetup.distancePerRev_AxisX = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
			Serial.print(distance_Char);
			Serial.println(configSetup.distancePerRev_AxisX, 5);
#endif // DEBUG
			break;
		}
		case 190: // ¾ - Filename-Index and Greek Key
		{
			GetFilenameFromSerial();
			lastFileIndex = -1;
			break;
		}
		case 191: // ¿ - Index: Size
		{
			float newIndexSize = GetSerialFloat(serialId);

			IndexSize(newIndexSize,configIndex_Prime.indexId);

#ifdef DEBUG
			Serial.print(indexId_Char);
			Serial.print(configIndex_Prime.indexId);
			Serial.print("   Size:   ");
			Serial.println(newIndexSize);

			Serial.print("Index AxisId: ");
			Serial.println(configIndex_Prime.axisId);
			Serial.print("configIndex_1.sizeInUnits:  ");
			Serial.println(configIndex_1.sizeInUnits);
			Serial.print("configIndex_2.sizeInUnits:  ");
			Serial.println(configIndex_2.sizeInUnits);
			Serial.print("configIndex_3.sizeInUnits:  ");
			Serial.println(configIndex_3.sizeInUnits);
			Serial.print("configIndex_4.sizeInUnits:  ");
			Serial.println(configIndex_4.sizeInUnits);
			Serial.print("configIndex_5.sizeInUnits:  ");
			Serial.println(configIndex_5.sizeInUnits);
#endif // DEBUG


			break;
		}
		case 192: // À - Sync: Revolutions
		{
			configSync.revolutions_Spindle = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Sync, configSync);
#ifdef DEBUG
			Serial.print(revolutions_Char);
			Serial.println(configSync.revolutions_Spindle);
#endif // DEBUG
			break;
		}
		case 193: // Á - Sync: Distance
		{
			configSync.distance = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Sync, configSync);
#ifdef DEBUG
			Serial.print(distance_Char);
			Serial.println(configSync.distance, 5);
#endif // DEBUG
			break;
		}
		case 194: // À - pageMulti: Direction
		{
			configMulti.direction = GetSerialInteger();
			EEPROM.put(eePromAddress_Multi, configMulti);
			Serial.print("configMulti.direction: ");
			Serial.println(configMulti.direction);
			break;
		}
		case 195: // Á - Setup: M3: Radial or Lineal
		{
			configSetup.radialOrLinear_Axis_M3 = GetSerialInteger();
			// Radial = 0, Linear = 1

			EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
			Serial.print("radialOrLinear_Axis_M4: ");
			Serial.println(configSetup.radialOrLinear_Axis_M3);
#endif // DEBUG
			break;
		}
		case 196: // Ä - Index:SourceFixed
		{
			int newSource = GetSerialInteger();
			switch (configIndex_Prime.indexId)
			{
				case INDEX_1:
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
					EEPROM.put(eePromAddress_Ind_1, configIndex_1);
	#ifdef DEBUG
					Serial.print(index1_Char);
					Serial.print(fileOrFixed_Char);
					Serial.println(configIndex_1.fileOrFixed);
	#endif // DEBUG
					break;
				}
				case INDEX_2:
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
					EEPROM.put(eePromAddress_Ind_2, configIndex_2);
	#ifdef DEBUG
					Serial.print(index2_Char);
					Serial.print(fileOrFixed_Char);
					Serial.println(configIndex_2.fileOrFixed);
	#endif // DEBUG
					break;
				}
				case INDEX_3:
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
					EEPROM.put(eePromAddress_Ind_3, configIndex_3);
	#ifdef DEBUG
					Serial.print(index3_Char);
					Serial.print(fileOrFixed_Char);
					Serial.println(configIndex_3.fileOrFixed);
	#endif // DEBUG
					break;
				}
				case INDEX_4:
				{
					configIndex_4.fileOrFixed = newSource;
					if (configIndex_4.fileOrFixed == 49)
					{
						configIndex_4.fileOrFixed = 1;
					}
					else if (configIndex_4.fileOrFixed == 48)
					{
						configIndex_4.fileOrFixed = 0;
					}
					EEPROM.put(eePromAddress_Ind_4, configIndex_4);
	#ifdef DEBUG
					Serial.print(index4_Char);
					Serial.print(fileOrFixed_Char);
					Serial.println(configIndex_4.fileOrFixed);
	#endif // DEBUG
					break;
				}
				case INDEX_5:
				{
					configIndex_5.fileOrFixed = newSource;
					if (configIndex_5.fileOrFixed == 49)
					{
						configIndex_5.fileOrFixed = 1;
					}
					else if (configIndex_5.fileOrFixed == 48)
					{
						configIndex_5.fileOrFixed = 0;
					}
					EEPROM.put(eePromAddress_Ind_5, configIndex_5);
	#ifdef DEBUG
					Serial.print(index5_Char);
					Serial.print(fileOrFixed_Char);
					Serial.println(configIndex_5.fileOrFixed);
	#endif // DEBUG
					break;
				}
			}

			break;
		}
		case 197: // Å - Not Used
		{

			break;
		}
		case 198: // Æ - Rec: Run  
		{
			int waveDir = GetSerialFloat(serialId);

			// In: -1  Out: 1  (Nextion can't send negative numbers)
			if (waveDir != WAVE_OUT)
			{
				waveDir = WAVE_IN;
			}

#ifdef DEBUG
			Serial.print(waveDir_Char);
			Serial.println(waveDir);
			Serial.print(axisId_Char);
			Serial.println(configRec.axisId);
			Serial.print(radialOrAxial_Char);
			Serial.println(configRec.radial_axial);
			Serial.print("Style:");
			Serial.println(configRec.style);
#endif // DEBUG
			switch (configRec.style)
			{
				case TRIANGLE_STYLE: // Triangle
				{
					Reciprocate_Triangle(waveDir);
					break;
				}
				case SAWTOOTH_STYLE: // Sawtooth
				{
					Reciprocate_Sawtooth(waveDir);
					break;
				}
				case SQUARE_STYLE: // Square
				{
					Reciprocate_Square(waveDir);
					break;
				}
			}

			break;
		}
		case 199: // Ç - pageMulti: TargetDistance-M4
		{
			configMulti.targetDistance_M4 = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Multi, configMulti);
			break;
		}
		case 200: // È - pageMulti: Multi Axes Checked
		{
			configMulti.multiAxesIDs = GetSerialInteger();
			Serial.print("configMulti.multiAxesIDs:  ");
			Serial.println(configMulti.multiAxesIDs);
			EEPROM.put(eePromAddress_Multi, configMulti);
			break;
		}
		case 201: // É - Rose: CW
		{
#ifdef DEBUG
			Serial.print(axisId_Char);
			Serial.println(configRose.axisId);
#endif // DEBUG
			RoseRadial(DIR_CW);
			//switch (configRose.axisId)
			//{
			//	case ID_AXIS_Z: // Z axis
			//	{
			//		RoseRadial_Z(DIR_CW);
			//		break;
			//	}
			//	case ID_AXIS_X: // X axis
			//	{
			//		RoseRadial_X(DIR_CW);
			//		break;
			//	}
			//	case ID_AXIS_B: // B axis
			//	{
			//		RoseRadial_B(DIR_CW);
			//		break;
			//	}
			//}

			break;
		}
		case 202: // Ê - Not Used
		{

			break;
		}
		case 203: // Ì - Rec: Axial Waves (Count)
		{
			configRec.waves_Axial = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rec, configRec);
#ifdef DEBUG
			Serial.print(waves_Char);
			Serial.println(configRec.waves_Axial);
#endif // DEBUG
			break;
		}
		case 204: // Ë - Rec: Axial Amplitude
		{
			configRec.amplitude_Axial_Spindle = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rec, configRec);
#ifdef DEBUG
			Serial.print(amplitude_Char);
			Serial.println(configRec.amplitude_Axial_Spindle);
#endif // DEBUG
			break;
		}
		case 205: // Í - Rec: Axial Distance
		{
			configRec.distance_Axial_Axis = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rec, configRec);
#ifdef DEBUG
			Serial.print(distance_Char);
			Serial.println(configRec.distance_Axial_Axis, 5);
#endif // DEBUG
			break;
		}
		case 206: // Î -  Return: Spindle and Z axis to start positions
		{
			pageCallerId = GetSerialInteger();

			switch (pageCallerId)
			{
				case PAGE_MAIN:
				{
					ReturnToStartPosition(configMain.axisId);
					break;
				}

				case PAGE_MOVE:
				{
					ReturnToStartPosition_MovePage(configMove.axisId);
					break;
				}
				case PAGE_SYNC:
				case PAGE_REC:
				case PAGE_GRK:
				case PAGE_PROGRAM:
				{
					ReturnToStartPosition(configSync.axisId);
					break;
				}
				case PAGE_ROSE:
				{
					ReturnToStartPosition(configRose.axisId);
					break;
				}

				case PAGE_MULTI:
				case PAGE_ONE:
				{
					ReturnToStartPosition(configMulti.axisId);
					break;
				}
				case PAGE_INDEX:
				{
					ReturnToStartPosition_Multi();
					break;
				}
			}

			break;
		}
		case 207: // Ï - Index: Synchro
		{
			configIndex_Prime.synchro_M3_Spindle = GetSerialInteger();
			EEPROM.put(eePromAddress_Ind_Prime, configIndex_Prime);
			break;
		}
		case 208: // Ð - Index: Synchro Percentage
		{
			configIndex_Prime.synchro_M3_Percentage = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Ind_Prime, configIndex_Prime);
			break;
		}
		case 209: // Ñ - Main: M4 Axis CCW
		{
			Main_TwoSteppers(DIR_CCW, DIR_CCW, ID_AXIS_4);
			break;
		}
		case 210: // Ò - Main: M4 Axis CW
		{
			Main_TwoSteppers(DIR_CW, DIR_CW, ID_AXIS_4);
			break;
		}
		case 211: // Ó - Not Used
		{

			break;
		}
		case 212: // Ô - Rec: radial Waves (Count)
		{
			configRec.waves_Radial = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rec, configRec);
#ifdef DEBUG
			Serial.print(waves_Char);
			Serial.println(configRec.waves_Radial);
#endif // DEBUG
			break;
		}
		case 213: // Õ - Rec: radial Degrees
		{
			configRec.degrees_Radial_Spindle = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rec, configRec);
#ifdef DEBUG
			Serial.print(degrees_Char);
			Serial.println(configRec.degrees_Radial_Spindle);
#endif // DEBUG
			break;
		}
		case 214: // Ö - Rec: radial Amplitude Axis
		{
			configRec.amplitude_Radial_Axis = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rec, configRec);
#ifdef DEBUG
			Serial.print(amplitude_Char);
			Serial.println(configRec.amplitude_Radial_Axis);
#endif // DEBUG
			break;
		}
		case 215: // × - XXXX Deprecate   Return: Spindle and X axis to start positions
		{
			pageCallerId = GetSerialInteger();

			if (pageCallerId == PAGE_MOVE)
			{
				ReturnToStartPosition_MovePage(configMove.axisId);
			}
			else
			{
				ReturnToStartPosition(ID_AXIS_X);
			}
			break;
		}
		case 216: // Ø - Setup:Polarity Spindle (High or Low)
		{
			int polaritySpindle = GetSerialInteger();

			// HIGH = 1, LOW = 0
			polaritySpindle >= 1 ? (configSetup.polarity_Spindle = true) : (configSetup.polarity_Spindle = false);
			EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
			Serial.print(polarity_Char);
			Serial.println(configSetup.polarity_Spindle);
#endif // DEBUG
			break;
		}
		case 217: // Ù - Setup: Polarity Z (High or Low)
		{
			int polarityZ = GetSerialInteger();

			// true (1): LOW  false (0): HIGH
			polarityZ >= 1 ? (configSetup.polarity_Axis_Z = true) : (configSetup.polarity_Axis_Z = false);
			EEPROM.put(eePromAddress_Setup, configSetup);
			break;
		}
		case 218: // Ú - Setup: Polarity X (High or Low)
		{
			int polarityX = GetSerialInteger();

			// true (1): LOW  false (0): HIGH
			polarityX >= 1 ? (configSetup.polarity_Axis_X = true) : (configSetup.polarity_Axis_X = false);
			EEPROM.put(eePromAddress_Setup, configSetup);
			break;
		}
		case 219: // Û - Setup: Polarity M3 (High or Low)
		{
			int polarityB = GetSerialInteger();

			// true (1): LOW  false (0): HIGH
			polarityB >= 1 ? (configSetup.polarity_Axis_M3 = true) : (configSetup.polarity_Axis_M3 = false);
			EEPROM.put(eePromAddress_Setup, configSetup);
			break;
		}
		case 220: // Ü - Limit: Z Min
		{
			//configSetup.limit_Min_Z = (int)GetSerialFloat(serialId);
			configSetup.limit_Min_Z = GetSerialFloat(serialId);
			Serial.print("Limit Z Min: ");
			Serial.println(configSetup.limit_Min_Z);
			if (configSetup.limit_Min_Z < 9)
			{
				configSetup.limit_Min_Z = 57;
			}
			EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
			Serial.print(z_LimitMin_Char);
			Serial.println(configSetup.limit_Min_Z);
#endif // DEBUG
			break;
		}
		case 221: // Ý - Limit:  Z Max
		{
			configSetup.limit_Max_Z = (int)GetSerialFloat(serialId);
			if (configSetup.limit_Max_Z < 9)
			{
				configSetup.limit_Max_Z = 57;
			}
			EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
			Serial.print(z_LimitMax_Char);
			Serial.println(configSetup.limit_Max_Z);
#endif // DEBUG
			break;
		}
		case 222: // Þ - Limit:  X Min
		{
			configSetup.limit_Min_X = (int)GetSerialFloat(serialId);
			if (configSetup.limit_Min_X < 9)
			{
				configSetup.limit_Min_X = 57;
			}
			EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
			Serial.print(x_LimitMin_Char);
			Serial.println(configSetup.limit_Min_X);
#endif // DEBUG
			break;
		}
		case 223: // ß - Limit:  X Max
		{
			configSetup.limit_Max_X = (int)GetSerialFloat(serialId);
			if (configSetup.limit_Max_X < 9)
			{
				configSetup.limit_Max_X = 57;
			}
			EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
			Serial.print(x_LimitMax_Char);
			Serial.println(configSetup.limit_Max_X);
#endif // DEBUG
			break;
		}
		case 224: // à - Limit:  M3 Min
		{
			configSetup.limit_Min_M3 = (int)GetSerialFloat(serialId);
			if (configSetup.limit_Min_M3 < 9)
			{
				configSetup.limit_Min_M3 = 57;
			}
			EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
			Serial.print(m3_LimitMin_Char);
			Serial.println(configSetup.limit_Min_M3);
#endif // DEBUG
			break;
		}
		case 225: // á - Limit:  M3 Max
		{
			configSetup.limit_Max_M3 = (int)GetSerialFloat(serialId);
			if (configSetup.limit_Max_M3 < 9)
			{
				configSetup.limit_Max_M3 = 57;
			}
			EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
			Serial.print(m3_LimitMax_Char);
			Serial.println(configSetup.limit_Max_M3);
#endif // DEBUG
			break;
		}
		case 226: // â - Not Used
		{
			break;
		}
		case 227: // ã - Rose: Axial Amplitude
		{
			switch (configRose.axisId)
			{
				case ID_AXIS_Z:
				{
					configRose.amplitude_Axial_Z = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Rose, configRose);
	#ifdef DEBUG
					Serial.print(amplitude_Char);
					Serial.println(configRose.amplitude_Axial_Z);
	#endif // DEBUG
					break;
				}
				case ID_AXIS_X:
				{
					configRose.amplitude_Axial_X = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Rose, configRose);
	#ifdef DEBUG
					Serial.print(amplitude_Char);
					Serial.println(configRose.amplitude_Axial_X);
	#endif // DEBUG
					break;
				}
				case ID_AXIS_3:
				{
					configRose.amplitude_Axial_M3 = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Rose, configRose);
	#ifdef DEBUG
					Serial.print(amplitude_Char);
					Serial.println(configRose.amplitude_Axial_M3);
	#endif // DEBUG
					break;
				}
				case ID_AXIS_4:
				{
					configRose.amplitude_Axial_M4 = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Rose, configRose);
#ifdef DEBUG
					Serial.print(amplitude_Char);
					Serial.println(configRose.amplitude_Axial_M4);
#endif // DEBUG
					break;
				}
			}

			break;
		}
		case 228: // ä - Rose: Radial Amplitude
		{
			switch (configRose.axisId)
			{
				case ID_AXIS_Z:
				{
					configRose.amplitude_Radial_Z = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Rose, configRose);
	#ifdef DEBUG
					Serial.print(amplitude_Char);
					Serial.println(configRose.amplitude_Radial_Z);
	#endif // DEBUG
					break;
				}
				case ID_AXIS_X:
				{
					configRose.amplitude_Radial_X = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Rose, configRose);
	#ifdef DEBUG
					Serial.print(amplitude_Char);
					Serial.println(configRose.amplitude_Radial_X);
	#endif // DEBUG
					break;
				}
				case ID_AXIS_3:
				{
					configRose.amplitude_Radial_M3 = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Rose, configRose);
	#ifdef DEBUG
					Serial.print(amplitude_Char);
					Serial.println(configRose.amplitude_Radial_M3);
	#endif // DEBUG
					break;
				}
				case ID_AXIS_4:
				{
					configRose.amplitude_Radial_M4 = GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Rose, configRose);
#ifdef DEBUG
					Serial.print(amplitude_Char);
					Serial.println(configRose.amplitude_Radial_M4);
#endif // DEBUG
					break;
				}
			}
			break;
		}
		case 229: // å - Not Used
		{

			break;
		}
		case 230: // å - Greek Key File: Pattern Count/360
		{
			configGreekKey.countPatternPer360_File = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Grk, configGreekKey);
#ifdef DEBUG
			Serial.print(patternCount_Char);
			Serial.println(configGreekKey.countPatternPer360_File);
#endif // DEBUG
			break;
		}
		case 231: // ç - Rose: Return Axis and Spindle
		{
			int axisId = GetSerialInteger();
			pageCallerId = PAGE_ROSE;
			ReturnToStartPosition(axisId);
			break;
		}
		case 232: // è - Return: Spindle MaxSpd
		{
			configSetup.maxSpd_Return_Spindle = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
			Serial.print(maxSpd_Char);
			Serial.println(configSetup.maxSpd_Return_Spindle);
#endif // DEBUG
			break;
		}
		case 233: // é - Return: Spindle Accel
		{
			configSetup.accel_Return_Spindle = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
			Serial.print(accel_Char);
			Serial.println(configSetup.accel_Return_Spindle);
#endif // DEBUG
			break;
		}
		case 234: // ê - Return: Axis MaxSpd
		{

			int returnAxisId = GetSerialIntegerOnly();

			int newValue = (int)GetSerialFloat(serialId);
#ifdef DEBUG
			Serial.print("returnAxisId: ");
			Serial.println(returnAxisId);
			Serial.print("Axis MaxSpd: ");
			Serial.println(newValue);
#endif // DEBUG
			switch (returnAxisId)
			{
				case ID_AXIS_Z:
				{
					configSetup.maxSpd_Return_Axis_Z = newValue;
					break;
				}
				case ID_AXIS_X:
				{
					configSetup.maxSpd_Return_Axis_X = newValue;
					break;
				}
				case ID_AXIS_3:
				{
					configSetup.maxSpd_Return_Axis_M3 = newValue;
					break;
				}
				case ID_AXIS_4:
				{
					configSetup.maxSpd_Return_Axis_M4 = newValue;
					break;
				}
			}

			EEPROM.put(eePromAddress_Setup, configSetup);

			break;
		}
		case 235: // ë - Return: Axis Accel
		{
			int returnAxisId = GetSerialIntegerOnly();
			switch (returnAxisId)
			{
				case ID_AXIS_Z:
				{
					configSetup.accel_Return_Axis_Z = (int)GetSerialFloat(serialId);
					break;
				}
				case ID_AXIS_X:
				{
					configSetup.accel_Return_Axis_X = (int)GetSerialFloat(serialId);
					break;
				}
				case ID_AXIS_3:
				{
					configSetup.accel_Return_Axis_M3 = (int)GetSerialFloat(serialId);
					break;
				}
				case ID_AXIS_4:
				{
					configSetup.accel_Return_Axis_M4 = (int)GetSerialFloat(serialId);
					break;
				}
			}

			EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
			Serial.print(accel_Char);
			Serial.println(configSetup.accel_Return_Axis_Z);
#endif // DEBUG
			break;
		}
		case 236: //ì- Clear Stepper positions and set pageCallerId
		{
			returnSteps_Spindle = 0;
			endPosition_Axis = 0;
			endPosition_Spindle = 0;

			pageCallerId = GetSerialInteger();

			Serial.print("Index AxisId: ");
			Serial.println(configIndex_Prime.axisId);

	#ifdef DEBUG
			switch (pageCallerId)
			{
				case PAGE_MAIN:
				{

					Serial.print("Main AxisId: ");
					Serial.println(configMain.axisId);

					break;
				}
				case PAGE_ONE:
				case PAGE_MULTI:
				{

					Serial.print("One/Multi AxisId: ");
					Serial.println(configMulti.axisId);

					break;
				}
				case PAGE_INDEX:
				{

					Serial.print("Index AxisId: ");
					Serial.println(configIndex_Prime.axisId);
					Serial.print("configIndex_1.sizeInUnits:  ");
					Serial.println(configIndex_1.sizeInUnits);
					Serial.print("configIndex_2.sizeInUnits:  ");
					Serial.println(configIndex_2.sizeInUnits);
					Serial.print("configIndex_3.sizeInUnits:  ");
					Serial.println(configIndex_3.sizeInUnits);
					Serial.print("configIndex_4.sizeInUnits:  ");
					Serial.println(configIndex_4.sizeInUnits);
					Serial.print("configIndex_5.sizeInUnits:  ");
					Serial.println(configIndex_5.sizeInUnits);
					break;
				}
				case PAGE_MOVE:
				{

					Serial.print("Move AxisId: ");
					Serial.println(configMove.axisId);

					break;
				}
				case PAGE_SYNC:
				case PAGE_REC:
				case PAGE_GRK:
				case PAGE_PROGRAM:
				{

					Serial.print("Shared AxisId: ");
					Serial.println(configSync.axisId);

					break;
				}
				default:
				{
					Serial.print("PageId: ");
					Serial.println(pageCallerId);
				}
			}


			Serial.print(pageCallerId_Char);
			Serial.println(pageCallerId);
#endif // DEBUG
			break;
		}
		case 237: // í - Track Positions
		{
			// Not implemented
//			configGeneral.trackPositions = GetSerialInteger();
//			EEPROM.put(eePromAddress_General, configGeneral);
//#ifdef DEBUG
//			Serial.print("trackPositions:");
//			Serial.println(configGeneral.trackPositions);
//#endif // DEBUG
			break;
		}
		case 238: // î - Not Used
		{

			break;
		}
		case 239: // ï - Setup: Alt X microsteps
		{
			configSetup.microsteps_Axis_XAlt = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configSetup);
			break;
		}
		case 240: // ð - Setup: Alt X Full Steps
		{
			configSetup.steps360_Axis_XAlt = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
			Serial.print(steps360_Char);
			Serial.println(configSetup.steps360_Axis_XAlt);
#endif // DEBUG
			break;
		}
		case 241: // ñ - Not Used
		{
			break;
		}
		case 242: // ò - Polarity (Enable) Alt X axis
		{
			int polarityAltX = GetSerialInteger();

			// true (1): LOW  false (0): HIGH
			polarityAltX >= 1 ? (configSetup.polarity_Axis_XAlt = true) : (configSetup.polarity_Axis_XAlt = false);
			EEPROM.put(eePromAddress_Setup, configSetup);
			break;
		}
		case 243: // ó - X or Alt X axis
		{
			configSetup.xAltX = GetSerialInteger();
			EEPROM.put(eePromAddress_Setup, configSetup);
			break;
		}
		case 244: // ô - Not used
		{

			break;
		}
		case 245: // õ - Not Used
		{

			break;
		}
		case 246: // ö - Index:Id 
		{
			configIndex_Prime.indexId = GetSerialInteger();
#ifdef DEBUG
			Serial.print(indexId_Char);
			Serial.println(configIndex_Prime.indexId);
#endif // DEBUG
			EEPROM.put(eePromAddress_Ind_Prime, configIndex_Prime);
			break;
		}
		case 247: // ÷ - Do Greek Key (Left and Right) 
		{
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
			Serial.print(axisId_Char);
			Serial.println(configGreekKey.axisId);
			Serial.print(pattern_Char);
			Serial.println(configGreekKey.patternId);
			Serial.print(reverseDirection_Char);
			Serial.println(reverseDirection);
#endif // DEBUG
			switch (configGreekKey.patternId)
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

			// 0 = Change to disabled, 1 = keep enabled
			if (configSetup.keepSteppersEnabled == 1)
			{
				SerialPrint("pageGrk.bt10.val=1"); // 0 = enabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageGrk.bt11.val=1"); // 0 = enabled
				SerialPrint("\xFF\xFF\xFF");
			}
			else
			{
				SerialPrint("pageGrk.bt10.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageGrk.bt10.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");

				SerialPrint("pageGrk.bt11.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageGrk.bt11.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
			}

			break;
		}
		case 248: // õ - Not Used
		{
			
			break;
		}
		case 249: // ù - Not Used
		{

			break;
		}
		case 250: // ù - Rose: Spindle revolutions
		{
			//float revs = GetSerialFloat(serialId);
			//configRose.spindleRevolutions = revs;
			configRose.spindleRevolutions = GetSerialFloat(serialId);

			EEPROM.put(eePromAddress_Rose, configRose);
#ifdef DEBUG
			Serial.print("Spindle Revolutions-250: ");
			Serial.println(configRose.spindleRevolutions);
			//Serial.println(revs);
#endif // DEBUG
			break;
		}
		case 251: // û - Greek Key Leg Segment Length
		{
			configGreekKey.segmentLength_Pattern = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Grk, configGreekKey);
#ifdef DEBUG
			Serial.print(segmentLength_Char);
			Serial.println(configGreekKey.segmentLength_Pattern);
#endif // DEBUG

			break;
		}
		case 252: // ü - Greek Key Count/360
		{
			configGreekKey.countPatternPer360_Pattern = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Grk, configGreekKey);
#ifdef DEBUG
			Serial.print(patternCount_Char);
			Serial.println(configGreekKey.countPatternPer360_Pattern);
#endif // DEBUG
			break;
		}
		case 253: // ü - Not Used
		{

			break;
		}
		case 254: // þ - All: M3 Radius
		{
			configSetup.radiusM3 = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
			Serial.print("radiusM3: ");
			Serial.println(configSetup.radiusM3);
#endif // DEBUG
			break;
		}
		//default:
		//{
		//	Serial.print("Input:");
		//	Serial.println("N/A");
		//}
			//Serial.println("1.End Switch");
		}


		//Serial.println("2.End Switch");
		MilliDelay(100);
		//Serial.println("3.End Switch");
	}

#ifdef DEBUG
	//Serial.println("2.End Loop.");
#endif // DEBUG
} // End loop()