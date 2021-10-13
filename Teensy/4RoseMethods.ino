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
	if (SD.begin(254)) // Builtin microSD card: Use 254 instead of BUILTIN_SDCARD for compatibility with Teensy 3.2
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
void SetEnable(int axisId, bool enabled, bool checkKeepEnabled)
{
	Serial.print("Enabled?: ");
	Serial.println(enabled);
	bool ignoreDisable = false;
	if (checkKeepEnabled)
	{
		if (configSetup.keepSteppersEnabled == 1)
		{
			ignoreDisable = true;
		}
	}

	MilliDelay(10);
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
				if(!ignoreDisable)
				{ 
					digitalWrite(PIN_SPINDLE_ENABLE, configSetup.polarity_Spindle ? (HIGH) : (LOW)); // Disable 
				}
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
				if (!ignoreDisable)
				{
					digitalWrite(PIN_AXIS_Z_ENABLE, configSetup.polarity_Axis_Z ? (HIGH) : (LOW)); // Disable 
				}
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
				if (!ignoreDisable)
				{
					digitalWrite(PIN_AXIS_X_ENABLE, configSetup.polarity_Axis_X ? (HIGH) : (LOW)); // Disable 
				}
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
				if (!ignoreDisable)
				{
					digitalWrite(PIN_AXIS_B_ENABLE, configSetup.polarity_Axis_B ? (HIGH) : (LOW)); // Disable 
				}
			}
			break;
		}
	}

	if (enabled)// Workaround for DM542T external driver
	{
		//Serial.println("Enable Delay True");
		MilliDelay(enableTimeout);
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
#ifdef DEBUG
		Serial.println("");
		Serial.print("InputChar:---------------------------------------: ");
		Serial.print(inputChar);
		Serial.print("  ");
		Serial.println(int(inputChar));
#endif //DEBUG
		// Use 255 as end chars
		//if (inputChar == 255)
		//{
		//	// Read and discard anything more on serial.
		//	while (port.available() > 0)
		//	{
		//		port.read();
		//	}
		//	break;
		//}
		//else
		//{
		//	in[i] = inputChar;
		//}
		switch (inputChar)
		{
			case 255:
			{
				// Read and discard anything more on serial.
				while (port.available() > 0)
				{
					port.read();
				}
				break;
			}
			case 4:
			{
#ifdef DEBUG
				Serial.println("Discard bogus 4----------");
#endif //Debug
				break;
			}
			default:
			{
				in[i] = inputChar;
#ifdef DEBUG
				Serial.println(in);
#endif //Debug
			}
		}

	}

	double retVal = atof(in);

#ifdef DEBUG
	Serial.print("In: ");
	Serial.println(in);
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
/// <returns>Integer value</returns>
int GetSerialInteger()
{
	char inputHex;

	// Value will be 0 to 100
	inputHex = SerialRead(serialId);

	Serial.print("inputHex:   ");
	Serial.println(inputHex);
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
			if(configSetup.xAltX==0)
			{ 
				retVal = (steps * configSetup.distancePerRev_AxisX) / (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X);
			}
			else
			{
				retVal = (steps * configSetup.distancePerRev_AxisXAlt) / (configSetup.steps360_Axis_XAlt * configSetup.microsteps_Axis_XAlt);
			}
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
#ifdef DEBUG
	Serial.print("DistanceToSteps_LinearB: ");
	Serial.println(retVal);
#endif // Debug	
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
			if (configSetup.xAltX == 0)
			{
				retVal = (distance / configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X);
			}
			else
			{
				retVal = (distance / configSetup.distancePerRev_AxisXAlt) * (configSetup.steps360_Axis_XAlt * configSetup.microsteps_Axis_XAlt);
			}
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
/// AngularLengthAndMaxSpeed
/// </summary>
/// <comment>
/// Calculate max speeds and length for Greek Key angular moves.
/// </comment>
/// <param name="spindleMaxSpeed">Spindle Max Speed</param>
/// <param name="axisMaxSpeed">Axis Max Speed</param>
/// <returns>void</returns>
void AngularLengthAndMaxSpeed(long spindleMaxSpeed, long axisMaxSpeed)
{

	// Pathagorean Theorem
	if (hv_SpindleLegLength < 0)
	{
		hv_SpindleLegLength = hv_SpindleLegLength * (-1);
	}
	if (hv_AxisLegLength < 0)
	{
		hv_AxisLegLength = hv_AxisLegLength * (-1);
	}
	float hypotenuse = sqrt((hv_SpindleLegLength * hv_SpindleLegLength) + (hv_AxisLegLength * hv_AxisLegLength));
	float axisMultiplier = hypotenuse / hv_AxisLegLength;
	float spindleMultiplier = hypotenuse / hv_SpindleLegLength;
	hv_AxisLegLength = round(axisMaxSpeed * axisMultiplier);
	hv_SpindleLegLength = round(spindleMaxSpeed * spindleMultiplier);

}

/// <summary>
/// Move axis
/// </summary>
/// <comment>
/// </comment>
/// <param name="axisId">Id of axis</param>
/// <param name="directionAxis">Counterclockwise: -1, Clockwise: 1</param>
/// <returns></returns>
void xMoveAxis(int axisId, int directionAxis)
{
	StepControl stepController;
	Stepper stepper_Z(PIN_AXIS_Z_STEP, PIN_AXIS_Z_DIR);
	Stepper stepper_X(PIN_AXIS_X_STEP, PIN_AXIS_X_DIR);
	Stepper stepper_B(PIN_AXIS_B_STEP, PIN_AXIS_B_DIR);

	int limitPin_Max = configSetup.limit_Max_Z;
	int limitPin_Min = configSetup.limit_Min_Z;
	limitTriggered = false;

	double stepsToMove = 0;
	float nextSpeed = 0;
	//int32_t position_Axis = endPosition_Axis;

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
#ifdef DEBUG
	Serial.println("AxisId: =============================================");
	Serial.println(axisId);
#endif // Debug
	switch (axisId)
	{
		case ID_MOVE_AXIS_Z1:
		{
			limitPin_Max = configSetup.limit_Max_Z;
			limitPin_Min = configSetup.limit_Min_Z;
			//stepper_Z.setPosition(0);
			Serial.print("Initial position: ");
			Serial.println(stepper_Z.getPosition());

			SetEnable(ID_AXIS_Z, true, true);

			Serial.print("Enable position: ");
			Serial.println(stepper_Z.getPosition());

			MilliDelay(6);

			stepsToMove = (configMove.distance_MoveZ1 / (configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z));

			// Set speed and acceleration
			nextSpeed = configMove.speedPercent_Axis_Z * configMove.maxSpd_Axis_Z * .01;
			stepper_Z.setMaxSpeed(nextSpeed);
			stepper_Z.setAcceleration(configMove.accel_Axis_Z);
			//MilliDelay(15);
			stepper_Z.setTargetRel(stepsToMove * directionAxis);
			//MilliDelay(6);
			Serial.print("setTargetRel position: ");
			Serial.println(stepper_Z.getPosition());
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

			//MilliDelay(10);
			stepController.moveAsync(stepper_Z);
			//stepController.move(stepper_Z);
			break;
		}
		case ID_MOVE_AXIS_Z2:
		{
			limitPin_Max = configSetup.limit_Max_Z;
			limitPin_Min = configSetup.limit_Min_Z;
			SetEnable(ID_AXIS_Z, true, true);
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

			stepController.moveAsync(stepper_Z);
			break;
		}

		case ID_MOVE_AXIS_X1:
		{
			limitPin_Max = configSetup.limit_Max_X;
			limitPin_Min = configSetup.limit_Min_X;
			SetEnable(ID_AXIS_X, true, true);
#ifdef DEBUG
			Serial.print("xAltX: ");
			Serial.println(configSetup.xAltX);
#endif // Debug
			if (configSetup.xAltX == 0)
			{
				stepsToMove = (configMove.distance_MoveX1 / (configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X));
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
				Serial.print(accel_Char);
				Serial.println(configMove.accel_Axis_X);
#endif // DEBUG
			}
			else
			{
				stepsToMove = (configMove.distance_MoveX1 / (configSetup.distancePerRev_AxisXAlt) * (configSetup.steps360_Axis_XAlt * configSetup.microsteps_Axis_XAlt));
#ifdef DEBUG
				Serial.println("Alt X-----------------: ");
				Serial.print(distancePer360_Char);
				Serial.println(configSetup.distancePerRev_AxisXAlt);
				Serial.print(stepsPer360_Char);
				Serial.println(configSetup.steps360_Axis_XAlt);
				Serial.print(microsteps_Char);
				Serial.println(configSetup.microsteps_Axis_XAlt);
				Serial.print(distance_Char);
				Serial.println(configMove.distance_MoveX1);

#endif // DEBUG
			}
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

			
			stepController.moveAsync(stepper_X);
			break;
		}
		case ID_MOVE_AXIS_X2:
		{
			limitPin_Max = configSetup.limit_Max_X;
			limitPin_Min = configSetup.limit_Min_X;
			SetEnable(ID_AXIS_X, true, true);
#ifdef DEBUG
			Serial.print("xAltX: ");
			Serial.println(configSetup.xAltX);
#endif // Debug
			if (configSetup.xAltX == 0)
			{
				stepsToMove = (configMove.distance_MoveX2 / (configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X));
			}
			else
			{
				stepsToMove = (configMove.distance_MoveX2 / (configSetup.distancePerRev_AxisXAlt) * (configSetup.steps360_Axis_XAlt * configSetup.microsteps_Axis_XAlt));
			}
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

			SetEnable(ID_AXIS_X, true, true);
			stepController.moveAsync(stepper_X);
			break;
		}

		case ID_MOVE_AXIS_B1:
		{
			limitPin_Max = configSetup.limit_Max_B;
			limitPin_Min = configSetup.limit_Min_B;
			SetEnable(ID_AXIS_B, true, true);
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


			stepController.moveAsync(stepper_B);
			break;
		}
		case ID_MOVE_AXIS_B2:
		{
			limitPin_Max = configSetup.limit_Max_B;
			limitPin_Min = configSetup.limit_Min_B;
			SetEnable(ID_AXIS_B, true, true);
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

			stepController.moveAsync(stepper_B);
			break;
		}
	}

	// Set limit switches HIGH
	digitalWrite(limitPin_Max, HIGH);
	digitalWrite(limitPin_Min, HIGH);
	
	while (stepController.isRunning())
	{

		if ((digitalRead(limitPin_Max) != configSetup.limit_NCorNO) && directionAxis > 0)
		{
			Serial.println("                          limitPin_Max");
			stepController.emergencyStop();
			stopSteppers = true;
			SetLimitColors(PAGE_MOVE, directionAxis);
			goto endLoop;

		}
		else if ((digitalRead(limitPin_Min) != configSetup.limit_NCorNO) && directionAxis < 0)
		{
			Serial.println("                          limitPin_Min");

			stepController.emergencyStop();
			stopSteppers = true;
			SetLimitColors(PAGE_MOVE, directionAxis);
			goto endLoop;

		}

		if (configSetup.eStop != 0)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
				Serial.println("                          EStop");

				stepController.emergencyStop();
				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_B, false, false);
				SetEStopColors(PAGE_MOVE);
				return;
			}
		}


		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
				case 83: // - S
				{
					stepController.stop();
					stopSteppers = true;
					break;
				}
			}
		}

		MilliDelay(2);

	}

	endLoop:

	if (!limitTriggered)
	{
		// Update Move flag on Nextion. 
		SerialPrint(pageMove_va0_Char);
		SerialPrint(nextionEnd);

		SerialPrint(pageMove_bt1_Char);
		SerialPrint(nextionEnd);
	}
	SerialPrint(pageMove_bt2_val_Char);
	SerialPrint(nextionEnd);
	SerialPrint(pageMove_bt3_Char);
	SerialPrint(nextionEnd);


	switch (axisId)
	{
		case ID_MOVE_AXIS_Z2:
		case ID_MOVE_AXIS_Z1:
		{
			SerialPrint(pageMove_t6_Char);
			SerialWrite(0x22);
			SerialPrint(distanceTotal_MoveZ);
			SerialPrint(nextionQuoteEnd);

			endPosition_Axis = stepper_Z.getPosition();
			Serial.print(position_Char);
			Serial.println(endPosition_Axis);
			//endPosition_Axis = endPosition_Axis + stepper_Z.getPosition();
			startPositionAbs_Axis = endPosition_Axis;
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
			////endPosition_Spindle = 0;

			SetEnable(ID_AXIS_Z, false,true);
			break;
		}
		case ID_MOVE_AXIS_X1:
		case ID_MOVE_AXIS_X2:
		{
			SerialPrint(pageMove_t6_Char);
			SerialWrite(0x22);
			SerialPrint(distanceTotal_MoveX);
			SerialPrint(nextionQuoteEnd);

			endPosition_Axis = stepper_X.getPosition();
			//endPosition_Axis = endPosition_Axis + stepper_X.getPosition();
			startPositionAbs_Axis = endPosition_Axis;
#ifdef DEBUG
			Serial.print(position_Char);
			Serial.println(endPosition_Axis);
#endif // DEBUG

//#ifdef SHOW_POSITION 
//			distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_X);
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
//			MilliDelay(5);
//#endif // Show Position
			////endPosition_Spindle = 0;

			SetEnable(ID_AXIS_X, false, true);
			break;
		}
		case ID_MOVE_AXIS_B1:
		case ID_MOVE_AXIS_B2:
		{
			// ToDo:
			//SerialPrint(pageMove_t6_Char);
			//SerialWrite(0x22);
			//SerialPrint(distanceTotal_MoveB);
			//SerialPrint(nextionQuoteEnd);

			endPosition_Axis = stepper_B.getPosition();
			////endPosition_Axis = endPosition_Axis + stepper_B.getPosition();
			startPositionAbs_Axis = endPosition_Axis;
#ifdef DEBUG
			Serial.print(position_Char);
			Serial.println(endPosition_Axis);
#endif // DEBUG

//#ifdef SHOW_POSITION 
//					distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_B);
//					SerialPrint("pageBE.t2.txt=");
//					SerialWrite(0x22);
//					SerialPrint(distance_Axis);
//					SerialWrite(0x22);
//					SerialWrite(0xff);
//					SerialWrite(0xff);
//					SerialWrite(0xff);
//					MilliDelay(5);
//					SerialPrint("pageBE.t2.txt=");
//					SerialWrite(0x22);
//					SerialPrint(distance_Axis);
//					SerialWrite(0x22);
//					SerialWrite(0xff);
//					SerialWrite(0xff);
//					SerialWrite(0xff);
//					MilliDelay(5);
//#endif // Show Position
			////endPosition_Spindle = 0;

			SetEnable(ID_AXIS_B, false, true);
			break;
		}
	}
	
	// 0 = Change to disabled, 1 = keep enabled
	if (configSetup.keepSteppersEnabled == 1)
	{
		SerialPrint("pageMove.bt11.val=1"); // 0 = enabled
		SerialPrint("\xFF\xFF\xFF");
	}
	else
	{
		SerialPrint("pageMove.bt11.val=0"); // 1 = disabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageMove.bt11.txt="); // 1 = disabled
		SerialPrint("\x22");
		SerialPrint("Disabled");
		SerialPrint("\x22\xFF\xFF\xFF");
	}

	////SetEnable(ID_AXIS_Z, false);
	////SetEnable(ID_AXIS_X, false);
	////SetEnable(ID_AXIS_B, false);
	////startPositionAbs_Axis = 0;
	//digitalWrite(PIN_AXIS_Z_DIR, HIGH);
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

	int limitPin_Max = configSetup.limit_Max_Z;
	int limitPin_Min = configSetup.limit_Min_Z;
	limitTriggered = false;

	double stepsToMove = 0;
	float nextSpeed = 0;
	//int32_t position_Axis = endPosition_Axis;

	const char* distancePer360_Char = "Distance/360:";
	const char* stepsPer360_Char = "Steps/360:";
	const char* microsteps_Char = "Microsteps:";
	const char* distance_Char = "Distance:";
	const char* steps_Char = "Steps:";
	const char* maxSpd_Char = "Max Speed:";
	const char* speedPercent_Char = "Speed Percent:";
	const char* nextSpeed_Char = "Next Speed:";
	const char* accel_Char = "Accel:";
	const char* position_Char = "Position:";
	//const char * pageMove_bt2_pco_Char = "pageMove.bt2.pco = 59391";
	const char* pageMove_bt2_val_Char = "pageMove.bt2.val=0";
	const char* pageMove_va0_Char = "pageMove.va0.val=0";
	const char* pageMove_bt1_Char = "pageMove.bt1.val=0";
	const char* pageMove_bt3_Char = "pageMove.bt3.val=0";
	const char* pageMove_t6_Char = "pageMove.t6.txt=";

	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	const char* nextionEnd = "\xFF\xFF\xFF";
#ifdef DEBUG
	Serial.println("AxisId: =============================================");
	Serial.println(axisId);
#endif // Debug
	switch (axisId)
	{
	case ID_MOVE_AXIS_Z1:
	{
		limitPin_Max = configSetup.limit_Max_Z;
		limitPin_Min = configSetup.limit_Min_Z;
		//stepper_Z.setPosition(0);
		Serial.print("Initial position: ");
		Serial.println(stepper_Z.getPosition());

		SetEnable(ID_AXIS_Z, true, true);

		Serial.print("Enable position: ");
		Serial.println(stepper_Z.getPosition());

		MilliDelay(6);

		stepsToMove = (configMove.distance_MoveZ1 / (configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z));

		// Set speed and acceleration
		nextSpeed = configMove.speedPercent_Axis_Z * configMove.maxSpd_Axis_Z * .01;
		stepper_Z.setMaxSpeed(nextSpeed);
		stepper_Z.setAcceleration(configMove.accel_Axis_Z);
		//MilliDelay(15);
		stepper_Z.setTargetRel(stepsToMove * directionAxis);
		//MilliDelay(6);
		Serial.print("setTargetRel position: ");
		Serial.println(stepper_Z.getPosition());
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

		//MilliDelay(10);
		stepController.moveAsync(stepper_Z);
		//stepController.move(stepper_Z);
		break;
	}
	case ID_MOVE_AXIS_Z2:
	{
		limitPin_Max = configSetup.limit_Max_Z;
		limitPin_Min = configSetup.limit_Min_Z;
		SetEnable(ID_AXIS_Z, true, true);
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

		stepController.moveAsync(stepper_Z);
		break;
	}

	case ID_MOVE_AXIS_X1:
	{
		limitPin_Max = configSetup.limit_Max_X;
		limitPin_Min = configSetup.limit_Min_X;
		SetEnable(ID_AXIS_X, true, true);
#ifdef DEBUG
		Serial.print("xAltX: ");
		Serial.println(configSetup.xAltX);
#endif // Debug
		if (configSetup.xAltX == 0)
		{
			stepsToMove = (configMove.distance_MoveX1 / (configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X));
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
			Serial.print(accel_Char);
			Serial.println(configMove.accel_Axis_X);
#endif // DEBUG
		}
		else
		{
			stepsToMove = (configMove.distance_MoveX1 / (configSetup.distancePerRev_AxisXAlt) * (configSetup.steps360_Axis_XAlt * configSetup.microsteps_Axis_XAlt));
#ifdef DEBUG
			Serial.println("Alt X-----------------: ");
			Serial.print(distancePer360_Char);
			Serial.println(configSetup.distancePerRev_AxisXAlt);
			Serial.print(stepsPer360_Char);
			Serial.println(configSetup.steps360_Axis_XAlt);
			Serial.print(microsteps_Char);
			Serial.println(configSetup.microsteps_Axis_XAlt);
			Serial.print(distance_Char);
			Serial.println(configMove.distance_MoveX1);

#endif // DEBUG
		}
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


		stepController.moveAsync(stepper_X);
		break;
	}
	case ID_MOVE_AXIS_X2:
	{
		limitPin_Max = configSetup.limit_Max_X;
		limitPin_Min = configSetup.limit_Min_X;
		SetEnable(ID_AXIS_X, true, true);
#ifdef DEBUG
		Serial.print("xAltX: ");
		Serial.println(configSetup.xAltX);
#endif // Debug
		if (configSetup.xAltX == 0)
		{
			stepsToMove = (configMove.distance_MoveX2 / (configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X));
		}
		else
		{
			stepsToMove = (configMove.distance_MoveX2 / (configSetup.distancePerRev_AxisXAlt) * (configSetup.steps360_Axis_XAlt * configSetup.microsteps_Axis_XAlt));
		}
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

		SetEnable(ID_AXIS_X, true, true);
		stepController.moveAsync(stepper_X);
		break;
	}

	case ID_MOVE_AXIS_B1:
	{
		limitPin_Max = configSetup.limit_Max_B;
		limitPin_Min = configSetup.limit_Min_B;
		SetEnable(ID_AXIS_B, true, true);
		if (configSetup.radialOrLinear_Axis_B == RADIAL_B)
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
		Serial.println(configMove.distance_MoveB1, 4);

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


		stepController.moveAsync(stepper_B);
		break;
	}
	case ID_MOVE_AXIS_B2:
	{
		limitPin_Max = configSetup.limit_Max_B;
		limitPin_Min = configSetup.limit_Min_B;
		SetEnable(ID_AXIS_B, true, true);
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

		stepController.moveAsync(stepper_B);
		break;
	}
	}

	// Set limit switches HIGH
	digitalWrite(limitPin_Max, HIGH);
	digitalWrite(limitPin_Min, HIGH);

	while (stepController.isRunning())
	{

		if ((digitalRead(limitPin_Max) != configSetup.limit_NCorNO) && directionAxis > 0)
		{
			Serial.println("                          limitPin_Max");
			stepController.emergencyStop();
			stopSteppers = true;
			SetLimitColors(PAGE_MOVE, directionAxis);
			goto endLoop;

		}
		else if ((digitalRead(limitPin_Min) != configSetup.limit_NCorNO) && directionAxis < 0)
		{
			Serial.println("                          limitPin_Min");

			stepController.emergencyStop();
			stopSteppers = true;
			SetLimitColors(PAGE_MOVE, directionAxis);
			goto endLoop;

		}

		if(configSetup.eStop !=0)
		{ 
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
				Serial.println("                          EStop");

				stepController.emergencyStop();
				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_B, false, false);
				SetEStopColors(PAGE_MOVE);
				return;
			}
		}
		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
			case 83: // - S
			{
				stepController.stop();
				stopSteppers = true;
				break;
			}
			}
		}

		MilliDelay(2);

	}

endLoop:

	if (!limitTriggered)
	{
		// Update Move flag on Nextion. 
		SerialPrint(pageMove_va0_Char);
		SerialPrint(nextionEnd);
		SerialPrint(pageMove_bt1_Char);
		SerialPrint(nextionEnd);
	}
	SerialPrint(pageMove_bt2_val_Char);
	SerialPrint(nextionEnd);
	SerialPrint(pageMove_bt3_Char);
	SerialPrint(nextionEnd);


	switch (axisId)
	{
	case ID_MOVE_AXIS_Z2:
	case ID_MOVE_AXIS_Z1:
	{
		SerialPrint(pageMove_t6_Char);
		SerialWrite(0x22);
		SerialPrint(distanceTotal_MoveZ);
		SerialPrint(nextionQuoteEnd);

		endPosition_Axis = stepper_Z.getPosition();
		Serial.print(position_Char);
		Serial.println(endPosition_Axis);
		//endPosition_Axis = endPosition_Axis + stepper_Z.getPosition();
		startPositionAbs_Axis = endPosition_Axis;
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
		////endPosition_Spindle = 0;

		SetEnable(ID_AXIS_Z, false, true);
		break;
	}
	case ID_MOVE_AXIS_X1:
	case ID_MOVE_AXIS_X2:
	{
		SerialPrint(pageMove_t6_Char);
		SerialWrite(0x22);
		SerialPrint(distanceTotal_MoveX);
		SerialPrint(nextionQuoteEnd);

		endPosition_Axis = stepper_X.getPosition();
		//endPosition_Axis = endPosition_Axis + stepper_X.getPosition();
		startPositionAbs_Axis = endPosition_Axis;
#ifdef DEBUG
		Serial.print(position_Char);
		Serial.println(endPosition_Axis);
#endif // DEBUG

		//#ifdef SHOW_POSITION 
		//			distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_X);
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
		//			MilliDelay(5);
		//#endif // Show Position
					////endPosition_Spindle = 0;

		SetEnable(ID_AXIS_X, false, true);
		break;
	}
	case ID_MOVE_AXIS_B1:
	case ID_MOVE_AXIS_B2:
	{
		// ToDo:
		//SerialPrint(pageMove_t6_Char);
		//SerialWrite(0x22);
		//SerialPrint(distanceTotal_MoveB);
		//SerialPrint(nextionQuoteEnd);

		endPosition_Axis = stepper_B.getPosition();
		////endPosition_Axis = endPosition_Axis + stepper_B.getPosition();
		startPositionAbs_Axis = endPosition_Axis;
#ifdef DEBUG
		Serial.print(position_Char);
		Serial.println(endPosition_Axis);
#endif // DEBUG

		//#ifdef SHOW_POSITION 
		//					distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_B);
		//					SerialPrint("pageBE.t2.txt=");
		//					SerialWrite(0x22);
		//					SerialPrint(distance_Axis);
		//					SerialWrite(0x22);
		//					SerialWrite(0xff);
		//					SerialWrite(0xff);
		//					SerialWrite(0xff);
		//					MilliDelay(5);
		//					SerialPrint("pageBE.t2.txt=");
		//					SerialWrite(0x22);
		//					SerialPrint(distance_Axis);
		//					SerialWrite(0x22);
		//					SerialWrite(0xff);
		//					SerialWrite(0xff);
		//					SerialWrite(0xff);
		//					MilliDelay(5);
		//#endif // Show Position
					////endPosition_Spindle = 0;

		SetEnable(ID_AXIS_B, false, true);
		break;
	}
	}

	// 0 = Change to disabled, 1 = keep enabled
	if (configSetup.keepSteppersEnabled == 1)
	{
		SerialPrint("pageMove.bt11.val=1"); // 0 = enabled
		SerialPrint("\xFF\xFF\xFF");
	}
	else
	{
		SerialPrint("pageMove.bt11.val=0"); // 1 = disabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageMove.bt11.txt="); // 1 = disabled
		SerialPrint("\x22");
		SerialPrint("Disabled");
		SerialPrint("\x22\xFF\xFF\xFF");
	}

	////SetEnable(ID_AXIS_Z, false);
	////SetEnable(ID_AXIS_X, false);
	////SetEnable(ID_AXIS_B, false);
	////startPositionAbs_Axis = 0;
	//digitalWrite(PIN_AXIS_Z_DIR, HIGH);
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
//	const char * pageOne_bt10_bco1_Char = "pageOne.bt10.bco1=55715";
//	const char* pageSplash_vaSpEnabled_val_Char = "pageSplash.vaSpEnabled.val=1";
	//const char* pageOne_bt10_val_Char = "pageOne.bt10.val=1";
