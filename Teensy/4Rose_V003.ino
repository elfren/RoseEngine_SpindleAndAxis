/* *****************************************************************
* TwoRose_V001 main entry
* Author: Edward French
* GT2-5mm / 9:1 gear ratio
******************************************************************/
#include "math.h"
#include <string>
#include <EEPROM.h>
#include "SpindleDefines.h"
#include "AccelStepper.h"
#include "SD.h"
#include "SPI.h"
#include "StepControl.h"

//==================================================================
// Pin assignments
//==================================================================
// See SpindleDefines.h
// Initialize AccelStepper object
AccelStepper stepper_Spindle(AccelStepper::DRIVER, PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
AccelStepper stepper_Axis_Z(AccelStepper::DRIVER, PIN_AXIS_Z_STEP, PIN_AXIS_Z_DIR);
// TeensyStep initialization
Stepper motor_Spindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
Stepper motor_Axis_Z(PIN_AXIS_Z_STEP, PIN_AXIS_Z_DIR);


#ifndef TWO_AXES_V2
Stepper motor_Axis_X(PIN_AXIS_X_STEP, PIN_AXIS_X_DIR);
AccelStepper stepper_Axis_X(AccelStepper::DRIVER, PIN_AXIS_X_STEP, PIN_AXIS_X_DIR);
#endif // TWO_AXES_V2

#ifdef FOUR_AXES
AccelStepper stepper_Axis_B(AccelStepper::DRIVER, PIN_AXIS_B_STEP, PIN_AXIS_B_DIR);
Stepper motor_Axis_B(PIN_AXIS_B_STEP, PIN_AXIS_B_DIR);
#endif // B_AXIS_ENABLED

stepperConfig configSteppers;
StepControl<> controller_Spindle;
StepControl<> controller_Axis;

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
		delay(100);
		digitalWrite(PIN_LED, LOW);
		delay(2000);
	}


#ifdef FOUR_AXES
	// Set Microstepping mode
	//SetMicrosteppingMode(configSteppers.microsteps_Spindle, PIN_SPINDLE_MS0, PIN_SPINDLE_MS1, PIN_SPINDLE_MS2);
	//SetMicrosteppingMode(configSteppers.microsteps_Axis_Z, PIN_AXIS_Z_MS0, PIN_AXIS_Z_MS1, PIN_AXIS_Z_MS2);
	//SetMicrosteppingMode(configSteppers.microsteps_Axis_B, PIN_AXIS_B_MS0, PIN_AXIS_B_MS1, PIN_AXIS_B_MS2);
#endif //FOUR_AXES
#ifndef TWO_AXES_V2
	//SetMicrosteppingMode(configSteppers.microsteps_Axis_X, PIN_AXIS_X_MS0, PIN_AXIS_X_MS1, PIN_AXIS_X_MS2);
#endif // TWO_AXES_V2

	// Set the enable pin for the stepper motor
	stepper_Spindle.setEnablePin(PIN_SPINDLE_ENABLE);

	// Set pins "inverted", HIGH == off
	stepper_Spindle.setPinsInverted(false, false, configSteppers.enable_Spindle);

	// Enable the spindle stepper motor
	stepper_Spindle.disableOutputs();

	// Main page opens by default, so set initial Sp1 microstepping, speed, and acceleration
	stepper_Spindle.setMaxSpeed(configSteppers.maxSpeedSpindle_SpZ);
	stepper_Spindle.setAcceleration(configSteppers.accelerationSpindle_SpZ);

	//-------------------------------
	// Z Axis stepper motor initialization
	// Configure Enable Z Axis stepper
	stepper_Axis_Z.setEnablePin(PIN_AXIS_Z_ENABLE);
	stepper_Axis_Z.setPinsInverted(false, false, configSteppers.enable_Axis_Z);

	// Disable the Z Axis stepper motor (enable in run method)
	stepper_Axis_Z.disableOutputs();

	// Set Z Axis MaxSpeed and Acceleration
	stepper_Axis_Z.setMaxSpeed(configSteppers.maxSpeed_Axis_Z);
	stepper_Axis_Z.setAcceleration(configSteppers.acceleration_Axis_Z);
	//-------------------------------
#ifdef FOUR_AXES
	// B Axis stepper motor initialization
	// Configure Enable B Axis stepper
	stepper_Axis_B.setEnablePin(PIN_AXIS_B_ENABLE);
	stepper_Axis_B.setPinsInverted(false, false, configSteppers.enable_Axis_B);

	// Disable the B Axis stepper motor (enable in run method)
	stepper_Axis_B.disableOutputs();

	// Set B Axis MaxSpeed and Acceleration
	stepper_Axis_B.setMaxSpeed(configSteppers.maxSpeed_Axis_Z);
	stepper_Axis_B.setAcceleration(configSteppers.acceleration_Axis_Z);
	//------------------------------
	motor_Axis_B
		.setMaxSpeed(configSteppers.maxSpeed_Axis_B)       // steps/s
		.setAcceleration(configSteppers.acceleration_Axis_B) // steps/s^2 
		.setStepPinPolarity(LOW); // driver expects active low pulses
