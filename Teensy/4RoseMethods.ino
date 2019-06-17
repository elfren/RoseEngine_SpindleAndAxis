/* *****************************************************************
* Rose Engine utility methods
* Author: Edward French
******************************************************************/
// ================================================================================= 
// Utility Methods
// ================================================================================= 
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

//////////////////////////////////
/// <summary>
/// Get a float from Bluetooth serial input (Serial3)
/// </summary>
/// <returns>Float from Bluetooth serial input</returns>
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

/////////////////////////////////////////
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
	float retVal = (steps / (configMain.steps360_Spindle * configMain.microsteps_Spindle* configMain.gearRatio_Spindle)) * 360;
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
			retVal = (steps * configMain.distancePerRev_AxisZ) / (configMain.steps360_Axis_Z * configMain.microsteps_Axis_Z);
			break;
		}
		case ID_AXIS_X:
		{
			retVal = (steps * configMain.distancePerRev_AxisX) / (configMain.steps360_Axis_X * configMain.microsteps_Axis_X);
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
			stepsToMove = (configMain.distance_MoveZ / (configMain.distancePerRev_AxisZ) * (configMain.steps360_Axis_Z * configMain.microsteps_Axis_Z));

			// Set speed and acceleration
			nextSpeed = configMain.speedPercent_MoveZ * configMoveZ.maxSpd * .01;
			stepper_Z
				.setMaxSpeed(nextSpeed)
				.setAcceleration(configMoveZ.accel)
				.setTargetRel(stepsToMove * directionAxis);

#ifdef DEBUG
			Serial.print("distancePerRev_AxisZ:");
			Serial.println(configMain.distancePerRev_AxisZ);
			Serial.print("steps360_Axis_Z:");
			Serial.println(configMain.steps360_Axis_Z);
			Serial.print("microsteps_Axis_Z:");
			Serial.println(configMain.microsteps_Axis_Z);
			Serial.print("distance_MoveZ:");
			Serial.println(configMain.distance_MoveZ);

			Serial.print("axisSteps:");
			Serial.println(stepsToMove);

			Serial.print("AxisMaxSpd:");
			Serial.println(configMoveZ.maxSpd);
			Serial.print("speedPercent_MoveZ:");
			Serial.println(configMain.speedPercent_MoveZ);
			Serial.print("nextSpeed:");
			Serial.println(nextSpeed);
			Serial.print("AxisAccel:");
			Serial.println(configMoveZ.accel);
#endif // DEBUG

			digitalWrite(PIN_AXIS_Z_ENABLE, LOW);
			stepController.moveAsync(stepper_Z);
			break;
		}
