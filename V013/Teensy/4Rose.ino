#pragma once
/* *****************************************************************
* 4Rose main entry
* Author: Edward French
* Version: 13 - xxxxxxx
******************************************************************/

#include "math.h"
#include <string>
#include <EEPROM.h>
#include "4RoseDefines.h"
#include "SD.h"
#include "SPI.h"
#include "TeensyStep.h" //  https://github.com/luni64/TeensyStep
#include "AccelStepper.h"
#include "IniFile.h" // https://github.com/stevemarple/IniFile
//#include <MemoryFree.h>

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
	const char* nextionEnd = "\xFF\xFF\xFF";
	pinMode(LED_BUILTIN, OUTPUT);

	// Initialize Serial0 (USB port) Baud rate is always at USB max.
	Serial.begin(115200);
	MilliDelay(15);
	uint32_t serialBaud = Serial.baud();
	Serial.print("Baud:");
	Serial.println(serialBaud);

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
		Serial.print("1-serialId: ");
		Serial.println(serialId);
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
		Serial.print("2-serialId: ");
		Serial.println(serialId);
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
		Serial.print("3-serialId: ");
		Serial.println(serialId);
	}
	Serial.print("serialId: ");
	Serial.println(serialId);

	// Update with values from EEProm
	EEPROM.get(eePromAddress_Setup, configSetup);
	EEPROM.get(eePromAddress_Main, configPageMain);

	EEPROM.get(eePromAddress_Sync, configSync);

	EEPROM.get(eePromAddress_One, configOne);
	EEPROM.get(eePromAddress_Ind_Main, configIndex_Main);
	EEPROM.get(eePromAddress_Ind_1, configIndex_1);
	EEPROM.get(eePromAddress_Ind_2, configIndex_2);
	EEPROM.get(eePromAddress_Ind_3, configIndex_3);
	EEPROM.get(eePromAddress_Mov, configMove);
	EEPROM.get(eePromAddress_Rose, configRose);
	EEPROM.get(eePromAddress_Rec, configRec);
	EEPROM.get(eePromAddress_Grk_Main, configGreekKey_Main);
	EEPROM.get(eePromAddress_Grk_Z, configGreekKey_Z);
	EEPROM.get(eePromAddress_Grk_X, configGreekKey_X);

	// Config as well as all other EEProm settings must be run from Nextion whenever Teensy is updated.  
	// EEProm may contain invalid settings otherwise.

	pinMode(PIN_SPINDLE_ENABLE, OUTPUT);
	pinMode(PIN_AXIS_Z_ENABLE, OUTPUT);
	pinMode(PIN_AXIS_X_ENABLE, OUTPUT);

	pinMode(PIN_AXIS_B_ENABLE, OUTPUT);
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

	// Configure Accelsteppers
	accelStep_Spindle.setEnablePin(PIN_SPINDLE_ENABLE);

	// Accelstepper default for Enable is HIGH.  setPinsInverted(false, false,true) for Enable LOW
	if(!configSetup.polarity_Spindle)
	{ 
		accelStep_Spindle.setPinsInverted(false, false, true);
	}

	SetEnable(ID_SPINDLE, false);
	accelStep_Axis_Z.setEnablePin(PIN_AXIS_Z_ENABLE);
	if (!configSetup.polarity_Axis_Z)
	{
		accelStep_Axis_Z.setPinsInverted(false, false, true);
	}

	accelStep_Axis_X.setEnablePin(PIN_AXIS_X_ENABLE);
	if (!configSetup.polarity_Axis_X)
	{
		accelStep_Axis_X.setPinsInverted(false, false, true);
	}

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
	const char * nextSpeed_Char = "Next Speed:";
	const char * axisId_Char = "Axis ID:";
	const char * indexId_Char = "Index ID:";
	const char * z_LimitMin_Char = "Limit Min Z";
	const char * z_LimitMax_Char = "Limit Max Z";
	const char * x_LimitMin_Char = "Limit Min X";
	const char * x_LimitMax_Char = "Limit Max X";
	const char * b_LimitMin_Char = "Limit Min B";
	const char * b_LimitMax_Char = "Limit Max B";
	const char * index1_Char = "Index1";
	const char * index2_Char = "Index2";
	const char * index3_Char = "Index3";
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
	const char * pageMain_va2_Char = "pageMain.va2.val=0";
	const char * activeAxis_Char = "Active Axis";

	const char * nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	const char * nextionEnd = "\xFF\xFF\xFF";

	if (Serial1.available() > 0)
	{
		serialId = 1;
		Serial.print("1.serialId: ");
	}
	else if (Serial2.available() > 0)
	{
		serialId = 2;
		//Serial.print("2.serialId: ");
	}
	else if (Serial3.available() > 0)
	{
		serialId = 3;
		Serial.print("3.serialId: ");
	}

	//Serial.print("serialId: ");
	//Serial.println(serialId);

	// All Nextion incoming data packets are terminated with one 0xFF byte
	//if (serialId < 9)
	if ((Serial1.available() > 0) || (Serial2.available() > 0)|| (Serial3.available() > 0))
	{
		incomingByte = SerialRead(serialId);

//#ifdef DEBUG
//		Serial.print("serialId: ");
//		Serial.println(serialId);
//		Serial.print("incomingByte: ");
//		Serial.println(incomingByte);
//		Serial.println("MilliDelay");
//#endif // DEBUG

		// Allow processing time for serial data
		MilliDelay(15);


		switch (incomingByte)
		{
			//case 255:
			//{
			//	Serial.println("byte-255");
			//	break;
			//}
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
				EEPROM.put(eePromAddress_Grk_Main, configGreekKey_Main);
				break;
			}
			case 44: // , - Greek Key: File radial or axial
			{
				configGreekKey_Main.radialOrAxial_File = GetSerialInteger();
				EEPROM.put(eePromAddress_Grk_Main, configGreekKey_Main);
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
				EEPROM.put(eePromAddress_Grk_Main, configGreekKey_Main);
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
			case 58: // : Colon - Spindle MaxSpeed
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
						configPageMain.maxSpd_Spindle = (int)GetSerialFloat(serialId);
						EEPROM.put(eePromAddress_Main, configPageMain);
	#ifdef DEBUG
						Serial.print(maxSpd_Char);
						Serial.println(configPageMain.maxSpd_Spindle);
	#endif // DEBUG
						break;
					}
					case PAGE_SYNCZ:
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
				}
				break;
			}
			case 59: // ; Semicolon - Spindle Acceleration
			{
				pageCallerId = GetSerialIntegerOnly();
				switch (pageCallerId)
				{
					case PAGE_MAIN:
					{
						configPageMain.accel_Spindle = (int)GetSerialFloat(serialId);
						EEPROM.put(eePromAddress_Main, configPageMain);
	#ifdef DEBUG
						Serial.print(accel_Char);
						Serial.println(configPageMain.accel_Spindle);
	#endif // DEBUG
						break;
					}
					case PAGE_SYNCZ:
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
						Serial.println(configRec.accel_Spindle);
						EEPROM.put(eePromAddress_Rec, configRec);
	#ifdef DEBUG
						Serial.print(accel_Char);
						Serial.println(configRec.accel_Spindle);
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
				} // end switch (pageCallerId)

				break;
			}
			case 60: // < Less than - Rose: Axis MaxSpd
			{
				configRose.maxSpd_Axis_Z = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Rose, configRose);
	#ifdef DEBUG
				Serial.print(maxSpd_Char);
				Serial.println(configRose.maxSpd_Axis_Z);
	#endif // DEBUG
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

				GreekKeyFromFile(reverseDirection);

				break;
			}
			case 62: // > Greater than - Set moveZ_ speed
			{
				configMove.speedPercent_Axis_Z = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Mov, configMove);
	#ifdef DEBUG
				Serial.print(z_SpeedPercent_Char);
				Serial.println(configMove.speedPercent_Axis_Z);
	#endif // DEBUG
				break;
			}
			case 63: // ? Question mark - moveZ_Distance (Text)
			{
				// Set Distance
				float newDistance = GetSerialFloat(serialId);
				configMove.distance_MoveZ = newDistance;
				EEPROM.put(eePromAddress_Mov, configMove);
	#ifdef DEBUG
				Serial.print(distance_Char);
				Serial.println(configMove.distance_MoveZ);
	#endif // DEBUG
				break;
			}
			case 64: // @ - Rose: Spindle RPM
			{
				configRose.speedPercent_Spindle = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Rose, configRose);
	#ifdef DEBUG
				Serial.print(spindle_SpeedPercent_Char);
				Serial.println(configRose.speedPercent_Spindle);
	#endif // DEBUG
				break;
			}
			case 65: // A - Load Settings.ini
			{
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
			// C: -> Cancel Stop Main/Sp2 Spindle
			// Implemented in Run methods
			case 67: // C - 
			{
			   break;
			}
			case 68: // D - SyncZ Spindle Speed percentage
			{
				configSync.speedPercent_Spindle = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Sync, configSync);
	#ifdef DEBUG
				Serial.print(spindle_SpeedPercent_Char);
				Serial.println(configSync.speedPercent_Spindle);
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
				Serial.print(maxSpd_Char);
				Serial.println(maxSpeed);
	#endif // DEBUG

				switch(configGreekKey_Main.axisId)
				{ 
					case ID_AXIS_Z: // Greek Key Z: Z Axis MaxSpd
					{
						configGreekKey_Z.maxSpd_Axis = maxSpeed;
						EEPROM.put(eePromAddress_Grk_Z, configGreekKey_Z);
						break;
					}
					case ID_AXIS_X: // Greek Key X: X Axis MaxSpd
					{
						configGreekKey_X.maxSpd_Axis = maxSpeed;
						EEPROM.put(eePromAddress_Grk_X, configGreekKey_X);
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
				configSync.helixType = GetSerialInteger();
				EEPROM.put(eePromAddress_Sync, configSync);
	#ifdef DEBUG
				Serial.print(helixType_Char);
				Serial.println(configSync.helixType);
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
				Serial.print(segmentActual_Char);
				Serial.println(configGreekKey_Main.segmentOrActual);
	#endif // DEBUG
				EEPROM.put(eePromAddress_Grk_Main, configGreekKey_Main);
				break;
			}
			case 77: // M - Greek Key File
			{
				configGreekKey_Main.countPatternFile = (int)GetSerialFloat(serialId);//  GetSerialInteger();
	#ifdef DEBUG
				Serial.print(patternCount_Char);
				Serial.println(configGreekKey_Main.countPatternFile);
	#endif // DEBUG
				EEPROM.put(eePromAddress_Grk_Main, configGreekKey_Main);
				break;
			}
			case 78: // N - GetFileListFromSD
			{
				currentFileIndex = GetSerialInteger();
				
				Serial.print("GetFileListFromSD: ");
				Serial.println(currentFileIndex);
				GetFileListFromSD(currentFileIndex);

				break;
			}
			case 79: // O - Sync Axis Speed
			{
				configSync.speedPercent_Axis_Z = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Sync, configSync);
	#ifdef DEBUG
				Serial.print(spindle_SpeedPercent_Char);
				Serial.println(configSync.speedPercent_Axis_Z);
	#endif // DEBUG
				break;
			}
			case 80: // P - Greek Key Source: Pattern or file
			{
				configGreekKey_Main.fileOrPattern = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Grk_Main, configGreekKey_Main);
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
							configIndex_1.size = (configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (newIndexSize / 360);
	#ifdef DEBUG
							Serial.print(lineNumber_Char);
							Serial.println(lineNumber);
							Serial.print(newIndexSize_Char);
							Serial.println(newIndexSize);
							Serial.println(index1_Char);
							Serial.print(indexSizeChar);
							Serial.println(configIndex_1.size);
							Serial.println("");
	#endif // DEBUG
							SerialPrint(pageIndex_t7_Char); // Nextion may not get the first packet
							SerialWrite(0x22);
							SerialPrint(newIndexSize);
							SerialPrint(nextionQuoteEnd);

							SerialPrint(pageIndex_t7_Char);
							SerialWrite(0x22);
							SerialPrint(newIndexSize);
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
							configIndex_2.size = (configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (newIndexSize / 360);
	#ifdef DEBUG

							Serial.print(lineNumber_Char);
							Serial.println(lineNumber);
							Serial.print(newIndexSize_Char);
							Serial.println(newIndexSize);
							Serial.print(index2_Char);
							Serial.print(indexSizeChar);
							Serial.println(configIndex_2.size);
	#endif // DEBUG
							SerialPrint(pageIndex_t7_Char);
							SerialWrite(0x22);
							SerialPrint(newIndexSize);
							SerialPrint(nextionQuoteEnd);

							SerialPrint(pageIndex_t7_Char);
							SerialWrite(0x22);
							SerialPrint(newIndexSize);
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
							configIndex_3.size = (configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (newIndexSize / 360);
	#ifdef DEBUG

							Serial.print(lineNumber_Char);
							Serial.println(lineNumber);
							Serial.print(newIndexSize_Char);
							Serial.println(newIndexSize);
							Serial.print(index3_Char);
							Serial.print(indexSizeChar);
							Serial.println(configIndex_3.size);
	#endif // DEBUG
							SerialPrint(pageIndex_t7_Char); // Nextion may not get the first packet
							SerialWrite(0x22);
							SerialPrint(newIndexSize);
							SerialPrint(nextionQuoteEnd);

							SerialPrint(pageIndex_t7_Char);
							SerialWrite(0x22);
							SerialPrint(newIndexSize);
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
							configIndex_1.size = (configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (newIndexSize / 360);
		#ifdef DEBUG
							Serial.print(lineNumber_Char);
							Serial.println(lineNumber);
							Serial.print(newIndexSize_Char);
							Serial.println(newIndexSize);
							Serial.print(index1_Char);
							Serial.print(indexSizeChar);
							Serial.println(configIndex_1.size);
							Serial.println("");
		#endif // DEBUG
							SerialPrint(pageIndex_t7_Char); // Nextion may not get the first packet
							SerialWrite(0x22);
							SerialPrint(newIndexSize);
							SerialPrint(nextionQuoteEnd);

							SerialPrint(pageIndex_t7_Char);
							SerialWrite(0x22);
							SerialPrint(newIndexSize);
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
							configIndex_2.size = (configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (newIndexSize / 360);
#ifdef DEBUG
							Serial.println("");
							Serial.print(lineNumber_Char);
							Serial.println(lineNumber);
							Serial.print(newIndexSize_Char);
							Serial.println(newIndexSize);
							Serial.print(index2_Char);
							Serial.print(indexSizeChar);
							Serial.println(configIndex_2.size);
							Serial.println("");
#endif // DEBUG
							SerialPrint(pageIndex_t7_Char);
							SerialWrite(0x22);
							SerialPrint(newIndexSize);
							SerialPrint(nextionQuoteEnd);

							SerialPrint(pageIndex_t7_Char);
							SerialWrite(0x22);
							SerialPrint(newIndexSize);
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
							configIndex_3.size = (configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (newIndexSize / 360);
		#ifdef DEBUG
							Serial.print(lineNumber_Char);
							Serial.println(lineNumber);
							Serial.print(newIndexSize_Char);
							Serial.println(newIndexSize);
							Serial.print(index3_Char);
							Serial.print(indexSizeChar);
							Serial.println(configIndex_3.size);
							Serial.println("");
		#endif // DEBUG
							SerialPrint(pageIndex_t7_Char); // Nextion may not get the first packet
							SerialWrite(0x22);
							SerialPrint(newIndexSize);
							SerialPrint(nextionQuoteEnd);

							SerialPrint(pageIndex_t7_Char);
							SerialWrite(0x22);
							SerialPrint(newIndexSize);
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
			case 83: // S - Set Index speed
			{
				configIndex_Main.speedPercent = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Ind_Main, configIndex_Main);
	#ifdef DEBUG
				Serial.print(spindle_SpeedPercent_Char);
				Serial.println(configIndex_Main.speedPercent);
	#endif // DEBUG
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
				configGreekKey_Main.segmentLengthForFile = GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Grk_Main, configGreekKey_Main);
	#ifdef DEBUG
				Serial.print(segmentLength_Char);
				Serial.println(configGreekKey_Main.segmentLengthForFile);
	#endif // DEBUG
				break;
			}
			case 86: // V - Not Used
			{
				break;
			}
			case 87: // W - Set Index1 type to Degrees or Divisions
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
				RunTwoSteppers_All(DIR_CW, DIR_CW, ID_AXIS_Z);
				//RunTwoSteppers_Sp_Axis(DIR_CW, DIR_CW, ID_AXIS_Z);
				break;
			}
			case 91:  // [ - Z Axis Counterclockwise
			{
				RunTwoSteppers_All(DIR_CCW, DIR_CCW, ID_AXIS_Z);
				//RunTwoSteppers_Sp_Axis(DIR_CCW, DIR_CCW, ID_AXIS_Z);
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
				switch (configPageMain.axisId)
				{
					case ID_AXIS_Z:
					{
						configPageMain.speedPercent_Axis_Z = newSpeedPercentage;
	#ifdef DEBUG
						Serial.print(z_SpeedPercent_Char);
						Serial.println(configPageMain.speedPercent_Axis_Z);
	#endif // DEBUG
						EEPROM.put(eePromAddress_Main, configPageMain);
						break;
					}
					case ID_AXIS_X:
					{
						configPageMain.speedPercent_Axis_X = newSpeedPercentage;
						EEPROM.put(eePromAddress_Main, configPageMain);
	#ifdef DEBUG
						Serial.print(x_SpeedPercent_Char);
						Serial.println(configPageMain.speedPercent_Axis_X);
	#endif // DEBUG
						break;
					}
					case ID_AXIS_B:
					{
						configPageMain.speedPercent_Axis_B = newSpeedPercentage;
						EEPROM.put(eePromAddress_Main, configPageMain);
	#ifdef DEBUG
						Serial.print(b_SpeedPercent_Char);
						Serial.println(configPageMain.speedPercent_Axis_B);
	#endif // DEBUG
						break;
					}
				}

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
				SerialPrint(pageMain_va2_Char);
				SerialPrint(nextionEnd);
				break;
			}
			case 100: // d - pageOne Clockwise
			{
				RunOneStepper(DIR_CW);
				Serial.println("End case 100 ");
				break;
			}
			case 101: // e - pageOne CounterClockwise
			{
				RunOneStepper(DIR_CCW);
				Serial.println("End case 101 ");
				break;
			}
			case 102: // f - Set pageOne Speed
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
			case 103: // g - Rose: Spindle MaxSpd
			{
				configRose.maxSpd_Spindle = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Rose, configRose);
	#ifdef DEBUG
				Serial.print(maxSpd_Char);
				Serial.println(configRose.maxSpd_Spindle);
	#endif // DEBUG
				break;
			}
			case 104: // h - Rose: Spindle Accel
			{
				configRose.accel_Spindle = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Rose, configRose);
	#ifdef DEBUG
				Serial.print(accel_Char);
				Serial.println(configRose.accel_Spindle);
	#endif // DEBUG
				break;
			}
			case 105: // i - Rose pattern CCW
			{

	#ifdef DEBUG
				Serial.print(axisId_Char);
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
				//RunTwoSteppers_Sp_Axis(DIR_CW, DIR_CW, ID_SPINDLE);
				break;
			}
			case 108: // l - Z spindle counter clockwise
			{
				RunTwoSteppers_All(DIR_CCW, DIR_CCW, ID_SPINDLE);
				//RunTwoSteppers_Sp_Axis(DIR_CCW, DIR_CCW, ID_SPINDLE);
				break;
			}
			case 109: // m - // pageMove AxisId
			{
				configMove.axisId = GetSerialInteger();
#ifdef DEBUG
				Serial.print("configMove.axisId:");
				Serial.println(configMove.axisId);
#endif // DEBUG

				EEPROM.put(eePromAddress_Mov, configMove);
#ifdef DEBUG
				Serial.print(axisId_Char);
				Serial.println(configMove.axisId);
#endif // DEBUG
				break;
			}
			case 110: // n - Set SpZ (Main) Spindle Speed
			{
				configPageMain.speedPercent_Spindle = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Main, configPageMain);
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
				configSetup.microsteps_Spindle = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Setup, configSetup);

				break;
			}
			case 113: // q - Move Z counterclockwise
			{
				MoveAxis(ID_AXIS_Z,DIR_CW);
				break;
			}
			case 114: // r - Main/Sync/pageOne Spindle FullSteps
			{
				configSetup.steps360_Spindle = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Setup, configSetup);
	#ifdef DEBUG
				Serial.print(steps360_Char);
				Serial.println(configSetup.steps360_Spindle);
	#endif // DEBUG
				break;
			}
			case 115: // s - Main/Sync/pageOne Spindle Gear Ratio
			{
				configSetup.gearRatio_Spindle = GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Setup, configSetup);
	#ifdef DEBUG
				Serial.print(gearRatio_Char);
				Serial.println(configSetup.gearRatio_Spindle);
	#endif // DEBUG
				break;
			}
			case 116: // t - B axis Acceleration
			{
				pageCallerId = GetSerialIntegerOnly();
				switch (pageCallerId)
				{
					case PAGE_MAIN:
					{
						configPageMain.accel_Axis_B = GetSerialFloat(serialId);
						EEPROM.put(eePromAddress_Main, configPageMain);
	#ifdef DEBUG
						Serial.print(accel_Char);
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
						EEPROM.put(eePromAddress_Main, configPageMain);
	#ifdef DEBUG
						Serial.print(maxSpd_Char);
						Serial.println(configPageMain.maxSpd_Axis_B);
	#endif // DEBUG
						break;
					}
				}
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
			case 124: // | - Z Axis Acceleration
			{
				pageCallerId = GetSerialIntegerOnly();
				switch (pageCallerId)
				{
					case PAGE_MAIN:
					{
						configPageMain.accel_Axis_Z = GetSerialFloat(serialId);
						EEPROM.put(eePromAddress_Main, configPageMain);
	#ifdef DEBUG
						Serial.print(accel_Char);
						Serial.println(configPageMain.accel_Axis_Z);
	#endif // DEBUG
						break;
					}
					case PAGE_SYNCZ:
					{
						configSync.accel_Axis_Z = GetSerialFloat(serialId);
						EEPROM.put(eePromAddress_Sync, configSync);
	#ifdef DEBUG
						Serial.print(accel_Char);
						Serial.println(configSync.accel_Axis_Z);
	#endif // DEBUG
						break;
					}
					case PAGE_REC:
					{
						switch (configRec.axisId)
						{
							case ID_AXIS_Z:
							{
								configRec.accel_Axis_Z = GetSerialFloat(serialId);
								EEPROM.put(eePromAddress_Rec, configRec);
		#ifdef DEBUG
								Serial.print(accel_Char);
								Serial.println(configRec.accel_Axis_Z);
		#endif // DEBUG
								break;
							}
							case ID_AXIS_X:
							{
								configRec.accel_Axis_X = GetSerialFloat(serialId);
								EEPROM.put(eePromAddress_Rec, configRec);
		#ifdef DEBUG
								Serial.print(accel_Char);
								Serial.println(configRec.accel_Axis_X);
		#endif // DEBUG
								break;
							}
						}

						break;
					}
					case PAGE_MOVE:
					{
						configMove.accel_Axis_Z = GetSerialFloat(serialId);
						EEPROM.put(eePromAddress_Mov, configMove);
	#ifdef DEBUG
						Serial.print(accel_Char);
						Serial.println(configMove.accel_Axis_Z);
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
				Serial.print(accel_Char);
				Serial.println(configRose.accel_Axis_Z);
	#endif // DEBUG
				break;
			}
			case 126: // ~ - Test EEPROM settings Config screen
			{
				pageCallerId = GetSerialIntegerOnly();
				Serial.print("PageCallerId:");
				Serial.println(pageCallerId);

				//Serial.print("Call TestEEPROMConfig:");
				Serial.println(pageCallerId);
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
			case 162: // ¢ - Sp-X axis speed
			{
				configPageMain.speedPercent_Axis_X = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Main, configPageMain);
	#ifdef DEBUG
				Serial.print(x_SpeedPercent_Char);
				Serial.println(configPageMain.speedPercent_Axis_X);
				float nextSpeed_AxisX = configPageMain.speedPercent_Axis_X  * configPageMain.maxSpd_Axis_X * .01;
				Serial.print(nextSpeed_Char);
				Serial.println(nextSpeed_AxisX);
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
			case 165: // ¥ - Sp-X Axis CCW
			{
				RunTwoSteppers_All(DIR_CCW, DIR_CCW, ID_AXIS_X);
				//RunTwoSteppers_Sp_Axis(DIR_CCW, DIR_CCW, ID_AXIS_X);
				break;
			}
			case 166: // ¦ - Sp-X Axis CW
			{
				RunTwoSteppers_All(DIR_CW, DIR_CW, ID_AXIS_X);
				//RunTwoSteppers_Sp_Axis(DIR_CW, DIR_CW, ID_AXIS_X);

				break;
			}
			case 167: // § - Not used
			{
				break;
			}
			case 168: // ¨ - SpB axis speed
			{
				configPageMain.speedPercent_Axis_B = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Main, configPageMain);
	#ifdef DEBUG
				Serial.print(b_SpeedPercent_Char);
				Serial.println(configPageMain.speedPercent_Axis_B);
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
			case 173: // Don't use 
			{
				break;
			}
			case 174: // ® - SyncX axis speed
			{
				configSync.speedPercent_Axis_X = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Sync, configSync);
	#ifdef DEBUG
				Serial.print(x_SpeedPercent_Char);
				Serial.println(configSync.speedPercent_Axis_X);
	#endif // DEBUG
				break;
			}
			case 175: // ¯ - SyncX spindle speed
			{
				configSync.speedPercent_Spindle = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Sync, configSync);
	#ifdef DEBUG
				Serial.print(spindle_SpeedPercent_Char);
				Serial.println(configSync.speedPercent_Spindle);
	#endif // DEBUG
				break;
			}
			case 176: // ° - SyncX Helix direction
			{
				configSync.helixType = GetSerialInteger();
				EEPROM.put(eePromAddress_Sync, configSync);
	#ifdef DEBUG
				Serial.print(helixType_Char);
				Serial.println(configSync.helixType);
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
				configMove.speedPercent_Axis_X = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Mov, configMove);
	#ifdef DEBUG
				Serial.print(x_SpeedPercent_Char);
				Serial.println(configMove.speedPercent_Axis_X);
	#endif // DEBUG
				break;
			}
			case 180: // Don't use
			{
				break;
			}
			case 181: // µ - MvX Distance (Text)
			{
				// Set Distance
				float newDistance = GetSerialFloat(serialId);
				configMove.distance_MoveX = newDistance;
				EEPROM.put(eePromAddress_Mov, configMove);
	#ifdef DEBUG
				Serial.print(distance_Char);
				Serial.println(configMove.distance_MoveX);
	#endif // DEBUG
				break;
			}
			case 182: // ¶ - Greek Key: Pattern Type  (2 through 5)
			{
				int pattern = GetSerialInteger();// 

				configGreekKey_Main.patternId = pattern;
				EEPROM.put(eePromAddress_Grk_Main, configGreekKey_Main);

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
						EEPROM.put(eePromAddress_Main, configPageMain);
	#ifdef DEBUG
						Serial.print(maxSpd_Char);
						Serial.println(configPageMain.maxSpd_Axis_X);
	#endif // DEBUG
						break;
					}
					case PAGE_SYNCZ:
					{
						configSync.maxSpd_Axis_X = GetSerialFloat(serialId);
						EEPROM.put(eePromAddress_Sync, configSync);
	#ifdef DEBUG
						Serial.print(maxSpd_Char);
						Serial.println(configSync.maxSpd_Axis_X);
	#endif // DEBUG
						break;
					}
					case PAGE_MOVE:
					{
						configMove.maxSpd_Axis_X = GetSerialFloat(serialId);
						EEPROM.put(eePromAddress_Mov, configMove);
	#ifdef DEBUG
						Serial.print(maxSpd_Char);
						Serial.println(configMove.maxSpd_Axis_X);
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
						EEPROM.put(eePromAddress_Main, configPageMain);
	#ifdef DEBUG
						Serial.print(accel_Char);
						Serial.println(configPageMain.accel_Axis_X);
	#endif // DEBUG
						break;
					}
					case PAGE_SYNCZ:
					{
						configSync.accel_Axis_X = GetSerialFloat(serialId);
						EEPROM.put(eePromAddress_Sync, configSync);
	#ifdef DEBUG
						Serial.print(accel_Char);
						Serial.println(configSync.accel_Axis_X);
	#endif // DEBUG
						break;
					}
					case PAGE_MOVE:
					{
						configMove.accel_Axis_X = GetSerialFloat(serialId);
						EEPROM.put(eePromAddress_Mov, configMove);
	#ifdef DEBUG
						Serial.print(accel_Char);
						Serial.println(configMove.accel_Axis_X);
	#endif // DEBUG
						break;
					}
				}
				break;
			}
			case 187: // » - X microsteps
			{
				configSetup.microsteps_Axis_X = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Setup, configSetup);
				break;
			}
			case 188: // ¼ - X Full Steps
			{
				configSetup.steps360_Axis_X = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Setup, configSetup);
	#ifdef DEBUG
				Serial.print(steps360_Char);
				Serial.println(configSetup.steps360_Axis_X);
	#endif // DEBUG
				break;
			}
			case 189: // ½ - X Distance per revolution (Text)
			{
				configSetup.distancePerRev_AxisX = GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Setup, configSetup);
	#ifdef DEBUG
				Serial.print(distance_Char);
				Serial.println(configSetup.distancePerRev_AxisX);
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
						Serial.print(index1_Char);
						Serial.print(indexSizeChar);
						Serial.println(configIndex_1.size);
		#endif // DEBUG
						EEPROM.put(eePromAddress_Ind_1, configIndex_1);
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
						Serial.print(index2_Char);
						Serial.print(indexSizeChar);
						Serial.println(configIndex_2.size);
		#endif // DEBUG
						EEPROM.put(eePromAddress_Ind_2, configIndex_2);
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
						Serial.print(index3_Char);
						Serial.print(indexSizeChar);
						Serial.println(configIndex_3.size);
		#endif // DEBUG
						EEPROM.put(eePromAddress_Ind_3, configIndex_3);
						break;
					}
				}

				break;
			}
			case 192: // À - Revolutions_SyncZ
			{
				configSync.revolutions_Spindle = GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Sync, configSync);
	#ifdef DEBUG
				Serial.print(revolutions_Char);
				Serial.println(configSync.revolutions_Spindle);
	#endif // DEBUG
				break;
			}
			case 193: // Á - Distance_SyncZ
			{
				configSync.distance = GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Sync, configSync);
	#ifdef DEBUG
				Serial.print(distance_Char);
				Serial.println(configSync.distance);
	#endif // DEBUG
				break;
			}
			case 194: // À - Revolutions_Sync
			{
				configSync.revolutions_Spindle = GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Sync, configSync);
	#ifdef DEBUG
				Serial.print(revolutions_Char);
				Serial.println(configSync.revolutions_Spindle);
	#endif // DEBUG
				break;
			}
			case 195: // Á - Distance_SyncZ
			{
				configSync.distance = GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Sync, configSync);
	#ifdef DEBUG
				Serial.print(distance_Char);
				Serial.println(configSync.distance);
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
						Serial.print(index1_Char);
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
						Serial.print(index1_Char);
						Serial.print(fileOrFixed_Char);
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
				EEPROM.put(eePromAddress_Main, configPageMain);
	#ifdef DEBUG
				Serial.print(axisId_Char);
				Serial.println(configPageMain.axisId);
	#endif // DEBUG
				break;
			}
			case 198: // Æ - Do Rec AxialZ 
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
	#endif // DEBUG

				switch (configRec.axisId)
				{
					case ID_AXIS_Z: // Z Axis
					{
						switch (configRec.radial_axial)
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
						switch (configRec.radial_axial)
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
				Serial.print(amplitude_Char);
				Serial.println(configRose.amplitude_Radial_Z);
	#endif // DEBUG
				break;
			}
			case 200: // È - Greek Key: Spindle Accel
			{
				//int speedBarId = GetSerialIntegerOnly();
				int accel = (int)GetSerialFloat(serialId);
	#ifdef DEBUG
				Serial.print(accel);
				Serial.println(accel);
	#endif // DEBUG
				switch (configGreekKey_Main.axisId)
				{
					case ID_AXIS_Z: // Greek Key Z: Spindle accel
					{
						configGreekKey_Z.accel_Spindle = accel;
						EEPROM.put(eePromAddress_Grk_Z, configGreekKey_Z);
						break;
					}
					case ID_AXIS_X: // Greek Key X: Spindle accel
					{
						configGreekKey_X.accel_Spindle = accel;
						EEPROM.put(eePromAddress_Grk_X, configGreekKey_X);
						break;
					}
				}
				break;
			}
			case 201: // É - DoRosePattern_X
			{
	#ifdef DEBUG
				Serial.print(axisId_Char);
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

	#ifdef DEBUG
				Serial.print(pageCallerId_Char);
				Serial.println(pageCallerId);
	#endif // DEBUG
				switch (pageCallerId)
				{
					case PAGE_MAIN:
					{
						configPageMain.maxSpd_Axis_Z = GetSerialFloat(serialId);
						EEPROM.put(eePromAddress_Main, configPageMain);
	#ifdef DEBUG
						Serial.print(maxSpd_Char);
						Serial.println(configPageMain.maxSpd_Axis_Z);
	#endif // DEBUG
						break;
					}
					case PAGE_SYNCZ:
					{
						configSync.maxSpd_Axis_Z = GetSerialFloat(serialId);
						EEPROM.put(eePromAddress_Sync, configSync);
	#ifdef DEBUG
						Serial.print(maxSpd_Char);
						Serial.println(configSync.maxSpd_Axis_Z);
	#endif // DEBUG
						break;
					}
					case PAGE_REC:
					{
						switch (configRec.axisId)
						{
							case ID_AXIS_Z:
							{
								configRec.maxSpd_Axis_Z = GetSerialFloat(serialId);
								EEPROM.put(eePromAddress_Rec, configRec);
		#ifdef DEBUG
								Serial.print(maxSpd_Char);
								Serial.println(configRec.maxSpd_Axis_Z);
		#endif // DEBUG
								break;
							}
							case ID_AXIS_X:
							{
								configRec.maxSpd_Axis_X = GetSerialFloat(serialId);
								EEPROM.put(eePromAddress_Rec, configRec);
		#ifdef DEBUG
								Serial.print(maxSpd_Char);
								Serial.println(configRec.maxSpd_Axis_X);
		#endif // DEBUG
								break;
							}
						}

						break;
					}
					case PAGE_MOVE:
					{
						configMove.maxSpd_Axis_Z = GetSerialFloat(serialId);
						EEPROM.put(eePromAddress_Mov, configMove);
	#ifdef DEBUG
						Serial.print(maxSpd_Char);
						Serial.println(configMove.maxSpd_Axis_Z);
	#endif // DEBUG
						break;
					}
				}

				break;
			}
			case 203: // Ì - Rec1_Z Axial Waves (Count)
			{
				configRec.waves_AxialZ = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Rec, configRec);
	#ifdef DEBUG
				Serial.print(waves_Char);
				Serial.println(configRec.waves_AxialZ);
	#endif // DEBUG
				break;
			}
			case 204: // Ë - Rec1_Z Axial Amplitude
			{
				configRec.amplitude_AxialZ = GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Rec, configRec);
	#ifdef DEBUG
				Serial.print(amplitude_Char);
				Serial.println(configRec.amplitude_AxialZ);
	#endif // DEBUG
				break;
			}
			case 205: // Í - Rec_Z Axial Distance
			{
				configRec.distance_AxialZ = GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Rec, configRec);
	#ifdef DEBUG
				Serial.print(distance_Char);
				Serial.println(configRec.distance_AxialZ);
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
			case 208: // Ð - pageOne: Active Stepper
			{
				configOne.axisId = GetSerialInteger();
				EEPROM.put(eePromAddress_One, configOne);
				break;
			}
			case 209: // Ñ - Reci AxialX and RadialX Axis Speed percentage
			{
				configRec.speedPercent_Axis_X = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Rec, configRec);
	#ifdef DEBUG
				Serial.print(x_SpeedPercent_Char);
				Serial.println(configRec.speedPercent_Axis_X);
	#endif // DEBUG
				break;
			}
			case 210: // Ò - Rec1_Spindle Spindle Speed percentage
			{
				configRec.speedPercent_Spindle = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Rec, configRec);
	#ifdef DEBUG
				Serial.print(spindle_SpeedPercent_Char);
				Serial.println(configRec.speedPercent_Spindle);
	#endif // DEBUG
				break;
			}
			case 211: // Ó - Rec AxialZ and RadialZ Axis Speed percentage 
			{
				configRec.speedPercent_Axis_Z = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Rec, configRec);
	#ifdef DEBUG
				Serial.print(axis_SpeedPercent_Char);
				Serial.println(configRec.speedPercent_Axis_Z);
	#endif // DEBUG
				break;
			}
			case 212: // Ô - Rec Z radial Waves (Count)
			{
				configRec.waves_RadialZ = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Rec, configRec);
	#ifdef DEBUG
				Serial.print(waves_Char);
				Serial.println(configRec.waves_RadialZ);
	#endif // DEBUG
				break;
			}
			case 213: // Õ - Rec Z radial Degrees
			{
				configRec.degrees_RadialZ = GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Rec, configRec);
	#ifdef DEBUG
				Serial.print(degrees_Char);
				Serial.println(configRec.degrees_RadialZ);
	#endif // DEBUG
				break;
			}
			case 214: // Ö - Rec1 Z radial Amplitude Axis
			{
				configRec.amplitude_RadialZ = GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Rec, configRec);
	#ifdef DEBUG
				Serial.print(amplitude_Char);
				Serial.println(configRec.amplitude_RadialZ);
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

				// HIGH = 1, LOW = 0
				polaritySpindle >= 1 ? (configSetup.polarity_Spindle = true) : (configSetup.polarity_Spindle = false);
				EEPROM.put(eePromAddress_Setup, configSetup);
	#ifdef DEBUG
				Serial.print(polarity_Char);
				Serial.println(configSetup.polarity_Spindle);
	#endif // DEBUG
				break;
			}
			case 217: // Ù - Polarity Z (High or Low)
			{
				int polarityZ = GetSerialInteger();

				// true (1): LOW  false (0): HIGH
				polarityZ >= 1 ? (configSetup.polarity_Axis_Z = true) : (configSetup.polarity_Axis_Z = false);
				EEPROM.put(eePromAddress_Setup, configSetup);
				break;
			}
			case 218: // Ú - Polarity X (High or Low)
			{
				int polarityX = GetSerialInteger();

				// true (1): LOW  false (0): HIGH
				polarityX >= 1 ? (configSetup.polarity_Axis_X = true) : (configSetup.polarity_Axis_X = false);
				EEPROM.put(eePromAddress_Setup, configSetup);
				break;
			}
			case 219: // Û - Polarity B (High or Low)
			{
				int polarityB = GetSerialInteger();

				// true (1): LOW  false (0): HIGH
				polarityB >= 1 ? (configSetup.polarity_Axis_B = true) : (configSetup.polarity_Axis_B = false);
				EEPROM.put(eePromAddress_Setup, configSetup);
				break;
			}
			case 220: // Ü - Limit switch Z Min
			{
				configSetup.limit_Min_Z = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Setup, configSetup);
	#ifdef DEBUG
				Serial.print(z_LimitMin_Char);
				Serial.println(configSetup.limit_Min_Z);
	#endif // DEBUG
				break;
			}
			case 221: // Ý - Limit switch Z Max
			{
				configSetup.limit_Max_Z = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Setup, configSetup);
	#ifdef DEBUG
				Serial.print(z_LimitMax_Char);
				Serial.println(configSetup.limit_Max_Z);
	#endif // DEBUG
				break;
			}
			case 222: // Þ - Limit switch X Min
			{
				configSetup.limit_Min_X = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Setup, configSetup);
	#ifdef DEBUG
				Serial.print(x_LimitMin_Char);
				Serial.println(configSetup.limit_Min_X);
	#endif // DEBUG
				break;
			}
			case 223: // ß - Limit switch X Max
			{
				configSetup.limit_Max_X = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Setup, configSetup);
	#ifdef DEBUG
				Serial.print(x_LimitMax_Char);
				Serial.println(configSetup.limit_Max_X);
	#endif // DEBUG
				break;
			}
			case 224: // à - Limit switch B Min
			{
				configSetup.limit_Min_B = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Setup, configSetup);
	#ifdef DEBUG
				Serial.print(b_LimitMin_Char);
				Serial.println(configSetup.limit_Min_B);
	#endif // DEBUG
				break;
			}
			case 225: // á - Limit switch B Max
			{
				configSetup.limit_Max_B = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Setup, configSetup);
	#ifdef DEBUG
				Serial.print(b_LimitMax_Char);
				Serial.println(configSetup.limit_Max_B);
	#endif // DEBUG
				break;
			}
			case 226: // â - Rose: X Axis MaxSpd
			{
				configRose.maxSpd_Axis_X = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Rose, configRose);
	#ifdef DEBUG
				Serial.print(maxSpd_Char);
				Serial.println(configRose.maxSpd_Axis_X);
	#endif // DEBUG
				break;
			}
			case 227: // ã - Rose: X Axis Accel
			{
				configRose.accel_Axis_X = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Rose, configRose);
	#ifdef DEBUG
				Serial.print(accel_Char);
				Serial.println(configRose.accel_Axis_X);
	#endif // DEBUG
				break;
			}
			case 228: // ä - Rose: X Axis Radial Amplitude
			{
				configRose.amplitude_Radial_X= GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Rose, configRose);
	#ifdef DEBUG
				Serial.print(amplitude_Char);
				Serial.println(configRose.amplitude_Radial_X);
	#endif // DEBUG
				break;
			}
			case 229: // å - Rose: Spindle Amplitude
			{
				configRose.amplitude_Axial_Z = GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Rose, configRose);
	#ifdef DEBUG
				Serial.print(amplitude_Char);
				Serial.println(configRose.amplitude_Axial_Z);
	#endif // DEBUG
				break;
			}
			case 230: // å - Greek Key File: Pattern Count/360
			{
				configGreekKey_Main.countPatternFile360 = GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Grk_Main, configGreekKey_Main);
	#ifdef DEBUG
				Serial.print(patternCount_Char);
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
				configSetup.maxSpd_Return_Spindle = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Setup, configSetup);
	#ifdef DEBUG
				Serial.print(maxSpd_Char);
				Serial.println(configSetup.maxSpd_Return_Spindle);
	#endif // DEBUG
				break;
			}
			case 233: // é - Return Spindle Accel
			{
				configSetup.accel_Return_Spindle = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Setup, configSetup);
	#ifdef DEBUG
				Serial.print(accel_Char);
				Serial.println(configSetup.accel_Return_Spindle);
	#endif // DEBUG
				break;
			}
			case 234: // ê - Return Spindle Accel
			{
				configSetup.maxSpd_Return_Axis = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Setup, configSetup);
	#ifdef DEBUG
				Serial.print(maxSpd_Char);
				Serial.println(configSetup.maxSpd_Return_Axis);
	#endif // DEBUG
				break;
			}
			case 235: // ë - Return Spindle Accel
			{
				configSetup.accel_Return_Axis = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Setup, configSetup);
	#ifdef DEBUG
				Serial.print(accel_Char);
				Serial.println(configSetup.accel_Return_Axis);
	#endif // DEBUG
				break;
			}
			case 236: //ì- Clear Stepper positions and set pageCallerId
			{
				returnSteps_Axis = 0;
				returnSteps_Spindle = 0;
				endPosition_Axis = 0;
				endPosition_Spindle = 0;

				pageCallerId = GetSerialInteger();
				
				// Show the Load ini button
				switch (pageCallerId)
				{
					case 1:
					{
						Serial.println ("vis bt0,1");
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
			case 238: // î - Rec axisId
			{
				configRec.axisId = GetSerialInteger();
				EEPROM.put(eePromAddress_Rec, configRec);
				break;
			}
			case 239: // ï - Rec1 AxialX Waves
			{
				configRec.waves_AxialX = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Rec, configRec);
	#ifdef DEBUG
				Serial.print(waves_Char);
				Serial.println(configRec.waves_AxialX);
	#endif // DEBUG
				break;
			}
			case 240: // ð - Rec1 AxialX Amplitude
			{
				configRec.amplitude_AxialX = GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Rec, configRec);
	#ifdef DEBUG
				Serial.print(amplitude_Char);
				Serial.println(configRec.amplitude_AxialX);
	#endif // DEBUG
				break;
			}
			case 241: // ñ - Rec1 AxialX Distance
			{
				configRec.distance_AxialX = GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Rec, configRec);
	#ifdef DEBUG
				Serial.print(distance_Char);
				Serial.println(configRec.distance_AxialX);
	#endif // DEBUG
				break;
			}
			case 242: // ò - Rec1 RadialX Waves
			{
				configRec.waves_RadialX = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Rec, configRec);
	#ifdef DEBUG
				Serial.print(waves_Char);
				Serial.println(configRec.waves_RadialX);
	#endif // DEBUG
				break;
			}
			case 243: // ó - Rec1 RadialX Degrees
			{
				configRec.degrees_RadialX = GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Rec, configRec);
	#ifdef DEBUG
				Serial.print(degrees_Char);
				Serial.println(configRec.degrees_RadialX);
	#endif // DEBUG
				break;
			}
			case 244: // ô - Rec1 RadialX Amplitude
			{
				configRec.amplitude_RadialX = GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Rec, configRec);
	#ifdef DEBUG
				Serial.print(amplitude_Char);
				Serial.println(configRec.amplitude_RadialX);
	#endif // DEBUG
				break;
			}
			case 245: // õ - Rose X Axial Amplitude
			{
				configRose.amplitude_Axial_X = GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Rose, configRose);
	#ifdef DEBUG
				Serial.print(amplitude_Char);
				Serial.println(configRose.amplitude_Axial_X);
	#endif // DEBUG
				break;
			}
			case 246: // ö - IndexId 
			{
				configIndex_Main.indexId = GetSerialInteger();
	#ifdef DEBUG
				Serial.print(indexId_Char);
				Serial.println(configIndex_Main.indexId);
	#endif // DEBUG
				EEPROM.put(eePromAddress_Ind_Main, configIndex_Main);
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
				Serial.print(axisId_Char);
				Serial.println(configGreekKey_Main.axisId);
				Serial.print(pattern_Char);
				Serial.println(configGreekKey_Main.patternId);
				Serial.print(reverseDirection_Char);
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
				Serial.print(maxSpd_Char);
				Serial.println(maxSpeed);
	#endif // DEBUG

				switch (configGreekKey_Main.axisId)
				{
					case 0: // Greek Key Z: Z Axis MaxSpd
					{
						configGreekKey_Z.maxSpd_Spindle = maxSpeed;
						EEPROM.put(eePromAddress_Grk_Z, configGreekKey_Z);
						break;
					}
					case 1: // Greek Key X: X Axis MaxSpd
					{
						configGreekKey_X.maxSpd_Spindle = maxSpeed;
						EEPROM.put(eePromAddress_Grk_X, configGreekKey_X);
						break;
					}
				}

				break;
			}
			case 249: // ù - Do Greek Key Axis Speed Percentage
			{
				int percentage = (int)GetSerialFloat(serialId);

	#ifdef DEBUG
				Serial.print(axisId_Char);
				Serial.println(configGreekKey_Main.axisId);
				Serial.print(axis_SpeedPercent_Char);
				Serial.println(percentage);
	#endif // DEBUG

				if (configGreekKey_Main.axisId == ID_AXIS_Z)
				{
					configGreekKey_Z.speedPercent_Axis = percentage;
					EEPROM.put(eePromAddress_Grk_Z, configGreekKey_Z);
				}
				else
				{
					configGreekKey_X.speedPercent_Axis = percentage;
					EEPROM.put(eePromAddress_Grk_X, configGreekKey_X);
				}

				break;
			}
			case 250: // ù -Greek Key: Axis Accel
			{
				int accel = (int)GetSerialFloat(serialId);
	#ifdef DEBUG
				Serial.print(accel);
				Serial.println(accel);
	#endif // DEBUG

	
				switch (configGreekKey_Main.axisId)
				{
					case ID_AXIS_Z: // Greek Key Z: Z Axis accel
					case 49:
					{
						configGreekKey_Z.accel_Axis = accel;
						EEPROM.put(eePromAddress_Grk_Z, configGreekKey_Z);
						break;
					}
					case ID_AXIS_X: // Greek Key X: Spindle accel
					case 50:
					{
						configGreekKey_X.accel_Axis = accel;
						EEPROM.put(eePromAddress_Grk_X, configGreekKey_X);
						break;
					}
				}
				break;
			}
			case 251: // û - Greek Key Leg Length
			{
				configGreekKey_Main.segmentLengthPattern = GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Grk_Main, configGreekKey_Main);
	#ifdef DEBUG
				Serial.print(segmentLength_Char);
				Serial.println(configGreekKey_Main.segmentLengthPattern);
	#endif // DEBUG

				break;
			}
			case 252: // ü - Greek Key Count/360
			{
				configGreekKey_Main.countPattern360 = GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Grk_Main, configGreekKey_Main);
	#ifdef DEBUG
				Serial.print(patternCount_Char);
				Serial.println(configGreekKey_Main.countPattern360);
	#endif // DEBUG
				break;
			}
			case 253: // ü - Greek Key AxisId
			{
				configGreekKey_Main.axisId = GetSerialInteger();
				EEPROM.put(eePromAddress_Grk_Main, configGreekKey_Main);
				break;
			}
			case 254: // þ - Greek Key Spindle Speed Percentage
			{
				int percentage = (int)GetSerialFloat(serialId);

				if (configGreekKey_Main.axisId == ID_AXIS_Z)
				{
					configGreekKey_Z.speedPercent_Spindle = percentage;
					EEPROM.put(eePromAddress_Grk_Z, configGreekKey_Z);
	#ifdef DEBUG
					Serial.print(z_SpeedPercent_Char);
					Serial.println(configGreekKey_Z.speedPercent_Spindle);
	#endif // DEBUG

				}
				else
				{
					configGreekKey_X.speedPercent_Spindle = percentage;
					EEPROM.put(eePromAddress_Grk_X, configGreekKey_X);
	#ifdef DEBUG
					Serial.print(x_SpeedPercent_Char);
					Serial.println(configGreekKey_X.speedPercent_Spindle);
	#endif // DEBUG
				}

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
		////MilliDelay(200);
		//Serial.println("3.End Switch");
	}

#ifdef DEBUG
	//Serial.println("2.End Loop.");
#endif // DEBUG
} // End loop()