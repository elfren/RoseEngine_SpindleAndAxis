/* *****************************************************************
* Rose Engine utility methods
* Author: Edward French
******************************************************************/

// ================================================================================= 
// Utility Methods
// ================================================================================= 

/// <summary>
/// MilliDelay
/// </summary>
/// <comment>
/// Uses elapsedMillis to pause program.  Allows interupts.
/// </comment>
/// <param name="interval">Milliseconds</param>
/// <returns>void</returns>
void MilliDelay(unsigned long interval)
{
	elapsedMillis timeout;
	while (timeout < interval)
	{
	}
}

/// <summary>
/// structSizeCalc
/// </summary>
/// <comment>
/// Calculate struct sizes.  Used to calculate EEProm addresses.
/// </comment>
/// <param name="interval">Milliseconds</param>
/// <returns>void</returns>
void structSizeCalc()
{
	const char * configIndex_Char = "configIndex:";
	const char * configPageMov_Char = "configPageMov:";
	const char * configPageSync_Char = "configPageSync:";
	const char * configPageSetup_Char = "configPageSetup:";
	const char * configSteppers_Char = "configSteppers:";
	const char * configPageReci_Char = "configPageReci:";
	const char * configPageRose_Char = "configPageRose:";
	const char * configPageMainOne_Char = "configPageMainOne:";
	const char * configPageGreekKey_Char = "configPageGreekKey:";
	const char * configPageIndex_Char = "configPageIndex:";

	Serial.print(configIndex_Char);
	int sizeS = sizeof(configIndex);
	Serial.println(sizeS);
	Serial.print(configPageMov_Char);
	sizeS = sizeof(configPageMov);
	Serial.println(sizeS);
	Serial.print(configPageSync_Char);
	sizeS = sizeof(configPageSync);
	Serial.println(sizeS);

	Serial.print(configPageSetup_Char);
	sizeS = sizeof(configPageSetup);
	Serial.println(sizeS);
	Serial.print(configSteppers_Char);
	sizeS = sizeof(configSteppers);
	Serial.println(sizeS);
	Serial.print(configPageReci_Char);
	sizeS = sizeof(configPageRec);
	Serial.println(sizeS);
	Serial.print(configPageRose_Char);
	sizeS = sizeof(configPageRose);
	Serial.println(sizeS);
	Serial.print(configPageMainOne_Char);
	sizeS = sizeof(configPageMainOne);
	Serial.println(sizeS);
	Serial.print(configPageGreekKey_Char);
	sizeS = sizeof(configPageGreekKey);
	Serial.println(sizeS);
	Serial.print(configPageIndex_Char);
	sizeS = sizeof(configPageIndex);
	Serial.println(sizeS);

}


//int freeMemory()
//{
//	char top;
//#ifdef __arm__
//	return &top - reinterpret_cast<char*>(sbrk(0));
//#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
//	return &top - __brkval;
//#else  // __arm__
//	return __brkval ? &top - __brkval : &top - __malloc_heap_start;
//#endif  // __arm__
//}

/// <summary>
/// BeginSD
/// </summary>
/// <comment>
/// Initiate microSD
/// </comment>
/// <param name=""></param>
/// <returns>void</returns>
void BeginSD()
{
	// Note: Each failed check requires 2 seconds.  Recommend using CS 10 for SPI card reader.
#ifdef DEBUG
	Serial.println("SD..begin");
#endif // DEBUG
	if (SD.begin(BUILTIN_SDCARD)) // Builtin microSD card
	{
		microSD_Id = 1;
#ifdef DEBUG
		Serial.println("microSD-1");
#endif // DEBUG
	}
	else if (SD.begin(PIN_SPI_CS_10)) // SPI microSD CS on pin 10
	{
		microSD_Id = 10;
#ifdef DEBUG
		Serial.println("microSD-10");
#endif // DEBUG
	}
	else if (SD.begin(PIN_SPI_CS_24)) // SPI microSD CS on pin 24
	{
		microSD_Id = 24;
#ifdef DEBUG
		Serial.println("microSD-24");
#endif // DEBUG
	}
	else if (SD.begin(PIN_SPI_CS_15)) // SPI microSD CS on pin 15
	{
		microSD_Id = 15;
#ifdef DEBUG
		Serial.println("microSD-15");
#endif // DEBUG
	}

	else if (SD.begin(PIN_SPI_CS_9)) // SPI microSD CS on pin 9
	{
		microSD_Id = 9;
#ifdef DEBUG
	Serial.println("microSD-9");
#endif // DEBUG
		
	}

	else // No card
	{
		microSD_Id = 0;
	}
#ifdef DEBUG
	Serial.print("microSD_Id: ");
	Serial.println(microSD_Id);
#endif // DEBUG
}

/// <summary>
/// SetEnable
/// </summary>
/// <comment>
/// Toggle stepper motor enable on and off
/// </comment>
/// <param name="axisId">Axis Id</param>
/// <param name="enabled">true to enable, false to disable</param>
/// <returns>void</returns>
void SetEnable(int axisId, bool enabled)
{
	switch (axisId)
	{
		case ID_SPINDLE:
		{
			if (enabled)
			{
				digitalWrite(PIN_SPINDLE_ENABLE, configSetup.polarity_Spindle ? (LOW) : (HIGH)); // Enable 
			}
			else
			{
				digitalWrite(PIN_SPINDLE_ENABLE, configSetup.polarity_Spindle ? (HIGH) : (LOW)); // Disable 
			}
			break;
		}
		case ID_AXIS_Z:
		{
			if (enabled)
			{
				digitalWrite(PIN_AXIS_Z_ENABLE, configSetup.polarity_Axis_Z ? (LOW) : (HIGH)); // Enable 
			}
			else
			{
				digitalWrite(PIN_AXIS_Z_ENABLE, configSetup.polarity_Axis_Z ? (HIGH) : (LOW)); // Disable 
			}
			break;
		}
		case ID_AXIS_X:
		{
			if (enabled)
			{
				digitalWrite(PIN_AXIS_X_ENABLE, configSetup.polarity_Axis_X ? (LOW) : (HIGH)); // Enable 
			}
			else
			{
				digitalWrite(PIN_AXIS_X_ENABLE, configSetup.polarity_Axis_X ? (HIGH) : (LOW)); // Disable 
			}
			break;
		}
		case ID_AXIS_B:
		{
			if (enabled)
			{
				digitalWrite(PIN_AXIS_B_ENABLE, configSetup.polarity_Axis_B ? (LOW) : (HIGH)); // Enable 
			}
			else
			{
				digitalWrite(PIN_AXIS_B_ENABLE, configSetup.polarity_Axis_B ? (HIGH) : (LOW)); // Disable 
			}
			break;
		}
	}
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
			retVal = GetSerialFloatA(Serial1);
			break;
		}
		case 2:
		{
			retVal = GetSerialFloatA(Serial2);
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
/// Get a float from serial input
/// </summary>
/// <returns>Float from serial input</returns>
double GetSerialFloatA(Stream& port)
{
	const int bufferLength = 10;
	char in[bufferLength];
	char inputChar;

	int i;
	for (i = 0; i <= bufferLength; i++)
	{
		//in[i] = port.read();
		inputChar = port.read();
		Serial.println("");
		Serial.print("InputChar:---------------------------------------");
		Serial.print(inputChar);
		Serial.print("  ");
		Serial.println(int(inputChar));
		// Use 255 as end chars
		if (inputChar == 255)
		{
			// Read and discard anything more on serial.
			while (port.available() > 0)
			{
				port.read();
			}
			break;
		}
		else
		{
			in[i] = inputChar;
		}

	}

	double retVal = atof(in);

#ifdef DEBUG
	Serial.print("Float: ");
	Serial.println(retVal, 4);
#endif //DEBUG
	return retVal;
}

///// Get a float from serial input
///// </summary>
///// <returns>Float from serial input</returns>
//double xxGetSerialFloatA(Stream &port )
//{
//	const int bufferLength = 10;
//	char in[bufferLength];
//	int decimalFound = -1;
//	int endFound = -1;
//	int i;
//	for (i = 0; i <= bufferLength; i++)
//	{
//		in[i] = port.read();
//
//		// Use 255 as end chars
//		if (in[i] == 255)
//		{
//			endFound = i;
//			// Read and discard anything more on serial.
//			while (port.available() > 0)
//			{
//				port.read();
//			}
//			break;
//		}
//		if (in[i] == 46)
//		{
//			decimalFound = i;
//		}
//	}
//
//
//
//	return GetFloatFromCharArray(in, decimalFound, endFound);
//}
//
///// <summary>
///// Get a float from serial input
///// </summary>
///// <returns>Float from serial input</returns>
//double xGetSerialFloatA(Stream &port )
//{
//	const int bufferLength = 10;
//	char in[bufferLength];
//	int decimalFound = -1;
//	int endFound = -1;
//	int i;
//	for (i = 0; i <= bufferLength; i++)
//	{
//		in[i] = port.read();
//
//		// Use 255 as end chars
//		if (in[i] == 255)
//		{
//			endFound = i;
//			// Read and discard anything more on serial.
//			while (port.available() > 0)
//			{
//				port.read();
//			}
//			break;
//		}
//		if (in[i] == 46)
//		{
//			decimalFound = i;
//		}
//	}
//
//	return GetFloatFromCharArray(in, decimalFound, endFound);
//}

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
	inputHex = SerialRead(serialId);

	int newInteger = (int)inputHex ;
	while (SerialAvailable() > 0)
	{
		// Discard remaining serial chars
		SerialRead(serialId);
	}

#ifdef DEBUG
	const char * int_Char = "Int:";
	Serial.print(int_Char);
	Serial.println(newInteger);
#endif // DEBUG
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
	inputHex = SerialRead(serialId);

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
double xGetFloatFromCharArray(char in[], int decimalFound, int endFound)
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
	const char * float_Char = "Float:";
	Serial.println(in);
	Serial.print(float_Char);
	Serial.println(inputNumber);
#endif //DEBUG
	return inputNumber;
}

/// <summary>
/// GetFloatFromCharArray
/// </summary>
/// <comment>
/// Convert Char array to float
/// </comment>
/// <param name="in">char array</param>
/// <param name="decimalFound">Index of decimal</param>
/// <param name="decimalFound">Index of end char</param>
/// <returns>double</returns>
double GetFloatFromCharArray(char in[], int decimalFound, int endFound)
{
	double retVal = 0;
	retVal = atof(in);

#ifdef DEBUG
	Serial.println("");
	Serial.print("Input-----------:");
	Serial.println(in);
	Serial.print("Float-----------:");
	Serial.println(retVal);
#endif //DEBUG
	return retVal;
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
/// RevolutionsToSteps_Spindle
/// </summary>
/// <comment>
/// Convert spindle revolutions to steps
/// </comment>
/// <param name="revolutions">revolutions</param>
/// <param name="direction">Direction of revolutions</param>
/// <returns>float</returns>
float RevolutionsToSteps_Spindle(float revolutions, int direction)
{
	float retVal = revolutions * configSetup.gearRatio_Spindle * configSetup.microsteps_Spindle * configSetup.steps360_Spindle * direction;

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
		case ID_AXIS_B: // Rotary axis
		{
			switch (configSetup.radialOrLinear_Axis_B)
			{
				case RADIAL_B:
				{
					retVal = (steps / (configSetup.steps360_Axis_B * configSetup.microsteps_Axis_B * configSetup.gearRatio_AxisB)) * 360;
				}
				case LINEAR_B:
				{
					retVal = (steps * configSetup.distancePerRev_AxisB) / (configSetup.steps360_Axis_B * configSetup.microsteps_Axis_B);
				}
			}
			break;
		}
	}
	return retVal;
}

/// <summary>
/// DistanceToSteps_RadialB
/// </summary>
/// <comment>
/// Convert distance to steps for axis B in radial mode.
/// </comment>
/// <param name="linearDistance">Distance</param>
/// <returns>float</returns>
float DistanceToSteps_RadialB(float linearDistance)
{
	float retVal = 0;

	float stepsPer360 = configSetup.steps360_Axis_B* configSetup.microsteps_Axis_B* configSetup.gearRatio_AxisB;
	float circumference = 2 * M_PI * configSetup.radiusB;
	float angle = linearDistance / circumference;
	retVal = stepsPer360 * angle;
#ifdef DEBUG
	Serial.print("stepsPer360: ");
	Serial.println(stepsPer360);
	Serial.print("gearRatio_AxisB: ");
	Serial.println(configSetup.gearRatio_AxisB);
	Serial.print("circumference: ");
	Serial.println(circumference);
	Serial.print("linearDistance: ");
	Serial.println(linearDistance);
	Serial.print("angle: ");
	Serial.println(angle);
	Serial.print("retVal: ");
	Serial.println(retVal);
#endif // DEBUG
	return retVal;
}

/// <summary>
/// DistanceToSteps_LinearB
/// </summary>
/// <comment>
/// Convert distance to steps for axis B in linear mode.
/// </comment>
/// <param name="distance">Distance</param>
/// <returns>float</returns>
float DistanceToSteps_LinearB(float distance)
{

	//float retVal = (configMove.distance_MoveB / (configSetup.distancePerRev_AxisB) * (configSetup.steps360_Axis_B * configSetup.microsteps_Axis_B));
	float retVal = (distance / (configSetup.distancePerRev_AxisB) * (configSetup.steps360_Axis_B * configSetup.microsteps_Axis_B));
	Serial.print("DistanceToSteps_LinearB: ");
	Serial.println(retVal);
	
	return retVal;
}

/// <summary>
/// DistanceToSteps_Axis
/// </summary>
/// <comment>
/// Convert distance to steps for axis.
/// </comment>
/// <param name="distance">Distance</param>
/// <param name="axisId">Axis Id</param>
/// <returns>float</returns>
float DistanceToSteps_Axis(float distance, int axisId)
{
	float retVal = 0;
	switch (axisId)
	{
		case ID_AXIS_Z:
		{
			retVal = (distance / configSetup.distancePerRev_AxisZ)* (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z);
			break;
		}
		case ID_AXIS_X:
		{
			retVal = (distance / configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X);
			
			break;
		}
		case ID_AXIS_B:
		{

			switch (configSetup.radialOrLinear_Axis_B)
			{
				case RADIAL_B: // Radial
				{
					retVal = DistanceToSteps_RadialB(distance);

					break;
				}
				case LINEAR_B: // Linear
				{
					retVal = DistanceToSteps_LinearB(distance);
					break;
				}
			}
			break;
		}
	}

	return retVal;
}

/// <summary>
/// AngularMaxSpeed
/// </summary>
/// <comment>
/// Calculate max speeds for Greek Key angular moves.
/// </comment>
/// <param name="spindleMaxSpeed">Spindle Max Speed</param>
/// <param name="axisMaxSpeed">Axis Max Speed</param>
/// <returns>void</returns>
void AngularMaxSpeed(long spindleMaxSpeed, long axisMaxSpeed)
{

	// Pathagorean Theorem
	if (angularSpindleLegLength < 0)
	{
		angularSpindleLegLength = angularSpindleLegLength * (-1);
	}
	if (angularAxisLegLength < 0)
	{
		angularAxisLegLength = angularAxisLegLength * (-1);
	}
	float hypotenuse = sqrt((angularSpindleLegLength * angularSpindleLegLength) + (angularAxisLegLength * angularAxisLegLength));
	float axisMultiplier = hypotenuse / angularAxisLegLength;
	float spindleMultiplier = hypotenuse / angularSpindleLegLength;
	angularAxisLegLength = round(axisMaxSpeed * axisMultiplier);
	angularSpindleLegLength = round(spindleMaxSpeed * spindleMultiplier);

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
	Stepper stepper_Z(PIN_AXIS_Z_STEP, PIN_AXIS_Z_DIR);
	Stepper stepper_X(PIN_AXIS_X_STEP, PIN_AXIS_X_DIR);
	Stepper stepper_B(PIN_AXIS_B_STEP, PIN_AXIS_B_DIR);
	double stepsToMove = 0;
	float nextSpeed = 0;

	const char * distancePer360_Char = "Distance/360:";
	const char * stepsPer360_Char = "Steps/360:";
	const char * microsteps_Char = "Microsteps:";
	const char * distance_Char = "Distance:";
	const char * steps_Char = "Steps:";
	const char * maxSpd_Char = "Max Speed:";
	const char * speedPercent_Char = "Speed Percent:";
	const char * nextSpeed_Char = "Next Speed:";
	const char * accel_Char = "Accel:";
	const char * position_Char = "Position:";
	//const char * pageMove_bt2_pco_Char = "pageMove.bt2.pco = 59391";
	const char * pageMove_bt2_val_Char = "pageMove.bt2.val=0";
	const char * pageMove_va0_Char = "pageMove.va0.val=0";
	const char * pageMove_bt1_Char = "pageMove.bt1.val=0";
	const char * pageMove_bt3_Char = "pageMove.bt3.val=0";
	const char * pageMove_t6_Char = "pageMove.t6.txt=";

	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	const char* nextionEnd = "\xFF\xFF\xFF";
	Serial.println("AxisId: =============================================");
	Serial.println(axisId);
	switch (axisId)
	{
		case ID_MOVE_AXIS_Z1:
		{
			stepper_Z.setPosition(0);
			stepsToMove = (configMove.distance_MoveZ1 / (configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z));

			// Set speed and acceleration
			nextSpeed = configMove.speedPercent_Axis_Z * configMove.maxSpd_Axis_Z * .01;
			stepper_Z
				.setMaxSpeed(nextSpeed)
				.setAcceleration(configMove.accel_Axis_Z)
				.setTargetRel(stepsToMove * directionAxis);

#ifdef DEBUG
			Serial.println("AxisId: =============================================");
			Serial.println(axisId);
			Serial.print(distancePer360_Char);
			Serial.println(configSetup.distancePerRev_AxisZ);
			Serial.print(stepsPer360_Char);
			Serial.println(configSetup.steps360_Axis_Z);
			Serial.print(microsteps_Char);
			Serial.println(configSetup.microsteps_Axis_Z);
			Serial.print(distance_Char);
			Serial.println(configMove.distance_MoveZ1);

			Serial.print(steps_Char);
			Serial.println(stepsToMove);

			Serial.print(maxSpd_Char);
			Serial.println(configMove.maxSpd_Axis_Z);
			Serial.print(speedPercent_Char);
			Serial.println(configMove.speedPercent_Axis_Z);
			Serial.print(nextSpeed_Char);
			Serial.println(nextSpeed);
			Serial.print(accel_Char);
			Serial.println(configMove.accel_Axis_Z);
#endif // DEBUG

			SetEnable(ID_AXIS_Z, true);
			stepController.moveAsync(stepper_Z);
			break;
		}
		case ID_MOVE_AXIS_Z2:
		{
			stepper_Z.setPosition(0);
			stepsToMove = (configMove.distance_MoveZ2 / (configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z));

			// Set speed and acceleration
			nextSpeed = configMove.speedPercent_Axis_Z * configMove.maxSpd_Axis_Z * .01;
			stepper_Z
				.setMaxSpeed(nextSpeed)
				.setAcceleration(configMove.accel_Axis_Z)
				.setTargetRel(stepsToMove * directionAxis);

#ifdef DEBUG
			Serial.println("AxisId: =============================================");
			Serial.println(axisId);
			Serial.print(distancePer360_Char);
			Serial.println(configSetup.distancePerRev_AxisZ);
			Serial.print(stepsPer360_Char);
			Serial.println(configSetup.steps360_Axis_Z);
			Serial.print(microsteps_Char);
			Serial.println(configSetup.microsteps_Axis_Z);
			Serial.print(distance_Char);
			Serial.println(configMove.distance_MoveZ2);

			Serial.print(steps_Char);
			Serial.println(stepsToMove);

			Serial.print(maxSpd_Char);
			Serial.println(configMove.maxSpd_Axis_Z);
			Serial.print(speedPercent_Char);
			Serial.println(configMove.speedPercent_Axis_Z);
			Serial.print(nextSpeed_Char);
			Serial.println(nextSpeed);
			Serial.print(accel_Char);
			Serial.println(configMove.accel_Axis_Z);
#endif // DEBUG

			SetEnable(ID_AXIS_Z, true);
			stepController.moveAsync(stepper_Z);
			break;
		}

		case ID_MOVE_AXIS_X1:
		{
			stepsToMove = (configMove.distance_MoveX1 / (configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X));
			stepper_X.setPosition(0);

			// Set speed and acceleration
			nextSpeed = configMove.speedPercent_Axis_X * configMove.maxSpd_Axis_X * .01;
			stepper_X
				.setMaxSpeed(nextSpeed)
				.setAcceleration(configMove.accel_Axis_X)
				.setTargetRel(stepsToMove * directionAxis);

#ifdef DEBUG
			Serial.print(distancePer360_Char);
			Serial.println(configSetup.distancePerRev_AxisX);
			Serial.print(stepsPer360_Char);
			Serial.println(configSetup.steps360_Axis_X);
			Serial.print(microsteps_Char);
			Serial.println(configSetup.microsteps_Axis_X);
			Serial.print(distance_Char);
			Serial.println(configMove.distance_MoveX1);

			Serial.print(steps_Char);
			Serial.println(stepsToMove);

			Serial.print(maxSpd_Char);
			Serial.println(configMove.maxSpd_Axis_X);
			Serial.print(speedPercent_Char);
			Serial.println(configMove.speedPercent_Axis_X);
			Serial.print(nextSpeed_Char);
			Serial.println(nextSpeed);
			Serial.print(accel_Char);
			Serial.println(configMove.accel_Axis_X);
#endif // DEBUG

			SetEnable(ID_AXIS_X, true);
			stepController.moveAsync(stepper_X);
			break;
		}
		case ID_MOVE_AXIS_X2:
		{
			stepsToMove = (configMove.distance_MoveX2 / (configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X));
			stepper_X.setPosition(0);

			// Set speed and acceleration
			nextSpeed = configMove.speedPercent_Axis_X * configMove.maxSpd_Axis_X * .01;
			stepper_X
				.setMaxSpeed(nextSpeed)
				.setAcceleration(configMove.accel_Axis_X)
				.setTargetRel(stepsToMove * directionAxis);

#ifdef DEBUG
			Serial.print(distancePer360_Char);
			Serial.println(configSetup.distancePerRev_AxisX);
			Serial.print(stepsPer360_Char);
			Serial.println(configSetup.steps360_Axis_X);
			Serial.print(microsteps_Char);
			Serial.println(configSetup.microsteps_Axis_X);
			Serial.print(distance_Char);
			Serial.println(configMove.distance_MoveX2);

			Serial.print(steps_Char);
			Serial.println(stepsToMove);

			Serial.print(maxSpd_Char);
			Serial.println(configMove.maxSpd_Axis_X);
			Serial.print(speedPercent_Char);
			Serial.println(configMove.speedPercent_Axis_X);
			Serial.print(nextSpeed_Char);
			Serial.println(nextSpeed);
			Serial.print(accel_Char);
			Serial.println(configMove.accel_Axis_X);
#endif // DEBUG

			SetEnable(ID_AXIS_X, true);
			stepController.moveAsync(stepper_X);
			break;
		}

		case ID_MOVE_AXIS_B1:
		{
			if (configSetup.radialOrLinear_Axis_B==RADIAL_B)
			{
				stepsToMove = DistanceToSteps_RadialB(configMove.distance_MoveB1);
			}
			else
			{
				stepsToMove = DistanceToSteps_LinearB(configMove.distance_MoveB1);
			}

			stepper_B.setPosition(0);

			// Set speed and acceleration
			nextSpeed = configMove.speedPercent_Axis_B * configMove.maxSpd_Axis_B * .01;
			stepper_B
				.setMaxSpeed(nextSpeed)
				.setAcceleration(configMove.accel_Axis_B)
				.setTargetRel(stepsToMove * directionAxis);

#ifdef DEBUG
			Serial.print(distancePer360_Char);
			Serial.println(configSetup.distancePerRev_AxisB);
			Serial.print(stepsPer360_Char);
			Serial.println(configSetup.steps360_Axis_B);
			Serial.print(microsteps_Char);
			Serial.println(configSetup.microsteps_Axis_B);
			Serial.print(distance_Char);
			Serial.println(configMove.distance_MoveB1,4);

			Serial.print(steps_Char);
			Serial.println(stepsToMove);

			Serial.print(maxSpd_Char);
			Serial.println(configMove.maxSpd_Axis_B);
			Serial.print(speedPercent_Char);
			Serial.println(configMove.speedPercent_Axis_B);
			Serial.print(nextSpeed_Char);
			Serial.println(nextSpeed);
			Serial.print(accel_Char);
			Serial.println(configMove.accel_Axis_B);
#endif // DEBUG

			SetEnable(ID_AXIS_B, true);
			stepController.moveAsync(stepper_B);
			break;
		}
		case ID_MOVE_AXIS_B2:
		{
			if (configSetup.radialOrLinear_Axis_B == RADIAL_B)
			{
				stepsToMove = DistanceToSteps_RadialB(configMove.distance_MoveB2);
			}
			else
			{
				stepsToMove = DistanceToSteps_LinearB(configMove.distance_MoveB2);
			}

			stepper_B.setPosition(0);

			// Set speed and acceleration
			nextSpeed = configMove.speedPercent_Axis_B * configMove.maxSpd_Axis_B * .01;
			stepper_B
				.setMaxSpeed(nextSpeed)
				.setAcceleration(configMove.accel_Axis_B)
				.setTargetRel(stepsToMove * directionAxis);

#ifdef DEBUG
			Serial.print(distancePer360_Char);
			Serial.println(configSetup.distancePerRev_AxisB);
			Serial.print(stepsPer360_Char);
			Serial.println(configSetup.steps360_Axis_B);
			Serial.print(microsteps_Char);
			Serial.println(configSetup.microsteps_Axis_B);
			Serial.print(distance_Char);
			Serial.println(configMove.distance_MoveB2, 4);

			Serial.print(steps_Char);
			Serial.println(stepsToMove);

			Serial.print(maxSpd_Char);
			Serial.println(configMove.maxSpd_Axis_B);
			Serial.print(speedPercent_Char);
			Serial.println(configMove.speedPercent_Axis_B);
			Serial.print(nextSpeed_Char);
			Serial.println(nextSpeed);
			Serial.print(accel_Char);
			Serial.println(configMove.accel_Axis_B);
#endif // DEBUG

			SetEnable(ID_AXIS_B, true);
			stepController.moveAsync(stepper_B);
			break;
		}
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

		//			SerialPrint("pageBE.t2.txt=");
		//			SerialWrite(0x22);
		//			SerialPrint(distance_Axis);
		//			SerialWrite(0x22);
		//			SerialWrite(0xff);
		//			SerialWrite(0xff);
		//			SerialWrite(0xff);
		//			MilliDelay(5);
		//			SerialPrint("pageBE.t2.txt=");
		//			SerialWrite(0x22);
		//			SerialPrint(distance_Axis);
		//			SerialWrite(0x22);
		//			SerialWrite(0xff);
		//			SerialWrite(0xff);
		//			SerialWrite(0xff);
		//			break;
		//		}
		//		case ID_AXIS_X:
		//		{
		//			endPosition_Axis = stepper_X.getPosition();
		//			distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);
		//			SerialPrint("pageBE.t2.txt=");
		//			SerialWrite(0x22);
		//			SerialPrint(distance_Axis);
		//			SerialWrite(0x22);
		//			SerialWrite(0xff);
		//			SerialWrite(0xff);
		//			SerialWrite(0xff);
		//			MilliDelay(5);
		//			SerialPrint("pageBE.t2.txt=");
		//			SerialWrite(0x22);
		//			SerialPrint(distance_Axis);
		//			SerialWrite(0x22);
		//			SerialWrite(0xff);
		//			SerialWrite(0xff);
		//			SerialWrite(0xff);
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

		MilliDelay(10);
#ifdef DEBUG

		if (iCounter % 50 == 0)
		{
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
					// ToDo:
					endPosition_Axis = stepper_B.getPosition();

					break;
				}
			}
#ifdef DEBUG
			Serial.print(position_Char);
			Serial.println(endPosition_Axis);
#endif // DEBUG
		}

		iCounter++;
