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
int RunTwoSteppersMain(
	int direction_Axis,
	int axisId,
	int direction_Spindle,
	int initialCaller
)
{
	// Axis Stop flag (Used to reset Nextion when limit switch is hit.)
	bool stopFlag = false;

	// axisId: Current selected axis
	// initialCaller: Spindle or axis
	bool stepper_Axis_Go = false;
	bool stepper_Spindle_Go = false;

	if (initialCaller < 2)// Update magic number when X axis works
	{
		stepper_Axis_Go = true;
	}
	else //if (initialCaller == 3)
	{
		stepper_Spindle_Go = true;
	}

	// Configure Axis
	float nextSpeed_Axis = configSteppers.speedPercentage_Axis * configSteppers.maxSpeed_Axis * .01;

	stepper_Axis.setMaxSpeed(nextSpeed_Axis);
	stepper_Axis.setCurrentPosition(0);
	long moveTo_Axis = 1000000000;
	if (direction_Axis == DIR_CCW)
	{
		moveTo_Axis = -moveTo_Axis;
	}
	stepper_Axis.moveTo(moveTo_Axis);
	stepper_Axis.enableOutputs();

	// Configure Spindle
	int spindleSpeedPercentage = configSteppers.speedPercentageSpindle_Main;
	int spindleMaxSpeed = configSteppers.maxSpeedSpindle_Main;

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

	while ((stepper_Spindle.distanceToGo() != 0) || (stepper_Axis.distanceToGo() !=0))
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
				configSteppers.speedPercentage_Axis = GetSerialInteger();
				EEPROM.put(eePromAddress, configSteppers);
				nextSpeed_Axis = configSteppers.speedPercentage_Axis  * configSteppers.maxSpeed_Axis * .01;
				stepper_Axis.setMaxSpeed(nextSpeed_Axis);
				stepper_Axis.setCurrentPosition(0);
				stepper_Axis.moveTo(1000000000);
				stepper_Axis_Go = true;
				break;
			}
			case 91: // [ - Axis counter clockwise
			{
				configSteppers.speedPercentage_Axis = GetSerialInteger();
				EEPROM.put(eePromAddress, configSteppers);
				nextSpeed_Axis = configSteppers.speedPercentage_Axis  * configSteppers.maxSpeed_Axis * .01;
				stepper_Axis.setMaxSpeed(nextSpeed_Axis);
				stepper_Axis.setCurrentPosition(0);
				stepper_Axis.moveTo(-1000000000);
				stepper_Axis_Go = true;
				break;
			}
			case 67: // C - Spindle Stop
			{
				stepper_Spindle.stop();
				stepper_Spindle.setCurrentPosition(0);
				if (!stepper_Axis_Go)
				{
					stepper_Axis.setCurrentPosition(0);
				}
				break;
			}
			case 97: // a - Axis speed
			{
				configSteppers.speedPercentage_Axis = GetSerialInteger();
				EEPROM.put(eePromAddress, configSteppers);
				nextSpeed_Axis = configSteppers.speedPercentage_Axis * configSteppers.maxSpeed_Axis * .01;
				stepper_Axis.setMaxSpeed(nextSpeed_Axis);
				break;
			}
			case 99: // c - Axis stop
			{
				stepper_Axis.stop();
				stepper_Axis.setCurrentPosition(0);
				if (!stepper_Spindle_Go)
				{
					stepper_Spindle.setCurrentPosition(0);
				}
				break;
			}
			case 107: // k - Spindle Clockwise
			{
				nextSpeed_Spindle = configSteppers.speedPercentageSpindle_Main * configSteppers.maxSpeedSpindle_Main * .01;
				stepper_Spindle.setMaxSpeed(nextSpeed_Spindle);
				stepper_Spindle.setCurrentPosition(0);
				stepper_Spindle.moveTo(1000000000);
				stepper_Spindle_Go = true;
				break;
			}
			case 108: // l - Spindle Counterclockwise
			{
				nextSpeed_Spindle = configSteppers.speedPercentageSpindle_Main * configSteppers.maxSpeedSpindle_Main * .01;
				stepper_Spindle.setMaxSpeed(nextSpeed_Spindle);
				stepper_Spindle.setCurrentPosition(0);
				stepper_Spindle.moveTo(-1000000000);
				stepper_Spindle_Go = true;
				break;
			}
			case 110: // n - Set Spindle Speed
			{
				configSteppers.speedPercentageSpindle_Main = GetSerialInteger();
				float nextSpeed = configSteppers.speedPercentageSpindle_Main * configSteppers.maxSpeedSpindle_Main * .01;
				stepper_Spindle.setMaxSpeed(nextSpeed);
				EEPROM.put(eePromAddress, configSteppers);
				break;
			}
			}
		}

		if (stepper_Axis.distanceToGo() > 0)
		{

			if (digitalRead(PIN_AXIS_LIMIT_MAX) == LOW)
			{
				stepper_Axis.stop();
				stopFlag = true;
				stepper_Axis_Go = false;
				if (!stepper_Spindle_Go)
				{
					stepper_Spindle.setCurrentPosition(0);
				}
			}
		}
		else
		{
			if (digitalRead(PIN_AXIS_LIMIT_MIN) == LOW)
			{
				stepper_Axis.stop();
				stopFlag = true;
				stepper_Axis_Go = false;
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

		if (stepper_Axis_Go)
		{
			// Check limit switches
			if (stepper_Axis.distanceToGo() > 0)
			{

				if (digitalRead(PIN_AXIS_LIMIT_MAX) == LOW)
				{ 
					stepper_Axis.stop();
					stopFlag = true;
					stepper_Axis_Go = false;
				}
			}
			else
			{
				if (digitalRead(PIN_AXIS_LIMIT_MIN) == LOW)
				{
					stepper_Axis.stop();
					stopFlag = true;
					stepper_Axis_Go = false;
				}
			}

			stepper_Axis.run();

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
	
	stepper_Axis_Go = false;
	stepper_Spindle_Go = false;
	stepper_Axis.setCurrentPosition(0);
	stepper_Spindle.setCurrentPosition(0);

	return 0;
}

/// <summary>
/// Run steppers
/// </summary>
/// <comment>
/// </comment>
/// <param name="direction">Counterclockwise: 0, Clockwise: 1</param>
/// <returns>Success: 0, Cancel/Stop: -1</returns>
int RunTwoSteppersSync(
	int direction_Axis,
	int direction_Spindle
)
{
	// Stop Flag
	bool stopFlag = false;

	//AccelStepper stepper_Axis;
	int acceleration_Axis = 0;
	int speedPercentage_Axis = 0;
	int maxSpeed_Axis = 0;

	acceleration_Axis = configSteppers.acceleration_Axis;
	speedPercentage_Axis = configSteppers.speedPercentage_Axis;
	maxSpeed_Axis = configSteppers.maxSpeed_Axis;

	// Configure Axis
	float nextSpeed_Axis = speedPercentage_Axis * maxSpeed_Axis * .01;
	stepper_Axis.setMaxSpeed(nextSpeed_Axis);
	stepper_Axis.setAcceleration(acceleration_Axis);
	stepper_Axis.setCurrentPosition(0);
	long moveTo_Axis = 1000000000;
	if (direction_Axis == 0)
	{
		moveTo_Axis = -moveTo_Axis;
	}

	stepper_Axis.moveTo(moveTo_Axis);
	stepper_Axis.enableOutputs();

	// Configure Spindle
	int spindleSpeedPercentage = configSteppers.speedPercentageSpindle_Sync;
	int spindleMaxSpeed = configSteppers.maxSpeedSpindle_Sync;
	stepper_Spindle.setAcceleration(configSteppers.accelerationSpindle_Sync);
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
				stepper_Axis.stop();
				stepper_Spindle.setCurrentPosition(0);
				stepper_Axis.setCurrentPosition(0);
				break;
			}
			}
		}

		if (direction_Axis == DIR_CW)
		{
			if (digitalRead(PIN_AXIS_LIMIT_MAX) == LOW)
			{
				stepper_Spindle.stop();
				stepper_Axis.stop();
				stepper_Spindle.setCurrentPosition(0);
				stepper_Axis.setCurrentPosition(0);
				stopFlag = true;
			}
		}
		else
		{
			if (digitalRead(PIN_AXIS_LIMIT_MIN) == LOW)
			{
				stepper_Spindle.stop();
				stepper_Axis.stop();
				stepper_Spindle.setCurrentPosition(0);
				stepper_Axis.setCurrentPosition(0);
				stopFlag = true;
			}
		}

		stepper_Axis.run();
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
/// Do a single index1 move
/// </summary>
/// <comment>
/// Cancel/Stop is not enabled on the Nextion
/// </comment>
/// <returns>Success: 0, Cancel/Stop: -1</returns>
int DoIndex1()
{
	int retVal = 0;
	stepper_Spindle.setAcceleration(configSteppers.acceleration_Index);
	float newSpeed = configSteppers.speedPercentage_Index1 * configSteppers.maxSpeed_Index * .01;
	stepper_Spindle.setMaxSpeed(newSpeed);

	// Set stepper move parameter
	long stepsToMove = configSteppers.size_Index1;
	if (moveDirection == DIR_CCW)
	{
		stepsToMove = -stepsToMove;
	}

	stepper_Spindle.setCurrentPosition(0);
	stepper_Spindle.move(stepsToMove);

	// Enable stepper
	////digitalWrite(PIN_SPINDLE_ENABLE, LOW);  // Enable stepper
	stepper_Spindle.enableOutputs();
	while (stepper_Spindle.distanceToGo() != 0)
	{
		// Check for Cancel code  
		//if (SerialAvailable() >= 0)
		//{
		//	incomingByte = SerialRead(serialId);
		//	switch (incomingByte)
		//	{
		//		case 67: // C
		//		case 99:
		//		{
		//			////digitalWrite(PIN_LED, HIGH);
		//			////delay(250);
		//			////digitalWrite(PIN_LED, LOW);
		//			stepper_Spindle.setCurrentPosition(0);
		//			retVal = -1;
		//			break;
		//		}
		//		break;
		//	}
		//}

		stepper_Spindle.run();
	}

	// Update Index flag on Nextion. 
	Serial1.print("pageIndex1.b6.pco=59391"); 
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageIndex1.va0.val=0");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageIndex1.b6.pco=59391");// The first one may be ignored by Nextion, so resend.
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageIndex1.b7.pco=59391");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);

	return retVal;
}