//	const char * nextionEnd = "\xFF\xFF\xFF";

	// Reset end positions
	startPositionAbs_Axis = 0;
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
	int32_t targetPosition = 2147000000;// *direction;

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

	// Set limit switches HIGH
	digitalWrite(limitPin_Max, HIGH);
	digitalWrite(limitPin_Min, HIGH);

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

	SetEnable(configOne.axisId, true, true);
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
			    case 83: // - S
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

		if ((digitalRead(limitPin_Max) != configSetup.limit_NCorNO) && direction == DIR_CW)
		{
			rotateController.overrideSpeed(0);
			rotateController.emergencyStop();
			SetLimitColors(PAGE_ONE, DIR_CW);
			goto endLoop;
		}
		else if ((digitalRead(limitPin_Min) != configSetup.limit_NCorNO) && direction == DIR_CCW)
		{
			rotateController.overrideSpeed(0);
			rotateController.emergencyStop();
			SetLimitColors(PAGE_ONE, DIR_CCW);
			goto endLoop;
		}

		if (configSetup.eStop != 0)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
				Serial.println("                          EStop ");

				rotateController.emergencyStop();
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_B, false, false);
				SetEStopColors(PAGE_ONE);
				return;
			}
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
		endPosition_Axis = 0;
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
		SerialPrint(endPosition_Axis);
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
	SetEnable(configOne.axisId, false, true);
	//SerialPrint(pageOne_bt10_bco1_Char);
	//SerialPrint(nextionEnd);
	//SerialPrint(pageSplash_vaSpEnabled_val_Char);
	//SerialPrint(nextionEnd);
	//SerialPrint(pageOne_bt10_val_Char);
	//SerialPrint(nextionEnd);


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

	switch (configOne.axisId)
	{
		case ID_AXIS_Z:
		{
			// 0 = Change to disabled, 1 = keep enabled
			if (configSetup.keepSteppersEnabled == 1)
			{
				SerialPrint("pageOne.bt11.val=1"); // 0 = enabled
				SerialPrint("\xFF\xFF\xFF");
			}
			else
			{
				SerialPrint("pageOne.bt11.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageOne.bt11.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
			}
			break;
		}
		case ID_AXIS_X:
		{
			// 0 = Change to disabled, 1 = keep enabled
			if (configSetup.keepSteppersEnabled == 1)
			{
				SerialPrint("pageOne.bt12.val=1"); // 0 = enabled
				SerialPrint("\xFF\xFF\xFF");
			}
			else
			{
				SerialPrint("pageOne.bt12.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageOne.bt12.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
			}
			break;
		}
		case ID_AXIS_B:
		{
			// 0 = Change to disabled, 1 = keep enabled
			if (configSetup.keepSteppersEnabled == 1)
			{
				SerialPrint("pageOne.bt13.val=1"); // 0 = enabled
				SerialPrint("\xFF\xFF\xFF");
			}
			else
			{
				SerialPrint("pageOne.bt13.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageOne.bt13.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
			}
			break;
		}
		case ID_SPINDLE:
		{
			// 0 = Change to disabled, 1 = keep enabled
			if (configSetup.keepSteppersEnabled == 1)
			{
				SerialPrint("pageOne.bt10.val=1"); // 0 = enabled
				SerialPrint("\xFF\xFF\xFF");
			}
			else
			{
				SerialPrint("pageOne.bt10.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageOne.bt10.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
			}
			break;
		}
	}



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
	int32_t targetPosition_Axis = 2147000000;
	int32_t targetPosition_Spindle = 2147000000;

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

	// Set limit switches HIGH
	digitalWrite(limitPin_Max, HIGH);
	digitalWrite(limitPin_Min, HIGH);

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
				SetEnable(ID_AXIS_Z, true, true);
				rotateController_MainAxis.rotateAsync(stepper_Z);
				rotateController_MainAxis.overrideSpeed(0);
				MilliDelay(10);
				rotateController_MainAxis.overrideSpeed(configMain.speedPercent_Axis_Z * .01);
				break;
			}
			case ID_AXIS_X:
			{
				SetEnable(ID_AXIS_X, true, true);
				rotateController_MainAxis.overrideSpeed(0);
				rotateController_MainAxis.overrideAcceleration(0);
				rotateController_MainAxis.rotateAsync(stepper_X);

				MilliDelay(10);
				rotateController_MainAxis.overrideSpeed(configMain.speedPercent_Axis_X * .01);
				break;
			}
			case ID_AXIS_B:
			{
				SetEnable(ID_AXIS_B, true, true);
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
		SetEnable(ID_SPINDLE, true, true);
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
		if (configSetup.eStop != 0)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
				Serial.println("                          EStop ");
				rotateController_MainSpindle.emergencyStop();
				rotateController_MainAxis.emergencyStop();
				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_B, false, false);

				// Change Btn color on Nextion
				SetEStopColors(PAGE_MAIN);
				return;
			}
		}

		MilliDelay(5);
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);

			switch (incomingByte)
			{
				case 83: // - S
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

					SetEnable(ID_SPINDLE, false, true);
					stepper_Spindle_Go = false;

					// 0 = Change to disabled, 1 = keep enabled
					if (configSetup.keepSteppersEnabled == 1)
					{
						SerialPrint("pageMain.bt10.val=1"); // 0 = enabled
						SerialPrint("\xFF\xFF\xFF");
					}
					else
					{
						SerialPrint("pageMain.bt10.val=0"); // 1 = disabled
						SerialPrint("\xFF\xFF\xFF");
						SerialPrint("pageMain.bt10.txt="); // 1 = disabled
						SerialPrint("\x22");
						SerialPrint("Disabled");
						SerialPrint("\x22\xFF\xFF\xFF");
					}
					break;
				}
				case 90: // Z - Z Axis CW
				{
					if (!rotateController_MainAxis.isRunning())
					{
						stepper_Axis_Go = true;
						direction_Axis = DIR_CW;
						targetPosition_Axis = 2147000000;
						speedPercentAxis = (float)((configMain.speedPercent_Axis_Z) * .01);
						stepper_Z
							.setMaxSpeed(configMain.maxSpd_Axis_Z * direction_Axis)
							.setTargetAbs(targetPosition_Axis);

						SetEnable(ID_AXIS_Z, true, true);
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
						targetPosition_Axis = -2147000000;
						speedPercentAxis = (float)((configMain.speedPercent_Axis_Z) * .01);
						stepper_Z
							.setMaxSpeed(configMain.maxSpd_Axis_Z * direction_Axis)
							.setTargetAbs(targetPosition_Axis);

						SetEnable(ID_AXIS_Z, true, true);
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
							SetEnable(ID_AXIS_Z, false, true);
							break;
						}
						case ID_AXIS_X:
						{
							SetEnable(ID_AXIS_X, false, true);
							break;
						}
						case ID_AXIS_B:
						{
							SetEnable(ID_AXIS_B, false, true);
							break;
						}
					}

					stepper_Axis_Go = false;
					// 0 = Change to disabled, 1 = keep enabled
					if (configSetup.keepSteppersEnabled == 1)
					{
						SerialPrint("pageMain.bt11.val=1"); // 0 = enabled
						SerialPrint("\xFF\xFF\xFF");
					}
					else
					{
						SerialPrint("pageMain.bt11.val=0"); // 1 = disabled
						SerialPrint("\xFF\xFF\xFF");
						SerialPrint("pageMain.bt11.txt="); // 1 = disabled
						SerialPrint("\x22");
						SerialPrint("Disabled");
						SerialPrint("\x22\xFF\xFF\xFF");
					}

					// Ensure user doesn't switch axes too fast.
					MilliDelay(1000);
					break;
				}
				case 107: // k -  Z spindle CW
				{
					if (!rotateController_MainSpindle.isRunning())
					{
						direction_Spindle = DIR_CW;
						targetPosition_Spindle = 2147000000;
						speedPercentSpindle = (float)(configMain.speedPercent_Spindle * .01);
						stepper_Spindle
							.setMaxSpeed(maxSpd_Spindle)
							.setTargetAbs(targetPosition_Spindle);
						stepper_Spindle_Go = true;

						SetEnable(ID_SPINDLE, true, true);
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
						targetPosition_Spindle = 2147000000;
						speedPercentSpindle = (float)(configMain.speedPercent_Spindle * .01);
						stepper_Spindle
							.setMaxSpeed(maxSpd_Spindle * DIR_CCW)
							.setTargetAbs(targetPosition_Spindle);
						stepper_Spindle_Go = true;

						SetEnable(ID_SPINDLE, true, true);
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
						targetPosition_Axis = -2147000000;
						speedPercentAxis = (float)(configMain.speedPercent_Axis_X * .01);
						stepper_X
							.setMaxSpeed(configMain.maxSpd_Axis_X * direction_Axis)
							.setTargetAbs(targetPosition_Axis);

						SetEnable(ID_AXIS_X, true, true);
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
						targetPosition_Axis = 2147000000;
						speedPercentAxis = (float)(configMain.speedPercent_Axis_X * .01);
						stepper_X
							.setMaxSpeed(configMain.maxSpd_Axis_X * direction_Axis)
							.setTargetAbs(targetPosition_Axis);

						SetEnable(ID_AXIS_X, true, true);
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
						targetPosition_Axis = -2147000000;

						speedPercentAxis = (float)((configMain.speedPercent_Axis_B) * .01);
						stepper_B
							.setMaxSpeed(configMain.maxSpd_Axis_B * direction_Axis)
							.setTargetAbs(targetPosition_Axis);

						SetEnable(ID_AXIS_B, true, true);
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
						targetPosition_Axis = 2147000000;
						speedPercentAxis = (float)((configMain.speedPercent_Axis_B) * .01);
						stepper_B
							.setMaxSpeed(configMain.maxSpd_Axis_B * direction_Axis)
							.setTargetAbs(targetPosition_Axis);

						SetEnable(ID_AXIS_B, true, true);
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

			if ((digitalRead(limitPin_Max) != configSetup.limit_NCorNO) && direction_Axis==DIR_CW)
			{
				Serial.print("Limit_Max - CW - Tripped:  ");
				Serial.println(configSetup.limit_NCorNO);

				rotateController_MainAxis.overrideSpeed(0);
				rotateController_MainAxis.emergencyStop();
				if (configSetup.limit_StopSpindle)
				{
					// Stop the spindle
					rotateController_MainSpindle.overrideSpeed(0);
					MilliDelay(5);
					rotateController_MainSpindle.emergencyStop();
					SetEnable(ID_SPINDLE, false, true);
					stepper_Spindle_Go = false;

					// 0 = Change to disabled, 1 = keep enabled
					if (configSetup.keepSteppersEnabled == 1)
					{
						SerialPrint("pageMain.bt10.val=1"); // 0 = enabled
						SerialPrint("\xFF\xFF\xFF");
					}
					else
					{
						SerialPrint("pageMain.bt10.val=0"); // 1 = disabled
						SerialPrint("\xFF\xFF\xFF");
						SerialPrint("pageMain.bt10.txt="); // 1 = disabled
						SerialPrint("\x22");
						SerialPrint("Disabled");
						SerialPrint("\x22\xFF\xFF\xFF");
					}
				}

				stepper_Axis_Go = false;

				// 0 = Change to disabled, 1 = keep enabled
				if (configSetup.keepSteppersEnabled == 1)
				{
					SerialPrint("pageMain.bt11.val=1"); // 0 = enabled
					SerialPrint("\xFF\xFF\xFF");
				}
				else
				{
					SerialPrint("pageMain.bt11.val=0"); // 1 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMain.bt11.txt="); // 1 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
				}
				SetLimitColors(PAGE_MAIN, direction_Axis);

			}

			//if ((digitalRead(limitPin_Min) == LOW) && direction_Axis == DIR_CCW)
			if ((digitalRead(limitPin_Min) != configSetup.limit_NCorNO) && direction_Axis == DIR_CCW)
			{
				Serial.println("Limit_Min - CCW - Tripped:  ");
				Serial.println(configSetup.limit_NCorNO);
				rotateController_MainAxis.overrideSpeed(0);
				rotateController_MainAxis.emergencyStop();
				stepper_Axis_Go = false;

				if (configSetup.limit_StopSpindle)
				{
					// Stop the spindle
					rotateController_MainSpindle.overrideSpeed(0);
					MilliDelay(5);
					rotateController_MainSpindle.emergencyStop();
					SetEnable(ID_SPINDLE, false, true);
					stepper_Spindle_Go = false;
					// 0 = Change to disabled, 1 = keep enabled
					if (configSetup.keepSteppersEnabled == 1)
					{
						SerialPrint("pageMain.bt10.val=1"); // 0 = enabled
						SerialPrint("\xFF\xFF\xFF");
					}
					else
					{
						SerialPrint("pageMain.bt10.val=0"); // 1 = disabled
						SerialPrint("\xFF\xFF\xFF");
						SerialPrint("pageMain.bt10.txt="); // 1 = disabled
						SerialPrint("\x22");
						SerialPrint("Disabled");
						SerialPrint("\x22\xFF\xFF\xFF");
					}
				}

				// 0 = Change to disabled, 1 = keep enabled
				if (configSetup.keepSteppersEnabled == 1)
				{
					SerialPrint("pageMain.bt11.val=1"); // 0 = enabled
					SerialPrint("\xFF\xFF\xFF");
				}
				else
				{
					SerialPrint("pageMain.bt11.val=0"); // 1 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMain.bt11.txt="); // 1 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
				}

				// Change Btn color on Nextion
				SerialPrint("pageMain.bt5.bco=55715");
				SerialPrint("\xFF\xFF\xFF");
				MilliDelay(10);
				SerialPrint("pageMain.bt5.val=0");
				SerialPrint("\xFF\xFF\xFF");
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
			endPosition_Axis = stepper_Z.getPosition();
			stepper_Z.setPosition(0);
			break;
		}
		case ID_AXIS_X:
		{
			endPosition_Axis = stepper_X.getPosition();
			stepper_X.setPosition(0);
			break;
		}
		case ID_AXIS_B:
		{
			endPosition_Axis = stepper_B.getPosition();
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
	distance_Axis = StepsToDistance_Axis(endPosition_Axis, configMain.axisId);
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
	startPositionAbs_Axis = 0;
	SetEnable(ID_SPINDLE, false, true);
	SetEnable(ID_AXIS_Z, false, true);
	SetEnable(ID_AXIS_X, false, true);
	SetEnable(ID_AXIS_B, false, true);
}

/// <summary>
/// Index spindle
/// </summary>
/// <comment>
/// </comment>
/// <param name="directionSpindle">Counterclockwise: -1, Clockwise: 1</param>
/// <returns></returns>
void xIndexSpindle(int directionSpindle)
{
	StepControl stepController;
	//StepControl stepController(configSetup.pulseWidth_Spindle, configSetup.speedUpdatePeriod_Spindle);
	//StepControl stepController(2.75, 10000);
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
	
	stepper_Spindle.setMaxSpeed(configIndex_Main.maxSpd);
	stepper_Spindle.setAcceleration(configIndex_Main.accel);
	stepper_Spindle.setTargetRel(stepsToMove * directionSpindle);
		
		
	//stepper_Spindle.setTargetRel(256000);

	Serial.print("setTargetRel:   ");
	Serial.println(stepsToMove * directionSpindle);

	SetEnable(ID_SPINDLE, true, true);
	stepController.moveAsync(stepper_Spindle);
	//StepControl(10, 8000) stepController;  // pulsewidth 10µs, acceleration update 8ms
	//stepController.move(stepper_Spindle);
	while (stepController.isRunning())
	{
		MilliDelay(5);
		if (configSetup.eStop != 0)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
				Serial.println("                          EStop ");
				stepController.emergencyStop();
				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_B, false, false);
				SetEStopColors(PAGE_INDEX);

				return;
			}
		}

		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);

			switch (incomingByte)
			{
				case 83: // - S
				{
					stepController.stop();
					stopSteppers = true;
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

	returnSteps_Spindle = stepper_Spindle.getPosition();

	endPosition_Spindle = endPosition_Spindle + returnSteps_Spindle;
#ifdef SHOW_POSITION 
	degrees_Spindle = StepsToDegrees_Spindle(returnSteps_Spindle);

	SerialPrint(pageBE_t1_Char);
	SerialWrite(0x22);
	SerialPrint(degrees_Spindle);
	SerialPrint(nextionQuoteEnd);
#endif // Show Position
	SetEnable(ID_SPINDLE, false, true);
	// 0 = Change to disabled, 1 = keep enabled
	if (configSetup.keepSteppersEnabled == 1)
	{
		SerialPrint("pageIndex.bt10.val=1"); // 0 = enabled
		SerialPrint("\xFF\xFF\xFF");
	}
	else
	{
		SerialPrint("pageIndex.bt10.val=0"); // 1 = disabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageIndex.bt10.txt="); // 1 = disabled
		SerialPrint("\x22");
		SerialPrint("Disabled");
		SerialPrint("\x22\xFF\xFF\xFF");
	}

	SerialPrint("pageIndex.bt0.val=0");
	SerialPrint(nextionEnd);
}

void IndexSpindle(int directionSpindle)
{
	StepControl stepController;
	//StepControl stepController(configSetup.pulseWidth_Spindle, configSetup.speedUpdatePeriod_Spindle);
	//StepControl stepController(2.75, 10000);
	Stepper stepper_Spindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);

	const char* index1_Char = "Index 1-";
	const char* index2_Char = "Index 2-";
	const char* index3_Char = "Index 3-";
	const char* index4_Char = "Index 4-";
	const char* index5_Char = "Index 5-";
	const char* size_Char = "Size:";
	const char* direction_Char = "Direction:";
	const char* stepsToMove_Char = "Steps to Move:";
	//const char * pageIndex_bt3_pco_Char = "pageIndex.bt3.pco=59391";
	const char* pageIndex_bt3_val_Char = "pageIndex.bt3.val=0";
	const char* pageIndex_va0_Char = "pageIndex.va0.val=0";
	const char* pageIndex_bt1_val_Char = "pageIndex.bt1.val=0";
	const char* pageIndex_bt2_val_Char = "pageIndex.bt2.val=0";
	const char* pageBE_t1_Char = "pageBE.t1.txt=";

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

	stepper_Spindle.setMaxSpeed(configIndex_Main.maxSpd);
	stepper_Spindle.setAcceleration(configIndex_Main.accel);
	stepper_Spindle.setTargetRel(stepsToMove * directionSpindle);


	//stepper_Spindle.setTargetRel(256000);

	Serial.print("setTargetRel:   ");
	Serial.println(stepsToMove * directionSpindle);

	SetEnable(ID_SPINDLE, true, true);
	stepController.moveAsync(stepper_Spindle);
	//StepControl(10, 8000) stepController;  // pulsewidth 10µs, acceleration update 8ms
	//stepController.move(stepper_Spindle);
	while (stepController.isRunning())
	{
		MilliDelay(5);
		if (configSetup.eStop != 0)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
				Serial.println("                          EStop ");
				stepController.emergencyStop();
				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_B, false, false);
				SetEStopColors(PAGE_INDEX);

				return;
			}
		}

		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);

			switch (incomingByte)
			{
			case 83: // - S
			{
				stepController.stop();
				stopSteppers = true;
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

	returnSteps_Spindle = stepper_Spindle.getPosition();

	endPosition_Spindle = endPosition_Spindle + returnSteps_Spindle;
#ifdef SHOW_POSITION 
	degrees_Spindle = StepsToDegrees_Spindle(returnSteps_Spindle);

	SerialPrint(pageBE_t1_Char);
	SerialWrite(0x22);
	SerialPrint(degrees_Spindle);
	SerialPrint(nextionQuoteEnd);
#endif // Show Position
	SetEnable(ID_SPINDLE, false, true);
	// 0 = Change to disabled, 1 = keep enabled
	if (configSetup.keepSteppersEnabled == 1)
	{
		SerialPrint("pageIndex.bt10.val=1"); // 0 = enabled
		SerialPrint("\xFF\xFF\xFF");
	}
	else
	{
		SerialPrint("pageIndex.bt10.val=0"); // 1 = disabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageIndex.bt10.txt="); // 1 = disabled
		SerialPrint("\x22");
		SerialPrint("Disabled");
		SerialPrint("\x22\xFF\xFF\xFF");
	}

	SerialPrint("pageIndex.bt0.val=0");
	SerialPrint(nextionEnd);
}

void IndexFromFile(int directionSpindle)
{
	const char* indexId_Char = "Index ID:";
	const char* index1_Char = "Index1";
	const char* index2_Char = "Index2";
	const char* index3_Char = "Index3";
	const char* index4_Char = "Index4";
	const char* index5_Char = "Index5";
	const char* newIndexSize_Char = "New Index Size: ";
	const char* indexSizeChar = " Size: ";
	const char* degreeDivision_Char = " Degree or Division: ";
	const char* fileOrFixed_Char = " File or Fixed: ";
	const char* lineNumber_Char = "Line Number: ";
	const char* pageIndex_t7_Char = "pageIndex.t7.txt=";
	//const char * pageIndex_bt3_pco_Char = "pageIndex.bt3.pco=59391";
	const char* pageIndex_va0_Char = "pageIndex.va0.val=0";
	const char* pageIndex_bt3_Char = "pageIndex.bt3.val=0";
	const char* pageIndex_bt2_Char = "pageIndex.bt2.val=0";
	const char* pageIndex_bt1_Char = "pageIndex.bt1.val=0";
	const char* pageSync_b6_Char = "pageSync.b6.pco=0";
	const char* pageSync_b5_Char = "pageSync.b5.pco=0";
	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	const char* nextionEnd = "\xFF\xFF\xFF";

	double newIndexSize = 0;
	int lineNumber = 0;
	badFilename = false;

	lineNumber = GetSerialInteger();

	Serial.print(lineNumber_Char);
	Serial.println(lineNumber);
	if (lineNumber == 255)
	{
		lineNumber = 0;
	}

#ifdef DEBUG
	Serial.print(indexId_Char);
	Serial.println(configIndex_Main.indexId);
	Serial.print(lineNumber_Char);
	Serial.println(lineNumber);
#endif // DEBUG

	newIndexSize = GetIndexDataFromSD(lineNumber);

	switch (configIndex_Main.indexId)
	{
	case 1:
		{
	#ifdef DEBUG
			Serial.print(fileOrFixed_Char);
			Serial.println(configIndex_1.fileOrFixed);
	#endif // DEBUG

			configIndex_1.sizeInSteps = (configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (newIndexSize / 360);
	#ifdef DEBUG
			Serial.print(lineNumber_Char);
			Serial.println(lineNumber);
			Serial.print(newIndexSize_Char);
			Serial.println(newIndexSize);
			Serial.println(index1_Char);
			Serial.print(indexSizeChar);
			Serial.println(configIndex_1.sizeInSteps);
			Serial.println("");
	#endif // DEBUG
			SerialPrint(pageIndex_t7_Char); // Nextion may not get the first packet
			SerialWrite(0x22);
			SerialPrint(newIndexSize, 4);
			SerialPrint(nextionQuoteEnd);

			SerialPrint(pageIndex_t7_Char);
			SerialWrite(0x22);
			SerialPrint(newIndexSize, 4);
			SerialPrint(nextionQuoteEnd);

			break;
		}
		case 2:
		{
	#ifdef DEBUG
			Serial.print(index2_Char);
			Serial.println(fileOrFixed_Char);
			Serial.println(configIndex_2.fileOrFixed);
	#endif // DEBUG

			configIndex_2.sizeInSteps = (configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (newIndexSize / 360);
	#ifdef DEBUG

			Serial.print(lineNumber_Char);
			Serial.println(lineNumber);
			Serial.print(newIndexSize_Char);
			Serial.println(newIndexSize);
			Serial.print(index2_Char);
			Serial.print(indexSizeChar);
			Serial.println(configIndex_2.sizeInSteps);
	#endif // DEBUG
			SerialPrint(pageIndex_t7_Char);
			SerialWrite(0x22);
			SerialPrint(newIndexSize, 4);
			SerialPrint(nextionQuoteEnd);

			SerialPrint(pageIndex_t7_Char);
			SerialWrite(0x22);
			SerialPrint(newIndexSize, 4);
			SerialPrint(nextionQuoteEnd);

			break;
		}
		case 3:
		{
	#ifdef DEBUG
			Serial.print(index3_Char);
			Serial.print(fileOrFixed_Char);
			Serial.println(configIndex_3.fileOrFixed);
	#endif // DEBUG

			configIndex_3.sizeInSteps = (configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (newIndexSize / 360);
	#ifdef DEBUG

			Serial.print(lineNumber_Char);
			Serial.println(lineNumber);
			Serial.print(newIndexSize_Char);
			Serial.println(newIndexSize);
			Serial.print(index3_Char);
			Serial.print(indexSizeChar);
			Serial.println(configIndex_3.sizeInSteps);
	#endif // DEBUG
			SerialPrint(pageIndex_t7_Char); // Nextion may not get the first packet
			SerialWrite(0x22);
			SerialPrint(newIndexSize, 4);
			SerialPrint(nextionQuoteEnd);

			SerialPrint(pageIndex_t7_Char);
			SerialWrite(0x22);
			SerialPrint(newIndexSize, 4);
			SerialPrint(nextionQuoteEnd);

			break;
		}
		case 4:
		{
	#ifdef DEBUG
			Serial.print(index4_Char);
			Serial.print(fileOrFixed_Char);
			Serial.println(configIndex_4.fileOrFixed);
	#endif // DEBUG

			configIndex_4.sizeInSteps = (configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (newIndexSize / 360);
	#ifdef DEBUG

			Serial.print(lineNumber_Char);
			Serial.println(lineNumber);
			Serial.print(newIndexSize_Char);
			Serial.println(newIndexSize);
			Serial.print(index4_Char);
			Serial.print(indexSizeChar);
			Serial.println(configIndex_4.sizeInSteps);
	#endif // DEBUG
			SerialPrint(pageIndex_t7_Char); // Nextion may not get the first packet
			SerialWrite(0x22);
			SerialPrint(newIndexSize, 4);
			SerialPrint(nextionQuoteEnd);

			SerialPrint(pageIndex_t7_Char);
			SerialWrite(0x22);
			SerialPrint(newIndexSize, 4);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case 5:
		{
	#ifdef DEBUG
			Serial.print(index5_Char);
			Serial.print(fileOrFixed_Char);
			Serial.println(configIndex_5.fileOrFixed);
	#endif // DEBUG

			configIndex_5.sizeInSteps = (configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (newIndexSize / 360);
	#ifdef DEBUG

			Serial.print(lineNumber_Char);
			Serial.println(lineNumber);
			Serial.print(newIndexSize_Char);
			Serial.println(newIndexSize);
			Serial.print(index5_Char);
			Serial.print(indexSizeChar);
			Serial.println(configIndex_5.sizeInSteps);
	#endif // DEBUG
			SerialPrint(pageIndex_t7_Char); // Nextion may not get the first packet
			SerialWrite(0x22);
			SerialPrint(newIndexSize, 4);
			SerialPrint(nextionQuoteEnd);

			SerialPrint(pageIndex_t7_Char);
			SerialWrite(0x22);
			SerialPrint(newIndexSize, 4);
			SerialPrint(nextionQuoteEnd);

			break;
		}
	}

	if (!badFilename)
	{
		IndexSpindle(directionSpindle);
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
}


/// <summary>
/// Index Size
/// </summary>
/// <comment>
/// Converts unit size of index to steps
/// </comment>
/// <param name="newIndexSize">Unit size of index</param>
/// <returns></returns>
void IndexSize(double newIndexSize)
{
	const char* indexId_Char = "Index ID:";
	const char* index1_Char = "Index1";
	const char* index2_Char = "Index2";
	const char* index3_Char = "Index3";
	const char* index4_Char = "Index4";
	const char* index5_Char = "Index5";
//	const char* newIndexSize_Char = "New Index Size:";
	const char* indexSizeChar = " Size:";
//	const char* degreeDivision_Char = " Degree or Division:";
	
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
			configIndex_1.sizeInUnits = newIndexSize;

			// Default: Divisions
			if (configIndex_1.degreeOrDivision == BY_DEGREES) // Degrees
			{
				configIndex_1.sizeInSteps = stepsPerRevolution * (newIndexSize / 360);
			}
			else
			{
				configIndex_1.sizeInSteps = stepsPerRevolution / (newIndexSize);
			}
	#ifdef DEBUG
			Serial.print(index1_Char);
			Serial.print(indexSizeChar);
			Serial.println(configIndex_1.sizeInSteps);
	#endif // DEBUG
			EEPROM.put(eePromAddress_Ind_1, configIndex_1);
			break;
		}
		case 2:
		{
			configIndex_2.sizeInUnits = newIndexSize;

			// Default: Divisions
			if (configIndex_2.degreeOrDivision == BY_DEGREES) // Degrees
			{
				configIndex_2.sizeInSteps = stepsPerRevolution * (newIndexSize / 360);
			}
			else
			{
				configIndex_2.sizeInSteps = stepsPerRevolution / (newIndexSize);
			}
	#ifdef DEBUG
			Serial.print(index2_Char);
			Serial.print(indexSizeChar);
			Serial.println(configIndex_2.sizeInSteps);
	#endif // DEBUG
			EEPROM.put(eePromAddress_Ind_2, configIndex_2);
			break;
		}
		case 3:
		{
			configIndex_3.sizeInUnits = newIndexSize;

			// Default: Divisions
			if (configIndex_3.degreeOrDivision == BY_DEGREES) // Degrees
			{
				configIndex_3.sizeInSteps = stepsPerRevolution * (newIndexSize / 360);
			}
			else
			{
				configIndex_3.sizeInSteps = stepsPerRevolution / (newIndexSize);
			}

	#ifdef DEBUG
			Serial.print(index3_Char);
			Serial.print(indexSizeChar);
			Serial.println(configIndex_3.sizeInSteps);
	#endif // DEBUG
			EEPROM.put(eePromAddress_Ind_3, configIndex_3);
			break;
		}
		case 4:
		{
			configIndex_4.sizeInUnits = newIndexSize;

			// Default: Divisions
			if (configIndex_4.degreeOrDivision == BY_DEGREES) // Degrees
			{
				configIndex_4.sizeInSteps = stepsPerRevolution * (newIndexSize / 360);
			}
			else
			{
				configIndex_4.sizeInSteps = stepsPerRevolution / (newIndexSize);
			}

	#ifdef DEBUG
			Serial.print(index4_Char);
			Serial.print(indexSizeChar);
			Serial.println(configIndex_4.sizeInSteps);
	#endif // DEBUG
			EEPROM.put(eePromAddress_Ind_4, configIndex_4);
			break;
		}
		case 5:
		{
			configIndex_5.sizeInUnits = newIndexSize;

			// Default: Divisions
			if (configIndex_5.degreeOrDivision == BY_DEGREES) // Degrees
			{
				configIndex_5.sizeInSteps = stepsPerRevolution * (newIndexSize / 360);
			}
			else
			{
				configIndex_5.sizeInSteps = stepsPerRevolution / (newIndexSize);
			}

	#ifdef DEBUG
			Serial.print("Steps per revolution: ");
			Serial.print(stepsPerRevolution);
			Serial.print("  NewIndexSize: ");
			Serial.print(newIndexSize);
			Serial.print("   ");
			Serial.print(index5_Char);
			Serial.print(indexSizeChar);
			Serial.println(configIndex_5.sizeInSteps);
	#endif // DEBUG
			EEPROM.put(eePromAddress_Ind_5, configIndex_5);
			break;
		}
	}

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
	const char* pageSync_bt3_pco_Char = "pageSync.bt3.pco=0";
	const char* pageSync_bt3_val_Char = "pageSync.bt3.val=0";
	const char* pageSync_va0_val_Char = "pageSync.va0.val=0";
	const char* pageSync_bt1_val_Char = "pageSync.bt1.val=0";
	const char* pageSync_bt2_val_Char = "pageSync.bt2.val=0";

	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	const char* nextionEnd = "\xFF\xFF\xFF";

	int stepPin = 0;
	int dirPin = 0;
	int limitPin_Max = configSetup.limit_Max_Z;
	int limitPin_Min = configSetup.limit_Min_Z;
	int32_t maxSpeed = 0;
	int32_t accel = 0;
	long axisSteps = 0;
	startPositionAbs_Axis = 0;
	limitTriggered = false;

	switch (configSync.axisId)
	{
		case ID_AXIS_Z:
		{
			stepPin = PIN_AXIS_Z_STEP;
			dirPin = PIN_AXIS_Z_DIR;
			limitPin_Max = configSetup.limit_Max_Z;
			limitPin_Min = configSetup.limit_Min_Z;
			maxSpeed = configSync.maxSpd_Axis_Z * configSync.speedPercent_Axis_Z * .01;
			accel = configSync.accel_Axis_Z;
			axisSteps = DistanceToSteps_Axis(configSync.distance, configSync.axisId) * directionAxis;
			break;
		}
		case ID_AXIS_X:
		{
			stepPin = PIN_AXIS_X_STEP;
			dirPin = PIN_AXIS_X_DIR;
			limitPin_Max = configSetup.limit_Max_X;
			limitPin_Min = configSetup.limit_Min_X;
			maxSpeed = configSync.maxSpd_Axis_X * configSync.speedPercent_Axis_X * .01;
			accel = configSync.accel_Axis_X;
			axisSteps = DistanceToSteps_Axis(configSync.distance, configSync.axisId) * directionAxis;
			break;
		}
		case ID_AXIS_B:
		{
			stepPin = PIN_AXIS_B_STEP;
			dirPin = PIN_AXIS_B_DIR;
			limitPin_Max = configSetup.limit_Max_B;
			limitPin_Min = configSetup.limit_Min_B;
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

	// Set limit switches HIGH
	digitalWrite(limitPin_Max, HIGH);
	digitalWrite(limitPin_Min, HIGH);

	StepControl stepController;
	Stepper stepper_Spindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
	Stepper stepper_Axis(stepPin, dirPin);

	float targetSteps_Spindle = 0;
	targetSteps_Spindle = RevolutionsToSteps_Spindle(configSync.revolutions_Spindle, directionSpindle);
	stepper_Spindle
		.setMaxSpeed(configSync.maxSpd_Spindle * configSync.speedPercent_Spindle * .01)
		.setAcceleration(configSync.accel_Spindle)
		.setTargetRel(targetSteps_Spindle);
	////stepper_Spindle.setPosition(0);

	stepper_Axis
		.setMaxSpeed(maxSpeed)
		.setAcceleration(accel)
		.setTargetRel(axisSteps);
	////stepper_Axis.setPosition(0);

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

	SetEnable(ID_SPINDLE, true, true);

	switch (configSync.axisId)
	{
		case ID_AXIS_Z:
		{
			SetEnable(ID_AXIS_Z, true, true);
			break;
		}
		case ID_AXIS_X:
		{
			SetEnable(ID_AXIS_X, true, true);

			break;
		}
		case ID_AXIS_B:
		{
			SetEnable(ID_AXIS_B, true, true);

			break;
		}
	}
#ifdef DEBUG
	Serial.println("MoveAsync..............................................");
#endif // Debug
	stepController.moveAsync(stepper_Spindle, stepper_Axis);

	while (stepController.isRunning())
	{

		///////////////////////////////////////////

		if ((digitalRead(limitPin_Max) != configSetup.limit_NCorNO))// && axisSteps > 0)
		{
			Serial.println("                          limitPin_Max");
			if (axisSteps > 0)
			{

				stepController.emergencyStop();
				stopSteppers = true;
				SetLimitColors(PAGE_SYNC, DIR_CW);
				goto endLoop;
			}
		}
		else if ((digitalRead(limitPin_Min) != configSetup.limit_NCorNO))// && axisSteps < 0)
		{
			Serial.println("                          limitPin_Min");
			if (axisSteps < 0)
			{
				stepController.emergencyStop();
				stopSteppers = true;
				SetLimitColors(PAGE_ONE, DIR_CCW);
				goto endLoop;
			}
		}

		if (configSetup.eStop != 0)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
				Serial.println("                          EStop");
				stepController.emergencyStop();
				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_B, false, false);
				SetEStopColors(PAGE_SYNC);
				return;
			}
		}

#ifdef DEBUG
		endPosition_Spindle = stepper_Spindle.getPosition();
		endPosition_Axis = stepper_Axis.getPosition();
#endif // DEBUG

		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
			case 83: // - S
			{
				stepController.stopAsync();
				stopSteppers = true;
				break;
			}
			}
		}

		MilliDelay(10);
	}

endLoop:

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
		SerialPrint(pageSync_bt3_pco_Char);
		SerialPrint(nextionEnd);

		SerialPrint(pageSync_bt3_val_Char);// The first one may be ignored by Nextion, so resend.
		SerialPrint(nextionEnd);
		if (!limitTriggered)
		{
			SerialPrint(pageSync_va0_val_Char);
			SerialPrint(nextionEnd);
			SerialPrint(pageSync_bt1_val_Char);
			SerialPrint(nextionEnd);
		}
		SerialPrint(pageSync_bt2_val_Char);
		SerialPrint(nextionEnd);

		SetEnable(ID_AXIS_X, false, true);
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
		SerialPrint(pageSync_bt3_pco_Char);
		SerialPrint(nextionEnd);

		SerialPrint(pageSync_bt3_val_Char);
		SerialPrint(nextionEnd);
		SerialPrint(pageSync_bt2_val_Char);
		SerialPrint(nextionEnd);

		if (!limitTriggered)
		{
			SerialPrint(pageSync_va0_val_Char);
			SerialPrint(nextionEnd);
			SerialPrint(pageSync_bt1_val_Char);
			SerialPrint(nextionEnd);
		}



		SetEnable(ID_AXIS_Z, false, true);
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
		SerialPrint(pageSync_bt3_pco_Char);
		SerialPrint(nextionEnd);

		SerialPrint(pageSync_bt3_val_Char);
		SerialPrint(nextionEnd);
		SerialPrint(pageSync_bt2_val_Char);
		SerialPrint(nextionEnd);
		if (!limitTriggered)
		{
			SerialPrint(pageSync_va0_val_Char);
			SerialPrint(nextionEnd);
			SerialPrint(pageSync_bt1_val_Char);
			SerialPrint(nextionEnd);
		}

		SetEnable(ID_AXIS_B, false, true);
		break;
	}
	}

	stepper_Spindle.setPosition(0);
	stepper_Axis.setPosition(0);

	SetEnable(ID_SPINDLE, false, true);
#ifdef DEBUG
	Serial.print(spindle_Char);
	Serial.println(endPosition_Spindle);
	Serial.print(axis_Char);
	Serial.println(endPosition_Axis);
#endif // DEBUG

	// 0 = Change to disabled, 1 = keep enabled
	if (configSetup.keepSteppersEnabled == 1)
	{
		SerialPrint("pageSync.bt10.val=1"); // 0 = enabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageSync.bt11.val=1"); // 0 = enabled
		SerialPrint("\xFF\xFF\xFF");
	}
	else
	{
		SerialPrint("pageSync.bt10.val=0"); // 1 = disabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageSync.bt10.txt="); // 1 = disabled
		SerialPrint("\x22");
		SerialPrint("Disabled");
		SerialPrint("\x22\xFF\xFF\xFF");

		SerialPrint("pageSync.bt11.val=0"); // 1 = disabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageSync.bt11.txt="); // 1 = disabled
		SerialPrint("\x22");
		SerialPrint("Disabled");
		SerialPrint("\x22\xFF\xFF\xFF");
	}
}

/// <summary>
/// Reciprocate
/// </summary>
/// <comment>
/// </comment>
/// <param name="wavDir">In: -1, Out: 1</param>
/// <returns></returns>
void Reciprocate_Triangle(int wavDir)
{
	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	const char* nextionEnd = "\xFF\xFF\xFF";
	int stepPin = 0;
	int dirPin = 0;
	int limitPin_Max = configSetup.limit_Max_Z;
	int limitPin_Min = configSetup.limit_Min_Z;
	int32_t maxSpeed = 0;
	int32_t accel = 0;
	int32_t waves = 0;
	int32_t speedPercent = 0;
	startPositionAbs_Axis = 0;
	limitTriggered = false;

	switch (configRec.axisId)
	{
		case ID_AXIS_Z:
		{
			stepPin = PIN_AXIS_Z_STEP;
			dirPin = PIN_AXIS_Z_DIR;
			limitPin_Max = configSetup.limit_Max_Z;
			limitPin_Min = configSetup.limit_Min_Z;
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
			limitPin_Max = configSetup.limit_Max_X;
			limitPin_Min = configSetup.limit_Min_X;
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
			limitPin_Max = configSetup.limit_Max_B;
			limitPin_Min = configSetup.limit_Min_B;
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

	// Set limit switches HIGH
	digitalWrite(limitPin_Max, HIGH);
	digitalWrite(limitPin_Min, HIGH);

	StepControl stepController;
	Stepper stepper_Spindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
	Stepper stepper_Axis(stepPin, dirPin);

	// Set speed and acceleration
	stepper_Spindle
		.setMaxSpeed(configRec.maxSpd_Spindle * configRec.speedPercent_Spindle * .01)
		.setAcceleration(configRec.accel_Spindle);
	////stepper_Spindle.setPosition(0);

	int spindleSteps = 0;
	long axisSteps = 0;
	switch (configRec.radial_axial)
	{
		case RADIAL: // Radial
		{ 
			waves = configRec.waves_Radial;
			float spindleDegrees = configRec.degrees_Radial_Spindle / (configRec.waves_Radial * 2);
			spindleSteps = round((configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (spindleDegrees / 360) * wavDir);
			Serial.print("configRec.degrees_Radial_Spindle:");
			Serial.println(configRec.degrees_Radial_Spindle);
			Serial.print("configRec.waves_Radial*2:");
			Serial.println(configRec.waves_Radial * 2);
			Serial.print("configSetup.microsteps_Spindle:");
			Serial.println(configSetup.microsteps_Spindle);
			Serial.print("configSetup.steps360_Spindle:");
			Serial.println(configSetup.steps360_Spindle);
			Serial.print("configSetup.gearRatio_Spindle:");
			Serial.println(configSetup.gearRatio_Spindle);
			Serial.print("spindleDegrees / 360:");
			Serial.println(spindleDegrees / 360);
			Serial.print("spindleSteps:");
			Serial.println(spindleSteps);
			////axisSteps = (DistanceToSteps_Axis(configRec.amplitude_Radial_Axis, configRec.axisId) / 2) * wavDir;
			axisSteps = (DistanceToSteps_Axis(configRec.amplitude_Radial_Axis, configRec.axisId) ) * wavDir;
			break;
		}
		case AXIAL: // Axial
		{
			waves = configRec.waves_Axial;
			spindleSteps = (configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (configRec.amplitude_Axial_Spindle / 360) * wavDir;
			axisSteps = (DistanceToSteps_Axis(configRec.distance_Axial_Axis, configRec.axisId) / (configRec.waves_Axial * 2))* wavDir;
			break;
		}
	}

#ifdef DEBUG
	Serial.println("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
	Serial.print("spindleSteps:");
	Serial.println(spindleSteps);
	Serial.print("endPosition_Axis:");
	Serial.println(endPosition_Axis);
	Serial.print("axisSteps:");
	Serial.println(axisSteps);
	Serial.println("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
#endif // DEBUG

	stepper_Axis
		.setMaxSpeed(maxSpeed)
		.setAcceleration(accel);
	////stepper_Axis.setPosition(0);

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

	SetEnable(ID_SPINDLE, true, true);

	switch (configRec.axisId)
	{
		case ID_AXIS_Z:
		{
			SetEnable(ID_AXIS_Z, true, true);
			break;
		}
		case ID_AXIS_X:
		{
			SetEnable(ID_AXIS_X, true, true);
			break;
		}
		case ID_AXIS_B:
		{
			SetEnable(ID_AXIS_B, true, true);
			Serial.println("B Enabled..........................");
			break;
		}
	}
	
	for (int i = 0; i < (waves * 2); i++)
	{

		if(i>=1)
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
			///////////////////////////////////////////

			if ((digitalRead(limitPin_Max) != configSetup.limit_NCorNO))// && axisSteps > 0)
			{
				Serial.println("                          limitPin_Max");
				if(axisSteps>0)
				{ 

					stepController.emergencyStop();
					stopSteppers = true;
					limitTriggered = true;
					SetLimitColors(PAGE_REC, DIR_CW);
					goto endLoop;
				}
			}
			else if ((digitalRead(limitPin_Min) != configSetup.limit_NCorNO))// && axisSteps < 0)
			{
				Serial.println("                          limitPin_Min");
				if (axisSteps < 0)
				{
					stepController.emergencyStop();
					stopSteppers = true;
					limitTriggered = true;
					SetLimitColors(PAGE_REC, DIR_CCW);
					goto endLoop;
				}
			}

			if (configSetup.eStop != 0)
			{
				if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
				{
					Serial.println("                          EStop ");
					stepController.emergencyStop();
					stopSteppers = true;
					// Disable all motors
					SetEnable(ID_SPINDLE, false, false);
					SetEnable(ID_AXIS_Z, false, false);
					SetEnable(ID_AXIS_X, false, false);
					SetEnable(ID_AXIS_B, false, false);
					SetEStopColors(PAGE_REC);
					return;
				}
			}

			// Check for Cancel code  
			if (SerialAvailable() > 0)
			{
				incomingByte = SerialRead(serialId);
				switch (incomingByte)
				{
					case 83: // - S
					{
#ifdef DEBUG
			Serial.println("STOP++++++++++++++++++++++");
#endif // DEBUG
						stepController.stop();
						stopSteppers = true;
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
	if (!limitTriggered)
	{
		SerialPrint("pageRec.va0.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageRec.bt1.val=0");
		SerialPrint(nextionEnd);
	}
	SerialPrint("pageRec.bt3.val=0");// The first one may be ignored by Nextion, so resend.
	SerialPrint(nextionEnd);
	SerialPrint("pageRec.bt2.val=0");
	SerialPrint(nextionEnd);
	//SerialPrint("pageRec.bt1.val=0");
	//SerialPrint(nextionEnd);

#ifdef DEBUG
	//endPosition_Axis = stepper_Axis.getPosition();
	//endPosition_Axis = endPosition_Axis + stepper_Axis.getPosition();
	//endPosition_Spindle = stepper_Spindle.getPosition();
	//endPosition_Spindle = endPosition_Spindle + stepper_Spindle.getPosition();

	Serial.print("Spindle:");
	Serial.println(endPosition_Spindle);
	Serial.print("Axis:");
	Serial.println(endPosition_Axis);
#endif // DEBUG
	returnSteps_Spindle = endPosition_Spindle;
	SetEnable(ID_SPINDLE, false, true);
	SetEnable(ID_AXIS_Z, false, true);
	SetEnable(ID_AXIS_X, false, true);
	SetEnable(ID_AXIS_B, false, true);

	// 0 = Change to disabled, 1 = keep enabled
	if (configSetup.keepSteppersEnabled == 1)
	{
		SerialPrint("pageRec.bt10.val=1"); // 0 = enabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageRec.bt11.val=1"); // 0 = enabled
		SerialPrint("\xFF\xFF\xFF");
	}
	else
	{
		SerialPrint("pageRec.bt10.val=0"); // 1 = disabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageRec.bt10.txt="); // 1 = disabled
		SerialPrint("\x22");
		SerialPrint("Disabled");
		SerialPrint("\x22\xFF\xFF\xFF");

		SerialPrint("pageRec.bt11.val=0"); // 1 = disabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageRec.bt11.txt="); // 1 = disabled
		SerialPrint("\x22");
		SerialPrint("Disabled");
		SerialPrint("\x22\xFF\xFF\xFF");
	}
}

void Reciprocate_Sawtooth(int wavDir)
{
	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	const char* nextionEnd = "\xFF\xFF\xFF";
	int limitPin_Max = configSetup.limit_Max_Z;
	int limitPin_Min = configSetup.limit_Min_Z;
	int stepPin = 0;
	int dirPin = 0;
	int32_t maxSpeed = 0;
	int32_t accel = 0;
	int32_t waves = 0;
	int32_t speedPercent = 0;
	startPositionAbs_Axis = 0;
	limitTriggered = false;

	switch (configRec.axisId)
	{
		case ID_AXIS_Z:
		{
			stepPin = PIN_AXIS_Z_STEP;
			dirPin = PIN_AXIS_Z_DIR;
			limitPin_Max = configSetup.limit_Max_Z;
			limitPin_Min = configSetup.limit_Min_Z;
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
			limitPin_Max = configSetup.limit_Max_X;
			limitPin_Min = configSetup.limit_Min_X;
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
			limitPin_Max = configSetup.limit_Max_B;
			limitPin_Min = configSetup.limit_Min_B;
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

	// Set limit switches HIGH
	digitalWrite(limitPin_Max, HIGH);
	digitalWrite(limitPin_Min, HIGH);
	StepControl stepController;
	Stepper stepper_Spindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
	Stepper stepper_Axis(stepPin, dirPin);

	// Set speed and acceleration
	stepper_Spindle
		.setMaxSpeed(configRec.maxSpd_Spindle * configRec.speedPercent_Spindle * .01)
		.setAcceleration(configRec.accel_Spindle);
	stepper_Spindle.setPosition(0);

	int32_t spindleSteps = 0;
	int32_t axisSteps = 0;
	switch (configRec.radial_axial)
	{
	case RADIAL: // Radial
	{
		waves = configRec.waves_Radial;
		float spindleDegrees = configRec.degrees_Radial_Spindle / (configRec.waves_Radial * 2);
		spindleSteps = round((configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (spindleDegrees / 360) * wavDir);


		Serial.print("configRec.degrees_Radial_Spindle:");
		Serial.println(configRec.degrees_Radial_Spindle);
		Serial.print("configRec.waves_Radial*2:");
		Serial.println(configRec.waves_Radial*2);
		Serial.print("configSetup.microsteps_Spindle:");
		Serial.println(configSetup.microsteps_Spindle);
		Serial.print("configSetup.steps360_Spindle:");
		Serial.println(configSetup.steps360_Spindle);
		Serial.print("configSetup.gearRatio_Spindle:");
		Serial.println(configSetup.gearRatio_Spindle);
		Serial.print("spindleDegrees / 360:");
		Serial.println(spindleDegrees / 360);
		Serial.print("spindleSteps:");
		Serial.println(spindleSteps);
		////axisSteps = (DistanceToSteps_Axis(configRec.amplitude_Radial_Axis, configRec.axisId) / 2) * wavDir;
		axisSteps = (DistanceToSteps_Axis(configRec.amplitude_Radial_Axis, configRec.axisId)) * wavDir;
		break;
	}
	case AXIAL: // Axial
	{
		waves = configRec.waves_Axial;
		spindleSteps = (configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (configRec.amplitude_Axial_Spindle / 360) * wavDir;
		axisSteps = (DistanceToSteps_Axis(configRec.distance_Axial_Axis, configRec.axisId) / (configRec.waves_Axial * 2)) * wavDir;
		break;
	}
	}

#ifdef DEBUG
	Serial.println("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
	Serial.print("spindleSteps:");
	Serial.println(spindleSteps);
	Serial.print("endPosition_Axis:");
	Serial.println(endPosition_Axis);
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

	SetEnable(ID_SPINDLE, true, true);

	switch (configRec.axisId)
	{
	case ID_AXIS_Z:
	{
		SetEnable(ID_AXIS_Z, true, true);
		break;
	}
	case ID_AXIS_X:
	{
		SetEnable(ID_AXIS_X, true, true);
		break;
	}
	case ID_AXIS_B:
	{
		SetEnable(ID_AXIS_B, true, true);
		Serial.println("B Enabled..........................");
		break;
	}
	}
//	int32_t axisSteps_0 = 0;
//	int32_t spindleSteps_0 = 0;
	int32_t i_mod_2 = 0;
	for (int i = 1; i <= (waves * 2) + 2; i++)
	{
		i_mod_2 = i % 2;
		Serial.print("i: ");
		Serial.println(i);
		Serial.print("i%2: ");
		Serial.println(i % 2);

		switch(i_mod_2)
		{ 
			case 0:
			{
				switch (configRec.radial_axial)
				{
					case RADIAL: // Radial
					{
						//stepper_Spindle.setTargetRel(spindleSteps);
						stepper_Axis.setTargetRel(-axisSteps);
						stepController.moveAsync(stepper_Axis);
						MilliDelay(5);
#ifdef DEBUG
						Serial.print("Axis i: ");
						Serial.println(i);
						Serial.println(i_mod_2);
						Serial.print("axisSteps:");
						Serial.println(axisSteps);
#endif // DEBUG
						break;
					}
					case AXIAL:
					{
						stepper_Spindle.setTargetRel(-spindleSteps);
						//stepper_Axis.setTargetRel(axisSteps);

						stepController.moveAsync(stepper_Spindle);
						MilliDelay(5);
#ifdef DEBUG
						Serial.println("");
						Serial.print("spindleSteps:");
						Serial.println(spindleSteps);
#endif // DEBUG
						break;
					}

				}
				break;
			}
			default:
			{
				// Start pattern
				switch (configRec.radial_axial)
				{
					case RADIAL: // Radial
					{
						stepper_Spindle.setTargetRel(spindleSteps);
						stepper_Axis.setTargetRel(axisSteps);
						break;
					}
					case AXIAL:
					{
						stepper_Spindle.setTargetRel(spindleSteps);
						stepper_Axis.setTargetRel(axisSteps);
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
				stepController.moveAsync(stepper_Spindle, stepper_Axis);
				MilliDelay(5);
				Serial.print("Spindle&Axis i: ");
				Serial.println(i);
				Serial.println(i_mod_2);
				break;
			}
		}

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

						///////////////////////////////////////////

			if ((digitalRead(limitPin_Max) != configSetup.limit_NCorNO))// && axisSteps > 0)
			{
				Serial.println("                          limitPin_Max");
				if (axisSteps > 0)
				{
					stepController.emergencyStop();
					stopSteppers = true;
					limitTriggered = true;
					SetLimitColors(PAGE_REC, DIR_CW);
					goto endLoop;
				}
			}
			else if ((digitalRead(limitPin_Min) != configSetup.limit_NCorNO))// && axisSteps < 0)
			{
				Serial.println("                          limitPin_Min");
				if (axisSteps < 0)
				{
					stepController.emergencyStop();
					stopSteppers = true;
					limitTriggered = true;
					SetLimitColors(PAGE_REC, DIR_CCW);
					goto endLoop;
				}
			}

			if (configSetup.eStop != 0)
			{
				if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
				{
					Serial.println("                          EStop");
					stepController.emergencyStop();
					stopSteppers = true;
					// Disable all motors
					SetEnable(ID_SPINDLE, false, false);
					SetEnable(ID_AXIS_Z, false, false);
					SetEnable(ID_AXIS_X, false, false);
					SetEnable(ID_AXIS_B, false, false);
					SetEStopColors(PAGE_REC);
					return;
				}
			}


			// Check for Cancel code  
			if (SerialAvailable() > 0)
			{
				incomingByte = SerialRead(serialId);
				switch (incomingByte)
				{
				case 83: // - S
				{
#ifdef DEBUG
					Serial.println("STOP++++++++++++++++++++++");
#endif // DEBUG
					stepController.stop();
					stopSteppers = true;
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
	if (!limitTriggered)
	{ 
		SerialPrint("pageRec.va0.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageRec.bt1.val=0");
		SerialPrint(nextionEnd);
	}
	SerialPrint("pageRec.bt3.val=0");// The first one may be ignored by Nextion, so resend.
	SerialPrint(nextionEnd);
	SerialPrint("pageRec.bt2.val=0");
	SerialPrint(nextionEnd);
	Serial.print("Limit triggered: ");
	Serial.println(limitTriggered);

#ifdef DEBUG
	/*endPosition_Axis = stepper_Axis.getPosition();
	endPosition_Spindle = stepper_Spindle.getPosition();*/
	Serial.print("Spindle:");
	Serial.println(endPosition_Spindle);
	Serial.print("Axis:");
	Serial.println(endPosition_Axis);
#endif // DEBUG
	returnSteps_Spindle = endPosition_Spindle;
	SetEnable(ID_SPINDLE, false, true);
	SetEnable(ID_AXIS_Z, false, true);
	SetEnable(ID_AXIS_X, false, true);
	SetEnable(ID_AXIS_B, false, true);

	// 0 = Change to disabled, 1 = keep enabled
	if (configSetup.keepSteppersEnabled == 1)
	{
		SerialPrint("pageRec.bt10.val=1"); // 0 = enabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageRec.bt11.val=1"); // 0 = enabled
		SerialPrint("\xFF\xFF\xFF");
	}
	else
	{
		SerialPrint("pageRec.bt10.val=0"); // 1 = disabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageRec.bt10.txt="); // 1 = disabled
		SerialPrint("\x22");
		SerialPrint("Disabled");
		SerialPrint("\x22\xFF\xFF\xFF");

		SerialPrint("pageRec.bt11.val=0"); // 1 = disabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageRec.bt11.txt="); // 1 = disabled
		SerialPrint("\x22");
		SerialPrint("Disabled");
		SerialPrint("\x22\xFF\xFF\xFF");
	}
}

void Reciprocate_Square(int wavDir)
{
	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	const char* nextionEnd = "\xFF\xFF\xFF";
	int stepPin = 0;
	int dirPin = 0;
	int limitPin_Max = configSetup.limit_Max_Z;
	int limitPin_Min = configSetup.limit_Min_Z;
	int32_t maxSpeed = 0;
	int32_t accel = 0;
	int32_t waves = 0;
	int32_t speedPercent = 0;
	startPositionAbs_Axis = 0;
	limitTriggered = false;

	switch (configRec.axisId)
	{
	case ID_AXIS_Z:
	{
		stepPin = PIN_AXIS_Z_STEP;
		dirPin = PIN_AXIS_Z_DIR;
		limitPin_Max = configSetup.limit_Max_Z;
		limitPin_Min = configSetup.limit_Min_Z;
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
		limitPin_Max = configSetup.limit_Max_X;
		limitPin_Min = configSetup.limit_Min_X;
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
		limitPin_Max = configSetup.limit_Max_B;
		limitPin_Min = configSetup.limit_Min_B;
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

	// Set limit switches HIGH
	digitalWrite(limitPin_Max, HIGH);
	digitalWrite(limitPin_Min, HIGH);

	StepControl stepController;
	Stepper stepper_Spindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
	Stepper stepper_Axis(stepPin, dirPin);

	// Set speed and acceleration
	stepper_Spindle
		.setMaxSpeed(configRec.maxSpd_Spindle * configRec.speedPercent_Spindle * .01)
		.setAcceleration(configRec.accel_Spindle);
	stepper_Spindle.setPosition(0);

	int32_t spindleSteps = 0;
	int32_t axisSteps = 0;
	switch (configRec.radial_axial)
	{
	case RADIAL: // Radial
	{
		waves = configRec.waves_Radial;
		float spindleDegrees = configRec.degrees_Radial_Spindle / (configRec.waves_Radial * 2);
		spindleSteps = round((configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (spindleDegrees / 360) * wavDir);
		////axisSteps = (DistanceToSteps_Axis(configRec.amplitude_Radial_Axis, configRec.axisId) / 2) * wavDir;
		axisSteps = (DistanceToSteps_Axis(configRec.amplitude_Radial_Axis, configRec.axisId)) * wavDir;
		break;
	}
	case AXIAL: // Axial
	{
		waves = configRec.waves_Axial;
		spindleSteps = (configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (configRec.amplitude_Axial_Spindle / 360) * wavDir;
		axisSteps = (DistanceToSteps_Axis(configRec.distance_Axial_Axis, configRec.axisId) / (configRec.waves_Axial * 2)) * wavDir;
		break;
	}
	}

#ifdef DEBUG
	Serial.println("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
	Serial.print("spindleSteps:");
	Serial.println(spindleSteps);
	Serial.print("endPosition_Axis:");
	Serial.println(endPosition_Axis);
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

	SetEnable(ID_SPINDLE, true, true);

	switch (configRec.axisId)
	{
	case ID_AXIS_Z:
	{
		SetEnable(ID_AXIS_Z, true, true);
		break;
	}
	case ID_AXIS_X:
	{
		SetEnable(ID_AXIS_X, true, true);
		break;
	}
	case ID_AXIS_B:
	{
		SetEnable(ID_AXIS_B, true, true);
		Serial.println("B Enabled..........................");
		break;
	}
	}
//	int32_t axisSteps_0 = 0;
//	int32_t spindleSteps_0 = 0;
	int32_t leg = 1;
	for (int i = 4; i < ((waves + 4) * 2); i++)
	{
		leg = i % 4;
		Serial.print("leg: ");
		Serial.println(leg);
		switch (leg)
		{
			case 0:
			{
				switch (configRec.radial_axial)
				{
					case RADIAL: // Radial
					{
						
						stepper_Axis.setTargetRel(axisSteps);
						stepController.moveAsync(stepper_Axis);
						MilliDelay(5);
						break;
					}
					case AXIAL:
					{
						stepper_Spindle.setTargetRel(spindleSteps);
						//stepper_Axis.setTargetRel(axisSteps);

						stepController.moveAsync(stepper_Spindle);
						MilliDelay(5);
						break;
					}
				}
				break;
			}
			case 1:
			case 3:
			{
				switch (configRec.radial_axial)
				{
					case RADIAL: // Radial
					{
						stepper_Spindle.setTargetRel(spindleSteps);
						stepController.moveAsync(stepper_Spindle);
						MilliDelay(5);
						break;
					}
					case AXIAL:
					{
						stepper_Axis.setTargetRel(axisSteps);

						stepController.moveAsync(stepper_Axis);
						MilliDelay(5);
						break;
					}
				}
				break;
			}
			case 2:
			{
				switch (configRec.radial_axial)
				{
					case RADIAL: // Radial
					{
						stepper_Axis.setTargetRel(axisSteps * (-1));
						stepController.moveAsync(stepper_Axis);
						MilliDelay(5);
						break;
					}
					case AXIAL:
					{
						stepper_Spindle.setTargetRel(spindleSteps * (-1));
						stepController.moveAsync(stepper_Spindle);
						MilliDelay(5);
						break;
					}
				}
				break;
			}
		}



#ifdef DEBUG
		//Serial.println("");
		//Serial.print("spindleSteps:");
		//Serial.println(spindleSteps);
		//Serial.print("axisSteps:");
		//Serial.println(axisSteps);
#endif // DEBUG

		////stepController.moveAsync(stepper_Spindle, stepper_Axis);
		////MilliDelay(5);

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

						///////////////////////////////////////////

			if ((digitalRead(limitPin_Max) != configSetup.limit_NCorNO))// && axisSteps > 0)
			{
				Serial.println("                          limitPin_Max");
				if (axisSteps > 0)
				{

					stepController.emergencyStop();
					stopSteppers = true;
					limitTriggered = true;
					SetLimitColors(PAGE_REC, DIR_CW);
					goto endLoop;
				}
			}
			else if ((digitalRead(limitPin_Min) != configSetup.limit_NCorNO))// && axisSteps < 0)
			{
				Serial.println("                          limitPin_Min");
				if (axisSteps < 0)
				{
					stepController.emergencyStop();
					stopSteppers = true;
					limitTriggered = true;
					SetLimitColors(PAGE_REC, DIR_CCW);
					goto endLoop;
				}
			}

			if (configSetup.eStop != 0)
			{
				if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
				{
					Serial.println("                          EStop ");
					stepController.emergencyStop();
					stopSteppers = true;
					// Disable all motors
					SetEnable(ID_SPINDLE, false, false);
					SetEnable(ID_AXIS_Z, false, false);
					SetEnable(ID_AXIS_X, false, false);
					SetEnable(ID_AXIS_B, false, false);
					SetEStopColors(PAGE_REC);
					return;
				}
			}

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
				case 83: // - S
				{
#ifdef DEBUG
					Serial.println("STOP++++++++++++++++++++++");
#endif // DEBUG
					stepController.stop();
					stopSteppers = true;
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
	if (!limitTriggered)
	{
		SerialPrint("pageRec.va0.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageRec.bt1.val=0");
		SerialPrint(nextionEnd);
	}
	SerialPrint("pageRec.bt3.val=0");// The first one may be ignored by Nextion, so resend.
	SerialPrint(nextionEnd);
	SerialPrint("pageRec.bt2.val=0");
	SerialPrint(nextionEnd);


#ifdef DEBUG
	//endPosition_Axis = stepper_Axis.getPosition();
	//endPosition_Spindle = stepper_Spindle.getPosition();
	Serial.print("Spindle:");
	Serial.println(endPosition_Spindle);
	Serial.print("Axis:");
	Serial.println(endPosition_Axis);
#endif // DEBUG
	returnSteps_Spindle = endPosition_Spindle;
	SetEnable(ID_SPINDLE, false, true);
	SetEnable(ID_AXIS_Z, false, true);
	SetEnable(ID_AXIS_X, false, true);
	SetEnable(ID_AXIS_B, false, true);

	// 0 = Change to disabled, 1 = keep enabled
	if (configSetup.keepSteppersEnabled == 1)
	{
		SerialPrint("pageRec.bt10.val=1"); // 0 = enabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageRec.bt11.val=1"); // 0 = enabled
		SerialPrint("\xFF\xFF\xFF");
	}
	else
	{
		SerialPrint("pageRec.bt10.val=0"); // 1 = disabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageRec.bt10.txt="); // 1 = disabled
		SerialPrint("\x22");
		SerialPrint("Disabled");
		SerialPrint("\x22\xFF\xFF\xFF");

		SerialPrint("pageRec.bt11.val=0"); // 1 = disabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageRec.bt11.txt="); // 1 = disabled
		SerialPrint("\x22");
		SerialPrint("Disabled");
		SerialPrint("\x22\xFF\xFF\xFF");
	}
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
			case 83: // - S
			{
				retVal = true;
				SetEnable(ID_SPINDLE, false, true);
				switch (configGreekKey.axisId)
				{
					case ID_AXIS_Z: // Z Axis
					{
						SetEnable(ID_AXIS_X, false, true);
					}
					case ID_AXIS_X: // X Axis
					{
						SetEnable(ID_AXIS_X, false, true);
					}
					case ID_AXIS_B: // B Axis
					{
						SetEnable(ID_AXIS_B, false, true);
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
	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	int totalSpindleSegments = 8;
	limitTriggered = false;
	eStopTriggered = false;
	if (configGreekKey.radialOrAxial_Pattern == AXIAL)
	{
		totalSpindleSegments = 4;
	}
	GreekKeyPattern_Initial(totalSpindleSegments);

	for (int i = 1; i <= configGreekKey.patternCount_Pattern;i++)// / 2; i++)
	{
		if (eStopTriggered || limitTriggered)
		{
			Serial.println("GreekKey 4a eStop or limit triggered.......................");
			return;
		}
		SerialPrint("t2.txt=");
		SerialWrite(0x22);
		SerialPrint(i);
		SerialPrint(nextionQuoteEnd);
		switch (configGreekKey.radialOrAxial_Pattern)
		{
			case RADIAL: // Axis Left CCW
			{
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, reverseDirection * DIR_CW, true); //1 
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, reverseDirection * DIR_CCW); //2
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CCW, true); //3
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CW); //4
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CCW, true); //5
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, reverseDirection * DIR_CCW); //6
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, reverseDirection * DIR_CW, true); // 7
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 4, reverseDirection * DIR_CW); //8
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 8, reverseDirection * DIR_CCW); //9
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, reverseDirection * DIR_CCW, true); //10
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, reverseDirection * DIR_CW); //11
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CW, true); //12
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CCW); //13
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CW, true); // 14
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, reverseDirection * DIR_CW); //15
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, reverseDirection * DIR_CCW, true); //16
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 4, reverseDirection * DIR_CW); //17
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 8, reverseDirection * DIR_CCW); //18
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				break;
			}
			case AXIAL: // Spindle Up CCW
			{
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, reverseDirection * DIR_CW); //1
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, reverseDirection * DIR_CCW, true); //2
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CCW); //3
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CW, true); //4
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CCW); //5
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, reverseDirection * DIR_CCW, true); //6
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, reverseDirection * DIR_CW); //7
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 4, reverseDirection * DIR_CW, true); //7
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 8, reverseDirection * DIR_CCW, true); //9
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, reverseDirection * DIR_CCW); //10
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, reverseDirection * DIR_CW, true); //11
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CW); //12
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CCW, true); //13
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CW); //14
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, reverseDirection * DIR_CW, true); //15
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, reverseDirection * DIR_CCW); //16
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 4, reverseDirection * DIR_CW, true); //17
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 8, reverseDirection * DIR_CCW, true); //18
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
	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	int totalSpindleSegments = 13;
	limitTriggered = false;
	eStopTriggered = false;
	if (configGreekKey.radialOrAxial_Pattern == AXIAL)
	{
		totalSpindleSegments = 4;
	}
	GreekKeyPattern_Initial(totalSpindleSegments);

	for (int i = 1; i <= configGreekKey.patternCount_Pattern; i++)
	{
		if (eStopTriggered || limitTriggered)
		{
			Serial.println("GreekKey 4b eStop or limit triggered.......................");
			return;
		}
		SerialPrint("t2.txt=");
		SerialWrite(0x22);
		SerialPrint(i);
		SerialPrint(nextionQuoteEnd);
		switch (configGreekKey.radialOrAxial_Pattern)
		{
			case RADIAL: // Axis Left CCW
			{
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CCW); //1-U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, reverseDirection * DIR_CW, true); //2-L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, reverseDirection * DIR_CCW); //3 U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CCW, true); //4 R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CW); //5 D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CCW, true); //6 R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, reverseDirection * DIR_CCW); //7 U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CW, true); // 8 L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CW); //9 D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CW, true); //10 L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, reverseDirection * DIR_CCW); //11 U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, reverseDirection * DIR_CCW, true); //12 R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 6, reverseDirection * DIR_CW); //13 D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 4, reverseDirection * DIR_CW, true); //14 L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 7, reverseDirection * DIR_CCW); //15 U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, reverseDirection * DIR_CCW, true); //16 R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, reverseDirection * DIR_CCW); //17 U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CW, true); //18 L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CW); //19 D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CW, true); //20
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, reverseDirection * DIR_CCW); //21
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CCW, true); //22
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CW); //23
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CCW, true); //24
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, reverseDirection * DIR_CCW); //25
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, reverseDirection * DIR_CW, true); //26
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 5, reverseDirection * DIR_CW); //27
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 6, reverseDirection * DIR_CCW); //28
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 4, reverseDirection * DIR_CCW, true); //29
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 7, reverseDirection * DIR_CW); //30
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 7, reverseDirection * DIR_CCW); //31
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
			
				}
				break;
			}
			case AXIAL: // Spindle Up CCW
			{

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CCW, true); //1-U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, reverseDirection * DIR_CW); //2-L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, reverseDirection * DIR_CCW, true); //3 U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CCW); //4 R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CW, true); //5 D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CCW); //6 R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, reverseDirection * DIR_CCW, true); //7 U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CW); // 8 L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CW, true); //9 D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CW); //10 L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, reverseDirection * DIR_CCW, true); //11 U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, reverseDirection * DIR_CCW); //12 R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 6, reverseDirection * DIR_CW, true); //13 D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 4, reverseDirection * DIR_CW); //14 L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 7, reverseDirection * DIR_CCW, true); //15 U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, reverseDirection * DIR_CCW); //16 R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, reverseDirection * DIR_CCW, true); //17 U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CW); //18 L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CW, true); //19 D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CW); //20
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, reverseDirection * DIR_CCW, true); //21
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CCW); //22
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CW, true); //23
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CCW); //24
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, reverseDirection * DIR_CCW, true); //25
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, reverseDirection * DIR_CW); //26
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 5, reverseDirection * DIR_CW, true); //27
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 6, reverseDirection * DIR_CCW, true); //28
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 4, reverseDirection * DIR_CCW); //29
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 7, reverseDirection * DIR_CW, true); //30
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 7, reverseDirection * DIR_CCW, true); //31
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
	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	int totalSpindleSegments = 4;
	limitTriggered = false;
	eStopTriggered = false;
	if (configGreekKey.radialOrAxial_Pattern == AXIAL)
	{
		totalSpindleSegments = 3;
	}
	GreekKeyPattern_Initial(totalSpindleSegments);

	for (int i = 1; i <= configGreekKey.patternCount_Pattern; i++)
	{
		if (eStopTriggered || limitTriggered)
		{
			Serial.println("GreekKey 3a eStop or limit triggered.......................");
			return;
		}
		SerialPrint("t2.txt=");
		SerialWrite(0x22);
		SerialPrint(i);
		SerialPrint(nextionQuoteEnd);
		switch (configGreekKey.radialOrAxial_Pattern)
		{
			case RADIAL: // Axis Left CCW
			{
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, reverseDirection * DIR_CW, true); //1
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, reverseDirection * DIR_CCW); //2
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, reverseDirection * DIR_CCW, true); //3
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CW); //4
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CW, true); //5
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CW); //6
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, reverseDirection * DIR_CCW, true); //7
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, reverseDirection * DIR_CCW); //8
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				break;
			}
			case AXIAL: // Spindle Up CCW
			{
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, reverseDirection * DIR_CW); //1
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, reverseDirection * DIR_CCW, true); //2
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, reverseDirection * DIR_CCW); //3
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CW, true); //4
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CW); //5
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CW, true); //6
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, reverseDirection * DIR_CCW); //7
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, reverseDirection * DIR_CCW, true); //8
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
/// GreekKey_Pattern_3b
/// </summary>
/// <comment>
/// </comment>
/// <returns></returns>
void GreekKey_Pattern_3b()
{
	bool stopAll = false;
	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	int totalSpindleSegments = 8;
	limitTriggered = false;
	eStopTriggered = false;
	if (configGreekKey.radialOrAxial_Pattern == AXIAL)
	{
		totalSpindleSegments = 3;
	}
	GreekKeyPattern_Initial(totalSpindleSegments);

	for (int i = 1; i <= configGreekKey.patternCount_Pattern; i++)
	{
		if (eStopTriggered || limitTriggered)
		{
			Serial.println("GreekKey 3b eStop or limit triggered.......................");
			return;
		}
		SerialPrint("t2.txt=");
		SerialWrite(0x22);
		SerialPrint(i);
		SerialPrint(nextionQuoteEnd);
		switch (configGreekKey.radialOrAxial_Pattern)
		{
			case RADIAL: // Axis Left CCW
			{
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, reverseDirection * DIR_CW, true); //1  3R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, reverseDirection * DIR_CCW); //2 3U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, reverseDirection * DIR_CCW, true); //3 2L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CW); //4 1D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CW, true); //5 1R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CW); //6 1D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, reverseDirection * DIR_CCW, true); //7 2L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 5, reverseDirection * DIR_CCW); //8 5U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, reverseDirection * DIR_CW, true); //9 2R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CW); //10 1D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CCW, true); //11 1L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CW); //12 1D
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, reverseDirection * DIR_CW, true); //13 2R
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, reverseDirection * DIR_CCW); //14 3U
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, reverseDirection * DIR_CCW, true); //15 3L
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CCW); //16 1U

				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				break;
			}
			case AXIAL: // Spindle Up CCW
			{

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, reverseDirection * DIR_CW); //1
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
	
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, reverseDirection * DIR_CCW, true); //2
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, reverseDirection * DIR_CCW); //3
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CW, true); //4
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CW); //5
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CW, true); //6
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, reverseDirection * DIR_CCW); //7
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 5, reverseDirection * DIR_CCW, true); //8
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, reverseDirection * DIR_CW); //9
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CW, true); //10
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CCW); //11
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CW, true); //12
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, reverseDirection * DIR_CW); //13
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, reverseDirection * DIR_CCW, true); //14
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, reverseDirection * DIR_CCW); //15
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CCW, true); //16
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
	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	int totalSpindleSegments = 3;
	limitTriggered = false;
	eStopTriggered = false;
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
		if (eStopTriggered || limitTriggered)
		{
			Serial.println("GreekKey 2a eStop or limit triggered.......................");
			return;
		}
		SerialPrint("t2.txt=");
		SerialWrite(0x22);
		SerialPrint(i);
		SerialPrint(nextionQuoteEnd);
		switch (configGreekKey.radialOrAxial_Pattern)
		{
			case RADIAL: // Axis Left CCW
			{

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, reverseDirection * DIR_CW, true); //1
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, reverseDirection * DIR_CCW); //2
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CCW, true); //3
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CW); //4
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CCW, true); //5
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, reverseDirection * DIR_CCW); //6
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

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, reverseDirection * DIR_CW); //1
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
			
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, reverseDirection * DIR_CCW, true); //2
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
			
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CCW); //3
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
			
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CCW, true); //4
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
			
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CCW); //5
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
			
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, reverseDirection * DIR_CCW, true); //6
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
	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	int totalSpindleSegments = 6;
	limitTriggered = false;
	eStopTriggered = false;
	if (configGreekKey.radialOrAxial_Pattern == AXIAL)
	{
		totalSpindleSegments = 2;
	}
	GreekKeyPattern_Initial(totalSpindleSegments);

	for (int i = 1; i <= configGreekKey.patternCount_Pattern; i++)
	{
		if (eStopTriggered || limitTriggered)
		{
			Serial.println("GreekKey 2b eStop or limit triggered.......................");
			return;
		}
		SerialPrint("t2.txt=");
		SerialWrite(0x22);
		SerialPrint(i);
		SerialPrint(nextionQuoteEnd);
		switch (configGreekKey.radialOrAxial_Pattern)
		{
			case RADIAL: // Axis Left CCW
			{
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CW); //0a
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CCW); //0b
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;		
				}
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, reverseDirection * DIR_CW, true); //1
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, reverseDirection * DIR_CCW); //2
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CCW, true); //3
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;		
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CW); //4
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CCW, true); //5
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;		
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 3, reverseDirection * DIR_CCW); //6
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CW, true); // 7
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;	
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CW); //8
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;			
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CW, true); //9
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;			
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, reverseDirection * DIR_CCW); //10
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;		
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, reverseDirection * DIR_CCW, true); //11
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;		
				}
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CCW); //12
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;		
				}
				break;
			}
			case AXIAL: // Spindle Up CCW
			{
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CW, true); //0a
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CCW, true); //0b
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				
				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, reverseDirection * DIR_CCW); //1
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, reverseDirection * DIR_CCW, true); //2
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, reverseDirection * DIR_CCW); //3
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CW, true); //4
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CCW); //5
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 3, reverseDirection * DIR_CCW, true); //6
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CW); //7
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CW, true); // 8
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CW); //9
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, reverseDirection * DIR_CCW, true); //10
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}

				stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, reverseDirection * DIR_CCW); //11
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;
				}
				
				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CCW, true); //12
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
#ifdef DEBUG
	Serial.println("Enter GreekKeyPattern_Initial");
