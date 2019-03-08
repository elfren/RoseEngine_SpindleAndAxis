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
			retVal = GetSerialFloatA(Serial);
			break;
		}
		case 3:
		{

			retVal = GetSerialFloatA(Serial1);
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
/// Get a float from Bluetooth serial input (Serial1)
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
		in[i] = Serial1.read();

		// Use 255 as end chars
		if (in[i] == 255)
		{
			endFound = i;
			while (Serial1.available() > 0)
			{
				Serial1.read();
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
/// Get a float from Bluetooth serial input (Serial1)
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
	inputHex = Serial1.read();

	int newInteger = (int)inputHex ;
	while (Serial1.available() > 0)
	{
		// Discard remaining serial chars
		Serial1.read();
	}
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
	inputHex = Serial1.read();

	int newInteger = (int)inputHex;
	//while (Serial1.available() > 0)
	//{
	//	// Discard remaining serial chars
	//	Serial1.read();
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

	return inputNumber;
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
			nextSpeed = configMain.speedPercent_MoveZ * configMoveZ.maxSpd_Axis_Z * .01;
			teensyStep_Axis_Z
				.setMaxSpeed(nextSpeed)
				.setAcceleration(configMoveZ.accel_Axis_Z)
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
			nextSpeed = configMain.speedPercent_MoveX * configMoveX.maxSpd_Axis_X * .01;
			teensyStep_Axis_X
				.setMaxSpeed(nextSpeed)
				.setAcceleration(configMoveX.accel_Axis_X)
				.setTargetRel(stepsToMove * directionAxis);
			digitalWrite(PIN_AXIS_X_ENABLE, LOW);
			stepControllerI.moveAsync(teensyStep_Axis_X);
			break;
		}
#endif // FOUR_AXES
	}

	while (stepControllerI.isRunning())
	{

		
		switch (axisId)
		{
			case ID_AXIS_Z:
			{
				endPosition_Axis = teensyStep_Axis_Z.getPosition();
				Serial1.print("pageMoveZ.t2.txt=");
				Serial1.write(0x22);
				Serial1.print(endPosition_Axis);
				Serial1.write(0x22);
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);
				delay(5);
				Serial1.print("pageMoveZ.t2.txt=");
				Serial1.write(0x22);
				Serial1.print(endPosition_Axis);
				Serial1.write(0x22);
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);
				break;
			}
			case ID_AXIS_X:
			{
				endPosition_Axis = teensyStep_Axis_X.getPosition();
				Serial1.print("pageMoveX.t2.txt=");
				Serial1.write(0x22);
				Serial1.print(endPosition_Axis);
				Serial1.write(0x22);
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);
				delay(5);
				Serial1.print("pageMoveX.t2.txt=");
				Serial1.write(0x22);
				Serial1.print(endPosition_Axis);
				Serial1.write(0x22);
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);
				break;
			}
		}



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
			Serial1.print("pageMoveZ.bt2.pco=59391");// The first one may be ignored by Nextion
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageMoveZ.va0.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageMoveZ.bt1.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageMoveZ.bt2.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageMoveZ.bt3.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageMoveZ.t6.txt=");
			Serial1.write(0x22);
			Serial1.print(distanceTotal_MoveZ);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);

			endPosition_Axis = teensyStep_Axis_Z.getPosition();
			Serial1.print("pageMoveZ.t2.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Axis);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(5);
			Serial1.print("pageMoveZ.t2.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Axis);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(5);
			endPosition_Spindle = 0;
			break;
		}
#endif // FOUR_AXES
		case ID_AXIS_X:
		{
			// Update Index flag on Nextion. 
			Serial1.print("pageMoveX.bt2.pco=59391");// The first one may be ignored by Nextion
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageMoveX.va0.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageMoveX.bt1.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageMoveX.bt2.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageMoveX.bt3.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageMoveX.t6.txt=");
			Serial1.write(0x22);
			Serial1.print(distanceTotal_MoveZ);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);

			endPosition_Axis = teensyStep_Axis_X.getPosition();
			Serial1.print("pageMoveX.t2.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Axis);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(5);
			Serial1.print("pageMoveX.t2.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Axis);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(5);
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
		//endPosition_Spindle = stepper.currentPosition();
		//Serial1.print("pageSpindle2.t1.txt=");
		//Serial1.write(0x22);
		//Serial1.print(endPosition_Spindle);
		//Serial1.write(0x22);
		//Serial1.write(0xff);
		//Serial1.write(0xff);
		//Serial1.write(0xff);
		//delay(5);
		//Serial1.print("pageSpindle2.t1.txt=");
		//Serial1.write(0x22);
		//Serial1.print(endPosition_Spindle);
		//Serial1.write(0x22);
		//Serial1.write(0xff);
		//Serial1.write(0xff);
		//Serial1.write(0xff);

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
	Serial1.print("pageSpindle2.t1.txt=");
	Serial1.write(0x22);
	Serial1.print(endPosition_Spindle);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(5);
	Serial1.print("pageSpindle2.t1.txt=");
	Serial1.write(0x22);
	Serial1.print(endPosition_Spindle);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);

	return 0;
}

/// <summary>
/// Run steppers
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
	// Axis Stop flag (Used to reset Nextion when limit switch is hit.)
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

	// Configure Axis
	float nextSpeed_Axis = configMain.speedPercent_Axis_Z * configSpZ.maxSpd_Axis_Z * .01;
	accelStep_Axis_Z.setMaxSpeed(nextSpeed_Axis);
	accelStep_Axis_Z.setAcceleration(configSpZ.accel_Axis_Z);
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
				case 90: // Z - Axis Clockwise
				{
					configMain.speedPercent_Axis_Z = GetSerialInteger();
					EEPROM.put(eePromAddress_Setup, configMain);
					nextSpeed_Axis = configMain.speedPercent_Axis_Z  * configSpZ.maxSpd_Axis_Z * .01;
					accelStep_Axis_Z.setMaxSpeed(nextSpeed_Axis);
					accelStep_Axis_Z.setCurrentPosition(0);
					accelStep_Axis_Z.moveTo(1000000000);
					stepper_Axis_Z_Go = true;
					break;
				}
				case 91: // [ - Axis counter clockwise
				{
					nextSpeed_Axis = configMain.speedPercent_Axis_Z  * configSpZ.maxSpd_Axis_Z * .01;
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
				case 97: // a - Axis speed
				{
					configMain.speedPercent_Axis_Z = GetSerialInteger();
					EEPROM.put(eePromAddress_Setup, configMain);
					nextSpeed_Axis = configMain.speedPercent_Axis_Z * configSpZ.maxSpd_Axis_Z * .01;
					accelStep_Axis_Z.setMaxSpeed(nextSpeed_Axis);
					break;
				}
				case 99: // c - Axis stop
				{
					accelStep_Axis_Z.stop();
					accelStep_Axis_Z.setCurrentPosition(0);
					if (!stepper_Spindle_Go)
					{
						accelStep_Spindle.setCurrentPosition(0);
					}
					stepper_Axis_Z_Go = false;
					break;
				}
				case 107: // k - Spindle Clockwise
				{
					nextSpeed_Spindle = configMain.speedPercentSpindle_SpZ * configSpZ.maxSpd_Spindle * .01;
					accelStep_Spindle.setMaxSpeed(nextSpeed_Spindle);
					accelStep_Spindle.setCurrentPosition(0);
					accelStep_Spindle.moveTo(1000000000);
					stepper_Spindle_Go = true;
					break;
				}
				case 108: // l - Spindle Counterclockwise
				{
					nextSpeed_Spindle = configMain.speedPercentSpindle_SpZ * configSpZ.maxSpd_Spindle * .01;
					accelStep_Spindle.setMaxSpeed(nextSpeed_Spindle);
					accelStep_Spindle.setCurrentPosition(0);
					accelStep_Spindle.moveTo(-1000000000);
					stepper_Spindle_Go = true;
					break;
				}
				case 110: // n - Set Spindle Speed
				{
					configMain.speedPercentSpindle_SpZ = GetSerialInteger();
					float nextSpeed = configMain.speedPercentSpindle_SpZ * configSpZ.maxSpd_Spindle * .01;
					accelStep_Spindle.setMaxSpeed(nextSpeed);
					EEPROM.put(eePromAddress_Setup, configMain);
					break;
				}
			}
		}

		// Limit switches ?
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

			endPosition_Spindle = accelStep_Spindle.currentPosition();
			Serial1.print("pageSpZ.t1.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(5);
			Serial1.print("pageSpZ.t1.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			
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
			endPosition_Axis = accelStep_Axis_Z.currentPosition();
			Serial1.print("pageSpZ.t2.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Axis);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(10);
			Serial1.print("pageSpZ.t2.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Axis);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);

			// Reset stop flag on Nextion
			if (stopFlag)
			{
				Serial1.print("pageSpZ.bt7.val=0");
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.print("pageSpZ.bt5.val=0");
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.print("pageSpZ.bt6.val=0");
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);

				Serial1.print("pageSpZ.va2.val=0");
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);
			}
		}
	}

	stepper_Axis_Z_Go = false;
	stepper_Spindle_Go = false;

	endPosition_Spindle = accelStep_Spindle.currentPosition();
	endPosition_Axis = accelStep_Axis_Z.currentPosition();

	Serial1.print("pageSpZ.t1.txt=");
	Serial1.write(0x22);
	Serial1.print(endPosition_Spindle);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(10);
	Serial1.print("pageSpZ.t2.txt=");
	Serial1.write(0x22);
	Serial1.print(endPosition_Axis);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);

	accelStep_Axis_Z.setCurrentPosition(0);
	accelStep_Spindle.setCurrentPosition(0);

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
	float nextSpeed_Axis = configMain.speedPercent_Axis_X * configSpX.maxSpd_Axis_X * .01;
	accelStep_Axis_X.setMaxSpeed(nextSpeed_Axis);
	accelStep_Axis_X.setAcceleration(configSpX.accel_Axis_X);
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
					nextSpeed_Axis = configMain.speedPercent_Axis_X  * configSpX.maxSpd_Axis_X * .01;
					accelStep_Axis_X.setMaxSpeed(nextSpeed_Axis);
					accelStep_Axis_X.setCurrentPosition(0);
					accelStep_Axis_X.moveTo(1000000000);
					stepper_Axis_X_Go = true;
					break;
				}
				case 165: // ¥ - Sp-X Axis CCW
				{
					nextSpeed_Axis = configMain.speedPercent_Axis_X  * configSpX.maxSpd_Axis_X * .01;
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
					configMain.speedPercent_Axis_X = GetSerialInteger();
					EEPROM.put(eePromAddress_Setup, configMain);
					nextSpeed_Axis = configMain.speedPercent_Axis_X * configSpX.maxSpd_Axis_X * .01;
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
					configMain.speedPercentSpindle_SpX = GetSerialInteger();
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
			endPosition_Spindle = accelStep_Spindle.currentPosition();
			Serial1.print("pageSpX.t1.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(5);
			Serial1.print("pageSpX.t1.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
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
			endPosition_Axis = accelStep_Axis_X.currentPosition();
			Serial1.print("pageSpX.t2.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Axis);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(10);
			Serial1.print("pageSpX.t2.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Axis);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			// Reset stop flag on Nextion
			if (stopFlag)
			{
				Serial1.print("pageSpX.bt7.val=0");
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.print("pageSpX.bt5.val=0");
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.print("pageSpX.bt6.val=0");
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);
				//delay(15);
				Serial1.print("pageSpX.va2.val=0");
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);
			}
		}
	}

	stepper_Axis_X_Go = false;
	stepper_Spindle_Go = false;

	endPosition_Spindle = accelStep_Spindle.currentPosition();
	endPosition_Axis = accelStep_Axis_X.currentPosition();

	Serial1.print("pageSpX.t1.txt=");
	Serial1.write(0x22);
	Serial1.print(endPosition_Spindle);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(10);
	Serial1.print("pageSpX.t2.txt=");
	Serial1.write(0x22);
	Serial1.print(endPosition_Axis);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);

	accelStep_Axis_X.setCurrentPosition(0);
	accelStep_Spindle.setCurrentPosition(0);

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
	float nextSpeed_Axis = configMain.speedPercent_Axis_B * configSpB.maxSpd_Axis_B * .01;
	accelStep_Axis_B.setMaxSpeed(nextSpeed_Axis);
	accelStep_Axis_B.setAcceleration(configSpB.accel_Axis_B);
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
					nextSpeed_Axis = configMain.speedPercent_Axis_B  * configSpB.maxSpd_Axis_B * .01;
					accelStep_Axis_B.setMaxSpeed(nextSpeed_Axis);
					accelStep_Axis_B.setCurrentPosition(0);
					accelStep_Axis_B.moveTo(1000000000);
					stepper_Axis_B_Go = true;
					break;
				}
				case 171: // « - Sp-B Axis CCW
				{
					nextSpeed_Axis = configMain.speedPercent_Axis_B  * configSpB.maxSpd_Axis_B * .01;
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
					configMain.speedPercent_Axis_B = GetSerialInteger();
					EEPROM.put(eePromAddress_Setup, configMain);
					nextSpeed_Axis = configMain.speedPercent_Axis_B * configSpB.maxSpd_Axis_B * .01;
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
					configMain.speedPercentSpindle_SpB = GetSerialInteger();
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

			endPosition_Spindle = accelStep_Spindle.currentPosition();
			Serial1.print("pageSpB.t1.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(5);
			Serial1.print("pageSpB.t1.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
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
			endPosition_Axis = accelStep_Axis_B.currentPosition();
			Serial1.print("pageSpB.t2.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Axis);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(10);
			Serial1.print("pageSpB.t2.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Axis);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);

			// Reset stop flag on Nextion
			if (stopFlag)
			{
				Serial1.print("pageSpB.bt7.val=0");
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.print("pageSpB.bt5.val=0");
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.print("pageSpB.bt6.val=0");
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.print("pageSpB.va2.val=0");
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);
			}
		}
	}

	stepper_Axis_B_Go = false;
	stepper_Spindle_Go = false;

	endPosition_Spindle = accelStep_Spindle.currentPosition();
	endPosition_Axis = accelStep_Axis_B.currentPosition();

	Serial1.print("pageSpB.t1.txt=");
	Serial1.write(0x22);
	Serial1.print(endPosition_Spindle);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(10);
	Serial1.print("pageSpB.t2.txt=");
	Serial1.write(0x22);
	Serial1.print(endPosition_Axis);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
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
	double stepsToMove = configMain.size_Index1;
	teensyStep_Spindle.setPosition(0);

	switch (indexId)
	{
		case 1:
		{
			// Set speed and acceleration
			teensyStep_Spindle
				.setMaxSpeed(configIndex1.maxSpd_Spindle)
				.setAcceleration(configIndex1.accel_Spindle)
				.setTargetRel(stepsToMove * directionSpindle);
			break;
		}
		case 2:
		{
			stepsToMove = configMain.size_Index2;

			// Set speed and acceleration
			teensyStep_Spindle
				.setMaxSpeed(configIndex2.maxSpd_Spindle)
				.setAcceleration(configIndex2.accel_Spindle)
				.setTargetRel(stepsToMove * directionSpindle);
			break;
		}
	}

	digitalWrite(PIN_SPINDLE_ENABLE, LOW);
	stepControllerI.moveAsync(teensyStep_Spindle);

	while (stepControllerI.isRunning())
	{

		endPosition_Spindle = teensyStep_Spindle.getPosition();
		switch (indexId)
		{
			case 1:
			{
				Serial1.print("pageIndex1.t1.txt=");
				break;
			}
			case 2:
			{
				Serial1.print("pageIndex2.t1.txt=");
				break;
			}
		}

		Serial1.write(0x22);
		Serial1.print(endPosition_Spindle);
		Serial1.write(0x22);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		delay(10);


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

	if (indexId == 2)
	{
		// Update Index flag on Nextion. 
		// Update Nextion
		Serial1.print("pageIndex2.bt3.pco=0");// The first one may be ignored by Nextion, so resend.
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.print("pageIndex2.va0.val=0");
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.print("pageIndex2.bt3.val=0");
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.print("pageIndex2.bt2.val=0");
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.print("pageIndex2.bt1.val=0");
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
	}
	else
	{ 
		
		// Update Index flag on Nextion. 
		Serial1.print("pageIndex1.bt3.pco=0");// The first one may be ignored by Nextion, so resend.
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.print("pageIndex1.va0.val=0");
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.print("pageIndex1.bt3.val=0");
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.print("pageIndex1.bt2.val=0");
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.print("pageIndex1.bt1.val=0");
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.print("pageSync.b6.pco=0");
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.print("pageSync.b5.pco=0");
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.print("pageSyncX.b6.pco=0");
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.print("pageSyncX.b5.pco=0");
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
	}

	endPosition_Axis = 0;
	endPosition_Spindle = teensyStep_Spindle.getPosition();
	switch (indexId)
	{
	case 1:
	{
		Serial1.print("pageIndex1.t1.txt=");
		break;
	}
	case 2:
	{
		Serial1.print("pageIndex2.t1.txt=");
		break;
	}
	}

	Serial1.write(0x22);
	Serial1.print(endPosition_Spindle);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(10);
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
		.setMaxSpeed(configSyncZ.maxSpd_Axis_Z * configMain.speedPercentAxis_SyncZ * .01)
		.setAcceleration(configSyncZ.accel_Axis_Z)
		.setTargetRel((configMain.distanceSyncZ / configMain.distancePerRev_AxisZ) * configMain.microsteps_Axis_Z * configMain.steps360_Axis_Z * directionAxis);
	teensyStep_Axis_Z.setPosition(0);

	digitalWrite(PIN_SPINDLE_ENABLE, LOW); // Enable (Uses TeensyStep library)
	digitalWrite(PIN_AXIS_Z_ENABLE, LOW);  // Enable (Uses TeensyStep library)
	
	stepController.moveAsync(teensyStep_Spindle, teensyStep_Axis_Z);

	while (stepController.isRunning())
	{ 
		endPosition_Axis = teensyStep_Axis_Z.getPosition();
		endPosition_Spindle = teensyStep_Spindle.getPosition();
		Serial1.print("pageSync.t1.txt=");
		Serial1.write(0x22);
		Serial1.print(endPosition_Spindle);
		Serial1.write(0x22);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		delay(10);
		Serial1.print("pageSync.t2.txt=");
		Serial1.write(0x22);
		Serial1.print(endPosition_Axis);
		Serial1.write(0x22);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
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

	// Update Nextion
	Serial1.print("pageSync.t1.txt=");
	Serial1.write(0x22);
	Serial1.print(endPosition_Spindle);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(10);
	Serial1.print("pageSync.t2.txt=");
	Serial1.write(0x22);
	Serial1.print(endPosition_Axis);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageSync.bt6.pco=0");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageSync.va0.val=0");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageSync.bt6.val=0");// The first one may be ignored by Nextion, so resend.
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageSync.bt2.val=0");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageSync.bt1.val=0");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
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
		.setMaxSpeed(configSyncX.maxSpd_Axis_X * configMain.speedPercentAxis_SyncX * .01)
		.setAcceleration(configSyncX.accel_Axis_X)
		.setTargetRel((configMain.distanceSyncX / configMain.distancePerRev_AxisX) * configMain.microsteps_Axis_X * configMain.steps360_Axis_X * directionAxis);
	teensyStep_Axis_X.setPosition(0);

	digitalWrite(PIN_SPINDLE_ENABLE, LOW); // Enable (Uses TeensyStep library)
	digitalWrite(PIN_AXIS_X_ENABLE, LOW);  // Enable (Uses TeensyStep library)

	stepController.moveAsync(teensyStep_Spindle, teensyStep_Axis_X);

	while (stepController.isRunning())
	{
		endPosition_Axis = teensyStep_Axis_X.getPosition();
		endPosition_Spindle = teensyStep_Spindle.getPosition();
		Serial1.print("pageSyncX.t1.txt=");
		Serial1.write(0x22);
		Serial1.print(endPosition_Spindle);
		Serial1.write(0x22);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		delay(10);
		Serial1.print("pageSyncX.t2.txt=");
		Serial1.write(0x22);
		Serial1.print(endPosition_Axis);
		Serial1.write(0x22);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
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

	// Update Nextion
	Serial1.print("pageSyncX.t1.txt=");
	Serial1.write(0x22);
	Serial1.print(endPosition_Spindle);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(10);
	Serial1.print("pageSyncX.t2.txt=");
	Serial1.write(0x22);
	Serial1.print(endPosition_Axis);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageSyncX.bt6.pco=0");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageSyncX.va0.val=0");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageSyncX.bt6.val=0");// The first one may be ignored by Nextion, so resend.
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageSyncX.bt2.val=0");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageSyncX.bt1.val=0");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
}

