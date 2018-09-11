/* *****************************************************************
* TwoRose_V001 main entry 
* Author: Edward French
* GT2-5mm / 9:1 gear ratio
******************************************************************/
#include "math.h"
#include <string>
#include <EEPROM.h>
#include "SpindleDefines_b.h"
#include "AccelStepper.h"

//==================================================================
// Pin assignments
//==================================================================
// See SpindleDefines.h
// Initialize AccelStepper object
AccelStepper stepper_Spindle(AccelStepper::DRIVER, PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
AccelStepper stepper_Axis(AccelStepper::DRIVER, PIN_AXIS_STEP, PIN_AXIS_DIR);

stepperConfig configSteppers;

/// <summary>
/// Setup
/// </summary>
/// <comment>
/// Initial setup
/// </comment>
/// <returns></returns>
void setup()
{
	// Initialize Serial0 (USB port)
	Serial.begin(9600); 

	// Initialize Nextion LCD for 115200 baud
	// Note: Nextion requires 3 0xFF bytes to signal end of transmission
	// First run must run at 9600.  Once the Nextion has been programmed for 115200, uncomment 115200 line and comment 9600 line.
	//Serial1.begin(9600);  
	Serial1.begin(115200); //Nextion Serial
	Serial1.print("bauds=115200");
	Serial1.write(0xFF);
	Serial1.write(0xFF);
	Serial1.write(0xFF);
	Serial1.print("bkcmd=0");  // Set Nextion to return NO replies to each command
	Serial1.write(0xFF);
	Serial1.write(0xFF);
	Serial1.write(0xFF);

	// Update with values from EEProm
	EEPROM.get(eePromAddress, configSteppers);

	// Config as well as all other EEProm settings must be run from Nextion whenever Teensy is updated.  
	// EEProm may contain invalid settings otherwise.

	pinMode(PIN_LED, OUTPUT); 
	for (int i = 0; i < 3; i++) // Verify Teensy is operational
	{
		digitalWrite(PIN_LED, HIGH);
		delay(500);
		digitalWrite(PIN_LED, LOW);
		delay(500);
	}

	// Enable spindle stepper
	pinMode(PIN_SPINDLE_ENABLE, OUTPUT);   // Enable
	digitalWrite(PIN_SPINDLE_ENABLE, LOW);  // Enable

	// Set the enable pin for the stepper motor
	stepper_Spindle.setEnablePin(PIN_SPINDLE_ENABLE);

	// Set pins "inverted", HIGH == off
	stepper_Spindle.setPinsInverted(false, true, true);

	// Enable the spindle stepper motor
	stepper_Spindle.enableOutputs();

	// Main page opens by default, so set initial Sp1 microstepping, speed, and acceleration
	stepper_Spindle.setMaxSpeed(configSteppers.maxSpeedSpindle_Main);
	stepper_Spindle.setAcceleration(configSteppers.accelerationSpindle_Main);

	// Axis stepper motor initialization
	pinMode(PIN_AXIS_ENABLE, OUTPUT);   // Enable
	digitalWrite(PIN_AXIS_ENABLE, LOW);  // Enable

	// Enable Axis stepper
	stepper_Axis.setEnablePin(PIN_AXIS_ENABLE);
	stepper_Axis.setPinsInverted(false, true, true);

	// Enable the Axis stepper motor (enable in run method)
	//stepper_Axis.enableOutputs();

	// Set Axis MaxSpeed and Acceleration
	stepper_Axis.setMaxSpeed(configSteppers.maxSpeed_Axis);
	stepper_Axis.setAcceleration(configSteppers.acceleration_Axis);

	// Initialize Limit switches
		pinMode(PIN_AXIS_LIMIT_MIN, INPUT_PULLUP);
		pinMode(PIN_AXIS_LIMIT_MAX, INPUT_PULLUP);
		delayMicroseconds(10);

	// Wait for pins to be pulled high
	delayMicroseconds(10);
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
		serialId = 0;
	}
	else if (Serial1.available() > 0)
	{
		serialId = 1;
	}

	// All Nextion incoming data packets are terminated with one 0xFF byte
	if (serialId <9) // If serial data is available, parse the data
	{
		incomingByte = SerialRead(serialId);
		delay(200);

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
			configSteppers.maxSpeedSpindle_Sync = GetSerialFloat(serialId);
			stepper_Spindle.setMaxSpeed(configSteppers.maxSpeedSpindle_Sync);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 59: // ; Semicolon - 
		{
			configSteppers.accelerationSpindle_Sync = GetSerialFloat(serialId);
			stepper_Spindle.setAcceleration(configSteppers.accelerationSpindle_Sync);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		////case 60: // < Less than - 
		////{
		////	break;
		////}
		////case 61: // = - 
		////{
		////	break;
		////}
		////case 62: // > Greater than - 
		////{
		////	break;
		////}
		////case 63: // ? Question mark - 
		////{
		////	break;
		////}
		////case 64: // @ - 
		////{
		////	break;
		////}
		case 65: // A - Enable spindle stepper
		{
			////digitalWrite(PIN_SPINDLE_ENABLE, LOW);  // Enable
			stepper_Spindle.enableOutputs();
			break;
		}
		case 66: //B - Disable spindle stepper
		{
			////digitalWrite(PIN_SPINDLE_ENABLE, HIGH);  // Disable
			stepper_Spindle.disableOutputs();
			break;
		}
		//// C: -> Cancel Stop Main/Sp2 Spindle
		//// Implemented in Run methods
		//case 67: // C - 
		//{
		//}
		//break;

		case 68: // D - Sync Spindle Speed
		{
			configSteppers.speedPercentageSpindle_Sync = GetSerialInteger();
			float nextSpeed = configSteppers.speedPercentageSpindle_Sync * configSteppers.maxSpeedSpindle_Sync * .01;
			stepper_Spindle.setMaxSpeed(nextSpeed);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 69: //E - Index2 by divisions
		{
			double newIndexSize = GetSerialFloat(serialId);
			configSteppers.size_Index2 = (configSteppers.microsteps_Spindle * configSteppers.steps360_Spindle * configSteppers.gearRatio_Spindle) / (newIndexSize);
			configSteppers.type_Index2 = BY_DIVISIONS;
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 70: // F - Index2 counter clockwise
		{
			moveDirection = DIR_CCW;
			DoIndex2();
			break;
		}
		case 71: // G - Index2 Clockwise
		{
			moveDirection = DIR_CW;
			DoIndex2();
			break;
		}
		case 72: // H - Set Index2 speed
		{
			configSteppers.speedPercentage_Index2 = GetSerialInteger();
			float nextSpeed = configSteppers.speedPercentage_Index2 * configSteppers.maxSpeed_Index * .01;
			stepper_Spindle.setMaxSpeed(nextSpeed);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 73: // I-Sync2 Helix Type
		{
			configSteppers.helixType_Sync = GetSerialInteger();
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 74: // J - Sync2 In
		{
			int helixType = GetSerialInteger();
			int directionSpindle = 1;
			int directionAxis = 1;

			switch (helixType)
			{
			case 0: // Left
			case 48:
			{
				directionAxis = 0; // CCW
				directionSpindle = 0; //CCW
				break;
			}
			case 1: //Right
			case 49:
			{
				directionAxis = 0; // CCW
				directionSpindle = 1; //CW
				break;
			}

			}
			Serial1.print("pageSync.t3.txt=");
			Serial1.write(0x22);
			Serial1.print(directionSpindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageSync.t3.txt=");
			Serial1.write(0x22);
			Serial1.print(directionSpindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageSync.t4.txt=");
			Serial1.write(0x22);
			Serial1.print(directionAxis);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			RunTwoSteppersSync(directionAxis, directionSpindle);
			break;
		}
		case 75: // K - Sync2 Out 
		{
			int helixType = GetSerialInteger();
			int directionSpindle = 1;
			int directionAxis = 1;


			switch (helixType)
			{
				case 0:// Left
				case 48:
				{
					directionAxis = 1; // CW
					directionSpindle = 1; // CW
					break;
				}
				case 1: //Right
				case 49:
				{
					directionAxis = 1; // CW
					directionSpindle = 0; // CCW
					break;
				}
			}

			Serial1.print("pageSync.t3.txt=");
			Serial1.write(0x22);
			Serial1.print(directionSpindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageSync.t3.txt=");
			Serial1.write(0x22);
			Serial1.print(directionSpindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageSync.t4.txt=");
			Serial1.write(0x22);
			Serial1.print(directionAxis);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			RunTwoSteppersSync( directionAxis, directionSpindle);
			break;
			
		}
		case 76: // L - Set Index2 type to Degrees
		{
			double newIndexSize = GetSerialFloat(serialId);
			configSteppers.size_Index2 = (configSteppers.microsteps_Spindle * configSteppers.steps360_Spindle * configSteppers.gearRatio_Spindle) * (newIndexSize / 360);
			configSteppers.type_Index2 = BY_DEGREES;
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 77: // M - Update Index2 size
		{
			double newIndexSize = GetSerialFloat(serialId);

			// Default: Divisions
			configSteppers.size_Index2 = (configSteppers.microsteps_Spindle * configSteppers.steps360_Spindle * configSteppers.gearRatio_Spindle) / (newIndexSize);
			if (configSteppers.type_Index2 == BY_DEGREES) // Degrees
			{
				configSteppers.size_Index2 = configSteppers.size_Index2 / 360;
			}

			EEPROM.put(eePromAddress, configSteppers);
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
				configSteppers.spindleDir_Sync = 0;
				break;
			}
			case 1:
			case 49:
			{
				configSteppers.spindleDir_Sync = 1;
				break;
			}

			}

			EEPROM.put(eePromAddress, configSteppers);

			//Serial1.print("pageSync.t5.txt=");
			//Serial1.write(0x22);
			//Serial1.print(configSteppers.spindleDir_Sync);
			//Serial1.write(0x22);
			//Serial1.write(0xff);
			//Serial1.write(0xff);
			//Serial1.write(0xff);
			//delay(2000);
			break;
		}
		case 79: // O - Sync Axis Speed
		{
			configSteppers.speedPercentageAxis_Sync = GetSerialInteger();
			float nextSpeed = configSteppers.speedPercentageAxis_Sync * configSteppers.maxSpeedSpindle_Sync * .01;
			configSteppers.speedPercentage_Axis = configSteppers.speedPercentageAxis_Sync;
			
			stepper_Axis.setMaxSpeed(nextSpeed);
			EEPROM.put(eePromAddress, configSteppers);

			break;
		}
		case 80: // P - Index1 by divisions
		{
			double newIndexSize = GetSerialFloat(serialId);
			configSteppers.size_Index1 = (configSteppers.microsteps_Spindle * configSteppers.steps360_Spindle * configSteppers.gearRatio_Spindle) / (newIndexSize);
			configSteppers.type_Index1 = BY_DIVISIONS;
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 81: // Q - Index1 counter clockwise
			{
				moveDirection = DIR_CCW;
				DoIndex1();
				break;
			}
		case 82: // R - Index1 Clockwise
			{
				moveDirection = DIR_CW;
				DoIndex1();
				break;
			}
		case 83: // S - Set Index1 speed
		{
			configSteppers.speedPercentage_Index1 = GetSerialInteger();
			float nextSpeed = configSteppers.speedPercentage_Index1 * configSteppers.maxSpeed_Index * .01;
			stepper_Spindle.setMaxSpeed(nextSpeed);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 84: // T - Index1 MaxSpeed
		{
			configSteppers.maxSpeed_Index = GetSerialFloat(serialId);
			stepper_Spindle.setMaxSpeed(configSteppers.maxSpeed_Index);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 85: //U - Index1 Microsteps
		{
			configSteppers.microsteps_Spindle = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
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
				configSteppers.axisDir_Sync = 0;
				break;
			}
			case 1:
			case 49:
			{
				configSteppers.axisDir_Sync = 1;
				break;
			}

			}
			//configSteppers.axisDir_Sync = newDir;
			EEPROM.put(eePromAddress, configSteppers);

			//Serial1.print("pageSync.t9.txt=");
			//Serial1.write(0x22);
			//Serial1.print(newDir);
			//Serial1.write(0x22);
			//Serial1.write(0xff);
			//Serial1.write(0xff);
			//Serial1.write(0xff);
			//delay(2000);
			break;
		}
		case 87: // W - Set Index1 type to Degrees
		{
			double newIndexSize = GetSerialFloat(serialId);
			configSteppers.size_Index1 = (configSteppers.microsteps_Spindle * configSteppers.steps360_Spindle * configSteppers.gearRatio_Spindle) * (newIndexSize / 360);
			configSteppers.type_Index1 = BY_DEGREES;
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 88:  // X - Update Index1 size
		{
			double newIndexSize = GetSerialFloat(serialId);

			// Default: Divisions
			configSteppers.size_Index1 = (configSteppers.microsteps_Spindle * configSteppers.steps360_Spindle * configSteppers.gearRatio_Spindle) / (newIndexSize);
			if (configSteppers.type_Index1 == BY_DEGREES) // Degrees
			{
				configSteppers.size_Index1 = configSteppers.size_Index1 / 360;
			}

			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 89:  // Y - Index1 Acceleration
		{
			configSteppers.acceleration_Index = GetSerialFloat(serialId);
			stepper_Spindle.setAcceleration(configSteppers.acceleration_Index);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 90:  // Z - Main Axis Clockwise
		{
			
			// Get current axis speed 
			int speedPercentage_Axis = GetSerialInteger();
			configSteppers.speedPercentage_Axis = speedPercentage_Axis;
			
			EEPROM.put(eePromAddress, configSteppers);
			RunTwoSteppersMain(
				DIR_CW,
				0,
				DIR_CW,
				0);
			break;

		}
		case 91:  // [ - Main Axis Counterclockwise
		{
			// Set Speed
			configSteppers.speedPercentage_Axis = GetSerialInteger();
			float nextSpeed = configSteppers.speedPercentage_Axis * configSteppers.maxSpeed_Axis * .01;
			stepper_Axis.setMaxSpeed(nextSpeed);
			EEPROM.put(eePromAddress, configSteppers);

			// Run
			stepper_Axis.enableOutputs();
			RunTwoSteppersMain(
				DIR_CCW,
				2,
				DIR_CW,
				0);

			break;
		}
		//case 92: // \ - *** Don't Use***
		//{
		//	break;
		//}
		//case 93: // ] - Sync Stop
		//{
		//	Implemented in Run methods
		//	break;
		//}
		case 94: // ^ - Enable axis stepper
		{
			////digitalWrite(PIN_AXIS_ENABLE, LOW);
			stepper_Axis.enableOutputs();
			break;
		}
		case 95: //_ - Disable axis stepper
		{
			digitalWrite(PIN_AXIS_ENABLE, HIGH);
			stepper_Axis.enableOutputs();
			break;
		}
		//case 96: // ' - *** Don't Use***
		//{
		//	break;
		//}
		case 97: // a - Main Axis Speed
		{
			// ToDo:
			configSteppers.speedPercentage_Axis = GetSerialInteger();
			float newSpeed = configSteppers.speedPercentage_Axis * configSteppers.maxSpeed_Axis * .01;
			stepper_Axis.setMaxSpeed(newSpeed);

			EEPROM.put(eePromAddress, configSteppers);
			break;
		}

		////case 98: // b - 
		////{
		////	break;
		////}
		case 99: // c - Main Axis Stop
		{
			Serial1.print("pageMain.va2.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			break;
		}
		case 100: // d - Sp2 Clockwise
		{
			// Set Speed
			configSteppers.speedPercentage_Sp2 = GetSerialInteger();
			float nextSpeed = configSteppers.speedPercentage_Sp2 * configSteppers.maxSpeed_Sp2 * .01;
			stepper_Spindle.setMaxSpeed(nextSpeed);
			EEPROM.put(eePromAddress, configSteppers);

			// Run
			RunStepper(DIR_CW, stepper_Spindle, configSteppers.acceleration_Sp2, configSteppers.speedPercentage_Sp2, configSteppers.maxSpeed_Sp2);
			break;
		}
		case 101: // e - Sp2 CounterClockwise
		{
			// Set Speed
			configSteppers.speedPercentage_Sp2 = GetSerialInteger();
			float nextSpeed = configSteppers.speedPercentage_Sp2 * configSteppers.maxSpeed_Sp2 * .01;
			stepper_Spindle.setMaxSpeed(nextSpeed);
			EEPROM.put(eePromAddress, configSteppers);

			// Run
			RunStepper(DIR_CCW, stepper_Spindle, configSteppers.acceleration_Sp2, configSteppers.speedPercentage_Sp2, configSteppers.maxSpeed_Sp2);
			break;
		}
		case 102: // f - Set Sp2 Speed
		{
			configSteppers.speedPercentage_Sp2 = GetSerialInteger();
			float nextSpeed = configSteppers.speedPercentage_Sp2 * configSteppers.maxSpeed_Sp2 * .01;
			stepper_Spindle.setMaxSpeed(nextSpeed);
			EEPROM.put(eePromAddress, configSteppers);

			// // Uncomment for debugging (add new t3 control to Nextion).
			//Serial1.print("pageFast.t3.txt=");
			//Serial1.write(0x22);
			//Serial1.print(fastSpeedPercentage);
			//Serial1.write(0x22);
			//Serial1.write(0xff);
			//Serial1.write(0xff);
			//Serial1.write(0xff);
			break;
		}
		case 103: // g - Sp2 MaxSpeed
		{
			configSteppers.maxSpeed_Sp2 = GetSerialFloat(serialId);
			stepper_Spindle.setMaxSpeed(configSteppers.maxSpeed_Sp2);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 104: // h - Sp2 Microsteps
		{
			configSteppers.microsteps_Spindle = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 105: // i - Sp2 Acceleration
		{
			configSteppers.acceleration_Sp2 = GetSerialFloat(serialId);
			stepper_Spindle.setAcceleration(configSteppers.acceleration_Sp2);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		////case 106: // j - 
		////{
		////	break;
		////}
		case 107: // k - Main Spindle Clockwise
		{
			// Set Speed
			configSteppers.speedPercentageSpindle_Main = GetSerialInteger();
			float nextSpeed = configSteppers.speedPercentageSpindle_Main * configSteppers.maxSpeedSpindle_Main * .01;
			stepper_Spindle.setMaxSpeed(nextSpeed);
			stepper_Spindle.setCurrentPosition(0);
			EEPROM.put(eePromAddress, configSteppers);

			RunTwoSteppersMain(
				DIR_CW,
				0,
				DIR_CW,
				3);
			break;
		}
		case 108: // l - Main spindle counter clockwise
		{
			// Set Speed
			configSteppers.speedPercentageSpindle_Main = GetSerialInteger();
			float nextSpeed = configSteppers.speedPercentageSpindle_Main * configSteppers.maxSpeedSpindle_Main * .01;
			stepper_Spindle.setMaxSpeed(nextSpeed);
			stepper_Spindle.setCurrentPosition(0);
			EEPROM.put(eePromAddress, configSteppers);

			// Run
			RunTwoSteppersMain(
				DIR_CCW,
				0,
				DIR_CCW,
				3);

			break;
		}
		case 109: // m - Main/Sync Spindle Acceleration
		{
			configSteppers.accelerationSpindle_Main = GetSerialFloat(serialId);
			stepper_Spindle.setAcceleration(configSteppers.accelerationSpindle_Main);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 110: // n - Set Main Speed
		{
			configSteppers.speedPercentageSpindle_Main = GetSerialInteger();
			float nextSpeed = configSteppers.speedPercentageSpindle_Main * configSteppers.maxSpeedSpindle_Main * .01;
			stepper_Spindle.setMaxSpeed(nextSpeed);
			EEPROM.put(eePromAddress, configSteppers);

			// // Uncomment for debugging (add new t3 control to Nextion).
			//Serial1.print("pageSlow.t3.txt=");
			//Serial1.write(0x22);
			//Serial1.print(slowSpeedPercentage);
			//Serial1.write(0x22);
			//Serial1.write(0xff);
			//Serial1.write(0xff);
			//Serial1.write(0xff);
			break;
		}
		case 111: // o - Main/Sync Spindle MaxSpeed
		{
			configSteppers.maxSpeedSpindle_Main = GetSerialFloat(serialId);
			stepper_Spindle.setMaxSpeed(configSteppers.maxSpeedSpindle_Main);
			configSteppers.maxSpeedSpindle_Sync = configSteppers.maxSpeedSpindle_Main;
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 112: // p - Main/Sync Spindle Microsteps
		{
			configSteppers.microsteps_Spindle = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		////case 113: // q - 
		////{
		////	break;
		////}
		case 114: // r - Main/Sync/Sp2 Spindle FullSteps
		{
			configSteppers.steps360_Spindle = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 115: // s - Main/Sync/Sp2 Spindle Gear Ratio
		{
			configSteppers.gearRatio_Spindle = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		//case 116: // t - 
		//{
		//	break;
		//}
		//case 117: // u - 
		//{
		//	break;
		//}
		//case 118: // v - 
		//{
		//	break;
		//}
		//case 119: // w - 
		//{
		//	break;
		//}
		//case 120: // x - 
		//{
		//	break;
		//}
		case 121: // y - Axis Microsteps
		{
			configSteppers.microsteps_Axis = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 122: // z - Axis FullSteps
		{
			configSteppers.steps360_Axis = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 123: // { - Axis gearRatio
		{
			configSteppers.gearRatio_Axis = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 124: // | - Axis Acceleration
		{
			configSteppers.acceleration_Axis = GetSerialFloat(serialId);
			stepper_Axis.setAcceleration(configSteppers.acceleration_Axis);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 125: // } - Axis MaxSpeed
		{
			configSteppers.maxSpeed_Axis = GetSerialFloat(serialId);
			stepper_Axis.setMaxSpeed(configSteppers.maxSpeed_Axis);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 126: // ~ - Get Config info
		{
			// Test EEPROM settings
			TestEEPROMConfig();
			break;
		}
		default:
			SerialPrint("Input:");
			SerialPrintLn("N/A");
		}
		delay(500);
	}
} // End loop()