#endif // Debug
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
			if (configSetup.xAltX == 0)
			{
				distance_Axis = configGreekKey.segmentLength_Pattern / configSetup.distancePerRev_AxisX;
				axisShortLegSteps = (int)round(distance_Axis * configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X);
			}
			else
			{
				distance_Axis = configGreekKey.segmentLength_Pattern / configSetup.distancePerRev_AxisXAlt;
				axisShortLegSteps = (int)round(distance_Axis * configSetup.steps360_Axis_XAlt * configSetup.microsteps_Axis_XAlt);
			}
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
	Serial.print("limitTriggered: ");
	if(limitTriggered || eStopTriggered)
	{ 
		Serial.print(limitTriggered);
		Serial.print("  ");
		Serial.println(eStopTriggered);
	}
	else
	{
		Serial.print(limitTriggered);
		Serial.print("  ");
		Serial.println(eStopTriggered);
		SerialPrint("pageGrk.va0.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageGrk.bt1.val=0");
		SerialPrint(nextionEnd);
	}
	SerialPrint("pageGrk.bt3.val=0");
	SerialPrint(nextionEnd);
	SerialPrint("pageGrk.bt2.val=0");
	SerialPrint(nextionEnd);


	degrees_Spindle = StepsToDegrees_Spindle(endPosition_Spindle);
	switch (configGreekKey.axisId)
	{
		case ID_AXIS_Z: // Z Axis
		{
			distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);
			SetEnable(ID_AXIS_Z, false, true);
			break;
		}
		case ID_AXIS_X: // X Axis
		{
			distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_X);
			SetEnable(ID_AXIS_X, false, true);
			break;
		}
		case ID_AXIS_B: // B Axis
		{
			distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_B);
			SetEnable(ID_AXIS_B, false, true);
			break;
		}
	}
	startPositionAbs_Axis = 0;
	returnSteps_Spindle = endPosition_Spindle;

