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
	if (serialId == 0)
	{
		retVal = GetSerial0Float();
	}
	else
	{
		retVal = GetSerial1Float();
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

	// Uncomment for debugging (add new t5 control to Nextion).
	////Serial1.print("pageSlow.t5.txt="); //Nextion global object
	////Serial1.write(0x22);
	////Serial1.print(in[0]); 
	////Serial1.print(in[1]); 
	////Serial1.print(in[2]); 
	////Serial1.print(in[3]); 
	////Serial1.write(0x22);
	////Serial1.write(0xff);
	////Serial1.write(0xff);
	////Serial1.write(0xff);
	return GetFloatFromCharArray(in, decimalFound,endFound);
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
	inputHex = Serial1.read();

	int newInteger = (int)inputHex ;
	while (Serial1.available() > 0)
	{
		// Discard remaining serial chars
		Serial1.read();
	}

	// Uncomment for debugging (add new t3 and t2 control to Nextion).
	//Serial1.print("pageSlow.t3.txt="); 
	//Serial1.write(0x22);
	//Serial1.print(inputHex); 
	//Serial1.write(0x22);
	//Serial1.write(0xff);
	//Serial1.write(0xff);
	//Serial1.write(0xff);
	//Serial1.print("pageSlow.t2.txt="); 
	//Serial1.write(0x22);
	//Serial1.print(newSpeed); 
	//Serial1.write(0x22);
	//Serial1.write(0xff);
	//Serial1.write(0xff);
	//Serial1.write(0xff);

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

	// // Uncomment for debugging (add new t11 control to Nextion).
	////Serial1.print("pageIndex.t11.txt="); 
	////Serial1.write(0x22);
	////Serial1.print(inputNumber); 
	////Serial1.write(0x22);
	////Serial1.write(0xff);
	////Serial1.write(0xff);
	////Serial1.write(0xff);
	return inputNumber;
}

/// <summary>
/// Move AxisZ
/// </summary>
/// <comment>
/// </comment>
/// <param name="direction">Counterclockwise: 0, Clockwise: 1</param>
/// <returns>Success: 0, Cancel/Stop: -1</returns>
int MoveAxisZ(int direction)
{
	stepper_Axis_Z.setAcceleration(configSteppers.acceleration_Axis_Z);
	float nextSpeed = configSteppers.speedPercentage_MoveZ * configSteppers.maxSpeed_Axis_Z * .01;
	stepper_Axis_Z.setMaxSpeed(nextSpeed);
	stepper_Axis_Z.setCurrentPosition(0);

	// Set distance to move
	long moveTo = (configSteppers.distance_MoveZ / (configSteppers.distancePerRev_AxisZ) * (configSteppers.steps360_Axis_Z * configSteppers.microsteps_Axis_Z));
	if (direction == DIR_CCW)
	{
		moveTo = -moveTo;
	}
	stepper_Axis_Z.moveTo(moveTo);
	stepper_Axis_Z.enableOutputs();

	while (stepper_Axis_Z.distanceToGo() != 0)
	{
		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
			case 99: // - c
			{
				stepper_Axis_Z.stop();
			}
			case 62: // > - Set Main Axis Speed
			{
				configSteppers.speedPercentage_MoveZ = GetSerialInteger();
				float newSpeed = configSteppers.speedPercentage_MoveZ * configSteppers.maxSpeed_Axis_Z * .01;
				stepper_Axis_Z.setMaxSpeed(newSpeed);
				EEPROM.put(eePromAddress, configSteppers);
			}
			}
		}

		stepper_Axis_Z.run();
	}

	if (direction == DIR_CCW)
	{
		distanceTotal_MoveZ = distanceTotal_MoveZ + configSteppers.distance_MoveZ;
	}
	else
	{
		distanceTotal_MoveZ = distanceTotal_MoveZ - configSteppers.distance_MoveZ;
	}

	Serial1.print("pageMoveZ.t5.txt=");
	Serial1.write(0x22);
	Serial1.print(moveTo);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageMoveZ.t5.txt=");
	Serial1.write(0x22);
	Serial1.print(moveTo);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);

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
	return 0;
}

