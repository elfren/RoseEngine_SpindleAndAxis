/* *****************************************************************
* Rose Engine utility methods
* Author: Edward French
******************************************************************/
// ================================================================================= 
// Utility Methods
// ================================================================================= 

int freeMemory()
{
	char top;
#ifdef __arm__
	return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
	return &top - __brkval;
#else  // __arm__
	return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}

void SetEnable(int axisId, bool enabled)
{
#ifdef DEBUG
	Serial.println("Enter SetEnable.");
	Serial.print("axisId:");
	Serial.println(axisId);
	Serial.print("enabled:");
	Serial.println(enabled);
#endif // DEBUG

	switch (axisId)
	{
		case ID_SPINDLE:
		{
			if (enabled)
			{
				digitalWrite(PIN_SPINDLE_ENABLE, configSetup.polarity_Spindle ? (LOW) : (HIGH)); // Enable 
				//digitalWrite(PIN_SPINDLE_ENABLE, LOW); // Enable 
			}
			else
			{
				digitalWrite(PIN_SPINDLE_ENABLE, configSetup.polarity_Spindle ? (HIGH) : (LOW)); // Disable 
				//digitalWrite(PIN_SPINDLE_ENABLE, HIGH); // Disable 
			}
			break;
		}
		case ID_AXIS_Z:
		{
			if (enabled)
			{
				digitalWrite(PIN_AXIS_Z_ENABLE, configSetup.polarity_Axis_Z ? (LOW) : (HIGH)); // Enable 
				//digitalWrite(PIN_AXIS_Z_ENABLE, LOW);// Enable 
			}
			else
			{
				digitalWrite(PIN_AXIS_Z_ENABLE, configSetup.polarity_Axis_Z ? (HIGH) : (LOW)); // Disable 
				//digitalWrite(PIN_AXIS_Z_ENABLE, HIGH);// Disable 
			}
			break;
		}
		case ID_AXIS_X:
		{
			if (enabled)
			{
				digitalWrite(PIN_AXIS_X_ENABLE, configSetup.polarity_Axis_X ? (LOW) : (HIGH)); // Enable 
				//digitalWrite(PIN_AXIS_X_ENABLE, LOW);// Enable 
			}
			else
			{
				digitalWrite(PIN_AXIS_X_ENABLE, configSetup.polarity_Axis_X ? (HIGH) : (LOW)); // Disable 
				//digitalWrite(PIN_AXIS_X_ENABLE, HIGH);// Disable 
			}
			break;
		}
		case ID_AXIS_B:
		{
			if (enabled)
			{
				digitalWrite(PIN_AXIS_B_ENABLE, configSetup.polarity_Axis_B ? (LOW) : (HIGH)); // Enable 
				//digitalWrite(PIN_AXIS_B_ENABLE, LOW);// Enable 
			}
			else
			{
				digitalWrite(PIN_AXIS_B_ENABLE, configSetup.polarity_Axis_B ? (HIGH) : (LOW)); // Disable 
				//digitalWrite(PIN_AXIS_B_ENABLE, HIGH);// Disable 
			}
			break;
		}
	}
#ifdef DEBUG
	Serial.println("Exit SetEnable.");
#endif // DEBUG
}

/// <summary>
/// Determines which Serial input to use when getting a float from input
/// </summary>
/// <comment>
/// </comment>
/// <param name="serialId">Id of serial input</param>
/// <returns>Float from serial input</returns>
double GetSerialFloat(int serialPortId)
{
	// ToDo: Auto select serial ports
	double retVal =-1;

	switch (serialPortId)
	{
		case 0:
		{
			retVal = GetSerialFloatA(Serial);
			break;
		}
		case 1:
		{
			retVal = GetSerialFloatA(Serial3);
			break;
		}
		case 3:
		{

			retVal = GetSerialFloatA(Serial3);
			break;
		}
	}

#ifdef DEBUG
	Serial.print("FloatA: ");
	Serial.println(retVal);
#endif // DEBUG


	return retVal;
}

/// <summary>
/// Get a float from USB serial input
/// </summary>
/// <returns>Float from USB serial input</returns>
double GetSerial0Float()
{
	const int bufferLength = 10;
	char in[bufferLength];
	int decimalFound = -1;
	int endFound = -1;
	int i;
	for (i = 0; i <= bufferLength; i++) 
	{
		in[i] = Serial.read();

		// Use 255 as end chars
		if (in[i] == 255)
		{
			endFound = i;
			break;
		}
		if (in[i] == 46)
		{
			decimalFound = i;
		}
	}

	return GetFloatFromCharArray(in, decimalFound, endFound);
}

/// <summary>
/// Get a float from Bluetooth serial input (Serial3)
/// </summary>
/// <returns>Float from Bluetooth serial input</returns>
double GetSerial1Float()
{
	const int bufferLength = 10;
	char in[bufferLength];
	int decimalFound = -1;
	int endFound = -1;
	int i;
	for (i = 0; i <= bufferLength; i++)
	{
		in[i] = Serial3.read();

		// Use 255 as end chars
		if (in[i] == 255)
		{
			endFound = i;
			while (Serial3.available() > 0)
			{
				Serial3.read();
			}
			break;
		}
		if (in[i] == 46)
		{
			decimalFound = i;
		}
	}

	return GetFloatFromCharArray(in, decimalFound,endFound);
}

/// <summary>
/// Get a float from serial input (Serial3)
/// </summary>
/// <returns>Float from serial input</returns>
double GetSerialFloatA(Stream &port )
{
	const int bufferLength = 10;
	char in[bufferLength];
	int decimalFound = -1;
	int endFound = -1;
	int i;
	for (i = 0; i <= bufferLength; i++)
	{
		in[i] = port.read();

		// Use 255 as end chars
		if (in[i] == 255)
		{
			endFound = i;
			while (port.available() > 0)
			{
				port.read();
			}
			break;
		}
		if (in[i] == 46)
		{
			decimalFound = i;
		}
	}

	return GetFloatFromCharArray(in, decimalFound, endFound);
}

/// <summary>
/// Parse serial input for new integer value
/// </summary>
/// <comment>
/// </comment>
/// <returns></returns>
int GetSerialInteger()
{
	char inputHex;

	// Value will be 0 to 100
	inputHex = Serial3.read();

	int newInteger = (int)inputHex ;
	while (Serial3.available() > 0)
	{
		// Discard remaining serial chars
		Serial3.read();
	}

	Serial.print("Int:");
	Serial.println(newInteger);
	return newInteger;
}

/// <summary>
/// Parse serial input for new integer value
/// </summary>
/// <comment>
/// </comment>
/// <returns></returns>
int GetSerialIntegerOnly()
{
	char inputHex;

	// Value will be 0 to 100
	inputHex = Serial3.read();

	int newInteger = (int)inputHex;

	return newInteger;
}

/// <summary>
/// Convert Char array to float (double)
/// </summary>
/// <comment>
/// </comment>
/// <param name="in">Char array</param>
/// <param name="decimalFound">Decimal place</param>
/// <param name="endFound">Length of array</param>
/// <returns>Double(float)</returns>
double GetFloatFromCharArray(char in[], int decimalFound, int endFound)
{
	int j;
	double inputNumber = 0;
	int integerPart = 0;
	for (j = 0; j < endFound; j++)
	{
		// Build the number
		if (j != decimalFound)
		{
			integerPart = in[j] - '0';
			inputNumber = (inputNumber * 10) + integerPart;
		}
	}

	// Set the decimal place
	int decimalPlace = 0;
	if (decimalFound > -1)
	{
		decimalPlace = endFound - 1 - decimalFound;
		int k;
		for (k = 0; k < decimalPlace; k++)
		{
			inputNumber = (inputNumber / 10);
		}
	}

#ifdef DEBUG
	Serial.print("Float:");
	Serial.println(inputNumber);
#endif //DEBUG
	return inputNumber;
}

/// <summary>
/// StepsToDegrees_Spindle
/// </summary>
/// <comment>
/// Convert spindle steps to degrees
/// </comment>
/// <param name="steps">Steps</param>
/// <returns></returns>
float StepsToDegrees_Spindle(float steps)
{
	float retVal = (steps / (configSetup.steps360_Spindle * configSetup.microsteps_Spindle* configSetup.gearRatio_Spindle)) * 360;
	return retVal;
}

/// <summary>
/// StepsToDistance_Axis
/// </summary>
/// <comment>
/// Convert axis steps to distance
/// </comment>
/// <param name="steps">Steps</param>
/// <param name="steps">Id of axis</param>
/// <returns></returns>
float StepsToDistance_Axis(float steps, int axisId)
{
	float retVal = 0;
	switch (axisId)
	{
		case ID_AXIS_Z:
		{
			retVal = (steps * configSetup.distancePerRev_AxisZ) / (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z);
			break;
		}
		case ID_AXIS_X:
		{
			retVal = (steps * configSetup.distancePerRev_AxisX) / (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X);
			break;
		}
	}
	return retVal;
}

/// <summary>
/// Move axis
/// </summary>
/// <comment>
/// </comment>
/// <param name="axisId">Id of axis</param>
/// <param name="directionAxis">Counterclockwise: -1, Clockwise: 1</param>
/// <returns></returns>
void MoveAxis(int axisId, int directionAxis)
{
	StepControl stepController;
	Stepper stepper_X(PIN_AXIS_X_STEP, PIN_AXIS_X_DIR);
	Stepper stepper_Z(PIN_AXIS_Z_STEP, PIN_AXIS_Z_DIR);
	double stepsToMove = 0;
	float nextSpeed = 0;

	switch (axisId)
	{
		case ID_AXIS_Z:
		{
			stepper_Z.setPosition(0);
			stepsToMove = (configMove.distance_MoveZ / (configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z));

			// Set speed and acceleration
			nextSpeed = configMove.speedPercent_Axis_Z * configMove.maxSpd_Axis_Z * .01;
			stepper_Z
				.setMaxSpeed(nextSpeed)
				.setAcceleration(configMove.accel_Axis_Z)
				.setTargetRel(stepsToMove * directionAxis);

#ifdef DEBUG
			Serial.print("distancePerRev_AxisZ:");
			Serial.println(configSetup.distancePerRev_AxisZ);
			Serial.print("steps360_Axis_Z:");
			Serial.println(configSetup.steps360_Axis_Z);
			Serial.print("microsteps_Axis_Z:");
			Serial.println(configSetup.microsteps_Axis_Z);
			Serial.print("distance_MoveZ:");
			Serial.println(configMove.distance_MoveZ);

			Serial.print("axisSteps:");
			Serial.println(stepsToMove);

			Serial.print("AxisMaxSpd:");
			Serial.println(configMove.maxSpd_Axis_Z);
			Serial.print("speedPercent_MoveZ:");
			Serial.println(configMove.speedPercent_Axis_Z);
			Serial.print("nextSpeed:");
			Serial.println(nextSpeed);
			Serial.print("AxisAccel:");
			Serial.println(configMove.accel_Axis_Z);
#endif // DEBUG

			SetEnable(ID_AXIS_Z, true);
			stepController.moveAsync(stepper_Z);
			break;
		}
#ifdef FOUR_AXES
		case ID_AXIS_X:
		{
			stepsToMove = (configMove.distance_MoveX / (configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X));
			stepper_X.setPosition(0);

			// Set speed and acceleration
			nextSpeed = configMove.speedPercent_Axis_X * configMove.maxSpd_Axis_X * .01;
			stepper_X
				.setMaxSpeed(nextSpeed)
				.setAcceleration(configMove.accel_Axis_X)
				.setTargetRel(stepsToMove * directionAxis);

#ifdef DEBUG
			Serial.print("distancePerRev_AxisX:");
			Serial.println(configSetup.distancePerRev_AxisX);
			Serial.print("steps360_Axis_X:");
			Serial.println(configSetup.steps360_Axis_X);
			Serial.print("microsteps_Axis_X:");
			Serial.println(configSetup.microsteps_Axis_X);
			Serial.print("distance_MoveX:");
			Serial.println(configMove.distance_MoveX);

			Serial.print("axisSteps:");
			Serial.println(stepsToMove);

			Serial.print("AxisMaxSpd:");
			Serial.println(configMove.maxSpd_Axis_X);
			Serial.print("speedPercent_MoveX:");
			Serial.println(configMove.speedPercent_Axis_X);
			Serial.print("nextSpeed:");
			Serial.println(nextSpeed);
			Serial.print("AxisAccel:");
			Serial.println(configMove.accel_Axis_X);
#endif // DEBUG

			SetEnable(ID_AXIS_X, true);
			stepController.moveAsync(stepper_X);
			break;
		}
#endif // FOUR_AXES
	}

#ifdef DEBUG
	int iCounter = 0;
#endif // DEBUG

	while (stepController.isRunning())
	{
#ifdef SHOW_POSITION 
		// Track Positions not implemented while steppers are running
		//if (configGeneral.trackPositions == 1)
		//{
		//	switch (axisId)
		//	{
		//		case ID_AXIS_Z:
		//		{
		//			endPosition_Axis = stepper_Z.getPosition();
		//			distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);

		//			Serial3.print("pageBE.t2.txt=");
		//			Serial3.write(0x22);
		//			Serial3.print(distance_Axis);
		//			Serial3.write(0x22);
		//			Serial3.write(0xff);
		//			Serial3.write(0xff);
		//			Serial3.write(0xff);
		//			delay(5);
		//			Serial3.print("pageBE.t2.txt=");
		//			Serial3.write(0x22);
		//			Serial3.print(distance_Axis);
		//			Serial3.write(0x22);
		//			Serial3.write(0xff);
		//			Serial3.write(0xff);
		//			Serial3.write(0xff);
		//			break;
		//		}
		//		case ID_AXIS_X:
		//		{
		//			endPosition_Axis = stepper_X.getPosition();
		//			distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);
		//			Serial3.print("pageBE.t2.txt=");
		//			Serial3.write(0x22);
		//			Serial3.print(distance_Axis);
		//			Serial3.write(0x22);
		//			Serial3.write(0xff);
		//			Serial3.write(0xff);
		//			Serial3.write(0xff);
		//			delay(5);
		//			Serial3.print("pageBE.t2.txt=");
		//			Serial3.write(0x22);
		//			Serial3.print(distance_Axis);
		//			Serial3.write(0x22);
		//			Serial3.write(0xff);
		//			Serial3.write(0xff);
		//			Serial3.write(0xff);
		//			break;
		//		}
		//	}
		//}
#endif // Show Position

		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
				case 67:
				case 99: // - c
				{
					stepController.stop();
					break;
				}
			}
		}

		delay(10);
#ifdef DEBUG

		if (iCounter % 50 == 0)
		{
			switch (axisId)
			{
				case ID_AXIS_Z:
				{
					endPosition_Axis = stepper_Z.getPosition();
					Serial.print("Z.position:");
					break;
				}
				case ID_AXIS_X:
				{
					endPosition_Axis = stepper_X.getPosition();
					Serial.print("X.position:");
					break;
				}
			}

			Serial.println(endPosition_Axis);
		}

		iCounter++;
#endif // DEBUG
	}
	// Update Move flag on Nextion. 
	Serial3.print("pageMove.bt2.pco=59391");// The first one may be ignored by Nextion
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageMove.va0.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageMove.bt1.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageMove.bt2.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageMove.bt3.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	switch (axisId)
	{
		case ID_AXIS_Z:
		{
			Serial3.print("pageMove.t6.txt=");
			Serial3.write(0x22);
			Serial3.print(distanceTotal_MoveZ);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);

			endPosition_Axis = stepper_Z.getPosition();
#ifdef DEBUG
			Serial.print("Z.position:");
			Serial.println(endPosition_Axis);
#endif // DEBUG

#ifdef SHOW_POSITION 
			distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);
			Serial3.print("pageBE.t2.txt=");
			Serial3.write(0x22);
			Serial3.print(distance_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(5);
			Serial3.print("pageBE.t2.txt=");
			Serial3.write(0x22);
			Serial3.print(distance_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(5);
#endif // Show Position
			endPosition_Spindle = 0;

			SetEnable(ID_AXIS_Z, false);
			break;
		}
#ifdef FOUR_AXES
		case ID_AXIS_X:
		{
			Serial3.print("pageMoveX.t6.txt=");
			Serial3.write(0x22);
			Serial3.print(distanceTotal_MoveZ);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);

			endPosition_Axis = stepper_X.getPosition();
			Serial.print("X.position:");
			Serial.println(endPosition_Axis);

#ifdef SHOW_POSITION 
			distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);
			Serial3.print("pageBE.t2.txt=");
			Serial3.write(0x22);
			Serial3.print(distance_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(5);
			Serial3.print("pageBE.t2.txt=");
			Serial3.write(0x22);
			Serial3.print(distance_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(5);
#endif // Show Position
			endPosition_Spindle = 0;

			SetEnable(ID_AXIS_X, false);
			break;
		}
#endif // FOUR_AXES
	}

	returnSteps_Axis = endPosition_Axis;
	endPosition_Axis = 0;
}

/// <summary>
/// RunOneStepper
/// </summary>
/// <comment>
/// </comment>
/// <param name="direction">Counterclockwise: -1, Clockwise: 1</param>
/// <returns></returns>
//void RunOneStepper_11b(int direction) // pageOne
//{
//	//v011
//#ifdef DEBUG
//	Serial.println("Enter RunOneStepper.");
//#endif // DEBUG
//
//	int stepPin = PIN_AXIS_Z_STEP;
//	int dirPin = PIN_AXIS_Z_DIR;
//
//	int limitPin_Max = configSetup.limit_Max_Z;
//	int limitPin_Min = configSetup.limit_Min_Z;
//
//	//int polarity = LOW;
//	int maxSpd = 0;
//	int accel = 0;
//
//	float speedPercent = 0;
//	float currentSpeedPercent = 0;
//	int32_t targetPosition = 2000000000;// *direction;
//
//	switch (configOne.activeAxis)
//	{
//	case ID_AXIS_Z:
//	{
//		stepPin = PIN_AXIS_Z_STEP;
//		dirPin = PIN_AXIS_Z_DIR;
//		//enablePin = PIN_AXIS_Z_ENABLE;
//		//polarity = configSetup.polarity_Axis_Z ? (LOW) : (HIGH);
//		maxSpd = configOne.maxSpd_Axis_Z;
//		accel = configOne.accel_Axis_Z;
//		speedPercent = configOne.speedPercent_Axis_Z;
//		currentSpeedPercent = (speedPercent) * .01 *direction;
//		limitPin_Max = configSetup.limit_Max_Z;
//		limitPin_Min = configSetup.limit_Min_Z;
//
//		break;
//	}
//	case ID_AXIS_X:
//	{
//		stepPin = PIN_AXIS_X_STEP;
//		dirPin = PIN_AXIS_X_DIR;
//		//polarity = configSetup.polarity_Axis_X ? (LOW) : (HIGH);
//		maxSpd = configOne.maxSpd_Axis_X;
//		accel = configOne.accel_Axis_X;
//		speedPercent = configOne.speedPercent_Axis_X;
//		currentSpeedPercent = speedPercent * .01 * direction;
//		limitPin_Max = configSetup.limit_Max_X;
//		limitPin_Min = configSetup.limit_Min_X;
//		break;
//	}
//	case ID_AXIS_B:
//	{
//		stepPin = PIN_AXIS_B_STEP;
//		dirPin = PIN_AXIS_B_DIR;
//		//polarity = configSetup.polarity_Axis_B ? (LOW) : (HIGH);
//		maxSpd = configOne.maxSpd_Axis_B;
//		accel = configOne.accel_Axis_B;
//		speedPercent = configOne.speedPercent_Axis_B;
//		currentSpeedPercent = speedPercent * .01 * direction;
//		limitPin_Max = configSetup.limit_Max_B;
//		limitPin_Min = configSetup.limit_Min_B;
//		break;
//	}
//	case ID_SPINDLE:
//	{
//		stepPin = PIN_SPINDLE_STEP;
//		dirPin = PIN_SPINDLE_DIR;
//		//polarity = configSetup.polarity_Spindle ? (LOW) : (HIGH);
//		maxSpd = configOne.maxSpd_Spindle;
//		accel = configOne.accel_Spindle;
//		speedPercent = configOne.speedPercent_Spindle;
//		currentSpeedPercent = speedPercent * .01 * direction;
//
//		break;
//	}
//	}
//
//#ifdef DEBUG
//	Serial.print("MaxSpd:");
//	Serial.println(maxSpd);
//	Serial.print("accel:");
//	Serial.println(accel);
//	Serial.print("speedPercent:");
//	Serial.println(speedPercent);
//	Serial.print("currentSpeedPercent:");
//	Serial.println(currentSpeedPercent);
//#endif // DEBUG
//	Stepper stepper_One(stepPin, dirPin);
//	stepper_One.setPosition(0);
//	delay(5);
//	stepper_One
//		.setMaxSpeed(maxSpd)
//		.setAcceleration(accel)
//		.setTargetAbs(targetPosition);
//
//#ifdef DEBUG
//	Serial.print("configSp.activeAxis:");
//	Serial.println(configOne.activeAxis);
//	Serial.print("direction:");
//	Serial.println(direction);
//	Serial.print("polarity:");
//	//Serial.println(polarity);
//	Serial.print("maxSpd:");
//	Serial.println(maxSpd);
//	Serial.print("accel:");
//	Serial.println(accel);
//	Serial.print("SpeedPercent:");
//	Serial.println(speedPercent);
//	Serial.print("currentSpeedPercent:");
//	Serial.println(currentSpeedPercent);
//	Serial.print("targetPosition:");
//	Serial.println(targetPosition);
//	Serial.println("+++++++++++++++++++++++++++++++");
//	Serial.print("getPosition:");
//	Serial.println(stepper_One.getPosition());
//#endif // DEBUG
//
//	SetEnable(configOne.activeAxis, true);
//#ifdef DEBUG
//	Serial.print("Call rotateAsync");
//#endif
//	rotateController1.rotateAsync(stepper_One);
//#ifdef DEBUG
//	Serial.print("Call overrideSpeed");
//#endif
//	rotateController1.overrideSpeed(currentSpeedPercent);
//	while (rotateController1.isRunning())
//	{
//		// Check for Cancel code  
//		if (SerialAvailable() > 0)
//		{
//			incomingByte = SerialRead(serialId);
//			switch (incomingByte)
//			{
//			case 67: // - C
//			{
//#ifdef DEBUG
//				Serial.println("call overrideSpeed:");
//#endif // DEBUG
//				rotateController1.overrideSpeed(0);
//				delay(5);
//#ifdef DEBUG
//				Serial.println("Call Stop:");
//#endif // DEBUG
//				//rotateController1.stop();
//				rotateController1.stop();
//				delay(100);
//#ifdef DEBUG
//				Serial.println("Stopped:");
//#endif // DEBUG
//				goto endLoop;
//				break;
//			}
//			case 102: // Speed
//			{
//				int newSpeedPercent = (int)GetSerialFloat(serialId);
//				switch (configOne.activeAxis)
//				{
//				case ID_AXIS_Z:
//				{
//					configOne.speedPercent_Axis_Z = newSpeedPercent;
//					currentSpeedPercent = float((newSpeedPercent) * .01 * direction);
//					break;
//				}
//				case ID_AXIS_X:
//				{
//					configOne.speedPercent_Axis_X = newSpeedPercent;
//					currentSpeedPercent = (float)(newSpeedPercent * .01 * direction);
//					break;
//				}
//				case ID_AXIS_B:
//				{
//					configOne.speedPercent_Axis_B = newSpeedPercent;
//					currentSpeedPercent = (float)(newSpeedPercent * .01 * direction);
//					break;
//				}
//				case ID_SPINDLE:
//				{
//					configOne.speedPercent_Spindle = newSpeedPercent;
//					currentSpeedPercent = (float)(newSpeedPercent * .01 * direction);
//					break;
//				}
//				}
//
//				EEPROM.put(eePromAddress_pageOne, configOne);
//
//				rotateController1.overrideSpeed(currentSpeedPercent); // set new speed
//				break;
//			}
//			default:
//			{
//				break;
//			}
//			}
//		}
//
//		if (digitalRead(limitPin_Max) == LOW)
//		{
//			rotateController1.overrideSpeed(0);
//			rotateController1.stop();
//			goto endLoop;
//		}
//		else if (digitalRead(limitPin_Min) == LOW)
//		{
//			rotateController1.overrideSpeed(0);
//			rotateController1.stop();
//			goto endLoop;
//		}
//		delay(10);
//	}
//
//endLoop:
//
//#ifdef DEBUG
//	Serial.println("endLoop:");
//#endif // DEBUG
//	// Set Return steps values
//	if (configOne.activeAxis == ID_SPINDLE)
//	{
//		endPosition_Spindle = stepper_One.getPosition();
//		returnSteps_Axis = 0;
//		returnSteps_Spindle = endPosition_Spindle;
//	}
//	else
//	{
//		endPosition_Axis = stepper_One.getPosition();
//		returnSteps_Axis = endPosition_Axis;
//		returnSteps_Spindle = 0;
//	}
//
//	// Reset enable/disable status on Nextion
//	SetEnable(configOne.activeAxis, false);
//	Serial3.print("pageOne.bt10.bco1=55715");// The first one may be ignored by Nextion
//	Serial3.write(0xff);
//	Serial3.write(0xff);
//	Serial3.write(0xff);
//	delay(10);
//	Serial3.print("pageMain.vaSpEnabled.val=1");
//	Serial3.write(0xff);
//	Serial3.write(0xff);
//	Serial3.write(0xff);
//	delay(10);
//	Serial3.print("pageOne.bt10.val=1");
//	Serial3.write(0xff);
//	Serial3.write(0xff);
//	Serial3.write(0xff);
//	delay(10);
//	//Serial3.print("pageOne.bt10.txt=");
//	//SerialWrite(0x22);
//	//SerialPrint("Disabled");
//	//SerialWrite(0x22);
//	//Serial3.write(0xff);
//	//Serial3.write(0xff);
//	//Serial3.write(0xff);
//#ifdef DEBUG
//	Serial.println("1.Exit RunOneStepper.");
//#endif // DEBUG
//	return;
//}

//void RunOneStepper_11(int direction) // pageOne
//{
//	//v011
//#ifdef DEBUG
//	Serial.println("Enter RunOneStepper.");
//#endif // DEBUG
//
//	int stepPin = PIN_AXIS_Z_STEP;
//	int dirPin = PIN_AXIS_Z_DIR;
//
//	int limitPin_Max = configSetup.limit_Max_Z;
//	int limitPin_Min = configSetup.limit_Min_Z;
//
//	//int polarity = LOW;
//	int maxSpd = 0;
//	int accel = 0;
//
//	float speedPercent = 0;
//	float currentSpeedPercent = 0;
//	int32_t targetPosition = 2000000000;// *direction;
//
//	switch (configOne.activeAxis)
//	{
//		case ID_AXIS_Z:
//		{
//			stepPin = PIN_AXIS_Z_STEP;
//			dirPin = PIN_AXIS_Z_DIR;
//			//enablePin = PIN_AXIS_Z_ENABLE;
//			//polarity = configSetup.polarity_Axis_Z ? (LOW) : (HIGH);
//			maxSpd = configOne.maxSpd_Axis_Z;
//			accel = configOne.accel_Axis_Z;
//			speedPercent = configOne.speedPercent_Axis_Z;
//			currentSpeedPercent = (speedPercent) * .01 *direction;
//			limitPin_Max = configSetup.limit_Max_Z;
//			limitPin_Min = configSetup.limit_Min_Z;
//	
//			break;
//		}
//		case ID_AXIS_X:
//		{
//			stepPin = PIN_AXIS_X_STEP;
//			dirPin = PIN_AXIS_X_DIR;
//			//polarity = configSetup.polarity_Axis_X ? (LOW) : (HIGH);
//			maxSpd = configOne.maxSpd_Axis_X;
//			accel = configOne.accel_Axis_X;
//			speedPercent = configOne.speedPercent_Axis_X;
//			currentSpeedPercent = speedPercent * .01 * direction;
//			limitPin_Max = configSetup.limit_Max_X;
//			limitPin_Min = configSetup.limit_Min_X;
//			break;
//		}
//		case ID_AXIS_B:
//		{
//			stepPin = PIN_AXIS_B_STEP;
//			dirPin = PIN_AXIS_B_DIR;
//			//polarity = configSetup.polarity_Axis_B ? (LOW) : (HIGH);
//			maxSpd = configOne.maxSpd_Axis_B;
//			accel = configOne.accel_Axis_B;
//			speedPercent = configOne.speedPercent_Axis_B;
//			currentSpeedPercent = speedPercent * .01 * direction;
//			limitPin_Max = configSetup.limit_Max_B;
//			limitPin_Min = configSetup.limit_Min_B;
//			break;
//		}
//		case ID_SPINDLE:
//		{
//			stepPin = PIN_SPINDLE_STEP;
//			dirPin = PIN_SPINDLE_DIR;
//			//polarity = configSetup.polarity_Spindle ? (LOW) : (HIGH);
//			maxSpd = configOne.maxSpd_Spindle;
//			accel = configOne.accel_Spindle;
//			speedPercent = configOne.speedPercent_Spindle;
//			currentSpeedPercent = speedPercent * .01 * direction;
//
//			break;
//		}
//	}
//
//#ifdef DEBUG
//	Serial.print("MaxSpd:");
//	Serial.println(maxSpd);
//	Serial.print("accel:");
//	Serial.println(accel);
//	Serial.print("speedPercent:");
//	Serial.println(speedPercent);
//	Serial.print("currentSpeedPercent:");
//	Serial.println(currentSpeedPercent);
//#endif // DEBUG
//	Stepper stepper_One(stepPin, dirPin);
//	stepper_One.setPosition(0);
//	delay(5);
//	stepper_One
//		.setMaxSpeed(maxSpd)
//		.setAcceleration(accel)
//		.setTargetAbs(targetPosition);
//
//#ifdef DEBUG
//	Serial.print("configSp.activeAxis:");
//	Serial.println(configOne.activeAxis);
//	Serial.print("direction:");
//	Serial.println(direction);
//	Serial.print("polarity:");
//	//Serial.println(polarity);
//	Serial.print("maxSpd:");
//	Serial.println(maxSpd);
//	Serial.print("accel:");
//	Serial.println(accel);
//	Serial.print("SpeedPercent:");
//	Serial.println(speedPercent);
//	Serial.print("currentSpeedPercent:");
//	Serial.println(currentSpeedPercent);
//	Serial.print("targetPosition:");
//	Serial.println(targetPosition);
//	Serial.println("+++++++++++++++++++++++++++++++");
//	Serial.print("getPosition:");
//	Serial.println(stepper_One.getPosition());
//#endif // DEBUG
//
//	SetEnable(configOne.activeAxis, true);
//#ifdef DEBUG
//	Serial.print("Call rotateAsync");
//#endif
//	rotateController1.rotateAsync(stepper_One);
//#ifdef DEBUG
//	Serial.print("Call overrideSpeed");
//#endif
//	rotateController1.overrideSpeed(currentSpeedPercent);
//	while (rotateController1.isRunning())
//	{
//		// Check for Cancel code  
//		if (SerialAvailable() > 0)
//		{
//			incomingByte = SerialRead(serialId);
//			switch (incomingByte)
//			{
//				case 67: // - C
//				{
//#ifdef DEBUG
//					Serial.println("call overrideSpeed:");
//#endif // DEBUG
//					rotateController1.overrideSpeed(0);
//					delay(5);
//#ifdef DEBUG
//					Serial.println("Call Stop:");
//#endif // DEBUG
//					//rotateController1.stop();
//					rotateController1.stop();
//					delay(100);
//#ifdef DEBUG
//					Serial.println("Stopped:");
//#endif // DEBUG
//					goto endLoop;
//					break;
//				}
//				case 102: // Speed
//				{
//					int newSpeedPercent = (int)GetSerialFloat(serialId);
//					switch (configOne.activeAxis)
//					{
//						case ID_AXIS_Z:
//						{
//							configOne.speedPercent_Axis_Z = newSpeedPercent;
//							currentSpeedPercent = float((newSpeedPercent) * .01 * direction);
//							break;
//						}
//						case ID_AXIS_X:
//						{
//							configOne.speedPercent_Axis_X = newSpeedPercent;
//							currentSpeedPercent = (float)(newSpeedPercent * .01 * direction);
//							break;
//						}
//						case ID_AXIS_B:
//						{
//							configOne.speedPercent_Axis_B = newSpeedPercent;
//							currentSpeedPercent = (float)(newSpeedPercent * .01 * direction);
//							break;
//						}
//						case ID_SPINDLE:
//						{
//							configOne.speedPercent_Spindle = newSpeedPercent;
//							currentSpeedPercent = (float)(newSpeedPercent * .01 * direction);
//							break;
//						}
//					}
//
//					EEPROM.put(eePromAddress_pageOne, configOne);
//
//					rotateController1.overrideSpeed(currentSpeedPercent); // set new speed
//					break;
//				}
//				default:
//				{
//					break;
//				}
//			}
//		}
//
//		if (digitalRead(limitPin_Max) == LOW)
//		{
//			rotateController1.overrideSpeed(0);
//			rotateController1.stop();
//			goto endLoop;
//		}
//		else if (digitalRead(limitPin_Min) == LOW)
//		{
//			rotateController1.overrideSpeed(0);
//			rotateController1.stop();
//			goto endLoop;
//		}
//		delay(10);
//	}
//
//endLoop:
//
//#ifdef DEBUG
//	Serial.println("endLoop:");
//#endif // DEBUG
//	// Set Return steps values
//	if (configOne.activeAxis == ID_SPINDLE)
//	{
//		endPosition_Spindle = stepper_One.getPosition();
//		returnSteps_Axis = 0;
//		returnSteps_Spindle = endPosition_Spindle;
//	}
//	else
//	{
//		endPosition_Axis = stepper_One.getPosition();
//		returnSteps_Axis = endPosition_Axis;
//		returnSteps_Spindle = 0;
//	}
//
//	// Reset enable/disable status on Nextion
//	SetEnable(configOne.activeAxis, false);
//	Serial3.print("pageOne.bt10.bco1=55715");// The first one may be ignored by Nextion
//	Serial3.write(0xff);
//	Serial3.write(0xff);
//	Serial3.write(0xff);
//	delay(10);
//	Serial3.print("pageMain.vaSpEnabled.val=1");
//	Serial3.write(0xff);
//	Serial3.write(0xff);
//	Serial3.write(0xff);
//	delay(10);
//	Serial3.print("pageOne.bt10.val=1");
//	Serial3.write(0xff);
//	Serial3.write(0xff);
//	Serial3.write(0xff);
//	delay(10);
//	//Serial3.print("pageOne.bt10.txt=");
//	//SerialWrite(0x22);
//	//SerialPrint("Disabled");
//	//SerialWrite(0x22);
//	//Serial3.write(0xff);
//	//Serial3.write(0xff);
//	//Serial3.write(0xff);
//#ifdef DEBUG
//	Serial.println("1.Exit RunOneStepper.");
//#endif // DEBUG
//	return;
//}

