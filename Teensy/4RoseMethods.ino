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
double GetSerialFloat(int serialId)
{
	double retVal =-1;

	switch (serialId)
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
	//while (Serial3.available() > 0)
	//{
	//	// Discard remaining serial chars
	//	Serial3.read();
	//}
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

	Serial.print("Float:");
	Serial.println(inputNumber);
	return inputNumber;
}

float StepsToDegrees_Spindle(float steps)
{

	// (Steps / fullsteps*microsteps*gearRatio)*360
	float retVal = (steps / (configMain.steps360_Spindle * configMain.microsteps_Spindle* configMain.gearRatio_Spindle)) * 360;
	return retVal;
}

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
/// Index spindle with TeensyStep
/// </summary>
/// <comment>
/// </comment>
/// <param name="directionSpindle">Counterclockwise: -1, Clockwise: 1</param>
/// <returns></returns>
void MoveAxis(int axisId, int directionAxis)
{
	double stepsToMove = 0;
	float nextSpeed = 0;
//	long moveTo = 0;

	switch (axisId)
	{
		case ID_AXIS_Z:
		{
			teensyStep_Axis_Z.setPosition(0);
			stepsToMove = (configMain.distance_MoveZ / (configMain.distancePerRev_AxisZ) * (configMain.steps360_Axis_Z * configMain.microsteps_Axis_Z));

			// Set speed and acceleration
			nextSpeed = configMain.speedPercent_MoveZ * configMoveZ.maxSpd * .01;
			teensyStep_Axis_Z
				.setMaxSpeed(nextSpeed)
				.setAcceleration(configMoveZ.accel)
				.setTargetRel(stepsToMove * directionAxis);
			digitalWrite(PIN_AXIS_Z_ENABLE, LOW);
			stepControllerI.moveAsync(teensyStep_Axis_Z);
			break;
		}
#ifdef FOUR_AXES
		case ID_AXIS_X:
		{
			teensyStep_Axis_X.setPosition(0);
			stepsToMove = (configMain.distance_MoveX / (configMain.distancePerRev_AxisX) * (configMain.steps360_Axis_X * configMain.microsteps_Axis_X));

			// Set speed and acceleration
			nextSpeed = configMain.speedPercent_MoveX * configMoveX.maxSpd * .01;
			teensyStep_Axis_X
				.setMaxSpeed(nextSpeed)
				.setAcceleration(configMoveX.accel)
				.setTargetRel(stepsToMove * directionAxis);
			digitalWrite(PIN_AXIS_X_ENABLE, LOW);
			stepControllerI.moveAsync(teensyStep_Axis_X);
			break;
		}
#endif // FOUR_AXES
	}

	while (stepControllerI.isRunning())
	{
#ifdef SHOW_POSITION //Controls must be added to Nextion pages
		if (configMain.trackPositions == 1)
		{
			switch (axisId)
			{
				case ID_AXIS_Z:
				{

					endPosition_Axis = teensyStep_Axis_Z.getPosition();
					distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);

					Serial3.print("pageMoveZ.t2.txt=");
					Serial3.write(0x22);
					Serial3.print(distance_Axis);
					Serial3.write(0x22);
					Serial3.write(0xff);
					Serial3.write(0xff);
					Serial3.write(0xff);
					delay(5);
					Serial3.print("pageMoveZ.t2.txt=");
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
					endPosition_Axis = teensyStep_Axis_X.getPosition();
					distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);
					Serial3.print("pageMoveX.t2.txt=");
					Serial3.write(0x22);
					Serial3.print(distance_Axis);
					Serial3.write(0x22);
					Serial3.write(0xff);
					Serial3.write(0xff);
					Serial3.write(0xff);
					delay(5);
					Serial3.print("pageMoveX.t2.txt=");
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
				case 93: // - ] 
				case 99: // - c
				case 109: // - m
				{
					stepControllerI.stopAsync();
					break;
				}
			}

		}
		delay(10);
	}

	switch (axisId)
	{
#ifdef FOUR_AXES
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

			endPosition_Axis = teensyStep_Axis_Z.getPosition();
#ifdef SHOW_POSITION //Controls must be added to Nextion pages
			distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);
			Serial3.print("pageMoveZ.t2.txt=");
			Serial3.write(0x22);
			Serial3.print(distance_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(5);
			Serial3.print("pageMoveZ.t2.txt=");
			Serial3.write(0x22);
			Serial3.print(distance_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(5);
#endif // Show Position
			endPosition_Spindle = 0;
			break;
		}
#endif // FOUR_AXES
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

			endPosition_Axis = teensyStep_Axis_X.getPosition();
#ifdef SHOW_POSITION //Controls must be added to Nextion pages
			distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);
			Serial3.print("pageMoveX.t2.txt=");
			Serial3.write(0x22);
			Serial3.print(distance_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(5);
			Serial3.print("pageMoveX.t2.txt=");
			Serial3.write(0x22);
			Serial3.print(distance_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(5);
#endif // Show Position
			endPosition_Spindle = 0;
			break;
		}
	}
}

//void RunSpindle()
//{
//
//	teensyStep_Spindle
//		.setMaxSpeed(configIndex1.maxSpd_Spindle)
//		.setAcceleration(configIndex1.accel_Spindle)
//		.setTargetRel(stepsToMove * directionSpindle);
//
//}



/// <summary>
/// Run stepper at preset speed in Sp2 mode
/// </summary>
/// <comment>
/// </comment>
/// <param name="direction">Counterclockwise: 0, Clockwise: 1</param>
/// <returns>Success: 0, Cancel/Stop: -1</returns>
int RunStepper(
	int direction, 
	AccelStepper stepper, 
	int acceleration, 
	int speedPercentage, 
	int maxSpeed)
{
	int newSpeedPercentage = 0;
	stepper.setAcceleration(acceleration);
	float newSpeed = speedPercentage * maxSpeed * .01;
	stepper.setMaxSpeed(newSpeed);

	stepper.setCurrentPosition(0);

	long moveTo = 1000000000;

	if (direction == DIR_CCW)
	{
		moveTo = -moveTo;
	}
	stepper.moveTo(moveTo);
	stepper.enableOutputs();

	while (stepper.distanceToGo() != 0)
	{

		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
			case 67:
			{
				stepper.stop();
			}
			case 102: // f - Set Sp2 Spindle Speed
			{
				newSpeedPercentage = GetSerialInteger();
				float newSpeed = newSpeedPercentage * maxSpeed * .01;
				stepper.setMaxSpeed(newSpeed);
			}
			}
		}

		stepper.run();
	}

	endPosition_Spindle = stepper.currentPosition();
	return 0;
}