#ifdef TEENSY_35
/// <summary>
/// Move AxisX
/// </summary>
/// <comment>
/// </comment>
/// <param name="direction">Counterclockwise: 0, Clockwise: 1</param>
/// <returns>Success: 0, Cancel/Stop: -1</returns>
int MoveAxisX(int direction)
{
	stepper_Axis_X.setAcceleration(configSteppers.acceleration_Axis_X);
	float nextSpeed = configSteppers.speedPercentage_MoveX * configSteppers.maxSpeed_Axis_X * .01;
	stepper_Axis_X.setMaxSpeed(nextSpeed);
	stepper_Axis_X.setCurrentPosition(0);

	// Set distance to move
	long moveTo = (configSteppers.distance_MoveX / (configSteppers.distancePerRev_AxisX) * (configSteppers.steps360_Axis_X * configSteppers.microsteps_Axis_X));
	if (direction == DIR_CCW)
	{
		moveTo = -moveTo;
	}
	stepper_Axis_X.moveTo(moveTo);
	stepper_Axis_X.enableOutputs();

	while (stepper_Axis_X.distanceToGo() != 0)
	{
		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
			case 99: // - c
			{
				stepper_Axis_X.stop();
			}
			case 179: // ³ - MvX Speed 
			{
				configSteppers.speedPercentage_MoveX = GetSerialInteger();
				float newSpeed = configSteppers.speedPercentage_MoveX * configSteppers.maxSpeed_Axis_X * .01;
				stepper_Axis_X.setMaxSpeed(newSpeed);
				EEPROM.put(eePromAddress, configSteppers);
			}
			}
		}

		stepper_Axis_X.run();
	}

	if (direction == DIR_CCW)
	{
		distanceTotal_MoveX = distanceTotal_MoveX + configSteppers.distance_MoveX;
	}
	else
	{
		distanceTotal_MoveX = distanceTotal_MoveX - configSteppers.distance_MoveX;
	}

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
	return 0;
}
#endif // TEENSY_35

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
			case 67: // C
			case 99:
			{
				stepper.stop();
			}
			case 97: // a - Set Main Axis Speed
			case 102: // f - Set Sp2 Spindle Speed
			case 110: // n - Set Main Spindle Speed
			{
				newSpeedPercentage = GetSerialInteger();
				float newSpeed = newSpeedPercentage * maxSpeed * .01;
				stepper.setMaxSpeed(newSpeed);
			}
			}
		}

		stepper.run();
	}

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
	float nextSpeed_Axis = configSteppers.speedPercentage_Axis_Z * configSteppers.maxSpeed_Axis_Z * .01;
	stepper_Axis_Z.setMaxSpeed(nextSpeed_Axis);
	stepper_Axis_Z.setCurrentPosition(0);
	long moveTo_Axis = 1000000000;
	if (direction_Axis == DIR_CCW)
	{
		moveTo_Axis = -moveTo_Axis;
	}
	stepper_Axis_Z.moveTo(moveTo_Axis);
	stepper_Axis_Z.enableOutputs();

	// Configure Spindle
	int spindleSpeedPercentage = configSteppers.speedPercentageSpindle_SpZ;
	int spindleMaxSpeed = configSteppers.maxSpeedSpindle_SpZ;

	float nextSpeed_Spindle = spindleSpeedPercentage * spindleMaxSpeed * .01;
	stepper_Spindle.setMaxSpeed(nextSpeed_Spindle);

	stepper_Spindle.setCurrentPosition(0);
	long moveTo_Spindle = 1000000000;

	if (direction_Spindle == DIR_CCW)
	{
		moveTo_Spindle = -moveTo_Spindle;
	}
	stepper_Spindle.moveTo(moveTo_Spindle);
	stepper_Spindle.enableOutputs();

	while ((stepper_Spindle.distanceToGo() != 0) || (stepper_Axis_Z.distanceToGo() != 0))
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
					configSteppers.speedPercentage_Axis_Z = GetSerialInteger();
					EEPROM.put(eePromAddress, configSteppers);
					nextSpeed_Axis = configSteppers.speedPercentage_Axis_Z  * configSteppers.maxSpeed_Axis_Z * .01;
					stepper_Axis_Z.setMaxSpeed(nextSpeed_Axis);
					stepper_Axis_Z.setCurrentPosition(0);
					stepper_Axis_Z.moveTo(1000000000);
					stepper_Axis_Z_Go = true;
					break;
				}
				case 91: // [ - Axis counter clockwise
				{
					nextSpeed_Axis = configSteppers.speedPercentage_Axis_Z  * configSteppers.maxSpeed_Axis_Z * .01;
					stepper_Axis_Z.setMaxSpeed(nextSpeed_Axis);
					stepper_Axis_Z.setCurrentPosition(0);
					stepper_Axis_Z.moveTo(-1000000000);
					stepper_Axis_Z_Go = true;
					break;
				}
				case 67: // C - Spindle Stop
				{
					stepper_Spindle.stop();
					stepper_Spindle.setCurrentPosition(0);
					if (!stepper_Axis_Z_Go)
					{
						stepper_Axis_Z.setCurrentPosition(0);
					}
					break;
				}
				case 97: // a - Axis speed
				{
					configSteppers.speedPercentage_Axis_Z = GetSerialInteger();
					EEPROM.put(eePromAddress, configSteppers);
					nextSpeed_Axis = configSteppers.speedPercentage_Axis_Z * configSteppers.maxSpeed_Axis_Z * .01;
					stepper_Axis_Z.setMaxSpeed(nextSpeed_Axis);
					break;
				}
				case 99: // c - Axis stop
				{
					stepper_Axis_Z.stop();
					stepper_Axis_Z.setCurrentPosition(0);
					if (!stepper_Spindle_Go)
					{
						stepper_Spindle.setCurrentPosition(0);
					}
					break;
				}
				case 107: // k - Spindle Clockwise
				{
					nextSpeed_Spindle = configSteppers.speedPercentageSpindle_SpZ * configSteppers.maxSpeedSpindle_SpZ * .01;
					stepper_Spindle.setMaxSpeed(nextSpeed_Spindle);
					stepper_Spindle.setCurrentPosition(0);
					stepper_Spindle.moveTo(1000000000);
					stepper_Spindle_Go = true;
					break;
				}
				case 108: // l - Spindle Counterclockwise
				{
					nextSpeed_Spindle = configSteppers.speedPercentageSpindle_SpZ * configSteppers.maxSpeedSpindle_SpZ * .01;
					stepper_Spindle.setMaxSpeed(nextSpeed_Spindle);
					stepper_Spindle.setCurrentPosition(0);
					stepper_Spindle.moveTo(-1000000000);
					stepper_Spindle_Go = true;
					break;
				}
				case 110: // n - Set Spindle Speed
				{
					configSteppers.speedPercentageSpindle_SpZ = GetSerialInteger();
					float nextSpeed = configSteppers.speedPercentageSpindle_SpZ * configSteppers.maxSpeedSpindle_SpZ * .01;
					stepper_Spindle.setMaxSpeed(nextSpeed);
					EEPROM.put(eePromAddress, configSteppers);
					break;
				}
			}
		}

		if (stepper_Axis_Z.distanceToGo() > 0)
		{
			if (digitalRead(PIN_LIMIT_MAX) == LOW)
			{
				stepper_Axis_Z.stop();
				stopFlag = true;
				stepper_Axis_Z_Go = false;
				if (!stepper_Spindle_Go)
				{
					stepper_Spindle.setCurrentPosition(0);
				}
			}
		}
		else
		{
			if (digitalRead(PIN_LIMIT_MIN) == LOW)
			{
				stepper_Axis_Z.stop();
				stopFlag = true;
				stepper_Axis_Z_Go = false;
				if (!stepper_Spindle_Go)
				{
					stepper_Spindle.setCurrentPosition(0);
				}
			}
		}


		if (stepper_Spindle_Go)
		{
			////for (int i = 0; i < 3; i++) // Verify Teensy is operational
			////{
			////	digitalWrite(PIN_LED, HIGH);
			////	delay(500);
			////	digitalWrite(PIN_LED, LOW);
			////	delay(500);
			////}
			stepper_Spindle.run();
		}

		if (stepper_Axis_Z_Go)
		{
			// Check limit switches
			if (stepper_Axis_Z.distanceToGo() > 0)
			{
				if (digitalRead(PIN_LIMIT_MAX) == LOW)
				{
					stepper_Axis_Z.stop();
					stopFlag = true;
					stepper_Axis_Z_Go = false;
				}
			}
			else
			{
				if (digitalRead(PIN_LIMIT_MIN) == LOW)
				{
					stepper_Axis_Z.stop();
					stopFlag = true;
					stepper_Axis_Z_Go = false;
				}
			}

			stepper_Axis_Z.run();

			// Reset stop flag on Nextion
			if (stopFlag)
			{
				Serial1.print("pageMain.t6.txt=");
				Serial1.write(0x22);
				Serial1.print("11");
				Serial1.write(0x22);
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.print("pageMain.bt7.val=0");
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.print("pageMain.bt5.val=0");
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.print("pageMain.bt6.val=0");
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);
				//delay(15);
				Serial1.print("pageMain.va2.val=0");
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);
			}
		}
	}

	stepper_Axis_Z_Go = false;
	stepper_Spindle_Go = false;
	stepper_Axis_Z.setCurrentPosition(0);
	stepper_Spindle.setCurrentPosition(0);

	return 0;
}