void RunOneStepper(int direction) // pageOne
{
	// V009
	//RotateControl rotateController;

	int stepPin = PIN_AXIS_Z_STEP;
	int dirPin = PIN_AXIS_Z_DIR;
	int enablePin = PIN_AXIS_Z_ENABLE;
	int limitPin_Max = configSetup.limit_Max_Z;
	int limitPin_Min = configSetup.limit_Min_Z;

	//int polarity = LOW;
	int maxSpd = 0;
	int accel = 0;

	float speedPercent = 0;
	float currentSpeedPercent = 0;
	int32_t targetPosition = 2000000000;// *direction;

	switch (configOne.activeAxis)
	{
	case ID_AXIS_Z:
	{
		stepPin = PIN_AXIS_Z_STEP;
		dirPin = PIN_AXIS_Z_DIR;
		enablePin = PIN_AXIS_Z_ENABLE;
		//polarity = configSetup.polarity_Axis_Z ? (LOW) : (HIGH);
		maxSpd = configOne.maxSpd_Axis_Z;
		accel = configOne.accel_Axis_Z;
		speedPercent = configOne.speedPercent_Axis_Z;
		currentSpeedPercent = (speedPercent) * .01 *direction;
		limitPin_Max = configSetup.limit_Max_Z;
		limitPin_Min = configSetup.limit_Min_Z;

		break;
	}
	case ID_AXIS_X:
	{
		stepPin = PIN_AXIS_X_STEP;
		dirPin = PIN_AXIS_X_DIR;
		enablePin = PIN_AXIS_X_ENABLE;
		//polarity = configSetup.polarity_Axis_X ? (LOW) : (HIGH);
		maxSpd = configOne.maxSpd_Axis_X;
		accel = configOne.accel_Axis_X;
		speedPercent = configOne.speedPercent_Axis_X;
		currentSpeedPercent = speedPercent * .01 * direction;
		limitPin_Max = configSetup.limit_Max_X;
		limitPin_Min = configSetup.limit_Min_X;
		break;
	}
	case ID_AXIS_B:
	{
		stepPin = PIN_AXIS_B_STEP;
		dirPin = PIN_AXIS_B_DIR;
		enablePin = PIN_AXIS_B_ENABLE;
		//polarity = configSetup.polarity_Axis_B ? (LOW) : (HIGH);
		maxSpd = configOne.maxSpd_Axis_B;
		accel = configOne.accel_Axis_B;
		speedPercent = configOne.speedPercent_Axis_B;
		currentSpeedPercent = speedPercent * .01 * direction;
		limitPin_Max = configSetup.limit_Max_B;
		limitPin_Min = configSetup.limit_Min_B;
		break;
	}
	case ID_SPINDLE:
	{
		stepPin = PIN_SPINDLE_STEP;
		dirPin = PIN_SPINDLE_DIR;
		enablePin = PIN_SPINDLE_ENABLE;
		//polarity = configSetup.polarity_Spindle ? (LOW) : (HIGH);
		maxSpd = configOne.maxSpd_Spindle;
		accel = configOne.accel_Spindle;
		speedPercent = configOne.speedPercent_Spindle;
		currentSpeedPercent = speedPercent * .01 * direction;
		break;
	}
	}


	Stepper stepper_One(stepPin, dirPin);
	stepper_One.setPosition(0);
	delay(5);
	stepper_One
		.setMaxSpeed(maxSpd)
		.setAcceleration(accel)
		.setTargetAbs(targetPosition);

#ifdef DEBUG
	Serial.print("configSp.activeAxis:");
	Serial.println(configOne.activeAxis);
	Serial.print("direction:");
	Serial.println(direction);
	//Serial.print("polarity:");
	//Serial.println(polarity);
	Serial.print("maxSpd:");
	Serial.println(maxSpd);
	Serial.print("accel:");
	Serial.println(accel);
	Serial.print("SpeedPercent:");
	Serial.println(speedPercent);
	Serial.print("currentSpeedPercent:");
	Serial.println(currentSpeedPercent);
	Serial.print("targetPosition:");
	Serial.println(targetPosition);
	Serial.println("+++++++++++++++++++++++++++++++");
	Serial.print("getPosition:");
	Serial.println(stepper_One.getPosition());
#endif // DEBUG

	digitalWrite(enablePin, LOW); // Enable
	rotateController1.rotateAsync(stepper_One);
	rotateController1.overrideSpeed(currentSpeedPercent);
	while (rotateController1.isRunning())
	{
		//#ifdef DEBUG
		//
		//		endPosition_Spindle = stepper_One.getPosition();
		//		Serial.print("Spindle:");
		//		Serial.println(endPosition_Spindle);
		//#endif // DEBUG

				// Check for Cancel code  
		if (SerialAvailable() > 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
			case 67: // - C
			{
#ifdef DEBUG
				Serial.print("stop:");

#endif // DEBUG
				rotateController1.overrideSpeed(0);
				delay(5);
				rotateController1.stop();
#ifdef DEBUG
				Serial.print("goto endLoop:");

#endif // DEBUG
				goto endLoop;
				break;
			}
			case 102: // Speed
			{

				int newSpeedPercent = (int)GetSerialFloat(serialId);
				switch (configOne.activeAxis)
				{
				case ID_AXIS_Z:
				{
					configOne.speedPercent_Axis_Z = newSpeedPercent;
					//speedPercent = (float)(newSpeedPercent);// *direction);
					currentSpeedPercent = float((-newSpeedPercent) * .01 * direction);
					break;
				}
				case ID_AXIS_X:
				{
					configOne.speedPercent_Axis_X = newSpeedPercent;
					currentSpeedPercent = (float)(newSpeedPercent * .01 * direction);
					break;
				}
				case ID_AXIS_B:
				{
					configOne.speedPercent_Axis_B = newSpeedPercent;
					currentSpeedPercent = (float)(newSpeedPercent * .01 * direction);
					break;
				}
				case ID_SPINDLE:
				{
					configOne.speedPercent_Spindle = newSpeedPercent;
					currentSpeedPercent = (float)(newSpeedPercent * .01 * direction);
					break;
				}
				}




				EEPROM.put(eePromAddress_pageOne, configOne);
#ifdef DEBUG
				//Serial.print("currentSpeedPercent:");
				//Serial.println(currentSpeedPercent);

#endif // DEBUG
				rotateController1.overrideSpeed(currentSpeedPercent); // set new speed
				break;
			}
			default:
			{
				break;
			}
			}
		}

		if (digitalRead(limitPin_Max) == LOW)
		{
			rotateController1.overrideSpeed(0);
			rotateController1.stop();
			goto endLoop;
		}
		else if (digitalRead(limitPin_Min) == LOW)
		{
			rotateController1.overrideSpeed(0);
			rotateController1.stop();
			goto endLoop;
		}
		delay(10);
	}

endLoop:

	// Set Return steps values
	if (configOne.activeAxis == ID_SPINDLE)
	{
		endPosition_Spindle = stepper_One.getPosition();
		returnSteps_Axis = 0;
		returnSteps_Spindle = endPosition_Spindle;
	}
	else
	{
		endPosition_Axis = stepper_One.getPosition();
		returnSteps_Axis = endPosition_Axis;
		returnSteps_Spindle = 0;
	}

	// Reset enable/disable status on Nextion
	digitalWrite(enablePin, HIGH); // Disable 
	Serial3.print("pageOne.bt10.bco1=55715");// The first one may be ignored by Nextion
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageMain.vaSpEnabled.val=1");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageOne.bt10.val=1");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	//Serial3.print("pageOne.bt10.txt=");
	//SerialWrite(0x22);
	//SerialPrint("Disabled");
	//SerialWrite(0x22);
	//Serial3.write(0xff);
	//Serial3.write(0xff);
	//Serial3.write(0xff);
	int freeMem = freeMemory();
	Serial.print("freeMem:");
	Serial.println(freeMem);
	//return;
}

 ///<summary>
/// RunTwoSteppers_All
/// </summary>
/// <comment>
/// Allow switching axis while spindle is running
/// </comment>
/// <param name="direction_Axis">Axis rotation</param>
/// <param name="direction_Spindle">Spindle rotation</param>
/// <param name="initialCaller">Spindle or Axis</param>
void RunTwoSteppers_All(
	int direction_Axis,
	int direction_Spindle,
	int initialCaller
)
{
	RotateControl rotateControllerAxis;
	Stepper stepper_Spindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
	int limitPin_Max = configSetup.limit_Max_Z;
	int limitPin_Min = configSetup.limit_Min_Z;

	//int polarity_Spindle = configSetup.polarity_Spindle ? (LOW) : (HIGH);
	int maxSpd_Spindle = 0;
	int accel_Spindle = 0;

	float speedPercentAxis = 0;
	float speedPercentSpindle = 0;
	float currentSpeedPercentSpindle = 0;
	int32_t targetPosition_Axis = 2000000000;
	int32_t targetPosition_Spindle = 2000000000;

	// initialCaller: Spindle or axis
	bool stepper_Axis_Go = false;
	bool stepper_Spindle_Go = false;

#ifdef DEBUG
	Serial.print("initialCaller:");
	Serial.println(initialCaller);
#endif // DEBUG
	if (initialCaller < ID_SPINDLE)
	{
		stepper_Axis_Go = true;
	}
	else
	{
		stepper_Spindle_Go = true;
	}

	Stepper stepper_Z(PIN_AXIS_Z_STEP, PIN_AXIS_Z_DIR);
	Stepper stepper_X(PIN_AXIS_X_STEP, PIN_AXIS_X_DIR);
	Stepper stepper_B(PIN_AXIS_B_STEP, PIN_AXIS_B_DIR);
	switch (configPageMain.axisId)
	{
		case ID_AXIS_Z:
		{
			speedPercentAxis = configPageMain.speedPercent_Axis_Z;
			speedPercentSpindle = configPageMain.speedPercent_Spindle;

			limitPin_Max = configSetup.limit_Max_Z;
			limitPin_Min = configSetup.limit_Min_Z;
			stepper_Z.setPosition(0);
			stepper_Z
				.setMaxSpeed(configPageMain.maxSpd_Axis_Z * direction_Axis)
				.setAcceleration(configPageMain.accel_Axis_Z)
				.setTargetAbs(targetPosition_Axis);
			break;
		}
		case ID_AXIS_X:
		{
			speedPercentAxis = configPageMain.speedPercent_Axis_X;
			speedPercentSpindle = configPageMain.speedPercent_Spindle;

			limitPin_Max = configSetup.limit_Max_X;
			limitPin_Min = configSetup.limit_Min_X;
			stepper_X.setPosition(0);
			stepper_X
				.setMaxSpeed(configPageMain.maxSpd_Axis_X * direction_Axis)
				.setAcceleration(configPageMain.accel_Axis_X)
				.setTargetAbs(targetPosition_Axis);
			break;
		}
		case ID_AXIS_B:
		{
			speedPercentAxis = configPageMain.speedPercent_Axis_B;
			speedPercentSpindle = configPageMain.speedPercent_Spindle;

			limitPin_Max = configSetup.limit_Max_B;
			limitPin_Min = configSetup.limit_Min_B;
			stepper_B.setPosition(0);
			stepper_B
				.setMaxSpeed(configPageMain.maxSpd_Axis_B * direction_Axis)
				.setAcceleration(configPageMain.accel_Axis_B)
				.setTargetAbs(targetPosition_Axis);
			
			break;
		}
	}

	maxSpd_Spindle = configPageMain.maxSpd_Spindle;
	accel_Spindle = configPageMain.accel_Spindle;
	currentSpeedPercentSpindle = speedPercentSpindle * .01;
	stepper_Spindle.setPosition(0);
	delay(5);

	stepper_Spindle
		.setMaxSpeed(maxSpd_Spindle * direction_Spindle)
		.setAcceleration(accel_Spindle)
		.setTargetAbs(targetPosition_Spindle);

#ifdef DEBUG
	Serial.print("maxSpd_Spindle:");
	Serial.println(maxSpd_Spindle);
	Serial.print("accel_Spindle:");
	Serial.println(accel_Spindle);
	Serial.print("targetPosition_Spindle:");
	Serial.println(targetPosition_Spindle);
#endif // DEBUG

	if (stepper_Axis_Go)
	{
		switch (configPageMain.axisId)
		{
			case ID_AXIS_Z:
			{
				SetEnable(ID_AXIS_Z, true);
				rotateControllerAxis.rotateAsync(stepper_Z);
				rotateControllerAxis.overrideSpeed(0);
				delay(10);
				rotateControllerAxis.overrideSpeed(configPageMain.speedPercent_Axis_Z * .01);
				break;
			}
			case ID_AXIS_X:
			{
				SetEnable(ID_AXIS_X, true);
				rotateControllerAxis.rotateAsync(stepper_X);
				rotateControllerAxis.overrideSpeed(0);
				delay(10);
				rotateControllerAxis.overrideSpeed(configPageMain.speedPercent_Axis_X * .01);
				break;
			}
			case ID_AXIS_B:
			{
				SetEnable(ID_AXIS_B, true);
				rotateControllerAxis.rotateAsync(stepper_B);
				rotateControllerAxis.overrideSpeed(0);
				delay(10);
				rotateControllerAxis.overrideSpeed(configPageMain.speedPercent_Axis_B * .01);
				break;
			}
		}

	}

	if (stepper_Spindle_Go)
	{
		SetEnable(ID_SPINDLE, true);
		delay(5);
//#ifdef DEBUG
//		Serial.println("PIN_SPINDLE_ENABLE:");
//		Serial.println(digitalRead(PIN_SPINDLE_ENABLE));
//#endif // DEBUG
		rotateController1.rotateAsync(stepper_Spindle);
		rotateController1.overrideSpeed(0);
		delay(10);
		rotateController1.overrideSpeed(currentSpeedPercentSpindle);
	}

	while (stepper_Axis_Go || stepper_Spindle_Go)
	{
//#ifdef DEBUG
//		Serial.print("stepper_Axis_Go:");
//		Serial.println(stepper_Axis_Go);
//		Serial.print("stepper_Spindle_Go:");
//		Serial.println(stepper_Spindle_Go);

		//if (!stepper_Axis_Go)
		//{
		//	Serial.print("0.stepper_Axis_Go:");
		//	Serial.println(stepper_Axis_Go);
		//	delay(5000);
		//}
//#endif // DEBUG
		delay(5);
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			//Serial.print("1.incomingByte:");
			//Serial.println(incomingByte);

			switch (incomingByte)
			{

				case 172: // ¬ - Sp-B Axis CW
				{
					if (!rotateControllerAxis.isRunning())
					{
						stepper_Axis_Go = true;
						direction_Axis = DIR_CW;
						targetPosition_Axis = 2000000000;
						speedPercentAxis = (float)((configPageMain.speedPercent_Axis_B) * .01);
						stepper_B
							.setMaxSpeed(configPageMain.maxSpd_Axis_B * direction_Axis)
							.setTargetAbs(targetPosition_Axis);

						SetEnable(ID_AXIS_B, true);
						rotateControllerAxis.rotateAsync(stepper_B);
						rotateControllerAxis.overrideSpeed(0);
						delay(5);
						rotateControllerAxis.overrideSpeed(speedPercentAxis); 
					}
					break;
				}

				case 90: // Z - Sp-Z Axis CW
				{
					if (!rotateControllerAxis.isRunning())
					{
						stepper_Axis_Go = true;
						direction_Axis = DIR_CW;
						targetPosition_Axis = 2000000000;
						speedPercentAxis = (float)((configPageMain.speedPercent_Axis_Z) * .01);
						stepper_Z
							.setMaxSpeed(configPageMain.maxSpd_Axis_Z * direction_Axis)
							.setTargetAbs(targetPosition_Axis);

						SetEnable(ID_AXIS_Z, true);
						rotateControllerAxis.rotateAsync(stepper_Z);
						rotateControllerAxis.overrideSpeed(0);
						delay(5);
						rotateControllerAxis.overrideSpeed(speedPercentAxis);
					}
					break;
				}
				case 166: // ¦ - Sp-X Axis CW
				{
					
					if (!rotateControllerAxis.isRunning())
					{
						stepper_Axis_Go = true;
						direction_Axis = DIR_CW;
						targetPosition_Axis = 2000000000;
						speedPercentAxis = (float)(configPageMain.speedPercent_Axis_X * .01);
						stepper_X
							.setMaxSpeed(configPageMain.maxSpd_Axis_X * direction_Axis)
							.setTargetAbs(targetPosition_Axis);

						SetEnable(ID_AXIS_X, true);
						rotateControllerAxis.rotateAsync(stepper_X);
						rotateControllerAxis.overrideSpeed(0);
						delay(5);
						rotateControllerAxis.overrideSpeed(speedPercentAxis); 
					}

					break;
				}
				case 171: // « - Sp-B Axis CCW
				{
					if (!rotateControllerAxis.isRunning())
					{
						stepper_Axis_Go = true;
						direction_Axis = DIR_CCW;
						targetPosition_Axis = -2000000000;

						speedPercentAxis = (float)((configPageMain.speedPercent_Axis_B) * .01);
						stepper_B
							.setMaxSpeed(configPageMain.maxSpd_Axis_B * direction_Axis)
							.setTargetAbs(targetPosition_Axis);

						SetEnable(ID_AXIS_B, true);
						rotateControllerAxis.rotateAsync(stepper_B);
						rotateControllerAxis.overrideSpeed(0);
						delay(5);
						rotateControllerAxis.overrideSpeed(speedPercentAxis);
					}
					break;
				}

				case 91: // [ - Sp-Z Axis CCW
				{
					if (!rotateControllerAxis.isRunning())
					{
						stepper_Axis_Go = true;
						direction_Axis = DIR_CCW;
						targetPosition_Axis = -2000000000;
						speedPercentAxis = (float)((configPageMain.speedPercent_Axis_Z) * .01);
						stepper_Z
							.setMaxSpeed(configPageMain.maxSpd_Axis_Z * direction_Axis)
							.setTargetAbs(targetPosition_Axis);

						SetEnable(ID_AXIS_Z, true);
						rotateControllerAxis.rotateAsync(stepper_Z);
						rotateControllerAxis.overrideSpeed(0);
						delay(5);
						rotateControllerAxis.overrideSpeed(speedPercentAxis);
					}
					break;
				}
				case 165: // ¥ - Sp-X Axis CCW
				{
					if (!rotateControllerAxis.isRunning())
					{
						stepper_Axis_Go = true;
						direction_Axis = DIR_CCW;
						targetPosition_Axis = -2000000000;
						speedPercentAxis = (float)(configPageMain.speedPercent_Axis_X * .01);
						stepper_X
							.setMaxSpeed(configPageMain.maxSpd_Axis_X * direction_Axis)
							.setTargetAbs(targetPosition_Axis);

						SetEnable(ID_AXIS_X, true);
						rotateControllerAxis.rotateAsync(stepper_X);
						rotateControllerAxis.overrideSpeed(0);
						delay(5);
						rotateControllerAxis.overrideSpeed(speedPercentAxis);
					}
					break;
				}
				case 67: // C - Spindle Stop
				{
#ifdef DEBUG
					Serial.print("Stop Spindle:");
#endif // DEBUG
					rotateController1.overrideSpeed(0);
					delay(5);
					rotateController1.stop();

#ifdef DEBUG
					Serial.println("Spindle Stopped.");
#endif // DEBUG

					SetEnable(ID_SPINDLE, false);
					stepper_Spindle_Go = false;

#ifdef DEBUG
					Serial.print("1.stepper_Axis_Go:");
					Serial.println(stepper_Axis_Go);
					Serial.print("1.stepper_Spindle_Go:");
					Serial.println(stepper_Spindle_Go);
#endif // DEBUG

					//if (!stepper_Axis_Go)
					//{
					//	Serial.print("1a.stepper_Axis_Go:");
					//	Serial.println(stepper_Axis_Go);
					//	delay(5000);
					//}
					break;
				}
				case 97: // a - Sp-Z axis speed
				case 162: // ¢ - Sp-X axis speed
				case 168: // ¨ - Sp-B axis speed
				{
					int newSpeedPercent = (int)GetSerialFloat(serialId);
					switch (configPageMain.axisId)
					{
						case ID_AXIS_Z:
						{
							configPageMain.speedPercent_Axis_Z = newSpeedPercent;
							speedPercentAxis = (float)((newSpeedPercent) * .01);
							break;
						}
						case ID_AXIS_X:
						{
							configPageMain.speedPercent_Axis_X = newSpeedPercent;
							speedPercentAxis = (float)(newSpeedPercent * .01);
							break;
						}
						case ID_AXIS_B:
						{
							configPageMain.speedPercent_Axis_B = newSpeedPercent;
							speedPercentAxis = (float)((newSpeedPercent) * .01);
							break;
						}
					}
					EEPROM.put(eePromAddress_pageMain, configPageMain);

					rotateControllerAxis.overrideSpeed(speedPercentAxis);
					break;
				}
				case 99: // c - Axis stop
				{
					rotateControllerAxis.overrideSpeed(0);
					delay(5);
					rotateControllerAxis.stop();
					switch (configPageMain.axisId)
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
					}

					stepper_Axis_Go = false;

					// Ensure user doesn't switch axes too fast.
					delay(1000);
					break;
				}
				case 107: // k - Sp-Z spindle CW
				case 164: // ¤ - Sp-X spindle CW
				case 170: // ª - Sp-B spindle CW
				{
					if (!rotateController1.isRunning())
					{
						direction_Spindle = DIR_CW;
						targetPosition_Spindle = 2000000000;
						speedPercentSpindle = (float)(configPageMain.speedPercent_Spindle *.01);
						stepper_Spindle
							.setMaxSpeed(maxSpd_Spindle)
							.setTargetAbs(targetPosition_Spindle);
						stepper_Spindle_Go = true;

						SetEnable(ID_SPINDLE, true);
						rotateController1.rotateAsync(stepper_Spindle);
						rotateController1.overrideSpeed(0);
						delay(5);
						rotateController1.overrideSpeed(speedPercentSpindle);
					}

					break;
				}
				case 108: // l - Sp-Z spindle CCW
				case 163: // £ - Sp-X spindle CCW
				case 169: // © - Sp-B spindle CCW
				{
					if (!rotateController1.isRunning())
					{
						direction_Spindle = DIR_CCW;
						targetPosition_Spindle = 2000000000;
						speedPercentSpindle = (float)(configPageMain.speedPercent_Spindle *.01);
						stepper_Spindle
							.setMaxSpeed(maxSpd_Spindle * DIR_CCW)
							.setTargetAbs(targetPosition_Spindle);
						stepper_Spindle_Go = true;

						SetEnable(ID_SPINDLE, true);
						rotateController1.rotateAsync(stepper_Spindle);
						rotateController1.overrideSpeed(0);
						delay(5);
						rotateController1.overrideSpeed(speedPercentSpindle);
					}
					break;
				}
				case 110: // n - Sp-Z spindle speed
				case 161: // ¡ - Sp-X spindle speed
				case 167: // § - Sp-B spindle speed
				{
					configPageMain.speedPercent_Spindle = (int)GetSerialFloat(serialId);
					speedPercentSpindle = (float)(configPageMain.speedPercent_Spindle * .01);
					EEPROM.put(eePromAddress_pageMain, configPageMain);
					rotateController1.overrideSpeed(speedPercentSpindle);
					break;
				}
				case 197: // Å - Set AxisID
				{
					configPageMain.axisId = GetSerialInteger();
					EEPROM.put(eePromAddress_pageMain, configPageMain);
					break;
				}
			}

#ifdef DEBUG
			Serial.print("2.stepper_Axis_Go:");
			Serial.println(stepper_Axis_Go);
			Serial.print("2.stepper_Spindle_Go:");
			Serial.println(stepper_Spindle_Go);
#endif // DEBUG
		}

//		/*if (stepper_Axis_Go)
//		{
//			if (digitalRead(limitPin_Max) == LOW)
//			{
//				rotateControllerAxis.overrideSpeed(0);
//				rotateControllerAxis.stop();
//				stepper_Axis_Go = false;
//			}
//		}
//		else
//		{
//			if (digitalRead(limitPin_Min) == LOW)
//			{
//				rotateControllerAxis.overrideSpeed(0);
//				rotateControllerAxis.stop();
//				stepper_Axis_Go = false;
//			}
//		}
//*/
		delay(5);
	}

#ifdef DEBUG
	Serial.println("All Stopped.");
#endif // DEBUG
	stepper_Axis_Go = false;
	stepper_Spindle_Go = false;

	switch (configPageMain.axisId)
	{
		case ID_AXIS_Z:
		{
			returnSteps_Axis = stepper_Z.getPosition();
			stepper_Z.setPosition(0);
			break;
		}
		case ID_AXIS_X:
		{
			returnSteps_Axis = stepper_X.getPosition();
			stepper_X.setPosition(0);
			break;
		}
		case ID_AXIS_B:
		{
			returnSteps_Axis = stepper_B.getPosition();
			stepper_B.setPosition(0);
			break;
		}
	}

	returnSteps_Spindle = stepper_Spindle.getPosition();
	stepper_Spindle.setPosition(0);

	SetEnable(ID_SPINDLE, false);
	SetEnable(ID_AXIS_Z, false);
	SetEnable(ID_AXIS_X, false);
	SetEnable(ID_AXIS_B, false);
#ifdef DEBUG
	Serial.println("Exit RunTwoSteppers_All.");
#endif // DEBUG
}