//#ifndef TWO_AXES_V2
/// <summary>
/// Run steppers X axis
/// </summary>
/// <comment>
/// </comment>
/// <param name="direction_Axis">Counterclockwise: 0, Clockwise: 1</param>
/// <returns>Success: 0, Cancel/Stop: -1</returns>
int RunTwoSteppers_SpX(
	int direction_Axis,
	int direction_Spindle,
	int initialCaller
)
{

	// X Axis Stop flag (Used to reset Nextion when limit switch is hit.)
	bool stopFlag = false;

	// initialCaller: Spindle or axis
	bool stepper_Axis_X_Go = false;
	bool stepper_Spindle_Go = false;

	if (initialCaller < 2)
	{
		stepper_Axis_X_Go = true;

	}
	else //if (initialCaller == 3)
	{
		stepper_Spindle_Go = true;
	}

	// Configure X Axis
	float nextSpeed_Axis = configMain.speedPercent_Axis_X * configSpX.maxSpd_Axis * .01;
	accelStep_Axis_X.setMaxSpeed(nextSpeed_Axis);
	accelStep_Axis_X.setAcceleration(configSpX.accel_Axis);
	accelStep_Axis_X.setCurrentPosition(0);
	long moveTo_Axis = 1000000000;
	if (direction_Axis == DIR_CCW)
	{
		moveTo_Axis = -moveTo_Axis;
	}
	accelStep_Axis_X.moveTo(moveTo_Axis);
	accelStep_Axis_X.enableOutputs();

	// Configure Spindle
	int spindleSpeedPercentage = configMain.speedPercentSpindle_SpX;
	int spindleMaxSpeed = configSpX.maxSpd_Spindle;

	float nextSpeed_Spindle = spindleSpeedPercentage * spindleMaxSpeed * .01;
	accelStep_Spindle.setMaxSpeed(nextSpeed_Spindle);
	accelStep_Spindle.setAcceleration(configSpX.accel_Spindle);

	accelStep_Spindle.setCurrentPosition(0);
	long moveTo_Spindle = 1000000000;

	if (direction_Spindle == DIR_CCW)
	{
		moveTo_Spindle = -moveTo_Spindle;
	}
	accelStep_Spindle.moveTo(moveTo_Spindle);
	accelStep_Spindle.enableOutputs();

	while ((accelStep_Spindle.distanceToGo() != 0) || (accelStep_Axis_X.distanceToGo() != 0))
	{
		stopFlag = false;

		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
				case 166: // ¦ - Sp-X Axis CW
				{
					configMain.speedPercent_Axis_X = GetSerialInteger();
					EEPROM.put(eePromAddress_Setup, configMain);
					nextSpeed_Axis = configMain.speedPercent_Axis_X  * configSpX.maxSpd_Axis * .01;
					accelStep_Axis_X.setMaxSpeed(nextSpeed_Axis);
					accelStep_Axis_X.setCurrentPosition(0);
					accelStep_Axis_X.moveTo(1000000000);
					stepper_Axis_X_Go = true;
					break;
				}
				case 165: // ¥ - Sp-X Axis CCW
				{
					nextSpeed_Axis = configMain.speedPercent_Axis_X  * configSpX.maxSpd_Axis * .01;
					accelStep_Axis_X.setMaxSpeed(nextSpeed_Axis);
					accelStep_Axis_X.setCurrentPosition(0);
					accelStep_Axis_X.moveTo(-1000000000);
					stepper_Axis_X_Go = true;
					break;
				}
				case 67: // C - Spindle Stop
				{
					accelStep_Spindle.stop();
					accelStep_Spindle.setCurrentPosition(0);
					if (!stepper_Axis_X_Go)
					{
						accelStep_Axis_B.setCurrentPosition(0);
					}
					stepper_Spindle_Go = false;
					break;
				}
				case 162: // ¢ - Sp-X axis speed
				{
					configMain.speedPercent_Axis_X = (int)GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Setup, configMain);
					nextSpeed_Axis = configMain.speedPercent_Axis_X * configSpX.maxSpd_Axis * .01;
					accelStep_Axis_X.setMaxSpeed(nextSpeed_Axis);
					break;
				}
				case 99: // c - Axis stop
				{
					accelStep_Axis_X.stop();
					accelStep_Axis_X.setCurrentPosition(0);
					if (!stepper_Spindle_Go)
					{
						accelStep_Spindle.setCurrentPosition(0);
					}
					stepper_Axis_X_Go = false;
					break;
				}
				case 164: // ¤ - Sp-X spindle CW
				{
					nextSpeed_Spindle = configMain.speedPercentSpindle_SpX * configSpX.maxSpd_Spindle * .01;
					accelStep_Spindle.setMaxSpeed(nextSpeed_Spindle);
					accelStep_Spindle.setCurrentPosition(0);
					accelStep_Spindle.moveTo(1000000000);
					stepper_Spindle_Go = true;
					break;
				}
				case 163: // £ - Sp-X spindle CCW
				{
					nextSpeed_Spindle = configMain.speedPercentSpindle_SpX * configSpX.maxSpd_Spindle * .01;
					accelStep_Spindle.setMaxSpeed(nextSpeed_Spindle);
					accelStep_Spindle.setCurrentPosition(0);
					accelStep_Spindle.moveTo(-1000000000);
					stepper_Spindle_Go = true;
					break;
				}
				case 161: // ¡ - Sp-X spindle speed
				{
					configMain.speedPercentSpindle_SpX = (int)GetSerialFloat(serialId);
					float nextSpeed = configMain.speedPercentSpindle_SpX * configSpX.maxSpd_Spindle * .01;
					accelStep_Spindle.setMaxSpeed(nextSpeed);
					EEPROM.put(eePromAddress_Setup, configMain);
					break;
				}
			}
		}

		if (accelStep_Axis_X.distanceToGo() > 0)
		{
			if (digitalRead(configMain.limit_Max_X) == LOW)
			{
				accelStep_Axis_X.stop();
				stopFlag = true;
				stepper_Axis_X_Go = false;
				if (!stepper_Spindle_Go)
				{
					accelStep_Spindle.setCurrentPosition(0);
				}
			}
		}
		else
		{
			if (digitalRead(configMain.limit_Min_X) == LOW)
			{
				accelStep_Axis_X.stop();

				stopFlag = true;
				stepper_Axis_X_Go = false;
				if (!stepper_Spindle_Go)
				{
					accelStep_Spindle.setCurrentPosition(0);
				}
			}
		}

		if (stepper_Spindle_Go)
		{

			accelStep_Spindle.run();
		}


		if (stepper_Axis_X_Go)
		{
			// Check limit switches
			if (accelStep_Axis_X.distanceToGo() > 0)
			{
				if (digitalRead(configMain.limit_Max_X) == LOW)
				{
					accelStep_Axis_X.stop();
					stopFlag = true;
					stepper_Axis_X_Go = false;
				}
			}
			else
			{
				if (digitalRead(configMain.limit_Min_X) == LOW)
				{
					accelStep_Axis_X.stop();
					stopFlag = true;
					stepper_Axis_X_Go = false;
				}
			}

			accelStep_Axis_X.run();

			// Reset stop flag on Nextion
			if (stopFlag)
			{
				Serial3.print("pageSpX.bt7.val=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.print("pageSpX.bt5.val=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.print("pageSpX.bt6.val=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				//delay(15);
				Serial3.print("pageSpX.va2.val=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
			}
		}
	}

	stepper_Axis_X_Go = false;
	stepper_Spindle_Go = false;

	endPosition_Spindle = accelStep_Spindle.currentPosition();
	endPosition_Axis = accelStep_Axis_X.currentPosition();

	//accelStep_Axis_X.setCurrentPosition(0);
	//accelStep_Spindle.setCurrentPosition(0);

	return 0;
}

//#endif //TWO_AXES_V2
//#ifdef FOUR_AXES
/// <summary>
/// Run steppers B axis (Spherical apparatus)
/// </summary>
/// <comment>
/// </comment>
/// <param name="direction_Axis">Counterclockwise: 0, Clockwise: 1</param>
/// <returns>Success: 0, Cancel/Stop: -1</returns>
int RunTwoSteppers_SpB(
	int direction_Axis,
	int direction_Spindle,
	int initialCaller
)
{
	// B Axis Stop flag (Used to reset Nextion when limit switch is hit.)
	bool stopFlag = false;

	// initialCaller: Spindle or axis
	bool stepper_Axis_B_Go = false;
	bool stepper_Spindle_Go = false;

	if (initialCaller < 2)
	{
		stepper_Axis_B_Go = true;
	}
	else //if (initialCaller == 3)
	{
		stepper_Spindle_Go = true;
	}

	// Configure B Axis
	float nextSpeed_Axis = configMain.speedPercent_Axis_B * configSpB.maxSpd_Axis * .01;
	accelStep_Axis_B.setMaxSpeed(nextSpeed_Axis);
	accelStep_Axis_B.setAcceleration(configSpB.accel_Axis);
	accelStep_Axis_B.setCurrentPosition(0);
	long moveTo_Axis = 1000000000;
	if (direction_Axis == DIR_CCW)
	{
		moveTo_Axis = -moveTo_Axis;
	}
	accelStep_Axis_B.moveTo(moveTo_Axis);
	accelStep_Axis_B.enableOutputs();

	// Configure Spindle
	int spindleSpeedPercentage = configMain.speedPercentSpindle_SpB;
	int spindleMaxSpeed = configSpB.maxSpd_Spindle;

	float nextSpeed_Spindle = spindleSpeedPercentage * spindleMaxSpeed * .01;
	accelStep_Spindle.setMaxSpeed(nextSpeed_Spindle);
	accelStep_Spindle.setAcceleration(configSpB.accel_Spindle);

	accelStep_Spindle.setCurrentPosition(0);
	long moveTo_Spindle = 1000000000;

	if (direction_Spindle == DIR_CCW)
	{
		moveTo_Spindle = -moveTo_Spindle;
	}
	accelStep_Spindle.moveTo(moveTo_Spindle);
	accelStep_Spindle.enableOutputs();
	while ((accelStep_Spindle.distanceToGo() != 0) || (accelStep_Axis_B.distanceToGo() != 0))
	{
		stopFlag = false;

		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
				case 172: // ¬ - Sp-B Axis CW
				{
					nextSpeed_Axis = configMain.speedPercent_Axis_B  * configSpB.maxSpd_Axis * .01;
					accelStep_Axis_B.setMaxSpeed(nextSpeed_Axis);
					accelStep_Axis_B.setCurrentPosition(0);
					accelStep_Axis_B.moveTo(1000000000);
					stepper_Axis_B_Go = true;
					break;
				}
				case 171: // « - Sp-B Axis CCW
				{
					nextSpeed_Axis = configMain.speedPercent_Axis_B  * configSpB.maxSpd_Axis * .01;
					accelStep_Axis_B.setMaxSpeed(nextSpeed_Axis);
					accelStep_Axis_B.setCurrentPosition(0);
					accelStep_Axis_B.moveTo(-1000000000);
					stepper_Axis_B_Go = true;
					break;
				}
				case 67: // C - Spindle Stop
				{
					accelStep_Spindle.stop();
					accelStep_Spindle.setCurrentPosition(0);
					if (!stepper_Axis_B_Go)
					{
						accelStep_Axis_B.setCurrentPosition(0);
					}
					stepper_Spindle_Go = false;
					break;
				}
				case 168: // ¨ - Sp-B axis speed
				{
					configMain.speedPercent_Axis_B = (int)GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Setup, configMain);
					nextSpeed_Axis = configMain.speedPercent_Axis_B * configSpB.maxSpd_Axis * .01;
					accelStep_Axis_B.setMaxSpeed(nextSpeed_Axis);
					break;
				}
				case 99: // c - Axis stop
				{
					accelStep_Axis_B.stop();
					accelStep_Axis_B.setCurrentPosition(0);
					if (!stepper_Axis_B_Go)
					{
						accelStep_Spindle.setCurrentPosition(0);
					}
					stepper_Axis_B_Go = false;
					break;
				}
				case 170: // ª - Sp-B spindle CW
				{
					nextSpeed_Spindle = configMain.speedPercentSpindle_SpB * configSpB.maxSpd_Spindle * .01;
					accelStep_Spindle.setMaxSpeed(nextSpeed_Spindle);
					accelStep_Spindle.setCurrentPosition(0);
					accelStep_Spindle.moveTo(1000000000);
					stepper_Spindle_Go = true;
					break;
				}
				case 169: // © - Sp-B spindle CCW
				{
					nextSpeed_Spindle = configMain.speedPercentSpindle_SpB * configSpB.maxSpd_Spindle * .01;
					accelStep_Spindle.setMaxSpeed(nextSpeed_Spindle);
					accelStep_Spindle.setCurrentPosition(0);
					accelStep_Spindle.moveTo(-1000000000);
					stepper_Spindle_Go = true;
					break;
				}
				case 167: // § - Sp-B spindle speed
				{
					configMain.speedPercentSpindle_SpB = (int)GetSerialFloat(serialId);
					float nextSpeed = configMain.speedPercentSpindle_SpB * configSpB.maxSpd_Spindle * .01;
					accelStep_Spindle.setMaxSpeed(nextSpeed);
					EEPROM.put(eePromAddress_Setup, configMain);
					break;
				}
			}
		}

		if (accelStep_Axis_B.distanceToGo() > 0)
		{
			if (digitalRead(configMain.limit_Max_B) == LOW)
			{
				accelStep_Axis_B.stop();
				stopFlag = true;
				stepper_Axis_B_Go = false;
				if (!stepper_Spindle_Go)
				{
					accelStep_Spindle.setCurrentPosition(0);
				}
			}
		}
		else
		{
			if (digitalRead(configMain.limit_Min_B) == LOW)
			{
				accelStep_Axis_B.stop();

				stopFlag = true;
				stepper_Axis_B_Go = false;
				if (!stepper_Spindle_Go)
				{
					accelStep_Spindle.setCurrentPosition(0);
				}
			}
		}

		if (stepper_Spindle_Go)
		{
			accelStep_Spindle.run();
		}

		if (stepper_Axis_B_Go)
		{
			// Check limit switches
			if (accelStep_Axis_B.distanceToGo() > 0)
			{
				if (digitalRead(configMain.limit_Max_B) == LOW)
				{
					accelStep_Axis_B.stop();
					stopFlag = true;
					stepper_Axis_B_Go = false;
				}
			}
			else
			{
				if (digitalRead(configMain.limit_Min_B) == LOW)
				{
					accelStep_Axis_B.stop();
					stopFlag = true;
					stepper_Axis_B_Go = false;
				}
			}

			accelStep_Axis_B.run();

			// Reset stop flag on Nextion
			if (stopFlag)
			{
				Serial3.print("pageSpB.bt7.val=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.print("pageSpB.bt5.val=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.print("pageSpB.bt6.val=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.print("pageSpB.va2.val=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
			}
		}
	}

	stepper_Axis_B_Go = false;
	stepper_Spindle_Go = false;

	endPosition_Spindle = accelStep_Spindle.currentPosition();
	endPosition_Axis = accelStep_Axis_B.currentPosition();

	return 0;
}