#ifdef FOUR_AXES
		case ID_AXIS_X:
		{
			//teensyStep_Axis_X.setPosition(0);
			stepsToMove = (configMain.distance_MoveX / (configMain.distancePerRev_AxisX) * (configMain.steps360_Axis_X * configMain.microsteps_Axis_X));
			stepper_X.setPosition(0);


			// Set speed and acceleration
			nextSpeed = configMain.speedPercent_MoveX * configMoveX.maxSpd * .01;
			//teensyStep_Axis_X
			stepper_X
				.setMaxSpeed(nextSpeed)
				.setAcceleration(configMoveX.accel)
				.setTargetRel(stepsToMove * directionAxis);

#ifdef DEBUG
			Serial.print("distancePerRev_AxisX:");
			Serial.println(configMain.distancePerRev_AxisX);
			Serial.print("steps360_Axis_X:");
			Serial.println(configMain.steps360_Axis_X);
			Serial.print("microsteps_Axis_X:");
			Serial.println(configMain.microsteps_Axis_X);
			Serial.print("distance_MoveX:");
			Serial.println(configMain.distance_MoveX);

			Serial.print("axisSteps:");
			Serial.println(stepsToMove);

			Serial.print("AxisMaxSpd:");
			Serial.println(configMoveX.maxSpd);
			Serial.print("speedPercent_MoveX:");
			Serial.println(configMain.speedPercent_MoveX);
			Serial.print("nextSpeed:");
			Serial.println(nextSpeed);
			Serial.print("AxisAccel:");
			Serial.println(configMoveX.accel);
#endif // DEBUG
			digitalWrite(PIN_AXIS_X_ENABLE, LOW);
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
		if (configMain.trackPositions == 1)
		{
			switch (axisId)
			{
				case ID_AXIS_Z:
				{

					endPosition_Axis = stepper_Z.getPosition();
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
					break;
				}
				case ID_AXIS_X:
				{
					endPosition_Axis = stepper_X.getPosition();
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
					break;
				}
			}
		}
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

	switch (axisId)
	{

		case ID_AXIS_Z:
		{
			// Update Index flag on Nextion. 
			Serial3.print("pageMoveZ.bt2.pco=59391");// The first one may be ignored by Nextion
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageMoveZ.va0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageMoveZ.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageMoveZ.bt2.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageMoveZ.bt3.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageMoveZ.t6.txt=");
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

			digitalWrite(PIN_AXIS_Z_ENABLE, HIGH);  // Disable 
			break;
		}
#ifdef FOUR_AXES
		case ID_AXIS_X:
		{
			// Update Index flag on Nextion. 
			Serial3.print("pageMoveX.bt2.pco=59391");// The first one may be ignored by Nextion
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageMoveX.va0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageMoveX.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageMoveX.bt2.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageMoveX.bt3.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
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

			digitalWrite(PIN_AXIS_X_ENABLE, HIGH);  // Disable 
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
void RunOneStepper(int direction) // pageOne
{
	RotateControl rotateController;

	int stepPin = PIN_AXIS_Z_STEP;
	int dirPin = PIN_AXIS_Z_DIR;
	int enablePin = PIN_AXIS_Z_ENABLE;
	int limitPin_Max = configMain.limit_Max_Z;
	int limitPin_Min = configMain.limit_Min_Z;

	int polarity = LOW;
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
			polarity = configMain.polarity_Axis_Z ? (LOW) : (HIGH);
			maxSpd = configOne.maxSpd_Axis_Z;
			accel = configOne.accel_Axis_Z;
			speedPercent = configOne.speedPercent_Axis_Z;
			currentSpeedPercent = (speedPercent) * .01 *direction;
			limitPin_Max = configMain.limit_Max_Z;
			limitPin_Min = configMain.limit_Min_Z;
	
			break;
		}
		case ID_AXIS_X:
		{
			stepPin = PIN_AXIS_X_STEP;
			dirPin = PIN_AXIS_X_DIR;
			enablePin = PIN_AXIS_X_ENABLE;
			polarity = configMain.polarity_Axis_X ? (LOW) : (HIGH);
			maxSpd = configOne.maxSpd_Axis_X;
			accel = configOne.accel_Axis_X;
			speedPercent = configOne.speedPercent_Axis_X;
			currentSpeedPercent = speedPercent * .01 * direction;
			limitPin_Max = configMain.limit_Max_X;
			limitPin_Min = configMain.limit_Min_X;
			break;
		}
		case ID_AXIS_B:
		{
			stepPin = PIN_AXIS_B_STEP;
			dirPin = PIN_AXIS_B_DIR;
			enablePin = PIN_AXIS_B_ENABLE;
			polarity = configMain.polarity_Axis_B ? (LOW) : (HIGH);
			maxSpd = configOne.maxSpd_Axis_B;
			accel = configOne.accel_Axis_B;
			speedPercent = configOne.speedPercent_Axis_B;
			currentSpeedPercent = speedPercent * .01 * direction;
			limitPin_Max = configMain.limit_Max_B;
			limitPin_Min = configMain.limit_Min_B;
			break;
		}
		case ID_SPINDLE:
		{
			stepPin = PIN_SPINDLE_STEP;
			dirPin = PIN_SPINDLE_DIR;
			enablePin = PIN_SPINDLE_ENABLE;
			polarity = configMain.polarity_Spindle ? (LOW) : (HIGH);
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
		.setStepPinPolarity(polarity)
		.setMaxSpeed(maxSpd)
		.setAcceleration(accel)
		.setTargetAbs(targetPosition);

#ifdef DEBUG
	Serial.print("configSp.activeAxis:");
	Serial.println(configOne.activeAxis);
	Serial.print("direction:");
	Serial.println(direction);
	Serial.print("polarity:");
	Serial.println(polarity);
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
	rotateController.rotateAsync(stepper_One);
	rotateController.overrideSpeed(currentSpeedPercent);
	while (rotateController.isRunning())
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
					rotateController.overrideSpeed(0);
					delay(5);
					rotateController.stop();
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
					rotateController.overrideSpeed(currentSpeedPercent); // set new speed
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
			rotateController.overrideSpeed(0);
			rotateController.stop();
			goto endLoop;
		}
		else if (digitalRead(limitPin_Min) == LOW)
		{
			rotateController.overrideSpeed(0);
			rotateController.stop();
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
	Serial3.print("pageOne.bt10.txt=");
	SerialWrite(0x22);
	SerialPrint("Disabled");
	SerialWrite(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
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
	int limitPin_Max = configMain.limit_Max_Z;
	int limitPin_Min = configMain.limit_Min_Z;

	int polarity_Spindle = configMain.polarity_Spindle ? (LOW) : (HIGH);
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
			speedPercentAxis = configMain.speedPercent_Axis_Z;
			speedPercentSpindle = configMain.speedPercentSpindle_Main;

			limitPin_Max = configMain.limit_Max_Z;
			limitPin_Min = configMain.limit_Min_Z;
			stepper_Z.setPosition(0);
			stepper_Z
				.setStepPinPolarity(configMain.polarity_Axis_Z ? (LOW) : (HIGH))
				.setMaxSpeed(configPageMain.maxSpd_Axis_Z * direction_Axis)
				.setAcceleration(configPageMain.accel_Axis_Z)
				.setTargetAbs(targetPosition_Axis);
			break;
		}
		case ID_AXIS_X:
		{
			speedPercentAxis = configMain.speedPercent_Axis_X;
			speedPercentSpindle = configMain.speedPercentSpindle_Main;

			limitPin_Max = configMain.limit_Max_X;
			limitPin_Min = configMain.limit_Min_X;
			stepper_X.setPosition(0);
			stepper_X
				.setStepPinPolarity(configMain.polarity_Axis_X ? (LOW) : (HIGH))
				.setMaxSpeed(configPageMain.maxSpd_Axis_X * direction_Axis)
				.setAcceleration(configPageMain.accel_Axis_X)
				.setTargetAbs(targetPosition_Axis);
			break;
		}
		case ID_AXIS_B:
		{
			speedPercentAxis = configMain.speedPercent_Axis_B;
			speedPercentSpindle = configMain.speedPercentSpindle_Main;

			limitPin_Max = configMain.limit_Max_B;
			limitPin_Min = configMain.limit_Min_B;
			stepper_B.setPosition(0);
			stepper_B
				.setStepPinPolarity(configMain.polarity_Axis_B ? (LOW) : (HIGH))
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
		.setStepPinPolarity(polarity_Spindle)
		.setMaxSpeed(maxSpd_Spindle * direction_Spindle)
		.setAcceleration(accel_Spindle)
		.setTargetAbs(targetPosition_Spindle);

	if (stepper_Axis_Go)
	{
		switch (configPageMain.axisId)
		{
			case ID_AXIS_Z:
			{
				digitalWrite(PIN_AXIS_Z_ENABLE, LOW);  // Enable
				rotateControllerAxis.rotateAsync(stepper_Z);
				rotateControllerAxis.overrideSpeed(0);
				delay(100);
				rotateControllerAxis.overrideSpeed(configMain.speedPercent_Axis_Z * .01);
				break;
			}
			case ID_AXIS_X:
			{
				digitalWrite(PIN_AXIS_X_ENABLE, LOW);  // Enable
				rotateControllerAxis.rotateAsync(stepper_X);
				rotateControllerAxis.overrideSpeed(0);
				delay(100);
				rotateControllerAxis.overrideSpeed(configMain.speedPercent_Axis_X * .01);
				break;
			}
			case ID_AXIS_B:
			{
				digitalWrite(PIN_AXIS_B_ENABLE, LOW);  // Enable
				rotateControllerAxis.rotateAsync(stepper_B);
				rotateControllerAxis.overrideSpeed(0);
				delay(100);
				rotateControllerAxis.overrideSpeed(configMain.speedPercent_Axis_B * .01);
				break;
			}
		}

	}
	if (stepper_Spindle_Go)
	{
		digitalWrite(PIN_SPINDLE_ENABLE, LOW); // Enable 
		delay(5);
		rotateController1.rotateAsync(stepper_Spindle);
		rotateController1.overrideSpeed(0);
		delay(100);
		rotateController1.overrideSpeed(currentSpeedPercentSpindle);
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

				case 172: // ¬ - Sp-B Axis CW
				{
					if (!rotateControllerAxis.isRunning())
					{
						direction_Axis = DIR_CW;
						targetPosition_Axis = 2000000000;
						speedPercentAxis = (float)((configMain.speedPercent_Axis_B) * .01);
						stepper_B
							.setMaxSpeed(configPageMain.maxSpd_Axis_B * direction_Axis)
							.setTargetAbs(targetPosition_Axis);
						digitalWrite(PIN_AXIS_B_ENABLE, LOW);  // Enable
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
						direction_Axis = DIR_CW;
						targetPosition_Axis = 2000000000;
						speedPercentAxis = (float)((configMain.speedPercent_Axis_Z) * .01);
						stepper_Z
							.setMaxSpeed(configPageMain.maxSpd_Axis_Z * direction_Axis)
							.setTargetAbs(targetPosition_Axis);
						digitalWrite(PIN_AXIS_Z_ENABLE, LOW);  // Enable
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
						direction_Axis = DIR_CW;
						targetPosition_Axis = 2000000000;
						speedPercentAxis = (float)(configMain.speedPercent_Axis_X * .01);
						stepper_X
							.setMaxSpeed(configPageMain.maxSpd_Axis_X * direction_Axis)
							.setTargetAbs(targetPosition_Axis);
						digitalWrite(PIN_AXIS_X_ENABLE, LOW);  // Enable
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
						direction_Axis = DIR_CCW;
						targetPosition_Axis = -2000000000;

						speedPercentAxis = (float)((configMain.speedPercent_Axis_B) * .01);
						stepper_B
							.setMaxSpeed(configPageMain.maxSpd_Axis_B * direction_Axis)
							.setTargetAbs(targetPosition_Axis);
						digitalWrite(PIN_AXIS_B_ENABLE, LOW);  // Enable
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
						direction_Axis = DIR_CCW;
						targetPosition_Axis = -2000000000;
						speedPercentAxis = (float)((configMain.speedPercent_Axis_Z) * .01);
						stepper_Z
							.setMaxSpeed(configPageMain.maxSpd_Axis_Z * direction_Axis)
							.setTargetAbs(targetPosition_Axis);
						digitalWrite(PIN_AXIS_Z_ENABLE, LOW);  // Enable
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
						direction_Axis = DIR_CCW;
						targetPosition_Axis = -2000000000;
						speedPercentAxis = (float)(configMain.speedPercent_Axis_X * .01);
						stepper_X
							.setMaxSpeed(configPageMain.maxSpd_Axis_X * direction_Axis)
							.setTargetAbs(targetPosition_Axis);
						digitalWrite(PIN_AXIS_X_ENABLE, LOW);  // Enable
						rotateControllerAxis.rotateAsync(stepper_X);
						rotateControllerAxis.overrideSpeed(0);
						delay(5);
						rotateControllerAxis.overrideSpeed(speedPercentAxis);
					}
					break;
				}
				case 67: // C - Spindle Stop
				{
					Serial.print("Stop Spindle:");
					rotateController1.overrideSpeed(0);
					delay(5);
					rotateController1.stop();
					digitalWrite(PIN_SPINDLE_ENABLE, HIGH); // Disable

					stepper_Spindle_Go = false;
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
							configMain.speedPercent_Axis_Z = newSpeedPercent;
							speedPercentAxis = (float)((newSpeedPercent) * .01);
							break;
						}
						case ID_AXIS_X:
						{
							configMain.speedPercent_Axis_X = newSpeedPercent;
							speedPercentAxis = (float)(newSpeedPercent * .01);
							break;
						}
						case ID_AXIS_B:
						{
							configMain.speedPercent_Axis_B = newSpeedPercent;
							speedPercentAxis = (float)((newSpeedPercent) * .01);
							break;
						}
					}
					EEPROM.put(eePromAddress_Setup, configMain);

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
							digitalWrite(PIN_AXIS_Z_ENABLE, HIGH);  // Disable 
							break;
						}
						case ID_AXIS_X:
						{
							digitalWrite(PIN_AXIS_X_ENABLE, HIGH);  // Disable 
							break;
						}
						case ID_AXIS_B:
						{
							digitalWrite(PIN_AXIS_B_ENABLE, HIGH);  // Disable 
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
						speedPercentSpindle = (float)(configMain.speedPercentSpindle_Main *.01);
						stepper_Spindle
							.setMaxSpeed(maxSpd_Spindle)
							.setTargetAbs(targetPosition_Spindle);
						stepper_Spindle_Go = true;
						digitalWrite(PIN_SPINDLE_ENABLE, LOW); // Enable
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
						speedPercentSpindle = (float)(configMain.speedPercentSpindle_Main *.01);
						stepper_Spindle
							.setMaxSpeed(maxSpd_Spindle * DIR_CCW)
							.setTargetAbs(targetPosition_Spindle);
						stepper_Spindle_Go = true;
						digitalWrite(PIN_SPINDLE_ENABLE, LOW); // Enable
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
					configMain.speedPercentSpindle_Main = (int)GetSerialFloat(serialId);
					speedPercentSpindle = (float)(configMain.speedPercentSpindle_Main * .01);
					EEPROM.put(eePromAddress_Setup, configMain);
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

		delay(5);
	}

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

	//if(stepsToMove == 0)
	//{

	//	return;
	//}

	// Set speed and acceleration
	stepper_Spindle
		.setMaxSpeed(configIndex_Main.maxSpd)// * configIndex_Main.speedPercent * .01)
		.setAcceleration(configIndex_Main.accel)
		.setTargetRel(stepsToMove * directionSpindle);
	digitalWrite(PIN_SPINDLE_ENABLE, LOW);
	stepController.moveAsync(stepper_Spindle);

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
					break;
				}
			}
		}
		delay(10);
	}
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

	digitalWrite(PIN_SPINDLE_ENABLE, HIGH); // Disable 
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
			targetSteps_Spindle = configMain.revolutionsSyncX_Spindle * configMain.gearRatio_Spindle * configMain.microsteps_Spindle * configMain.steps360_Spindle * directionSpindle;
			stepper_Spindle
				.setMaxSpeed(configSyncX.maxSpd_Spindle * configMain.speedPercentSpindle_SyncX * .01)
				.setAcceleration(configSyncX.accel_Spindle)
				.setTargetRel(targetSteps_Spindle);

			stepper_Spindle.setPosition(0);
			break;
		}
		case ID_AXIS_Z:
		{
			targetSteps_Spindle = configMain.revolutionsSyncZ_Spindle * configMain.gearRatio_Spindle * configMain.microsteps_Spindle * configMain.steps360_Spindle * directionSpindle;
			stepper_Spindle
				.setMaxSpeed(configSyncZ.maxSpd_Spindle * configMain.speedPercentSpindle_SyncZ * .01)
				.setAcceleration(configSyncZ.accel_Spindle)
				.setTargetRel(targetSteps_Spindle);
			stepper_Spindle.setPosition(0);
			break;
		}
	}

	int32_t axisStepsZ = (((configMain.distanceSyncZ) / configMain.distancePerRev_AxisZ) * (configMain.microsteps_Axis_Z * configMain.steps360_Axis_Z)) * directionAxis;
	stepper_Z
		.setMaxSpeed(configSyncZ.maxSpd_Axis * configMain.speedPercentAxis_SyncZ * .01)
		.setAcceleration(configSyncZ.accel_Axis)
		.setTargetRel(axisStepsZ);
	stepper_Z.setPosition(0);

	int32_t axisStepsX = (configMain.distanceSyncX / configMain.distancePerRev_AxisX) * configMain.microsteps_Axis_X * configMain.steps360_Axis_X * directionAxis;
	stepper_X
		.setMaxSpeed(configSyncX.maxSpd_Axis * configMain.speedPercentAxis_SyncX * .01)
		.setAcceleration(configSyncX.accel_Axis)
		.setTargetRel(axisStepsX);
	stepper_X.setPosition(0);