#ifdef TEENSY_35
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
	float nextSpeed_Axis = configSteppers.speedPercentage_Axis_X * configSteppers.maxSpeed_Axis_X * .01;
	stepper_Axis_X.setMaxSpeed(nextSpeed_Axis);
	stepper_Axis_X.setCurrentPosition(0);
	long moveTo_Axis = 1000000000;
	if (direction_Axis == DIR_CCW)
	{
		moveTo_Axis = -moveTo_Axis;
	}
	stepper_Axis_X.moveTo(moveTo_Axis);
	stepper_Axis_X.enableOutputs();

	// Configure Spindle
	int spindleSpeedPercentage = configSteppers.speedPercentageSpindle_SpX;
	int spindleMaxSpeed = configSteppers.maxSpeedSpindle_SpX;

	float nextSpeed_Spindle = spindleSpeedPercentage * spindleMaxSpeed * .01;
	stepper_Spindle.setMaxSpeed(nextSpeed_Spindle);

	stepper_Spindle.setCurrentPosition(0);
	long moveTo_Spindle = 1000000000;

	if (direction_Spindle == DIR_CCW)
	{
		moveTo_Spindle = -moveTo_Spindle;
	}
	stepper_Spindle.moveTo(moveTo_Spindle);
	stepper_Spindle.enableOutputs();
	while ((stepper_Spindle.distanceToGo() != 0) || (stepper_Axis_X.distanceToGo() != 0))
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
					configSteppers.speedPercentage_Axis_X = GetSerialInteger();
					EEPROM.put(eePromAddress, configSteppers);
					nextSpeed_Axis = configSteppers.speedPercentage_Axis_X  * configSteppers.maxSpeed_Axis_X * .01;
					stepper_Axis_X.setMaxSpeed(nextSpeed_Axis);
					stepper_Axis_X.setCurrentPosition(0);
					stepper_Axis_X.moveTo(1000000000);
					stepper_Axis_X_Go = true;
					break;
				}
				case 165: // ¥ - Sp-X Axis CCW
				{
					nextSpeed_Axis = configSteppers.speedPercentage_Axis_X  * configSteppers.maxSpeed_Axis_X * .01;
					stepper_Axis_X.setMaxSpeed(nextSpeed_Axis);
					stepper_Axis_X.setCurrentPosition(0);
					stepper_Axis_X.moveTo(-1000000000);
					stepper_Axis_X_Go = true;
					break;
				}
				case 67: // C - Spindle Stop
				{
					stepper_Spindle.stop();
					stepper_Spindle.setCurrentPosition(0);
					if (!stepper_Axis_X_Go)
					{
						stepper_Axis_X.setCurrentPosition(0);
					}
					break;
				}
				case 162: // ¢ - Sp-X axis speed
				{
					configSteppers.speedPercentage_Axis_X = GetSerialInteger();
					EEPROM.put(eePromAddress, configSteppers);
					nextSpeed_Axis = configSteppers.speedPercentage_Axis_X * configSteppers.maxSpeed_Axis_X * .01;
					stepper_Axis_X.setMaxSpeed(nextSpeed_Axis);
					break;
				}
				case 99: // c - Axis stop
				{
					stepper_Axis_X.stop();
					stepper_Axis_X.setCurrentPosition(0);
					if (!stepper_Spindle_Go)
					{
						stepper_Spindle.setCurrentPosition(0);
					}
					break;
				}
				case 164: // ¤ - Sp-X spindle CW
				{
					nextSpeed_Spindle = configSteppers.speedPercentageSpindle_SpX * configSteppers.maxSpeedSpindle_SpX * .01;
					stepper_Spindle.setMaxSpeed(nextSpeed_Spindle);
					stepper_Spindle.setCurrentPosition(0);
					stepper_Spindle.moveTo(1000000000);
					stepper_Spindle_Go = true;
					break;
				}
				case 163: // £ - Sp-X spindle CCW
				{
					nextSpeed_Spindle = configSteppers.speedPercentageSpindle_SpX * configSteppers.maxSpeedSpindle_SpX * .01;
					stepper_Spindle.setMaxSpeed(nextSpeed_Spindle);
					stepper_Spindle.setCurrentPosition(0);
					stepper_Spindle.moveTo(-1000000000);
					stepper_Spindle_Go = true;
					break;
				}
				case 161: // ¡ - Sp-X spindle speed
				{
					configSteppers.speedPercentageSpindle_SpX = GetSerialInteger();
					float nextSpeed = configSteppers.speedPercentageSpindle_SpX * configSteppers.maxSpeedSpindle_SpX * .01;
					stepper_Spindle.setMaxSpeed(nextSpeed);
					EEPROM.put(eePromAddress, configSteppers);
					break;
				}
			}
		}

		if (stepper_Axis_X.distanceToGo() > 0)
		{
			if (digitalRead(PIN_LIMIT_MAX) == LOW)
			{
				stepper_Axis_X.stop();
				stopFlag = true;
				stepper_Axis_X_Go = false;
				if (!stepper_Spindle_Go)
				{
					stepper_Spindle.setCurrentPosition(0);
				}
			}
		}
		else
		{
			if (digitalRead(PIN_LIMIT_MIN) == LOW)
			{
				stepper_Axis_X.stop();

				stopFlag = true;
				stepper_Axis_X_Go = false;
				if (!stepper_Spindle_Go)
				{
					stepper_Spindle.setCurrentPosition(0);
				}
			}
		}

		if (stepper_Spindle_Go)
		{
			stepper_Spindle.run();
		}

		if (stepper_Axis_X_Go)
		{
			// Check limit switches
			if (stepper_Axis_X.distanceToGo() > 0)
			{
				if (digitalRead(PIN_LIMIT_MAX) == LOW)
				{
					stepper_Axis_X.stop();
					stopFlag = true;
					stepper_Axis_X_Go = false;
				}
			}
			else
			{
				if (digitalRead(PIN_LIMIT_MIN) == LOW)
				{
					stepper_Axis_X.stop();
					stopFlag = true;
					stepper_Axis_X_Go = false;
				}
			}

			stepper_Axis_X.run();

			// Reset stop flag on Nextion
			if (stopFlag)
			{
				Serial1.print("pageSpX.t6.txt=");
				Serial1.write(0x22);
				Serial1.print("11");
				Serial1.write(0x22);
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);
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
	stepper_Axis_X.setCurrentPosition(0);
	stepper_Spindle.setCurrentPosition(0);

	return 0;
}

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
	float nextSpeed_Axis = configSteppers.speedPercentage_Axis_B * configSteppers.maxSpeed_Axis_B * .01;
	stepper_Axis_B.setMaxSpeed(nextSpeed_Axis);
	stepper_Axis_B.setCurrentPosition(0);
	long moveTo_Axis = 1000000000;
	if (direction_Axis == DIR_CCW)
	{
		moveTo_Axis = -moveTo_Axis;
	}
	stepper_Axis_B.moveTo(moveTo_Axis);
	stepper_Axis_B.enableOutputs();

	// Configure Spindle
	int spindleSpeedPercentage = configSteppers.speedPercentageSpindle_SpB;
	int spindleMaxSpeed = configSteppers.maxSpeedSpindle_SpB;

	float nextSpeed_Spindle = spindleSpeedPercentage * spindleMaxSpeed * .01;
	stepper_Spindle.setMaxSpeed(nextSpeed_Spindle);

	stepper_Spindle.setCurrentPosition(0);
	long moveTo_Spindle = 1000000000;

	if (direction_Spindle == DIR_CCW)
	{
		moveTo_Spindle = -moveTo_Spindle;
	}
	stepper_Spindle.moveTo(moveTo_Spindle);
	stepper_Spindle.enableOutputs();
	while ((stepper_Spindle.distanceToGo() != 0) || (stepper_Axis_B.distanceToGo() != 0))
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
					nextSpeed_Axis = configSteppers.speedPercentage_Axis_B  * configSteppers.maxSpeed_Axis_B * .01;
					stepper_Axis_B.setMaxSpeed(nextSpeed_Axis);
					stepper_Axis_B.setCurrentPosition(0);
					stepper_Axis_B.moveTo(1000000000);
					stepper_Axis_B_Go = true;
					break;
				}
				case 171: // « - Sp-B Axis CCW
				{
					nextSpeed_Axis = configSteppers.speedPercentage_Axis_B  * configSteppers.maxSpeed_Axis_B * .01;
					stepper_Axis_B.setMaxSpeed(nextSpeed_Axis);
					stepper_Axis_B.setCurrentPosition(0);
					stepper_Axis_B.moveTo(-1000000000);
					stepper_Axis_B_Go = true;
					break;
				}
				case 67: // C - Spindle Stop
				{
					stepper_Spindle.stop();
					stepper_Spindle.setCurrentPosition(0);
					if (!stepper_Axis_B_Go)
					{
						stepper_Axis_B.setCurrentPosition(0);
					}
					break;
				}
				case 168: // ¨ - Sp-B axis speed
				{
					configSteppers.speedPercentage_Axis_B = GetSerialInteger();
					EEPROM.put(eePromAddress, configSteppers);
					nextSpeed_Axis = configSteppers.speedPercentage_Axis_B * configSteppers.maxSpeed_Axis_B * .01;
					stepper_Axis_B.setMaxSpeed(nextSpeed_Axis);
					break;
				}
				case 99: // c - Axis stop
				{
					stepper_Axis_B.stop();
					stepper_Axis_B.setCurrentPosition(0);
					if (!stepper_Spindle_Go)
					{
						stepper_Spindle.setCurrentPosition(0);
					}
					break;
				}
				case 170: // ª - Sp-B spindle CW
				{
					nextSpeed_Spindle = configSteppers.speedPercentageSpindle_SpB * configSteppers.maxSpeedSpindle_SpB * .01;
					stepper_Spindle.setMaxSpeed(nextSpeed_Spindle);
					stepper_Spindle.setCurrentPosition(0);
					stepper_Spindle.moveTo(1000000000);
					stepper_Spindle_Go = true;
					break;
				}
				case 169: // © - Sp-B spindle CCW
				{
					nextSpeed_Spindle = configSteppers.speedPercentageSpindle_SpB * configSteppers.maxSpeedSpindle_SpB * .01;
					stepper_Spindle.setMaxSpeed(nextSpeed_Spindle);
					stepper_Spindle.setCurrentPosition(0);
					stepper_Spindle.moveTo(-1000000000);
					stepper_Spindle_Go = true;
					break;
				}
				case 167: // § - Sp-B spindle speed
				{
					configSteppers.speedPercentageSpindle_SpB = GetSerialInteger();
					float nextSpeed = configSteppers.speedPercentageSpindle_SpB * configSteppers.maxSpeedSpindle_SpB * .01;
					stepper_Spindle.setMaxSpeed(nextSpeed);
					EEPROM.put(eePromAddress, configSteppers);
					break;
				}
			}
		}

		if (stepper_Axis_B.distanceToGo() > 0)
		{
			if (digitalRead(PIN_LIMIT_MAX) == LOW)
			{
				stepper_Axis_B.stop();
				stopFlag = true;
				stepper_Axis_B_Go = false;
				if (!stepper_Spindle_Go)
				{
					stepper_Spindle.setCurrentPosition(0);
				}
			}
		}
		else
		{
			if (digitalRead(PIN_LIMIT_MIN) == LOW)
			{
				stepper_Axis_B.stop();

				stopFlag = true;
				stepper_Axis_B_Go = false;
				if (!stepper_Spindle_Go)
				{
					stepper_Spindle.setCurrentPosition(0);
				}
			}
		}

		if (stepper_Spindle_Go)
		{
			stepper_Spindle.run();
		}

		if (stepper_Axis_B_Go)
		{
			// Check limit switches
			if (stepper_Axis_B.distanceToGo() > 0)
			{
				if (digitalRead(PIN_LIMIT_MAX) == LOW)
				{
					stepper_Axis_B.stop();
					stopFlag = true;
					stepper_Axis_B_Go = false;
				}
			}
			else
			{
				if (digitalRead(PIN_LIMIT_MIN) == LOW)
				{
					stepper_Axis_B.stop();
					stopFlag = true;
					stepper_Axis_B_Go = false;
				}
			}

			stepper_Axis_B.run();

			// Reset stop flag on Nextion
			if (stopFlag)
			{
				Serial1.print("pageSpB.t6.txt=");
				Serial1.write(0x22);
				Serial1.print("11");
				Serial1.write(0x22);
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);
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
				//delay(15);
				Serial1.print("pageSpB.va2.val=0");
				Serial1.write(0xff);
				Serial1.write(0xff);
				Serial1.write(0xff);
			}
		}
	}

	stepper_Axis_B_Go = false;
	stepper_Spindle_Go = false;
	stepper_Axis_B.setCurrentPosition(0);
	stepper_Spindle.setCurrentPosition(0);

	return 0;
}
#endif // TEENSY_35