/// <summary>
/// Run steppers B axis (Spherical apparatus)
/// </summary>
/// <comment>
/// </comment>
/// <param name="direction_Axis">Counterclockwise: 0, Clockwise: 1</param>
/// <returns>Success: 0, Cancel/Stop: -1</returns>
int RunTwoSteppers_SpZ(
	int direction_Axis,
	int direction_Spindle,
	int initialCaller
)
{
	// B Axis Stop flag (Used to reset Nextion when limit switch is hit.)
	bool stopFlag = false;

	// initialCaller: Spindle or axis
	bool stepper_Axis_Z_Go = false;
	bool stepper_Spindle_Go = false;

	if (initialCaller < 2)
	{
		stepper_Axis_Z_Go = true;
	}
	else //if (initialCaller == 3)
	{
		stepper_Spindle_Go = true;
	}

	// Configure B Axis
	float nextSpeed_Axis = configMain.speedPercent_Axis_Z * configSpZ.maxSpd_Axis * .01;
	accelStep_Axis_Z.setMaxSpeed(nextSpeed_Axis);
	accelStep_Axis_Z.setAcceleration(configSpZ.accel_Axis);
	accelStep_Axis_Z.setCurrentPosition(0);
	long moveTo_Axis = 1000000000;
	if (direction_Axis == DIR_CCW)
	{
		moveTo_Axis = -moveTo_Axis;
	}
	accelStep_Axis_Z.moveTo(moveTo_Axis);
	accelStep_Axis_Z.enableOutputs();

	// Configure Spindle
	int spindleSpeedPercentage = configMain.speedPercentSpindle_SpZ;
	int spindleMaxSpeed = configSpZ.maxSpd_Spindle;

	float nextSpeed_Spindle = spindleSpeedPercentage * spindleMaxSpeed * .01;
	accelStep_Spindle.setMaxSpeed(nextSpeed_Spindle);
	accelStep_Spindle.setAcceleration(configSpZ.accel_Spindle);

	accelStep_Spindle.setCurrentPosition(0);
	long moveTo_Spindle = 1000000000;

	if (direction_Spindle == DIR_CCW)
	{
		moveTo_Spindle = -moveTo_Spindle;
	}
	accelStep_Spindle.moveTo(moveTo_Spindle);
	accelStep_Spindle.enableOutputs();
	while ((accelStep_Spindle.distanceToGo() != 0) || (accelStep_Axis_Z.distanceToGo() != 0))
	{
		stopFlag = false;


		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
			case 90: // ¬ - Sp-Z Axis CW
			{
				nextSpeed_Axis = configMain.speedPercent_Axis_Z  * configSpZ.maxSpd_Axis * .01;
				accelStep_Axis_Z.setMaxSpeed(nextSpeed_Axis);
				accelStep_Axis_Z.setCurrentPosition(0);
				accelStep_Axis_Z.moveTo(1000000000);
				stepper_Axis_Z_Go = true;
				break;
			}
			case 91: // « - Sp-Z Axis CCW
			{
				nextSpeed_Axis = configMain.speedPercent_Axis_Z  * configSpZ.maxSpd_Axis * .01;
				accelStep_Axis_Z.setMaxSpeed(nextSpeed_Axis);
				accelStep_Axis_Z.setCurrentPosition(0);
				accelStep_Axis_Z.moveTo(-1000000000);
				stepper_Axis_Z_Go = true;
				break;
			}
			case 67: // C - Spindle Stop
			{
				accelStep_Spindle.stop();
				accelStep_Spindle.setCurrentPosition(0);
				if (!stepper_Axis_Z_Go)
				{
					accelStep_Axis_Z.setCurrentPosition(0);
				}
				stepper_Spindle_Go = false;
				break;
			}
			case 97: // ¨ - Sp-Z axis speed
			{
				configMain.speedPercent_Axis_Z = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Setup, configMain);
				nextSpeed_Axis = configMain.speedPercent_Axis_Z * configSpZ.maxSpd_Axis * .01;
				accelStep_Axis_Z.setMaxSpeed(nextSpeed_Axis);
				break;
			}
			case 99: // c - Axis stop
			{
				accelStep_Axis_Z.stop();
				accelStep_Axis_Z.setCurrentPosition(0);
				if (!stepper_Axis_Z_Go)
				{
					accelStep_Spindle.setCurrentPosition(0);
				}
				stepper_Axis_Z_Go = false;
				break;
			}
			case 107: // ª - Sp-Z spindle CW
			{
				nextSpeed_Spindle = configMain.speedPercentSpindle_SpZ * configSpZ.maxSpd_Spindle * .01;
				accelStep_Spindle.setMaxSpeed(nextSpeed_Spindle);
				accelStep_Spindle.setCurrentPosition(0);
				accelStep_Spindle.moveTo(1000000000);
				stepper_Spindle_Go = true;
				break;
			}
			case 108: // © - Sp-Z spindle CCW
			{
				nextSpeed_Spindle = configMain.speedPercentSpindle_SpZ * configSpZ.maxSpd_Spindle * .01;
				accelStep_Spindle.setMaxSpeed(nextSpeed_Spindle);
				accelStep_Spindle.setCurrentPosition(0);
				accelStep_Spindle.moveTo(-1000000000);
				stepper_Spindle_Go = true;
				break;
			}
			case 110: // § - Sp-Z spindle speed
			{
				configMain.speedPercentSpindle_SpZ = (int)GetSerialFloat(serialId);
				float nextSpeed = configMain.speedPercentSpindle_SpZ * configSpZ.maxSpd_Spindle * .01;
				accelStep_Spindle.setMaxSpeed(nextSpeed);
				EEPROM.put(eePromAddress_Setup, configMain);
				break;
			}
			}
		}

		if (accelStep_Axis_Z.distanceToGo() > 0)
		{
			if (digitalRead(configMain.limit_Max_Z) == LOW)
			{
				accelStep_Axis_Z.stop();
				stopFlag = true;
				stepper_Axis_Z_Go = false;
				if (!stepper_Spindle_Go)
				{
					accelStep_Spindle.setCurrentPosition(0);
				}
			}
		}
		else
		{
			if (digitalRead(configMain.limit_Min_Z) == LOW)
			{
				accelStep_Axis_Z.stop();

				stopFlag = true;
				stepper_Axis_Z_Go = false;
				if (!stepper_Spindle_Go)
				{
					accelStep_Spindle.setCurrentPosition(0);
				}
			}
		}

		if (stepper_Spindle_Go)
		{
			accelStep_Spindle.run();
		}

		if (stepper_Axis_Z_Go)
		{
			// Check limit switches
			if (accelStep_Axis_Z.distanceToGo() > 0)
			{
				if (digitalRead(configMain.limit_Max_Z) == LOW)
				{
					accelStep_Axis_Z.stop();
					stopFlag = true;
					stepper_Axis_Z_Go = false;
				}
			}
			else
			{
				if (digitalRead(configMain.limit_Min_Z) == LOW)
				{
					accelStep_Axis_Z.stop();
					stopFlag = true;
					stepper_Axis_Z_Go = false;
				}
			}

			accelStep_Axis_Z.run();

			// Reset stop flag on Nextion
			if (stopFlag)
			{
				Serial3.print("pageSpB.bt7.val=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.print("pageSpB.bt5.val=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.print("pageSpB.bt6.val=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.print("pageSpB.va2.val=0");
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
			}
		}
	}

	stepper_Axis_Z_Go = false;
	stepper_Spindle_Go = false;

	endPosition_Spindle = accelStep_Spindle.currentPosition();
	endPosition_Axis = accelStep_Axis_Z.currentPosition();

	//accelStep_Axis_B.setCurrentPosition(0);
	//accelStep_Spindle.setCurrentPosition(0);

	return 0;
}
//#endif // FOUR_AXES