#ifdef DEBUG
	Serial.print("idAxis:");
	Serial.println(idAxis);

	Serial.print("targetSteps_Spindle:");
	Serial.println(targetSteps_Spindle);
	Serial.print("microsteps_Spindle:");
	Serial.println(configMain.microsteps_Spindle);
	Serial.print("steps360_Spindle:");
	Serial.println(configMain.steps360_Spindle);
	Serial.print("gearRatio_Spindle:");
	Serial.println(configMain.gearRatio_Spindle);
	Serial.print("revolutionsSyncX_Spindle:");
	Serial.println(configMain.revolutionsSyncX_Spindle);
	switch (idAxis)
	{
		case ID_AXIS_X:
		{

			Serial.print("SpindleMaxSpd:");
			Serial.println(configSyncX.maxSpd_Spindle);
			Serial.print("SpindleSpeedPercent:");
			Serial.println(configMain.speedPercentSpindle_SyncX);
			Serial.print("SpindleAccel:");
			Serial.println(configSyncX.accel_Spindle);
			Serial.print("revolutionsSyncX_Spindle:");
			Serial.println(configMain.revolutionsSyncX_Spindle);
			/////////////////////////////
			Serial.print("distanceSyncX:");
			Serial.println(configMain.distanceSyncX);
			Serial.print("distancePerRev_AxisX:");
			Serial.println(configMain.distancePerRev_AxisX);
			Serial.print("steps360_Axis_X:");
			Serial.println(configMain.steps360_Axis_X);
			Serial.print("microsteps_Axis_X:");
			Serial.println(configMain.microsteps_Axis_X);
			Serial.print("distanceSyncX:");
			Serial.println(configMain.distanceSyncX);
			Serial.print("axisStepsX:");
			Serial.println(axisStepsX);

			Serial.print("AxisMaxSpd:");
			Serial.println(configSyncX.maxSpd_Axis);
			Serial.print("speedPercentAxis_SyncX:");
			Serial.println(configMain.speedPercentAxis_SyncX);
			Serial.print("AxisAccel:");
			Serial.println(configSyncX.accel_Axis);
			break;
		}
		case ID_AXIS_Z:
		{
			Serial.print("SpindleMaxSpd:");
			Serial.println(configSyncZ.maxSpd_Spindle);
			Serial.print("SpindleSpeedPercent:");
			Serial.println(configMain.speedPercentSpindle_SyncZ);
			Serial.print("SpindleAccel:");
			Serial.println(configSyncZ.accel_Spindle);
			Serial.print("revolutionsSyncZ_Spindle:");
			Serial.println(configMain.revolutionsSyncZ_Spindle);
			/////////////////////////////
			Serial.print("distanceSyncZ:");
			Serial.println(configMain.distanceSyncZ);
			Serial.print("distancePerRev_AxisZ:");
			Serial.println(configMain.distancePerRev_AxisZ);
			Serial.print("steps360_Axis_Z:");
			Serial.println(configMain.steps360_Axis_Z);
			Serial.print("microsteps_Axis_Z:");
			Serial.println(configMain.microsteps_Axis_Z);
			Serial.print("distanceSyncZ:");
			Serial.println(configMain.distanceSyncZ);
			Serial.print("axisStepsZ:");
			Serial.println(axisStepsZ);

			Serial.print("AxisMaxSpd:");
			Serial.println(configSyncZ.maxSpd_Axis);
			Serial.print("speedPercentAxis_SyncZ:");
			Serial.println(configMain.speedPercentAxis_SyncZ);
			Serial.print("AxisAccel:");
			Serial.println(configSyncZ.accel_Axis);
			break;
		}
	}
#endif // DEBUG

	switch (idAxis)
	{
		case ID_AXIS_X:
		{

			digitalWrite(PIN_SPINDLE_ENABLE, LOW); // Enable (Uses TeensyStep library)
			digitalWrite(PIN_AXIS_X_ENABLE, LOW);  // Enable (Uses TeensyStep library)
			stepController.moveAsync(stepper_Spindle, stepper_X);
			break;
		}
		case ID_AXIS_Z:
		{

			digitalWrite(PIN_SPINDLE_ENABLE, LOW); // Enable (Uses TeensyStep library)
			digitalWrite(PIN_AXIS_Z_ENABLE, LOW);  // Enable (Uses TeensyStep library)
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
				if (configMain.trackPositions == 1)
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
				}
#endif // Show Position
				break;
			}
			case ID_AXIS_Z:
			{
				endPosition_Axis = stepper_Z.getPosition();
#ifdef SHOW_POSITION 
				if (configMain.trackPositions == 1)
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
				}
#endif // Show Position
				break;
			}
		}

		//Serial.print("Spindle:");
		//Serial.println(endPosition_Spindle);
		//Serial.print("Axis:");
		//Serial.println(endPosition_Axis);
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
			Serial3.print("pageSyncX.bt6.pco=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageSyncX.va0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageSyncX.bt6.val=0");// The first one may be ignored by Nextion, so resend.
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageSyncX.bt2.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageSyncX.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);

			digitalWrite(PIN_AXIS_X_ENABLE, HIGH);  // Disable 
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

			digitalWrite(PIN_AXIS_Z_ENABLE, HIGH);  // Disable 
			break;
		}
	}

	returnSteps_Axis = endPosition_Axis;

	stepper_Spindle.setPosition(0);
	stepper_X.setPosition(0);
	stepper_Z.setPosition(0);

	digitalWrite(PIN_SPINDLE_ENABLE, HIGH); // Disable 
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
		.setMaxSpeed(configReci.maxSpd_Spindle * configReci.speedPercent_Spindle * .01)
		.setAcceleration(configReci.accel_Spindle);
	stepper_Spindle.setPosition(0);

	float spindleDegrees = configReci.degrees_RadialZ/ (configReci.waves_RadialZ * 2);
	int spindleSteps = round((configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) * (spindleDegrees / 360) * wavDir);
	returnSteps_Spindle = spindleSteps* configReci.waves_RadialZ * 2;

	stepper_Z
		.setMaxSpeed(configReci.maxSpd_Axis_Z * configReci.speedPercent_Axis_Z * .01)
		.setAcceleration(configReci.accel_Axis_Z);
	stepper_Z.setPosition(0);
	float axisDistance = configReci.amplitude_RadialZ;
	int axisSteps = round((axisDistance / configMain.distancePerRev_AxisZ) * configMain.microsteps_Axis_Z * configMain.steps360_Axis_Z * wavDir);

#ifdef DEBUG
	Serial.print("SpindleMaxSpd:");
	Serial.println(configReci.maxSpd_Spindle);
	Serial.print("SpindleSpeedPercent:");
	Serial.println(configReci.speedPercent_Spindle);
	Serial.print("SpindleAccel:");
	Serial.println(configReci.accel_Spindle);

	Serial.print("microsteps_Spindle:");
	Serial.println(configMain.microsteps_Spindle);
	Serial.print("steps360_Spindle:");
	Serial.println(configMain.steps360_Spindle);
	Serial.print("gearRatio_Spindle:");
	Serial.println(configMain.gearRatio_Spindle);
	Serial.print("amplitude_AxialZ:");
	Serial.println(configReci.amplitude_AxialZ);

	Serial.print("distancePerRev_AxisZ:");
	Serial.println(configMain.distancePerRev_AxisZ);
	Serial.print("steps360_Axis_Z:");
	Serial.println(configMain.steps360_Axis_Z);
	Serial.print("microsteps_Axis_Z:");
	Serial.println(configMain.microsteps_Axis_Z);
	Serial.print("amplitude_RadialZ:");
	Serial.println(configReci.amplitude_RadialZ);
	Serial.print("waves_RadialZ:");
	Serial.println(configReci.waves_RadialZ);

	Serial.print("spindleSteps:");
	Serial.println(spindleSteps);
	Serial.print("axisSteps:");
	Serial.println(axisSteps);

	Serial.print("AxisMaxSpd:");
	Serial.println(configReci.maxSpd_Axis_Z);
	Serial.print("AxisSpeedPercent:");
	Serial.println(configReci.speedPercent_Axis_Z);
	Serial.print("AxisAccel:");
	Serial.println(configReci.accel_Axis_Z);
#endif // DEBUG

	digitalWrite(PIN_SPINDLE_ENABLE, LOW); // Enable
	digitalWrite(PIN_AXIS_Z_ENABLE, LOW);  // Enable
	for (int i = 0; i < (configReci.waves_RadialZ * 2); i++)
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
			if (configMain.trackPositions == 1)
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
			}
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
	digitalWrite(PIN_SPINDLE_ENABLE, HIGH); // Disable 
	digitalWrite(PIN_AXIS_Z_ENABLE, HIGH);  // Disable 

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
	int stepsPerRevolution_Spindle = (int)(round(configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle));
	StepControl stepController;
	Stepper stepper_Spindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
	Stepper stepper_X(PIN_AXIS_X_STEP, PIN_AXIS_X_DIR);

	// Set speed and acceleration
	stepper_Spindle
		.setMaxSpeed(configReci.maxSpd_Spindle * configReci.speedPercent_Spindle * .01)
		.setAcceleration(configReci.accel_Spindle);
	stepper_Spindle.setPosition(0);

	float spindleDegrees = configReci.degrees_RadialX / (configReci.waves_RadialX * 2);
	int spindleSteps = (int)round((stepsPerRevolution_Spindle) * (spindleDegrees / 360) * wavDir);
	returnSteps_Spindle = spindleSteps * configReci.waves_RadialX * 2;


#ifdef DEBUG
	Serial.print("stepsPerRevolution_Spindle:");
	Serial.println(stepsPerRevolution_Spindle);
	Serial.print("degrees_RadialX:");
    Serial.println(configReci.degrees_RadialX);
	Serial.print("waves_RadialX  x 2:");
	Serial.println(configReci.waves_RadialX * 2);
#endif


	stepper_X
		.setMaxSpeed(configReci.maxSpd_Axis_X * configReci.speedPercent_Axis_X * .01)
		.setAcceleration(configReci.accel_Axis_X);
	stepper_X.setPosition(0);
	float axisDistance = configReci.amplitude_RadialX;
	int axisSteps = round((axisDistance / configMain.distancePerRev_AxisX) * configMain.microsteps_Axis_X * configMain.steps360_Axis_X * wavDir);

#ifdef DEBUG
	//Serial.print("SpindleMaxSpd:");
	//Serial.println(configReci.maxSpd_Spindle);
	//Serial.print("SpindleSpeedPercent:");
	//Serial.println(configReci.speedPercent_Spindle);
	//Serial.print("SpindleAccel:");
	//Serial.println(configReci.accel_Spindle);

	//Serial.print("microsteps_Spindle:");
	//Serial.println(configMain.microsteps_Spindle);
	//Serial.print("steps360_Spindle:");
	//Serial.println(configMain.steps360_Spindle);
	//Serial.print("gearRatio_Spindle:");
	//Serial.println(configMain.gearRatio_Spindle);
	Serial.print("amplitude_AxialX:");
	Serial.println(configReci.amplitude_AxialX);

	Serial.print("distancePerRev_AxisX:");
	Serial.println(configMain.distancePerRev_AxisX);
	Serial.print("steps360_Axis_X:");
	Serial.println(configMain.steps360_Axis_X);
	Serial.print("microsteps_Axis_X:");
	Serial.println(configMain.microsteps_Axis_X);
	Serial.print("amplitude_RadialX:");
	Serial.println(configReci.amplitude_RadialX);
	Serial.print("waves_RadialX:");
	Serial.println(configReci.waves_RadialX);

	Serial.print("spindleSteps:");
	Serial.println(spindleSteps);
	Serial.print("axisSteps:");
	Serial.println(axisSteps);

	Serial.print("maxSpd_Axis_X:");
	Serial.println(configReci.maxSpd_Axis_X);
	Serial.print("speedPercent_Axis_X:");
	Serial.println(configReci.speedPercent_Axis_X);
	Serial.print("accel_Axis_X:");
	Serial.println(configReci.accel_Axis_X);