///<summary>
/// RunTwoSteppers_Sp_Axis
/// </summary>
/// <comment>
/// </comment>
/// <param name="direction_Axis">Axis motor rotation direction</param>
/// <param name="direction_Spindle">Spindle rotation direction</param>
/// <param name="initialCaller">Spindle or Axis</param>
/// <param name="axisId">Z, X, or B Axis</param>
void RunTwoSteppers_Sp_Axis(
	int direction_Axis,
	int direction_Spindle,
	int initialCaller
)
{
	RotateControl rotateControllerAxis;
	RotateControl rotateControllerSpindle;
	Stepper stepper_Spindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
	int stepPin_Axis = PIN_AXIS_Z_STEP;
	int dirPin_Axis = PIN_AXIS_Z_DIR;
	int enablePin_Axis = PIN_AXIS_Z_ENABLE;
	int limitPin_Max = configSetup.limit_Max_Z;
	int limitPin_Min = configSetup.limit_Min_Z;

	int polarity_Axis = LOW;
	int maxSpd_Axis = 0;
	int accel_Axis = 0;

	int polarity_Spindle = configSetup.polarity_Spindle ? (LOW) : (HIGH);
	int maxSpd_Spindle = 0;
	int accel_Spindle = 0;

	float speedPercentAxis = 0;
	float currentSpeedPercentAxis = 0;
	float speedPercentSpindle = 0;
	float currentSpeedPercentSpindle = 0;
	int32_t targetPosition_Axis = 2000000000;
	int32_t targetPosition_Spindle = 2000000000;

	// initialCaller: Spindle or axis
	bool stepper_Axis_Go = false;
	bool stepper_Spindle_Go = false;

	if (initialCaller < ID_SPINDLE)
	{
		stepper_Axis_Go = true;
	}
	else
	{
		stepper_Spindle_Go = true;
	}

	//switch (axisId)
	switch (configPageMain.axisId)
	{
	case ID_AXIS_Z:
	{
		stepPin_Axis = PIN_AXIS_Z_STEP;
		dirPin_Axis = PIN_AXIS_Z_DIR;
		enablePin_Axis = PIN_AXIS_Z_ENABLE;
		polarity_Axis = configSetup.polarity_Axis_Z ? (LOW) : (HIGH);
		maxSpd_Axis = configPageMain.maxSpd_Axis_Z;
		accel_Axis = configPageMain.accel_Axis_Z;

		maxSpd_Spindle = configPageMain.maxSpd_Spindle;
		accel_Spindle = configPageMain.accel_Spindle;
		speedPercentAxis = configPageMain.speedPercent_Axis_Z;
		speedPercentSpindle = configPageMain.speedPercent_Spindle;

		limitPin_Max = configSetup.limit_Max_Z;
		limitPin_Min = configSetup.limit_Min_Z;
		break;
	}
	case ID_AXIS_X:
	{
		stepPin_Axis = PIN_AXIS_X_STEP;
		dirPin_Axis = PIN_AXIS_X_DIR;
		enablePin_Axis = PIN_AXIS_X_ENABLE;
		polarity_Axis = configSetup.polarity_Axis_X ? (LOW) : (HIGH);
		maxSpd_Axis = configPageMain.maxSpd_Axis_X;
		accel_Axis = configPageMain.accel_Axis_X;

		maxSpd_Spindle = configPageMain.maxSpd_Spindle;
		accel_Spindle = configPageMain.accel_Spindle;

		speedPercentAxis = configPageMain.speedPercent_Axis_X;
		speedPercentSpindle = configPageMain.speedPercent_Spindle;

		limitPin_Max = configSetup.limit_Max_X;
		limitPin_Min = configSetup.limit_Min_X;
		break;
	}
	case ID_AXIS_B:
	{
		stepPin_Axis = PIN_AXIS_B_STEP;
		dirPin_Axis = PIN_AXIS_B_DIR;
		enablePin_Axis = PIN_AXIS_B_ENABLE;
		polarity_Axis = configSetup.polarity_Axis_B ? (LOW) : (HIGH);
		maxSpd_Axis = configPageMain.maxSpd_Axis_B;
		accel_Axis = configPageMain.accel_Axis_B;

		maxSpd_Spindle = configPageMain.maxSpd_Spindle;
		accel_Spindle = configPageMain.accel_Spindle;

		speedPercentAxis = configPageMain.speedPercent_Axis_B;
		speedPercentSpindle = configPageMain.speedPercent_Spindle;

		limitPin_Max = configSetup.limit_Max_B;
		limitPin_Min = configSetup.limit_Min_B;
		break;
	}
	}

	currentSpeedPercentAxis = speedPercentAxis * .01;
	currentSpeedPercentSpindle = speedPercentSpindle * .01;
#ifdef DEBUG
	Serial.print("configPageMain.axisId:");
	Serial.println(configPageMain.axisId);
#endif // DEBUG


	Stepper stepper_Axis(stepPin_Axis, dirPin_Axis);
	stepper_Axis.setPosition(0);
	stepper_Spindle.setPosition(0);

	delay(5);

	stepper_Axis
		.setStepPinPolarity(polarity_Axis)
		.setMaxSpeed(maxSpd_Axis * direction_Axis)
		.setAcceleration(accel_Axis)
		.setTargetAbs(targetPosition_Axis);

	stepper_Spindle
		.setStepPinPolarity(polarity_Spindle)
		.setMaxSpeed(maxSpd_Spindle * direction_Spindle)
		.setAcceleration(accel_Spindle)
		.setTargetAbs(targetPosition_Spindle);


#ifdef DEBUG
	Serial.print("initialCaller:");
	Serial.println(initialCaller);

	//Serial.print("direction_Spindle:");
	//Serial.println(direction_Spindle);
	//Serial.print("maxSpd_Spindle:");
	//Serial.println(maxSpd_Spindle);
	//Serial.print("speedPercentSpindle:");
	//Serial.println(speedPercentSpindle);
	//Serial.print("targetPosition_Axis:");
	//Serial.println(targetPosition_Axis);
	//Serial.println("------------------------------");
	//Serial.print("SpindleAccel:");
	//Serial.println(accel_Spindle);

	//Serial.print("microsteps_Spindle:");
	//Serial.println(configMain.microsteps_Spindle);
	//Serial.print("steps360_Spindle:");
	//Serial.println(configMain.steps360_Spindle);
	//Serial.print("gearRatio_Spindle:");
	//Serial.println(configMain.gearRatio_Spindle);
	Serial.println("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
	Serial.print("axisId:");
	Serial.println(configPageMain.axisId);
	Serial.print("direction_Axis:");
	Serial.println(direction_Axis);
	Serial.print("polarity_Axis");
	Serial.println(polarity_Axis);
	Serial.print("maxSpd_Axis:");
	Serial.println(maxSpd_Axis * direction_Axis);
	Serial.print("accel_Axis:");
	Serial.println(accel_Axis);
	Serial.print("speedPercentAxis:");
	Serial.println(speedPercentAxis);
	Serial.print("currentSpeedPercentAxis:");
	Serial.println(currentSpeedPercentAxis);
	Serial.print("targetPosition_Axis:");
	Serial.println(targetPosition_Axis);
	Serial.println("+++++++++++++++++++++++++++++++");
	Serial.print("getPosition:");
	Serial.println(stepper_Axis.getPosition());
	//Serial.print("AxisAccel:");
	//Serial.println(accel_Axis);

	//Serial.print("distancePerRev_AxisZ:");
	//Serial.println(configMain.distancePerRev_AxisZ);
	Serial.print("steps360_Axis_Z:");
	Serial.println(configSetup.steps360_Axis_Z);
	Serial.print("microsteps_Axis_Z:");
	Serial.println(configSetup.microsteps_Axis_Z);
	Serial.println("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-");

#endif // DEBUG

	if (stepper_Axis_Go)
	{
		digitalWrite(enablePin_Axis, LOW);  // Enable
		rotateControllerAxis.rotateAsync(stepper_Axis);
		rotateControllerAxis.overrideSpeed(0);
		delay(100);
		rotateControllerAxis.overrideSpeed(currentSpeedPercentAxis);
	}
	if (stepper_Spindle_Go)
	{
		digitalWrite(PIN_SPINDLE_ENABLE, LOW); // Enable 
		delay(5);
		rotateControllerSpindle.rotateAsync(stepper_Spindle);
		rotateControllerSpindle.overrideSpeed(0);
		delay(100);
		rotateControllerSpindle.overrideSpeed(currentSpeedPercentSpindle);
	}

	while (stepper_Axis_Go || stepper_Spindle_Go)
	{

		delay(5);
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			Serial.print("incomingByte:");
			Serial.println(incomingByte);
			switch (incomingByte)
			{
			case 90: // Z - Sp-Z Axis CW
			case 166: // ¦ - Sp-X Axis CW
			case 172: // ¬ - Sp-B Axis CW
			{
				if (!rotateControllerAxis.isRunning())
				{
					Serial.println("Axis is not running.");

					direction_Axis = DIR_CW;
					targetPosition_Axis = -2000000000;
					switch (configPageMain.axisId)
					{
					case ID_AXIS_Z:
					{
						speedPercentAxis = (float)((configPageMain.speedPercent_Axis_Z) * .01);
						break;
					}
					case ID_AXIS_X:
					{
						speedPercentAxis = (float)(configPageMain.speedPercent_Axis_X * .01);
						break;
					}
					case ID_AXIS_B:
					{
						speedPercentAxis = (float)((configPageMain.speedPercent_Axis_B) * .01);
						break;
					}
					}
					stepper_Axis
						.setMaxSpeed(maxSpd_Axis)
						.setTargetAbs(targetPosition_Axis);
					stepper_Axis_Go = true;
					digitalWrite(enablePin_Axis, LOW);  // Enable
					rotateControllerAxis.rotateAsync(stepper_Axis);
					rotateControllerAxis.overrideSpeed(0);
					delay(5);
					rotateControllerAxis.overrideSpeed(speedPercentAxis);
				}
				else
				{
					Serial.println("Axis is running.");
				}
				break;
			}
			case 91: // [ - Sp-Z Axis CCW
			case 165: // ¥ - Sp-X Axis CCW
			case 171: // « - Sp-B Axis CCW
			{
				if (!rotateControllerAxis.isRunning())
				{
					direction_Axis = DIR_CCW;
					targetPosition_Axis = -2000000000;


					switch (configPageMain.axisId)
					{
					case ID_AXIS_Z:
					{
						speedPercentAxis = (float)((configPageMain.speedPercent_Axis_Z) * .01);
						break;
					}
					case ID_AXIS_X:
					{
						speedPercentAxis = (float)(configPageMain.speedPercent_Axis_X * .01);
						break;
					}
					case ID_AXIS_B:
					{
						speedPercentAxis = (float)((configPageMain.speedPercent_Axis_B) * .01);
						break;
					}
					}

					stepper_Axis
						.setMaxSpeed(maxSpd_Axis * DIR_CCW)
						.setTargetAbs(targetPosition_Axis);
					stepper_Axis_Go = true;
					digitalWrite(enablePin_Axis, LOW);  // Enable 
					rotateControllerAxis.rotateAsync(stepper_Axis);
					rotateControllerAxis.overrideSpeed(0);
					delay(5);
					rotateControllerAxis.overrideSpeed(speedPercentAxis);
				}
				break;
			}
			case 67: // C - Spindle Stop
			{
				Serial.print("Stop Spindle:");
				rotateControllerSpindle.overrideSpeed(0);
				delay(5);
				rotateControllerSpindle.stop();
				digitalWrite(PIN_SPINDLE_ENABLE, HIGH); // Disable

				stepper_Spindle_Go = false;
				break;
			}
			case 97: // a - Sp-Z axis speed
			case 162: // ¢ - Sp-X axis speed
			case 168: // ¨ - Sp-B axis speed
			{
				int newSpeedPercent = (int)GetSerialFloat(serialId);
#ifdef DEBUG
				Serial.print("NewSpeedPercent: ");
				Serial.println(newSpeedPercent);
#endif // DEBUG

				switch (configPageMain.axisId)
				{
				case ID_AXIS_Z:
				{
					configPageMain.speedPercent_Axis_Z = newSpeedPercent;
					speedPercentAxis = (float)((newSpeedPercent) * .01);
					break;
				}
				case ID_AXIS_X:
				{
					configPageMain.speedPercent_Axis_X = newSpeedPercent;
					speedPercentAxis = (float)(newSpeedPercent * .01);
					break;
				}
				case ID_AXIS_B:
				{
					configPageMain.speedPercent_Axis_B = newSpeedPercent;
					speedPercentAxis = (float)((newSpeedPercent) * .01);
					break;
				}
				}
				EEPROM.put(eePromAddress_pageMain, configPageMain);

				rotateControllerAxis.overrideSpeed(speedPercentAxis);
				break;
			}
			case 99: // c - Axis stop
			{
				rotateControllerAxis.overrideSpeed(0);
				delay(5);
				rotateControllerAxis.stop();
				digitalWrite(enablePin_Axis, HIGH);  // Disable 
				//stepper_Z.setPosition(0);
				stepper_Axis_Go = false;
				break;
			}
			case 107: // k - Sp-Z spindle CW
			case 164: // ¤ - Sp-X spindle CW
			case 170: // ª - Sp-B spindle CW
			{
				if (!rotateControllerSpindle.isRunning())
				{
					direction_Spindle = DIR_CW;
					targetPosition_Spindle = 2000000000;

					speedPercentSpindle = (float)(configPageMain.speedPercent_Spindle *.01);
#ifdef DEBUG
					Serial.print("+++++++++++++++++++targetPosition_Spindle");
					Serial.println(targetPosition_Spindle);
					Serial.print("+++++++++++++++++++speedPercentSpindle");
					Serial.println(speedPercentSpindle);
					Serial.print("+++++++++++++++++++maxSpd_Spindle");
					Serial.println(maxSpd_Spindle);

#endif // DEBUG

					stepper_Spindle
						.setMaxSpeed(maxSpd_Spindle)
						.setTargetAbs(targetPosition_Spindle);
					stepper_Spindle_Go = true;
					digitalWrite(PIN_SPINDLE_ENABLE, LOW); // Enable
					rotateControllerSpindle.rotateAsync(stepper_Spindle);
					rotateControllerSpindle.overrideSpeed(0);
					delay(5);
					rotateControllerSpindle.overrideSpeed(speedPercentSpindle);
				}
				break;
			}
			case 108: // l - Sp-Z spindle CCW
			case 163: // £ - Sp-X spindle CCW
			case 169: // © - Sp-B spindle CCW
			{
				if (!rotateControllerSpindle.isRunning())
				{
					direction_Spindle = DIR_CCW;
					targetPosition_Spindle = 2000000000;
					speedPercentSpindle = (float)(configPageMain.speedPercent_Spindle *.01);
#ifdef DEBUG
					Serial.print("----------------------targetPosition_Spindle");
					Serial.println(targetPosition_Spindle);
					Serial.print("---------------------speedPercentSpindle");
					Serial.println(speedPercentSpindle);
					Serial.print("---------------------maxSpd_Spindle");
					Serial.println(maxSpd_Spindle);
#endif // DEBUG

					stepper_Spindle
						.setMaxSpeed(maxSpd_Spindle * DIR_CCW)
						.setTargetAbs(targetPosition_Spindle);
					stepper_Spindle_Go = true;
					digitalWrite(PIN_SPINDLE_ENABLE, LOW); // Enable
					rotateControllerSpindle.rotateAsync(stepper_Spindle);
					rotateControllerSpindle.overrideSpeed(0);
					delay(5);
					rotateControllerSpindle.overrideSpeed(speedPercentSpindle);
				}
				break;
			}
			case 110: // n - Sp-Z spindle speed
			case 161: // ¡ - Sp-X spindle speed
			case 167: // § - Sp-B spindle speed
			{

#ifdef DEBUG
				Serial.print("configMain.speedPercentSpindle_Main-0:");
				Serial.println(configPageMain.speedPercent_Spindle);
#endif // DEBUG


				configPageMain.speedPercent_Spindle = (int)GetSerialFloat(serialId);
				speedPercentSpindle = (float)(configPageMain.speedPercent_Spindle * .01);
#ifdef DEBUG
				Serial.print("configMain.speedPercentSpindle_Main-1:");
				Serial.println(configPageMain.speedPercent_Spindle);
#endif // DEBUG
				EEPROM.put(eePromAddress_pageMain, configPageMain);
				rotateControllerSpindle.overrideSpeed(speedPercentSpindle);
				break;
			}
			}
		}

		if (stepper_Axis_Go)
		{
			if (digitalRead(limitPin_Max) == LOW)
			{
				rotateControllerAxis.overrideSpeed(0);
				rotateControllerAxis.stop();
				stepper_Axis_Go = false;
			}
		}
		else
		{
			if (digitalRead(limitPin_Min) == LOW)
			{
				rotateControllerAxis.overrideSpeed(0);
				rotateControllerAxis.stop();
				stepper_Axis_Go = false;
			}
		}

		//#ifdef DEBUG
		//
		//		endPosition_Spindle = stepper_Spindle.getPosition();
		//		Serial.print("Spindle:");
		//		Serial.println(endPosition_Spindle);
		//#endif // DEBUG
		delay(5);
	}

	stepper_Axis_Go = false;
	stepper_Spindle_Go = false;

	returnSteps_Axis = stepper_Axis.getPosition();
	returnSteps_Spindle = stepper_Spindle.getPosition();
	stepper_Axis.setPosition(0);
	stepper_Spindle.setPosition(0);
	digitalWrite(PIN_SPINDLE_ENABLE, HIGH); // Disable 
	digitalWrite(PIN_AXIS_Z_ENABLE, HIGH);  // Disable 
	digitalWrite(PIN_AXIS_X_ENABLE, HIGH);  // Disable 
	digitalWrite(PIN_AXIS_B_ENABLE, HIGH);  // Disable 
}

/// <summary>
/// Index spindle
/// </summary>
/// <comment>
/// </comment>
/// <param name="directionSpindle">Counterclockwise: -1, Clockwise: 1</param>
/// <returns></returns>
void IndexSpindle(int directionSpindle)
{
	StepControl stepController;
	Stepper stepper_Spindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
	
	stepper_Spindle.setPosition(0);
	int stepsToMove = 0;

	switch (configIndex_Main.indexId)
	{
		case 1:
		{
			Serial.print("configIndex_1.size(Switch):");
			Serial.println(configIndex_1.size);
			stepsToMove = (int)round(configIndex_1.size);
			break;
		}
		case 2:
		{
			Serial.print("configIndex_2.size(Switch):");
			Serial.println(configIndex_2.size);
			stepsToMove = (int)round(configIndex_2.size);
			break;
		}
		case 3:
		{
			Serial.print("configIndex_3.size(Switch):");
			Serial.println(configIndex_3.size);
			stepsToMove = (int)round(configIndex_3.size);
			break;
		}
	}

#ifdef DEBUG
	Serial.print("directionSpindle:");
	Serial.println(directionSpindle);
	Serial.print("stepsToMove:");
	Serial.println(stepsToMove);
#endif // DEBUG

	// Set speed and acceleration
	stepper_Spindle
		.setMaxSpeed(configIndex_Main.maxSpd)
		.setAcceleration(configIndex_Main.accel)
		.setTargetRel(stepsToMove * directionSpindle);

	SetEnable(ID_SPINDLE, true);

#ifdef DEBUG
	Serial.println("1:moveAsync:");

#endif // DEBUG
	stepController.moveAsync(stepper_Spindle);
#ifdef DEBUG

	Serial.println("Running:moveAsync:");

#endif // DEBUG
	while (stepController.isRunning())
	{
		delay(5);
#ifdef DEBUG
		Serial.println("stepController:isRunning:");

#endif // DEBUG
		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);

#ifdef DEBUG
			Serial.print("Index-incomingByte:");
			Serial.println(incomingByte);
#endif // DEBUG
			switch (incomingByte)
			{
				case 67:
				case 99: // - c
				{
					stepController.stop();
					break;
				}
			}
		}
		delay(10);
	}
	// Update Index flag on Nextion. 
	Serial3.print("pageIndex.bt3.pco=59391");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageIndex.va0.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageIndex.bt3.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageIndex.bt2.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageIndex.bt1.val=0");
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

	returnSteps_Axis = 0;
	returnSteps_Spindle = stepper_Spindle.getPosition();
#ifdef SHOW_POSITION 
	degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
	Serial3.print("pageBE.t1.txt=");

	Serial3.write(0x22);
	Serial3.print(degrees_Spindle);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(10);
#endif // Show Position
	SetEnable(ID_SPINDLE, false);
}

/// <summary>
/// Sync spindle and axis
/// </summary>
/// <comment>
/// </comment>
/// <param name="directionSpindle">Counterclockwise: -1, Clockwise: 1</param>
/// <param name = "directionAxis">Counterclockwise: -1, Clockwise : 1< /param>
/// <param name ="idAxis"> Id of axis: Z = 0 or X = 1< /param>
/// <returns></returns>
void Sync(int directionSpindle, int directionAxis, int idAxis)
{
	StepControl stepController;
	Stepper stepper_Spindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
	Stepper stepper_X(PIN_AXIS_X_STEP, PIN_AXIS_X_DIR);
	Stepper stepper_Z(PIN_AXIS_Z_STEP, PIN_AXIS_Z_DIR);
	float targetSteps_Spindle = 0;

	// Set speed and acceleration
	switch (idAxis)
	{
		case ID_AXIS_X:
		{
			targetSteps_Spindle = configSync.revolutions_Spindle * configSetup.gearRatio_Spindle * configSetup.microsteps_Spindle * configSetup.steps360_Spindle * directionSpindle;
			stepper_Spindle
				.setMaxSpeed(configSync.maxSpd_Spindle * configSync.speedPercent_Spindle * .01)
				.setAcceleration(configSync.accel_Spindle)
				.setTargetRel(targetSteps_Spindle);

			stepper_Spindle.setPosition(0);
			break;
		}
		case ID_AXIS_Z:
		{
			targetSteps_Spindle = configSync.revolutions_Spindle * configSetup.gearRatio_Spindle * configSetup.microsteps_Spindle * configSetup.steps360_Spindle * directionSpindle;
			stepper_Spindle
				.setMaxSpeed(configSync.maxSpd_Spindle * configSync.speedPercent_Spindle * .01)
				.setAcceleration(configSync.accel_Spindle)
				.setTargetRel(targetSteps_Spindle);
			stepper_Spindle.setPosition(0);
			break;
		}
	}

	int32_t axisStepsZ = (((configSync.distance) / configSetup.distancePerRev_AxisZ) * (configSetup.microsteps_Axis_Z * configSetup.steps360_Axis_Z)) * directionAxis;
	stepper_Z
		.setMaxSpeed(configSync.maxSpd_Axis_Z * configSync.speedPercent_Axis_Z * .01)
		.setAcceleration(configSync.accel_Axis_Z)
		.setTargetRel(axisStepsZ);
	stepper_Z.setPosition(0);

	int32_t axisStepsX = (configSync.distance / configSetup.distancePerRev_AxisX) * configSetup.microsteps_Axis_X * configSetup.steps360_Axis_X * directionAxis;
	stepper_X
		.setMaxSpeed(configSync.maxSpd_Axis_X * configSync.speedPercent_Axis_X * .01)
		.setAcceleration(configSync.accel_Axis_X)
		.setTargetRel(axisStepsX);
	stepper_X.setPosition(0);

#ifdef DEBUG
	Serial.print("idAxis:");
	Serial.println(idAxis);

	Serial.print("targetSteps_Spindle:");
	Serial.println(targetSteps_Spindle);
	Serial.print("microsteps_Spindle:");
	Serial.println(configSetup.microsteps_Spindle);
	Serial.print("steps360_Spindle:");
	Serial.println(configSetup.steps360_Spindle);
	Serial.print("gearRatio_Spindle:");
	Serial.println(configSetup.gearRatio_Spindle);
	Serial.print("revolutionsSyncX_Spindle:");
	Serial.println(configSync.revolutions_Spindle);
	switch (idAxis)
	{
		case ID_AXIS_X:
		{

			Serial.print("SpindleMaxSpd:");
			Serial.println(configSync.maxSpd_Spindle);
			Serial.print("SpindleSpeedPercent:");
			Serial.println(configSync.speedPercent_Spindle);
			Serial.print("SpindleAccel:");
			Serial.println(configSync.accel_Spindle);
			Serial.print("revolutionsSyncX_Spindle:");
			Serial.println(configSync.revolutions_Spindle);

			Serial.print("distanceSyncX:");
			Serial.println(configSync.distance);
			Serial.print("distancePerRev_AxisX:");
			Serial.println(configSetup.distancePerRev_AxisX);
			Serial.print("steps360_Axis_X:");
			Serial.println(configSetup.steps360_Axis_X);
			Serial.print("microsteps_Axis_X:");
			Serial.println(configSetup.microsteps_Axis_X);
			Serial.print("distanceSyncX:");
			Serial.println(configSync.distance);
			Serial.print("axisStepsX:");
			Serial.println(axisStepsX);

			Serial.print("AxisMaxSpd:");
			Serial.println(configSync.maxSpd_Axis_X);
			Serial.print("speedPercentAxis_SyncX:");
			Serial.println(configSync.speedPercent_Axis_X);
			Serial.print("AxisAccel:");
			Serial.println(configSync.accel_Axis_X);
			break;
		}
		case ID_AXIS_Z:
		{
			Serial.print("SpindleMaxSpd:");
			Serial.println(configSync.maxSpd_Spindle);
			Serial.print("SpindleSpeedPercent:");
			Serial.println(configSync.speedPercent_Spindle);
			Serial.print("SpindleAccel:");
			Serial.println(configSync.accel_Spindle);
			Serial.print("revolutionsSyncZ_Spindle:");
			Serial.println(configSync.revolutions_Spindle);

			Serial.print("distanceSyncZ:");
			Serial.println(configSync.distance);
			Serial.print("distancePerRev_AxisZ:");
			Serial.println(configSetup.distancePerRev_AxisZ);
			Serial.print("steps360_Axis_Z:");
			Serial.println(configSetup.steps360_Axis_Z);
			Serial.print("microsteps_Axis_Z:");
			Serial.println(configSetup.microsteps_Axis_Z);
			Serial.print("distanceSyncZ:");
			Serial.println(configSync.distance);
			Serial.print("axisStepsZ:");
			Serial.println(axisStepsZ);

			Serial.print("AxisMaxSpd:");
			Serial.println(configSync.maxSpd_Axis_Z);
			Serial.print("speedPercentAxis_Sync_Z:");
			Serial.println(configSync.speedPercent_Axis_Z);
			Serial.print("AxisAccel:");
			Serial.println(configSync.accel_Axis_Z);
			break;
		}
	}
#endif // DEBUG

	switch (idAxis)
	{
		case ID_AXIS_X:
		{
			SetEnable(ID_SPINDLE, true);
			SetEnable(ID_AXIS_X, true);
			stepController.moveAsync(stepper_Spindle, stepper_X);
			break;
		}
		case ID_AXIS_Z:
		{
			SetEnable(ID_SPINDLE, true);
			SetEnable(ID_AXIS_Z, true);
			stepController.moveAsync(stepper_Spindle, stepper_Z);
			break;
		}
	}
	
	while (stepController.isRunning())
	{
#ifdef DEBUG

		endPosition_Spindle = stepper_Spindle.getPosition();
		switch (idAxis)
		{
			case ID_AXIS_X:
			{
				endPosition_Axis = stepper_X.getPosition();
#ifdef SHOW_POSITION 
				// Track Positions not implemented while steppers are running
				/*if (configGeneral.trackPositions == 1)
				{
					degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
					distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_X);
					Serial3.print("pageBE.t1.txt=");
					Serial3.write(0x22);
					Serial3.print(degrees_Spindle);
					Serial3.write(0x22);
					Serial3.write(0xff);
					Serial3.write(0xff);
					Serial3.write(0xff);
					delay(10);
					Serial3.print("pageBE.t2.txt=");
					Serial3.write(0x22);
					Serial3.print(distance_Axis);
					Serial3.write(0x22);
					Serial3.write(0xff);
					Serial3.write(0xff);
					Serial3.write(0xff);
				}*/
#endif // Show Position
				break;
			}
			case ID_AXIS_Z:
			{
				endPosition_Axis = stepper_Z.getPosition();
#ifdef SHOW_POSITION 
				// Track Positions not implemented while steppers are running
				/*if (configGeneral.trackPositions == 1)
				{
					degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
					distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);
					Serial3.print("pageBE.t1.txt=");
					Serial3.write(0x22);
					Serial3.print(degrees_Spindle);
					Serial3.write(0x22);
					Serial3.write(0xff);
					Serial3.write(0xff);
					Serial3.write(0xff);
					delay(10);
					Serial3.print("pageBE.t2.txt=");
					Serial3.write(0x22);
					Serial3.print(distance_Axis);
					Serial3.write(0x22);
					Serial3.write(0xff);
					Serial3.write(0xff);
					Serial3.write(0xff);
				}*/
#endif // Show Position
				break;
			}
		}

#endif // DEBUG

		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
				case 67:
				case 99: // - c
				{
					stepController.stopAsync();
					break;
				}
			}
		}

		delay(10);
	}

	returnSteps_Spindle = stepper_Spindle.getPosition();

	switch (idAxis)
	{
		case ID_AXIS_X:
		{
			endPosition_Axis = stepper_X.getPosition();

	#ifdef SHOW_POSITION 
			degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
			distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_X);

			// Update Nextion
			Serial3.print("pageBE.t1.txt=");
			Serial3.write(0x22);
			Serial3.print(degrees_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(10);
			Serial3.print("pageBE.t2.txt=");
			Serial3.write(0x22);
			Serial3.print(distance_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
	#endif // Show Position
			Serial3.print("pageSync.bt6.pco=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageSync.va0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageSync.bt6.val=0");// The first one may be ignored by Nextion, so resend.
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageSync.bt2.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageSync.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);

			SetEnable(ID_AXIS_X, false);
			break;
		}
		case ID_AXIS_Z:
		{
			endPosition_Axis = stepper_Z.getPosition();
	#ifdef SHOW_POSITION 
			degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
			distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);

			// Update Nextion
			Serial3.print("pageBE.t1.txt=");
			Serial3.write(0x22);
			Serial3.print(degrees_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(10);
			Serial3.print("pageBE.t2.txt=");
			Serial3.write(0x22);
			Serial3.print(distance_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
	#endif // Show Position
			Serial3.print("pageSync.bt6.pco=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageSync.va0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageSync.bt6.val=0");// The first one may be ignored by Nextion, so resend.
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageSync.bt2.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageSync.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
 
			SetEnable(ID_AXIS_Z, false);
			break;
		}
	}

	returnSteps_Axis = endPosition_Axis;

	stepper_Spindle.setPosition(0);
	stepper_X.setPosition(0);
	stepper_Z.setPosition(0);

	SetEnable(ID_SPINDLE, false);
#ifdef DEBUG
	Serial.print("Spindle:");
	Serial.println(endPosition_Spindle);
	Serial.print("Axis:");
	Serial.println(endPosition_Axis);

	Serial.println("End Sync()");
#endif // DEBUG
}

// ToDo: Combine Reciprocate functions
/// <summary>
/// Reciprocate_RadialZ
/// </summary>
/// <comment>
/// </comment>
/// <param name="wavDir">Counterclockwise: -1, Clockwise: 1</param>
/// <returns></returns>
void Reciprocate_RadialZ(int wavDir)
{
	StepControl stepController;
	Stepper stepper_Spindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
	Stepper stepper_Z(PIN_AXIS_Z_STEP, PIN_AXIS_Z_DIR);

	// Set speed and acceleration
	stepper_Spindle
		.setMaxSpeed(configRec.maxSpd_Spindle * configRec.speedPercent_Spindle * .01)
		.setAcceleration(configRec.accel_Spindle);
	stepper_Spindle.setPosition(0);

	float spindleDegrees = configRec.degrees_RadialZ/ (configRec.waves_RadialZ * 2);
	int spindleSteps = round((configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (spindleDegrees / 360) * wavDir);
	returnSteps_Spindle = spindleSteps* configRec.waves_RadialZ * 2;

	stepper_Z
		.setMaxSpeed(configRec.maxSpd_Axis_Z * configRec.speedPercent_Axis_Z * .01)
		.setAcceleration(configRec.accel_Axis_Z);
	stepper_Z.setPosition(0);
	float axisDistance = configRec.amplitude_RadialZ;
	int axisSteps = round((axisDistance / configSetup.distancePerRev_AxisZ) * configSetup.microsteps_Axis_Z * configSetup.steps360_Axis_Z * wavDir);

#ifdef DEBUG
	Serial.print("SpindleMaxSpd:");
	Serial.println(configRec.maxSpd_Spindle);
	Serial.print("SpindleSpeedPercent:");
	Serial.println(configRec.speedPercent_Spindle);
	Serial.print("SpindleAccel:");
	Serial.println(configRec.accel_Spindle);

	Serial.print("microsteps_Spindle:");
	Serial.println(configSetup.microsteps_Spindle);
	Serial.print("steps360_Spindle:");
	Serial.println(configSetup.steps360_Spindle);
	Serial.print("gearRatio_Spindle:");
	Serial.println(configSetup.gearRatio_Spindle);
	Serial.print("amplitude_AxialZ:");
	Serial.println(configRec.amplitude_AxialZ);

	Serial.print("distancePerRev_AxisZ:");
	Serial.println(configSetup.distancePerRev_AxisZ);
	Serial.print("steps360_Axis_Z:");
	Serial.println(configSetup.steps360_Axis_Z);
	Serial.print("microsteps_Axis_Z:");
	Serial.println(configSetup.microsteps_Axis_Z);
	Serial.print("amplitude_RadialZ:");
	Serial.println(configRec.amplitude_RadialZ);
	Serial.print("waves_RadialZ:");
	Serial.println(configRec.waves_RadialZ);

	Serial.print("spindleSteps:");
	Serial.println(spindleSteps);
	Serial.print("axisSteps:");
	Serial.println(axisSteps);

	Serial.print("AxisMaxSpd:");
	Serial.println(configRec.maxSpd_Axis_Z);
	Serial.print("AxisSpeedPercent:");
	Serial.println(configRec.speedPercent_Axis_Z);
	Serial.print("AxisAccel:");
	Serial.println(configRec.accel_Axis_Z);
#endif // DEBUG

	SetEnable(ID_SPINDLE, true);
	SetEnable(ID_AXIS_Z, true);
	for (int i = 0; i < (configRec.waves_RadialZ * 2); i++)
	{
		axisSteps *= -1;

		stepper_Spindle.setTargetRel(spindleSteps);
		stepper_Z.setTargetRel(axisSteps);

		stepController.moveAsync(stepper_Spindle, stepper_Z);

		while (stepController.isRunning())
		{
#ifdef DEBUG
			endPosition_Axis = stepper_Z.getPosition();
			endPosition_Spindle = stepper_Spindle.getPosition();
			Serial.print("Spindle:");
			Serial.println(endPosition_Spindle);
			Serial.print("Axis:");
			Serial.println(endPosition_Axis);
#endif // DEBUG
#ifdef SHOW_POSITION 
			// Track Positions not implemented while steppers are running
			/*if (configGeneral.trackPositions == 1)
			{

				degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
				distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);
				Serial3.print("pageBE.t1.txt=");
				Serial3.write(0x22);
				Serial3.print(degrees_Spindle);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				delay(10);
				Serial3.print("pageBE.t2.txt=");
				Serial3.write(0x22);
				Serial3.print(distance_Axis);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
			}*/
#endif // Show Position
			// Check for Cancel code  
			if (SerialAvailable() > 0)
			{
				incomingByte = SerialRead(serialId);
				switch (incomingByte)
				{
					case 67:
					case 99: // - c
					{
						stepController.stop();
						goto endLoop;
						break;
					}
					default:
					{
						break;
					}
				}
			}

			delay(10);
		}
	}

endLoop:

	endPosition_Axis = stepper_Z.getPosition();
	endPosition_Spindle = stepper_Spindle.getPosition();
#ifdef SHOW_POSITION 
	degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
	distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);
	Serial3.print("pageBE.t1.txt=");
	Serial3.write(0x22);
	Serial3.print(degrees_Spindle);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(10);
	Serial3.print("pageBE.t2.txt=");
	Serial3.write(0x22);
	Serial3.print(distance_Axis);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
#endif // Show Position
	// Update Nextion
	Serial3.print("pageRec.bt3.pco=0");// The first one may be ignored by Nextion, so resend.
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRec.va0.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRec.bt3.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRec.bt2.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRec.bt1.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);

	returnSteps_Axis = endPosition_Axis;
	returnSteps_Spindle = endPosition_Spindle;