#endif // FOUR_AXES
#ifndef TWO_AXES_V2
	// X Axis stepper motor initialization
	// Configure Enable X Axis stepper
	stepper_Axis_X.setEnablePin(PIN_AXIS_X_ENABLE);
	stepper_Axis_X.setPinsInverted(false, false, configSteppers.enable_Axis_X);

	// Disable the X Axis stepper motor (enable in run method)
	stepper_Axis_X.disableOutputs();

	// Set X Axis MaxSpeed and Acceleration
	stepper_Axis_X.setMaxSpeed(configSteppers.maxSpeed_Axis_Z);
	stepper_Axis_X.setAcceleration(configSteppers.acceleration_Axis_Z);
	//------------------------------
	// Configure TeensyStep motors
	motor_Axis_X
		.setMaxSpeed(configSteppers.maxSpeed_Axis_X)       // steps/s
		.setAcceleration(configSteppers.acceleration_Axis_X) // steps/s^2    
		.setStepPinPolarity(LOW); // driver expects active low pulses
#endif // TWO_AXES_V2
	
	// Configure TeensyStep motors

	motor_Spindle
		.setMaxSpeed(configSteppers.maxSpeedSpindle_SpB)       // steps/s
		.setAcceleration(configSteppers.accelerationSpindle_SpB) // steps/s^2 
	    .setStepPinPolarity(LOW); // driver expects active low pulses

	motor_Axis_Z
		.setMaxSpeed(configSteppers.maxSpeed_Axis_Z)       // steps/s
		.setAcceleration(configSteppers.acceleration_Axis_Z) // steps/s^2 
	    .setStepPinPolarity(LOW); // driver expects active low pulses


	// Initialize Limit switches
	pinMode(PIN_LIMIT_MIN, INPUT_PULLUP);
	pinMode(PIN_LIMIT_MAX, INPUT_PULLUP);
	delayMicroseconds(10);

	digitalWrite(PIN_LIMIT_MIN, HIGH);  // Enable
	digitalWrite(PIN_LIMIT_MAX, HIGH);  // Enable
	delayMicroseconds(10);

#ifdef FOUR_AXES
	// Enable microSD card
	if (!(SD.begin(BUILTIN_SDCARD)))
	{
			Serial.println("Unable to access the SD card"); // ToDo: Send to Nextion
			delay(200);
	}
#else
	if (!(SD.begin(PIN_SPI_CS)))
	{
		Serial.println("Unable to access the SD card"); // ToDo: Send to Nextion
		delay(200);
	}