//#endif // TWO_AXES_V2


/// <summary>
/// DoRec1_Spindle
/// </summary>
/// <comment>
/// </comment>
/// <param name="wavDir">Counterclockwise: -1, Clockwise: 1</param>
/// <returns></returns>
void DoRec1_Spindle(int wavDir)
{
	StepControl stepController;

	// Set speed and acceleration
	teensyStep_Spindle
		.setMaxSpeed(configRec1_S.maxSpd_Spindle * configMain.speedPercentSpindle_Rec1S * .01)
		.setAcceleration(configRec1_S.accel_Spindle);
	teensyStep_Spindle.setPosition(0);

	float spindleDegrees = configMain.degrees_Rec1_S / (configMain.waves_Rec1_S * 2);
	int spindleSteps = round((configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) * (spindleDegrees / 360) * wavDir);
	returnSteps_Rec1_S = spindleSteps*configMain.waves_Rec1_S * 2;

	teensyStep_Axis_Z
		.setMaxSpeed(configRec1_S.maxSpd_Axis_Z * configMain.speedPercentAxis_Rec1S * .01)
		.setAcceleration(configRec1_S.accel_Axis_Z);
	teensyStep_Axis_Z.setPosition(0);
	float axisDistance = configMain.amplitude_Rec1_S;
	int axisSteps = round((axisDistance / configMain.distancePerRev_AxisZ) * configMain.microsteps_Axis_Z * configMain.steps360_Axis_Z * wavDir);

	digitalWrite(PIN_SPINDLE_ENABLE, LOW); // Enable
	digitalWrite(PIN_AXIS_Z_ENABLE, LOW);  // Enable
	for (int i = 0; i < (configMain.waves_Rec1_S * 2); i++)
	{
		axisSteps *= -1;

		teensyStep_Spindle.setTargetRel(spindleSteps);
		teensyStep_Axis_Z.setTargetRel(axisSteps);

		stepController.moveAsync(teensyStep_Spindle, teensyStep_Axis_Z);

		while (stepController.isRunning())
		{
			endPosition_Axis = teensyStep_Axis_Z.getPosition();
			endPosition_Spindle = teensyStep_Spindle.getPosition();
			Serial1.print("pageRec1_Sp.t1.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(10);
			Serial1.print("pageRec1_Sp.t2.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Axis);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
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

	// Update Nextion
	Serial1.print("pageRec1_Sp.bt3.pco=0");// The first one may be ignored by Nextion, so resend.
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageRec1_Sp.va0.val=0");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageRec1_Sp.bt3.val=0");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageRec1_Sp.bt2.val=0");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageRec1_Sp.bt1.val=0");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
}

/// <summary>
/// DoRec1_Z
/// </summary>
/// <comment>
/// </comment>
/// <param name="wavDir">In: -1, Out: 1</param>
/// <returns></returns>
void DoRec1_Z(int wavDir)
{
	StepControl stepController;

	// Set speed and acceleration
	teensyStep_Spindle
		.setMaxSpeed(configRec1_Z.maxSpd_Spindle * configMain.speedPercentSpindle_Rec1Z * .01)
		.setAcceleration(configRec1_Z.accel_Spindle);
	teensyStep_Spindle.setPosition(0);

	//int spindleSteps = round((configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) * (configMain.amplitude_Rec1_Z / 360) * wavDir);
	//long axisSteps = round((((configMain.distance_Rec1_Z / (configMain.distancePerRev_AxisZ)) * (configMain.steps360_Axis_Z * configMain.microsteps_Axis_Z))/(configMain.waves_Rec1_Z * 2)) * wavDir);
	int spindleSteps = (configMain.microsteps_Spindle * configMain.steps360_Spindle * configMain.gearRatio_Spindle) * (configMain.amplitude_Rec1_Z / 360) * wavDir;
	long axisSteps = (((configMain.distance_Rec1_Z / (configMain.distancePerRev_AxisZ)) * (configMain.steps360_Axis_Z * configMain.microsteps_Axis_Z)) / (configMain.waves_Rec1_Z * 2)) * wavDir;
	returnSteps_Rec1_Z = axisSteps*configMain.waves_Rec1_Z * 2;

	//long axisReturnSteps = (configMain.distance_Rec1_Z / (configMain.distancePerRev_AxisZ)) * (configMain.steps360_Axis_Z * configMain.microsteps_Axis_Z);
	//long divisor = (configMain.steps360_Axis_Z * configMain.microsteps_Axis_Z) / (configMain.waves_Rec1_Z * 2);
	//Serial1.print("pageRec1_Z.t10.txt=");
	//Serial1.write(0x22);
	//Serial1.print(axisSteps);
	//Serial1.write(0x22);
	//Serial1.write(0xff);
	//Serial1.write(0xff);
	//Serial1.write(0xff);
	//delay(10);
	//Serial1.print("pageRec1_Z.t10.txt=");
	//Serial1.write(0x22);
	//Serial1.print(axisSteps);
	//Serial1.write(0x22);
	//Serial1.write(0xff);
	//Serial1.write(0xff);
	//Serial1.write(0xff);
	//delay(10);
	//Serial1.print("pageRec1_Z.t12.txt=");
	//Serial1.write(0x22);
	//Serial1.print(returnSteps_Rec1_Z);
	//Serial1.write(0x22);
	//Serial1.write(0xff);
	//Serial1.write(0xff);
	//Serial1.write(0xff);
	//delay(10);
	//Serial1.print("pageRec1_Z.t9.txt=");
	//Serial1.write(0x22);
	//Serial1.print(axisReturnSteps);
	//Serial1.write(0x22);
	//Serial1.write(0xff);
	//Serial1.write(0xff);
	//Serial1.write(0xff);
	//delay(10);
	//Serial1.print("pageRec1_Z.t0.txt=");
	//Serial1.write(0x22);
	//Serial1.print(divisor);
	//Serial1.write(0x22);
	//Serial1.write(0xff);
	//Serial1.write(0xff);
	//Serial1.write(0xff);



	teensyStep_Axis_Z
		.setMaxSpeed(configRec1_Z.maxSpd_Axis_Z * configMain.speedPercentAxis_Rec1Z * .01)
		.setAcceleration(configRec1_Z.accel_Axis_Z);
	teensyStep_Axis_Z.setPosition(0);

	digitalWrite(PIN_SPINDLE_ENABLE, LOW); // Enable (Uses TeensyStep library)
	digitalWrite(PIN_AXIS_Z_ENABLE, LOW);  // Enable (Uses TeensyStep library)
	for (int i = 0; i < (configMain.waves_Rec1_Z * 2); i++)
	{
		spindleSteps *= -1;

		teensyStep_Spindle.setTargetRel(spindleSteps);
		teensyStep_Axis_Z.setTargetRel(axisSteps);

		stepController.moveAsync(teensyStep_Spindle, teensyStep_Axis_Z);
		delay(10);

		while (stepController.isRunning())
		{
			endPosition_Axis = teensyStep_Axis_Z.getPosition();
			endPosition_Spindle = teensyStep_Spindle.getPosition();
			Serial1.print("pageRec1_Z.t1.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(10);
			Serial1.print("pageRec1_Z.t2.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Axis);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);

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

	endPosition_Axis = teensyStep_Axis_Z.getPosition();
	endPosition_Spindle = teensyStep_Spindle.getPosition();

	// Update Nextion
	Serial1.print("pageRec1_Z.bt3.pco=59391");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageRec1_Z.va0.val=0");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageRec1_Z.bt3.val=0");// The first one may be ignored by Nextion, so resend.
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageRec1_Z.bt2.val=0");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageRec1_Z.bt1.val=0");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
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
	Serial1.print(text);
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
	Serial1.write(text);
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
	Serial1.println(text);
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
		incomingData = Serial1.read();
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
	//		incomingData = Serial1.read();
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
		incomingData = Serial1.read();
	}

	return incomingData;
}

/// <summary>
/// Serial Available
/// </summary>
/// <comment>
/// Verifies a Serial port is active
/// Serial is USB connected to PC
/// Serial1 is connected to pins 0 and 1 (Nextion LCD)
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
	else if (Serial1.available() > 0)
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
		input = Serial1.read();

		// Use 255 as end chars
		if (input == 255)
		{
			EEPROM.write(eePromAddress_Filename + i, 0);
			break;
		}
		EEPROM.write(eePromAddress_Filename + i, input);
	}

	EEPROM.put(eePromAddress_Filename_Length, i);
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

		endPosition_Axis = teensyStep_Axis_Z.getPosition();
		endPosition_Spindle = teensyStep_Spindle.getPosition();

		// print current values of spindle angle [rad] and slide position [steps]
		float phiZ = endPosition_Spindle * (TWO_PI / spindleStepsPerRev);
		//int32_t rZ = teensyStep_Axis_Z.getPosition();

		Serial.printf("%f\t%d\n", phiZ, endPosition_Axis);

		Serial1.print("pageRose.t12.txt=");
		Serial1.write(0x22);
		Serial1.print(endPosition_Spindle);
		Serial1.write(0x22);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		delay(50);
		Serial1.print("pageRose.t12.txt=");
		Serial1.write(0x22);
		Serial1.print(endPosition_Spindle);
		Serial1.write(0x22);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		delay(50);
		Serial1.print("pageRose.t13.txt=");
		Serial1.write(0x22);
		Serial1.print(endPosition_Axis);
		Serial1.write(0x22);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		delay(50);

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
			Serial1.print("pageSync.bt1.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageSync.bt1.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageSync.va0.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageSync.bt0.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageSync.t1.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(10);
			Serial1.print("pageSync.t2.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Axis);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			break;
		}
		case PAGE_SYNCX:
		{
			Serial1.print("pageSyncX.bt1.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageSyncX.bt1.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageSyncX.va0.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageSyncX.bt0.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageSyncX.t1.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(10);
			Serial1.print("pageSyncX.t2.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Axis);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			break;
		}
		case PAGE_REC1_Z:
		{
			Serial1.print("pageRec1_Z.bt1.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageRec1_Z.bt1.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageRec1_Z.va0.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageRec1_Z.bt0.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageRec1_Z.t1.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(10);
			Serial1.print("pageRec1_Z.t2.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Axis);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			break;
		}
		case PAGE_REC1_S:
		{
			Serial1.print("pageRec1_Sp.bt1.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageRec1_Sp.bt1.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageRec1_Sp.va0.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageRec1_Sp.bt0.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageRec1_Sp.t1.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(10);
			Serial1.print("pageRec1_Sp.t2.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Axis);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			break;
		}
		case PAGE_INDEX1:
		{
			Serial1.print("pageIndex1.bt1.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageIndex1.bt1.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageIndex1.va0.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageIndex1.bt0.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageIndex1.t1.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			break;
		}
		case PAGE_INDEX2:
		{
			Serial1.print("pageIndex2.bt1.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageIndex2.bt1.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageIndex2.va0.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageIndex2.bt0.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageIndex2.t1.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			break;
		}
		case PAGE_MOVEZ:
		{
			Serial1.print("pageMoveZ.bt1.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageMoveZ.bt1.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageMoveZ.va0.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageMoveZ.bt0.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageMoveZ.t2.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Axis);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			break;
		}
		case PAGE_MOVEX:
		{
			Serial1.print("pageMoveX.bt1.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageMoveX.bt1.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageMoveX.va0.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageMoveX.bt0.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageMoveX.t2.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Axis);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			break;
		}
		case PAGE_ROSE:
		{
			Serial1.print("pageRose.bt1.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(50);
			Serial1.print("pageRose.bt1.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(50);
			Serial1.print("pageRose.va0.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(50);
			Serial1.print("pageRose.t12.txt=");
			Serial1.write(0x22);
			Serial1.print(endPosition_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(50);
			Serial1.print("pageRose.t13.txt=");
			Serial1.write(0x22);
			Serial1.print(static_cast<int>(endPosition_Axis-initialPosition_Axis));
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
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

		endPosition_Axis = teensyStep_Axis_Z.getPosition();
		endPosition_Spindle = teensyStep_Spindle.getPosition();

		// print current values of spindle angle [rad] and slide position [steps]
		//float phiZ = endPosition_Spindle * (TWO_PI / spindleStepsPerRev);
		//Serial.printf("%f\t%d\n", phiZ, endPosition_Axis);

		Serial1.print("pageRose.t12.txt=");
		Serial1.write(0x22);
		Serial1.print(endPosition_Spindle);
		Serial1.write(0x22);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		delay(10);
		Serial1.print("pageRose.t13.txt=");
		Serial1.write(0x22);
		Serial1.print(endPosition_Axis);
		Serial1.write(0x22);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);


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

		endPosition_Axis = teensyStep_Axis_X.getPosition();
		endPosition_Spindle = teensyStep_Spindle.getPosition();

		// print current values of spindle angle [rad] and slide position [steps]
		//float phiZ = endPosition_Spindle * (TWO_PI / spindleStepsPerRev);
		//Serial.printf("%f\t%d\n", phiZ, endPosition_Axis);

		Serial1.print("pageRose.t12.txt=");
		Serial1.write(0x22);
		Serial1.print(endPosition_Spindle);
		Serial1.write(0x22);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		delay(10);
		Serial1.print("pageRose.t13.txt=");
		Serial1.write(0x22);
		Serial1.print(endPosition_Axis);
		Serial1.write(0x22);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);

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
	while (Serial1.available() > 0)
	{
		// Discard remaining serial chars
		Serial1.read();
	}
	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Page:");
	Serial1.print(pageCallerId);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	switch (pageCallerId)
	{
		case PAGE_SPZ:
		{
			configPageSpZ eePageSpZ;
			EEPROM.get(eePromAddress_SpZ, eePageSpZ);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("Main-MaxSpeed:");
			Serial1.print(eePageSpZ.maxSpd_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("Main-Accel:");
			Serial1.print(eePageSpZ.accel_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("Z Axis-MaxSpeed:");
			Serial1.print(eePageSpZ.maxSpd_Axis_Z);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("Z Axis-Accel:");
			Serial1.print(eePageSpZ.accel_Axis_Z);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			break;
		}
		case PAGE_SPX:
		{
			configPageSpX eePageSpX;
			EEPROM.get(eePromAddress_SpX, eePageSpX);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("Main-MaxSpeed:");
			Serial1.print(eePageSpX.maxSpd_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("Main-Accel:");
			Serial1.print(eePageSpX.accel_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("X Axis-MaxSpeed:");
			Serial1.print(eePageSpX.maxSpd_Axis_X);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("X Axis-Accel:");
			Serial1.print(eePageSpX.accel_Axis_X);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			break;
		}
		case PAGE_SPB:
		{
			configPageSpB eePageSpB;
			EEPROM.get(eePromAddress_SpB, eePageSpB);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("Main-MaxSpeed:");
			Serial1.print(eePageSpB.maxSpd_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("Main-Accel:");
			Serial1.print(eePageSpB.accel_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("B Axis-MaxSpeed:");
			Serial1.print(eePageSpB.maxSpd_Axis_B);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("B Axis-Accel:");
			Serial1.print(eePageSpB.accel_Axis_B);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			break;
		}
		case PAGE_SYNCZ:
		{
			configPageSyncZ eePageSyncZ;
			EEPROM.get(eePromAddress_SyncZ, eePageSyncZ);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("SyncZ-MaxSpeed:");
			Serial1.print(eePageSyncZ.maxSpd_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("SyncZ-Accel:");
			Serial1.print(eePageSyncZ.accel_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("Z Axis-MaxSpeed:");
			Serial1.print(eePageSyncZ.maxSpd_Axis_Z);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("Z Axis-Accel:");
			Serial1.print(eePageSyncZ.accel_Axis_Z);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			break;
		}
		case PAGE_SYNCX:
		{
			configPageSyncX eePageSyncX;
			EEPROM.get(eePromAddress_SyncX, eePageSyncX);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("SyncX-MaxSpeed:");
			Serial1.print(eePageSyncX.maxSpd_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("SyncX-Accel:");
			Serial1.print(eePageSyncX.accel_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("X Axis-MaxSpeed:");
			Serial1.print(eePageSyncX.maxSpd_Axis_X);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("X Axis-Accel:");
			Serial1.print(eePageSyncX.accel_Axis_X);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			break;
		}
		case PAGE_REC1_Z:
		{
			configPageRec1_ZAxis eePageRec1_ZAxis;
			EEPROM.get(eePromAddress_Rec1_Z, eePageRec1_ZAxis);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("Rec1 ZAxis-MaxSpeed:");
			Serial1.print(eePageRec1_ZAxis.maxSpd_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("Rec1 ZAxis-Accel:");
			Serial1.print(eePageRec1_ZAxis.accel_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("Z Axis-MaxSpeed:");
			Serial1.print(eePageRec1_ZAxis.maxSpd_Axis_Z);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("Z Axis-Accel:");
			Serial1.print(eePageRec1_ZAxis.accel_Axis_Z);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			break;
		}
		case PAGE_REC1_S:
		{
			configPageRec1_Spindle eePageRec1_SP;
			EEPROM.get(eePromAddress_Rec1_S, eePageRec1_SP);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("Rec1 Spindle-MaxSpeed:");
			Serial1.print(eePageRec1_SP.maxSpd_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("Rec1 Spindle-Accel:");
			Serial1.print(eePageRec1_SP.accel_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("Z Axis-MaxSpeed:");
			Serial1.print(eePageRec1_SP.maxSpd_Axis_Z);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("Z Axis-Accel:");
			Serial1.print(eePageRec1_SP.accel_Axis_Z);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			break;
		}
		case PAGE_SP:
		{
			configPageSp eePageSp;
			EEPROM.get(eePromAddress_Sp, eePageSp);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("Spindle-MaxSpeed:");
			Serial1.print(eePageSp.maxSpd_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("Spindle-Accel:");
			Serial1.print(eePageSp.accel_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			break;
		}
		case PAGE_INDEX1:
		{
			configPageIndex1 eePageIndex1;
			EEPROM.get(eePromAddress_Index1, eePageIndex1);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("Index1-MaxSpeed:");
			Serial1.print(eePageIndex1.maxSpd_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("Index1-Accel:");
			Serial1.print(eePageIndex1.accel_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			break;
		}
		case PAGE_INDEX2:
		{
			configPageIndex2 eePageIndex2;
			EEPROM.get(eePromAddress_Index2, eePageIndex2);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("Index2-MaxSpeed:");
			Serial1.print(eePageIndex2.maxSpd_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("Index2-Accel:");
			Serial1.print(eePageIndex2.accel_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			break;
		}
		case PAGE_MOVEZ:
		{
			configPageMoveZ eePageMoveZ;
			EEPROM.get(eePromAddress_MoveZ, eePageMoveZ);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("MoveZ-MaxSpeed:");
			Serial1.print(eePageMoveZ.maxSpd_Axis_Z);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("MoveZ-Accel:");
			Serial1.print(eePageMoveZ.accel_Axis_Z);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			break;
		}
		case PAGE_MOVEX:
		{
			configPageMoveX eePageMoveX;
			EEPROM.get(eePromAddress_MoveX, eePageMoveX);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("MoveX-MaxSpeed:");
			Serial1.print(eePageMoveX.maxSpd_Axis_X);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("MoveX-Accel:");
			Serial1.print(eePageMoveX.accel_Axis_X);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			break;
		}
		case PAGE_ROSE:
		{
			configPageRose eePageRose;
			EEPROM.get(eePromAddress_Rose, eePageRose);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("Rose-Spindle MaxSpeed:");
			Serial1.print(eePageRose.maxSpd_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("Rose-Spindle Accel:");
			Serial1.print(eePageRose.accel_Spindle);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("Rose-ZAxis MaxSpeed:");
			Serial1.print(eePageRose.maxSpd_Axis_Z);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("Rose-ZAxis Accel:");
			Serial1.print(eePageRose.accel_Axis_Z);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("Rose-XAxis MaxSpeed:");
			Serial1.print(eePageRose.maxSpd_Axis_X);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			delay(2000);
			Serial1.print("pageConfig.t19.txt=");
			Serial1.write(0x22);
			Serial1.print("Rose-XAxis Accel:");
			Serial1.print(eePageRose.accel_Axis_X);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
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
	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Setup:");
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);

	// pageSetup
	Serial1.print("pageSetup.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Spindle-Microsteps:");
	Serial1.print(eePromConfig.microsteps_Spindle);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);

	Serial1.print("pageSetup.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Spindle-Steps360:");
	Serial1.print(eePromConfig.steps360_Spindle);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageSetup.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Spindle-Gear Ratio:");
	Serial1.print(eePromConfig.gearRatio_Spindle);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageSetup.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Spindle-Enable:");
	Serial1.print(eePromConfig.enable_Spindle? "Low":"High");
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);

	Serial1.print("pageSetup.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Z Axis-Microsteps:");
	Serial1.print(eePromConfig.microsteps_Axis_Z);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageSetup.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Z Axis-Steps/360:");
	Serial1.print(eePromConfig.steps360_Axis_Z);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageSetup.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Z Axis-Dist/360:");
	Serial1.print(eePromConfig.distancePerRev_AxisZ);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageSetup.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Z Axis-Enable:");
	Serial1.print(eePromConfig.polarity_Axis_Z ? "Low" : "High");
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);

#ifndef TWO_AXES_V2
	Serial1.print("pageSetup.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("X Axis-Microsteps:");
	Serial1.print(eePromConfig.microsteps_Axis_X);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageSetup.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("X Axis-Steps/360:");
	Serial1.print(eePromConfig.steps360_Axis_X);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageSetup.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("X Axis-Dist/360:");
	Serial1.print(eePromConfig.distancePerRev_AxisX);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageSetup.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("X Axis-Enable:");
	Serial1.print(eePromConfig.polarity_Axis_X ? "Low" : "High");
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
#endif // TWO_AXES_V2
#ifdef FOUR_AXES
	Serial1.print("pageSetup.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("B Axis-Microsteps:");
	Serial1.print(eePromConfig.microsteps_Axis_B);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);

	Serial1.print("pageSetup.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("B Axis-Steps360:");
	Serial1.print(eePromConfig.steps360_Axis_B);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	//Serial1.print("pageSetup.t19.txt=");
	//Serial1.write(0x22);
	//Serial1.print("B Axis-Gear Ratio:");
	//Serial1.print(eePromConfig.gearRatio_AxisB);
	//Serial1.write(0x22);
	//Serial1.write(0xff);
	//Serial1.write(0xff);
	//Serial1.write(0xff);
	//delay(2000);
	Serial1.print("pageSetup.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("B Axis-Enable:");
	Serial1.print(eePromConfig.polarity_Axis_B ? "Low" : "High");
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
#endif // FOUR_AXES
	
	Serial1.print("pageSetup.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Limit Min Z:");
	Serial1.print(configMain.limit_Min_Z);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageSetup.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Limit Max Z:");
	Serial1.print(configMain.limit_Max_Z);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
#ifndef TWO_AXES_V2
	Serial1.print("pageSetup.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Limit Min X:");
	Serial1.print(configMain.limit_Min_X);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);

	Serial1.print("pageSetup.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Limit Max X:");
	Serial1.print(configMain.limit_Max_X);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
#endif
#ifdef FOUR_AXES
	Serial1.print("pageSetup.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Limit Min B:");
	Serial1.print(configMain.limit_Min_B);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageSetup.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Limit Max B:");
	Serial1.print(configMain.limit_Max_B);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
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
	////Serial1.print("pageRose.t8.txt=");
	////Serial1.write(0x22);
	////Serial1.print(spindleStepsPerRev);
	////Serial1.write(0x22);
	////Serial1.write(0xff);
	////Serial1.write(0xff);
	////Serial1.write(0xff);
	////delay(50);
	////Serial1.print("pageRose.t8.txt=");
	////Serial1.write(0x22);
	////Serial1.print(spindleStepsPerRev);
	////Serial1.write(0x22);
	////Serial1.write(0xff);
	////Serial1.write(0xff);
	////Serial1.write(0xff);
	////delay(50);
	//Serial1.print("pageRose.t9.txt=");
	//Serial1.write(0x22);
	//Serial1.print(eePageRose.maxSpd_Spindle);
	//Serial1.write(0x22);
	//Serial1.write(0xff);
	//Serial1.write(0xff);
	//Serial1.write(0xff);
	//delay(50);
	Serial1.print("pageRose.t10.txt=");
	Serial1.write(0x22);
	Serial1.print(eePageRose.accel_Spindle);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(50);
	Serial1.print("pageRose.t10.txt=");
	Serial1.write(0x22);
	Serial1.print(eePageRose.accel_Spindle);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(50);
	////Serial1.print("pageRose.t11.txt=");
	////Serial1.write(0x22);
	////Serial1.print(eePageRose.rpm);
	////Serial1.write(0x22);
	////Serial1.write(0xff);
	////Serial1.write(0xff);
	////Serial1.write(0xff);
	////delay(50);
	Serial1.print("pageRose.t3.txt=");
	Serial1.write(0x22);
	Serial1.print(eePageRose.maxSpd_Axis_Z);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(50);
	Serial1.print("pageRose.t4.txt=");
	Serial1.write(0x22);
	Serial1.print(eePageRose.accel_Axis_Z);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(50);
	Serial1.print("pageRose.t5.txt=");
	Serial1.write(0x22);
	Serial1.print(eePageRose.amplitude_Axis_Z);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(50);
	Serial1.print("pageRose.t1.txt=");
	Serial1.write(0x22);
	Serial1.print(configRose.n);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(50);
	Serial1.print("pageRose.t2.txt=");
	Serial1.write(0x22);
	Serial1.print(configRose.d);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
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