#ifdef DEBUG

	Serial.print("Spindle:");
	Serial.println(endPosition_Spindle);
	Serial.print("Axis:");
	Serial.println(endPosition_Axis);
#endif // DEBUG

	SetEnable(ID_SPINDLE, false);
	SetEnable(ID_AXIS_Z, false);

}

/// <summary>
/// Reciprocate_RadialX
/// </summary>
/// <comment>
/// </comment>
/// <param name="wavDir">Counterclockwise: -1, Clockwise: 1</param>
/// <returns></returns>
void Reciprocate_RadialX(int wavDir)
{
	int stepsPerRevolution_Spindle = (int)(round(configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle));
	StepControl stepController;
	Stepper stepper_Spindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
	Stepper stepper_X(PIN_AXIS_X_STEP, PIN_AXIS_X_DIR);

	// Set speed and acceleration
	stepper_Spindle
		.setMaxSpeed(configRec.maxSpd_Spindle * configRec.speedPercent_Spindle * .01)
		.setAcceleration(configRec.accel_Spindle);
	stepper_Spindle.setPosition(0);

	float spindleDegrees = configRec.degrees_RadialX / (configRec.waves_RadialX * 2);
	int spindleSteps = (int)round((stepsPerRevolution_Spindle) * (spindleDegrees / 360) * wavDir);
	returnSteps_Spindle = spindleSteps * configRec.waves_RadialX * 2;


#ifdef DEBUG
	Serial.print("stepsPerRevolution_Spindle:");
	Serial.println(stepsPerRevolution_Spindle);
	Serial.print("degrees_RadialX:");
    Serial.println(configRec.degrees_RadialX);
	Serial.print("waves_RadialX  x 2:");
	Serial.println(configRec.waves_RadialX * 2);
#endif


	stepper_X
		.setMaxSpeed(configRec.maxSpd_Axis_X * configRec.speedPercent_Axis_X * .01)
		.setAcceleration(configRec.accel_Axis_X);
	stepper_X.setPosition(0);
	float axisDistance = configRec.amplitude_RadialX;
	int axisSteps = round((axisDistance / configSetup.distancePerRev_AxisX) * configSetup.microsteps_Axis_X * configSetup.steps360_Axis_X * wavDir);

#ifdef DEBUG
	Serial.print("amplitude_AxialX:");
	Serial.println(configRec.amplitude_AxialX);

	Serial.print("distancePerRev_AxisX:");
	Serial.println(configSetup.distancePerRev_AxisX);
	Serial.print("steps360_Axis_X:");
	Serial.println(configSetup.steps360_Axis_X);
	Serial.print("microsteps_Axis_X:");
	Serial.println(configSetup.microsteps_Axis_X);
	Serial.print("amplitude_RadialX:");
	Serial.println(configRec.amplitude_RadialX);
	Serial.print("waves_RadialX:");
	Serial.println(configRec.waves_RadialX);

	Serial.print("spindleSteps:");
	Serial.println(spindleSteps);
	Serial.print("axisSteps:");
	Serial.println(axisSteps);

	Serial.print("maxSpd_Axis_X:");
	Serial.println(configRec.maxSpd_Axis_X);
	Serial.print("speedPercent_Axis_X:");
	Serial.println(configRec.speedPercent_Axis_X);
	Serial.print("accel_Axis_X:");
	Serial.println(configRec.accel_Axis_X);
#endif // DEBUG

	SetEnable(ID_SPINDLE, true);
	SetEnable(ID_AXIS_X, true);
	for (int i = 0; i < (configRec.waves_RadialX * 2); i++)
	{
		axisSteps *= -1;

		stepper_Spindle.setTargetRel(spindleSteps);
		stepper_X.setTargetRel(axisSteps);

		stepController.moveAsync(stepper_Spindle, stepper_X);

		while (stepController.isRunning())
		{
#ifdef DEBUG
			//endPosition_Axis = stepper_X.getPosition();
			//endPosition_Spindle = stepper_Spindle.getPosition();
			//Serial.print("Spindle:");
			//Serial.println(endPosition_Spindle);
			//Serial.print("Axis:");
			//Serial.println(endPosition_Axis);
#endif // DEBUG
#ifdef SHOW_POSITION 
			// Track Positions not implemented while steppers are running
			/*if (configGeneral.trackPositions == 1)
			{
				endPosition_Axis = stepper_X.getPosition();
				endPosition_Spindle = stepper_Spindle.getPosition();
				degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
				distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_X);
				Serial3.print("pageBE.t1.txt=");
				Serial3.write(0x22);
				Serial3.print(degrees_Spindle);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				delay(10);
				Serial3.print("pageBE.t2.txt=");
				Serial3.write(0x22);
				Serial3.print(distance_Axis);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
			}*/
#endif // Show Position
			// Check for Cancel code  
			if (SerialAvailable() > 0)
			{
				incomingByte = SerialRead(serialId);
				switch (incomingByte)
				{
					case 67:
					case 99: // - c
					{
						stepController.stop();
						goto endLoop;
						break;
					}
					default:
					{
						break;
					}
				}
			}

			delay(10);
		}
	}

endLoop:

	endPosition_Axis = stepper_X.getPosition();
	endPosition_Spindle = stepper_Spindle.getPosition();
#ifdef SHOW_POSITION 
	degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
	distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_X);
	Serial3.print("pageBE.t1.txt=");
	Serial3.write(0x22);
	Serial3.print(degrees_Spindle);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(10);
	Serial3.print("pageBE.t2.txt=");
	Serial3.write(0x22);
	Serial3.print(distance_Axis);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
#endif // Show Position
	// Update Nextion
	Serial3.print("pageRec.bt3.pco=0");// The first one may be ignored by Nextion, so resend.
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRec.va0.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRec.bt3.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRec.bt2.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRec.bt1.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);

	returnSteps_Axis = endPosition_Axis;
	returnSteps_Spindle = endPosition_Spindle;

#ifdef DEBUG
	Serial.print("Spindle:");
	Serial.println(endPosition_Spindle);
	Serial.print("Axis:");
	Serial.println(endPosition_Axis);
#endif // DEBUG

	SetEnable(ID_SPINDLE, false);
	SetEnable(ID_AXIS_X, false);
}

/// <summary>
/// Reciprocate_AxialZ
/// </summary>
/// <comment>
/// </comment>
/// <param name="wavDir">In: -1, Out: 1</param>
/// <returns></returns>
void Reciprocate_AxialZ(int wavDir)
{
	StepControl stepController;
	Stepper stepper_Spindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
	Stepper stepper_Z(PIN_AXIS_Z_STEP, PIN_AXIS_Z_DIR);
	// Set speed and acceleration
	stepper_Spindle
		.setMaxSpeed(configRec.maxSpd_Spindle * configRec.speedPercent_Spindle * .01)
		.setAcceleration(configRec.accel_Spindle);
	stepper_Spindle.setPosition(0);

	int spindleSteps = (configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (configRec.amplitude_AxialZ / 360) * wavDir;

	long axisSteps = (((configRec.distance_AxialZ / (configSetup.distancePerRev_AxisZ)) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z)) / (configRec.waves_AxialZ * 2));
	returnSteps_Axis = (configRec.distance_AxialZ / configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z);

#ifdef DEBUG
	Serial.print("1-wavDir:");
	Serial.println(wavDir);
	Serial.print("1-axisSteps:");
	Serial.println(axisSteps);
#endif // DEBUG

	if (wavDir != DIR_CW)
	{
		axisSteps = -axisSteps;
	}

#ifdef DEBUG
	Serial.print("2-wavDir:");
	Serial.println(wavDir);
	Serial.print("2-axisSteps:");
	Serial.println(axisSteps);
#endif // DEBUG
	stepper_Z
		.setMaxSpeed(configRec.maxSpd_Axis_Z * configRec.speedPercent_Axis_Z * .01)
		.setAcceleration(configRec.accel_Axis_Z);
	stepper_Z.setPosition(0);

#ifdef DEBUG
	Serial.print("waves_AxialZ:");
	Serial.println(configRec.waves_AxialZ);
	Serial.print("spindleSteps:");
	Serial.println(spindleSteps);
	Serial.println("========================");
	Serial.print("axisSteps:");
	Serial.println(axisSteps);
	Serial.println("+++++++++++++++++++++++++++");

#endif // DEBUG

	SetEnable(ID_SPINDLE, true);
	SetEnable(ID_AXIS_Z, true);
	for (int i = 0; i < (configRec.waves_AxialZ * 2); i++)
	{
		spindleSteps *= -1;

		stepper_Spindle.setTargetRel(spindleSteps);
		stepper_Z.setTargetRel(axisSteps);

		stepController.moveAsync(stepper_Spindle, stepper_Z);
		delay(10);

		while (stepController.isRunning())
		{
#ifdef DEBUG
			endPosition_Axis = stepper_Z.getPosition();
			endPosition_Spindle = stepper_Spindle.getPosition();
			Serial.print("Spindle:");
			Serial.println(endPosition_Spindle);
			Serial.print("Axis:");
			Serial.println(endPosition_Axis);
#endif // DEBUG
//#ifdef SHOW_POSITION 
//			if (configMain.trackPositions == 1)
//			{
//				endPosition_Axis = stepper_Z.getPosition();
//				endPosition_Spindle = stepper_Spindle.getPosition();
//				degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
//				distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);
//				Serial3.print("pageBE.t1.txt=");
//				Serial3.write(0x22);
//				Serial3.print(degrees_Spindle);
//				Serial3.write(0x22);
//				Serial3.write(0xff);
//				Serial3.write(0xff);
//				Serial3.write(0xff);
//				delay(10);
//				Serial3.print("pageBE.t2.txt=");
//				Serial3.write(0x22);
//				Serial3.print(distance_Axis);
//				Serial3.write(0x22);
//				Serial3.write(0xff);
//				Serial3.write(0xff);
//				Serial3.write(0xff);
//			}
//#endif //Show position

			// Check for Cancel code  
			if (SerialAvailable() > 0)
			{
				incomingByte = SerialRead(serialId);
				switch (incomingByte)
				{
					case 67:
					case 99: // - c
					{
						stepController.stop();
						goto endLoop;
						break;
					}
					default:
					{
						break;
					}
				}
			}
		}
	}

endLoop:

	endPosition_Axis = stepper_Z.getPosition();
	endPosition_Spindle = stepper_Spindle.getPosition();
#ifdef SHOW_POSITION 
	degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
	distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);
	Serial3.print("pageBE.t1.txt=");
	Serial3.write(0x22);
	Serial3.print(degrees_Spindle);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(10);
	Serial3.print("pageBE.t2.txt=");
	Serial3.write(0x22);
	Serial3.print(distance_Axis);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
#endif //Show position

	// Update Nextion
	Serial3.print("pageRec.bt3.pco=59391");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRec.va0.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRec.bt3.val=0");// The first one may be ignored by Nextion, so resend.
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRec.bt2.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRec.bt1.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);

#ifdef DEBUG
	endPosition_Axis = stepper_Z.getPosition();
	endPosition_Spindle = stepper_Spindle.getPosition();
	Serial.print("Spindle:");
	Serial.println(endPosition_Spindle);
	Serial.print("Axis:");
	Serial.println(endPosition_Axis);
#endif // DEBUG
	returnSteps_Axis = endPosition_Axis;
	returnSteps_Spindle = endPosition_Spindle;
	SetEnable(ID_SPINDLE, false); 
	SetEnable(ID_AXIS_Z, false);
}

/// <summary>
/// Reciprocate_AxialX
/// </summary>
/// <comment>
/// </comment>
/// <param name="wavDir">In: -1, Out: 1</param>
/// <returns></returns>
void Reciprocate_AxialX(int wavDir)
{
	StepControl stepController;
	Stepper stepper_Spindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
	Stepper stepper_X(PIN_AXIS_X_STEP, PIN_AXIS_X_DIR);

	// Set speed and acceleration
	stepper_Spindle
		.setMaxSpeed(configRec.maxSpd_Spindle * configRec.speedPercent_Spindle * .01)
		.setAcceleration(configRec.accel_Spindle);
	stepper_Spindle.setPosition(0);

int spindleSteps = (configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (configRec.amplitude_AxialX / 360) * wavDir;
	long axisSteps = (((configRec.distance_AxialX / (configSetup.distancePerRev_AxisX)) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X)) / (configRec.waves_AxialX * 2)) * wavDir;
	returnSteps_Axis = (configRec.distance_AxialX / configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X);

	stepper_X
		.setMaxSpeed(configRec.maxSpd_Axis_X * configRec.speedPercent_Axis_X * .01)
		.setAcceleration(configRec.accel_Axis_X);
	stepper_X.setPosition(0);

#ifdef DEBUG
	Serial.print("SpindleMaxSpd:");
	Serial.println(configRec.maxSpd_Spindle);
	Serial.print("SpindleSpeedPercent:");
	Serial.println(configRec.speedPercent_Spindle);
	Serial.print("SpindleAccel:");
	Serial.println(configRec.accel_Spindle);

	Serial.print("microsteps_Spindle:");
	Serial.println(configSetup.microsteps_Spindle);
	Serial.print("steps360_Spindle:");
	Serial.println(configSetup.steps360_Spindle);
	Serial.print("gearRatio_Spindle:");
	Serial.println(configSetup.gearRatio_Spindle);
	Serial.print("amplitude_AxialX:");
	Serial.println(configRec.amplitude_AxialX);

	Serial.print("distancePerRev_AxisX:");
	Serial.println(configSetup.distancePerRev_AxisX);
	Serial.print("steps360_Axis_X:");
	Serial.println(configSetup.steps360_Axis_X);
	Serial.print("microsteps_Axis_X:");
	Serial.println(configSetup.microsteps_Axis_X);
	Serial.print("distance_AxialX:");
	Serial.println(configRec.distance_AxialX);
	Serial.print("waves_AxialX:");
	Serial.println(configRec.waves_AxialX);

	Serial.print("spindleSteps:");
	Serial.println(spindleSteps);
	Serial.print("axisSteps:");
	Serial.println(axisSteps);

	Serial.print("AxisMaxSpd:");
	Serial.println(configRec.maxSpd_Axis_X);
	Serial.print("AxisSpeedPercent:");
	Serial.println(configRec.speedPercent_Axis_X);
	Serial.print("AxisAccel:");
	Serial.println(configRec.accel_Axis_X);
#endif // DEBUG

	SetEnable(ID_SPINDLE, true);
	SetEnable(ID_AXIS_X, true);
	for (int i = 0; i < (configRec.waves_AxialX * 2); i++)
	{
		spindleSteps *= -1;

		stepper_Spindle.setTargetRel(spindleSteps);
		stepper_X.setTargetRel(axisSteps);

		stepController.moveAsync(stepper_Spindle, stepper_X);
		delay(5);

		while (stepController.isRunning())
		{
#ifdef DEBUG
			//endPosition_Axis = stepper_X.getPosition();
			//endPosition_Spindle = stepper_Spindle.getPosition();
			//Serial.print("Spindle:");
			//Serial.println(endPosition_Spindle);
			//Serial.print("Axis:");
			//Serial.println(endPosition_Axis);
#endif // DEBUG
#ifdef SHOW_POSITION 
			// Track Positions not implemented while steppers are running
			/*if (configGeneral.trackPositions == 1)
			{
				endPosition_Axis = stepper_X.getPosition();
				endPosition_Spindle = stepper_Spindle.getPosition();
				degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
				distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_X);
				Serial3.print("pageBE.t1.txt=");
				Serial3.write(0x22);
				Serial3.print(degrees_Spindle);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				delay(10);
				Serial3.print("pageBE.t2.txt=");
				Serial3.write(0x22);
				Serial3.print(distance_Axis);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
			}*/
#endif //SHOW_POSITION

			// Check for Cancel code  
			if (SerialAvailable() > 0)
			{
				incomingByte = SerialRead(serialId);
				switch (incomingByte)
				{
					case 67:
					case 99: // - c
					{
						stepController.stop();
						goto endLoop;
						break;
					}
					default:
					{
						break;
					}
				}
			}
		}
	}

endLoop:

	endPosition_Axis = stepper_X.getPosition();
	endPosition_Spindle = stepper_Spindle.getPosition();
#ifdef SHOW_POSITION 
	degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
	distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_X);
	Serial3.print("pageBE.t1.txt=");
	Serial3.write(0x22);
	Serial3.print(degrees_Spindle);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(10);
	Serial3.print("pageBE.t2.txt=");
	Serial3.write(0x22);
	Serial3.print(distance_Axis);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
#endif //SHOW_POSITION
	// Update Nextion
	Serial3.print("pageRec.bt3.pco=59391");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRec.va0.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRec.bt3.val=0");// The first one may be ignored by Nextion, so resend.
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRec.bt2.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRec.bt1.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);

#ifdef DEBUG
	endPosition_Axis = stepper_X.getPosition();
	endPosition_Spindle = stepper_Spindle.getPosition();
	Serial.print("Spindle:");
	Serial.println(endPosition_Spindle);
	Serial.print("Axis:");
	Serial.println(endPosition_Axis);
#endif // DEBUG
	returnSteps_Axis = endPosition_Axis;
	returnSteps_Spindle = endPosition_Spindle;
	SetEnable(ID_SPINDLE, false);
	SetEnable(ID_AXIS_X, false);
}


/// <summary>
/// StopGreekKey
/// </summary>
/// <comment>
/// </comment>
/// <returns>true or false</returns>
bool StopGreekKey()
{
	bool retVal = false;
	if (SerialAvailable() >= 0)
	{
		incomingByte = SerialRead(serialId);
		switch (incomingByte)
		{
			case 67: 
			case 99: // - c
			{
				retVal = true;
				SetEnable(ID_SPINDLE, false);
				switch (configGreekKey_Main.axisId)
				{
				case ID_AXIS_Z: // Z Axis
				{
					SetEnable(ID_AXIS_X, false);
				}
				case ID_AXIS_X: // X Axis
				{
					SetEnable(ID_AXIS_X, false);
				}
				}
				break;
			}
		}
	}

	return retVal;
}

/// <summary>
/// GreekKey_Pattern_4a
/// </summary>
/// <comment>
/// </comment>
/// <returns></returns>
void GreekKey_Pattern_4a()
{
	bool stopAll = false;
	int totalSpindleSegments = 8;
	if (configGreekKey_Main.radialOrAxial_Pattern == AXIAL)
	{
		totalSpindleSegments = 4;
	}
	GreekKeyPattern_Initial(totalSpindleSegments);

	for (int i = 1; i <= configGreekKey_Main.countPattern;i++)// / 2; i++)
	{
		switch (configGreekKey_Main.radialOrAxial_Pattern)
		{
			case RADIAL: // Axis Left CCW
			{
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CW); //1
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //2
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW); //3
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //4
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW); //5
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //6
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CW); // 7
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 4, DIR_CW); //8
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 8, DIR_CCW); //9
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CCW); //10
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CW); //11
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //12
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //13
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); // 14
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CW); //15
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CCW); //16
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 4, DIR_CW); //17
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 8, DIR_CCW); //18
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				break;
			}
			case AXIAL: // Spindle Up CCW
			{
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CW); //1
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW); //2
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //3
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //4
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //5
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW); //6
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CW); //7
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 4, DIR_CW); //7
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 8, DIR_CCW); //9
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CCW); //10
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CW); //11
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //12
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW); //13
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //14
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CW); //15
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CCW); //16
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 4, DIR_CW); //17
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 8, DIR_CCW); //18
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				break;
			}

		}
	}

EndLoop:

	GreekKeyPattern_End();

	return;
}

/// <summary>
/// GreekKey_Pattern_4b
/// </summary>
/// <comment>
/// </comment>
/// <returns></returns>
void GreekKey_Pattern_4b()
{

	bool stopAll = false;
	int totalSpindleSegments = 13;
	if (configGreekKey_Main.radialOrAxial_Pattern == AXIAL)
	{
		totalSpindleSegments = 4;
	}
	GreekKeyPattern_Initial(totalSpindleSegments);

	for (int i = 1; i <= configGreekKey_Main.countPattern; i++)
	{
		switch (configGreekKey_Main.radialOrAxial_Pattern)
		{
			case RADIAL: // Axis Left CCW
			{
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //1-U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CW); //2-L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //3 U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW); //4 R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //5 D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW); //6 R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CCW); //7 U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); // 8 L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //9 D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //10 L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //11 U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CCW); //12 R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 6, DIR_CW); //13 D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 4, DIR_CW); //14 L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 7, DIR_CCW); //15 U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CCW); //16 R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //17 U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //18 L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //19 D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //20
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CCW); //21
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW); //22
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //23
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW); //24
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //25
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CW); //26
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 5, DIR_CW); //27
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 6, DIR_CCW); //28
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 4, DIR_CCW); //29
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 7, DIR_CW); //30
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 7, DIR_CCW); //31
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				break;
			}
			case AXIAL: // Spindle Up CCW
			{

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW); //1-U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CW); //2-L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW); //3 U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //4 R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //5 D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //6 R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CCW); //7 U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); // 8 L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //9 D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //10 L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW); //11 U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CCW); //12 R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 6, DIR_CW); //13 D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 4, DIR_CW); //14 L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 7, DIR_CCW); //15 U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CCW); //16 R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW); //17 U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //18 L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //19 D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //20
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CCW); //21
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //22
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //23
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //24
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW); //25
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CW); //26
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 5, DIR_CW); //27
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 6, DIR_CCW); //28
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 4, DIR_CCW); //29
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 7, DIR_CW); //30
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 7, DIR_CCW); //31
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				break;
			}

		}
	}

EndLoop:

	GreekKeyPattern_End();

	return;
}

/// <summary>
/// GreekKey_Pattern_3a
/// </summary>
/// <comment>
/// </comment>
/// <returns></returns>
void GreekKey_Pattern_3a()
{
	bool stopAll = false;
	int totalSpindleSegments = 4;
	if (configGreekKey_Main.radialOrAxial_Pattern == AXIAL)
	{
		totalSpindleSegments = 3;
	}
	GreekKeyPattern_Initial(totalSpindleSegments);

	for (int i = 1; i <= configGreekKey_Main.countPattern; i++)
	{

		switch (configGreekKey_Main.radialOrAxial_Pattern)
		{
			case RADIAL: // Axis Left CCW
			{
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CW); //1
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CCW); //2
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW); //3
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //4
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //5
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //6
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW); //7
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CCW); //8
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				break;
			}
			case AXIAL: // Spindle Up CCW
			{
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CW); //1
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CCW); //2
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //3
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //4
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //5
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //6
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //7
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CCW); //8
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				break;
			}

		}

	}

EndLoop:
	GreekKeyPattern_End();

	return;
}

/// <summary>
/// GreekKey_Pattern_3a
/// </summary>
/// <comment>
/// </comment>
/// <returns></returns>
void GreekKey_Pattern_3b()
{
	bool stopAll = false;
	int totalSpindleSegments = 8;
	if (configGreekKey_Main.radialOrAxial_Pattern == AXIAL)
	{
		totalSpindleSegments = 3;
	}
	GreekKeyPattern_Initial(totalSpindleSegments);

	for (int i = 1; i <= configGreekKey_Main.countPattern; i++)
	{
		switch (configGreekKey_Main.radialOrAxial_Pattern)
		{
			case RADIAL: // Axis Left CCW
			{
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CW); //1  3R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CCW); //2 3U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW); //3 2L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //4 1D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //5 1R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //6 1D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW); //7 2L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 5, DIR_CCW); //8 5U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CW); //9 2R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //10 1D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW); //11 1L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //12 1D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CW); //13 2R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CCW); //14 3U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CCW); //15 3L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //16 1U

				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				break;
			}
			case AXIAL: // Spindle Up CCW
			{

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CW); //1
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
	
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CCW); //2
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //3
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //4
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //5
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //6
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //7
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 5, DIR_CCW); //8
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CW); //9
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //10
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //11
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //12
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CW); //13
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CCW); //14
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CCW); //15
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW); //16
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				break;
			}

		}
	}

EndLoop:

	GreekKeyPattern_End();
	return;
}

/// <summary>
/// GreekKey_Pattern_2a
/// </summary>
/// <comment>
/// </comment>
/// <returns></returns>
void GreekKey_Pattern_2a()
{
	bool stopAll = false;
	int totalSpindleSegments = 3;
	if (configGreekKey_Main.radialOrAxial_Pattern == AXIAL)
	{
		totalSpindleSegments = 2;
	}

	GreekKeyPattern_Initial(totalSpindleSegments);
#ifdef DEBUG
	Serial.print("1.axisShortLegSteps:");
	Serial.println(axisShortLegSteps);
#endif // DEBUG

	for (int i = 1; i <= configGreekKey_Main.countPattern; i++)
	{
#ifdef DEBUG
		Serial.print("----------------i:");
		Serial.println(i);
#endif // DEBUG

		switch (configGreekKey_Main.radialOrAxial_Pattern)
		{
			case RADIAL: // Axis Left CCW
			{
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CW); //0a
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //0b
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CW); //1
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //2
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW); //3
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //4
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW); //5
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //6
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				break;
			}
			case AXIAL: // Spindle Up CCW
			{
#ifdef DEBUG
				Serial.print("2.axisShortLegSteps:");
				Serial.println(axisShortLegSteps);
#endif // DEBUG


				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CW); //0a
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
			
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW); //0b
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
			
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CW); //1
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
			
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW); //2
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
			
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //3
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
			
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW); //4
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
			
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //5
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
			
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW); //6
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				break;
			}

		}
	}

EndLoop:

	GreekKeyPattern_End();

	return;
}

/// <summary>
/// GreekKey_Pattern_2b
/// </summary>
/// <comment>
/// </comment>
/// <returns></returns>
void GreekKey_Pattern_2b()
{
	bool stopAll = false;
	int totalSpindleSegments = 6;
	if (configGreekKey_Main.radialOrAxial_Pattern == AXIAL)
	{
		totalSpindleSegments = 2;
	}
	GreekKeyPattern_Initial(totalSpindleSegments);

	for (int i = 1; i <= configGreekKey_Main.countPattern; i++)
	{

		switch (configGreekKey_Main.radialOrAxial_Pattern)
		{
			case RADIAL: // Axis Left CCW
			{
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //0a
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //0b
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;		
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CW); //1
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //2
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW); //3
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;		
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //4
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW); //5
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;		
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CCW); //6
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); // 7
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;	
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //8
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //9
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;			
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //10
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;		
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW); //11
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;		
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //12
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;		
				}
				break;
			}
			case AXIAL: // Spindle Up CCW
			{
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //0a
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW); //0b
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //1
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW); //2
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //3
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //4
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //5
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CCW); //6
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //7
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); // 8
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //9
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW); //10
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //11
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW); //12
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				break;
			}
		}
	} // End For

EndLoop:

	GreekKeyPattern_End();
	return;
}


void GreekKeyPattern_Initial(int segmentCount)
{
	// Reset end positions
	endPosition_Spindle = 0;
	endPosition_Axis = 0;

	// Set spindle segment steps
	float revolutions_Spindle = configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle;
	spindleShortLegSteps = (int)round((revolutions_Spindle) / (configGreekKey_Main.countPattern360) / segmentCount);

	// Set axis segment steps
	float revolutions_Axis = 0;
	switch (configGreekKey_Main.axisId)
	{
		case ID_AXIS_Z: // Z Axis
		{
			revolutions_Axis = configGreekKey_Main.segmentLengthPattern / configSetup.distancePerRev_AxisZ;
			axisShortLegSteps = (int)round(revolutions_Axis * configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z);
			break;
		}
		case ID_AXIS_X: // X Axis
		{
			revolutions_Axis = configGreekKey_Main.segmentLengthPattern / configSetup.distancePerRev_AxisX;
			axisShortLegSteps = (int)round(revolutions_Axis * configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X);
			break;
		}
	}

#ifdef DEBUG
	Serial.print("spindleShortLegSteps:");
	Serial.println(spindleShortLegSteps);
	Serial.print("configGreekKey_Main.countPattern360:");
	Serial.println(configGreekKey_Main.countPattern360);

	Serial.print("configGreekKey_Main.segmentLengthPattern:");
	Serial.println(configGreekKey_Main.segmentLengthPattern);
	Serial.print("0.axisShortLegSteps:");
	Serial.println(axisShortLegSteps);
	Serial.println("--------------------");
#endif // DEBUG
}

void GreekKeyPattern_End()
{
	// Update Nextion
	Serial3.print("pageGrk.bt3.pco=0");// The first one may be ignored by Nextion, so resend.
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageGrk.va0.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageGrk.bt3.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageGrk.bt2.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageGrk.bt1.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);

	degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
	switch (configGreekKey_Main.axisId)
	{
		case ID_AXIS_Z: // Z Axis
		{
			distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);
			break;
		}
		case ID_AXIS_X: // X Axis
		{
			distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_X);
			break;
		}
	}

	returnSteps_Axis = endPosition_Axis;
	returnSteps_Spindle = endPosition_Spindle;