#endif // DEBUG
	}
	// Update Move flag on Nextion. 
	SerialPrint(pageMove_va0_Char);
	SerialPrint(nextionEnd);
	SerialPrint(pageMove_bt1_Char);
	SerialPrint(nextionEnd);
	SerialPrint(pageMove_bt2_val_Char);
	SerialPrint(nextionEnd);
	SerialPrint(pageMove_bt3_Char);
	SerialPrint(nextionEnd);
	switch (axisId)
	{
		case ID_AXIS_Z:
		{
			SerialPrint(pageMove_t6_Char);
			SerialWrite(0x22);
			SerialPrint(distanceTotal_MoveZ);
			SerialPrint(nextionQuoteEnd);

			endPosition_Axis = stepper_Z.getPosition();
#ifdef DEBUG
			Serial.print(position_Char);
			Serial.println(endPosition_Axis);
#endif // DEBUG

#ifdef SHOW_POSITION 
			distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);
			SerialPrint("pageBE.t2.txt=");
			SerialWrite(0x22);
			SerialPrint(distance_Axis);
			SerialWrite(0x22);
			SerialWrite(0xff);
			SerialWrite(0xff);
			SerialWrite(0xff);
			MilliDelay(5);
			SerialPrint("pageBE.t2.txt=");
			SerialWrite(0x22);
			SerialPrint(distance_Axis);
			SerialWrite(0x22);
			SerialWrite(0xff);
			SerialWrite(0xff);
			SerialWrite(0xff);
			MilliDelay(5);
#endif // Show Position
			endPosition_Spindle = 0;

			SetEnable(ID_AXIS_Z, false);
			break;
		}
		case ID_AXIS_X:
		{
			SerialPrint(pageMove_t6_Char);
			SerialWrite(0x22);
			SerialPrint(distanceTotal_MoveX);
			SerialPrint(nextionQuoteEnd);

			endPosition_Axis = stepper_X.getPosition();
#ifdef DEBUG
			Serial.print(position_Char);
			Serial.println(endPosition_Axis);
#endif // DEBUG

#ifdef SHOW_POSITION 
	/*		distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_X);
			SerialPrint("pageBE.t2.txt=");
			SerialWrite(0x22);
			SerialPrint(distance_Axis);
			SerialWrite(0x22);
			SerialWrite(0xff);
			SerialWrite(0xff);
			SerialWrite(0xff);
			MilliDelay(5);
			SerialPrint("pageBE.t2.txt=");
			SerialWrite(0x22);
			SerialPrint(distance_Axis);
			SerialWrite(0x22);
			SerialWrite(0xff);
			SerialWrite(0xff);
			SerialWrite(0xff);
			MilliDelay(5);*/
#endif // Show Position
			endPosition_Spindle = 0;

			SetEnable(ID_AXIS_X, false);
			break;
		}
		case ID_AXIS_B:
		{
			// ToDo:
			//SerialPrint(pageMove_t6_Char);
			//SerialWrite(0x22);
			//SerialPrint(distanceTotal_MoveB);
			//SerialPrint(nextionQuoteEnd);

			endPosition_Axis = stepper_B.getPosition();
#ifdef DEBUG
			Serial.print(position_Char);
			Serial.println(endPosition_Axis);
#endif // DEBUG

#ifdef SHOW_POSITION 
			/*		distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_B);
					SerialPrint("pageBE.t2.txt=");
					SerialWrite(0x22);
					SerialPrint(distance_Axis);
					SerialWrite(0x22);
					SerialWrite(0xff);
					SerialWrite(0xff);
					SerialWrite(0xff);
					MilliDelay(5);
					SerialPrint("pageBE.t2.txt=");
					SerialWrite(0x22);
					SerialPrint(distance_Axis);
					SerialWrite(0x22);
					SerialWrite(0xff);
					SerialWrite(0xff);
					SerialWrite(0xff);
					MilliDelay(5);*/
#endif // Show Position
			endPosition_Spindle = 0;

			SetEnable(ID_AXIS_B, false);
			break;
		}
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
void OneRunStepper(int direction) // pageOne
{
	RotateControl rotateController;

	//v011
	const char * pageOne_bt10_bco1_Char = "pageOne.bt10.bco1=55715";
	const char* pageMain_vaSpEnabled_val_Char = "pageMain.vaSpEnabled.val=1";
	const char* pageOne_bt10_val_Char = "pageOne.bt10.val=1";
	const char * nextionEnd = "\xFF\xFF\xFF";

	// Reset end positions
	endPosition_Spindle = 0;
	endPosition_Axis = 0;

//#ifdef DEBUG
//	Serial.println("Enter RunOneStepper.");
//#endif // DEBUG

	int stepPin = PIN_AXIS_Z_STEP;
	int dirPin = PIN_AXIS_Z_DIR;

	int limitPin_Max = configSetup.limit_Max_Z;
	int limitPin_Min = configSetup.limit_Min_Z;

	int maxSpd = 0;
	int accel = 0;

	float speedPercent = 0;
	float currentSpeedPercent = 0;
	int32_t targetPosition = 2000000000;// *direction;

//#ifdef DEBUG
//	Serial.print("ActiveAxis");
//	Serial.println(configOne.axisId);
//#endif // DEBUG
	switch (configOne.axisId)
	{
		case ID_AXIS_Z:
		{
			stepPin = PIN_AXIS_Z_STEP;
			dirPin = PIN_AXIS_Z_DIR;
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
			maxSpd = configOne.maxSpd_Spindle;
			accel = configOne.accel_Spindle;
			speedPercent = configOne.speedPercent_Spindle;
			currentSpeedPercent = speedPercent * .01 * direction;

			break;
		}
	}

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
	Stepper stepper_One(stepPin, dirPin);
	stepper_One.setPosition(0);
	MilliDelay(5);
	stepper_One
		.setMaxSpeed(maxSpd)
		.setAcceleration(accel)
		.setTargetAbs(targetPosition);

//#ifdef DEBUG
//	Serial.print("configSp.activeAxis:");
//	Serial.println(configOne.axisId);
//	Serial.print("direction:");
//	Serial.println(direction);
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

	SetEnable(configOne.axisId, true);
//#ifdef DEBUG
//	Serial.print("Call rotateAsync");
//#endif
	rotateController.rotateAsync(stepper_One);
//#ifdef DEBUG
//	Serial.print("Rotating Async");
//#endif
	MilliDelay(5);
	rotateController.overrideSpeed(currentSpeedPercent);

//#ifdef DEBUG
//	Serial.print("currentSpeedPercent:");
//	Serial.println(currentSpeedPercent);
//#endif
	while (rotateController.isRunning())
	{
//#ifdef DEBUG
//		Serial.println("isRunning");
//#endif // DEBUG
		// Check for Cancel code  
		if (SerialAvailable() > 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
				case 67: // - C
				{
//#ifdef DEBUG
//					Serial.println("call overrideSpeed:");
//#endif // DEBUG
					rotateController.overrideSpeed(0);
					MilliDelay(5);
//#ifdef DEBUG
//					Serial.println("Call Stop:");
//#endif // DEBUG
					rotateController.stop();
					MilliDelay(100);
//#ifdef DEBUG
//					Serial.println("Stopped:");
//#endif // DEBUG

					goto endLoop;
					break;
				}
				case 102: // Speed
				{
					int newSpeedPercent = (int)GetSerialFloat(serialId);
					switch (configOne.axisId)
					{
						case ID_AXIS_Z:
						{
							configOne.speedPercent_Axis_Z = newSpeedPercent;
							currentSpeedPercent = float((newSpeedPercent) * .01 * direction);
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

					EEPROM.put(eePromAddress_One, configOne);

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
		MilliDelay(10);
	}

endLoop:

//#ifdef DEBUG
//	Serial.println("endLoop:");
//#endif // DEBUG
	// Set Return steps values
	if (configOne.axisId == ID_SPINDLE)
	{
		endPosition_Spindle = stepper_One.getPosition();
		returnSteps_Axis = 0;
		returnSteps_Spindle = endPosition_Spindle;

#ifdef SHOW_POSITION 
		degrees_Spindle = StepsToDegrees_Spindle(returnSteps_Spindle);
		SerialPrint("pageBE.t1.txt=");
		SerialWrite(0x22);
		SerialPrint(degrees_Spindle);
		SerialWrite(0x22);
		SerialWrite(0xff);
		SerialWrite(0xff);
		SerialWrite(0xff);
		MilliDelay(5);
		SerialPrint("pageBE.t1.txt=");
		SerialWrite(0x22);
		SerialPrint(degrees_Spindle);
		SerialWrite(0x22);
		SerialWrite(0xff);
		SerialWrite(0xff);
		SerialWrite(0xff);
		MilliDelay(5);
		// Set Axis end distance to 0
		SerialPrint("pageBE.t2.txt=");
		SerialWrite(0x22);
		SerialPrint(returnSteps_Axis);
		SerialWrite(0x22);
		SerialWrite(0xff);
		SerialWrite(0xff);
		SerialWrite(0xff);
		MilliDelay(5);
#endif // Show Position
	}
	else
	{
		endPosition_Axis = stepper_One.getPosition();
		returnSteps_Axis = endPosition_Axis;
		returnSteps_Spindle = 0;

#ifdef SHOW_POSITION 
		distance_Axis = StepsToDistance_Axis(endPosition_Axis, configOne.axisId);
		SerialPrint("pageBE.t2.txt=");
		SerialWrite(0x22);
		SerialPrint(distance_Axis);
		SerialWrite(0x22);
		SerialWrite(0xff);
		SerialWrite(0xff);
		SerialWrite(0xff);
		MilliDelay(5);
		SerialPrint("pageBE.t2.txt=");
		SerialWrite(0x22);
		SerialPrint(distance_Axis);
		SerialWrite(0x22);
		SerialWrite(0xff);
		SerialWrite(0xff);
		SerialWrite(0xff);
		MilliDelay(5);
		// Set Spindle end distance to 0
		SerialPrint("pageBE.t1.txt=");
		SerialWrite(0x22);
		SerialPrint(returnSteps_Spindle);
		SerialWrite(0x22);
		SerialWrite(0xff);
		SerialWrite(0xff);
		SerialWrite(0xff);
		MilliDelay(5);
#endif // Show Position
	}

	// Reset enable/disable status on Nextion
	SetEnable(configOne.axisId, false);
	SerialPrint(pageOne_bt10_bco1_Char);
	SerialPrint(nextionEnd);
	SerialPrint(pageMain_vaSpEnabled_val_Char);
	SerialPrint(nextionEnd);
	SerialPrint(pageOne_bt10_val_Char);
	SerialPrint(nextionEnd);


//#ifdef DEBUG
	//int rErr = 0;
	//rErr = Serial3.getReadError();
	//Serial.print("End:");
	//Serial1.println(rErr);
	//rErr = Serial3.getWriteError();
	//Serial1.println(rErr);
	//Serial3.clear();

//	Serial.println("1.Exit RunOneStepper.");
//#endif // DEBUG

	// ToDo: Verify steppers are disabled
	return;
}

 ///<summary>
/// Main_TwoSteppers
/// </summary>
/// <comment>
/// Allow switching axis while spindle is running
/// </comment>
/// <param name="direction_Axis">Axis rotation</param>
/// <param name="direction_Spindle">Spindle rotation</param>
/// <param name="initialCaller">Spindle or Axis</param>
void Main_TwoSteppers(
	int direction_Axis,
	int direction_Spindle,
	int initialCaller
)
{
	RotateControl rotateController_MainSpindle;
	RotateControl rotateController_MainAxis;
	Stepper stepper_Spindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
	
	int limitPin_Max = configSetup.limit_Max_Z;
	int limitPin_Min = configSetup.limit_Min_Z;

	//int polarity_Spindle = configSetup.polarity_Spindle ? (LOW) : (HIGH);
	int maxSpd_Spindle = 0;
	int accel_Spindle = 0;

	float speedPercentAxis = 0;
	int32_t maxSpeed_Axis = 0;
	int32_t accel_Axis = 0;
	float speedPercentSpindle = 0;
	float currentSpeedPercentSpindle = 0;
	int32_t targetPosition_Axis = 2000000000;
	int32_t targetPosition_Spindle = 2000000000;

	// initialCaller: Spindle or axis
	bool stepper_Axis_Go = false;
	bool stepper_Spindle_Go = false;

	const char * initialCaller_Char = "Initial Caller: ";
	const char * spindle_Char = "Spindle-";
	const char * maxSpd_Char = "MaxSpeed: ";
	const char * accel_Char = "Accel: ";
	const char * targetPosition_Char = "Target Position: ";
	const char * stop_Char = "Stop: ";
	const char * stopped_Char = "Stopped";
	const char * go_Char = "Go: ";
	const char * axis_Char = "Axis-";

#ifdef DEBUG
	Serial.print(initialCaller_Char);
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

	switch (configMain.axisId)
	{
		case ID_AXIS_Z:
		{
			speedPercentAxis = configMain.speedPercent_Axis_Z;
			limitPin_Max = configSetup.limit_Max_Z;
			limitPin_Min = configSetup.limit_Min_Z;
			maxSpeed_Axis = configMain.maxSpd_Axis_Z * direction_Axis;
			accel_Axis = configMain.accel_Axis_Z;
			stepper_Z.setPosition(0);
			stepper_Z
				.setMaxSpeed(maxSpeed_Axis)
				.setAcceleration(accel_Axis)
				.setTargetRel(0);
			MilliDelay(5);
			stepper_Z.setTargetRel(targetPosition_Axis);
			break;
		}
		case ID_AXIS_X:
		{

			speedPercentAxis = configMain.speedPercent_Axis_X;
			limitPin_Max = configSetup.limit_Max_X;
			limitPin_Min = configSetup.limit_Min_X;
			maxSpeed_Axis = configMain.maxSpd_Axis_X * direction_Axis;
			accel_Axis = configMain.accel_Axis_X;
			stepper_X.setPosition(0);
			stepper_X
				.setMaxSpeed(maxSpeed_Axis)
				.setAcceleration(accel_Axis)
				.setTargetRel(0);

			MilliDelay(5);
			stepper_X.setTargetRel(targetPosition_Axis);
			break;
		}
		case ID_AXIS_B:
		{
			speedPercentAxis = configMain.speedPercent_Axis_B;
			limitPin_Max = configSetup.limit_Max_B;
			limitPin_Min = configSetup.limit_Min_B;
			maxSpeed_Axis = configMain.maxSpd_Axis_B * direction_Axis;
			accel_Axis = configMain.accel_Axis_B;
			stepper_B.setPosition(0);
			stepper_B
				.setMaxSpeed(maxSpeed_Axis)
				.setAcceleration(accel_Axis)
				.setTargetRel(0);
			MilliDelay(5);
			stepper_B.setTargetRel(targetPosition_Axis);
			break;
		}
	}

	speedPercentSpindle = configMain.speedPercent_Spindle;
	maxSpd_Spindle = configMain.maxSpd_Spindle;
	accel_Spindle = configMain.accel_Spindle;
	currentSpeedPercentSpindle = speedPercentSpindle * .01;
	stepper_Spindle.setPosition(0);
	MilliDelay(5);

	stepper_Spindle
		.setMaxSpeed(maxSpd_Spindle * direction_Spindle)
		.setAcceleration(accel_Spindle)
		.setTargetAbs(targetPosition_Spindle);

#ifdef DEBUG
	Serial.print(spindle_Char);
	Serial.print(maxSpd_Char);
	Serial.println(maxSpd_Spindle);
	Serial.print(spindle_Char);
	Serial.print(accel_Char);
	Serial.println(accel_Spindle);
	Serial.print(spindle_Char);
	Serial.print(targetPosition_Char);
	Serial.println(targetPosition_Spindle);
#endif // DEBUG

	if (stepper_Axis_Go)
	{
		switch (configMain.axisId)
		{
			case ID_AXIS_Z:
			{
				SetEnable(ID_AXIS_Z, true);
				rotateController_MainAxis.rotateAsync(stepper_Z);
				rotateController_MainAxis.overrideSpeed(0);
				MilliDelay(10);
				rotateController_MainAxis.overrideSpeed(configMain.speedPercent_Axis_Z * .01);
				break;
			}
			case ID_AXIS_X:
			{
				SetEnable(ID_AXIS_X, true);
				rotateController_MainAxis.overrideSpeed(0);
				rotateController_MainAxis.overrideAcceleration(0);
				rotateController_MainAxis.rotateAsync(stepper_X);

				MilliDelay(10);
				rotateController_MainAxis.overrideSpeed(configMain.speedPercent_Axis_X * .01);
				break;
			}
			case ID_AXIS_B:
			{
				SetEnable(ID_AXIS_B, true);
				rotateController_MainAxis.rotateAsync(stepper_B);
				rotateController_MainAxis.overrideSpeed(0);
				MilliDelay(10);
				rotateController_MainAxis.overrideSpeed(configMain.speedPercent_Axis_B * .01);
				break;
			}
		}

#ifdef DEBUG
		Serial.print(axis_Char);
		Serial.print(go_Char);
		Serial.println(stepper_Axis_Go);

#endif // DEBUG
	}

	if (stepper_Spindle_Go)
	{
		SetEnable(ID_SPINDLE, true);
		MilliDelay(5);
		rotateController_MainSpindle.rotateAsync(stepper_Spindle);
		rotateController_MainSpindle.overrideSpeed(0);
		MilliDelay(10);
		rotateController_MainSpindle.overrideSpeed(currentSpeedPercentSpindle);
#ifdef DEBUG
		Serial.print(spindle_Char);
		Serial.print(go_Char);
		Serial.println(stepper_Spindle_Go);
#endif // DEBUG
	}

	while (stepper_Axis_Go || stepper_Spindle_Go)
	{
		MilliDelay(5);
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);

			switch (incomingByte)
			{
				case 67: // C - Spindle Stop
				{
	#ifdef DEBUG
					Serial.print(spindle_Char);
					Serial.print(stop_Char);
	#endif // DEBUG
					rotateController_MainSpindle.overrideSpeed(0);
					MilliDelay(5);
					rotateController_MainSpindle.stop();

	#ifdef DEBUG
					Serial.print(spindle_Char);
					Serial.println(stopped_Char);
	#endif // DEBUG

					SetEnable(ID_SPINDLE, false);
					stepper_Spindle_Go = false;


					break;
				}
				case 90: // Z - Z Axis CW
				{
					if (!rotateController_MainAxis.isRunning())
					{
						stepper_Axis_Go = true;
						direction_Axis = DIR_CW;
						targetPosition_Axis = 2000000000;
						speedPercentAxis = (float)((configMain.speedPercent_Axis_Z) * .01);
						stepper_Z
							.setMaxSpeed(configMain.maxSpd_Axis_Z * direction_Axis)
							.setTargetAbs(targetPosition_Axis);

						SetEnable(ID_AXIS_Z, true);
						rotateController_MainAxis.rotateAsync(stepper_Z);
						rotateController_MainAxis.overrideSpeed(0);
						MilliDelay(5);
						rotateController_MainAxis.overrideSpeed(speedPercentAxis);
					}
					break;
				}
				case 91: // [ - Z Axis CCW
				{
					if (!rotateController_MainAxis.isRunning())
					{
						stepper_Axis_Go = true;
						direction_Axis = DIR_CCW;
						targetPosition_Axis = -2000000000;
						speedPercentAxis = (float)((configMain.speedPercent_Axis_Z) * .01);
						stepper_Z
							.setMaxSpeed(configMain.maxSpd_Axis_Z * direction_Axis)
							.setTargetAbs(targetPosition_Axis);

						SetEnable(ID_AXIS_Z, true);
						rotateController_MainAxis.rotateAsync(stepper_Z);
						rotateController_MainAxis.overrideSpeed(0);
						MilliDelay(5);
						rotateController_MainAxis.overrideSpeed(speedPercentAxis);
					}
					break;
				}
				case 99: // c - Axis stop
				{
					rotateController_MainAxis.overrideSpeed(0);
					MilliDelay(5);
					rotateController_MainAxis.stop();
					switch (configMain.axisId)
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
					MilliDelay(1000);
					break;
				}
				case 107: // k -  Z spindle CW
				{
					if (!rotateController_MainSpindle.isRunning())
					{
						direction_Spindle = DIR_CW;
						targetPosition_Spindle = 2000000000;
						speedPercentSpindle = (float)(configMain.speedPercent_Spindle * .01);
						stepper_Spindle
							.setMaxSpeed(maxSpd_Spindle)
							.setTargetAbs(targetPosition_Spindle);
						stepper_Spindle_Go = true;

						SetEnable(ID_SPINDLE, true);
						rotateController_MainSpindle.rotateAsync(stepper_Spindle);
						rotateController_MainSpindle.overrideSpeed(0);
						MilliDelay(5);
						rotateController_MainSpindle.overrideSpeed(speedPercentSpindle);
					}

					break;
				}

				case 108: // l - Z spindle CCW
				{
					if (!rotateController_MainSpindle.isRunning())
					{
						direction_Spindle = DIR_CCW;
						targetPosition_Spindle = 2000000000;
						speedPercentSpindle = (float)(configMain.speedPercent_Spindle * .01);
						stepper_Spindle
							.setMaxSpeed(maxSpd_Spindle * DIR_CCW)
							.setTargetAbs(targetPosition_Spindle);
						stepper_Spindle_Go = true;

						SetEnable(ID_SPINDLE, true);
						rotateController_MainSpindle.rotateAsync(stepper_Spindle);
						rotateController_MainSpindle.overrideSpeed(0);
						MilliDelay(5);
						rotateController_MainSpindle.overrideSpeed(speedPercentSpindle);
					}
					break;
				}
				case 110: // n - Main spindle speed
				{
					configMain.speedPercent_Spindle = (int)GetSerialFloat(serialId);
					speedPercentSpindle = (float)(configMain.speedPercent_Spindle * .01);
					EEPROM.put(eePromAddress_Main, configMain);
					rotateController_MainSpindle.overrideSpeed(speedPercentSpindle);
					break;
				}
				case 111: // Axis Speed
				{
					int newSpeedPercent = (int)GetSerialFloat(serialId);
					switch (configMain.axisId)
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
					EEPROM.put(eePromAddress_Main, configMain);

					rotateController_MainAxis.overrideSpeed(speedPercentAxis);
					break;
				}
				case 165: // ¥ - X Axis CCW
				{
					if (!rotateController_MainAxis.isRunning())
					{
						stepper_Axis_Go = true;
						direction_Axis = DIR_CCW;
						targetPosition_Axis = -2000000000;
						speedPercentAxis = (float)(configMain.speedPercent_Axis_X * .01);
						stepper_X
							.setMaxSpeed(configMain.maxSpd_Axis_X * direction_Axis)
							.setTargetAbs(targetPosition_Axis);

						SetEnable(ID_AXIS_X, true);
						rotateController_MainAxis.rotateAsync(stepper_X);
						rotateController_MainAxis.overrideSpeed(0);
						MilliDelay(5);
						rotateController_MainAxis.overrideSpeed(speedPercentAxis);
					}
					break;
				}
				case 166: // ¦ - X Axis CW
				{
					
					if (!rotateController_MainAxis.isRunning())
					{
						stepper_Axis_Go = true;
						direction_Axis = DIR_CW;
						targetPosition_Axis = 2000000000;
						speedPercentAxis = (float)(configMain.speedPercent_Axis_X * .01);
						stepper_X
							.setMaxSpeed(configMain.maxSpd_Axis_X * direction_Axis)
							.setTargetAbs(targetPosition_Axis);

						SetEnable(ID_AXIS_X, true);
						rotateController_MainAxis.rotateAsync(stepper_X);
						rotateController_MainAxis.overrideSpeed(0);
						MilliDelay(5);
						rotateController_MainAxis.overrideSpeed(speedPercentAxis); 
					}

					break;
				}
				case 171: // « - Sp-B Axis CCW
				{
					if (!rotateController_MainAxis.isRunning())
					{
						stepper_Axis_Go = true;
						direction_Axis = DIR_CCW;
						targetPosition_Axis = -2000000000;

						speedPercentAxis = (float)((configMain.speedPercent_Axis_B) * .01);
						stepper_B
							.setMaxSpeed(configMain.maxSpd_Axis_B * direction_Axis)
							.setTargetAbs(targetPosition_Axis);

						SetEnable(ID_AXIS_B, true);
						rotateController_MainAxis.rotateAsync(stepper_B);
						rotateController_MainAxis.overrideSpeed(0);
						MilliDelay(5);
						rotateController_MainAxis.overrideSpeed(speedPercentAxis);
					}
					break;
				}
				case 172: // ¬ - B Axis CW
				{
					if (!rotateController_MainAxis.isRunning())
					{
						stepper_Axis_Go = true;
						direction_Axis = DIR_CW;
						targetPosition_Axis = 2000000000;
						speedPercentAxis = (float)((configMain.speedPercent_Axis_B) * .01);
						stepper_B
							.setMaxSpeed(configMain.maxSpd_Axis_B * direction_Axis)
							.setTargetAbs(targetPosition_Axis);

						SetEnable(ID_AXIS_B, true);
						rotateController_MainAxis.rotateAsync(stepper_B);
						rotateController_MainAxis.overrideSpeed(0);
						MilliDelay(5);
						rotateController_MainAxis.overrideSpeed(speedPercentAxis);
					}
					break;
				}

				case 197: // Å - Set AxisID
				{
					configMain.axisId = GetSerialInteger();
					EEPROM.put(eePromAddress_Main, configMain);
					break;
				}
			}
		}

		if (stepper_Axis_Go)
		{
			if (digitalRead(limitPin_Max) == LOW)
			{
				rotateController_MainAxis.overrideSpeed(0);
				rotateController_MainAxis.stop();
				if (configSetup.limit_StopSpindle)
				{
					// Stop the spindle
					rotateController_MainSpindle.overrideSpeed(0);
					MilliDelay(5);
					rotateController_MainSpindle.stop();
					SetEnable(ID_SPINDLE, false);
					stepper_Spindle_Go = false;
				}
				stepper_Axis_Go = false;
			}
		}
		else
		{
			if (digitalRead(limitPin_Min) == LOW)
			{
				rotateController_MainAxis.overrideSpeed(0);
				rotateController_MainAxis.stop();
				stepper_Axis_Go = false;

				if (configSetup.limit_StopSpindle)
				{
					// Stop the spindle
					rotateController_MainSpindle.overrideSpeed(0);
					MilliDelay(5);
					rotateController_MainSpindle.stop();
					SetEnable(ID_SPINDLE, false);
					stepper_Spindle_Go = false;
				}
			}
		}

		MilliDelay(5);
	}

#ifdef DEBUG
	Serial.print(spindle_Char);
	Serial.print(axis_Char);
	Serial.println(stopped_Char);
#endif // DEBUG
	stepper_Axis_Go = false;
	stepper_Spindle_Go = false;

	switch (configMain.axisId)
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

#ifdef SHOW_POSITION 
	degrees_Spindle = StepsToDegrees_Spindle(returnSteps_Spindle);
	SerialPrint("pageBE.t1.txt=");
	SerialWrite(0x22);
	SerialPrint(degrees_Spindle);
	SerialWrite(0x22);
	SerialWrite(0xff);
	SerialWrite(0xff);
	SerialWrite(0xff);
	MilliDelay(5);
	SerialPrint("pageBE.t1.txt=");
	SerialWrite(0x22);
	SerialPrint(degrees_Spindle);
	SerialWrite(0x22);
	SerialWrite(0xff);
	SerialWrite(0xff);
	SerialWrite(0xff);
	MilliDelay(5);
	distance_Axis = StepsToDistance_Axis(returnSteps_Axis, configMain.axisId);
	SerialPrint("pageBE.t2.txt=");
	SerialWrite(0x22);
	SerialPrint(distance_Axis);
	SerialWrite(0x22);
	SerialWrite(0xff);
	SerialWrite(0xff);
	SerialWrite(0xff);
	MilliDelay(5);
	SerialPrint("pageBE.t2.txt=");
	SerialWrite(0x22);
	SerialPrint(distance_Axis);
	SerialWrite(0x22);
	SerialWrite(0xff);
	SerialWrite(0xff);
	SerialWrite(0xff);
	MilliDelay(5);
#endif // Show Position

	stepper_Spindle.setPosition(0);

	SetEnable(ID_SPINDLE, false);
	SetEnable(ID_AXIS_Z, false);
	SetEnable(ID_AXIS_X, false);
	SetEnable(ID_AXIS_B, false);
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
	//StepControl stepController(configSetup.pulseWidth_Spindle, configSetup.speedUpdatePeriod_Spindle);
	Stepper stepper_Spindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
	
	const char * index1_Char = "Index 1-";
	const char * index2_Char = "Index 2-";
	const char * index3_Char = "Index 3-";
	const char * index4_Char = "Index 4-";
	const char * index5_Char = "Index 5-";
	const char * size_Char = "Size:";
	const char * direction_Char = "Direction:";
	const char * stepsToMove_Char = "Steps to Move:";
	//const char * pageIndex_bt3_pco_Char = "pageIndex.bt3.pco=59391";
	const char * pageIndex_bt3_val_Char = "pageIndex.bt3.val=0";
	const char * pageIndex_va0_Char = "pageIndex.va0.val=0";
	const char * pageIndex_bt1_val_Char = "pageIndex.bt1.val=0";
	const char * pageIndex_bt2_val_Char = "pageIndex.bt2.val=0";
	const char * pageBE_t1_Char = "pageBE.t1.txt=";

	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	const char* nextionEnd = "\xFF\xFF\xFF";
	stepper_Spindle.setPosition(0);
	int stepsToMove = 0;

	switch (configIndex_Main.indexId)
	{
		case 1:
		{
#ifdef DEBUG
			Serial.print(index1_Char);
			Serial.print(size_Char);
			Serial.println(configIndex_1.sizeInSteps);
#endif // DEBUG

			stepsToMove = (int)round(configIndex_1.sizeInSteps);
			break;
		}
		case 2:
		{
#ifdef DEBUG

			Serial.print(index2_Char);
			Serial.print(size_Char);
			Serial.println(configIndex_2.sizeInSteps);
#endif // DEBUG

			stepsToMove = (int)round(configIndex_2.sizeInSteps);
			break;
		}
		case 3:
		{
#ifdef DEBUG
			Serial.print(index3_Char);
			Serial.print(size_Char);
			Serial.println(configIndex_3.sizeInSteps);
#endif // DEBUG
			stepsToMove = (int)round(configIndex_3.sizeInSteps);
			break;
		}
		case 4:
		{
#ifdef DEBUG
			Serial.print(index4_Char);
			Serial.print(size_Char);
			Serial.println(configIndex_4.sizeInSteps);
#endif // DEBUG
			stepsToMove = (int)round(configIndex_4.sizeInSteps);
			break;
		}
		case 5:
		{
#ifdef DEBUG
			Serial.print(index5_Char);
			Serial.print(size_Char);
			Serial.println(configIndex_5.sizeInSteps);
#endif // DEBUG
			stepsToMove = (int)round(configIndex_5.sizeInSteps);
			break;
		}
	}

#ifdef DEBUG
	Serial.print(direction_Char);
	Serial.println(directionSpindle);
	Serial.print("Microsteps: ");
	Serial.println(configSetup.microsteps_Spindle);
	Serial.print("GearRatio: ");
	Serial.println(configSetup.gearRatio_Spindle);
	Serial.print("Steps/360: ");
	Serial.println(configSetup.steps360_Spindle);
	Serial.print("PulseWidth: ");
	Serial.println(configSetup.pulseWidth_Spindle);
	Serial.print("SpeedUpdatePeriod: ");
	Serial.println(configSetup.speedUpdatePeriod_Spindle);

	Serial.print(stepsToMove_Char);
	Serial.println(stepsToMove);
#endif // DEBUG

	// Set speed and acceleration
	stepper_Spindle
		.setMaxSpeed(configIndex_Main.maxSpd)
		.setAcceleration(configIndex_Main.accel)
		.setTargetRel(stepsToMove * directionSpindle);
		
		

	SetEnable(ID_SPINDLE, true);
	stepController.moveAsync(stepper_Spindle);
	while (stepController.isRunning())
	{
		MilliDelay(5);

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
		MilliDelay(10);
	}
	// Update Index flag on Nextion. 
	//SerialPrint(pageIndex_bt3_pco_Char);
	//SerialPrint(nextionEnd);
	SerialPrint(pageIndex_va0_Char);
	SerialPrint(nextionEnd);
	SerialPrint(pageIndex_bt3_val_Char);
	SerialPrint(nextionEnd);
	SerialPrint(pageIndex_bt2_val_Char);
	SerialPrint(nextionEnd);
	SerialPrint(pageIndex_bt1_val_Char);
	SerialPrint(nextionEnd);

	returnSteps_Axis = 0;
	returnSteps_Spindle = stepper_Spindle.getPosition();
#ifdef SHOW_POSITION 
	degrees_Spindle = StepsToDegrees_Spindle(returnSteps_Spindle);

	SerialPrint(pageBE_t1_Char);
	SerialWrite(0x22);
	SerialPrint(degrees_Spindle);
	SerialPrint(nextionQuoteEnd);
#endif // Show Position
	SetEnable(ID_SPINDLE, false);
}

/// <summary>
/// Sync
/// </summary>
/// <comment>
/// </comment>
/// <param name="directionSpindle">Counterclockwise: -1, Clockwise: 1</param>
/// <param name = "directionAxis">Counterclockwise: -1, Clockwise : 1< /param>
/// <returns></returns>
void Sync(int directionSpindle, int directionAxis)
{
	const char* axisId_Char = "Axis ID:";
	const char* spindle_Char = "Spindle-";
	const char* axis_Char = "Axis-";
	const char* targetSteps_Char = "Target Steps:";
	const char* revolutions_Char = "Revolutions:";
	const char* distance_Char = "Distance:";

	const char* pageBE_t1_Char = "pageBE.t1.txt=";
	const char* pageBE_t2_Char = "pageBE.t2.txt=";
	const char* pageSync_bt6_pco_Char = "pageSync.bt6.pco=0";
	const char* pageSync_bt6_val_Char = "pageSync.bt6.val=0";
	const char* pageSync_va0_val_Char = "pageSync.va0.val=0";
	const char* pageSync_bt1_val_Char = "pageSync.bt1.val=0";
	const char* pageSync_bt2_val_Char = "pageSync.bt2.val=0";

	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	const char* nextionEnd = "\xFF\xFF\xFF";

	int stepPin = 0;
	int dirPin = 0;
	int32_t maxSpeed = 0;
	int32_t accel = 0;
	long axisSteps = 0;

	switch (configSync.axisId)
	{
		case ID_AXIS_Z:
		{
			stepPin = PIN_AXIS_Z_STEP;
			dirPin = PIN_AXIS_Z_DIR;
			maxSpeed = configSync.maxSpd_Axis_Z * configSync.speedPercent_Axis_Z * .01;
			accel = configSync.accel_Axis_Z;
			axisSteps = DistanceToSteps_Axis(configSync.distance, configSync.axisId) * directionAxis;
			break;
		}
		case ID_AXIS_X:
		{
			stepPin = PIN_AXIS_X_STEP;
			dirPin = PIN_AXIS_X_DIR;
			maxSpeed = configSync.maxSpd_Axis_X * configSync.speedPercent_Axis_X * .01;
			accel = configSync.accel_Axis_X;
			axisSteps = DistanceToSteps_Axis(configSync.distance, configSync.axisId) * directionAxis;
			break;
		}
		case ID_AXIS_B:
		{
			stepPin = PIN_AXIS_B_STEP;
			dirPin = PIN_AXIS_B_DIR;
			maxSpeed = configSync.maxSpd_Axis_B * configSync.speedPercent_Axis_B * .01;
			accel = configSync.accel_Axis_B;
			switch (configSetup.radialOrLinear_Axis_B)
			{
				case RADIAL_B:
				{
					axisSteps = DistanceToSteps_RadialB(configSync.distance)* directionAxis;
					break;
				}
				case LINEAR_B:
				{
					axisSteps = DistanceToSteps_LinearB(configSync.distance) * directionAxis;
					break;
				}
			}
			
			break;
		}
	}
	StepControl stepController;
	Stepper stepper_Spindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
	Stepper stepper_Axis(stepPin, dirPin);

	float targetSteps_Spindle = 0;
	targetSteps_Spindle = RevolutionsToSteps_Spindle(configSync.revolutions_Spindle, directionSpindle);
	stepper_Spindle
		.setMaxSpeed(configSync.maxSpd_Spindle * configSync.speedPercent_Spindle * .01)
		.setAcceleration(configSync.accel_Spindle)
		.setTargetRel(targetSteps_Spindle);
	stepper_Spindle.setPosition(0);

	stepper_Axis
		.setMaxSpeed(maxSpeed)
		.setAcceleration(accel)
		.setTargetRel(axisSteps);
	stepper_Axis.setPosition(0);

#ifdef DEBUG
	Serial.print(axisId_Char);
	Serial.println(configSync.axisId);
	Serial.print(spindle_Char);
	Serial.print(targetSteps_Char);
	Serial.println(targetSteps_Spindle);
	Serial.print(spindle_Char);
	Serial.print(revolutions_Char);
	Serial.println(configSync.revolutions_Spindle);

	Serial.print(axisId_Char);
	Serial.print(distance_Char);
	Serial.println(configSync.distance);
	Serial.print(axisId_Char);
	Serial.print(targetSteps_Char);
	Serial.println(axisSteps);

#endif // DEBUG

	SetEnable(ID_SPINDLE, true);

	switch (configSync.axisId)
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
	}

	Serial.println("MoveAsync..............................................");
	stepController.moveAsync(stepper_Spindle, stepper_Axis);
	while (stepController.isRunning())
	{
#ifdef DEBUG

		endPosition_Spindle = stepper_Spindle.getPosition();
		endPosition_Axis = stepper_Axis.getPosition();
//		switch (idAxis)
//		{
//		case ID_AXIS_X:
//		{
//			endPosition_Axis = stepper_X.getPosition();
//#ifdef SHOW_POSITION 
//			// Track Positions not implemented while steppers are running
//			/*if (configGeneral.trackPositions == 1)
//			{
//				degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
//				distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_X);
//				SerialPrint("pageBE.t1.txt=");
//				SerialWrite(0x22);
//				SerialPrint(degrees_Spindle);
//				SerialWrite(0x22);
//				SerialWrite(0xff);
//				SerialWrite(0xff);
//				SerialWrite(0xff);
//				MilliDelay(10);
//				SerialPrint("pageBE.t2.txt=");
//				SerialWrite(0x22);
//				SerialPrint(distance_Axis);
//				SerialWrite(0x22);
//				SerialWrite(0xff);
//				SerialWrite(0xff);
//				SerialWrite(0xff);
//			}*/
//#endif // Show Position
//			break;
//		}
//		case ID_AXIS_Z:
//		{
//			endPosition_Axis = stepper_Z.getPosition();
//#ifdef SHOW_POSITION 
//			// Track Positions not implemented while steppers are running
//			/*if (configGeneral.trackPositions == 1)
//			{
//				degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
//				distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);
//				SerialPrint("pageBE.t1.txt=");
//				SerialWrite(0x22);
//				SerialPrint(degrees_Spindle);
//				SerialWrite(0x22);
//				SerialWrite(0xff);
//				SerialWrite(0xff);
//				SerialWrite(0xff);
//				MilliDelay(10);
//				SerialPrint("pageBE.t2.txt=");
//				SerialWrite(0x22);
//				SerialPrint(distance_Axis);
//				SerialWrite(0x22);
//				SerialWrite(0xff);
//				SerialWrite(0xff);
//				SerialWrite(0xff);
//			}*/
//#endif // Show Position
//			break;
//		}
//		}

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

		MilliDelay(10);
	}

	returnSteps_Spindle = stepper_Spindle.getPosition();
	endPosition_Axis = stepper_Axis.getPosition();
	switch (configSync.axisId)
	{
	case ID_AXIS_X:
	{

#ifdef SHOW_POSITION 
		degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
		distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_X);

		// Update Nextion
		SerialPrint(pageBE_t1_Char);
		SerialWrite(0x22);
		SerialPrint(degrees_Spindle);
		SerialPrint(nextionQuoteEnd);
		SerialPrint(pageBE_t2_Char);
		SerialWrite(0x22);
		SerialPrint(distance_Axis);
		SerialPrint(nextionQuoteEnd);
#endif // Show Position
		SerialPrint(pageSync_bt6_pco_Char);
		SerialPrint(nextionEnd);
		SerialPrint(pageSync_va0_val_Char);
		SerialPrint(nextionEnd);
		SerialPrint(pageSync_bt6_val_Char);// The first one may be ignored by Nextion, so resend.
		SerialPrint(nextionEnd);
		SerialPrint(pageSync_bt1_val_Char);
		SerialPrint(nextionEnd);
		SerialPrint(pageSync_bt2_val_Char);
		SerialPrint(nextionEnd);

		SetEnable(ID_AXIS_X, false);
		break;
	}
	case ID_AXIS_Z:
	{

#ifdef SHOW_POSITION 
		degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
		distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);

		// Update Nextion
		SerialPrint(pageBE_t1_Char);
		SerialWrite(0x22);
		SerialPrint(degrees_Spindle);
		SerialPrint(nextionQuoteEnd);
		SerialPrint(pageBE_t2_Char);
		SerialWrite(0x22);
		SerialPrint(distance_Axis);
		SerialPrint(nextionQuoteEnd);
#endif // Show Position
		SerialPrint(pageSync_bt6_pco_Char);
		SerialPrint(nextionEnd);
		SerialPrint(pageSync_va0_val_Char);
		SerialPrint(nextionEnd);
		SerialPrint(pageSync_bt6_val_Char);
		SerialPrint(nextionEnd);
		SerialPrint(pageSync_bt2_val_Char);
		SerialPrint(nextionEnd);
		SerialPrint(pageSync_bt1_val_Char);
		SerialPrint(nextionEnd);

		SetEnable(ID_AXIS_Z, false);
		break;
	}

	case ID_AXIS_B:
	{

#ifdef SHOW_POSITION 
		degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
		distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_B);

		// Update Nextion
		SerialPrint(pageBE_t1_Char);
		SerialWrite(0x22);
		SerialPrint(degrees_Spindle);
		SerialPrint(nextionQuoteEnd);
		SerialPrint(pageBE_t2_Char);
		SerialWrite(0x22);
		SerialPrint(distance_Axis);
		SerialPrint(nextionQuoteEnd);
#endif // Show Position
		SerialPrint(pageSync_bt6_pco_Char);
		SerialPrint(nextionEnd);
		SerialPrint(pageSync_va0_val_Char);
		SerialPrint(nextionEnd);
		SerialPrint(pageSync_bt6_val_Char);
		SerialPrint(nextionEnd);
		SerialPrint(pageSync_bt2_val_Char);
		SerialPrint(nextionEnd);
		SerialPrint(pageSync_bt1_val_Char);
		SerialPrint(nextionEnd);

		SetEnable(ID_AXIS_B, false);
		break;
	}
	}

	returnSteps_Axis = endPosition_Axis;

	stepper_Spindle.setPosition(0);
	stepper_Axis.setPosition(0);

	SetEnable(ID_SPINDLE, false);