#endif // FOUR_AXES

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
			configSteppers.maxSpeedSpindle_SyncZ = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 59: // ; Semicolon - Sync Spindle Acceleration
		{
			configSteppers.accelerationSpindle_SyncZ = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 60: // < Less than - moveZ_Acceleration
		{
			configSteppers.acceleration_Axis_Z = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 61: // = 
		{
			bool sdStarted = SD.begin(BUILTIN_SDCARD);
			if (!sdStarted)
			{
				// ToDo: Write to Nextion
			}
			break;
		}
		case 62: // > Greater than - Set moveZ_ speed
		{
			configSteppers.speedPercentage_MoveZ = GetSerialInteger();
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 63: // ? Question mark - moveZ_Distance (Text)
		{
			// Set Distance
			float newDistance = GetSerialFloat(serialId);
			configSteppers.distance_MoveZ = newDistance;
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		////case 64: // @ - 
		////{
		////	break;
		////}
		case 65: // A - Enable spindle stepper
		{
			stepper_Spindle.enableOutputs();
			break;
		}
		case 66: //B - Disable spindle stepper
		{
			stepper_Spindle.disableOutputs();
			break;
		}
		//// C: -> Cancel Stop Main/Sp2 Spindle
		//// Implemented in Run methods
		//case 67: // C - 
		//{
		//}
		//break;

		case 68: // D - SyncZ Spindle Speed
		{
			configSteppers.speedPercentageSpindle_SyncZ = GetSerialInteger();
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 69: //E - Index2 by divisions
		{
			double newIndexSize = GetSerialFloat(serialId);
			configSteppers.size_Index2 = (configSteppers.microsteps_Spindle * configSteppers.steps360_Spindle * configSteppers.gearRatio_Spindle) / (newIndexSize);
			configSteppers.type_Index2 = BY_DIVISIONS;
			EEPROM.put(eePromAddress, configSteppers);

			////Serial1.print("pageIndexF2.t3.txt=");
			////Serial1.write(0x22);
			////Serial1.print(configSteppers.size_Index2);
			////Serial1.write(0x22);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			////Serial1.print("pageIndexF2.t3.txt=");
			////Serial1.write(0x22);
			////Serial1.print(configSteppers.size_Index2);
			////Serial1.write(0x22);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			break;
		}
		case 70: // F - Index2 counter clockwise
		{

			Serial1.print("pageIndexF2.t10.txt="); // Nextion may not get the first packet
			Serial1.write(0x22);
			Serial1.print(configSteppers.source_Index2);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(100);
			Serial1.print("pageIndexF2.t10.txt=");
			Serial1.write(0x22);
			Serial1.print(configSteppers.source_Index2);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(100);
			Serial1.print("pageIndexF2.t3.txt=");
			Serial1.write(0x22);
			Serial1.print(configSteppers.size_Index2);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageIndexF2.t3.txt=");
			Serial1.write(0x22);
			Serial1.print(configSteppers.size_Index2);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			if (configSteppers.source_Index2 == FILE_SD)
			{
				//for (int i = 0; i < 3; i++) // Verify Teensy is operational
				//{
				//	digitalWrite(PIN_LED, HIGH);
				//	delay(500);
				//	digitalWrite(PIN_LED, LOW);
				//	delay(500);
				//}
				int lineNumber = GetSerialInteger();
				if (lineNumber == 255)
				{
					lineNumber = 0;
				}
				double newIndexSize = GetDataFromSD(lineNumber);
				configSteppers.size_Index2 = (configSteppers.microsteps_Spindle * configSteppers.steps360_Spindle * configSteppers.gearRatio_Spindle) * (newIndexSize / 360);
				Serial1.print("pageIndexF2.t7.txt="); // Nextion may not get the first packet
				Serial1.write(0x22);
				Serial1.print(newIndexSize);
				Serial1.write(0x22);
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);
				delay(100);
				Serial1.print("pageIndexF2.t7.txt=");
				Serial1.write(0x22);
				Serial1.print(newIndexSize);
				Serial1.write(0x22);
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);
				delay(100);
			}
			else
			{
				digitalWrite(PIN_LED, HIGH);
				delay(500);
				digitalWrite(PIN_LED, LOW);
				delay(500);
			}

			// Check for 0 size.  GetDataFromSD function will return 0 when it reaches end of file.
			if (configSteppers.size_Index2 > 0)
			{
				//DoIndex2();
				IndexSpindle(ID_INDEX_2, DIR_CCW); //ToDo:

			}
			break;
		}
		case 71: // G - Index2 Clockwise
		{
			Serial1.print("pageIndexF2.t10.txt="); // Nextion may not get the first packet
			Serial1.write(0x22);
			Serial1.print(configSteppers.source_Index2);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(100);
			Serial1.print("pageIndexF2.t10.txt=");
			Serial1.write(0x22);
			Serial1.print(configSteppers.source_Index2);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(100);
			Serial1.print("pageIndexF2.t3.txt=");
			Serial1.write(0x22);
			Serial1.print(configSteppers.size_Index2);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageIndexF2.t3.txt=");
			Serial1.write(0x22);
			Serial1.print(configSteppers.size_Index2);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);

			if (configSteppers.source_Index2 == FILE_SD)
			{
				int lineNumber = GetSerialInteger();
				if (lineNumber == 255)
				{
					lineNumber = 0;
				}

				// Get size from file
				digitalWrite(PIN_LED, HIGH);
				delay(500);
				digitalWrite(PIN_LED, LOW);
				delay(500);
				double newIndexSize = GetDataFromSD(lineNumber);
				configSteppers.size_Index2 = (configSteppers.microsteps_Spindle * configSteppers.steps360_Spindle * configSteppers.gearRatio_Spindle) * (newIndexSize / 360);
				Serial1.print("pageIndexF2.t7.txt=");
				Serial1.write(0x22);
				Serial1.print(newIndexSize);
				Serial1.write(0x22);
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);
				delay(300);
				Serial1.print("pageIndexF2.t7.txt=");
				Serial1.write(0x22);
				Serial1.print(newIndexSize);
				Serial1.write(0x22);
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);
				delay(100);
			}

			// Check for 0 size. GetDataFromSD function will return 0 when it reaches end of file.
			if (configSteppers.size_Index2 > 0)
			{
				//DoIndex2();
				IndexSpindle(ID_INDEX_2, DIR_CW); 
			}
			break;
		}
		case 72: // H - Set Index2 speed
		{
			configSteppers.speedPercentage_Index2 = GetSerialInteger();
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 73: // I-SyncZ Helix Type
		{
			configSteppers.helixType_SyncZ = GetSerialInteger();
			EEPROM.put(eePromAddress, configSteppers);
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

			//RunTwoSteppersSyncZ(directionAxis, directionSpindle);
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

			//RunTwoSteppersSyncZ(directionAxis, directionSpindle);
			Sync_SpindleZ(directionSpindle, directionAxis);

			break;

		}
		case 76: // L - Set Index2 type to Degrees
		{
			double newIndexSize = GetSerialFloat(serialId);
			configSteppers.size_Index2 = (configSteppers.microsteps_Spindle * configSteppers.steps360_Spindle * configSteppers.gearRatio_Spindle) * (newIndexSize / 360);
			configSteppers.type_Index2 = BY_DEGREES;
			EEPROM.put(eePromAddress, configSteppers);
			////Serial1.print("pageIndexF2.t3.txt=");
			////Serial1.write(0x22);
			////Serial1.print(configSteppers.size_Index2);
			////Serial1.write(0x22);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			////Serial1.print("pageIndexF2.t3.txt=");
			////Serial1.write(0x22);
			////Serial1.print(configSteppers.size_Index2);
			////Serial1.write(0x22);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			break;
		}
		case 77: // M - Update Index2 size
		{
			double newIndexSize = GetSerialFloat(serialId);

			////Serial1.print("pageIndexF2.t5.txt=");
			////Serial1.write(0x22);
			////Serial1.print(newIndexSize);
			////Serial1.write(0x22);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			////Serial1.print("pageIndexF2.t5.txt=");
			////Serial1.write(0x22);
			////Serial1.print(newIndexSize);
			////Serial1.write(0x22);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			////Serial1.write(0xff);

			// Default: Divisions
			
			if (configSteppers.type_Index2 == BY_DEGREES) // Degrees
			{
				configSteppers.size_Index2 = (configSteppers.microsteps_Spindle * configSteppers.steps360_Spindle * configSteppers.gearRatio_Spindle) * (newIndexSize / 360);
			}
			else
			{
				configSteppers.size_Index2 = (configSteppers.microsteps_Spindle * configSteppers.steps360_Spindle * configSteppers.gearRatio_Spindle) / (newIndexSize);
			}

			EEPROM.put(eePromAddress, configSteppers);
			////Serial1.print("pageIndexF2.t3.txt=");
			////Serial1.write(0x22);
			////Serial1.print(configSteppers.size_Index2);
			////Serial1.write(0x22);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			////Serial1.print("pageIndexF2.t3.txt=");
			////Serial1.write(0x22);
			////Serial1.print(configSteppers.size_Index2);
			////Serial1.write(0x22);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
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
					configSteppers.spindleDir_SyncZ = 0;
					break;
				}
				case 1:
				case 49:
				{
					configSteppers.spindleDir_SyncZ = 1;
					break;
				}
			}

			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 79: // O - Sync Axis Speed
		{
			configSteppers.speedPercentageAxis_SyncZ = GetSerialInteger();
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
			Serial1.print("pageIndex1.t5.txt="); // Nextion may not get the first packet
			Serial1.write(0x22);
			Serial1.print(configSteppers.source_Index1);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(100);
			Serial1.print("pageIndex1.t5.txt="); // Nextion may not get the first packet
			Serial1.write(0x22);
			Serial1.print(configSteppers.source_Index1);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(100);
			Serial1.print("pageIndex1.t10.txt=");
			Serial1.write(0x22);
			Serial1.print(configSteppers.size_Index1);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(100);

			// Check for 0 size
			if (configSteppers.size_Index1 > 0)
			{
				//DoIndex1();
				IndexSpindle(ID_INDEX_1, DIR_CCW);
			}

			Serial1.print("pageIndex1.t3.txt=");
			Serial1.write(0x22);
			Serial1.print(configSteppers.size_Index1);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageIndex1.t3.txt=");
			Serial1.write(0x22);
			Serial1.print(configSteppers.size_Index1);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(100);
			break;
		}
		case 82: // R - Index1 Clockwise
		{
			Serial1.print("pageIndex1.t5.txt="); // Nextion may not get the first packet
			Serial1.write(0x22);
			Serial1.print(configSteppers.source_Index1);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(100);
			Serial1.print("pageIndex1.t5.txt="); // Nextion may not get the first packet
			Serial1.write(0x22);
			Serial1.print(configSteppers.source_Index1);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(100);


			// Check for 0 size
			if (configSteppers.size_Index1 > 0)
			{
				//DoIndex1();
				IndexSpindle(ID_INDEX_1, DIR_CW);
			}

			Serial1.print("pageIndex1.t3.txt=");
			Serial1.write(0x22);
			Serial1.print(configSteppers.size_Index1);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageIndex1.t3.txt=");
			Serial1.write(0x22);
			Serial1.print(configSteppers.size_Index1);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(100);
			break;
		}
		case 83: // S - Set Index1 speed
		{
			configSteppers.speedPercentage_Index1 = GetSerialInteger();
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 84: // T - Index1 MaxSpeed
		{
			configSteppers.maxSpeed_Index = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 85: //U - MoveZ total distance
		{
			// Total distance only valid for current session
			distanceTotal_MoveZ = GetSerialFloat(serialId);

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
					configSteppers.axisDir_SyncZ = 0;
					break;
				}
				case 1:
				case 49:
				{
					configSteppers.axisDir_SyncZ = 1;
					break;
				}
			}

			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 87: // W - Set Index1 type to Degrees
		{
			double newIndexSize = GetSerialFloat(serialId);
			configSteppers.size_Index1 = (configSteppers.microsteps_Spindle * configSteppers.steps360_Spindle * configSteppers.gearRatio_Spindle) * (newIndexSize / 360);
			configSteppers.type_Index1 = BY_DEGREES;
			EEPROM.put(eePromAddress, configSteppers);
			////Serial1.print("pageIndex1.t3.txt=");
			////Serial1.write(0x22);
			////Serial1.print(configSteppers.size_Index1);
			////Serial1.write(0x22);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			////Serial1.print("pageIndex1.t3.txt=");
			////Serial1.write(0x22);
			////Serial1.print(configSteppers.size_Index1);
			////Serial1.write(0x22);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			break;
		}
		case 88:  // X - Update Index1 size
		{
			double newIndexSize = GetSerialFloat(serialId);

			////Serial1.print("pageIndex1.t5.txt=");
			////Serial1.write(0x22);
			////Serial1.print(newIndexSize);
			////Serial1.write(0x22);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			////Serial1.print("pageIndex1.t5.txt=");
			////Serial1.write(0x22);
			////Serial1.print(newIndexSize);
			////Serial1.write(0x22);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			////Serial1.write(0xff);

			// Default: Divisions
			//configSteppers.size_Index1 = (configSteppers.microsteps_Spindle * configSteppers.steps360_Spindle * configSteppers.gearRatio_Spindle) / (newIndexSize);
			if (configSteppers.type_Index1 == BY_DEGREES) // Degrees
			{
				configSteppers.size_Index1 = (configSteppers.microsteps_Spindle * configSteppers.steps360_Spindle * configSteppers.gearRatio_Spindle) * (newIndexSize / 360);
			}
			else
			{
				configSteppers.size_Index1 = (configSteppers.microsteps_Spindle * configSteppers.steps360_Spindle * configSteppers.gearRatio_Spindle) / (newIndexSize);
			}

			EEPROM.put(eePromAddress, configSteppers);
			////Serial1.print("pageIndex1.t3.txt=");
			////Serial1.write(0x22);
			////Serial1.print(configSteppers.size_Index1);
			////Serial1.write(0x22);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			////Serial1.print("pageIndex1.t3.txt=");
			////Serial1.write(0x22);
			////Serial1.print(configSteppers.size_Index1);
			////Serial1.write(0x22);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			break;
		}
		case 89:  // Y - Index1 Acceleration
		{
			configSteppers.acceleration_Index = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 90:  // Z - Main Axis Clockwise
		{
			// Get current axis speed 
			configSteppers.speedPercentage_Axis_Z = GetSerialInteger();
			EEPROM.put(eePromAddress, configSteppers);
			RunTwoSteppers_SpZ(
				DIR_CW,
				DIR_CW,
				0);
			break;

		}
		case 91:  // [ - Z Axis Counterclockwise
		{
			// Set Speed
			configSteppers.speedPercentage_Axis_Z = GetSerialInteger();
			EEPROM.put(eePromAddress, configSteppers);

			// Run
			stepper_Axis_Z.enableOutputs();
			RunTwoSteppers_SpZ(
				DIR_CCW,
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
		case 94: // ^ - Enable Z axis stepper
		{
			int axisId = GetSerialInteger();
			switch (axisId)
			{
			case 1:
			{
				stepper_Axis_Z.enableOutputs();
				//digitalWrite(PIN_AXIS_Z_ENABLE, LOW);  // Enable
				break;
			}
			case 2:
			{
#ifndef TWO_AXES_V2
				stepper_Axis_X.enableOutputs();
#endif // TWO_AXES_V2
				break;
			}
			case 3:
			{
#ifdef FOUR_AXES
				stepper_Axis_B.enableOutputs();
				//digitalWrite(PIN_AXIS_B_ENABLE, LOW);  // Enable
#endif // FOUR_AXES
				break;
			}
			break;
			}
		}
		case 95: //_ - Disable Z axis stepper
		{
			int axisId = GetSerialInteger();
			switch (axisId)
			{
			case 1:
			{
				stepper_Axis_Z.disableOutputs();
				//digitalWrite(PIN_AXIS_Z_ENABLE, HIGH);  // Enable
				break;
			}
			case 2:
			{
#ifndef TWO_AXES_V2
				stepper_Axis_X.disableOutputs();
#endif // TWO_AXES_V2
				break;

			}
			case 3:
			{
#ifdef FOUR_AXES
				stepper_Axis_B.disableOutputs();
				//digitalWrite(PIN_AXIS_B_ENABLE, HIGH);  // Enable
#endif // FOUR_AXES
				break;
			}
			break;
			}
		}
		//case 96: // ' - *** Don't Use***
		//{
		//	break;
		//}
		case 97: // a - Z Axis Speed
		{
			configSteppers.speedPercentage_Axis_Z = GetSerialInteger();
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 98: // b - Test EEPROM settings Setup screen
		{
			TestEEPROMSetup();
			break;
		}
		case 99: // c - Z Axis Stop
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
			EEPROM.put(eePromAddress, configSteppers);

			// Run
			RunStepper(DIR_CW, stepper_Spindle, configSteppers.acceleration_Sp2, configSteppers.speedPercentage_Sp2, configSteppers.maxSpeed_Sp2);
			break;
		}
		case 101: // e - Sp2 CounterClockwise
		{
			// Set Speed
			configSteppers.speedPercentage_Sp2 = GetSerialInteger();
			EEPROM.put(eePromAddress, configSteppers);

			// Run
			RunStepper(DIR_CCW, stepper_Spindle, configSteppers.acceleration_Sp2, configSteppers.speedPercentage_Sp2, configSteppers.maxSpeed_Sp2);
			break;
		}
		case 102: // f - Set Sp2 Speed
		{
			configSteppers.speedPercentage_Sp2 = GetSerialInteger();
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 103: // g - Sp2 MaxSpeed
		{
			configSteppers.maxSpeed_Sp2 = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		////case 104: // h - 
		////{
		////	break;
		////}
		case 105: // i - Sp2 Acceleration
		{
			configSteppers.acceleration_Sp2 = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 106: // j - Move Z counter clockwise
		{
			configSteppers.speedPercentage_MoveZ = GetSerialInteger();
			EEPROM.put(eePromAddress, configSteppers);

			// Run
			MoveAxisZ(DIR_CCW);
			break;
		}
		case 107: // k - Z Spindle Clockwise
		{
			// Set Speed
			configSteppers.speedPercentageSpindle_SpZ = GetSerialInteger();
			stepper_Spindle.setCurrentPosition(0);
			EEPROM.put(eePromAddress, configSteppers);

			RunTwoSteppers_SpZ(
				DIR_CW,
				DIR_CW,
				3);
			break;
		}
		case 108: // l - Z spindle counter clockwise
		{
			// Set Speed
			configSteppers.speedPercentageSpindle_SpZ = GetSerialInteger();
			stepper_Spindle.setCurrentPosition(0);
			EEPROM.put(eePromAddress, configSteppers);

			// Run
			RunTwoSteppers_SpZ(
				DIR_CCW,
				DIR_CCW,
				3);

			break;
		}
		case 109: // m - Z/Sync Spindle Acceleration
		{
			configSteppers.accelerationSpindle_SpZ = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 110: // n - Set SpZ (Main) Spindle Speed
		{
			configSteppers.speedPercentageSpindle_SpZ = GetSerialInteger();
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 111: // o - SpZ/SpX/SpB/Sync Spindle MaxSpeed
		{
			configSteppers.maxSpeedSpindle_SpZ = GetSerialFloat(serialId);
			configSteppers.maxSpeedSpindle_SyncZ = configSteppers.maxSpeedSpindle_SpZ;
			configSteppers.maxSpeedSpindle_SpX = configSteppers.maxSpeedSpindle_SpZ;
			configSteppers.maxSpeedSpindle_SpB = configSteppers.maxSpeedSpindle_SpZ;
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 112: // p - Z/Sync Spindle Microsteps
		{
			configSteppers.microsteps_Spindle = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			//SetMicrosteppingMode(configSteppers.microsteps_Spindle, PIN_SPINDLE_MS0, PIN_SPINDLE_MS1, PIN_SPINDLE_MS2);
			break;
		}
		case 113: // q - Move Z clockwise
		{
			configSteppers.speedPercentage_MoveZ = GetSerialInteger();
			EEPROM.put(eePromAddress, configSteppers);

			// Run
			MoveAxisZ(DIR_CW);
			break;
		}
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
#ifdef FOUR_AXES
		case 116: // t - B axis Acceleration
		{
			configSteppers.acceleration_Axis_B = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 117: // u - B axis Max Speed
		{
			configSteppers.maxSpeed_Axis_B = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 118: // v - B axis microsteps
		{
			configSteppers.microsteps_Axis_B = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			//SetMicrosteppingMode(configSteppers.microsteps_Axis_B, PIN_AXIS_B_MS0, PIN_AXIS_B_MS1, PIN_AXIS_B_MS2);
			break;
		}
		case 119: // w - B axis Steps/360 
		{
			configSteppers.steps360_Axis_B = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 120: // x - B axis Gear Ratio
		{
			configSteppers.gearRatio_AxisB = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
#endif // FOUR_AXES
		case 121: // y - Z Axis Microsteps
		{
			configSteppers.microsteps_Axis_Z = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			//SetMicrosteppingMode(configSteppers.microsteps_Axis_Z, PIN_AXIS_Z_MS0, PIN_AXIS_Z_MS1, PIN_AXIS_Z_MS2);
			break;
		}

		case 122: // z - Z Axis FullSteps
		{
			configSteppers.steps360_Axis_Z = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 123: // { - Z Distance per revolution
		{
			configSteppers.distancePerRev_AxisZ = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 124: // | - Z Axis Acceleration
		{
			configSteppers.acceleration_Axis_Z = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 125: // } - Z Axis MaxSpeed
		{
			configSteppers.maxSpeed_Axis_Z = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 126: // ~ - Test EEPROM settings Config screen
		{
			TestEEPROMConfig();
			break;
		}
#ifndef TWO_AXES_V2
		case 161: // ¡ - Sp-X spindle speed
		{
			configSteppers.speedPercentageSpindle_SpX = GetSerialInteger();
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 162: // ¢ - Sp-X axis speed
		{
			configSteppers.speedPercentage_Axis_X = GetSerialInteger();
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 163: // £ - Sp-X spindle CCW
		{
			// Set Speed
			configSteppers.speedPercentageSpindle_SpX = GetSerialInteger();
			stepper_Spindle.setCurrentPosition(0);
			EEPROM.put(eePromAddress, configSteppers);

			// ToDo:
			RunTwoSteppers_SpX(
				DIR_CCW,
				DIR_CCW,
				3);
			break;
		}
		case 164: // ¤ - Sp-X spindle CW
		{
			// Set Speed
			configSteppers.speedPercentageSpindle_SpX = GetSerialInteger();
			stepper_Spindle.setCurrentPosition(0);
			EEPROM.put(eePromAddress, configSteppers);

			// ToDo:
			RunTwoSteppers_SpX(
				DIR_CW,
				DIR_CW,
				3);
			break;
		}
		case 165: // ¥ - Sp-X Axis CCW
		{
			// Set Speed
			configSteppers.speedPercentage_Axis_X = GetSerialInteger();
			EEPROM.put(eePromAddress, configSteppers);

			// Run
			stepper_Axis_X.enableOutputs();
			RunTwoSteppers_SpX(
				DIR_CCW,
				DIR_CW,
				0);
			break;
		}
		case 166: // ¦ - Sp-X Axis CW
		{
			// Set Speed
			configSteppers.speedPercentage_Axis_X = GetSerialInteger();
			EEPROM.put(eePromAddress, configSteppers);

			// Run
			stepper_Axis_X.enableOutputs();
			RunTwoSteppers_SpX(
				DIR_CW,
				DIR_CW,
				0);
			break;
		}
#endif //TWO_AXES_V2
#ifdef FOUR_AXES

		case 167: // § - Sp-B spindle speed
		{
			configSteppers.speedPercentageSpindle_SpB = GetSerialInteger();
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 168: // ¨ - Sp-B axis speed
		{
			configSteppers.speedPercentage_Axis_B = GetSerialInteger();
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 169: // © - Sp-B spindle CCW
		{
			// Set Speed
			configSteppers.speedPercentageSpindle_SpB = GetSerialInteger();
			stepper_Spindle.setCurrentPosition(0);
			EEPROM.put(eePromAddress, configSteppers);

			RunTwoSteppers_SpB(
				DIR_CCW,
				DIR_CCW,
				3);
			break;
		}
		case 170: // ª - Sp-B spindle CW
		{
			// Set Speed
			configSteppers.speedPercentageSpindle_SpB = GetSerialInteger();
			stepper_Spindle.setCurrentPosition(0);
			EEPROM.put(eePromAddress, configSteppers);

			RunTwoSteppers_SpB(
				DIR_CW,
				DIR_CW,
				3);
			break;
		}
		case 171: // « - Sp-B Axis CCW
		{
			// Set Speed
			configSteppers.speedPercentage_Axis_B = GetSerialInteger();
			EEPROM.put(eePromAddress, configSteppers);

			// Run
			stepper_Axis_B.enableOutputs();
			RunTwoSteppers_SpB(
				DIR_CCW,
				DIR_CW,
				0);
			break;
		}
		case 172: // ¬ - Sp-B Axis CW
		{
			// Set Speed
			configSteppers.speedPercentage_Axis_B = GetSerialInteger();
			EEPROM.put(eePromAddress, configSteppers);

			// Run
			stepper_Axis_B.enableOutputs();
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
#endif // FOUR_AXES
#ifndef TWO_AXES_V2
		case 174: // ® - SyncX axis speed
		{
			configSteppers.speedPercentageAxis_SyncX = GetSerialInteger();
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 175: // ¯ - SyncX spindle speed
		{
			configSteppers.speedPercentageSpindle_SyncX = GetSerialInteger();
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 176: // ° - SyncX Helix direction
		{
			configSteppers.helixType_SyncX = GetSerialInteger();
			EEPROM.put(eePromAddress, configSteppers);
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
			//RunTwoSteppersSyncX(directionAxis, directionSpindle);
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
			//RunTwoSteppersSyncX(directionAxis, directionSpindle);
			break;
		}
		case 179: // ³ - MvX Speed 
		{
			configSteppers.speedPercentage_MoveX = GetSerialInteger();
			EEPROM.put(eePromAddress, configSteppers);
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
			configSteppers.distance_MoveX = newDistance;
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 182: // ¶ - MvX Total
		{
			// Total distance only valid for current session
			distanceTotal_MoveX = GetSerialFloat(serialId);

			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 183: // · - MvX CCW
		{
			configSteppers.speedPercentage_MoveX = GetSerialInteger();
			EEPROM.put(eePromAddress, configSteppers);

			// Run
			MoveAxisX(DIR_CCW);
			break;
		}
		case 184: // ¸ - MvX CW
		{
			configSteppers.speedPercentage_MoveX = GetSerialInteger();
			EEPROM.put(eePromAddress, configSteppers);

			// Run
			MoveAxisX(DIR_CW);
			break;
		}
		case 185: // ¹ - X Max speed
		{
			configSteppers.maxSpeed_Axis_X = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 186: // º - X Accel
		{
			configSteppers.acceleration_Axis_X = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 187: // » - X microsteps
		{
			configSteppers.microsteps_Axis_X = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			//SetMicrosteppingMode(configSteppers.microsteps_Axis_X, PIN_AXIS_X_MS0, PIN_AXIS_X_MS1, PIN_AXIS_X_MS2);
			break;
		}
		case 188: // ¼ - X Full Steps
		{
			configSteppers.steps360_Axis_X = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 189: // ½ - X Distance per revolution (Text)
		{
			configSteppers.distancePerRev_AxisX = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
#endif // TWO_AXES_V2
		case 190: // ¾ - Index1 Source:Fixed
		{
			//double newIndexSize = GetSerialFloat(serialId);
			//configSteppers.size_Index1 = (configSteppers.microsteps_Spindle * configSteppers.steps360_Spindle * configSteppers.gearRatio_Spindle) * (newIndexSize / 360);
			configSteppers.source_Index1 = FIXED;
			EEPROM.put(eePromAddress, configSteppers);
			////Serial1.print("pageIndex1.t3.txt=");
			////Serial1.write(0x22);
			////Serial1.print(configSteppers.size_Index1);
			////Serial1.write(0x22);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			////Serial1.print("pageIndex1.t3.txt=");
			////Serial1.write(0x22);
			////Serial1.print(configSteppers.size_Index1);
			////Serial1.write(0x22);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			break;
		}
		case 191: // ¿ - Index1 Source: Filename
		{
			filename_Index1 = GetSerial1String();
			configSteppers.source_Index1 = FILE_SD;

			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 192: // À - Revolutions_SyncZ
		{
			configSteppers.revolutionsSyncZ_Spindle = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 193: // Á - Distance_SyncZ
		{
			configSteppers.distanceSyncZ = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 194: // À - Revolutions_SyncX
		{
			configSteppers.revolutionsSyncX_Spindle = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 195: // Á - Distance_SyncZ
		{
			configSteppers.distanceSyncX = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 196: // Ä - Index2_SourceFixed
		{
			configSteppers.source_Index2 = FIXED;
			EEPROM.put(eePromAddress, configSteppers);
			////Serial1.print("pageIndexF2.t3.txt=");
			////Serial1.write(0x22);
			////Serial1.print(configSteppers.size_Index2);
			////Serial1.write(0x22);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			////Serial1.print("pageIndexF2.t3.txt=");
			////Serial1.write(0x22);
			////Serial1.print(configSteppers.size_Index2);
			////Serial1.write(0x22);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			break;
		}
		case 197: // Å - Index2_SourceFile
		{
		filename_Index2 = GetSerial1String();
		configSteppers.source_Index2 = FILE_SD;

		EEPROM.put(eePromAddress, configSteppers);
		break;
		}
		case 198: // Æ - Do Recip1_Z 
		{
			
			int waveDir = GetSerialFloat(serialId);

			////Serial1.print("pageRecip1_Z.t8.txt=");
			////Serial1.write(0x22);
			////Serial1.print(waveDir);
			////Serial1.write(0x22);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			////Serial1.print("pageRecip1_Z.t8.txt=");
			////Serial1.write(0x22);
			////Serial1.print(waveDir);
			////Serial1.write(0x22);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			// In: -1  Out: 1
			if (waveDir != 1)
			{
				waveDir = -1;
			}
			DoRecip1_Z(waveDir);
			break;
		}
		case 199: // Ç - Recip1_Z Spindle Acceleration
		{
			configSteppers.acceleration_Recip1_Z_Spindle = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 200: // È - Recip1_Z Axis Acceleration
		{
			configSteppers.acceleration_Recip1_Z_Axis = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 201: // É - Recip1_Z Spindle MaxSpeed 
		{
			configSteppers.maxSpeed_Recip1_Z_Spindle = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 202: // Ê - Recip1_Z Axis MaxSpeed 
		{
			configSteppers.maxSpeed_Recip1_Z_Axis = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 203: // Ì - Recip1_Z Repeats (Count)
		{
			configSteppers.repeats_Recip1_Z = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 204: // Ë - Recip1_Z Distance
		{
			configSteppers.distance_Recip1_Z = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}

		case 205: // Í - Recip1_Z Amplitude
		{
			configSteppers.amplitude_Recip1_Z = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 206: // Î - Recip1_Z Return
		{
			Return_Recip1_Z();
			break;
		}
		// ---------Recip1_SpZ
		case 207: // Ï - Do Recip1_SpZ 
		{

			int waveDir = GetSerialFloat(serialId);

			//Serial1.print("pageRecip1_Sp.t8.txt=");
			//Serial1.write(0x22);
			//Serial1.print(waveDir);
			//Serial1.write(0x22);
			//Serial1.write(0xff);
			//Serial1.write(0xff);
			//Serial1.write(0xff);
			//Serial1.print("pageRecip1_Sp.t8.txt=");
			//Serial1.write(0x22);
			//Serial1.print(waveDir);
			//Serial1.write(0x22);
			//Serial1.write(0xff);
			//Serial1.write(0xff);
			//Serial1.write(0xff);
			// In: -1  Out: 1
			if (waveDir != 1)
			{
				waveDir = -1;
			}
			DoRecip1_SpZ(waveDir);
			break;
		}
		case 208: // Ð - Recip1_SpZ Spindle Acceleration
		{
			configSteppers.acceleration_Recip1_SpZ_Spindle = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 209: // Ñ - Recip1_SpZ Axis Acceleration
		{
			configSteppers.acceleration_Recip1_SpZ_Axis = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 210: // Ò - Recip1_SpZ Spindle MaxSpeed 
		{
			configSteppers.maxSpeed_Recip1_SpZ_Spindle = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 211: // Ó - Recip1_SpZ Axis MaxSpeed 
		{
			configSteppers.maxSpeed_Recip1_SpZ_Axis = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 212: // Ô - Recip1_SpZ Repeats (Count)
		{
			configSteppers.repeats_Recip1_SpZ = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 213: // Õ - Recip1_SpZ Distance
		{
			configSteppers.degrees_Recip1_SpZ = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}

		case 214: // Ö - Recip1_SpZ Amplitude
		{
			configSteppers.amplitude_Recip1_SpZ = GetSerialFloat(serialId);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 215: // × - Recip1_SpZ Return
		{
			Return_Recip1_SpZ();
			break;
		}


		case 216: // Ø - Enable Spindle (High or Low)
		{
			int enableSpindle = GetSerialInteger();
			enableSpindle >= 1 ? (configSteppers.enable_Spindle = true) : (configSteppers.enable_Spindle = false);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 217: // Ù - Enable Z (High or Low)
		{
			int enableZ = GetSerialInteger();
			enableZ >= 1 ? (configSteppers.enable_Axis_Z = true) : (configSteppers.enable_Axis_Z = false);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 218: // Ú - Enable X (High or Low)
		{
			int enableX = GetSerialInteger();
			enableX >= 1 ? (configSteppers.enable_Axis_X = true) : (configSteppers.enable_Axis_X = false);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		case 219: // Û - Enable B (High or Low)
		{
			int enableB = GetSerialInteger();

			//Serial1.print("pageSetup.t19.txt=");
			//Serial1.write(0x22);
			//Serial1.print("Z Axis-Enable:");
			//Serial1.print(enableB);
			//Serial1.write(0x22);
			//Serial1.write(0xff);
			//Serial1.write(0xff);
			//Serial1.write(0xff);
			//delay(2000);
			//Serial1.print("pageSetup.t19.txt=");
			//Serial1.write(0x22);
			//Serial1.print("Z Axis-Enable:");
			//Serial1.print(enableB);
			//Serial1.write(0x22);
			//Serial1.write(0xff);
			//Serial1.write(0xff);
			//Serial1.write(0xff);
			//delay(2000);
			enableB >= 1 ? (configSteppers.enable_Axis_B = true) : (configSteppers.enable_Axis_B = false);
			EEPROM.put(eePromAddress, configSteppers);
			break;
		}
		default:
			SerialPrint("Input:");
			SerialPrintLn("N/A");
		}
		delay(500);
	}
} // End loop()