#endif // DEBUG
	digitalWrite(PIN_SPINDLE_ENABLE, LOW); // Enable
	digitalWrite(PIN_AXIS_X_ENABLE, LOW);  // Enable
	for (int i = 0; i < (configReci.waves_RadialX * 2); i++)
	{
		axisSteps *= -1;

		stepper_Spindle.setTargetRel(spindleSteps);
		stepper_X.setTargetRel(axisSteps);

		stepController.moveAsync(stepper_Spindle, stepper_X);

		while (stepController.isRunning())
		{
#ifdef DEBUG
			endPosition_Axis = stepper_X.getPosition();
			endPosition_Spindle = stepper_Spindle.getPosition();
			Serial.print("Spindle:");
			Serial.println(endPosition_Spindle);
			Serial.print("Axis:");
			Serial.println(endPosition_Axis);
#endif // DEBUG
#ifdef SHOW_POSITION 
			if (configMain.trackPositions == 1)
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
			}
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
	digitalWrite(PIN_SPINDLE_ENABLE, HIGH); // Disable 
	digitalWrite(PIN_AXIS_X_ENABLE, HIGH);  // Disable 
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
		.setMaxSpeed(configReci.maxSpd_Spindle * configReci.speedPercent_Spindle * .01)
		.setAcceleration(configReci.accel_Spindle);
	stepper_Spindle.setPosition(0);

	int spindleSteps = (configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) * (configReci.amplitude_AxialZ / 360) * wavDir;

	long axisSteps = (((configReci.distance_AxialZ / (configMain.distancePerRev_AxisZ)) * (configMain.steps360_Axis_Z * configMain.microsteps_Axis_Z)) / (configReci.waves_AxialZ * 2));
	returnSteps_Axis = (configReci.distance_AxialZ / configMain.distancePerRev_AxisZ) * (configMain.steps360_Axis_Z * configMain.microsteps_Axis_Z);
	//returnSteps_Axis = axisSteps* configReci.waves_AxialZ * 2;

#ifdef DEBUG
	Serial.print("1-wavDir:");
	Serial.println(wavDir);
	Serial.print("1-axisSteps:");
	Serial.println(axisSteps);
#endif // DEBUG
	//if (wavDir == DIR_CW)
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
		.setMaxSpeed(configReci.maxSpd_Axis_Z * configReci.speedPercent_Axis_Z * .01)
		.setAcceleration(configReci.accel_Axis_Z);
	stepper_Z.setPosition(0);

#ifdef DEBUG
	Serial.print("waves_AxialZ:");
	Serial.println(configReci.waves_AxialZ);
	//Serial.println("---------------------------");
	//Serial.print("SpindleMaxSpd:");
	//Serial.println(configReci.maxSpd_Spindle);
	//Serial.print("SpindleSpeedPercent:");
	//Serial.println(configReci.speedPercent_Spindle);
	//Serial.print("SpindleAccel:");
	//Serial.println(configReci.accel_Spindle);
	//Serial.println("========================");
	//Serial.print("microsteps_Spindle:");
	//Serial.println(configMain.microsteps_Spindle);
	//Serial.print("steps360_Spindle:");
	//Serial.println(configMain.steps360_Spindle);
	//Serial.print("gearRatio_Spindle:");
	//Serial.println(configMain.gearRatio_Spindle);
	//Serial.print("amplitude_AxialZ:");
	//Serial.println(configReci.amplitude_AxialZ);
	Serial.print("spindleSteps:");
	Serial.println(spindleSteps);
	Serial.println("========================");
	//Serial.print("AxisMaxSpd:");
	//Serial.println(configReci.maxSpd_Axis_Z);
	//Serial.print("AxisSpeedPercent:");
	//Serial.println(configReci.speedPercent_Axis_Z);
	//Serial.print("AxisAccel:");
	//Serial.println(configReci.accel_Axis_Z);
	//Serial.println("+++++++++++++++++++++++++++");
	//Serial.print("distancePerRev_AxisZ:");
	//Serial.println(configMain.distancePerRev_AxisZ);
	//Serial.print("steps360_Axis_Z:");
	//Serial.println(configMain.steps360_Axis_Z);
	//Serial.print("microsteps_Axis_Z:");
	//Serial.println(configMain.microsteps_Axis_Z);
	//Serial.print("distance_AxialZ:");
	//Serial.println(configReci.distance_AxialZ);
	Serial.print("axisSteps:");
	Serial.println(axisSteps);
	Serial.println("+++++++++++++++++++++++++++");

#endif // DEBUG

	digitalWrite(PIN_SPINDLE_ENABLE, LOW); // Enable (Uses TeensyStep library)
	digitalWrite(PIN_AXIS_Z_ENABLE, LOW);  // Enable (Uses TeensyStep library)
	for (int i = 0; i < (configReci.waves_AxialZ * 2); i++)
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
	digitalWrite(PIN_SPINDLE_ENABLE, HIGH); // Disable 
	digitalWrite(PIN_AXIS_Z_ENABLE, HIGH);  // Disable 
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
		.setMaxSpeed(configReci.maxSpd_Spindle * configReci.speedPercent_Spindle * .01)
		.setAcceleration(configReci.accel_Spindle);
	stepper_Spindle.setPosition(0);

int spindleSteps = (configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) * (configReci.amplitude_AxialX / 360) * wavDir;
	long axisSteps = (((configReci.distance_AxialX / (configMain.distancePerRev_AxisX)) * (configMain.steps360_Axis_X * configMain.microsteps_Axis_X)) / (configReci.waves_AxialX * 2)) * wavDir;
	returnSteps_Axis = (configReci.distance_AxialX / configMain.distancePerRev_AxisX) * (configMain.steps360_Axis_X * configMain.microsteps_Axis_X);
	//returnSteps_Axis = axisSteps * configReci.waves_AxialX * 2;

	stepper_X
		.setMaxSpeed(configReci.maxSpd_Axis_X * configReci.speedPercent_Axis_X * .01)
		.setAcceleration(configReci.accel_Axis_X);
	stepper_X.setPosition(0);

#ifdef DEBUG
	Serial.print("SpindleMaxSpd:");
	Serial.println(configReci.maxSpd_Spindle);
	Serial.print("SpindleSpeedPercent:");
	Serial.println(configReci.speedPercent_Spindle);
	Serial.print("SpindleAccel:");
	Serial.println(configReci.accel_Spindle);

	Serial.print("microsteps_Spindle:");
	Serial.println(configMain.microsteps_Spindle);
	Serial.print("steps360_Spindle:");
	Serial.println(configMain.steps360_Spindle);
	Serial.print("gearRatio_Spindle:");
	Serial.println(configMain.gearRatio_Spindle);
	Serial.print("amplitude_AxialX:");
	Serial.println(configReci.amplitude_AxialX);

	Serial.print("distancePerRev_AxisX:");
	Serial.println(configMain.distancePerRev_AxisX);
	Serial.print("steps360_Axis_X:");
	Serial.println(configMain.steps360_Axis_X);
	Serial.print("microsteps_Axis_X:");
	Serial.println(configMain.microsteps_Axis_X);
	Serial.print("distance_AxialX:");
	Serial.println(configReci.distance_AxialX);
	Serial.print("waves_AxialX:");
	Serial.println(configReci.waves_AxialX);

	Serial.print("spindleSteps:");
	Serial.println(spindleSteps);
	Serial.print("axisSteps:");
	Serial.println(axisSteps);

	Serial.print("AxisMaxSpd:");
	Serial.println(configReci.maxSpd_Axis_X);
	Serial.print("AxisSpeedPercent:");
	Serial.println(configReci.speedPercent_Axis_X);
	Serial.print("AxisAccel:");
	Serial.println(configReci.accel_Axis_X);
#endif // DEBUG
	digitalWrite(PIN_SPINDLE_ENABLE, LOW); // Enable (Uses TeensyStep library)
	digitalWrite(PIN_AXIS_X_ENABLE, LOW);  // Enable (Uses TeensyStep library)
	for (int i = 0; i < (configReci.waves_AxialX * 2); i++)
	{
		spindleSteps *= -1;

		stepper_Spindle.setTargetRel(spindleSteps);
		stepper_X.setTargetRel(axisSteps);

		stepController.moveAsync(stepper_Spindle, stepper_X);
		delay(5);

		while (stepController.isRunning())
		{
#ifdef DEBUG
			endPosition_Axis = stepper_X.getPosition();
			endPosition_Spindle = stepper_Spindle.getPosition();
			Serial.print("Spindle:");
			Serial.println(endPosition_Spindle);
			Serial.print("Axis:");
			Serial.println(endPosition_Axis);
#endif // DEBUG
#ifdef SHOW_POSITION 
			if (configMain.trackPositions == 1)
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
			}
#endif //SHOW_POSITION
			////digitalWriteFast(LED_BUILTIN, !digitalReadFast(LED_BUILTIN)); // toggle LED
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
	digitalWrite(PIN_SPINDLE_ENABLE, HIGH); // Disable 
	digitalWrite(PIN_AXIS_X_ENABLE, HIGH);  // Disable 
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
				digitalWrite(PIN_SPINDLE_ENABLE, HIGH); // Disable (Uses TeensyStep library)
				switch (configGreekKey_Main.axisId)
				{
				case ID_AXIS_Z: // Z Axis
				{
					digitalWrite(PIN_AXIS_Z_ENABLE, HIGH);  // Disable (Uses TeensyStep library)
				}
				case ID_AXIS_X: // X Axis
				{
					digitalWrite(PIN_AXIS_X_ENABLE, HIGH);  // Disable (Uses TeensyStep library)
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
				//stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CW); //1
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CW); //1
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //2
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW); //2
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				//stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW); //3
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //3
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //4
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //4
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				//stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW); //5
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //5
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //6
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW); //6
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				//stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CW); // 7
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CW); //7
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 4, DIR_CW); //8
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 4, DIR_CW); //7
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 8, DIR_CCW); //
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 8, DIR_CCW); //9
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				//stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CCW); //
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CCW); //10
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CW); //
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CW); //11
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				//stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //12
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW); //13
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				//stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); // 14
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //14
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CW); //
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CW); //15
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				//stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CCW); //
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CCW); //16
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 4, DIR_CW); //
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 4, DIR_CW); //17
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 8, DIR_CCW); //
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
				//stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CW); //1
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CW); //1
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CCW); //2
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CCW); //2
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				//stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW); //3
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //3
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //4
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //4
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				//stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //5
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //5
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //6
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //6
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				//stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW); //7
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //7
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CCW); //8
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
				//stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CW); //1  3R
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CW); //1
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CCW); //2 3U
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CCW); //2
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				//stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW); //3 2L
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //3
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //4 1D
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //4
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				//stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //5 1R
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //5
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //6 1D
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //6
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				//stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW); //7 2L
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //7
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 5, DIR_CCW); //8 5U
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 5, DIR_CCW); //8
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				//stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CW); //9 2R
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CW); //9
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //10 1D
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //10
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				//stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW); //11 1L
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //11
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //12 1D
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //12
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				//stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CW); //13 2R
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CW); //13
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CCW); //14 3U
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CCW); //14
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				//stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CCW); //15 3L
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CCW); //15
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //16 1U
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
			//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CW); //0a
			stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CW); //0a
			if (StopGreekKey() || stopAll)
			{
				goto EndLoop;

			}
			//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //0b
			stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW); //0b
			if (StopGreekKey() || stopAll)
			{
				goto EndLoop;

			}
			//stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CW); //1
			stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CW); //1
			if (StopGreekKey() || stopAll)
			{
				goto EndLoop;

			}
			//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //2
			stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW); //2
			if (StopGreekKey() || stopAll)
			{
				goto EndLoop;

			}
			//stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW); //3
			stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //3
			if (StopGreekKey() || stopAll)
			{
				goto EndLoop;

			}
			//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //4
			stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW); //4
			if (StopGreekKey() || stopAll)
			{
				goto EndLoop;

			}
			//stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW); //5
			stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //5
			if (StopGreekKey() || stopAll)
			{
				goto EndLoop;

			}
			//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //6
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

	//for (int i = 0; i < configGreekKey_Main.countPattern360; i++)
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
				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //0a
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //0a
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //0b
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW); //0b
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				//stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CW); //1
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //1
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //2
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW); //2
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				//stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW); //3
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //3
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //4
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //4
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				//stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW); //5
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //5
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CCW); //6
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CCW); //6
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				//stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); // 7
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //7
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //8
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); // 8
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				//stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW); //9
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //9
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //10
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW); //10
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				//stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW); //11
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //11
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //12
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
	int stepsPerRevolution_Spindle = (int)round(configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle);
	spindleShortLegSteps = (stepsPerRevolution_Spindle) / (configGreekKey_Main.countPattern360) / segmentCount;

	// Set axis segment steps
	int stepsPerRevolution_Axis = 0;
	switch (configGreekKey_Main.axisId)
	{
		case ID_AXIS_Z: // Z Axis
		{
			stepsPerRevolution_Axis = (int)round(configGreekKey_Main.segmentLengthPattern / configMain.distancePerRev_AxisZ);
			axisShortLegSteps = ((stepsPerRevolution_Axis) * configMain.steps360_Axis_Z * configMain.microsteps_Axis_Z);
		}
		case ID_AXIS_X: // X Axis
		{
			stepsPerRevolution_Axis = (int)round(configGreekKey_Main.segmentLengthPattern / configMain.distancePerRev_AxisX);
			axisShortLegSteps = ((configGreekKey_Main.segmentLengthPattern / configMain.distancePerRev_AxisX) * configMain.steps360_Axis_X * configMain.microsteps_Axis_X);
		}
	}