#ifdef DEBUG
	Serial.print(spindle_Char);
	Serial.println(endPosition_Spindle);
	Serial.print(axis_Char);
	Serial.println(endPosition_Axis);
#endif // DEBUG
}

/// <summary>
/// Reciprocate
/// </summary>
/// <comment>
/// </comment>
/// <param name="wavDir">In: -1, Out: 1</param>
/// <returns></returns>
void Reciprocate(int wavDir)
{
	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	const char* nextionEnd = "\xFF\xFF\xFF";
	int stepPin = 0;
	int dirPin = 0;
	int32_t maxSpeed = 0;
	int32_t accel = 0;
	int32_t waves = 0;
	int32_t speedPercent = 0;

	switch (configRec.axisId)
	{
		case ID_AXIS_Z:
		{
			stepPin = PIN_AXIS_Z_STEP;
			dirPin = PIN_AXIS_Z_DIR;
			maxSpeed = configRec.maxSpd_Axis_Z * configRec.speedPercent_Axis_Z * .01;
			accel = configRec.accel_Axis_Z;
			speedPercent = configRec.speedPercent_Axis_Z;
#ifdef DEBUG
			Serial.print("Z---AxisMaxSpd:");
			Serial.println(configRec.maxSpd_Axis_Z);
			Serial.print("Z---AxisSpeedPercent:");
			Serial.println(configRec.speedPercent_Axis_Z);
			Serial.print("Z---AxisAccel:");
			Serial.println(configRec.accel_Axis_Z);
#endif // DEBUG
			break;
		}
		case ID_AXIS_X:
		{
			stepPin = PIN_AXIS_X_STEP;
			dirPin = PIN_AXIS_X_DIR;
			maxSpeed = configRec.maxSpd_Axis_X * configRec.speedPercent_Axis_X * .01;
			accel = configRec.accel_Axis_X;
			speedPercent = configRec.speedPercent_Axis_X;
#ifdef DEBUG
			Serial.print("X---AxisMaxSpd:");
			Serial.println(configRec.maxSpd_Axis_X);
			Serial.print("X---AxisSpeedPercent:");
			Serial.println(configRec.speedPercent_Axis_X);
			Serial.print("X---AxisAccel:");
			Serial.println(configRec.accel_Axis_X);
#endif // DEBUG
			break;
		}
		case ID_AXIS_B:
		{
			stepPin = PIN_AXIS_B_STEP;
			dirPin = PIN_AXIS_B_DIR;
			maxSpeed = configRec.maxSpd_Axis_B * configRec.speedPercent_Axis_B * .01;
			accel = configRec.accel_Axis_B;
			speedPercent = configRec.speedPercent_Axis_B;
#ifdef DEBUG
			Serial.print("B---AxisMaxSpd:");
			Serial.println(configRec.maxSpd_Axis_B);
			Serial.print("B---AxisSpeedPercent:");
			Serial.println(configRec.speedPercent_Axis_B);
			Serial.print("B---AxisAccel:");
			Serial.println(configRec.accel_Axis_B);
#endif // DEBUG
			break;
		}
	}

	StepControl stepController;
	Stepper stepper_Spindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
	Stepper stepper_Axis(stepPin, dirPin);

	// Set speed and acceleration
	stepper_Spindle
		.setMaxSpeed(configRec.maxSpd_Spindle * configRec.speedPercent_Spindle * .01)
		.setAcceleration(configRec.accel_Spindle);
	stepper_Spindle.setPosition(0);

	int spindleSteps = 0;
	long axisSteps = 0;
	switch (configRec.radial_axial)
	{
		case RADIAL: // Radial
		{ 
			waves = configRec.waves_Radial;
			float spindleDegrees = configRec.degrees_Radial_Spindle / (configRec.waves_Radial * 2);
			spindleSteps = round((configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (spindleDegrees / 360) * wavDir);
			axisSteps = (DistanceToSteps_Axis(configRec.amplitude_Radial_Axis, configRec.axisId) / 2) * wavDir;
			returnSteps_Axis = DistanceToSteps_Axis(configRec.amplitude_Radial_Axis, configRec.axisId);
			break;
		}
		case AXIAL: // Axial
		{
			waves = configRec.waves_Axial;
			spindleSteps = (configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (configRec.amplitude_Axial_Spindle / 360) * wavDir;
			axisSteps = (DistanceToSteps_Axis(configRec.distance_Axial_Axis, configRec.axisId) / (configRec.waves_Axial * 2))* wavDir;
			returnSteps_Axis = DistanceToSteps_Axis(configRec.distance_Axial_Axis, configRec.axisId);
			break;
		}
	}

#ifdef DEBUG
	Serial.println("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
	Serial.print("spindleSteps:");
	Serial.println(spindleSteps);
	Serial.print("returnSteps_Axis:");
	Serial.println(returnSteps_Axis);
	Serial.print("axisSteps:");
	Serial.println(axisSteps);
	Serial.println("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
#endif // DEBUG

	stepper_Axis
		.setMaxSpeed(maxSpeed)
		.setAcceleration(accel);
	stepper_Axis.setPosition(0);

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

	switch (configRec.radial_axial)
	{
		case RADIAL: // Radial
		{
			Serial.print("degrees_Radial:");
			Serial.println(configRec.degrees_Radial_Spindle);
			Serial.print("amplitude_Radial (Distance):");
			Serial.println(configRec.amplitude_Radial_Axis);

			Serial.print("waves_Radial:");
			Serial.println(configRec.waves_Radial);
			break;
		}
		case AXIAL: // Axial
		{
			Serial.print("amplitude_Axial:");
			Serial.println(configRec.amplitude_Axial_Spindle);
			Serial.print("distance_Axial:");
			Serial.println(configRec.distance_Axial_Axis);
			Serial.print("waves_Axial:");
			Serial.println(configRec.waves_Axial);
			break;
		}
	}
	//Serial.print("distancePerRev_AxisX:");
	//Serial.println(configSetup.distancePerRev_AxisX);
	//Serial.print("steps360_Axis_X:");
	//Serial.println(configSetup.steps360_Axis_X);
	//Serial.print("microsteps_Axis_X:");
	//Serial.println(configSetup.microsteps_Axis_X);


	Serial.print("spindleSteps:");
	Serial.println(spindleSteps);
	Serial.print("axisSteps:");
	Serial.println(axisSteps);

	Serial.print("AxisMaxSpd:");
	Serial.println(maxSpeed);
	Serial.print("AxisSpeedPercent:");
	Serial.println(speedPercent);
	Serial.print("AxisAccel:");
	Serial.println(accel);
#endif // DEBUG

	SetEnable(ID_SPINDLE, true);

	switch (configRec.axisId)
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
			Serial.println("B Enabled..........................");
			break;
		}
	}
	
	for (int i = 0; i < (waves * 2); i++)
	{


		switch (configRec.radial_axial)
		{
			case RADIAL: // Radial
			{
				axisSteps *= -1;
				break;
			}
			case AXIAL: // Axial
			{
				spindleSteps *= -1;
				break;
			}
		}
#ifdef DEBUG
		Serial.println("");
		Serial.print("spindleSteps:");
		Serial.println(spindleSteps);
		Serial.print("axisSteps:");
		Serial.println(axisSteps);
#endif // DEBUG
		stepper_Spindle.setTargetRel(spindleSteps);
		stepper_Axis.setTargetRel(axisSteps);

		stepController.moveAsync(stepper_Spindle, stepper_Axis);
		MilliDelay(5);

		while (stepController.isRunning())
		{
#ifdef DEBUG
			//Serial.println("isRunning");
			//delay(100);

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
				SerialPrint("pageBE.t1.txt=");
				SerialWrite(0x22);
				SerialPrint(degrees_Spindle);
				SerialWrite(0x22);
				SerialWrite(0xff);
				SerialWrite(0xff);
				SerialWrite(0xff);
				MilliDelay(10);
				SerialPrint("pageBE.t2.txt=");
				SerialWrite(0x22);
				SerialPrint(distance_Axis);
				SerialWrite(0x22);
				SerialWrite(0xff);
				SerialWrite(0xff);
				SerialWrite(0xff);
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
#ifdef DEBUG
			Serial.println("STOP++++++++++++++++++++++");
#endif // DEBUG
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

	endPosition_Axis = stepper_Axis.getPosition();
	endPosition_Spindle = stepper_Spindle.getPosition();
#ifdef SHOW_POSITION 
	degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
	distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_X);
	SerialPrint("pageBE.t1.txt=");
	SerialWrite(0x22);
	SerialPrint(degrees_Spindle);
	SerialPrint(nextionQuoteEnd);
	SerialPrint("pageBE.t2.txt=");
	SerialWrite(0x22);
	SerialPrint(distance_Axis);
	SerialPrint(nextionQuoteEnd);
#endif //SHOW_POSITION
	// Update Nextion
	SerialPrint("pageRec.va0.val=0");
	SerialPrint(nextionEnd);
	SerialPrint("pageRec.bt3.val=0");// The first one may be ignored by Nextion, so resend.
	SerialPrint(nextionEnd);
	SerialPrint("pageRec.bt2.val=0");
	SerialPrint(nextionEnd);
	SerialPrint("pageRec.bt1.val=0");
	SerialPrint(nextionEnd);

#ifdef DEBUG
	endPosition_Axis = stepper_Axis.getPosition();
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
	SetEnable(ID_AXIS_X, false);
	SetEnable(ID_AXIS_B, false);
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
				switch (configGreekKey.axisId)
				{
					case ID_AXIS_Z: // Z Axis
					{
						SetEnable(ID_AXIS_X, false);
					}
					case ID_AXIS_X: // X Axis
					{
						SetEnable(ID_AXIS_X, false);
					}
					case ID_AXIS_B: // B Axis
					{
						SetEnable(ID_AXIS_B, false);
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
	if (configGreekKey.radialOrAxial_Pattern == AXIAL)
	{
		totalSpindleSegments = 4;
	}
	GreekKeyPattern_Initial(totalSpindleSegments);

	for (int i = 1; i <= configGreekKey.patternCount_Pattern;i++)// / 2; i++)
	{
		switch (configGreekKey.radialOrAxial_Pattern)
		{
			case RADIAL: // Axis Left CCW
			{
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CW, true); //1
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //2
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW, true); //3
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //4
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW, true); //5
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //6
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CW, true); // 7
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

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CCW, true); //10
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CW); //11
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW, true); //12
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //13
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW, true); // 14
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CW); //15
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CCW, true); //16
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
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW, true); //2
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //3
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW, true); //4
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //5
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW, true); //6
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CW); //7
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 4, DIR_CW, true); //7
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 8, DIR_CCW, true); //9
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CCW); //10
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CW, true); //11
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //12
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW, true); //13
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //14
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CW, true); //15
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CCW); //16
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 4, DIR_CW, true); //17
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 8, DIR_CCW, true); //18
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
	if (configGreekKey.radialOrAxial_Pattern == AXIAL)
	{
		totalSpindleSegments = 4;
	}
	GreekKeyPattern_Initial(totalSpindleSegments);

	for (int i = 1; i <= configGreekKey.patternCount_Pattern; i++)
	{
		switch (configGreekKey.radialOrAxial_Pattern)
		{
			case RADIAL: // Axis Left CCW
			{
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //1-U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CW, true); //2-L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //3 U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW, true); //4 R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //5 D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW, true); //6 R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CCW); //7 U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW, true); // 8 L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //9 D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW, true); //10 L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //11 U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CCW, true); //12 R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 6, DIR_CW); //13 D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 4, DIR_CW, true); //14 L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 7, DIR_CCW); //15 U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CCW, true); //16 R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //17 U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW, true); //18 L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //19 D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW, true); //20
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CCW); //21
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW, true); //22
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //23
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW, true); //24
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //25
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CW, true); //26
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
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 4, DIR_CCW, true); //29
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

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW, true); //1-U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CW); //2-L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW, true); //3 U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //4 R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW, true); //5 D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //6 R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CCW, true); //7 U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); // 8 L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW, true); //9 D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //10 L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW, true); //11 U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CCW); //12 R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 6, DIR_CW, true); //13 D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 4, DIR_CW); //14 L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 7, DIR_CCW, true); //15 U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CCW); //16 R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW, true); //17 U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //18 L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW, true); //19 D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //20
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CCW, true); //21
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //22
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW, true); //23
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //24
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW, true); //25
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CW); //26
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 5, DIR_CW, true); //27
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 6, DIR_CCW, true); //28
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 4, DIR_CCW); //29
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 7, DIR_CW, true); //30
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 7, DIR_CCW, true); //31
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
	if (configGreekKey.radialOrAxial_Pattern == AXIAL)
	{
		totalSpindleSegments = 3;
	}
	GreekKeyPattern_Initial(totalSpindleSegments);

	for (int i = 1; i <= configGreekKey.patternCount_Pattern; i++)
	{

		switch (configGreekKey.radialOrAxial_Pattern)
		{
			case RADIAL: // Axis Left CCW
			{
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CW, true); //1
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CCW); //2
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW, true); //3
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //4
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW, true); //5
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //6
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW, true); //7
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

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CCW, true); //2
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //3
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW, true); //4
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //5
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW, true); //6
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //7
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CCW, true); //8
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
	if (configGreekKey.radialOrAxial_Pattern == AXIAL)
	{
		totalSpindleSegments = 3;
	}
	GreekKeyPattern_Initial(totalSpindleSegments);

	for (int i = 1; i <= configGreekKey.patternCount_Pattern; i++)
	{
		switch (configGreekKey.radialOrAxial_Pattern)
		{
			case RADIAL: // Axis Left CCW
			{
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CW, true); //1  3R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CCW); //2 3U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW, true); //3 2L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //4 1D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW, true); //5 1R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //6 1D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW, true); //7 2L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 5, DIR_CCW); //8 5U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CW, true); //9 2R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //10 1D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW, true); //11 1L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //12 1D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CW, true); //13 2R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CCW); //14 3U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CCW, true); //15 3L
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
	
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CCW, true); //2
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //3
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW, true); //4
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //5
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW, true); //6
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //7
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 5, DIR_CCW, true); //8
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CW); //9
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW, true); //10
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //11
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW, true); //12
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CW); //13
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CCW, true); //14
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CCW); //15
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW, true); //16
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
	if (configGreekKey.radialOrAxial_Pattern == AXIAL)
	{
		totalSpindleSegments = 2;
	}

	GreekKeyPattern_Initial(totalSpindleSegments);
#ifdef DEBUG
	Serial.print("1.axisShortLegSteps:");
	Serial.println(axisShortLegSteps);
#endif // DEBUG

	for (int i = 1; i <= configGreekKey.patternCount_Pattern; i++)
	{
#ifdef DEBUG
		Serial.print("----------------i:");
		Serial.println(i);
#endif // DEBUG

		switch (configGreekKey.radialOrAxial_Pattern)
		{
			case RADIAL: // Axis Left CCW
			{
				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CW); //0a
				//if (StopGreekKey() || stopAll)
				//{
				//	goto EndLoop;

				//}
				//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //0b
				//if (StopGreekKey() || stopAll)
				//{
				//	goto EndLoop;

				//}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CW, true); //1
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //2
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW, true); //3
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //4
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW, true); //5
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


				//stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CW); //0a
				//if (StopGreekKey() || stopAll)
				//{
				//	goto EndLoop;

				//}
			
				//stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW); //0b
				//if (StopGreekKey() || stopAll)
				//{
				//	goto EndLoop;

				//}
			
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CW); //1
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
			
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW, true); //2
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
			
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //3
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
			
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW, true); //4
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
			
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //5
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
			
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW, true); //6
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
	if (configGreekKey.radialOrAxial_Pattern == AXIAL)
	{
		totalSpindleSegments = 2;
	}
	GreekKeyPattern_Initial(totalSpindleSegments);

	for (int i = 1; i <= configGreekKey.patternCount_Pattern; i++)
	{

		switch (configGreekKey.radialOrAxial_Pattern)
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
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CW, true); //1
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //2
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW, true); //3
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;		
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //4
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW, true); //5
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;		
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, DIR_CCW); //6
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW, true); // 7
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;	
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //8
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW, true); //9
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;			
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //10
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;		
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW, true); //11
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
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW, true); //0a
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW, true); //0b
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //1
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW, true); //2
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //3
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW, true); //4
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CCW); //5
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, DIR_CCW, true); //6
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //7
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CW, true); // 8
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, DIR_CW); //9
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, DIR_CCW, true); //10
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, DIR_CCW); //11
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, DIR_CCW, true); //12
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

/// <summary>
/// GreekKeyPattern_Initial
/// </summary>
/// <comment>
/// </comment>
/// <param name="segmentCount">Segment count</param>
/// <returns></returns>
void GreekKeyPattern_Initial(int segmentCount)
{
	Serial.println("Enter GreekKeyPattern_Initial");
	// Reset end positions
	endPosition_Spindle = 0;
	endPosition_Axis = 0;

	// Set spindle segment steps
	float revolutions_Spindle = configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle;
	spindleShortLegSteps = (int)round((revolutions_Spindle) / (configGreekKey.countPatternPer360_Pattern) / segmentCount);

	// Set axis segment steps
	float distance_Axis = 0;
	switch (configGreekKey.axisId)
	{
		case ID_AXIS_Z: // Z Axis
		{
			distance_Axis = configGreekKey.segmentLength_Pattern / configSetup.distancePerRev_AxisZ;
			axisShortLegSteps = (int)round(distance_Axis * configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z);
#ifdef DEBUG
			Serial.print("Z segmentLengthPattern: ");
			Serial.println(configGreekKey.segmentLength_Pattern);
			Serial.print("Z configSetup.distancePerRev_AxisZ: ");
			Serial.println(configSetup.distancePerRev_AxisZ);
			Serial.print("Z distance_Axis: ");
			Serial.println(distance_Axis);
#endif // DEBUG
			break;
		}
		case ID_AXIS_X: // X Axis
		{
			distance_Axis = configGreekKey.segmentLength_Pattern / configSetup.distancePerRev_AxisX;
			axisShortLegSteps = (int)round(distance_Axis * configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X);
			break;
		}
		case ID_AXIS_B: // B Axis
		{
			distance_Axis = configGreekKey.segmentLength_Pattern;// / configSetup.distancePerRev_AxisB;
#ifdef DEBUG
			Serial.print("segmentLengthPattern:                 ...: ");
			Serial.println(configGreekKey.segmentLength_Pattern);
#endif // DEBUG
			switch (configSetup.radialOrLinear_Axis_B)
			{
				case RADIAL_B: // Radial
				{
					axisShortLegSteps = DistanceToSteps_RadialB(distance_Axis);
					break;
				}
				case LINEAR_B: // Linear
				{
					axisShortLegSteps = DistanceToSteps_LinearB(distance_Axis);

					break;
				}
			}
			break;
		}
	}

#ifdef DEBUG
	Serial.print("spindleShortLegSteps:");
	Serial.println(spindleShortLegSteps);
	Serial.print("configGreekKey_Main.countPattern360:");
	Serial.println(configGreekKey.countPatternPer360_Pattern);

	Serial.print("configGreekKey_Main.segmentLengthPattern:");
	Serial.println(configGreekKey.segmentLength_Pattern);
	Serial.print("0.axisShortLegSteps:");
	Serial.println(axisShortLegSteps);
	Serial.println("--------------------");
#endif // DEBUG
}