/// <summary>
/// Run steppers
/// </summary>
/// <comment>
/// </comment>
/// <param name="direction">Counterclockwise: 0, Clockwise: 1</param>
/// <returns>Success: 0, Cancel/Stop: -1</returns>
int RunTwoSteppersSyncZ(
	int direction_Axis,
	int direction_Spindle
)
{
	// Stop Flag
	bool stopFlag = false;

	//AccelStepper stepper_Axis_Z;
	int acceleration_Axis = 0;
	int speedPercentage_Axis = 0;
	int maxSpeed_Axis = 0;

	acceleration_Axis = configSteppers.acceleration_Axis_Z;
	speedPercentage_Axis = configSteppers.speedPercentage_Axis_Z;
	maxSpeed_Axis = configSteppers.maxSpeed_Axis_Z;

	// Configure Z Axis
	float nextSpeed_Axis = speedPercentage_Axis * maxSpeed_Axis * .01;
	stepper_Axis_Z.setMaxSpeed(nextSpeed_Axis);
	stepper_Axis_Z.setAcceleration(acceleration_Axis);
	stepper_Axis_Z.setCurrentPosition(0);
	long moveTo_Axis = 1000000000;
	if (direction_Axis == 0)
	{
		moveTo_Axis = -moveTo_Axis;
	}

	stepper_Axis_Z.moveTo(moveTo_Axis);
	stepper_Axis_Z.enableOutputs();

	// Configure Spindle
	int spindleSpeedPercentage = configSteppers.speedPercentageSpindle_SyncZ;
	int spindleMaxSpeed = configSteppers.maxSpeedSpindle_SyncZ;
	stepper_Spindle.setAcceleration(configSteppers.accelerationSpindle_SyncZ);
	float nextSpeed_Spindle = spindleSpeedPercentage * spindleMaxSpeed * .01;
	stepper_Spindle.setMaxSpeed(nextSpeed_Spindle);
	stepper_Spindle.setCurrentPosition(0);
	long moveTo_Spindle = 1000000000;

	if (direction_Spindle == 1)
	{
		moveTo_Spindle = -moveTo_Spindle;
	}
	stepper_Spindle.moveTo(moveTo_Spindle);
	stepper_Spindle.enableOutputs();


	while (stepper_Spindle.distanceToGo() != 0)
	{
		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			//delay(10);

			switch (incomingByte)
			{
			case 93: // ] - Stop Sync
			{
				stepper_Spindle.stop();
				stepper_Axis_Z.stop();
				stepper_Spindle.setCurrentPosition(0);
				stepper_Axis_Z.setCurrentPosition(0);
				break;
			}
			}
		}

		if (direction_Axis == DIR_CW)
		{
			if (digitalRead(PIN_LIMIT_MAX) == LOW)
			{
				stepper_Spindle.stop();
				stepper_Axis_Z.stop();
				stepper_Spindle.setCurrentPosition(0);
				stepper_Axis_Z.setCurrentPosition(0);
				stopFlag = true;
			}
		}
		else
		{
			if (digitalRead(PIN_LIMIT_MIN) == LOW)
			{
				stepper_Spindle.stop();
				stepper_Axis_Z.stop();
				stepper_Spindle.setCurrentPosition(0);
				stepper_Axis_Z.setCurrentPosition(0);
				stopFlag = true;
			}
		}

		stepper_Axis_Z.run();
		stepper_Spindle.run();
	}

	// Reset stop flag on Nextion
	if (stopFlag)
	{

		Serial1.print("pageSync.t3.txt=");
		Serial1.write(0x22);
		Serial1.print("11");
		Serial1.write(0x22);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.print("pageSync.bt2.val=0");
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.print("pageSync.bt6.val=0");
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.print("pageSync.bt1.val=0");
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		delay(15);
		Serial1.print("pageSync.va0.val=0");
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
	}

	return 0;
}