/// <summary>
/// Index spindle with TeensyStep
/// </summary>
/// <comment>
/// </comment>
/// <param name="directionSpindle">Counterclockwise: -1, Clockwise: 1</param>
/// <returns></returns>
void IndexSpindle(int indexId, int directionSpindle)
{
	teensyStep_Spindle.setPosition(0);
	double stepsToMove = configMain.size_Index1;

	switch (indexId)
	{
		case 1:
		{
			// Set speed and acceleration
			teensyStep_Spindle
				.setMaxSpeed(configIndex1.maxSpd)
				.setAcceleration(configIndex1.accel)
				.setTargetRel(stepsToMove * directionSpindle);
			break;
		}
		case 2:
		{
			stepsToMove = configMain.size_Index2;

			// Set speed and acceleration
			teensyStep_Spindle
				.setMaxSpeed(configIndex2.maxSpd)
				.setAcceleration(configIndex2.accel)
				.setTargetRel(stepsToMove * directionSpindle);
			break;
		}
	}

	digitalWrite(PIN_SPINDLE_ENABLE, LOW);
	stepControllerI.moveAsync(teensyStep_Spindle);

	while (stepControllerI.isRunning())
	{
#ifdef SHOW_POSITION //Controls must be added to Nextion pages
		//if (configMain.trackPositions == 1)
		//{
		//	endPosition_Spindle = teensyStep_Spindle.getPosition();
		//	degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
		//	switch (indexId)
		//		{
		//		case 1:
		//		{
		//			Serial3.print("pageIndex1.t1.txt=");
		//			break;
		//		}
		//		case 2:
		//		{
		//			Serial3.print("pageIndex2.t1.txt=");
		//			break;
		//		}
		//	}

		//	Serial3.write(0x22);
		//	Serial3.print(degrees_Spindle);
		//	Serial3.write(0x22);
		//	Serial3.write(0xff);
		//	Serial3.write(0xff);
		//	Serial3.write(0xff);
		//	delay(10);
		//}
#endif // Show Position

		// Check for Cancel code  
		//if (SerialAvailable() >= 0)
		//{
		//	incomingByte = SerialRead(serialId);
		//	switch (incomingByte)
		//	{
		//		case 93: // - ] 
		//		case 99: // - c
		//		case 109: // - m
		//		{
		//			stepControllerI.stopAsync();
		//			break;
		//		}
		//	}

		//}
		delay(10);
	}

	if (indexId == 2)
	{
		// Update Index flag on Nextion. 
		// Update Nextion
		Serial3.print("pageIndex2.bt3.pco=0");// The first one may be ignored by Nextion, so resend.
		Serial3.write(0xff);
		Serial3.write(0xff);
		Serial3.write(0xff);
		Serial3.print("pageIndex2.va0.val=0");
		Serial3.write(0xff);
		Serial3.write(0xff);
		Serial3.write(0xff);
		Serial3.print("pageIndex2.bt3.val=0");
		Serial3.write(0xff);
		Serial3.write(0xff);
		Serial3.write(0xff);
		Serial3.print("pageIndex2.bt2.val=0");
		Serial3.write(0xff);
		Serial3.write(0xff);
		Serial3.write(0xff);
		Serial3.print("pageIndex2.bt1.val=0");
		Serial3.write(0xff);
		Serial3.write(0xff);
		Serial3.write(0xff);
	}
	else
	{ 
		
		// Update Index flag on Nextion. 
		Serial3.print("pageIndex1.bt3.pco=0");// The first one may be ignored by Nextion, so resend.
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

	endPosition_Axis = 0;
	endPosition_Spindle = teensyStep_Spindle.getPosition();
#ifdef SHOW_POSITION //Controls must be added to Nextion pages
	degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
	switch (indexId)
	{
	case 1:
	{
		Serial3.print("pageIndex1.t1.txt=");
		break;
	}
	case 2:
	{
		Serial3.print("pageIndex2.t1.txt=");
		break;
	}
	}

	Serial3.write(0x22);
	Serial3.print(degrees_Spindle);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(10);
#endif // Show Position
}

/// <summary>
/// Sync spindle and Z axis  with TeensyStep
/// </summary>
/// <comment>
/// </comment>
/// <param name="directionSpindle">Counterclockwise: -1, Clockwise: 1</param>
/// <param name = "directionAxis">Counterclockwise: -1, Clockwise : 1< / param>
/// <returns></returns>
void Sync_SpindleZ(int directionSpindle, int directionAxis)
{
	StepControl stepController;
	// Set speed and acceleration
	teensyStep_Spindle
		.setMaxSpeed(configSyncZ.maxSpd_Spindle * configMain.speedPercentSpindle_SyncZ * .01)
		.setAcceleration(configSyncZ.accel_Spindle)
		.setTargetRel(configMain.revolutionsSyncZ_Spindle * configMain.gearRatio_Spindle * configMain.microsteps_Spindle * configMain.steps360_Spindle * directionSpindle);
	teensyStep_Spindle.setPosition(0);

	teensyStep_Axis_Z
		.setMaxSpeed(configSyncZ.maxSpd_Axis * configMain.speedPercentAxis_SyncZ * .01)
		.setAcceleration(configSyncZ.accel_Axis)
		.setTargetRel((configMain.distanceSyncZ / configMain.distancePerRev_AxisZ) * configMain.microsteps_Axis_Z * configMain.steps360_Axis_Z * directionAxis);
	teensyStep_Axis_Z.setPosition(0);

	digitalWrite(PIN_SPINDLE_ENABLE, LOW); // Enable (Uses TeensyStep library)
	digitalWrite(PIN_AXIS_Z_ENABLE, LOW);  // Enable (Uses TeensyStep library)
	
	stepController.moveAsync(teensyStep_Spindle, teensyStep_Axis_Z);

	while (stepController.isRunning())
	{ 
#ifdef DEBUG
		endPosition_Axis = teensyStep_Axis_Z.getPosition();
		endPosition_Spindle = teensyStep_Spindle.getPosition();
		Serial.print("Spindle:");
		Serial.println(endPosition_Spindle);
		Serial.print("Axis:");
		Serial.println(endPosition_Axis);
#endif // DEBUG
#ifdef SHOW_POSITION //Controls must be added to Nextion pages
		if (configMain.trackPositions == 1)
		{
			endPosition_Axis = teensyStep_Axis_Z.getPosition();
			endPosition_Spindle = teensyStep_Spindle.getPosition();
			degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
			distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);
			Serial3.print("pageSync.t1.txt=");
			Serial3.write(0x22);
			Serial3.print(degrees_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(10);
			Serial3.print("pageSync.t2.txt=");
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
				case 93: // - ] 
				case 99: // - c
				case 109: // - m
				{
					stepController.stopAsync();
					break;
				}
			}
		}

		delay(10);
	}

	endPosition_Axis = teensyStep_Axis_Z.getPosition();
	endPosition_Spindle = teensyStep_Spindle.getPosition();
#ifdef SHOW_POSITION //Controls must be added to Nextion pages
	degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
	distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);

	// Update Nextion
	Serial3.print("pageSync.t1.txt=");
	Serial3.write(0x22);
	Serial3.print(degrees_Spindle);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(10);
	Serial3.print("pageSync.t2.txt=");
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
}

//#ifndef TWO_AXES_V2
/// <summary>
/// Sync spindle and X axis with TeensyStep
/// </summary>
/// <comment>
/// </comment>
/// <param name="directionSpindle">Counterclockwise: -1, Clockwise: 1</param>
/// <param name = "directionAxis">Counterclockwise: -1, Clockwise : 1< / param>
/// <returns></returns>
void Sync_SpindleX(int directionSpindle, int directionAxis)
{
	StepControl stepController;
	// Set speed and acceleration
	teensyStep_Spindle
		.setMaxSpeed(configSyncX.maxSpd_Spindle * configMain.speedPercentSpindle_SyncX * .01)
		.setAcceleration(configSyncX.accel_Spindle)
		.setTargetRel(configMain.revolutionsSyncX_Spindle * configMain.gearRatio_Spindle * configMain.microsteps_Spindle * configMain.steps360_Spindle * directionSpindle);
	teensyStep_Spindle.setPosition(0);

	teensyStep_Axis_X
		.setMaxSpeed(configSyncX.maxSpd_Axis * configMain.speedPercentAxis_SyncX * .01)
		.setAcceleration(configSyncX.accel_Axis)
		.setTargetRel((configMain.distanceSyncX / configMain.distancePerRev_AxisX) * configMain.microsteps_Axis_X * configMain.steps360_Axis_X * directionAxis);
	teensyStep_Axis_X.setPosition(0);

#ifdef DEBUG
	Serial.print("SpindleMaxSpd:");
	Serial.println(configSyncX.maxSpd_Spindle);
	Serial.print("SpindleSpeedPercent:");
	Serial.println(configMain.speedPercentSpindle_SyncX);
	Serial.print("SpindleAccel:");
	Serial.println(configSyncX.accel_Spindle);

	Serial.print("microsteps_Spindle:");
	Serial.println(configMain.microsteps_Spindle);
	Serial.print("steps360_Spindle:");
	Serial.println(configMain.steps360_Spindle);
	Serial.print("gearRatio_Spindle:");
	Serial.println(configMain.gearRatio_Spindle);
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

	Serial.print("AxisMaxSpd:");
	Serial.println(configSyncX.maxSpd_Axis);
	Serial.print("speedPercentAxis_SyncX:");
	Serial.println(configMain.speedPercentAxis_SyncX);
	Serial.print("AxisAccel:");
	Serial.println(configSyncX.accel_Axis);
#endif // DEBUG


	digitalWrite(PIN_SPINDLE_ENABLE, LOW); // Enable (Uses TeensyStep library)
	digitalWrite(PIN_AXIS_X_ENABLE, LOW);  // Enable (Uses TeensyStep library)

	stepController.moveAsync(teensyStep_Spindle, teensyStep_Axis_X);

	while (stepController.isRunning())
	{
#ifdef DEBUG
		endPosition_Axis = teensyStep_Axis_X.getPosition();
		endPosition_Spindle = teensyStep_Spindle.getPosition();
		Serial.print("Spindle:");
		Serial.println(endPosition_Spindle);
		Serial.print("Axis:");
		Serial.println(endPosition_Axis);
#endif // DEBUG
#ifdef SHOW_POSITION //Controls must be added to Nextion pages
		if (configMain.trackPositions == 1)
		{
			endPosition_Axis = teensyStep_Axis_X.getPosition();
			endPosition_Spindle = teensyStep_Spindle.getPosition();
			degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
			distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_X);
			Serial3.print("pageSyncX.t1.txt=");
			Serial3.write(0x22);
			Serial3.print(degrees_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(10);
			Serial3.print("pageSyncX.t2.txt=");
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
				case 93: // - ] 
				case 99: // - c
				case 109: // - m
				{
					stepController.stopAsync();
					break;
				}
			}
		}

		delay(10);
	}

	endPosition_Axis = teensyStep_Axis_X.getPosition();
	endPosition_Spindle = teensyStep_Spindle.getPosition();
#ifdef SHOW_POSITION //Controls must be added to Nextion pages
	degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
	distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_X);

	// Update Nextion
	Serial3.print("pageSyncX.t1.txt=");
	Serial3.write(0x22);
	Serial3.print(degrees_Spindle);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(10);
	Serial3.print("pageSyncX.t2.txt=");
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
}

//#endif // TWO_AXES_V2