/// <summary>
/// GreekKeyPattern_End
/// </summary>
/// <comment>
/// </comment>
/// <param name=""></param>
/// <returns></returns>
void GreekKeyPattern_End()
{
	const char* nextionEnd = "\xFF\xFF\xFF";

	// Update Nextion
	//SerialPrint("pageGrk.bt3.pco=0");
	//SerialPrint(nextionEnd);
	SerialPrint("pageGrk.va0.val=0");
	SerialPrint(nextionEnd);
	SerialPrint("pageGrk.bt3.val=0");
	SerialPrint(nextionEnd);
	SerialPrint("pageGrk.bt2.val=0");
	SerialPrint(nextionEnd);
	SerialPrint("pageGrk.bt1.val=0");
	SerialPrint(nextionEnd);

	degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
	switch (configGreekKey.axisId)
	{
		case ID_AXIS_Z: // Z Axis
		{
			distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);
			SetEnable(ID_AXIS_Z, false);
			break;
		}
		case ID_AXIS_X: // X Axis
		{
			distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_X);
			SetEnable(ID_AXIS_X, false);
			break;
		}
		case ID_AXIS_B: // B Axis
		{
			distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_B);
			SetEnable(ID_AXIS_B, false);
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
bool GreekKey_Move_Axis(float segmentSteps, float multiplier, int direction, bool updatePosition)
{
	bool retVal = false;
	int newDirection = direction * reverseDirection;
	String axisName = "";
	int stepPin = 0;
	int dirPin = 0;
	int maxSpeed = 0;
	int accel = 0;
	int speedPercentage = 0;
	int actualSpeed = 0;
	int currentAxis = configGreekKey.axisId;

	// When updatePosition is false, the cutter is moved in or out of the workpiece and the distance
	// is not accumulated.
	if (!updatePosition)
	{
		switch (configGreekKey.axisId)
		{
			case ID_AXIS_Z:
			{
				currentAxis = ID_AXIS_X;
				Serial.print("X-CurrentAxis:  ");
				Serial.println(currentAxis);
				break;
			}
			case ID_AXIS_X:
			{
				currentAxis = ID_AXIS_Z;
				Serial.print("Z-CurrentAxis:  ");
				Serial.println(currentAxis);
				break;
			}
			case ID_AXIS_B: // Assumes the toolpath is on the outside of the workpiece generally moving in Z axis
			{
				currentAxis = ID_AXIS_X;
				Serial.print("X-CurrentAxis:  ");
				Serial.println(currentAxis);
				break;
			}
		}
	}
#ifdef DEBUG
	Serial.print("Enter GreekKey_Move_Axis.......................:");
	Serial.println(currentAxis);
	Serial.print("Move_Axis:segmentSteps ");
	Serial.println(segmentSteps);
	Serial.print("Move_Axis:multiplier ");
	Serial.println(multiplier);
	Serial.print("Move_Axis:newDirection ");
	Serial.println(newDirection);
	Serial.print("Move_Axis:updatePosition ");
	Serial.println(updatePosition);
#endif // DEBUG


	switch (currentAxis)
	{
		case ID_AXIS_Z:
		{
			axisName = "Z";
			stepPin = PIN_AXIS_Z_STEP;
			dirPin = PIN_AXIS_Z_DIR;
			SetEnable(ID_AXIS_Z, true);
			maxSpeed = configGreekKey.maxSpd_Axis_Z;
			accel = configGreekKey.accel_Axis_Z;
			speedPercentage = configGreekKey.speedPercent_Axis_Z;
			break;
		}
		case ID_AXIS_X:
		{
			axisName = "X";
			stepPin = PIN_AXIS_X_STEP;
			dirPin = PIN_AXIS_X_DIR;
			SetEnable(ID_AXIS_X, true);
			maxSpeed = configGreekKey.maxSpd_Axis_X;
			accel = configGreekKey.accel_Axis_X;
			speedPercentage = configGreekKey.speedPercent_Axis_X;
			break;
		}
		case ID_AXIS_B:
		{
			axisName = "B";
			stepPin = PIN_AXIS_B_STEP;
			dirPin = PIN_AXIS_B_DIR;
			SetEnable(ID_AXIS_B, true);
			maxSpeed = configGreekKey.maxSpd_Axis_B;
			accel = configGreekKey.accel_Axis_B;
			speedPercentage = configGreekKey.speedPercent_Axis_B;
			break;
		}
	}

	actualSpeed = maxSpeed * speedPercentage * .01;
	StepControl stepControllerAxis;
	Stepper stepperAxis(stepPin, dirPin);
    int32_t targetSteps = round(segmentSteps * multiplier * direction);
	stepperAxis
		.setMaxSpeed(actualSpeed)
		.setAcceleration(accel)
		.setTargetRel(targetSteps);

	stepperAxis.setPosition(0);

#ifdef DEBUG
	Serial.print("+++");
	Serial.print(axisName);
	Serial.print("-------------------------");
	Serial.print(axisName);
	Serial.println("+++");
	Serial.print("segmentSteps: ");
	Serial.println(segmentSteps);
	Serial.print("multiplier: ");
	Serial.println(multiplier);
	Serial.print("direction: ");
	Serial.println(direction);
	Serial.print("targetSteps: ");
	Serial.println(targetSteps);
	Serial.print("actualSpeed: ");
	Serial.println(actualSpeed);
	Serial.print("MaxSpeed: ");
	Serial.println(maxSpeed);
	Serial.print("Acceleration: ");
	Serial.println(accel);
	Serial.print("speedPercentage: ");
	Serial.println(speedPercentage);
	Serial.print("===");
	Serial.print(axisName);
	Serial.print("-------------------------");
	Serial.print(axisName);
	Serial.println("===");

#endif // DEBUG

	stepControllerAxis.moveAsync(stepperAxis);
	while (stepControllerAxis.isRunning())
	{
		if (StopGreekKey())
		{
#ifdef DEBUG
			Serial.println("StopGreekKey() is true:");
#endif // DEBUG
			stepControllerAxis.stop();

			// Disable all motors
			SetEnable(ID_SPINDLE, false);
			SetEnable(ID_AXIS_Z, false);
			SetEnable(ID_AXIS_X, false);
			SetEnable(ID_AXIS_B, false);

			retVal = true;
			break;
		}
		MilliDelay(5);
	}

#ifdef DEBUG
	Serial.print("endPosition_Axis:");
	Serial.println(endPosition_Axis);
#endif // DEBUG
	if (updatePosition)
	{
		// Update position only for R(ight) and L(eft), not O(ut) or I(n).
		endPosition_Axis = endPosition_Axis + stepperAxis.getPosition();
	}
#ifdef DEBUG
	Serial.print("endPosition_Axis:");
	Serial.println(endPosition_Axis);
#endif // DEBUG

#ifdef DEBUG
	Serial.print("axisId:               ....");
	Serial.println(currentAxis);
	Serial.println("Exit GreekKey_Move_Axis");
#endif // DEBUG
	return retVal;
}


/// <summary>
/// xGreekKey_Move_Axis
/// </summary>
/// <comment>
/// Selects axis to move
/// </comment>
/// <param name="shortLegLength">Segment(leg) length</param>
/// <param name="multiplier">Number of segments</param>
/// <param name="direction">Direction to move</param>
/// <returns></returns>
//bool xGreekKey_Move_Axis(float shortLegLength, float multiplier, int direction)
//{
//	bool retVal = false;
//	int newDirection = direction * reverseDirection;
//#ifdef DEBUG
//	Serial.print("Enter GreekKey_Move_Axis.......................:");
//	Serial.println(configGreekKey.axisId);
//	Serial.print("Move_Axis:shortLegLength ");
//	Serial.println(shortLegLength);
//	Serial.print("Move_Axis:multiplier ");
//	Serial.println(multiplier);
//	Serial.print("Move_Axis:newDirection ");
//	Serial.println(newDirection);
//#endif // DEBUG
//
//
//
//	switch (configGreekKey.axisId)
//	{
//		case ID_AXIS_Z:
//		{
//			retVal = GreekKey_Move_Z(shortLegLength, multiplier, newDirection, true);
//			break;
//		}
//		case ID_AXIS_X:
//		{
//			retVal = GreekKey_Move_X(shortLegLength, multiplier, newDirection, true);
//			break;
//		}
//		case ID_AXIS_B:
//		{
//			retVal = GreekKey_Move_B(shortLegLength, multiplier, newDirection, true);
//			break;
//		}
//	}
//#ifdef DEBUG
//	Serial.print("axisId:               ....");
//	Serial.println(configGreekKey.axisId);
//	Serial.println("Exit GreekKey_Move_Axis");
//#endif // DEBUG
//	return retVal;
//}

/// <summary>
/// GreekKey_Move_Axis_Alt
/// </summary>
/// <comment>
/// Used to move cutter away from and to workpiece
/// </comment>
/// <param name="distance">Distance to move</param>
/// <param name="direction">Direction to move</param>
/// <returns></returns>
//bool GreekKey_Move_Axis_Alt(float distance, float multiplier, int direction)
//{
//	bool retVal = false;
//
//	// Move the opposite axis. Note: Moves cutter into or out of workpiece.
//
//	switch (configGreekKey.axisId)
//	{
//		case ID_AXIS_Z:
//		{
//			retVal = GreekKey_Move_X(distance, multiplier, direction, false);
//			SetEnable(ID_AXIS_X, false);
//			break;
//		}
//		case ID_AXIS_X:
//		{
//			retVal = GreekKey_Move_Z(distance, multiplier, direction, false);
//			SetEnable(ID_AXIS_Z, false);
//			break;
//		}
//		case ID_AXIS_B:
//		{
//			// Assumes the toolpath is on the outside of the workpiece generally moving in Z axis
//			retVal = GreekKey_Move_X(distance, multiplier, direction, false);
//			SetEnable(ID_AXIS_X, false);
//			break;
//		}
//	}
//	Serial.println("Exit GreekKey_Move_Axis_Alt");
//	return retVal;
//}

/// <summary>
/// GreekKey_Move_Angular_TeensyStep
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

	maxSpd_Spindle = configGreekKey.maxSpd_Spindle;
	accel_Spindle = configGreekKey.accel_Spindle;
	speedPercent_Spindle = configGreekKey.speedPercent_Spindle;
	switch (configGreekKey.axisId)
	{
		case ID_AXIS_Z:
		{
			stepPin_Axis = PIN_AXIS_Z_STEP;
			dirPin_Axis = PIN_AXIS_Z_DIR;
			SetEnable(ID_AXIS_Z, true);

			maxSpd_Axis = configGreekKey.maxSpd_Axis_Z;
			accel_Axis = configGreekKey.accel_Axis_Z;
			speedPercent_Axis = configGreekKey.speedPercent_Axis_Z;

			break;
		}
		case ID_AXIS_X:
		{
			stepPin_Axis = PIN_AXIS_X_STEP;
			dirPin_Axis = PIN_AXIS_X_DIR;

			SetEnable(ID_AXIS_X, true);

			maxSpd_Axis = configGreekKey.maxSpd_Axis_X;
			accel_Axis = configGreekKey.accel_Axis_X;
			speedPercent_Axis = configGreekKey.speedPercent_Axis_X;

			break;
		}
		case ID_AXIS_B:
		{
			stepPin_Axis = PIN_AXIS_B_STEP;
			dirPin_Axis = PIN_AXIS_B_DIR;

			SetEnable(ID_AXIS_B, true);

			maxSpd_Axis = configGreekKey.maxSpd_Axis_B;
			accel_Axis = configGreekKey.accel_Axis_B;
			speedPercent_Axis = configGreekKey.speedPercent_Axis_B;
#ifdef DEBUG
			Serial.print("MaxSpd:");
			Serial.println(configGreekKey.maxSpd_Axis_B);
			Serial.print("Accel:");
			Serial.println(configGreekKey.accel_Axis_B);
			Serial.print("SpeedPercent:");
			Serial.println(configGreekKey.speedPercent_Axis_B);
#endif // DEBUG
			break;
		}
	}

	// ToDo:
	AngularMaxSpeed(maxSpd_Spindle, maxSpd_Axis);

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
		MilliDelay(5);
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
/// Uses TeensyStep
/// </comment>
/// <param name="segmentSteps">Segment(leg) length</param>
/// <param name="multiplier">Number of segments</param>
/// <param name="direction">Direction to move</param>
/// <returns>True if stop button touched otherwise false</returns>
bool GreekKey_Move_Spindle(float segmentSteps, float multiplier, int direction)
{
	Serial.println("Enter GreekKey_Move_Spindle.......................");
	bool retVal = false;

	SetEnable(ID_SPINDLE, true);
	StepControl stepControllerSpindle;

	Stepper stepper_Spindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
		stepper_Spindle
			.setMaxSpeed(configGreekKey.maxSpd_Spindle * configGreekKey.speedPercent_Spindle * .01)
			.setAcceleration(configGreekKey.accel_Spindle);

	int32_t mSpeed = configGreekKey.maxSpd_Spindle * configGreekKey.speedPercent_Spindle * .01;


	stepper_Spindle.setMaxSpeed(mSpeed);
	stepper_Spindle.setPosition(0);

	int32_t targetSteps = segmentSteps * multiplier * direction;
#ifdef DEBUG
	Serial.print("targetSteps:");
	Serial.println(targetSteps);
	targetSteps = round(targetSteps);
	Serial.print("targetSteps:");
	Serial.println(targetSteps);
	Serial.print("shortLegLength:");
	Serial.println(segmentSteps);
	Serial.print("multiplier:");
	Serial.println(multiplier);
	Serial.print("direction:");
	Serial.println(direction);
#endif // DEBUG

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

		MilliDelay(5);
	}
#ifdef DEBUG
	Serial.print("endPosition_Spindle:");
	Serial.println(endPosition_Spindle);
#endif // DEBUG


	endPosition_Spindle = endPosition_Spindle + stepper_Spindle.getPosition();
#ifdef DEBUG
	Serial.print("endPosition_Spindle:");
	Serial.println(endPosition_Spindle);
	Serial.println("Exit GreekKey_Move_Spindle.......................");
#endif // DEBUG

	return retVal;
}

/// <summary>
/// GreekKey_Move_X
/// </summary>
/// <comment>
/// Uses TeensyStep
/// </comment>
/// <param name="segmentSteps">Segment(leg) length</param>
/// <param name="multiplier">Number of segments</param>
/// <param name="direction">Direction to move</param>
/// <returns>True if stop button touched otherwise false</returns>
//bool GreekKey_Move_X(float segmentSteps, float multiplier, int direction, bool updatePosition)
//{
//	bool retVal = false;
//#ifdef DEBUG
//	Serial.print("XshortLegLength:");
//	Serial.println(segmentSteps);
//	Serial.print("Xmultiplier:");
//	Serial.println(multiplier);
//	Serial.print("Xdirection:");
//	Serial.println(direction);
//	Serial.print("XTarget:");
//	Serial.println(segmentSteps * multiplier * direction);
//#endif // DEBUG
//
//	SetEnable(ID_AXIS_X, true);
//	StepControl stepControllerAxis;
//
//	int mSpeed = configGreekKey.maxSpd_Axis_X * configGreekKey.speedPercent_Axis_X * .01;
//#ifdef DEBUG
//	Serial.print("mSpeed---------X--:");
//	Serial.println(mSpeed);
//	Serial.print("maxSpd_Axis------:");
//	Serial.println(configGreekKey.maxSpd_Axis_X);
//	Serial.print("speedPercent_Axis------------:");
//	Serial.println(configGreekKey.speedPercent_Axis_X);
//	Serial.print("accel_Axis-------:");
//	Serial.println(configGreekKey.accel_Axis_X);
//#endif // DEBUG
//	stepperAxis_X
//		.setMaxSpeed(mSpeed)
//		.setAcceleration(configGreekKey.accel_Axis_X);
//	stepperAxis_X.setPosition(0);
//	int32_t targetSteps = round(segmentSteps * multiplier * direction);
//	stepperAxis_X.setTargetRel(targetSteps);
//	stepControllerAxis.moveAsync(stepperAxis_X);
//	while (stepControllerAxis.isRunning())
//	{
//		if (StopGreekKey())
//		{
//#ifdef DEBUG
//			Serial.println("StopGreekKey() is true:");
//#endif // DEBUG
//			stepControllerAxis.stop();
//
//			SetEnable(ID_SPINDLE, false);
//			SetEnable(ID_AXIS_Z, false);
//			SetEnable(ID_AXIS_X, false);
//
//			retVal = true;
//			break;
//		}
//		MilliDelay(5);
//	}
//
//#ifdef DEBUG
//	Serial.print("endPosition_Axis:");
//	Serial.println(endPosition_Axis);
//#endif // DEBUG
//	if (updatePosition)
//	{
//		// Update position only for R(ight) and L(eft), not O(ut) or I(n).
//		endPosition_Axis = endPosition_Axis + stepperAxis_X.getPosition();
//	}
//#ifdef DEBUG
//	Serial.print("endPosition_Axis:");
//	Serial.println(endPosition_Axis);
//#endif // DEBUG
//	return retVal;
//}

/// <summary>
/// GreekKey_Move_B
/// </summary>
/// <comment>
/// Uses TeensyStep
/// </comment>
/// <param name="segmentSteps">Segment(leg) length</param>
/// <param name="multiplier">Number of segments</param>
/// <param name="direction">Direction to move</param>
/// <returns>True if stop button touched otherwise false</returns>
//bool GreekKey_Move_B(float segmentSteps, float multiplier, int direction, bool updatePosition)
//{
//	bool retVal = false;
//#ifdef DEBUG
//
//
//#endif // DEBUG
//
//	SetEnable(ID_AXIS_B, true);
//	StepControl stepControllerAxis;
//
//	int actualSpeed = configGreekKey.maxSpd_Axis_B * configGreekKey.speedPercent_Axis_B * .01;
//	stepperAxis_B
//		.setMaxSpeed(actualSpeed)
//		.setAcceleration(configGreekKey.accel_Axis_B);
//	stepperAxis_B.setPosition(0);
//	int32_t targetSteps = round(segmentSteps * multiplier * direction);
//	stepperAxis_B.setTargetRel(targetSteps);
//#ifdef DEBUG
//	Serial.println("+++B======================================B+++");
//	Serial.print("BshortLegLength:-----------B-");
//	Serial.println(segmentSteps);
//	Serial.print("Bmultiplier:");
//	Serial.println(multiplier);
//	Serial.print("Bdirection:");
//	Serial.println(direction);
//	Serial.print("targetSteps:");
//	Serial.println(targetSteps);
//	Serial.print("mSpeed---------B--:");
//	Serial.println(actualSpeed);
//	Serial.print("MaxSpd:");
//	Serial.println(configGreekKey.maxSpd_Axis_B);
//	Serial.print("Accel:");
//	Serial.println(configGreekKey.accel_Axis_B);
//	Serial.print("SpeedPercent:");
//	Serial.println(configGreekKey.speedPercent_Axis_B);
//	Serial.println("=B======================================B=");
//
//#endif // DEBUG
//
//
//	stepControllerAxis.moveAsync(stepperAxis_B);
//	while (stepControllerAxis.isRunning())
//	{
//		if (StopGreekKey())
//		{
//#ifdef DEBUG
//			Serial.println("StopGreekKey() is true:");
//#endif // DEBUG
//			stepControllerAxis.stop();
//
//			SetEnable(ID_SPINDLE, false);
//			SetEnable(ID_AXIS_Z, false);
//			SetEnable(ID_AXIS_X, false);
//			SetEnable(ID_AXIS_B, false);
//
//			retVal = true;
//			break;
//		}
//		MilliDelay(5);
//	}
//
//#ifdef DEBUG
//	Serial.print("endPosition_Axis:");
//	Serial.println(endPosition_Axis);
//#endif // DEBUG
//	if (updatePosition)
//	{
//		// Update position only for R(ight) and L(eft), not O(ut) or I(n).
//		endPosition_Axis = endPosition_Axis + stepperAxis_B.getPosition();
//	}
//#ifdef DEBUG
//	Serial.print("endPosition_Axis:");
//	Serial.println(endPosition_Axis);
//#endif // DEBUG
//	return retVal;
//}

/// <summary>
/// GreekKey_Move_Z
/// </summary>
/// <comment>
/// Uses TeensyStep
/// </comment>
/// <param name="segmentSteps">Segment(leg) length</param>
/// <param name="multiplier">Number of segments</param>
/// <param name="direction">Direction to move</param>
/// <returns>True if stop button touched otherwise false</returns>
//bool GreekKey_Move_Z(float segmentSteps, float multiplier, int direction, bool updatePosition)
//{ 
//	Serial.println("Enter GreekKey_Move_Z.......................");
//	bool retVal = false;
//#ifdef DEBUG
//	Serial.print("Move_Z:shortLegLength ");
//	Serial.println(segmentSteps);
//	Serial.print("Move_Z:multiplier ");
//	Serial.println(multiplier);
//	Serial.print("Move_Z:direction ");
//	Serial.println(direction);
//	Serial.print("ZTarget:");
//	Serial.println(segmentSteps * multiplier * direction);
//	Serial.print("configGreekKey_Z.accel_Axis:");
//	Serial.println(configGreekKey.accel_Axis_Z);
//
//#endif // DEBUG
//	SetEnable(ID_AXIS_Z, true);
//	StepControl stepControllerAxis;
//
//	int mSpeed = configGreekKey.maxSpd_Axis_Z * configGreekKey.speedPercent_Axis_Z * .01;
//#ifdef DEBUG
//	Serial.print("mSpeed---------Z--:");
//	Serial.println(mSpeed);
//	Serial.print("maxSpd_Axis------:");
//	Serial.println(configGreekKey.maxSpd_Axis_Z);
//	Serial.print("speedPercent_Axis------------:");
//	Serial.println(configGreekKey.speedPercent_Axis_Z);
//	Serial.print("accel_Axis-------:");
//	Serial.println(configGreekKey.accel_Axis_Z);
//#endif // DEBUG
//
//	stepperAxis_Z
//		.setMaxSpeed(mSpeed)
//		.setAcceleration(configGreekKey.accel_Axis_Z);
//	stepperAxis_Z.setPosition(0);
//	int targetSteps = (int)round(segmentSteps * multiplier * direction);
//#ifdef DEBUG
//	Serial.print("Move_Z:targetSteps ");
//	Serial.println(targetSteps);
//#endif // DEBUG
//	stepperAxis_Z.setTargetRel(targetSteps);
//	stepControllerAxis.moveAsync(stepperAxis_Z);
//	while (stepControllerAxis.isRunning())
//	{
//		if (StopGreekKey())
//		{
//			stepControllerAxis.stop();
//
//			SetEnable(ID_SPINDLE, false);
//			SetEnable(ID_AXIS_Z, false);
//			SetEnable(ID_AXIS_X, false);
//
//			retVal = true;
//			break;
//		}
//		MilliDelay(5);
//	}
//
//#ifdef DEBUG
//	Serial.print("endPosition_Axis:");
//	Serial.println(endPosition_Axis);
//#endif // DEBUG
//	if (updatePosition)
//	{
//		// Update position only for R(ight) and L(eft), not O(ut) or I(n).
//		endPosition_Axis = endPosition_Axis + stepperAxis_Z.getPosition();
//	}
//#ifdef DEBUG
//	Serial.print("endPosition_Axis:");
//	Serial.println(endPosition_Axis);
//
//	Serial.println("Exit GreekKey_Move_Z.......................");
//#endif // DEBUG
//
//	return retVal;
//}

/// <summary>
/// GreekKeyFromFile
/// </summary>
/// <comment>
/// Move spindle and axis independently from values stored in file
/// </comment>
/// <returns></returns>
void GreekKey_FromFile(int direction)
{
	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	const char* nextionEnd = "\xFF\xFF\xFF";

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
	int shortSegmentStepsAxisB = 0;
	int axisSteps = 0;

	int spindleShortLegSteps = 0;
	int spindleSegmentCount = 0;
	int spindleSteps = 0;

	// Reset end positions
	endPosition_Spindle = 0;
	endPosition_Axis = 0;

	// String variables
	const char* pageBE_t3 = "pageBE.t3.txt="; // Spindle Begin
	const char* pageBE_t1 = "pageBE.t1.txt="; // Spindle End
	const char* pageBE_t4 = "pageBE.t3.txt="; // Axis Begin
	const char* pageBE_t2 = "pageBE.t1.txt="; // Axis End
	const char* pageGrkFile_t15 = "pageGrkFile.t15.txt="; // Segments
	const char* pageGrkFile_t21 = "pageGrkFile.t21.txt="; // Direction
	const char* pageGrkFile_t22 = "pageGrkFile.t22.txt="; // Line Number

#ifdef DEBUG
	Serial.print(">>>>>>>>>>>>>>>>>>>>segmentOrActual:");
	Serial.println(configGreekKey.segmentOrActual);
#endif // DEBUG
	// Get Spindle segment count and line count from file :
	badFilename = false;
	if (configGreekKey.segmentOrActual == 2) // 2: Segment 
	{
		for (lineCounter = 0; lineCounter <= fileLineCount; lineCounter++)
		{
			segmentMultiplier = GetGreekKeyDataFromSD(lineCounter);
			if (badFilename)
			{
#ifdef DEBUG
				Serial.println("Exit Greek Key: File not found");
#endif // DEBUG
				SerialPrint("pageGrkFile.tFileName.txt=");
				SerialWrite(0x22);
				SerialPrint("File not found");
				SerialPrint(nextionQuoteEnd);

				goto EndLoops;
			}

#ifdef DEBUG
			//Serial.print(">>>segmentMultiplier:");
			//Serial.println(segmentMultiplier);
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
					if (configGreekKey.radialOrAxial_File == RADIAL)
					{
						spindleSegmentCount = segmentMultiplier;
					}
					break;
				}

				case 65: // A - Axial segment count
				case 97: // a
				{
					if (configGreekKey.radialOrAxial_File == AXIAL)
					{
						spindleSegmentCount = segmentMultiplier;
					}
					break;
				}
			}
#ifdef DEBUG
			//Serial.print("spindleSegmentCount:");
			//Serial.println(spindleSegmentCount);
#endif // DEBUG
		}
		
	EndSegmentCount:
		fileLineCount = lineCounter;
		
		//int stepsPerRevolution_Spindle = (int)(floor(configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle));
		int stepsPerRevolution_Spindle = (int)(round(configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle));
		float patternDivisor = ((float)configGreekKey.countPatternPer360_File * (float)spindleSegmentCount);
		//spindleShortLegSteps = (int)((floor(stepsPerRevolution_Spindle)) / (patternDivisor));
		spindleShortLegSteps = (int)((round(stepsPerRevolution_Spindle)) / (patternDivisor));
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
	shortSegmentStepsAxisZ = (int)(round((configGreekKey.segmentLength_File / configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z)));
	shortSegmentStepsAxisX = (int)(round((configGreekKey.segmentLength_File / configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X)));
	switch (configSetup.radialOrLinear_Axis_B)
	{
		case RADIAL_B:
		{
			shortSegmentStepsAxisB = (int)(round(DistanceToSteps_RadialB(configGreekKey.segmentLength_File)));
		}
		case LINEAR_B:
		{
			shortSegmentStepsAxisB = (int)(round(DistanceToSteps_LinearB(configGreekKey.segmentLength_File)));
		}
	}
	
#ifdef DEBUG
	Serial.print("configGreekKey.axisId:");
	Serial.println(configGreekKey.axisId);
