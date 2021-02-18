#pragma once
/* *****************************************************************
* 4Rose main entry
* Author: Edward French
* Version: 19 - 021821
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
Stepper stepperAxis_B(PIN_AXIS_B_STEP, PIN_AXIS_B_DIR);

Stepper stepperAxis_MainX(PIN_AXIS_X_STEP, PIN_AXIS_X_DIR);

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
	const char* nextionEnd = "\xFF\xFF\xFF";
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
	Serial1.begin(115200); //Nextion Serial baud rate set in Nextion pageMain Preinitialize Event tab
	Serial1.print("bauds=115200");
	Serial1.print(nextionEnd);
	MilliDelay(50);
	Serial1.print("bauds=115200");
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
	Serial2.begin(115200); //Nextion Serial baud rate set in Nextion pageMain Preinitialize Event tab
	Serial2.print("bauds=115200");
	Serial2.print(nextionEnd);
	MilliDelay(50);
	Serial2.print("bauds=115200");
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
	Serial3.begin(115200); //Nextion Serial baud rate set in Nextion pageMain Preinitialize Event tab
	Serial3.print("bauds=115200");
	Serial3.print(nextionEnd);
	MilliDelay(50);
	Serial3.print("bauds=115200");
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

#ifdef DEBUG
	Serial.print("serialId: ");
	Serial.println(serialId);
#endif // Debug
	// Update with values from EEProm
	EEPROM.get(eePromAddress_Setup, configSetup);
	EEPROM.get(eePromAddress_Main, configMain);

	EEPROM.get(eePromAddress_Sync, configSync);

	EEPROM.get(eePromAddress_One, configOne);
	EEPROM.get(eePromAddress_Ind_Main, configIndex_Main);
	EEPROM.get(eePromAddress_Ind_1, configIndex_1);
	EEPROM.get(eePromAddress_Ind_2, configIndex_2);
	EEPROM.get(eePromAddress_Ind_3, configIndex_3);
	EEPROM.get(eePromAddress_Mov, configMove);
	EEPROM.get(eePromAddress_Rose, configRose);
	EEPROM.get(eePromAddress_Rec, configRec);
	EEPROM.get(eePromAddress_Grk, configGreekKey);

#ifdef DEBUG
	Serial.print("configMain.axisId: ");
	Serial.println(configMain.axisId);
	Serial.print("configOne.axisId: ");
	Serial.println(configOne.axisId);
	Serial.print("configIndex_Main.indexId: ");
	Serial.println(configIndex_Main.indexId);
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
	pinMode(PIN_AXIS_B_ENABLE, OUTPUT);

	SetEnable(ID_SPINDLE, false);
	SetEnable(ID_AXIS_Z, false);
	SetEnable(ID_AXIS_X, false);
	SetEnable(ID_AXIS_B, false);

	// Initialize Limit switches
	pinMode(configSetup.limit_Min_Z, INPUT_PULLUP);
	pinMode(configSetup.limit_Max_Z, INPUT_PULLUP);
	MilliDelay(10);
	digitalWrite(configSetup.limit_Min_Z, HIGH);  // Enable
	digitalWrite(configSetup.limit_Max_Z, HIGH);  // Enable

	pinMode(configSetup.limit_Min_X, INPUT_PULLUP);
	pinMode(configSetup.limit_Max_X, INPUT_PULLUP);

	MilliDelay(10);
	digitalWrite(configSetup.limit_Min_X, HIGH);  // Enable
	digitalWrite(configSetup.limit_Max_X, HIGH);  // Enable

	pinMode(configSetup.limit_Min_B, INPUT_PULLUP);
	pinMode(configSetup.limit_Max_B, INPUT_PULLUP);
	digitalWrite(configSetup.limit_Min_B, HIGH);  // Enable
	digitalWrite(configSetup.limit_Max_B, HIGH);  // Enable

	MilliDelay(10);

	// Enable SD card reader
	pinMode(PIN_SPI_CS_24, OUTPUT);
	digitalWrite(PIN_SPI_CS_24, HIGH);
	pinMode(PIN_SPI_CS_15, OUTPUT);
	digitalWrite(PIN_SPI_CS_15, HIGH);
	pinMode(PIN_SPI_CS_10, OUTPUT);
	digitalWrite(PIN_SPI_CS_10, HIGH);
	pinMode(PIN_SPI_CS_9, OUTPUT);
	digitalWrite(PIN_SPI_CS_9, HIGH);

	// Start SD
	BeginSD();

	SetEnable(ID_SPINDLE, false);

	for (int i = 0; i < 3; i++) // Verify Teensy is operational
	{
		digitalWrite(LED_BUILTIN, HIGH);
		MilliDelay(50);
		digitalWrite(LED_BUILTIN, LOW);
		MilliDelay(300);
	}

#ifdef DEBUG
	const char * initialized = "Initialized...";
	Serial.println(initialized);
#endif // DEBUG

	// Uncomment to get size of defined structs.
	//structSizeCalc();
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
	const char * pageCallerId_Char = "Page Caller ID:";
	const char * axis_SpeedPercent_Char = "Axis Speed Percent:";
	const char * spindle_SpeedPercent_Char = "Spindle Speed Percent:";
	const char * x_SpeedPercent_Char = "X axis Speed Percent:";
	const char * b_SpeedPercent_Char = "B axis Speed Percent:";
	const char * z_SpeedPercent_Char = "Z axis Speed Percent:";
	const char * steps360_Char = "Steps/360: ";
	const char * gearRatio_Char = "Gear Ratio: ";
	const char * maxSpd_Char = "MaxSpeed:";
	const char * accel_Char = "Accel:";
	const char * polarity_Char = "Polarity:";
	const char * distance_Char = "Distance:";
	//const char * nextSpeed_Char = "Next Speed:";
	const char * axisId_Char = "Axis ID:";
	const char * indexId_Char = "Index ID:";
	const char * z_LimitMin_Char = "Limit Min Z";
	const char * z_LimitMax_Char = "Limit Max Z";
	const char * z_Home_Char = "Home Z";
	const char * x_LimitMin_Char = "Limit Min X";
	const char * x_LimitMax_Char = "Limit Max X";
	const char * x_Home_Char = "Home X";
	const char * b_LimitMin_Char = "Limit Min B";
	const char * b_LimitMax_Char = "Limit Max B";
	const char * b_Home_Char = "Home B";
	const char * index1_Char = "Index1";
	const char * index2_Char = "Index2";
	const char * index3_Char = "Index3";
	const char * index4_Char = "Index4";
	const char * index5_Char = "Index5";
	const char * newIndexSize_Char = "New Index Size:";
	const char * indexSizeChar = " Size:";
	const char * degreeDivision_Char = " Degree or Division:";
	const char * radialOrAxial_Char = "Radial or Axial:";
	const char * helixType_Char = "Helix Type:";
	const char * amplitude_Char = "Amplitude:";
	const char * revolutions_Char = "Revolutions:";
	const char * waveDir_Char = "Wave Direction:";
	const char * waves_Char = "Waves:";
	const char * degrees_Char = "Degrees";
	const char * segmentActual_Char = "Segment or Actual:";
	const char * segmentLength_Char = "Segment Length:";
	const char * patternCount_Char = "Pattern Count:";
	const char * pattern_Char = "Pattern:";
	const char * reverseDirection_Char = "Reverse Direction:";
	const char * fileOrFixed_Char = " File or Fixed:";
	const char * lineNumber_Char = "Line Number:";
	const char * pageIndex_t7_Char = "pageIndex.t7.txt=";
	//const char * pageIndex_bt3_pco_Char = "pageIndex.bt3.pco=59391";
	const char * pageIndex_va0_Char = "pageIndex.va0.val=0";
	const char * pageIndex_bt3_Char = "pageIndex.bt3.val=0";
	const char * pageIndex_bt2_Char = "pageIndex.bt2.val=0";
	const char * pageIndex_bt1_Char = "pageIndex.bt1.val=0";
	const char * pageSync_b6_Char = "pageSync.b6.pco=0";
	const char * pageSync_b5_Char = "pageSync.b5.pco=0";
	const char * rose_D_Char = "d:";
	const char * rose_N_Char = "n:";
	//const char * pageMain_va2_Char = "pageMain.va2.val=0";
	const char * activeAxis_Char = "Active Axis";

	const char * nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	const char * nextionEnd = "\xFF\xFF\xFF";

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

	//SerialPrint("serialId: ");
	//SerialPrintln(serialId);

	// All Nextion incoming data packets are terminated with one 0xFF byte
	//if (serialId < 9)
	if ((Serial1.available() > 0) || (Serial2.available() > 0)|| (Serial3.available() > 0))
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
				pageCallerId = GetSerialInteger();
				ReturnToStartPosition(ID_AXIS_B);
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
			case 58: // : Colon - Spindle MaxSpeed and pageOne axis max speeds
			{
				pageCallerId = GetSerialIntegerOnly();
	#ifdef DEBUG
				const char * pageCallerId_Char = "pageCallerId:";
				Serial.print(pageCallerId_Char);
				Serial.println(pageCallerId);
	#endif // DEBUG
				switch (pageCallerId)
				{
					case PAGE_MAIN:
					{
						configMain.maxSpd_Spindle = (int)GetSerialFloat(serialId);
						EEPROM.put(eePromAddress_Main, configMain);
	#ifdef DEBUG
						Serial.print(maxSpd_Char);
						Serial.println(configMain.maxSpd_Spindle);
	#endif // DEBUG
						break;
					}
					case PAGE_ONE:
					{
						switch (configOne.axisId)
						{
						case ID_AXIS_Z:
						{
							configOne.maxSpd_Axis_Z = (int)GetSerialFloat(serialId);
#ifdef DEBUG
							Serial.print(maxSpd_Char);
							Serial.println(configOne.maxSpd_Axis_Z);
#endif // DEBUG
							break;
						}
						case ID_AXIS_X:
						{
							configOne.maxSpd_Axis_X = (int)GetSerialFloat(serialId);
#ifdef DEBUG
							Serial.print(maxSpd_Char);
							Serial.println(configOne.maxSpd_Axis_X);
#endif // DEBUG
							break;
						}
						case ID_AXIS_B:
						{
							configOne.maxSpd_Axis_B = (int)GetSerialFloat(serialId);
#ifdef DEBUG
							Serial.print(maxSpd_Char);
							Serial.println(configOne.maxSpd_Axis_B);
#endif // DEBUG
							break;
						}
						case ID_SPINDLE:
						{
							configOne.maxSpd_Spindle = (int)GetSerialFloat(serialId);
#ifdef DEBUG
							Serial.print(maxSpd_Char);
							Serial.println(configOne.maxSpd_Spindle);
#endif // DEBUG
							break;
						}
						} // end switch (configOne.axisId)

						EEPROM.put(eePromAddress_One, configOne);

						break;
					}
					case PAGE_INDEX:
					{
						configIndex_Main.maxSpd = (int)GetSerialFloat(serialId);
						EEPROM.put(eePromAddress_Ind_Main, configIndex_Main);
#ifdef DEBUG
						Serial.print(maxSpd_Char);
						Serial.println(configIndex_Main.maxSpd);
#endif // DEBUG
						break;
					}
					case PAGE_SYNC:
					{
						configSync.maxSpd_Spindle = (int)GetSerialFloat(serialId);
						EEPROM.put(eePromAddress_Sync, configSync);
	#ifdef DEBUG
						Serial.print(maxSpd_Char);
						Serial.println(configSync.maxSpd_Spindle);
	#endif // DEBUG
						break;
					}
					case PAGE_REC:
					{
						configRec.maxSpd_Spindle = (int)GetSerialFloat(serialId);
						EEPROM.put(eePromAddress_Rec, configRec);
	#ifdef DEBUG
						Serial.print(maxSpd_Char);
						Serial.println(configRec.maxSpd_Spindle);
	#endif // DEBUG
						break;
					}
					case PAGE_GRK:
					case PAGE_GRKFILE:
					{
						configGreekKey.maxSpd_Spindle = (int)GetSerialFloat(serialId);
						
						EEPROM.put(eePromAddress_Grk, configGreekKey);

#ifdef DEBUG
						Serial.print(maxSpd_Char);
						Serial.println(configGreekKey.maxSpd_Spindle);
#endif // DEBUG


						break;
					}
					case PAGE_GEO:
					{
						configRose.maxSpd_Spindle = (int)GetSerialFloat(serialId);
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
			case 59: // ; Semicolon - Spindle Acceleration and pageOne axis accel
			{
				pageCallerId = GetSerialIntegerOnly();
				switch (pageCallerId)
				{
					case PAGE_MAIN:
					{
						configMain.accel_Spindle = (int)GetSerialFloat(serialId);
						EEPROM.put(eePromAddress_Main, configMain);
	#ifdef DEBUG
						Serial.print(accel_Char);
						Serial.println(configMain.accel_Spindle);
	#endif // DEBUG
						break;
					}

					case PAGE_ONE:
					{
						switch (configOne.axisId)
						{
							case ID_AXIS_Z:
							{
								configOne.accel_Axis_Z = (int)GetSerialFloat(serialId);
								break;
							}
							case ID_AXIS_X:
							{
								configOne.accel_Axis_X = (int)GetSerialFloat(serialId);
								break;
							}
							case ID_AXIS_B:
							{
								configOne.accel_Axis_B = (int)GetSerialFloat(serialId);
#ifdef DEBUG
								Serial.print(accel_Char);
								Serial.println(configOne.accel_Axis_B);
#endif // DEBUG
								break;
							}
							case ID_SPINDLE:
							{
								configOne.accel_Spindle = (int)GetSerialFloat(serialId);
		#ifdef DEBUG
								Serial.print(accel_Char);
								Serial.println(configOne.accel_Spindle);
		#endif // DEBUG
								break;
							}
						} // end switch (pageCallerId)

						EEPROM.put(eePromAddress_One, configOne);
	#ifdef DEBUG
						Serial.print(accel_Char);
						Serial.println(configOne.accel_Spindle);
	#endif // DEBUG
						break;
					}
					case PAGE_INDEX:
					{
						configIndex_Main.accel = (int)GetSerialFloat(serialId);
						EEPROM.put(eePromAddress_Ind_Main, configIndex_Main);
	#ifdef DEBUG
						Serial.print(accel_Char);
						Serial.println(configIndex_Main.accel);
	#endif // DEBUG
						break;
					}
					case PAGE_SYNC:
					{
						configSync.accel_Spindle = (int)GetSerialFloat(serialId);
						EEPROM.put(eePromAddress_Sync, configSync);
#ifdef DEBUG
						Serial.print(accel_Char);
						Serial.println(configSync.accel_Spindle);
#endif // DEBUG
						break;
					}
					case PAGE_REC:
					{
						configRec.accel_Spindle = (int)GetSerialFloat(serialId);
						EEPROM.put(eePromAddress_Rec, configRec);
#ifdef DEBUG
						Serial.print(accel_Char);
						Serial.println(configRec.accel_Spindle);
#endif // DEBUG
						break;
					}
					case PAGE_GRK:
					case PAGE_GRKFILE:
					{
						configGreekKey.accel_Spindle = (int)GetSerialFloat(serialId);
						EEPROM.put(eePromAddress_Grk, configGreekKey);
#ifdef DEBUG
						Serial.print("accel_Spindle: ");
						Serial.println(configGreekKey.accel_Spindle);
#endif // DEBUG
						break;

					}
					case PAGE_GEO:
					{
						configRose.accel_Spindle = (int)GetSerialFloat(serialId);
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

				GreekKey_FromFile(reverseDirection);

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
			case 64: // @ - Debugging: Reset all Nextion values to 999
			{
				iniFileType = INI_RESET;
				LoadSettings();
				break;
			}
			case 65: // A - Load Settings.ini
			{
				iniFileType = INI_4AXES;
				LoadSettings();
				break;
			}
			case 66: //B - Sync AxisId
			{
				configSync.axisId = GetSerialInteger();
				EEPROM.put(eePromAddress_Sync, configSync);
	#ifdef DEBUG
				Serial.print(axisId_Char);
				Serial.println(configSync.axisId);
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
				switch (configIndex_Main.indexId)
				{
					case 1:
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
					case 2:
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
					case 3:
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
					case 4:
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
					case 5:
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
			case 71: // G - Not Used
			{	

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
				currentFileIndex = GetSerialInteger();
#ifdef DEBUG				
				Serial.print("GetFileListFromSD: ");
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
				Serial.println(configSetup.distancePerRev_AxisXAlt);
#endif // DEBUG
				break;
			}
			case 80: // P - Greek Key Source: Pattern or file
			{
				configGreekKey.fileOrPattern = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Grk, configGreekKey);
				break;
			}
			case 81: // Q - Index1 counter clockwise
			{
				double newIndexSize = 0;
	#ifdef DEBUG
				Serial.print(indexId_Char);
				Serial.println(configIndex_Main.indexId);
	#endif // DEBUG

				badFilename = false;

				switch (configIndex_Main.indexId)
				{
					case 1:
					{
	#ifdef DEBUG
						Serial.print(fileOrFixed_Char);
						Serial.println(configIndex_1.fileOrFixed);
	#endif // DEBUG
						if (configIndex_1.fileOrFixed == FILE_SD)
						{
							int lineNumber = GetSerialInteger();
							if (lineNumber == 255)
							{
								lineNumber = 0;
							}

							newIndexSize = GetIndexDataFromSD(lineNumber);
							configIndex_1.sizeInSteps = (configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (newIndexSize / 360);
	#ifdef DEBUG
							Serial.print(lineNumber_Char);
							Serial.println(lineNumber);
							Serial.print(newIndexSize_Char);
							Serial.println(newIndexSize);
							Serial.println(index1_Char);
							Serial.print(indexSizeChar);
							Serial.println(configIndex_1.sizeInSteps);
							Serial.println("");
	#endif // DEBUG
							SerialPrint(pageIndex_t7_Char); // Nextion may not get the first packet
							SerialWrite(0x22);
							SerialPrint(newIndexSize,4);
							SerialPrint(nextionQuoteEnd);

							SerialPrint(pageIndex_t7_Char);
							SerialWrite(0x22);
							SerialPrint(newIndexSize,4);
							SerialPrint(nextionQuoteEnd);
						}

						break;
					}
					case 2:
					{
	#ifdef DEBUG
						Serial.print(index2_Char);
						Serial.println(fileOrFixed_Char);
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
							configIndex_2.sizeInSteps = (configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (newIndexSize / 360);
	#ifdef DEBUG

							Serial.print(lineNumber_Char);
							Serial.println(lineNumber);
							Serial.print(newIndexSize_Char);
							Serial.println(newIndexSize);
							Serial.print(index2_Char);
							Serial.print(indexSizeChar);
							Serial.println(configIndex_2.sizeInSteps);
	#endif // DEBUG
							SerialPrint(pageIndex_t7_Char);
							SerialWrite(0x22);
							SerialPrint(newIndexSize, 4);
							SerialPrint(nextionQuoteEnd);

							SerialPrint(pageIndex_t7_Char);
							SerialWrite(0x22);
							SerialPrint(newIndexSize, 4);
							SerialPrint(nextionQuoteEnd);
						}

						break;
					}
					case 3:
					{
	#ifdef DEBUG
						Serial.print(index3_Char);
						Serial.print(fileOrFixed_Char);
						Serial.println(configIndex_3.fileOrFixed);
	#endif // DEBUG


						if (configIndex_3.fileOrFixed == FILE_SD)
						{
							int lineNumber = GetSerialInteger();
							if (lineNumber == 255)
							{
								lineNumber = 0;
							}
							newIndexSize = GetIndexDataFromSD(lineNumber);
							configIndex_3.sizeInSteps = (configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (newIndexSize / 360);
	#ifdef DEBUG

							Serial.print(lineNumber_Char);
							Serial.println(lineNumber);
							Serial.print(newIndexSize_Char);
							Serial.println(newIndexSize);
							Serial.print(index3_Char);
							Serial.print(indexSizeChar);
							Serial.println(configIndex_3.sizeInSteps);
	#endif // DEBUG
							SerialPrint(pageIndex_t7_Char); // Nextion may not get the first packet
							SerialWrite(0x22);
							SerialPrint(newIndexSize, 4);
							SerialPrint(nextionQuoteEnd);

							SerialPrint(pageIndex_t7_Char);
							SerialWrite(0x22);
							SerialPrint(newIndexSize, 4);
							SerialPrint(nextionQuoteEnd);
						}

						break;
					}
					case 4:
					{
#ifdef DEBUG
						Serial.print(index4_Char);
						Serial.print(fileOrFixed_Char);
						Serial.println(configIndex_4.fileOrFixed);
#endif // DEBUG


						if (configIndex_4.fileOrFixed == FILE_SD)
						{
							int lineNumber = GetSerialInteger();
							if (lineNumber == 255)
							{
								lineNumber = 0;
							}
							newIndexSize = GetIndexDataFromSD(lineNumber);
							configIndex_4.sizeInSteps = (configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (newIndexSize / 360);
#ifdef DEBUG

							Serial.print(lineNumber_Char);
							Serial.println(lineNumber);
							Serial.print(newIndexSize_Char);
							Serial.println(newIndexSize);
							Serial.print(index4_Char);
							Serial.print(indexSizeChar);
							Serial.println(configIndex_4.sizeInSteps);
#endif // DEBUG
							SerialPrint(pageIndex_t7_Char); // Nextion may not get the first packet
							SerialWrite(0x22);
							SerialPrint(newIndexSize, 4);
							SerialPrint(nextionQuoteEnd);

							SerialPrint(pageIndex_t7_Char);
							SerialWrite(0x22);
							SerialPrint(newIndexSize, 4);
							SerialPrint(nextionQuoteEnd);
						}

						break;
					}
					case 5:
					{
#ifdef DEBUG
						Serial.print(index5_Char);
						Serial.print(fileOrFixed_Char);
						Serial.println(configIndex_5.fileOrFixed);
#endif // DEBUG


						if (configIndex_5.fileOrFixed == FILE_SD)
						{
							int lineNumber = GetSerialInteger();
							if (lineNumber == 255)
							{
								lineNumber = 0;
							}
							newIndexSize = GetIndexDataFromSD(lineNumber);
							configIndex_5.sizeInSteps = (configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (newIndexSize / 360);
#ifdef DEBUG

							Serial.print(lineNumber_Char);
							Serial.println(lineNumber);
							Serial.print(newIndexSize_Char);
							Serial.println(newIndexSize);
							Serial.print(index5_Char);
							Serial.print(indexSizeChar);
							Serial.println(configIndex_5.sizeInSteps);
#endif // DEBUG
							SerialPrint(pageIndex_t7_Char); // Nextion may not get the first packet
							SerialWrite(0x22);
							SerialPrint(newIndexSize, 4);
							SerialPrint(nextionQuoteEnd);

							SerialPrint(pageIndex_t7_Char);
							SerialWrite(0x22);
							SerialPrint(newIndexSize, 4);
							SerialPrint(nextionQuoteEnd);
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
					//SerialPrint(pageIndex_bt3_pco_Char);
					//SerialPrint(nextionEnd);
					SerialPrint(pageIndex_va0_Char);
					SerialPrint(nextionEnd);
					SerialPrint(pageIndex_bt3_Char);
					SerialPrint(nextionEnd);
					SerialPrint(pageIndex_bt2_Char);
					SerialPrint(nextionEnd);
					SerialPrint(pageIndex_bt1_Char);
					SerialPrint(nextionEnd);
					SerialPrint(pageSync_b6_Char);
					SerialPrint(nextionEnd);
					SerialPrint(pageSync_b5_Char);
					SerialPrint(nextionEnd);
				}
				break;
			}
			case 82: // R - Index1 Clockwise
			{
				double newIndexSize = 0;
				badFilename = false;
	#ifdef DEBUG
				Serial.print(indexId_Char);
				Serial.println(configIndex_Main.indexId);
				Serial.print(index1_Char);
				Serial.print(fileOrFixed_Char);
				Serial.println(configIndex_1.fileOrFixed);
	#endif // DEBUG
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
							configIndex_1.sizeInSteps = (configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (newIndexSize / 360);
		#ifdef DEBUG
							Serial.print(lineNumber_Char);
							Serial.println(lineNumber);
							Serial.print(newIndexSize_Char);
							Serial.println(newIndexSize);
							Serial.print(index1_Char);
							Serial.print(indexSizeChar);
							Serial.println(configIndex_1.sizeInSteps);
							Serial.println("");
		#endif // DEBUG
							SerialPrint(pageIndex_t7_Char); // Nextion may not get the first packet
							SerialWrite(0x22);
							SerialPrint(newIndexSize, 4);
							SerialPrint(nextionQuoteEnd);

							SerialPrint(pageIndex_t7_Char);
							SerialWrite(0x22);
							SerialPrint(newIndexSize, 4);
							SerialPrint(nextionQuoteEnd);
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
							configIndex_2.sizeInSteps = (configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (newIndexSize / 360);
#ifdef DEBUG
							Serial.println("");
							Serial.print(lineNumber_Char);
							Serial.println(lineNumber);
							Serial.print(newIndexSize_Char);
							Serial.println(newIndexSize);
							Serial.print(index2_Char);
							Serial.print(indexSizeChar);
							Serial.println(configIndex_2.sizeInSteps);
							Serial.println("");
#endif // DEBUG
							SerialPrint(pageIndex_t7_Char);
							SerialWrite(0x22);
							SerialPrint(newIndexSize, 4);
							SerialPrint(nextionQuoteEnd);

							SerialPrint(pageIndex_t7_Char);
							SerialWrite(0x22);
							SerialPrint(newIndexSize, 4);
							SerialPrint(nextionQuoteEnd);
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
							configIndex_3.sizeInSteps = (configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (newIndexSize / 360);
		#ifdef DEBUG
							Serial.print(lineNumber_Char);
							Serial.println(lineNumber);
							Serial.print(newIndexSize_Char);
							Serial.println(newIndexSize);
							Serial.print(index3_Char);
							Serial.print(indexSizeChar);
							Serial.println(configIndex_3.sizeInSteps);
							Serial.println("");
		#endif // DEBUG
							SerialPrint(pageIndex_t7_Char); // Nextion may not get the first packet
							SerialWrite(0x22);
							SerialPrint(newIndexSize, 4);
							SerialPrint(nextionQuoteEnd);

							SerialPrint(pageIndex_t7_Char);
							SerialWrite(0x22);
							SerialPrint(newIndexSize, 4);
							SerialPrint(nextionQuoteEnd);
						}

						break;
					}
					case 4:
					{
						if (configIndex_4.fileOrFixed == FILE_SD)
						{
							int lineNumber = GetSerialInteger();
							if (lineNumber == 255)
							{
								lineNumber = 0;
							}
							newIndexSize = GetIndexDataFromSD(lineNumber);
							configIndex_4.sizeInSteps = (configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (newIndexSize / 360);
#ifdef DEBUG
							Serial.print(lineNumber_Char);
							Serial.println(lineNumber);
							Serial.print(newIndexSize_Char);
							Serial.println(newIndexSize);
							Serial.print(index3_Char);
							Serial.print(indexSizeChar);
							Serial.println(configIndex_4.sizeInSteps);
							Serial.println("");
#endif // DEBUG
							SerialPrint(pageIndex_t7_Char); // Nextion may not get the first packet
							SerialWrite(0x22);
							SerialPrint(newIndexSize, 4);
							SerialPrint(nextionQuoteEnd);

							SerialPrint(pageIndex_t7_Char);
							SerialWrite(0x22);
							SerialPrint(newIndexSize, 4);
							SerialPrint(nextionQuoteEnd);
						}

						break;
					}
					case 5:
					{
						if (configIndex_5.fileOrFixed == FILE_SD)
						{
							int lineNumber = GetSerialInteger();
							if (lineNumber == 255)
							{
								lineNumber = 0;
							}
							newIndexSize = GetIndexDataFromSD(lineNumber);
							configIndex_5.sizeInSteps = (configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (newIndexSize / 360);
#ifdef DEBUG
							Serial.print(lineNumber_Char);
							Serial.println(lineNumber);
							Serial.print(newIndexSize_Char);
							Serial.println(newIndexSize);
							Serial.print(index3_Char);
							Serial.print(indexSizeChar);
							Serial.println(configIndex_5.sizeInSteps);
							Serial.println("");
#endif // DEBUG
							SerialPrint(pageIndex_t7_Char); // Nextion may not get the first packet
							SerialWrite(0x22);
							SerialPrint(newIndexSize, 4);
							SerialPrint(nextionQuoteEnd);

							SerialPrint(pageIndex_t7_Char);
							SerialWrite(0x22);
							SerialPrint(newIndexSize, 4);
							SerialPrint(nextionQuoteEnd);
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
					// On fail reset Index flag on Nextion. 
					//SerialPrint(pageIndex_bt3_pco_Char);
					//SerialPrint(nextionEnd);
					SerialPrint(pageIndex_va0_Char);
					SerialPrint(nextionEnd);
					SerialPrint(pageIndex_bt3_Char);
					SerialPrint(nextionEnd);
					SerialPrint(pageIndex_bt2_Char);
					SerialPrint(nextionEnd);
					SerialPrint(pageIndex_bt1_Char);
					SerialPrint(nextionEnd);
					SerialPrint(pageSync_b6_Char);
					SerialPrint(nextionEnd);
					SerialPrint(pageSync_b5_Char);
					SerialPrint(nextionEnd);

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
			case 87: // W - Rec: Radial or Axial
			{
				configRec.radial_axial = GetSerialInteger();
				EEPROM.put(eePromAddress_Rec, configRec);
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
			case 90:  // Z - Main Axis Clockwise
			{
				Main_TwoSteppers(DIR_CW, DIR_CW, ID_AXIS_Z);
				break;
			}
			case 91:  // [ - Z Axis Counterclockwise
			{
				Main_TwoSteppers(DIR_CCW, DIR_CCW, ID_AXIS_Z);
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
			case 95: //_ - Disable stepper
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
			case 97: // a - Rose: Spindle revolutions
			{
				configRose.spindleRevolutions = GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Rose, configRose);
#ifdef DEBUG
				Serial.print("Spindle Revolutions: ");
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
			case 100: // d - pageOne Clockwise
			{
				OneRunStepper(DIR_CW);

				break;
			}
			case 101: // e - pageOne CounterClockwise
			{
				OneRunStepper(DIR_CCW);
	
				break;
			}
			case 102: // f - Set pageOne Speed Percentage
			{
	#ifdef DEBUG
				Serial.print(activeAxis_Char);
				Serial.println(configOne.axisId);

	#endif // DEBUG
				switch (configOne.axisId)
				{
					case ID_AXIS_Z:
					{
						configOne.speedPercent_Axis_Z = (int)GetSerialFloat(serialId);
	#ifdef DEBUG
						Serial.print(z_SpeedPercent_Char);
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
				EEPROM.put(eePromAddress_One, configOne);

				break;
			}
			case 103: // g - Home Z
			{
				configSetup.home_Z = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
				Serial.print(z_Home_Char);
				Serial.println(configSetup.home_Z);
#endif // DEBUG
				break;
			}
			case 104: // h - Home X
			{
				configSetup.home_X = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
				Serial.print(x_Home_Char);
				Serial.println(configSetup.home_X);
#endif // DEBUG
				break;
			}
			case 105: // i - Rose pattern CCW
			{

	#ifdef DEBUG
				Serial.print(axisId_Char);
				Serial.println(configRose.axisId);
	#endif // DEBUG
				RoseRadial(DIR_CCW);
				//switch (configRose.axisId)
				//{
				//	case ID_AXIS_Z: // Z axis
				//	{
				//		RoseRadial_Z(DIR_CCW);
				//		break;
				//	}
				//	case ID_AXIS_X: // X axis
				//	{
				//		RoseRadial_X(DIR_CCW);
				//		break;
				//	}
				//	case ID_AXIS_B: // B axis
				//	{
				//		RoseRadial_B(DIR_CCW);
				//		break;
				//	}
				//}

				break;
			}
			case 106: // j - Home B
			{
				configSetup.home_B = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
				Serial.print(b_Home_Char);
				Serial.println(configSetup.home_B);
#endif // DEBUG
				break;
			}
			case 107: // k - Main: Spindle Clockwise
			{
				Main_TwoSteppers(DIR_CW, DIR_CW, ID_SPINDLE);
				//RunTwoSteppers_Sp_Axis(DIR_CW, DIR_CW, ID_SPINDLE);
				break;
			}
			case 108: // l - Main: Spindle counter clockwise
			{
				Main_TwoSteppers(DIR_CCW, DIR_CCW, ID_SPINDLE);
				//RunTwoSteppers_Sp_Axis(DIR_CCW, DIR_CCW, ID_SPINDLE);
				break;
			}
			case 109: // m - // pageMove: AxisId
			{
				configMove.axisId = GetSerialInteger();
#ifdef DEBUG
				Serial.print("configMove.axisId:");
				Serial.println(configMove.axisId);
#endif // DEBUG

				EEPROM.put(eePromAddress_Mov, configMove);
#ifdef DEBUG
				Serial.print("configMove.axisId: ");
				Serial.println(configMove.axisId);
				configPageMov eepageMove;
				EEPROM.get(eePromAddress_Mov, eepageMove);
				Serial.print("eepageMove.axisId: ");
				Serial.println(eepageMove.axisId);

#endif // DEBUG
				break;
			}
			case 110: // n - Spindle Speed Percentage
			{
				switch (pageCallerId)
				{
					case PAGE_MAIN:
					{
						configMain.speedPercent_Spindle = (int)GetSerialFloat(serialId);
						EEPROM.put(eePromAddress_Main, configMain);
						break;
					}

					case PAGE_ONE:
					{
						// See Case 102
						break;
					}
					case PAGE_INDEX:
					{
						configIndex_Main.speedPercent = (int)GetSerialFloat(serialId);
						EEPROM.put(eePromAddress_Ind_Main, configIndex_Main);
#ifdef DEBUG
						Serial.print(spindle_SpeedPercent_Char);
						Serial.println(configIndex_Main.speedPercent);
#endif // DEBUG
						break;
					}

					case PAGE_BE:
					{
						break;
					}
					case PAGE_SYNC:
					{
						configSync.speedPercent_Spindle = (int)GetSerialFloat(serialId);
						EEPROM.put(eePromAddress_Sync, configSync);
#ifdef DEBUG
						Serial.print(spindle_SpeedPercent_Char);
						Serial.println(configSync.speedPercent_Spindle);
#endif // DEBUG
						break;
					}
					case PAGE_REC:
					{
						configRec.speedPercent_Spindle = (int)GetSerialFloat(serialId);
						EEPROM.put(eePromAddress_Rec, configRec);
#ifdef DEBUG
						Serial.print(spindle_SpeedPercent_Char);
						Serial.println(configRec.speedPercent_Spindle);
#endif // DEBUG
						break;
					}
					case PAGE_GRK:
					case PAGE_GRKFILE:
					{
						configGreekKey.speedPercent_Spindle = (int)GetSerialFloat(serialId);
						EEPROM.put(eePromAddress_Grk, configGreekKey);
#ifdef DEBUG
						Serial.print(spindle_SpeedPercent_Char);
						Serial.println(configGreekKey.speedPercent_Spindle);
#endif // DEBUG

						break;
					}
					case PAGE_GEO:
					{
						configRose.speedPercent_Spindle = (int)GetSerialFloat(serialId);
						EEPROM.put(eePromAddress_Rose, configRose);
#ifdef DEBUG
						Serial.print(spindle_SpeedPercent_Char);
						Serial.println(configRose.speedPercent_Spindle);
#endif // DEBUG
						break;
					}
				}


				break;
			}
			case 111: // o - Axis Speed Percentage
			{
#ifdef DEBUG
				
				Serial.println("Enter case 111: >>>>>>>>>>>>>>>>>>>>>>>>>>");
				Serial.print("pageCallerId: ");
				Serial.println(pageCallerId);
#endif // DEBUG
				switch (pageCallerId)
				{
					case PAGE_MAIN:
					{
						int newSpeedPercentage = (int)GetSerialFloat(serialId);
						switch (configMain.axisId)
						{
							case ID_AXIS_Z:
							{
								configMain.speedPercent_Axis_Z = newSpeedPercentage;
	#ifdef DEBUG
								Serial.print(z_SpeedPercent_Char);
								Serial.println(configMain.speedPercent_Axis_Z);
	#endif // DEBUG
								EEPROM.put(eePromAddress_Main, configMain);
								break;
							}
							case ID_AXIS_X:
							{
								configMain.speedPercent_Axis_X = newSpeedPercentage;
								EEPROM.put(eePromAddress_Main, configMain);
	#ifdef DEBUG
								Serial.print(x_SpeedPercent_Char);
								Serial.println(configMain.speedPercent_Axis_X);
	#endif // DEBUG
								break;
							}
							case ID_AXIS_B:
							{
								configMain.speedPercent_Axis_B = newSpeedPercentage;
								EEPROM.put(eePromAddress_Main, configMain);
	#ifdef DEBUG
								Serial.print(b_SpeedPercent_Char);
								Serial.println(configMain.speedPercent_Axis_B);
	#endif // DEBUG
								break;
							}
						}
						break;
					}

					case PAGE_MOVE:
					{
						switch (configMove.axisId)
						{
							case ID_MOVE_AXIS_Z1:
							case ID_MOVE_AXIS_Z2:
							{
								configMove.speedPercent_Axis_Z = (int)GetSerialFloat(serialId);
								EEPROM.put(eePromAddress_Mov, configMove);
#ifdef DEBUG
								Serial.print(z_SpeedPercent_Char);
								Serial.println(configMove.speedPercent_Axis_Z);
#endif // DEBUG
								break;
							}
							case ID_MOVE_AXIS_X1:
							case ID_MOVE_AXIS_X2:
							{
								configMove.speedPercent_Axis_X = (int)GetSerialFloat(serialId);
								EEPROM.put(eePromAddress_Mov, configMove);
#ifdef DEBUG
								Serial.print(x_SpeedPercent_Char);
								Serial.println(configMove.speedPercent_Axis_X);
#endif // DEBUG
								break;
							}
							case ID_MOVE_AXIS_B1:
							case ID_MOVE_AXIS_B2:
							{
								configMove.speedPercent_Axis_B = (int)GetSerialFloat(serialId);
								EEPROM.put(eePromAddress_Mov, configMove);
#ifdef DEBUG
								Serial.print(b_SpeedPercent_Char);
								Serial.println(configMove.speedPercent_Axis_B);
#endif // DEBUG
								break;
							}
						}
						break;
					}
					case PAGE_BE:
					{
						break;
					}
					case PAGE_SYNC:
					{
						switch (configSync.axisId)
						{
							case ID_AXIS_Z:
							{
								configSync.speedPercent_Axis_Z = (int)GetSerialFloat(serialId);
								EEPROM.put(eePromAddress_Sync, configSync);
#ifdef DEBUG
								Serial.print(z_SpeedPercent_Char);
								Serial.println(configSync.speedPercent_Axis_Z);
#endif // DEBUG
								break;
							}
							case ID_AXIS_X:
							{
								configSync.speedPercent_Axis_X = (int)GetSerialFloat(serialId);
								EEPROM.put(eePromAddress_Sync, configSync);
#ifdef DEBUG
								Serial.print(x_SpeedPercent_Char);
								Serial.println(configSync.speedPercent_Axis_X);
#endif // DEBUG
								break;
							}
							case ID_AXIS_B:
							{
								configSync.speedPercent_Axis_B = (int)GetSerialFloat(serialId);
								EEPROM.put(eePromAddress_Sync, configSync);
#ifdef DEBUG
								Serial.print(b_SpeedPercent_Char);
								Serial.println(configSync.speedPercent_Axis_B);
#endif // DEBUG
								break;
							}
						}
						break;
					}
					case PAGE_REC:
					{
						switch (configRec.axisId)
						{
							case ID_AXIS_Z:
							{
								configRec.speedPercent_Axis_Z = (int)GetSerialFloat(serialId);
								EEPROM.put(eePromAddress_Rec, configRec);
#ifdef DEBUG
								Serial.print(axis_SpeedPercent_Char);
								Serial.println(configRec.speedPercent_Axis_Z);
#endif // DEBUG
								break;
							}
							case ID_AXIS_X:
							{
								configRec.speedPercent_Axis_X = (int)GetSerialFloat(serialId);
								EEPROM.put(eePromAddress_Rec, configRec);
#ifdef DEBUG
								Serial.print(x_SpeedPercent_Char);
								Serial.println(configRec.speedPercent_Axis_X);
#endif // DEBUG
								break;
							}
							case ID_AXIS_B:
							{
								configRec.speedPercent_Axis_B = (int)GetSerialFloat(serialId);
								EEPROM.put(eePromAddress_Rec, configRec);
#ifdef DEBUG
								Serial.print(b_SpeedPercent_Char);
								Serial.println(configRec.speedPercent_Axis_B);
#endif // DEBUG
								break;
							}
						}
						break;
					}
					case PAGE_GRK:
					case PAGE_GRKFILE:
					{
						int percentage = (int)GetSerialFloat(serialId);

#ifdef DEBUG
						Serial.print(axisId_Char);
						Serial.println(configGreekKey.axisId);
						Serial.print(axis_SpeedPercent_Char);
						Serial.println(percentage);
#endif // DEBUG

						switch (configGreekKey.axisId)
						{
							case ID_AXIS_Z:
							{
								configGreekKey.speedPercent_Axis_Z = percentage;
								EEPROM.put(eePromAddress_Grk, configGreekKey);
								break;
							}
							case ID_AXIS_X:
							{
								configGreekKey.speedPercent_Axis_X = percentage;
								EEPROM.put(eePromAddress_Grk, configGreekKey);
								break;
							}
							case ID_AXIS_B:
							{
								configGreekKey.speedPercent_Axis_B = percentage;
								EEPROM.put(eePromAddress_Grk, configGreekKey);
								break;
							}
						}

						break;
					}
					case PAGE_GEO:
					{
						if (configRose.axisId == ID_AXIS_Z)
						{
							configRose.speedPercent_Axis_Z = (int)GetSerialFloat(serialId);
						}
						else if (configRose.axisId == ID_AXIS_X)
						{
							configRose.speedPercent_Axis_X = (int)GetSerialFloat(serialId);
						}
						else
						{
							configRose.speedPercent_Axis_B = (int)GetSerialFloat(serialId);
						}
						EEPROM.put(eePromAddress_Rose, configRose);
						break;
					}
				}

				break;
			}
			case 112: // p - Setup: Spindle Microsteps
			{
				configSetup.microsteps_Spindle = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Setup, configSetup);

				break;
			}
			case 113: // q - Setup: Spindle PulseWidth
			{
				configSetup.pulseWidth_Spindle = (int)GetSerialFloat(serialId);
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
	#ifdef DEBUG
				Serial.print(gearRatio_Char);
				Serial.println(configSetup.gearRatio_Spindle);
	#endif // DEBUG
				break;
			}
			case 116: // t - Setup B: Distance per revolution
			{
				configSetup.distancePerRev_AxisB = GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
				Serial.print(distance_Char);
				Serial.println(configSetup.distancePerRev_AxisB);
#endif // DEBUG
				break;
			}
			case 117: // u - Setup: Spindle speedUpdatePeriod
			{
				configSetup.speedUpdatePeriod_Spindle = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Setup, configSetup);
				break;
			}
			case 118: // v - B axis microsteps
			{
				configSetup.microsteps_Axis_B = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Setup, configSetup);
				break;
			}
			case 119: // w - B axis Steps/360 
			{
				configSetup.steps360_Axis_B = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Setup, configSetup);
	#ifdef DEBUG
				Serial.print(steps360_Char);
				Serial.println(configSetup.steps360_Axis_B);
	#endif // DEBUG
				break;
			}
			case 120: // x - B axis Gear Ratio
			{
				configSetup.gearRatio_AxisB = GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Setup, configSetup);
	#ifdef DEBUG
				Serial.print(gearRatio_Char);
				Serial.println(configSetup.gearRatio_AxisB);
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
				Serial.println(configSetup.distancePerRev_AxisZ);
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
				Serial.println(configOne.axisId);
				Serial.print("configIndex_Main.indexId: ");
				Serial.println(configIndex_Main.indexId);
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
			case 126: // ~ - Test EEPROM settings Config screen
			{
				pageCallerId = GetSerialIntegerOnly();
#ifdef DEBUG
				Serial.print("PageCallerId:");
				Serial.println(pageCallerId);
#endif // DEBUG
				TestEEPROMConfig(pageCallerId);
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
			case 161: // ¡ - Not used
			{
	
				break;
			}
			case 162: // ¢ - Not Used
			{

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
				//RunTwoSteppers_Sp_Axis(DIR_CCW, DIR_CCW, ID_AXIS_X);
				break;
			}
			case 166: // ¦ - Main: X Axis CW
			{
				Main_TwoSteppers(DIR_CW, DIR_CW, ID_AXIS_X);
				//RunTwoSteppers_Sp_Axis(DIR_CW, DIR_CW, ID_AXIS_X);

				break;
			}
			case 167: // § - Not used
			{
				break;
			}
			case 168: // ¨ - Not Used
			{

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
			case 171: // « - Main: B Axis CCW
			{
				Main_TwoSteppers(DIR_CCW, DIR_CCW, ID_AXIS_B);
				break;
			}
			case 172: // ¬ - Main: B Axis CW
			{
				Main_TwoSteppers(DIR_CW, DIR_CW, ID_AXIS_B);
				break;
			}
			case 173: // Don't use 
			{
				break;
			}
			case 174: // ® - Not Used
			{

				break;
			}
			case 175: // ¯ - Not Used
			{

				break;
			}
			case 176: // ° - Not Used
			{
	
				break;
			}
			case 177: // ± - Not Used
			{

				break;
			}
			case 178: // ² - Not Used
			{

				break;
			}
			case 179: // ³ - Not Used
			{

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
					case ID_MOVE_AXIS_B1:
					{
						configMove.distance_MoveB1 = newDistance;
						break;
					}
					case ID_MOVE_AXIS_B2:
					{
						configMove.distance_MoveB2 = newDistance;
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
				pageCallerId = GetSerialIntegerOnly();
				switch (pageCallerId)
				{
					case PAGE_MAIN:
					{
						switch (configMain.axisId)
						{
							case ID_AXIS_Z:
							{
								configMain.maxSpd_Axis_Z = GetSerialFloat(serialId);
								break;
							}
							case ID_AXIS_X:
							{
								configMain.maxSpd_Axis_X = GetSerialFloat(serialId);
								break;
							}
							case ID_AXIS_B:
							{
								configMain.maxSpd_Axis_B = GetSerialFloat(serialId);
								break;
							}
						}
					
						EEPROM.put(eePromAddress_Main, configMain);

						break;
					}
					case PAGE_SYNC:
					{
						switch (configSync.axisId)
						{
						case ID_AXIS_Z:
						{
							configSync.maxSpd_Axis_Z = GetSerialFloat(serialId);
							break;
						}
						case ID_AXIS_X:
						{
							configSync.maxSpd_Axis_X = GetSerialFloat(serialId);
							break;
						}
						case ID_AXIS_B:
						{
							configSync.maxSpd_Axis_B = GetSerialFloat(serialId);
							break;
						}
						}
						EEPROM.put(eePromAddress_Sync, configSync);

						break;
					}
					case PAGE_MOVE:
					{
						switch (configMove.axisId)
						{
							case ID_MOVE_AXIS_Z1:
							case ID_MOVE_AXIS_Z2:
							{
								configMove.maxSpd_Axis_Z = GetSerialFloat(serialId);
								break;
							}
							case ID_MOVE_AXIS_X1:
							case ID_MOVE_AXIS_X2:
							{
								configMove.maxSpd_Axis_X = GetSerialFloat(serialId);
								break;
							}
							case ID_MOVE_AXIS_B1:
							case ID_MOVE_AXIS_B2:
							{
								configMove.maxSpd_Axis_B = GetSerialFloat(serialId);
								break;
							}
						}

						EEPROM.put(eePromAddress_Mov, configMove);
						break;
					}

					case PAGE_ONE:
					{
						// See Case 58
						break;
					}
					case PAGE_REC:
					{
						switch (configRec.axisId)
						{
							case ID_AXIS_Z:
							{
								configRec.maxSpd_Axis_Z = GetSerialFloat(serialId);
								break;
							}
							case ID_AXIS_X:
							{
								configRec.maxSpd_Axis_X = GetSerialFloat(serialId);
								break;
							}
							case ID_AXIS_B:
							{
								configRec.maxSpd_Axis_B = GetSerialFloat(serialId);
								break;
							}
						}

						EEPROM.put(eePromAddress_Rec, configRec);
						break;
					}
					case PAGE_GRK:
					case PAGE_GRKFILE:
					{
						switch (configGreekKey.axisId)
						{
							case ID_AXIS_Z: // Greek Key Z: Z Axis MaxSpd
							{
								configGreekKey.maxSpd_Axis_Z = GetSerialFloat(serialId);
								EEPROM.put(eePromAddress_Grk, configGreekKey);
								break;
							}
							case ID_AXIS_X: // Greek Key X: X Axis MaxSpd
							{
								configGreekKey.maxSpd_Axis_X = GetSerialFloat(serialId);
								EEPROM.put(eePromAddress_Grk, configGreekKey);
								break;
							}
							case ID_AXIS_B:
							{
								configGreekKey.maxSpd_Axis_B = GetSerialFloat(serialId);
								EEPROM.put(eePromAddress_Grk, configGreekKey);
								break;
							}
						}

						break;
					}
					case PAGE_GEO:
					{
						switch (configRose.axisId)
						{
						case ID_AXIS_Z:
						{
							configRose.maxSpd_Axis_Z = GetSerialFloat(serialId);
							break;
						}
						case ID_AXIS_X:
						{
							configRose.maxSpd_Axis_X = GetSerialFloat(serialId);
							break;
						}
						case ID_AXIS_B:
						{
							configRose.maxSpd_Axis_B = GetSerialFloat(serialId);
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
				pageCallerId = GetSerialIntegerOnly();
				switch (pageCallerId)
				{
					case PAGE_MAIN:
					{
						switch (configMain.axisId)
						{
							case ID_AXIS_Z:
							{
								configMain.accel_Axis_Z = GetSerialFloat(serialId);
								break;
							}
							case ID_AXIS_X:
							{
								configMain.accel_Axis_X = GetSerialFloat(serialId);
								break;
							}
							case ID_AXIS_B:
							{
								configMain.accel_Axis_B = GetSerialFloat(serialId);
#ifdef DEBUG
								Serial.print("axisId:");
								Serial.println(configMain.axisId);
								Serial.print("....................accel_Axis_B:");
								Serial.println(configMain.accel_Axis_B);
#endif // DEBUG
								break;
							}
						}
						EEPROM.put(eePromAddress_Main, configMain);
						break;
						
					}
					case PAGE_SYNC:
					{
						{
							switch (configSync.axisId)
							{
								case ID_AXIS_Z:
								{
									configSync.accel_Axis_Z = GetSerialFloat(serialId);
									break;
								}
								case ID_AXIS_X:
								{
									configSync.accel_Axis_X = GetSerialFloat(serialId);
									break;
								}
								case ID_AXIS_B:
								{
									configSync.accel_Axis_B = GetSerialFloat(serialId);
									break;
								}
							}
							EEPROM.put(eePromAddress_Sync, configSync);

							break;
						}
					}
					case PAGE_MOVE:
					{
						switch (configMove.axisId)
						{
							case ID_MOVE_AXIS_Z1:
							case ID_MOVE_AXIS_Z2:
							{
								configMove.accel_Axis_Z = GetSerialFloat(serialId);
								break;
							}
							case ID_MOVE_AXIS_X1:
							case ID_MOVE_AXIS_X2:
							{
								configMove.accel_Axis_X = GetSerialFloat(serialId);
								break;
							}
							case ID_MOVE_AXIS_B1:
							case ID_MOVE_AXIS_B2:
							{
								configMove.accel_Axis_B = GetSerialFloat(serialId);
								break;
							}
						}

						EEPROM.put(eePromAddress_Mov, configMove);
						break;
					}

					case PAGE_ONE:
					{
						// See case 59
						break;
					}
					case PAGE_REC:
					{
						switch (configRec.axisId)
						{
							case ID_AXIS_Z:
							{
								configRec.accel_Axis_Z = GetSerialFloat(serialId);
								break;
							}
							case ID_AXIS_X:
							{
								configRec.accel_Axis_X = GetSerialFloat(serialId);
								break;
							}
							case ID_AXIS_B:
							{
								configRec.accel_Axis_B = GetSerialFloat(serialId);
								break;
							}
						}

						EEPROM.put(eePromAddress_Rec, configRec);
						break;
					}
					case PAGE_GRK:
					case PAGE_GRKFILE:
					{
						switch (configGreekKey.axisId)
						{
							case ID_AXIS_Z: // Greek Key Z: Z Axis MaxSpd
							{
								configGreekKey.accel_Axis_Z = GetSerialFloat(serialId);
								break;
							}
							case ID_AXIS_X: // Greek Key X: X Axis MaxSpd
							{
								configGreekKey.accel_Axis_X = GetSerialFloat(serialId);
								break;
							}
							case ID_AXIS_B:
							{
								configGreekKey.accel_Axis_B = GetSerialFloat(serialId);
								
								break;
							}
						}

						EEPROM.put(eePromAddress_Grk, configGreekKey);
						break;
					}
					case PAGE_GEO:
					{
						switch (configRose.axisId)
						{
						case ID_AXIS_Z:
						{
							configRose.accel_Axis_Z = GetSerialFloat(serialId);
							break;
						}
						case ID_AXIS_X:
						{
							configRose.accel_Axis_X = GetSerialFloat(serialId);
							break;
						}
						case ID_AXIS_B:
						{
							configRose.accel_Axis_B = GetSerialFloat(serialId);
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
				Serial.println(configSetup.distancePerRev_AxisX);
	#endif // DEBUG
				break;
			}
			case 190: // ¾ - Filename-Index and Greek Key
			{
				GetFilenameFromSerial();
				break;
			}
			case 191: // ¿ - Index: Size
			{
				double newIndexSize = GetSerialFloat(serialId);
				int stepsPerRevolution = (int)round(configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle);
	#ifdef DEBUG
				Serial.print(indexId_Char);
				Serial.println(configIndex_Main.indexId);
				Serial.println(newIndexSize);
	#endif // DEBUG
				switch (configIndex_Main.indexId)
				{
					case 1:
					{
						configIndex_1.sizeInUnits = newIndexSize;

						// Default: Divisions
						if (configIndex_1.degreeOrDivision == BY_DEGREES) // Degrees
						{
							configIndex_1.sizeInSteps = stepsPerRevolution * (newIndexSize / 360);
						}
						else
						{
							configIndex_1.sizeInSteps = stepsPerRevolution / (newIndexSize);
						}
		#ifdef DEBUG
						Serial.print(index1_Char);
						Serial.print(indexSizeChar);
						Serial.println(configIndex_1.sizeInSteps);
		#endif // DEBUG
						EEPROM.put(eePromAddress_Ind_1, configIndex_1);
						break;
					}
					case 2:
					{
						configIndex_2.sizeInUnits = newIndexSize;

						// Default: Divisions
						if (configIndex_2.degreeOrDivision == BY_DEGREES) // Degrees
						{
							configIndex_2.sizeInSteps = stepsPerRevolution * (newIndexSize / 360);
						}
						else
						{
							configIndex_2.sizeInSteps = stepsPerRevolution / (newIndexSize);
						}
		#ifdef DEBUG
						Serial.print(index2_Char);
						Serial.print(indexSizeChar);
						Serial.println(configIndex_2.sizeInSteps);
		#endif // DEBUG
						EEPROM.put(eePromAddress_Ind_2, configIndex_2);
						break;
					}
					case 3:
					{
						configIndex_3.sizeInUnits = newIndexSize;

						// Default: Divisions
						if (configIndex_3.degreeOrDivision == BY_DEGREES) // Degrees
						{
							configIndex_3.sizeInSteps = stepsPerRevolution * (newIndexSize / 360);
						}
						else
						{
							configIndex_3.sizeInSteps = stepsPerRevolution / (newIndexSize);
						}

		#ifdef DEBUG
						Serial.print(index3_Char);
						Serial.print(indexSizeChar);
						Serial.println(configIndex_3.sizeInSteps);
		#endif // DEBUG
						EEPROM.put(eePromAddress_Ind_3, configIndex_3);
						break;
					}
					case 4:
					{
						configIndex_4.sizeInUnits = newIndexSize;

						// Default: Divisions
						if (configIndex_4.degreeOrDivision == BY_DEGREES) // Degrees
						{
							configIndex_4.sizeInSteps = stepsPerRevolution * (newIndexSize / 360);
						}
						else
						{
							configIndex_4.sizeInSteps = stepsPerRevolution / (newIndexSize);
						}

#ifdef DEBUG
						Serial.print(index4_Char);
						Serial.print(indexSizeChar);
						Serial.println(configIndex_4.sizeInSteps);
#endif // DEBUG
						EEPROM.put(eePromAddress_Ind_4, configIndex_4);
						break;
					}
					case 5:
					{
						configIndex_5.sizeInUnits = newIndexSize;

						// Default: Divisions
						if (configIndex_5.degreeOrDivision == BY_DEGREES) // Degrees
						{
							configIndex_5.sizeInSteps = stepsPerRevolution * (newIndexSize / 360);
						}
						else
						{
							configIndex_5.sizeInSteps = stepsPerRevolution / (newIndexSize);
						}

#ifdef DEBUG
						Serial.print(index5_Char);
						Serial.print(indexSizeChar);
						Serial.println(configIndex_5.sizeInSteps);
#endif // DEBUG
						EEPROM.put(eePromAddress_Ind_5, configIndex_5);
						break;
					}
				}

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
				Serial.println(configSync.distance);
	#endif // DEBUG
				break;
			}
			case 194: // À - Not used
			{

				break;
			}
			case 195: // Á - Setup: B: Radial or Lineal
			{
				configSetup.radialOrLinear_Axis_B = GetSerialInteger();
				// Radial = 0, Linear = 1

				EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
				Serial.print("radialOrLinear_Axis_B: ");
				Serial.println(configSetup.radialOrLinear_Axis_B);
#endif // DEBUG
				break;
			}
			case 196: // Ä - Index:SourceFixed
			{
				int newSource = GetSerialInteger();
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
						EEPROM.put(eePromAddress_Ind_1, configIndex_1);
		#ifdef DEBUG
						Serial.print(index1_Char);
						Serial.print(fileOrFixed_Char);
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
						EEPROM.put(eePromAddress_Ind_2, configIndex_2);
		#ifdef DEBUG
						Serial.print(index2_Char);
						Serial.print(fileOrFixed_Char);
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
						EEPROM.put(eePromAddress_Ind_3, configIndex_3);
		#ifdef DEBUG
						Serial.print(index3_Char);
						Serial.print(fileOrFixed_Char);
						Serial.println(configIndex_3.fileOrFixed);
		#endif // DEBUG
						break;
					}
					case 4:
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
					case 5:
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
			case 197: // Å - Main: Set axisId
			{
				configMain.axisId = GetSerialInteger();
	#ifdef DEBUG
				Serial.print("configPageMain.axisId:");
				Serial.println(configMain.axisId);
	#endif // DEBUG
				EEPROM.put(eePromAddress_Main, configMain);
	#ifdef DEBUG
				Serial.print(axisId_Char);
				Serial.println(configMain.axisId);
	#endif // DEBUG
				break;
			}
			case 198: // Æ - Rec: Run  
			{
				int waveDir = GetSerialFloat(serialId);

				// In: -1  Out: 1
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
					case 0: // Triangle
					{
						Reciprocate_Triangle(waveDir);
						break;
					}
					case 1: // Sawtooth
					{
						Reciprocate_Sawtooth(waveDir);
						break;
					}
					case 2: // Square
					{
						Reciprocate_Square(waveDir);
						break;
					}
				}

				break;
			}
			case 199: // Ç - Not Used
			{

				break;
			}
			case 200: // È - Not Used
			{
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
				Serial.println(configRec.distance_Axial_Axis);
	#endif // DEBUG
				break;
			}
			case 206: // Î -  Return: Spindle and Z axis to start positions
			{
				pageCallerId = GetSerialInteger();
				ReturnToStartPosition(ID_AXIS_Z);
				break;
			}
			case 207: // Ï - Not Used
			{

				break;
			}
			case 208: // Ð - One: Active Stepper Id
			{
				configOne.axisId = GetSerialInteger();
				EEPROM.put(eePromAddress_One, configOne);
				break;
			}
			case 209: // Ñ - Not Used
			{

				break;
			}
			case 210: // Ò - Not Used
			{

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
			case 215: // × - Return: Spindle and X axis to start positions
			{
				pageCallerId = GetSerialInteger();
				ReturnToStartPosition(ID_AXIS_X);
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
			case 219: // Û - Setup: Polarity B (High or Low)
			{
				int polarityB = GetSerialInteger();

				// true (1): LOW  false (0): HIGH
				polarityB >= 1 ? (configSetup.polarity_Axis_B = true) : (configSetup.polarity_Axis_B = false);
				EEPROM.put(eePromAddress_Setup, configSetup);
				break;
			}
			case 220: // Ü - Limit: Z Min
			{
				configSetup.limit_Min_Z = (int)GetSerialFloat(serialId);
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
				EEPROM.put(eePromAddress_Setup, configSetup);
	#ifdef DEBUG
				Serial.print(x_LimitMax_Char);
				Serial.println(configSetup.limit_Max_X);
	#endif // DEBUG
				break;
			}
			case 224: // à - Limit:  B Min
			{
				configSetup.limit_Min_B = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Setup, configSetup);
	#ifdef DEBUG
				Serial.print(b_LimitMin_Char);
				Serial.println(configSetup.limit_Min_B);
	#endif // DEBUG
				break;
			}
			case 225: // á - Limit:  B Max
			{
				configSetup.limit_Max_B = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Setup, configSetup);
	#ifdef DEBUG
				Serial.print(b_LimitMax_Char);
				Serial.println(configSetup.limit_Max_B);
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
					case ID_AXIS_B:
					{
						configRose.amplitude_Axial_B = GetSerialFloat(serialId);
						EEPROM.put(eePromAddress_Rose, configRose);
#ifdef DEBUG
						Serial.print(amplitude_Char);
						Serial.println(configRose.amplitude_Axial_B);
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
					case ID_AXIS_B:
					{
						configRose.amplitude_Radial_B = GetSerialFloat(serialId);
						EEPROM.put(eePromAddress_Rose, configRose);
#ifdef DEBUG
						Serial.print(amplitude_Char);
						Serial.println(configRose.amplitude_Radial_B);
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
				pageCallerId = PAGE_GEO;
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
					case ID_AXIS_B:
					{
						configSetup.maxSpd_Return_Axis_B = newValue;
						break;
					}
				}
				
				EEPROM.put(eePromAddress_Setup, configSetup);
	#ifdef DEBUG
				Serial.print(maxSpd_Char);
				Serial.println(configSetup.maxSpd_Return_Axis_Z);
	#endif // DEBUG
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
				case ID_AXIS_B:
				{
					configSetup.accel_Return_Axis_B = (int)GetSerialFloat(serialId);
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
				
				// Show the Load ini button
				switch (pageCallerId)
				{
					case 1:
					{
#ifdef DEBUG
						Serial.println ("vis bt0,1");
#endif // DEBUG
						SerialPrint("vis bt0,1");
						SerialPrint(nextionEnd);
					}
				}
	#ifdef DEBUG
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
			case 238: // î - Rec: axisId
			{
				configRec.axisId = GetSerialInteger();
				EEPROM.put(eePromAddress_Rec, configRec);
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
				configIndex_Main.indexId = GetSerialInteger();
	#ifdef DEBUG
				Serial.print(indexId_Char);
				Serial.println(configIndex_Main.indexId);
	#endif // DEBUG
				EEPROM.put(eePromAddress_Ind_Main, configIndex_Main);
				break;
			}
			case 247: // ÷ - Do Greek Key (Left and Right) 
			{
				reverseDirection = GetSerialInteger();
				if (reverseDirection == 0)
				{
					reverseDirection = -1; // Nextion won't send negative number
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

				break;
			}
			case 248: // õ - Not used
			{

				break;
			}
			case 249: // ù - Not Used
			{
			
				break;
			}
			case 250: // ù - Rose: Spindle revolutions
			{
				float revs = GetSerialFloat(serialId);
				configRose.spindleRevolutions = revs;
				EEPROM.put(eePromAddress_Rose, configRose);
#ifdef DEBUG
				Serial.print("Spindle Revolutions-250: ");
				Serial.println(configRose.spindleRevolutions);
				Serial.println(revs);
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
			case 253: // ü - Greek Key AxisId
			{
				configGreekKey.axisId = GetSerialInteger();
				EEPROM.put(eePromAddress_Grk, configGreekKey);
				break;
			}
			case 254: // þ - All: B Radius
			{
				configSetup.radiusB = GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Setup, configSetup);
#ifdef DEBUG
				Serial.print("radiusB: ");
				Serial.println(configSetup.radiusB);
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