/// <summary>
/// Index spindle with TeensyStep
/// </summary>
/// <comment>
/// </comment>
/// <param name="directionSpindle">Counterclockwise: -1, Clockwise: 1</param>
/// <returns></returns>
void IndexSpindle(int indexId, int directionSpindle)
{
	double stepsToMove = configSteppers.size_Index1;
	if (indexId == 2)
	{
		stepsToMove = configSteppers.size_Index2;
	}
	// Set speed and acceleration
	motor_Spindle
		.setMaxSpeed(configSteppers.maxSpeed_Index)
		.setAcceleration(configSteppers.acceleration_Index)
		.setTargetRel(stepsToMove * directionSpindle);

	digitalWrite(PIN_SPINDLE_ENABLE, LOW);
	controller_Spindle.moveAsync(motor_Spindle);

	if (!controller_Spindle.isRunning())
	{ 
		if (indexId == 2)
		{
			// Update Index flag on Nextion. 
			Serial1.print("pageIndexF2.b6.pco=59391");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageIndexF2.va0.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageIndexF2.b6.pco=59391");// The first one may be ignored by Nextion, so resend.
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageIndexF2.b7.pco=59391");
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
		else
		{ 
		
			// Update Index flag on Nextion. 
			Serial1.print("pageIndex1.b6.pco=59391");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			////Serial1.print("pageIndex1.t3.txt=");
			////Serial1.write(0x22);
			////Serial1.print(stepsToMove);
			////Serial1.write(0x22);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			////Serial1.write(0xff);
			Serial1.print("pageIndex1.va0.val=0");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageIndex1.b6.pco=59391");
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageIndex1.b7.pco=59391");
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
		}
	}
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
	// Set speed and acceleration
	motor_Spindle
		.setMaxSpeed(configSteppers.maxSpeedSpindle_SyncZ * configSteppers.speedPercentageSpindle_SyncZ)
		.setAcceleration(configSteppers.accelerationSpindle_SyncZ)
		.setTargetRel(configSteppers.revolutionsSyncZ_Spindle * configSteppers.gearRatio_Spindle * configSteppers.microsteps_Spindle * configSteppers.steps360_Spindle * directionSpindle);

	motor_Axis_Z
		.setMaxSpeed(configSteppers.maxSpeed_Axis_Z * configSteppers.speedPercentageAxis_SyncZ)       
		.setAcceleration(configSteppers.acceleration_Axis_Z) 
		.setTargetRel((configSteppers.distanceSyncZ / configSteppers.distancePerRev_AxisZ) * configSteppers.microsteps_Axis_Z * configSteppers.steps360_Axis_Z * directionAxis);

	digitalWrite(PIN_SPINDLE_ENABLE, LOW);
	digitalWrite(PIN_AXIS_Z_ENABLE, LOW);  // Disable

	controller_Spindle.moveAsync(motor_Spindle, motor_Axis_Z);

	while (controller_Spindle.isRunning())
	{ 
		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
				case 93: // ] - Stop Sync
				{
					controller_Spindle.stopAsync();
					break;
				}
			}
		}

		delay(10);
	}

	// Update Nextion
	Serial1.print("pageSync.bt6.pco=0");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	
	Serial1.print("pageSync.t11.txt=");
	Serial1.write(0x22);
	Serial1.print(configSteppers.revolutionsSyncZ_Spindle * configSteppers.gearRatio_Spindle * configSteppers.microsteps_Spindle * configSteppers.steps360_Spindle);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageSync.t12.txt=");
	Serial1.write(0x22);
	Serial1.print((configSteppers.distanceSyncZ / configSteppers.distancePerRev_AxisZ) * configSteppers.microsteps_Axis_Z * configSteppers.steps360_Axis_Z);
	Serial1.write(0x22);
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

#ifdef TEENSY_35
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
	// Set speed and acceleration
	motor_Spindle
		.setMaxSpeed(configSteppers.maxSpeedSpindle_SyncX * configSteppers.speedPercentageSpindle_SyncX)
		.setAcceleration(configSteppers.accelerationSpindle_SyncX)
		.setTargetRel(configSteppers.revolutionsSyncX_Spindle * configSteppers.gearRatio_Spindle * configSteppers.microsteps_Spindle * configSteppers.steps360_Spindle * directionSpindle);

	motor_Axis_X
		.setMaxSpeed(configSteppers.maxSpeed_Axis_X * configSteppers.speedPercentageAxis_SyncX)
		.setAcceleration(configSteppers.acceleration_Axis_X)
		.setTargetRel((configSteppers.distanceSyncX / configSteppers.distancePerRev_AxisX) * configSteppers.microsteps_Axis_X * configSteppers.steps360_Axis_X * directionAxis);

	digitalWrite(PIN_SPINDLE_ENABLE, LOW);
	digitalWrite(PIN_AXIS_X_ENABLE, LOW);  // Disable

	controller_Spindle.moveAsync(motor_Spindle, motor_Axis_X);

	while (controller_Spindle.isRunning())
	{
		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
			case 93: // ] - Stop Sync
			{
				controller_Spindle.stopAsync();
				break;
			}
			}
		}

		delay(10);
	}

	// Update Nextion
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


/// <summary>
/// Run steppers
/// </summary>
/// <comment>
/// </comment>
/// <param name="direction">Counterclockwise: 0, Clockwise: 1</param>
/// <returns>Success: 0, Cancel/Stop: -1</returns>
int RunTwoSteppersSyncX(
	int direction_Axis,
	int direction_Spindle
)
{
	// Stop Flag
	bool stopFlag = false;

	//AccelStepper stepper_Axis_X;
	int acceleration_Axis = 0;
	int speedPercentage_Axis = 0;
	int maxSpeed_Axis = 0;

	acceleration_Axis = configSteppers.acceleration_Axis_X;
	speedPercentage_Axis = configSteppers.speedPercentage_Axis_X;
	maxSpeed_Axis = configSteppers.maxSpeed_Axis_X;

	// Configure X Axis
	float nextSpeed_Axis = speedPercentage_Axis * maxSpeed_Axis * .01;
	stepper_Axis_X.setMaxSpeed(nextSpeed_Axis);
	stepper_Axis_X.setAcceleration(acceleration_Axis);
	stepper_Axis_X.setCurrentPosition(0);
	long moveTo_Axis = 1000000000;
	if (direction_Axis == 0)
	{
		moveTo_Axis = -moveTo_Axis;
	}

	stepper_Axis_X.moveTo(moveTo_Axis);
	stepper_Axis_X.enableOutputs();

	// Configure Spindle
	int spindleSpeedPercentage = configSteppers.speedPercentageSpindle_SyncX;
	int spindleMaxSpeed = configSteppers.maxSpeedSpindle_SyncX;
	stepper_Spindle.setAcceleration(configSteppers.accelerationSpindle_SyncX);
	float nextSpeed_Spindle = spindleSpeedPercentage * spindleMaxSpeed * .01;
	stepper_Spindle.setMaxSpeed(nextSpeed_Spindle);
	stepper_Spindle.setCurrentPosition(0);
	long moveTo_Spindle = 1000000000;

	if (direction_Spindle == 1)
	{
		moveTo_Spindle = -moveTo_Spindle;
	}
	stepper_Spindle.moveTo(moveTo_Spindle);
	stepper_Spindle.enableOutputs();


	while (stepper_Spindle.distanceToGo() != 0)
	{
		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			//delay(10);

			switch (incomingByte)
			{
			case 93: // ] - Stop Sync
			{
				stepper_Spindle.stop();
				stepper_Axis_X.stop();
				stepper_Spindle.setCurrentPosition(0);
				stepper_Axis_X.setCurrentPosition(0);
				break;
			}
			}
		}

		if (direction_Axis == DIR_CW)
		{
			if (digitalRead(PIN_LIMIT_MAX) == LOW)
			{
				stepper_Spindle.stop();
				stepper_Axis_X.stop();
				stepper_Spindle.setCurrentPosition(0);
				stepper_Axis_X.setCurrentPosition(0);
				stopFlag = true;
			}
		}
		else
		{
			if (digitalRead(PIN_LIMIT_MIN) == LOW)
			{
				stepper_Spindle.stop();
				stepper_Axis_X.stop();
				stepper_Spindle.setCurrentPosition(0);
				stepper_Axis_X.setCurrentPosition(0);
				stopFlag = true;
			}
		}

		stepper_Axis_X.run();
		stepper_Spindle.run();
	}

	// Reset stop flag on Nextion
	if (stopFlag)
	{
		Serial1.print("pageSyncX.t3.txt=");
		Serial1.write(0x22);
		Serial1.print("11");
		Serial1.write(0x22);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.print("pageSyncX.bt2.val=0");
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.print("pageSyncX.bt6.val=0");
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.print("pageSyncX.bt1.val=0");
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		delay(15);
		Serial1.print("pageSyncX.va0.val=0");
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.print("pageSyncX.bt6.pco=0");
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.print("pageSyncX.bt1.pco=0");
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
	}

	return 0;
}
#endif // TEENSY_35