#ifdef DEBUG
	Serial.print("Spindle:");
	Serial.println(endPosition_Spindle);
	Serial.print("Axis:");
	Serial.println(endPosition_Axis);
#endif // DEBUG

	SetEnable(ID_SPINDLE, false);
	switch (configGreekKey_Main.axisId)
	{
		case ID_AXIS_Z: // Z Axis
		{
			SetEnable(ID_AXIS_X, false);
		}
		case ID_AXIS_X: // X Axis
		{
			SetEnable(ID_AXIS_X, false);
		}
	}
}

/// <summary>
/// GreekKey_Move_Axis
/// </summary>
/// <comment>
/// Selects axis to move
/// </comment>
/// <param name="shortLegLength">Segment(leg) length</param>
/// <param name="multiplier">Number of segments</param>
/// <param name="direction">Direction to move</param>
/// <returns></returns>
bool GreekKey_Move_Axis(float shortLegLength, float multiplier, int direction)
{
	bool retVal = false;
	int newDirection = direction * reverseDirection;

	Serial.print("Enter GreekKey_Move_Axis.......................:");
	Serial.println(configGreekKey_Main.axisId);
	Serial.print("Move_Axis:shortLegLength ");
	Serial.println(shortLegLength);
	Serial.print("Move_Axis:multiplier ");
	Serial.println(multiplier);
	Serial.print("Move_Axis:newDirection ");
	Serial.println(newDirection);
	if (configGreekKey_Main.axisId == ID_AXIS_Z)
	{	
		retVal = GreekKey_Move_Z(shortLegLength, multiplier, newDirection, true);
	}
	else
	{
		retVal = GreekKey_Move_X(shortLegLength, multiplier, newDirection, true);
	}

	Serial.println("Exit GreekKey_Move_Axis");
	return retVal;
}

/// <summary>
/// GreekKey_Move_Axis
/// </summary>
/// <comment>
/// Used to move cutter away from and to workpiece
/// </comment>
/// <param name="distance">Distance to move</param>
/// <param name="direction">Direction to move</param>
/// <returns></returns>
bool GreekKey_Move_Axis_Alt(float distance, float multiplier, int direction)
{
	bool retVal = false;

	// Move the opposite axis. Note: Moves cutter into or out of workpiece.
	if (configGreekKey_Main.axisId == ID_AXIS_X)
	{
		retVal = GreekKey_Move_Z(distance, multiplier, direction, false);
		SetEnable(ID_AXIS_X, false);
	}
	else
	{
		retVal = GreekKey_Move_X(distance, multiplier, direction, false);
		SetEnable(ID_AXIS_X, false);
	}

	Serial.println("Exit GreekKey_Move_Axis_Alt");
	return retVal;
}

/// <summary>
/// GreekKey_Move_Spindle
/// </summary>
/// <comment>
/// </comment>
/// <param name="shortLegLength">Segment(leg) length</param>
/// <param name="multiplier">Number of segments</param>
/// <param name="direction">Direction to move</param>
/// <returns>True if stop button touched otherwise false</returns>
bool GreekKey_Move_Spindle(float shortLegLength, float multiplier, int direction)
{
	Serial.println("Enter GreekKey_Move_Spindle.......................");
	float nextSpeed = 0;
	bool retVal = false;

	accelStep_Spindle.setCurrentPosition(0);
	SetEnable(ID_SPINDLE, true);

	int32_t targetSteps = shortLegLength * multiplier * direction;

	accelStep_Spindle.moveTo(targetSteps);

	if (configGreekKey_Main.axisId == ID_AXIS_Z) // Z axis
	{
		accelStep_Spindle.setMaxSpeed(configGreekKey_Z.maxSpd_Spindle * configGreekKey_Z.speedPercent_Spindle * .01);
		accelStep_Spindle.setAcceleration(configGreekKey_Z.accel_Spindle);
	}
	else
	{
		accelStep_Spindle.setMaxSpeed(configGreekKey_X.maxSpd_Spindle * configGreekKey_X.speedPercent_Spindle * .01);
		accelStep_Spindle.setAcceleration(configGreekKey_X.accel_Spindle);
	}

#ifdef DEBUG
	Serial.print("accelStep_Spindle.distanceToGo():");
	Serial.println(accelStep_Spindle.distanceToGo());
	Serial.print("shortLegLength:");
	Serial.println(shortLegLength);
	Serial.print("multiplier:");
	Serial.println(multiplier);
	Serial.print("direction:");
	Serial.println(direction);
	Serial.print("Target:");
	Serial.println(shortLegLength * multiplier * direction);
#endif // DEBUG

	while ((accelStep_Spindle.distanceToGo() != 0))
	{
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
				case 67:
				case 99: // - c
				{
					endPosition_Spindle = endPosition_Spindle + accelStep_Spindle.currentPosition();
					accelStep_Spindle.setCurrentPosition(0);
					SetEnable(ID_SPINDLE, false);
					retVal = true;
					break;
				}
				case 249: // Set Axis speed
				{
					delay(10);

					switch (configGreekKey_Main.axisId)
					{
						case ID_AXIS_Z:
						{
							configGreekKey_Z.speedPercent_Axis = (int)GetSerialFloat(serialId);
		#ifdef DEBUG
							Serial.print("speedPercentSpindle_Z: ");
							Serial.println(configGreekKey_Z.speedPercent_Axis);
		#endif // DEBUG
							EEPROM.put(eePromAddress_GreekKey_Z, configGreekKey_Z);
						}
						case ID_AXIS_X:
						{
							configGreekKey_X.speedPercent_Axis = (int)GetSerialFloat(serialId);
		#ifdef DEBUG
							Serial.print("speedPercentSpindle_X: ");
							Serial.println(configGreekKey_X.speedPercent_Axis);
		#endif // DEBUG
							EEPROM.put(eePromAddress_GreekKey_X, configGreekKey_X);
						}
					}
					break;
				}
				case 254: // Set Spindle speed
				{
					delay(10);
					switch (configGreekKey_Main.axisId)
					{
						case ID_AXIS_Z:
						{
							configGreekKey_Z.speedPercent_Spindle = (int)GetSerialFloat(serialId);
		#ifdef DEBUG
							Serial.print("speedPercentSpindle_Z: ");
							Serial.println(configGreekKey_Z.speedPercent_Spindle);
		#endif // DEBUG
							nextSpeed = configGreekKey_Z.speedPercent_Spindle * configGreekKey_Z.maxSpd_Spindle * .01;
							accelStep_Spindle.setMaxSpeed(nextSpeed);
							EEPROM.put(eePromAddress_GreekKey_Z, configGreekKey_Z);
							break;
						}
						case ID_AXIS_X:
						{
							configGreekKey_X.speedPercent_Spindle = (int)GetSerialFloat(serialId);
		#ifdef DEBUG
							Serial.print("speedPercentSpindle_X: ");
							Serial.println(configGreekKey_X.speedPercent_Spindle);
		#endif // DEBUG
							nextSpeed = configGreekKey_X.speedPercent_Spindle * configGreekKey_X.maxSpd_Spindle * .01;
							accelStep_Spindle.setMaxSpeed(nextSpeed);
							EEPROM.put(eePromAddress_GreekKey_X, configGreekKey_X);
							break;
						}
					}
#ifdef DEBUG
					Serial.print("Speed: ");
					Serial.println(nextSpeed);
#endif // DEBUG
					break;
				}

			}
		}

		accelStep_Spindle.run();
	}

	SetEnable(ID_SPINDLE, false);
	Serial.print("accelStep_Spindle.currentPosition()>>>>:");
	Serial.println(accelStep_Spindle.currentPosition());
	Serial.print("endPosition_Spindle:");
	Serial.println(endPosition_Spindle);
	endPosition_Spindle = endPosition_Spindle + accelStep_Spindle.currentPosition();
	Serial.print("endPosition_Spindle:");
	Serial.println(endPosition_Spindle);
	Serial.println("Exit GreekKey_Move_Spindle.......................");
	return retVal;
}

/// <summary>
/// GreekKey_Move_Z
/// </summary>
/// <comment>
/// </comment>
/// <param name="shortLegLength">Segment(leg) length</param>
/// <param name="multiplier">Number of segments</param>
/// <param name="direction">Direction to move</param>
/// <returns>True if stop button touched otherwise false</returns>
bool GreekKey_Move_Z(float shortLegLength, float multiplier, int direction, bool updatePosition)
{
	Serial.println("Enter GreekKey_Move_Z.......................");
	float nextSpeed = 0;
	float nextSpeed_Z = 0;
	bool retVal = false;
#ifdef DEBUG
	Serial.print("0-endPosition_Axis:");
	Serial.println(endPosition_Axis);
	Serial.print("shortLegLength:");
	Serial.println(shortLegLength);
	Serial.print("multiplier:");
	Serial.println(multiplier);
	Serial.print("direction:");
	Serial.println(direction);
	Serial.print("Target:");
	Serial.println(shortLegLength * multiplier * direction);
#endif // DEBUG

	accelStep_Axis_Z.setCurrentPosition(0);
	SetEnable(ID_AXIS_Z, true);
	nextSpeed_Z = configGreekKey_Z.maxSpd_Axis * configGreekKey_Z.speedPercent_Axis * .01;
	accelStep_Axis_Z.setAcceleration(configGreekKey_Z.accel_Axis);

	accelStep_Axis_Z.setMaxSpeed(nextSpeed_Z);
	int32_t targetSteps = shortLegLength * multiplier * direction;
	accelStep_Axis_Z.moveTo(targetSteps);

	#ifdef DEBUG
		Serial.print("targetSteps:");
		Serial.println(targetSteps);
		targetSteps = round(targetSteps);
		Serial.print("targetSteps:");
		Serial.println(targetSteps);
		Serial.print("shortLegLength:");
		Serial.println(shortLegLength);
		Serial.print("multiplier:");
		Serial.println(multiplier);
		Serial.print("direction:");
		Serial.println(direction);
	#endif // DEBUG

	while ((accelStep_Axis_Z.distanceToGo() != 0))
	{
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
				case 67:
				case 99: // - c
				{
					endPosition_Axis = endPosition_Axis + accelStep_Axis_Z.currentPosition();
					accelStep_Axis_Z.setCurrentPosition(0);
					SetEnable(ID_AXIS_Z, false);
					retVal = true;
					break;
				}
				case 249: // Set Axis speed
				{
					delay(10);

					configGreekKey_Z.speedPercent_Axis = (int)GetSerialFloat(serialId);
	#ifdef DEBUG
					Serial.print("speedPercentSpindle_Z: ");
					Serial.println(configGreekKey_Z.speedPercent_Axis);
	#endif // DEBUG
					nextSpeed = configGreekKey_Z.speedPercent_Axis * configGreekKey_Z.maxSpd_Axis * .01;
					accelStep_Axis_Z.setMaxSpeed(nextSpeed);
					EEPROM.put(eePromAddress_GreekKey_Z, configGreekKey_Z);
	#ifdef DEBUG
					Serial.print("Speed: ");
					Serial.println(nextSpeed);
	#endif // DEBUG
					break;
				}
				case 254: // Set Spindle speed
				{
					delay(10);
					switch (configGreekKey_Main.axisId)
					{
						case ID_AXIS_Z:
						{
							configGreekKey_Z.speedPercent_Spindle = (int)GetSerialFloat(serialId);
	#ifdef DEBUG
							Serial.print("speedPercentSpindle_Z: ");
							Serial.println(configGreekKey_Z.speedPercent_Spindle);
	#endif // DEBUG
							EEPROM.put(eePromAddress_GreekKey_Z, configGreekKey_Z);
							break;
						}
						case ID_AXIS_X:
						{
							configGreekKey_X.speedPercent_Spindle = (int)GetSerialFloat(serialId);
	#ifdef DEBUG
							Serial.print("speedPercentSpindle_X: ");
							Serial.println(configGreekKey_X.speedPercent_Spindle);
	#endif // DEBUG
							EEPROM.put(eePromAddress_GreekKey_X, configGreekKey_X);
							break;
						}
					}
					break;
				}
			}
		}

		accelStep_Axis_Z.run();
	}

	SetEnable(ID_AXIS_Z, false);
	Serial.print("1-endPosition_Axis:");
	Serial.println(endPosition_Axis);
	if (updatePosition)
	{
		endPosition_Axis = endPosition_Axis + accelStep_Axis_Z.currentPosition();
	}
	Serial.print("2-endPosition_Axis:");
	Serial.println(endPosition_Axis);
	Serial.println("Exit GreekKey_Move_Z.......................");
	return retVal;
}

/// <summary>
/// GreekKey_Move_X
/// </summary>
/// <comment>
/// </comment>
/// <param name="shortLegLength">Segment(leg) length</param>
/// <param name="multiplier">Number of segments</param>
/// <param name="direction">Direction to move</param>
/// <returns>True if stop button touched otherwise false</returns>
bool GreekKey_Move_X(float shortLegLength, float multiplier, int direction, bool updatePosition)
{
	Serial.println("Enter GreekKey_Move_X.......................");
	float nextSpeed = 0;
	float nextSpeed_X = 0;
	bool retVal = false;
#ifdef DEBUG
	Serial.print("shortLegLength:");
	Serial.println(shortLegLength);
	Serial.print("multiplier:");
	Serial.println(multiplier);
	Serial.print("direction:");
	Serial.println(direction);
	Serial.print("Target:");
	Serial.println(shortLegLength * multiplier * direction);
#endif // DEBUG

	accelStep_Axis_X.setCurrentPosition(0);
	SetEnable(ID_AXIS_X, true);
	nextSpeed_X = configGreekKey_X.maxSpd_Axis * configGreekKey_X.speedPercent_Axis * .01;
	accelStep_Axis_X.setAcceleration(configGreekKey_X.accel_Axis);

	accelStep_Axis_X.setMaxSpeed(nextSpeed_X);
	int32_t targetSteps = shortLegLength * multiplier * direction;
	accelStep_Axis_X.moveTo(targetSteps);

	while ((accelStep_Axis_X.distanceToGo() != 0))
	{
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
				case 67:
				case 99: // - c
				{
					endPosition_Axis = endPosition_Axis + accelStep_Axis_X.currentPosition();
					accelStep_Axis_X.setCurrentPosition(0);
					SetEnable(ID_AXIS_X, false);
					retVal = true;
					break;
				}
				case 249: // Set Axis speed
				{
					delay(10);

					configGreekKey_X.speedPercent_Axis = (int)GetSerialFloat(serialId);
	#ifdef DEBUG
					Serial.print("speedPercentSpindle_X: ");
					Serial.println(configGreekKey_X.speedPercent_Axis);
	#endif // DEBUG
					nextSpeed = configGreekKey_X.speedPercent_Axis * configGreekKey_X.maxSpd_Axis * .01;
					accelStep_Axis_X.setMaxSpeed(nextSpeed);
					EEPROM.put(eePromAddress_GreekKey_X, configGreekKey_X);
	#ifdef DEBUG
					Serial.print("Speed: ");
					Serial.println(nextSpeed);
	#endif // DEBUG
					break;
				}
				case 254: // Set Spindle speed
				{
					delay(10);
					switch (configGreekKey_Main.axisId)
					{
						case ID_AXIS_Z:
						{
							configGreekKey_Z.speedPercent_Spindle = (int)GetSerialFloat(serialId);
	#ifdef DEBUG
							Serial.print("speedPercentSpindle_Z: ");
							Serial.println(configGreekKey_Z.speedPercent_Spindle);
	#endif // DEBUG
							EEPROM.put(eePromAddress_GreekKey_Z, configGreekKey_Z);
							break;
						}
						case ID_AXIS_X:
						{
							configGreekKey_X.speedPercent_Spindle = (int)GetSerialFloat(serialId);
	#ifdef DEBUG
							Serial.print("speedPercentSpindle_X: ");
							Serial.println(configGreekKey_X.speedPercent_Spindle);
	#endif // DEBUG
							EEPROM.put(eePromAddress_GreekKey_X, configGreekKey_X);
							break;
						}
					}
					break;
				}
			}
		}

		accelStep_Axis_X.run();
	}

	SetEnable(ID_AXIS_X, false);
	Serial.print("endPosition_Axis:");
	Serial.println(endPosition_Axis);
	if(updatePosition)
	{ 
	  endPosition_Axis = endPosition_Axis + accelStep_Axis_X.currentPosition();
	}
	Serial.print("endPosition_Axis:");
	Serial.println(endPosition_Axis);
	Serial.println("Exit GreekKey_Move_X.......................");
	return retVal;
}

/// <summary>
/// GreekKey_Move_X
/// </summary>
/// <comment>
/// </comment>
/// <param name="shortLegLength">Segment(leg) length</param>
/// <param name="multiplier">Number of segments</param>
/// <param name="direction">Direction to move</param>
/// <returns>True if stop button touched otherwise false</returns>
bool GreekKey_Move_Angular_TeensyStep(
	float shortLegLength_Axis, 
	float multiplier_Axis, 
	float shortLegLength_Spindle, 
	float multiplier_Spindle)
{
	int stepPin_Axis = PIN_AXIS_Z_STEP;
	int dirPin_Axis = PIN_AXIS_Z_DIR;

	int maxSpd_Axis = 0;
	int accel_Axis = 0;
	float speedPercent_Axis = 0;
	float currentSpeedPercent_Axis = 0;

	int maxSpd_Spindle = 0;
	int accel_Spindle = 0;
	float speedPercent_Spindle = 0;
	float currentSpeedPercent_Spindle = 0;

	bool retVal = false;

	switch (configGreekKey_Main.axisId)
	{
		case ID_AXIS_Z:
		{
			stepPin_Axis = PIN_AXIS_Z_STEP;
			dirPin_Axis = PIN_AXIS_Z_DIR;
			SetEnable(ID_AXIS_Z, true);

			maxSpd_Axis = configGreekKey_Z.maxSpd_Axis;
			accel_Axis = configGreekKey_Z.accel_Axis;
			speedPercent_Axis = configGreekKey_Z.speedPercent_Axis;

			maxSpd_Spindle = configGreekKey_Z.maxSpd_Spindle;
			accel_Spindle = configGreekKey_Z.accel_Spindle;
			speedPercent_Spindle = configGreekKey_Z.speedPercent_Spindle;

			break;
		}
		case ID_AXIS_X:
		{
			stepPin_Axis = PIN_AXIS_X_STEP;
			dirPin_Axis = PIN_AXIS_X_DIR;

			SetEnable(ID_AXIS_X, true);

			maxSpd_Axis = configGreekKey_X.maxSpd_Axis;
			accel_Axis = configGreekKey_X.accel_Axis;
			speedPercent_Axis = configGreekKey_X.speedPercent_Axis;

			maxSpd_Spindle = configGreekKey_Z.maxSpd_Spindle;
			accel_Spindle = configGreekKey_Z.accel_Spindle;
			speedPercent_Spindle = configGreekKey_Z.speedPercent_Spindle;

			break;
		}
	}

	SetEnable(ID_SPINDLE, true);
	currentSpeedPercent_Axis = speedPercent_Axis * .01;
	currentSpeedPercent_Spindle = speedPercent_Spindle * .01;
	
	StepControl stepController;
	Stepper stepper_Axis(stepPin_Axis, dirPin_Axis);
	stepper_Axis
		.setMaxSpeed(maxSpd_Axis * currentSpeedPercent_Axis)
		.setAcceleration(accel_Axis);
	stepper_Axis.setPosition(0);
	int32_t targetSteps = round(shortLegLength_Axis * multiplier_Axis);// *direction_Axis);
	stepper_Axis.setTargetRel(targetSteps);

	stepperSpindle
		.setMaxSpeed(maxSpd_Spindle * currentSpeedPercent_Spindle)
		.setAcceleration(accel_Spindle);
	stepperSpindle.setPosition(0);
	float spindleSteps = shortLegLength_Spindle * multiplier_Spindle;
	int32_t targetSteps_Spindle = (int32_t)round(spindleSteps);

	// Convert Up to negative and Down to positive
	targetSteps_Spindle = targetSteps_Spindle * (-1);
	stepperSpindle.setTargetRel(targetSteps_Spindle);

#ifdef DEBUG
	Serial.println("-=-=-=-=-=-=-=-=-=-=-=-===-=-=-==-=-=-=-");
	Serial.print("spindleSteps:");
	Serial.println(spindleSteps);
	Serial.print("targetSteps_Spindle:");
	Serial.println(targetSteps_Spindle);
	Serial.print("targetSteps_Axis:");
	Serial.println(targetSteps);
#endif // DEBUG

	stepController.moveAsync(stepperSpindle,stepper_Axis);
	while (stepController.isRunning())
	{
		if (StopGreekKey())
		{
#ifdef DEBUG
			Serial.println("StopGreekKey() is true:");
#endif // DEBUG
			stepController.stop();
			SetEnable(ID_SPINDLE, false);
			SetEnable(ID_AXIS_Z, false);
			SetEnable(ID_AXIS_X, false);
			retVal = true;
			break;
		}
		delay(5);
	}

	SetEnable(ID_SPINDLE, false);
	SetEnable(ID_AXIS_Z, false);
	SetEnable(ID_AXIS_X, false);
#ifdef DEBUG
	Serial.print("endPosition_Axis:");
	Serial.println(endPosition_Axis);
#endif // DEBUG

	endPosition_Axis = endPosition_Axis + stepper_Axis.getPosition();
	endPosition_Spindle = endPosition_Spindle + stepperSpindle.getPosition();
#ifdef DEBUG
	Serial.print("endPosition_Axis:");
	Serial.println(endPosition_Axis);
	Serial.print("endPosition_Spindle:");
	Serial.println(endPosition_Spindle);
#endif // DEBUG
	return retVal;
}

/// <summary>
/// GreekKey_Move_Spindle
/// </summary>
/// <comment>
/// </comment>
/// <param name="shortLegLength">Segment(leg) length</param>
/// <param name="multiplier">Number of segments</param>
/// <param name="direction">Direction to move</param>
/// <returns>True if stop button touched otherwise false</returns>
bool GreekKey_Move_SpindleTeensy(float shortLegLength, float multiplier, int direction)
{
	Serial.println("Enter GreekKey_Move_Spindle.......................");
	bool retVal = false;

	SetEnable(ID_SPINDLE, true);
	StepControl stepControllerSpindle;

	Stepper stepper_Spindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
	if (configGreekKey_Main.axisId == ID_AXIS_Z) // Z axis
	{
		stepper_Spindle
			.setMaxSpeed(configGreekKey_Z.maxSpd_Spindle * configGreekKey_Z.speedPercent_Spindle * .01)
			.setAcceleration(configGreekKey_Z.accel_Spindle);
	}
	else
	{
		stepper_Spindle
			.setMaxSpeed(configGreekKey_X.maxSpd_Spindle * configGreekKey_X.speedPercent_Spindle * .01)
			.setAcceleration(configGreekKey_X.accel_Spindle);
	}

	int32_t mSpeed = configGreekKey_Z.maxSpd_Spindle * configGreekKey_Z.speedPercent_Spindle * .01;
	if (configGreekKey_Main.axisId == ID_AXIS_X) // X axis
	{
		mSpeed = configGreekKey_X.maxSpd_Spindle * configGreekKey_X.speedPercent_Spindle * .01;
	}

	stepper_Spindle.setMaxSpeed(mSpeed);
	stepper_Spindle.setPosition(0);

	int32_t targetSteps = shortLegLength * multiplier * direction;

	Serial.print("targetSteps:");
	Serial.println(targetSteps);
	targetSteps = round(targetSteps);
	Serial.print("targetSteps:");
	Serial.println(targetSteps);
	Serial.print("shortLegLength:");
	Serial.println(shortLegLength);
	Serial.print("multiplier:");
	Serial.println(multiplier);
	Serial.print("direction:");
	Serial.println(direction);
	stepper_Spindle.setTargetRel(targetSteps);
	stepControllerSpindle.moveAsync(stepper_Spindle);

	while (stepControllerSpindle.isRunning())
	{
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
				case 67:
				case 99: // - c
				{
					stepControllerSpindle.stop();

					SetEnable(ID_SPINDLE, false);
					SetEnable(ID_AXIS_Z, false);
					SetEnable(ID_AXIS_X, false);

					retVal = true;
					break;
				}
			}
		}

		delay(5);
	}

	Serial.print("endPosition_Spindle:");
	Serial.println(endPosition_Spindle);
	endPosition_Spindle = endPosition_Spindle + stepper_Spindle.getPosition();
	Serial.print("endPosition_Spindle:");
	Serial.println(endPosition_Spindle);
	Serial.println("Exit GreekKey_Move_Spindle.......................");
	return retVal;
}

/// <summary>
/// GreekKey_Move_X
/// </summary>
/// <comment>
/// </comment>
/// <param name="shortLegLength">Segment(leg) length</param>
/// <param name="multiplier">Number of segments</param>
/// <param name="direction">Direction to move</param>
/// <returns>True if stop button touched otherwise false</returns>
bool GreekKey_Move_X_TeensyStep(float shortLegLength, float multiplier, int direction)
{
	bool retVal = false;
#ifdef DEBUG
	Serial.print("XshortLegLength:");
	Serial.println(shortLegLength);
	Serial.print("Xmultiplier:");
	Serial.println(multiplier);
	Serial.print("Xdirection:");
	Serial.println(direction);
	Serial.print("XTarget:");
	Serial.println(shortLegLength * multiplier * direction);
#endif // DEBUG

	SetEnable(ID_AXIS_X, true);
	StepControl stepControllerAxis;
	Stepper stepper_X(PIN_AXIS_X_STEP, PIN_AXIS_X_DIR);
	stepper_X
		.setMaxSpeed(configGreekKey_X.maxSpd_Axis * configGreekKey_X.speedPercent_Axis * .01)
		.setAcceleration(configGreekKey_X.accel_Axis);
	stepper_X.setPosition(0);
	int32_t targetSteps = round(shortLegLength * multiplier * direction);
	stepper_X.setTargetRel(targetSteps);
	stepControllerAxis.moveAsync(stepper_X);
	while (stepControllerAxis.isRunning())
	{
		if (StopGreekKey())
		{
#ifdef DEBUG
			Serial.println("StopGreekKey() is true:");
#endif // DEBUG
			stepControllerAxis.stop();

			SetEnable(ID_SPINDLE, false);
			SetEnable(ID_AXIS_Z, false);
			SetEnable(ID_AXIS_X, false);

			retVal = true;
			break;
		}
		delay(5);
	}

#ifdef DEBUG
	Serial.print("endPosition_Axis:");
	Serial.println(endPosition_Axis);
#endif // DEBUG

	endPosition_Axis = endPosition_Axis + stepper_X.getPosition();

#ifdef DEBUG
	Serial.print("endPosition_Axis:");
	Serial.println(endPosition_Axis);
#endif // DEBUG
	return retVal;
}

/// <summary>
/// GreekKey_Move_Z
/// </summary>
/// <comment>
/// </comment>
/// <param name="shortLegLength">Segment(leg) length</param>
/// <param name="multiplier">Number of segments</param>
/// <param name="direction">Direction to move</param>
/// <returns>True if stop button touched otherwise false</returns>
bool GreekKey_Move_Z_TeensyStep(float shortLegLength, float multiplier, int direction)
{ 
	Serial.println("Enter GreekKey_Move_Z.......................");
	bool retVal = false;
#ifdef DEBUG
	Serial.print("Move_Z:shortLegLength ");
	Serial.println(shortLegLength);
	Serial.print("Move_Z:multiplier ");
	Serial.println(multiplier);
	Serial.print("Move_Z:direction ");
	Serial.println(direction);
	Serial.print("ZTarget:");
	Serial.println(shortLegLength * multiplier * direction);
	Serial.print("configGreekKey_Z.accel_Axis:");
	Serial.println(configGreekKey_Z.accel_Axis);

#endif // DEBUG
	SetEnable(ID_AXIS_Z, true);
	StepControl stepControllerAxis;
	Stepper stepper_Z(PIN_AXIS_Z_STEP, PIN_AXIS_Z_DIR);

	int mSpeed = configGreekKey_Z.maxSpd_Axis * configGreekKey_Z.speedPercent_Axis * .01;
#ifdef DEBUG
	Serial.print("mSpeed------------:");
	Serial.println(mSpeed);
	Serial.print("configGreekKey_Z.maxSpd_Axis------------:");
	Serial.println(configGreekKey_Z.maxSpd_Axis);
	Serial.print("configGreekKey_Z.speedPercent_Axis------------:");
	Serial.println(configGreekKey_Z.speedPercent_Axis);
	Serial.print("configGreekKey_Z.accel_Axis------------:");
	Serial.println(configGreekKey_Z.accel_Axis);
#endif // DEBUG

	stepper_Z
		.setMaxSpeed(mSpeed)
		.setAcceleration(configGreekKey_Z.accel_Axis);
	stepper_Z.setPosition(0);
	int targetSteps = (int)round(shortLegLength * multiplier * direction);
	Serial.print("Move_Z:targetSteps ");
	Serial.println(targetSteps);

	stepper_Z.setTargetRel(targetSteps);
	stepControllerAxis.moveAsync(stepper_Z);
	while (stepControllerAxis.isRunning())
	{
		if (StopGreekKey())
		{
			stepControllerAxis.stop();

			SetEnable(ID_SPINDLE, false);
			SetEnable(ID_AXIS_Z, false);
			SetEnable(ID_AXIS_X, false);

			retVal = true;
			break;
		}
		delay(5);
	}


	Serial.print("endPosition_Axis:");
	Serial.println(endPosition_Axis);
	endPosition_Axis = endPosition_Axis + stepper_Z.getPosition();
	Serial.print("endPosition_Axis:");
	Serial.println(endPosition_Axis);

	Serial.println("Exit GreekKey_Move_Z.......................");
	return retVal;
}