#ifdef DEBUG
	Serial.print("Spindle:");
	Serial.println(endPosition_Spindle);
	Serial.print("Axis:");
	Serial.println(endPosition_Axis);
#endif // DEBUG

	SetEnable(ID_SPINDLE, false, true);
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
	//int newDirection = direction; //* reverseDirection;

	//Serial.print("newDirection: ");
	//Serial.println(newDirection);
	String axisName = "";
	int stepPin = 0;
	int dirPin = 0;
	int limitPin_Max = configSetup.limit_Max_Z;
	int limitPin_Min = configSetup.limit_Min_Z;
	int maxSpeed = 0;
	int accel = 0;
	int speedPercentage = 0;
	int actualSpeed = 0;
	int currentAxis = configGreekKey.axisId;
	limitTriggered = false;

	// When updatePosition is false, the cutter is moved in or out of the workpiece and the distance
	// is not accumulated.
	if (!updatePosition)
	{
		switch (configGreekKey.axisId)
		{
			case ID_AXIS_Z:
			{

				currentAxis = ID_AXIS_X;
#ifdef DEBUG
				Serial.print("X-CurrentAxis:  ");
				Serial.println(currentAxis);
#endif // Debug
				break;
			}
			case ID_AXIS_X:
			{

				currentAxis = ID_AXIS_Z;
#ifdef DEBUG
				Serial.print("Z-CurrentAxis:  ");
				Serial.println(currentAxis);
#endif // Debug
				break;
			}
			case ID_AXIS_B: // Assumes the toolpath is on the outside of the workpiece generally moving in Z axis
			{

				currentAxis = ID_AXIS_X;
#ifdef DEBUG
				Serial.print("X-CurrentAxis:  ");
				Serial.println(currentAxis);
#endif // Debug
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
	Serial.print("Move_Axis:direction ");
	Serial.println(direction);
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
			limitPin_Max = configSetup.limit_Max_Z;
			limitPin_Min = configSetup.limit_Min_Z;
			digitalWrite(configSetup.limit_Max_Z, HIGH);
			digitalWrite(configSetup.limit_Min_Z, HIGH);
			SetEnable(ID_AXIS_Z, true, true);
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
			limitPin_Max = configSetup.limit_Max_X;
			limitPin_Min = configSetup.limit_Min_X;
			digitalWrite(configSetup.limit_Max_X, HIGH);
			digitalWrite(configSetup.limit_Min_X, HIGH);
			SetEnable(ID_AXIS_X, true, true);
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
			limitPin_Max = configSetup.limit_Max_B;
			limitPin_Min = configSetup.limit_Min_B;
			digitalWrite(configSetup.limit_Max_B, HIGH);
			digitalWrite(configSetup.limit_Min_B, HIGH);
			SetEnable(ID_AXIS_B, true, true);
			maxSpeed = configGreekKey.maxSpd_Axis_B;
			accel = configGreekKey.accel_Axis_B;
			speedPercentage = configGreekKey.speedPercent_Axis_B;
			break;
		}
	}

	// Set limit switches HIGH
	//digitalWrite(limitPin_Max, HIGH);
	//digitalWrite(limitPin_Min, HIGH);

	actualSpeed = maxSpeed * speedPercentage * .01;
	StepControl stepControllerAxis;
	Stepper stepperAxis(stepPin, dirPin);
	float targetSteps = (segmentSteps * multiplier * direction);
   //////// int32_t targetSteps = round(segmentSteps * multiplier * direction);
	//int32_t targetSteps = round(segmentSteps * multiplier * newDirection);
	//int32_t targetSteps = round(segmentSteps * multiplier); // *newDirection);
	targetSteps = round(targetSteps);

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
		if (digitalRead(limitPin_Max) != configSetup.limit_NCorNO)
		{
			Serial.print("                          limit_NCorNO: ");
			Serial.print(configSetup.limit_NCorNO);
			Serial.print(" limit_Max: ");
			Serial.println(digitalRead(limitPin_Max));

			stepControllerAxis.emergencyStop();
			stopSteppers = true;
			// Disable all motors
			SetEnable(ID_SPINDLE, false, true);
			SetEnable(ID_AXIS_Z, false, true);
			SetEnable(ID_AXIS_X, false, true);
			SetEnable(ID_AXIS_B, false, true);
			SetLimitColors(PAGE_GRK, DIR_CW);
			//goto EndLoops;
			return;
		}
		if  (digitalRead(limitPin_Min) != configSetup.limit_NCorNO)
		{
			Serial.print("                          limit_NCorNO: ");
			Serial.print(configSetup.limit_NCorNO);
			Serial.print(" limit_Min: ");
			Serial.println(digitalRead(limitPin_Max));

			stepControllerAxis.emergencyStop();
			stopSteppers = true;
			// Disable all motors
			SetEnable(ID_SPINDLE, false, true);
			SetEnable(ID_AXIS_Z, false, true);
			SetEnable(ID_AXIS_X, false, true);
			SetEnable(ID_AXIS_B, false, true);
			SetLimitColors(PAGE_GRK, DIR_CCW);

			//goto EndLoops;
			return;
		}

		if (configSetup.eStop != 0)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
				Serial.println("  GreekKey_Move                        EStop ");
				stepControllerAxis.emergencyStop();
				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_B, false, false);
				SetEStopColors(PAGE_GRK);
				return;
			}
		}

		if (StopGreekKey())
		{
#ifdef DEBUG
			Serial.println("StopGreekKey() is true:");
#endif // DEBUG
			stepControllerAxis.stop();

			// Disable all motors
			SetEnable(ID_SPINDLE, false, true);
			SetEnable(ID_AXIS_Z, false, true);
			SetEnable(ID_AXIS_X, false, true);
			SetEnable(ID_AXIS_B, false, true);

			retVal = true;
			break;
		}
		else
		{
			//Serial.println("StopGreekKey() is false:");
		}
		MilliDelay(5);
	}