void aaDoWaveZ(int wavDir)
{
	int t1 = 3000;
	int t2 = 0;
	int t3 = 3000;
	targetPositions[0] = t1;
	targetPositions[1] = t2;
	targetPositions[2] = t3;
	targetPositions[3] = t2;
	nrOfPositions = 4;

	motor_Axis_Z.setAcceleration(configSteppers.acceleration_ZiZa_Z_Axis);
	motor_Axis_Z.setMaxSpeed(configSteppers.maxSpeed_ZiZa_Z_Axis);
	controller_Axis.rotateAsync(motor_Axis_Z);

	motor_Spindle.setAcceleration(configSteppers.acceleration_ZiZa_Z_Spindle);
	motor_Spindle.setMaxSpeed(configSteppers.maxSpeed_ZiZa_Z_Spindle);

	//xxxxcontroller_Spindle.setCallback(newMotorTarget);
	//xxxxnewMotorTarget();  // start the movment sequence
}
void xxxnewMotorTarget()
{
	static int posNr = 0;

	motor_Spindle.setTargetAbs(targetPositions[posNr]);
	controller_Spindle.moveAsync(motor_Spindle);

	posNr = (posNr + 1);// % nrOfPositions;
	if (posNr == nrOfPositions + 1)
	{
		controller_Axis.stop();
	}
}
void xxxxnewMotorTarget()
{
	static int posNr = -1;

	// Run through targetPositions once
	posNr = (posNr + 1);

	// If targetPositions completed, stop Axis motor
	if (posNr == nrOfPositions + 1)
	{
		controller_Axis.stop();
		//return;
	}
	else
	{
		// Move spindle motor
		motor_Spindle.setTargetAbs(targetPositions[posNr]);
		controller_Spindle.moveAsync(motor_Spindle);
	}

	Serial1.print("pageZiZa_Z.t9.txt=");
	Serial1.write(0x22);
	Serial1.print(posNr);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageZiZa_Z.t9.txt=");
	Serial1.write(0x22);
	Serial1.print(posNr);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	return;
}

void Return_ZigZag_SpZ()
{
	double returnTarget = returnSteps_ZigZag_SpZ * (-1);
	motor_Spindle
		.setMaxSpeed(configSteppers.maxSpeedSpindle_SpB)
		.setAcceleration(configSteppers.accelerationSpindle_SpB)
		.setTargetRel(returnTarget);

	controller_Spindle.moveAsync(motor_Spindle);

	while (controller_Spindle.isRunning())
	{
		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
			case 93: // ] - Stop Sync
			{
				controller_Spindle.stop();
				break;
			}
			}
		}

		delay(10);
	}
	// Update Nextion
	Serial1.print("pageZiZaSp_Z.bt0.pco=0");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageZiZaSp_Z.va0.val=0");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageZiZaSp_Z.bt0.val=0");// The first one may be ignored by Nextion, so resend.
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageZiZaSp_Z.bt1.val=0");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
}

void DoZigZag_SpZ(int wavDir)
{
	////int32_t startPosition_Z = motor_Axis_Z.getPosition();
	////int32_t startPosition_Sp = motor_Spindle.getPosition();
	// Set speed and acceleration
	motor_Spindle
		.setMaxSpeed(configSteppers.maxSpeed_ZiZa_Z_Spindle)
		.setAcceleration(configSteppers.acceleration_ZiZa_Z_Spindle);

	float spindleDegrees = configSteppers.degrees_ZigZag_SpZ / (configSteppers.repeats_ZigZag_SpZ * 2);
	int spindleSteps = (configSteppers.microsteps_Spindle * configSteppers.steps360_Spindle * configSteppers.gearRatio_Spindle) * (spindleDegrees / 360) * wavDir;


	float axisDistance = configSteppers.amplitude_ZigZag_SpZ / (configSteppers.repeats_ZiZa_Z * 2);
	int axisSteps = round((axisDistance / configSteppers.distancePerRev_AxisZ) * configSteppers.microsteps_Axis_Z * configSteppers.steps360_Axis_Z * wavDir);

	returnSteps_ZigZag_SpZ = spindleSteps*configSteppers.repeats_ZigZag_SpZ * 2;

	motor_Axis_Z
		.setMaxSpeed(configSteppers.maxSpeed_ZiZa_Z_Axis)
		.setAcceleration(configSteppers.acceleration_ZiZa_Z_Axis);

	digitalWrite(PIN_SPINDLE_ENABLE, LOW);
	digitalWrite(PIN_AXIS_Z_ENABLE, LOW);  // Enable
	for (int i = 0; i < (configSteppers.repeats_ZiZa_Z * 2); i++)
	{
		axisSteps *= -1;

		motor_Spindle.setTargetRel(spindleSteps);
		motor_Axis_Z.setTargetRel(axisSteps);

		controller_Spindle.moveAsync(motor_Spindle, motor_Axis_Z);

		while (controller_Spindle.isRunning())
		{
			// Check for Cancel code  
			if (SerialAvailable() > 0)
			{
				incomingByte = SerialRead(serialId);
				switch (incomingByte)
				{
					case 93: // ] - Stop Sync
					{
						controller_Spindle.stop();
						return;
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
	Serial1.print("pageZiZaSp_Z.t9.txt=");
	Serial1.write(0x22);
	Serial1.print(spindleSteps);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageZiZaSp_Z.t9.txt=");
	Serial1.write(0x22);
	Serial1.print(spindleSteps);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageZiZaSp_Z.t12.txt=");
	Serial1.write(0x22);
	Serial1.print(axisSteps*(configSteppers.repeats_ZiZa_Z * 2));
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageZiZaSp_Z.t10.txt=");
	Serial1.write(0x22);
	Serial1.print(round(returnSteps_ZigZag_SpZ));
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	////int32_t endPosition_Z = motor_Axis_Z.getPosition();
	////int32_t endPosition_Sp = motor_Spindle.getPosition();
/*
	Serial1.print("pageZiZaSp_Z.t0.txt=");
	Serial1.write(0x22);
	Serial1.print(startPosition_Sp);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageZiZaSp_Z.t0.txt=");
	Serial1.write(0x22);
	Serial1.print(startPosition_Sp);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageZiZaSp_Z.t11.txt=");
	Serial1.write(0x22);
	Serial1.print(endPosition_Sp);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageZiZaSp_Z.t7.txt=");
	Serial1.write(0x22);
	Serial1.print(startPosition_Z);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageZiZaSp_Z.t8.txt=");
	Serial1.write(0x22);
	Serial1.print(endPosition_Z);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageZiZaSp_Z.t9.txt=");
	Serial1.write(0x22);
	Serial1.print(returnSteps_ZiZa_Z);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	*/
	// Update Nextion
	Serial1.print("pageZiZaSp_Z.bt3.pco=0");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageZiZaSp_Z.va0.val=0");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageZiZaSp_Z.bt3.val=0");// The first one may be ignored by Nextion, so resend.
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageZiZaSp_Z.bt2.val=0");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageZiZaSp_Z.bt1.val=0");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
}

void Return_ZigZag_Z()
{
	double returnTarget = returnSteps_ZiZa_Z * (-1);
	motor_Axis_Z
		.setMaxSpeed(configSteppers.maxSpeed_Axis_Z)
		.setAcceleration(configSteppers.acceleration_Axis_Z)
		.setTargetRel(returnTarget);

	controller_Axis.moveAsync(motor_Axis_Z);

	while (controller_Axis.isRunning())
	{
		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
			case 93: // ] - Stop Sync
			{
				controller_Axis.stop();
				break;
			}
			}
		}

		delay(10);
	}
	// Update Nextion
	Serial1.print("pageZiZa_Z.bt0.pco=0");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageZiZa_Z.va0.val=0");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageZiZa_Z.bt0.val=0");// The first one may be ignored by Nextion, so resend.
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageZiZa_Z.bt1.val=0");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
}