/// <summary>
/// GreekKeyFromFile
/// </summary>
/// <comment>
/// Move spindle and axis independently from values stored in file
/// </comment>
/// <returns></returns>
void GreekKeyFromFile(int direction)
{
#ifdef DEBUG
	Serial.println("Enter GreekKeyFromFile");
#endif
	bool stopAll = false;
	bool exitInnerForLoop = false;

	int lineCounter = 0;
	int fileLineCount = 500;
	float segmentMultiplier = 0;

	int shortSegmentStepsAxis = 0;
	int shortSegmentStepsAxisZ = 0;
	int shortSegmentStepsAxisX = 0;
	int axisSteps = 0;

	int spindleShortLegSteps = 0;
	int spindleSegmentCount = 0;
	int spindleSteps = 0;

	// Reset end positions
	endPosition_Spindle = 0;
	endPosition_Axis = 0;
#ifdef DEBUG
	Serial.print(">>>>>>>>>>>>>>>>>>>>segmentOrActual:");
	Serial.println(configGreekKey_Main.segmentOrActual);
#endif // DEBUG
	// Get Spindle segment count and line count from file :
	badFilename = false;
	if (configGreekKey_Main.segmentOrActual == 2) // 2: Segment 
	{
		for (lineCounter = 0; lineCounter <= fileLineCount; lineCounter++)
		{
			segmentMultiplier = GetGreekKeyDataFromSD(lineCounter);
			if (badFilename)
			{
				Serial.println("Exit Greek Key: File not found");

				Serial3.print("pageGrkFile.tFileName.txt=");
				Serial3.write(0x22);
				Serial3.print("File not found");

				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);

				goto EndLoops;
			}

#ifdef DEBUG
			Serial.print(">>>segmentMultiplier:");
			Serial.println(segmentMultiplier);
#endif // DEBUG

			switch (moveType)
			{
				case 69: // E - Exit
				case 101: // e
				{
					// Store total line count in file
					goto EndSegmentCount;

					break;
				}
				case 83: // S - Spindle segment count (Axial and Spindle segments reverse when Axial is selected.)
				case 115: // s
				{
					if (configGreekKey_Main.radialOrAxial_File == RADIAL)
					{
						spindleSegmentCount = segmentMultiplier;
					}
					break;
				}

				case 65: // A - Axial segment count
				case 97: // a
				{
					if (configGreekKey_Main.radialOrAxial_File == AXIAL)
					{
						spindleSegmentCount = segmentMultiplier;
					}
					break;
				}
			}
#ifdef DEBUG
			Serial.print("spindleSegmentCount:");
			Serial.println(spindleSegmentCount);
#endif // DEBUG
		}
		
	EndSegmentCount:
		fileLineCount = lineCounter;
		
		int stepsPerRevolution_Spindle = (int)(floor(configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle));
		float patternDivisor = ((float)configGreekKey_Main.countPatternFile360 * (float)spindleSegmentCount);
		spindleShortLegSteps = (int)((floor(stepsPerRevolution_Spindle)) / (patternDivisor));
#ifdef DEBUG
		Serial.print("lineCounter:");
		Serial.println(lineCounter);
		Serial.print("patternDivisor:");
		Serial.println(patternDivisor);
		Serial.print("spindleShortLegSteps:");
		Serial.println(spindleShortLegSteps);
#endif // DEBUG
	}

	// Calculate axis steps
	shortSegmentStepsAxisZ = round((configGreekKey_Main.segmentLengthForFile / configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z));
	shortSegmentStepsAxisX = round((configGreekKey_Main.segmentLengthForFile / configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X));
	switch (configGreekKey_Main.axisId)
	{
		case ID_AXIS_Z: // Z Axis
		{	
			shortSegmentStepsAxis = shortSegmentStepsAxisZ;
			break;
		}
		case ID_AXIS_X: // X Axis
		{
			shortSegmentStepsAxis = shortSegmentStepsAxisX;
			break;
		}
	}

#ifdef DEBUG

	Serial.print("configGreekKey_Main.countPatternFile:");
	Serial.println(configGreekKey_Main.countPatternFile);
	Serial.print("shortSegmentStepsAxisZ:");
	Serial.println(shortSegmentStepsAxisZ);
	Serial.print("shortSegmentStepsAxisX:");
	Serial.println(shortSegmentStepsAxisX);
	Serial.print("shortSegmentStepsAxis:");
	Serial.println(shortSegmentStepsAxis);
#endif // DEBUG

	for (int i = 1; i <= configGreekKey_Main.countPatternFile; i++)
	{ 
		exitInnerForLoop = false;
		// Inner loop
		for (int i = 0; i <= fileLineCount; i++)
		{
			// Reset hVal and vVal
			hVal = 0;
			vVal = 0;

			// Get data
			segmentMultiplier = GetGreekKeyDataFromSD(i);
	
	#ifdef DEBUG
			Serial.print("segmentMultiplier:");
			Serial.println(segmentMultiplier);
			Serial.print("moveType:");
			Serial.println(moveType);
			Serial.print("Segment/Actual:");
			Serial.println(configGreekKey_Main.segmentOrActual);
			Serial.print("Line Number:");
			Serial.println(i);

	#endif // DEBUG

	#ifdef DEBUG

			//Serial.print("Comment0:");
			//Serial.println(comment);
	#endif // DEBUG

			switch (moveType)
			{
				case 68: // D  - Radial: Spindle Down CW   Axial: Axis Right CW
				{
					switch (configGreekKey_Main.radialOrAxial_File)
					{
						case RADIAL: // Spindle Down CW
						{
							if (configGreekKey_Main.segmentOrActual == 2) // 2: Segment 
							{
								stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, segmentMultiplier, DIR_CW);
							}
							else  //3: Actual
							{
								spindleSteps = round((configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (segmentMultiplier / 360));
								stopAll = GreekKey_Move_Spindle(spindleSteps, 1, DIR_CW);
							}
							break;
						}
						case AXIAL: // Axis Right CW
						{
							if (configGreekKey_Main.segmentOrActual == 2) // 2: Segment  
							{
								stopAll = GreekKey_Move_Axis(shortSegmentStepsAxis, segmentMultiplier, DIR_CW);
							}
							else  //3: Actual
							{
								switch (configGreekKey_Main.axisId)
								{
									case ID_AXIS_Z: // Z Axis
									{
										axisSteps = round((segmentMultiplier / configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z));
										break;
									}
									case ID_AXIS_X: // X Axis
									{
										axisSteps = round((segmentMultiplier / configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X));
										break;
									}
								}

								stopAll = GreekKey_Move_Axis(axisSteps, 1, DIR_CW);
							}
						}
					}

					if (!stopAll)
					{
						if (StopGreekKey())
						{
							goto EndLoops;
						}
					}
					else
					{
						goto EndLoops;
					}

					break;
				}

				case 69: // E - Exit
				{
					exitInnerForLoop = true;
					break;
				}
				case 72: // H  - Angular Move. Line must also contain V
				{
					switch (configGreekKey_Main.radialOrAxial_File)
					{
						case RADIAL: // 
						{
							if (configGreekKey_Main.segmentOrActual == 2) // 2: Segment  
							{
								switch (configGreekKey_Main.axisId)
								{
									case ID_AXIS_Z: // Z Axis is primary
									{
										stopAll = GreekKey_Move_Angular_TeensyStep(shortSegmentStepsAxisZ, hVal * direction, spindleShortLegSteps, vVal);
										break;
									}
									case ID_AXIS_X: // X Axis is primary
									{
										stopAll = GreekKey_Move_Angular_TeensyStep(shortSegmentStepsAxisX, hVal * direction, spindleShortLegSteps, vVal);
										break;
									}
								}
							}
							else  // Actual
							{
								switch (configGreekKey_Main.axisId)
								{
									case ID_AXIS_Z: // Z Axis
									{
										axisSteps = round((hVal / configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z));
										break;
									}
									case ID_AXIS_X: // X Axis
									{
										axisSteps = round((hVal / configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X));
										break;
									}

								}

								axisSteps = axisSteps * direction;
								spindleSteps = round((configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (vVal / 360));
								stopAll = GreekKey_Move_Angular_TeensyStep(axisSteps, 1, spindleSteps, 1);

							}
							break;
						}
						case AXIAL: // Switch spindle and axis
						{
							if (configGreekKey_Main.segmentOrActual == 2) // 2: Segment  
							{
								switch (configGreekKey_Main.axisId)
								{
								case ID_AXIS_Z: //
								{
									stopAll = GreekKey_Move_Angular_TeensyStep(shortSegmentStepsAxisZ, vVal * direction, spindleShortLegSteps, hVal);
									break;
								}
								case ID_AXIS_X: // 
								{
									stopAll = GreekKey_Move_Angular_TeensyStep(shortSegmentStepsAxisX, vVal * direction, spindleShortLegSteps, hVal);
									break;
								}
								}
							}
							else  // Actual
							{
								switch (configGreekKey_Main.axisId)
								{
									case ID_AXIS_Z: // Z Axis
									{
										axisSteps = round((vVal / configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z));
										break;
									}
									case ID_AXIS_X: // X Axis
									{
										axisSteps = round((vVal / configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X));
										break;
									}

								}

								axisSteps = axisSteps * direction;
								spindleSteps = round((configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (hVal / 360));
								stopAll = GreekKey_Move_Angular_TeensyStep(axisSteps, 1, spindleSteps, 1);
							}

							break;
						}
					}

					if (!stopAll)
					{
						if (StopGreekKey())
						{
							goto EndLoops;
						}
					}
					else
					{
						goto EndLoops;
					}
					break;
				}
				case 73: // I - Move alternate axis in (Doesn't change for Radial or Axial)
				{
					if (configGreekKey_Main.segmentOrActual == 2) // 2: Segment  
					{
						switch (configGreekKey_Main.axisId)
						{
							case ID_AXIS_Z: // Z Axis is primary
							{
								stopAll = GreekKey_Move_Axis_Alt(shortSegmentStepsAxisX, segmentMultiplier, DIR_CCW);
								break;
							}
							case ID_AXIS_X: // X Axis is primary
							{
								stopAll = GreekKey_Move_Axis_Alt(shortSegmentStepsAxisZ, segmentMultiplier, DIR_CCW);
								break;
							}
						}
					}
					else // Actual
					{
						switch (configGreekKey_Main.axisId)
						{
							case ID_AXIS_Z: // Z Axis is primary
							{
								axisSteps = round((segmentMultiplier / configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X));
								
								break;
							}
							case ID_AXIS_X: // X Axis is primary
							{
								axisSteps = round((segmentMultiplier / configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z));
								break;
							}
						}

						// Uses actual value
						stopAll = GreekKey_Move_Axis_Alt(axisSteps, 1, DIR_CCW);
					}

					if (!stopAll)
					{
						if (StopGreekKey())
						{
							goto EndLoops;
						}
					}
					else
					{
						goto EndLoops;
					}

					break;
				}
				case 76: // L - Move axis left
				{

					switch (configGreekKey_Main.radialOrAxial_File)
					{
						case RADIAL: // Axis Left CCW
						{
							if (configGreekKey_Main.segmentOrActual == 2) // 2: Segment  
							{
								stopAll = GreekKey_Move_Axis(shortSegmentStepsAxis, segmentMultiplier, DIR_CCW);
							}
							else //3: Actual
							{
								switch (configGreekKey_Main.axisId)
								{
									case ID_AXIS_Z: // Z Axis
									{
										axisSteps = round((segmentMultiplier / configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z));
										break;
									}
									case ID_AXIS_X: // X Axis
									{
										axisSteps = round((segmentMultiplier / configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X));
										break;
									}
								}

								stopAll = GreekKey_Move_Axis(axisSteps, 1, DIR_CCW);
							}
							break;
						}
						case AXIAL: // Spindle Up CCW
						{
							if (configGreekKey_Main.segmentOrActual == 2) // 2: Segment 
							{
								stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, segmentMultiplier, DIR_CCW);
							}
							else  //3: Actual
							{
								spindleSteps = round((configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (segmentMultiplier / 360));
								stopAll = GreekKey_Move_Spindle(spindleSteps, 1, DIR_CCW);
							}
							break;
						}
					}

					if (!stopAll)
					{
						if (StopGreekKey())
						{
							goto EndLoops;
						}
					}
					else
					{
						goto EndLoops;
					}

					break;
				}
				case 79: // O - Move alternate axis out (Doesn't change for Radial or Axial)
				{
					if (configGreekKey_Main.segmentOrActual == 2) // 2: Segment  
					{
						switch (configGreekKey_Main.axisId)
						{
							case ID_AXIS_Z: // Z Axis is primary, so use X to move cutter away from workpiece
							{
								stopAll = GreekKey_Move_Axis_Alt(shortSegmentStepsAxisX, segmentMultiplier, DIR_CW);
								break;
							}
							case ID_AXIS_X: // X Axis is primary, so use Z to move cutter away from workpiece
							{
								stopAll = GreekKey_Move_Axis_Alt(shortSegmentStepsAxisZ, segmentMultiplier, DIR_CW);
								break;
							}
						}
					}
					else
					{
						switch (configGreekKey_Main.axisId)
						{
							case ID_AXIS_Z: // Z Axis is primary
							{
								axisSteps = round((segmentMultiplier / configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X));

								break;
							}
							case ID_AXIS_X: // X Axis is primary
							{
								axisSteps = round((segmentMultiplier / configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z));
								break;
							}
						}

						// Uses actual value
						stopAll = GreekKey_Move_Axis_Alt(axisSteps, 1, DIR_CW);
					}

					if (!stopAll)
					{
						if (StopGreekKey())
						{
							goto EndLoops;
						}
					}
					else
					{
						goto EndLoops;
					}

					break;
				}
				case 82: // R - Move axis right CW
				{ 
					switch (configGreekKey_Main.radialOrAxial_File)
					{
						case RADIAL: // Axis Right CW
						{
							if (configGreekKey_Main.segmentOrActual == 2) // 2: Segment  
							{
								stopAll = GreekKey_Move_Axis(shortSegmentStepsAxis, segmentMultiplier, DIR_CW);
							}
							else  //3: Actual
							{
								switch (configGreekKey_Main.axisId)
								{
									case ID_AXIS_Z: // Z Axis
									{
										axisSteps = round((segmentMultiplier / configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z));
										break;
									}
									case ID_AXIS_X: // X Axis
									{
										axisSteps = round((segmentMultiplier / configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X));
										break;
									}
								}

								stopAll = GreekKey_Move_Axis(axisSteps, 1, DIR_CW);
							}

							break;
						}
						case AXIAL: // Spindle Down CW
						{
							if (configGreekKey_Main.segmentOrActual == 2) // 2: Segment 
							{
								stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, segmentMultiplier, DIR_CW);
							}
							else  //3: Actual
							{
								spindleSteps = round((configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (segmentMultiplier / 360));
								stopAll = GreekKey_Move_Spindle(spindleSteps, 1, DIR_CW);
							}
							break;
						}
					}

					if (!stopAll)
					{
						if (StopGreekKey())
						{
							goto EndLoops;
						}
					}
					else
					{
						goto EndLoops;
					}

					break;
				}
				case 85: // U - Spindle up CCW
				{
					switch (configGreekKey_Main.radialOrAxial_File)
					{
						case RADIAL: // Spindle Up CCW
						{
							if (configGreekKey_Main.segmentOrActual == 2) // 2: Segment 
							{
								stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, segmentMultiplier, DIR_CCW);
							}
							else  //3: Actual
							{
								spindleSteps = round((configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (segmentMultiplier / 360));
								stopAll = GreekKey_Move_Spindle(spindleSteps, 1, DIR_CCW);
							}

							break;
						}
						case AXIAL: // Axis Left CCW
						{
							if (configGreekKey_Main.segmentOrActual == 2) // 2: Segment  
							{
								stopAll = GreekKey_Move_Axis(shortSegmentStepsAxis, segmentMultiplier, DIR_CCW);
							}
							else //3: Actual
							{
								switch (configGreekKey_Main.axisId)
								{
									case ID_AXIS_Z: // Z Axis
									{
										axisSteps = round((segmentMultiplier / configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z));
										break;
									}
									case ID_AXIS_X: // X Axis
									{
										axisSteps = round((segmentMultiplier / configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X));
										break;
									}
								}

								stopAll = GreekKey_Move_Axis(axisSteps, 1, DIR_CCW);
							}

							break;
						}

					}

					if(!stopAll)
					{ 
						if(StopGreekKey())
						{
							goto EndLoops;
						}
					}
					else
					{
						goto EndLoops;
					}
	
					break;
				}
			} // End Switch(moveType)

			stopAll = StopGreekKey();
			if (stopAll)
			{
				goto EndLoops;
			}

			if (exitInnerForLoop)
			{
#ifdef DEBUG
				Serial.println("ExitInnerForLoop>>>>>>>>");
#endif // DEBUG

				break;
			}
		}// End Inner loop

	}

EndLoops:
		// Update Nextion
	Serial3.print("pageGrkFile.bt3.pco=0");// The first one may be ignored by Nextion, so resend.
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageGrkFile.va0.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageGrkFile.bt3.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageGrkFile.bt2.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageGrkFile.bt1.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);

	degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
	switch (configGreekKey_Main.axisId)
	{
		case ID_AXIS_Z: // Z Axis
		{
			distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);
			break;
		}
		case ID_AXIS_X: // X Axis
		{
			distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_X);
			break;
		}
	}

	returnSteps_Axis = endPosition_Axis;
	returnSteps_Spindle = endPosition_Spindle;

#ifdef DEBUG
	Serial.print("endPosition_Spindle :");
	Serial.println(endPosition_Spindle);
	Serial.print("endPosition_Axis :");
	Serial.println(endPosition_Axis);
#endif // DEBUG

#ifdef SHOW_POSITION 
	Serial3.print("pageBE.t1.txt=");
	Serial3.write(0x22);
	Serial3.print(degrees_Spindle);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(10);
	Serial3.print("pageBE.t2.txt=");
	Serial3.write(0x22);
	Serial3.print(distance_Axis);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
#endif // Show Position

	SetEnable(ID_SPINDLE, false);
	SetEnable(ID_AXIS_Z, false);
	SetEnable(ID_AXIS_X, false); 
}


/// <summary>
/// Serial Print
/// </summary>
/// <comment>
/// Prints data to USB and Bluetooth serial ports.  Only one will be active.
/// </comment>
/// <param name="text">Text to send</param>
/// <returns></returns>
void SerialPrint(String text)
{
	Serial.print(text);
	////Serialxx1.print(text);
	Serial3.print(text);
}

/// <summary>
/// Serial Write
/// </summary>
/// <comment>
/// Writes data to USB and Bluetooth serial ports.  Only one will be active.
/// </comment>
/// <param name="text">Char to send</param>
/// <returns></returns>
void SerialWrite(char text)
{
	Serial.write(text);
	////Serialxx1.write(text);
	Serial3.write(text);
}

/// <summary>
/// Serial Print Line
/// </summary>
/// <comment>
/// Prints data to USB and Bluetooth serial ports.  Only one will be active.
/// </comment>
/// <param name="text">Text to send</param>
/// <returns></returns>
void SerialPrintLn(String text)
{
	Serial.println(text);
	Serial3.println(text);
}

/// <summary>
/// Serial Read
/// </summary>
/// <comment>
/// Gets data from USB or Bluetooth serial ports.  Only one will be active.
/// </comment>
/// <param name="serialId">Id of serial port</param>
/// <returns>Integer</returns>
int SerialRead(int serialPortId)
{
	int incomingData = 0;
	if (serialPortId == 0)
	{
		incomingData = Serial.read();
	}
	else
	{
		incomingData = Serial3.read();
	}

	return incomingData;
}

/// <summary>
/// Serial Read Byte
/// </summary>
/// <comment>
/// Gets data from USB or Bluetooth serial ports.  Only one will be active.
/// </comment>
/// <param name="serialId">Id of serial port</param>
/// <returns>Integer</returns>
int SerialReadByte()
{
	int incomingData = 0;
	if (Serial.available() > 0)
	{
		incomingData = Serial.read();
	}
	else
	{
		incomingData = Serial3.read();
	}

	return incomingData;
}

/// <summary>
/// Serial Available
/// </summary>
/// <comment>
/// Verifies a Serial port is active
/// Serial is USB connected to PC
/// Serial3 is connected to pins 0 and 1 (Nextion LCD)
/// </comment>
/// <returns>SerialId. -1 if no serial port is active</returns>
int SerialAvailable()
{
	int serialId = -1;
	if (Serial.available() > 0)
	{
		serialId = 0;
	}
	//else if (Serialxx1.available() > 0)
	//{
	//	serialId = 1;
	//}
	else if (Serial3.available() > 0)
	{
		serialId = 3;
	}
	return serialId;
}

/// <summary>
/// Get a filename from serial
/// </summary>
/// <returns></returns>
void GetFilenameFromSerial()
{
	const int bufferLength = 20;
	char input = 0;
	int i;

#ifdef DEBUG
	Serial.println("Enter GetFilenameFromSerial ");
	Serial.print("pageCallerId: ");
	Serial.println(pageCallerId);
#endif // DEBUG

	for (i = 0; i <= bufferLength; i++)
	{
		input = Serial3.read();
		//Serial.println(input);
		// Use 255 as end chars
		if (input == 255)
		{

			if (pageCallerId == 8) // pageIndex
			{
				EEPROM.write(eePromAddress_Filename_Index + i, 0);
			}
			else // pageGrkFile
			{
				EEPROM.write(eePromAddress_Filename_Grk + i, 0);
			}
			break;
		}

		if (pageCallerId == 8) // pageIndex
		{
			EEPROM.write(eePromAddress_Filename_Index + i, input);
		}
		else // pageGrkFile
		{
			EEPROM.write(eePromAddress_Filename_Grk + i, input);
		}

	}

	if (pageCallerId == 8) // pageIndex
	{
		EEPROM.put(eePromAddress_Filename_Length_Index, i);
	}
	else // pageGrkFile
	{
		EEPROM.put(eePromAddress_Filename_Length_Grk, i);
	}
}

/// <summary>
/// GetIndexDataFromSD
/// </summary>
/// <comment>
/// Get a value from file on SD card
/// </comment>
/// <param name="lineNumber">Line containing data</param>
/// <returns>Double value</returns>
double GetIndexDataFromSD(int lineNumber)
{
	String newSizeString = "";
	currentLineNumber = 0;
	int filename_Length;

	// Get filename from EEPROM
	EEPROM.get(eePromAddress_Filename_Length_Index, filename_Length);
	char arChar[20];
	int i;
	for (i = 0; i <= filename_Length; i++)
	{
		arChar[i] = EEPROM.read(i + eePromAddress_Filename_Index);
	}
	arChar[i++] = 0;

#ifdef DEBUG
	Serial.print("arChar[i]:");
	Serial.println(arChar);

#endif // DEBUG

	// Open file
	File sdFile = SD.open(arChar, FILE_READ);
	if (!sdFile)
	{
		badFilename = true;
#ifdef DEBUG
		Serial.println("File not found.");
		Serial.println("Exit Index.");
#endif // DEBUG
		

		Serial3.print("pageIndex.tFileName.txt=");
		Serial3.write(0x22);
		Serial3.print("File not found");

		Serial3.write(0x22);
		Serial3.write(0xff);
		Serial3.write(0xff);
		Serial3.write(0xff);
		return 0;
	}

	delay(100);
	while (sdFile.available())
	{
		newSizeString = sdFile.readStringUntil('\n');

		if (currentLineNumber == lineNumber)
		{
			break;
		}
		currentLineNumber++; // Increment the line number
	}

	// Close the file
	sdFile.close();

	// Return size string as double
	const char *newSizeChar = static_cast<const char*>(newSizeString.c_str());
	return atof(newSizeChar);
}

/// <summary>
/// GetGreekKeyDataFromSD
/// </summary>
/// <comment>
/// Get a value from file on SD card
/// </comment>
/// <param name="lineNumber">Line containing data</param>
/// <returns>Double value</returns>
double GetGreekKeyDataFromSD(int lineNumber)
{
	double retVal = 0;
	currentLineNumber = 0;
	String newSizeString = "";
	int filename_Length;

	String nStr;
	String nStrA;

	// Get filename from EEPROM
	EEPROM.get(eePromAddress_Filename_Length_Grk, filename_Length);

	char arChar[20];
	int i;
	for (i = 0; i <= filename_Length; i++)
	{
		arChar[i] = EEPROM.read(i + eePromAddress_Filename_Grk);
	}
	arChar[i++] = 0;

	// Open file
	File sdFile = SD.open(arChar, FILE_READ);
	if (!sdFile)
	{
		badFilename = true;
#ifdef DEBUG
		Serial.println("File not found.");
#endif // DEBUG

		return -1;
	}

	delay(20);
	while (sdFile.available())
	{
		// Apple/Unix only has linefeed, no carriage return.
		newSizeString = sdFile.readStringUntil('\n');

		if (currentLineNumber == lineNumber)
		{
			break;
		}
	
		currentLineNumber++; // Increment the line number
	}

	// Close the file
	sdFile.close();

	// Parse the output
		moveType = newSizeString[0];
		nStr = newSizeString.substring(1);

#ifdef DEBUG
	Serial.print("moveType:");
	Serial.println(moveType);
#endif

	if (moveType == 59) // ;
	{
		Serial.println("MoveType 59");
		comment = nStr;
		return 0;
	}

	if (moveType == 72) // H
	{
		// Horizontal and Vertical move combined.
		// Parse nStr 
		//String delimiter = "V";
		//String horizontal = nStr.substring(0,nStr.find)
		//String h = split(nStr, "V");
		char *val;
		char charBuf[20];
		nStr.toCharArray(charBuf, 20);
		val = strtok(charBuf, "V");
		hVal = atof(val);
		val = strtok(NULL, "\n");
		vVal = atof(val);
		Serial.print("nStr: ");
		Serial.println(nStr);
		Serial.print("hVal: ");
		Serial.println(hVal);
		Serial.print("val: ");
		Serial.println(val);
		Serial.print("vVal: ");
		Serial.println(vVal);
	}

	// Return size string as double
	const char *newSizeChar = static_cast<const char*>(nStr.c_str());

	retVal = atof(newSizeChar);

#ifdef DEBUG
	Serial.print("retVal:");
	Serial.println(retVal);
	Serial.println("Exit GetGreekKeyDataFromSD");
#endif
	return retVal;
}