#ifdef DEBUG
	Serial.print("spindleShortLegSteps:");
	Serial.println(spindleShortLegSteps);
	Serial.print("configGreekKey_Main.count:");
	Serial.println(configGreekKey_Main.countPattern360);

	Serial.print("configGreekKey_Main.legLength:");
	Serial.println(configGreekKey_Main.segmentLengthPattern);
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
	digitalWrite(PIN_SPINDLE_ENABLE, HIGH); // Disable (Uses TeensyStep library)
	switch (configGreekKey_Main.axisId)
	{
		case ID_AXIS_Z: // Z Axis
		{
			digitalWrite(PIN_AXIS_Z_ENABLE, HIGH);  // Disable (Uses TeensyStep library)
		}
		case ID_AXIS_X: // X Axis
		{
			digitalWrite(PIN_AXIS_X_ENABLE, HIGH);  // Disable (Uses TeensyStep library)
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


	Serial.println("Enter GreekKey_Move_Axis.......................");
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

	Serial.println("Enter GreekKey_Move_Axis_Alt...................>....");
	Serial.println(configGreekKey_Main.axisId);
	// Move the opposite axis. Note: Moves cutter into or out of workpiece.
	if (configGreekKey_Main.axisId == ID_AXIS_X)
	{
		retVal = GreekKey_Move_Z(distance, multiplier, direction, false);
		digitalWrite(PIN_AXIS_Z_ENABLE, HIGH);
	}
	else
	{
		retVal = GreekKey_Move_X(distance, multiplier, direction, false);
		digitalWrite(PIN_AXIS_X_ENABLE, HIGH);  
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
	//float nextSpeed_Spindle = 0;
	bool retVal = false;

	accelStep_Spindle.setCurrentPosition(0);
	accelStep_Spindle.enableOutputs();

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
	//Serial.print("configGreekKey_Z.maxSpd_Spindle:");
	//Serial.println(configGreekKey_Z.maxSpd_Spindle);
	//Serial.print("configGreekKey_Z.speedPercent_Spindle:");
	//Serial.println(configGreekKey_Z.speedPercent_Spindle);
	//Serial.print("configGreekKey_Z.accel_Axis:");
	//Serial.println(configGreekKey_Z.accel_Axis);
//	Serial.print("targetSteps:");
//	Serial.println(targetSteps);
//	targetSteps = round(targetSteps);
//	Serial.print("targetSteps:");
//	Serial.println(targetSteps);
//	Serial.print("shortLegLength:");
//	Serial.println(shortLegLength);
//	Serial.print("multiplier:");
//	Serial.println(multiplier);
//	Serial.print("direction:");
//	Serial.println(direction);
#endif // DEBUG

	//int i = 0;
	while ((accelStep_Spindle.distanceToGo() != 0))
	{
		//if(i > 999)
		//{ 
		//	if (i % 1000 == 0)
		//	{
		//		Serial.println(accelStep_Spindle.currentPosition());
		//	}
		//}
		//i++;
//#ifdef DEBUG
//		Serial.print("distanceToGo: ");
//		Serial.println(accelStep_Spindle.distanceToGo());
//
//#endif // DEBUG

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
					accelStep_Spindle.disableOutputs();
					//digitalWrite(PIN_SPINDLE_ENABLE, HIGH); // Disable (Uses TeensyStep library)
					digitalWrite(PIN_AXIS_Z_ENABLE, HIGH);  // Disable (Uses TeensyStep library)
					digitalWrite(PIN_AXIS_X_ENABLE, HIGH);  // Disable (Uses TeensyStep library)
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
	//Serial.print("configGreekKey_Z.maxSpd_Axis:");
	//Serial.println(configGreekKey_Z.maxSpd_Axis);
	//Serial.print("configGreekKey_Z.speedPercent_Axis:");
	//Serial.println(configGreekKey_Z.speedPercent_Axis);
	//Serial.print("configGreekKey_Z.accel_Axis:");
	//Serial.println(configGreekKey_Z.accel_Axis);

#endif // DEBUG
	accelStep_Axis_Z.setCurrentPosition(0);
	accelStep_Axis_Z.enableOutputs();
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
					accelStep_Axis_Z.disableOutputs();
					digitalWrite(PIN_SPINDLE_ENABLE, HIGH); // Disable (Uses TeensyStep library)
					//digitalWrite(PIN_AXIS_Z_ENABLE, HIGH);  // Disable (Uses TeensyStep library)
					digitalWrite(PIN_AXIS_X_ENABLE, HIGH);  // Disable (Uses TeensyStep library)
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
	//Serial.print("configGreekKey_X.maxSpd_Axis:");
	//Serial.println(configGreekKey_X.maxSpd_Axis);
	//Serial.print("configGreekKey_X.speedPercent_Axis:");
	//Serial.println(configGreekKey_X.speedPercent_Axis);
	//Serial.print("configGreekKey_X.accel_Axis:");
	//Serial.println(configGreekKey_X.accel_Axis);

#endif // DEBUG

	accelStep_Axis_X.setCurrentPosition(0);
	accelStep_Axis_X.enableOutputs();
	nextSpeed_X = configGreekKey_X.maxSpd_Axis * configGreekKey_X.speedPercent_Axis * .01;
	accelStep_Axis_X.setAcceleration(configGreekKey_X.accel_Axis);

	accelStep_Axis_X.setMaxSpeed(nextSpeed_X);
	int32_t targetSteps = shortLegLength * multiplier * direction;
	accelStep_Axis_X.moveTo(targetSteps);

	//#ifdef DEBUG
	//	Serial.print("targetSteps:");
	//	Serial.println(targetSteps);
	//	targetSteps = round(targetSteps);
	//	Serial.print("targetSteps:");
	//	Serial.println(targetSteps);
	//	Serial.print("shortLegLength:");
	//	Serial.println(shortLegLength);
	//	Serial.print("multiplier:");
	//	Serial.println(multiplier);
	//	Serial.print("direction:");
	//	Serial.println(direction);
	//#endif // DEBUG


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
					accelStep_Axis_X.disableOutputs();
					digitalWrite(PIN_SPINDLE_ENABLE, HIGH); // Disable (Uses TeensyStep library)
					digitalWrite(PIN_AXIS_Z_ENABLE, HIGH);  // Disable (Uses TeensyStep library)
					//digitalWrite(PIN_AXIS_X_ENABLE, HIGH);  // Disable (Uses TeensyStep library)
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

	int polarity_Axis = LOW;
	int maxSpd_Axis = 0;
	int accel_Axis = 0;
	float speedPercent_Axis = 0;
	float currentSpeedPercent_Axis = 0;

	int polarity_Spindle = configMain.polarity_Spindle ? (LOW) : (HIGH);
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
			digitalWrite(PIN_AXIS_Z_ENABLE, LOW);
			polarity_Axis = configMain.polarity_Axis_Z ? (LOW) : (HIGH);

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

			digitalWrite(PIN_AXIS_X_ENABLE, LOW);
			polarity_Axis = configMain.polarity_Axis_X ? (LOW) : (HIGH);
			maxSpd_Axis = configGreekKey_X.maxSpd_Axis;
			accel_Axis = configGreekKey_X.accel_Axis;
			speedPercent_Axis = configGreekKey_X.speedPercent_Axis;

			maxSpd_Spindle = configGreekKey_Z.maxSpd_Spindle;
			accel_Spindle = configGreekKey_Z.accel_Spindle;
			speedPercent_Spindle = configGreekKey_Z.speedPercent_Spindle;

			break;
		}
	}

	digitalWrite(PIN_SPINDLE_ENABLE, LOW);
	currentSpeedPercent_Axis = speedPercent_Axis * .01;
	currentSpeedPercent_Spindle = speedPercent_Spindle * .01;
	
	StepControl stepController;
	Stepper stepper_Axis(stepPin_Axis, dirPin_Axis);
	stepper_Axis
		.setStepPinPolarity(polarity_Axis)
		.setMaxSpeed(maxSpd_Axis * currentSpeedPercent_Axis)
		.setAcceleration(accel_Axis);
	stepper_Axis.setPosition(0);
	int32_t targetSteps = round(shortLegLength_Axis * multiplier_Axis);// *direction_Axis);
	stepper_Axis.setTargetRel(targetSteps);

	stepperSpindle
		.setStepPinPolarity(polarity_Spindle)
		.setMaxSpeed(maxSpd_Spindle * currentSpeedPercent_Spindle)
		.setAcceleration(accel_Spindle);
	stepperSpindle.setPosition(0);
	int32_t targetSteps_Spindle = shortLegLength_Spindle * multiplier_Spindle;
	stepperSpindle.setTargetRel(targetSteps_Spindle);

#ifdef DEBUG
	Serial.println("-=-=-=-=-=-=-=-=-=-=-=-===-=-=-==-=-=-=-");
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
			digitalWrite(PIN_SPINDLE_ENABLE, HIGH); // Disable (Uses TeensyStep library)
			digitalWrite(PIN_AXIS_Z_ENABLE, HIGH);  // Disable (Uses TeensyStep library)
			digitalWrite(PIN_AXIS_X_ENABLE, HIGH);  // Disable (Uses TeensyStep library)
			retVal = true;
			break;
		}
		delay(5);
	}

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
//#ifdef DEBUG
//	Serial.print("shortLegLength:");
//	Serial.println(shortLegLength);
//	Serial.print("multiplier:");
//	Serial.println(multiplier);
//	Serial.print("direction:");
//	Serial.println(direction);
//	Serial.print("Target:");
//	Serial.println(shortLegLength * multiplier * direction);
//	Serial.print("configGreekKey_Z.maxSpd_Spindle:");
//	Serial.println(configGreekKey_Z.maxSpd_Spindle);
//	Serial.print("configGreekKey_Z.speedPercent_Spindle:");
//	Serial.println(configGreekKey_Z.speedPercent_Spindle);
//	Serial.print("configGreekKey_Z.accel_Axis:");
//	Serial.println(configGreekKey_Z.accel_Axis);
//
//#endif // DEBUG
	digitalWrite(PIN_SPINDLE_ENABLE, LOW); // Enable (Uses TeensyStep library)
	StepControl stepControllerSpindle;
	//RotateControl rotateControllerSpindle;
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

	//stepper_Spindle.setPosition(0);
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
					digitalWrite(PIN_SPINDLE_ENABLE, HIGH); // Disable (Uses TeensyStep library)
					digitalWrite(PIN_AXIS_Z_ENABLE, HIGH);  // Disable (Uses TeensyStep library)
					digitalWrite(PIN_AXIS_X_ENABLE, HIGH);  // Disable (Uses TeensyStep library)
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
	//Serial.print("configGreekKey_X.maxSpd_Axis:");
	//Serial.println(configGreekKey_X.maxSpd_Axis);
	//Serial.print("configGreekKey_X.speedPercent_Axis:");
	//Serial.println(configGreekKey_X.speedPercent_Axis);
	//Serial.print("configGreekKey_X.accel_Axis:");
	//Serial.println(configGreekKey_X.accel_Axis);