/// <summary>
/// DoRec1_RadialZ
/// </summary>
/// <comment>
/// </comment>
/// <param name="wavDir">Counterclockwise: -1, Clockwise: 1</param>
/// <returns></returns>
void DoReci_RadialZ(int wavDir)
{
	StepControl stepController;

	// Set speed and acceleration
	teensyStep_Spindle
		.setMaxSpeed(configReci.maxSpd_Spindle * configReci.speedPercent_Spindle * .01)
		.setAcceleration(configReci.accel_Spindle);
	teensyStep_Spindle.setPosition(0);

	float spindleDegrees = configReci.degrees_RadialZ/ (configReci.waves_RadialZ * 2);
	int spindleSteps = round((configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) * (spindleDegrees / 360) * wavDir);
	returnSteps_Rec1_S = spindleSteps* configReci.waves_RadialZ * 2;

	teensyStep_Axis_Z
		.setMaxSpeed(configReci.maxSpd_Axis_Z * configReci.speedPercent_Axis_Z * .01)
		.setAcceleration(configReci.accel_Axis_Z);
	teensyStep_Axis_Z.setPosition(0);
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

		teensyStep_Spindle.setTargetRel(spindleSteps);
		teensyStep_Axis_Z.setTargetRel(axisSteps);

		stepController.moveAsync(teensyStep_Spindle, teensyStep_Axis_Z);

		while (stepController.isRunning())
		{
#ifdef DEBUG
			endPosition_Axis = teensyStep_Axis_Z.getPosition();
			endPosition_Spindle = teensyStep_Spindle.getPosition();
			Serial.print("Spindle:");
			Serial.println(endPosition_Spindle);
			Serial.print("Axis:");
			Serial.println(endPosition_Axis);
#endif // DEBUG
#ifdef SHOW_POSITION //Controls must be added to Nextion pages
			if (configMain.trackPositions == 1)
			{

				degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
				distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);
				Serial3.print("pageRecRadialZ.t1.txt=");
				Serial3.write(0x22);
				Serial3.print(degrees_Spindle);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				delay(10);
				Serial3.print("pageRecRadialZ.t2.txt=");
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
					case 93: // - ] 
					case 99: // - c
					case 109: // - m
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

	endPosition_Axis = teensyStep_Axis_Z.getPosition();
	endPosition_Spindle = teensyStep_Spindle.getPosition();
#ifdef SHOW_POSITION //Controls must be added to Nextion pages
	degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
	distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);
	Serial3.print("pageRecRadialZ.t1.txt=");
	Serial3.write(0x22);
	Serial3.print(degrees_Spindle);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(10);
	Serial3.print("pageRecRadialZ.t2.txt=");
	Serial3.write(0x22);
	Serial3.print(distance_Axis);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
#endif // Show Position
	// Update Nextion
	Serial3.print("pageRecRadialZ.bt3.pco=0");// The first one may be ignored by Nextion, so resend.
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRecRadialZ.va0.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRecRadialZ.bt3.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRecRadialZ.bt2.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRecRadialZ.bt1.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
}

/// <summary>
/// DoRec1_RadialX
/// </summary>
/// <comment>
/// </comment>
/// <param name="wavDir">Counterclockwise: -1, Clockwise: 1</param>
/// <returns></returns>
void DoReci_RadialX(int wavDir)
{
	StepControl stepController;

	// Set speed and acceleration
	teensyStep_Spindle
		.setMaxSpeed(configReci.maxSpd_Spindle * configReci.speedPercent_Spindle * .01)
		.setAcceleration(configReci.accel_Spindle);
	teensyStep_Spindle.setPosition(0);

	float spindleDegrees = configReci.degrees_RadialX / (configReci.waves_RadialX * 2);
	int spindleSteps = round((configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) * (spindleDegrees / 360) * wavDir);
	returnSteps_Rec1_S = spindleSteps * configReci.waves_RadialX * 2;

	teensyStep_Axis_Z
		.setMaxSpeed(configReci.maxSpd_Axis_X * configReci.speedPercent_Axis_X * .01)
		.setAcceleration(configReci.accel_Axis_X);
	teensyStep_Axis_X.setPosition(0);
	float axisDistance = configReci.amplitude_RadialX;
	int axisSteps = round((axisDistance / configMain.distancePerRev_AxisX) * configMain.microsteps_Axis_X * configMain.steps360_Axis_X * wavDir);

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

		teensyStep_Spindle.setTargetRel(spindleSteps);
		teensyStep_Axis_X.setTargetRel(axisSteps);

		stepController.moveAsync(teensyStep_Spindle, teensyStep_Axis_X);

		while (stepController.isRunning())
		{
#ifdef DEBUG
			endPosition_Axis = teensyStep_Axis_X.getPosition();
			endPosition_Spindle = teensyStep_Spindle.getPosition();
			Serial.print("Spindle:");
			Serial.println(endPosition_Spindle);
			Serial.print("Axis:");
			Serial.println(endPosition_Axis);
#endif // DEBUG
#ifdef SHOW_POSITION //Controls must be added to Nextion pages
			if (configMain.trackPositions == 1)
			{
				endPosition_Axis = teensyStep_Axis_X.getPosition();
				endPosition_Spindle = teensyStep_Spindle.getPosition();
				degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
				distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_X);
				Serial3.print("pageRecRadialX.t1.txt=");
				Serial3.write(0x22);
				Serial3.print(degrees_Spindle);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				delay(10);
				Serial3.print("pageRecRadialX.t2.txt=");
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
				case 93: // - ] 
				case 99: // - c
				case 109: // - m
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

	endPosition_Axis = teensyStep_Axis_X.getPosition();
	endPosition_Spindle = teensyStep_Spindle.getPosition();
#ifdef SHOW_POSITION //Controls must be added to Nextion pages
	degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
	distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_X);
	Serial3.print("pageRecRadialX.t1.txt=");
	Serial3.write(0x22);
	Serial3.print(degrees_Spindle);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(10);
	Serial3.print("pageRecRadialX.t2.txt=");
	Serial3.write(0x22);
	Serial3.print(distance_Axis);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
#endif // Show Position
	// Update Nextion
	Serial3.print("pageRecRadialX.bt3.pco=0");// The first one may be ignored by Nextion, so resend.
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRecRadialX.va0.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRecRadialX.bt3.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRecRadialX.bt2.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRecRadialX.bt1.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
}