EndLoops:
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
	int limitPin_Max = configSetup.limit_Max_Z;
	int limitPin_Min = configSetup.limit_Min_Z;


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
			limitPin_Max = configSetup.limit_Max_Z;
			limitPin_Min = configSetup.limit_Min_Z;
			SetEnable(ID_AXIS_Z, true, true);

			maxSpd_Axis = configGreekKey.maxSpd_Axis_Z;
			accel_Axis = configGreekKey.accel_Axis_Z;
			speedPercent_Axis = configGreekKey.speedPercent_Axis_Z;

			break;
		}
		case ID_AXIS_X:
		{
			stepPin_Axis = PIN_AXIS_X_STEP;
			dirPin_Axis = PIN_AXIS_X_DIR;
			limitPin_Max = configSetup.limit_Max_X;
			limitPin_Min = configSetup.limit_Min_X;
			SetEnable(ID_AXIS_X, true, true);

			maxSpd_Axis = configGreekKey.maxSpd_Axis_X;
			accel_Axis = configGreekKey.accel_Axis_X;
			speedPercent_Axis = configGreekKey.speedPercent_Axis_X;

			break;
		}
		case ID_AXIS_B:
		{
			stepPin_Axis = PIN_AXIS_B_STEP;
			dirPin_Axis = PIN_AXIS_B_DIR;
			limitPin_Max = configSetup.limit_Max_B;
			limitPin_Min = configSetup.limit_Min_B;
			SetEnable(ID_AXIS_B, true, true);

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

	// Set limit switches HIGH
	digitalWrite(limitPin_Max, HIGH);
	digitalWrite(limitPin_Min, HIGH);

	// ToDo:
	AngularLengthAndMaxSpeed(maxSpd_Spindle, maxSpd_Axis);

	SetEnable(ID_SPINDLE, true, true);
	currentSpeedPercent_Axis = speedPercent_Axis * .01;
	currentSpeedPercent_Spindle = speedPercent_Spindle * .01;
	
	StepControl stepController;
	Stepper stepper_Axis(stepPin_Axis, dirPin_Axis);
	stepper_Axis
		.setMaxSpeed(maxSpd_Axis * currentSpeedPercent_Axis)
		.setAcceleration(accel_Axis);
	stepper_Axis.setPosition(0);


	//int32_t targetSteps_Axis = round(abs(shortLegLength_Axis) * multiplier_Axis);
	int32_t targetSteps_Axis = round((shortLegLength_Axis) * multiplier_Axis);
	Serial.print("shortLegLength_Axis: ");
	Serial.print(shortLegLength_Axis);
	Serial.print("   multiplier_Axis: ");
	Serial.print(multiplier_Axis);
	Serial.print("   Axis targetSteps: ");
	Serial.println(targetSteps_Axis);
	stepper_Axis.setTargetRel(targetSteps_Axis);

	stepperSpindle
		.setMaxSpeed(maxSpd_Spindle * currentSpeedPercent_Spindle)
		.setAcceleration(accel_Spindle);
	stepperSpindle.setPosition(0);
	//float spindleSteps = abs(shortLegLength_Spindle) * multiplier_Spindle;
	float spindleSteps = (shortLegLength_Spindle) * multiplier_Spindle;
	int32_t targetSteps_Spindle = (int32_t)round(spindleSteps);
	Serial.print("shortLegLength_Spindle: ");
	Serial.print(shortLegLength_Spindle);
	Serial.print("   multiplier_Spindle: ");
	Serial.print(multiplier_Spindle);
	Serial.print("Spindle targetSteps: ");
	Serial.println(targetSteps_Spindle);
	stepperSpindle.setTargetRel(targetSteps_Spindle);

#ifdef DEBUG
	Serial.println("-=-=-=-=-=-=-=-=-=-=-=-===-=-=-==-=-=-=-");
	Serial.print("spindleSteps:");
	Serial.println(spindleSteps);
	Serial.print("targetSteps_Spindle:");
	Serial.println(targetSteps_Spindle);
	Serial.print("targetSteps_Axis:");
	Serial.println(targetSteps_Axis);
#endif // DEBUG

	stepController.moveAsync(stepperSpindle,stepper_Axis);
	while (stepController.isRunning())
	{
		if (digitalRead(limitPin_Max) != configSetup.limit_NCorNO)
		{
			Serial.println("                          limitPin");

			stepController.emergencyStop();
			stopSteppers = true;
			// Disable all motors
			SetEnable(ID_SPINDLE, false, true);
			SetEnable(ID_AXIS_Z, false, true);
			SetEnable(ID_AXIS_X, false, true);
			SetEnable(ID_AXIS_B, false, true);
			SetLimitColors(PAGE_GRK, DIR_CW);
			return;
			//break;

		}
		if (digitalRead(limitPin_Min) != configSetup.limit_NCorNO)
		{
			Serial.println("                          limitPin");

			stepController.emergencyStop();
			stopSteppers = true;
			// Disable all motors
			SetEnable(ID_SPINDLE, false, true);
			SetEnable(ID_AXIS_Z, false, true);
			SetEnable(ID_AXIS_X, false, true);
			SetEnable(ID_AXIS_B, false, true);
			SetLimitColors(PAGE_GRK, DIR_CCW);
			return;
			//break;

		}

		if (configSetup.eStop != 0)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
				Serial.println("                          EStop");
				stepController.emergencyStop();
				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_B, false, false);
				SetEStopColors(PAGE_GRK);
				return;
			}
		}

		if (StopGreekKey())
		{
#ifdef DEBUG
			Serial.println("StopGreekKey() is true:");
#endif // DEBUG
			stepController.stop();
			SetEnable(ID_SPINDLE, false, true);
			SetEnable(ID_AXIS_Z, false, true);
			SetEnable(ID_AXIS_X, false, true);
			SetEnable(ID_AXIS_B, false, true);
			retVal = true;
			break;
		}
		MilliDelay(5);
	}

	SetEnable(ID_SPINDLE, false, true);
	SetEnable(ID_AXIS_Z, false, true);
	SetEnable(ID_AXIS_X, false, true);
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

	SetEnable(ID_SPINDLE, true, true);
	StepControl stepControllerSpindle;

	Stepper stepper_Spindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
		stepper_Spindle
			.setMaxSpeed(configGreekKey.maxSpd_Spindle * configGreekKey.speedPercent_Spindle * .01)
			.setAcceleration(configGreekKey.accel_Spindle);

	int32_t mSpeed = configGreekKey.maxSpd_Spindle * configGreekKey.speedPercent_Spindle * .01;


	stepper_Spindle.setMaxSpeed(mSpeed);
	stepper_Spindle.setPosition(0);

	float targetSteps = segmentSteps * multiplier * direction;

	float targetStepsRounded = round(targetSteps);
	
	targetSteps = targetStepsRounded;