void DoZigZag_Z(int wavDir)
{
	//int32_t startPosition_Z = motor_Axis_Z.getPosition();
	//int32_t startPosition_Sp = motor_Spindle.getPosition();
	// Set speed and acceleration
	motor_Spindle
		.setMaxSpeed(configSteppers.maxSpeed_ZiZa_Z_Spindle)
		.setAcceleration(configSteppers.acceleration_ZiZa_Z_Spindle);
	int spindleSteps = (configSteppers.microsteps_Spindle * configSteppers.steps360_Spindle * configSteppers.gearRatio_Spindle) * (configSteppers.amplitude_ZiZa_Z / 360) * wavDir;
	long axisSteps = round((((configSteppers.distance_ZiZa_Z / (configSteppers.distancePerRev_AxisZ)) * (configSteppers.steps360_Axis_Z * configSteppers.microsteps_Axis_Z))/(configSteppers.repeats_ZiZa_Z * 2))*wavDir);

	returnSteps_ZiZa_Z = axisSteps*configSteppers.repeats_ZiZa_Z * 2;
	motor_Axis_Z
		.setMaxSpeed(configSteppers.maxSpeed_ZiZa_Z_Axis)
		.setAcceleration(configSteppers.acceleration_ZiZa_Z_Axis);

	digitalWrite(PIN_SPINDLE_ENABLE, LOW);
	digitalWrite(PIN_AXIS_Z_ENABLE, LOW);  // Enable
	for (int i = 0; i < (configSteppers.repeats_ZiZa_Z * 2); i++)
	{
		spindleSteps *= -1;

		motor_Spindle.setTargetRel(spindleSteps);
		motor_Axis_Z.setTargetRel(axisSteps);

		controller_Spindle.moveAsync(motor_Spindle, motor_Axis_Z);
		delay(10);

		while (controller_Spindle.isRunning())
		{
			digitalWriteFast(LED_BUILTIN, !digitalReadFast(LED_BUILTIN)); // toggle LED
			// Check for Cancel code  
			if (SerialAvailable() > 0)
			{
				incomingByte = SerialRead(serialId);
				switch (incomingByte)
				{
					case 93: // ] - Stop Sync
					{
						controller_Spindle.stop();
						return;
						break;
					}
					default:
					{
						break;
					}
				}
			}

			delay(100);
		}
		////while (controller_Spindle.isRunning())
		////{
		////	// Check for Cancel code  
		////	if (SerialAvailable() > 0)
		////	{
		////		incomingByte = SerialRead(serialId);
		////		switch (incomingByte)
		////		{
		////		case 93: // ] - Stop Sync
		////		{
		////			controller_Spindle.stop();
		////			break;
		////		}
		////		}
		////	}

		////	delay(10);
		////}
	}
	////int32_t endPosition_Z = motor_Axis_Z.getPosition();
	////int32_t endPosition_Sp = motor_Spindle.getPosition();

	Serial1.print("pageZiZa_Z.t9.txt=");
	Serial1.write(0x22);
	Serial1.print(axisSteps);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageZiZa_Z.t9.txt=");
	Serial1.write(0x22);
	Serial1.print(axisSteps*(configSteppers.repeats_ZiZa_Z * 2));
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageZiZa_Z.t12.txt=");
	Serial1.write(0x22);
	Serial1.print(spindleSteps);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
/*	Serial1.print("pageZiZa_Z.t10.txt=");
	Serial1.write(0x22);
	Serial1.print(round(returnSteps_ZiZa_Z));
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageZiZa_Z.t8.txt=");
	Serial1.write(0x22);
	Serial1.print(endPosition_Z);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageZiZa_Z.t9.txt=");
	Serial1.write(0x22);
	Serial1.print(returnSteps_ZiZa_Z);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	*/
	// Update Nextion
	Serial1.print("pageZiZa_Z.bt3.pco=0");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageZiZa_Z.va0.val=0");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageZiZa_Z.bt3.val=0");// The first one may be ignored by Nextion, so resend.
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageZiZa_Z.bt2.val=0");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageZiZa_Z.bt1.val=0");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
}

void ggoodDoWaveZ(int wavDir)
{
	int32_t startPosition_Z = motor_Axis_Z.getPosition();
	int32_t startPosition_Sp = motor_Spindle.getPosition();

	targetPositions[0] = configSteppers.amplitude_ZiZa_Z;
	targetPositions[1] = 0;
	targetPositions[2] = configSteppers.amplitude_ZiZa_Z;
	targetPositions[3] = 0;
	targetPositions[4] = configSteppers.amplitude_ZiZa_Z * 2;
	targetPositions[5] = 0;
	targetPositions[6] = configSteppers.amplitude_ZiZa_Z;
	targetPositions[7] = 0;
	nrOfPositions = configSteppers.repeats_ZiZa_Z;

	motor_Axis_Z.setAcceleration(configSteppers.acceleration_ZiZa_Z_Axis);
	int axisSpeed = configSteppers.maxSpeed_ZiZa_Z_Axis * wavDir;
	motor_Axis_Z.setMaxSpeed(axisSpeed);
	controller_Axis.rotateAsync(motor_Axis_Z);

	motor_Spindle.setAcceleration(configSteppers.acceleration_ZiZa_Z_Spindle);
	motor_Spindle.setMaxSpeed(configSteppers.maxSpeed_ZiZa_Z_Spindle);

	for (int i = 0; i < nrOfPositions + 2; i++)
	{
		if (i == nrOfPositions + 1)
		{
			controller_Axis.stop();
		}
		else
		{
			motor_Spindle.setTargetAbs(targetPositions[i]);
			controller_Spindle.move(motor_Spindle);
		}

		startPosition_Z = motor_Axis_Z.getPosition();
		startPosition_Sp = motor_Spindle.getPosition();
		Serial1.print("pageWaveZ.t9.txt=");
		Serial1.write(0x22);
		Serial1.print(i);
		Serial1.write(0x22);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.print("pageWaveZ.t9.txt=");
		Serial1.write(0x22);
		Serial1.print(i);
		Serial1.write(0x22);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.print("pageWaveZ.t12.txt=");
		Serial1.write(0x22);
		Serial1.print(startPosition_Z);
		Serial1.write(0x22);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.print("pageWaveZ.t10.txt=");
		Serial1.write(0x22);
		Serial1.print(startPosition_Sp);
		Serial1.write(0x22);
		Serial1.write(0xff);
		Serial1.write(0xff);
		Serial1.write(0xff);
	}


	Serial1.print("pageWaveZ.t9.txt=");
	Serial1.write(0x22);
	Serial1.print("fin");
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageWaveZ.t9.txt=");
	Serial1.write(0x22);
	Serial1.print("fin");
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);

	motor_Axis_Z.setPosition(0);
	motor_Spindle.setPosition(0);

}