/// <summary>
/// DoRec1_AxialZ
/// </summary>
/// <comment>
/// </comment>
/// <param name="wavDir">In: -1, Out: 1</param>
/// <returns></returns>
void DoReci_AxialZ(int wavDir)
{
	StepControl stepController;


	// Set speed and acceleration
	teensyStep_Spindle
		.setMaxSpeed(configReci.maxSpd_Spindle * configReci.speedPercent_Spindle * .01)
		.setAcceleration(configReci.accel_Spindle);
	teensyStep_Spindle.setPosition(0);

	int spindleSteps = (configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) * (configReci.amplitude_AxialZ / 360) * wavDir;
	long axisSteps = (((configReci.distance_AxialZ / (configMain.distancePerRev_AxisZ)) * (configMain.steps360_Axis_Z * configMain.microsteps_Axis_Z)) / (configReci.waves_AxialZ * 2)) * wavDir;
	returnSteps_Rec1_Z = axisSteps* configReci.waves_AxialZ * 2;

	teensyStep_Axis_Z
		.setMaxSpeed(configReci.maxSpd_Axis_Z * configReci.speedPercent_Axis_Z * .01)
		.setAcceleration(configReci.accel_Axis_Z);
	teensyStep_Axis_Z.setPosition(0);

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
	Serial.print("distance_AxialZ:");
	Serial.println(configReci.distance_AxialZ);
	Serial.print("waves_AxialZ:");
	Serial.println(configReci.waves_AxialZ);

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


	digitalWrite(PIN_SPINDLE_ENABLE, LOW); // Enable (Uses TeensyStep library)
	digitalWrite(PIN_AXIS_Z_ENABLE, LOW);  // Enable (Uses TeensyStep library)
	for (int i = 0; i < (configReci.waves_AxialZ * 2); i++)
	{
		spindleSteps *= -1;

		teensyStep_Spindle.setTargetRel(spindleSteps);
		teensyStep_Axis_Z.setTargetRel(axisSteps);

		stepController.moveAsync(teensyStep_Spindle, teensyStep_Axis_Z);
		delay(10);

		while (stepController.isRunning())
		{
#ifdef DEBUG
			endPosition_Axis = teensyStep_Axis_Z.getPosition();
			endPosition_Spindle = teensyStep_Spindle.getPosition();
			Serial.print("Spindle:");
			Serial.println(endPosition_Spindle);
			Serial.print("Axis:");
			Serial.println(endPosition_Axis);
#endif // DEBUG
#ifdef SHOW_POSITION //Controls must be added to Nextion pages
			if (configMain.trackPositions == 1)
			{
				endPosition_Axis = teensyStep_Axis_Z.getPosition();
				endPosition_Spindle = teensyStep_Spindle.getPosition();
				degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
				distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);
				Serial3.print("pageRecAxialZ.t1.txt=");
				Serial3.write(0x22);
				Serial3.print(degrees_Spindle);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				delay(10);
				Serial3.print("pageRecAxialZ.t2.txt=");
				Serial3.write(0x22);
				Serial3.print(distance_Axis);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
			}
#endif //Show position

			// Check for Cancel code  
			if (SerialAvailable() > 0)
			{
				incomingByte = SerialRead(serialId);
				switch (incomingByte)
				{
					case 93: // - ] 
					case 99: // - c
					case 109: // - m
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

	endPosition_Axis = teensyStep_Axis_Z.getPosition();
	endPosition_Spindle = teensyStep_Spindle.getPosition();
#ifdef SHOW_POSITION //Controls must be added to Nextion pages
	degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
	distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);
	Serial3.print("pageRecAxialZ.t1.txt=");
	Serial3.write(0x22);
	Serial3.print(degrees_Spindle);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(10);
	Serial3.print("pageRecAxialZ.t2.txt=");
	Serial3.write(0x22);
	Serial3.print(distance_Axis);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
#endif //Show position

	// Update Nextion
	Serial3.print("pageRecAxialZ.bt3.pco=59391");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRecAxialZ.va0.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRecAxialZ.bt3.val=0");// The first one may be ignored by Nextion, so resend.
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRecAxialZ.bt2.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRecAxialZ.bt1.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
}

/// <summary>
/// DoRec1_AxialX
/// </summary>
/// <comment>
/// </comment>
/// <param name="wavDir">In: -1, Out: 1</param>
/// <returns></returns>
void DoReci_AxialX(int wavDir)
{
	StepControl stepController;


	// Set speed and acceleration
	teensyStep_Spindle
		.setMaxSpeed(configReci.maxSpd_Spindle * configReci.speedPercent_Spindle * .01)
		.setAcceleration(configReci.accel_Spindle);
	teensyStep_Spindle.setPosition(0);

int spindleSteps = (configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) * (configReci.amplitude_AxialX / 360) * wavDir;
	long axisSteps = (((configReci.distance_AxialX / (configMain.distancePerRev_AxisZ)) * (configMain.steps360_Axis_Z * configMain.microsteps_Axis_Z)) / (configReci.waves_AxialX * 2)) * wavDir;
	returnSteps_Rec1_Z = axisSteps * configReci.waves_AxialX * 2;

	teensyStep_Axis_X
		.setMaxSpeed(configReci.maxSpd_Axis_X * configReci.speedPercent_Axis_X * .01)
		.setAcceleration(configReci.accel_Axis_X);
	teensyStep_Axis_X.setPosition(0);

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

		teensyStep_Spindle.setTargetRel(spindleSteps);
		teensyStep_Axis_X.setTargetRel(axisSteps);

		stepController.moveAsync(teensyStep_Spindle, teensyStep_Axis_X);
		delay(10);

		while (stepController.isRunning())
		{
#ifdef DEBUG
			endPosition_Axis = teensyStep_Axis_X.getPosition();
			endPosition_Spindle = teensyStep_Spindle.getPosition();
			Serial.print("Spindle:");
			Serial.println(endPosition_Spindle);
			Serial.print("Axis:");
			Serial.println(endPosition_Axis);
#endif // DEBUG
#ifdef SHOW_POSITION //Controls must be added to Nextion pages
			if (configMain.trackPositions == 1)
			{
				endPosition_Axis = teensyStep_Axis_X.getPosition();
				endPosition_Spindle = teensyStep_Spindle.getPosition();
				degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
				distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_X);
				Serial3.print("pageRecAxialX.t1.txt=");
				Serial3.write(0x22);
				Serial3.print(degrees_Spindle);
				Serial3.write(0x22);
				Serial3.write(0xff);
				Serial3.write(0xff);
				Serial3.write(0xff);
				delay(10);
				Serial3.print("pageRecAxialX.t2.txt=");
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
				case 93: // - ] 
				case 99: // - c
				case 109: // - m
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

	endPosition_Axis = teensyStep_Axis_X.getPosition();
	endPosition_Spindle = teensyStep_Spindle.getPosition();
#ifdef SHOW_POSITION //Controls must be added to Nextion pages
	degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
	distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_X);
	Serial3.print("pageRecAxialX.t1.txt=");
	Serial3.write(0x22);
	Serial3.print(degrees_Spindle);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(10);
	Serial3.print("pageRecAxialX.t2.txt=");
	Serial3.write(0x22);
	Serial3.print(distance_Axis);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
#endif //SHOW_POSITION
	// Update Nextion
	Serial3.print("pageRecAxialX.bt3.pco=59391");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRecAxialX.va0.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRecAxialX.bt3.val=0");// The first one may be ignored by Nextion, so resend.
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRecAxialX.bt2.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.print("pageRecAxialX.bt1.val=0");
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
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
	////Serialxx1.println(text);
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
int SerialRead(int serialId)
{
	int incomingData = 0;
	if (serialId == 0)
	{
		incomingData = Serial.read();
	}
	else
	{
		incomingData = Serial3.read();
	}

	///*switch (serialId)
	//{
	//	case 0:
	//	{
	//		incomingData = Serial.read();
	//		break;
	//	}
	//	case 1:
	//	{
	//		incomingData = Serialxx1.read();
	//		break;
	//	}
	//	case 3:
	//	{
	//		incomingData = Serial3.read();
	//		break;
	//	}
	//}*/

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
/// Get a char from USB serial input
/// </summary>
/// <returns>char from USB serial input</returns>
void GetIndex2FilenameFromSerial1()
{
	const int bufferLength = 20;
	char input = 0;
	int i;
	for (i = 0; i <= bufferLength; i++)
	{
		input = Serial3.read();

		// Use 255 as end chars
		if (input == 255)
		{
			EEPROM.write(eePromAddress_Filename + i, 0);
			break;
		}
		EEPROM.write(eePromAddress_Filename + i, input);
	}

	EEPROM.put(eePromAddress_Filename_Length, i);
#ifdef DEBUG
	Serial.print("Filename:");
	Serial.println(input);
#endif // DEBUG
}

/// <summary>
/// GetDataFromSD
/// </summary>
/// <comment>
/// Get a value from file on SD card
/// </comment>
/// <param name="lineNumber">Line containing data</param>
/// <returns>Double value</returns>
double GetDataFromSD(int lineNumber)
{
	String newSizeString = "";
	int currentLineNumber = 0;
	int filename_Length;

	// Get filename from EEPROM
	EEPROM.get(eePromAddress_Filename_Length, filename_Length);
	char arChar[20];
	int i;
	for (i = 0; i <= filename_Length; i++)
	{
		arChar[i] = EEPROM.read(i + eePromAddress_Filename);
	}
	arChar[i++] = 0;

	// Open file
	File sdFile = SD.open(arChar, FILE_READ);

	delay(100);
	while (sdFile.available())
	{
		newSizeString = sdFile.readStringUntil('\r');
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



// ================================================================================= 
// RosePatternSpindle Methods
// ================================================================================= 



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
	float spindleAngleZ = teensyStep_Axis_Z.getPosition();// *(TWO_PI / spindleStepsPerRev); //convert steps to angle
	float slidePositionZ = teensyStep_Spindle.getPosition();
	float slideTargetZ = slideFunc_Spindle_Z(spindleAngleZ);

	float newSpeedZ = (slideTargetZ - slidePositionZ) / dtRose; // speed to reach target in given delta t (neglecting acceleration)
	float speedFacZ = newSpeedZ / newMaxSpd_RoseSpindle;              // transform in relative factor (-1.0 .. 1.0)

	controllerRose_Axis.overrideSpeed(speedFacZ);             // set new speed
}

void DoRosePattern_SpindleZ()
{
	IntervalTimer tickTimerZ;
	RotateControl controllerRose;
	bool runPattern = true;

	kRatio = (float)configRose.n / configRose.d;
	spindleStepsPerRev = configMain.gearRatio_Spindle * configMain.microsteps_Spindle * configMain.steps360_Spindle;

	slideStepsAmplitude = ((configRose.amplitude_Axis_Z / (configMain.distancePerRev_AxisZ)) * (configMain.steps360_Axis_Z * configMain.microsteps_Axis_Z));

	spindleStepsAmplitude = (configRose.amplitude_Spindle / 360) * spindleStepsPerRev;

	newMaxSpd_RoseSpindle = configRose.maxSpd_Spindle * configRose.speedPercent_Spindle * .01;
	teensyStep_Spindle
		.setMaxSpeed(newMaxSpd_RoseSpindle)
		.setAcceleration(configRose.accel_Spindle)
		.setPosition(0);

	newMaxSpd_RoseAxis = configRose.maxSpd_Axis_Z * configRose.speedPercent_Axis * .01;
	teensyStep_Axis_Z
		.setMaxSpeed(newMaxSpd_RoseAxis)
		.setAcceleration(configRose.accel_Axis_Z)
		.setPosition(0);
		//.setPosition(slideFunc_Axis_Z(0)); // set start position of counter

		// Enable steppers
	digitalWrite(PIN_SPINDLE_ENABLE, LOW);
	digitalWrite(PIN_AXIS_Z_ENABLE, LOW);

	controllerRose_Axis.rotateAsync(teensyStep_Axis_Z);
	//controllerRose_Axis.overrideSpeed(0); // start with stopped slide
	controllerRose.rotateAsync(teensyStep_Spindle); // let the spindle run with constant speed

	tickZ();

	// use a timer to periodically calculate new targets for the slide
	tickTimerZ.priority(128); // lowest priority, potentially long caclulations need to be interruptable by TeensyStep
	tickTimerZ.begin(tickZ, recalcPeriod);

	while (runPattern)
	{
		digitalWriteFast(LED_BUILTIN, !digitalReadFast(LED_BUILTIN));

#ifdef SHOW_POSITION //Controls must be added to Nextion pages
		if (configMain.trackPositions == 1)
		{
			endPosition_Axis = teensyStep_Axis_Z.getPosition();
			endPosition_Spindle = teensyStep_Spindle.getPosition();

			// print current values of spindle angle [rad] and slide position [steps]
			//float phiZ = endPosition_Spindle * (TWO_PI / spindleStepsPerRev);
			//Serial.printf("%f\t%d\n", phiZ, endPosition_Axis);
			degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
			distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);
			Serial3.print("pageRose.t12.txt=");
			Serial3.write(0x22);
			Serial3.print(degrees_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(5);
			Serial3.print("pageRose.t12.txt=");
			Serial3.write(0x22);
			Serial3.print(degrees_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(5);
			Serial3.print("pageRose.t13.txt=");
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
			case 109:
			{
				runPattern = false;
				controllerRose.stopAsync();
				controllerRose_Axis.stopAsync();
				break;
			}
			}
		}
	}

	endPosition_Spindle = teensyStep_Spindle.getPosition();
	endPosition_Axis = teensyStep_Axis_Z.getPosition();
}

// ================================================================================= 
// RosePatternZ Methods
// ================================================================================= 

/// <summary>
/// Return Spindle and/or axes to start position
/// </summary>
/// <comment>
/// </comment>
/// <returns></returns>
void ReturnToStartPosition(int selection)
{
	int32_t target_Spindle = endPosition_Spindle % spindleStepsPerRev * (-1);
	StepControl stepController;
	
	teensyStep_Spindle
		.setMaxSpeed(configMain.maxSpd_Return_Spindle)
		.setAcceleration(configMain.accel_Return_Spindle)
		.setTargetRel(target_Spindle);
	teensyStep_Axis_Z
		.setMaxSpeed(configMain.maxSpd_Return_Axis)
		.setAcceleration(configMain.accel_Return_Axis)
		.setTargetAbs(initialPosition_Axis);
	teensyStep_Axis_X
		.setMaxSpeed(configMain.maxSpd_Return_Axis)
		.setAcceleration(configMain.accel_Return_Axis)
		.setTargetAbs(initialPosition_Axis);

	stepControllerI.moveAsync(teensyStep_Spindle);
	switch (selection)
	{
		case 0:
		case 2:
		{
			stepController.moveAsync(teensyStep_Axis_Z);
			break;
		}
		case 1:
		case 3:
		{
			stepController.moveAsync(teensyStep_Axis_X);
			break;
		}
	}

	while ((stepController.isRunning()) || (stepControllerI.isRunning()))
	{
		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
				case 93: // - ] 
				case 99: // - c
				case 109: // - m
				{
					stepController.stop();
					stepControllerI.stop();
					break;
				}
			}
		}

		delay(10);
	}

	endPosition_Spindle = teensyStep_Spindle.getPosition();
	switch (selection)
	{
		case 0:
		case 2:
		{
			endPosition_Axis = teensyStep_Axis_Z.getPosition();
			break;
		}
		case 1:
		case 3:
		{
			endPosition_Axis = teensyStep_Axis_X.getPosition();
			break;
		}
	}

	switch (pageCallerId)
	{
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
			Serial3.print("pageSync.t1.txt=");
			Serial3.write(0x22);
			Serial3.print(endPosition_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(10);
			Serial3.print("pageSync.t2.txt=");
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
			Serial3.print("pageSyncX.t1.txt=");
			Serial3.write(0x22);
			Serial3.print(endPosition_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(10);
			Serial3.print("pageSyncX.t2.txt=");
			Serial3.write(0x22);
			Serial3.print(endPosition_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			break;
		}
		case PAGE_RECI_AXIAL_Z:
		{
			Serial3.print("pageRecAxialZ.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageRecAxialZ.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageRecAxialZ.va0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageRecAxialZ.bt0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageRecAxialZ.t1.txt=");
			Serial3.write(0x22);
			Serial3.print(endPosition_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(10);
			Serial3.print("pageRecAxialZ.t2.txt=");
			Serial3.write(0x22);
			Serial3.print(endPosition_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			break;
		}
		case PAGE_RECI_RADIAL_Z:
		{
			Serial3.print("pageRecRadialZ.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageRecRadialZ.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageRecRadialZ.va0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageRecRadialZ.bt0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageRecRadialZ.t1.txt=");
			Serial3.write(0x22);
			Serial3.print(endPosition_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(10);
			Serial3.print("pageRecRadialZ.t2.txt=");
			Serial3.write(0x22);
			Serial3.print(endPosition_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			break;
		}
		case PAGE_RECI_AXIAL_X:
		{
			Serial3.print("pageRecAxialX.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageRecAxialX.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageRecAxialX.va0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageRecAxialX.bt0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageRecAxialX.t1.txt=");
			Serial3.write(0x22);
			Serial3.print(endPosition_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(10);
			Serial3.print("pageRecAxialX.t2.txt=");
			Serial3.write(0x22);
			Serial3.print(endPosition_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			break;
		}
		case PAGE_RECI_RADIAL_X:
		{
			Serial3.print("pageRecRadialX.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageRecRadialX.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageRecRadialX.va0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageRecRadialX.bt0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageRecRadialX.t1.txt=");
			Serial3.write(0x22);
			Serial3.print(endPosition_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(10);
			Serial3.print("pageRecRadialX.t2.txt=");
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
			Serial3.print("pageIndex1.t1.txt=");
			Serial3.write(0x22);
			Serial3.print(endPosition_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			break;
		}
		case PAGE_INDEX2:
		{
			Serial3.print("pageIndex2.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageIndex2.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageIndex2.va0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageIndex2.bt0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.print("pageIndex2.t1.txt=");
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
			Serial3.print("pageMoveZ.t2.txt=");
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
			Serial3.print("pageMoveX.t2.txt=");
			Serial3.write(0x22);
			Serial3.print(endPosition_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			break;
		}
		case PAGE_ROSE:
		{
			Serial3.print("pageRose.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(50);
			Serial3.print("pageRose.bt1.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(50);
			Serial3.print("pageRose.va0.val=0");
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(50);
			Serial3.print("pageRose.t12.txt=");
			Serial3.write(0x22);
			Serial3.print(endPosition_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(50);
			Serial3.print("pageRose.t13.txt=");
			Serial3.write(0x22);
			Serial3.print(static_cast<int>(endPosition_Axis-initialPosition_Axis));
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(50);
			initialPosition_Axis = 0;
			break;
		}

		}

}


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
	float spindleAngleZ = teensyStep_Spindle.getPosition() * (TWO_PI / spindleStepsPerRev); //convert steps to angle
	float slidePositionZ = teensyStep_Axis_Z.getPosition();

	float phi = fmodf(spindleAngleZ * kRatio, TWO_PI);
	float slideTargetZ = slideStepsAmplitude * cosf(phi);


	//float slideTargetZ = slideFunc_Axis_Z(spindleAngleZ);



	float newSpeedZ = (slideTargetZ - slidePositionZ) / dtRose; // speed to reach target in given delta t (neglecting acceleration)
	float speedFacZ = newSpeedZ / newMaxSpd_RoseAxis;              // transform in relative factor (-1.0 .. 1.0)

	controllerRose_Axis.overrideSpeed(speedFacZ);             // set new speed
}

void DoRosePattern_Z()
{
	IntervalTimer tickTimerZ;
	RotateControl controllerRose;
	bool runPattern = true;

	kRatio = (float)configRose.n / configRose.d;
	spindleStepsPerRev = configMain.gearRatio_Spindle * configMain.microsteps_Spindle * configMain.steps360_Spindle;
	slideStepsAmplitude = ((configRose.amplitude_Axis_Z / (configMain.distancePerRev_AxisZ)) * configMain.steps360_Axis_Z * configMain.microsteps_Axis_Z)/2;  // Amplitude is normally measured from the middle to the top

	newMaxSpd_RoseSpindle = configRose.maxSpd_Spindle * configRose.speedPercent_Spindle * .01;
	teensyStep_Spindle
		.setMaxSpeed(newMaxSpd_RoseSpindle)
		.setAcceleration(configRose.accel_Spindle)
		.setPosition(0);

	initialPosition_Axis = static_cast<int>(slideFunc_Axis_Z(0));
	newMaxSpd_RoseAxis = configRose.maxSpd_Axis_Z * configRose.speedPercent_Axis * .01;
	teensyStep_Axis_Z
		.setMaxSpeed(newMaxSpd_RoseAxis)
		.setAcceleration(configRose.accel_Axis_Z)
		.setPosition(initialPosition_Axis); // set start position of counter

		// Enable steppers
	digitalWrite(PIN_SPINDLE_ENABLE, LOW);
	digitalWrite(PIN_AXIS_Z_ENABLE, LOW);

	controllerRose_Axis.rotateAsync(teensyStep_Axis_Z);
	//controllerRose_Axis.overrideSpeed(0); // start with stopped slide
	controllerRose.rotateAsync(teensyStep_Spindle); // let the spindle run with constant speed

	tickZ();

	// use a timer to periodically calculate new targets for the slide
	tickTimerZ.priority(128); // lowest priority, potentially long caclulations need to be interruptable by TeensyStep
	tickTimerZ.begin(tickZ, recalcPeriod);

	while (runPattern)
	{
		digitalWriteFast(LED_BUILTIN, !digitalReadFast(LED_BUILTIN));
#ifdef SHOW_POSITION //Controls must be added to Nextion pages
		endPosition_Axis = teensyStep_Axis_Z.getPosition();
		endPosition_Spindle = teensyStep_Spindle.getPosition();

		// print current values of spindle angle [rad] and slide position [steps]
		//float phiZ = endPosition_Spindle * (TWO_PI / spindleStepsPerRev);
		//Serial.printf("%f\t%d\n", phiZ, endPosition_Axis);
		degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
		distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);
		Serial3.print("pageRose.t12.txt=");
		Serial3.write(0x22);
		Serial3.print(degrees_Spindle);
		Serial3.write(0x22);
		Serial3.write(0xff);
		Serial3.write(0xff);
		Serial3.write(0xff);
		delay(10);
		Serial3.print("pageRose.t13.txt=");
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
			case 109:
			{
				runPattern = false;
				controllerRose.stopAsync();
				controllerRose_Axis.stopAsync();
				break;
			}
			}
		}
	}
	endPosition_Spindle = teensyStep_Spindle.getPosition();
	endPosition_Axis = teensyStep_Axis_Z.getPosition();
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
	float spindleAngleX = teensyStep_Spindle.getPosition() * (TWO_PI / spindleStepsPerRev); //convert steps to angle
	float slidePositionX = teensyStep_Axis_X.getPosition();
	float slideTargetX = slideFunc_Axis_X(spindleAngleX);

	float newSpeedX = (slideTargetX - slidePositionX) / dtRose; // speed to reach target in given delta t (neglecting acceleration)
	float speedFacX = newSpeedX / newMaxSpd_RoseAxis;              // transform in relative factor (-1.0 .. 1.0)

	controllerRose_Axis.overrideSpeed(speedFacX);             // set new speed
}
void DoRosePattern_X()
{
	IntervalTimer tickTimerX;
	RotateControl controllerRose_Spindle;
	bool runPatternX = true;

	kRatio = (float)configRose.n / configRose.d;
	spindleStepsPerRev = configMain.gearRatio_Spindle * configMain.microsteps_Spindle * configMain.steps360_Spindle;
	slideStepsAmplitude = ((configRose.amplitude_Axis_X / (configMain.distancePerRev_AxisX)) * configMain.steps360_Axis_X * configMain.microsteps_Axis_X)/2;

	newMaxSpd_RoseSpindle = configRose.maxSpd_Spindle * configRose.speedPercent_Spindle * .01;
	teensyStep_Spindle
		.setMaxSpeed(newMaxSpd_RoseSpindle)
		.setAcceleration(configRose.accel_Spindle)
		.setPosition(0);

	newMaxSpd_RoseAxis = configRose.maxSpd_Axis_X * configRose.speedPercent_Axis * .01;
	teensyStep_Axis_X
		.setMaxSpeed(newMaxSpd_RoseAxis)
		.setAcceleration(configRose.accel_Axis_X)
		.setPosition(slideFunc_Axis_X(0)); // set start position of counter

		// Enable steppers
	digitalWrite(PIN_SPINDLE_ENABLE, LOW);
	digitalWrite(PIN_AXIS_X_ENABLE, LOW);

	controllerRose_Axis.rotateAsync(teensyStep_Axis_X);
	//controllerRose_Axis.overrideSpeed(0); // start with stopped slide
	controllerRose_Spindle.rotateAsync(teensyStep_Spindle); // let the spindle run with constant speed

	tickX();

	// use a timer to periodically calculate new targets for the slide
	tickTimerX.priority(128); // lowest priority, potentially long caclulations need to be interruptable by TeensyStep
	tickTimerX.begin(tickX, recalcPeriod);

	while (runPatternX)
	{
		digitalWriteFast(LED_BUILTIN, !digitalReadFast(LED_BUILTIN));
#ifdef SHOW_POSITION //Controls must be added to Nextion pages
		endPosition_Axis = teensyStep_Axis_X.getPosition();
		endPosition_Spindle = teensyStep_Spindle.getPosition();

		// print current values of spindle angle [rad] and slide position [steps]
		//float phiZ = endPosition_Spindle * (TWO_PI / spindleStepsPerRev);
		//Serial.printf("%f\t%d\n", phiZ, endPosition_Axis);
		degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
		distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_X);
		Serial3.print("pageRose.t12.txt=");
		Serial3.write(0x22);
		Serial3.print(degrees_Spindle);
		Serial3.write(0x22);
		Serial3.write(0xff);
		Serial3.write(0xff);
		Serial3.write(0xff);
		delay(10);
		Serial3.print("pageRose.t13.txt=");
		Serial3.write(0x22);
		Serial3.print(distance_Axis);
		Serial3.write(0x22);
		Serial3.write(0xff);
		Serial3.write(0xff);
		Serial3.write(0xff);
#endif // Show Position
		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
			case 109:
			{
				runPatternX = false;
				controllerRose_Spindle.stopAsync();
				controllerRose_Axis.stopAsync();
				break;
			}
			}
		}
	}
	endPosition_Spindle = teensyStep_Spindle.getPosition();
	endPosition_Axis = teensyStep_Axis_X.getPosition();
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

	//pageCallerId = GetSerial1Float();
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
		case PAGE_SPZ:
		{
			configSteppers eePageSpZ;
			EEPROM.get(eePromAddress_SpZ, eePageSpZ);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("Main-MaxSpeed:");
			Serial3.print(eePageSpZ.maxSpd_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("Main-Accel:");
			Serial3.print(eePageSpZ.accel_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("Z Axis-MaxSpeed:");
			Serial3.print(eePageSpZ.maxSpd_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("Z Axis-Accel:");
			Serial3.print(eePageSpZ.accel_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			break;
		}
		case PAGE_SPX:
		{
			configSteppers eePageSpX;
			EEPROM.get(eePromAddress_SpX, eePageSpX);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("Main-MaxSpeed:");
			Serial3.print(eePageSpX.maxSpd_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("Main-Accel:");
			Serial3.print(eePageSpX.accel_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("X Axis-MaxSpeed:");
			Serial3.print(eePageSpX.maxSpd_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("X Axis-Accel:");
			Serial3.print(eePageSpX.accel_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			break;
		}
		case PAGE_SPB:
		{
			configSteppers eePageSpB;
			EEPROM.get(eePromAddress_SpB, eePageSpB);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("Main-MaxSpeed:");
			Serial3.print(eePageSpB.maxSpd_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("Main-Accel:");
			Serial3.print(eePageSpB.accel_Spindle);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("B Axis-MaxSpeed:");
			Serial3.print(eePageSpB.maxSpd_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("B Axis-Accel:");
			Serial3.print(eePageSpB.accel_Axis);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
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
		case PAGE_RECI_AXIAL_Z:
		case PAGE_RECI_RADIAL_Z:
		case PAGE_RECI_AXIAL_X:
		case PAGE_RECI_RADIAL_X:
		{
			configPageReci eePageReciRadialZ;
			EEPROM.get(eePromAddress_Reci, eePageReciRadialZ);
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
			break;
		}
		case PAGE_SP:
		{
			configStepper eePageSp;
			EEPROM.get(eePromAddress_Sp, eePageSp);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("Spindle-MaxSpeed:");
			Serial3.print(eePageSp.maxSpd);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("Spindle-Accel:");
			Serial3.print(eePageSp.accel);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			break;
		}
		case PAGE_INDEX1:
		{
			configStepper eePageIndex1;
			EEPROM.get(eePromAddress_Index1, eePageIndex1);
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
		case PAGE_INDEX2:
		{
			configStepper eePageIndex2;
			EEPROM.get(eePromAddress_Index2, eePageIndex2);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("Index2-MaxSpeed:");
			Serial3.print(eePageIndex2.maxSpd);
			Serial3.write(0x22);
			Serial3.write(0xff);
			Serial3.write(0xff);
			Serial3.write(0xff);
			delay(2000);
			Serial3.print("pageConfig.t19.txt=");
			Serial3.write(0x22);
			Serial3.print("Index2-Accel:");
			Serial3.print(eePageIndex2.accel);
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
		case PAGE_ROSE:
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

		
	}		 
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
	// pageConfig
	Serial3.print("pageConfig.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Setup:");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(2000);

	// pageSetup
	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Spindle-Microsteps:");
	Serial3.print(eePromConfig.microsteps_Spindle);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(2000);

	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Spindle-Steps360:");
	Serial3.print(eePromConfig.steps360_Spindle);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(2000);
	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Spindle-Gear Ratio:");
	Serial3.print(eePromConfig.gearRatio_Spindle);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(2000);
	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Spindle-Enable:");
	Serial3.print(eePromConfig.enable_Spindle? "Low":"High");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(2000);

	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Z Axis-Microsteps:");
	Serial3.print(eePromConfig.microsteps_Axis_Z);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(2000);
	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Z Axis-Steps/360:");
	Serial3.print(eePromConfig.steps360_Axis_Z);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(2000);
	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Z Axis-Dist/360:");
	Serial3.print(eePromConfig.distancePerRev_AxisZ);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(2000);
	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Z Axis-Enable:");
	Serial3.print(eePromConfig.polarity_Axis_Z ? "Low" : "High");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(2000);

#ifndef TWO_AXES_V2
	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("X Axis-Microsteps:");
	Serial3.print(eePromConfig.microsteps_Axis_X);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(2000);
	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("X Axis-Steps/360:");
	Serial3.print(eePromConfig.steps360_Axis_X);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(2000);
	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("X Axis-Dist/360:");
	Serial3.print(eePromConfig.distancePerRev_AxisX);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(2000);
	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("X Axis-Enable:");
	Serial3.print(eePromConfig.polarity_Axis_X ? "Low" : "High");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(2000);
#endif // TWO_AXES_V2
#ifdef FOUR_AXES
	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("B Axis-Microsteps:");
	Serial3.print(eePromConfig.microsteps_Axis_B);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(2000);

	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("B Axis-Steps360:");
	Serial3.print(eePromConfig.steps360_Axis_B);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(2000);
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
	Serial3.print(eePromConfig.polarity_Axis_B ? "Low" : "High");
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(2000);
#endif // FOUR_AXES
	
	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Limit Min Z:");
	Serial3.print(configMain.limit_Min_Z);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(2000);
	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Limit Max Z:");
	Serial3.print(configMain.limit_Max_Z);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(2000);
#ifndef TWO_AXES_V2
	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Limit Min X:");
	Serial3.print(configMain.limit_Min_X);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(2000);

	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Limit Max X:");
	Serial3.print(configMain.limit_Max_X);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(2000);
#endif
#ifdef FOUR_AXES
	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Limit Min B:");
	Serial3.print(configMain.limit_Min_B);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(2000);
	Serial3.print("pageSetup.t19.txt=");
	Serial3.write(0x22);
	Serial3.print("Limit Max B:");
	Serial3.print(configMain.limit_Max_B);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(2000);
#endif

}

void TestEEPROMRose()
{
	////for (int i = 0; i < 3; i++) // Verify Teensy is operational
	////{
	////	digitalWrite(LED_BUILTIN, HIGH);
	////	delay(50);
	////	digitalWrite(LED_BUILTIN, LOW);
	////	delay(300);
	////}

	configPageRose eePageRose;
	EEPROM.get(eePromAddress_Rose, eePageRose);
	////Serial3.print("pageRose.t8.txt=");
	////Serial3.write(0x22);
	////Serial3.print(spindleStepsPerRev);
	////Serial3.write(0x22);
	////Serial3.write(0xff);
	////Serial3.write(0xff);
	////Serial3.write(0xff);
	////delay(50);
	////Serial3.print("pageRose.t8.txt=");
	////Serial3.write(0x22);
	////Serial3.print(spindleStepsPerRev);
	////Serial3.write(0x22);
	////Serial3.write(0xff);
	////Serial3.write(0xff);
	////Serial3.write(0xff);
	////delay(50);
	//Serial3.print("pageRose.t9.txt=");
	//Serial3.write(0x22);
	//Serial3.print(eePageRose.maxSpd_Spindle);
	//Serial3.write(0x22);
	//Serial3.write(0xff);
	//Serial3.write(0xff);
	//Serial3.write(0xff);
	//delay(50);
	Serial3.print("pageRose.t10.txt=");
	Serial3.write(0x22);
	Serial3.print(eePageRose.accel_Spindle);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(50);
	Serial3.print("pageRose.t10.txt=");
	Serial3.write(0x22);
	Serial3.print(eePageRose.accel_Spindle);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(50);
	////Serial3.print("pageRose.t11.txt=");
	////Serial3.write(0x22);
	////Serial3.print(eePageRose.rpm);
	////Serial3.write(0x22);
	////Serial3.write(0xff);
	////Serial3.write(0xff);
	////Serial3.write(0xff);
	////delay(50);
	Serial3.print("pageRose.t3.txt=");
	Serial3.write(0x22);
	Serial3.print(eePageRose.maxSpd_Axis_Z);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(50);
	Serial3.print("pageRose.t4.txt=");
	Serial3.write(0x22);
	Serial3.print(eePageRose.accel_Axis_Z);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(50);
	Serial3.print("pageRose.t5.txt=");
	Serial3.write(0x22);
	Serial3.print(eePageRose.amplitude_Axis_Z);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(50);
	Serial3.print("pageRose.t1.txt=");
	Serial3.write(0x22);
	Serial3.print(configRose.n);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(50);
	Serial3.print("pageRose.t2.txt=");
	Serial3.write(0x22);
	Serial3.print(configRose.d);
	Serial3.write(0x22);
	Serial3.write(0xff);
	Serial3.write(0xff);
	Serial3.write(0xff);
	delay(50);
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