#ifdef DEBUG
	Serial.print("targetStepsRounded:");
	Serial.println(targetStepsRounded);
	///////targetSteps = round(targetSteps);
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
		if (configSetup.eStop != 0)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
				Serial.println("                          EStop");
				stepControllerSpindle.emergencyStop();
				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_B, false, false);
				SetEStopColors(PAGE_GRK);
				return;
			}
		}

		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
				case 83: // - S
				{
					stepControllerSpindle.stop();

					SetEnable(ID_SPINDLE, false, true);
					SetEnable(ID_AXIS_Z, false, true);
					SetEnable(ID_AXIS_X, false, true);

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
void Program_FromFile(int direction)
{
	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	const char* nextionEnd = "\xFF\xFF\xFF";
	limitTriggered = false;
	eStopTriggered = false;

	int eePromAddress = 0;

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

	int selectedAxis = 0;

	// Reset start and end positions
	startPositionAbs_Axis = 0;
	endPosition_Spindle = 0;
	endPosition_Axis = 0;

	// Reset stopSteppers
	stopSteppers = false;

	// String variables
	const char* pageBE_t3 = "pageBE.t3.txt="; // Spindle Begin
	const char* pageBE_t1 = "pageBE.t1.txt="; // Spindle End
	const char* pageBE_t4 = "pageBE.t3.txt="; // Axis Begin
	const char* pageBE_t2 = "pageBE.t1.txt="; // Axis End
	const char* pageProgram_t15 = "pageProgram.t15.txt="; // Segments
	const char* pageProgram_t21 = "pageProgram.t21.txt="; // Direction
	const char* pageProgram_t22 = "pageProgram.t22.txt="; // Line Number

#ifdef DEBUG
	Serial.print(">>>>>>>>>>>>>>>>>>>>segmentOrActual:");
	Serial.println(configGreekKey.segmentOrActual);
#endif // DEBUG
	// Get Spindle segment count and line count from file :
	badFilename = false;
	badCommand = false;
	if (configGreekKey.segmentOrActual == 2) // 2: Segment 
	{
		for (lineCounter = 0; lineCounter <= fileLineCount; lineCounter++)
		{
			segmentMultiplier = GetGreekKeyDataFromSD(lineCounter);
			Serial.print(segmentMultiplier);
			Serial.print("    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>badCommand:  ");
			Serial.println(badCommand);
			if (badFilename)
			{
#ifdef DEBUG
				Serial.println("Exit Greek Key: File not found");
#endif // DEBUG
				SerialPrint("pageProgram.tFileName.txt=");
				SerialWrite(0x22);
				SerialPrint("File not found");
				SerialPrint(nextionQuoteEnd);

				goto EndLoops;
			}

			if (badCommand)
			{
#ifdef DEBUG
				Serial.println("Exit Greek Key: Bad Command");
#endif // DEBUG
				SerialPrint("pageProgram.tFileName.txt=");
				SerialWrite(0x22);
				SerialPrint("Bad Command");
				SerialPrint(nextionQuoteEnd);

				SerialPrint(pageProgram_t22);
				SerialWrite(0x22);
				SerialPrint(lineCounter);
				SerialPrint(nextionQuoteEnd);

				SerialPrint(pageProgram_t21);
				SerialWrite(0x22);
				SerialPrint(currentCommand);
				SerialPrint(nextionQuoteEnd);

				goto EndLoops;
			}

#ifdef DEBUG
			//Serial.print(">>>segmentMultiplier:");
			//Serial.println(segmentMultiplier);
#endif // DEBUG

			switch (moveType)
			{
				case 1: // Spindle segment count (Axial and Spindle segments reverse when Axial is selected.)
				{
					if (configGreekKey.radialOrAxial_File == RADIAL)
					{
						spindleSegmentCount = segmentMultiplier;
					}
					break;
				}
				case 2: // Axis segment count (Axial and Spindle segments reverse when Axial is selected.)
				{
					if (configGreekKey.radialOrAxial_File == AXIAL)
					{
						spindleSegmentCount = segmentMultiplier;
					}
					break;
				}
				case 69: // E - Exit
				case 101: // e
				{
					// Store total line count in file
					goto EndSegmentCount;

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
		////int stepsPerRevolution_Spindle = (int)(round(configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle));
		int stepsPerRevolution_Spindle = (int)((configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle));
		float patternDivisor = ((float)configGreekKey.countPatternPer360_File * (float)spindleSegmentCount);
		//spindleShortLegSteps = (int)((floor(stepsPerRevolution_Spindle)) / (patternDivisor));
		////spindleShortLegSteps = (int)((round(stepsPerRevolution_Spindle)) / (patternDivisor));
		spindleShortLegSteps = (int)(((stepsPerRevolution_Spindle)) / (patternDivisor));
#ifdef DEBUG

		Serial.print("lineCounter:");
		Serial.println(lineCounter);
		Serial.print("patternDivisor:");
		Serial.println(patternDivisor);
		Serial.print("stepsPerRevolution_Spindle:");
		Serial.println(stepsPerRevolution_Spindle);
		Serial.print("patternDivisor:");
		Serial.println(patternDivisor);
		Serial.print("spindleShortLegSteps:");
		Serial.println(spindleShortLegSteps);
#endif // DEBUG
	}

	// Calculate axis steps
	////shortSegmentStepsAxisZ = (int)(round((configGreekKey.segmentLength_File / configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z)));
	shortSegmentStepsAxisZ = (int)(((configGreekKey.segmentLength_File / configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z)));
#ifdef DEBUG
	Serial.print("configGreekKey.segmentLength_File:");
	Serial.println(configGreekKey.segmentLength_File);
	
		Serial.print("configSetup.distancePerRev_AxisZ:");
	Serial.println(configSetup.distancePerRev_AxisZ);
		
			Serial.print("configSetup.steps360_Axis_Z:");
		Serial.println(configSetup.steps360_Axis_Z);
		
			Serial.print("configSetup.microsteps_Axis_Z:");
		Serial.println(configSetup.microsteps_Axis_Z);
		
			Serial.print("configSetup.microsteps_Axis_Z:");
		Serial.println(configSetup.microsteps_Axis_Z);

		Serial.print("shortSegmentStepsAxisZ:");
		Serial.println(shortSegmentStepsAxisZ);

#endif // DEBUG
	if (configSetup.xAltX == 0)
	{
		////shortSegmentStepsAxisX = (int)(round((configGreekKey.segmentLength_File / configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X)));
		shortSegmentStepsAxisX = (int)(((configGreekKey.segmentLength_File / configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X)));
	}
	else
	{
		////shortSegmentStepsAxisX = (int)(round((configGreekKey.segmentLength_File / configSetup.distancePerRev_AxisXAlt) * (configSetup.steps360_Axis_XAlt * configSetup.microsteps_Axis_XAlt)));
		shortSegmentStepsAxisX = (int)(((configGreekKey.segmentLength_File / configSetup.distancePerRev_AxisXAlt) * (configSetup.steps360_Axis_XAlt * configSetup.microsteps_Axis_XAlt)));
	}

	switch (configSetup.radialOrLinear_Axis_B)
	{
		case RADIAL_B:
		{
			////shortSegmentStepsAxisB = (int)(round(DistanceToSteps_RadialB(configGreekKey.segmentLength_File)));
			shortSegmentStepsAxisB = (int)((DistanceToSteps_RadialB(configGreekKey.segmentLength_File)));
		}
		case LINEAR_B:
		{
			////shortSegmentStepsAxisB = (int)(round(DistanceToSteps_LinearB(configGreekKey.segmentLength_File)));
			shortSegmentStepsAxisB = (int)((DistanceToSteps_LinearB(configGreekKey.segmentLength_File)));
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

		SerialPrint("t2.txt=");
		SerialWrite(0x22);
		SerialPrint(j);
		SerialPrint(nextionQuoteEnd);
		// Inner loop
		for (int i = 0; i <= fileLineCount; i++)
		{
			// Index or Move page stop pressed.
			if (stopSteppers)
			{
				goto EndLoops;
			}

			// Reset hVal and vVal
			hv_AxisLegLength = 0;
			hv_SpindleLegLength = 0;

			// Get data
			segmentMultiplier = GetGreekKeyDataFromSD(i);
			Serial.print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>badCommand:  ");
			Serial.println(badCommand);

			if (badCommand)
			{
#ifdef DEBUG
				Serial.println("Exit Greek Key: Bad Command");
#endif // DEBUG
				SerialPrint("pageProgram.tFileName.txt=");
				SerialWrite(0x22);
				SerialPrint("Bad Command");
				SerialPrint(nextionQuoteEnd);

				SerialPrint(pageProgram_t22);
				SerialWrite(0x22);
				SerialPrint(lineCounter);
				SerialPrint(nextionQuoteEnd);

				SerialPrint(pageProgram_t21);
				SerialWrite(0x22);
				SerialPrint(currentCommand);
				SerialPrint(nextionQuoteEnd);

				goto EndLoops;
			}



			int moveDirection = DIR_CW;
			////////if ((segmentMultiplier * reverseDirection) < 0)
			////////{
			////////	moveDirection = DIR_CCW;
			////////}



			int moveDirection_H = DIR_CW;
			////////if ((hv_AxisLegLength * reverseDirection) < 0)
			////////{
			////////	moveDirection_H = DIR_CCW;
			////////}

			int moveDirection_V = DIR_CW;
			////////if ((hv_SpindleLegLength * reverseDirection) < 0)
			////////{
			////////	moveDirection_V = DIR_CCW;
			////////}

			Serial.print("reverseDirection:             ");
			Serial.println(reverseDirection);
			Serial.print("moveDirection:             ");
			Serial.println(moveDirection);
			Serial.print("hv_SpindleLegLength:             ");
			Serial.println(hv_SpindleLegLength);
			Serial.print("moveDirection_H:             ");
			Serial.println(moveDirection_H);
			Serial.print("hv_AxisLegLength:             ");
			Serial.println(hv_AxisLegLength);
			Serial.print("moveDirection_V:             ");
			Serial.println(moveDirection_V);

			SerialPrint(pageProgram_t22);
			SerialWrite(0x22);
			SerialPrint(i);
			SerialPrint(nextionQuoteEnd);

			SerialPrint(pageProgram_t21);
			SerialWrite(0x22);
			SerialPrint(currentCommand);
			SerialPrint(nextionQuoteEnd);

			SerialPrint(pageProgram_t15);
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


				case 65: // A - Axis
				{

					switch (configGreekKey.radialOrAxial_File)
					{
						case RADIAL: // Axis: Negative move direction is CCW
						{
							if (configGreekKey.segmentOrActual == 2) // 2: Segment  
							{
								Serial.print("----shortSegmentStepsAxis--------------------------------------------------");
								Serial.println(shortSegmentStepsAxis);
								////stopAll = GreekKey_Move_Axis(shortSegmentStepsAxis, abs(segmentMultiplier), moveDirection, true);
								stopAll = GreekKey_Move_Axis(shortSegmentStepsAxis, segmentMultiplier, moveDirection, true);
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
									if (configSetup.xAltX == 0)
									{
										axisSteps = round((segmentMultiplier / configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X));
									}
									else
									{
										axisSteps = round((segmentMultiplier / configSetup.distancePerRev_AxisXAlt) * (configSetup.steps360_Axis_XAlt * configSetup.microsteps_Axis_XAlt));
									}
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

								stopAll = GreekKey_Move_Axis(axisSteps, 1, moveDirection, true);
							}
							break;
						}
						case AXIAL: // Spindle
						{
							if (configGreekKey.segmentOrActual == 2) // 2: Segment 
							{
								////stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, abs(segmentMultiplier), moveDirection);
								stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, segmentMultiplier, moveDirection);
							}
							else  //3: Actual
							{
								spindleSteps = round((configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (segmentMultiplier / 360));
								stopAll = GreekKey_Move_Spindle(spindleSteps, 1, moveDirection);
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

				case 67: // C - Spindle
				case 83: // S - Spindle
				{
					switch (configGreekKey.radialOrAxial_File)
					{
						case RADIAL: // Spindle Down CW
						{
							// Negative steps move CCW
							if (configGreekKey.segmentOrActual == 2) // 2: Segment 
							{
								//stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, segmentMultiplier, DIR_CW);
								////stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, abs(segmentMultiplier), moveDirection);
								stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, segmentMultiplier, moveDirection);
							}
							else  //3: Actual
							{
								spindleSteps = round((configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (segmentMultiplier / 360));
								stopAll = GreekKey_Move_Spindle(spindleSteps, 1, moveDirection);
							}
							break;
						}
						case AXIAL: // Move Axis instead of Spindle
						{
							if (configGreekKey.segmentOrActual == 2) // 2: Segment  
							{
								////stopAll = GreekKey_Move_Axis(shortSegmentStepsAxis, abs(segmentMultiplier), moveDirection, true);
								stopAll = GreekKey_Move_Axis(shortSegmentStepsAxis, segmentMultiplier, moveDirection, true);
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
					if (stopAll)
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

				case 70:  // F - Move page command
				{

					if (commandMove == "D")
					{ 
						switch (configMove.axisId)
						{
							case ID_MOVE_AXIS_Z1:
							{
								configMove.distance_MoveZ1 = segmentMultiplier;
								eePromAddress = 168;
								break;
							}
							case ID_MOVE_AXIS_Z2:
							{
								configMove.distance_MoveZ2 = segmentMultiplier;
								eePromAddress = 396;
								break;
							}
							case ID_MOVE_AXIS_X1:
							{
								configMove.distance_MoveX1 = segmentMultiplier;
								eePromAddress = 340;
								break;
							}
							case ID_MOVE_AXIS_X2:
							{
								configMove.distance_MoveX2 = segmentMultiplier;
								eePromAddress = 316;
								break;
							}
							case ID_MOVE_AXIS_B1:
							{
								configMove.distance_MoveB1 = segmentMultiplier;
								eePromAddress = 332;
								break;
							}
							case ID_MOVE_AXIS_B2:
							{
								configMove.distance_MoveB2 = segmentMultiplier;
								eePromAddress = 500;
								break;
							}
						}

						SerialPrint("wepo ");
						SerialWrite(0x22);
						SerialPrint(segmentMultiplier);
						SerialWrite(0x22);
						SerialPrint(",");
						SerialPrint(eePromAddress);
						SerialPrint(nextionEnd);
						MilliDelay(20);

						Serial.print("wepo ");
						Serial.write(0x22);
						Serial.print(segmentMultiplier);
						Serial.write(0x22);
						Serial.print(",");
						Serial.print(eePromAddress);
						Serial.println(nextionEnd);

						const char* pageMove_t10 = "pageMove.t10.txt=";
						SerialPrint(pageMove_t10);
						SerialPrint("\x22");
						SerialPrint(segmentMultiplier);
						SerialPrint(nextionQuoteEnd);

						Serial.print("Move Distance: ");
						Serial.print(segmentMultiplier);
						Serial.print("      eeprom:  ");
						Serial.println(eePromAddress);
					}
					else if (commandMove == "F") // Run Move page
					{
						//int moveDirection = abs(segmentMultiplier);
						////if (abs(segmentMultiplier) * moveDirection < 0)
						if (segmentMultiplier * moveDirection < 0)
						{
							MoveAxis(configMove.axisId, DIR_CCW);
						}
						else
						{
							MoveAxis(configMove.axisId, DIR_CW);
						}

					}
					break;
				}

				case 71:  // G - Sync page command
				{
					SerialPrint("wepo ");
					if (commandSync == "H")
					{
						// 0 = Left, 1 = Right
						configSync.helixType = segmentMultiplier;
						SerialPrint(segmentMultiplier);
						eePromAddress = 420;
					}
					else if (commandSync == "D")
					{
						configSync.distance = segmentMultiplier;
						SerialWrite(0x22);
						SerialPrint(segmentMultiplier);
						SerialWrite(0x22);
						eePromAddress = 628;
					}
					else if (commandSync == "G") // Run Sync page
					{
						int directionSpindle = 1;
						int directionAxis = 1;
						////if (abs(segmentMultiplier) * moveDirection < 0) // Move in (towards headstock).
						if (segmentMultiplier * moveDirection < 0) // Move in (towards headstock).
						{
							if (configSync.helixType == 1) // Right hand
							{
								directionSpindle = DIR_CW; //CW
								directionAxis = DIR_CCW; // CCW
							}
							else // Left hand
							{
								directionSpindle = DIR_CCW;// 0; //CCW
								directionAxis = DIR_CCW;// 0; // CCW	
							}

						}
						else // Move out (away from headstock).
						{
							//if (segmentMultiplier < 0) // Move in (towards headstock).
							//{
							if (configSync.helixType == 1) // Right hand
							{
								directionSpindle = DIR_CCW; // CW
								directionAxis = DIR_CW; // CW
							}
							else // Left hand
							{
								directionSpindle = DIR_CW;//0; // CCW
								directionAxis = DIR_CW; // CW
							}

							//}
						}


						Sync(directionSpindle, directionAxis);

					}
					else if (commandSync == "R")
					{
						configSync.revolutions_Spindle = segmentMultiplier;
						SerialWrite(0x22);
						SerialPrint(segmentMultiplier);
						SerialWrite(0x22);
						eePromAddress = 612;
					}

					SerialPrint(",");
					SerialPrint(eePromAddress);
					SerialPrint(nextionEnd);
					break;
				}

				case 72: // H  - Angular Move. Line must also contain V
				{
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
										Serial.print("shortSegmentStepsAxisZ: ");
										Serial.print(shortSegmentStepsAxisZ);
										Serial.print("   (abs)angularAxisLegLength: ");
										Serial.print(abs(hv_AxisLegLength));
										Serial.print("   Axis moveDirection_H: ");
										Serial.println(moveDirection_H);
										////stopAll = GreekKey_Move_Angular_TeensyStep(abs(shortSegmentStepsAxisZ), abs(hv_AxisLegLength) * moveDirection_H, abs(spindleShortLegSteps), abs(hv_SpindleLegLength) * moveDirection_V);
										stopAll = GreekKey_Move_Angular_TeensyStep((shortSegmentStepsAxisZ), (hv_AxisLegLength) * moveDirection_H, (spindleShortLegSteps), (hv_SpindleLegLength) * moveDirection_V);
										break;
									}
									case ID_AXIS_X: // X Axis is primary
									{
										////stopAll = GreekKey_Move_Angular_TeensyStep(shortSegmentStepsAxisX, abs(hv_AxisLegLength) * moveDirection_H, spindleShortLegSteps, abs(hv_SpindleLegLength) * moveDirection_V);
										stopAll = GreekKey_Move_Angular_TeensyStep(shortSegmentStepsAxisX, (hv_AxisLegLength) * moveDirection_H, spindleShortLegSteps, (hv_SpindleLegLength) * moveDirection_V);
										break;
									}
									case ID_AXIS_B: // B Axis is primary
									{
										////stopAll = GreekKey_Move_Angular_TeensyStep(shortSegmentStepsAxisB, abs(hv_AxisLegLength) * moveDirection_H, spindleShortLegSteps, abs(hv_SpindleLegLength) * moveDirection_V);
										stopAll = GreekKey_Move_Angular_TeensyStep(shortSegmentStepsAxisB, (hv_AxisLegLength) * moveDirection_H, spindleShortLegSteps, (hv_SpindleLegLength) * moveDirection_V);
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
										axisSteps = (int)(round((hv_AxisLegLength / configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z)));
										break;
									}
									case ID_AXIS_X: // X Axis
									{
										if (configSetup.xAltX == 0)
										{
											axisSteps = (int)(round((hv_AxisLegLength / configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X)));
										}
										else
										{
											axisSteps = (int)(round((hv_AxisLegLength / configSetup.distancePerRev_AxisXAlt) * (configSetup.steps360_Axis_XAlt * configSetup.microsteps_Axis_XAlt)));
										}
										break;
									}
									case ID_AXIS_B: // B Axis
									{
										switch (configSetup.radialOrLinear_Axis_B)
										{
											case RADIAL_B:
											{
												axisSteps = (int)(DistanceToSteps_RadialB(hv_AxisLegLength));
												break;
											}
											case LINEAR_B:
											{
												axisSteps = (int)(DistanceToSteps_LinearB(hv_AxisLegLength));
												break;
											}
										}
										break;
									}

								}

								axisSteps = axisSteps * direction;
								spindleSteps = round((configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (hv_SpindleLegLength / 360));
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
										Serial.print("shortSegmentStepsAxisZ: ");
										Serial.print(shortSegmentStepsAxisZ);
										Serial.print("   hv_AxisLegLength: ");
										Serial.print(hv_AxisLegLength);
										Serial.print("   moveDirection_H: ");
										Serial.print(moveDirection_H);
										Serial.print("   spindleShortLegSteps: ");
										Serial.print(spindleShortLegSteps);
										Serial.print("   hv_SpindleLegLength: ");
										Serial.print(hv_SpindleLegLength);
										Serial.print("   moveDirection_V: ");
										Serial.println(moveDirection_V);


										//stopAll = GreekKey_Move_Angular_TeensyStep(abs(spindleShortLegSteps), abs(hv_SpindleLegLength) * moveDirection_V, shortSegmentStepsAxisZ, abs(hv_AxisLegLength) * moveDirection_H);

										////stopAll = GreekKey_Move_Angular_TeensyStep(shortSegmentStepsAxisZ, abs(hv_SpindleLegLength) * moveDirection_V, abs(spindleShortLegSteps), abs(hv_AxisLegLength) * moveDirection_H);
										stopAll = GreekKey_Move_Angular_TeensyStep(shortSegmentStepsAxisZ, (hv_SpindleLegLength) * moveDirection_V, (spindleShortLegSteps), (hv_AxisLegLength) * moveDirection_H);
										//stopAll = GreekKey_Move_Angular_TeensyStep(abs(shortSegmentStepsAxisZ), abs(hv_AxisLegLength) * moveDirection_H, abs(spindleShortLegSteps), abs(hv_SpindleLegLength) * moveDirection_V);
										break;
									}
									case ID_AXIS_X: // 
									{
										stopAll = GreekKey_Move_Angular_TeensyStep(shortSegmentStepsAxisX, hv_SpindleLegLength * direction, spindleShortLegSteps, hv_AxisLegLength);
										break;
									}
									case ID_AXIS_B: // 
									{
										stopAll = GreekKey_Move_Angular_TeensyStep(shortSegmentStepsAxisB, hv_SpindleLegLength * direction, spindleShortLegSteps, hv_AxisLegLength);
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
										axisSteps = round((hv_SpindleLegLength / configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z));
										break;
									}
									case ID_AXIS_X: // X Axis
									{
										if (configSetup.xAltX == 0)
										{
											axisSteps = round((hv_SpindleLegLength / configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X));
										}
										else
										{
											axisSteps = round((hv_SpindleLegLength / configSetup.distancePerRev_AxisXAlt) * (configSetup.steps360_Axis_XAlt * configSetup.microsteps_Axis_XAlt));
										}
										break;
									}
									case ID_AXIS_B: // B Axis
									{
										switch (configSetup.radialOrLinear_Axis_B)
										{
											case RADIAL_B:
											{
												axisSteps = DistanceToSteps_RadialB(hv_SpindleLegLength);
												break;
											}
											case LINEAR_B:
											{
												axisSteps = DistanceToSteps_LinearB(hv_SpindleLegLength);
												break;
											}
										}
										break;
									}
								}

								axisSteps = axisSteps * direction;
								spindleSteps = round((configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (hv_AxisLegLength / 360));
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
					//SerialPrint(pageProgram_t21);
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
								SetEnable(ID_AXIS_X, false, true); // Allow depth of cut to be changed
								break;
							}
							case ID_AXIS_X: // X Axis is primary
							{
								//stopAll = GreekKey_Move_Axis_Alt(shortSegmentStepsAxisZ, segmentMultiplier, DIR_CCW);
								stopAll = GreekKey_Move_Axis(shortSegmentStepsAxisZ, segmentMultiplier, DIR_CCW, false);
								SetEnable(ID_AXIS_Z, false, true); // Allow depth of cut to be changed
								break;
							}
							case ID_AXIS_B: // B Axis is primary
							{
								//stopAll = GreekKey_Move_Axis_Alt(shortSegmentStepsAxisX, segmentMultiplier, DIR_CCW);
								stopAll = GreekKey_Move_Axis(shortSegmentStepsAxisX, segmentMultiplier, DIR_CCW, false);
								SetEnable(ID_AXIS_X, false, true); // Allow depth of cut to be changed
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
								if (configSetup.xAltX == 0)
								{
									axisSteps = round((segmentMultiplier / configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X));
								}
								else
								{
									axisSteps = round((segmentMultiplier / configSetup.distancePerRev_AxisXAlt) * (configSetup.steps360_Axis_XAlt * configSetup.microsteps_Axis_XAlt));
								}
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

				case 74: // J - BRadius
				{
					configSetup.radiusB = segmentMultiplier;
					SerialPrint("wepo ");
					SerialWrite(0x22);
					SerialPrint(segmentMultiplier);
					SerialWrite(0x22);
					eePromAddress = 688;
					SerialPrint(",");
					SerialPrint(eePromAddress);
					SerialPrint(nextionEnd);
					break;
				}

				case 75:// K - Enable/DisableStepper
				{
					//enableDisableStepperID = newSizeString[1];
					if(enableDisableStepperID=="S")
					{ 
						if (segmentMultiplier == 0)
						{
							SetEnable(ID_SPINDLE, false, false);
						}
						else
						{
							SetEnable(ID_SPINDLE, true, false);
						}

					}
					else if (enableDisableStepperID == "X")
					{
						if (segmentMultiplier == 0)
						{
							SetEnable(ID_AXIS_X, false, false);
						}
						else
						{
							SetEnable(ID_AXIS_X, true, false);
						}
					}
					else if (enableDisableStepperID == "Z")
					{
						if (segmentMultiplier == 0)
						{
							SetEnable(ID_AXIS_Z, false, false);
						}
						else
						{
							SetEnable(ID_AXIS_Z, true, false);
						}
					}
					else if (enableDisableStepperID == "B")
					{
						if (segmentMultiplier == 0)
						{
							SetEnable(ID_AXIS_B, false, false);
						}
						else
						{
							SetEnable(ID_AXIS_B, true, false);
						}
					}
					else if (enableDisableStepperID == "A")
					{
						switch (configGreekKey.axisId)
						{
							case ID_AXIS_Z: // Z Axis
							{
								if (segmentMultiplier == 0)
								{
									SetEnable(ID_AXIS_Z, false, false);
								}
								else
								{
									SetEnable(ID_AXIS_Z, true, false);
								}
								break;
							}
							case ID_AXIS_X: // Z Axis
							{
								if (segmentMultiplier == 0)
								{
									SetEnable(ID_AXIS_X, false, false);
								}
								else
								{
									SetEnable(ID_AXIS_X, true, false);
								}
								break;
							}
							case ID_AXIS_B: // Z Axis
							{
								if (segmentMultiplier == 0)
								{
									SetEnable(ID_AXIS_B, false, false);
								}
								else
								{
									SetEnable(ID_AXIS_B, true, false);
								}

								break;
							}
						}
					}

					break;
				}

				case 78:  // N - Index page command
				{
					SerialPrint("wepo ");
					if (commandIndex == "J")
					{
						configIndex_Main.indexId = segmentMultiplier;
						SerialPrint(segmentMultiplier);
						eePromAddress = 368;
					}
					else if (commandIndex == "N")
					{
						if (abs(segmentMultiplier) * moveDirection < 0)
						{
							IndexSpindle(DIR_CCW);
							//IndexFromFile(DIR_CCW);
						}
						else
						{
							IndexSpindle(DIR_CW);
							//IndexFromFile(DIR_CW);
						}

					}
					else if (commandIndex == "T")
					{
						// 1 = Divisions, 2 = Degrees
						switch (configIndex_Main.indexId)
						{
							case 1:
							{
								configIndex_1.degreeOrDivision = segmentMultiplier;
								SerialPrint(segmentMultiplier);
								eePromAddress = 192;
								break;
							}
							case 2:
							{
								configIndex_2.degreeOrDivision = segmentMultiplier;
								SerialPrint(segmentMultiplier);
								eePromAddress = 160;
								break;
							}
							case 3:
							{
								configIndex_3.degreeOrDivision = segmentMultiplier;
								SerialPrint(segmentMultiplier);
								eePromAddress = 152;
								break;
							}
							case 4:
							{
								configIndex_4.degreeOrDivision = segmentMultiplier;
								SerialPrint(segmentMultiplier);
								eePromAddress = 428;
								break;
							}
							case 5:
							{
								configIndex_5.degreeOrDivision = segmentMultiplier;
								SerialPrint(segmentMultiplier);
								eePromAddress = 440;
								break;
							}
						}
						//ToDo: Update Nextion and EEPROM
					}
					else if (commandIndex == "D") // Size
					{
						////int stepsPerRevolution = (int)round(configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle);
						int stepsPerRevolution = (int)(configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle);
						double sizeInSteps_Degrees = stepsPerRevolution * (segmentMultiplier / 360);
						int32_t sizeInSteps_Divisions = stepsPerRevolution / segmentMultiplier;

						switch (configIndex_Main.indexId)
						{
							case 1:
							{
								configIndex_1.sizeInUnits = segmentMultiplier;
								configIndex_1.sizeInSteps = sizeInSteps_Divisions;
								if (configIndex_1.degreeOrDivision == BY_DEGREES) // Degrees
								{

									configIndex_1.sizeInSteps = sizeInSteps_Degrees;
								}

								SerialWrite(0x22);
								SerialPrint(segmentMultiplier);
								SerialWrite(0x22);
								eePromAddress = 352;
								break;
							}
							case 2:
							{
								configIndex_2.sizeInUnits = segmentMultiplier;
								configIndex_2.sizeInSteps = sizeInSteps_Divisions;
								if (configIndex_2.degreeOrDivision == BY_DEGREES) // Degrees
								{

									configIndex_2.sizeInSteps = sizeInSteps_Degrees;
								}

								SerialWrite(0x22);
								SerialPrint(segmentMultiplier);
								SerialWrite(0x22);
								eePromAddress = 360;
								break;
							}
							case 3:
							{
								configIndex_3.sizeInUnits = segmentMultiplier;
								configIndex_3.sizeInSteps = sizeInSteps_Divisions;
								if (configIndex_3.degreeOrDivision == BY_DEGREES) // Degrees
								{

									configIndex_3.sizeInSteps = sizeInSteps_Degrees;
								}

								SerialWrite(0x22);
								SerialPrint(segmentMultiplier);
								SerialWrite(0x22);
								eePromAddress = 580;
								break;
							}
							case 4:
							{
								configIndex_4.sizeInUnits = segmentMultiplier;
								configIndex_4.sizeInSteps = sizeInSteps_Divisions;
								if (configIndex_4.degreeOrDivision == BY_DEGREES) // Degrees
								{

									configIndex_4.sizeInSteps = sizeInSteps_Degrees;
								}

								SerialWrite(0x22);
								SerialPrint(segmentMultiplier);
								SerialWrite(0x22);
								eePromAddress = 672;
								break;
							}
							case 5:
							{
								configIndex_5.sizeInUnits = segmentMultiplier;
								configIndex_5.sizeInSteps = sizeInSteps_Divisions;
								if (configIndex_5.degreeOrDivision == BY_DEGREES) // Degrees
								{

									configIndex_5.sizeInSteps = sizeInSteps_Degrees;
								}

								SerialWrite(0x22);
								SerialPrint(segmentMultiplier);
								SerialWrite(0x22);
								eePromAddress = 680;
								break;
							}
						}
						
					}

					SerialPrint(",");
					SerialPrint(eePromAddress);
					SerialPrint(nextionEnd);
					break;
				}


				case 79: // O - Move alternate axis out (Doesn't change for Radial or Axial)
				{
					//SerialPrint(pageProgram_t21);
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
					else // Actual
					{
						switch (configGreekKey.axisId)
						{
							case ID_AXIS_Z: // Z Axis is primary
							case ID_AXIS_B:
							{
								if (configSetup.xAltX == 0)
								{
									axisSteps = round((segmentMultiplier / configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X));
								}
								else
								{
									axisSteps = round((segmentMultiplier / configSetup.distancePerRev_AxisXAlt) * (configSetup.steps360_Axis_XAlt * configSetup.microsteps_Axis_XAlt));
								}
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
				{
					MilliDelay(segmentMultiplier * 1000);
					break;
				}
				case 81: // Q - (deprecate)Page routines
				{
					//switch (runPageID)
					//{
					//	case 3: // (deprecate)Ind
					//	{

					//		if (abs(segmentMultiplier) * moveDirection < 0)
					//		{
					//			IndexSpindle(DIR_CCW);
					//		}
					//		else
					//		{
					//			IndexSpindle(DIR_CW);
					//		}

					//		break;
					//	}
					//	case 4: // (deprecate)Move
					//	{

					//		//int moveDirection = abs(segmentMultiplier);
					//		if (abs(segmentMultiplier) * moveDirection < 0)
					//		{
					//			MoveAxis(configMove.axisId, DIR_CCW);
					//		}
					//		else
					//		{
					//			MoveAxis(configMove.axisId, DIR_CW);
					//		}

					//		break;
					//	}
					//	case 6: // (deprecate)Sync
					//	{

					//		int directionSpindle = 1;
					//		int directionAxis = 1;
					//		////if (abs(segmentMultiplier) * moveDirection < 0) // Move in (towards headstock).
					//		if ((segmentMultiplier) * moveDirection < 0) // Move in (towards headstock).
					//		{
					//			if (configSync.helixType == 1) // Right hand
					//			{
					//				directionSpindle = DIR_CW; //CW
					//				directionAxis = DIR_CCW; // CCW
					//			}
					//			else // Left hand
					//			{
					//				directionSpindle = DIR_CCW;// 0; //CCW
					//				directionAxis = DIR_CCW;// 0; // CCW	
					//			}

					//		}
					//		else // Move out (away from headstock).
					//		{
					//			//if (segmentMultiplier < 0) // Move in (towards headstock).
					//			//{
					//			if (configSync.helixType == 1) // Right hand
					//			{
					//				directionSpindle = DIR_CCW; // CW
					//				directionAxis = DIR_CW; // CW
					//			}
					//			else // Left hand
					//			{
					//				directionSpindle = DIR_CW;//0; // CCW
					//				directionAxis = DIR_CW; // CW
					//			}

					//			//}
					//		}


					//		Sync(directionSpindle, directionAxis);
					//		break;
					//	}
					//	case 7: // (deprecate)Recip
					//	{

					//		int waveDir = DIR_CW;
					//		////if (abs(segmentMultiplier) * moveDirection < 0)
					//		if ((segmentMultiplier) * moveDirection < 0)
					//		{
					//			waveDir = DIR_CCW;
					//		}

					//		switch (configRec.style)
					//		{
					//			case 1: // Triangle
					//			{
					//				Reciprocate_Triangle(waveDir);
					//				break;
					//			}
					//			case 2: // Sawtooth
					//			{
					//				Reciprocate_Sawtooth(waveDir);
					//				break;
					//			}
					//			case 3: // Square
					//			{
					//				Reciprocate_Square(waveDir);
					//				break;
					//			}
					//		}
					//		break;
					//	}
					//}


					break;
				}

				case 86:  // V - Recip page command
				{
					SerialPrint("wepo ");
					if (commandRecip == "T") // Radial or Axial
					{
						// 0 = Radial, 1 = Axial
						configRec.radial_axial = segmentMultiplier;
						SerialPrint(segmentMultiplier);
						eePromAddress = 560;
					}
					else if (commandRecip == "K") // Style
					{
						// 1 = V, 2 = sawtooth, 3 = square wave
						configRec.style = segmentMultiplier;
						SerialPrint(segmentMultiplier);
						eePromAddress = 564;
					}
					else if (commandRecip == "V") // Run Recip page
					{

						int waveDir = DIR_CW;
						////if (abs(segmentMultiplier) * moveDirection < 0)
						if ((segmentMultiplier) * moveDirection < 0)
						{
							waveDir = DIR_CCW;
						}

						switch (configRec.style)
						{
							case 1: // Triangle
							{
								Reciprocate_Triangle(waveDir);
								break;
							}
							case 2: // Sawtooth
							{
								Reciprocate_Sawtooth(waveDir);
								break;
							}
							case 3: // Square
							{
								Reciprocate_Square(waveDir);
								break;
							}
						}

					}
					else if (commandRecip == "W")
					{
						if (configRec.radial_axial == 0)//Radial
						{
							configRec.waves_Radial = segmentMultiplier;
							SerialPrint(segmentMultiplier);
							eePromAddress = 556;
						}
						else
						{
							configRec.waves_Axial = segmentMultiplier;
							SerialPrint(segmentMultiplier);
							eePromAddress = 740;
						}
						
						//ToDo: Update Nextion and EEPROM
					}
					else if (commandRecip == "S") // Spindle degrees/amplitude
					{
						if (configRec.radial_axial == 0)//Radial
						{
							configRec.degrees_Radial_Spindle = segmentMultiplier;
							eePromAddress = 308;
							SerialWrite(0x22);
							SerialPrint(segmentMultiplier);
							SerialWrite(0x22);
						}
						else
						{
							configRec.amplitude_Axial_Spindle = segmentMultiplier;
							eePromAddress = 744;
							SerialWrite(0x22);
							SerialPrint(segmentMultiplier);
							SerialWrite(0x22);
						}
						//ToDo: Update Nextion and EEPROM
					}
					else if (commandRecip == "A") // Axis distance/amplitude
					{
						if (configRec.radial_axial == 0)//Radial
						{
							configRec.amplitude_Radial_Axis = segmentMultiplier;
							eePromAddress = 232;
							SerialWrite(0x22);
							SerialPrint(segmentMultiplier);
							SerialWrite(0x22);
						}
						else
						{
							configRec.distance_Axial_Axis = segmentMultiplier;
							eePromAddress = 324;
							SerialWrite(0x22);
							SerialPrint(segmentMultiplier);
							SerialWrite(0x22);
						}
						
					}

					SerialPrint(",");
					SerialPrint(eePromAddress);
					SerialPrint(nextionEnd);
					break;
					break;
				}

				case 87: // W - Count identifier
				{
					// Count identifier
					break;
				}
				case 88: // X - X axis
				{
					selectedAxis = configGreekKey.axisId;
					configGreekKey.axisId = ID_AXIS_X;
					if (configGreekKey.segmentOrActual == 2) // 2: Segment  
					{
						stopAll = GreekKey_Move_Axis(shortSegmentStepsAxisX, segmentMultiplier, DIR_CW, true);
					}
					else  //3: Actual
					{
						if (configSetup.xAltX == 0)
						{
							axisSteps = round((segmentMultiplier / configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X));
						}
						else
						{
							axisSteps = round((segmentMultiplier / configSetup.distancePerRev_AxisXAlt) * (configSetup.steps360_Axis_XAlt * configSetup.microsteps_Axis_XAlt));
						}

						// Negative axisSteps will move CCW
						stopAll = GreekKey_Move_Axis(axisSteps, 1, DIR_CW, true);
					}

					configGreekKey.axisId = selectedAxis;
					if (stopAll)
					{
						goto EndLoops;
					}
					break;
				}
				case 90: // Z - Z axis
				{
					selectedAxis = configGreekKey.axisId;
					configGreekKey.axisId = ID_AXIS_Z;
					if (configGreekKey.segmentOrActual == 2) // 2: Segment  
					{
						stopAll = GreekKey_Move_Axis(shortSegmentStepsAxisZ, segmentMultiplier, DIR_CW, true);
					}
					else  //3: Actual
					{

						axisSteps = round((segmentMultiplier / configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z));
						// Negative axisSteps will move CCW
						stopAll = GreekKey_Move_Axis(axisSteps, 1, DIR_CW, true);
					}

					configGreekKey.axisId = selectedAxis;
					if (stopAll)
					{
						goto EndLoops;
					}

					break;
				}

				case 66: // B - B axis
				{
					selectedAxis = configGreekKey.axisId;
					configGreekKey.axisId = ID_AXIS_B;
					if (configGreekKey.segmentOrActual == 2) // 2: Segment  
					{
						stopAll = GreekKey_Move_Axis(shortSegmentStepsAxisB, segmentMultiplier, DIR_CW, true);
					}
					else  //3: Actual
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

						// Negative axisSteps will move CCW
						stopAll = GreekKey_Move_Axis(axisSteps, 1, DIR_CW, true);
					}

					configGreekKey.axisId = selectedAxis;
					if (stopAll)
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
	Serial.print("limitTriggered: ");
	if (limitTriggered || eStopTriggered)
	{
		Serial.print(limitTriggered);
		Serial.print("  ");
		Serial.println(eStopTriggered);
	}
	else
	{
		Serial.print(limitTriggered);
		Serial.print("  ");
		Serial.println(eStopTriggered);
		SerialPrint("pageProgram.va0.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageProgram.bt1.val=0");
		SerialPrint(nextionEnd);
	}

	SerialPrint("pageProgram.bt3.val=0");
	SerialPrint(nextionEnd);
	SerialPrint("pageProgram.bt2.val=0");
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

	SetEnable(ID_SPINDLE, false, true);
	SetEnable(ID_AXIS_Z, false, true);
	SetEnable(ID_AXIS_X, false, true);

	// 0 = Change to disabled, 1 = keep enabled
	if (configSetup.keepSteppersEnabled == 1)
	{
		SerialPrint("pageProgram.bt10.val=1"); // 0 = enabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageProgram.bt11.val=1"); // 0 = enabled
		SerialPrint("\xFF\xFF\xFF");
	}
	else
	{
		SerialPrint("pageProgram.bt10.val=0"); // 1 = disabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageProgram.bt10.txt="); // 1 = disabled
		SerialPrint("\x22");
		SerialPrint("Disabled");
		SerialPrint("\x22\xFF\xFF\xFF");

		SerialPrint("pageProgram.bt11.val=0"); // 1 = disabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageProgram.bt11.txt="); // 1 = disabled
		SerialPrint("\x22");
		SerialPrint("Disabled");
		SerialPrint("\x22\xFF\xFF\xFF");
	}
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
		Serial.print("input: ");
		Serial.println(input);
		// Use 255 as end chars
		if (input == 255)
		{

			if (pageCallerId == PAGE_INDEX) // pageIndex
			{
				EEPROM.write(eePromAddress_Filename_Ind + i, 0);
			}
			else // pageProgram
			{
				EEPROM.write(eePromAddress_Filename_Program + i, 0);
			}
			break;
		}

		if (pageCallerId == PAGE_INDEX) // pageIndex
		{
			EEPROM.write(eePromAddress_Filename_Ind + i, input);
		}
		else // pageProgram
		{
			EEPROM.write(eePromAddress_Filename_Program + i, input);
			Serial.print("input-2: ");
			Serial.println(input);
		}

	}

	/// ////
	char arChar[20];
	int j;
	for (j = 0; j <= 12; j++)
	{
		arChar[j] = EEPROM.read(j + eePromAddress_Filename_Program);
		Serial.print("arChar[j]:  ");
		Serial.println(arChar[j]);
	}


	Serial.print("Filename: ");
	Serial.println(arChar);
	Serial.println(input);
	filename_Program = arChar;
	Serial.println(filename_Program);
	//////////////////
	if (pageCallerId == PAGE_INDEX) // pageIndex
	{
		EEPROM.put(eePromAddress_Filename_Length_Ind, i);
	}
	else // pageProgram
	{
		EEPROM.put(eePromAddress_Filename_Length_Program, i);
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

#ifdef DEBUG
	Serial.print("00000000000000000LineNumber :  ");
	Serial.println(lineNumber);

#endif // DEBUG

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
		Serial.println("File not found-01.");
		Serial.println("Exit Index.");
#endif // DEBUG
		

		SerialPrint("pageIndex.tFileName.txt=");
		SerialWrite(0x22);
		SerialPrint("File not found-02");
		SerialPrint(nextionQuoteEnd);
		return 0;
	}

	MilliDelay(100);
	while (sdFile.available())
	{
		newSizeString = sdFile.readStringUntil('\n');

		if (currentLineNumber == lineNumber)
		{
#ifdef DEBUG
			Serial.print("==------------=LineNumber :  ");
			Serial.println(lineNumber);

#endif // DEBUG
			break;
		}
		currentLineNumber++; // Increment the line number
	}

	// Close the file
	sdFile.close();

	// Return size string as double
	const char *newSizeChar = static_cast<const char*>(newSizeString.c_str());
#ifdef DEBUG
	Serial.print("^^^^newSizeChar :  ");
	Serial.println(newSizeChar);

#endif // DEBUG
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
	EEPROM.get(eePromAddress_Filename_Length_Program, filename_Length);

	char arChar[20];
	int i;
	for (i = 0; i <= filename_Length; i++)
	{
		arChar[i] = EEPROM.read(i + eePromAddress_Filename_Program);
		Serial.print("arChar[i]:  ");
		Serial.println(arChar[i]);
	}
	arChar[i++] = 0;

	// Open file
	Serial.print("arChar:  ");
	Serial.println(arChar);
	File sdFile = SD.open(arChar, FILE_READ);


	//const char* fileName = filename_Program.c_str();
	//File sdFile = SD.open(fileName, FILE_READ);

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
		Serial.print("**********************************1.Line#+1: ");
		Serial.println(currentLineNumber);
	Serial.print("1.moveType: ");
	Serial.println(moveType);
	Serial.print("1.currentCommand: ");
	Serial.println(currentCommand);
	Serial.print("1.nStr: ");
	Serial.println(nStr);
#endif

	switch (moveType)
	{
		
		case 59:// ; - Comment
		{
	#ifdef DEBUG
			Serial.println("MoveType 59");
	#endif // DEBUG

			comment = nStr;
			return 0;
			break;
		}
		case 13: // Empty line
		case 65: // A
		case 66: // B
		case 67: // C
		case 69: // E
		case 73: // I
		case 79: // O
		case 80: // P
		case 83: // S
		case 88: // X
		case 90: // Z
		{
			// Valid entries
			//return 0;
			break;
		}
		case 70:// F - Move page settings
		{
			commandMove = newSizeString[1];

			nStr = newSizeString.substring(2);
#ifdef DEBUG
			Serial.print("page 70: ");
			Serial.println(commandMove);
			Serial.print("nStr 70: ");
			Serial.println(nStr);
#endif // DEBUG
			break;
		} 

		case 71:// G - Sync page settings
		{
			commandSync = newSizeString[1];

			nStr = newSizeString.substring(2);
#ifdef DEBUG
			Serial.print("page 71: ");
			Serial.println(commandSync);
			Serial.print("nStr 71: ");
			Serial.println(nStr);
#endif // DEBUG
			break;
		}

		case 72:// H and V
		{
			// Horizontal and Vertical move combined.
			// Parse nStr 
			//String delimiter = "V";
			//String horizontal = nStr.substring(0,nStr.find)
			//String h = split(nStr, "V");
			char* val;
			char charBuf[20];
			nStr.toCharArray(charBuf, 20);
			val = strtok(charBuf, "V");
			hv_AxisLegLength = atof(val);
			val = strtok(NULL, "\n");
			hv_SpindleLegLength = atof(val);
#ifdef DEBUG
			Serial.print("nStr: ");
			Serial.print(nStr);
			Serial.print("   hVal: ");
			Serial.print(hv_AxisLegLength);
			//Serial.print("val: ");
			//Serial.println(val);
			Serial.print("   vVal: ");
			Serial.println(hv_SpindleLegLength);
#endif // DEBUG
			break;
		}

		case 74:// J - BRadius
		{
			commandBRadius = newSizeString[1];
			nStr = newSizeString.substring(2);
#ifdef DEBUG
			Serial.print("page 74: ");
			Serial.println(commandSync);
			Serial.print("nStr 74: ");
			Serial.println(nStr);
#endif // DEBUG
			break;
		}

		case 75:// K - Enable/DisableStepper
		{
			enableDisableStepperID = newSizeString[1];

			nStr = newSizeString.substring(2);
#ifdef DEBUG
			Serial.print("enable/disable stepper ID: ");
			Serial.println(enableDisableStepperID);
			Serial.print("nStr value: ");
			Serial.println(nStr);
#endif // DEBUG
			break;
		}

		case 78:// N - Index page settings
		{
			commandIndex = newSizeString[1];

			nStr = newSizeString.substring(2);
#ifdef DEBUG
			Serial.print("page 78: ");
			Serial.println(commandIndex);
			Serial.print("nStr 78: ");
			Serial.println(nStr);
#endif // DEBUG
			break;
		}


		case 81:// Q - Run page functions -No longer valid
		{
			badCommand = true;
			Serial.print("================================Bad Command: ");
			Serial.println(badCommand);
			return 0;
			////		String page = newSizeString[1];
			////		if (page == "I")
			////		{
			////			runPageID = 3;
			////		}
			////		else if (page == "M")
			////		{
			////			runPageID = 4;
			////		}
			////		else if (page == "S")
			////		{
			////			runPageID = 6;
			////		}
			////		else if (page == "R")
			////		{
			////			runPageID = 7;
			////		}
			////
			////
			////
			////		nStr = newSizeString.substring(2);
			////#ifdef DEBUG
			////		Serial.print("page 81: ");
			////		Serial.println(page);
			////		Serial.print("nStr 81: ");
			////		Serial.println(nStr);
			////#endif // DEBUG

			break;
		}

		case 86:// V - Recip page settings
		{

			commandRecip = newSizeString[1];

			nStr = newSizeString.substring(2);
#ifdef DEBUG
			Serial.print("page 86: ");
			Serial.println(commandRecip);
			Serial.print("nStr 86: ");
			Serial.println(nStr);
#endif // DEBUG
			break;
		}

		case 87:// W - Spindle and Axis segment count
		{
			String countType = newSizeString[1];
			if (countType == "S")
			{
				moveType = 1;
#ifdef DEBUG
				Serial.print("countType S: ");
				Serial.print(moveType);
				Serial.print("  -  ");
				Serial.println(nStr);
#endif // DEBUG
			}
			else if (countType == "A")
			{
				moveType = 2;
			}
			nStr = newSizeString.substring(2);
#ifdef DEBUG
			Serial.print("countType A: ");
			Serial.print(moveType);
			Serial.print("  -  ");
			Serial.println(nStr);
#endif // DEBUG
			break;
		}

		default:
		{
			badCommand = true;
			Serial.print("================================Bad Command2: ");
			Serial.println(badCommand);
			return 0;
		}
	}


	/*
	if (moveType == 59) // ;
	{
#ifdef DEBUG
		Serial.println("MoveType 59");
#endif // DEBUG

		comment = nStr;
		return 0;
	}

	if (moveType == 87) // W - Spindle and Axis segment count
	{
		String countType = newSizeString[1];
		if (countType == "S")
		{
			moveType = 1;
#ifdef DEBUG
			Serial.print("countType S: ");
			Serial.print(moveType);
			Serial.print("  -  ");
			Serial.println(nStr);
#endif // DEBUG
		}
		else if (countType == "A")
		{
			moveType = 2;
		}
		nStr = newSizeString.substring(2);
#ifdef DEBUG
		Serial.print("countType A: ");
		Serial.print(moveType);
		Serial.print("  -  ");
		Serial.println(nStr);
#endif // DEBUG
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
		hv_AxisLegLength = atof(val);
		val = strtok(NULL, "\n");
		hv_SpindleLegLength = atof(val);
#ifdef DEBUG
		Serial.print("nStr: ");
		Serial.print(nStr);
		Serial.print("   hVal: ");
		Serial.print(hv_AxisLegLength);
		//Serial.print("val: ");
		//Serial.println(val);
		Serial.print("   vVal: ");
		Serial.println(hv_SpindleLegLength);
#endif // DEBUG

	}

	if (moveType == 70) // E - Move page settings
	{
		commandMove = newSizeString[1];

		nStr = newSizeString.substring(2);
#ifdef DEBUG
		Serial.print("page 70: ");
		Serial.println(commandMove);
		Serial.print("nStr 70: ");
		Serial.println(nStr);
#endif // DEBUG

	}

	if (moveType == 71) // G - Sync page settings
	{
		commandSync = newSizeString[1];

		nStr = newSizeString.substring(2);
#ifdef DEBUG
		Serial.print("page 71: ");
		Serial.println(commandSync);
		Serial.print("nStr 71: ");
		Serial.println(nStr);
#endif // DEBUG

	}

	if (moveType == 74) // J - BRadius
	{
		commandBRadius = newSizeString[1];
		nStr = newSizeString.substring(2);
#ifdef DEBUG
		Serial.print("page 74: ");
		Serial.println(commandSync);
		Serial.print("nStr 74: ");
		Serial.println(nStr);
#endif // DEBUG
	}

	if (moveType == 78) // N - Index page settings
	{
		commandIndex = newSizeString[1];

		nStr = newSizeString.substring(2);
#ifdef DEBUG
		Serial.print("page 78: ");
		Serial.println(commandIndex);
		Serial.print("nStr 78: ");
		Serial.println(nStr);
#endif // DEBUG

	}

	if (moveType == 81) // Q - Run page functions
	{
		badCommand = true;
		return -1;
////		String page = newSizeString[1];
////		if (page == "I")
////		{
////			runPageID = 3;
////		}
////		else if (page == "M")
////		{
////			runPageID = 4;
////		}
////		else if (page == "S")
////		{
////			runPageID = 6;
////		}
////		else if (page == "R")
////		{
////			runPageID = 7;
////		}
////
////
////
////		nStr = newSizeString.substring(2);
////#ifdef DEBUG
////		Serial.print("page 81: ");
////		Serial.println(page);
////		Serial.print("nStr 81: ");
////		Serial.println(nStr);
////#endif // DEBUG

	}

	if (moveType == 86)  // V - Recip page settings
	{

		commandRecip = newSizeString[1];

		nStr = newSizeString.substring(2);
#ifdef DEBUG
		Serial.print("page 86: ");
		Serial.println(commandRecip);
		Serial.print("nStr 86: ");
		Serial.println(nStr);
#endif // DEBUG
	}
	*/
	// Return size string as double
	const char *newSizeChar = static_cast<const char*>(nStr.c_str());

	retVal = atof(newSizeChar);

#ifdef DEBUG
	Serial.print("atof(nStr):");
	Serial.println(nStr);
	Serial.print("newSizeChar:");
	Serial.println(newSizeChar);
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

	// Get shortest path to the spindle start position.
	int32_t target_Spindle = returnSteps_Spindle % spindleStepsPerRev * (-1);

	StepControl stepController;
	Stepper stepper_Spindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
	Stepper stepper_Z(PIN_AXIS_Z_STEP, PIN_AXIS_Z_DIR);
	Stepper stepper_X(PIN_AXIS_X_STEP, PIN_AXIS_X_DIR);
	Stepper stepper_B(PIN_AXIS_B_STEP, PIN_AXIS_B_DIR);

	// Spindle uses relative position to start from.
	stepper_Spindle.setPosition(0);

	MilliDelay(15);
	stepper_Spindle
		.setMaxSpeed(configSetup.maxSpd_Return_Spindle)
		.setAcceleration(configSetup.accel_Return_Spindle)
		.setTargetRel(target_Spindle);

#ifdef DEBUG
	Serial.println("Begin ReturnToStartPosition()");
	Serial.print("target_Spindle: ");
	Serial.println(target_Spindle);
	Serial.print("returnPositionAbs_Axis: ");
	Serial.println(startPositionAbs_Axis);
	Serial.print("endPosition_Axis: ");
	Serial.println(endPosition_Axis);
#endif // DEBUG

	SetEnable(ID_SPINDLE, true, true);
	switch (axisId)
	{
	case ID_AXIS_Z:
	{
		// Axis uses absolute position
		// Ensure axis position is set to end position of last routine.
		stepper_Z.setPosition(endPosition_Axis);
		MilliDelay(20);
		Serial.print("Z position: ");
		Serial.println(stepper_Z.getPosition());
		stepper_Z
			.setMaxSpeed(configSetup.maxSpd_Return_Axis_Z)
			.setAcceleration(configSetup.accel_Return_Axis_Z)
			.setTargetAbs(startPositionAbs_Axis);

#ifdef DEBUG
		Serial.print("maxSpd_Return_Axis_Z: ");
		Serial.println(configSetup.accel_Return_Axis_Z);
#endif // DEBUG
		SetEnable(ID_AXIS_Z, true, true);
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
		// Axis uses absolute position
		// Ensure axis position is set to end position of last routine.
		stepper_X.setPosition(endPosition_Axis);
		MilliDelay(20);
#ifdef DEBUG
		Serial.print("X position: ");
		Serial.println(stepper_X.getPosition());
#endif // Debug
		stepper_X
			.setMaxSpeed(configSetup.maxSpd_Return_Axis_X)
			.setAcceleration(configSetup.accel_Return_Axis_X)
			.setTargetAbs(startPositionAbs_Axis);
#ifdef DEBUG
		Serial.print("maxSpd_Return_Axis_X: ");
		Serial.println(configSetup.accel_Return_Axis_X);
#endif // DEBUG

		SetEnable(ID_AXIS_X, true, true);
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
		// Axis uses absolute position
		// Ensure axis position is set to end position of last routine.
		stepper_B.setPosition(endPosition_Axis);
		MilliDelay(20);
#ifdef DEBUG
		Serial.print("B position: ");
		Serial.println(stepper_B.getPosition());
#endif // Debug
		stepper_B
			.setMaxSpeed(configSetup.maxSpd_Return_Axis_B)
			.setAcceleration(configSetup.accel_Return_Axis_B)
			.setTargetAbs(startPositionAbs_Axis);
#ifdef DEBUG
		Serial.print("maxSpd_Return_Axis_B: ");
		Serial.println(configSetup.accel_Return_Axis_B);
#endif // DEBUG
		SetEnable(ID_AXIS_B, true, true);
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
		if (configSetup.eStop != 0)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
				Serial.println("                          EStop");
				stepController.emergencyStop();
				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_B, false, false);
				SetEStopColors(PAGE_RETURN);
				return;
			}
		}

		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
				case 83: // - S
				{
					stepController.stop();
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
			// 0 = Change to disabled, 1 = keep enabled
			if (configSetup.keepSteppersEnabled == 1)
			{
				SerialPrint("pageMain.bt10.val=1"); // 0 = enabled
				SerialPrint("\xFF\xFF\xFF");
			}
			else
			{
				SerialPrint("pageMain.bt10.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageMain.bt10.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
			}
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
			// 0 = Change to disabled, 1 = keep enabled
			if (configSetup.keepSteppersEnabled == 1)
			{
				SerialPrint("pageMain.bt11.val=1"); // 0 = enabled
				SerialPrint("\xFF\xFF\xFF");
			}
			else
			{
				SerialPrint("pageMain.bt11.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageMain.bt11.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
			}
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

		//ToDo: ******************
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

	case PAGE_ROSE:
	{
		SerialPrint("pageRose.bt1.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageRose.bt1.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageRose.bt0.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageRose.va0.val=0");
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
	case PAGE_PROGRAM:
	{
		SerialPrint("pageProgram.bt1.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageProgram.bt1.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageProgram.va0.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageProgram.bt0.val=0");
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
			break;
		}
		case ID_AXIS_X:
		{
			stepper_X.setPosition(0);
			endPosition_Axis = 0;
			break;
		}
		case ID_AXIS_B:
		{
			stepper_B.setPosition(0);
			endPosition_Axis = 0;
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
		returnSteps_Spindle = 0;
	}

	SetEnable(ID_SPINDLE, false, true);
	SetEnable(ID_AXIS_Z, false, true);
	SetEnable(ID_AXIS_X, false, true);
	SetEnable(ID_AXIS_B, false, true);
}

/// <summary>
/// Return Spindle and/or axes to start position
/// </summary>
/// <comment>
/// </comment>
/// <param name="axisId">Id of axis to move</param>
void ReturnToStartPosition_MovePage(int axisId)
{
	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	const char* nextionEnd = "\xFF\xFF\xFF";

	StepControl stepController;
	Stepper stepper_Z(PIN_AXIS_Z_STEP, PIN_AXIS_Z_DIR);
	Stepper stepper_X(PIN_AXIS_X_STEP, PIN_AXIS_X_DIR);
	Stepper stepper_B(PIN_AXIS_B_STEP, PIN_AXIS_B_DIR);


#ifdef DEBUG
	Serial.println("Begin ReturnToStartPosition()");
	Serial.print("returnPositionAbs_Axis: ");
	Serial.println(startPositionAbs_Axis);
	Serial.print("endPosition_Axis: ");
	Serial.println(endPosition_Axis);
#endif // DEBUG

	switch (axisId)
	{
		case ID_MOVE_AXIS_Z2:
		case ID_AXIS_Z:
		{
			// Axis uses absolute position
			// Ensure axis position is set to end position of last routine.
			stepper_Z.setPosition(endPosition_Axis);
			//stepper_Z.setPosition(0);


			MilliDelay(20);
			Serial.print("Z position: ");
			Serial.println(stepper_Z.getPosition());
			stepper_Z
				.setMaxSpeed(configSetup.maxSpd_Return_Axis_Z)
				.setAcceleration(configSetup.accel_Return_Axis_Z)
				.setTargetAbs(0);
				//.setTargetAbs(startPositionAbs_Axis);

	#ifdef DEBUG
			Serial.print("maxSpd_Return_Axis_Z: ");
			Serial.println(configSetup.accel_Return_Axis_Z);
	#endif // DEBUG
			SetEnable(ID_AXIS_Z, true, true);

				// Only move the Z Axis
				stepController.moveAsync(stepper_Z);

			break;
		}
		case ID_MOVE_AXIS_X2:
		case ID_AXIS_X:
		{
			// Axis uses absolute position
			// Ensure axis position is set to end position of last routine.
			stepper_X.setPosition(endPosition_Axis);
			//stepper_X.setPosition(0);
			MilliDelay(20);
	#ifdef DEBUG
			Serial.print("X position: ");
			Serial.println(stepper_X.getPosition());
	#endif // Debug
			stepper_X
				.setMaxSpeed(configSetup.maxSpd_Return_Axis_X)
				.setAcceleration(configSetup.accel_Return_Axis_X)
				.setTargetAbs(0);
			//.setTargetAbs(startPositionAbs_Axis);
	#ifdef DEBUG
			Serial.print("maxSpd_Return_Axis_X: ");
			Serial.println(configSetup.accel_Return_Axis_X);
	#endif // DEBUG

			SetEnable(ID_AXIS_X, true, true);

				// Only move the X Axis
				stepController.moveAsync(stepper_X);


			break;
		}
		case ID_MOVE_AXIS_B2:
		case ID_AXIS_B:
		{
			// Axis uses absolute position
			// Ensure axis position is set to end position of last routine.
			stepper_B.setPosition(endPosition_Axis);
			//stepper_B.setPosition(0);
			MilliDelay(20);
	#ifdef DEBUG
			Serial.print("B position: ");
			Serial.println(stepper_B.getPosition());
	#endif // Debug
			stepper_B
				.setMaxSpeed(configSetup.maxSpd_Return_Axis_B)
				.setAcceleration(configSetup.accel_Return_Axis_B)
				.setTargetAbs(0);
			//.setTargetAbs(startPositionAbs_Axis);
	#ifdef DEBUG
			Serial.print("maxSpd_Return_Axis_B: ");
			Serial.println(configSetup.accel_Return_Axis_B);
	#endif // DEBUG
			SetEnable(ID_AXIS_B, true, true);
				stepController.moveAsync(stepper_B);

			break;
		}
	}

	while (stepController.isRunning())
	{
		if (configSetup.eStop != 0)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
				Serial.println("                          EStop");
				stepController.emergencyStop();
				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_B, false, false);
				SetEStopColors(PAGE_RETURN);
				return;
			}
		}

		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
			case 83: // - S
			{
				stepController.stop();
				break;
			}
			}
		}

		MilliDelay(10);
	}

	switch (axisId)
	{
		case ID_MOVE_AXIS_Z2:
		case ID_AXIS_Z:
		{
			endPosition_Axis = stepper_Z.getPosition();
			SetEnable(ID_AXIS_Z, false, true);
			break;
		}
		case ID_MOVE_AXIS_X2:
		case ID_AXIS_X:
		{
			endPosition_Axis = stepper_X.getPosition();
			SetEnable(ID_AXIS_X, false, true);
			break;
		}
		case ID_MOVE_AXIS_B2:
		case ID_AXIS_B:
		{
			endPosition_Axis = stepper_B.getPosition();
			SetEnable(ID_AXIS_B, false, true);
			break;
		}
	}


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

		stepper_X.setPosition(0);
		stepper_Z.setPosition(0);
		stepper_B.setPosition(0);
		endPosition_Axis = 0;


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
	//return slideStepsAmplitude * cosf(phi);
	float retVal = slideStepsAmplitude * cosf(phi);
	//float altVal = slideStepsAmplitude * (pow(sin(2.8 * spindleAngle * kRatio), 2) + cos(pow(2 * spindleAngle * kRatio, 2)));
	//Serial.println(altVal);
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

	//float phi = fmodf(spindleAngle * kRatio, TWO_PI);
	float slideTarget = slideFunc_Axis(spindleAngle);//slideStepsAmplitude * cosf(phi);//

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
	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	const char* nextionEnd = "\xFF\xFF\xFF";
	int limitPin_Max = configSetup.limit_Max_Z;
	int limitPin_Min = configSetup.limit_Min_Z;
	limitTriggered = false;
	bool runPattern = true;
	int32_t spindleCurrentPosition = 0;
	IntervalTimer tickTimer;
	//RotateControl controllerRoseSpindle;
	StepControl controllerRoseSpindle;

	kRatio = (float)configRose.n / configRose.d;

	// Initialize variables to 0
	returnSteps_Spindle = 0;
	slideStepsAmplitude = 0;
	startPositionAbs_Axis = 0;

	// Get spindle steps per revolution and max speed
	spindleStepsPerRev = configSetup.gearRatio_Spindle * configSetup.microsteps_Spindle * configSetup.steps360_Spindle;
	int32_t spindleMaxRotation = configRose.spindleRevolutions*spindleStepsPerRev;
	newMaxSpd_RoseSpindle = configRose.maxSpd_Spindle * configRose.speedPercent_Spindle * .01;// *direction;
	volatile int32_t spindleTarget = 2147000000 * direction;
#ifdef DEBUG
	Serial.print("Spindle Revolutions: ");
	Serial.println(configRose.spindleRevolutions);
	Serial.print("spindleTarget: ");
	Serial.println(spindleTarget);
#endif // Debug
	// Set spindle max speed and acceleration
	stepperSpindle
		.setMaxSpeed(newMaxSpd_RoseSpindle)
		.setAcceleration(configRose.accel_Spindle)
		.setPosition(0);
	stepperSpindle.setTargetRel(spindleTarget);

	// Enable spindle
	SetEnable(ID_SPINDLE, true, true);

	switch (configRose.axisId)
	{
		case ID_AXIS_Z:
		{
			limitPin_Max = configSetup.limit_Max_Z;
			limitPin_Min = configSetup.limit_Min_Z;
			// Get Z axis amplitude steps and max speed
			//slideStepsAmplitude = ((configRose.amplitude_Radial_Z / (configSetup.distancePerRev_AxisZ)) * configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z) / 2;  // Amplitude is normally measured from the middle to the top
			//newMaxSpd_RoseAxis = configRose.maxSpd_Axis_Z * configRose.speedPercent_Axis_Z * .01 * direction;

			slideStepsAmplitude = direction * (((configRose.amplitude_Radial_Z / (configSetup.distancePerRev_AxisZ)) * configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z) / 2);  // Amplitude is normally measured from the middle to the top
			newMaxSpd_RoseAxis = configRose.maxSpd_Axis_Z * configRose.speedPercent_Axis_Z * .01;


			stepperAxis_Z.setPosition(0);
#ifdef DEBUG
			Serial.print("Z Initial position:  ");
			Serial.println(stepperAxis_Z.getPosition());
			Serial.print("Z Max position:  ");
			Serial.println(slideStepsAmplitude);

			// Use Absolute position for Return function
			// slideFunc_Axis initial call sets axis position to max amplitude (slideStepsAmplitude).
			Serial.print("returnPositionAbs_Axis:  ");
#endif // Debug
			startPositionAbs_Axis = slideStepsAmplitude;
#ifdef DEBUG
			Serial.print(startPositionAbs_Axis);
#endif // Debug
			// Set Z axis max speed and acceleration
			stepperAxis_Z
				.setMaxSpeed(newMaxSpd_RoseAxis)
				.setAcceleration(configRose.accel_Axis_Z)
				.setPosition(slideFunc_Axis(0)); 
#ifdef DEBUG
			Serial.print("Z Initial position_2:  ");
			Serial.println(stepperAxis_Z.getPosition());
#endif // Debug
			SetEnable(ID_AXIS_Z, true, true);
			controllerAxis.rotateAsync(stepperAxis_Z);
			break;
		}
		case ID_AXIS_X:
		{
			limitPin_Max = configSetup.limit_Max_X;
			limitPin_Min = configSetup.limit_Min_X;
			if (configSetup.xAltX == 0)
			{
				slideStepsAmplitude = direction * (((configRose.amplitude_Radial_X / (configSetup.distancePerRev_AxisX)) * configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X) / 2);  // Amplitude is normally measured from the middle to the top
			}
			else
			{
				slideStepsAmplitude = direction * (((configRose.amplitude_Radial_X / (configSetup.distancePerRev_AxisXAlt)) * configSetup.steps360_Axis_XAlt * configSetup.microsteps_Axis_XAlt) / 2);  // Amplitude is normally measured from the middle to the top
			}
			newMaxSpd_RoseAxis = configRose.maxSpd_Axis_X * configRose.speedPercent_Axis_X * .01;

			// Use Absolute position for Return function
			// slideFunc_Axis initial call sets axis position to max amplitude (slideStepsAmplitude).
			startPositionAbs_Axis = slideStepsAmplitude;	
#ifdef DEBUG
			Serial.print("returnPositionAbs_Axis:  ");
			Serial.print(startPositionAbs_Axis);
#endif // Debug
			stepperAxis_X
				.setMaxSpeed(newMaxSpd_RoseAxis)
				.setAcceleration(configRose.accel_Axis_X)
				.setPosition(slideFunc_Axis(0)); // set start position of counter	
			SetEnable(ID_AXIS_X, true, true);
			controllerAxis.rotateAsync(stepperAxis_X);
			break;
		}
		case ID_AXIS_B:
		{
			limitPin_Max = configSetup.limit_Max_B;
			limitPin_Min = configSetup.limit_Min_B;
			switch (configSetup.radialOrLinear_Axis_B)
			{
			case RADIAL_B:
			{
				slideStepsAmplitude = direction * (DistanceToSteps_RadialB(configRose.amplitude_Radial_B) / 2);
				break;
			}
			case LINEAR_B:
			{
				slideStepsAmplitude = direction * (DistanceToSteps_LinearB(configRose.amplitude_Radial_B) / 2);
				break;
			}
			}

			newMaxSpd_RoseAxis = configRose.maxSpd_Axis_B * configRose.speedPercent_Axis_B * .01;

			// Use Absolute position for Return function
			// slideFunc_Axis initial call sets axis position to max amplitude (slideStepsAmplitude).
			startPositionAbs_Axis = slideStepsAmplitude;
#ifdef DEBUG
			Serial.print("returnPositionAbs_Axis:  ");
			Serial.print(startPositionAbs_Axis);
#endif // Debug
			stepperAxis_B
				.setMaxSpeed(newMaxSpd_RoseAxis)
				.setAcceleration(configRose.accel_Axis_B)
				.setPosition(slideFunc_Axis(0)); // set start position of counter
			SetEnable(ID_AXIS_B, true, true);
			controllerAxis.rotateAsync(stepperAxis_B);
			break;
		}
	}

	// Set limit switches HIGH
	digitalWrite(limitPin_Max, HIGH);
	digitalWrite(limitPin_Min, HIGH);

	//controllerRoseSpindle.rotateAsync(stepperSpindle); // let the spindle run with constant speed
	controllerRoseSpindle.moveAsync(stepperSpindle);
	controllerAxis.overrideSpeed(0);

	tick();

	// use a timer to periodically calculate new targets for the slide
	tickTimer.priority(priorityLevel); // lowest priority, potentially long caclulations need to be interruptable by TeensyStep
	tickTimer.begin(tick, recalcPeriod);
#ifdef DEBUG
	Serial.print("spindleMaxRotation:");
	Serial.println(spindleMaxRotation);
#endif // Debug
	while (runPattern)
	{
		spindleCurrentPosition = stepperSpindle.getPosition();
		if (spindleCurrentPosition < 0)
		{
			spindleCurrentPosition *= (-1);
		}
#ifdef DEBUG
		//Serial.println(spindleCurrentPosition);
#endif // Debug
		if (spindleMaxRotation > 0)
		{
			if (spindleCurrentPosition >= spindleMaxRotation)
			{
				tickTimer.end();
				controllerAxis.overrideSpeed(0);
				controllerAxis.stopAsync();
				controllerRoseSpindle.stopAsync();

				MilliDelay(2000);
				runPattern = false;
			}
		}

		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
				case 83: // - S
				{
					tickTimer.end();
					//controllerRoseSpindle.overrideSpeed(0);
					controllerAxis.overrideSpeed(0);
					controllerAxis.stopAsync();
					controllerRoseSpindle.stopAsync();

					MilliDelay(2000);
					runPattern = false;
					break;
				}
			}
		}

		///////////////////////////////////////////

		if ((digitalRead(limitPin_Max) != configSetup.limit_NCorNO))// && axisSteps > 0)
		{
			Serial.println("                          limitPin_Max");
			tickTimer.end();
			controllerAxis.overrideSpeed(0);
			controllerAxis.emergencyStop();
			controllerRoseSpindle.emergencyStop();
			SetLimitColors(PAGE_ROSE, DIR_CW);
			MilliDelay(2000);
			runPattern = false;
		}
		else if ((digitalRead(limitPin_Min) != configSetup.limit_NCorNO))// && axisSteps < 0)
		{
			Serial.println("                          limitPin_Min");
			tickTimer.end();
			controllerAxis.overrideSpeed(0);
			controllerAxis.emergencyStop();
			controllerRoseSpindle.emergencyStop();
			SetLimitColors(PAGE_ROSE, DIR_CCW);
			MilliDelay(2000);
			runPattern = false;
		}

		if (configSetup.eStop != 0)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
				Serial.println("                          EStop ");
				controllerAxis.emergencyStop();
				controllerRoseSpindle.emergencyStop();
				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_B, false, false);
				SetEStopColors(PAGE_ROSE);
				return;
			}
		}

		MilliDelay(20);
	} // End while

	// endPosition_Spindle and endPosition_Axis may be used in future for reporting purposes
	// Store current spindle position for possible use in Return to 0 method
	endPosition_Spindle = stepperSpindle.getPosition();
	returnSteps_Spindle = endPosition_Spindle;

	// Set spindle position to 0
	stepperSpindle.setPosition(0);
#ifdef DEBUG
	Serial.println("===============");
	Serial.print("Spindle Reset Position: ");
	Serial.println(stepperSpindle.getPosition());
#endif // Debug
	// Disable spindle
	SetEnable(ID_SPINDLE, false, true);

	switch (configRose.axisId)
	{
		case ID_AXIS_Z:
		{
			// Store Z axis position for possible use in Return to 0 method
			endPosition_Axis = stepperAxis_Z.getPosition();
#ifdef DEBUG
			Serial.print("Z position:  ");
			Serial.println(stepperAxis_Z.getPosition());
#endif // Debug
			// Disable Z axis
			SetEnable(ID_AXIS_Z, false, true);
			break;
		}
		case ID_AXIS_X:
		{
			endPosition_Axis = stepperAxis_X.getPosition();
			SetEnable(ID_AXIS_X, false, true);
			break;
		}
		case ID_AXIS_B:
		{
			endPosition_Axis = stepperAxis_B.getPosition();
			SetEnable(ID_AXIS_B, false, true);
			break;
		}
	}

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

	SerialPrint("pageRose.bt3.val=0");// The first one may be ignored by Nextion, so resend.
	SerialPrint(nextionEnd);
	SerialPrint("pageRose.bt2.val=0");
	SerialPrint(nextionEnd);

	if (!limitTriggered)
	{
		SerialPrint("pageRose.va0.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageRose.bt1.val=0");
		SerialPrint(nextionEnd);
	}

#ifdef DEBUG
	Serial.print("returnSteps_Spindle:");
	Serial.println(returnSteps_Spindle);
	Serial.print("endPosition_Axis: ");
	Serial.println(endPosition_Axis);
	Serial.println("---------------");
#endif // DEBUG
	// 0 = Change to disabled, 1 = keep enabled
	if (configSetup.keepSteppersEnabled == 1)
	{
		SerialPrint("pageRose.bt10.val=1"); // 0 = enabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageRose.bt11.val=1"); // 0 = enabled
		SerialPrint("\xFF\xFF\xFF");
	}
	else
	{
		SerialPrint("pageRose.bt10.val=0"); // 1 = disabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageRose.bt10.txt="); // 1 = disabled
		SerialPrint("\x22");
		SerialPrint("Disabled");
		SerialPrint("\x22\xFF\xFF\xFF");

		SerialPrint("pageRose.bt11.val=0"); // 1 = disabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageRose.bt11.txt="); // 1 = disabled
		SerialPrint("\x22");
		SerialPrint("Disabled");
		SerialPrint("\x22\xFF\xFF\xFF");
	}
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
void TestEEPROMConfig(int callerId)
{
#ifdef DEBUG
	Serial.println("Enter TestEEPROMConfig");
#endif // Debug
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
#ifdef DEBUG
			Serial.println("EEProm Read");
#endif // Debug
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
		case PAGE_ROSE:
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
		case PAGE_PROGRAM:
		{
			configPageGreekKey eePageGrk;

			EEPROM.get(eePromAddress_Grk, eePageGrk);
			SerialPrint(pageConfig_t10);
			SerialWrite(0x22);
			SerialPrint(eePageGrk.maxSpd_Spindle);
			SerialPrint(nextionQuoteEnd);

			SerialPrint(pageConfig_t11);
			SerialWrite(0x22);
			SerialPrint(eePageGrk.accel_Spindle);
			SerialPrint(nextionQuoteEnd);
#ifdef DEBUG
			Serial.print("Spindle Accel: ");
			Serial.println(configGreekKey.accel_Spindle);
			Serial.print("Spindle Accel-eeprom: ");
			Serial.println(eePageGrk.accel_Spindle);
#endif // Debug
			switch (configGreekKey.axisId)
			{
				case ID_AXIS_Z:
				{
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

	// Radial
	const char* pageB_c0_0 = "pageB.c0.val=0";
	const char* pageB_c0_1 = "pageB.c0.val=1";

	// Linear
	const char* pageB_c1_0 = "pageB.c1.val=0";
	const char* pageB_c1_1 = "pageB.c1.val=1";

	const char* pageX_t60 = "pageX.t60.txt=";
	const char* pageX_t61 = "pageX.t61.txt=";
	const char* pageX_t62 = "pageX.t62.txt=";
	const char* pageX_t63 = "pageX.t63.txt=";
	const char* pageX_c2_0 = "pageX.c2.val=0";
	const char* pageX_c2_1 = "pageX.c2.val=1";
	const char* pageX_c3_0 = "pageX.c3.val=0";
	const char* pageX_c3_1 = "pageX.c3.val=1";

	const char* pageAlternateX_c2_0 = "pageAlternateX.c2.val=0";
	const char* pageAlternateX_c2_1 = "pageAlternateX.c2.val=1";
	const char* pageAlternateX_c3_0 = "pageAlternateX.c3.val=0";
	const char* pageAlternateX_c3_1 = "pageAlternateX.c3.val=1";

	const char* pageAlternateX_t60 = "pageAlternateX.t60.txt=";
	const char* pageAlternateX_t61 = "pageAlternateX.t61.txt=";
	const char* pageAlternateX_t62 = "pageAlternateX.t62.txt=";
	const char* pageAlternateX_t63 = "pageAlternateX.t63.txt=";



	// More
	const char* pageMore_c1_0 = "pageMore.c1.val=0";
	const char* pageMore_c1_1 = "pageMore.c1.val=1";

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

	// Page X
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

	// Page AlternateX
	SerialPrint(pageAlternateX_t60);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.microsteps_Axis_XAlt);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageAlternateX_t61);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.steps360_Axis_XAlt);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageAlternateX_t62);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.distancePerRev_AxisXAlt, 5);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageAlternateX_t63);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.polarity_Axis_XAlt ? lowChar : highChar);
	SerialPrint(nextionQuoteEnd);


	if (eePromPageSetup.xAltX == 1) // X axis
	{
		SerialPrint(pageX_c2_0);
		SerialPrint(nextionEnd);
		SerialPrint(pageX_c3_1);
		SerialPrint(nextionEnd);

		SerialPrint(pageAlternateX_c2_0);
		SerialPrint(nextionEnd);
		SerialPrint(pageAlternateX_c3_1);
		SerialPrint(nextionEnd);
	}
	else  // AltX axis
	{
		SerialPrint(pageX_c2_1);
		SerialPrint(nextionEnd);
		SerialPrint(pageX_c3_0);
		SerialPrint(nextionEnd);

		SerialPrint(pageAlternateX_c2_1);
		SerialPrint(nextionEnd);
		SerialPrint(pageAlternateX_c3_0);
		SerialPrint(nextionEnd);
	}


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

	// True=Linear, False = Radial
	if (eePromPageSetup.radialOrLinear_Axis_B) 
	{
		SerialPrint(pageB_c0_0);
		SerialPrint(nextionEnd);
		SerialPrint(pageB_c1_1);
		SerialPrint(nextionEnd);
	}
	else
	{
		SerialPrint(pageB_c0_1);
		SerialPrint(nextionEnd);
		SerialPrint(pageB_c1_0);
		SerialPrint(nextionEnd);
	}

	SerialPrint(pageB_t64);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.distancePerRev_AxisB, 4);
	SerialPrint(nextionQuoteEnd);

	// pageMore
	// 0 = Disabled, 1 = Enabled
	if (eePromPageSetup.keepSteppersEnabled == 0)
	{
		SerialPrint(pageMore_c1_0);
		SerialPrint(nextionEnd);
	}
	else
	{
		SerialPrint(pageMore_c1_1);
		SerialPrint(nextionEnd);

	}

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
	const char* pageLimits_t13 = "pageLimits.t13.txt=";
	const char* pageLimits_c1_0 = "pageLimits.c1.val=0";
	const char* pageLimits_c1_1 = "pageLimits.c1.val=1";

	const char* pageLimits_c2_0 = "pageLimits.c2.val=0";
	const char* pageLimits_c2_1 = "pageLimits.c2.val=1";
	const char* pageLimits_c3_0 = "pageLimits.c3.val=0";
	const char* pageLimits_c3_1 = "pageLimits.c3.val=1";

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

	SerialPrint(pageLimits_t13);
	SerialWrite(0x22);
	SerialPrint(configSetup.eStop);
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

	/////////////////////////
	Serial.print("limit_NCorNO: ");
	Serial.println(configSetup.limit_NCorNO);
	if(configSetup.limit_NCorNO == 0)
	{

			SerialPrint(pageLimits_c2_1);
			SerialPrint(nextionEnd);
			SerialPrint(pageLimits_c3_0);
			SerialPrint(nextionEnd);
		}
	else
		{
			SerialPrint(pageLimits_c2_0);
			SerialPrint(nextionEnd);
			SerialPrint(pageLimits_c3_1);
			SerialPrint(nextionEnd);
		}

	

	///////////////////////////

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

			SerialPrint("pageEEPROM.t35.txt=");
			SerialWrite(0x22);
			SerialPrint(configRec.style);
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
		case PAGE_PROGRAM:
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
		case PAGE_ROSE:
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

			SerialPrint("pageEEPROM.t29.txt=");
			SerialWrite(0x22);
			SerialPrint(configRose.spindleRevolutions);
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



					SerialPrint("pageEEPROM.t32.txt=");
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

					//SerialPrint("pageEEPROM.t29.txt=");
					//SerialWrite(0x22);
					//SerialPrint(configRose.spindleRevolutions);
					//SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t32.txt=");
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

					//SerialPrint("pageEEPROM.t29.txt=");
					//SerialWrite(0x22);
					//SerialPrint(configRose.spindleRevolutions);
					//SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t32.txt=");
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
#ifdef DEBUG
		Serial.println("No Card.................");
#endif // Debug
		// Update Nextion
		SerialPrint("bt0.bco=9563");
		//Serial.println("bt0.bco=9563");
		SerialPrint(nextionEnd);
		SerialPrint("bt0.val=0");
		//Serial.println("bt0.val=0");
		SerialPrint(nextionEnd);
		//Serial.println("vis t1,1");
		SerialPrint("vis t1,1");
		SerialPrint(nextionEnd);
		return;
	}
	else
	{
#ifdef DEBUG
		Serial.println("Ini file found......");
#endif // Debug
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
	LoadSettings_PageRose();
	LoadSettings_PageProgram();

	// Update Nextion
	// Splash Page
	//Serial.println("Show restart");
	SerialPrint("bt0.bco=9563");
	SerialPrint(nextionEnd);
	MilliDelay(15);
	SerialPrint("bt0.val=0");
	SerialPrint(nextionEnd);
	SerialPrint("vis g0,1");
	SerialPrint(nextionEnd);

	SerialPrint("vis t5, 0");
	SerialPrint(nextionEnd);
	SerialPrint("vis t6, 0");
	SerialPrint(nextionEnd);
	SerialPrint("vis t7, 0");
	SerialPrint(nextionEnd);
	SerialPrint("vis t8, 0");
	SerialPrint(nextionEnd);
	SerialPrint("vis t11, 0");
	SerialPrint(nextionEnd);
	SerialPrint("vis t12, 0");
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
	const char* iniKey = "Index";
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
	configIndex_1.sizeInUnits = returnVal;
	IndexSize(returnVal);

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
	configIndex_2.sizeInUnits = returnVal;
	IndexSize(returnVal);

	Serial.print("returnVal:  ");
	Serial.println(returnVal);
	Serial.print("configIndex_2.sizeInSteps: ");
	Serial.println(configIndex_2.sizeInSteps);
	
	Serial.print("configIndex_2.sizeInUnits: ");
	Serial.println(configIndex_2.sizeInUnits);

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
	configIndex_3.sizeInUnits = returnVal;
	IndexSize(returnVal);

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
	configIndex_4.sizeInUnits = returnVal;
	IndexSize(returnVal);

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
	configIndex_5.sizeInUnits = returnVal;
	IndexSize(returnVal);

	EEPROM.put(eePromAddress_Ind_Main, configIndex_Main);
	EEPROM.put(eePromAddress_Ind_1, configIndex_1);
	EEPROM.put(eePromAddress_Ind_2, configIndex_2);
	EEPROM.put(eePromAddress_Ind_3, configIndex_3);
	EEPROM.put(eePromAddress_Ind_4, configIndex_4);
	EEPROM.put(eePromAddress_Ind_5, configIndex_5);
}

/// <summary>
/// LoadSettings_PageRose
/// </summary>
/// <comment>
/// Load ini file settings for Geo page
/// </comment>
/// <returns></returns>
void LoadSettings_PageRose()
{

	const char* iniKey = "Rose";
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

	iniValue = "SpindleRevolutions";
	eePromAddress_Nextion = 696;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configRose.spindleRevolutions = returnVal;

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
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
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
	const char* iniKey = "Move";
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
	const char* iniKey = "GreekKey";

	const char* iniValue = "AxisId"; //(Shared by GreekKey and Program)
	int eePromAddress_Nextion = 752;
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.axisId = (int)returnVal;

	/*iniValue = "FileOrPattern";
	eePromAddress_Nextion = 200;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.fileOrPattern = (int)returnVal;*/


	// Greek Key Spindle (Shared by GreekKey and Program)
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

	// Z axis (Shared by GreekKey and Program)
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

	// X axis (Shared by GreekKey and Program)
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

	// B axis  (Shared by GreekKey and Program)
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

	iniValue = "RadialOrAxial";
	eePromAddress_Nextion = 432;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.radialOrAxial_Pattern = (int)returnVal;

	iniValue = "PatternType";
	eePromAddress_Nextion = 508;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.patternId = (int)returnVal;

	iniValue = "PatternsPer360";
	eePromAddress_Nextion = 372;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configGreekKey.countPatternPer360_Pattern = (int)returnVal;

	iniValue = "PatternCount";
	eePromAddress_Nextion = 496;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.patternCount_Pattern = (int)returnVal;

	iniValue = "SegmentLength";
	eePromAddress_Nextion = 620;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configGreekKey.segmentLength_Pattern = (int)returnVal;

	EEPROM.put(eePromAddress_Grk, configGreekKey);

}

void LoadSettings_PageProgram()
{
	// Greek Key Main
	const char* iniKey = "Program";

	const char* iniValue = "SegmentsOrActual";
	int eePromAddress_Nextion = 156;
	int returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.segmentOrActual = (int)returnVal;

	iniValue = "SegmentLength";
	eePromAddress_Nextion = 300;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configGreekKey.segmentLength_File = (int)returnVal;

    iniValue = "RadialOrAxial";
	eePromAddress_Nextion = 436;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.radialOrAxial_File = (int)returnVal;

	iniValue = "PatternsPer360";
	eePromAddress_Nextion = 636;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configGreekKey.countPatternPer360_File = (int)returnVal;

	iniValue = "PatternCount";
	eePromAddress_Nextion = 756;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.patternCount_File = (int)returnVal;



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
	const char* iniKey = "Recip";
	const char* iniValue = "AxisId";
	int eePromAddress_Nextion = 464;
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.axisId = (int)returnVal;

	iniValue = "Style";
	eePromAddress_Nextion = 564;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.style = (int)returnVal;

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

	iniValue = "Radial_Spindle_Degrees"; // Radial Degrees
	eePromAddress_Nextion = 308;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configRec.degrees_Radial_Spindle = returnVal;

	iniValue = "Radial_Axis_Amplitude";
	eePromAddress_Nextion = 232;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configRec.amplitude_Radial_Axis = returnVal;

	iniValue = "Axial_Waves";
	eePromAddress_Nextion = 740;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.waves_Axial = (int)returnVal;

	iniValue = "Axial_Spindle_Amplitude";
	eePromAddress_Nextion = 744;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configRec.amplitude_Axial_Spindle = returnVal;

	iniValue = "Axial_Axis_Distance";
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
	const char* nextionEnd = "\xFF\xFF\xFF";
	const char* iniKey = "Setup";
	// Show Vendor Splashscreen
	const char* iniValue = "BoardType";
	int eePromAddress_Nextion = 180;
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);

	// Keep steppers enabled
	iniValue = "KeepSteppersEnabled";
	eePromAddress_Nextion = 200;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);

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
	// Set default to X axis
	// pageX.vaXaltX
	eePromAddress_Nextion = 188;
	SerialPrint("wepo 0,188");
	SerialPrint(nextionEnd);
	configSetup.xAltX = 0;


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

	// Page Alternate X Axis
	iniValue = "StepsPer360_AltX";
	eePromAddress_Nextion = 204;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.steps360_Axis_XAlt = (int)returnVal;

	iniValue = "Microsteps_AltX";
	eePromAddress_Nextion = 196;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.microsteps_Axis_XAlt = (int)returnVal;

	iniValue = "DistancePer360_AltX";
	eePromAddress_Nextion = 216;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configSetup.distancePerRev_AxisXAlt = returnVal;

	iniValue = "Polarity_AltX";
	eePromAddress_Nextion = 916;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	returnVal >= 1 ? (configSetup.polarity_Axis_XAlt = true) : (configSetup.polarity_Axis_XAlt = false);

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

	iniValue = "EStop";
	eePromAddress_Nextion = 264;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.eStop = (int)returnVal;

	iniValue = "StopSpindle";
	eePromAddress_Nextion = 184;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.home_X = (int)returnVal;

	iniValue = "NC-NO";
	eePromAddress_Nextion = 348;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.limit_NCorNO = (int)returnVal;
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
	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
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
	if (!ini.validate(buffer, bufferLen)) 
	{
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

		SerialPrint("pageMore.t11.txt=");
		SerialWrite(0x22);
		SerialPrint(iniKey);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageMore.t12.txt=");
		SerialWrite(0x22);
		SerialPrint(iniValue);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageMore.t13.txt=");
		SerialWrite(0x22);
		SerialPrint(buffer);
		SerialPrint(nextionQuoteEnd);
	}
	else
	{
#ifdef DEBUG
		Serial.println("----------------");
		Serial.print("Bad:");
		Serial.print(iniKey);
		Serial.print(" - ");
		Serial.println(iniValue);
#endif // Debug
	}
	iniKey = "Main";
	MilliDelay(100);
	return atof(buffer);
}

void SetEStopColors(uint32_t page)
{
	String pageName = "";
	String printString = "";
	String bt0_bco = "bt0.bco=55715";
	String bt0_val = "bt0.val=0";
	String bt2_bco = "bt2.bco=55715";
	String bt2_val = "bt2.val=0";
	String bt3_bco = "bt3.bco=55715";
	String bt3_val = "bt3.val=0";
	String nextionEnd = "\xFF\xFF\xFF";
	eStopTriggered = true;
	if (page == PAGE_MAIN)
	{
			// Change Btn color on Nextion
			SerialPrint("pageMain.bt4.bco=55715");
			SerialPrint(nextionEnd);
			MilliDelay(10);
			SerialPrint("pageMain.bt4.val=0");
			SerialPrint(nextionEnd);

			SerialPrint("pageMain.bt5.bco=55715");
			SerialPrint(nextionEnd);
			MilliDelay(10);
			SerialPrint("pageMain.bt5.val=0");
			SerialPrint(nextionEnd);

			SerialPrint("pageMain.bt6.bco=55715");
			SerialPrint(nextionEnd);
			MilliDelay(10);
			SerialPrint("pageMain.bt6.val=0");
			SerialPrint(nextionEnd);

	}
	else
	{
		switch (page)
		{
			case PAGE_ONE:
			{
				pageName = "pageOne";
				break;
			}
			case PAGE_INDEX:
			{
				pageName = "pageIndex";
				break;
			}
			case PAGE_MOVE:
			{
				pageName = "pageMove";
				break;
			}
			case PAGE_SYNC:
			{
				pageName = "pageSync";
				break;
			}
			case PAGE_REC:
			{
				pageName = "pageRec";
				break;
			}
			case PAGE_GRK:
			{
				pageName = "pageGrk";
				break;
			}
			case PAGE_PROGRAM:
			{
				pageName = "pageProgram";
				break;
			}
			case PAGE_ROSE:
			{
				pageName = "pageRose";
				break;
			}
		}

		//printString = pageName + bt0_bco;
		printString = bt0_bco;
		Serial.println(printString);
		//printString = bt0_bco;
		SerialPrint(printString);
		SerialPrint(nextionEnd);
		MilliDelay(15);
		printString = pageName + bt0_val;

		Serial.println(bt0_val);
		SerialPrint(bt0_val);
		SerialPrint(nextionEnd);
		MilliDelay(15);

		printString = bt2_bco;
		Serial.println(bt2_bco);
		SerialPrint(bt2_bco);
		SerialPrint(nextionEnd);
		MilliDelay(10);
		printString = bt2_val;
		Serial.println(bt2_val);
		SerialPrint(bt2_val);
		SerialPrint(nextionEnd);
		MilliDelay(15);
		printString = bt3_bco;
		Serial.println(printString);
		SerialPrint(bt3_bco);
		SerialPrint(nextionEnd);
		MilliDelay(15);
		printString = bt3_val;
		Serial.println(printString);
		SerialPrint(bt3_val);
		SerialPrint(nextionEnd);
	}
}

void SetLimitColors(uint32_t page, uint32_t direction)
{
	String pageName = "";
	String printString = "";

	String bt2_bco = "bt2.bco=55715";
	String bt2_val = "bt2.val=0";
	String bt3_bco = "bt3.bco=55715";
	String bt3_val = "bt3.val=0";
	String nextionEnd = "\xFF\xFF\xFF";
	limitTriggered = true;
	if (page == PAGE_MAIN)
	{
		// Change Btn color on Nextion
		switch (direction)
		{
			case DIR_CCW:
			{
				SerialPrint("pageMain.bt5.bco=55715");
				SerialPrint(nextionEnd);
				MilliDelay(10);
				SerialPrint("pageMain.bt5.val=0");
				SerialPrint(nextionEnd);
				break;
			}
			case DIR_CW:
			{
				SerialPrint("pageMain.bt6.bco=55715");
				SerialPrint(nextionEnd);
				MilliDelay(10);
				SerialPrint("pageMain.bt6.val=0");
				SerialPrint(nextionEnd);
				break;
			}
		}

	}
	else
	{
		//switch (page)
		//{
		//	case PAGE_ONE:
		//	{
		//		pageName = "pageOne";
		//		break;
		//	}
		//	case PAGE_MOVE:
		//	{
		//		pageName = "pageMove";
		//		break;
		//	}
		//	case PAGE_SYNC:
		//	{
		//		pageName = "pageSync";
		//		break;
		//	}
		//	case PAGE_GRK:
		//	{
		//		pageName = "pageGrk";
		//		break;
		//	}
		//	case PAGE_PROGRAM:
		//	{
		//		pageName = "pageProgram";
		//		break;
		//	}
		//	case PAGE_ROSE:
		//	{
		//		pageName = "pageRose";
		//		break;
		//	}
		//}

		switch (direction)
		{ 
			case DIR_CCW:
			{
				printString = bt2_bco;
				Serial.println(printString);
				SerialPrint(printString);
				SerialPrint(nextionEnd);
				MilliDelay(10);
				printString = bt2_val;
				SerialPrint(printString);
				SerialPrint(nextionEnd);
				break;
			}
			case DIR_CW:
			{
				printString = bt3_bco;
				SerialPrint(printString);
				SerialPrint(nextionEnd);
				MilliDelay(10);
				printString = bt3_val;
				SerialPrint(printString);
				SerialPrint(nextionEnd);
				break;
			}

		}

	}
}