/// <summary>
/// Return Spindle and/or axes to start position
/// </summary>
/// <comment>
/// </comment>
/// <param name="axisId">Id of axis to move</param>
void ReturnToStartPosition(int axisId)
{
	int32_t target_Spindle = returnSteps_Spindle % spindleStepsPerRev * (-1);
	int32_t target_Axis = returnSteps_Axis * (-1);

	StepControl stepController;
	Stepper stepper_Spindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
	Stepper stepper_Z(PIN_AXIS_Z_STEP, PIN_AXIS_Z_DIR);
	Stepper stepper_X(PIN_AXIS_X_STEP, PIN_AXIS_X_DIR);
	Stepper stepper_B(PIN_AXIS_B_STEP, PIN_AXIS_B_DIR);

	stepper_Spindle.setPosition(0);

	endPosition_Axis = 0;
	endPosition_Spindle = 0;
	delay(15);
	stepper_Spindle
		.setMaxSpeed(configSetup.maxSpd_Return_Spindle)
		.setAcceleration(configSetup.accel_Return_Spindle)
		.setTargetRel(target_Spindle);

#ifdef DEBUG
	Serial.println("Begin ReturnToStartPosition()");
	Serial.print("target_Spindle: ");
	Serial.println(target_Spindle);
	Serial.print("target_Axis: ");
	Serial.println(target_Axis);
#endif // DEBUG

	SetEnable(ID_SPINDLE, true);
	delay(20);
	switch (axisId)
	{
		case ID_AXIS_Z:
		{
			stepper_Z.setPosition(0);
			stepper_Z
				.setMaxSpeed(configSetup.maxSpd_Return_Axis)
				.setAcceleration(configSetup.accel_Return_Axis)
				.setTargetAbs(target_Axis);

			SetEnable(ID_AXIS_Z, true);
			if (pageCallerId == PAGE_MAIN)
			{
				// Only move the Z Axis
				stepController.moveAsync(stepper_Z);
			}
			else
			{
				stepController.moveAsync(stepper_Z, stepper_Spindle);
			}

			break;
		}
		case ID_AXIS_X:
		{
			stepper_X.setPosition(0);
			stepper_X
				.setMaxSpeed(configSetup.maxSpd_Return_Axis)
				.setAcceleration(configSetup.accel_Return_Axis)
				.setTargetAbs(target_Axis);

			SetEnable(ID_AXIS_X, true);
			if (pageCallerId == PAGE_MAIN)
			{
				// Only move the X Axis
				stepController.moveAsync(stepper_X);
			}
			else
			{
				stepController.moveAsync(stepper_X, stepper_Spindle);
			}

			break;
		}
		case ID_AXIS_B:
		{
			stepper_B.setPosition(0);
			stepper_B
				.setMaxSpeed(configSetup.maxSpd_Return_Axis)
				.setAcceleration(configSetup.accel_Return_Axis)
				.setTargetAbs(target_Axis);

			SetEnable(ID_AXIS_B, true);
			if (pageCallerId == PAGE_MAIN)
			{
				// Only move the B Axis
				stepController.moveAsync(stepper_B);
			}
			else
			{
				stepController.moveAsync(stepper_B, stepper_Spindle);
			}

			break;
		}
		case ID_SPINDLE:
		{
			// Only called from pages Main and One 
			stepController.moveAsync(stepper_Spindle);
			break;
		}
	}

	while (stepController.isRunning())
	{

		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
				case 67:
				case 99: // - c
				{
					stepController.stop();

					SetEnable(ID_SPINDLE, false);
					SetEnable(ID_AXIS_Z, false);
					SetEnable(ID_AXIS_X, false);
					SetEnable(ID_AXIS_B, false);
					stepper_X.setPosition(0);
					stepper_Z.setPosition(0);

					break;
				}
			}
		}

		delay(10);
	}

	endPosition_Spindle = stepper_Spindle.getPosition();
	switch (axisId)
	{
		case ID_AXIS_Z:
		{
			endPosition_Axis = stepper_Z.getPosition();
			break;
		}
		case ID_AXIS_X:
		{
			endPosition_Axis = stepper_X.getPosition();
			break;
		}
		case ID_AXIS_B:
		{
			endPosition_Axis = stepper_B.getPosition();
			break;
		}
	}

	switch (pageCallerId)
	{
		case PAGE_MAIN:
		{
			if (axisId == ID_SPINDLE)
			{
				Serial3.print("pageMain.bt1.val=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.print("pageMain.bt1.val=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.print("pageMain.va0.val=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.print("pageMain.bt0.val=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.print("pageBE.t1.txt=");
				Serial3.write(0x22);
				Serial3.print(endPosition_Spindle);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
			}
			else
			{
				Serial3.print("pageMain.bt7.val=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.print("pageMain.bt7.val=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.print("pageMain.va0.val=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.print("pageMain.bt4.val=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				delay(10);

				Serial3.print("pageBE.t2.txt=");
				Serial3.write(0x22);
				Serial3.print(endPosition_Axis);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
			}
			break;
		}
		case PAGE_ONE:
		{
			Serial3.print("pageOne.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageOne.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageOne.va0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageOne.bt0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageBE.t1.txt=");
			Serial3.write(0x22);
			Serial3.print(endPosition_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(10);

			Serial3.print("pageBE.t2.txt=");
			Serial3.write(0x22);
			Serial3.print(endPosition_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			break;
		}
		case PAGE_SYNCZ:
		{
			Serial3.print("pageSync.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageSync.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageSync.va0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageSync.bt0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageBE.t1.txt=");
			Serial3.write(0x22);
			Serial3.print(endPosition_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(10);
			Serial3.print("pageBE.t2.txt=");
			Serial3.write(0x22);
			Serial3.print(endPosition_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			break;
		}
		case PAGE_REC:
		{
			Serial3.print("pageRec.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageRec.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageRec.va0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageRec.bt0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageBE.t1.txt=");
			Serial3.write(0x22);
			Serial3.print(endPosition_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(10);

			Serial3.print("pageBE.t2.txt=");
			Serial3.write(0x22);
			Serial3.print(endPosition_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			break;
		}
		case PAGE_INDEX:
		{
			Serial3.print("pageIndex.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageIndex.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageIndex.va0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageIndex.bt0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageBE.t1.txt=");
			Serial3.write(0x22);
			Serial3.print(endPosition_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			break;
		}
		case PAGE_BE:
		{
			Serial3.print("pageBE.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageBE.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageBE.va0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageBE.bt0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageBE.t1.txt=");
			Serial3.write(0x22);
			Serial3.print(endPosition_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			break;
		}
		case PAGE_MOVE:
		{
			Serial3.print("pageMove.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageMove.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageMove.va0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageMove.bt0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageBE.t2.txt=");
			Serial3.write(0x22);
			Serial3.print(endPosition_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			break;
		}

		case PAGE_GEO:
		{
			Serial3.print("pageGeo.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(50);
			Serial3.print("pageGeo.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(50);
			Serial3.print("pageGeo.bt0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(50);
			Serial3.print("pageGeo.va0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(50);
			Serial3.print("pageBE.t1.txt=");
			Serial3.write(0x22);
			Serial3.print(endPosition_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(10);

			Serial3.print("pageBE.t2.txt=");
			Serial3.write(0x22);
			Serial3.print(endPosition_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			initialPosition_Axis = 0;
			break;
		}
		case PAGE_GRK:
		{
			Serial3.print("pageGrk.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageGrk.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageGrk.va0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageGrk.bt0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageBE.t1.txt=");
			Serial3.write(0x22);
			Serial3.print(endPosition_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(10);

			Serial3.print("pageBE.t2.txt=");
			Serial3.write(0x22);
			Serial3.print(endPosition_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			break;
		}
		case PAGE_GRKFILE:
		{
			Serial3.print("pageGrkFile.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageGrkFile.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageGrkFile.va0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageGrkFile.bt0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageBE.t1.txt=");
			Serial3.write(0x22);
			Serial3.print(endPosition_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(10);

			Serial3.print("pageBE.t2.txt=");
			Serial3.write(0x22);
			Serial3.print(endPosition_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			break;
		}
	}

	if (pageCallerId == PAGE_MAIN)
	{
		switch (axisId)
		{
			case ID_SPINDLE:
			{
				stepper_Spindle.setPosition(0);
				endPosition_Spindle = 0;
				returnSteps_Spindle = 0;
				break;
			}
			case ID_AXIS_Z:
			{
				stepper_Z.setPosition(0);
				endPosition_Axis = 0;
				returnSteps_Axis = 0;
				break;
			}
			case ID_AXIS_X:
			{
				stepper_X.setPosition(0);
				endPosition_Axis = 0;
				returnSteps_Axis = 0;
				break;
			}
			case ID_AXIS_B:
			{
				stepper_B.setPosition(0);
				endPosition_Axis = 0;
				returnSteps_Axis = 0;
				break;
			}
		}
	}
	else
	{
		stepper_Spindle.setPosition(0);
		stepper_X.setPosition(0);
		stepper_Z.setPosition(0);
		stepper_B.setPosition(0);
		endPosition_Axis = 0;
		endPosition_Spindle = 0;
		returnSteps_Axis = 0;
		returnSteps_Spindle = 0;
	}
 
	SetEnable(ID_SPINDLE, false);
	SetEnable(ID_AXIS_Z, false);
	SetEnable(ID_AXIS_X, false);
	SetEnable(ID_AXIS_B, false);
	Serial.println("End ReturnToStartPosition()");
}

// ================================================================================= 
// RosePatternSpindle Methods
// ================================================================================= 
// Adapted from code written by Lutz Niggl: https://github.com/luni64/TeensyStep

float slideFunc_Spindle_Z(float spindleAngleZ)
{
	float phi = fmodf(spindleAngleZ * kRatio, TWO_PI);
	float retVal = spindleStepsAmplitude * cosf(phi);
	return retVal;
}

//------------------------------------------------------------------------------------
// tick()
//
// This function is called periodically with period recalcPeriod. 
// It calculates 
//  1) a new target value for the slide depending on the spindle angle
//  2) the new speed for the spindle so that it will reach the target until it is called again
void tickSpindleZ()
{
	float spindleAngleZ = stepperAxis_Z.getPosition();// *(TWO_PI / spindleStepsPerRev); //convert steps to angle
	float slidePositionZ = stepperSpindle.getPosition();
	float slideTargetZ = slideFunc_Spindle_Z(spindleAngleZ);

	float newSpeedZ = (slideTargetZ - slidePositionZ) / dtRose; // speed to reach target in given delta t (neglecting acceleration)
	float speedFacZ = newSpeedZ / newMaxSpd_RoseSpindle;              // transform in relative factor (-1.0 .. 1.0)

	rotateController1.overrideSpeed(speedFacZ);             // set new speed
}

// ToDo: Not Functional
void RosePattern_Axial_Z(int direction)
{
	IntervalTimer tickTimerZ;
	RotateControl controllerRose;
	bool runPattern = true;

	kRatio = (float)configRose.n / configRose.d;
	spindleStepsPerRev = configSetup.gearRatio_Spindle * configSetup.microsteps_Spindle * configSetup.steps360_Spindle;
	spindleStepsAmplitude = (configRose.amplitude_Axial_Z / 360) * spindleStepsPerRev;

	slideStepsAmplitude = ((configRose.amplitude_Axial_X / (configSetup.distancePerRev_AxisZ)) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z));

	newMaxSpd_RoseSpindle = configRose.maxSpd_Spindle * configRose.speedPercent_Spindle * .01;
	stepperSpindle
		.setMaxSpeed(newMaxSpd_RoseSpindle)
		.setAcceleration(configRose.accel_Spindle)
		.setPosition(0);

	newMaxSpd_RoseAxis = configRose.maxSpd_Axis_Z * configRose.speedPercent_Axis_Z * .01 * direction;
	stepperAxis_Z
		.setMaxSpeed(newMaxSpd_RoseAxis)
		.setAcceleration(configRose.accel_Axis_Z)
		.setPosition(0);
		//.setPosition(slideFunc_Axis_Z(0)); // set start position of counter

	// Enable steppers
	SetEnable(ID_SPINDLE, true);
	SetEnable(ID_AXIS_Z, true);

	rotateController1.rotateAsync(stepperAxis_Z);
	//controllerRose_Axis.overrideSpeed(0); // start with stopped slide
	controllerRose.rotateAsync(stepperSpindle); // let the spindle run with constant speed

	tickZ();

	// use a timer to periodically calculate new targets for the slide
	tickTimerZ.priority(priorityLevel); // lowest priority, potentially long caclulations need to be interruptable by TeensyStep
	tickTimerZ.begin(tickZ, recalcPeriod);

	while (runPattern)
	{
		digitalWriteFast(LED_BUILTIN, !digitalReadFast(LED_BUILTIN));

#ifdef SHOW_POSITION 
		// Track Positions not implemented while steppers are running
		//if (configGeneral.trackPositions == 1)
		//{
		//	endPosition_Axis = stepperAxis_Z.getPosition();
		//	endPosition_Spindle = stepperSpindle.getPosition();

		//	// print current values of spindle angle [rad] and slide position [steps]
		//	//float phiZ = endPosition_Spindle * (TWO_PI / spindleStepsPerRev);
		//	//Serial.printf("%f\t%d\n", phiZ, endPosition_Axis);
		//	degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
		//	distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);
		//	Serial3.print("pageBE.t1.txt=");
		//	Serial3.write(0x22);
		//	Serial3.print(degrees_Spindle);
		//	Serial3.write(0x22);
		//	Serial3.write(0xff);
		//	Serial3.write(0xff);
		//	Serial3.write(0xff);
		//	delay(5);
		//	Serial3.print("pageBE.t1.txt=");
		//	Serial3.write(0x22);
		//	Serial3.print(degrees_Spindle);
		//	Serial3.write(0x22);
		//	Serial3.write(0xff);
		//	Serial3.write(0xff);
		//	Serial3.write(0xff);
		//	delay(5);
		//	Serial3.print("pageBE.t2.txt=");
		//	Serial3.write(0x22);
		//	Serial3.print(distance_Axis);
		//	Serial3.write(0x22);
		//	Serial3.write(0xff);
		//	Serial3.write(0xff);
		//	Serial3.write(0xff);
		//}
#endif // Show Position

		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
			case 67:
			case 99:
			{
				runPattern = false;
				controllerRose.overrideSpeed(0);
				rotateController1.overrideSpeed(0);
				controllerRose.stopAsync();
				rotateController1.stopAsync();
				break;
			}
			}
		}
	}

	endPosition_Spindle = stepperSpindle.getPosition();
	endPosition_Axis = stepperAxis_Z.getPosition();
	returnSteps_Axis = endPosition_Axis;
	returnSteps_Spindle = endPosition_Spindle;
	stepperSpindle.setPosition(0);
	stepperAxis_Z.setPosition(0);

	SetEnable(ID_SPINDLE, false);
	SetEnable(ID_AXIS_Z, false);
}

// ================================================================================= 
// RosePatternZ Methods
// ================================================================================= 

float slideFunc_Axis_Z(float spindleAngleZ)
{
	float phi = fmodf(spindleAngleZ * kRatio, TWO_PI);
	float retVal = slideStepsAmplitude * cosf(phi);
	return retVal;
}

//------------------------------------------------------------------------------------
// tick()
//
// This function is called periodically with period recalcPeriod. 
// It calculates 
//  1) a new target value for the slide depending on the spindle angle
//  2) the new speed for the spindle so that it will reach the target until it is called again
void tickZ()
{
	float spindleAngleZ = stepperSpindle.getPosition() * (TWO_PI / spindleStepsPerRev); //convert steps to angle
	float slidePositionZ = stepperAxis_Z.getPosition();

	float phi = fmodf(spindleAngleZ * kRatio, TWO_PI);
	float slideTargetZ = slideStepsAmplitude * cosf(phi);

	float newSpeedZ = (slideTargetZ - slidePositionZ) / dtRose; // speed to reach target in given delta t (neglecting acceleration)
	float speedFacZ = newSpeedZ / newMaxSpd_RoseAxis;              // transform in relative factor (-1.0 .. 1.0)

	rotateController1.overrideSpeed(speedFacZ);             // set new speed
}

void RoseRadial_Z(int direction)
{
	IntervalTimer tickTimerZ;
	RotateControl controllerRoseSpindle;
	bool runPattern = true;

	kRatio = (float)configRose.n / configRose.d;
	spindleStepsPerRev = configSetup.gearRatio_Spindle * configSetup.microsteps_Spindle * configSetup.steps360_Spindle;
	slideStepsAmplitude = ((configRose.amplitude_Radial_Z / (configSetup.distancePerRev_AxisZ)) * configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z)/2;  // Amplitude is normally measured from the middle to the top

	newMaxSpd_RoseSpindle = configRose.maxSpd_Spindle * configRose.speedPercent_Spindle * .01 * direction;
	stepperSpindle
		.setMaxSpeed(newMaxSpd_RoseSpindle)
		.setAcceleration(configRose.accel_Spindle)
		.setPosition(0);

	initialPosition_Axis = static_cast<int>(slideFunc_Axis_Z(0));
	newMaxSpd_RoseAxis = configRose.maxSpd_Axis_Z * configRose.speedPercent_Axis_Z * .01 * direction;
	stepperAxis_Z
		.setMaxSpeed(newMaxSpd_RoseAxis)
		.setAcceleration(configRose.accel_Axis_Z)
		.setPosition(initialPosition_Axis); // set start position of counter
	
#ifdef DEBUG
	int beginPosition = stepperAxis_Z.getPosition();
	Serial.print("BeginPosition: ");
	Serial.println(beginPosition);
	Serial.println("---------------------------");
	Serial.print("SpindleMaxSpd:");
	Serial.println(configRose.maxSpd_Spindle);
	Serial.print("SpindleSpeedPercent:");
	Serial.println(configRose.speedPercent_Spindle);

	Serial.println("========================");
	Serial.print("microsteps_Spindle:");
	Serial.println(configSetup.microsteps_Spindle);
	Serial.print("steps360_Spindle:");
	Serial.println(configSetup.steps360_Spindle);
	Serial.print("gearRatio_Spindle:");
	Serial.println(configSetup.gearRatio_Spindle);
	Serial.print("spindleStepsPerRev:");
	Serial.println(spindleStepsPerRev);
	Serial.println("========================");
	Serial.print("AxisMaxSpd:");
	Serial.println(configRose.maxSpd_Axis_Z);
	Serial.print("AxisSpeedPercent:");
	Serial.println(configRose.amplitude_Radial_Z);
	Serial.print("AxisAccel:");
	Serial.println(configRose.accel_Axis_Z);
	Serial.println("+++++++++++++++++++++++++++");
	Serial.print("newMaxSpd_RoseAxis:");
	Serial.println(newMaxSpd_RoseAxis);
	Serial.print("amplitude_Axis_Z:");
	Serial.println(configRose.amplitude_Radial_Z);
	Serial.println("+++++++++++++++++++++++++++");
	Serial.print("initialPosition_Axis:");
	Serial.println(initialPosition_Axis);
	Serial.println("+++++++++++++++++++++++++++");

#endif // DEBUG


	// Enable steppers
	SetEnable(ID_SPINDLE, true);
	SetEnable(ID_AXIS_Z, true);

	rotateController1.rotateAsync(stepperAxis_Z);
	//controllerRose_Axis.overrideSpeed(0); // start with stopped slide
	controllerRoseSpindle.rotateAsync(stepperSpindle); // let the spindle run with constant speed

	tickZ();

	// use a timer to periodically calculate new targets for the slide
	tickTimerZ.priority(priorityLevel); // lowest priority, potentially long caclulations need to be interruptable by TeensyStep
	tickTimerZ.begin(tickZ, recalcPeriod);

	while (runPattern)
	{
#ifdef SHOW_POSITION 
		endPosition_Axis = stepperAxis_Z.getPosition();
		endPosition_Spindle = stepperSpindle.getPosition();

		// print current values of spindle angle [rad] and slide position [steps]
		//float phiZ = endPosition_Spindle * (TWO_PI / spindleStepsPerRev);
		//Serial.printf("%f\t%d\n", phiZ, endPosition_Axis);
		degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
		distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);
		Serial3.print("pageBE.t1.txt=");
		Serial3.write(0x22);
		Serial3.print(degrees_Spindle);
		Serial3.write(0x22);
		Serial3.write(0xff);
		Serial3.write(0xff);
		Serial3.write(0xff);
		delay(10);
		Serial3.print("pageBE.t2.txt=");
		Serial3.write(0x22);
		Serial3.print(distance_Axis);
		Serial3.write(0x22);
		Serial3.write(0xff);
		Serial3.write(0xff);
		Serial3.write(0xff);

#endif // Show Position
		//delay(5);
		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
				case 67:
				case 99:
				{
					runPattern = false;
					controllerRoseSpindle.overrideSpeed(0);
					rotateController1.overrideSpeed(0);
					rotateController1.stopAsync();
					controllerRoseSpindle.stopAsync();
				
					break;
				}
			}
		}
		delay(5);
	} // End while

	endPosition_Spindle = stepperSpindle.getPosition();
	endPosition_Axis = stepperAxis_Z.getPosition();
	returnSteps_Axis = endPosition_Axis - initialPosition_Axis;
	returnSteps_Spindle = endPosition_Spindle;

#ifdef DEBUG
	Serial.print("Spindle:");
	Serial.println(endPosition_Spindle);
	Serial.print("returnSteps_Spindle:");
	Serial.println(returnSteps_Spindle);
	Serial.print("Axis:");
	Serial.println(endPosition_Axis);
	Serial.print("returnSteps_Axis: ");
	Serial.println(returnSteps_Axis);
#endif // DEBUG



	stepperSpindle.setPosition(0);
	stepperAxis_Z.setPosition(0);

	SetEnable(ID_SPINDLE, false); 
	SetEnable(ID_AXIS_Z, false);
}

// ================================================================================= 
// RosePatternX Methods
// ================================================================================= 

float slideFunc_Axis_X(float spindleAngleX)
{
	float phi = fmodf(spindleAngleX * kRatio, TWO_PI);
	float retVal = slideStepsAmplitude * cosf(phi);
	return retVal;
}

////------------------------------------------------------------------------------------
//// tick()
////
//// This function is called periodically with period recalcPeriod. 
//// It calculates 
////  1) a new target value for the slide depending on the spindle angle
////  2) the new speed for the spindle so that it will reach the target until it is called again
//
void tickX()
{
	float spindleAngleX = stepperSpindle.getPosition() * (TWO_PI / spindleStepsPerRev); //convert steps to angle
	float slidePositionX = stepperAxis_X.getPosition();
	float slideTargetX = slideFunc_Axis_X(spindleAngleX);

	float newSpeedX = (slideTargetX - slidePositionX) / dtRose; // speed to reach target in given delta t (neglecting acceleration)
	float speedFacX = newSpeedX / newMaxSpd_RoseAxis;              // transform in relative factor (-1.0 .. 1.0)

	rotateController1.overrideSpeed(speedFacX);             // set new speed
}
void RoseRadial_X(int direction)
{
	IntervalTimer tickTimerX;
	RotateControl controllerRose_Spindle;
	bool runPatternX = true;

	kRatio = (float)configRose.n / configRose.d;
	spindleStepsPerRev = configSetup.gearRatio_Spindle * configSetup.microsteps_Spindle * configSetup.steps360_Spindle;
	slideStepsAmplitude = ((configRose.amplitude_Radial_X / (configSetup.distancePerRev_AxisX)) * configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X)/2;
	stepperAxis_X.setPosition(0);

	newMaxSpd_RoseSpindle = configRose.maxSpd_Spindle * configRose.speedPercent_Spindle * .01 * direction;
	stepperSpindle
		.setMaxSpeed(newMaxSpd_RoseSpindle)
		.setAcceleration(configRose.accel_Spindle)
		.setPosition(0);

	newMaxSpd_RoseAxis = configRose.maxSpd_Axis_X * configRose.speedPercent_Axis_Z * .01 * direction;
	initialPosition_Axis = static_cast<int>(slideFunc_Axis_X(0));
#ifdef DEBUG
	Serial.print("...initialPosition_Axis:");
	Serial.println(initialPosition_Axis);
#endif // DEBUG

	stepperAxis_X
		.setMaxSpeed(newMaxSpd_RoseAxis)
		.setAcceleration(configRose.accel_Axis_X)
		.setPosition(initialPosition_Axis); // set start position of counter

	// Enable steppers
	SetEnable(ID_SPINDLE, true);
	SetEnable(ID_AXIS_X, true);

	rotateController1.rotateAsync(stepperAxis_X);
	controllerRose_Spindle.rotateAsync(stepperSpindle); // let the spindle run with constant speed

	tickX();

	// use a timer to periodically calculate new targets for the slide
	tickTimerX.priority(priorityLevel); // lowest priority, potentially long caclulations need to be interruptable by TeensyStep
	tickTimerX.begin(tickX, recalcPeriod);

	while (runPatternX)
	{
		//digitalWriteFast(LED_BUILTIN, !digitalReadFast(LED_BUILTIN));
//#ifdef SHOW_POSITION 
//		endPosition_Axis = stepperAxis_X.getPosition();
//		endPosition_Spindle = stepperSpindle.getPosition();
//
//		degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
//		distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_X);
//		Serial3.print("pageBE.t1.txt=");
//		Serial3.write(0x22);
//		Serial3.print(degrees_Spindle);
//		Serial3.write(0x22);
//		Serial3.write(0xff);
//		Serial3.write(0xff);
//		Serial3.write(0xff);
//		delay(10);
//		Serial3.print("pageBE.t2.txt=");
//		Serial3.write(0x22);
//		Serial3.print(distance_Axis);
//		Serial3.write(0x22);
//		Serial3.write(0xff);
//		Serial3.write(0xff);
//		Serial3.write(0xff);
//#endif // Show Position

		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
				case 67:
				case 99:
				{
					runPatternX = false;
					controllerRose_Spindle.overrideSpeed(0);
					rotateController1.overrideSpeed(0);
					controllerRose_Spindle.stopAsync();
					rotateController1.stopAsync(); 
					SetEnable(ID_AXIS_X, false);
					break;
				}
			}
		}

		delay(5);
	}

	endPosition_Spindle = stepperSpindle.getPosition();
	endPosition_Axis = stepperAxis_X.getPosition();
	returnSteps_Axis = endPosition_Axis - initialPosition_Axis;
	returnSteps_Spindle = endPosition_Spindle;
	stepperSpindle.setPosition(0);
	stepperAxis_X.setPosition(0);
#ifdef DEBUG

	Serial.print("...returnSteps_Axis:");
	Serial.println(returnSteps_Axis);

	Serial.print("Spindle:");
	Serial.println(endPosition_Spindle);
	Serial.print("Axis:");
	Serial.println(endPosition_Axis);
#endif // DEBUG

	SetEnable(ID_SPINDLE, false);
	SetEnable(ID_AXIS_X, false);
}

// ========================================================
 //Test EEPROM
 //========================================================
/// <summary>
/// Write current Config screen eeprom values to Nextion
/// </summary>
/// <comment>
/// </comment>
/// <returns></returns>
void TestEEPROMConfig()
{
	// Write the setting to Nextion LCD
	// pageConfig
	SerialPrint("pageConfig.t19.txt=");
	SerialWrite(0x22);
	SerialPrint("Config");
	SerialWrite(0x22);
	SerialWrite(0xff);
	SerialWrite(0xff);
	SerialWrite(0xff);
	delay(2000);

	pageCallerId = GetSerialIntegerOnly();
	while (Serial3.available() > 0)
	{
		// Discard remaining serial chars
		Serial3.read();
	}
	Serial3.print("pageConfig.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Page:");
	Serial3.print(pageCallerId);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(2000);
	switch (pageCallerId)
	{
		case PAGE_MAIN:
		{
			configPageMainOne eePageMain;
			EEPROM.get(eePromAddress_pageMain, eePageMain);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("Main-MaxSpeed:");
			Serial3.print(eePageMain.maxSpd_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("Main-Accel:");
			Serial3.print(eePageMain.accel_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);

			switch (eePageMain.axisId)
			{
				case ID_AXIS_Z:
				{
					Serial3.print("pageConfig.t19.txt=");
					Serial3.write(0x22);
					Serial3.print("Z Axis-MaxSpeed:");
					Serial3.print(eePageMain.maxSpd_Axis_Z);
					Serial3.write(0x22);
					Serial3.write(0xff);
					Serial3.write(0xff);
					Serial3.write(0xff);
					delay(2000);
					Serial3.print("pageConfig.t19.txt=");
					Serial3.write(0x22);
					Serial3.print("Z Axis-Accel:");
					Serial3.print(eePageMain.accel_Axis_Z);
					Serial3.write(0x22);
					Serial3.write(0xff);
					Serial3.write(0xff);
					Serial3.write(0xff);
					delay(2000);
					break;
				}
				case ID_AXIS_X:
				{
					Serial3.print("pageConfig.t19.txt=");
					Serial3.write(0x22);
					Serial3.print("X Axis-MaxSpeed:");
					Serial3.print(eePageMain.maxSpd_Axis_X);
					Serial3.write(0x22);
					Serial3.write(0xff);
					Serial3.write(0xff);
					Serial3.write(0xff);
					delay(2000);
					Serial3.print("pageConfig.t19.txt=");
					Serial3.write(0x22);
					Serial3.print("X Axis-Accel:");
					Serial.print("1.eePageMain.accel_Axis_X:");
					Serial.println(eePageMain.accel_Axis_X);
					Serial3.println(eePageMain.accel_Axis_X);
					Serial.println("2.eePageMain.accel_Axis_X:");
					Serial3.write(0x22);
					Serial3.write(0xff);
					Serial3.write(0xff);
					Serial3.write(0xff);
					delay(2000);
					break;
				}
				case ID_AXIS_B:
				{
					Serial3.print("pageConfig.t19.txt=");
					Serial3.write(0x22);
					Serial3.print("B Axis-MaxSpeed:");
					Serial3.print(eePageMain.maxSpd_Axis_B);
					Serial3.write(0x22);
					Serial3.write(0xff);
					Serial3.write(0xff);
					Serial3.write(0xff);
					delay(2000);
					Serial3.print("pageConfig.t19.txt=");
					Serial3.write(0x22);
					Serial3.print("B Axis-Accel:");
					Serial3.print(eePageMain.accel_Axis_B);
					Serial3.write(0x22);
					Serial3.write(0xff);
					Serial3.write(0xff);
					Serial3.write(0xff);
					delay(2000);
					break;
				}
			}

			break;
		}
		case PAGE_SYNCZ:
		{
			configPageSync eePageSync;
			//configSteppers eePageSyncZ;
			EEPROM.get(eePromAddress_Sync, eePageSync);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("SyncZ-MaxSpeed:");
			Serial3.print(eePageSync.maxSpd_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("SyncZ-Accel:");
			Serial3.print(eePageSync.accel_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			if(configSync.axisId==0) // Z Axis
			{ 
				Serial3.print("pageConfig.t19.txt=");
				Serial3.write(0x22);
				Serial3.print("Z Axis-MaxSpeed:");
				Serial3.print(eePageSync.maxSpd_Axis_Z);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				delay(2000);
				Serial3.print("pageConfig.t19.txt=");
				Serial3.write(0x22);
				Serial3.print("Z Axis-Accel:");
				Serial3.print(eePageSync.accel_Axis_Z);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				delay(2000);
			}
			else
			{
				Serial3.print("pageConfig.t19.txt=");
				Serial3.write(0x22);
				Serial3.print("X Axis-MaxSpeed:");
				Serial3.print(eePageSync.maxSpd_Axis_X);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				delay(2000);
				Serial3.print("pageConfig.t19.txt=");
				Serial3.write(0x22);
				Serial3.print("X Axis-Accel:");
				Serial3.print(eePageSync.accel_Axis_X);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				delay(2000);
			}
			break;
		}

		case PAGE_REC:
		{
			configPageReci eePageReciRadialZ;
			EEPROM.get(eePromAddress_Rec, eePageReciRadialZ);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("Rec1 Spindle-MaxSpeed:");
			Serial3.print(eePageReciRadialZ.maxSpd_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("Rec1 Spindle-Accel:");
			Serial3.print(eePageReciRadialZ.accel_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);

			if (configRec.axisId == ID_AXIS_Z) // Z axis
			{
				Serial3.print("pageConfig.t19.txt=");
				Serial3.write(0x22);
				Serial3.print("Z Axis-MaxSpeed:");
				Serial3.print(eePageReciRadialZ.maxSpd_Axis_Z);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				delay(2000);
				Serial3.print("pageConfig.t19.txt=");
				Serial3.write(0x22);
				Serial3.print("Z Axis-Accel:");
				Serial3.print(eePageReciRadialZ.accel_Axis_Z);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				delay(2000);
			}
			else
			{ 
				Serial3.print("pageConfig.t19.txt=");
				Serial3.write(0x22);
				Serial3.print("X Axis-MaxSpeed:");
				Serial3.print(eePageReciRadialZ.maxSpd_Axis_X);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				delay(2000);
				Serial3.print("pageConfig.t19.txt=");
				Serial3.write(0x22);
				Serial3.print("X Axis-Accel:");
				Serial3.print(eePageReciRadialZ.accel_Axis_X);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				delay(2000);
			}
			break;
		}
		case PAGE_ONE:
		{
			configPageMainOne eePageOne;
			EEPROM.get(eePromAddress_pageOne, eePageOne);
			switch (eePageOne.activeAxis)
			{
				case ID_SPINDLE:
				{
					Serial3.print("pageConfig.t19.txt=");
					Serial3.write(0x22);
					Serial3.print("Spindle-MaxSpeed:");
					Serial3.print(eePageOne.maxSpd_Spindle);
					Serial3.write(0x22);
					Serial3.write(0xff);
					Serial3.write(0xff);
					Serial3.write(0xff);
					delay(2000);
					Serial3.print("pageConfig.t19.txt=");
					Serial3.write(0x22);
					Serial3.print("Spindle-Accel:");
					Serial3.print(eePageOne.accel_Spindle);
					Serial3.write(0x22);
					Serial3.write(0xff);
					Serial3.write(0xff);
					Serial3.write(0xff);
					delay(2000);
					break;
				}
				case ID_AXIS_Z:
				{
					Serial3.print("pageConfig.t19.txt=");
					Serial3.write(0x22);
					Serial3.print("maxSpd_Axis_Z:");
					Serial3.print(eePageOne.maxSpd_Axis_Z);
					Serial3.write(0x22);
					Serial3.write(0xff);
					Serial3.write(0xff);
					Serial3.write(0xff);
					delay(2000);
					Serial3.print("pageConfig.t19.txt=");
					Serial3.write(0x22);
					Serial3.print("accel_Axis_Z:");
					Serial3.print(eePageOne.accel_Axis_Z);
					Serial3.write(0x22);
					Serial3.write(0xff);
					Serial3.write(0xff);
					Serial3.write(0xff);
					delay(2000);
					break;
				}
				case ID_AXIS_X:
				{
					Serial3.print("pageConfig.t19.txt=");
					Serial3.write(0x22);
					Serial3.print("maxSpd_Axis_X:");
					Serial3.print(eePageOne.maxSpd_Axis_X);
					Serial3.write(0x22);
					Serial3.write(0xff);
					Serial3.write(0xff);
					Serial3.write(0xff);
					delay(2000);
					Serial3.print("pageConfig.t19.txt=");
					Serial3.write(0x22);
					Serial3.print("accel_Axis_X:");
					Serial3.print(eePageOne.accel_Axis_X);
					Serial3.write(0x22);
					Serial3.write(0xff);
					Serial3.write(0xff);
					Serial3.write(0xff);
					delay(2000);
					break;
				}
				case ID_AXIS_B:
				{
					Serial3.print("pageConfig.t19.txt=");
					Serial3.write(0x22);
					Serial3.print("maxSpd_Axis_B:");
					Serial3.print(eePageOne.maxSpd_Axis_B);
					Serial3.write(0x22);
					Serial3.write(0xff);
					Serial3.write(0xff);
					Serial3.write(0xff);
					delay(2000);
					Serial3.print("pageConfig.t19.txt=");
					Serial3.write(0x22);
					Serial3.print("accel_Axis_B:");
					Serial3.print(eePageOne.accel_Axis_B);
					Serial3.write(0x22);
					Serial3.write(0xff);
					Serial3.write(0xff);
					Serial3.write(0xff);
					delay(2000);
					break;
				}
			}
			break;
		}
		case PAGE_INDEX:
		{
			configPageIndex eepageIndex;
			EEPROM.get(eePromAddress_Index_Main, eepageIndex);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("Index1-MaxSpeed:");
			Serial3.print(eepageIndex.maxSpd);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("Index1-Accel:");
			Serial3.print(eepageIndex.accel);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			break;
		}
		case PAGE_MOVE:
		{
			configPageMov eepageMove;
			EEPROM.get(eePromAddress_pageMov, eepageMove);
			if(eepageMove.axisId == ID_AXIS_Z)
			{ 
				Serial3.print("pageConfig.t19.txt=");
				Serial3.write(0x22);
				Serial3.print("MoveZ-MaxSpeed:");
				Serial3.print(eepageMove.maxSpd_Axis_Z);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				delay(2000);
				Serial3.print("pageConfig.t19.txt=");
				Serial3.write(0x22);
				Serial3.print("MoveZ-Accel:");
				Serial3.print(eepageMove.accel_Axis_Z);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				delay(2000);
			}
			else
			{
				Serial3.print("pageConfig.t19.txt=");
				Serial3.write(0x22);
				Serial3.print("MoveX-MaxSpeed:");
				Serial3.print(eepageMove.maxSpd_Axis_X);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				delay(2000);
				Serial3.print("pageConfig.t19.txt=");
				Serial3.write(0x22);
				Serial3.print("MoveX-Accel:");
				Serial3.print(eepageMove.accel_Axis_X);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				delay(2000);

			}
			break;
		}
		case PAGE_GEO:
		{
			configPageRose eePageRose;
			EEPROM.get(eePromAddress_Rose, eePageRose);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("Rose-Spindle MaxSpeed:");
			Serial3.print(eePageRose.maxSpd_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("Rose-Spindle Accel:");
			Serial3.print(eePageRose.accel_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("Rose-ZAxis MaxSpeed:");
			Serial3.print(eePageRose.maxSpd_Axis_Z);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("Rose-ZAxis Accel:");
			Serial3.print(eePageRose.accel_Axis_Z);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("Rose-XAxis MaxSpeed:");
			Serial3.print(eePageRose.maxSpd_Axis_X);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("Rose-XAxis Accel:");
			Serial3.print(eePageRose.accel_Axis_X);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			break;
		}
		case PAGE_GRK:
		case PAGE_GRKFILE:
		{
#ifdef DEBUG
			Serial.print("pageCallerId:");
			Serial.println(pageCallerId);
			Serial.print("eePageMain.axisId:");
			Serial.println(configGreekKey_Main.axisId);
#endif // DEBUG
			if (configGreekKey_Main.axisId == ID_AXIS_Z)
			{			
				configSteppers eePageGrkZ;
				EEPROM.get(eePromAddress_GreekKey_Z, eePageGrkZ);
				Serial3.print("pageConfig.t19.txt=");
				Serial3.write(0x22);
				Serial3.print("Grk-Spindle MaxSpeed:");
				Serial3.print(eePageGrkZ.maxSpd_Spindle);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				delay(2000);
				Serial3.print("pageConfig.t19.txt=");
				Serial3.write(0x22);
				Serial3.print("Grk-Spindle Accel:");
				Serial3.print(eePageGrkZ.accel_Spindle);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				delay(2000);

				Serial3.print("pageConfig.t19.txt=");
				Serial3.write(0x22);
				Serial3.print("Grk-ZAxis MaxSpeed:");
				Serial3.print(eePageGrkZ.maxSpd_Axis);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				delay(2000);
				Serial3.print("pageConfig.t19.txt=");
				Serial3.write(0x22);
				Serial3.print("Grk-ZAxis Accel:");
				Serial3.print(eePageGrkZ.accel_Axis);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				delay(2000);
			}
			else  // X Axis
			{ 
				configSteppers eePageGrkX;
				EEPROM.get(eePromAddress_GreekKey_X, eePageGrkX);
				Serial3.print("pageConfig.t19.txt=");
				Serial3.write(0x22);
				Serial3.print("Grk-Spindle MaxSpeed:");
				Serial3.print(eePageGrkX.maxSpd_Spindle);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				delay(2000);
				Serial3.print("pageConfig.t19.txt=");
				Serial3.write(0x22);
				Serial3.print("Grk-Spindle Accel:");
				Serial3.print(eePageGrkX.accel_Spindle);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				delay(2000);

				Serial3.print("pageConfig.t19.txt=");
				Serial3.write(0x22);
				Serial3.print("Grk-XAxis MaxSpeed:");
				Serial3.print(eePageGrkX.maxSpd_Axis);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				delay(2000);
				Serial3.print("pageConfig.t19.txt=");
				Serial3.write(0x22);
				Serial3.print("Grk-XAxis Accel:");
				Serial3.print(eePageGrkX.accel_Axis);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				delay(2000);
			}
			break;
		}
	}	

	// Update Nextion
	Serial3.print("pageConfig.bt1.bco=7158");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageConfig.bt1.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
}

/// <summary>
/// Write current Setup screen eeprom values to Nextion
/// </summary>
/// <comment>
/// </comment>
/// <returns></returns>
void TestEEPROMSetup()
{
	configPageSetup eePromPageSetup;
	EEPROM.get(eePromAddress_PageSetup, eePromPageSetup);
	delay(2000);

	// Write the setting to Nextion LCD
	// pageSetup
	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Setup:");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(1000);

	// pageSetup
	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Spindle-Microsteps:");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(15);
	Serial3.print("pageSetup.t63.txt=");
	Serial3.write(0x22);
	Serial3.print(eePromPageSetup.microsteps_Spindle);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(1700);

	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Spindle-Steps360:");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(15);
	Serial3.print("pageSetup.t63.txt=");
	Serial3.write(0x22);
	Serial3.print(eePromPageSetup.steps360_Spindle);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(1700);

	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Spindle-Gear Ratio:");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(15);
	Serial3.print("pageSetup.t63.txt=");
	Serial3.write(0x22);
	Serial3.print(eePromPageSetup.gearRatio_Spindle);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(1700);

	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Spindle-Enable:");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(15);
	Serial3.print("pageSetup.t63.txt=");
	Serial3.write(0x22);
	Serial3.print(eePromPageSetup.polarity_Spindle? "Low":"High");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(1700);

	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Z Axis-Microsteps:");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(15);
	Serial3.print("pageSetup.t63.txt=");
	Serial3.write(0x22);
	Serial3.print(eePromPageSetup.microsteps_Axis_Z);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(1700);
	
	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Z Axis-Steps/360:");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(15);
	Serial3.print("pageSetup.t63.txt=");
	Serial3.write(0x22);
	Serial3.print(eePromPageSetup.steps360_Axis_Z);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(1700);

	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Z Axis-Dist/360:");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(15);
	Serial3.print("pageSetup.t63.txt=");
	Serial3.write(0x22);
	Serial3.print(eePromPageSetup.distancePerRev_AxisZ);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(1700);

	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Z Axis-Enable:");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(15);
	Serial3.print("pageSetup.t63.txt=");
	Serial3.write(0x22);
	Serial3.print(eePromPageSetup.polarity_Axis_Z ? "Low" : "High");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(1700);

#ifndef TWO_AXES_V2
	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("X Axis-Microsteps:");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(15);
	Serial3.print("pageSetup.t63.txt=");
	Serial3.write(0x22);
	Serial3.print(eePromPageSetup.microsteps_Axis_X);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(1700);

	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("X Axis-Steps/360:");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(15);
	Serial3.print("pageSetup.t63.txt=");
	Serial3.write(0x22);
	Serial3.print(eePromPageSetup.steps360_Axis_X);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(1700);

	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("X Axis-Dist/360:");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(15);
	Serial3.print("pageSetup.t63.txt=");
	Serial3.write(0x22);
	Serial3.print(eePromPageSetup.distancePerRev_AxisX);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(1700);

	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("X Axis-Enable:");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(15);
	Serial3.print("pageSetup.t63.txt=");
	Serial3.write(0x22);
	Serial3.print(eePromPageSetup.polarity_Axis_X ? "Low" : "High");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(1700);
#endif // TWO_AXES_V2
#ifdef FOUR_AXES
	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("B Axis-Microsteps:");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(15);
	Serial3.print("pageSetup.t63.txt=");
	Serial3.write(0x22);
	Serial3.print(eePromPageSetup.microsteps_Axis_B);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(1700);

	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("B Axis-Steps360:");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(15);
	Serial3.print("pageSetup.t63.txt=");
	Serial3.write(0x22);
	Serial3.print(eePromPageSetup.steps360_Axis_B);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(1700);
	//Serial3.print("pageSetup.t19.txt=");
	//Serial3.write(0x22);
	//Serial3.print("B Axis-Gear Ratio:");
	//Serial3.print(eePromConfig.gearRatio_AxisB);
	//Serial3.write(0x22);
	//Serial3.write(0xff);
	//Serial3.write(0xff);
	//Serial3.write(0xff);
	//delay(2000);
	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("B Axis-Enable:");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(15);
	Serial3.print("pageSetup.t63.txt=");
	Serial3.write(0x22);
	Serial3.print(eePromPageSetup.polarity_Axis_B ? "Low" : "High");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(1700);
#endif // FOUR_AXES
	
	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Limit Min Z:");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(15);
	Serial3.print("pageSetup.t63.txt=");
	Serial3.write(0x22);
	Serial3.print(configSetup.limit_Min_Z);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(1700);

	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Limit Max Z:");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(15);
	Serial3.print("pageSetup.t63.txt=");
	Serial3.write(0x22);
	Serial3.print(configSetup.limit_Max_Z);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(1700);

#ifndef TWO_AXES_V2
	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Limit Min X:");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(15);
	Serial3.print("pageSetup.t63.txt=");
	Serial3.write(0x22);
	Serial3.print(configSetup.limit_Min_X);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(1700);

	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Limit Max X:");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(15);
	Serial3.print("pageSetup.t63.txt=");
	Serial3.write(0x22);
	Serial3.print(configSetup.limit_Max_X);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(1700);
#endif
#ifdef FOUR_AXES
	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Limit Min B:");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(15);
	Serial3.print("pageSetup.t63.txt=");
	Serial3.write(0x22);
	Serial3.print(configSetup.limit_Min_B);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(1700);

	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Limit Max B:");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(15);
	Serial3.print("pageSetup.t63.txt=");
	Serial3.write(0x22);
	Serial3.print(configSetup.limit_Max_B);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(1700);

	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Return Spindle MaxSpd:");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(15);
	Serial3.print("pageSetup.t63.txt=");
	Serial3.write(0x22);
	Serial3.print(configSetup.maxSpd_Return_Spindle);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(1700);

	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Return Spindle Accel:");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(15);
	Serial3.print("pageSetup.t63.txt=");
	Serial3.write(0x22);
	Serial3.print(configSetup.accel_Return_Spindle);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(1700);

	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Return Axis MaxSpd:");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(15);
	Serial3.print("pageSetup.t63.txt=");
	Serial3.write(0x22);
	Serial3.print(configSetup.maxSpd_Return_Axis);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(1700);

	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Return Axis Accel :");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(15);
	Serial3.print("pageSetup.t63.txt=");
	Serial3.write(0x22);
	Serial3.print(configSetup.accel_Return_Axis);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(1700);

	// Update Nextion
	Serial3.print("pageSetup.bt1.bco=23964");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageSetup.bt1.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
#endif
}

/// <summary>
/// Set microstepping mode
/// </summary>
/// <comment>
/// </comment>
/// <param name="microsteps">Microsteps</param>
/// <param name="pinMs0">Pin MS0</param>
/// <param name="pinMs1">Pin MS1</param>
/// <param name="pinMs2">Pin MS2</param>
/// <returns></returns>
void SetMicrosteppingMode(int microsteps, int pinMs0, int pinMs1, int pinMs2)
{
	// Settings for DRV8825 and MKS-LV8729
	switch (microsteps)
	{
		case 1:
		{
			digitalWrite(pinMs0, LOW);
			digitalWrite(pinMs1, LOW);
			digitalWrite(pinMs2, LOW);
			break;
		}
		case 2:
		{
			digitalWrite(pinMs0, HIGH);
			digitalWrite(pinMs1, LOW);
			digitalWrite(pinMs2, LOW);
			break;
		}
		case 4:
		{
			digitalWrite(pinMs0, LOW);
			digitalWrite(pinMs1, HIGH);
			digitalWrite(pinMs2, LOW);
			break;
		}
		case 8:
		{
			digitalWrite(pinMs0, HIGH);
			digitalWrite(pinMs1, HIGH);
			digitalWrite(pinMs2, LOW);
			break;
		}
		case 16:
		{
			digitalWrite(pinMs0, LOW);
			digitalWrite(pinMs1, LOW);
			digitalWrite(pinMs2, HIGH);
			break;
		}
		case 32:
		{
			digitalWrite(pinMs0, HIGH);
			digitalWrite(pinMs1, LOW);
			digitalWrite(pinMs2, HIGH);
			break;
		}
		case 64:
		{
			digitalWrite(pinMs0, LOW);
			digitalWrite(pinMs1, HIGH);
			digitalWrite(pinMs2, HIGH);
			break;
		}
		case 128:
		{
			digitalWrite(pinMs0, HIGH);
			digitalWrite(pinMs1, HIGH);
			digitalWrite(pinMs2, HIGH);
			break;
		}
	}
}


void LoadSettings()
{
	LoadSettings_PageSetup();
	LoadSettings_PageSync();
	LoadSettings_PageMain();
	LoadSettings_PageOne();
	LoadSettings_PageRec();
	LoadSettings_PageGrk();
	LoadSettings_PageMove();
	LoadSettings_PageGeo();
	LoadSettings_PageIndex();
}

void LoadSettings_PageIndex()
{
	// Index Main
	const char* iniKey = "Ind";
	const char* iniValue = "IndexId";
	int eePromAddress_Nextion = 370;
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_Main.indexId = (int)returnVal;

	iniValue = "MaxSpeed_Spindle";
	eePromAddress_Nextion = 904;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_Main.maxSpd = (int)returnVal;

	iniValue = "Accel_Spindle";
	eePromAddress_Nextion = 944;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_Main.accel = (int)returnVal;

	iniValue = "SpeedPercentage_Spindle";
	eePromAddress_Nextion = 104;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_Main.speedPercent = (int)returnVal;

	// Index 1
	iniValue = "DivisionsOrDegrees_1";
	eePromAddress_Nextion = 100;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_1.degreeOrDivision = (int)returnVal;

	iniValue = "FixedOrFile_1";
	eePromAddress_Nextion = 574;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_1.fileOrFixed = (int)returnVal;

	// Index 2
	iniValue = "DivisionsOrDegrees_1";
	eePromAddress_Nextion = 162;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_2.degreeOrDivision = (int)returnVal;

	iniValue = "FixedOrFile_2";
	eePromAddress_Nextion = 708;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_2.fileOrFixed = (int)returnVal;

	// Index 3
	iniValue = "DivisionsOrDegrees_3";
	eePromAddress_Nextion = 154;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_3.degreeOrDivision = (int)returnVal;

	iniValue = "FixedOrFile_3";
	eePromAddress_Nextion = 592;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_3.fileOrFixed = (int)returnVal;

	EEPROM.put(eePromAddress_Index_Main, configIndex_Main);
	EEPROM.put(eePromAddress_Index_1, configIndex_1);
	EEPROM.put(eePromAddress_Index_2, configIndex_2);
	EEPROM.put(eePromAddress_Index_3, configIndex_3);
}

void LoadSettings_PageGeo()
{
	const char* iniKey = "Geo";
	const char* iniValue = "AxisId";
	int eePromAddress_Nextion = 96;
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.axisId = (int)returnVal;

	iniValue = "RadialOrAxial";
	eePromAddress_Nextion = 948;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.radial_Axial = (int)returnVal;

	iniValue = "MaxSpeed_Spindle";
	eePromAddress_Nextion = 124;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.maxSpd_Spindle = (int)returnVal;

	iniValue = "Accel_Spindle";
	eePromAddress_Nextion = 128;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.accel_Spindle = (int)returnVal;

	iniValue = "SpeedPercentage_Spindle";
	eePromAddress_Nextion = 112;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.speedPercent_Spindle = (int)returnVal;

	// Z axis
	iniValue = "MaxSpeed_Z";
	eePromAddress_Nextion = 166;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.maxSpd_Axis_Z = (int)returnVal;

	iniValue = "Accel_Z";
	eePromAddress_Nextion = 136;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.accel_Axis_Z = (int)returnVal;

	iniValue = "SpeedPercentage_Z";
	eePromAddress_Nextion = 132;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.speedPercent_Axis_Z = (int)returnVal;

	// X axis
	iniValue = "MaxSpeed_X";
	eePromAddress_Nextion = 148;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.maxSpd_Axis_X = (int)returnVal;

	iniValue = "Accel_X";
	eePromAddress_Nextion = 208;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.accel_Axis_X = (int)returnVal;

	iniValue = "SpeedPercentage_X";
	eePromAddress_Nextion = 712;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.speedPercent_Axis_X = (int)returnVal;

	EEPROM.put(eePromAddress_Rose, configRose);
}

void LoadSettings_PageMove()
{
	const char* iniKey = "Mov";
	const char* iniValue = "AxisId";
	int eePromAddress_Nextion = 456;
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMove.axisId = (int)returnVal;

	// Z axis
	iniValue = "MaxSpeed_Z";
	eePromAddress_Nextion = 968;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMove.maxSpd_Axis_Z = (int)returnVal;

	iniValue = "Accel_Z";
	eePromAddress_Nextion = 988;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMove.accel_Axis_Z = (int)returnVal;

	iniValue = "SpeedPercentage_Z";
	eePromAddress_Nextion = 170;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMove.speedPercent_Axis_Z = (int)returnVal;

	// X axis
	iniValue = "MaxSpeed_X";
	eePromAddress_Nextion = 1000;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMove.maxSpd_Axis_X = (int)returnVal;

	iniValue = "Accel_X";
	eePromAddress_Nextion = 1012;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMove.accel_Axis_X = (int)returnVal;

	iniValue = "SpeedPercentage_X";
	eePromAddress_Nextion = 36;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMove.speedPercent_Axis_X = (int)returnVal;

	EEPROM.put(eePromAddress_pageMov, configMove);
}

void LoadSettings_PageGrk()
{
	// Greek Key Main
	const char* iniKey = "Grk";
	const char* iniValue = "AxisId";
	int eePromAddress_Nextion = 766;
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey_Main.axisId = (int)returnVal;

	iniValue = "RadialOrAxial";
	eePromAddress_Nextion = 432;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey_Main.radialOrAxial_Pattern = (int)returnVal;

	iniValue = "FileOrPattern";
	eePromAddress_Nextion = 108;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey_Main.fileOrPattern = (int)returnVal;

	// Greek Key Spindle (Shared by Z and X in Pattern and File)
	iniValue = "MaxSpeed_Spindle";
	eePromAddress_Nextion = 264;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey_Z.maxSpd_Spindle = (int)returnVal;
	configGreekKey_X.maxSpd_Spindle = (int)returnVal;

	iniValue = "Accel_Spindle";
	eePromAddress_Nextion = 404;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey_Z.accel_Spindle = (int)returnVal;
	configGreekKey_X.accel_Spindle = (int)returnVal;

	iniValue = "SpeedPercentage_Spindle";
	eePromAddress_Nextion = 356;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey_Z.speedPercent_Spindle = (int)returnVal;
	configGreekKey_X.speedPercent_Spindle = (int)returnVal;

	// Z axis
	iniValue = "MaxSpeed_Z";
	eePromAddress_Nextion = 280;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey_Z.maxSpd_Axis = (int)returnVal;

	iniValue = "Accel_Z";
	eePromAddress_Nextion = 464;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey_Z.accel_Axis = (int)returnVal;

	iniValue = "SpeedPercentage_Z";
	eePromAddress_Nextion = 374;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey_Z.speedPercent_Axis = (int)returnVal;

	// X axis
	iniValue = "MaxSpeed_X";
	eePromAddress_Nextion = 310;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey_X.maxSpd_Axis = (int)returnVal;

	iniValue = "Accel_X";
	eePromAddress_Nextion = 558;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey_X.accel_Axis = (int)returnVal;

	iniValue = "SpeedPercentage_X";
	eePromAddress_Nextion = 396;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey_X.speedPercent_Axis = (int)returnVal;

	EEPROM.put(eePromAddress_GreekKey_Main, configGreekKey_Main);
	EEPROM.put(eePromAddress_GreekKey_X, configGreekKey_X);
	EEPROM.put(eePromAddress_GreekKey_Z, configGreekKey_Z);
}

void LoadSettings_PageRec()
{
	const char* iniKey = "Rec";
	const char* iniValue = "AxisId";
	int eePromAddress_Nextion = 144;
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.axisId = (int)returnVal;

	iniValue = "RadialOrAxial"; 
	eePromAddress_Nextion = 562;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.radial_axial = (int)returnVal;

	iniValue = "MaxSpeed_Spindle";
	eePromAddress_Nextion = 896;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.speedPercent_Spindle = (int)returnVal;

	iniValue = "Accel_Spindle";
	eePromAddress_Nextion = 936;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.accel_Spindle = (int)returnVal;

	iniValue = "SpeedPercentage_Spindle";
	eePromAddress_Nextion = 804;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.speedPercent_Spindle = (int)returnVal;

	iniValue = "MaxSpeed_Z";
	eePromAddress_Nextion = 964;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.maxSpd_Axis_Z = (int)returnVal;

	iniValue = "Accel_Z";
	eePromAddress_Nextion = 932;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.accel_Axis_Z = (int)returnVal;

	iniValue = "SpeedPercentage_Z";
	eePromAddress_Nextion = 808;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.speedPercent_Axis_Z = (int)returnVal;

	iniValue = "MaxSpeed_X";
	eePromAddress_Nextion = 960;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.speedPercent_Axis_X = (int)returnVal;

	iniValue = "Accel_X";
	eePromAddress_Nextion = 980;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.speedPercent_Axis_X = (int)returnVal;

	iniValue = "SpeedPercentage_X";
	eePromAddress_Nextion = 800;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.speedPercent_Axis_X = (int)returnVal;
	
	EEPROM.put(eePromAddress_Rec, configRec);
}

void LoadSettings_PageOne()
{
	const char* iniKey = "One";
	const char* iniValue = "AxisId";
	int eePromAddress_Nextion = 530;
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configOne.axisId = (int)returnVal;

	iniValue = "MaxSpeed_Spindle";
	eePromAddress_Nextion = 900;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configOne.maxSpd_Spindle = (int)returnVal;

	iniValue = "Accel_Spindle";
	eePromAddress_Nextion = 940;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configOne.accel_Spindle = (int)returnVal;

	iniValue = "SpeedPercentage_Spindle";
	eePromAddress_Nextion = 20;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configOne.speedPercent_Spindle = (int)returnVal;

	iniValue = "MaxSpeed_Z";
	eePromAddress_Nextion = 50;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configOne.maxSpd_Axis_Z = (int)returnVal;

	iniValue = "Accel_Z";
	eePromAddress_Nextion = 54;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configOne.accel_Axis_Z = (int)returnVal;

	iniValue = "SpeedPercentage_Z";
	eePromAddress_Nextion = 58;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configOne.speedPercent_Axis_Z = (int)returnVal;

	iniValue = "MaxSpeed_X";
	eePromAddress_Nextion = 62;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configOne.maxSpd_Axis_X = (int)returnVal;

	iniValue = "Accel_X";
	eePromAddress_Nextion = 66;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configOne.accel_Axis_X = (int)returnVal;

	iniValue = "SpeedPercentage_X";
	eePromAddress_Nextion = 70;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configOne.speedPercent_Axis_X = (int)returnVal;
	
	iniValue = "MaxSpeed_B";
	eePromAddress_Nextion = 74;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configOne.maxSpd_Axis_B = (int)returnVal;

	iniValue = "Accel_B";
	eePromAddress_Nextion = 522;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configOne.accel_Axis_B = (int)returnVal;

	iniValue = "SpeedPercentage_B";
	eePromAddress_Nextion = 526;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configOne.speedPercent_Axis_B = (int)returnVal;

	EEPROM.put(eePromAddress_pageOne, configOne);
}

void LoadSettings_PageMain()
{
	const char* iniKey = "Main";
	const char* iniValue = "AxisId";
	int eePromAddress_Nextion = 570;
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configPageMain.axisId = (int)returnVal;
	
	iniValue = "MaxSpeed_Spindle";
	eePromAddress_Nextion = 872;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configPageMain.maxSpd_Spindle = (int)returnVal;

	iniValue = "Accel_Spindle";
	eePromAddress_Nextion = 912;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configPageMain.accel_Spindle = (int)returnVal;

	iniValue = "SpeedPercentage_Spindle";
	eePromAddress_Nextion = 0;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configPageMain.speedPercent_Spindle = (int)returnVal;

	iniValue = "MaxSpeed_Z";
	eePromAddress_Nextion = 952;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configPageMain.maxSpd_Axis_Z = (int)returnVal;

	iniValue = "Accel_Z";
	eePromAddress_Nextion = 972;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configPageMain.accel_Axis_Z = (int)returnVal;

	iniValue = "SpeedPercentage_Z";
	eePromAddress_Nextion = 440;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configPageMain.speedPercent_Axis_Z = (int)returnVal;

	iniValue = "MaxSpeed_X";
	eePromAddress_Nextion = 992;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configPageMain.maxSpd_Axis_X = (int)returnVal;

	iniValue = "Accel_X";
	eePromAddress_Nextion = 1004;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configPageMain.accel_Axis_X = (int)returnVal;

	iniValue = "SpeedPercentage_X";
	eePromAddress_Nextion = 8;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configPageMain.speedPercent_Axis_X = (int)returnVal;

	iniValue = "MaxSpeed_B";
	eePromAddress_Nextion = 1016;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configPageMain.maxSpd_Axis_B = (int)returnVal;

	iniValue = "Accel_B";
	eePromAddress_Nextion = 1020;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configPageMain.accel_Axis_B = (int)returnVal;

	iniValue = "SpeedPercentage_B";
	eePromAddress_Nextion = 12;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configPageMain.speedPercent_Axis_B = (int)returnVal;

	EEPROM.put(eePromAddress_pageMain, configPageMain);
}

void LoadSettings_PageSync()
{
	const char* iniKey = "Sync";
	const char* iniValue = "AxisId";
	int eePromAddress_Nextion = 608;
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.axisId = (int)returnVal;

	iniValue = "LeftOrRight"; //Helix Type
	eePromAddress_Nextion = 420;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.helixType = (int)returnVal;

	iniValue = "MaxSpeed_Spindle";
	eePromAddress_Nextion = 884;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.maxSpd_Spindle = (int)returnVal;

	iniValue = "Accel_Spindle";
	eePromAddress_Nextion = 924;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.accel_Spindle = (int)returnVal;

	iniValue = "SpeedPercentage_Spindle";
	eePromAddress_Nextion = 412;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.speedPercent_Spindle = (int)returnVal;

	iniValue = "MaxSpeed_Z";
	eePromAddress_Nextion = 956;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.maxSpd_Axis_Z = (int)returnVal;

	iniValue = "Accel_Z";
	eePromAddress_Nextion = 976;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.accel_Axis_Z = (int)returnVal;

	iniValue = "SpeedPercentage_Z";
	eePromAddress_Nextion = 400;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.speedPercent_Axis_Z = (int)returnVal;

	iniValue = "MaxSpeed_X";
	eePromAddress_Nextion = 996;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.maxSpd_Axis_X = (int)returnVal;

	iniValue = "Accel_X";
	eePromAddress_Nextion = 1008;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.accel_Axis_X = (int)returnVal;

	iniValue = "SpeedPercentage_X";
	eePromAddress_Nextion = 24;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.speedPercent_Axis_X = (int)returnVal;

	EEPROM.put(eePromAddress_Sync, configSync);
}

void LoadSettings_PageSetup()
{
	const char* iniKey = "Setup";
	const char* iniValue = "Microsteps_Spindle";
	int eePromAddress_Nextion = 212;
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.microsteps_Spindle = (int)returnVal;

	iniValue = "StepsPer360_Spindle";
	eePromAddress_Nextion = 244;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.steps360_Spindle = (int)returnVal;

	iniValue = "GearRatio_Spindle";
	eePromAddress_Nextion = 270;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configSetup.gearRatio_Spindle = returnVal;

	iniValue = "Polarity_Spindle";
	eePromAddress_Nextion = 248;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	returnVal >= 1 ? (configSetup.polarity_Spindle = true) : (configSetup.polarity_Spindle = false);

	// Z Axis
	iniValue = "StepsPer360_Z";
	eePromAddress_Nextion = 468;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.steps360_Axis_Z = (int)returnVal;

	iniValue = "Microsteps_Z";
	eePromAddress_Nextion = 460;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.microsteps_Axis_Z = (int)returnVal;

	iniValue = "DistancePer360_Z";
	eePromAddress_Nextion = 512;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configSetup.distancePerRev_AxisZ = returnVal;

	iniValue = "Polarity_Z";
	eePromAddress_Nextion = 252;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	returnVal >= 1 ? (configSetup.polarity_Axis_Z = true) : (configSetup.polarity_Axis_Z = false);

	iniValue = "Min_Z";
	eePromAddress_Nextion = 856;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.limit_Min_Z = (int)returnVal;

	iniValue = "Max_Z";
	eePromAddress_Nextion = 860;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.limit_Max_Z = (int)returnVal;

	// X Axis
	iniValue = "StepsPer360_X";
	eePromAddress_Nextion = 484;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.steps360_Axis_X = (int)returnVal;

	iniValue = "Microsteps_X";
	eePromAddress_Nextion = 480;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.microsteps_Axis_X = (int)returnVal;

	iniValue = "DistancePer360_X";
	eePromAddress_Nextion = 488;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configSetup.distancePerRev_AxisX = returnVal;

	iniValue = "Polarity_X";
	eePromAddress_Nextion = 256;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	returnVal >= 1 ? (configSetup.polarity_Axis_X = true) : (configSetup.polarity_Axis_X = false);

	iniValue = "Min_X";
	eePromAddress_Nextion = 864;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.limit_Min_X = (int)returnVal;

	iniValue = "Max_X";
	eePromAddress_Nextion = 868;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.limit_Max_X = (int)returnVal;

	// B Axis
	iniValue = "StepsPer360_B";
	eePromAddress_Nextion = 544;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.steps360_Axis_B = (int)returnVal;

	iniValue = "Microsteps_B";
	eePromAddress_Nextion = 540;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.microsteps_Axis_B = (int)returnVal;

	iniValue = "GearRatio_B";
	eePromAddress_Nextion = 548;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configSetup.gearRatio_AxisB = returnVal;

	iniValue = "Polarity_B";
	eePromAddress_Nextion = 260;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	returnVal >= 1 ? (configSetup.polarity_Axis_B = true) : (configSetup.polarity_Axis_B = false);

	iniValue = "Min_B";
	eePromAddress_Nextion = 848;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.limit_Min_B = (int)returnVal;

	iniValue = "Max_B";
	eePromAddress_Nextion = 852;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.limit_Max_B = (int)returnVal;

	// Return settings
	iniValue = "MaxSpeed_Spindle";
	eePromAddress_Nextion = 80;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.maxSpd_Return_Spindle = (int)returnVal;

	iniValue = "Accel_Spindle";
	eePromAddress_Nextion = 84;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.accel_Return_Spindle = (int)returnVal;

	iniValue = "MaxSpeed_Axis";
	eePromAddress_Nextion = 88;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.maxSpd_Return_Axis = (int)returnVal;

	iniValue = "Accel_Axis";
	eePromAddress_Nextion = 92;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.accel_Return_Axis = (int)returnVal;

	EEPROM.put(eePromAddress_PageSetup, configSetup);
}

float GetIniValue(const char* iniKey, const char* iniValue, int eePromNextion, bool asString)
{
	const size_t bufferLen = 80;
	char buffer[bufferLen];

	const char *filename = "/4Axes.ini";
#ifdef TWO_AXES_V2
	filename = "/2Axes.ini";
#endif // TWO_AXES_V2

	IniFile ini(filename);

	if (!ini.open()) {
		Serial.print("Ini file ");
		Serial.print(filename);
		Serial.println(" does not exist");
		return 0;
	}

	Serial.println("Ini file opened");

	// Check the file is valid. This can be used to warn if any lines are longer than the buffer.
	if (!ini.validate(buffer, bufferLen)) {

		Serial.print("ini file ");
		Serial.print(ini.getFilename());
		Serial.print(" not valid: ");

		return 0;
	}

	// Process values
	if (ini.getValue(iniKey, iniValue, buffer, bufferLen))
	{
		Serial.println(buffer);

		Serial3.print("wepo ");

		if (asString)
		{
			Serial3.write(0x22);
			Serial3.print(buffer);
			Serial3.write(0x22);
		}
		else
		{
			Serial3.print(buffer);
		}
		Serial3.print(",");
		Serial3.print(eePromNextion);
		Serial3.write(0xff);
		Serial3.write(0xff);
		Serial3.write(0xff);

#ifdef DEBUG
		Serial.print("wepo ");
		if (asString)
		{
			Serial.write(0x22);
			Serial.print(buffer);
			Serial.write(0x22);
		}
		else
		{
			Serial.print(buffer);
		}
		Serial.print(",");
		Serial.println(eePromNextion);
#endif // DEBUG
	}
	else
	{
		// Serial.println("Bad");
	}
	iniKey = "Main";

	return atof(buffer);
}