#endif // DEBUG

	digitalWrite(PIN_AXIS_X_ENABLE, LOW);  // Enable (Uses TeensyStep library)
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
			digitalWrite(PIN_SPINDLE_ENABLE, HIGH); // Disable (Uses TeensyStep library)
			digitalWrite(PIN_AXIS_Z_ENABLE, HIGH);  // Disable (Uses TeensyStep library)
			digitalWrite(PIN_AXIS_X_ENABLE, HIGH);  // Disable (Uses TeensyStep library)
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
	//Serial.print("configGreekKey_Z.maxSpd_Axis:");
	//Serial.println(configGreekKey_Z.maxSpd_Axis);
	//Serial.print("configGreekKey_Z.speedPercent_Axis:");
	//Serial.println(configGreekKey_Z.speedPercent_Axis);
	Serial.print("configGreekKey_Z.accel_Axis:");
	Serial.println(configGreekKey_Z.accel_Axis);

#endif // DEBUG

	digitalWrite(PIN_AXIS_Z_ENABLE, LOW);  // Enable (Uses TeensyStep library)
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
			digitalWrite(PIN_SPINDLE_ENABLE, HIGH); // Disable (Uses TeensyStep library)
			digitalWrite(PIN_AXIS_Z_ENABLE, HIGH);  // Disable (Uses TeensyStep library)
			digitalWrite(PIN_AXIS_X_ENABLE, HIGH);  // Disable (Uses TeensyStep library)
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
void GreekKeyFromFile()
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
		
		int stepsPerRevolution_Spindle = (int)(floor(configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle));
		float patternDivisor = ((float)configGreekKey_Main.countPatternFile360 * (float)spindleSegmentCount);
		spindleShortLegSteps = (int)((floor(stepsPerRevolution_Spindle)) / (patternDivisor));
#ifdef DEBUG
		Serial.print("lineCounter:");
		Serial.println(lineCounter);
		Serial.print("patternDivisor:");
		Serial.println(patternDivisor);
#endif // DEBUG