/*
void xDoWaveZ(int wavDir)
{

	motor_Axis_X.setPosition(0);
	motor_Spindle.setPosition(0);
	delay(10);
	int32_t startPosition_Z = motor_Axis_Z.getPosition();
	cycleCounter = 1;
	motor_Spindle.setAcceleration(configSteppers.acceleration_ZiZa_Z_Spindle);
	motor_Axis_Z.setAcceleration(configSteppers.acceleration_ZiZa_Z_Axis);

	cycleWidth = 20000; // steps
						//cycleWidth = (configSteppers.distance_ZiZa_Z / configSteppers.distancePerRev_AxisZ) * configSteppers.microsteps_Axis_Z * configSteppers.steps360_Axis_Z;
						//maxSpeed = 30000;
						//numberOfCycles = 3;

	currentstate = state::starting;


	//for (int i = 0; i <= numberOfCycles; i++)
	while (cycleCounter <= configSteppers.repeats_ZiZa_Z)
	{
		if (stopwatch > 300)
		{
			stopwatch = 0;
			digitalWriteFast(LED_BUILTIN, !digitalReadFast(LED_BUILTIN));

		}
		tick();
	}
	int32_t endPosition_Z = motor_Axis_Z.getPosition();

	Serial1.print("pageWaveZ.t9.txt=");
	Serial1.write(0x22);
	Serial1.print(startPosition_Z);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageWaveZ.t9.txt=");
	Serial1.write(0x22);
	Serial1.print(startPosition_Z);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageWaveZ.t12.txt=");
	Serial1.write(0x22);
	Serial1.print(endPosition_Z);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);


}

void tick()
{
	int position = motor_Spindle.getPosition();

	switch (currentstate)
	{
	case state::waiting:
		break;

	case state::starting:
		direction = 1;
		motor_Axis_Z.setMaxSpeed(configSteppers.maxSpeed_ZiZa_Z_Axis);
		controller_Axis.rotateAsync(motor_Axis_Z);
		// fallthrough intended

	case state::startingCycle:
		direction *= -1;
		motor_Spindle.setMaxSpeed(direction * configSteppers.maxSpeed_ZiZa_Z_Spindle);
		controller_Spindle.rotateAsync(motor_Spindle); // start spindle motor, as long as maxSpeed and acceleration of both motors are the same the will run in sync
		currentstate = state::runningCycle;
		break;

	case state::runningCycle:
		if (direction * position >= cycleWidth) // did the spindle motor reach the cycleWidth
		{
			Serial1.print("pageWaveZ.t10.txt=");
			Serial1.write(0x22);
			Serial1.print(position);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.print("pageWaveZ.t10.txt=");
			Serial1.write(0x22);
			Serial1.print(position);
			Serial1.write(0x22);
			Serial1.write(0xff);
			Serial1.write(0xff);
			Serial1.write(0xff);
			if (cycleCounter < configSteppers.repeats_ZiZa_Z) // cycles left? -> initiate new cycle
			{
				cycleCounter++;
				controller_Spindle.stopAsync();
				currentstate = state::stoppingCycle;
			}
			else // all cycles done, stopping both motors
			{
				controller_Spindle.stop();
				controller_Axis.stop();

				currentstate = state::stopping;
				cycleCounter = configSteppers.repeats_ZiZa_Z + 2;
			}
		}
		break;

	case state::stoppingCycle:
		if (!controller_Spindle.isRunning())
			currentstate = state::startingCycle;
		break;

	case state::stopping:
		if (!controller_Spindle.isRunning() && !controller_Axis.isRunning())
		{
			currentstate = state::waiting;
			Serial.println("done");
		}
		break;
	}
}
*/


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
	Serial1.print(text);
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
	else if (Serial1.available() > 0)
	{
		serialId = 1;
	}

	return serialId;
}

/// <summary>
/// Get a string from USB serial input
/// </summary>
/// <returns>String from USB serial input</returns>
String GetSerial1String()
{
	String newString = "";
	const int bufferLength = 20;
	char in[bufferLength];

	int i;
	for (i = 0; i <= bufferLength; i++)
	{
		in[i] = Serial1.read();
		newString = in;
		// Use 255 as end chars
		if (in[i] == 255)
		{
			break;
		}
	}

	String outString = in;
	return newString;
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

	// Convert filename string to char array
	const unsigned int strLength = filename_Index1.length();
	char charFileName[sizeof(filename_Index1)];
	filename_Index1.toCharArray(charFileName, strLength);

	File sdFile = SD.open(charFileName, FILE_READ);


	delay(100);
	while (sdFile.available())
	{
		for (int i = 0; i < 2; i++) // Verify Teensy is operational
		{
			digitalWrite(PIN_LED, HIGH);
			delay(500);
			digitalWrite(PIN_LED, LOW);
			delay(500);
		}
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

/// <summary>
/// Write current Config screen eeprom values to Nextion
/// </summary>
/// <comment>
/// </comment>
/// <returns></returns>
void TestEEPROMConfig()
{
	stepperConfig eePromConfig;
	EEPROM.get(eePromAddress, eePromConfig);
	delay(2000);

	// Write the setting to Nextion LCD
	// pageConfig
	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Config");
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Main-MaxSpeed:");
	Serial1.print(eePromConfig.maxSpeedSpindle_SpZ);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Main-Accel:");
	Serial1.print(eePromConfig.accelerationSpindle_SpZ);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);

	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Sync-MaxSpeed:");
	Serial1.print(eePromConfig.maxSpeedSpindle_SyncZ);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Sync-Accel:");
	Serial1.print(eePromConfig.accelerationSpindle_SyncZ);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);

	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Sp2-MaxSpeed:");
	Serial1.print(eePromConfig.maxSpeed_Sp2);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Sp2-Accel:");
	Serial1.print(eePromConfig.acceleration_Sp2);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);

	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Index-MaxSpeed:");
	Serial1.print(eePromConfig.maxSpeed_Index);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Index-Accel:");
	Serial1.print(eePromConfig.acceleration_Index);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);

	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Z Axis-MaxSpeed:");
	Serial1.print(eePromConfig.maxSpeed_Axis_Z);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Z Axis-Accel:");
	Serial1.print(eePromConfig.acceleration_Axis_Z);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);

#ifdef TEENSY_35


	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("X Axis-MaxSpeed:");
	Serial1.print(eePromConfig.maxSpeed_Axis_X);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("X Axis-Accel:");
	Serial1.print(eePromConfig.acceleration_Axis_X);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);

	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("B Axis-MaxSpeed:");
	Serial1.print(eePromConfig.maxSpeed_Axis_B);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("B Axis-Accel:");
	Serial1.print(eePromConfig.acceleration_Axis_B);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
#endif // TEENSY_35
}

/// <summary>
/// Write current Setup screen eeprom values to Nextion
/// </summary>
/// <comment>
/// </comment>
/// <returns></returns>
void TestEEPROMSetup()
{
	stepperConfig eePromConfig;
	EEPROM.get(eePromAddress, eePromConfig);
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

#ifdef TEENSY_35
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
	Serial1.print("pageSetup.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("B Axis-Gear Ratio:");
	Serial1.print(eePromConfig.gearRatio_AxisB);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
#endif // TEENSY_35
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