#endif // DEBUG
	switch (configGreekKey.axisId)
	{
		case ID_AXIS_Z: // Z Axis
		{	
			shortSegmentStepsAxis = shortSegmentStepsAxisZ;
#ifdef DEBUG
			Serial.print("configGreekKey_Main.patternCount_File:");
			Serial.println(configGreekKey.patternCount_File);
			Serial.print("segmentLength_File:");
			Serial.println(configGreekKey.segmentLength_File);
			Serial.print("configSetup.distancePerRev_AxisZ:");
			Serial.println(configSetup.distancePerRev_AxisZ);
			Serial.print("configSetup.steps360_Axis_Z:");
			Serial.println(configSetup.steps360_Axis_Z);
			Serial.print("configSetup.microsteps_Axis_Z:");
			Serial.println(configSetup.microsteps_Axis_Z);
			Serial.print("shortSegmentStepsAxisZ:");
			Serial.println(shortSegmentStepsAxisZ);
#endif // DEBUG
			break;
		}
		case ID_AXIS_X: // X Axis
		{
			shortSegmentStepsAxis = shortSegmentStepsAxisX;
#ifdef DEBUG
			Serial.print("configGreekKey_Main.patternCount_File:");
			Serial.println(configGreekKey.patternCount_File);
			Serial.print("segmentLength_File:");
			Serial.println(configGreekKey.segmentLength_File);
			Serial.print("configSetup.distancePerRev_AxisX:");
			Serial.println(configSetup.distancePerRev_AxisX);
			Serial.print("configSetup.steps360_Axis_X:");
			Serial.println(configSetup.steps360_Axis_X);
			Serial.print("configSetup.microsteps_Axis_X:");
			Serial.println(configSetup.microsteps_Axis_X);
			Serial.print("shortSegmentStepsAxisX:");
			Serial.println(shortSegmentStepsAxisX);
#endif // DEBUG
			break;
		}
		case ID_AXIS_B: // B Axis
		{
			shortSegmentStepsAxis = shortSegmentStepsAxisB;
#ifdef DEBUG
			Serial.print("configGreekKey_Main.patternCount_File:");
			Serial.println(configGreekKey.patternCount_File);
			Serial.print("segmentLength_File:");
			Serial.println(configGreekKey.segmentLength_File);
			Serial.print("configSetup.distancePerRev_AxisB:");
			Serial.println(configSetup.distancePerRev_AxisB);
			Serial.print("configSetup.steps360_Axis_B:");
			Serial.println(configSetup.steps360_Axis_B);
			Serial.print("configSetup.microsteps_Axis_B:");
			Serial.println(configSetup.microsteps_Axis_B);
			Serial.print("shortSegmentStepsAxisB:");
			Serial.println(shortSegmentStepsAxisB);
#endif // DEBUG
			break;
		}
	}



	for (int j = 1; j <= configGreekKey.patternCount_File; j++)
	{ 
		exitInnerForLoop = false;


		// Inner loop
		for (int i = 0; i <= fileLineCount; i++)
		{

			// Reset hVal and vVal
			angularAxisLegLength = 0;
			angularSpindleLegLength = 0;

			// Get data
			segmentMultiplier = GetGreekKeyDataFromSD(i);

			SerialPrint(pageGrkFile_t22);
			SerialWrite(0x22);
			SerialPrint(i);
			SerialPrint(nextionQuoteEnd);

			SerialPrint(pageGrkFile_t21);
			SerialWrite(0x22);
			SerialPrint(currentCommand);
			SerialPrint(nextionQuoteEnd);


			SerialPrint(pageGrkFile_t15);
			SerialWrite(0x22);
			SerialPrint(segmentMultiplier);
			SerialPrint(nextionQuoteEnd);
	
	#ifdef DEBUG
			Serial.print("segmentMultiplier:");
			Serial.println(segmentMultiplier);
			Serial.print("moveType:");
			Serial.println(moveType);
			Serial.print("Segment/Actual:");
			Serial.println(configGreekKey.segmentOrActual);
			Serial.print("Line Number:");
			Serial.println(i);

	#endif // DEBUG

	#ifdef DEBUG

			//Serial.print("Comment0:");
			//Serial.println(comment);
	#endif // DEBUG

			// Set pageBE begin values to 0
			SerialPrint(pageBE_t3);
			SerialWrite(0x22);
			SerialPrint(endPosition_Spindle);
			SerialPrint(nextionQuoteEnd);

			SerialPrint(pageBE_t4);
			SerialWrite(0x22);
			SerialPrint(endPosition_Axis);
			SerialPrint(nextionQuoteEnd);

			// Set pageBE end values to 0
			SerialPrint(pageBE_t1);
			SerialWrite(0x22);
			SerialPrint(endPosition_Spindle);
			SerialPrint(nextionQuoteEnd);

			SerialPrint(pageBE_t2);
			SerialWrite(0x22);
			SerialPrint(endPosition_Axis);
			SerialPrint(nextionQuoteEnd);

			switch (moveType)
			{
				case 68: // D  - Radial: Spindle Down CW   Axial: Axis Right CW
				{

					switch (configGreekKey.radialOrAxial_File)
					{
						case RADIAL: // Spindle Down CW
						{
							//SerialPrint(pageGrkFile_t21);
							//SerialWrite(0x22);
							//SerialPrint("Down");
							//SerialPrint(nextionQuoteEnd);
							if (configGreekKey.segmentOrActual == 2) // 2: Segment 
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
							//SerialPrint(pageGrkFile_t21);
							//SerialWrite(0x22);
							//SerialPrint("Right");
							//SerialPrint(nextionQuoteEnd);
							if (configGreekKey.segmentOrActual == 2) // 2: Segment  
							{
								stopAll = GreekKey_Move_Axis(shortSegmentStepsAxis, segmentMultiplier, DIR_CW, true);
							}
							else  //3: Actual
							{
								switch (configGreekKey.axisId)
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
									case ID_AXIS_B: // B Axis
									{
										switch (configSetup.radialOrLinear_Axis_B)
										{
											case RADIAL_B:
											{
												axisSteps = round(DistanceToSteps_RadialB(segmentMultiplier));
											}
											case LINEAR_B:
											{
												axisSteps = round(DistanceToSteps_LinearB(segmentMultiplier));
											}
										}
										
										break;
									}
								}

								stopAll = GreekKey_Move_Axis(axisSteps, 1, DIR_CW, true);
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
					//SerialPrint(pageGrkFile_t21);
					//SerialWrite(0x22);
					//SerialPrint("Angle");
					//SerialPrint(nextionQuoteEnd);
					switch (configGreekKey.radialOrAxial_File)
					{
						case RADIAL: // 
						{
							if (configGreekKey.segmentOrActual == 2) // 2: Segment  
							{
								switch (configGreekKey.axisId)
								{
									case ID_AXIS_Z: // Z Axis is primary
									{
										stopAll = GreekKey_Move_Angular_TeensyStep(shortSegmentStepsAxisZ, angularAxisLegLength * direction, spindleShortLegSteps, angularSpindleLegLength);
										break;
									}
									case ID_AXIS_X: // X Axis is primary
									{
										stopAll = GreekKey_Move_Angular_TeensyStep(shortSegmentStepsAxisX, angularAxisLegLength * direction, spindleShortLegSteps, angularSpindleLegLength);
										break;
									}
									case ID_AXIS_B: // B Axis is primary
									{
										stopAll = GreekKey_Move_Angular_TeensyStep(shortSegmentStepsAxisB, angularAxisLegLength * direction, spindleShortLegSteps, angularSpindleLegLength);
										break;
									}
								}
							}
							else  // Actual
							{
								switch (configGreekKey.axisId)
								{
									case ID_AXIS_Z: // Z Axis
									{
										axisSteps = (int)(round((angularAxisLegLength / configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z)));
										break;
									}
									case ID_AXIS_X: // X Axis
									{
										axisSteps = (int)(round((angularAxisLegLength / configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X)));
										break;
									}
									case ID_AXIS_B: // B Axis
									{
										switch (configSetup.radialOrLinear_Axis_B)
										{
											case RADIAL_B:
											{
												axisSteps = (int)(DistanceToSteps_RadialB(angularAxisLegLength));
												break;
											}
											case LINEAR_B:
											{
												axisSteps = (int)(DistanceToSteps_LinearB(angularAxisLegLength));
												break;
											}
										}
										break;
									}

								}

								axisSteps = axisSteps * direction;
								spindleSteps = round((configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (angularSpindleLegLength / 360));
								stopAll = GreekKey_Move_Angular_TeensyStep(axisSteps, 1, spindleSteps, 1);
							}
							break;
						}
						case AXIAL: // Switch spindle and axis
						{
							if (configGreekKey.segmentOrActual == 2) // 2: Segment  
							{
								switch (configGreekKey.axisId)
								{
									case ID_AXIS_Z: //
									{
										stopAll = GreekKey_Move_Angular_TeensyStep(shortSegmentStepsAxisZ, angularSpindleLegLength * direction, spindleShortLegSteps, angularAxisLegLength);
										break;
									}
									case ID_AXIS_X: // 
									{
										stopAll = GreekKey_Move_Angular_TeensyStep(shortSegmentStepsAxisX, angularSpindleLegLength * direction, spindleShortLegSteps, angularAxisLegLength);
										break;
									}
									case ID_AXIS_B: // 
									{
										stopAll = GreekKey_Move_Angular_TeensyStep(shortSegmentStepsAxisB, angularSpindleLegLength * direction, spindleShortLegSteps, angularAxisLegLength);
										break;
									}
								}
							}
							else  // Actual
							{
								switch (configGreekKey.axisId)
								{
									case ID_AXIS_Z: // Z Axis
									{
										axisSteps = round((angularSpindleLegLength / configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z));
										break;
									}
									case ID_AXIS_X: // X Axis
									{
										axisSteps = round((angularSpindleLegLength / configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X));
										break;
									}
									case ID_AXIS_B: // B Axis
									{
										switch (configSetup.radialOrLinear_Axis_B)
										{
											case RADIAL_B:
											{
												axisSteps = DistanceToSteps_RadialB(angularSpindleLegLength);
												break;
											}
											case LINEAR_B:
											{
												axisSteps = DistanceToSteps_LinearB(angularSpindleLegLength);
												break;
											}
										}
										break;
									}
								}

								axisSteps = axisSteps * direction;
								spindleSteps = round((configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (angularAxisLegLength / 360));
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
					//SerialPrint(pageGrkFile_t21);
					//SerialWrite(0x22);
					//SerialPrint("In");
					//SerialPrint(nextionQuoteEnd);
					if (configGreekKey.segmentOrActual == 2) // 2: Segment  
					{
						switch (configGreekKey.axisId)
						{
							case ID_AXIS_Z: // Z Axis is primary
							{
								//stopAll = GreekKey_Move_Axis_Alt(shortSegmentStepsAxisX, segmentMultiplier, DIR_CCW);
								stopAll = GreekKey_Move_Axis(shortSegmentStepsAxisX, segmentMultiplier, DIR_CCW, false);
								break;
							}
							case ID_AXIS_X: // X Axis is primary
							{
								//stopAll = GreekKey_Move_Axis_Alt(shortSegmentStepsAxisZ, segmentMultiplier, DIR_CCW);
								stopAll = GreekKey_Move_Axis(shortSegmentStepsAxisZ, segmentMultiplier, DIR_CCW, false);
								break;
							}
							case ID_AXIS_B: // B Axis is primary
							{
								//stopAll = GreekKey_Move_Axis_Alt(shortSegmentStepsAxisX, segmentMultiplier, DIR_CCW);
								stopAll = GreekKey_Move_Axis(shortSegmentStepsAxisX, segmentMultiplier, DIR_CCW, false);
								break;
							}
						}
					}
					else // Actual
					{
						switch (configGreekKey.axisId)
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
							case ID_AXIS_B: // B Axis
							{
								switch (configSetup.radialOrLinear_Axis_B)
								{
									case RADIAL_B:
									{
										axisSteps = DistanceToSteps_RadialB(segmentMultiplier);
										break;
									}
									case LINEAR_B:
									{
										axisSteps = DistanceToSteps_LinearB(segmentMultiplier);
										break;
									}
								}
								break;
							}
						}

						// Uses actual value
						//stopAll = GreekKey_Move_Axis_Alt(axisSteps, 1, DIR_CCW);
						stopAll = GreekKey_Move_Axis(axisSteps, 1, DIR_CCW, false);
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

					switch (configGreekKey.radialOrAxial_File)
					{
						case RADIAL: // Axis Left CCW
						{
							//SerialPrint(pageGrkFile_t21);
							//SerialWrite(0x22);
							//SerialPrint("Left");
							//SerialPrint(nextionQuoteEnd);
							if (configGreekKey.segmentOrActual == 2) // 2: Segment  
							{
								Serial.print("Z----shortSegmentStepsAxis--------------------------------------------------");
								Serial.println(shortSegmentStepsAxis);
								stopAll = GreekKey_Move_Axis(shortSegmentStepsAxis, segmentMultiplier, DIR_CCW, true);
							}
							else //3: Actual
							{
								switch (configGreekKey.axisId)
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
									case ID_AXIS_B: // B Axis
									{
										switch (configSetup.radialOrLinear_Axis_B)
										{
										case RADIAL_B:
										{
											axisSteps = DistanceToSteps_RadialB(segmentMultiplier);
											break;
										}
										case LINEAR_B:
										{
											axisSteps = DistanceToSteps_LinearB(segmentMultiplier);
											break;
										}
										}
										break;
									}
								}

								stopAll = GreekKey_Move_Axis(axisSteps, 1, DIR_CCW, true);
							}
							break;
						}
						case AXIAL: // Spindle Up CCW
						{
							//SerialPrint(pageGrkFile_t21);
							//SerialWrite(0x22);
							//SerialPrint("Up");
							//SerialPrint(nextionQuoteEnd);
							if (configGreekKey.segmentOrActual == 2) // 2: Segment 
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
					//SerialPrint(pageGrkFile_t21);
					//SerialWrite(0x22);
					//SerialPrint("Out");
					//SerialPrint(nextionQuoteEnd);
					if (configGreekKey.segmentOrActual == 2) // 2: Segment  
					{
						switch (configGreekKey.axisId)
						{
							case ID_AXIS_Z: // Z Axis is primary, so use X to move cutter away from workpiece
							case ID_AXIS_B: // Assumes toolpath is on the outside of the workpiece
							{
								//stopAll = GreekKey_Move_Axis_Alt(shortSegmentStepsAxisX, segmentMultiplier, DIR_CW);
								stopAll = GreekKey_Move_Axis(shortSegmentStepsAxisX, segmentMultiplier, DIR_CW, false);
								break;
							}
							case ID_AXIS_X: // X Axis is primary, so use Z to move cutter away from workpiece
							{
								//stopAll = GreekKey_Move_Axis_Alt(shortSegmentStepsAxisZ, segmentMultiplier, DIR_CW);
								stopAll = GreekKey_Move_Axis(shortSegmentStepsAxisZ, segmentMultiplier, DIR_CW, false);
								break;
							}
						}
					}
					else
					{
						switch (configGreekKey.axisId)
						{
							case ID_AXIS_Z: // Z Axis is primary
							case ID_AXIS_B:
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
						//stopAll = GreekKey_Move_Axis_Alt(axisSteps, 1, DIR_CW);
						stopAll = GreekKey_Move_Axis(axisSteps, 1, DIR_CW, false);
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
				case 80: // P - Pause in seconds
				case 87: // W - Pause in seconds
				{
					GreekKey_Pause(segmentMultiplier);
					break;
				}
				case 82: // R - Move axis right CW
				{ 

					switch (configGreekKey.radialOrAxial_File)
					{
						case RADIAL: // Axis Right CW
						{
							//SerialPrint(pageGrkFile_t21);
							//SerialWrite(0x22);
							//SerialPrint("Right");
							//SerialPrint(nextionQuoteEnd);
							if (configGreekKey.segmentOrActual == 2) // 2: Segment  
							{
								stopAll = GreekKey_Move_Axis(shortSegmentStepsAxis, segmentMultiplier, DIR_CW, true);
							}
							else  //3: Actual
							{
								switch (configGreekKey.axisId)
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
									case ID_AXIS_B: // B Axis
									{
										switch (configSetup.radialOrLinear_Axis_B)
										{
										case RADIAL_B:
										{
											axisSteps = DistanceToSteps_RadialB(segmentMultiplier);
											break;
										}
										case LINEAR_B:
										{
											axisSteps = DistanceToSteps_LinearB(segmentMultiplier);
											break;
										}
										}
										break;
									}
								}

								stopAll = GreekKey_Move_Axis(axisSteps, 1, DIR_CW, true);
							}

							break;
						}
						case AXIAL: // Spindle Down CW
						{
							//SerialPrint(pageGrkFile_t21);
							//SerialWrite(0x22);
							//SerialPrint("Down");
							//SerialPrint(nextionQuoteEnd);
							if (configGreekKey.segmentOrActual == 2) // 2: Segment 
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

					switch (configGreekKey.radialOrAxial_File)
					{
						case RADIAL: // Spindle Up CCW
						{
							//SerialPrint(pageGrkFile_t21);
							//SerialWrite(0x22);
							//SerialPrint("Up");
							//SerialPrint(nextionQuoteEnd);
							if (configGreekKey.segmentOrActual == 2) // 2: Segment 
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
							//SerialPrint(pageGrkFile_t21);
							//SerialWrite(0x22);
							//SerialPrint("Left");
							//SerialPrint(nextionQuoteEnd);
							if (configGreekKey.segmentOrActual == 2) // 2: Segment  
							{
								stopAll = GreekKey_Move_Axis(shortSegmentStepsAxis, segmentMultiplier, DIR_CCW, true);
							}
							else //3: Actual
							{
								switch (configGreekKey.axisId)
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
									case ID_AXIS_B: // B Axis
									{
										switch (configSetup.radialOrLinear_Axis_B)
										{
										case RADIAL_B:
										{
											axisSteps = DistanceToSteps_RadialB(segmentMultiplier);
											break;
										}
										case LINEAR_B:
										{
											axisSteps = DistanceToSteps_LinearB(segmentMultiplier);
											break;
										}
										}
										break;
									}
								}

								stopAll = GreekKey_Move_Axis(axisSteps, 1, DIR_CCW, true);
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
	SerialPrint("pageGrkFile.va0.val=0");
	SerialPrint(nextionEnd);
	SerialPrint("pageGrkFile.bt3.val=0");
	SerialPrint(nextionEnd);
	SerialPrint("pageGrkFile.bt2.val=0");
	SerialPrint(nextionEnd);
	SerialPrint("pageGrkFile.bt1.val=0");
	SerialPrint(nextionEnd);

	degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
	switch (configGreekKey.axisId)
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
		case ID_AXIS_B: // B Axis
		{
			// ToDo: 
			distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_B);
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

	//// Set pageBE end values
#ifdef SHOW_POSITION 
	SerialPrint("pageBE.t1.txt=");
	SerialWrite(0x22);
	SerialPrint(degrees_Spindle);
	SerialPrint(nextionQuoteEnd);
	SerialPrint("pageBE.t2.txt=");
	SerialWrite(0x22);
	SerialPrint(distance_Axis);
	SerialPrint(nextionQuoteEnd);
#endif // Show Position

	SetEnable(ID_SPINDLE, false);
	SetEnable(ID_AXIS_Z, false);
	SetEnable(ID_AXIS_X, false); 
}

/// <summary>
/// GreekKey_Pause
/// </summary>
/// <comment>
/// Pauses Greek Key based on setting in a Greek Key file
/// </comment>
/// <param name="timeout">Pause in seconds</param>
/// <returns></returns>
void GreekKey_Pause(unsigned long timeout)
{
	MilliDelay(timeout*1000);
}

/// <summary>
/// Serial Print
/// </summary>
/// <comment>
/// Prints data to USB and Bluetooth serial ports.  Only one will be active.
/// </comment>
/// <param name="text">Text to send</param>
/// <returns></returns>
void SerialPrint(String text, int decimalPlaces = 0)
{

	switch (serialId)
	{
		case 0:
		{
			Serial.print(text);
			break;
		}
		case 1:
		{
				Serial1.print(text);
			break;
		}
		case 2:
		{
			Serial2.print(text);
			break;
		}
		case 3:
		{
			Serial3.print(text);
			break;
		}
	}
}

/// <summary>
/// Serial Print
/// </summary>
/// <comment>
/// Prints data to USB and Bluetooth serial ports.  Only one will be active.
/// </comment>
/// <param name="number">Float to send</param>
/// <returns></returns>
void SerialPrint(float number, int decimalPlaces = 0)
{
	int trim = PrintFloat(number);
	switch (serialId)
	{
	case 0:
	{
		Serial.print(number, trim);
		break;
	}
	case 1:
	{
		Serial1.print(number, trim);
		break;
	}
	case 2:
	{
		Serial2.print(number, trim);
		break;
	}
	case 3:
	{
		Serial3.print(number, trim);
		break;
	}
	}
}

/// <summary>
/// PrintFloat
/// </summary>
/// <comment>
/// Gets decimal place of a float
/// </comment>
/// <param name="floatValue">Float</param>
/// <returns>Decimal places</returns>
int PrintFloat(float floatValue) {
	int decimalPlaces;
	float tempValue = floatValue;

	for (decimalPlaces = 0; decimalPlaces < 6; decimalPlaces++) 
	{

		if (tempValue == (long)tempValue)
		{ 
			break;
		}
		tempValue *= 10.0;  // Shift left one decimal digit
	}
	//Serial.print("decimalPlaces:");
	//Serial.println(decimalPlaces);
	//Serial.print("PrintFloat: ");
	//Serial.println(floatValue, decimalPlaces);
	return decimalPlaces;
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
	switch (serialId)
	{
		case 0:
		{
			Serial.write(text);
			break;
		}
		case 1:
		{
			Serial1.write(text);
			break;
		}
		case 2:
		{
			Serial2.write(text);
			break;
		}
		case 3:
		{
			Serial3.write(text);
			break;
		}
	}
}

/// <summary>
/// Serial Print Line
/// </summary>
/// <comment>
/// Prints data to USB and Bluetooth serial ports.  Only one will be active.
/// </comment>
/// <param name="text">Text to send</param>
/// <returns></returns>
void SerialPrintln(String text)
{
	Serial.println(text);
	Serial1.println(text);
	Serial2.println(text);
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

	switch (serialPortId)
	{
		case 0:
		{
			incomingData = Serial.read();
			break;
		}
		case 1:
		{
			incomingData = Serial1.read();
			break;
		}
		case 2:
		{
			incomingData = Serial2.read();
			break;
		}
		case 3:
		{
			incomingData = Serial3.read();
			break;
		}
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
	int serialId = 0;// -1;

	if (Serial1.available() > 0)
	{
		serialId = 1;
	}
	else if (Serial2.available() > 0)
	{
		serialId = 2;
	}
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
		input = SerialRead(serialId);
		//Serial.println(input);
		// Use 255 as end chars
		if (input == 255)
		{

			if (pageCallerId == PAGE_INDEX) // pageIndex
			{
				EEPROM.write(eePromAddress_Filename_Ind + i, 0);
			}
			else // pageGrkFile
			{
				EEPROM.write(eePromAddress_Filename_Grk + i, 0);
			}
			break;
		}

		if (pageCallerId == PAGE_INDEX) // pageIndex
		{
			EEPROM.write(eePromAddress_Filename_Ind + i, input);
		}
		else // pageGrkFile
		{
			EEPROM.write(eePromAddress_Filename_Grk + i, input);
		}

	}

	if (pageCallerId == PAGE_INDEX) // pageIndex
	{
		EEPROM.put(eePromAddress_Filename_Length_Ind, i);
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
	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";

	String newSizeString = "";
	currentLineNumber = 0;
	int filename_Length;

	// Get filename from EEPROM
	EEPROM.get(eePromAddress_Filename_Length_Ind, filename_Length);
	char arChar[20];
	int i;
	for (i = 0; i <= filename_Length; i++)
	{
		arChar[i] = EEPROM.read(i + eePromAddress_Filename_Ind);
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
		

		SerialPrint("pageIndex.tFileName.txt=");
		SerialWrite(0x22);
		SerialPrint("File not found");
		SerialPrint(nextionQuoteEnd);
		return 0;
	}

	MilliDelay(100);
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
/// GetFileListFromSD
/// </summary>
/// <comment>
/// Gets file list from microSD card
/// </comment>
/// <param name="fileIndex">Index of file</param>
/// <returns></returns>
void GetFileListFromSD(int fileIndex)
{
	File dir;
	const char* pageFileList_t21 = "pageFileList.t21.txt=";
	const char* pageFileList_t22 = "pageFileList.t22.txt=";
	const char* pageFileList_t23 = "pageFileList.t23.txt=";
	const char* pageFileList_t24 = "pageFileList.t24.txt=";
	const char* pageFileList_t25 = "pageFileList.t25.txt=";
	const char* pageFileList_t26 = "pageFileList.t26.txt=";
	const char* pageFileList_t27 = "pageFileList.t27.txt=";

	//const char* pageIndex_t21 = "pageIndex.t21.txt=";
	//const char* pageIndex_t22 = "pageIndex.t22.txt=";
	//const char* pageIndex_t23 = "pageIndex.t23.txt=";
	//const char* pageIndex_t24 = "pageIndex.t24.txt=";
	//const char* pageIndex_t25 = "pageIndex.t25.txt=";
	//const char* pageIndex_t26 = "pageIndex.t26.txt=";
	//const char* pageIndex_t27 = "pageIndex.t27.txt=";
	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";

	const char* infile_4 = "4AXES.INI";

	int iCompare = 0;

	//int iCounter = 0;
	int fileArrayIndex = 0;
	String* fileNames = NULL;

	fileNames = new String[8];
#ifdef DEBUG
	Serial.println("Enter GetFileListFromSD");
#endif // DEBUG

	dir = SD.open("/");

	// Verify card exists
	if (dir)
	{
		uint32_t dirSize = dir.size();
#ifdef DEBUG
		Serial.print("Dir Size: ");
		Serial.println(dirSize);
#endif // DEBUG
	}
	else
	{
#ifdef DEBUG
		// print "Card not found"
		Serial.print("Card not found");
#endif // DEBUG
		return;
	}

	// Reset lastFileIndex
	if (fileIndex == 0)
	{
		// Default maximum files is 100
		lastFileIndex = 100;
	}
	else
	{
		// Set lastFileIndex to requested fileIndex plus 8
		// fileIndex is set whenever the up or down arrows are touched.
		// lastFileIndex = fileIndex + 6;
	}

	//////////////////////////////////////////////
	//int i = fileIndex;

	for (int iCounter = 0; iCounter < fileIndex + 7; iCounter++)
	{
		File entry = dir.openNextFile();

		Serial.println(entry.name());

		if (entry.isDirectory())
		{
			iCounter--;
		}
		else
		{
			// Exclude ini files
			// 4Axes.ini
			iCompare = strcmp(infile_4, entry.name());
			if (iCompare == 0)
			{
#ifdef DEBUG
				Serial.print("Compare1: ");
				Serial.println(entry.name());
#endif // DEBUG
				// Decrement iCounter
				iCounter--;
				goto IniFileFound;
			}

			// fileIndex is the top line in the dropdown list on the Nextion
#ifdef DEBUG
			Serial.print("iCounter: ");
			Serial.print(iCounter);
			Serial.print(" : ");
			Serial.print(entry.name());
			Serial.print(" : ");
#endif // DEBUG

			if(iCounter >= fileIndex)
			{ 
			   fileNames[fileArrayIndex] = entry.name();
			   fileArrayIndex++;
			}
#ifdef DEBUG
			Serial.println(fileNames[fileArrayIndex]);
#endif // DEBUG
		}

		// Exit when last file found
		if (!entry)
		{
			// no more files
#ifdef DEBUG
			Serial.print("No More Files: ");
			Serial.print(iCounter);
			Serial.print(" : ");
			Serial.print(lastFileIndex);
#endif // DEBUG
			lastFileIndex = iCounter + 8;
#ifdef DEBUG
			Serial.print(" : ");
			Serial.println(lastFileIndex);
#endif // DEBUG

			SerialPrint("pageSplash.t13.txt=");
			SerialWrite(0x22);
			SerialPrint(iCounter-1);
			SerialPrint(nextionQuoteEnd);
		}
	IniFileFound:
		entry.close();
	}

		for (int j=0; j < 7; j++)
		{
#ifdef DEBUG
			Serial.print("j: ");
			Serial.print(j);
			Serial.print(" : ");
			Serial.println(fileNames[j]);
#endif // DEBUG
			// Copy filenames to Nextion
			switch (j)
			{
				case 0:
				{
					SerialPrint(pageFileList_t21);
					SerialPrint("\x22");
					SerialPrint(fileNames[j]);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case 1:
				{
					SerialPrint(pageFileList_t22);
					SerialPrint("\x22");
					SerialPrint(fileNames[j]);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case 2:
				{
					SerialPrint(pageFileList_t23);
					SerialPrint("\x22");
					SerialPrint(fileNames[j]);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case 3:
				{
					SerialPrint(pageFileList_t24);
					SerialPrint("\x22");
					SerialPrint(fileNames[j]);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case 4:
				{

					SerialPrint(pageFileList_t25);
					SerialPrint("\x22");
					SerialPrint(fileNames[j]);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case 5:
				{

					SerialPrint(pageFileList_t26);
					SerialPrint("\x22");
					SerialPrint(fileNames[j]);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case 6:
				{
					SerialPrint(pageFileList_t27);
					SerialPrint("\x22");
					SerialPrint(fileNames[j]);
					SerialPrint(nextionQuoteEnd);
					break;
				}

			}
		}

	return;
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

	MilliDelay(20);
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
		currentCommand = newSizeString;
#ifdef DEBUG
	Serial.print("1.moveType:");
	Serial.println(moveType);
	Serial.println(currentCommand);
#endif

	if (moveType == 59) // ;
	{
#ifdef DEBUG
		Serial.println("MoveType 59");
#endif // DEBUG

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
		angularAxisLegLength = atof(val);
		val = strtok(NULL, "\n");
		angularSpindleLegLength = atof(val);
#ifdef DEBUG
		Serial.print("nStr: ");
		Serial.println(nStr);
		Serial.print("hVal: ");
		Serial.println(angularAxisLegLength);
		Serial.print("val: ");
		Serial.println(val);
		Serial.print("vVal: ");
		Serial.println(angularSpindleLegLength);
#endif // DEBUG

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
	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	const char* nextionEnd = "\xFF\xFF\xFF";

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
	MilliDelay(15);
	stepper_Spindle
		.setMaxSpeed(configSetup.maxSpd_Return_Spindle)
		.setAcceleration(configSetup.accel_Return_Spindle)
		.setTargetRel(target_Spindle);

#ifdef DEBUG
	Serial.println("Begin ReturnToStartPosition()");
	Serial.print("target_Spindle: ");
	Serial.println(target_Spindle);
	Serial.print("maxSpd_Return_Spindle: ");
	Serial.println(configSetup.accel_Return_Spindle);
	Serial.print("target_Axis: ");
	Serial.println(target_Axis);
#endif // DEBUG

	SetEnable(ID_SPINDLE, true);
	MilliDelay(20);
	switch (axisId)
	{
		case ID_AXIS_Z:
		{
			stepper_Z.setPosition(0);
			stepper_Z
				.setMaxSpeed(configSetup.maxSpd_Return_Axis_Z)
				.setAcceleration(configSetup.accel_Return_Axis_Z)
				.setTargetAbs(target_Axis);
#ifdef DEBUG
			Serial.print("maxSpd_Return_Axis_Z: ");
			Serial.println(configSetup.accel_Return_Axis_Z);
#endif // DEBUG
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
				.setMaxSpeed(configSetup.maxSpd_Return_Axis_X)
				.setAcceleration(configSetup.accel_Return_Axis_X)
				.setTargetAbs(target_Axis);
#ifdef DEBUG
			Serial.print("maxSpd_Return_Axis_X: ");
			Serial.println(configSetup.accel_Return_Axis_X);
#endif // DEBUG

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
				.setMaxSpeed(configSetup.maxSpd_Return_Axis_B)
				.setAcceleration(configSetup.accel_Return_Axis_B)
				.setTargetAbs(target_Axis);
#ifdef DEBUG
			Serial.print("maxSpd_Return_Axis_B: ");
			Serial.println(configSetup.accel_Return_Axis_B);
#endif // DEBUG
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

		MilliDelay(10);
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
				SerialPrint("pageMain.bt1.val=0");
				SerialPrint(nextionEnd);
				SerialPrint("pageMain.bt1.val=0");
				SerialPrint(nextionEnd);
				SerialPrint("pageMain.va0.val=0");
				SerialPrint(nextionEnd);
				SerialPrint("pageMain.bt0.val=0");
				SerialPrint(nextionEnd);
				SerialPrint("pageBE.t1.txt=");
				SerialWrite(0x22);
				SerialPrint(endPosition_Spindle);
				SerialPrint(nextionQuoteEnd);
			}
			else
			{
				SerialPrint("pageMain.bt7.val=0");
				SerialPrint(nextionEnd);
				SerialPrint("pageMain.bt7.val=0");
				SerialPrint(nextionEnd);
				SerialPrint("pageMain.va0.val=0");
				SerialPrint(nextionEnd);
				SerialPrint("pageMain.bt4.val=0");
				SerialPrint(nextionEnd);
				MilliDelay(10);

				SerialPrint("pageBE.t2.txt=");
				SerialWrite(0x22);
				SerialPrint(endPosition_Axis);
				SerialPrint(nextionQuoteEnd);
			}
			break;
		}
		case PAGE_ONE:
		{
			SerialPrint("pageOne.bt1.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageOne.bt1.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageOne.va0.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageOne.bt0.val=0");
			SerialPrint(nextionEnd);

			switch (configOne.axisId)
			{
				case ID_AXIS_Z:
				{
					SerialPrint("pageOne.bt11.val=1");
					SerialPrint(nextionEnd);
					SerialPrint("pageOne.bt11.txt=");
					SerialWrite(0x22);
					SerialPrint("Disabled");
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case ID_AXIS_X:
				{
					SerialPrint("pageOne.bt12.val=1");
					SerialPrint(nextionEnd);
					SerialPrint("pageOne.bt12.txt=");
					SerialWrite(0x22);
					SerialPrint("Disabled");
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case ID_AXIS_B:
				{
					SerialPrint("pageOne.bt13.val=1");
					SerialPrint(nextionEnd);
					SerialPrint("pageOne.bt13.txt=");
					SerialWrite(0x22);
					SerialPrint("Disabled");
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case ID_SPINDLE:
				{
					SerialPrint("pageOne.bt10.val=1");
					SerialPrint(nextionEnd);
					SerialPrint("pageOne.bt10.txt=");
					SerialWrite(0x22);
					SerialPrint("Disabled");
					SerialPrint(nextionQuoteEnd);
					break;
				}
			}

			SerialPrint("pageBE.t1.txt=");
			SerialWrite(0x22);
			SerialPrint(endPosition_Spindle);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageBE.t2.txt=");
			SerialWrite(0x22);
			SerialPrint(endPosition_Axis);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case PAGE_SYNC:
		{
			SerialPrint("pageSync.bt1.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageSync.bt1.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageSync.va0.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageSync.bt0.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageBE.t1.txt=");
			SerialWrite(0x22);
			SerialPrint(endPosition_Spindle);
			SerialPrint(nextionQuoteEnd);
			SerialPrint("pageBE.t2.txt=");
			SerialWrite(0x22);
			SerialPrint(endPosition_Axis);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case PAGE_REC:
		{
			SerialPrint("pageRec.bt1.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageRec.bt1.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageRec.va0.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageRec.bt0.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageBE.t1.txt=");
			SerialWrite(0x22);
			SerialPrint(endPosition_Spindle);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageBE.t2.txt=");
			SerialWrite(0x22);
			SerialPrint(endPosition_Axis);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case PAGE_INDEX:
		{
			SerialPrint("pageIndex.bt1.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageIndex.bt1.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageIndex.va0.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageIndex.bt0.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageBE.t1.txt=");
			SerialWrite(0x22);
			SerialPrint(endPosition_Spindle);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case PAGE_BE:
		{
			SerialPrint("pageBE.bt1.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageBE.bt1.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageBE.va0.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageBE.bt0.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageBE.t1.txt=");
			SerialWrite(0x22);
			SerialPrint(endPosition_Spindle);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case PAGE_MOVE:
		{
			SerialPrint("pageMove.bt1.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageMove.bt1.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageMove.va0.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageMove.bt0.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageBE.t2.txt=");
			SerialWrite(0x22);
			SerialPrint(endPosition_Axis);
			SerialPrint(nextionQuoteEnd);
			break;
		}

		case PAGE_GEO:
		{
			SerialPrint("pageGeo.bt1.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageGeo.bt1.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageGeo.bt0.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageGeo.va0.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageBE.t1.txt=");
			SerialWrite(0x22);
			SerialPrint(endPosition_Spindle);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageBE.t2.txt=");
			SerialWrite(0x22);
			SerialPrint(endPosition_Axis);
			SerialPrint(nextionQuoteEnd);
			initialPosition_Axis = 0;
			break;
		}
		case PAGE_GRK:
		{
			SerialPrint("pageGrk.bt1.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageGrk.bt1.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageGrk.va0.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageGrk.bt0.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageBE.t1.txt=");
			SerialWrite(0x22);
			SerialPrint(endPosition_Spindle);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageBE.t2.txt=");
			SerialWrite(0x22);
			SerialPrint(endPosition_Axis);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case PAGE_GRKFILE:
		{
			SerialPrint("pageGrkFile.bt1.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageGrkFile.bt1.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageGrkFile.va0.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageGrkFile.bt0.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageBE.t1.txt=");
			SerialWrite(0x22);
			SerialPrint(endPosition_Spindle);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageBE.t2.txt=");
			SerialWrite(0x22);
			SerialPrint(endPosition_Axis);
			SerialPrint(nextionQuoteEnd);
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
// Rose Patterns
// ================================================================================= 
// Adapted from code written by Lutz Niggl: https://github.com/luni64/TeensyStep

/// <summary>
/// slideFunc_Axis
/// </summary>
/// <comment>
/// Calculate axis positon relative to spindle
/// </comment>
/// <param name="spindleAngle">Spindle Angle</param>
/// <returns>float</returns>
float slideFunc_Axis(float spindleAngle)
{
	float phi = fmodf(spindleAngle * kRatio, TWO_PI);
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
void tick()
{
	float spindleAngle = stepperSpindle.getPosition() * (TWO_PI / spindleStepsPerRev); //convert steps to angle
	float slidePosition = 0;
	switch (configRose.axisId)
	{
		case ID_AXIS_Z:
		{
			slidePosition = stepperAxis_Z.getPosition();
			break;
		}
		case ID_AXIS_X:
		{
			slidePosition = stepperAxis_X.getPosition();
			break;
		}
		case ID_AXIS_B:
		{
			slidePosition = stepperAxis_B.getPosition();
			break;
		}
	}

	float phi = fmodf(spindleAngle * kRatio, TWO_PI);
	float slideTarget = slideStepsAmplitude * cosf(phi);

	float newSpeed = (slideTarget - slidePosition) / dtRose; // speed to reach target in given delta t (neglecting acceleration)
	float speedFac = newSpeed / newMaxSpd_RoseAxis;              // transform in relative factor (-1.0 .. 1.0)

	controllerAxis.overrideSpeed(speedFac);             // set new speed
}

/// <summary>
/// RoseRadial
/// </summary>
/// <comment>
/// Do radial rose pattern
/// </comment>
/// <param name="direction">Direction of pattern</param>
/// <returns></returns>
void RoseRadial(int direction)
{
	//const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	bool runPattern = true;
	IntervalTimer tickTimer;
	RotateControl controllerRoseSpindle;

	kRatio = (float)configRose.n / configRose.d;

	spindleStepsPerRev = configSetup.gearRatio_Spindle * configSetup.microsteps_Spindle * configSetup.steps360_Spindle;
	newMaxSpd_RoseSpindle = configRose.maxSpd_Spindle * configRose.speedPercent_Spindle * .01 * direction;
	stepperSpindle
		.setMaxSpeed(newMaxSpd_RoseSpindle)
		.setAcceleration(configRose.accel_Spindle)
		.setPosition(0);

	SetEnable(ID_SPINDLE, true);

	initialPosition_Axis = static_cast<int>(slideFunc_Axis(0));

	switch (configRose.axisId)
	{
		case ID_AXIS_Z:
		{
			slideStepsAmplitude = ((configRose.amplitude_Radial_Z / (configSetup.distancePerRev_AxisZ)) * configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z) / 2;  // Amplitude is normally measured from the middle to the top
			newMaxSpd_RoseAxis = configRose.maxSpd_Axis_Z * configRose.speedPercent_Axis_Z * .01 * direction;
			stepperAxis_Z
				.setMaxSpeed(newMaxSpd_RoseAxis)
				.setAcceleration(configRose.accel_Axis_Z)
				.setPosition(initialPosition_Axis); // set start position of counter
			SetEnable(ID_AXIS_Z, true);
			controllerAxis.rotateAsync(stepperAxis_Z);
			break;
		}
		case ID_AXIS_X:
		{
			slideStepsAmplitude = ((configRose.amplitude_Radial_X / (configSetup.distancePerRev_AxisX)) * configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X) / 2;  // Amplitude is normally measured from the middle to the top
			newMaxSpd_RoseAxis = configRose.maxSpd_Axis_X * configRose.speedPercent_Axis_X * .01 * direction;
			stepperAxis_X
				.setMaxSpeed(newMaxSpd_RoseAxis)
				.setAcceleration(configRose.accel_Axis_X)
				.setPosition(initialPosition_Axis); // set start position of counter	
			SetEnable(ID_AXIS_X, true);
			controllerAxis.rotateAsync(stepperAxis_X);
			break;
		}
		case ID_AXIS_B:
		{
			//slideStepsAmplitude = ((configRose.amplitude_Radial_B / (configSetup.distancePerRev_AxisB)) * configSetup.steps360_Axis_B * configSetup.microsteps_Axis_B) / 2;  // Amplitude is normally measured from the middle to the top
			switch (configSetup.radialOrLinear_Axis_B)
			{
				case RADIAL_B:
				{
					slideStepsAmplitude = DistanceToSteps_RadialB(configRose.amplitude_Radial_B) / 2;
					break;
				}
				case LINEAR_B:
				{
					slideStepsAmplitude = DistanceToSteps_LinearB(configRose.amplitude_Radial_B) / 2;
					break;
				}
			}

			newMaxSpd_RoseAxis = configRose.maxSpd_Axis_B * configRose.speedPercent_Axis_B * .01 * direction;
			stepperAxis_B
				.setMaxSpeed(newMaxSpd_RoseAxis)
				.setAcceleration(configRose.accel_Axis_B)
				.setPosition(initialPosition_Axis); // set start position of counter
			SetEnable(ID_AXIS_B, true);
			controllerAxis.rotateAsync(stepperAxis_B);
			break;
		}
	}

	controllerRoseSpindle.rotateAsync(stepperSpindle); // let the spindle run with constant speed

	tick();

	// use a timer to periodically calculate new targets for the slide
	tickTimer.priority(priorityLevel); // lowest priority, potentially long caclulations need to be interruptable by TeensyStep
	tickTimer.begin(tick, recalcPeriod);

	//while (controllerRoseSpindle.isRunning())
	while (runPattern)
	{
		// print current values of spindle angle [rad] and slide position [steps]
		//float phiZ = endPosition_Spindle * (TWO_PI / spindleStepsPerRev);
		//Serial.printf("%f\t%d\n", phiZ, endPosition_Axis);

		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
				case 67:
				case 99:
				{
					controllerAxis.stopAsync();
					controllerRoseSpindle.stopAsync();
					runPattern = false;
					break;
				}
			}
		}

		MilliDelay(100);
	} // End while

	tickTimer.end();
	endPosition_Spindle = stepperSpindle.getPosition();
	returnSteps_Spindle = endPosition_Spindle;
	stepperSpindle.setPosition(0);
	SetEnable(ID_SPINDLE, false);

	switch (configRose.axisId)
	{
		case ID_AXIS_Z:
		{
			endPosition_Axis = stepperAxis_Z.getPosition();
			stepperAxis_Z.setPosition(0);
			stepperAxis_Z.setTargetRel(0);
			stepperAxis_Z.setAcceleration(100);
			stepperAxis_Z.setMaxSpeed(100);
			SetEnable(ID_AXIS_Z, false);
			break;
		}
		case ID_AXIS_X:
		{
			endPosition_Axis = stepperAxis_X.getPosition();
			stepperAxis_X.setPosition(0);
			stepperAxis_X.setTargetRel(0);
			stepperAxis_X.setAcceleration(100);
			stepperAxis_X.setMaxSpeed(100);
			SetEnable(ID_AXIS_X, false);
			break;
		}
		case ID_AXIS_B:
		{
			endPosition_Axis = stepperAxis_B.getPosition();
			stepperAxis_B.setPosition(0);
			stepperAxis_B.setTargetRel(0);
			stepperAxis_B.setAcceleration(100);
			stepperAxis_B.setMaxSpeed(100);
			SetEnable(ID_AXIS_B, false);
			break;
		}
	}

	returnSteps_Axis = endPosition_Axis - initialPosition_Axis;

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
	
}

//float slideFunc_Spindle_Z(float spindleAngleZ)
//{
//	float phi = fmodf(spindleAngleZ * kRatio, TWO_PI);
//	float retVal = spindleStepsAmplitude * cosf(phi);
//	return retVal;
//}
//
////------------------------------------------------------------------------------------
//// tick()
////
//// This function is called periodically with period recalcPeriod. 
//// It calculates 
////  1) a new target value for the slide depending on the spindle angle
////  2) the new speed for the spindle so that it will reach the target until it is called again
//void tickSpindleZ(RotateControl &rotateController)
//{
//	float spindleAngleZ = stepperAxis_Z.getPosition();// *(TWO_PI / spindleStepsPerRev); //convert steps to angle
//	float slidePositionZ = stepperSpindle.getPosition();
//	float slideTargetZ = slideFunc_Spindle_Z(spindleAngleZ);
//
//	float newSpeedZ = (slideTargetZ - slidePositionZ) / dtRose; // speed to reach target in given delta t (neglecting acceleration)
//	float speedFacZ = newSpeedZ / newMaxSpd_RoseSpindle;              // transform in relative factor (-1.0 .. 1.0)
//
//	rotateController.overrideSpeed(speedFacZ);             // set new speed
//}
//// ToDo: Not Functional
//void RosePattern_Axial_Z(int direction)
//{
//	IntervalTimer tickTimerZ;
//	RotateControl controllerRose;
//	RotateControl rotateController1;
//	bool runPattern = true;
//
//	kRatio = (float)configRose.n / configRose.d;
//	spindleStepsPerRev = configSetup.gearRatio_Spindle * configSetup.microsteps_Spindle * configSetup.steps360_Spindle;
//	spindleStepsAmplitude = (configRose.amplitude_Axial_Z / 360) * spindleStepsPerRev;
//
//	slideStepsAmplitude = ((configRose.amplitude_Axial_X / (configSetup.distancePerRev_AxisZ)) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z));
//
//	newMaxSpd_RoseSpindle = configRose.maxSpd_Spindle * configRose.speedPercent_Spindle * .01;
//	stepperSpindle
//		.setMaxSpeed(newMaxSpd_RoseSpindle)
//		.setAcceleration(configRose.accel_Spindle)
//		.setPosition(0);
//
//	newMaxSpd_RoseAxis = configRose.maxSpd_Axis_Z * configRose.speedPercent_Axis_Z * .01 * direction;
//	stepperAxis_Z
//		.setMaxSpeed(newMaxSpd_RoseAxis)
//		.setAcceleration(configRose.accel_Axis_Z)
//		.setPosition(0);
//		//.setPosition(slideFunc_Axis_Z(0)); // set start position of counter
//
//	// Enable steppers
//	SetEnable(ID_SPINDLE, true);
//	SetEnable(ID_AXIS_Z, true);
//
//	rotateController1.rotateAsync(stepperAxis_Z);
//	//controllerRose_Axis.overrideSpeed(0); // start with stopped slide
//	controllerRose.rotateAsync(stepperSpindle); // let the spindle run with constant speed
//
//	tickZ(rotateController1);
//
//	// use a timer to periodically calculate new targets for the slide
//	tickTimerZ.priority(priorityLevel); // lowest priority, potentially long caclulations need to be interruptable by TeensyStep
//	tickTimerZ.begin(tickZ, recalcPeriod);
//
//	while (runPattern)
//	{
//		digitalWriteFast(LED_BUILTIN, !digitalReadFast(LED_BUILTIN));
//
//#ifdef SHOW_POSITION 
//		// Track Positions not implemented while steppers are running
//		//if (configGeneral.trackPositions == 1)
//		//{
//		//	endPosition_Axis = stepperAxis_Z.getPosition();
//		//	endPosition_Spindle = stepperSpindle.getPosition();
//
//		//	// print current values of spindle angle [rad] and slide position [steps]
//		//	//float phiZ = endPosition_Spindle * (TWO_PI / spindleStepsPerRev);
//		//	//Serial.printf("%f\t%d\n", phiZ, endPosition_Axis);
//		//	degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
//		//	distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);
//		//	SerialPrint("pageBE.t1.txt=");
//		//	SerialWrite(0x22);
//		//	SerialPrint(degrees_Spindle);
//		//	SerialWrite(0x22);
//		//	SerialWrite(0xff);
//		//	SerialWrite(0xff);
//		//	SerialWrite(0xff);
//		//	MilliDelay(5);
//		//	SerialPrint("pageBE.t1.txt=");
//		//	SerialWrite(0x22);
//		//	SerialPrint(degrees_Spindle);
//		//	SerialWrite(0x22);
//		//	SerialWrite(0xff);
//		//	SerialWrite(0xff);
//		//	SerialWrite(0xff);
//		//	MilliDelay(5);
//		//	SerialPrint("pageBE.t2.txt=");
//		//	SerialWrite(0x22);
//		//	SerialPrint(distance_Axis);
//		//	SerialWrite(0x22);
//		//	SerialWrite(0xff);
//		//	SerialWrite(0xff);
//		//	SerialWrite(0xff);
//		//}
//#endif // Show Position
//
//		// Check for Cancel code  
//		if (SerialAvailable() >= 0)
//		{
//			incomingByte = SerialRead(serialId);
//			switch (incomingByte)
//			{
//			case 67:
//			case 99:
//			{
//				runPattern = false;
//				controllerRose.overrideSpeed(0);
//				rotateController1.overrideSpeed(0);
//				controllerRose.stopAsync();
//				rotateController1.stopAsync();
//				break;
//			}
//			}
//		}
//	}
//
//	endPosition_Spindle = stepperSpindle.getPosition();
//	endPosition_Axis = stepperAxis_Z.getPosition();
//	returnSteps_Axis = endPosition_Axis;
//	returnSteps_Spindle = endPosition_Spindle;
//	stepperSpindle.setPosition(0);
//	stepperAxis_Z.setPosition(0);
//
//	SetEnable(ID_SPINDLE, false);
//	SetEnable(ID_AXIS_Z, false);
//}
//

// ========================================================
 //Test EEPROM
 //========================================================
/// <summary>
/// Write current Config screen eeprom values to Nextion
/// </summary>
/// <comment>
/// </comment>
/// <returns></returns>
void TestEEPROMConfig(int callerId)
{
	Serial.println("Enter TestEEPROMConfig");
	const char* pageConfig_t10 = "pageConfig.t10.txt=";
	const char* pageConfig_t11 = "pageConfig.t11.txt=";
	const char* pageConfig_t12 = "pageConfig.t12.txt=";
	const char* pageConfig_t13 = "pageConfig.t13.txt=";

	const char* bt1_bco = "pageConfig.bt1.bco=7158";
	const char* bt1_val = "pageConfig.bt1.val=0";

	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	const char* nextionEnd = "\xFF\xFF\xFF";

	int32_t maxSpd_Spindle = 0;
	int32_t accel_Spindle = 0;
	int32_t maxSpd_Axis_Z = 0;
	int32_t accel_Axis_Z = 0;
	int32_t maxSpd_Axis_X = 0;
	int32_t accel_Axis_X = 0;
	int32_t maxSpd_Axis_B = 0;
	int32_t accel_Axis_B = 0;

	switch (callerId)
	{
		case PAGE_MAIN:
		{
			configPageMainOne eePageMain;
			EEPROM.get(eePromAddress_Main, eePageMain);

			configPageMainOne configMain2;
			EEPROM.get(eePromAddress_Main, configMain2);

			SerialPrint(pageConfig_t10);
			SerialPrint("\x22");
			SerialPrint(eePageMain.maxSpd_Spindle);
			SerialPrint(nextionQuoteEnd);
 
			SerialPrint(pageConfig_t11);
			SerialPrint("\x22");
			SerialPrint(eePageMain.accel_Spindle);
			SerialPrint(nextionQuoteEnd);

			switch (eePageMain.axisId)
			{
				case ID_AXIS_Z:
				{
					SerialPrint(pageConfig_t12);
					SerialPrint("\x22");
					SerialPrint(eePageMain.maxSpd_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint(pageConfig_t13);
					SerialPrint("\x22");
					SerialPrint(eePageMain.accel_Axis_Z);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case ID_AXIS_X:
				{
					SerialPrint(pageConfig_t12);
					SerialWrite(0x22);
					SerialPrint(eePageMain.maxSpd_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint(pageConfig_t13);
					SerialWrite(0x22);
					SerialPrint(eePageMain.accel_Axis_X);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case ID_AXIS_B:
				{
#ifdef DEBUG
					Serial.print("configMain.accel_Axis_B:");
					Serial.println(configMain.accel_Axis_B);
					Serial.print("configMain2.accel_Axis_B:");
					Serial.println(configMain2.accel_Axis_B);
					Serial.print("eePageMain.accel_Axis_B:");
					Serial.println(eePageMain.accel_Axis_B);
#endif // DEBUG
					SerialPrint(pageConfig_t12);
					SerialWrite(0x22);
					SerialPrint(eePageMain.maxSpd_Axis_B);
					SerialPrint(nextionQuoteEnd);
					delay(100);
					SerialPrint(pageConfig_t13);
					SerialWrite(0x22);
					SerialPrint(eePageMain.accel_Axis_B);
					SerialPrint(nextionQuoteEnd);
					break;
				}
			}

			break;
		}
		case PAGE_SYNC:
		{
			configPageSync eePageSync;
			EEPROM.get(eePromAddress_Sync, eePageSync);

			SerialPrint(pageConfig_t10);
			SerialWrite(0x22);
			SerialPrint(eePageSync.maxSpd_Spindle);
			SerialPrint(nextionQuoteEnd);

			SerialPrint(pageConfig_t11);
			SerialWrite(0x22);
			SerialPrint(eePageSync.accel_Spindle);
			SerialPrint(nextionQuoteEnd);

			switch (eePageSync.axisId)
			{
				case ID_AXIS_Z:
				{
					SerialPrint(pageConfig_t12);
					SerialWrite(0x22);
					SerialPrint(eePageSync.maxSpd_Axis_Z);
					SerialPrint(nextionQuoteEnd);;

					SerialPrint(pageConfig_t13);
					SerialWrite(0x22);
					SerialPrint(eePageSync.accel_Axis_Z);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case ID_AXIS_X:
				{
					SerialPrint(pageConfig_t12);
					SerialWrite(0x22);
					SerialPrint(eePageSync.maxSpd_Axis_X);
					SerialPrint(nextionQuoteEnd);;

					SerialPrint(pageConfig_t13);
					SerialWrite(0x22);
					SerialPrint(eePageSync.accel_Axis_X);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case ID_AXIS_B:
				{
					SerialPrint(pageConfig_t12);
					SerialWrite(0x22);
					SerialPrint(eePageSync.maxSpd_Axis_B);
					SerialPrint(nextionQuoteEnd);;

					SerialPrint(pageConfig_t13);
					SerialWrite(0x22);
					SerialPrint(eePageSync.accel_Axis_B);
					SerialPrint(nextionQuoteEnd);
					break;
				}
			}
			break;
		}
		case PAGE_REC:
		{
			configPageRec eePageRec;
			EEPROM.get(eePromAddress_Rec, eePageRec);

			SerialPrint(pageConfig_t10);
			SerialWrite(0x22);
			SerialPrint(eePageRec.maxSpd_Spindle);
			SerialPrint(nextionQuoteEnd);

			SerialPrint(pageConfig_t11);
			SerialWrite(0x22);
			SerialPrint(eePageRec.accel_Spindle);
			SerialPrint(nextionQuoteEnd);
#ifdef DEBUG
			Serial.print("AxisId:");
			Serial.println(eePageRec.axisId);
#endif // DEBUG
			switch (eePageRec.axisId)
			{
				case ID_AXIS_Z:
				{
					SerialPrint(pageConfig_t12);
					SerialWrite(0x22);
					SerialPrint(eePageRec.maxSpd_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint(pageConfig_t13);
					SerialWrite(0x22);
					SerialPrint(eePageRec.accel_Axis_Z);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case ID_AXIS_X:
				{
					SerialPrint(pageConfig_t12);
					SerialWrite(0x22);
					SerialPrint(configRec.maxSpd_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint(pageConfig_t13);
					SerialWrite(0x22);
					SerialPrint(eePageRec.accel_Axis_X);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case ID_AXIS_B:
				{
					SerialPrint(pageConfig_t12);
					SerialWrite(0x22);
					SerialPrint(configRec.maxSpd_Axis_B);
					SerialPrint(nextionQuoteEnd);

					SerialPrint(pageConfig_t13);
					SerialWrite(0x22);
					SerialPrint(eePageRec.accel_Axis_B);
					SerialPrint(nextionQuoteEnd);
					break;
				}
			}
			break;
		}
		case PAGE_ONE:
		{
			configPageMainOne eePageOne;
			EEPROM.get(eePromAddress_One, eePageOne);
			maxSpd_Spindle = eePageOne.maxSpd_Spindle;
			accel_Spindle = eePageOne.accel_Spindle;
			maxSpd_Axis_Z = eePageOne.maxSpd_Axis_Z;
			accel_Axis_Z = eePageOne.accel_Axis_Z;
			maxSpd_Axis_X = eePageOne.maxSpd_Axis_X;
			accel_Axis_X = eePageOne.accel_Axis_X;
			maxSpd_Axis_B = eePageOne.maxSpd_Axis_B;
			accel_Axis_B = eePageOne.accel_Axis_B;

			Serial.println("EEProm Read");

			switch (eePageOne.axisId)
			{
				case ID_SPINDLE:
				{
					SerialPrint(pageConfig_t10);
					SerialWrite(0x22);
					SerialPrint(maxSpd_Spindle);
					SerialPrint(nextionQuoteEnd);

					SerialPrint(pageConfig_t11);
					SerialWrite(0x22);
					SerialPrint(accel_Spindle);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case ID_AXIS_Z:
				{
					SerialPrint(pageConfig_t12);
					SerialWrite(0x22);
					SerialPrint(maxSpd_Axis_Z);
					SerialPrint(nextionQuoteEnd);


					SerialPrint(pageConfig_t13);
					SerialWrite(0x22);
					SerialPrint(accel_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					break;
				}
				case ID_AXIS_X:
				{
					SerialPrint(pageConfig_t12);
					SerialWrite(0x22);
					SerialPrint(maxSpd_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint(pageConfig_t13);
					SerialWrite(0x22);
					SerialPrint(accel_Axis_X);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case ID_AXIS_B:
				{
					SerialPrint(pageConfig_t12);
					SerialWrite(0x22);
					SerialPrint(maxSpd_Axis_B);
					SerialPrint(nextionQuoteEnd);

					SerialPrint(pageConfig_t13);
					SerialWrite(0x22);
					SerialPrint(accel_Axis_B);
					SerialPrint(nextionQuoteEnd);
					break;
				}
			}

			break;
		}
		case PAGE_INDEX:
		{
			configPageIndex eepageIndex;
			EEPROM.get(eePromAddress_Ind_Main, eepageIndex);
			SerialPrint(pageConfig_t10);
			SerialWrite(0x22);
			SerialPrint(eepageIndex.maxSpd);
			SerialPrint(nextionQuoteEnd);

			SerialPrint(pageConfig_t11);
			SerialWrite(0x22);
			SerialPrint(eepageIndex.accel);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case PAGE_MOVE:
		{
			configPageMov eepageMove;
			EEPROM.get(eePromAddress_Mov, eepageMove);
			switch (eepageMove.axisId)
			{
				case ID_MOVE_AXIS_Z1:
				case ID_MOVE_AXIS_Z2:
				{
					SerialPrint(pageConfig_t12);
					SerialWrite(0x22);
					SerialPrint(eepageMove.maxSpd_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint(pageConfig_t13);
					SerialWrite(0x22);
					SerialPrint(eepageMove.accel_Axis_Z);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case ID_MOVE_AXIS_X1:
				case ID_MOVE_AXIS_X2:
				{
					SerialPrint(pageConfig_t12);
					SerialWrite(0x22);
					SerialPrint(eepageMove.maxSpd_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint(pageConfig_t13);
					SerialWrite(0x22);
					SerialPrint(eepageMove.accel_Axis_X);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case ID_MOVE_AXIS_B1:
				case ID_MOVE_AXIS_B2:
				{
					SerialPrint(pageConfig_t12);
					SerialWrite(0x22);
					SerialPrint(eepageMove.maxSpd_Axis_B);
					SerialPrint(nextionQuoteEnd);

					SerialPrint(pageConfig_t13);
					SerialWrite(0x22);
					SerialPrint(eepageMove.accel_Axis_B);
					SerialPrint(nextionQuoteEnd);
					break;
				}
			}
			break;
		}
		case PAGE_GEO:
		{
			configPageRose eePageRose;
			EEPROM.get(eePromAddress_Rose, eePageRose);

			SerialPrint(pageConfig_t10);
			SerialWrite(0x22);
			SerialPrint(eePageRose.maxSpd_Spindle);
			SerialPrint(nextionQuoteEnd);

			SerialPrint(pageConfig_t11);
			SerialWrite(0x22);
			SerialPrint(eePageRose.accel_Spindle);
			SerialPrint(nextionQuoteEnd);

			switch (eePageRose.axisId)
			{
				case ID_AXIS_Z:
				{
					SerialPrint(pageConfig_t12);
					SerialWrite(0x22);
					SerialPrint(eePageRose.maxSpd_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint(pageConfig_t13);
					SerialWrite(0x22);
					SerialPrint(eePageRose.accel_Axis_Z);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case ID_AXIS_X:
				{
					SerialPrint(pageConfig_t12);
					SerialWrite(0x22);
					SerialPrint(eePageRose.maxSpd_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint(pageConfig_t13);
					SerialWrite(0x22);
					SerialPrint(eePageRose.accel_Axis_X);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case ID_AXIS_B:
				{
					SerialPrint(pageConfig_t12);
					SerialWrite(0x22);
					SerialPrint(eePageRose.maxSpd_Axis_B);
					SerialPrint(nextionQuoteEnd);

					SerialPrint(pageConfig_t13);
					SerialWrite(0x22);
					SerialPrint(eePageRose.accel_Axis_B);
					SerialPrint(nextionQuoteEnd);
					break;
				}
			}
			break;
		}
		case PAGE_GRK:
		case PAGE_GRKFILE:
		{
			configPageGreekKey eePageGrk;
			EEPROM.get(eePromAddress_Grk, eePageGrk);
			switch (configGreekKey.axisId)
			{
				case ID_AXIS_Z:
				{

					SerialPrint(pageConfig_t10);
					SerialWrite(0x22);
					SerialPrint(eePageGrk.maxSpd_Spindle);
					SerialPrint(nextionQuoteEnd);

					SerialPrint(pageConfig_t11);
					SerialWrite(0x22);
					SerialPrint(eePageGrk.accel_Spindle);
					SerialPrint(nextionQuoteEnd);

					SerialPrint(pageConfig_t12);
					SerialWrite(0x22);
					SerialPrint(eePageGrk.maxSpd_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint(pageConfig_t13);
					SerialWrite(0x22);
					SerialPrint(eePageGrk.accel_Axis_Z);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case ID_AXIS_X: // X Axis
				{
					SerialPrint(pageConfig_t10);
					SerialWrite(0x22);
					SerialPrint(eePageGrk.maxSpd_Spindle);
					SerialPrint(nextionQuoteEnd);

					SerialPrint(pageConfig_t11);
					SerialWrite(0x22);
					SerialPrint(eePageGrk.accel_Spindle);
					SerialPrint(nextionQuoteEnd);

					SerialPrint(pageConfig_t12);
					SerialWrite(0x22);
					SerialPrint(eePageGrk.maxSpd_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint(pageConfig_t13);
					SerialWrite(0x22);
					SerialPrint(eePageGrk.accel_Axis_X);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case ID_AXIS_B: // B Axis
				{
					SerialPrint(pageConfig_t10);
					SerialWrite(0x22);
					SerialPrint(eePageGrk.maxSpd_Spindle);
					SerialPrint(nextionQuoteEnd);

					SerialPrint(pageConfig_t11);
					SerialWrite(0x22);
					SerialPrint(eePageGrk.accel_Spindle);
					SerialPrint(nextionQuoteEnd);

					SerialPrint(pageConfig_t12);
					SerialWrite(0x22);
					SerialPrint(eePageGrk.maxSpd_Axis_B);
					SerialPrint(nextionQuoteEnd);

					SerialPrint(pageConfig_t13);
					SerialWrite(0x22);
					SerialPrint(eePageGrk.accel_Axis_B);
					SerialPrint(nextionQuoteEnd);
					break;
				}
			}
			break;
		}
	}

	// Update Nextion
	SerialPrint(bt1_bco);
	SerialPrint(nextionEnd);
	SerialPrint(bt1_val);
	SerialPrint(nextionEnd);
}

/// <summary>
/// Write current Setup screen eeprom values to Nextion
/// </summary>
/// <comment>
/// </comment>
/// <returns></returns>
void TestEEPROMSetup()
{

	const char* pageSpindle_t42 = "pageSpindle.t42.txt=";
	const char* pageSpindle_t43 = "pageSpindle.t43.txt=";
	const char* pageSpindle_t44 = "pageSpindle.t44.txt=";
	const char* pageSpindle_t45 = "pageSpindle.t45.txt=";
	const char* pageSpindle_t8 = "pageSpindle.t8.txt=";
	const char* pageSpindle_t12 = "pageSpindle.t12.txt=";
	const char* pageZ_t46 = "pageZ.t46.txt=";
	const char* pageZ_t47 = "pageZ.t47.txt=";
	const char* pageZ_t48 = "pageZ.t48.txt=";
	const char* pageZ_t49 = "pageZ.t49.txt=";
	const char* pageB_t56 = "pageB.t56.txt=";
	const char* pageB_t57 = "pageB.t57.txt=";
	const char* pageB_t58 = "pageB.t58.txt=";
	const char* pageB_t59 = "pageB.t59.txt=";
	const char* pageB_t64 = "pageB.t64.txt=";
	const char* pageX_t60 = "pageX.t60.txt=";
	const char* pageX_t61 = "pageX.t61.txt=";
	const char* pageX_t62 = "pageX.t62.txt=";
	const char* pageX_t63 = "pageX.t63.txt=";

	const char* lowChar = "Low";
	const char* highChar = "High";
	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	const char* nextionEnd = "\xFF\xFF\xFF";

	configPageSetup eePromPageSetup;
	EEPROM.get(eePromAddress_Setup, eePromPageSetup);

	// Write the settings to Nextion LCD
	SerialPrint(pageSpindle_t42);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.microsteps_Spindle);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageSpindle_t43);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.steps360_Spindle);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageSpindle_t44);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.gearRatio_Spindle,4);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageSpindle_t45);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.polarity_Spindle ? lowChar : highChar);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageSpindle_t8);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.pulseWidth_Spindle);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageSpindle_t12);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.speedUpdatePeriod_Spindle);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageZ_t46);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.microsteps_Axis_Z);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageZ_t47);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.steps360_Axis_Z);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageZ_t48);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.distancePerRev_AxisZ, 4);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageZ_t49);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.polarity_Axis_Z ? lowChar : highChar);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageX_t60);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.microsteps_Axis_X);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageX_t61);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.steps360_Axis_X);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageX_t62);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.distancePerRev_AxisX, 5);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageX_t63);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.polarity_Axis_X ? lowChar : highChar);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageB_t56);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.microsteps_Axis_B);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageB_t57);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.steps360_Axis_B);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageB_t58);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.gearRatio_AxisB, 4);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageB_t59);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.polarity_Axis_B ? lowChar : highChar);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageB_t64);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.distancePerRev_AxisB, 4);
	SerialPrint(nextionQuoteEnd);

	// Update Nextion
	SerialPrint("pageSetup.bt1.bco=23964");
	SerialPrint(nextionEnd);
	SerialPrint("pageSetup.bt1.val=0");
	SerialPrint(nextionEnd);
}

/// <summary>
/// Write current Limits screen eeprom values to Nextion
/// </summary>
/// <comment>
/// </comment>
/// <returns></returns>
void TestEEPROM_Limits()
{
	const char* pageLimits_t0 = "pageLimits.t0.txt=";
	const char* pageLimits_t1 = "pageLimits.t1.txt=";
	const char* pageLimits_t2 = "pageLimits.t2.txt=";
	const char* pageLimits_t3 = "pageLimits.t3.txt=";
	const char* pageLimits_t4 = "pageLimits.t4.txt=";
	const char* pageLimits_t5 = "pageLimits.t5.txt=";

	const char* pageLimits_t6 = "pageLimits.t6.txt=";
	const char* pageLimits_t7 = "pageLimits.t7.txt=";
	const char* pageLimits_t8 = "pageLimits.t8.txt=";
	const char* pageLimits_c1_0 = "pageLimits.c1.val=0";
	const char* pageLimits_c1_1 = "pageLimits.c1.val=1";

	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	const char* nextionEnd = "\xFF\xFF\xFF";

	configPageSetup eePromPageSetup;
	EEPROM.get(eePromAddress_Setup, eePromPageSetup);

	SerialPrint(pageLimits_t0);
	SerialWrite(0x22);
	SerialPrint(configSetup.limit_Min_Z);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageLimits_t1);
	SerialWrite(0x22);
	SerialPrint(configSetup.limit_Max_Z);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageLimits_t6);
	SerialWrite(0x22);
	SerialPrint(configSetup.home_Z);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageLimits_t2);
	SerialWrite(0x22);
	SerialPrint(configSetup.limit_Min_X);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageLimits_t3);
	SerialWrite(0x22);
	SerialPrint(configSetup.limit_Max_X);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageLimits_t7);
	SerialWrite(0x22);
	SerialPrint(configSetup.home_X);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageLimits_t4);
	SerialWrite(0x22);
	SerialPrint(configSetup.limit_Min_B);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageLimits_t5);
	SerialWrite(0x22);
	SerialPrint(configSetup.limit_Max_B);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageLimits_t8);
	SerialWrite(0x22);
	SerialPrint(configSetup.home_B);
	SerialPrint(nextionQuoteEnd);

	Serial.print("StopSpindle: ");
	Serial.println(configSetup.limit_StopSpindle);
	if(configSetup.limit_StopSpindle)
	{ 
		SerialPrint(pageLimits_c1_1);
	}
	else
	{
		SerialPrint(pageLimits_c1_0);
	}

	SerialPrint(nextionEnd);

	// Update Nextion
	SerialPrint("pageLimits.bt1.bco=23964");
	SerialPrint(nextionEnd);
	SerialPrint("pageLimits.bt1.val=0");
	SerialPrint(nextionEnd);
}

/// <summary>
/// Write current Returns screen eeprom values to Nextion
/// </summary>
/// <comment>
/// </comment>
/// <returns></returns>
void TestEEPROM_Returns()
{
	const char * pageReturns_t0 = "pageReturns.t0.txt=";
	const char * pageReturns_t1 = "pageReturns.t1.txt=";
	const char * pageReturns_t2 = "pageReturns.t2.txt=";
	const char * pageReturns_t3 = "pageReturns.t3.txt=";
	const char* pageReturns_t7 = "pageReturns.t7.txt=";
	const char* pageReturns_t8 = "pageReturns.t8.txt=";
	const char* pageReturns_t9 = "pageReturns.t9.txt=";
	const char* pageReturns_t10 = "pageReturns.t10.txt=";

	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	const char* nextionEnd = "\xFF\xFF\xFF";

	configPageSetup eePromPageSetup;
	EEPROM.get(eePromAddress_Setup, eePromPageSetup);

	SerialPrint(pageReturns_t0);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.maxSpd_Return_Spindle);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageReturns_t1);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.accel_Return_Spindle);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageReturns_t2);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.maxSpd_Return_Axis_Z);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageReturns_t3);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.accel_Return_Axis_Z);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageReturns_t7);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.maxSpd_Return_Axis_X);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageReturns_t8);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.accel_Return_Axis_X);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageReturns_t9);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.maxSpd_Return_Axis_B);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageReturns_t10);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.accel_Return_Axis_B);
	SerialPrint(nextionQuoteEnd);
	// Update Nextion
	SerialPrint("pageReturns.bt1.bco=23964");
	SerialPrint(nextionEnd);
	SerialPrint("pageReturns.bt1.val=0");
	SerialPrint(nextionEnd);

}

/// <summary>
/// Write current page eeprom values to Nextion
/// </summary>
/// <comment>
/// </comment>
/// <returns></returns>
void TestAllTeensyEEPROMValues()
{
	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	//const char* nextionEnd = "\xFF\xFF\xFF";
	switch (pageCallerId)
	{
		case PAGE_MAIN:
		{

			SerialPrint("pageEEPROM.t5.txt=");
			SerialWrite(0x22);
			SerialPrint(configMain.maxSpd_Spindle);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t8.txt=");
			SerialWrite(0x22);
			SerialPrint(configMain.accel_Spindle);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t11.txt=");
			SerialWrite(0x22);
			SerialPrint(configMain.speedPercent_Spindle);
			SerialPrint(nextionQuoteEnd);

			switch (configMain.axisId)
			{
				case ID_AXIS_Z:
				{
					SerialPrint("pageEEPROM.t2.txt=");
					SerialWrite(0x22);
					SerialPrint("Z");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t14.txt=");
					SerialWrite(0x22);
					SerialPrint(configMain.maxSpd_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t17.txt=");
					SerialWrite(0x22);
					SerialPrint(configMain.accel_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t20.txt=");
					SerialWrite(0x22);
					SerialPrint(configMain.speedPercent_Axis_Z);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case ID_AXIS_X: // X Axis
				{
					SerialPrint("pageEEPROM.t2.txt=");
					SerialWrite(0x22);
					SerialPrint("X");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t14.txt=");
					SerialWrite(0x22);
					SerialPrint(configMain.maxSpd_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t17.txt=");
					SerialWrite(0x22);
					SerialPrint(configMain.accel_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t20.txt=");
					SerialWrite(0x22);
					SerialPrint(configMain.speedPercent_Axis_X);
					SerialPrint(nextionQuoteEnd);

					break;
				}
				case ID_AXIS_B: // B Axis
				{
					SerialPrint("pageEEPROM.t2.txt=");
					SerialWrite(0x22);
					SerialPrint("B");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t14.txt=");
					SerialWrite(0x22);
					SerialPrint(configMain.maxSpd_Axis_B);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t17.txt=");
					SerialWrite(0x22);
					SerialPrint(configMain.accel_Axis_B);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t20.txt=");
					SerialWrite(0x22);
					SerialPrint(configMain.speedPercent_Axis_B);
					SerialPrint(nextionQuoteEnd);
					break;
				}
			}
			break;
		}
		case PAGE_ONE:
		{
			switch (configOne.axisId)
			{
				case ID_AXIS_Z:
				{
					SerialPrint("pageEEPROM.t2.txt=");
					SerialWrite(0x22);
					SerialPrint("Z");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t14.txt=");
					SerialWrite(0x22);
					SerialPrint(configOne.maxSpd_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t17.txt=");
					SerialWrite(0x22);
					SerialPrint(configOne.accel_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t20.txt=");
					SerialWrite(0x22);
					SerialPrint(configOne.speedPercent_Axis_Z);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case ID_AXIS_X: // X Axis
				{
					SerialPrint("pageEEPROM.t2.txt=");
					SerialWrite(0x22);
					SerialPrint("X");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t14.txt=");
					SerialWrite(0x22);
					SerialPrint(configOne.maxSpd_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t17.txt=");
					SerialWrite(0x22);
					SerialPrint(configOne.accel_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t20.txt=");
					SerialWrite(0x22);
					SerialPrint(configOne.speedPercent_Axis_X);
					SerialPrint(nextionQuoteEnd);

					break;
				}
				case ID_AXIS_B: // B Axis
				{
					SerialPrint("pageEEPROM.t2.txt=");
					SerialWrite(0x22);
					SerialPrint("B");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t14.txt=");
					SerialWrite(0x22);
					SerialPrint(configOne.maxSpd_Axis_B);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t17.txt=");
					SerialWrite(0x22);
					SerialPrint(configOne.accel_Axis_B);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t20.txt=");
					SerialWrite(0x22);
					SerialPrint(configOne.speedPercent_Axis_B);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case ID_SPINDLE: // Spindle
				{
					SerialPrint("pageEEPROM.t2.txt=");
					SerialWrite(0x22);
					SerialPrint("Spindle");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t5.txt=");
					SerialWrite(0x22);
					SerialPrint(configOne.maxSpd_Spindle);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t8.txt=");
					SerialWrite(0x22);
					SerialPrint(configOne.accel_Spindle);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t11.txt=");
					SerialWrite(0x22);
					SerialPrint(configOne.speedPercent_Spindle);
					SerialPrint(nextionQuoteEnd);
					break;
				}
			}
			break;
		}
		case PAGE_INDEX:
		{
			SerialPrint("pageEEPROM.t5.txt=");
			SerialWrite(0x22);
			SerialPrint(configIndex_Main.maxSpd);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t8.txt=");
			SerialWrite(0x22);
			SerialPrint(configIndex_Main.accel);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t11.txt=");
			SerialWrite(0x22);
			SerialPrint(configIndex_Main.speedPercent);
			SerialPrint(nextionQuoteEnd);

			switch (configIndex_Main.indexId)
			{
				case ID_INDEX_1:
				{
					SerialPrint("pageEEPROM.t2.txt=");
					SerialWrite(0x22);
					SerialPrint("1");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t23.txt=");
					SerialWrite(0x22);
					SerialPrint(configIndex_1.sizeInUnits);
					SerialPrint(nextionQuoteEnd);

					switch (configIndex_1.fileOrFixed)
					{
						case 0:
						case 48:
						{
							SerialPrint("pageEEPROM.t26.txt=");
							SerialWrite(0x22);
							SerialPrint("Fixed");
							SerialPrint(nextionQuoteEnd);
							break;
						}
						case 1:
						case 49:
						{
							SerialPrint("pageEEPROM.t26.txt=");
							SerialWrite(0x22);
							SerialPrint("File");
							SerialPrint(nextionQuoteEnd);
							break;
						}
					}
					break;
				}
				case ID_INDEX_2: 
				{
					SerialPrint("pageEEPROM.t2.txt=");
					SerialWrite(0x22);
					SerialPrint("2");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t23.txt=");
					SerialWrite(0x22);
					SerialPrint(configIndex_2.sizeInUnits);
					SerialPrint(nextionQuoteEnd);

					switch (configIndex_2.fileOrFixed)
					{
						case 0:
						case 48:
						{
							SerialPrint("pageEEPROM.t26.txt=");
							SerialWrite(0x22);
							SerialPrint("Fixed");
							SerialPrint(nextionQuoteEnd);
							break;
						}
						case 1:
						case 49:
						{
							SerialPrint("pageEEPROM.t26.txt=");
							SerialWrite(0x22);
							SerialPrint("File");
							SerialPrint(nextionQuoteEnd);
							break;
						}
					}
					break;
				}
				case ID_INDEX_3: 
				{
					SerialPrint("pageEEPROM.t2.txt=");
					SerialWrite(0x22);
					SerialPrint("3");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t23.txt=");
					SerialWrite(0x22);
					SerialPrint(configIndex_3.sizeInUnits);
					SerialPrint(nextionQuoteEnd);

					switch (configIndex_3.fileOrFixed)
					{
						case 0:
						case 48:
						{
							SerialPrint("pageEEPROM.t26.txt=");
							SerialWrite(0x22);
							SerialPrint("Fixed");
							SerialPrint(nextionQuoteEnd);
							break;
						}
						case 1:
						case 49:
						{
							SerialPrint("pageEEPROM.t26.txt=");
							SerialWrite(0x22);
							SerialPrint("File");
							SerialPrint(nextionQuoteEnd);
							break;
						}
					}
					break;
				}

				case ID_INDEX_4:
				{
					SerialPrint("pageEEPROM.t2.txt=");
					SerialWrite(0x22);
					SerialPrint("4");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t23.txt=");
					SerialWrite(0x22);
					SerialPrint(configIndex_4.sizeInUnits);
					SerialPrint(nextionQuoteEnd);

					switch (configIndex_4.fileOrFixed)
					{
						case 0:
						case 48:
						{
							SerialPrint("pageEEPROM.t26.txt=");
							SerialWrite(0x22);
							SerialPrint("Fixed");
							SerialPrint(nextionQuoteEnd);
							break;
						}
						case 1:
						case 49:
						{
							SerialPrint("pageEEPROM.t26.txt=");
							SerialWrite(0x22);
							SerialPrint("File");
							SerialPrint(nextionQuoteEnd);
							break;
						}
					}
					break;
				}

				case ID_INDEX_5:
				{
					SerialPrint("pageEEPROM.t2.txt=");
					SerialWrite(0x22);
					SerialPrint("5");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t23.txt=");
					SerialWrite(0x22);
					SerialPrint(configIndex_5.sizeInUnits);
					SerialPrint(nextionQuoteEnd);

					switch (configIndex_5.fileOrFixed)
					{
						case 0:
						case 48:
						{
							SerialPrint("pageEEPROM.t26.txt=");
							SerialWrite(0x22);
							SerialPrint("Fixed");
							SerialPrint(nextionQuoteEnd);
							break;
						}
						case 1:
						case 49:
						{
							SerialPrint("pageEEPROM.t26.txt=");
							SerialWrite(0x22);
							SerialPrint("File");
							SerialPrint(nextionQuoteEnd);
							break;
						}
					}
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
				{
					SerialPrint("pageEEPROM.t2.txt=");
					SerialWrite(0x22);
					SerialPrint("Z1");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t14.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.maxSpd_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t17.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.accel_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t20.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.speedPercent_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t23.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.distance_MoveZ1);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case ID_MOVE_AXIS_Z2:
				{
					SerialPrint("pageEEPROM.t2.txt=");
					SerialWrite(0x22);
					SerialPrint("Z2");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t14.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.maxSpd_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t17.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.accel_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t20.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.speedPercent_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t23.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.distance_MoveZ2);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case ID_MOVE_AXIS_X1: // X Axis
				{
					SerialPrint("pageEEPROM.t2.txt=");
					SerialWrite(0x22);
					SerialPrint("X1");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t14.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.maxSpd_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t17.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.accel_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t20.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.speedPercent_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t23.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.distance_MoveX1);
					SerialPrint(nextionQuoteEnd);

					break;
				}
				case ID_MOVE_AXIS_X2: // X Axis
				{
					SerialPrint("pageEEPROM.t2.txt=");
					SerialWrite(0x22);
					SerialPrint("X2");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t14.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.maxSpd_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t17.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.accel_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t20.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.speedPercent_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t23.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.distance_MoveX2);
					SerialPrint(nextionQuoteEnd);

					break;
				}
				case ID_MOVE_AXIS_B1: // B Axis
				{
					SerialPrint("pageEEPROM.t2.txt=");
					SerialWrite(0x22);
					SerialPrint("B1");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t14.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.maxSpd_Axis_B);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t17.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.accel_Axis_B);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t20.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.speedPercent_Axis_B);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t23.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.distance_MoveB1);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case ID_MOVE_AXIS_B2: // B Axis
				{
					SerialPrint("pageEEPROM.t2.txt=");
					SerialWrite(0x22);
					SerialPrint("B2");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t14.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.maxSpd_Axis_B);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t17.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.accel_Axis_B);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t20.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.speedPercent_Axis_B);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t23.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.distance_MoveB2);
					SerialPrint(nextionQuoteEnd);
					break;
				}
			}
			break;
		}

		case PAGE_SYNC:
		{
			SerialPrint("pageEEPROM.t5.txt=");
			SerialWrite(0x22);
			SerialPrint(configSync.maxSpd_Spindle);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t8.txt=");
			SerialWrite(0x22);
			SerialPrint(configSync.accel_Spindle);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t11.txt=");
			SerialWrite(0x22);
			SerialPrint(configSync.speedPercent_Spindle);
			SerialPrint(nextionQuoteEnd);

			Serial.print("configSync.helixType: ");
			Serial.println(configSync.helixType);
			switch (configSync.helixType)
			{
				case 0: //Left
				case 48:
				{
					SerialPrint("pageEEPROM.t23.txt=");
					SerialWrite(0x22);
					SerialPrint("Left");
					SerialPrint(nextionQuoteEnd);

					break;
				}
				case 1: //Right
				case 49:
				{
					SerialPrint("pageEEPROM.t23.txt=");
					SerialWrite(0x22);
					SerialPrint("Right");
					SerialPrint(nextionQuoteEnd);
					break;
				}

			}

			SerialPrint("pageEEPROM.t26.txt=");
			SerialWrite(0x22);
			SerialPrint(configSync.revolutions_Spindle);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t29.txt=");
			SerialWrite(0x22);
			SerialPrint(configSync.distance);
			SerialPrint(nextionQuoteEnd);
			switch (configSync.axisId)
			{
			case ID_AXIS_Z:
			{
				SerialPrint("pageEEPROM.t2.txt=");
				SerialWrite(0x22);
				SerialPrint("Z");
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t14.txt=");
				SerialWrite(0x22);
				SerialPrint(configSync.maxSpd_Axis_Z);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t17.txt=");
				SerialWrite(0x22);
				SerialPrint(configSync.accel_Axis_Z);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t20.txt=");
				SerialWrite(0x22);
				SerialPrint(configSync.speedPercent_Axis_Z);
				SerialPrint(nextionQuoteEnd);
				break;
			}
			case ID_AXIS_X: // X Axis
			{
				SerialPrint("pageEEPROM.t2.txt=");
				SerialWrite(0x22);
				SerialPrint("X");
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t14.txt=");
				SerialWrite(0x22);
				SerialPrint(configSync.maxSpd_Axis_X);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t17.txt=");
				SerialWrite(0x22);
				SerialPrint(configSync.accel_Axis_X);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t20.txt=");
				SerialWrite(0x22);
				SerialPrint(configSync.speedPercent_Axis_X);
				SerialPrint(nextionQuoteEnd);

				break;
			}
			case ID_AXIS_B: // B Axis
			{
				SerialPrint("pageEEPROM.t2.txt=");
				SerialWrite(0x22);
				SerialPrint("B");
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t14.txt=");
				SerialWrite(0x22);
				SerialPrint(configSync.maxSpd_Axis_B);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t17.txt=");
				SerialWrite(0x22);
				SerialPrint(configSync.accel_Axis_B);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t20.txt=");
				SerialWrite(0x22);
				SerialPrint(configSync.speedPercent_Axis_B);
				SerialPrint(nextionQuoteEnd);
				break;
			}
			}

			break;
		}
		case PAGE_REC:
		{
			SerialPrint("pageEEPROM.t5.txt=");
			SerialWrite(0x22);
			SerialPrint(configRec.maxSpd_Spindle);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t8.txt=");
			SerialWrite(0x22);
			SerialPrint(configRec.accel_Spindle);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t11.txt=");
			SerialWrite(0x22);
			SerialPrint(configRec.speedPercent_Spindle);
			SerialPrint(nextionQuoteEnd);

			switch (configRec.radial_axial)
			{
				case RADIAL: //Radial
				{
					SerialPrint("pageEEPROM.t23.txt=");
					SerialWrite(0x22);
					SerialPrint("Radial");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t26.txt=");
					SerialWrite(0x22);
					SerialPrint(configRec.waves_Radial);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t29.txt=");
					SerialWrite(0x22);
					SerialPrint(configRec.degrees_Radial_Spindle);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t32.txt=");
					SerialWrite(0x22);
					SerialPrint(configRec.amplitude_Radial_Axis);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case AXIAL: //Axial
				{
					SerialPrint("pageEEPROM.t23.txt=");
					SerialWrite(0x22);
					SerialPrint("Axial");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t26.txt=");
					SerialWrite(0x22);
					SerialPrint(configRec.waves_Axial);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t29.txt=");
					SerialWrite(0x22);
					SerialPrint(configRec.amplitude_Axial_Spindle);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t32.txt=");
					SerialWrite(0x22);
					SerialPrint(configRec.distance_Axial_Axis);
					SerialPrint(nextionQuoteEnd);
					break;
				}

			}
			switch (configRec.axisId)
			{
				case ID_AXIS_Z:
				{
					SerialPrint("pageEEPROM.t2.txt=");
					SerialWrite(0x22);
					SerialPrint("Z");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t14.txt=");
					SerialWrite(0x22);
					SerialPrint(configRec.maxSpd_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t17.txt=");
					SerialWrite(0x22);
					SerialPrint(configRec.accel_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t20.txt=");
					SerialWrite(0x22);
					SerialPrint(configRec.speedPercent_Axis_Z);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case ID_AXIS_X: // X Axis
				{
					SerialPrint("pageEEPROM.t2.txt=");
					SerialWrite(0x22);
					SerialPrint("X");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t14.txt=");
					SerialWrite(0x22);
					SerialPrint(configRec.maxSpd_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t17.txt=");
					SerialWrite(0x22);
					SerialPrint(configRec.accel_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t20.txt=");
					SerialWrite(0x22);
					SerialPrint(configRec.speedPercent_Axis_X);
					SerialPrint(nextionQuoteEnd);

					break;
				}
				case ID_AXIS_B: // B Axis
				{
					SerialPrint("pageEEPROM.t2.txt=");
					SerialWrite(0x22);
					SerialPrint("B");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t14.txt=");
					SerialWrite(0x22);
					SerialPrint(configRec.maxSpd_Axis_B);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t17.txt=");
					SerialWrite(0x22);
					SerialPrint(configRec.accel_Axis_B);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t20.txt=");
					SerialWrite(0x22);
					SerialPrint(configRec.speedPercent_Axis_B);
					SerialPrint(nextionQuoteEnd);
					break;
				}
			}

			break;
		}
		case PAGE_GRK:
		{


			SerialPrint("pageEEPROM.t5.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.maxSpd_Spindle);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t8.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.accel_Spindle);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t11.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.speedPercent_Spindle);
			SerialPrint(nextionQuoteEnd);
			SerialPrint("pageEEPROM.t23.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.patternCount_Pattern);
			SerialPrint(nextionQuoteEnd);
			SerialPrint("pageEEPROM.t26.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.countPatternPer360_Pattern);
			SerialPrint(nextionQuoteEnd);
			SerialPrint("pageEEPROM.t29.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.segmentLength_Pattern);
			SerialPrint(nextionQuoteEnd);

			switch (configGreekKey.radialOrAxial_Pattern)
			{
				case RADIAL:
				{
					SerialPrint("pageEEPROM.t32.txt=");
					SerialWrite(0x22);
					SerialPrint("Radial");
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case AXIAL:
				{
					SerialPrint("pageEEPROM.t32.txt=");
					SerialWrite(0x22);
					SerialPrint("Axial");
					SerialPrint(nextionQuoteEnd);
					break;
				}
			}

			switch (configGreekKey.patternId)
			{
				case 2: // 4a
				{
					SerialPrint("pageEEPROM.t35.txt=");
					SerialWrite(0x22);
					SerialPrint("4a");
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case 3: // 4b
				{
					SerialPrint("pageEEPROM.t35.txt=");
					SerialWrite(0x22);
					SerialPrint("4b");
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case 4: // 2a
				{
					SerialPrint("pageEEPROM.t35.txt=");
					SerialWrite(0x22);
					SerialPrint("2a");
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case 5: // 2b
				{
					SerialPrint("pageEEPROM.t35.txt=");
					SerialWrite(0x22);
					SerialPrint("2b");
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case 6: // 3a
				{
					SerialPrint("pageEEPROM.t35.txt=");
					SerialWrite(0x22);
					SerialPrint("3a");
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case 7: // 3b
				{
					SerialPrint("pageEEPROM.t35.txt=");
					SerialWrite(0x22);
					SerialPrint("3b");
					SerialPrint(nextionQuoteEnd);
					break;
				}
			}

			switch (configGreekKey.axisId)
			{
				case ID_AXIS_Z:
				{
					SerialPrint("pageEEPROM.t2.txt=");
					SerialWrite(0x22);
					SerialPrint("Z");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t14.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.maxSpd_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t17.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.accel_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t20.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.speedPercent_Axis_Z);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case ID_AXIS_X: // X Axis
				{
					SerialPrint("pageEEPROM.t2.txt=");
					SerialWrite(0x22);
					SerialPrint("X");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t14.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.maxSpd_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t17.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.accel_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t20.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.speedPercent_Axis_X);
					SerialPrint(nextionQuoteEnd);

					break;
				}
				case ID_AXIS_B: // B Axis
				{
					SerialPrint("pageEEPROM.t2.txt=");
					SerialWrite(0x22);
					SerialPrint("B");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t14.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.maxSpd_Axis_B);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t17.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.accel_Axis_B);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t20.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.speedPercent_Axis_B);
					SerialPrint(nextionQuoteEnd);
					break;
				}
			}
			break;

		}
		case PAGE_GRKFILE:
		{
			SerialPrint("pageEEPROM.t5.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.maxSpd_Spindle);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t8.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.accel_Spindle);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t11.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.speedPercent_Spindle);
			SerialPrint(nextionQuoteEnd);
			SerialPrint("pageEEPROM.t23.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.patternCount_File);
			SerialPrint(nextionQuoteEnd);
			SerialPrint("pageEEPROM.t26.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.countPatternPer360_File);
			SerialPrint(nextionQuoteEnd);
			SerialPrint("pageEEPROM.t29.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.segmentLength_File);
			SerialPrint(nextionQuoteEnd);

			switch (configGreekKey.radialOrAxial_File)
			{
				case RADIAL:
				{
					SerialPrint("pageEEPROM.t32.txt=");
					SerialWrite(0x22);
					SerialPrint("Radial");
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case AXIAL:
				{
					SerialPrint("pageEEPROM.t32.txt=");
					SerialWrite(0x22);
					SerialPrint("Axial");
					SerialPrint(nextionQuoteEnd);
					break;
				}
			}

			switch (configGreekKey.segmentOrActual)
			{
				case 2:
				{
					SerialPrint("pageEEPROM.t35.txt=");
					SerialWrite(0x22);
					SerialPrint("Segments");
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case 3:
				{
					SerialPrint("pageEEPROM.t35.txt=");
					SerialWrite(0x22);
					SerialPrint("Actual");
					SerialPrint(nextionQuoteEnd);
					break;
				}
			}

			switch (configGreekKey.axisId)
			{
				case ID_AXIS_Z:
				{
					SerialPrint("pageEEPROM.t2.txt=");
					SerialWrite(0x22);
					SerialPrint("Z");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t14.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.maxSpd_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t17.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.accel_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t20.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.speedPercent_Axis_Z);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case ID_AXIS_X: // X Axis
				{
					SerialPrint("pageEEPROM.t2.txt=");
					SerialWrite(0x22);
					SerialPrint("X");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t14.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.maxSpd_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t17.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.accel_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t20.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.speedPercent_Axis_X);
					SerialPrint(nextionQuoteEnd);

					break;
				}
				case ID_AXIS_B: // B Axis
				{
					SerialPrint("pageEEPROM.t2.txt=");
					SerialWrite(0x22);
					SerialPrint("B");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t14.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.maxSpd_Axis_B);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t17.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.accel_Axis_B);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t20.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.speedPercent_Axis_B);
					SerialPrint(nextionQuoteEnd);
					break;
				}
			}
			break;
		}
		case PAGE_GEO:
		{
			SerialPrint("pageEEPROM.t5.txt=");
			SerialWrite(0x22);
			SerialPrint(configRose.maxSpd_Spindle);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t8.txt=");
			SerialWrite(0x22);
			SerialPrint(configRose.accel_Spindle);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t11.txt=");
			SerialWrite(0x22);
			SerialPrint(configRose.speedPercent_Spindle);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t23.txt=");
			SerialWrite(0x22);
			SerialPrint(configRose.n);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t26.txt=");
			SerialWrite(0x22);
			SerialPrint(configRose.d);
			SerialPrint(nextionQuoteEnd);

			switch (configRose.axisId)
			{
				case ID_AXIS_Z:
				{

					SerialPrint("pageEEPROM.t2.txt=");
					SerialWrite(0x22);
					SerialPrint("Z");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t14.txt=");
					SerialWrite(0x22);
					SerialPrint(configRose.maxSpd_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t17.txt=");
					SerialWrite(0x22);
					SerialPrint(configRose.accel_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t20.txt=");
					SerialWrite(0x22);
					SerialPrint(configRose.speedPercent_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t29.txt=");
					SerialWrite(0x22);
					SerialPrint(configRose.amplitude_Radial_Z);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case ID_AXIS_X: // X Axis
				{
					SerialPrint("pageEEPROM.t2.txt=");
					SerialWrite(0x22);
					SerialPrint("X");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t14.txt=");
					SerialWrite(0x22);
					SerialPrint(configRose.maxSpd_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t17.txt=");
					SerialWrite(0x22);
					SerialPrint(configRose.accel_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t20.txt=");
					SerialWrite(0x22);
					SerialPrint(configRose.speedPercent_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t29.txt=");
					SerialWrite(0x22);
					SerialPrint(configRose.amplitude_Radial_X);
					SerialPrint(nextionQuoteEnd);

					break;
				}
				case ID_AXIS_B: // B Axis
				{
					SerialPrint("pageEEPROM.t2.txt=");
					SerialWrite(0x22);
					SerialPrint("B");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t14.txt=");
					SerialWrite(0x22);
					SerialPrint(configRose.maxSpd_Axis_B);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t17.txt=");
					SerialWrite(0x22);
					SerialPrint(configRose.accel_Axis_B);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t20.txt=");
					SerialWrite(0x22);
					SerialPrint(configRose.speedPercent_Axis_B);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t29.txt=");
					SerialWrite(0x22);
					SerialPrint(configRose.amplitude_Radial_B);
					SerialPrint(nextionQuoteEnd);
					break;
				}
			}

			break;
		}
	}
}

/// <summary>
/// LoadSettings
/// </summary>
/// <comment>
/// Load ini file settings
/// </comment>
/// <returns></returns>
void LoadSettings()
{
	const char* nextionEnd = "\xFF\xFF\xFF";
	// Verify ini file exists.
	const char* filename = filename_4Axes;
	if (iniFileType==INI_RESET)
	{
		filename = filename_Reset;
	}	
	IniFile ini(filename);

	if (!ini.open())
	{
		Serial.println("No Card.................");
		// Update Nextion
		SerialPrint("bt0.bco=9563");
		Serial.println("bt0.bco=9563");
		SerialPrint(nextionEnd);
		SerialPrint("bt0.val=0");
		Serial.println("bt0.val=0");
		SerialPrint(nextionEnd);
		Serial.println("vis t1,1");
		SerialPrint("vis t1,1");
		SerialPrint(nextionEnd);
		return;
	}
	else
	{
		Serial.println("Ini file found......");
		ini.close();
	}

	LoadSettings_PagePreferences();
	LoadSettings_PageReturns();
	LoadSettings_PageLimits();
	LoadSettings_PageMain();
	LoadSettings_PageOne();
	LoadSettings_PageIndex();
	LoadSettings_PageMove();
	LoadSettings_PageSync();
	LoadSettings_PageRec();
	LoadSettings_PageGrk();
	LoadSettings_PageGeo();

	// Update Nextion
	Serial.println("Show restart");
	SerialPrint("bt0.bco=9563");
	SerialPrint(nextionEnd);
	MilliDelay(15);
	SerialPrint("bt0.val=0");
	SerialPrint(nextionEnd);
	SerialPrint("vis g0,1");
	SerialPrint(nextionEnd);
}

/// <summary>
/// LoadSettings_PageIndex
/// </summary>
/// <comment>
/// Load ini file settings for Index page
/// </comment>
/// <returns></returns>
void LoadSettings_PageIndex()
{
	// Index Main
	const char* iniKey = "Ind";
	const char* iniValue = "IndexId";
	int eePromAddress_Nextion = 368;
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
	eePromAddress_Nextion = 192;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_1.degreeOrDivision = (int)returnVal;

	iniValue = "FixedOrFile_1";
	eePromAddress_Nextion = 572;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_1.fileOrFixed = (int)returnVal;

	iniValue = "Size_1";
	eePromAddress_Nextion = 352;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configIndex_1.sizeInSteps = returnVal;

	// Index 2
	iniValue = "DivisionsOrDegrees_1";
	eePromAddress_Nextion = 160;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_2.degreeOrDivision = (int)returnVal;

	iniValue = "FixedOrFile_2";
	eePromAddress_Nextion = 708;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_2.fileOrFixed = (int)returnVal;

	iniValue = "Size_2";
	eePromAddress_Nextion = 360;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configIndex_2.sizeInSteps = returnVal;

	// Index 3
	iniValue = "DivisionsOrDegrees_3";
	eePromAddress_Nextion = 152;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_3.degreeOrDivision = (int)returnVal;

	iniValue = "FixedOrFile_3";
	eePromAddress_Nextion = 592;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_3.fileOrFixed = (int)returnVal;

	iniValue = "Size_3";
	eePromAddress_Nextion = 580;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configIndex_3.sizeInSteps = returnVal;

	// Index 4
	iniValue = "DivisionsOrDegrees_4";
	eePromAddress_Nextion = 428;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_4.degreeOrDivision = (int)returnVal;

	iniValue = "FixedOrFile_4";
	eePromAddress_Nextion = 588;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_4.fileOrFixed = (int)returnVal;

	iniValue = "Size_4";
	eePromAddress_Nextion = 672;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configIndex_4.sizeInSteps = returnVal;

	// Index 5
	iniValue = "DivisionsOrDegrees_5";
	eePromAddress_Nextion = 440;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_5.degreeOrDivision = (int)returnVal;

	iniValue = "FixedOrFile_5";
	eePromAddress_Nextion = 536;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_5.fileOrFixed = (int)returnVal;

	iniValue = "Size_5";
	eePromAddress_Nextion = 680;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configIndex_5.sizeInSteps = returnVal;
	EEPROM.put(eePromAddress_Ind_Main, configIndex_Main);
	EEPROM.put(eePromAddress_Ind_1, configIndex_1);
	EEPROM.put(eePromAddress_Ind_2, configIndex_2);
	EEPROM.put(eePromAddress_Ind_3, configIndex_3);
}

/// <summary>
/// LoadSettings_PageGeo
/// </summary>
/// <comment>
/// Load ini file settings for Geo page
/// </comment>
/// <returns></returns>
void LoadSettings_PageGeo()
{

	const char* iniKey = "Geo";
	const char* iniValue = "AxisId";
	int eePromAddress_Nextion = 164;
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.axisId = (int)returnVal;

	iniValue = "Rose_n";
	eePromAddress_Nextion = 208;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.n = (int)returnVal;

	iniValue = "RadialOrAxial";
	eePromAddress_Nextion = 240;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.radial_Axial = (int)returnVal;

	iniValue = "Rose_d";
	eePromAddress_Nextion = 452;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.d = (int)returnVal;

	iniValue = "MaxSpeed_Spindle";
	eePromAddress_Nextion = 828;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.maxSpd_Spindle = (int)returnVal;

	iniValue = "Accel_Spindle";
	eePromAddress_Nextion = 832;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.accel_Spindle = (int)returnVal;

	iniValue = "SpeedPercentage_Spindle";
	eePromAddress_Nextion = 100;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.speedPercent_Spindle = (int)returnVal;

	// Z axis
	iniValue = "MaxSpeed_Z";
	eePromAddress_Nextion = 880;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.maxSpd_Axis_Z = (int)returnVal;

	iniValue = "Accel_Z";
	eePromAddress_Nextion = 948;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.accel_Axis_Z = (int)returnVal;

	iniValue = "SpeedPercentage_Z";
	eePromAddress_Nextion = 52;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.speedPercent_Axis_Z = (int)returnVal;

	iniValue = "RadialAmplitude_Z";
	eePromAddress_Nextion = 276;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configRose.amplitude_Radial_Z = (int)returnVal;

	iniValue = "AxialAmplitude_Z";
	eePromAddress_Nextion = 292;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configRose.amplitude_Axial_Z = (int)returnVal;

	// X axis
	iniValue = "MaxSpeed_X";
	eePromAddress_Nextion = 876;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.maxSpd_Axis_X = (int)returnVal;

	iniValue = "Accel_X";
	eePromAddress_Nextion = 868;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.accel_Axis_X = (int)returnVal;

	iniValue = "SpeedPercentage_X";
	eePromAddress_Nextion = 44;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.speedPercent_Axis_X = (int)returnVal;

	iniValue = "RadialAmplitude_X";
	eePromAddress_Nextion = 284;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configRose.amplitude_Radial_X = (int)returnVal;

	iniValue = "AxialAmplitude_X";
	eePromAddress_Nextion = 224;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configRose.amplitude_Axial_X = (int)returnVal;

	// B axis
	iniValue = "MaxSpeed_B";
	eePromAddress_Nextion = 824;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.maxSpd_Axis_B = (int)returnVal;

	iniValue = "Accel_B";
	eePromAddress_Nextion = 820;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.accel_Axis_B = (int)returnVal;

	iniValue = "SpeedPercentage_B";
	eePromAddress_Nextion = 32;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.speedPercent_Axis_B = (int)returnVal;

	iniValue = "RadialAmplitude_B";
	eePromAddress_Nextion = 520;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configRose.amplitude_Radial_B = (int)returnVal;

	iniValue = "AxialAmplitude_B";
	eePromAddress_Nextion = 528;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configRose.amplitude_Axial_B = (int)returnVal;

	EEPROM.put(eePromAddress_Rose, configRose);
}

/// <summary>
/// LoadSettings_PageMove
/// </summary>
/// <comment>
/// Load ini file settings for Move page
/// </comment>
/// <returns></returns>
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
	eePromAddress_Nextion = 40;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMove.speedPercent_Axis_Z = (int)returnVal;

	iniValue = "Distance_Z1";
	eePromAddress_Nextion = 168;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configMove.distance_MoveZ1 = returnVal;

	iniValue = "Distance_Z2";
	eePromAddress_Nextion = 396;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configMove.distance_MoveZ2 = returnVal;

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

	iniValue = "Distance_X1";
	eePromAddress_Nextion = 340;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configMove.distance_MoveX1 = returnVal;

	iniValue = "Distance_X2";
	eePromAddress_Nextion = 316;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configMove.distance_MoveX2 = returnVal;

	// B axis
	iniValue = "MaxSpeed_B";
	eePromAddress_Nextion = 888;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMove.maxSpd_Axis_B = (int)returnVal;

	iniValue = "Accel_B";
	eePromAddress_Nextion = 892;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMove.accel_Axis_B = (int)returnVal;

	iniValue = "SpeedPercentage_B";
	eePromAddress_Nextion = 92;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMove.speedPercent_Axis_B = (int)returnVal;

	iniValue = "Distance_B1";
	eePromAddress_Nextion = 332;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configMove.distance_MoveB1 = returnVal;
	EEPROM.put(eePromAddress_Mov, configMove);

	iniValue = "Distance_B2";
	eePromAddress_Nextion = 500;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configMove.distance_MoveB2 = returnVal;
	EEPROM.put(eePromAddress_Mov, configMove);
}

/// <summary>
/// LoadSettings_PageGrk
/// </summary>
/// <comment>
/// Load ini file settings for Grk page
/// </comment>
/// <returns></returns>
void LoadSettings_PageGrk()
{
	// Greek Key Main
	const char* iniKey = "Grk";
	const char* iniValue = "AxisId";
	int eePromAddress_Nextion = 752;
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.axisId = (int)returnVal;

	iniValue = "FileOrPattern";
	eePromAddress_Nextion = 200;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.fileOrPattern = (int)returnVal;


	// Greek Key Spindle (Shared by Pattern and File)
	iniValue = "MaxSpeed_Spindle";
	eePromAddress_Nextion = 804;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.maxSpd_Spindle = (int)returnVal;
	
	iniValue = "Accel_Spindle";
	eePromAddress_Nextion = 808;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.accel_Spindle = (int)returnVal;

	iniValue = "SpeedPercentage_Spindle";
	eePromAddress_Nextion = 60;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.speedPercent_Spindle = (int)returnVal;

	// Z axis
	iniValue = "MaxSpeed_Z";
	eePromAddress_Nextion = 864;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.maxSpd_Axis_Z = (int)returnVal;

	iniValue = "Accel_Z";
	eePromAddress_Nextion = 860;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.accel_Axis_Z = (int)returnVal;

	iniValue = "SpeedPercentage_Z";
	eePromAddress_Nextion = 64;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.speedPercent_Axis_Z = (int)returnVal;

	// X axis
	iniValue = "MaxSpeed_X";
	eePromAddress_Nextion = 852;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.maxSpd_Axis_X = (int)returnVal;

	iniValue = "Accel_X";
	eePromAddress_Nextion = 856;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.accel_Axis_X = (int)returnVal;

	iniValue = "SpeedPercentage_X";
	eePromAddress_Nextion = 68;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.speedPercent_Axis_X = (int)returnVal;

	// B axis
	iniValue = "MaxSpeed_B";
	eePromAddress_Nextion = 816;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.maxSpd_Axis_B = (int)returnVal;

	iniValue = "Accel_B";
	eePromAddress_Nextion = 812;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.accel_Axis_B = (int)returnVal;

	iniValue = "SpeedPercentage_B";
	eePromAddress_Nextion = 28;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.speedPercent_Axis_B = (int)returnVal;

	iniValue = "RadialOrAxial_Pattern";
	eePromAddress_Nextion = 432;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.radialOrAxial_Pattern = (int)returnVal;

	iniValue = "PatternType";
	eePromAddress_Nextion = 696;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.patternId = (int)returnVal;

	iniValue = "Pattern_PatternsPer360";
	eePromAddress_Nextion = 372;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configGreekKey.countPatternPer360_Pattern = (int)returnVal;

	iniValue = "Pattern_PatternCount";
	eePromAddress_Nextion = 496;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.patternCount_Pattern = (int)returnVal;

	iniValue = "Pattern_SegmentLength";
	eePromAddress_Nextion = 620;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configGreekKey.segmentLength_Pattern = (int)returnVal;

	iniValue = "RadialOrAxial_File";
	eePromAddress_Nextion = 436;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.radialOrAxial_File = (int)returnVal;

	iniValue = "File_PatternsPer360";
	eePromAddress_Nextion = 636;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configGreekKey.countPatternPer360_File = (int)returnVal;

	iniValue = "File_PatternCount";
	eePromAddress_Nextion = 756;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.patternCount_File = (int)returnVal;

	iniValue = "File_SegmentLength";
	eePromAddress_Nextion = 300;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configGreekKey.segmentLength_File = (int)returnVal;

	EEPROM.put(eePromAddress_Grk, configGreekKey);

}

/// <summary>
/// LoadSettings_PageRec
/// </summary>
/// <comment>
/// Load ini file settings for Rec page
/// </comment>
/// <returns></returns>
void LoadSettings_PageRec()
{
	const char* iniKey = "Rec";
	const char* iniValue = "AxisId";
	int eePromAddress_Nextion = 464;
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.axisId = (int)returnVal;

	iniValue = "RadialOrAxial"; 
	eePromAddress_Nextion = 560;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.radial_axial = (int)returnVal;

	iniValue = "MaxSpeed_Spindle";
	eePromAddress_Nextion = 896;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.maxSpd_Spindle = (int)returnVal;

	iniValue = "Accel_Spindle";
	eePromAddress_Nextion = 936;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.accel_Spindle = (int)returnVal;

	iniValue = "SpeedPercentage_Spindle";
	eePromAddress_Nextion = 4;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.speedPercent_Spindle = (int)returnVal;

	// Z axis
	iniValue = "MaxSpeed_Z";
	eePromAddress_Nextion = 964;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.maxSpd_Axis_Z = (int)returnVal;

	iniValue = "Accel_Z";
	eePromAddress_Nextion = 932;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.accel_Axis_Z = (int)returnVal;

	iniValue = "SpeedPercentage_Z";
	eePromAddress_Nextion = 88;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.speedPercent_Axis_Z = (int)returnVal;

	// X Axis
	iniValue = "MaxSpeed_X";
	eePromAddress_Nextion = 960;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.maxSpd_Axis_X = (int)returnVal;

	iniValue = "Accel_X";
	eePromAddress_Nextion = 980;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.accel_Axis_X = (int)returnVal;

	iniValue = "SpeedPercentage_X";
	eePromAddress_Nextion = 96;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.speedPercent_Axis_X = (int)returnVal;

	// B Axis
	iniValue = "MaxSpeed_B";
	eePromAddress_Nextion = 796;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.maxSpd_Axis_B = (int)returnVal;

	iniValue = "Accel_B";
	eePromAddress_Nextion = 800;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.accel_Axis_B = (int)returnVal;

	iniValue = "SpeedPercentage_B";
	eePromAddress_Nextion = 48;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.speedPercent_Axis_B = (int)returnVal;
	//////////
	iniValue = "Radial_Waves";
	eePromAddress_Nextion = 556;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.waves_Radial = (int)returnVal;

	iniValue = "Radial_Spindle_Amplitude"; // Radial Degrees
	eePromAddress_Nextion = 308;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configRec.amplitude_Radial_Axis = returnVal;

	iniValue = "Radial_Axis_Distance";
	eePromAddress_Nextion = 232;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configRec.amplitude_Radial_Axis = returnVal;

	iniValue = "Axial_Waves";
	eePromAddress_Nextion = 740;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.waves_Axial = (int)returnVal;

	iniValue = "Axial_Spindle_Degrees";
	eePromAddress_Nextion = 744;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configRec.amplitude_Axial_Spindle = returnVal;

	iniValue = "Axial_Axis_Amplitude";
	eePromAddress_Nextion = 324;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configRec.distance_Axial_Axis = returnVal;



	EEPROM.put(eePromAddress_Rec, configRec);
}

/// <summary>
/// LoadSettings_PageOne
/// </summary>
/// <comment>
/// Load ini file settings for One page
/// </comment>
/// <returns></returns>
void LoadSettings_PageOne()
{
	const char* iniKey = "One";
	const char* iniValue = "AxisId";
	int eePromAddress_Nextion = 472;
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
	eePromAddress_Nextion = 844;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configOne.maxSpd_Axis_Z = (int)returnVal;

	iniValue = "Accel_Z";
	eePromAddress_Nextion = 848;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configOne.accel_Axis_Z = (int)returnVal;

	iniValue = "SpeedPercentage_Z";
	eePromAddress_Nextion = 56;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configOne.speedPercent_Axis_Z = (int)returnVal;

	iniValue = "MaxSpeed_X";
	eePromAddress_Nextion = 984;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configOne.maxSpd_Axis_X = (int)returnVal;

	iniValue = "Accel_X";
	eePromAddress_Nextion = 836;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configOne.accel_Axis_X = (int)returnVal;

	iniValue = "SpeedPercentage_X";
	eePromAddress_Nextion = 84;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configOne.speedPercent_Axis_X = (int)returnVal;
	
	iniValue = "MaxSpeed_B";
	eePromAddress_Nextion = 840;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configOne.maxSpd_Axis_B = (int)returnVal;

	iniValue = "Accel_B";
	eePromAddress_Nextion = 792;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configOne.accel_Axis_B = (int)returnVal;

	iniValue = "SpeedPercentage_B";
	eePromAddress_Nextion = 108;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configOne.speedPercent_Axis_B = (int)returnVal;

	EEPROM.put(eePromAddress_One, configOne);
}

/// <summary>
/// LoadSettings_PageMain
/// </summary>
/// <comment>
/// Load ini file settings for Main page
/// </comment>
/// <returns></returns>
void LoadSettings_PageMain()
{
	const char* iniKey = "Main";
	const char* iniValue = "AxisId";
	int eePromAddress_Nextion = 568;
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMain.axisId = (int)returnVal;
	
	iniValue = "MaxSpeed_Spindle";
	eePromAddress_Nextion = 872;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMain.maxSpd_Spindle = (int)returnVal;

	iniValue = "Accel_Spindle";
	eePromAddress_Nextion = 912;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMain.accel_Spindle = (int)returnVal;

	iniValue = "SpeedPercentage_Spindle";
	eePromAddress_Nextion = 0;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMain.speedPercent_Spindle = (int)returnVal;

	iniValue = "MaxSpeed_Z";
	eePromAddress_Nextion = 952;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMain.maxSpd_Axis_Z = (int)returnVal;

	iniValue = "Accel_Z";
	eePromAddress_Nextion = 972;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMain.accel_Axis_Z = (int)returnVal;

	iniValue = "SpeedPercentage_Z";
	eePromAddress_Nextion = 16;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMain.speedPercent_Axis_Z = (int)returnVal;

	iniValue = "MaxSpeed_X";
	eePromAddress_Nextion = 992;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMain.maxSpd_Axis_X = (int)returnVal;

	iniValue = "Accel_X";
	eePromAddress_Nextion = 1004;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMain.accel_Axis_X = (int)returnVal;

	iniValue = "SpeedPercentage_X";
	eePromAddress_Nextion = 8;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMain.speedPercent_Axis_X = (int)returnVal;

	iniValue = "MaxSpeed_B";
	eePromAddress_Nextion = 1016;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMain.maxSpd_Axis_B = (int)returnVal;

	iniValue = "Accel_B";
	eePromAddress_Nextion = 1020;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMain.accel_Axis_B = (int)returnVal;

	iniValue = "SpeedPercentage_B";
	eePromAddress_Nextion = 12;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMain.speedPercent_Axis_B = (int)returnVal;

	EEPROM.put(eePromAddress_Main, configMain);
}

/// <summary>
/// LoadSettings_PageSync
/// </summary>
/// <comment>
/// Load ini file settings for Sync page
/// </comment>
/// <returns></returns>
void LoadSettings_PageSync()
{
	const char* iniKey = "Sync";
	const char* iniValue = "AxisId";
	int eePromAddress_Nextion = 608;
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.axisId = (int)returnVal;

	iniValue = "HelixType"; //Helix Type
	eePromAddress_Nextion = 420;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.helixType = (int)returnVal;

	iniValue = "Revolutions";
	eePromAddress_Nextion = 612;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configSync.revolutions_Spindle = returnVal;

	iniValue = "Distance"; 
	eePromAddress_Nextion = 628;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configSync.distance = returnVal;

	iniValue = "MaxSpeed_Spindle";
	eePromAddress_Nextion = 884;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.maxSpd_Spindle = (int)returnVal;

	iniValue = "Accel_Spindle";
	eePromAddress_Nextion = 924;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.accel_Spindle = (int)returnVal;

	iniValue = "SpeedPercentage_Spindle";
	eePromAddress_Nextion = 76;
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
	eePromAddress_Nextion = 72;
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

	iniValue = "MaxSpeed_B";
	eePromAddress_Nextion = 928;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.maxSpd_Axis_B = (int)returnVal;

	iniValue = "Accel_B";
	eePromAddress_Nextion = 908;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.accel_Axis_B = (int)returnVal;

	iniValue = "SpeedPercentage_B";
	eePromAddress_Nextion = 80;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.speedPercent_Axis_B = (int)returnVal;

	EEPROM.put(eePromAddress_Sync, configSync);
}

/// <summary>
/// LoadSettings_PagePreferences
/// </summary>
/// <comment>
/// Load ini file settings for Preferences pages
/// </comment>
/// <returns></returns>
void LoadSettings_PagePreferences()
{
	const char* iniKey = "Setup";
	// Show Vendor Splashscreen
	const char* iniValue = "BoardType";
	int eePromAddress_Nextion = 180;
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);

	// pageSpindle
	iniValue = "Microsteps_Spindle";
	eePromAddress_Nextion = 212;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.microsteps_Spindle = (int)returnVal;

	iniValue = "StepsPer360_Spindle";
	eePromAddress_Nextion = 244;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.steps360_Spindle = (int)returnVal;

	iniValue = "GearRatio_Spindle";
	eePromAddress_Nextion = 268;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configSetup.gearRatio_Spindle = returnVal;

	iniValue = "Polarity_Spindle";
	eePromAddress_Nextion = 248;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	returnVal >= 1 ? (configSetup.polarity_Spindle = true) : (configSetup.polarity_Spindle = false);

	iniValue = "PulseWidth_Spindle";
	eePromAddress_Nextion = 216;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configSetup.pulseWidth_Spindle = returnVal;

	iniValue = "SpeedUpdatePeriod_Spindle";
	eePromAddress_Nextion = 220;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configSetup.speedUpdatePeriod_Spindle = returnVal;

	// Page Z Axis
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

	// Page X Axis
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

	iniValue = "DistancePer360_B";
	eePromAddress_Nextion = 408;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configSetup.distancePerRev_AxisB = returnVal;

	iniValue = "RadialOrLineal";
	eePromAddress_Nextion = 148;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.radialOrLinear_Axis_B = returnVal;

	iniValue = "BRadius";
	eePromAddress_Nextion = 688;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configSetup.radiusB = returnVal;
	EEPROM.put(eePromAddress_Setup, configSetup);
}

/// <summary>
/// LoadSettings_PageLimits
/// </summary>
/// <comment>
/// Load ini file settings for Limits page
/// </comment>
/// <returns></returns>
void LoadSettings_PageLimits()
{

	const char* iniKey = "Limits";

	const char* iniValue = "Min_B";
	int eePromAddress_Nextion = 476;
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.limit_Min_B = (int)returnVal;

	iniValue = "Max_B";
	eePromAddress_Nextion = 392;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.limit_Max_B = (int)returnVal;

	iniValue = "Home_B";
	eePromAddress_Nextion = 176;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.home_B = (int)returnVal;

	iniValue = "Min_Z";
	eePromAddress_Nextion = 424;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.limit_Min_Z = (int)returnVal;

	iniValue = "Max_Z";
	eePromAddress_Nextion = 704;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.limit_Max_Z = (int)returnVal;

	iniValue = "Home_Z";
	eePromAddress_Nextion = 140;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.home_Z = (int)returnVal;

	iniValue = "Min_X";
	eePromAddress_Nextion = 596;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.limit_Min_X = (int)returnVal;

	iniValue = "Max_X";
	eePromAddress_Nextion = 576;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.limit_Max_X = (int)returnVal;

	iniValue = "Home_X";
	eePromAddress_Nextion = 176;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.home_X = (int)returnVal;

	iniValue = "StopSpindle";
	eePromAddress_Nextion = 184;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.home_X = (int)returnVal;

	EEPROM.put(eePromAddress_Setup, configSetup);
}

/// <summary>
/// LoadSettings_PageReturns
/// </summary>
/// <comment>
/// Load ini file settings for Returns page
/// </comment>
/// <returns></returns>
void LoadSettings_PageReturns()
{
	// Return settings
	const char* iniKey = "Returns";
	const char* iniValue = "MaxSpeed_Spindle";
	int eePromAddress_Nextion = 760;
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.maxSpd_Return_Spindle = (int)returnVal;

	iniValue = "Accel_Spindle";
	eePromAddress_Nextion = 764;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.accel_Return_Spindle = (int)returnVal;

	iniValue = "MaxSpeed_Axis_Z";
	eePromAddress_Nextion = 768;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.maxSpd_Return_Axis_Z = (int)returnVal;

	iniValue = "Accel_Axis_Z";
	eePromAddress_Nextion = 772;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.accel_Return_Axis_Z = (int)returnVal;

	iniValue = "MaxSpeed_Axis_X";
	eePromAddress_Nextion = 784;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.maxSpd_Return_Axis_X = (int)returnVal;

	iniValue = "Accel_Axis_X";
	eePromAddress_Nextion = 788;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.accel_Return_Axis_X = (int)returnVal;

	iniValue = "MaxSpeed_Axis_B";
	eePromAddress_Nextion = 776;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.maxSpd_Return_Axis_B = (int)returnVal;

	iniValue = "Accel_Axis_B";
	eePromAddress_Nextion = 780;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.accel_Return_Axis_B = (int)returnVal;
	EEPROM.put(eePromAddress_Setup, configSetup);
}

/// <summary>
/// GetIniValue
/// </summary>
/// <comment>
/// Get value from ini file
/// </comment>
/// <param name="iniKey">Key name</param>
/// <param name="iniValue">Value name</param>
/// <param name="eePromNextion">Nextion EEProm address</param>
/// <param name="asString">True:Get value as string. False: Value as number</param>
/// <returns>float</returns>
float GetIniValue(const char* iniKey, const char* iniValue, int eePromNextion, bool asString)
{
	const char* nextionEnd = "\xFF\xFF\xFF";
	const size_t bufferLen = 80;
	char buffer[bufferLen];
	const char* filename = filename_4Axes;
	if (iniFileType == INI_RESET)
	{
		filename = filename_Reset;
	}
	IniFile ini(filename);


	if (!ini.open()) 
	{
#ifdef DEBUG
		Serial.print("Ini file ");
		Serial.print(filename);
		Serial.println(" does not exist");

		SerialPrint("vis t1,1");
		SerialPrint(nextionEnd);
#endif // DEBUG
		return 0;
	}
#ifdef DEBUG
	//Serial.println("Ini file opened");
#endif // DEBUG

	// Check the file is valid. This can be used to warn if any lines are longer than the buffer.
	if (!ini.validate(buffer, bufferLen)) {
#ifdef DEBUG
		Serial.print("ini file ");
		Serial.print(ini.getFilename());
		Serial.print(" not valid: ");
#endif // DEBUG

		return 0;
	}

	// Process values
	if (ini.getValue(iniKey, iniValue, buffer, bufferLen))
	{
#ifdef DEBUG
		Serial.println(buffer);
#endif // DEBUG
		SerialPrint("wepo ");

		if (asString)
		{
			SerialWrite(0x22);
			SerialPrint(buffer);
			SerialWrite(0x22);
		}
		else
		{
			SerialPrint(buffer);
		}
		SerialPrint(",");
		SerialPrint(eePromNextion);

		SerialPrint(nextionEnd);

#ifdef DEBUG
		Serial.println(iniKey);
		Serial.println(iniValue);
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
		Serial.println("----------------");
		Serial.print("Bad:");
		Serial.print(iniKey);
		Serial.print(" - ");
		Serial.println(iniValue);
	}
	iniKey = "Main";
	MilliDelay(100);
	return atof(buffer);
}