//#ifdef DEBUG
//		Serial.print("stepsPerRevolution_Spindle:");
//		Serial.println(stepsPerRevolution_Spindle);
//		Serial.print("patternDivisor:");
//		Serial.println(patternDivisor);
//		Serial.print("configMain.microsteps_Spindle:");
//		Serial.println(configMain.microsteps_Spindle);
//		Serial.print("configMain.steps360_Spindle:");
//		Serial.println(configMain.steps360_Spindle);
//		Serial.print("configMain.gearRatio_Spindle:");
//		Serial.println(configMain.gearRatio_Spindle);
//		Serial.print("configGreekKey_Main.countPatternFile360 :");
//		Serial.println(configGreekKey_Main.countPatternFile360);
//		Serial.print("spindleSegmentCount:");
//		Serial.println(spindleSegmentCount);
//		Serial.print("spindleShortLegSteps:");
//		Serial.println(spindleShortLegSteps);
//#endif // DEBUG

	}

	// Calculate axis steps
	shortSegmentStepsAxisZ = round((configGreekKey_Main.segmentLengthForFile / configMain.distancePerRev_AxisZ) * (configMain.steps360_Axis_Z * configMain.microsteps_Axis_Z));
	shortSegmentStepsAxisX = round((configGreekKey_Main.segmentLengthForFile / configMain.distancePerRev_AxisX) * (configMain.steps360_Axis_X * configMain.microsteps_Axis_X));
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


	#endif // DEBUG

	#ifdef DEBUG

			Serial.print("Comment0:");
			Serial.println(comment);
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
								//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, floor(segmentMultiplier), DIR_CW);
								stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, segmentMultiplier, DIR_CW);
							}
							else  //3: Actual
							{
								spindleSteps = round((configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) * (segmentMultiplier / 360));
								stopAll = GreekKey_Move_Spindle(spindleSteps, 1, DIR_CW);
							}
							break;
						}
						case AXIAL: // Axis Right CW
						{
							if (configGreekKey_Main.segmentOrActual == 2) // 2: Segment  
							{
								//stopAll = GreekKey_Move_Axis(shortSegmentStepsAxis, floor(segmentMultiplier), DIR_CW);
								stopAll = GreekKey_Move_Axis(shortSegmentStepsAxis, segmentMultiplier, DIR_CW);
							}
							else  //3: Actual
							{

								switch (configGreekKey_Main.axisId)
								{
									case ID_AXIS_Z: // Z Axis
									{
										axisSteps = round((segmentMultiplier / configMain.distancePerRev_AxisZ) * (configMain.steps360_Axis_Z * configMain.microsteps_Axis_Z));
										break;
									}
									case ID_AXIS_X: // X Axis
									{
										axisSteps = round((segmentMultiplier / configMain.distancePerRev_AxisX) * (configMain.steps360_Axis_X * configMain.microsteps_Axis_X));
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
									case ID_AXIS_Z: // Z Axis is primary, so use X to move cutter into workpiece
									{
										stopAll = GreekKey_Move_Angular_TeensyStep(shortSegmentStepsAxisZ, hVal, spindleShortLegSteps, vVal);
										break;
									}
									case ID_AXIS_X: // X Axis is primary, so use Z to move cutter into workpiece
									{
										stopAll = GreekKey_Move_Angular_TeensyStep(shortSegmentStepsAxisX, hVal, spindleShortLegSteps, vVal);
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
										axisSteps = round((hVal / configMain.distancePerRev_AxisZ) * (configMain.steps360_Axis_Z * configMain.microsteps_Axis_Z));
										break;
									}
									case ID_AXIS_X: // X Axis
									{
										axisSteps = round((hVal / configMain.distancePerRev_AxisX) * (configMain.steps360_Axis_X * configMain.microsteps_Axis_X));
										break;
									}

								}

								spindleSteps = round((configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) * (vVal / 360));
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
									stopAll = GreekKey_Move_Angular_TeensyStep(shortSegmentStepsAxisZ, vVal, spindleShortLegSteps, hVal);
									break;
								}
								case ID_AXIS_X: // 
								{
									stopAll = GreekKey_Move_Angular_TeensyStep(shortSegmentStepsAxisX, vVal, spindleShortLegSteps, hVal);
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
										axisSteps = round((vVal / configMain.distancePerRev_AxisZ) * (configMain.steps360_Axis_Z * configMain.microsteps_Axis_Z));
										break;
									}
									case ID_AXIS_X: // X Axis
									{
										axisSteps = round((vVal / configMain.distancePerRev_AxisX) * (configMain.steps360_Axis_X * configMain.microsteps_Axis_X));
										break;
									}

								}

								spindleSteps = round((configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) * (hVal / 360));
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
							case ID_AXIS_Z: // Z Axis is primary, so use X to move cutter into workpiece
							{
								//stopAll = GreekKey_Move_Axis_Alt(shortSegmentStepsAxisX, floor(segmentMultiplier), DIR_CCW);
								stopAll = GreekKey_Move_Axis_Alt(shortSegmentStepsAxisX, segmentMultiplier, DIR_CCW);
								break;
							}
							case ID_AXIS_X: // X Axis is primary, so use Z to move cutter into workpiece
							{
								//stopAll = GreekKey_Move_Axis_Alt(shortSegmentStepsAxisZ, floor(segmentMultiplier), DIR_CCW);
								stopAll = GreekKey_Move_Axis_Alt(shortSegmentStepsAxisZ, segmentMultiplier, DIR_CCW);
								break;
							}
						}
					}
					else // Actual
					{

						switch (configGreekKey_Main.axisId)
						{
							case ID_AXIS_Z: // Z Axis is primary, so use X to move cutter into workpiece
							{
								axisSteps = round((segmentMultiplier / configMain.distancePerRev_AxisX) * (configMain.steps360_Axis_X * configMain.microsteps_Axis_X));
								
								break;
							}
							case ID_AXIS_X: // X Axis is primary, so use Z to move cutter into workpiece
							{
								axisSteps = round((segmentMultiplier / configMain.distancePerRev_AxisZ) * (configMain.steps360_Axis_Z * configMain.microsteps_Axis_Z));
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
								//stopAll = GreekKey_Move_Axis(shortSegmentStepsAxis, floor(segmentMultiplier), DIR_CCW);
								stopAll = GreekKey_Move_Axis(shortSegmentStepsAxis, segmentMultiplier, DIR_CCW);
							}
							else //3: Actual
							{
								switch (configGreekKey_Main.axisId)
								{
									case ID_AXIS_Z: // Z Axis
									{
										axisSteps = round((segmentMultiplier / configMain.distancePerRev_AxisZ) * (configMain.steps360_Axis_Z * configMain.microsteps_Axis_Z));
										break;
									}
									case ID_AXIS_X: // X Axis
									{
										axisSteps = round((segmentMultiplier / configMain.distancePerRev_AxisX) * (configMain.steps360_Axis_X * configMain.microsteps_Axis_X));
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
								//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, floor(segmentMultiplier), DIR_CCW);
								stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, segmentMultiplier, DIR_CCW);
							}
							else  //3: Actual
							{
								spindleSteps = round((configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) * (segmentMultiplier / 360));
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
								//stopAll = GreekKey_Move_Axis_Alt(shortSegmentStepsAxisX, floor(segmentMultiplier), DIR_CW);
								stopAll = GreekKey_Move_Axis_Alt(shortSegmentStepsAxisX, segmentMultiplier, DIR_CW);
								break;
							}
							case ID_AXIS_X: // X Axis is primary, so use Z to move cutter away from workpiece
							{
								//stopAll = GreekKey_Move_Axis_Alt(shortSegmentStepsAxisZ, floor(segmentMultiplier), DIR_CW);
								stopAll = GreekKey_Move_Axis_Alt(shortSegmentStepsAxisZ, segmentMultiplier, DIR_CW);
								break;
							}
						}
					}
					else
					{

						switch (configGreekKey_Main.axisId)
						{
							case ID_AXIS_Z: // Z Axis is primary, so use X to move cutter away from workpiece
							{
								axisSteps = round((segmentMultiplier / configMain.distancePerRev_AxisX) * (configMain.steps360_Axis_X * configMain.microsteps_Axis_X));

								break;
							}
							case ID_AXIS_X: // X Axis is primary, so use Z to move cutter away from workpiece
							{
								axisSteps = round((segmentMultiplier / configMain.distancePerRev_AxisZ) * (configMain.steps360_Axis_Z * configMain.microsteps_Axis_Z));
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
								//stopAll = GreekKey_Move_Axis(shortSegmentStepsAxis, floor(segmentMultiplier), DIR_CW);
								stopAll = GreekKey_Move_Axis(shortSegmentStepsAxis, segmentMultiplier, DIR_CW);
							}
							else  //3: Actual
							{
								switch (configGreekKey_Main.axisId)
								{
								case ID_AXIS_Z: // Z Axis
								{
									axisSteps = round((segmentMultiplier / configMain.distancePerRev_AxisZ) * (configMain.steps360_Axis_Z * configMain.microsteps_Axis_Z));
									break;
								}
								case ID_AXIS_X: // X Axis
								{
									axisSteps = round((segmentMultiplier / configMain.distancePerRev_AxisX) * (configMain.steps360_Axis_X * configMain.microsteps_Axis_X));
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
								//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, floor(segmentMultiplier), DIR_CW);
								stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, segmentMultiplier, DIR_CW);
							}
							else  //3: Actual
							{
								spindleSteps = round((configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) * (segmentMultiplier / 360));
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
								//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, floor(segmentMultiplier), DIR_CCW);
								stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, segmentMultiplier, DIR_CCW);
							}
							else  //3: Actual
							{
								spindleSteps = round((configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) * (segmentMultiplier / 360));
								stopAll = GreekKey_Move_Spindle(spindleSteps, 1, DIR_CCW);
							}

							break;
						}
						case AXIAL: // Axis Left CCW
						{
							if (configGreekKey_Main.segmentOrActual == 2) // 2: Segment  
							{
								//stopAll = GreekKey_Move_Axis(shortSegmentStepsAxis, floor(segmentMultiplier), DIR_CCW);
								stopAll = GreekKey_Move_Axis(shortSegmentStepsAxis, segmentMultiplier, DIR_CCW);
							}
							else //3: Actual
							{
								switch (configGreekKey_Main.axisId)
								{
								case ID_AXIS_Z: // Z Axis
								{
									axisSteps = round((segmentMultiplier / configMain.distancePerRev_AxisZ) * (configMain.steps360_Axis_Z * configMain.microsteps_Axis_Z));
									break;
								}
								case ID_AXIS_X: // X Axis
								{
									axisSteps = round((segmentMultiplier / configMain.distancePerRev_AxisX) * (configMain.steps360_Axis_X * configMain.microsteps_Axis_X));
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
			//endPosition_Axis = stepperAxis_Z.getPosition();
			distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);
			break;
		}
		case ID_AXIS_X: // X Axis
		{
			//endPosition_Axis = stepperAxis_X.getPosition();
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

	digitalWrite(PIN_SPINDLE_ENABLE, HIGH); // Disable 
	digitalWrite(PIN_AXIS_Z_ENABLE, HIGH);  // Disable 
	digitalWrite(PIN_AXIS_X_ENABLE, HIGH);  // Disable 
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

			if (pageCallerId == 8) // pageIndex1
			{
				EEPROM.write(eePromAddress_Filename_Index + i, 0);
			}
			else // pageGrkFile
			{
				EEPROM.write(eePromAddress_Filename_Grk + i, 0);
			}
			break;
		}

		if (pageCallerId == 8) // pageIndex1
		{
			EEPROM.write(eePromAddress_Filename_Index + i, input);
		}
		else // pageGrkFile
		{
			EEPROM.write(eePromAddress_Filename_Grk + i, input);
		}

	}

//#ifdef DEBUG
//
//	Serial.print("Filename Length: ");
//	Serial.println(input);
//	Serial.print("  <--> ");
//	Serial.print(i);
//	Serial.println(pageCallerId);
//#endif // DEBUG

	if (pageCallerId == 8) // pageIndex1
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
		

		Serial3.print("pageIndex1.tFileName.txt=");
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
//#ifdef DEBUG
//		Serial.print("newSizeString(Get):");
//		Serial.println(newSizeString);
//#endif // DEBUG
//
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

//#ifdef DEBUG
//	Serial.print("filename_Length:  ");
//
//	Serial.println(filename_Length);
//#endif // DEBUG

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
//#ifdef DEBUG
//			//Serial.print("lineNumber at break:");
//			Serial.print(lineNumber);
//			Serial.println("  <------------------");
//#endif // DEBUG
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
	//delay(1000);
	Serial.print("moveType:");
	Serial.println(moveType);
		//Serial.print("nStr:");
		//Serial.println(nStr);
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
		char charBuf[10];
		nStr.toCharArray(charBuf, 10);
		val = strtok(charBuf, "V");
		hVal = atof(val);
		val = strtok(NULL, "\n");
		vVal = atof(val);

		Serial.print("hVal: ");
		Serial.println(hVal);
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
//delay(1000);
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
		.setMaxSpeed(configMain.maxSpd_Return_Spindle)
		.setAcceleration(configMain.accel_Return_Spindle)
		.setTargetRel(target_Spindle);

#ifdef DEBUG
	Serial.println("Begin ReturnToStartPosition()");
	Serial.print("target_Spindle: ");
	Serial.println(target_Spindle);
	Serial.print("target_Axis: ");
	Serial.println(target_Axis);
#endif // DEBUG
	digitalWrite(PIN_SPINDLE_ENABLE, LOW); // Enable 
	delay(20);
	switch (axisId)
	{
		case ID_AXIS_Z:
		{
			stepper_Z.setPosition(0);
			stepper_Z
				.setMaxSpeed(configMain.maxSpd_Return_Axis)
				.setAcceleration(configMain.accel_Return_Axis)
				.setTargetAbs(target_Axis);
			digitalWrite(PIN_AXIS_Z_ENABLE, LOW);  // Enable 
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
				.setMaxSpeed(configMain.maxSpd_Return_Axis)
				.setAcceleration(configMain.accel_Return_Axis)
				.setTargetAbs(target_Axis);
			digitalWrite(PIN_AXIS_X_ENABLE, LOW);  // Enable 
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
				.setMaxSpeed(configMain.maxSpd_Return_Axis)
				.setAcceleration(configMain.accel_Return_Axis)
				.setTargetAbs(target_Axis);
			digitalWrite(PIN_AXIS_B_ENABLE, LOW);  // Enable 
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
				digitalWrite(PIN_SPINDLE_ENABLE, HIGH); // Disable 
				digitalWrite(PIN_AXIS_Z_ENABLE, HIGH);  // Disable 
				digitalWrite(PIN_AXIS_X_ENABLE, HIGH);  // Disable 
				digitalWrite(PIN_AXIS_B_ENABLE, HIGH);  // Disable 
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
		case PAGE_SYNCX:
		{
			Serial3.print("pageSyncX.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageSyncX.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageSyncX.va0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageSyncX.bt0.val=0");
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

		case PAGE_INDEX1:
		{
			Serial3.print("pageIndex1.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageIndex1.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageIndex1.va0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageIndex1.bt0.val=0");
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
		case PAGE_MOVEZ:
		{
			Serial3.print("pageMoveZ.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageMoveZ.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageMoveZ.va0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageMoveZ.bt0.val=0");
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
		case PAGE_MOVEX:
		{
			Serial3.print("pageMoveX.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageMoveX.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageMoveX.va0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageMoveX.bt0.val=0");
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

	digitalWrite(PIN_SPINDLE_ENABLE, HIGH); // Disable 
	digitalWrite(PIN_AXIS_Z_ENABLE, HIGH);  // Disable 
	digitalWrite(PIN_AXIS_X_ENABLE, HIGH);  // Disable 
	digitalWrite(PIN_AXIS_B_ENABLE, HIGH);  // Disable 
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
	spindleStepsPerRev = configMain.gearRatio_Spindle * configMain.microsteps_Spindle * configMain.steps360_Spindle;
	spindleStepsAmplitude = (configRose.amplitude_Axial_Z / 360) * spindleStepsPerRev;

	slideStepsAmplitude = ((configRose.amplitude_Axial_X / (configMain.distancePerRev_AxisZ)) * (configMain.steps360_Axis_Z * configMain.microsteps_Axis_Z));

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
	digitalWrite(PIN_SPINDLE_ENABLE, LOW);
	digitalWrite(PIN_AXIS_Z_ENABLE, LOW);

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
		if (configMain.trackPositions == 1)
		{
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
			delay(5);
			Serial3.print("pageBE.t1.txt=");
			Serial3.write(0x22);
			Serial3.print(degrees_Spindle);
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
		}
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

	digitalWrite(PIN_SPINDLE_ENABLE, HIGH); // Disable 
	digitalWrite(PIN_AXIS_Z_ENABLE, HIGH);  // Disable 
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


	//float slideTargetZ = slideFunc_Axis_Z(spindleAngleZ);

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
	spindleStepsPerRev = configMain.gearRatio_Spindle * configMain.microsteps_Spindle * configMain.steps360_Spindle;
	slideStepsAmplitude = ((configRose.amplitude_Radial_Z / (configMain.distancePerRev_AxisZ)) * configMain.steps360_Axis_Z * configMain.microsteps_Axis_Z)/2;  // Amplitude is normally measured from the middle to the top

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
	Serial.println("^^^^^^^^^^^^^^^^^");
#endif // DEBUG

#ifdef DEBUG

	Serial.println("---------------------------");
	Serial.print("SpindleMaxSpd:");
	Serial.println(configRose.maxSpd_Spindle);
	Serial.print("SpindleSpeedPercent:");
	Serial.println(configRose.speedPercent_Spindle);

	Serial.println("========================");
	Serial.print("microsteps_Spindle:");
	Serial.println(configMain.microsteps_Spindle);
	Serial.print("steps360_Spindle:");
	Serial.println(configMain.steps360_Spindle);
	Serial.print("gearRatio_Spindle:");
	Serial.println(configMain.gearRatio_Spindle);
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
	digitalWrite(PIN_SPINDLE_ENABLE, LOW);
	digitalWrite(PIN_AXIS_Z_ENABLE, LOW);

	rotateController1.rotateAsync(stepperAxis_Z);
	//controllerRose_Axis.overrideSpeed(0); // start with stopped slide
	controllerRoseSpindle.rotateAsync(stepperSpindle); // let the spindle run with constant speed

	tickZ();

	// use a timer to periodically calculate new targets for the slide
	tickTimerZ.priority(priorityLevel); // lowest priority, potentially long caclulations need to be interruptable by TeensyStep
	tickTimerZ.begin(tickZ, recalcPeriod);

	while (runPattern)
	{
		//digitalWriteFast(LED_BUILTIN, !digitalReadFast(LED_BUILTIN));
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

	digitalWrite(PIN_SPINDLE_ENABLE, HIGH); // Disable 
	digitalWrite(PIN_AXIS_Z_ENABLE, HIGH);  // Disable 
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
	spindleStepsPerRev = configMain.gearRatio_Spindle * configMain.microsteps_Spindle * configMain.steps360_Spindle;
	slideStepsAmplitude = ((configRose.amplitude_Radial_X / (configMain.distancePerRev_AxisX)) * configMain.steps360_Axis_X * configMain.microsteps_Axis_X)/2;
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
	digitalWrite(PIN_SPINDLE_ENABLE, LOW);
	digitalWrite(PIN_AXIS_X_ENABLE, LOW);

	rotateController1.rotateAsync(stepperAxis_X);
	//controllerRose_Axis.overrideSpeed(0); // start with stopped slide
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
					digitalWrite(PIN_AXIS_X_ENABLE, HIGH);  // Disable 
					break;
				}
			}
		}
//#ifdef DEBUG
//		endPosition_Spindle = stepperSpindle.getPosition();
//		endPosition_Axis = stepperAxis_X.getPosition();
//		Serial.print("Spindle:");
//		Serial.println(endPosition_Spindle);
//		Serial.print("Axis:");
//		Serial.println(endPosition_Axis);
//#endif // DEBUG
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

	digitalWrite(PIN_SPINDLE_ENABLE, HIGH); // Disable 
	digitalWrite(PIN_AXIS_X_ENABLE, HIGH);  // Disable 
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
					Serial3.print(eePageMain.accel_Axis_X);
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
			configSteppers eePageSyncZ;
			EEPROM.get(eePromAddress_SyncZ, eePageSyncZ);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("SyncZ-MaxSpeed:");
			Serial3.print(eePageSyncZ.maxSpd_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("SyncZ-Accel:");
			Serial3.print(eePageSyncZ.accel_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("Z Axis-MaxSpeed:");
			Serial3.print(eePageSyncZ.maxSpd_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("Z Axis-Accel:");
			Serial3.print(eePageSyncZ.accel_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			break;
		}
		case PAGE_SYNCX:
		{
			configSteppers eePageSyncX;
			EEPROM.get(eePromAddress_SyncX, eePageSyncX);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("SyncX-MaxSpeed:");
			Serial3.print(eePageSyncX.maxSpd_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("SyncX-Accel:");
			Serial3.print(eePageSyncX.accel_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("X Axis-MaxSpeed:");
			Serial3.print(eePageSyncX.maxSpd_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("X Axis-Accel:");
			Serial3.print(eePageSyncX.accel_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
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
			if (configReci.axisId == ID_AXIS_Z) // Z axis
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
			configPageMainOne eePageSp;
			EEPROM.get(eePromAddress_pageOne, eePageSp);
			switch (eePageSp.activeAxis)
			{
				case ID_SPINDLE:
				{
					Serial3.print("pageConfig.t19.txt=");
					Serial3.write(0x22);
					Serial3.print("Spindle-MaxSpeed:");
					Serial3.print(eePageSp.maxSpd_Spindle);
					Serial3.write(0x22);
					Serial3.write(0xff);
					Serial3.write(0xff);
					Serial3.write(0xff);
					delay(2000);
					Serial3.print("pageConfig.t19.txt=");
					Serial3.write(0x22);
					Serial3.print("Spindle-Accel:");
					Serial3.print(eePageSp.accel_Spindle);
					Serial3.write(0x22);
					Serial3.write(0xff);
					Serial3.write(0xff);
					Serial3.write(0xff);
					delay(2000);
					Serial3.print("pageConfig.t19.txt=");
					Serial3.write(0x22);
					Serial3.print("Spindle-Accel2:");
					Serial3.print(configOne.accel_Spindle);
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
					Serial3.print(eePageSp.maxSpd_Axis_Z);
					Serial3.write(0x22);
					Serial3.write(0xff);
					Serial3.write(0xff);
					Serial3.write(0xff);
					delay(2000);
					Serial3.print("pageConfig.t19.txt=");
					Serial3.write(0x22);
					Serial3.print("accel_Axis_Z:");
					Serial3.print(eePageSp.accel_Axis_Z);
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
					Serial3.print(eePageSp.maxSpd_Axis_X);
					Serial3.write(0x22);
					Serial3.write(0xff);
					Serial3.write(0xff);
					Serial3.write(0xff);
					delay(2000);
					Serial3.print("pageConfig.t19.txt=");
					Serial3.write(0x22);
					Serial3.print("accel_Axis_X:");
					Serial3.print(eePageSp.accel_Axis_X);
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
					Serial3.print(eePageSp.maxSpd_Axis_B);
					Serial3.write(0x22);
					Serial3.write(0xff);
					Serial3.write(0xff);
					Serial3.write(0xff);
					delay(2000);
					Serial3.print("pageConfig.t19.txt=");
					Serial3.write(0x22);
					Serial3.print("accel_Axis_B:");
					Serial3.print(eePageSp.accel_Axis_B);
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
		case PAGE_INDEX1:
		{
			configPageIndex eePageIndex1;
			EEPROM.get(eePromAddress_Index_Main, eePageIndex1);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("Index1-MaxSpeed:");
			Serial3.print(eePageIndex1.maxSpd);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("Index1-Accel:");
			Serial3.print(eePageIndex1.accel);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			break;
		}

		case PAGE_MOVEZ:
		{
			configStepper eePageMoveZ;
			EEPROM.get(eePromAddress_MoveZ, eePageMoveZ);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("MoveZ-MaxSpeed:");
			Serial3.print(eePageMoveZ.maxSpd);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("MoveZ-Accel:");
			Serial3.print(eePageMoveZ.accel);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			break;
		}
		case PAGE_MOVEX:
		{
			configStepper eePageMoveX;
			EEPROM.get(eePromAddress_MoveX, eePageMoveX);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("MoveX-MaxSpeed:");
			Serial3.print(eePageMoveX.maxSpd);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("MoveX-Accel:");
			Serial3.print(eePageMoveX.accel);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
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
	configs eePromConfig;
	EEPROM.get(eePromAddress_Setup, eePromConfig);
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
	Serial3.print(eePromConfig.microsteps_Spindle);
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
	Serial3.print(eePromConfig.steps360_Spindle);
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
	Serial3.print(eePromConfig.gearRatio_Spindle);
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
	Serial3.print(eePromConfig.polarity_Spindle? "Low":"High");
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
	Serial3.print(eePromConfig.microsteps_Axis_Z);
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
	Serial3.print(eePromConfig.steps360_Axis_Z);
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
	Serial3.print(eePromConfig.distancePerRev_AxisZ);
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
	Serial3.print(eePromConfig.polarity_Axis_Z ? "Low" : "High");
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
	Serial3.print(eePromConfig.microsteps_Axis_X);
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
	Serial3.print(eePromConfig.steps360_Axis_X);
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
	Serial3.print(eePromConfig.distancePerRev_AxisX);
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
	Serial3.print(eePromConfig.polarity_Axis_X ? "Low" : "High");
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
	Serial3.print(eePromConfig.microsteps_Axis_B);
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
	Serial3.print(eePromConfig.steps360_Axis_B);
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
	Serial3.print(eePromConfig.polarity_Axis_B ? "Low" : "High");
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
	Serial3.print(configMain.limit_Min_Z);
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
	Serial3.print(configMain.limit_Max_Z);
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
	Serial3.print(configMain.limit_Min_X);
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
	Serial3.print(configMain.limit_Max_X);
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
	Serial3.print(configMain.limit_Min_B);
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
	Serial3.print(configMain.limit_Max_B);
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
	Serial3.print(configMain.maxSpd_Return_Spindle);
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
	Serial3.print(configMain.accel_Return_Spindle);
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
	Serial3.print(configMain.maxSpd_Return_Axis);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(1700);

	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Return Accel MaxSpd:");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(15);
	Serial3.print("pageSetup.t63.txt=");
	Serial3.write(0x22);
	Serial3.print(configMain.accel_Return_Axis);
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