/// <summary>
/// Do a single index2 move 
/// </summary>
/// <comment>
/// Cancel/Stop is not enabled on the Nextion
/// </comment>
/// <returns>Success: 0, Cancel/Stop: -1</returns>
int DoIndex2()
{
	int retVal = 0;
	stepper_Spindle.setAcceleration(configSteppers.acceleration_Index);
	float newSpeed = configSteppers.speedPercentage_Index2 * configSteppers.maxSpeed_Index * .01;
	stepper_Spindle.setMaxSpeed(newSpeed);

	// Set stepper move parameter
	long stepsToMove = configSteppers.size_Index2;
	if (moveDirection == DIR_CCW)
	{
		stepsToMove = -stepsToMove;
	}

	stepper_Spindle.setCurrentPosition(0);
	stepper_Spindle.move(stepsToMove);

	// Enable stepper
	////digitalWrite(PIN_SPINDLE_ENABLE, LOW);  // Enable stepper
	stepper_Spindle.enableOutputs();
	while (stepper_Spindle.distanceToGo() != 0)
	{
		// Check for Cancel code  
		////if (SerialAvailable() >= 0)
		////{
		////	incomingByte = SerialRead(serialId);
		////	switch (incomingByte)
		////	{
		////		case 67: // C
		////		case 99:
		////		{
		////			//digitalWrite(PIN_LED, HIGH);
		////			//delay(250);
		////			//digitalWrite(PIN_LED, LOW);
		////			stepper_Spindle.setCurrentPosition(0);
		////			retVal = -1;
		////			break;
		////		}
		////		break;
		////	}
		////}

		stepper_Spindle.run();
	}

	// Update Index flag on Nextion.
	Serial1.print("pageIndex2.b6.pco=59391");// The first one may be ignored by Nextion, so resend.
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageIndex2.b6.pco=59391");// The first one may be ignored by Nextion, so resend.
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.print("pageIndex2.b7.pco=59391");
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);

	return retVal;
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
/// Write current eeprom values to Nextion screen
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
	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Sp1");
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("MaxSpeed:");
	Serial1.print(eePromConfig.maxSpeedSpindle_Main);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Accel:");
	Serial1.print(eePromConfig.accelerationSpindle_Main);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Microsteps:");
	Serial1.print(eePromConfig.microsteps_Spindle);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Gear Ratio:");
	Serial1.print(eePromConfig.gearRatio_Spindle);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Steps360:");
	Serial1.print(eePromConfig.steps360_Spindle);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);

	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Sp2");
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("MaxSpeed:");
	Serial1.print(eePromConfig.maxSpeed_Sp2);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Accel:");
	Serial1.print(eePromConfig.acceleration_Sp2);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);

	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Index");
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("MaxSpeed:");
	Serial1.print(eePromConfig.maxSpeed_Index);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Accel:");
	Serial1.print(eePromConfig.acceleration_Index);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);

	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Sync");
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("MaxSpeed:");
	Serial1.print(eePromConfig.maxSpeedSpindle_Sync);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Accel:");
	Serial1.print(eePromConfig.accelerationSpindle_Sync);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);

	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Axis");
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);

	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("MaxSpeed:");
	Serial1.print(eePromConfig.maxSpeed_Axis);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Accel:");
	Serial1.print(eePromConfig.acceleration_Axis);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Microsteps:");
	Serial1.print(eePromConfig.microsteps_Axis);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("GearRatio:");
	Serial1.print(eePromConfig.gearRatio_Axis);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);
	Serial1.print("pageConfig.t19.txt=");
	Serial1.write(0x22);
	Serial1.print("Steps360:");
	Serial1.print(eePromConfig.steps360_Axis);
	Serial1.write(0x22);
	Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0xff);
	delay(2000);

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