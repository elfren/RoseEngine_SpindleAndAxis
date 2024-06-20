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
	const char* configIndex_Char = "configIndex:";
	const char* configPageMov_Char = "configPageMov:";
	const char* configPageSync_Char = "configPageSync:";
	const char* configPageSetup_Char = "configPageSetup:";
	//	const char * configSteppers_Char = "configSteppers:";
	const char* configPageReci_Char = "configPageReci:";
	const char* configPageRose_Char = "configPageRose:";
	const char* configPageMainMulti_Char = "configPageMainMulti:";
	const char* configPageGreekKey_Char = "configPageGreekKey:";
	const char* configPageIndex_Char = "configPageIndex:";
	const char* configPageOne_Char = "configPageOne: ";

#ifdef DEBUG
	Serial.print(configIndex_Char);
	int sizeS = sizeof(configIndex);
	Serial.println(sizeS);
	Serial.print(configPageIndex_Char);
	sizeS = sizeof(configPageIndex);
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

	Serial.print(configPageReci_Char);
	sizeS = sizeof(configPageRec);
	Serial.println(sizeS);
	Serial.print(configPageRose_Char);
	sizeS = sizeof(configPageRose);
	Serial.println(sizeS);
	Serial.print(configPageMainMulti_Char);
	sizeS = sizeof(configPageMainMulti);
	Serial.println(sizeS);
	Serial.print(configPageGreekKey_Char);
	sizeS = sizeof(configPageGreekKey);
	Serial.println(sizeS);
	Serial.print(configPageOne_Char);
	sizeS = sizeof(configPageOne);
	Serial.println(sizeS);

#endif // DEBUG

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

int32_t ProgressReport(int32_t target, int32_t position)
{
	float progress;

	progress = floor(((float(position)) / (float(target))) * 100);

	if (progress < 1)
	{
		progress *= (-1);
	}

	switch (pageCallerId)
	{
		case PAGE_MULTI:
		{
			SerialPrint("pageMulti.jProgress.val=");
			break;
		}
		case PAGE_INDEX:
		{
			SerialPrint("pageIndex.jProgress.val=");
			break;
		}
		case PAGE_MOVE:
		{
			SerialPrint("pageMove.jProgress.val=");
			break;
		}
		case PAGE_SYNC:
		{
			SerialPrint("pageSync.jProgress.val=");
			break;
		}
		case PAGE_REC:
		{
			SerialPrint("pageRec.jProgress.val=");
			break;
		}
		case PAGE_GRK:
		{
			SerialPrint("pageGrk.jProgress.val=");
			break;
		}
		case PAGE_PROGRAM:
		{

			break;
		}
		case PAGE_ROSE:
		{
			SerialPrint("pageRose.jProgress.val=");
			break;
		}
	}

	Serial.print("target: ");
	Serial.print(target);
	Serial.print("  position: ");
	Serial.print(position);
	Serial.print("  Progress: ");
	Serial.println(progress);
	SerialPrint((float)progress);
	SerialPrint(nextionEnd);
	return progress;
}

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
//#ifdef DEBUG
	Serial.println("SD..begin");
	//#endif // DEBUG
	if (SD.begin(254)) // Builtin microSD card: Use 254 instead of BUILTIN_SDCARD for compatibility with Teensy 3.2
	{
		microSD_Id = 1;
		//#ifdef VERBOSE
		Serial.println("microSD-1");
		//#endif // VERBOSE
	}
	else if (SD.begin(PIN_SPI_CS_10)) // SPI microSD CS on pin 10
	{
		microSD_Id = 10;

#ifdef VERBOSE
		Serial.println("microSD-10");
#endif // VERBOSE
	}

	else if (SD.begin(PIN_SPI_CS_9)) // SPI microSD CS on pin 9
	{
		microSD_Id = 9;
#ifdef VERBOSE
		Serial.println("microSD-9");
#endif // VERBOSE

	}
	else // No card
	{
		microSD_Id = 0;
	}
#ifdef VERBOSE
	Serial.print("microSD_Id: ");
	Serial.println(microSD_Id);
#endif // VERBOSE
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
	bool ignoreDisable = false;
	int currentPinState = 0;
	int currentPin = 0;
	if (checkKeepEnabled)
	{
		if (configSetup.keepSteppersEnabled == 1)
		{
			ignoreDisable = true;
			Serial.print("ignoreDisable:  ");
			Serial.println(ignoreDisable);
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
			SerialPrint("pageSplash.vaSpEnabled.val=1"); // 1 = enabled
			SerialPrint("\xFF\xFF\xFF");
		}
		else
		{
			if (!ignoreDisable)
			{
				digitalWrite(PIN_SPINDLE_ENABLE, configSetup.polarity_Spindle ? (HIGH) : (LOW)); // Disable 
				SerialPrint("pageSplash.vaSpEnabled.val=0"); // 1 = enabled
				SerialPrint("\xFF\xFF\xFF");
			}
		}

		currentPinState = digitalReadFast(PIN_SPINDLE_ENABLE);
		currentPin = PIN_SPINDLE_ENABLE;
#ifdef DEBUG
		Serial.print("currentPinState: ");
		Serial.println(currentPinState);
		Serial.print("Spindle enable pin #: ");
		Serial.println(currentPin);
#endif // DEBUG
		break;
	}
	case ID_AXIS_Z:
	{
		if (enabled)
		{
			digitalWrite(PIN_AXIS_Z_ENABLE, configSetup.polarity_Axis_Z ? (LOW) : (HIGH)); // Enable 
			SerialPrint("pageSplash.vaAxisEnabledZ.val=1"); // 1 = enabled
			SerialPrint("\xFF\xFF\xFF");
		}
		else
		{
			if (!ignoreDisable)
			{
				digitalWrite(PIN_AXIS_Z_ENABLE, configSetup.polarity_Axis_Z ? (HIGH) : (LOW)); // Disable 
				SerialPrint("pageSplash.vaAxisEnabledZ.val=0"); // 1 = enabled
				SerialPrint("\xFF\xFF\xFF");
			}
		}

		currentPinState = digitalReadFast(PIN_AXIS_Z_ENABLE);
		currentPin = PIN_AXIS_Z_ENABLE;
#ifdef DEBUG
		Serial.print("Z enable pin #: ");
		Serial.println(PIN_AXIS_Z_ENABLE);
#endif // DEBUG

		break;
	}
	case ID_AXIS_X:
	{
		if (enabled)
		{
			digitalWrite(PIN_AXIS_X_ENABLE, configSetup.polarity_Axis_X ? (LOW) : (HIGH)); // Enable 
			SerialPrint("pageSplash.vaAxisEnabledX.val=1"); // 1 = enabled
			SerialPrint("\xFF\xFF\xFF");
		}
		else
		{
			if (!ignoreDisable)
			{
				digitalWrite(PIN_AXIS_X_ENABLE, configSetup.polarity_Axis_X ? (HIGH) : (LOW)); // Disable 
				SerialPrint("pageSplash.vaAxisEnabledX.val=0"); // 1 = enabled
				SerialPrint("\xFF\xFF\xFF");
			}
		}

		currentPinState = digitalReadFast(PIN_AXIS_X_ENABLE);
		currentPin = PIN_AXIS_X_ENABLE;
#ifdef DEBUG
		Serial.print("X enable pin #: ");
		Serial.println(PIN_AXIS_X_ENABLE);
#endif // DEBUG

		break;
	}
	case ID_AXIS_3:
	{
		Serial.print("M3 enable state: ");
		Serial.println(configSetup.polarity_Axis_M3 ? (LOW) : (HIGH));
		if (enabled)
		{
			Serial.print(">>M3 enabled: ");
			digitalWrite(PIN_AXIS_3_ENABLE, configSetup.polarity_Axis_M3 ? (LOW) : (HIGH)); // Enable 
			SerialPrint("pageSplash.vAxisEnabledM3.val=1"); // 1 = enabled
			SerialPrint("\xFF\xFF\xFF");
		}
		else
		{
			if (!ignoreDisable)
			{
				digitalWrite(PIN_AXIS_3_ENABLE, configSetup.polarity_Axis_M3 ? (HIGH) : (LOW)); // Disable 
				SerialPrint("pageSplash.vAxisEnabledM3.val=0"); // 1 = enabled
				SerialPrint("\xFF\xFF\xFF");
			}
		}

		currentPinState = digitalReadFast(PIN_AXIS_3_ENABLE);
		currentPin = PIN_AXIS_3_ENABLE;
#ifdef DEBUG
		Serial.print("M3 enable pin #: ");
		Serial.println(PIN_AXIS_3_ENABLE);
#endif // DEBUG
		Serial.print("2.......M3 enable state: ");
		Serial.println(configSetup.polarity_Axis_M3 ? (LOW) : (HIGH));

		break;
	}
	case ID_AXIS_4:
	{
		if (enabled)
		{
			Serial.println("PIN_AXIS_4_ENABLE:  ");
			digitalWrite(PIN_AXIS_4_ENABLE, configSetup.polarity_Axis_M4 ? (LOW) : (HIGH)); // Enable 
			SerialPrint("pageSplash.vAxisEnabledM4.val=1"); // 1 = enabled
			SerialPrint("\xFF\xFF\xFF");
			Serial.println("pageSplash.vAxisEnabledM4.val=1  ");
	
		}
		else
		{
			if (!ignoreDisable)
			{
				Serial.println("PIN_AXIS_4_ENABLE:  ");
				digitalWrite(PIN_AXIS_4_ENABLE, configSetup.polarity_Axis_M4 ? (HIGH) : (LOW)); // Disable 
				SerialPrint("pageSplash.vAxisEnabledM4.val=0");
				SerialPrint("\xFF\xFF\xFF");
				Serial.println("pageSplash.vAxisEnabledM4.val=0  ");
			}
		}

		currentPinState = digitalReadFast(PIN_AXIS_4_ENABLE);
		currentPin = PIN_AXIS_4_ENABLE;
#ifdef DEBUG
		Serial.print("M4 enable pin #: ");
		Serial.println(PIN_AXIS_4_ENABLE);
#endif // DEBUG

		break;
	}
	}

#ifdef DEBUG
	Serial.print("SetEnable-currentPinState: ");
	Serial.println(currentPinState);
#endif // DEBUG

	if (enabled)// Workaround for DM542T external driver
	{
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
	double retVal = -1;

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
	//case 2:
	//{
	//	retVal = GetSerialFloatA(Serial2);
	//	break;
	//}
	case 3:
	{
		retVal = GetSerialFloatA(Serial3);
		break;
	}

	case 4:
	{
		retVal = GetSerialFloatA(Serial4);
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
	char discardChar;
	double retVal = 0;
	int i;
	int j;

	for (j = 0; j <= bufferLength; j++)
	{
		in[j] = char(0);
	}
	for (i = 0; i <= bufferLength; i++)
	{
#ifdef VERBOSE
		Serial.println(in);
#endif //VERBOSE
		inputChar = port.read();
#ifdef VERBOSE
		Serial.println("");
		Serial.print("InputChar:----------abc------------ ");
		Serial.print(i);
		Serial.print(" :  ");
		Serial.print(inputChar);
		Serial.print("  ");
		Serial.println(int(inputChar));
#endif //VERBOSE
		switch (inputChar)
		{
		case 255:
		{
			Serial.print(">>>>>>>>>>>>>>>255: ");
			Serial.println(in);
			// Read and discard anything more on serial.
			while (port.available() > 0)
			{
				discardChar = port.read();
				Serial.print(">>>>>>>>>>>>>>>discardChar: ");
				Serial.println(discardChar);
			}

			goto endLoop;
			break;
		}
		case 4:
		{
#ifdef VERBOSE
			Serial.print("Discard bogus 4----------");
			Serial.println(i);
			i = -1;
#endif //VERBOSE
			break;
		}
		case 127:
		{
#ifdef VERBOSE

			Serial.print("Discard bogus 127----------");
			Serial.println(i);
			i = -1;
#endif //VERBOSE
			break;
		}
		default:
		{
			in[i] = inputChar;
#ifdef VERBOSE
			Serial.println(in);
#endif //VERBOSE
		}
		}
	}
endLoop:

	retVal = (float)atof(in);

#ifdef DEBUG
	Serial.print("In: ");
	Serial.println(in);
	Serial.print("retVal: ");
	Serial.println(retVal, 4);
#endif //DEBUG
	return retVal;
}


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
#ifdef VERBOSE
	Serial.print("inputHex:   ");
	Serial.println(inputHex);
#endif // VERBOSE
	int newInteger = (int)inputHex;
	while (SerialAvailable() > 0)
	{
		// Discard remaining serial chars
		SerialRead(serialId);
	}

#ifdef DEBUG
	const char* int_Char = "Int:";
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
	Serial.println("Exit GetSerialIntergerOnly..........");
	return newInteger;
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
/// StepsToDegrees
/// </summary>
/// <comment>
/// Convert steps to degrees
/// </comment>
/// <param name="steps">Steps</param>
/// <param name="axisId">axisId</param>
/// <returns></returns>
float StepsToDegrees(float steps, int axisId)
{
	float retVal = (steps / (configSetup.steps360_Spindle * configSetup.microsteps_Spindle * configSetup.gearRatio_Spindle)) * 360;
	if (axisId == ID_AXIS_3)
	{
		retVal = (steps / (configSetup.steps360_Axis_M3 * configSetup.microsteps_Axis_M3 * configSetup.gearRatio_AxisM3)) * 360;
	}
	else if (axisId == ID_AXIS_4)
	{
		retVal = (steps / (configSetup.steps360_Axis_M3 * configSetup.microsteps_Axis_M3 * configSetup.gearRatio_AxisM3)) * 360;
	}
	return retVal;
}

/// <summary>
/// RevolutionsToSteps
/// </summary>
/// <comment>
/// Convert spindle revolutions to steps
/// </comment>
/// <param name="revolutions">revolutions</param>
/// <param name="direction">Direction of revolutions</param>
/// <param name="axisId">axisId</param>
/// <returns>float</returns>
float RevolutionsToSteps(float revolutions, int direction, int axisId)
{
	// Spindle
	float retVal = revolutions * configSetup.gearRatio_Spindle * configSetup.microsteps_Spindle * configSetup.steps360_Spindle * direction;
	if (axisId == ID_AXIS_3)
	{
		retVal = revolutions * configSetup.gearRatio_AxisM3 * configSetup.microsteps_Axis_M3 * configSetup.steps360_Axis_M3 * direction;
	}
	else if (axisId == ID_AXIS_4)
	{
		retVal = revolutions * configSetup.gearRatio_AxisM4 * configSetup.microsteps_Axis_M4 * configSetup.steps360_Axis_M4 * direction;
	}

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
		if (configSetup.xAltX == 0)
		{
			retVal = (steps * configSetup.distancePerRev_AxisX) / (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X);
		}
		else
		{
			retVal = (steps * configSetup.distancePerRev_AxisXAlt) / (configSetup.steps360_Axis_XAlt * configSetup.microsteps_Axis_XAlt);
		}
		break;
	}
	case ID_AXIS_3: // Rotary axis
	{
		switch (configSetup.radialOrLinear_Axis_M3)
		{
		case RADIAL_M3:
		{
			retVal = (steps / (configSetup.steps360_Axis_M3 * configSetup.microsteps_Axis_M3 * configSetup.gearRatio_AxisM3)) * 360;
		}
		case LINEAR_M3:
		{
			retVal = (steps * configSetup.distancePerRev_AxisM3) / (configSetup.steps360_Axis_M3 * configSetup.microsteps_Axis_M3);
		}
		}
		break;
	}
	case ID_AXIS_4: // Rotary axis
	{
		switch (configSetup.radialOrLinear_Axis_M4)
		{
		case RADIAL_M4:
		{
			retVal = (steps / (configSetup.steps360_Axis_M3 * configSetup.microsteps_Axis_M3 * configSetup.gearRatio_AxisM3)) * 360;
		}
		case LINEAR_M4:
		{
			retVal = (steps * configSetup.distancePerRev_AxisM4) / (configSetup.steps360_Axis_M4 * configSetup.microsteps_Axis_M4);
		}
		}
		break;
	}
	}
	return retVal;
}

/// <summary>
/// DistanceToSteps_M3Radial
/// </summary>
/// <comment>
/// Convert distance to steps for axis B in radial mode.
/// </comment>
/// <param name="linearDistance">Distance</param>
/// <returns>float</returns>
float DistanceToSteps_M3Radial(float linearDistance)
{
	float retVal = 0;

	float stepsPer360 = configSetup.steps360_Axis_M3 * configSetup.microsteps_Axis_M3 * configSetup.gearRatio_AxisM3;
	float circumference = 2 * M_PI * configSetup.radiusM3;
	float angle = linearDistance / circumference;
	retVal = stepsPer360 * angle;
#ifdef VERBOSE
	Serial.print("stepsPer360: ");
	Serial.println(stepsPer360);
	Serial.print("gearRatio_AxisB: ");
	Serial.println(configSetup.gearRatio_AxisM3);
	Serial.print("circumference: ");
	Serial.println(circumference);
	Serial.print("linearDistance: ");
	Serial.println(linearDistance);
	Serial.print("angle: ");
	Serial.println(angle);
	Serial.print("retVal: ");
	Serial.println(retVal);
#endif // VERBOSE
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
float DistanceToSteps_M4Radial(float linearDistance)
{
	float retVal = 0;

	float stepsPer360 = configSetup.steps360_Axis_M4 * configSetup.microsteps_Axis_M4 * configSetup.gearRatio_AxisM4;
	float circumference = 2 * M_PI * configSetup.radiusM4;
	float angle = linearDistance / circumference;
	retVal = stepsPer360 * angle;
#ifdef VERBOSE
	Serial.print("stepsPer360: ");
	Serial.println(stepsPer360);
	Serial.print("gearRatio_Axis M4: ");
	Serial.println(configSetup.gearRatio_AxisM4);
	Serial.print("circumference: ");
	Serial.println(circumference);
	Serial.print("linearDistance: ");
	Serial.println(linearDistance);
	Serial.print("angle: ");
	Serial.println(angle);
	Serial.print("retVal: ");
	Serial.println(retVal);
#endif // VERBOSE
	return retVal;
}

/// <summary>
/// DistanceToSteps_M3Linear
/// </summary>
/// <comment>
/// Convert distance to steps for axis M3 in linear mode.
/// </comment>
/// <param name="distance">Distance</param>
/// <returns>float</returns>
float DistanceToSteps_M3Linear(float distance)
{
	float retVal = (distance / (configSetup.distancePerRev_AxisM3) * (configSetup.steps360_Axis_M3 * configSetup.microsteps_Axis_M3));
#ifdef VERBOSE
	Serial.print("distance: ");
	Serial.println(distance);
	Serial.print("DistanceToSteps_M3Linear: ");
	Serial.println(retVal);
#endif // VERBOSE	
	return retVal;
}

/// <summary>
/// DistanceToSteps_M4Linear
/// </summary>
/// <comment>
/// Convert distance to steps for axis M4 in linear mode.
/// </comment>
/// <param name="distance">Distance</param>
/// <returns>float</returns>
float DistanceToSteps_M4Linear(float distance)
{
	float retVal = (distance / (configSetup.distancePerRev_AxisM4) * (configSetup.steps360_Axis_M4 * configSetup.microsteps_Axis_M4));
#ifdef VERBOSE
	Serial.print("distance: ");
	Serial.println(distance);
	Serial.print("DistanceToSteps_M4Linear: ");
	Serial.println(retVal);
#endif // VERBOSE	
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
		retVal = (distance / configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z);
		//Serial.print("8distance-----------------------:");
		//Serial.println(distance);
		//Serial.print("distancePerRev_AxisZ------------:");
		//Serial.println(configSetup.distancePerRev_AxisZ);
		//Serial.print("steps360_Axis_Z-----------------:");
		//Serial.println(configSetup.steps360_Axis_Z);
		//Serial.print("microsteps_Axis_Z---------------:");
		//Serial.println(configSetup.microsteps_Axis_Z);
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
	case ID_AXIS_3:
	{
		switch (configSetup.radialOrLinear_Axis_M3)
		{
		case RADIAL_M3: // Radial
		{
			retVal = DistanceToSteps_M3Radial(distance);

			break;
		}
		case LINEAR_M3: // Linear
		{
			retVal = DistanceToSteps_M3Linear(distance);
			break;
		}
		}
		break;
	}
	case ID_AXIS_4:
	{
		switch (configSetup.radialOrLinear_Axis_M4)
		{
		case RADIAL_M4: // Radial
		{
			retVal = DistanceToSteps_M4Radial(distance);

			break;
		}
		case LINEAR_M3: // Linear
		{
			retVal = DistanceToSteps_M4Linear(distance);
			break;
		}
		}
		break;
	}
	}
	Serial.print("retVal---------------:");
	Serial.println(retVal);
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

void GetSpindleSpeed()
{
	const char* spindle_SpeedPercent_Char = "Spindle Speed Percent:";

	switch (pageCallerId)
	{
		case PAGE_MAIN:
		{
			Serial.print("Current Spindle Speed percentage: ");
			Serial.println(configMain.speedPercent_Spindle);
			configMain.speedPercent_Spindle = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Main, configMain);
			Serial.print("New Spindle Speed percentage: ");
			Serial.println(configMain.speedPercent_Spindle);
			break;
		}

		case PAGE_MULTI:
		{
			configMulti.speedPercent_Spindle = (int)GetSerialFloat(serialId);
#ifdef DEBUG
			Serial.println(configMulti.speedPercent_Spindle);
#endif // DEBUG
			EEPROM.put(eePromAddress_Multi, configMulti);
			break;
		}
		case PAGE_ONE:
		{
			configOne.speedPercent_Spindle = (int)GetSerialFloat(serialId);
	#ifdef DEBUG
			Serial.println(configOne.speedPercent_Spindle);
	#endif // DEBUG
			EEPROM.put(eePromAddress_One, configOne);
			break;
		}
		case PAGE_INDEX:
		{
			configIndex_Prime.speedPercent_Spindle = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Ind_Prime, configIndex_Prime);
	#ifdef DEBUG
			Serial.print(spindle_SpeedPercent_Char);
			Serial.println(configIndex_Prime.speedPercent_Spindle);
	#endif // DEBUG
			break;
		}
		case PAGE_BE:
		{
			break;
		}
		case PAGE_REC:
		{
			configRec.speedPercent_Spindle = configSync.speedPercent_Spindle;
			EEPROM.put(eePromAddress_Rec, configRec);
	#ifdef VERBOSE
			Serial.print(spindle_SpeedPercent_Char);
			Serial.println(configSync.speedPercent_Spindle);
	#endif // VERBOSE
			break;
		}
		case PAGE_SYNC:
		{
			configSync.speedPercent_Spindle = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Sync, configSync);
	#ifdef VERBOSE
			Serial.print(spindle_SpeedPercent_Char);
			Serial.println(configSync.speedPercent_Spindle);
	#endif // VERBOSE
			break;
		}
		case PAGE_GRK:
		case PAGE_PROGRAM:
		{
			configGreekKey.speedPercent_Spindle = configSync.speedPercent_Spindle;
			EEPROM.put(eePromAddress_Grk, configGreekKey);
	#ifdef VERBOSE
			Serial.print(spindle_SpeedPercent_Char);
			Serial.println(configSync.speedPercent_Spindle);
	#endif // VERBOSE
			break;
		}
		case PAGE_ROSE:
		{
			configRose.speedPercent_Spindle = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rose, configRose);
	#ifdef VERBOSE
			Serial.print(spindle_SpeedPercent_Char);
			Serial.println(configRose.speedPercent_Spindle);
	#endif // VERBOSE
			break;
		}
	}
}

void GetAxisSpeed()
{
#ifdef DEBUG

	Serial.println("Enter GetAxisSpeed - ");
	Serial.print("pageCallerId: ");
	Serial.println(pageCallerId);
#endif // DEBUG
	//const char* x_SpeedPercent_Char = "X axis Speed Percent:";
	//const char* m3_SpeedPercent_Char = "M3 axis Speed Percent:";
	//const char* m4_SpeedPercent_Char = "M4 axis Speed Percent:";
	//const char* z_SpeedPercent_Char = "Z axis Speed Percent:";
	//const char* axisId_Char = "Axis ID:";
	switch (pageCallerId)
	{
		case PAGE_MAIN:
		{
			int newSpeedPercentage = (int)GetSerialFloat(serialId);
			switch (configMain.axisId)
			{
			case ID_AXIS_Z:
			{
				configMain.speedPercent_Z = newSpeedPercentage;
				EEPROM.put(eePromAddress_Main, configMain);
				break;
			}
			case ID_AXIS_X:
			{
				configMain.speedPercent_X = newSpeedPercentage;
				EEPROM.put(eePromAddress_Main, configMain);
				break;
			}
			case ID_AXIS_3:
			{
				configMain.speedPercent_M3 = newSpeedPercentage;
				EEPROM.put(eePromAddress_Main, configMain);
				break;
			}
			case ID_AXIS_4:
			{
				configMain.speedPercent_M4 = newSpeedPercentage;
				EEPROM.put(eePromAddress_Main, configMain);
				break;
			}
			}
			break;
		}
		case PAGE_MULTI:
		{
			int newSpeedPercentage = (int)GetSerialFloat(serialId);
			switch (configMulti.axisId)
			{
			case ID_AXIS_Z:
			{
				configMulti.speedPercent_Z = newSpeedPercentage;
				break;
			}
			case ID_AXIS_X:
			{
				configMulti.speedPercent_X = newSpeedPercentage;
				break;
			}
			case ID_AXIS_3:
			{
				configMulti.speedPercent_M3 = newSpeedPercentage;
				break;
			}
			case ID_AXIS_4:
			{
				configMulti.speedPercent_M4 = newSpeedPercentage;
				break;
			}
			case ID_SPINDLE:
			{
				configMulti.speedPercent_Spindle = newSpeedPercentage;
				break;
			}
			}

			EEPROM.put(eePromAddress_Multi, configMulti);
			break;
		}
		case PAGE_ONE:
		{
			int newSpeedPercentage = (int)GetSerialFloat(serialId);
			switch (configOne.axisId)
			{
			case ID_AXIS_Z:
			{
				configOne.speedPercent_Z = newSpeedPercentage;
				break;
			}
			case ID_AXIS_X:
			{
				configOne.speedPercent_X = newSpeedPercentage;
				break;
			}
			case ID_AXIS_3:
			{
				configOne.speedPercent_M3 = newSpeedPercentage;
				break;
			}
			case ID_AXIS_4:
			{
				configOne.speedPercent_M4 = newSpeedPercentage;
				break;
			}
			case ID_SPINDLE:
			{
				configOne.speedPercent_Spindle = newSpeedPercentage;
				break;
			}
			}

			EEPROM.put(eePromAddress_One, configOne);
			break;
		}
		case PAGE_INDEX:
		{
			switch (configIndex_Prime.axisId)
			{
			case ID_AXIS_3:
			{
				configIndex_Prime.speedPercent_M3 = (int)GetSerialFloat(serialId);
				break;
			}
			case ID_AXIS_4:
			{
				configIndex_Prime.speedPercent_M4 = (int)GetSerialFloat(serialId);
				break;
			}
			}

			EEPROM.put(eePromAddress_Ind_Prime, configIndex_Prime);
			break;
		}
		case PAGE_MOVE:
		{
			switch (configMove.axisId)
			{
			case ID_MOVE_AXIS_Z1:
			case ID_MOVE_AXIS_Z2:
			{
				configMove.speedPercent_Z = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Mov, configMove);
				break;
			}
			case ID_MOVE_AXIS_X1:
			case ID_MOVE_AXIS_X2:
			{
				configMove.speedPercent_X = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Mov, configMove);
				break;
			}
			case ID_MOVE_AXIS_M3_1:
			case ID_MOVE_AXIS_M3_2:
			{
				configMove.speedPercent_M3 = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Mov, configMove);
				break;
			}
			case ID_MOVE_AXIS_M4_1:
			case ID_MOVE_AXIS_M4_2:
			{
				configMove.speedPercent_M4 = (int)GetSerialFloat(serialId);
				EEPROM.put(eePromAddress_Mov, configMove);
				break;
			}
			}
			break;
		}
		case PAGE_BE:
		{
			break;
		}
		case PAGE_REC:
		{
			switch (configSync.axisId)
			{
				case ID_AXIS_Z:
				{
					configRec.speedPercent_Z = configSync.speedPercent_Z;
					break;
				}
				case ID_AXIS_X:
				{
					configRec.speedPercent_X = configSync.speedPercent_X;
					break;
				}
				case ID_AXIS_3:
				{
					configRec.speedPercent_M3 = configSync.speedPercent_M3;
					break;
				}
				case ID_AXIS_4:
				{
					configRec.speedPercent_M4 = configSync.speedPercent_M4;
					break;
				}
			}

			EEPROM.put(eePromAddress_Rec, configRec);
			break;
		}
		case PAGE_SYNC:
		{
			switch (configSync.axisId)
			{
				case ID_AXIS_Z:
				{
					configSync.speedPercent_Z = (int)GetSerialFloat(serialId);
					break;
				}
				case ID_AXIS_X:
				{
					configSync.speedPercent_X = (int)GetSerialFloat(serialId);
					break;
				}
				case ID_AXIS_3:
				{
					configSync.speedPercent_M3 = (int)GetSerialFloat(serialId);
					break;
				}
				case ID_AXIS_4:
				{
					configSync.speedPercent_M4 = (int)GetSerialFloat(serialId);
					break;
				}
			}

			EEPROM.put(eePromAddress_Grk, configGreekKey);
			break;
		}
		case PAGE_GRK:
		case PAGE_PROGRAM:
		{
			switch (configSync.axisId)
			{
			case ID_AXIS_Z:
			{
				configSync.speedPercent_Z = (int)GetSerialFloat(serialId);
				configGreekKey.speedPercent_Z = configSync.speedPercent_Z;
				break;
			}
			case ID_AXIS_X:
			{
				configSync.speedPercent_X = (int)GetSerialFloat(serialId);
				configGreekKey.speedPercent_X = configSync.speedPercent_X;
				break;
			}
			case ID_AXIS_3:
			{
				configSync.speedPercent_M3 = (int)GetSerialFloat(serialId);
				configGreekKey.speedPercent_M3 = configSync.speedPercent_M3;
				break;
			}
			case ID_AXIS_4:
			{
				configSync.speedPercent_M4 = (int)GetSerialFloat(serialId);
				configGreekKey.speedPercent_M4 = configSync.speedPercent_M4;
				break;
			}
			}

			EEPROM.put(eePromAddress_Sync, configSync);
			EEPROM.put(eePromAddress_Grk, configGreekKey);
			break;
		}
		case PAGE_ROSE:
		{
			if (configRose.axisId == ID_AXIS_Z)
			{
				configRose.speedPercent_Z = (int)GetSerialFloat(serialId);
			}
			else if (configRose.axisId == ID_AXIS_X)
			{
				configRose.speedPercent_X = (int)GetSerialFloat(serialId);
			}
			else if (configRose.axisId == ID_AXIS_3)
			{
				configRose.speedPercent_M3 = (int)GetSerialFloat(serialId);
			}
			else if (configRose.axisId == ID_AXIS_4)
			{
				configRose.speedPercent_M4 = (int)GetSerialFloat(serialId);
			}

			EEPROM.put(eePromAddress_Rose, configRose);
			break;
		}
	}
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
	int stepPin = 0;
	int dirPin = 0;
	//	int enablePin = 0;
	int limitPin_Max = configSetup.limit_Max_Z;
	int limitPin_Min = configSetup.limit_Min_Z;
	int32_t currentPosition = 0;
	double stepsToMove = 0;
	float maxSpeed = 0;
	float acceleration = 0;
	//const char* distancePer360_Char = "Distance/360:";
	//const char* stepsPer360_Char = "Steps/360:";
	//const char* microsteps_Char = "Microsteps:";
	//const char* distance_Char = "Distance:";
	//const char* steps_Char = "Steps:";
	//const char* maxSpd_Char = "Max Speed:";
	//const char* speedPercent_Char = "Speed Percent:";
	//const char* nextSpeed_Char = "Next Speed:";
	//const char* accel_Char = "Accel:";
	const char* position_Char = "Position:";
	const char* pageMove_bt2_val_Char = "pageMove.bt2.val=0";
	const char* pageMove_va0_Char = "pageMove.va0.val=0";
	const char* pageMove_bt1_Char = "pageMove.bt1.val=0";
	const char* pageMove_bt3_Char = "pageMove.bt3.val=0";
	const char* pageMove_t6_Char = "pageMove.t6.txt=";

	BeginMethodSettings();
	Serial.print("pageCallerId: ");
	Serial.println(pageCallerId);


	// Set limit switches HIGH
	digitalWrite(limitPin_Max, HIGH);
	digitalWrite(limitPin_Min, HIGH);

	switch (axisId)
	{
		case ID_MOVE_AXIS_Z1:
		{
			limitPin_Max = configSetup.limit_Max_Z;
			limitPin_Min = configSetup.limit_Min_Z;

			SetEnable(ID_AXIS_Z, true, true);

			stepPin = PIN_AXIS_Z_STEP;
			dirPin = PIN_AXIS_Z_DIR;


			stepsToMove = (configMove.distance_MoveZ1 / (configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z));

			// Set speed and acceleration
			maxSpeed = configMove.speedPercent_Z * configMove.maxSpd_Z * .01;
			acceleration = configMove.accel_Z;


			break;
		}
		case ID_MOVE_AXIS_Z2:
		{
			limitPin_Max = configSetup.limit_Max_Z;
			limitPin_Min = configSetup.limit_Min_Z;
			SetEnable(ID_AXIS_Z, true, true);

			stepPin = PIN_AXIS_Z_STEP;
			dirPin = PIN_AXIS_Z_DIR;

			stepsToMove = (configMove.distance_MoveZ2 / (configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z));

			// Set speed and acceleration
			maxSpeed = configMove.speedPercent_Z * configMove.maxSpd_Z * .01;
			acceleration = configMove.accel_Z;

			break;
		}

		case ID_MOVE_AXIS_X1:
		{
			limitPin_Max = configSetup.limit_Max_X;
			limitPin_Min = configSetup.limit_Min_X;
			SetEnable(ID_AXIS_X, true, true);

			stepPin = PIN_AXIS_X_STEP;
			dirPin = PIN_AXIS_X_DIR;

	#ifdef VERBOSE
			Serial.print("xAltX: ");
			Serial.println(configSetup.xAltX);
	#endif // VERBOSE
			if (configSetup.xAltX == 0)
			{
				stepsToMove = (configMove.distance_MoveX1 / (configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X));

			}
			else
			{
				stepsToMove = (configMove.distance_MoveX1 / (configSetup.distancePerRev_AxisXAlt) * (configSetup.steps360_Axis_XAlt * configSetup.microsteps_Axis_XAlt));

			}

			// Set speed and acceleration
			maxSpeed = configMove.speedPercent_X * configMove.maxSpd_X * .01;
			acceleration = configMove.accel_X;

			break;
		}
		case ID_MOVE_AXIS_X2:
		{
			limitPin_Max = configSetup.limit_Max_X;
			limitPin_Min = configSetup.limit_Min_X;
			SetEnable(ID_AXIS_X, true, true);

			stepPin = PIN_AXIS_X_STEP;
			dirPin = PIN_AXIS_X_DIR;

	#ifdef VERBOSE
			Serial.print("xAltX: ");
			Serial.println(configSetup.xAltX);
	#endif // VERBOSE
			if (configSetup.xAltX == 0)
			{
				stepsToMove = (configMove.distance_MoveX2 / (configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X));
			}
			else
			{
				stepsToMove = (configMove.distance_MoveX2 / (configSetup.distancePerRev_AxisXAlt) * (configSetup.steps360_Axis_XAlt * configSetup.microsteps_Axis_XAlt));
			}

			// Set speed and acceleration
			maxSpeed = configMove.speedPercent_X * configMove.maxSpd_X * .01;
			acceleration = configMove.accel_X;


			SetEnable(ID_AXIS_X, true, true);

			break;
		}
		case ID_MOVE_AXIS_M3_1:
		{
			limitPin_Max = configSetup.limit_Max_M3;
			limitPin_Min = configSetup.limit_Min_M3;
			SetEnable(ID_AXIS_3, true, true);

			stepPin = PIN_AXIS_3_STEP;
			dirPin = PIN_AXIS_3_DIR;

			if (configSetup.radialOrLinear_Axis_M3 == RADIAL_M3)
			{
				stepsToMove = DistanceToSteps_M3Radial(configMove.distance_MoveM3_1);
			}
			else
			{
				stepsToMove = DistanceToSteps_M3Linear(configMove.distance_MoveM3_1);// *(-1);
			}


			// Set speed and acceleration
			maxSpeed = configMove.speedPercent_M3 * configMove.maxSpd_M3 * .01;
			acceleration = configMove.accel_M3;

			break;
		}
		case ID_MOVE_AXIS_M3_2:
		{
			limitPin_Max = configSetup.limit_Max_M3;
			limitPin_Min = configSetup.limit_Min_M3;
			SetEnable(ID_AXIS_3, true, true);

			stepPin = PIN_AXIS_3_STEP;
			dirPin = PIN_AXIS_3_DIR;

			if (configSetup.radialOrLinear_Axis_M3 == RADIAL_M3)
			{
				stepsToMove = DistanceToSteps_M3Radial(configMove.distance_MoveM3_2);
			}
			else
			{
				stepsToMove = DistanceToSteps_M3Linear(configMove.distance_MoveM3_2);// *(-1);
			}

			// Set speed and acceleration
			maxSpeed = configMove.speedPercent_M3 * configMove.maxSpd_M3 * .01;
			acceleration = configMove.accel_M3;

			break;
		}

		case ID_MOVE_AXIS_M4_1:
		{
			limitPin_Max = configSetup.limit_Max_M4;
			limitPin_Min = configSetup.limit_Min_M4;
			SetEnable(ID_AXIS_4, true, true);

			stepPin = PIN_AXIS_4_STEP;
			dirPin = PIN_AXIS_4_DIR;

			if (configSetup.radialOrLinear_Axis_M4 == RADIAL_M4)
			{
				stepsToMove = DistanceToSteps_M4Radial(configMove.distance_MoveM4_1);
			}
			else
			{
				stepsToMove = DistanceToSteps_M4Linear(configMove.distance_MoveM4_1);// *(-1);
			}

			// Set speed and acceleration
			maxSpeed = configMove.speedPercent_M4 * configMove.maxSpd_M4 * .01;
			acceleration = configMove.accel_M4;


			break;
		}
		case ID_MOVE_AXIS_M4_2:
		{
			limitPin_Max = configSetup.limit_Max_M4;
			limitPin_Min = configSetup.limit_Min_M4;
			SetEnable(ID_AXIS_4, true, true);

			stepPin = PIN_AXIS_4_STEP;
			dirPin = PIN_AXIS_4_DIR;

			if (configSetup.radialOrLinear_Axis_M4 == RADIAL_M4)
			{
				stepsToMove = DistanceToSteps_M4Radial(configMove.distance_MoveM4_2);
			}
			else
			{
				stepsToMove = DistanceToSteps_M4Linear(configMove.distance_MoveM4_2);// *(-1);
			}

			// Set speed and acceleration
			maxSpeed = configMove.speedPercent_M4 * configMove.maxSpd_M4 * .01;
			acceleration = configMove.accel_M4;

			break;
		}
	}

	// Set limit switches HIGH
	digitalWrite(limitPin_Max, HIGH);
	digitalWrite(limitPin_Min, HIGH);

	Stepper stepper_MoveAxis(stepPin, dirPin);
	stepper_MoveAxis.setPosition(0);

	// Set speed, acceleration, and target
	stepper_MoveAxis
		.setMaxSpeed(maxSpeed)
		.setAcceleration(acceleration)
		.setTargetAbs(stepsToMove * directionAxis);

	stepper_MoveAxis.moveAsync();


	//while (stepGroup.isRunning())
	while (stepper_MoveAxis.isMoving)
	{
		if (limitPin_Max > 9 && limitPin_Max < 40)
		{
			if ((digitalRead(limitPin_Max) != configSetup.limit_NCorNO) && directionAxis > 0)
			{
#ifdef DEBUG
				Serial.println("                          limitPin_Max");
#endif // DEBUG
				stepper_MoveAxis.emergencyStop();
				stopSteppers = true;
				SetLimitColors(PAGE_MOVE, directionAxis);
				goto endLoop;
			}
		}
		if (limitPin_Min > 9 && limitPin_Min < 40)
		{
			if ((digitalRead(limitPin_Min) != configSetup.limit_NCorNO) && directionAxis < 0)
			{
#ifdef DEBUG
				Serial.println("                          limitPin_Min");
#endif // DEBUG
				stepper_MoveAxis.emergencyStop();
				stopSteppers = true;
				SetLimitColors(PAGE_MOVE, directionAxis);
				goto endLoop;
			}
		}
//#ifdef DEBUG
//		Serial.print(" Position:  ");
//		Serial.print(stepsToMove* directionAxis);
//		Serial.print("  ");
//		Serial.println(stepper_MoveAxis.getPosition());
//		MilliDelay(100);
//#endif // DEBUG
		if (configSetup.eStop > 9 && configSetup.eStop < 40)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
#ifdef DEBUG
				//		Serial.println("                          EStop");
#endif // DEBUG
				stepper_MoveAxis.emergencyStop();
				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEStopColors(PAGE_MOVE);
				if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
				{
					digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
#ifdef DEBUG
					Serial.println("MotorPin: High =============================================");
					Serial.println(axisId);
#endif // Debug
				}
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
				stepper_MoveAxis.stop();
				stopSteppers = true;
				break;
			}
			}
		}

		MilliDelay(100);
		currentPosition = stepper_MoveAxis.getPosition();
		ProgressReport(stepsToMove * directionAxis, currentPosition);
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

			endPosition_Axis = stepper_MoveAxis.getPosition();
			startPositionAbs_Axis = endPosition_Axis;
			startPositionAbs_Axis = endPosition_Axis;
			endPosition_Z = endPosition_Axis;
			cumulative_Z = cumulative_Z + endPosition_Z;
	#ifdef VERBOSE
			Serial.print(position_Char);
			Serial.println(endPosition_Axis);
	#endif // VERBOSE

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

			endPosition_Axis = stepper_MoveAxis.getPosition();
			startPositionAbs_Axis = endPosition_Axis;
			startPositionAbs_Axis = endPosition_Axis;
			endPosition_X = endPosition_Axis;
			cumulative_X = cumulative_X + endPosition_X;
	#ifdef VERBOSE
			Serial.print(position_Char);
			Serial.println(endPosition_Axis);
	#endif // VERBOSE

			SetEnable(ID_AXIS_X, false, true);
			break;
		}
		case ID_MOVE_AXIS_M3_1:
		case ID_MOVE_AXIS_M3_2:
		{
			endPosition_Axis = stepper_MoveAxis.getPosition();
			startPositionAbs_Axis = endPosition_Axis;
			endPosition_M3 = endPosition_Axis;
			cumulative_M3 = cumulative_M3 + endPosition_M3;
	#ifdef VERBOSE
			Serial.print(position_Char);
			Serial.println(endPosition_Axis);
	#endif // VERBOSE

			SetEnable(ID_AXIS_3, false, true);
			break;
		}
		case ID_MOVE_AXIS_M4_1:
		case ID_MOVE_AXIS_M4_2:
		{
			endPosition_Axis = stepper_MoveAxis.getPosition();
			startPositionAbs_Axis = endPosition_Axis;
			endPosition_M4 = endPosition_Axis;
			cumulative_M4 = cumulative_M4 + endPosition_M4;
		
	#ifdef VERBOSE
			Serial.print(position_Char);
			Serial.println(endPosition_Axis);
	#endif // VERBOSE

			SetEnable(ID_AXIS_4, false, true);
			break;
		}
	}

	cumulative_Axis = cumulative_Axis + endPosition_Axis;

#ifdef DEBUG
	Serial.println("Move: ");
	Serial.print("endPosition_Axis: ");
	Serial.println(endPosition_Axis);
	Serial.print("cumulative_Axis: ");
	Serial.println(cumulative_Axis);
#endif // DEBUG

	// Update pageRec
	SerialPrint("pageRec.va0.val=0");
	SerialPrint(nextionEnd);
	SerialPrint("pageRec.bt1.val=0");
	SerialPrint(nextionEnd);

	// 0 = Change to disabled, 1 = keep enabled
	if (configSetup.keepSteppersEnabled == 1)
	{
		SerialPrint("pageMove.bt11.val=1"); // 1 = enabled
		SerialPrint("\xFF\xFF\xFF");
	}
	else
	{
		SerialPrint("pageMove.bt11.val=0"); // 0 = disabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageMove.bt11.txt="); // 0 = disabled
		SerialPrint("\x22");
		SerialPrint("Disabled");
		SerialPrint("\x22\xFF\xFF\xFF");
	}

	if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
	{
		digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
#ifdef DEBUG
		Serial.println("MotorPin: High =============================================");
		Serial.println(axisId);
#endif // Debug
	}
}


float CalculateNewSpeedRatio_One(uint32_t currentSpeed, uint32_t maxSpeed, uint32_t axisId)
{
	

	storedSpeedPercent = (int)GetSerialFloat(serialId);
	float newSpeedPercent = (float)(storedSpeedPercent * .01);

	int32_t desiredSpeed = (int32_t)(maxSpeed * newSpeedPercent);

	Serial.print("configOne.axisId: ");
	Serial.println(configOne.axisId);

	Serial.print("1.currentSpeed: ");
	Serial.println(currentSpeed);

	Serial.print("maxSpeed: ");
	Serial.println(maxSpeed);

	Serial.print("storedSpeedPercent:  ");
	Serial.println(storedSpeedPercent);

	Serial.print(" newSpeedPercent: ");
	Serial.println(newSpeedPercent);

	Serial.print("desiredSpeed: ");
	Serial.println(desiredSpeed);



	switch (configMulti.axisId)
	{
		case ID_AXIS_Z:
		{
			configOne.speedPercent_Z = storedSpeedPercent;
			currentSpeed_Axis = desiredSpeed;

			break;
		}
		case ID_AXIS_X:
		{
			configOne.speedPercent_X = newSpeedPercent;
			currentSpeed_Axis = desiredSpeed;
			break;
		}
		case ID_AXIS_3:
		{
			configOne.speedPercent_M3 = newSpeedPercent;
			currentSpeed_Axis = desiredSpeed;
			break;
		}
		case ID_AXIS_4:
		{
			configOne.speedPercent_M4 = newSpeedPercent;
			currentSpeed_Axis = desiredSpeed;
			break;
		}
		case ID_SPINDLE:
		{
			configOne.speedPercent_Spindle = newSpeedPercent;
			currentSpeed_Spindle = desiredSpeed;
			break;
		}
	}

	EEPROM.put(eePromAddress_One, configOne);

	Serial.print("2.currentSpeed: ");
	Serial.println(currentSpeed);
	float speedRatio = (float)(desiredSpeed) / (float)(currentSpeed);
	Serial.print("speedRatio: ");
	Serial.println(speedRatio);

	//return speedRatio;
	return newSpeedPercent;
}

float CalculateNewSpeedRatio_Multi(uint32_t currentSpeed, uint32_t maxSpeed, uint32_t axisId)
{


	storedSpeedPercent = (int)GetSerialFloat(serialId);
	float newSpeedPercent = (float)(storedSpeedPercent * .01);

	int32_t desiredSpeed = (int32_t)(maxSpeed * newSpeedPercent);

	Serial.print("configMulti.axisId: ");
	Serial.println(configMulti.axisId);

	Serial.print("1.currentSpeed: ");
	Serial.println(currentSpeed);

	Serial.print("maxSpeed: ");
	Serial.println(maxSpeed);

	Serial.print("storedSpeedPercent:  ");
	Serial.println(storedSpeedPercent);

	Serial.print(" newSpeedPercent: ");
	Serial.println(newSpeedPercent);

	Serial.print("desiredSpeed: ");
	Serial.println(desiredSpeed);



	switch (configMulti.axisId)
	{
		case ID_AXIS_Z:
		{
			configMulti.speedPercent_Z = storedSpeedPercent;
			currentSpeed_Axis = desiredSpeed;

			break;
		}
		case ID_AXIS_X:
		{
			configMulti.speedPercent_X = newSpeedPercent;
			currentSpeed_Axis = desiredSpeed;
			break;
		}
		case ID_AXIS_3:
		{
			configMulti.speedPercent_M3 = newSpeedPercent;
			currentSpeed_Axis = desiredSpeed;
			break;
		}
		case ID_AXIS_4:
		{
			configMulti.speedPercent_M4 = newSpeedPercent;
			currentSpeed_Axis = desiredSpeed;
			break;
		}
		case ID_SPINDLE:
		{
			configMulti.speedPercent_Spindle = newSpeedPercent;
			currentSpeed_Spindle = desiredSpeed;
			break;
		}
	}

	EEPROM.put(eePromAddress_Multi, configMulti);
	Serial.print("2.currentSpeed: ");
	Serial.println(currentSpeed);
	float speedRatio = (float)(desiredSpeed) / (float)(currentSpeed);
	Serial.print("speedRatio: ");
	Serial.println(speedRatio);

	//return speedRatio;
	return newSpeedPercent;
}


/// <summary>
/// RunOneStepper
/// </summary>
/// <comment>
/// </comment>
/// <param name="direction">Counterclockwise: -1, Clockwise: 1</param>
/// <returns></returns>
void OneStepper(int direction) // pageMulti
{
#ifdef VERBOSE
	Serial.println("Enter RunOneStepper.");

#endif // VERBOSE

	BeginMethodSettings();

	Stepper stepSpindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
	Stepper stepAxis_Z(PIN_AXIS_Z_STEP, PIN_AXIS_Z_DIR);
	Stepper stepAxis_X(PIN_AXIS_X_STEP, PIN_AXIS_X_DIR);
	Stepper stepAxis_M3(PIN_AXIS_3_STEP, PIN_AXIS_3_DIR);
	Stepper stepAxis_M4(PIN_AXIS_4_STEP, PIN_AXIS_4_DIR);

	stepAxis_Z.setAcceleration(0);
	stepAxis_X.setAcceleration(0);
	stepAxis_M3.setAcceleration(0);
	stepAxis_M4.setAcceleration(0);
	stepSpindle.setAcceleration(0);

	stepAxis_Z.setMaxSpeed(0);
	stepAxis_X.setMaxSpeed(0);
	stepAxis_M3.setMaxSpeed(0);
	stepAxis_M4.setMaxSpeed(0);
	stepSpindle.setMaxSpeed(0);

	stepAxis_Z.setPosition(0);
	stepAxis_X.setPosition(0);
	stepAxis_M3.setPosition(0);
	stepAxis_M4.setPosition(0);
	stepSpindle.setPosition(0);

	stepAxis_Z.setTargetAbs(0);
	stepAxis_X.setTargetAbs(0);
	stepAxis_M3.setTargetAbs(0);
	stepAxis_M4.setTargetAbs(0);
	stepSpindle.setTargetAbs(0);



	StepperGroup spindleGroup{ stepSpindle,stepAxis_M3 };

#ifdef VERBOSE
	Serial.print("1.pageCallerId:  ");
	Serial.println(pageCallerId);
#endif // VERBOSE
	checkedCount = 1;

	int limitPin_Max = configSetup.limit_Max_Z;
	int limitPin_Min = configSetup.limit_Min_Z;

	int maxSpd = 0;
	int accel = 0;

	int32_t axisId = 10;

//	int newSpeedPercent = 1;
	float speedPercent = 1;
	float currentSpeedPercent = 1;
//	float nextSpeedPercent = 1;
	float newSpeedRatio = 0;
//	int maxSpeedAndDirection = 0;
	int32_t speed_M3 = 1;

	int spindle_MaxSpd = 0;
	int z_MaxSpd = 0;
	int x_MaxSpd = 0;
	int m3_MaxSpd = 0;
	int m4_MaxSpd = 0;

	int spindle_Accel = 0;
	int z_Accel = 0;
	int x_Accel = 0;
	int m3_Accel = 0;
	int m4_Accel = 0;

	float spindle_speedPercent = 0;
	float z_speedPercent = 0;
	float x_speedPercent = 0;
	float m3_speedPercent = 0;
	float m4_speedPercent = 0;
	currentSpeed_Axis = 0;
	currentSpeed_Spindle = 0;
	switch (pageCallerId)
	{
		case PAGE_ONE:
		{ 
			 axisId = configOne.axisId;
			 spindle_MaxSpd = configOne.maxSpd_Spindle;
			 z_MaxSpd = configOne.maxSpd_Z;
			 x_MaxSpd = configOne.maxSpd_X;
			 m3_MaxSpd = configOne.maxSpd_M3;
			 m4_MaxSpd = configOne.maxSpd_M3;

			 spindle_Accel = configOne.accel_Spindle;
			 z_Accel = configOne.accel_Z;
			 x_Accel = configOne.accel_X;
			 m3_Accel = configOne.accel_M3;
			 m4_Accel = configOne.accel_M4;

			 spindle_speedPercent = configOne.speedPercent_Spindle;
			 z_speedPercent = configOne.speedPercent_Z;
			 x_speedPercent = configOne.speedPercent_X;
			 m3_speedPercent = configOne.speedPercent_M3;
			 m4_speedPercent = configOne.speedPercent_M4;
			break;
		}
		case PAGE_MULTI:
		{
			axisId = configMulti.axisId;
			spindle_MaxSpd = configMulti.maxSpd_Spindle;
			z_MaxSpd = configMulti.maxSpd_Z;
			x_MaxSpd = configMulti.maxSpd_X;
			m3_MaxSpd = configMulti.maxSpd_M3;
			m4_MaxSpd = configMulti.maxSpd_M3;

			spindle_Accel = configMulti.accel_Spindle;
			z_Accel = configMulti.accel_Z;
			x_Accel = configMulti.accel_X;
			m3_Accel = configMulti.accel_M3;
			m4_Accel = configMulti.accel_M4;

			spindle_speedPercent = configMulti.speedPercent_Spindle;
			z_speedPercent = configMulti.speedPercent_Z;
			x_speedPercent = configMulti.speedPercent_X;
			m3_speedPercent = configMulti.speedPercent_M3;
			m4_speedPercent = configMulti.speedPercent_M4;
			break;
		}
	}



	switch (axisId)
	{
		case ID_AXIS_Z:
		{
			stepPin = PIN_AXIS_Z_STEP;
			dirPin = PIN_AXIS_Z_DIR;
			maxSpd = z_MaxSpd;
			accel = z_Accel;
			speedPercent = z_speedPercent;
			currentSpeedPercent = (speedPercent) * .01;// *direction;
			currentSpeed_Axis = (uint32_t)(maxSpd * currentSpeedPercent);
			Serial.print("------------------------------------------currentSpeed_Axis: ");
			Serial.println(currentSpeed_Axis);

			limitPin_Max = configSetup.limit_Max_Z;
			limitPin_Min = configSetup.limit_Min_Z;
			stepAxis_Z.setMaxSpeed(maxSpd * direction);
			stepAxis_Z.setAcceleration(accel);

				Serial.print("axisId: ");
				Serial.println(axisId);
				Serial.print("spindle_MaxSpd: ");
				Serial.println(spindle_MaxSpd);
				Serial.print("z_MaxSpd: ");
				Serial.println(z_MaxSpd);
				Serial.print("x_MaxSpd: ");
				Serial.println(x_MaxSpd);
				Serial.print("m3_MaxSpd: ");
				Serial.println(m3_MaxSpd);
				Serial.print("m4_MaxSpd: ");
				Serial.println(m4_MaxSpd);
				Serial.print("currentSpeedPercent: ");
				Serial.println(currentSpeedPercent);
				Serial.println("|||ZZZZZZZZZZZZZZZZZZZZZZZZZZZ|||||||||||||||||||||||||||||||||");

			SetEnable(ID_AXIS_Z, true, true);
			stepAxis_Z.rotateAsync(currentSpeedPercent);
			
			break;
		}
		case ID_AXIS_X:
		{
			stepPin = PIN_AXIS_X_STEP;
			dirPin = PIN_AXIS_X_DIR;
			maxSpd = x_MaxSpd;
			accel = x_Accel;
			speedPercent = x_speedPercent;
			currentSpeedPercent = speedPercent * .01;// *direction;
			limitPin_Max = configSetup.limit_Max_X;
			limitPin_Min = configSetup.limit_Min_X;
			stepAxis_X.setMaxSpeed(maxSpd * direction);
			stepAxis_X.setAcceleration(accel);

			Serial.print("axisId: ");
			Serial.println(axisId);
			Serial.print("spindle_MaxSpd: ");
			Serial.println(spindle_MaxSpd);
			Serial.print("z_MaxSpd: ");
			Serial.println(z_MaxSpd);
			Serial.print("x_MaxSpd: ");
			Serial.println(x_MaxSpd);
			Serial.print("m3_MaxSpd: ");
			Serial.println(m3_MaxSpd);
			Serial.print("m4_MaxSpd: ");
			Serial.println(m4_MaxSpd);
			Serial.print("currentSpeedPercent: ");
			Serial.println(currentSpeedPercent);
			Serial.println("|||XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX|||||||||||||||||||||||||||||||||");
			SetEnable(ID_AXIS_X, true, true);
			stepAxis_X.rotateAsync(currentSpeedPercent);
			break;
		}
		case ID_AXIS_3:
		{
			stepPin = PIN_AXIS_3_STEP;
			dirPin = PIN_AXIS_3_DIR;
			maxSpd = m3_MaxSpd;
			accel = m3_Accel;
			speedPercent = m3_speedPercent;
			currentSpeedPercent = speedPercent * .01;// *direction;
	#ifdef VERBOSE
			Serial.print("configSetup.radialOrLinear_Axis_M3:  ");
			Serial.println(configSetup.radialOrLinear_Axis_M3);
	#endif // VERBOSE

			limitPin_Max = configSetup.limit_Max_M3;
			limitPin_Min = configSetup.limit_Min_M3;

			stepAxis_M3.setMaxSpeed(maxSpd * direction);
			stepAxis_M3.setAcceleration(accel);
			SetEnable(ID_AXIS_3, true, true);
			stepAxis_M3.rotateAsync(currentSpeedPercent);
			break;
		}
		case ID_AXIS_4:
		{
			stepPin = PIN_AXIS_4_STEP;
			dirPin = PIN_AXIS_4_DIR;
			maxSpd = m4_MaxSpd;
			accel = m4_Accel;
			speedPercent = m4_speedPercent;
			currentSpeedPercent = speedPercent * .01;// *direction;
	#ifdef VERBOSE
			Serial.print("configSetup.radialOrLinear_Axis_M4:  ");
			Serial.println(configSetup.radialOrLinear_Axis_M4);
			Serial.print("1-maxSpd:  ");
			Serial.println(maxSpd);
	#endif // VERBOSE
			limitPin_Max = configSetup.limit_Max_M4;
			limitPin_Min = configSetup.limit_Min_M4;

			stepAxis_M4.setMaxSpeed(maxSpd * direction);
			stepAxis_M4.setAcceleration(accel);
			SetEnable(ID_AXIS_4, true, true);
			stepAxis_M4.rotateAsync(currentSpeedPercent);
			break;
		}
		case ID_SPINDLE:
		{

			stepPin = PIN_SPINDLE_STEP;
			dirPin = PIN_SPINDLE_DIR;
			Serial.print("--------------StepPin: ");
			Serial.println(stepPin);
			Serial.print("dirPin: ");
			Serial.println(dirPin);

			maxSpd = spindle_MaxSpd;
			accel = spindle_Accel;
			speedPercent = spindle_speedPercent;
			currentSpeedPercent = speedPercent * .01;// *direction;

			currentSpeed_Spindle = (uint32_t)(maxSpd * currentSpeedPercent);
			stepSpindle.setMaxSpeed(maxSpd * direction);
			stepSpindle.setAcceleration(accel);
			stepSpindle.setPosition(0);

			Serial.print("axisId: ");
			Serial.println(axisId);
			Serial.print("spindle_MaxSpd: ");
			Serial.println(spindle_MaxSpd);
			Serial.print("z_MaxSpd: ");
			Serial.println(z_MaxSpd);
			Serial.print("x_MaxSpd: ");
			Serial.println(x_MaxSpd);
			Serial.print("m3_MaxSpd: ");
			Serial.println(m3_MaxSpd);
			Serial.print("m4_MaxSpd: ");
			Serial.println(m4_MaxSpd);
			Serial.print("speedPercent: ");
			Serial.println(speedPercent);
			Serial.print("currentSpeedPercent: ");
			Serial.println(currentSpeedPercent);
			Serial.println("|||SSSSSSSSSSSSSSSSSSSpindle||||||||||||||||||||||||||||||||");
			SetEnable(ID_SPINDLE, true, true);
			if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
			{
				stepperId_2 = ID_AXIS_3;
				stepper2_step = PIN_AXIS_3_STEP;
				stepper2_dir = PIN_AXIS_3_DIR;
#ifdef DEBUG
				Serial.print("maxSpd * direction: ");
				Serial.println(maxSpd * direction);
#endif // DEBUG

				speed_M3 = round((maxSpd * direction) * configMain.synchro_M3_Percentage * .01); // * synchro_Ratio
#ifdef DEBUG
				Serial.print("maxSpd * direction: ");
				Serial.println(maxSpd * direction);
				Serial.print("speed_M3:  ");
				Serial.println(speed_M3);
				Serial.print("configMain.synchro_M3_Percentage:  ");
				Serial.println(configMain.synchro_M3_Percentage);
#endif // DEBUG

				SetEnable(ID_AXIS_3, true, true);
				stepAxis_M3.setPosition(0);
				stepAxis_M3
					.setMaxSpeed(speed_M3)
					.setAcceleration(configRec.accel_M3);
				MilliDelay(5);

				Serial.println("rotateAsync Spindle 66666666666666666666666666666666");
				spindleGroup.startRotate();
			}
			else
			{
				Serial.print("Run Spindle only----------------------  ");
				Serial.println(currentSpeedPercent);
				

				stepSpindle.rotateAsync(currentSpeedPercent);
			}

			break;
		}
	}

	// Set limit switches HIGH
	digitalWrite(limitPin_Max, HIGH);
	digitalWrite(limitPin_Min, HIGH);

	Serial.println("888888888888888888888888888888888888888888");

	while (stepSpindle.isMoving || stepAxis_Z.isMoving || stepAxis_X.isMoving || stepAxis_M3.isMoving || stepAxis_M4.isMoving)
	{
		// Check for Cancel code  
		if (SerialAvailable() > 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
				case 83: // - S
				{
					MilliDelay(5);
					switch (configMulti.axisId)
					{
						case ID_AXIS_Z:
						{
							stepAxis_Z.stopAsync();
							break;
						}
						case ID_AXIS_X:
						{
							stepAxis_X.stopAsync();
							break;
						}
						case ID_AXIS_3:
						{
							stepAxis_M3.stopAsync();
							break;
						}
						case ID_AXIS_4:
						{
							Serial.println("M4 stopAsync next  ");
							stepAxis_M4.stop();
							//stepAxis_M4.stopAsync();
							Serial.println("M4 stopAsync  ");
							MilliDelay(200);
							Serial.println("MilliDelay  ");
							break;
						}
						case ID_SPINDLE:
						{

							if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
							{
								spindleGroup.stopAsync();
							}
							else
							{
								stepSpindle.stopAsync();
							}
							MilliDelay(200);
							break;
						}
					}


					//////////////////////////////////////////////////////////////////
					MilliDelay(100);
					goto endLoop;
					break;
				}
				case 110:  // Spindle speed
				{
					//newSpeedPercent = (int)GetSerialFloat(serialId);
					//configMulti.speedPercent_Spindle = newSpeedPercent;
					//currentSpeedPercent = (float)(newSpeedPercent * .01);// *direction);
					switch (pageCallerId)
					{
						case PAGE_ONE:
						{

							newSpeedRatio = CalculateNewSpeedRatio_One(currentSpeed_Spindle, maxSpd, configOne.axisId);
							break;
						}
						case PAGE_MULTI:
						{
							newSpeedRatio = CalculateNewSpeedRatio_Multi(currentSpeed_Spindle, maxSpd, configMulti.axisId);

							break;
						}
					}
					Serial.print(".........................newSpeedRatio:  ");
					Serial.println(newSpeedRatio);
					if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
					{

						spindleGroup.overrideSpeed(currentSpeedPercent); // set new speed
					}
					else
					{
						stepSpindle.overrideSpeed(newSpeedRatio);
					}

					//EEPROM.put(eePromAddress_Multi, configMulti);

					break;
				}
				case 111:  // Axis Speed
				{
					switch (pageCallerId)
					{
						case PAGE_ONE:
						{

							newSpeedRatio = CalculateNewSpeedRatio_One(currentSpeed_Axis, maxSpd, configOne.axisId);
							break;
						}
						case PAGE_MULTI:
						{
							newSpeedRatio = CalculateNewSpeedRatio_Multi(currentSpeed_Axis, maxSpd, configMulti.axisId);

							break;
						}
					}


					switch (configMulti.axisId)
					{
						case ID_AXIS_Z:
						{
							stepAxis_Z.overrideSpeed(newSpeedRatio);
							break;
						}
						case ID_AXIS_X:
						{
							//configMulti.speedPercent_Axis_X = newSpeedPercent;
							//currentSpeedPercent = (float)(newSpeedPercent * .01);// *direction);
							stepAxis_X.overrideSpeed(newSpeedRatio);
							break;
						}
						case ID_AXIS_3:
						{
							//configMulti.speedPercent_Axis_M3 = newSpeedPercent;
							//currentSpeedPercent = (float)(newSpeedPercent * .01);// *direction);
							stepAxis_M3.overrideSpeed(newSpeedRatio);
							break;
						}
						case ID_AXIS_4:
						{
							//configMulti.speedPercent_Axis_M4 = newSpeedPercent;
							//currentSpeedPercent = (float)(newSpeedPercent * .01);// *direction);
							stepAxis_M4.overrideSpeed(newSpeedRatio);
							break;
						}
					}

					break;
				}
				default:
				{
					break;
				}
			}
		}

		if (limitPin_Max > 9 && limitPin_Max < 40)
		{
			if ((digitalRead(limitPin_Max) != configSetup.limit_NCorNO) && direction == DIR_CW)
			{
				stepAxis_Z.emergencyStop();
				stepAxis_X.emergencyStop();
				stepAxis_M3.emergencyStop();
				stepAxis_M4.emergencyStop();
				stepSpindle.emergencyStop();
				spindleGroup.stopAsync();
				SetLimitColors(pageCallerId, DIR_CW);
				goto endLoop;
			}
		}
		if (limitPin_Min > 9 && limitPin_Min < 40)
		{
			if ((digitalRead(limitPin_Min) != configSetup.limit_NCorNO) && direction == DIR_CCW)
			{
				stepAxis_Z.emergencyStop();
				stepAxis_X.emergencyStop();
				stepAxis_M3.emergencyStop();
				stepAxis_M4.emergencyStop();
				stepSpindle.emergencyStop();
				spindleGroup.stopAsync();
				SetLimitColors(pageCallerId, DIR_CCW);
				goto endLoop;
			}
		}

#ifdef ESTOP_ENABLED
		//if (configSetup.eStop != 0)
		if (configSetup.eStop > 9 && configSetup.eStop < 40)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
#ifdef DEBUG
				Serial.println("                          EStop ");
#endif // DEBUG
				stepAxis_Z.emergencyStop();
				stepAxis_X.emergencyStop();
				stepAxis_M3.emergencyStop();
				stepAxis_M4.emergencyStop();
				stepSpindle.emergencyStop();
				spindleGroup.stopAsync();
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEnable(ID_AXIS_4, false, false);

				SetEStopColors(pageCallerId);
				if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
				{
					digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
				}
				return;
			}
		}
#endif

		MilliDelay(10);
	}

endLoop:

	Serial.println("End Loop");

	// Set Return steps values
	endPosition_Spindle = stepSpindle.getPosition();
	endPosition_Z = stepAxis_Z.getPosition();
	endPosition_X = stepAxis_X.getPosition();
	endPosition_M3 = stepAxis_M3.getPosition();
	endPosition_M4 = stepAxis_M4.getPosition();

	switch (axisId)
	{
		case ID_SPINDLE:
		{
			stepperId_1 = ID_SPINDLE;
			stepperPosition_1 = - endPosition_Spindle;

			if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
			{

				stepperId_2 = ID_AXIS_3;
				stepper2_step = PIN_AXIS_3_STEP;
				stepper2_dir = PIN_AXIS_3_DIR;
				stepperPosition_1 = - endPosition_M3;
			}
			break;
		}
		case ID_AXIS_Z:
		{
			stepperId_1 = ID_AXIS_Z;
			stepperPosition_1 = - endPosition_Z;

			break;
		}
		case ID_AXIS_X:
		{
			stepperId_1 = ID_AXIS_X;
			stepperPosition_1 = - endPosition_X;
			break;
		}
		case ID_AXIS_3:
		{
			stepperId_1 = ID_AXIS_3;
			stepperPosition_1 = - endPosition_M3;
			break;
		}
		case ID_AXIS_4:
		{
			stepperId_1 = ID_AXIS_4;
			stepperPosition_1 = - endPosition_M4;

			Serial.print("stepperPosition_1:  ");
			Serial.println(stepperPosition_1);
			stepAxis_M4.setPosition(0);
			break;
		}
	}

	stepper1_dir = dirPin;
	stepper1_step = stepPin;
	/////////////////////////////////////////
	//SetEnable(configMulti.axisId, false, true);
	//SetEnable(ID_SPINDLE, false, true);
	//SetEnable(ID_AXIS_Z, false, true);
	//SetEnable(ID_AXIS_X, false, true);
	//SetEnable(ID_AXIS_3, false, true);
	//SetEnable(ID_AXIS_4, false, true);

	
	switch (pageCallerId)
	{
		case PAGE_ONE:
		{
			SetEnable(axisId, false, true);
			break;
		}
		case PAGE_MULTI:
		{
			SetEnable(configMulti.axisId, false, true);
			break;
		}
	}


#ifdef VERBOSE
	Serial.print("keepSteppersEnabled:  ");
	Serial.println(configSetup.keepSteppersEnabled);
	Serial.print("axisId:  ");
	Serial.println(configMulti.axisId);
	Serial.print("		************pageCallerId:  ");
	Serial.println(pageCallerId);
#endif // VERBOSE

	switch (pageCallerId)
	{
		case PAGE_ONE:
		{

	#ifdef VERBOSE
			Serial.print("ONE--configMain.synchro_M3_Spindle:  ");
			Serial.println(configMain.synchro_M3_Spindle);
	#endif // VERBOSE

			Serial.println("Update Nextion One");
			switch (axisId)
			{
				case ID_AXIS_Z:
				{
					// 0 = Change to disabled, 1 = keep enabled
					if (configSetup.keepSteppersEnabled != 1)
					{
		#ifdef VERBOSE
						Serial.print("2.configSetup.keepSteppersEnabled: ");
						Serial.println(configSetup.keepSteppersEnabled);
		#endif // VERBOSE
						SerialPrint("pageOne.bt301.val=0"); // 0 = disabled
						SerialPrint("\xFF\xFF\xFF");
						MilliDelay(6);
						SerialPrint("pageOne.bt301.val=0"); // 0 = disabled
						SerialPrint("\xFF\xFF\xFF");
						MilliDelay(6);
						SerialPrint("pageOne.bt301.txt="); // 0 = disabled
						SerialPrint("\x22");
						SerialPrint("Disabled");
						SerialPrint("\x22\xFF\xFF\xFF");
#ifdef VERBOSE
						Serial.print("2a.configSetup.keepSteppersEnabled: ");
						Serial.println(configSetup.keepSteppersEnabled);
#endif // VERBOSE
					}
					break;
				}
				case ID_AXIS_X:
				{
					// 0 = Change to disabled, 1 = keep enabled
					if (configSetup.keepSteppersEnabled != 1)
					{
						SerialPrint("pageOne.bt302.val=0"); // 0 = disabled
						SerialPrint("\xFF\xFF\xFF");
						MilliDelay(6);
						SerialPrint("pageOne.bt302.txt="); // 0 = disabled
						SerialPrint("\x22");
						SerialPrint("Disabled");
						SerialPrint("\x22\xFF\xFF\xFF");
					}
					break;
				}
				case ID_AXIS_3:
				{
					Serial.print("^^^^^^^^^^^^^^^^^^^^keepSteppersEnabled:  ");
					Serial.println(configSetup.keepSteppersEnabled);
					// 0 = Change to disabled, 1 = keep enabled
					if (configSetup.keepSteppersEnabled != 1)
					{
						Serial.println(" >>>>>>>>> pageOne.bt303.val=0");
						SerialPrint("pageOne.bt303.val=0"); // 0 = disabled
						SerialPrint("\xFF\xFF\xFF");
						MilliDelay(6);
						SerialPrint("pageOne.bt303.txt="); // 0 = disabled
						SerialPrint("\x22");
						SerialPrint("Disabled");
						SerialPrint("\x22\xFF\xFF\xFF");
						MilliDelay(15);
						SerialPrint("pageOne.bt303.val=0"); // 0 = disabled
						SerialPrint("\xFF\xFF\xFF");
						MilliDelay(6);
						SerialPrint("pageOne.bt303.txt="); // 0 = disabled
						SerialPrint("\x22");
						SerialPrint("Disabled");
						SerialPrint("\x22\xFF\xFF\xFF");
					}
					break;
				}
				case ID_AXIS_4:
				{
					// 0 = Change to disabled, 1 = keep enabled
					if (configSetup.keepSteppersEnabled != 1)
					{
						SerialPrint("pageOne.bt304.val=0"); // 0 = disabled
						SerialPrint("\xFF\xFF\xFF");
						MilliDelay(6);
						SerialPrint("pageOne.bt304.txt="); // 0 = disabled
						SerialPrint("\x22");
						SerialPrint("Disabled");
						SerialPrint("\x22\xFF\xFF\xFF");
					}
					Serial.println(" Nextion updated?");
					MilliDelay(200);
					Serial.println(" Nextion updated?");
					break;
				}
				case ID_SPINDLE:
				{
					// 0 = Change to disabled, 1 = keep enabled
					if (configSetup.keepSteppersEnabled != 1)
					{
						SerialPrint("pageOne.bt300.val=0"); // 0 = disabled
						SerialPrint("\xFF\xFF\xFF");
						MilliDelay(6);
						SerialPrint("pageOne.bt300.txt="); // 0 = disabled
						SerialPrint("\x22");
						SerialPrint("Disabled");
						SerialPrint("\x22\xFF\xFF\xFF");
					}

					break;
				}
			}

			break;
		}
		case PAGE_MULTI:
		{
			Serial.println("Update Nextion MultiSync");
			switch (configMulti.axisId)
			{
				case ID_AXIS_Z:
				{
#ifdef VERBOSE
					Serial.print("5.configSetup.keepSteppersEnabled: ");
					Serial.println(configSetup.keepSteppersEnabled);
#endif // VERBOSE
					// 0 = Change to disabled, 1 = keep enabled
					if (configSetup.keepSteppersEnabled != 1)
					{

						SerialPrint("pageMulti.bt1301.val=0"); // 0 = disabled
						SerialPrint("\xFF\xFF\xFF");
						MilliDelay(6);
						SerialPrint("pageMulti.bt1301.txt="); // 0 = disabled
						SerialPrint("\x22");
						SerialPrint("Disabled");
						SerialPrint("\x22\xFF\xFF\xFF");
					}
					break;
				}
				case ID_AXIS_X:
				{
					// 0 = Change to disabled, 1 = keep enabled
					if (configSetup.keepSteppersEnabled != 1)
					{
						SerialPrint("pageMulti.bt1302.val=0"); // 0 = disabled
						SerialPrint("\xFF\xFF\xFF");
						MilliDelay(6);
						SerialPrint("pageMulti.bt1302.txt="); // 0 = disabled
						SerialPrint("\x22");
						SerialPrint("Disabled");
						SerialPrint("\x22\xFF\xFF\xFF");
					}
					break;
				}
				case ID_AXIS_3:
				{
					// 0 = Change to disabled, 1 = keep enabled
					if (configSetup.keepSteppersEnabled != 1)
					{
						SerialPrint("pageMulti.bt1303.val=0"); // 0 = disabled
						SerialPrint("\xFF\xFF\xFF");
						MilliDelay(6);
						SerialPrint("pageMulti.bt1303.txt="); // 0 = disabled
						SerialPrint("\x22");
						SerialPrint("Disabled");
						SerialPrint("\x22\xFF\xFF\xFF");
					}
					break;
				}
				case ID_AXIS_4:
				{
					// 0 = Change to disabled, 1 = keep enabled
					if (configSetup.keepSteppersEnabled != 1)
					{
						SerialPrint("pageMulti.bt1304.val=0"); // 0 = disabled
						SerialPrint("\xFF\xFF\xFF");
						MilliDelay(6);
						SerialPrint("pageMulti.bt1304.txt="); // 0 = disabled
						SerialPrint("\x22");
						SerialPrint("Disabled");
						SerialPrint("\x22\xFF\xFF\xFF");
					}
					break;
				}
				case ID_SPINDLE:
				{
					// 0 = Change to disabled, 1 = keep enabled
					if (configSetup.keepSteppersEnabled != 1)
					{
						SerialPrint("pageMulti.bt1300.val=0"); // 0 = disabled
						SerialPrint("\xFF\xFF\xFF");
						MilliDelay(6);
						SerialPrint("pageMulti.bt1300.txt="); // 0 = disabled
						SerialPrint("\x22");
						SerialPrint("Disabled");
						SerialPrint("\x22\xFF\xFF\xFF");
					}
					break;
				}
			}
			break;
		}
	}
#ifdef DEBUG
	Serial.print("endPosition_Spindle:  ");
	Serial.println(endPosition_Spindle);
	Serial.print("endPosition_Z:  ");
	Serial.println(endPosition_Z);
	Serial.print("endPosition_X:  ");
	Serial.println(endPosition_X);
	Serial.print("endPosition_M3:  ");
	Serial.println(endPosition_M3);
	Serial.print("endPosition_M4:  ");
	Serial.println(endPosition_M4);

#endif // DEBUG

	if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
	{
		digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
	}



	 maxSpd = 0;
	accel = 0;

	axisId = 10;

//	newSpeedPercent = 1;
	speedPercent = 1;
	currentSpeedPercent = 1;
//	nextSpeedPercent = 1;
	newSpeedRatio = 0;
//	maxSpeedAndDirection = 0;
	speed_M3 = 1;

	spindle_MaxSpd = 0;
	z_MaxSpd = 0;
	x_MaxSpd = 0;
	m3_MaxSpd = 0;
	m4_MaxSpd = 0;

	spindle_Accel = 0;
	z_Accel = 0;
	x_Accel = 0;
	m3_Accel = 0;
	m4_Accel = 0;

	spindle_speedPercent = 0;
	z_speedPercent = 0;
	x_speedPercent = 0;
	m3_speedPercent = 0;
	m4_speedPercent = 0;

	currentSpeed_Axis = 0;
	currentSpeed_Spindle = 0;

	Serial.println("Exit OneRun");
	return;
}

void Main_TwoSteppers(
	int direction_Axis,
	int direction_Spindle,
	int initialCaller
)
{
	Serial.print("Enter Main_TwoSteppers: ");
	Serial.print(direction_Axis);
	Serial.print("  ");
	Serial.print(direction_Spindle);
	Serial.print("  ");
	Serial.println(initialCaller);

	BeginMethodSettings();

	StepperGroup spindleM3Group{ stepperSpindle,stepperAxis_M3 };

	stepPin = PIN_AXIS_Z_STEP;
	dirPin = PIN_AXIS_Z_DIR;
	int limitPin_Max = configSetup.limit_Max_Z;
	int limitPin_Min = configSetup.limit_Min_Z;
	int maxSpd_Spindle = 0;
	int accel_Spindle = 0;
	float speedPercentAxis = 1;
	int32_t maxSpeed_Axis = 0;
	int32_t accel_Axis = 0;
	float speedPercentSpindle = 1;
//	float currentSpeedPercentSpindle = 1;
//	float currentSpeedPercentAxis = 1;
	int32_t currentSpindleSpeed = 0;
	int32_t currentAxisSpeed = 0;
	float overrideSpeedMultiplier = 1;
	int32_t newSpindleSpeed = 0;
	int32_t newAxisSpeed = 0;
	bool stepper_Axis_Go = false;
	bool stepper_Spindle_Go = false;
	bool keepAxisEnabledState = true;

//	const char* initialCaller_Char = "Initial Caller: ";
	const char* spindle_Char = "Spindle-";
//	const char* maxSpd_Char = "MaxSpeed: ";
//	const char* accel_Char = "Accel: ";

	const char* stop_Char = "Stop: ";
	const char* stopped_Char = "Stopped";
	const char* go_Char = "Go: ";
	const char* axis_Char = "Axis-";

	eStopTriggered = false;
	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);

	if (initialCaller > ID_SPINDLE)
	{
		stepper_Axis_Go = true;
	}
	else
	{
		stepper_Spindle_Go = true;
		stepperId_1 = ID_SPINDLE;
		stepper1_step = PIN_SPINDLE_STEP;
		stepper1_dir = PIN_SPINDLE_DIR;
	}

	stepperAxis_Z.setPosition(0);
	stepperAxis_X.setPosition(0);
	stepperAxis_M3.setPosition(0);
	stepperAxis_M4.setPosition(0);
	stepperSpindle.setPosition(0);

	// Set MaxSpd and Accel
	switch (configMain.axisId)
	{
		case ID_AXIS_Z:
		{
			speedPercentAxis = static_cast<float>(configMain.speedPercent_Z * .01);
			stepPin = PIN_AXIS_Z_STEP;
			dirPin = PIN_AXIS_Z_DIR;

			limitPin_Max = configSetup.limit_Max_Z;
			limitPin_Min = configSetup.limit_Min_Z;
			maxSpeed_Axis = configMain.maxSpd_Z * direction_Axis;

			currentAxisSpeed = maxSpeed_Axis * speedPercentAxis;

			Serial.print("maxSpeed_Axis: ");
			Serial.print(maxSpeed_Axis);
			Serial.print("  currentAxisSpeed: ");
			Serial.println(currentAxisSpeed);

			accel_Axis = configMain.accel_Z;
			stepperAxis_Z.setPosition(0);
			stepperAxis_Z
				.setMaxSpeed(currentAxisSpeed)
				.setAcceleration(accel_Axis);

			MilliDelay(5);

			break;
		}
		case ID_AXIS_X:
		{

			speedPercentAxis = static_cast<float>(configMain.speedPercent_X * .01);
			stepPin = PIN_AXIS_X_STEP;
			dirPin = PIN_AXIS_X_DIR;

			limitPin_Max = configSetup.limit_Max_X;
			limitPin_Min = configSetup.limit_Min_X;
			maxSpeed_Axis = configMain.maxSpd_X * direction_Axis;

			currentAxisSpeed = maxSpeed_Axis * speedPercentAxis;
			accel_Axis = configMain.accel_X;
			stepperAxis_X.setPosition(0);
			stepperAxis_X
				.setMaxSpeed(currentAxisSpeed)
				.setAcceleration(accel_Axis);

			MilliDelay(5);

			break;
		}
		case ID_AXIS_3:
		{
			Serial.println(">>>>>>>>>>M3 selected");
			speedPercentAxis = static_cast<float>(configMain.speedPercent_M3 * .01);
			stepPin = PIN_AXIS_3_STEP;
			dirPin = PIN_AXIS_3_DIR;

			limitPin_Max = configSetup.limit_Max_M3;
			limitPin_Min = configSetup.limit_Min_M3;
			maxSpeed_Axis = configMain.maxSpd_M3 * direction_Axis;
			currentAxisSpeed = maxSpeed_Axis * speedPercentAxis;
			accel_Axis = configMain.accel_M3;
			stepperAxis_M3.setPosition(0);
			stepperAxis_M3.setMaxSpeed(currentAxisSpeed);
			stepperAxis_M3.setAcceleration(accel_Axis);

			MilliDelay(5);

			break;
		}
		case ID_AXIS_4:
		{
			speedPercentAxis = static_cast<float>(configMain.speedPercent_M4 * .01);
			stepPin = PIN_AXIS_4_STEP;
			dirPin = PIN_AXIS_4_DIR;

			limitPin_Max = configSetup.limit_Max_M4;
			limitPin_Min = configSetup.limit_Min_M4;
			maxSpeed_Axis = configMain.maxSpd_M4 * direction_Axis;
			currentAxisSpeed = maxSpeed_Axis * speedPercentAxis;
			accel_Axis = configMain.accel_M4;
			stepperAxis_M4.setPosition(0);
			stepperAxis_M4
				.setMaxSpeed(currentAxisSpeed)
				.setAcceleration(accel_Axis);

			MilliDelay(5);

			break;
		}
	}

	stepperId_2 = configMain.axisId;
	stepper2_step = stepPin;
	stepper2_dir = dirPin;

	// Set limit switches HIGH
	digitalWrite(limitPin_Max, HIGH);
	digitalWrite(limitPin_Min, HIGH);


	maxSpd_Spindle = configMain.maxSpd_Spindle;
	accel_Spindle = configMain.accel_Spindle;
	speedPercentSpindle = (float)(configMain.speedPercent_Spindle * .01);
	currentSpindleSpeed = maxSpd_Spindle * direction_Spindle * speedPercentSpindle;

	MilliDelay(5);
#ifdef VERBOSE
	Serial.print("maxSpd_Spindle * direction_Spindle:  ");
	Serial.println(maxSpd_Spindle * direction_Spindle);
	Serial.print("accel_Spindle:  ");
	Serial.println(accel_Spindle);
#endif // VERBOSE
	stepperSpindle
		.setMaxSpeed(currentSpindleSpeed)
		.setAcceleration(accel_Spindle);

	checkedCount = 2;

	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		checkedCount = 3;
		stepperId_3 = ID_AXIS_3;
		stepper3_step = PIN_AXIS_3_STEP;
		stepper3_dir = PIN_AXIS_3_DIR;
		MilliDelay(5);
	}

	if (stepper_Axis_Go)
	{
		keepAxisEnabledState = false;

		// Rotate Axis
		switch (configMain.axisId)
		{
			case ID_AXIS_Z:
			{
				SetEnable(ID_AXIS_Z, true, true);
				Serial.print("RotateAsync..................Z: ");
				Serial.println(configMain.speedPercent_Z * .01);

				stepperAxis_Z.rotateAsync(configMain.speedPercent_Z * .01);

				break;
			}
			case ID_AXIS_X:
			{
				SetEnable(ID_AXIS_X, true, true);
				Serial.println("RotateAsync.......................X: ");
				Serial.println(configMain.speedPercent_X * .01);
				stepperAxis_X.rotateAsync(configMain.speedPercent_X * .01);

				break;
			}
			case ID_AXIS_3:
			{
				Serial.println("                             Rotate M3");
				SetEnable(ID_AXIS_3, true, true);
				Serial.print("M3 maxSpd:");
				Serial.println(configMain.maxSpd_M3 * direction_Axis);
				Serial.print("M3 accel:");
				Serial.println(configMain.accel_M3);
				Serial.print("M3 speed percent:");
				Serial.println(configMain.speedPercent_M3 * .01);

				stepperAxis_M3.setPosition(0);
				stepperAxis_M3.setAcceleration(configMain.accel_M3);
				stepperAxis_M3.setMaxSpeed(configMain.maxSpd_M3 * direction_Axis);
				digitalWrite(PIN_AXIS_3_ENABLE, LOW);
				Serial.println("RotateAsync.......................M3....");
				stepperAxis_M3.rotateAsync(configMain.speedPercent_M3 * .01);

				MilliDelay(100);
				Serial.print("Position:  ");
				Serial.println(stepperAxis_M3.getPosition());
				break;
			}
			case ID_AXIS_4:
			{
				SetEnable(ID_AXIS_4, true, true);
				stepperAxis_M4.rotateAsync(configMain.speedPercent_M4 * .01);
				////MilliDelay(10);
				break;
			}
		}

#ifdef VERBOSE
		Serial.print(axis_Char);
		Serial.print(go_Char);
		Serial.println(stepper_Axis_Go);
#endif // VERBOSE
	}

	if (stepper_Spindle_Go)
	{
		//SetEnable(ID_SPINDLE, true, true);
		digitalWrite(PIN_SPINDLE_ENABLE, LOW);
		MilliDelay(5);


		if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
		{
			currentSpindleSpeed = maxSpd_Spindle * direction_Spindle * speedPercentSpindle;

			stepperAxis_M3.setPosition(0);
			stepperAxis_M3
				.setMaxSpeed(currentSpindleSpeed * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01))
				.setAcceleration(accel_Spindle);

			MilliDelay(5);
			SetEnable(ID_AXIS_3, true, true);
			SetEnable(ID_SPINDLE, true, true);
			Serial.print("currentSpindleSpeed:  ");
			Serial.println(currentSpindleSpeed);
			Serial.print("currentSpindleSpeed * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01)");
			Serial.println(currentSpindleSpeed * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01));
			Serial.println("M3 added >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
			speedPercentSpindle = (float)(configMain.speedPercent_Spindle * .01);

			stepperSpindle.setPosition(0);
			stepperSpindle
				.setMaxSpeed(currentSpindleSpeed)
				.setAcceleration(accel_Spindle);
			//spindleM3Group.overrideSpeed(1);
			Serial.println("spindleM3Group.startRotate");
			spindleM3Group.startRotate();
			//spindleM3Group.overrideSpeed(1);

		}
		else
		{
			speedPercentSpindle = (float)(configMain.speedPercent_Spindle * .01);
			Serial.print("configMain.speedPercent_Spindle:  ");
			Serial.println(configMain.speedPercent_Spindle);
			Serial.print("maxSpd_Spindle * direction_Spindle * speedPercentSpindle:  ");
			Serial.println(maxSpd_Spindle * direction_Spindle * speedPercentSpindle);
			Serial.println("==========================");

			////stepSpindle
			////	.setMaxSpeed(maxSpd_Spindle * direction_Spindle * speedPercentSpindle)
			////	.setAcceleration(accel_Spindle);

			stepperSpindle.setPosition(0);
			stepperSpindle.rotateAsync(maxSpd_Spindle * direction_Spindle * speedPercentSpindle);

		}
	}

	while (stepper_Axis_Go || stepper_Spindle_Go || stepperAxis_M3.isMoving)
	{
#ifdef ESTOP_ENABLED
		if (configSetup.eStop > 9 && configSetup.eStop < 40)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
#ifdef DEBUG
				Serial.println("                          EStop ");
#endif // DEBUG
				stepperSpindle.emergencyStop();
				stepperAxis_Z.emergencyStop();
				stepperAxis_X.emergencyStop();
				stepperAxis_M3.emergencyStop();
				stepperAxis_M4.emergencyStop();

				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEnable(ID_AXIS_4, false, false);
				// Change Btn color on Nextion
				SetEStopColors(PAGE_MAIN);
				if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
				{
					digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
				}
				return;
			}
		}

#endif // Estop
		MilliDelay(5);
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);

			switch (incomingByte)
			{
			case 66: //B - AxisId
			{
				configMain.axisId = GetSerialInteger();
				EEPROM.put(eePromAddress_Main, configMain);
				break;
			}
			case 83: // - S Spindle stop
			{
#ifdef VERBOSE
				Serial.print(spindle_Char);
				Serial.print(stop_Char);
#endif // VERBOSE
				MilliDelay(5);
				if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
				{
					spindleM3Group.stopAsync();

					SetEnable(ID_AXIS_3, false, true);
				}
				else
				{
					stepperSpindle.stopAsync();
				}
				MilliDelay(200);


#ifdef VERBOSE
				Serial.print(spindle_Char);
				Serial.println(stopped_Char);
#endif // VERBOSE

				SetEnable(ID_SPINDLE, false, true);
				stepper_Spindle_Go = false;

				// 0 = Change to disabled, 1 = keep enabled
				if (configSetup.keepSteppersEnabled == 1)
				{
					SerialPrint("pageMain.bt10.val=1"); // 1 = enabled
					SerialPrint("\xFF\xFF\xFF");
				}
				else
				{
					SerialPrint("pageMain.bt10.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMain.bt10.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
				}
				break;
			}
			case 90: // Z - Z Axis CW
			{
				if (!stepperAxis_Z.isMoving)
				{
					stepper_Axis_Go = true;
					direction_Axis = DIR_CW;

					speedPercentAxis = (float)((configMain.speedPercent_Z) * .01);
					stepperAxis_Z.setMaxSpeed(configMain.maxSpd_Z * direction_Axis);

					SetEnable(ID_AXIS_Z, true, true);
					stepperAxis_Z.rotateAsync();
				}
				break;
			}
			case 91: // [ - Z Axis CCW
			{
				if (!stepperAxis_Z.isMoving)
				{
					stepper_Axis_Go = true;
					direction_Axis = DIR_CCW;

					speedPercentAxis = (float)((configMain.speedPercent_Z) * .01);
					stepperAxis_Z.setMaxSpeed(configMain.maxSpd_Z * direction_Axis);

					SetEnable(ID_AXIS_Z, true, true);
					stepperAxis_Z.rotateAsync();
				}
				break;
			}
			case 99: // c - Axis stop
			{

#ifdef VERBOSE
				Serial.print("configMain.axisId:  ");
				Serial.println(configMain.axisId);
#endif // VERBOSE
				switch (configMain.axisId)
				{
				case ID_AXIS_Z:
				{
					stepperAxis_Z.stop();
					SetEnable(ID_AXIS_Z, false, true);
					break;
				}
				case ID_AXIS_X:
				{
					stepperAxis_X.stop();
					SetEnable(ID_AXIS_X, false, true);
					break;
				}
				case ID_AXIS_3:
				{
					stepperAxis_M3.stop();
					SetEnable(ID_AXIS_3, false, true);
					break;
				}
				case ID_AXIS_4:
				{
					stepperAxis_M4.stop();
					SetEnable(ID_AXIS_4, false, true);
					break;
				}
				}
				MilliDelay(200);
				stepper_Axis_Go = false;
				// 0 = Change to disabled, 1 = keep enabled
				if (configSetup.keepSteppersEnabled == 1)
				{
					SerialPrint("pageMain.bt11.val=1"); // 1 = enabled
					SerialPrint("\xFF\xFF\xFF");
				}
				else
				{
					SerialPrint("pageMain.bt11.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMain.bt11.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
				}

				// Ensure user doesn't switch axes too fast.
				MilliDelay(200);

				break;
			}
			case 107: // k - Spindle CW
			{
				if (!stepperSpindle.isMoving)
				{
					direction_Spindle = DIR_CW;

					speedPercentSpindle = (float)(configMain.speedPercent_Spindle * .01);
					currentSpindleSpeed = static_cast<int32_t>(maxSpd_Spindle * direction_Spindle * speedPercentSpindle);
					stepperSpindle.setMaxSpeed(currentSpindleSpeed);
					stepper_Spindle_Go = true;
					SetEnable(ID_SPINDLE, true, true);

					Serial.print("configMain.synchro_M3_Spindle: ");
					Serial.println(configMain.synchro_M3_Spindle);

					if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
					{
						////#ifdef VERBOSE
						////						Serial.println("                 CW    Synchro enabled");
						////						Serial.print("maxSpd_Spindle * direction_Spindle * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01):  ");
						////						Serial.println(maxSpd_Spindle * direction_Spindle * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01));
						////
						////						Serial.print("synchro_Ratio:  ");
						////						Serial.println(synchro_Ratio);
						////
						////						Serial.print("configMain.synchro_M3_Percentage * 0.01:  ");
						////						Serial.println(configMain.synchro_M3_Percentage * 0.01);
						////#endif // VERBOSE
						stepperAxis_M3.setPosition(0);
						stepperAxis_M3
							.setMaxSpeed(currentSpindleSpeed * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01))
							.setAcceleration(accel_Spindle);
						//stepAxis_M3
						//	.setMaxSpeed(maxSpd_Spindle * direction_Spindle * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01))
						//	.setAcceleration(accel_Spindle);

						MilliDelay(5);
						SetEnable(ID_AXIS_3, true, true);
						spindleM3Group.startRotate();
					}
					else
					{
						stepperSpindle.rotateAsync(maxSpd_Spindle * speedPercentSpindle);
					}
					Serial.println("CW");


				}

				break;
			}
			case 108: // l - Spindle CCW
			{
				if (!stepperSpindle.isMoving)
				{
					Serial.println("CCW");
					direction_Spindle = DIR_CCW;
					speedPercentSpindle = (float)(configMain.speedPercent_Spindle * .01);
					currentSpindleSpeed = static_cast<int32_t>(maxSpd_Spindle * direction_Spindle * speedPercentSpindle);
					stepperSpindle.setMaxSpeed(currentSpindleSpeed);
					stepper_Spindle_Go = true;

					SetEnable(ID_SPINDLE, true, true);

					Serial.print("configMain.synchro_M3_Spindle: ");
					Serial.println(configMain.synchro_M3_Spindle);
					if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
					{

						stepperAxis_M3.setPosition(0);
						stepperAxis_M3
							.setMaxSpeed(currentSpindleSpeed * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01))// * DIR_CCW)
							.setAcceleration(accel_Spindle);
						////stepAxis_M3
						////	.setMaxSpeed(maxSpd_Spindle * direction_Spindle * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01))// * DIR_CCW)
						////	.setAcceleration(accel_Spindle);
						MilliDelay(5);
						SetEnable(ID_AXIS_3, true, true);
						spindleM3Group.startRotate();

					}
					else
					{
						stepperSpindle.rotateAsync(maxSpd_Spindle * speedPercentSpindle * DIR_CCW);
					}
				}
				break;
			}
			case 110: // n - Main spindle speed
			{

				configMain.speedPercent_Spindle = (int)GetSerialFloat(serialId);
				speedPercentSpindle = (float)(configMain.speedPercent_Spindle * .01);
				newSpindleSpeed = static_cast<int32_t>(maxSpd_Spindle * direction_Spindle * configMain.speedPercent_Spindle * .01);

				overrideSpeedMultiplier = static_cast<float>(newSpindleSpeed) / static_cast<float>(currentSpindleSpeed);
				EEPROM.put(eePromAddress_Main, configMain);

				Serial.print("CurrentSpindleSpeed: ");
				Serial.println(currentSpindleSpeed);
				Serial.print("newSpindleSpdde:");
				Serial.println(newSpindleSpeed);
				Serial.print("overrideSpeedMultiplier:  ");
				Serial.println(overrideSpeedMultiplier);
				if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
				{
					spindleM3Group.overrideSpeed(overrideSpeedMultiplier);
				}
				else
				{
					stepperSpindle.overrideSpeed(overrideSpeedMultiplier);
				}
				break;
			}
			case 111: // Axis Speed
			{
				int newSpeedPercent = (int)GetSerialFloat(serialId);

				switch (configMain.axisId)
				{
				case ID_AXIS_Z:
				{
					configMain.speedPercent_Z = newSpeedPercent;
					speedPercentAxis = (float)((newSpeedPercent) * .01);

					newAxisSpeed = static_cast<int32_t>(maxSpeed_Axis * direction_Spindle * configMain.speedPercent_Z * .01);
					overrideSpeedMultiplier = static_cast<float>(newAxisSpeed) / static_cast<float>(currentSpindleSpeed);

					stepperAxis_Z.overrideSpeed(overrideSpeedMultiplier);
					break;
				}
				case ID_AXIS_X:
				{
					configMain.speedPercent_X = newSpeedPercent;
					speedPercentAxis = (float)(newSpeedPercent * .01);

					newAxisSpeed = static_cast<int32_t>(maxSpeed_Axis * direction_Spindle * configMain.speedPercent_X * .01);
					overrideSpeedMultiplier = static_cast<float>(newAxisSpeed) / static_cast<float>(currentSpindleSpeed);
					stepperAxis_X.overrideSpeed(speedPercentAxis);
					break;
				}
				case ID_AXIS_3:
				{
					configMain.speedPercent_M3 = newSpeedPercent;
					speedPercentAxis = (float)((newSpeedPercent) * .01);

					newAxisSpeed = static_cast<int32_t>(maxSpeed_Axis * direction_Spindle * configMain.speedPercent_M3 * .01);
					overrideSpeedMultiplier = static_cast<float>(newAxisSpeed) / static_cast<float>(currentSpindleSpeed);
					stepperAxis_M3.overrideSpeed(speedPercentAxis);
					break;
				}
				case ID_AXIS_4:
				{
					configMain.speedPercent_M4 = newSpeedPercent;
					speedPercentAxis = (float)((newSpeedPercent) * .01);
					newAxisSpeed = static_cast<int32_t>(maxSpeed_Axis * direction_Spindle * configMain.speedPercent_M4 * .01);
					overrideSpeedMultiplier = static_cast<float>(newAxisSpeed) / static_cast<float>(currentSpindleSpeed);
					stepperAxis_M4.overrideSpeed(speedPercentAxis);
					break;
				}
				}
				EEPROM.put(eePromAddress_Main, configMain);

				break;
			}
			case 165: // ¥ - X Axis CCW
			{
				if (!stepperAxis_X.isMoving)
				{
					stepper_Axis_Go = true;
					direction_Axis = DIR_CCW;

					speedPercentAxis = (float)(configMain.speedPercent_X * .01);
					stepperAxis_X.setMaxSpeed(configMain.maxSpd_X * direction_Axis);

					SetEnable(ID_AXIS_X, true, true);
					stepperAxis_X.rotateAsync();
					stepperAxis_X.overrideSpeed(speedPercentAxis);
					MilliDelay(5);

				}
				break;
			}
			case 166: // ¦ - X Axis CW
			{

				if (!stepperAxis_X.isMoving)
				{
					stepper_Axis_Go = true;
					direction_Axis = DIR_CW;

					speedPercentAxis = (float)(configMain.speedPercent_X * .01);
					stepperAxis_X.setMaxSpeed(configMain.maxSpd_X * direction_Axis);

					SetEnable(ID_AXIS_X, true, true);
					stepperAxis_X.rotateAsync(speedPercentAxis);
					stepperAxis_X.overrideSpeed(speedPercentAxis);
					MilliDelay(5);

				}

				break;
			}
			case 171: // « - M3 Axis CCW
			{
				if (!stepperAxis_M3.isMoving)
				{
					stepper_Axis_Go = true;
					direction_Axis = DIR_CCW;

					//if (configSetup.radialOrLinear_Axis_M3 == 1) // Linear
					//{
					//	direction_Axis = DIR_CW;
					//}

#ifdef VERBOSE
					Serial.print("configMain.maxSpd_Axis_M3* direction_Axis:  ");
					Serial.println(configMain.maxSpd_M3 * direction_Axis);
#endif // VERBOSE

					speedPercentAxis = (float)((configMain.speedPercent_M3) * .01);
					stepperAxis_M3.setMaxSpeed(configMain.maxSpd_M3 * direction_Axis);

					SetEnable(ID_AXIS_3, true, true);
					stepperAxis_M3.rotateAsync();
					stepperAxis_M3.overrideSpeed(speedPercentAxis);
					MilliDelay(5);

				}
				break;
			}
			case 172: // ¬ - M3 Axis CW
			{
				if (!stepperAxis_M3.isMoving)
				{
					stepper_Axis_Go = true;
					direction_Axis = DIR_CW;

#ifdef VERBOSE
					Serial.print("configMain.maxSpd_Axis_M3* direction_Axis:  ");
					Serial.println(configMain.maxSpd_M3 * direction_Axis);
#endif // VERBOSE

					speedPercentAxis = (float)((configMain.speedPercent_M3) * .01);
					stepperAxis_M3.setMaxSpeed(configMain.maxSpd_M3 * direction_Axis);

					SetEnable(ID_AXIS_3, true, true);
					stepperAxis_M3.rotateAsync();
					stepperAxis_M3.overrideSpeed(speedPercentAxis);
					MilliDelay(5);

				}
				break;
			}
			case 209: // « - M4 Axis CCW
			{
				if (!stepperAxis_M4.isMoving)
				{
					stepper_Axis_Go = true;
					direction_Axis = DIR_CCW;

					if (configSetup.radialOrLinear_Axis_M4 == 1) // Linear
					{
						direction_Axis = DIR_CW;
					}

					speedPercentAxis = (float)((configMain.speedPercent_M4) * .01);
					stepperAxis_M4.setMaxSpeed(configMain.maxSpd_M4 * direction_Axis);

					SetEnable(ID_AXIS_4, true, true);
					stepperAxis_M4.rotateAsync();
					stepperAxis_M4.overrideSpeed(speedPercentAxis);
					MilliDelay(5);

				}
				break;
			}
			case 210: // ¬ - M4 Axis CW
			{
				if (!stepperAxis_M4.isMoving)
				{
					stepper_Axis_Go = true;
					direction_Axis = DIR_CW;

					if (configSetup.radialOrLinear_Axis_M4 == 1) // Linear
					{
						direction_Axis = DIR_CCW;
					}

					speedPercentAxis = (float)((configMain.speedPercent_M4) * .01);
					stepperAxis_M4.setMaxSpeed(configMain.maxSpd_M4 * direction_Axis);

					SetEnable(ID_AXIS_4, true, true);
					stepperAxis_M4.rotateAsync();
					stepperAxis_M4.overrideSpeed(speedPercentAxis);
					MilliDelay(5);

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
			if (limitPin_Max > 9 && limitPin_Max < 40)
			{
				if ((digitalRead(limitPin_Max) != configSetup.limit_NCorNO) && direction_Axis == DIR_CW)
				{
#ifdef DEBUG
					Serial.print("Limit_Max - CW - Tripped:  ");
					Serial.println(configSetup.limit_NCorNO);
#endif // DEBUG

					switch (configMain.axisId)
					{
					case ID_AXIS_Z:
					{
						stepperAxis_Z.emergencyStop();
						break;
					}
					case ID_AXIS_X:
					{
						stepperAxis_X.emergencyStop();
						break;
					}
					case ID_AXIS_3:
					{
						stepperAxis_M3.emergencyStop();
						break;
					}
					case ID_AXIS_4:
					{
						stepperAxis_M4.emergencyStop();
						break;
					}
					}
					/////////////////////////////
					if (configSetup.limit_StopSpindle)
					{
						// Stop the spindle
						stepperSpindle.overrideSpeed(0);
						MilliDelay(5);
						stepperSpindle.emergencyStop();

						if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
						{
							stepperAxis_M3.emergencyStop();
							SetEnable(ID_AXIS_3, false, true);
						}
						SetEnable(ID_SPINDLE, false, true);
						stepper_Spindle_Go = false;

						// 0 = Change to disabled, 1 = keep enabled
						if (configSetup.keepSteppersEnabled == 1)
						{
							SerialPrint("pageMain.bt10.val=1"); // 1 = enabled
							SerialPrint("\xFF\xFF\xFF");
						}
						else
						{
							SerialPrint("pageMain.bt10.val=0"); // 0 = disabled
							SerialPrint("\xFF\xFF\xFF");
							SerialPrint("pageMain.bt10.txt="); // 0 = disabled
							SerialPrint("\x22");
							SerialPrint("Disabled");
							SerialPrint("\x22\xFF\xFF\xFF");
						}
					}

					stepper_Axis_Go = false;

					// 0 = Change to disabled, 1 = keep enabled
					if (configSetup.keepSteppersEnabled == 1)
					{
						SerialPrint("pageMain.bt11.val=1"); // 1 = enabled
						SerialPrint("\xFF\xFF\xFF");
					}
					else
					{
						SerialPrint("pageMain.bt11.val=0"); // 0 = disabled
						SerialPrint("\xFF\xFF\xFF");
						SerialPrint("pageMain.bt11.txt="); // 0 = disabled
						SerialPrint("\x22");
						SerialPrint("Disabled");
						SerialPrint("\x22\xFF\xFF\xFF");
					}
					SetLimitColors(PAGE_MAIN, direction_Axis);

				}
			}
			if (limitPin_Min > 9 && limitPin_Min < 40)
			{
				if ((digitalRead(limitPin_Min) != configSetup.limit_NCorNO) && direction_Axis == DIR_CCW)
				{
#ifdef DEBUG
					Serial.println("Limit_Min - CCW - Tripped:  ");
					Serial.println(configSetup.limit_NCorNO);
#endif // DEBUG
					stepper_Axis_Go = false;

					switch (configMain.axisId)
					{
					case ID_AXIS_Z:
					{
						stepperAxis_Z.emergencyStop();
						break;
					}
					case ID_AXIS_X:
					{
						stepperAxis_X.emergencyStop();
						break;
					}
					case ID_AXIS_3:
					{
						stepperAxis_M3.emergencyStop();
						break;
					}
					case ID_AXIS_4:
					{
						stepperAxis_M4.emergencyStop();
						break;
					}
					}

					if (configSetup.limit_StopSpindle)
					{
						// Stop the spindle
						stepperSpindle.overrideSpeed(0);
						MilliDelay(5);
						stepperSpindle.emergencyStop();

						if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
						{
							stepperAxis_M3.emergencyStop();
							SetEnable(ID_AXIS_3, false, true);
						}
						SetEnable(ID_SPINDLE, false, true);
						stepper_Spindle_Go = false;

						// 0 = Change to disabled, 1 = keep enabled
						if (configSetup.keepSteppersEnabled == 1)
						{
							SerialPrint("pageMain.bt10.val=1"); // 1 = enabled
							SerialPrint("\xFF\xFF\xFF");
						}
						else
						{
							SerialPrint("pageMain.bt10.val=0"); // 0 = disabled
							SerialPrint("\xFF\xFF\xFF");
							SerialPrint("pageMain.bt10.txt="); // 0 = disabled
							SerialPrint("\x22");
							SerialPrint("Disabled");
							SerialPrint("\x22\xFF\xFF\xFF");
						}
					}

					// 0 = Change to disabled, 1 = keep enabled
					if (configSetup.keepSteppersEnabled == 1)
					{
						SerialPrint("pageMain.bt11.val=1"); // 1 = enabled
						SerialPrint("\xFF\xFF\xFF");
					}
					else
					{
						SerialPrint("pageMain.bt11.val=0"); // 0 = disabled
						SerialPrint("\xFF\xFF\xFF");
						SerialPrint("pageMain.bt11.txt="); // 0 = disabled
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
		}

		MilliDelay(5);
	}

	MilliDelay(15);
#ifdef VERBOSE
	Serial.print(spindle_Char);
	Serial.print(axis_Char);
	Serial.println(stopped_Char);
#endif // VERBOSE
	stepper_Axis_Go = false;
	stepper_Spindle_Go = false;

	endPosition_Spindle = stepperSpindle.getPosition();
	endPosition_Z = stepperAxis_Z.getPosition();
	endPosition_X = stepperAxis_X.getPosition();
	endPosition_M3 = stepperAxis_M3.getPosition();
	endPosition_M4 = stepperAxis_M4.getPosition();


#ifdef DEBUG
	Serial.print("endPosition_M3: ");
	Serial.println(endPosition_M3);
#endif // DEBUG

	if (!keepAxisEnabledState)
	{
		switch (configMain.axisId)
		{
		case ID_AXIS_Z:
		{
			endPosition_Axis = stepperAxis_Z.getPosition();

			SetEnable(ID_AXIS_Z, false, true);
			break;
		}
		case ID_AXIS_X:
		{
			endPosition_Axis = stepperAxis_X.getPosition();

			SetEnable(ID_AXIS_X, false, true);
			break;
		}
		case ID_AXIS_3:
		{
			endPosition_Axis = stepperAxis_M3.getPosition();
#ifdef DEBUG
			Serial.print("2.endPosition-M3: ");
			Serial.println(endPosition_Axis);
#endif // DEBUG

			SetEnable(ID_AXIS_3, false, true);
			MilliDelay(15);
			break;
		}
		case ID_AXIS_4:
		{
			endPosition_Axis = stepperAxis_M4.getPosition();

			SetEnable(ID_AXIS_4, false, true);
			break;
		}
		}
	}

	spindle_Position = stepperSpindle.getPosition();// *(-1);
#ifdef DEBUG
	Serial.print("spindle_Position: ");
	Serial.println(spindle_Position);
#endif // DEBUG

	axis_Position = endPosition_Axis;// *(-1);
#ifdef DEBUG
	Serial.print("axis_Position: ");
	Serial.println(axis_Position);
#endif // DEBUG

	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		SetEnable(ID_AXIS_3, false, true);
		m3_Position = stepperAxis_M3.getPosition();

#ifdef DEBUG
		Serial.print("m3_Position: ");
		Serial.println(m3_Position);
#endif // DEBUG
	}
	MilliDelay(15);
#ifdef SHOW_POSITION 
	degrees_Spindle = StepsToDegrees(spindle_Position, ID_SPINDLE);
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

#ifdef DEBUG
	Serial.print("stepper_Spindle.getPosition() : ");
	Serial.println(stepperSpindle.getPosition());

	Serial.print("stepper_Z.getPosition() : ");
	Serial.println(stepperAxis_Z.getPosition());

	Serial.print("stepper_X.getPosition() : ");
	Serial.println(stepperAxis_X.getPosition());

	Serial.print("stepper_M3.getPosition() : ");
	Serial.println(stepperAxis_M3.getPosition());

	Serial.print("             stepper_M4.getPosition(): ");
	Serial.println(stepperAxis_M4.getPosition());
	Serial.println("  end           stepper_M4.getPosition(): ");
#endif // DEBUG
	MilliDelay(15);
	if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
	{
		digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
	}
	MilliDelay(100);
	Serial.println("Exit Main");
}




///<summary>
/// BeginMethod
/// </summary>
/// <comment>
/// Common begin method settings
/// </comment>
void BeginMethodSettings()
{
	endPosition_Spindle = 0;
	endPosition_Z = 0;
	endPosition_X = 0;
	endPosition_M3 = 0;
	endPosition_M4 = 0;

	stepperAxis_Z.setPosition(0);
	stepperAxis_X.setPosition(0);
	stepperAxis_M3.setPosition(0);
	stepperAxis_M4.setPosition(0);
	stepperSpindle.setPosition(0);

	stepPin = 0;
	dirPin = 0;
	startPositionAbs_Axis = 0;
	limitTriggered = false;
	eStopTriggered = false;

	if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
	{
		digitalWrite(configSetup.auxillaryPin, LOW);  // Enable
	}

	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);

	//if(pageCallerId != PAGE_PROGRAM ||)
	//switch (pageCallerId)
	//{
	//	case PAGE_MAIN:
	//	case PAGE_ONE:
	//	case PAGE_INDEX:
	//	case PAGE_MOVE:
	//	case PAGE_SYNC:
	//	case PAGE_ROSE:
	//	case PAGE_MULTI:
	//	{
			cumulative_Spindle = 0;
			cumulative_Z = 0;
			cumulative_X = 0;
			cumulative_M3 = 0;
			cumulative_M4 = 0;
	//		break;
	//	}
	//	case PAGE_PROGRAM:
	//	case PAGE_GRK:
	//	case PAGE_REC:
	//	{
	//		// Don't reset cumulative
	//		break;
	//	}

	//}
}

///<summary>
/// Multi_SimultaneousTargets
/// </summary>
/// <comment>
/// Run multiple steppers simultaneously
/// </comment>
void Multi_Sync()
{
#ifdef VERBOSE
	Serial.println("Enter Multi_SimultaneousTargets");
#endif // VERBOSE
	int32_t currentPosition = 0;
	int32_t targetSteps = 0;

	BeginMethodSettings();

	Stepper stepSpindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
	Stepper stepAxis_Z(PIN_AXIS_Z_STEP, PIN_AXIS_Z_DIR);
	Stepper stepAxis_X(PIN_AXIS_X_STEP, PIN_AXIS_X_DIR);
	Stepper stepAxis_M3(PIN_AXIS_3_STEP, PIN_AXIS_3_DIR);
	Stepper stepAxis_M4(PIN_AXIS_4_STEP, PIN_AXIS_4_DIR);

	stepAxis_Z.setAcceleration(0);
	stepAxis_X.setAcceleration(0);
	stepAxis_M3.setAcceleration(0);
	stepAxis_M4.setAcceleration(0);
	stepSpindle.setAcceleration(0);

	stepAxis_Z.setMaxSpeed(0);
	stepAxis_X.setMaxSpeed(0);
	stepAxis_M3.setMaxSpeed(0);
	stepAxis_M4.setMaxSpeed(0);
	stepSpindle.setMaxSpeed(0);

	stepAxis_Z.setPosition(0);
	stepAxis_X.setPosition(0);
	stepAxis_M3.setPosition(0);
	stepAxis_M4.setPosition(0);
	stepSpindle.setPosition(0);

	stepAxis_Z.setTargetAbs(0);
	stepAxis_X.setTargetAbs(0);
	stepAxis_M3.setTargetAbs(0);
	stepAxis_M4.setTargetAbs(0);
	stepSpindle.setTargetAbs(0);

	// Reset global variables
	stepperId_1 = 40;
	stepperId_2 = 40;
	stepperId_3 = 40;
	stepperId_4 = 40;
	stepperId_5 = 40;

	int limitPin_Max_1 = configSetup.limit_Max_Z;
	int limitPin_Min_1 = configSetup.limit_Min_Z;
	int limitPin_Max_2 = configSetup.limit_Max_Z;
	int limitPin_Min_2 = configSetup.limit_Min_Z;
	int limitPin_Max_3 = configSetup.limit_Max_Z;
	int limitPin_Min_3 = configSetup.limit_Min_Z;
	int limitPin_Max_4 = configSetup.limit_Max_Z;
	int limitPin_Min_4 = configSetup.limit_Min_Z;



	int stepsPerRevolution = 0;
	int direction = 0;
	double stepsToMove_1 = 0;
	double stepsToMove_2 = 0;
	double stepsToMove_3 = 0;
	double stepsToMove_4 = 0;
	double stepsToMove_5 = 0;

	float newSpeed_1 = 0;
	float newSpeed_2 = 0;
	float newSpeed_3 = 0;
	float newSpeed_4 = 0;
	float newSpeed_5 = 0;
	int direction_1 = 1;
	int direction_2 = 1;
	int direction_3 = 1;
	int direction_4 = 1;
	int direction_5 = 1;
	int accel_1 = 1;
	int accel_2 = 1;
	int accel_3 = 1;
	int accel_4 = 1;
	int accel_5 = 1;

	int multiAxisChecked = 0;

	checkedCount = 0;


	// Z Axis
	multiAxisChecked = 1 & configMulti.multiAxesIDs; // Z
#ifdef VERBOSE
	Serial.print("configMulti.checked_Z:  ");
	Serial.println(configMulti.checked_Z);
#endif // VERBOSE
	//if (multiAxisChecked == 1)
	if (configMulti.checked_Z == 1)
	{
		checkedCount += 1;
		Serial.print("Z checkedCount:  ");
		Serial.println(checkedCount);
		stepper1_step = PIN_AXIS_Z_STEP;
		stepper1_dir = PIN_AXIS_Z_DIR;
		stepperId_1 = ID_AXIS_Z;

		limitPin_Max_1 = configSetup.limit_Max_Z;
		limitPin_Min_1 = configSetup.limit_Min_Z;
		digitalWrite(limitPin_Max_1, HIGH);
		digitalWrite(limitPin_Min_1, HIGH);

		direction_1 = configMulti.direction_Z;

		stepsToMove_1 = (configMulti.targetDistance_Z / (configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z));
#ifdef VERBOSE		
		Serial.print("stepsToMove_1:  ");
		Serial.println(stepsToMove_1);
#endif // VERBOSE
		stepsToMove_1 = stepsToMove_1 * direction_1;

		// Set speed and acceleration
		newSpeed_1 = configMulti.speedPercent_Z * configMulti.maxSpd_Z * .01;
		accel_1 = configMulti.accel_Z;
#ifdef VERBOSE
		Serial.println("-++++++++++++++++++++++++++++++++++++++++++++");
		Serial.print("configMulti.accel_Axis_Z:  ");
		Serial.println(configMulti.accel_Z);
		Serial.print("configMulti.targetDistance_Z:  ");
		Serial.println(configMulti.targetDistance_Z);
		Serial.print("configSetup.distancePerRev_AxisZ:  ");
		Serial.println(configSetup.distancePerRev_AxisZ);
		Serial.print("configSetup.steps360_Axis_Z:  ");
		Serial.println(configSetup.steps360_Axis_Z);
		Serial.print("configSetup.microsteps_Axis_Z:  ");
		Serial.println(configSetup.microsteps_Axis_Z);
		Serial.print("stepsToMove_1:  ");
		Serial.println(stepsToMove_1);
		Serial.print("configMulti.speedPercent_Axis_Z:  ");
		Serial.println(configMulti.speedPercent_Z);
		Serial.print("configMulti.maxSpd_Axis_Z:  ");
		Serial.println(configMulti.maxSpd_Z);
		Serial.println("-++++++++++++++++++++++++++++++++++++++++++++");
#endif // VERBOSE
		stepAxis_Z
			.setMaxSpeed(newSpeed_1)
			.setAcceleration(accel_1)
			.setTargetAbs(stepsToMove_1);

		SetEnable(ID_AXIS_Z, true, true);
	}

	// X Axis
	multiAxisChecked = 2 & configMulti.multiAxesIDs; // X
#ifdef VERBOSE
	Serial.print("configMulti.checked X:  ");
	Serial.println(configMulti.checked_X);
#endif // VERBOSE
	//if (multiAxisChecked == 2)
	if (configMulti.checked_X == 1)
	{
		//checked_X = 1;
		checkedCount += 1;
		Serial.print("X checkedCount:  ");
		Serial.println(checkedCount);
		direction_1 = configMulti.direction_X;
		if (checkedCount == 1)
		{
			stepper1_step = PIN_AXIS_X_STEP;
			stepper1_dir = PIN_AXIS_X_DIR;
			stepperId_1 = ID_AXIS_X;

#ifdef DEBUG
			Serial.print("X direction: ");
			Serial.println(direction_1);
#endif // DEBUG

			stepsToMove_1 = (configMulti.targetDistance_X / (configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X)) * direction_1;

			// Set speed and acceleration
			newSpeed_1 = configMulti.speedPercent_X * configMulti.maxSpd_X * .01;
			accel_1 = configMulti.accel_X;

			limitPin_Max_1 = configSetup.limit_Max_X;
			limitPin_Min_1 = configSetup.limit_Min_X;
			digitalWrite(limitPin_Max_1, HIGH);
			digitalWrite(limitPin_Min_1, HIGH);

			stepAxis_X
				.setMaxSpeed(newSpeed_1)
				.setAcceleration(accel_1)
				.setTargetAbs(stepsToMove_1);
		}
		else // checkedCount = 2
		{
			stepper2_step = PIN_AXIS_X_STEP;
			stepper2_dir = PIN_AXIS_X_DIR;
			stepperId_2 = ID_AXIS_X;
			direction_2 = configMulti.direction_X;

			stepsToMove_2 = static_cast<int32_t>(configMulti.targetDistance_X / (configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X)) * direction_2;

			// Set speed and acceleration
			newSpeed_2 = configMulti.speedPercent_X * configMulti.maxSpd_X * .01;
			accel_2 = configMulti.accel_X;

			limitPin_Max_2 = configSetup.limit_Max_X;
			limitPin_Min_2 = configSetup.limit_Min_X;
			digitalWrite(limitPin_Max_2, HIGH);
			digitalWrite(limitPin_Min_2, HIGH);

			stepAxis_X
				.setMaxSpeed(newSpeed_2)
				.setAcceleration(accel_2)
				.setTargetAbs(stepsToMove_2);
		}

		SetEnable(ID_AXIS_X, true, true);
	}

	// M3 Axis
	multiAxisChecked = 4 & configMulti.multiAxesIDs; // M3
#ifdef VERBOSE
	Serial.print("multiAxisChecked M3:   ");
	Serial.println(multiAxisChecked);
#endif // VERBOSE
	//if (multiAxisChecked == 4)
	if (configMulti.checked_M3 == 1)
	{
		//checked_M3 = 1;
		checkedCount += 1;
		Serial.print("M3 checkedCount:  ");
		Serial.println(checkedCount);
		direction_1 = configMulti.direction_M3;

		if (checkedCount == 1)
		{
			stepper1_step = PIN_AXIS_3_STEP;
			stepper1_dir = PIN_AXIS_3_DIR;
			stepperId_1 = ID_AXIS_3;

#ifdef VERBOSE
			Serial.print("direction_1:   ");
			Serial.println(direction_1);
			Serial.print("mconfigMulti.direction_M3:   ");
			Serial.println(configMulti.direction_M3);
			Serial.print("direction:   ");
			Serial.println(direction);
#endif // VERBOSE
			//if (direction == 4)
			//{
			//	direction_1 = -1;
			//}
			//else
			//{
			//	direction_1 = 1;
			//}
#ifdef VERBOSE
			Serial.println("------------------------------------------------");
			Serial.print("direction_1:   ");
			Serial.println(direction_1);
#endif // VERBOSE
			if (configSetup.radialOrLinear_Axis_M3 == RADIAL_M3)
			{
				stepsPerRevolution = (int)round(configSetup.microsteps_Axis_M3 * configSetup.steps360_Axis_M3 * configSetup.gearRatio_AxisM3);
				stepsToMove_1 = stepsPerRevolution * (configMulti.targetDistance_M3 / 360) * direction_1;

				////Serial.print("configSetup.microsteps_Axis_M3:   ");
				////Serial.println(configSetup.microsteps_Axis_M3);
				////Serial.print("configSetup.steps360_Axis_M3:   ");
				////Serial.println(configSetup.steps360_Axis_M3);
				////Serial.print("configSetup.gearRatio_AxisM3:   ");
				////Serial.println(configSetup.gearRatio_AxisM3);
				////Serial.print("stepsPerRevolution:  ");
				////Serial.println(stepsPerRevolution);

				////Serial.print("direction_1:   ");
				////Serial.println(direction_1);



			}
			else
			{
				limitPin_Max_1 = configSetup.limit_Max_M3;
				limitPin_Min_1 = configSetup.limit_Min_M3;
				digitalWrite(limitPin_Max_1, HIGH);
				digitalWrite(limitPin_Min_1, HIGH);

				stepsToMove_1 = (configMulti.targetDistance_M3 / (configSetup.distancePerRev_AxisM3) * (configSetup.steps360_Axis_M3 * configSetup.microsteps_Axis_M3)) * direction_1;
				Serial.print("stepsToMove_1A:  ");
				Serial.println(stepsToMove_1);
			}

			newSpeed_1 = configMulti.speedPercent_M3 * configMulti.maxSpd_M3 * .01;
			accel_1 = configMulti.accel_M3;

			Serial.print("stepsToMove_1Aa:  ");
			Serial.println(stepsToMove_1);
			Serial.print("newSpeed_1:  ");
			Serial.println(newSpeed_1);
			Serial.print("accel_1:  ");
			Serial.println(accel_1);

			stepAxis_M3
				.setMaxSpeed(newSpeed_1)
				.setAcceleration(accel_1)
				.setTargetAbs(stepsToMove_1);

		}
		else if (checkedCount == 2)
		{
			stepper2_step = PIN_AXIS_3_STEP;
			stepper2_dir = PIN_AXIS_3_DIR;
			stepperId_2 = ID_AXIS_3;
			direction_2 = configMulti.direction_M3;

			if (configSetup.radialOrLinear_Axis_M3 == RADIAL_M3)
			{
				stepsPerRevolution = (int)round(configSetup.microsteps_Axis_M3 * configSetup.steps360_Axis_M3 * configSetup.gearRatio_AxisM3);
				stepsToMove_2 = static_cast<int32_t>(stepsPerRevolution * (configMulti.targetDistance_M3 / 360) * direction_2);
			}
			else
			{
				limitPin_Max_2 = configSetup.limit_Max_M3;
				limitPin_Min_2 = configSetup.limit_Min_M3;
				digitalWrite(limitPin_Max_2, HIGH);
				digitalWrite(limitPin_Min_2, HIGH);

				stepsToMove_2 = (configMulti.targetDistance_M3 / (configSetup.distancePerRev_AxisM3) * (configSetup.steps360_Axis_M3 * configSetup.microsteps_Axis_M3)) * direction_2;
			}

			newSpeed_2 = configMulti.speedPercent_M3 * configMulti.maxSpd_M3 * .01;
			accel_2 = configMulti.accel_M3;

			stepAxis_M3
				.setMaxSpeed(newSpeed_2)
				.setAcceleration(accel_2)
				.setTargetAbs(stepsToMove_2);
		}
		else // checkedCount = 3
		{
			stepper3_step = PIN_AXIS_3_STEP;
			stepper3_dir = PIN_AXIS_3_DIR;
			stepperId_3 = ID_AXIS_3;
			direction_3 = configMulti.direction_M3;

			if (configSetup.radialOrLinear_Axis_M3 == RADIAL_M3)
			{
				stepsPerRevolution = (int)round(configSetup.microsteps_Axis_M3 * configSetup.steps360_Axis_M3 * configSetup.gearRatio_AxisM3);
				stepsToMove_3 = stepsPerRevolution * (configMulti.targetDistance_M3 / 360) * direction_3;
			}
			else
			{
				limitPin_Max_3 = configSetup.limit_Max_M3;
				limitPin_Min_3 = configSetup.limit_Min_M3;
				digitalWrite(limitPin_Max_3, HIGH);
				digitalWrite(limitPin_Min_3, HIGH);

				stepsToMove_3 = (configMulti.targetDistance_M3 / (configSetup.distancePerRev_AxisM3) * (configSetup.steps360_Axis_M3 * configSetup.microsteps_Axis_M3)) * direction_3;
			}

			newSpeed_3 = configMulti.speedPercent_M3 * configMulti.maxSpd_M3 * .01;
			accel_3 = configMulti.accel_M3;

			stepAxis_M3
				.setMaxSpeed(newSpeed_3)
				.setAcceleration(accel_3)
				.setTargetAbs(stepsToMove_3);
		}

#ifdef VERBOSE
		Serial.println("------------------------------------------------");
		Serial.print("configMulti.accel_Axis_M3:  ");
		Serial.println(configMulti.accel_M3);
		Serial.print("configMulti.targetDistance_M3:  ");
		Serial.println(configMulti.targetDistance_M3);
		Serial.print("configSetup.distancePerRev_AxisM3:  ");
		Serial.println(configSetup.distancePerRev_AxisM3);
		Serial.print("configSetup.steps360_Axis_3:  ");
		Serial.println(configSetup.steps360_Axis_M3);
		Serial.print("configSetup.microsteps_Axis_M3:  ");
		Serial.println(configSetup.microsteps_Axis_M3);
		Serial.print("stepsToMove_2:  ");
		Serial.println(stepsToMove_2);
		Serial.print("configMulti.speedPercent_Axis_M3:  ");
		Serial.println(configMulti.speedPercent_M3);
		Serial.print("configMulti.maxSpd_Axis_M3:  ");
		Serial.println(configMulti.maxSpd_M3);
		Serial.println("------------------------------------------------");
#endif // VERBOSE

		SetEnable(ID_AXIS_3, true, true);
	}


	// M4 Axis
	multiAxisChecked = 8 & configMulti.multiAxesIDs; // M4
#ifdef VERBOSE
	Serial.print("multiAxisChecked M4:  ");
	Serial.println(multiAxisChecked);
#endif // VERBOSE
	//if (multiAxisChecked == 8)
	if (configMulti.checked_M4 == 1)
	{
		//checked_M4 = 1;
		checkedCount += 1;
		Serial.print("M4 checkedCount:  ");
		Serial.println(checkedCount);
		direction_1 = configMulti.direction_M4;
		if (checkedCount == 1)
		{
			stepper1_step = PIN_AXIS_4_STEP;
			stepper1_dir = PIN_AXIS_4_DIR;
			stepperId_1 = ID_AXIS_4;

			if (configSetup.radialOrLinear_Axis_M4 == RADIAL_M4)
			{
				stepsPerRevolution = (int)round(configSetup.microsteps_Axis_M4 * configSetup.steps360_Axis_M4 * configSetup.gearRatio_AxisM4);
				stepsToMove_1 = stepsPerRevolution * (configMulti.targetDistance_M4 / 360) * direction_1;
			}
			else
			{
				limitPin_Max_1 = configSetup.limit_Max_M4;
				limitPin_Min_1 = configSetup.limit_Min_M4;
				digitalWrite(limitPin_Max_1, HIGH);
				digitalWrite(limitPin_Min_1, HIGH);
				stepsToMove_1 = (configMulti.targetDistance_M4 / (configSetup.distancePerRev_AxisM4) * (configSetup.steps360_Axis_M4 * configSetup.microsteps_Axis_M4)) * direction_1;
			}
			newSpeed_1 = configMulti.speedPercent_M4 * configMulti.maxSpd_M4 * .01;
			accel_1 = configMulti.accel_M4;

			stepAxis_M4
				.setMaxSpeed(newSpeed_1)
				.setAcceleration(accel_1)
				.setTargetAbs(stepsToMove_1);
		}
		else if (checkedCount == 2)
		{
			stepper2_step = PIN_AXIS_4_STEP;
			stepper2_dir = PIN_AXIS_4_DIR;
			stepperId_2 = ID_AXIS_4;
			direction_2 = configMulti.direction_M4;

			if (configSetup.radialOrLinear_Axis_M4 == RADIAL_M4)
			{
				stepsPerRevolution = (int)round(configSetup.microsteps_Axis_M4 * configSetup.steps360_Axis_M4 * configSetup.gearRatio_AxisM4);
				stepsToMove_2 = static_cast<int32_t>(stepsPerRevolution * (configMulti.targetDistance_M4 / 360) * direction_2);
			}
			else
			{
				limitPin_Max_2 = configSetup.limit_Max_M4;
				limitPin_Min_2 = configSetup.limit_Min_M4;
				digitalWrite(limitPin_Max_2, HIGH);
				digitalWrite(limitPin_Min_2, HIGH);

				stepsToMove_2 = static_cast<int32_t>(configMulti.targetDistance_M4 / (configSetup.distancePerRev_AxisM4) * (configSetup.steps360_Axis_M4 * configSetup.microsteps_Axis_M4)) * direction_2;
			}
			newSpeed_2 = configMulti.speedPercent_M4 * configMulti.maxSpd_M4 * .01;
			accel_2 = configMulti.accel_M4;

			Serial.print("M4 target: ");
			Serial.print(stepsToMove_2);
			Serial.print(" speed: ");
			Serial.println(newSpeed_2);

			stepAxis_M4
				.setMaxSpeed(newSpeed_2)
				.setAcceleration(accel_2)
				.setTargetAbs(stepsToMove_2);


		}
		else if (checkedCount == 3)
		{
			stepper3_step = PIN_AXIS_4_STEP;
			stepper3_dir = PIN_AXIS_4_DIR;
			stepperId_3 = ID_AXIS_4;
			direction_3 = configMulti.direction_M4;

			if (configSetup.radialOrLinear_Axis_M4 == RADIAL_M4)
			{
				stepsPerRevolution = (int)round(configSetup.microsteps_Axis_M4 * configSetup.steps360_Axis_M4 * configSetup.gearRatio_AxisM4);
				stepsToMove_3 = stepsPerRevolution * (configMulti.targetDistance_M4 / 360) * direction_3;
			}
			else
			{
				limitPin_Max_3 = configSetup.limit_Max_M4;
				limitPin_Min_3 = configSetup.limit_Min_M4;
				digitalWrite(limitPin_Max_3, HIGH);
				digitalWrite(limitPin_Min_3, HIGH);

				stepsToMove_3 = (configMulti.targetDistance_M4 / (configSetup.distancePerRev_AxisM4) * (configSetup.steps360_Axis_M4 * configSetup.microsteps_Axis_M4)) * direction_3;
			}

			newSpeed_3 = configMulti.speedPercent_M4 * configMulti.maxSpd_M4 * .01;
			accel_3 = configMulti.accel_M4;

			stepAxis_M4
				.setMaxSpeed(newSpeed_3)
				.setAcceleration(accel_3)
				.setTargetAbs(stepsToMove_3);

		}
		else //(checkedCount == 4)
		{
			stepper4_step = PIN_AXIS_4_STEP;
			stepper4_dir = PIN_AXIS_4_DIR;
			stepperId_4 = ID_AXIS_4;
			direction_4 = configMulti.direction_M4;

			if (configSetup.radialOrLinear_Axis_M4 == RADIAL_M4)
			{
				stepsPerRevolution = (int)round(configSetup.microsteps_Axis_M4 * configSetup.steps360_Axis_M4 * configSetup.gearRatio_AxisM4);
				stepsToMove_4 = stepsPerRevolution * (configMulti.targetDistance_M4 / 360) * direction_4;
			}
			else
			{
				limitPin_Max_4 = configSetup.limit_Max_M4;
				limitPin_Min_4 = configSetup.limit_Min_M4;
				digitalWrite(limitPin_Max_4, HIGH);
				digitalWrite(limitPin_Min_4, HIGH);

				stepsToMove_4 = (configMulti.targetDistance_M4 / (configSetup.distancePerRev_AxisM4) * (configSetup.steps360_Axis_M4 * configSetup.microsteps_Axis_M4)) * direction_4;
			}

			newSpeed_4 = configMulti.speedPercent_M4 * configMulti.maxSpd_M4 * .01;
			accel_4 = configMulti.accel_M4;

			stepAxis_M4
				.setMaxSpeed(newSpeed_4)
				.setAcceleration(accel_4)
				.setTargetAbs(stepsToMove_4);
		}

#ifdef VERBOSE
		Serial.println("-+++4444444++++++++++++++++++++++++++++++");
		Serial.print("configMulti.accel_Axis_M4:  ");
		Serial.println(configMulti.accel_M4);
		Serial.print("configMulti.targetDistance_M4:  ");
		Serial.println(configMulti.targetDistance_M4);
		Serial.print("configSetup.distancePerRev_AxisM4:  ");
		Serial.println(configSetup.distancePerRev_AxisM4);
		Serial.print("configSetup.steps360_Axis_M4:  ");
		Serial.println(configSetup.steps360_Axis_M4);
		Serial.print("configSetup.microsteps_Axis_M4:  ");
		Serial.println(configSetup.microsteps_Axis_M4);
		Serial.print("stepsToMove_1:  ");
		Serial.println(stepsToMove_1);
		Serial.print("stepsToMove_2:  ");
		Serial.println(stepsToMove_2);
		Serial.print("stepsToMove_3:  ");
		Serial.println(stepsToMove_3);
		Serial.print("stepsToMove_4:  ");
		Serial.println(stepsToMove_4);
		Serial.print("configMulti.speedPercent_Axis_M4:  ");
		Serial.println(configMulti.speedPercent_M4);
		Serial.print("configMulti.maxSpd_Axis_M4:  ");
		Serial.println(configMulti.maxSpd_M4);
		Serial.println("-+++++++44444444444+++++++++++++++++++++++");
#endif // VERBOSE


		SetEnable(ID_AXIS_4, true, true);
	}


	// Spindle
	multiAxisChecked = 16 & configMulti.multiAxesIDs; // Spindle
#ifdef VERBOSE
	Serial.print("multiAxisChecked Spindle:  ");
	Serial.println(multiAxisChecked);
	Serial.print("6666checkedCount: ");
	Serial.println(checkedCount);
#endif // VERBOSE
	//if (multiAxisChecked == 16)
	if (configMulti.checked_Spindle == 1)
	{
		checkedCount += 1;
		Serial.print("Spindle checkedCount:  ");
		Serial.println(checkedCount);
		direction_1 = configMulti.direction_Spindle;

		if (checkedCount == 1)
		{
			stepper1_step = PIN_SPINDLE_STEP;
			stepper1_dir = PIN_SPINDLE_DIR;
			stepperId_1 = ID_SPINDLE;

			stepsPerRevolution = (int)round(configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle);
#ifdef VERBOSE
			Serial.print("stepsPerRevolution:  ");
			Serial.println(stepsPerRevolution);
#endif // VERBOSE
			stepsToMove_1 = stepsPerRevolution * (configMulti.targetDistance_Spindle / 360) * direction_1;

			// Set speed and acceleration
			newSpeed_1 = configMulti.speedPercent_Spindle * configMulti.maxSpd_Spindle * .01;
			accel_1 = configMulti.accel_Spindle;
#ifdef VERBOSE

			Serial.print("configMulti.accel_Spindle:  ");
			Serial.println(configMulti.accel_Spindle);
			Serial.print("configMulti.targetDistance_Spindle:  ");
			Serial.println(configMulti.targetDistance_Spindle);
			Serial.print("configSetup.gearRatio_Spindle:  ");
			Serial.println(configSetup.gearRatio_Spindle);
			Serial.print("configSetup.steps360_Spindle:  ");
			Serial.println(configSetup.steps360_Spindle);
			Serial.print("configSetup.microsteps_Spindle:  ");
			Serial.println(configSetup.microsteps_Spindle);
			Serial.print("stepsToMove_1:  ");
			Serial.println(stepsToMove_1);
			Serial.print("configMulti.speedPercent_Spindle:  ");
			Serial.println(configMulti.speedPercent_Spindle);
			Serial.print("configMulti.maxSpd_Spindle:  ");
			Serial.println(configMulti.maxSpd_Spindle);

			stepSpindle
				.setMaxSpeed(newSpeed_1)
				.setAcceleration(accel_1)
				.setTargetAbs(stepsToMove_1);
#endif // VERBOSE
		}
		else if (checkedCount == 2)
		{
			stepper2_step = PIN_SPINDLE_STEP;
			stepper2_dir = PIN_SPINDLE_DIR;
			stepperId_2 = ID_SPINDLE;
			direction_2 = configMulti.direction_Spindle;

			stepsPerRevolution = (int)round(configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle);
			stepsToMove_2 = static_cast<int32_t>(stepsPerRevolution * (configMulti.targetDistance_Spindle / 360) * direction_2);

			// Set speed and acceleration
			newSpeed_2 = configMulti.speedPercent_Spindle * configMulti.maxSpd_Spindle * .01;
			accel_2 = configMulti.accel_Spindle;

			stepSpindle
				.setMaxSpeed(newSpeed_2)
				.setAcceleration(accel_2)
				.setTargetAbs(stepsToMove_2);
		}
		else if (checkedCount == 3)
		{
			stepper3_step = PIN_SPINDLE_STEP;
			stepper3_dir = PIN_SPINDLE_DIR;
			stepperId_3 = ID_SPINDLE;
			direction_3 = configMulti.direction_Spindle;

			stepsPerRevolution = (int)round(configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle);
			stepsToMove_3 = stepsPerRevolution * (configMulti.targetDistance_Spindle / 360) * direction_3;

			// Set speed and acceleration
			newSpeed_3 = configMulti.speedPercent_Spindle * configMulti.maxSpd_Spindle * .01;
			accel_3 = configMulti.accel_Spindle;

			stepSpindle
				.setMaxSpeed(newSpeed_3)
				.setAcceleration(accel_3)
				.setTargetAbs(stepsToMove_3);
		}
		else if (checkedCount == 4)
		{
			stepper4_step = PIN_SPINDLE_STEP;
			stepper4_dir = PIN_SPINDLE_DIR;
			stepperId_4 = ID_SPINDLE;
			direction_4 = configMulti.direction_Spindle;

			stepsPerRevolution = (int)round(configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle);
			stepsToMove_4 = stepsPerRevolution * (configMulti.targetDistance_Spindle / 360) * direction_4;

			// Set speed and acceleration
			newSpeed_4 = configMulti.speedPercent_Spindle * configMulti.maxSpd_Spindle * .01;
			accel_4 = configMulti.accel_Spindle;

			stepSpindle
				.setMaxSpeed(newSpeed_4)
				.setAcceleration(accel_4)
				.setTargetAbs(stepsToMove_4);
		}
		else //(checkedCount == 5)
		{
			stepper5_step = PIN_SPINDLE_STEP;
			stepper5_dir = PIN_SPINDLE_DIR;
			stepperId_5 = ID_SPINDLE;
			direction_5 = configMulti.direction_Spindle;

			stepsPerRevolution = (int)round(configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle);
			stepsToMove_5 = stepsPerRevolution * (configMulti.targetDistance_Spindle / 360) * direction_5;

			// Set speed and acceleration
			newSpeed_5 = configMulti.speedPercent_Spindle * configMulti.maxSpd_Spindle * .01;
			accel_5 = configMulti.accel_Spindle;
			stepSpindle
				.setMaxSpeed(newSpeed_5)
				.setAcceleration(accel_5)
				.setTargetAbs(stepsToMove_5);
		}

		SetEnable(ID_SPINDLE, true, true);
	}

#ifdef VERBOSE
	Serial.print("2:checkedCount: ");
	Serial.println(checkedCount);
	Serial.print("1: ");
	Serial.println(stepperId_1);
	Serial.print("2: ");
	Serial.println(stepperId_2);
	Serial.print("3: ");
	Serial.println(stepperId_3);
	Serial.print("4: ");
	Serial.println(stepperId_4);
	Serial.print("5: ");
	Serial.println(stepperId_5);
#endif // VERBOSE

	StepperGroup stepGroup{ stepAxis_Z,stepAxis_X,stepAxis_M3,stepAxis_M4,stepSpindle };

#ifdef DEBUG
	Serial.println("CheckedCount: ");
	Serial.println(checkedCount);

	Serial.println("Pins: ");
	Serial.println(stepper1_step);
	Serial.println(stepper1_dir);
	Serial.println(stepper2_step);
	Serial.println(stepper2_dir);
	Serial.println(stepper3_step);
	Serial.println(stepper3_dir);
	Serial.println(stepper4_step);
	Serial.println(stepper4_dir);
	Serial.println(stepper5_step);
	Serial.println(stepper5_dir);
	Serial.println("End pins");

	Serial.println("MaxSpd: ");
	Serial.println(newSpeed_1);
	Serial.println(newSpeed_2);
	Serial.println(newSpeed_3);
	Serial.println(newSpeed_4);
	Serial.println(newSpeed_5);
	Serial.println("End MaxSpd");
	Serial.println("accel: ");
	Serial.println(accel_1);
	Serial.println(accel_2);
	Serial.println(accel_3);
	Serial.println(accel_4);
	Serial.println(accel_5);
	Serial.println("End accel");
	Serial.println("steps to move: ");
	Serial.println(stepsToMove_1);
	Serial.println(stepsToMove_2);
	Serial.println(stepsToMove_3);
	Serial.println(stepsToMove_4);
	Serial.println(stepsToMove_5);
	Serial.println("End steps to move");
#endif // DEBUG

#ifdef DEBUG
	Serial.print(" xxxxxxxxxx                         limitPin_Min_1:  ");
	Serial.println(digitalRead(limitPin_Min_1));
	Serial.print("configSetup.limit_NCorNO:");
	Serial.println(configSetup.limit_NCorNO);

#endif // DEBUG

	ProgressReport(stepsToMove_1, 0);
	//SetEnable(ID_AXIS_4, true, true);
	Serial.print("2 - Target: ");
	Serial.print(stepsToMove_2);
	Serial.print("  2 - Speed: ");
	Serial.println(newSpeed_2);

	stepGroup.startMove();
	//stepper_1.moveAsync();
	Serial.println("====================Moving");


#ifdef DEBUG
	////Serial.print(" zzzzzzzzzzzz                         limitPin_Min_1:  ");
	////Serial.println(digitalRead(limitPin_Min_1));
	////Serial.print("configSetup.limit_NCorNO:");
	////Serial.println(configSetup.limit_NCorNO);

#endif // DEBUG


	int eStop = 0;

	while (stepAxis_Z.isMoving || stepAxis_X.isMoving || stepAxis_M3.isMoving || stepAxis_M4.isMoving || stepSpindle.isMoving)
	{
		if (limitPin_Max_1 > 9 && limitPin_Max_1 < 40)
		{
			if ((digitalRead(limitPin_Max_1) != configSetup.limit_NCorNO) && direction_1 > 0)
			{
#ifdef DEBUG
				Serial.println("                          limitPin_Max_1");
#endif // DEBUG

				stepAxis_Z.emergencyStop();
				stepAxis_X.emergencyStop();
				stepAxis_M3.emergencyStop();
				stepAxis_M4.emergencyStop();
				stepSpindle.emergencyStop();

				stopSteppers = true;

				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEnable(ID_AXIS_4, false, false);
				SetEStopColors(PAGE_MULTI);
				goto endLoop;
			}
		}

		if (limitPin_Min_1 > 9 && limitPin_Min_1 < 40)
		{
			if ((digitalRead(limitPin_Min_1) != configSetup.limit_NCorNO) && direction_1 < 0)
			{
#ifdef DEBUG
				Serial.print("                          limitPin_Min_1:  ");
				Serial.println(digitalRead(limitPin_Min_1));
				Serial.print("configSetup.limit_NCorNO:");
				Serial.println(configSetup.limit_NCorNO);

#endif // DEBUG
				stepAxis_Z.emergencyStop();
				stepAxis_X.emergencyStop();
				stepAxis_M3.emergencyStop();
				stepAxis_M4.emergencyStop();
				stepSpindle.emergencyStop();

				stopSteppers = true;

				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEStopColors(PAGE_MULTI);
				goto endLoop;
			}
		}

		if (limitPin_Max_2 > 9 && limitPin_Max_2 < 40)
		{
			if ((digitalRead(limitPin_Max_2) != configSetup.limit_NCorNO) && direction_2 > 0)
			{
#ifdef DEBUG
				Serial.println("                          limitPin_Max_2");
#endif // DEBUG
				stepAxis_Z.emergencyStop();
				stepAxis_X.emergencyStop();
				stepAxis_M3.emergencyStop();
				stepAxis_M4.emergencyStop();
				stepSpindle.emergencyStop();

				stopSteppers = true;

				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEStopColors(PAGE_MULTI);
				goto endLoop;
			}
		}
		if (limitPin_Min_2 > 9 && limitPin_Min_2 < 40)
		{
			if ((digitalRead(limitPin_Min_2) != configSetup.limit_NCorNO) && direction_2 < 0)
			{
#ifdef DEBUG
				Serial.println("                          limitPin_Min_2");
#endif // DEBUG
				stepAxis_Z.emergencyStop();
				stepAxis_X.emergencyStop();
				stepAxis_M3.emergencyStop();
				stepAxis_M4.emergencyStop();
				stepSpindle.emergencyStop();

				stopSteppers = true;

				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEStopColors(PAGE_MULTI);
				goto endLoop;
			}
		}

		if (limitPin_Max_3 > 9 && limitPin_Max_3 < 40)
		{
			if ((digitalRead(limitPin_Max_3) != configSetup.limit_NCorNO) && direction_3 > 0)
			{
#ifdef DEBUG
				Serial.println("                          limitPin_Max_3");
#endif // DEBUG
				stepAxis_Z.emergencyStop();
				stepAxis_X.emergencyStop();
				stepAxis_M3.emergencyStop();
				stepAxis_M4.emergencyStop();
				stepSpindle.emergencyStop();

				stopSteppers = true;

				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEStopColors(PAGE_MULTI);
				goto endLoop;
			}
		}
		if (limitPin_Min_3 > 9 && limitPin_Min_3 < 40)
		{
			if ((digitalRead(limitPin_Min_3) != configSetup.limit_NCorNO) && direction_3 < 0)
			{
#ifdef DEBUG
				Serial.println("                          limitPin_Min_3");
#endif // DEBUG
				stepAxis_Z.emergencyStop();
				stepAxis_X.emergencyStop();
				stepAxis_M3.emergencyStop();
				stepAxis_M4.emergencyStop();
				stepSpindle.emergencyStop();

				stopSteppers = true;

				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEStopColors(PAGE_MULTI);
				goto endLoop;
			}
		}

		if (limitPin_Max_4 > 9 && limitPin_Max_4 < 40)
		{
			if ((digitalRead(limitPin_Max_4) != configSetup.limit_NCorNO) && direction_4 > 0)
			{
#ifdef DEBUG
				Serial.println("                          limitPin_Max_4");
#endif // DEBUG
				stepAxis_Z.emergencyStop();
				stepAxis_X.emergencyStop();
				stepAxis_M3.emergencyStop();
				stepAxis_M4.emergencyStop();
				stepSpindle.emergencyStop();

				stopSteppers = true;

				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEnable(ID_AXIS_4, false, false);
				SetEStopColors(PAGE_MULTI);
				goto endLoop;
			}
		}
		if (limitPin_Min_4 > 9 && limitPin_Min_4 < 40)
		{
			if ((digitalRead(limitPin_Min_4) != configSetup.limit_NCorNO) && direction_4 < 0)
			{
#ifdef DEBUG
				Serial.println("                          limitPin_Min_4");
#endif // DEBUG
				stepAxis_Z.emergencyStop();
				stepAxis_X.emergencyStop();
				stepAxis_M3.emergencyStop();
				stepAxis_M4.emergencyStop();
				stepSpindle.emergencyStop();

				stopSteppers = true;

				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEnable(ID_AXIS_4, false, false);
				SetEStopColors(PAGE_MULTI);
				goto endLoop;
			}
		}

		if (configSetup.eStop > 9 && configSetup.eStop < 40)
		{
			eStop = digitalRead(configSetup.eStop);
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
#ifdef DEBUG
				Serial.print("EStop:  ");
				Serial.println(eStop);
#endif // DEBUG
				stepAxis_Z.emergencyStop();
				stepAxis_X.emergencyStop();
				stepAxis_M3.emergencyStop();
				stepAxis_M4.emergencyStop();
				stepSpindle.emergencyStop();

				stopSteppers = true;

				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEnable(ID_AXIS_4, false, false);
				SetEStopColors(PAGE_MULTI);
				if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
				{
					digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
				}
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
				stepGroup.stopAsync();
				stopSteppers = true;
				MilliDelay(200);
				break;
			}
			}
		}

		// Get progress
		switch (stepperId_1)
		{
			case ID_AXIS_Z:
			{
				currentPosition = stepAxis_Z.getPosition();
				break;
			}
			case ID_AXIS_X:
			{
				currentPosition = stepAxis_X.getPosition();
				break;
			}
			case ID_AXIS_3:
			{
				currentPosition = stepAxis_M3.getPosition();
				break;
			}
			case ID_AXIS_4:
			{
				currentPosition = stepAxis_M4.getPosition();
				break;
			}
			case ID_SPINDLE:
			{
				currentPosition = stepSpindle.getPosition();
				break;
			}
		}

		targetSteps = int32_t(stepsToMove_1);
		MilliDelay(10);
		ProgressReport(targetSteps, currentPosition);
	}

endLoop:
#ifdef VERBOSE
	Serial.print("keepSteppersEnabled:  ");
	Serial.println(configSetup.keepSteppersEnabled);
#endif // VERBOSE
	if (configSetup.keepSteppersEnabled != 1)
	{
#ifdef VERBOSE
		Serial.print("stepper1 axisId:  ");
		Serial.println(stepperId_1);
#endif // VERBOSE
		switch (stepperId_1)
		{
		case ID_AXIS_Z:
		{
			SerialPrint("pageMulti.bt1301.val=0"); // 0 = disabled
			SerialPrint("\xFF\xFF\xFF");
			SerialPrint("pageMulti.bt1301.txt="); // 0 = disabled
			SerialPrint("\x22");
			SerialPrint("Disabled");
			SerialPrint("\x22\xFF\xFF\xFF");
			//Serial.print("Disable Z:  ");
			break;
		}
		case ID_AXIS_X:
		{
			SerialPrint("pageMulti.bt1302.val=0"); // 0 = disabled
			SerialPrint("\xFF\xFF\xFF");
			SerialPrint("pageMulti.bt1302.txt="); // 0 = disabled
			SerialPrint("\x22");
			SerialPrint("Disabled");
			SerialPrint("\x22\xFF\xFF\xFF");
			break;
		}
		case ID_AXIS_3:
		{
			SerialPrint("pageMulti.bt1303.val=0"); // 0 = disabled
			SerialPrint("\xFF\xFF\xFF");
			SerialPrint("pageMulti.bt1303.txt="); // 0 = disabled
			SerialPrint("\x22");
			SerialPrint("Disabled");
			SerialPrint("\x22\xFF\xFF\xFF");
			break;
		}
		case ID_AXIS_4:
		{
			SerialPrint("pageMulti.bt1404.val=0"); // 0 = disabled
			SerialPrint("\xFF\xFF\xFF");
			SerialPrint("pageMulti.bt1404.txt="); // 0 = disabled
			SerialPrint("\x22");
			SerialPrint("Disabled");
			SerialPrint("\x22\xFF\xFF\xFF");
			break;
		}
		case ID_SPINDLE:
		{
			SerialPrint("pageMulti.bt1300.val=0"); // 0 = disabled
			SerialPrint("\xFF\xFF\xFF");
			SerialPrint("pageMulti.bt1300.txt="); // 0 = disabled
			SerialPrint("\x22");
			SerialPrint("Disabled");
			SerialPrint("\x22\xFF\xFF\xFF");

			break;
		}
		}
		switch (stepperId_2)
		{
		case ID_AXIS_X:
		{
			SerialPrint("pageMulti.bt1302.val=0"); // 0 = disabled
			SerialPrint("\xFF\xFF\xFF");
			SerialPrint("pageMulti.bt1302.txt="); // 0 = disabled
			SerialPrint("\x22");
			SerialPrint("Disabled");
			SerialPrint("\x22\xFF\xFF\xFF");
			break;
		}
		case ID_AXIS_3:
		{
			SerialPrint("pageMulti.bt1303.val=0"); // 0 = disabled
			SerialPrint("\xFF\xFF\xFF");
			SerialPrint("pageMulti.bt1303.txt="); // 0 = disabled
			SerialPrint("\x22");
			SerialPrint("Disabled");
			SerialPrint("\x22\xFF\xFF\xFF");
			break;
		}
		case ID_AXIS_4:
		{
			SerialPrint("pageMulti.bt1304.val=0"); // 0 = disabled
			SerialPrint("\xFF\xFF\xFF");
			SerialPrint("pageMulti.bt1304.txt="); // 0 = disabled
			SerialPrint("\x22");
			SerialPrint("Disabled");
			SerialPrint("\x22\xFF\xFF\xFF");
			break;
		}
		case ID_SPINDLE:
		{
			SerialPrint("pageMulti.bt1300.val=0"); // 0 = disabled
			SerialPrint("\xFF\xFF\xFF");
			SerialPrint("pageMulti.bt1300.txt="); // 0 = disabled
			SerialPrint("\x22");
			SerialPrint("Disabled");
			SerialPrint("\x22\xFF\xFF\xFF");
			break;
		}
		}
		switch (stepperId_3)
		{
		case ID_AXIS_3:
		{
			SerialPrint("pageMulti.bt1303.val=0"); // 0 = disabled
			SerialPrint("\xFF\xFF\xFF");
			SerialPrint("pageMulti.bt1303.txt="); // 0 = disabled
			SerialPrint("\x22");
			SerialPrint("Disabled");
			SerialPrint("\x22\xFF\xFF\xFF");
			break;
		}
		case ID_AXIS_4:
		{
			SerialPrint("pageMulti.bt1304.val=0"); // 0 = disabled
			SerialPrint("\xFF\xFF\xFF");
			SerialPrint("pageMulti.bt1304.txt="); // 0 = disabled
			SerialPrint("\x22");
			SerialPrint("Disabled");
			SerialPrint("\x22\xFF\xFF\xFF");
			break;
		}
		case ID_SPINDLE:
		{
			SerialPrint("pageMulti.bt1300.val=0"); // 0 = disabled
			SerialPrint("\xFF\xFF\xFF");
			SerialPrint("pageMulti.bt1300.txt="); // 0 = disabled
			SerialPrint("\x22");
			SerialPrint("Disabled");
			SerialPrint("\x22\xFF\xFF\xFF");
			break;
		}
		}
		switch (stepperId_4)
		{
		case ID_AXIS_4:
		{
			SerialPrint("pageMulti.bt1304.val=0"); // 0 = disabled
			SerialPrint("\xFF\xFF\xFF");
			SerialPrint("pageMulti.bt1304.txt="); // 0 = disabled
			SerialPrint("\x22");
			SerialPrint("Disabled");
			SerialPrint("\x22\xFF\xFF\xFF");
			break;
		}
		case ID_SPINDLE:
		{
			SerialPrint("pageMulti.bt1300.val=0"); // 0 = disabled
			SerialPrint("\xFF\xFF\xFF");
			SerialPrint("pageMulti.bt1300.txt="); // 0 = disabled
			SerialPrint("\x22");
			SerialPrint("Disabled");
			SerialPrint("\x22\xFF\xFF\xFF");
			break;
		}
		}
		switch (stepperId_5)
		{
		case ID_SPINDLE:
		{
			SerialPrint("pageMulti.bt1300.val=0"); // 0 = disabled
			SerialPrint("\xFF\xFF\xFF");
			SerialPrint("pageMulti.bt1300.txt="); // 0 = disabled
			SerialPrint("\x22");
			SerialPrint("Disabled");
			SerialPrint("\x22\xFF\xFF\xFF");
			break;
		}
		}
	}
	else
	{
#ifdef VERBOSE
		Serial.print("keepSteppersEnabled:  ");
		Serial.println(configSetup.keepSteppersEnabled);
#endif // VERBOSE
	}

	/*switch (stepperId_1)
	{
		case ID_AXIS_Z:
		{
			stepperPosition_1 = stepAxis_Z.getPosition() * (-1);
			break;
		}
		case ID_AXIS_X:
		{
			stepperPosition_1 = stepAxis_X.getPosition() * (-1);
			break;
		}
		case ID_AXIS_3:
		{
			stepperPosition_1 = stepAxis_M3.getPosition() * (-1);
			break;
		}
		case ID_AXIS_4:
		{
			stepperPosition_1 = stepAxis_M4.getPosition() * (-1);
			break;
		}
		case ID_SPINDLE:
		{
			stepperPosition_1 = stepSpindle.getPosition() * (-1);
			break;
		}
	}

	Serial.print("stepperPosition_1:  ");
	Serial.println(stepperPosition_1);

	switch (stepperId_2)
	{
		case ID_AXIS_X:
		{
			stepperPosition_2 = stepAxis_X.getPosition() * (-1);
			break;
		}
		case ID_AXIS_3:
		{
			stepperPosition_2 = stepAxis_M3.getPosition() * (-1);
			break;
		}
		case ID_AXIS_4:
		{
			stepperPosition_2 = stepAxis_M4.getPosition() * (-1);
			break;
		}
		case ID_SPINDLE:
		{
			stepperPosition_2 = stepSpindle.getPosition() * (-1);
			break;
		}
	}

	Serial.print("stepperPosition_2:  ");
	Serial.println(stepperPosition_2);

	switch (stepperId_3)
	{
		case ID_AXIS_3:
		{
			stepperPosition_3 = stepAxis_M3.getPosition() * (-1);
			break;
		}
		case ID_AXIS_4:
		{
			stepperPosition_3 = stepAxis_M4.getPosition() * (-1);
			break;
		}
		case ID_SPINDLE:
		{
			stepperPosition_3 = stepSpindle.getPosition() * (-1);
			break;
		}
	}

	Serial.print("stepperPosition_3:  ");
	Serial.println(stepperPosition_3);

	switch (stepperId_4)
	{
		case ID_AXIS_4:
		{
			stepperPosition_4 = stepAxis_M4.getPosition() * (-1);
			break;
		}
		case ID_SPINDLE:
		{
			stepperPosition_4 = stepSpindle.getPosition() * (-1);
			break;
		}
	}

	Serial.print("stepperPosition_4:  ");
	Serial.println(stepperPosition_4);

	switch (stepperId_5)
	{
		case ID_SPINDLE:
		{
			stepperPosition_5 = stepSpindle.getPosition() * (-1);
			break;
		}
	}

	Serial.print("stepperPosition_5:  ");
	Serial.println(stepperPosition_5);*/


	// Set Return page values
	endPosition_Z = stepAxis_Z.getPosition();
	endPosition_X = stepAxis_X.getPosition();
	endPosition_M3 = stepAxis_M3.getPosition();
	endPosition_M4 = stepAxis_M4.getPosition();
	endPosition_Spindle = stepSpindle.getPosition();

	SetEnable(stepperId_1, false, true);
	SetEnable(stepperId_2, false, true);
	SetEnable(stepperId_3, false, true);
	SetEnable(stepperId_4, false, true);
	SetEnable(stepperId_5, false, true);

	SerialPrint("pageMulti.bt1.val=0");
	SerialPrint(nextionEnd);

	SerialPrint("pageMulti.bt2.val=0");
	SerialPrint(nextionEnd);

	SerialPrint("pageMulti.va0.val=0");
	SerialPrint(nextionEnd);

	// Reset global variables
	////stepperId_1 = 40;
	////stepperId_2 = 40;
	////stepperId_3 = 40;
	////stepperId_4 = 40;
	////stepperId_5 = 40;

	Serial.println("Pins: ");
	Serial.println(stepper1_step);
	Serial.println(stepper1_dir);
	Serial.println(stepper2_step);
	Serial.println(stepper2_dir);
	Serial.println(stepper3_step);
	Serial.println(stepper3_dir);
	Serial.println(stepper4_step);
	Serial.println(stepper4_dir);
	Serial.println(stepper5_step);
	Serial.println(stepper5_dir);
	Serial.println("End pins");

	////stepAxis_Z.setPosition(0);
	////stepAxis_X.setPosition(0);
	////stepAxis_M3.setPosition(0);
	////stepAxis_M4.setPosition(0);
	////stepSpindle.setPosition(0);
	if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
	{
		digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
	}
}

/// <summary>
/// Index
/// </summary>
/// <comment>
/// 
/// </comment>
/// <param name="directionSpindle">Counterclockwise: -1, Clockwise: 1</param>
/// <returns></returns>
void Index(int directionAxis)
{
	BeginMethodSettings();
	Stepper stepSpindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
	Stepper stepAxis_M3(PIN_AXIS_3_STEP, PIN_AXIS_3_DIR);
	Stepper stepAxis_M4(PIN_AXIS_4_STEP, PIN_AXIS_4_DIR);
	StepperGroup spindleGroup{ stepSpindle, stepAxis_M3 };
	uint32_t maxSpd_M3 = 0;

//	const char* direction_Char = "Direction:";
//	const char* stepsToMove_Char = "Steps to Move:";

	const char* pageIndex_bt3_val_Char = "pageIndex.bt3.val=0";
	const char* pageIndex_va0_Char = "pageIndex.va0.val=0";
	const char* pageIndex_bt1_val_Char = "pageIndex.bt1.val=0";
	const char* pageIndex_bt2_val_Char = "pageIndex.bt2.val=0";
	//if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	//{
	//	digitalWrite(configSetup.cutterMotorPin, LOW);  // Enable
	//}

	stepAxis_M3.setAcceleration(0);
	stepAxis_M4.setAcceleration(0);
	stepSpindle.setAcceleration(0);

	stepAxis_M3.setMaxSpeed(0);
	stepAxis_M4.setMaxSpeed(0);
	stepSpindle.setMaxSpeed(0);

	stepAxis_M3.setPosition(0);
	stepAxis_M4.setPosition(0);
	stepSpindle.setPosition(0);

	stepAxis_M3.setTargetAbs(0);
	stepAxis_M4.setTargetAbs(0);
	stepSpindle.setTargetAbs(0);




	//test
	cumulative_M3 = 0;
	

	stepperId_1 = configIndex_Prime.axisId;
	checkedCount = 1;
	switch (stepperId_1)
	{
		case ID_SPINDLE:
		{
			stepper1_step = PIN_SPINDLE_STEP;
			stepper1_dir = PIN_SPINDLE_DIR;
			break;
		}
		case ID_AXIS_3:
		{
			stepper1_step = PIN_AXIS_3_STEP;
			stepper1_dir = PIN_AXIS_3_DIR;
			break;
		}
		case ID_AXIS_4:
		{
			stepper1_step = PIN_AXIS_4_STEP;
			stepper1_dir = PIN_AXIS_4_DIR;
			break;
		}
	}

	int32_t stepsPerRevolution = 0;
	int32_t stepsPerRevolution_Spindle = 0;
	int32_t stepsPerRevolution_M3 = 0;
	int32_t stepsPerRevolution_M4 = 0;
	int32_t stepsToMove = 0;
	int32_t stepsToMove_Spindle = 0;
	int32_t stepsToMove_M3 = 0;
	int32_t stepsToMove_M4 = 0;

	int stepsToMoveSynchro = 0;
//	int enableAxis = 0;
	uint32_t maxSpeed = 0;
	uint32_t accel = 0;
	double indexSize = 0;
	int degreesOrDivisions = 0;
//	int32_t progress = 0;
	int32_t currentPosition = 0;

	switch (configIndex_Prime.indexId)
	{
		case INDEX_1:
		{
			indexSize = configIndex_1.sizeInUnits;
			degreesOrDivisions = configIndex_1.degreeOrDivision;
			break;
		}
		case INDEX_2:
		{
			indexSize = configIndex_2.sizeInUnits;
			degreesOrDivisions = configIndex_2.degreeOrDivision;
			break;
		}
		case INDEX_3:
		{
			indexSize = configIndex_3.sizeInUnits;
			degreesOrDivisions = configIndex_3.degreeOrDivision;
			break;
		}
		case INDEX_4:
		{
			indexSize = configIndex_4.sizeInUnits;
			degreesOrDivisions = configIndex_4.degreeOrDivision;
			break;
		}
		case INDEX_5:
		{
			indexSize = configIndex_5.sizeInUnits;
			degreesOrDivisions = configIndex_5.degreeOrDivision;
			break;
		}
	}
#ifdef DEBUG
	Serial.print("IndexId: ");
	Serial.println(configIndex_Prime.indexId);
	Serial.print("axisId: ");
	Serial.println(configIndex_Prime.axisId);
#endif // DEBUG
	float stepRev_Temp = 0;
	switch (configIndex_Prime.axisId)
	{
		case ID_SPINDLE:
		{
			stepperId_1 = ID_SPINDLE;
			stepper1_dir = PIN_SPINDLE_DIR ;
			stepper1_step = PIN_SPINDLE_STEP;
			stepsToMove = 0;
			SetEnable(ID_SPINDLE, true, true);
			stepRev_Temp = (configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle);
			stepsPerRevolution_Spindle = static_cast<int>(stepRev_Temp);

			//maxSpeed
			maxSpeed = configIndex_Prime.maxSpd_Spindle * (configIndex_Prime.speedPercent_Spindle * .01);
			stepSpindle.setMaxSpeed(maxSpeed);
			//accel
			accel = configIndex_Prime.accel_Spindle;
			stepSpindle.setAcceleration(accel);

			if (degreesOrDivisions == BY_DEGREES) // Degrees
			{
				//stepsToMove_Spindle = stepsPerRevolution_Spindle * (indexSize / 360) * directionAxis;
				stepsToMove_Spindle = static_cast<int32_t>(stepRev_Temp * (indexSize / 360) * directionAxis);
			}
			else
			{
				stepsToMove_Spindle = stepsPerRevolution_Spindle / (indexSize)*directionAxis;
			}

			Serial.print("1. stepsToMove_Spindle:  ");
			Serial.println(stepsToMove_Spindle);
			stepSpindle.setTargetAbs(stepsToMove_Spindle);


			if (configIndex_Prime.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
			{
				Serial.println("   >>>         Set M3");
				Serial.print("stepsToMove_Spindle:  ");
				Serial.println(stepsToMove_Spindle);
				Serial.print("synchro_Ratio:  ");
				Serial.println(synchro_Ratio);
				checkedCount = 2;
				stepsToMoveSynchro = stepsToMove_Spindle * synchro_Ratio * (configIndex_Prime.synchro_M3_Percentage * 0.01);
				
				Serial.print("stepsToMoveSynchro:  ");
				Serial.println(stepsToMoveSynchro);
				
				maxSpd_M3 = configIndex_Prime.maxSpd_M3 * (configIndex_Prime.speedPercent_M3 * .01);
				
		#ifdef VERBOSE
				Serial.print("configSetup.gearRatio_AxisM3: ");
				Serial.println(configSetup.gearRatio_AxisM3);
				
				Serial.print("configSetup.microsteps_Axis_M3: ");
				Serial.println(configSetup.microsteps_Axis_M3);
				
				Serial.print("configSetup.steps360_Axis_M3: ");
				Serial.println(configSetup.steps360_Axis_M3);
				
				Serial.print("stepsToMove: ");
				Serial.println(stepsToMove);
				Serial.print("synchro_Ratio: ");
				Serial.println(synchro_Ratio);
				Serial.print("configIndex_Prime.synchro_M3_Percentage: ");
				Serial.println(configIndex_Prime.synchro_M3_Percentage);
				
				Serial.print("stepsToMoveSynchro: ");
				Serial.println(stepsToMoveSynchro);
				Serial.print("maxSpd_M3: ");
				Serial.println(maxSpd_M3);
				Serial.print("maxSpeed: ");
				Serial.println(maxSpeed);
		#endif // VERBOSE
				
				stepperId_2 = ID_AXIS_3;
				stepper2_dir = PIN_AXIS_3_DIR;
				stepper2_step = PIN_AXIS_3_STEP;
				SetEnable(ID_AXIS_3, true, true);
				stepAxis_M3.setPosition(0);
				stepAxis_M3.setMaxSpeed(maxSpd_M3);
				stepAxis_M3.setAcceleration(configIndex_Prime.accel_M3);
				stepAxis_M3.setTargetAbs(stepsToMoveSynchro);
				
		#ifdef VERBOSE
				Serial.print("maxSpeed_M3: ");
				Serial.println(maxSpd_M3);
				Serial.print("accel: ");
				Serial.println(configIndex_Prime.accel_M3);
		#endif // VERBOSE
				spindleGroup.startMove();
				MilliDelay(5);
			}
			else
			{
				// Spindle only
				Serial.print("Position                              :  ");
				Serial.println(stepSpindle.getPosition());
				stepSpindle.moveAsync();
			}



	#ifdef VERBOSE			
			Serial.print("                               configSetup.microsteps_Spindle:  ");
			Serial.println(configSetup.microsteps_Spindle);
			Serial.print("stepRev_Temp:  ");
			Serial.println(stepRev_Temp);
			Serial.print("configSetup.steps360_Spindle:  ");
			Serial.println(configSetup.steps360_Spindle);

			Serial.print("configSetup.gearRatio_Spindle:  ");
			Serial.println(configSetup.gearRatio_Spindle);

			Serial.print("stepsPerRevolution:  ");
			Serial.println(stepsPerRevolution);

			Serial.print("configIndex_Main.maxSpd_Spindle:  ");
			Serial.println(configIndex_Prime.maxSpd_Spindle);
			Serial.print("configIndex_Main.speedPercent_Spindle:  ");
			Serial.println(configIndex_Prime.speedPercent_Spindle);

			Serial.print("configIndex_Main.accel_Spindle:  ");
			Serial.println(configIndex_Prime.accel_Spindle);

			Serial.print("stepsToMove_Spindle:  ");
			Serial.println(stepsToMove_Spindle);
	#endif // VERBOSE
			break;
		}
		case ID_AXIS_3:
		{
			stepsToMove = 0;

			SetEnable(ID_AXIS_3, true, true);
			stepsPerRevolution_M3 = (int)round(configSetup.microsteps_Axis_M3 * configSetup.steps360_Axis_M3 * configSetup.gearRatio_AxisM3);
			//maxSpeed
			maxSpeed = configIndex_Prime.maxSpd_M3 * (configIndex_Prime.speedPercent_M3 * .01);
			stepAxis_M3.setMaxSpeed(maxSpeed);
			//accel
			accel = configIndex_Prime.accel_M3;
			stepAxis_M3.setAcceleration(accel);

			if (degreesOrDivisions == BY_DEGREES) // Degrees
			{
				stepsToMove_M3 = stepsPerRevolution_M3 * (indexSize / 360) * directionAxis;
			}
			else
			{
				stepsToMove_M3 = stepsPerRevolution_M3 / (indexSize)*directionAxis;
			}

			stepAxis_M3.setTargetAbs(stepsToMove_M3);
			stepperId_1 = ID_AXIS_3;
			stepper1_dir = PIN_AXIS_3_DIR;
			stepper1_step = PIN_AXIS_3_STEP;
		
	#ifdef VERBOSE
			Serial.print("configIndex_Main.maxSpd_Axis_M3:  ");
			Serial.println(configIndex_Prime.maxSpd_M3);
			Serial.print("configIndex_Main.speedPercent_Axis_M3:  ");
			Serial.println(configIndex_Prime.speedPercent_M3);

			Serial.print("configIndex_Main.accel_Axis_M3:  ");
			Serial.println(configIndex_Prime.accel_M3);

			Serial.print("stepsToMove_M3:  ");
			Serial.println(stepsToMove_M3);
	#endif // VERBOSE

			stepAxis_M3.moveAsync();
			break;
		}
		case ID_AXIS_4:
		{
			stepsToMove = 0;

			SetEnable(ID_AXIS_4, true, true);
			stepsPerRevolution_M4 = (int)round(configSetup.microsteps_Axis_M4 * configSetup.steps360_Axis_M4 * configSetup.gearRatio_AxisM4);
			//maxSpeed
			maxSpeed = configIndex_Prime.maxSpd_M4 * (configIndex_Prime.speedPercent_M4 * .01);
			stepAxis_M4.setMaxSpeed(maxSpeed);
			//accel
			accel = configIndex_Prime.accel_M4;
			stepAxis_M4.setAcceleration(accel);

			if (degreesOrDivisions == BY_DEGREES) // Degrees
			{
				stepsToMove_M4 = stepsPerRevolution_M4 * (indexSize / 360) * directionAxis;
			}
			else
			{
				stepsToMove_M4 = stepsPerRevolution_M4 / (indexSize)*directionAxis;
			}

			stepAxis_M4.setTargetAbs(stepsToMove_M4);
			stepperId_1 = ID_AXIS_4;
			stepper1_dir = PIN_AXIS_4_DIR;
			stepper1_step = PIN_AXIS_4_STEP;
			Serial.print("stepsToMove_M4:  ");
			Serial.println(stepsToMove_M4);

			stepAxis_M4.moveAsync();
			break;
		}
	}


	while (stepSpindle.isMoving || stepAxis_M3.isMoving || stepAxis_M4.isMoving)
	{
		Serial.print("Spindle position:  ");
		Serial.print(stepSpindle.getPosition());
		//Serial.print("   m3 position:  ");
		//Serial.println(stepAxis_M3.getPosition());

		MilliDelay(150);
#ifdef ESTOP_ENABLED

		if (configSetup.eStop > 9 && configSetup.eStop < 40)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
#ifdef DEBUG
				Serial.println("                          EStop ");
#endif // DEBUG
				stepSpindle.emergencyStop();
				stepAxis_M3.emergencyStop();
				stepAxis_M4.emergencyStop();
				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEnable(ID_AXIS_4, false, false);
				SetEStopColors(PAGE_INDEX);
				if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
				{
					digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
				}
				return;
			}
		}
#endif //estop
		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);

			switch (incomingByte)
			{
				case 83: // - S
				{ 
					Serial.print("incomingByte: ");
					Serial.println(incomingByte);
					stepSpindle.stopAsync();
					stepAxis_M3.stopAsync();
					stepAxis_M4.stop();
	#ifdef VERBOSE
					Serial.println("-----------------Stopped---------------------------------");
	#endif // VERBOSE
					MilliDelay(200);
					stopSteppers = true;
					break;
				}
			}
		}

		//Progress bar
		switch (configIndex_Prime.axisId)
		{
			case ID_SPINDLE:
			{
				currentPosition = stepSpindle.getPosition();
				ProgressReport(stepsToMove_Spindle, currentPosition);
				break;
			}
			case ID_AXIS_3:
			{
				currentPosition = stepAxis_M3.getPosition();
				ProgressReport(stepsToMove_M3, currentPosition);
				break;
			}
			case ID_AXIS_4:
			{
				currentPosition = stepAxis_M4.getPosition();
				ProgressReport(stepsToMove_M4, currentPosition);
				break;
			}
		}

		

		MilliDelay(10);
	}
#ifdef DEBUG
	Serial.println("End loop--------------------------------------------------------------");
#endif // DEBUG


	// Update Index flag on Nextion. 
	SerialPrint(pageIndex_va0_Char);
	SerialPrint(nextionEnd);
	SerialPrint(pageIndex_bt3_val_Char);
	SerialPrint(nextionEnd);
	SerialPrint(pageIndex_bt2_val_Char);
	SerialPrint(nextionEnd);
	SerialPrint(pageIndex_bt1_val_Char);
	SerialPrint(nextionEnd);

	// Update pageSync
	SerialPrint("pageSync.va0.val=0");
	SerialPrint(nextionEnd);
	SerialPrint("pageSync.bt1.val=0");
	SerialPrint(nextionEnd);

	// Update pageRec
	SerialPrint("pageRec.va0.val=0");
	SerialPrint(nextionEnd);
	SerialPrint("pageRec.bt1.val=0");
	SerialPrint(nextionEnd);

	// Update pageProgram
	//SerialPrint("pageRec.va0.val=0");
	//SerialPrint("pageRec.bt1.val=0");

	endPosition_Spindle = stepSpindle.getPosition();
	cumulative_Spindle = cumulative_Spindle + stepSpindle.getPosition();
	SetEnable(ID_SPINDLE, false, true);


	endPosition_M3 = stepAxis_M3.getPosition();
	cumulative_M3 = cumulative_M3 + stepAxis_M3.getPosition();
	SetEnable(ID_AXIS_3, false, true);

	endPosition_M4 = stepAxis_M4.getPosition();
	cumulative_M4 = cumulative_M4 + stepAxis_M4.getPosition();
	SetEnable(ID_AXIS_4, false, true);

	switch (configIndex_Prime.axisId)
	{
		case ID_SPINDLE:
		{
			stepperPosition_1 = - endPosition_Spindle;

			if (configIndex_Prime.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
			{
				stepperPosition_2 = - endPosition_M3;
				break;
			}

			Serial.print("stepperPosition_1: ");
			Serial.println(stepperPosition_1);
			break;
		}
		case ID_AXIS_3:
		{
			stepperPosition_1 = - endPosition_M3;
			break;
		}
		case ID_AXIS_4:
		{
			stepperPosition_1 = - endPosition_M4;
			break;
		}
	}

	// 0 = Change to disabled, 1 = keep enabled
	if (configSetup.keepSteppersEnabled == 0)
	{
		switch (configIndex_Prime.axisId)
		{
		case ID_SPINDLE:
		{
			SerialPrint("pageIndex.bt10.val=0"); // 0 = disabled
			SerialPrint("\xFF\xFF\xFF");
			SerialPrint("pageIndex.bt10.txt="); // 0 = disabled
			SerialPrint("\x22");
			SerialPrint("Disabled");
			SerialPrint("\x22\xFF\xFF\xFF");
			break;
		}
		case ID_AXIS_3:
		case ID_AXIS_4:
		{
			SerialPrint("pageIndex.bt11.val=0"); // 0 = disabled
			SerialPrint("\xFF\xFF\xFF");
			SerialPrint("pageIndex.bt11.txt="); // 0 = disabled
			SerialPrint("\x22");
			SerialPrint("Disabled");
			SerialPrint("\x22\xFF\xFF\xFF");
			break;
		}
		}
	}

	SerialPrint("pageIndex.bt0.val=0");
	SerialPrint(nextionEnd);
	SerialPrint("pageIndex.bt1.val=0");
	SerialPrint(nextionEnd);
	SerialPrint("pageIndex.bt2.val=0");
	SerialPrint(nextionEnd);
	if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
	{
		digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
	}
}

void IndexFromFile(int direction)
{
	const char* indexId_Char = "Index ID:";
	const char* index1_Char = "Index1";
	const char* index2_Char = "Index2";
	const char* index3_Char = "Index3";
	const char* index4_Char = "Index4";
	const char* index5_Char = "Index5";
	const char* newIndexSize_Char = "New Index Size: ";
	const char* indexSizeChar = " Size: ";
	const char* fileOrFixed_Char = " File or Fixed: ";
	const char* lineNumber_Char = "Line Number: ";
	const char* pageIndex_t7_Char = "pageIndex.t7.txt=";

	const char* pageIndex_va0_Char = "pageIndex.va0.val=0";
	const char* pageIndex_bt3_Char = "pageIndex.bt3.val=0";
	const char* pageIndex_bt2_Char = "pageIndex.bt2.val=0";
	const char* pageIndex_bt1_Char = "pageIndex.bt1.val=0";
	const char* pageSync_b6_Char = "pageSync.b6.pco=0";
	const char* pageSync_b5_Char = "pageSync.b5.pco=0";
	const char* pageIndex_EndData_Char = "pageIndex.n0.bco=63488";
	const char* pageIndex_StartData_Char = "pageIndex.n0.bco=65395";

	eStopTriggered = false;
	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);

	double newIndexSize = 0;
	int lineNumber = 0;
	badFilename = false;

	SerialPrint(pageIndex_StartData_Char);
	SerialPrint(nextionEnd);

	Serial.print("10. fileNameIndex: ");
	Serial.println(fileNameIndex);
	lineNumber = GetSerialInteger();
#ifdef DEBUG
	Serial.print(lineNumber_Char);
	Serial.println(lineNumber);
#endif // DEBUG
	if (lineNumber == 255)
	{
		lineNumber = 0;
	}

#ifdef VERBOSE
	Serial.print(indexId_Char);
	Serial.println(configIndex_Prime.indexId);
	Serial.print(lineNumber_Char);
	Serial.println(lineNumber);
#endif // VERBOSE

	Serial.print("11. fileNameIndex: ");
	Serial.println(fileNameIndex);
	//GetFilenameFromSerial();

	Serial.print("20. fileNameIndex: ");
	Serial.println(fileNameIndex);

	endOfData_Index = false;
	newIndexSize = GetIndexDataFromSD(lineNumber);

	if( endOfData_Index)
	{
		Serial.println("End of data");

		// Update Index flag on Nextion. 
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

		SerialPrint(pageIndex_EndData_Char);
		SerialPrint(nextionEnd);
		return;
	}

	switch (configIndex_Prime.indexId)
	{
		case INDEX_1:
		{
	#ifdef DEBUG
			Serial.print(fileOrFixed_Char);
			Serial.println(configIndex_1.fileOrFixed);
	#endif // DEBUG

			configIndex_1.sizeInUnits = newIndexSize;
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
		case INDEX_2:
		{
	#ifdef DEBUG
			Serial.print(index2_Char);
			Serial.println(fileOrFixed_Char);
			Serial.println(configIndex_2.fileOrFixed);
	#endif // DEBUG

			configIndex_2.sizeInUnits = newIndexSize;
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
		case INDEX_3:
		{
	#ifdef DEBUG
			Serial.print(index3_Char);
			Serial.print(fileOrFixed_Char);
			Serial.println(configIndex_3.fileOrFixed);
	#endif // DEBUG

			configIndex_3.sizeInUnits = newIndexSize;
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
		case INDEX_4:
		{
	#ifdef DEBUG
			Serial.print(index4_Char);
			Serial.print(fileOrFixed_Char);
			Serial.println(configIndex_4.fileOrFixed);
	#endif // DEBUG

			configIndex_4.sizeInUnits = newIndexSize;
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
		case INDEX_5:
		{
	#ifdef DEBUG
			Serial.print(index5_Char);
			Serial.print(fileOrFixed_Char);
			Serial.println(configIndex_5.fileOrFixed);
	#endif // DEBUG

			configIndex_5.sizeInUnits = newIndexSize;
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

	if (!badFilename && !endOfData_Index)
	{
		Index(direction);
	}
	else
	{
		// Update Index flag on Nextion. 
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
/// 
/// </comment>
/// <param name="newIndexSize">Unit size of index</param>
/// <returns></returns>
void IndexSize(float newIndexSize, int currentIndex)
{
	const char* indexId_Char = "Index ID:";
	const char* index1_Char = "Index1";
	const char* index2_Char = "Index2";
	const char* index3_Char = "Index3";
	const char* index4_Char = "Index4";
	const char* index5_Char = "Index5";
	const char* indexSizeChar = " Size:";
	int stepsPerRevolution = (int)round(configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle);
#ifdef VERBOSE
	Serial.print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	Serial.print(indexId_Char);
	Serial.println(currentIndex);
	Serial.println(newIndexSize);
	Serial.print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
#endif // VERBOSE

	switch (currentIndex)
	{
	case INDEX_1:
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
#ifdef VERBOSE
		Serial.print(index1_Char);
		Serial.print(indexSizeChar);
		Serial.println(configIndex_1.sizeInSteps);
#endif // VERBOSE
		EEPROM.put(eePromAddress_Ind_1, configIndex_1);
		break;
	}
	case INDEX_2:
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
#ifdef VERBOSE
		Serial.print(index2_Char);
		Serial.print(indexSizeChar);
		Serial.println(configIndex_2.sizeInSteps);
#endif // VERBOSE
		EEPROM.put(eePromAddress_Ind_2, configIndex_2);
		break;
	}
	case INDEX_3:
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

#ifdef VERBOSE
		Serial.print(index3_Char);
		Serial.print(indexSizeChar);
		Serial.println(configIndex_3.sizeInSteps);
#endif // VERBOSE
		EEPROM.put(eePromAddress_Ind_3, configIndex_3);
		break;
	}
	case INDEX_4:
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

#ifdef VERBOSE
		Serial.print(index4_Char);
		Serial.print(indexSizeChar);
		Serial.println(configIndex_4.sizeInSteps);
#endif // VERBOSE
		EEPROM.put(eePromAddress_Ind_4, configIndex_4);
		break;
	}
	case INDEX_5:
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

#ifdef VERBOSE
		Serial.print("Steps per revolution: ");
		Serial.print(stepsPerRevolution);
		Serial.print("  NewIndexSize: ");
		Serial.print(newIndexSize);
		Serial.print("   ");
		Serial.print(index5_Char);
		Serial.print(indexSizeChar);
		Serial.println(configIndex_5.sizeInSteps);
#endif // VERBOSE
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
	BeginMethodSettings();
	int32_t currentPosition = 0;

	const char* axisId_Char = "Axis ID:";
	const char* spindle_Char = "Spindle: ";
	const char* axis_Char = "Axis: ";
	const char* targetSteps_Char = "Target Steps:";
	const char* revolutions_Char = "Revolutions:";
	const char* distance_Char = "Distance:";

#ifdef SHOW_POSITION 
	const char* pageBE_t1_Char = "pageBE.t1.txt=";
	const char* pageBE_t2_Char = "pageBE.t2.txt=";
#endif // SHOW_POSITION

	const char* pageSync_bt3_pco_Char = "pageSync.bt3.pco=0";
	const char* pageSync_bt3_val_Char = "pageSync.bt3.val=0";
	const char* pageSync_va0_val_Char = "pageSync.va0.val=0";
	const char* pageSync_bt1_val_Char = "pageSync.bt1.val=0";
	const char* pageSync_bt2_val_Char = "pageSync.bt2.val=0";

	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);

	stepPin = 0;
	dirPin = 0;
	int limitPin_Max = configSetup.limit_Max_Z;
	int limitPin_Min = configSetup.limit_Min_Z;
	int32_t maxSpeed = 0;
	int32_t accel = 0;
	long axisSteps = 0;

	cumulative_Spindle = 0;
	cumulative_Z = 0;
	cumulative_X = 0;
	cumulative_M3 = 0;
	cumulative_M4 = 0;

	switch (configSync.axisId)
	{
		case ID_AXIS_Z:
		{
			stepPin = PIN_AXIS_Z_STEP;
			dirPin = PIN_AXIS_Z_DIR;
			limitPin_Max = configSetup.limit_Max_Z;
			limitPin_Min = configSetup.limit_Min_Z;
			maxSpeed = configSync.maxSpd_Z * configSync.speedPercent_Z * .01;
			accel = configSync.accel_Z;
			axisSteps = DistanceToSteps_Axis(configSync.distance, configSync.axisId) * directionAxis;
			SetEnable(ID_AXIS_Z, true, true);
			break;
		}
		case ID_AXIS_X:
		{
			stepPin = PIN_AXIS_X_STEP;
			dirPin = PIN_AXIS_X_DIR;
			limitPin_Max = configSetup.limit_Max_X;
			limitPin_Min = configSetup.limit_Min_X;
			maxSpeed = configSync.maxSpd_X * configSync.speedPercent_X * .01;
			accel = configSync.accel_X;
			axisSteps = DistanceToSteps_Axis(configSync.distance, configSync.axisId) * directionAxis;
			SetEnable(ID_AXIS_X, true, true);
			break;
		}
		case ID_AXIS_3:
		{
			stepPin = PIN_AXIS_3_STEP;
			dirPin = PIN_AXIS_3_DIR;
			limitPin_Max = configSetup.limit_Max_M3;
			limitPin_Min = configSetup.limit_Min_M3;
			maxSpeed = configSync.maxSpd_M3 * configSync.speedPercent_M3 * .01;
			accel = configSync.accel_M3;
			switch (configSetup.radialOrLinear_Axis_M3)
			{
				case RADIAL_M3:
				{
					axisSteps = DistanceToSteps_M3Radial(configSync.distance) * directionAxis;
					break;
				}
				case LINEAR_M3:
				{
					axisSteps = DistanceToSteps_M3Linear(configSync.distance) * directionAxis;// * (-1);
					break;
				}
			}

			SetEnable(ID_AXIS_3, true, true);
			break;
		}
		case ID_AXIS_4:
		{
			stepPin = PIN_AXIS_4_STEP;
			dirPin = PIN_AXIS_4_DIR;
			limitPin_Max = configSetup.limit_Max_M4;
			limitPin_Min = configSetup.limit_Min_M4;
			maxSpeed = configSync.maxSpd_M4 * configSync.speedPercent_M4 * .01;
			accel = configSync.accel_M4;
			switch (configSetup.radialOrLinear_Axis_M4)
			{
				case RADIAL_M3:
				{
					axisSteps = DistanceToSteps_M4Radial(configSync.distance) * directionAxis;
					break;
				}
				case LINEAR_M3:
				{
					axisSteps = DistanceToSteps_M4Linear(configSync.distance) * directionAxis;// *(-1);
					break;
				}
			}

			SetEnable(ID_AXIS_4, true, true);
			break;
		}
	}

	
	// Set limit switches HIGH
	digitalWrite(limitPin_Max, HIGH);
	digitalWrite(limitPin_Min, HIGH);

	//Stepper stepper_Axis(stepPin, dirPin);
	//StepperGroup stepGroup{ stepperSpindle,stepper_Axis };
	StepperGroup stepGroup{ stepperAxis_Z,stepperAxis_X,stepperAxis_M3,stepperAxis_M4,stepperSpindle };

	Serial.print("Axis step pin:  ");
	Serial.println(stepPin);

	stepperId_1 = ID_SPINDLE;
	stepper1_step = PIN_SPINDLE_STEP;
	stepper1_dir = PIN_SPINDLE_DIR;

	stepperId_2 = configSync.axisId;
	stepper2_step = stepPin;
	stepper2_dir = dirPin;

	float targetSteps_Spindle = 0;
	float revolutions = configSync.revolutions_Spindle;
	if (configSync.spindle_RevolutionsOrDegrees == 1)
	{
		revolutions = 360 / configSync.revolutions_Spindle;
	}
	targetSteps_Spindle = RevolutionsToSteps(revolutions, directionSpindle, ID_SPINDLE);
	stepperSpindle.setPosition(0);
	stepperSpindle
		.setMaxSpeed(configSync.maxSpd_Spindle * configSync.speedPercent_Spindle * .01)
		.setAcceleration(configSync.accel_Spindle);

	stepperSpindle.setTargetAbs(targetSteps_Spindle);

	switch (configSync.axisId)
	{
		case ID_AXIS_Z:
		{
			stepperAxis_Z.setPosition(0);
			stepperAxis_Z
				.setMaxSpeed(maxSpeed)
				.setAcceleration(accel);

			stepperAxis_Z.setTargetAbs(axisSteps);
			break;
		}
		case ID_AXIS_X:
		{
			stepperAxis_X.setPosition(0);
			stepperAxis_X
				.setMaxSpeed(maxSpeed)
				.setAcceleration(accel);

			stepperAxis_X.setTargetAbs(axisSteps);
			break;
		}
		case ID_AXIS_3:
		{
			stepperAxis_M3.setPosition(0);
			stepperAxis_M3
				.setMaxSpeed(maxSpeed)
				.setAcceleration(accel);

			stepperAxis_M3.setTargetAbs(axisSteps);
			break;
		}
		case ID_AXIS_4:
		{
			stepperAxis_M4.setPosition(0);
			stepperAxis_M4
				.setMaxSpeed(maxSpeed)
				.setAcceleration(accel);

			stepperAxis_M4.setTargetAbs(axisSteps);
			break;
		}
	}
#ifdef VERBOSE
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
#endif // VERBOSE

	SetEnable(ID_SPINDLE, true, true);
	SetEnable(configSync.axisId, true, true);

#ifdef VERBOSE
	Serial.println("MoveAsync..............................................");
	Serial.print("configMain.synchro_M3_Spindle: ");
	Serial.println(configMain.synchro_M3_Spindle);
#endif // VERBOSE

	checkedCount = 2;
	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		checkedCount = 3;
		stepperId_3 = ID_AXIS_3;
		stepper3_step = PIN_AXIS_3_STEP;
		stepper3_dir = PIN_AXIS_3_DIR;
#ifdef VERBOSE
		Serial.print("targetSteps_Spindle * synchro_Ratio: ");
		Serial.println(targetSteps_Spindle * synchro_Ratio);
#endif // VERBOSE
		SetEnable(ID_AXIS_3, true, true);
		stepperAxis_M3.setPosition(0);
		stepperAxis_M3
			.setMaxSpeed((configSync.maxSpd_Spindle * configSync.speedPercent_Spindle * .01))
			.setAcceleration(configSync.accel_Spindle)
			.setTargetAbs(targetSteps_Spindle * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01));
		MilliDelay(5);

	}

	Serial.print("Spindle enable state:  ");
	Serial.println(digitalReadFast(PIN_SPINDLE_ENABLE));

	Serial.print("Z enable state:  ");
	Serial.println(digitalReadFast(PIN_AXIS_Z_ENABLE));

	Serial.println("                          StartMove");

	stepGroup.startMove();
	Serial.println("   Moving");

	while (stepperAxis_Z.isMoving || stepperAxis_X.isMoving || stepperAxis_M3.isMoving || stepperAxis_M4.isMoving || stepperSpindle.isMoving)
	{
		//Serial.print("Spindle position:  ");
		//Serial.print(stepperSpindle.getPosition());
		//Serial.print("Axis position:  ");
		//Serial.println(stepper_Axis.getPosition());

		if (limitPin_Max > 9 && limitPin_Max < 40)
		{
			if ((digitalRead(limitPin_Max) != configSetup.limit_NCorNO))// && axisSteps > 0)
			{
#ifdef DEBUG
				Serial.println("                          limitPin_Max");
#endif // Debug
				if (axisSteps > 0)
				{
					stepperAxis_Z.emergencyStop();
					stepperAxis_X.emergencyStop();
					stepperAxis_M3.emergencyStop();
					stepperAxis_M4.emergencyStop();
					stepperSpindle.emergencyStop();;

					stopSteppers = true;
					SetLimitColors(PAGE_SYNC, DIR_CW);
					goto endLoop;
				}
			}
		}
		if (limitPin_Min > 9 && limitPin_Min < 40)
		{
			if ((digitalRead(limitPin_Min) != configSetup.limit_NCorNO))// && axisSteps < 0)
			{
#ifdef DEBUG
				Serial.println("                          limitPin_Min");
#endif // Debug
				if (axisSteps < 0)
				{
					stepperAxis_Z.emergencyStop();
					stepperAxis_X.emergencyStop();
					stepperAxis_M3.emergencyStop();
					stepperAxis_M4.emergencyStop();
					stepperSpindle.emergencyStop();

					stopSteppers = true;
					SetLimitColors(PAGE_SYNC, DIR_CCW);
					goto endLoop;
				}
			}
		}

#ifdef ESTOP_ENABLED
		if (configSetup.eStop > 9 && configSetup.eStop < 40)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
#ifdef DEBUG
				Serial.println("                          EStop");
#endif // Debug
				stepperAxis_Z.emergencyStop();
				stepperAxis_X.emergencyStop();
				stepperAxis_M3.emergencyStop();
				stepperAxis_M4.emergencyStop();
				stepperSpindle.emergencyStop();

				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEnable(ID_AXIS_4, false, false);
				SetEStopColors(PAGE_SYNC);
				if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
				{
					digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
				}
				return;
			}
		}
#endif // estop

#ifdef VERBOSE
		endPosition_Spindle = stepperSpindle.getPosition();

		endPosition_Z = stepperAxis_Z.getPosition();
		endPosition_X = stepperAxis_Z.getPosition();
		endPosition_M3 = stepperAxis_M3.getPosition();
		endPosition_M4 = stepperAxis_M4.getPosition();
#endif // VERBOSE

		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
				case 83: // - S
				{
					stepperAxis_Z.stop();
					stepperAxis_X.stop();
					stepperAxis_M3.stop();
					stepperAxis_M4.stop();
					stepperSpindle.stop();
					stopSteppers = true;
					MilliDelay(200);
					break;
				}
			}
		}

		MilliDelay(100);
		currentPosition = stepperSpindle.getPosition();
		ProgressReport(targetSteps_Spindle, currentPosition);
	}

endLoop:



	endPosition_Spindle = stepperSpindle.getPosition();
	endPosition_Z = stepperAxis_Z.getPosition();
	endPosition_X = stepperAxis_X.getPosition();
	endPosition_M3 = stepperAxis_M3.getPosition();
	endPosition_M4 = stepperAxis_M4.getPosition();

	Serial.print("~~~~~~~~~~~~~~~endPosition_Spindle:  ");
	Serial.println(endPosition_Spindle);
	Serial.print("endPosition_Z:  ");
	Serial.println(endPosition_Z);

	stepperId_1 = ID_SPINDLE;
	//stepperPosition_1 = -endPosition_Spindle;
	stepper1_step = PIN_SPINDLE_STEP;
	stepper1_dir = PIN_SPINDLE_DIR;
	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{

		stepperId_3 = ID_AXIS_3;
		stepper3_step = PIN_AXIS_3_STEP;
		stepper3_dir = PIN_AXIS_3_DIR;
		//stepperPosition_3 = -endPosition_M3;
	}

	// Short distance for Spindle and synchro M3
	stepsPerRev_Spindle = configSetup.gearRatio_Spindle * configSetup.microsteps_Spindle * configSetup.steps360_Spindle;
	stepperPosition_1 = endPosition_Spindle % stepsPerRev_Spindle * (-1);

	stepsPerRev_M3 = configSetup.gearRatio_AxisM3 * configSetup.microsteps_Axis_M3 * configSetup.steps360_Axis_M3;
	stepperPosition_3 = endPosition_M3 % stepsPerRev_M3 * (-1);

	switch (configSync.axisId)
	{
		case ID_AXIS_Z:
		{
			stepperId_2 = ID_AXIS_Z;
			stepperPosition_2 = -endPosition_Z;

			break;
		}
		case ID_AXIS_X:
		{
			stepperId_2 = ID_AXIS_X;
			stepperPosition_2 = -endPosition_X;
			break;
		}
		case ID_AXIS_3:
		{
			stepperId_2 = ID_AXIS_3;
			stepperPosition_2 = -endPosition_M3;
			break;
		}
		case ID_AXIS_4:
		{
			stepperId_2 = ID_AXIS_4;
			stepperPosition_2 = -endPosition_M4;
			break;
		}
	}

	Serial.print("1.~~~~~~~~~~~~~~~stepperPosition_1:  ");
	Serial.println(stepperPosition_1);
	Serial.print("stepperPosition_2:  ");
	Serial.println(stepperPosition_2);
	Serial.print("stepperPosition_3:  ");
	Serial.println(stepperPosition_3);
	Serial.println("1.~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~1111111111111111111");

	Serial.print("2.~~~~~~~~~~~~~~~stepperPosition_1:  ");
	Serial.println(stepperPosition_1);
	Serial.print("stepperPosition_2:  ");
	Serial.println(stepperPosition_2);
	Serial.print("stepperPosition_3:  ");
	Serial.println(stepperPosition_3);
	Serial.println("2.~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~222222222222222222");


	cumulative_Spindle = cumulative_Spindle + endPosition_Spindle;
	cumulative_Z = cumulative_Z + endPosition_Z;
	cumulative_X = cumulative_X + endPosition_X;
	cumulative_M3 = cumulative_M3 + endPosition_M3;
	cumulative_M4 = cumulative_M4 + endPosition_M4;


	SetEnable(ID_SPINDLE, false, true);
#ifdef VERBOSE
	Serial.println("----Sync-------------Sync-------Sync-----------------");
	Serial.print(spindle_Char);
	Serial.println(stepperPosition_1);
	Serial.print(axis_Char);
	Serial.println(stepperPosition_2);

	//Serial.print("stepper_Spindle.getPosition(): ");
	//Serial.println(stepperSpindle.getPosition());
	//Serial.print("stepper_Axis.getPosition():  ");
	//Serial.println(stepper_Axis.getPosition());

	Serial.print("cumulative_Spindle: ");
	Serial.println(cumulative_Spindle);
	Serial.print("cumulative_Axis:  ");
	Serial.println(cumulative_Axis);
#endif // VERBOSE
	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		SetEnable(ID_AXIS_3, false, true);
		stepperPosition_3 = stepperAxis_M3.getPosition() * (-1);
		cumulative_M3 = cumulative_M3 + endPosition_M3;
#ifdef VERBOSE
		Serial.print("stepperPosition_3-M3: ");
		Serial.println(stepperPosition_3);

		Serial.print("stepperAxis_M3.getPosition(): ");
		Serial.println(stepperAxis_M3.getPosition());

		Serial.print("stepperPosition_Cumulative_M3: ");
		Serial.println(cumulative_M3);
#endif // VERBOSE
	}



#ifdef VERBOSE
	Serial.print("stepsPerRev_Spindle:  ");
	Serial.println(stepsPerRev_Spindle);
	Serial.print("stepperPosition_1:  ");
	Serial.println(stepperPosition_1);

	Serial.print("stepperPosition_2:  ");
	Serial.println(stepperPosition_2);

	Serial.print("stepsPerRev_M3:  ");
	Serial.println(stepsPerRev_M3);
	Serial.print("stepperPosition_3:  ");
	Serial.println(stepperPosition_3);
#endif // VERBOSE

	////////////////////////////////////////////////
	switch (configSync.axisId)
	{
	case ID_AXIS_X:
	{
#ifdef SHOW_POSITION 
		degrees_Spindle = StepsToDegrees(endPosition_Spindle, ID_SPINDLE);
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
		degrees_Spindle = StepsToDegrees(endPosition_Spindle, ID_SPINDLE);
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
	case ID_AXIS_3:
	{

#ifdef SHOW_POSITION 
		degrees_Spindle = StepsToDegrees(endPosition_Spindle, ID_SPINDLE);
		distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_3);

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

		SetEnable(ID_AXIS_3, false, true);
		break;
	}
	case ID_AXIS_4:
	{

#ifdef SHOW_POSITION 
		degrees_Spindle = StepsToDegrees(endPosition_Spindle, ID_SPINDLE);
		distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_4);

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

		SetEnable(ID_AXIS_4, false, true);
		break;
	}
	}



	// 0 = Change to disabled, 1 = keep enabled
	if (configSetup.keepSteppersEnabled == 1)
	{
		SerialPrint("pageSync.bt10.val=1"); // 1 = enabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageSync.bt11.val=1"); // 1 = enabled
		SerialPrint("\xFF\xFF\xFF");
	}
	else
	{
		SerialPrint("pageSync.bt10.val=0"); // 0 = disabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageSync.bt10.txt="); // 0 = disabled
		SerialPrint("\x22");
		SerialPrint("Disabled");
		SerialPrint("\x22\xFF\xFF\xFF");

		SerialPrint("pageSync.bt11.val=0"); // 0 = disabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageSync.bt11.txt="); // 0 = disabled
		SerialPrint("\x22");
		SerialPrint("Disabled");
		SerialPrint("\x22\xFF\xFF\xFF");
	}
	if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
	{
		digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
	}

#ifdef DEBUG
	Serial.println("---------Sync End--------------------------------");
#endif // DEBUG
}

void GetSpindleRetroSteps()
{
	Serial.print("55555++++++++++++retroSpindleChangeSteps:");
	Serial.println(retroSpindleChangeSteps);
	Serial.print("55555++++++++++++spindleSteps_Next:");
	Serial.println(spindleSteps_Next);


	if (configRec.grow_AxisChange > 0)
	{
		if (retroSpindleSteps > fabsl(spindleSteps))
		{
			if (spindleSteps_Next > 0)
			{
				spindleSteps_Next += retroSpindleChangeSteps;
			}
			else
			{
				spindleSteps_Next -= retroSpindleChangeSteps;
			}
			Serial.print("11111++++++++++++++spindleSteps_Next:");
			Serial.println(spindleSteps_Next);
		}
		else
		{
			if (spindleSteps_Next < 0)
			{
				spindleSteps_Next -= retroSpindleChangeSteps;
			}
			else
			{
				spindleSteps_Next += retroSpindleChangeSteps;
			}

			Serial.print("66666++++++++++++++++spindleSteps_Next:");
			Serial.println(spindleSteps_Next);

		}
	}

	Serial.print("99999++++++++++++++++axisSteps:        ");
	Serial.println(axisSteps);
	Serial.print("99999++++++++++++++++axisSteps_Next:   ");
	Serial.println(axisSteps_Next);
}


void GetAxisRetroSteps()
{
	Serial.print("55555++++++++++++retroAxisChangeSteps:");
	Serial.println(retroAxisChangeSteps);
	Serial.print("55555++++++++++++axisSteps_Next:");
	Serial.println(axisSteps_Next);


	if (configRec.grow_AxisChange > 0)
	{
		if (retroAxisSteps > fabsl(axisSteps))
		{
			if (axisSteps_Next > 0)
			{
				axisSteps_Next += retroAxisChangeSteps;
			}
			else
			{
				axisSteps_Next -= retroAxisChangeSteps;
			}

			if (spindleSteps_Next > 0)
			{
				Serial.print("11111++++++++++++++spindleSteps_Next:");
				Serial.println(spindleSteps_Next);
				spindleSteps_Next += retroSpindleChangeSteps;
				Serial.print("22222+++++++++++++spindleSteps_Next:");
				Serial.println(spindleSteps_Next);
			}
			else
			{
				Serial.print("33333++++++++++++++++spindleSteps_Next:");
				Serial.println(spindleSteps_Next);
				spindleSteps_Next -= retroSpindleChangeSteps;
				Serial.print("33333++++++++++++++++spindleSteps_Next:");
				Serial.println(spindleSteps_Next);
			}
		}
		else
		{
			if (axisSteps_Next > 0)
			{
				axisSteps_Next -= retroAxisChangeSteps;
			}
			else
			{
				axisSteps_Next += retroAxisChangeSteps;
			}

			//if (spindleSteps_Next > 0)
			//{
			//	Serial.print("66666++++++++++++++++spindleSteps_Next:");
			//	Serial.println(spindleSteps_Next);
			//	spindleSteps_Next -= retroSpindleChangeSteps;
			//	Serial.print("77777++++++++++++++++spindleSteps_Next:");
			//	Serial.println(spindleSteps_Next);

			//}
			//else
			//{
			//	Serial.print("88888++++++++++++++++spindleSteps_Next:");
			//	Serial.println(spindleSteps_Next);
			//	spindleSteps_Next += retroSpindleChangeSteps;
			//	Serial.print("88888++++++++++++++++spindleSteps_Next:");
			//	Serial.println(spindleSteps_Next);
			//}

		}
	}

	Serial.print("99999++++++++++++++++axisSteps:        ");
	Serial.println(axisSteps);
	Serial.print("99999++++++++++++++++axisSteps_Next:   ");
	Serial.println(axisSteps_Next);
	//Serial.print("99999++++++++++++++++spindleSteps_Next:");
	//Serial.println(spindleSteps_Next);
}

void GetRetroAxisSteps()
{
	if (configRec.grow_AxisChange != 0)
	{
			if (axisSteps_Next > 0)
			{ 
				axisSteps_Next += retroAxisSteps;
			}
			else
			{
				axisSteps_Next -= retroAxisSteps;
			}
	}

	Serial.print("axisSteps_Next:   ");
	Serial.println(axisSteps_Next);
}

void GetRetroSpindleSteps()
{

		if (configRec.grow_SpindleChange != 0)
		{
			if (spindleSteps_Next > 0)
			{
				spindleSteps_Next += retroSpindleSteps;
			}
			else
			{
				spindleSteps_Next -= retroSpindleSteps;
			}
		}

	Serial.print("sssssss++++++++++++++++spindleSteps_Next:   ");
	Serial.println(spindleSteps_Next);
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
	int32_t currentPosition = 0;
	BeginMethodSettings();

	Stepper stepSpindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
	Stepper stepAxis_Z(PIN_AXIS_Z_STEP, PIN_AXIS_Z_DIR);
	Stepper stepAxis_X(PIN_AXIS_X_STEP, PIN_AXIS_X_DIR);
	Stepper stepAxis_M3(PIN_AXIS_3_STEP, PIN_AXIS_3_DIR);
	Stepper stepAxis_M4(PIN_AXIS_4_STEP, PIN_AXIS_4_DIR);

	stepAxis_Z.setAcceleration(0);
	stepAxis_X.setAcceleration(0);
	stepAxis_M3.setAcceleration(0);
	stepAxis_M4.setAcceleration(0);
	stepSpindle.setAcceleration(0);

	stepAxis_Z.setMaxSpeed(0);
	stepAxis_X.setMaxSpeed(0);
	stepAxis_M3.setMaxSpeed(0);
	stepAxis_M4.setMaxSpeed(0);
	stepSpindle.setMaxSpeed(0);

	stepAxis_Z.setPosition(0);
	stepAxis_X.setPosition(0);
	stepAxis_M3.setPosition(0);
	stepAxis_M4.setPosition(0);
	stepSpindle.setPosition(0);

	stepAxis_Z.setTargetAbs(0);
	stepAxis_X.setTargetAbs(0);
	stepAxis_M3.setTargetAbs(0);
	stepAxis_M4.setTargetAbs(0);
	stepSpindle.setTargetAbs(0);


	int limitPin_Max = configSetup.limit_Max_Z;
	int limitPin_Min = configSetup.limit_Min_Z;
	int32_t maxSpeed = 0;
	int32_t accel = 0;
	int32_t waves = 0;
//	int32_t speedPercent = 0;

	float spindleDegrees = 0;
	long steps_Spindle = 0;
	long steps_Axis = 0;

	cumulative_Spindle = 0;
	cumulative_Z = 0;
	cumulative_X = 0;
	cumulative_M3 = 0;
	cumulative_M4 = 0;

	StepperGroup stepGroup{ stepAxis_Z,stepAxis_X,stepAxis_M3,stepAxis_M4,stepSpindle };

	// Set MaxSpd and Accel
	switch (configRec.axisId)
	{
		case ID_AXIS_Z:
		{
			stepPin = PIN_AXIS_Z_STEP;
			dirPin = PIN_AXIS_Z_DIR;
			limitPin_Max = configSetup.limit_Max_Z;
			limitPin_Min = configSetup.limit_Min_Z;
			maxSpeed = configRec.maxSpd_Z * configRec.speedPercent_Z * .01;
			accel = configRec.accel_Z;
		//	speedPercent = configRec.speedPercent_Z;
	#ifdef VERBOSE
			//Serial.print("Z---AxisMaxSpd:");
			//Serial.println(configRec.maxSpd_Axis_Z);
			//Serial.print("Z---AxisSpeedPercent:");
			//Serial.println(configRec.speedPercent_Axis_Z);
			//Serial.print("Z---AxisAccel:");
			//Serial.println(configRec.accel_Axis_Z);
	#endif // VERBOSE

			stepAxis_Z
				.setMaxSpeed(maxSpeed)
				.setAcceleration(accel)
				.setTargetAbs(0);

			break;
		}
		case ID_AXIS_X:
		{
			stepPin = PIN_AXIS_X_STEP;
			dirPin = PIN_AXIS_X_DIR;
			limitPin_Max = configSetup.limit_Max_X;
			limitPin_Min = configSetup.limit_Min_X;
			maxSpeed = configRec.maxSpd_X * configRec.speedPercent_X * .01;
			accel = configRec.accel_X;
		//	speedPercent = configRec.speedPercent_X;
	#ifdef VERBOSE
			Serial.print("X---AxisMaxSpd:");
			Serial.println(configRec.maxSpd_X);
			Serial.print("X---AxisSpeedPercent:");
			Serial.println(configRec.speedPercent_X);
			Serial.print("X---AxisAccel:");
			Serial.println(configRec.accel_X);
	#endif // VERBOSE

			stepAxis_X
				.setMaxSpeed(maxSpeed)
				.setAcceleration(accel)
				.setTargetAbs(0);
			break;
		}
		case ID_AXIS_3:
		{
			stepPin = PIN_AXIS_3_STEP;
			dirPin = PIN_AXIS_3_DIR;
			limitPin_Max = configSetup.limit_Max_M3;
			limitPin_Min = configSetup.limit_Min_M3;
			maxSpeed = configRec.maxSpd_M3 * configRec.speedPercent_M3 * .01;
			accel = configRec.accel_M3;
		//	speedPercent = configRec.speedPercent_M3;
	#ifdef VERBOSE
			Serial.print("M3---AxisMaxSpd:");
			Serial.println(configRec.maxSpd_M3);
			Serial.print("M3---AxisSpeedPercent:");
			Serial.println(configRec.speedPercent_M3);
			Serial.print("M3---AxisAccel:");
			Serial.println(configRec.accel_M3);
	#endif // VERBOSE

			stepAxis_M3
				.setMaxSpeed(maxSpeed)
				.setAcceleration(accel)
				.setTargetAbs(0);
			break;
		}
		case ID_AXIS_4:
		{
			stepPin = PIN_AXIS_4_STEP;
			dirPin = PIN_AXIS_4_DIR;
			limitPin_Max = configSetup.limit_Max_M4;
			limitPin_Min = configSetup.limit_Min_M4;
			maxSpeed = configRec.maxSpd_M4 * configRec.speedPercent_M4 * .01;
			accel = configRec.accel_M4;
		//	speedPercent = configRec.speedPercent_M4;
	#ifdef VERBOSE
			Serial.print("M4---AxisMaxSpd:");
			Serial.println(configRec.maxSpd_M4);
			Serial.print("M4---AxisSpeedPercent:");
			Serial.println(configRec.speedPercent_M4);
			Serial.print("M4---AxisAccel:");
			Serial.println(configRec.accel_M3);
	#endif // VERBOSE

			stepAxis_M4
				.setMaxSpeed(maxSpeed)
				.setAcceleration(accel)
				.setTargetAbs(0);
			break;
		}
	}

	stepperId_1 = ID_SPINDLE;
	stepper1_step = PIN_SPINDLE_STEP;
	stepper1_dir = PIN_SPINDLE_DIR;

	stepperId_2 = configRec.axisId;
	stepper2_step = stepPin;
	stepper2_dir = dirPin;

	// Set limit switches HIGH
	digitalWrite(limitPin_Max, HIGH);
	digitalWrite(limitPin_Min, HIGH);

	// Set speed and acceleration
	stepSpindle
		.setMaxSpeed(configRec.maxSpd_Spindle * configRec.speedPercent_Spindle * .01)
		.setAcceleration(configRec.accel_Spindle);

	// Retro Change Steps
	retroSpindleChangeSteps = (configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (configRec.grow_SpindleChange / 360);
	retroSpindleSteps = (configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (configRec.grow_SpindleChange / 360);

	retroAxisSteps = DistanceToSteps_Axis(configRec.grow_AxisChange, configRec.axisId);
	retroAxisChangeSteps = DistanceToSteps_Axis(configRec.grow_AxisChange, configRec.axisId);
	//Serial.print("$$$$$$$$$$$$$$$$$$$$$$ retroAxisChangeSteps: ");
	//Serial.println(retroAxisChangeSteps);
	//Serial.print("configRec.retro_AxisChange: ");
	//Serial.println(configRec.retro_AxisChange);
	switch (configRec.radial_Axial)
	{
		case RADIAL: // Radial
		{
			waves = configRec.waves_Radial;
			switch (configRec.style)
			{
				case TRIANGLE_STYLE: // Triangle
				case SQUARE_STYLE: // Square
				{
					spindleDegrees = configRec.degrees_Radial_Spindle / (configRec.waves_Radial * 2);
					break;
				}
				case SAWTOOTH_STYLE: // Sawtooth
				{
					spindleDegrees = configRec.degrees_Radial_Spindle / (configRec.waves_Radial);
					break;
				}
			}

			spindleSteps = round((configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (spindleDegrees / 360) * wavDir);
			axisSteps = (DistanceToSteps_Axis(configRec.amplitude_Radial_Axis, configRec.axisId)) * wavDir;
			break;
		}
		case AXIAL: // Axial
		{
			waves = configRec.waves_Axial;
			spindleSteps = (configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (configRec.amplitude_Axial_Spindle / 360) * wavDir;

			switch (configRec.style)
			{
				case TRIANGLE_STYLE: // Triangle
				case SQUARE_STYLE: // Square
				{
					axisSteps = (DistanceToSteps_Axis(configRec.distance_Axial_Axis, configRec.axisId) / (configRec.waves_Axial * 2)) * wavDir;
					break;
				}
				case SAWTOOTH_STYLE: // Sawtooth
				{
					axisSteps = (DistanceToSteps_Axis(configRec.distance_Axial_Axis, configRec.axisId) / (configRec.waves_Axial)) * wavDir;
					break;
				}
			}

			break;
		}
	}
	spindleSteps_Next = spindleSteps;
	axisSteps_Next = axisSteps;
	steps_Spindle = spindleSteps_Next;
	steps_Axis = axisSteps_Next;

	SetEnable(ID_SPINDLE, true, true);
	checkedCount = 2;
	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		checkedCount = 3;
		stepperId_3 = ID_AXIS_3;
		stepper3_step = PIN_AXIS_3_STEP;
		stepper3_dir = PIN_AXIS_3_DIR;
		SetEnable(ID_AXIS_3, true, true);
		stepAxis_M3.setPosition(0);
		stepAxis_M3
			.setMaxSpeed((configRec.maxSpd_Spindle * configRec.speedPercent_Spindle * .01))
			.setAcceleration(configRec.accel_Spindle);
		MilliDelay(5);
	}

	// Enable Steppers
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
		case ID_AXIS_3:
		{
			SetEnable(ID_AXIS_3, true, true);
			break;
		}
		case ID_AXIS_4:
		{
			SetEnable(ID_AXIS_4, true, true);
			break;
		}
	}

	int32_t i_mod_2 = 0;
	int32_t leg = 1;
	int32_t waveCount = waves * 2;
	switch (configRec.style)
	{
		case TRIANGLE_STYLE: // Triangle
		{
			waveCount = waves * 2;
			Serial.print("waveCount: ");
			Serial.println(waveCount);
			break;
		}
		case SAWTOOTH_STYLE: // Sawtooth
		{
			waveCount = (waves * 2);// +2;

			// Double the distance
			switch (configRec.radial_Axial)
			{
			case RADIAL: // Radial
			{
				spindleSteps = spindleSteps;
				spindleSteps_Next = spindleSteps;
				break;
			}
			case AXIAL:
			{
				axisSteps = axisSteps;
				axisSteps_Next = axisSteps;
				break;
			}
			}


			break;
		}
		case SQUARE_STYLE: // Square
		{
			waveCount = ((waves * 4));// +4) * 2);
			break;
		}
	}


	for (int i = 1; i <= waveCount; i++)
	{
		Serial.print("i......................: ");
		Serial.println(i);
		currentPosition = i;
		ProgressReport(waveCount, currentPosition);
		////stepAxis_Z.setPosition(0);
		////stepAxis_X.setPosition(0);
		////stepAxis_M3.setPosition(0);
		////stepAxis_M4.setPosition(0);
		////stepSpindle.setPosition(0);

		switch (configRec.style)
		{
		case TRIANGLE_STYLE: // Triangle
		{
			if (i > 1)
			{
				if (i > 2) // Skip the first wave
				{
					if (i % 2 == 1)
					{
						if (configRec.grow_AxisChange != 0)
						{
							GetRetroAxisSteps();
						}

						if (configRec.grow_SpindleChange != 0)
						{
							GetRetroSpindleSteps();
						}
					}
				}

				// Change direction on each leg
				switch (configRec.radial_Axial)
				{
				case RADIAL: // Radial
				{
					axisSteps_Next *= -1;

					break;
				}
				case AXIAL: // Axial
				{
					spindleSteps_Next *= -1;
					break;
				}
				}
			}

			steps_Spindle = spindleSteps_Next;
			steps_Axis = axisSteps_Next;

			break;
		}

		case SQUARE_STYLE: // Square
		{
			leg = i % 4;
			Serial.print("leg: ");
			Serial.println(leg);
			//if (i > 4)
			//{
			switch (leg)
			{
			case 1:
			{
				switch (configRec.radial_Axial)
				{
				case RADIAL: // Radial
				{
					if (i > 4)
					{
						GetRetroAxisSteps();
					}
					steps_Spindle = 0;
					steps_Axis = axisSteps_Next;
					break;
				}
				case AXIAL:
				{
					if (i > 4)
					{
						GetRetroSpindleSteps();
					}
					steps_Spindle = spindleSteps_Next;
					steps_Axis = 0;
					break;
				}
				}
				break;
			}
			case 2:
			{
				switch (configRec.radial_Axial)
				{
				case RADIAL: // Radial
				{
					if (i > 4)
					{
						GetRetroSpindleSteps();
					}
					steps_Spindle = spindleSteps_Next;
					steps_Axis = 0;
					break;
				}
				case AXIAL:
				{
					if (i > 4)
					{
						GetRetroAxisSteps();
					}

					steps_Spindle = 0;
					steps_Axis = axisSteps_Next;

					break;
				}
				}

				break;
			}

			case 3:
			{
				switch (configRec.radial_Axial)
				{
				case RADIAL: // Radial
				{
					//if (i > 1)
					//{
				//		GetRetroAxisSteps();
					//}
					steps_Spindle = 0;
					steps_Axis = -axisSteps_Next;

					break;
				}
				case AXIAL:
				{
					//if (i > 1)
					//{
					//	GetRetroSpindleSteps();
					//}
					steps_Spindle = -spindleSteps_Next;
					steps_Axis = 0;

					break;
				}
				}
				break;
			}

			case 0:
			{
				switch (configRec.radial_Axial)
				{
				case RADIAL: // Radial
				{
					//if (i > 1)
					//{
					//	GetRetroSpindleSteps();
					//}
					steps_Spindle = spindleSteps_Next;
					steps_Axis = 0;

					break;
				}
				case AXIAL:
				{
					//if (i > 1)
					//{
					//	GetRetroAxisSteps();
					//}
					steps_Spindle = 0;
					steps_Axis = axisSteps_Next;

					break;
				}
				}
				break;
			}
			}

			//}

			Serial.print("steps_Spindle: ");
			Serial.println(steps_Spindle);
			Serial.print("steps_Axis: ");
			Serial.println(steps_Axis);
			break;
		}

		case SAWTOOTH_STYLE: // Sawtooth
		{
			i_mod_2 = i % 2;
			if (i > 1)
			{
				switch (i_mod_2)
				{
				case 0:
				{
					switch (configRec.radial_Axial)
					{
					case RADIAL: // Radial
					{
						steps_Spindle = 0;
						steps_Axis = -axisSteps_Next;

						//Serial.print("Radial steps_Axis-mod 0:");
						//Serial.println(steps_Axis);
						//Serial.print("Radial steps_Spindle-mod 0:");
						//Serial.println(steps_Spindle);

						MilliDelay(5);
						break;
					}
					case AXIAL:
					{
						steps_Spindle = -spindleSteps_Next;
						steps_Axis = 0;

						//Serial.print("Axial steps_Spindle-mod 0:");
						//Serial.println(steps_Spindle);
						//Serial.print("Axial steps_Axis-mod 0:");
						//Serial.println(steps_Axis);
						break;
					}
					}

					Serial.print("0. steps_Axis:");
					Serial.println(steps_Axis);
					break;
				}
				default:
				{
					if (i > 1)
					{
						GetRetroAxisSteps();

						GetRetroSpindleSteps();
					}

					Serial.print("1. steps_Axis:");
					Serial.println(steps_Axis);

					steps_Spindle = spindleSteps_Next;
					steps_Axis = axisSteps_Next;

					//Serial.print("steps_Spindle:");
					//Serial.println(steps_Spindle);
					Serial.print("2. steps_Axis:");
					Serial.println(steps_Axis);
					break;
				}
				}
			}

			Serial.print("steps_Spindle: ");
			Serial.println(steps_Spindle);
			Serial.print("steps_Axis: ");
			Serial.println(steps_Axis);
			break;
		}
		}

		switch (configRec.axisId)
		{
			case ID_AXIS_Z:
			{
				stepAxis_Z.setPosition(0);
				stepAxis_Z.setTargetAbs(steps_Axis);

				break;
			}
			case ID_AXIS_X:
			{
				stepAxis_X.setPosition(0);
				stepAxis_X.setTargetAbs(steps_Axis);
				break;
			}
			case ID_AXIS_3:
			{
				stepAxis_M3.setPosition(0);
				stepAxis_M3.setTargetAbs(steps_Axis);
				break;
			}
			case ID_AXIS_4:
			{
				stepAxis_M4.setPosition(0);
				stepAxis_M4.setTargetAbs(steps_Axis);
				break;
			}
		}

		stepSpindle.setPosition(0);
		stepSpindle.setTargetAbs(steps_Spindle);
		Serial.print("steps_Spindle: ");
		Serial.print(steps_Spindle);
		Serial.print("   steps_Axis: ");
		Serial.println(steps_Axis);

		if (configRec.style == SQUARE_STYLE && configRec.noLast == 1 && i == (waveCount))
		{
			Serial.print("Stop Square -- waveCount: ");

			Serial.println(waveCount);

			////endPosition_Spindle = stepperSpindle.getPosition();
			////endPosition_Z = stepAxis_Z.getPosition();
			////endPosition_X = stepAxis_X.getPosition();
			////endPosition_M3 = stepAxis_M3.getPosition();
			////endPosition_M4 = stepAxis_M4.getPosition();
			////Serial.println("0...************************");
			////Serial.print("Z: ");
			////Serial.println(endPosition_Z);
			////Serial.print("X: ");
			////Serial.println(endPosition_X);
			////Serial.print("M3: ");
			////Serial.println(endPosition_M3);
			////Serial.print("M4: ");
			////Serial.println(endPosition_M4);
			////Serial.print("Spindle: ");
			////Serial.println(endPosition_Spindle);
			////Serial.println("0...************************");
			goto endLoop;
		}

		if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
		{
			checkedCount = 3;
			stepperId_3 = ID_AXIS_3;
			stepper3_step = PIN_AXIS_3_STEP;
			stepper3_dir = PIN_AXIS_3_DIR;
#ifdef VERBOSE
			Serial.print("steps_Spindle * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01):  ");
			Serial.println(steps_Spindle* synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01));
#endif // VERBOSE
			stepAxis_M3.setPosition(0);
			stepAxis_M3.setTargetAbs(steps_Spindle* synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01));
			MilliDelay(5);
		}

		stepGroup.startMove();

		while (stepAxis_Z.isMoving || stepAxis_X.isMoving || stepAxis_M3.isMoving || stepAxis_M4.isMoving || stepSpindle.isMoving)
		{
			if (limitPin_Max > 9 && limitPin_Max < 40)
			{
				if ((digitalRead(limitPin_Max) != configSetup.limit_NCorNO))// && axisSteps > 0)
				{
#ifdef DEBUG
					Serial.println("                         limitPin_Max");
#endif // DEBUG
					if (axisSteps > 0)
					{
						stepAxis_Z.emergencyStop();
						stepAxis_X.emergencyStop();
						stepAxis_M3.emergencyStop();
						stepAxis_M4.emergencyStop();
						stepSpindle.emergencyStop();

						stopSteppers = true;
						limitTriggered = true;
						SetLimitColors(PAGE_REC, DIR_CW);
						goto endLoop;
					}
				}
			}
			if (limitPin_Min > 9 && limitPin_Min < 40)
			{
				if ((digitalRead(limitPin_Min) != configSetup.limit_NCorNO))// && axisSteps < 0)
				{
#ifdef DEBUG
					Serial.println("                          limitPin_Min");
#endif // DEBUG
					if (axisSteps < 0)
					{
						stepAxis_Z.emergencyStop();
						stepAxis_X.emergencyStop();
						stepAxis_M3.emergencyStop();
						stepAxis_M4.emergencyStop();
						stepSpindle.emergencyStop();

						stopSteppers = true;
						limitTriggered = true;
						SetLimitColors(PAGE_REC, DIR_CCW);
						goto endLoop;
					}
				}
			}
#ifdef ESTOP_ENABLED

			if (configSetup.eStop > 9 && configSetup.eStop < 40)
			{
				if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
				{
#ifdef DEBUG
					Serial.println("                          EStop ");
#endif // DEBUG
					stepAxis_Z.emergencyStop();
					stepAxis_X.emergencyStop();
					stepAxis_M3.emergencyStop();
					stepAxis_M4.emergencyStop();
					stepSpindle.emergencyStop();

					stopSteppers = true;
					// Disable all motors
					SetEnable(ID_SPINDLE, false, false);
					SetEnable(ID_AXIS_Z, false, false);
					SetEnable(ID_AXIS_X, false, false);
					SetEnable(ID_AXIS_3, false, false);
					SetEnable(ID_AXIS_4, false, false);
					SetEStopColors(PAGE_REC);
					if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
					{
						digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
					}
					return;
				}
			}
#endif // estop
			// Check for Cancel code  
			if (SerialAvailable() > 0)
			{
				incomingByte = SerialRead(serialId);
				Serial.print("incomingByte: ");
				Serial.println(incomingByte);
				switch (incomingByte)
				{
				case 83: // - S   Stop Spindle
				{
					stepAxis_Z.stop();
					stepAxis_X.stop();
					stepAxis_M3.stop();
					stepAxis_M4.stop();
					stepSpindle.stop();
					stopSteppers = true;
					MilliDelay(200);
					goto endLoop;
					break;
				}
				case 99: // - c  Stop Axis
				{
					stepAxis_Z.stop();
					stepAxis_X.stop();
					stepAxis_M3.stop();
					stepAxis_M4.stop();
					stepSpindle.stop();
					stopSteppers = true;
					MilliDelay(200);
					goto endLoop;
					break;
				}
				default:
				{
					break;
				}
				}
			}

			MilliDelay(100);
		}


		cumulative_Spindle = cumulative_Spindle + stepSpindle.getPosition();
		cumulative_Z = cumulative_Z + stepAxis_Z.getPosition();
		cumulative_X = cumulative_X + stepAxis_X.getPosition();
		cumulative_M3 = cumulative_M3 + stepAxis_M3.getPosition();
		cumulative_M4 = cumulative_M4 + stepAxis_M4.getPosition();

		//Serial.println("************************");
		//Serial.print("Cumulative_Z: ");
		//Serial.println(cumulative_Z);
		//Serial.print("Cumulative_X: ");
		//Serial.println(cumulative_X);
		//Serial.print("Cumulative_M3: ");
		//Serial.println(cumulative_M3);
		//Serial.print("Cumulative_M4: ");
		//Serial.println(cumulative_M4);
		//Serial.print("Cumulative_Spindle: ");
		//Serial.println(cumulative_Spindle);
		//Serial.println("************************");

		////endPosition_Spindle = stepSpindle.getPosition();
		////endPosition_Z = stepAxis_Z.getPosition();
		////endPosition_X = stepAxis_X.getPosition();
		////endPosition_M3 = stepAxis_M3.getPosition();
		////endPosition_M4 = stepAxis_M4.getPosition();
		////Serial.println("1...************************");
		////Serial.print("Z: ");
		////Serial.println(endPosition_Z);
		////Serial.print("X: ");
		////Serial.println(endPosition_X);
		////Serial.print("M3: ");
		////Serial.println(endPosition_M3);
		////Serial.print("M4: ");
		////Serial.println(endPosition_M4);
		////Serial.print("Spindle: ");
		////Serial.println(endPosition_Spindle);
		////Serial.println("1...************************");
	}

endLoop:


	MilliDelay(50);
	endPosition_Spindle = cumulative_Spindle;
	endPosition_Z = cumulative_Z;
	endPosition_X = cumulative_X;
	endPosition_M3 = cumulative_M3;
	endPosition_M4 = cumulative_M4;

	Serial.println("2.....************************");
	Serial.print("Z: ");
	Serial.println(endPosition_Z);
	Serial.print("X: ");
	Serial.println(endPosition_X);
	Serial.print("M3: ");
	Serial.println(endPosition_M3);
	Serial.print("M4: ");
	Serial.println(endPosition_M4);
	Serial.print("Spindle: ");
	Serial.println(endPosition_Spindle);
	Serial.println("2...........************************");


	Serial.println(">>>>>>>>>>>>>>>>");
	Serial.print("Cumulative_Z: ");
	Serial.println(cumulative_Z);
	Serial.print("Cumulative_X: ");
	Serial.println(cumulative_X);
	Serial.print("Cumulative_M3: ");
	Serial.println(cumulative_M3);
	Serial.print("Cumulative_M4: ");
	Serial.println(cumulative_M4);
	Serial.print("Cumulative_Spindle: ");
	Serial.println(cumulative_Spindle);
	Serial.println("<<<<<<<<<<");





	stepperPosition_1 = endPosition_Spindle * (-1);
	stepperPosition_2 = endPosition_Axis * (-1);

	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		SetEnable(ID_AXIS_3, false, true);
		stepperPosition_3 = stepAxis_M3.getPosition() * (-1);
#ifdef VERBOSE
		Serial.print("stepperPosition_3: ");
		Serial.println(stepperPosition_3);
#endif // VERBOSE
	}
#ifdef SHOW_POSITION 
	degrees_Spindle = StepsToDegrees(endPosition_Spindle, ID_SPINDLE);
	distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_X);//Todo: B&E page
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

	MilliDelay(750);
	SerialPrint("pageRec.bt3.val=0");// The first one may be ignored by Nextion, so resend.
	SerialPrint(nextionEnd);
	SerialPrint("pageRec.bt2.val=0");
	SerialPrint(nextionEnd);

#ifdef VERBOSE
	Serial.print("Spindle:");
	Serial.println(endPosition_Spindle);
	Serial.print("Axis:");
	Serial.println(endPosition_Axis);
#endif // VERBOSE
	//returnSteps_Spindle = endPosition_Spindle;
	SetEnable(ID_SPINDLE, false, true);
	SetEnable(configRec.axisId, false, true);

	// 0 = Change to disabled, 1 = keep enabled
	if (configSetup.keepSteppersEnabled == 1)
	{
		SerialPrint("pageRec.bt10.val=1"); // 1 = enabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageRec.bt11.val=1"); // 1 = enabled
		SerialPrint("\xFF\xFF\xFF");
	}
	else
	{
		SerialPrint("pageRec.bt10.val=0"); // 0 = disabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageRec.bt10.txt="); // 0 = disabled
		SerialPrint("\x22");
		SerialPrint("Disabled");
		SerialPrint("\x22\xFF\xFF\xFF");

		SerialPrint("pageRec.bt11.val=0"); // 0 = disabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageRec.bt11.txt="); // 0 = disabled
		SerialPrint("\x22");
		SerialPrint("Disabled");
		SerialPrint("\x22\xFF\xFF\xFF");
	}
	if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
	{
		digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
	}
	MilliDelay(250);
	Serial.println("-----------------------End");
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
			case ID_AXIS_3: // M3 Axis
			{
				SetEnable(ID_AXIS_3, false, true);
			}
			case ID_AXIS_4: // M4 Axis
			{
				SetEnable(ID_AXIS_4, false, true);
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
	int32_t currentPosition = 0;
	bool stopAll = false;

	int totalSpindleSegments = 8;
	limitTriggered = false;
	eStopTriggered = false;

	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);
	if (configGreekKey.radialOrAxial_Pattern == AXIAL)
	{
		totalSpindleSegments = 4;
	}
	GreekKeyPattern_Initial(totalSpindleSegments);

	for (uint32_t i = 1; i <= configGreekKey.patternCount_Grk; i++)// / 2; i++)
	{
		if (eStopTriggered || limitTriggered)
		{
			Serial.println("GreekKey 4a eStop or limit triggered.......................");
			return;
		}
		SerialPrint("t200.txt=");
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

		MilliDelay(100);
		currentPosition = i;
		ProgressReport(configGreekKey.patternCount_Grk, currentPosition);
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
	int32_t currentPosition = 0;
	bool stopAll = false;
	int totalSpindleSegments = 13;
	limitTriggered = false;

	eStopTriggered = false;

	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);

	if (configGreekKey.radialOrAxial_Pattern == AXIAL)
	{
		totalSpindleSegments = 4;
	}

	GreekKeyPattern_Initial(totalSpindleSegments);

	for (uint32_t i = 1; i <= configGreekKey.patternCount_Grk; i++)
	{
		if (eStopTriggered || limitTriggered)
		{
#ifdef DEBUG
			Serial.println("GreekKey 4b eStop or limit triggered.......................");
#endif // DEBUG
			return;
		}
		SerialPrint("t200.txt=");
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

		MilliDelay(100);
		currentPosition = i;
		ProgressReport(configGreekKey.patternCount_Grk, currentPosition);
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
	int32_t currentPosition = 0;
	bool stopAll = false;
	int totalSpindleSegments = 4;
	limitTriggered = false;
	eStopTriggered = false;

	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);

	if (configGreekKey.radialOrAxial_Pattern == AXIAL)
	{
		totalSpindleSegments = 3;
	}

	GreekKeyPattern_Initial(totalSpindleSegments);

	for (uint32_t i = 1; i <= configGreekKey.patternCount_Grk; i++)
	{
		if (eStopTriggered || limitTriggered)
		{
#ifdef DEBUG
			Serial.println("GreekKey 3a eStop or limit triggered.......................");
#endif // DEBUG
			return;
		}

		SerialPrint("t200.txt=");
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

		MilliDelay(100);
		currentPosition = i;
		ProgressReport(configGreekKey.patternCount_Grk, currentPosition);
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
	int32_t currentPosition = 0;
	bool stopAll = false;
	int totalSpindleSegments = 8;
	limitTriggered = false;
	eStopTriggered = false;

	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);

	if (configGreekKey.radialOrAxial_Pattern == AXIAL)
	{
		totalSpindleSegments = 3;
	}
	GreekKeyPattern_Initial(totalSpindleSegments);

	for (uint32_t i = 1; i <= configGreekKey.patternCount_Grk; i++)
	{
		if (eStopTriggered || limitTriggered)
		{
#ifdef DEBUG
			Serial.println("GreekKey 3b eStop or limit triggered.......................");
#endif // DEBUG
			return;
		}
		SerialPrint("t200.txt=");
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

		MilliDelay(100);
		currentPosition = i;
		ProgressReport(configGreekKey.patternCount_Grk, currentPosition);
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
	int32_t currentPosition = 0;
	bool stopAll = false;
	int totalSpindleSegments = 3;
	limitTriggered = false;
	eStopTriggered = false;

	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);
	if (configGreekKey.radialOrAxial_Pattern == AXIAL)
	{
		totalSpindleSegments = 2;
	}

	GreekKeyPattern_Initial(totalSpindleSegments);
#ifdef VERBOSE
	Serial.print("1.axisShortLegSteps:");
	Serial.println(axisShortLegSteps);
#endif // VERBOSE

	for (uint32_t i = 1; i <= configGreekKey.patternCount_Grk; i++)
	{
#ifdef VERBOSE
		Serial.print("----------------i:");
		Serial.println(i);
#endif // VERBOSE

		if (eStopTriggered || limitTriggered)
		{
#ifdef DEBUG
			Serial.println("GreekKey 2a eStop or limit triggered.......................");
#endif // DEBUG
			return;
		}
		SerialPrint("t200.txt=");
		SerialWrite(0x22);
		SerialPrint(i);
		SerialPrint(nextionQuoteEnd);
#ifdef VERBOSE
		Serial.print(" >>>>>>>>>>>>>>>              reverseDirection: ");
		Serial.println(reverseDirection);
#endif // VERBOSE

		switch (configGreekKey.radialOrAxial_Pattern)
		{
		case RADIAL: // Axis Left CCW
		{
#ifdef VERBOSE
			Serial.println("Move_Axis CW");
#endif // VERBOSE
			stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, reverseDirection * DIR_CW, true); //1
#ifdef VERBOSE
			Serial.println("Done");
#endif // VERBOSE
			if (StopGreekKey() || stopAll)
			{
				goto EndLoop;
			}
#ifdef VERBOSE
			Serial.println("Move_Spindle CCW");
#endif // VERBOSE
			stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, reverseDirection * DIR_CCW); //2
#ifdef VERBOSE
			Serial.println("Done");
#endif // VERBOSE
			if (StopGreekKey() || stopAll)
			{
				goto EndLoop;
			}
#ifdef VERBOSE
			Serial.println("Move_Axis CCW");
#endif // VERBOSE
			stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CCW, true); //3
#ifdef VERBOSE
			Serial.println("Done");
#endif // VERBOSE
			if (StopGreekKey() || stopAll)
			{
				goto EndLoop;
			}
#ifdef VERBOSE
			Serial.println("Move_Spindle CW");
#endif // VERBOSE

			stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 1, reverseDirection * DIR_CW); //4
#ifdef VERBOSE
			Serial.println("Done");
#endif // VERBOSE
			if (StopGreekKey() || stopAll)
			{
				goto EndLoop;
			}
#ifdef VERBOSE
			Serial.println("Move_Axis CCW");
#endif // VERBOSE
			stopAll = GreekKey_Move_Axis(axisShortLegSteps, 1, reverseDirection * DIR_CCW, true); //5
#ifdef VERBOSE
			Serial.println("Done");
#endif // VERBOSE
			if (StopGreekKey() || stopAll)
			{
				goto EndLoop;
			}
#ifdef VERBOSE
			Serial.println("Move_Spindle CCW");
#endif // VERBOSE

			stopAll = GreekKey_Move_Spindle(spindleShortLegSteps, 2, reverseDirection * DIR_CCW); //6
#ifdef VERBOSE
			Serial.println("Done");
#endif // VERBOSE
			if (StopGreekKey() || stopAll)
			{
				goto EndLoop;
			}

			break;
		}
		case AXIAL: // Spindle Up CCW
		{
#ifdef VERBOSE
			Serial.print("2.axisShortLegSteps:");
			Serial.println(axisShortLegSteps);
#endif // VERBOSE
			Serial.println("DIR_CW ");
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
			Serial.println("DIR_CCW ");
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
			Serial.println("DIR_CCW ");
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

		MilliDelay(100);
		currentPosition = i;
		ProgressReport(configGreekKey.patternCount_Grk, currentPosition);

	}

EndLoop:

#ifdef DEBUG
	Serial.println("EndLoop:");
#endif // DEBUG

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
	int32_t currentPosition = 0;
	bool stopAll = false;
	int totalSpindleSegments = 6;
	limitTriggered = false;
	eStopTriggered = false;

	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);

	if (configGreekKey.radialOrAxial_Pattern == AXIAL)
	{
		totalSpindleSegments = 2;
	}

	GreekKeyPattern_Initial(totalSpindleSegments);

	for (uint32_t i = 1; i <= configGreekKey.patternCount_Grk; i++)
	{
		if (eStopTriggered || limitTriggered)
		{
#ifdef DEBUG
			Serial.println("GreekKey 2b eStop or limit triggered.......................");
#endif // DEBUG
			return;
		}

		SerialPrint("t200.txt=");
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
		MilliDelay(100);
		currentPosition = i;
		ProgressReport(configGreekKey.patternCount_Grk, currentPosition);
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
#ifdef VERBOSE
	Serial.println("Enter GreekKeyPattern_Initial");
#endif // VERBOSE
	if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
	{
		digitalWrite(configSetup.auxillaryPin, LOW);  // Enable
	}
	// Reset end positions
	endPosition_Spindle = 0;
	endPosition_Axis = 0;
	endPosition_M3 = 0;

	// Set spindle segment steps
	float revolutions_Spindle = configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle;
	spindleShortLegSteps = (int)round((revolutions_Spindle) / (configGreekKey.countPatternPer360_Grk) / segmentCount);

	// Set axis segment steps
	float distance_Axis = 0;
	switch (configGreekKey.axisId)
	{
	case ID_AXIS_Z: // Z Axis
	{
		distance_Axis = configGreekKey.segmentLength_Grk / configSetup.distancePerRev_AxisZ;
		axisShortLegSteps = (int)round(distance_Axis * configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z);
#ifdef VERBOSE
		Serial.print("Z segmentLengthPattern: ");
		Serial.println(configGreekKey.segmentLength_Grk);
		Serial.print("Z configSetup.distancePerRev_AxisZ: ");
		Serial.println(configSetup.distancePerRev_AxisZ);
		Serial.print("Z distance_Axis: ");
		Serial.println(distance_Axis);
#endif // VERBOSE
		break;
	}
	case ID_AXIS_X: // X Axis
	{
		if (configSetup.xAltX == 0)
		{
			distance_Axis = configGreekKey.segmentLength_Grk / configSetup.distancePerRev_AxisX;
			axisShortLegSteps = (int)round(distance_Axis * configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X);
		}
		else
		{
			distance_Axis = configGreekKey.segmentLength_Grk / configSetup.distancePerRev_AxisXAlt;
			axisShortLegSteps = (int)round(distance_Axis * configSetup.steps360_Axis_XAlt * configSetup.microsteps_Axis_XAlt);
		}

		break;
	}
	case ID_AXIS_3: // M3 Axis
	{
		distance_Axis = configGreekKey.segmentLength_Grk;// / configSetup.distancePerRev_AxisB;
#ifdef VERBOSE
		Serial.print("segmentLengthPattern:                 ...: ");
		Serial.println(configGreekKey.segmentLength_Grk);
#endif // VERBOSE
		switch (configSetup.radialOrLinear_Axis_M3)
		{
		case RADIAL_M3: // Radial
		{
			axisShortLegSteps = DistanceToSteps_M3Radial(distance_Axis);
			break;
		}
		case LINEAR_M3: // Linear
		{
			axisShortLegSteps = DistanceToSteps_M3Linear(distance_Axis);
			break;
		}
		}

		break;
	}
	case ID_AXIS_4: // M4 Axis
	{
		distance_Axis = configGreekKey.segmentLength_Grk;
#ifdef VERBOSE
		Serial.print("segmentLengthPattern:                 ...: ");
		Serial.println(configGreekKey.segmentLength_Grk);
#endif // VERBOSE
		switch (configSetup.radialOrLinear_Axis_M4)
		{
		case RADIAL_M4: // Radial
		{
			axisShortLegSteps = DistanceToSteps_M4Radial(distance_Axis);
			break;
		}
		case LINEAR_M4: // Linear
		{
			axisShortLegSteps = DistanceToSteps_M4Linear(distance_Axis);

			break;
		}
		}
		break;
	}
	}

#ifdef VERBOSE
	Serial.print("spindleShortLegSteps:");
	Serial.println(spindleShortLegSteps);
	Serial.print("configGreekKey_Main.countPattern360:");
	Serial.println(configGreekKey.countPatternPer360_Grk);

	Serial.print("configGreekKey_Main.segmentLengthPattern:");
	Serial.println(configGreekKey.segmentLength_Grk);
	Serial.print("0.axisShortLegSteps:");
	Serial.println(axisShortLegSteps);
	Serial.println("--------------------");
#endif // VERBOSE
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
#ifdef VERBOSE
	Serial.print("limitTriggered: ");
#endif // VERBOSE

	if (limitTriggered || eStopTriggered)
	{
#ifdef DEBUG
		Serial.print(limitTriggered);
		Serial.print("  ");
		Serial.println(eStopTriggered);
#endif // DEBUG
	}
	else
	{
#ifdef DEBUG
		Serial.print(limitTriggered);
		Serial.print("  ");
		Serial.println(eStopTriggered);
#endif // DEBUG
		SerialPrint("pageGrk.va0.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageGrk.bt1.val=0");
		SerialPrint(nextionEnd);
	}
	SerialPrint("pageGrk.bt3.val=0");
	SerialPrint(nextionEnd);
	SerialPrint("pageGrk.bt2.val=0");
	SerialPrint(nextionEnd);


	degrees_Spindle = StepsToDegrees(endPosition_Spindle, ID_SPINDLE);
	switch (configGreekKey.axisId)
	{
	case ID_AXIS_Z: // Z Axis
	{
		distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_Z);
		endPosition_Z = endPosition_Axis;
		SetEnable(ID_AXIS_Z, false, true);
		break;
	}
	case ID_AXIS_X: // X Axis
	{
		distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_X);
		endPosition_X = endPosition_Axis;
		SetEnable(ID_AXIS_X, false, true);
		break;
	}
	case ID_AXIS_3: // M3 Axis
	{
		distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_3);
		endPosition_M3 = endPosition_Axis;
		SetEnable(ID_AXIS_3, false, true);
		break;
	}
	case ID_AXIS_4: // M4 Axis
	{
		distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_4);
		endPosition_M4 = endPosition_Axis;
		SetEnable(ID_AXIS_4, false, true);
		break;
	}
	}
	startPositionAbs_Axis = 0;
	//returnSteps_Spindle = endPosition_Spindle;

	stepperPosition_1 = endPosition_Spindle * (-1);
	stepperPosition_2 = endPosition_Axis * (-1);
	stepperPosition_3 = endPosition_M3 * (-1);

#ifdef VERBOSE
	Serial.print("Final endPosition_Spindle:");
	Serial.println(endPosition_Spindle);

	Serial.print("Final endPosition_Axis:");
	Serial.println(endPosition_Axis);

	Serial.print("Final endPosition_M3:");
	Serial.println(endPosition_M3);
#endif // VERBOSE

	SetEnable(ID_SPINDLE, false, true);
	if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
	{
		digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
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
bool GreekKey_Move_Axis(float segmentSteps, float multiplier, int direction, bool updatePosition)
{
	bool retVal = false;
	String axisName = "";
	stepPin = 0;
	dirPin = 0;
	int limitPin_Max = configSetup.limit_Max_Z;
	int limitPin_Min = configSetup.limit_Min_Z;
	int maxSpeed = 0;
	int accel = 0;
	int speedPercentage = 0;
	int actualSpeed = 0;
	int currentAxis = configGreekKey.axisId;
	limitTriggered = false;

	// When updatePosition is false, the cutter is moved in or out of the workpiece and the distance is not accumulated.
	if (!updatePosition)
	{
		switch (configGreekKey.axisId)
		{
		case ID_AXIS_Z:
		{

			currentAxis = ID_AXIS_X;
#ifdef VERBOSE
			Serial.print("X-CurrentAxis:  ");
			Serial.println(currentAxis);
#endif // VERBOSE
			break;
		}
		case ID_AXIS_X:
		{

			currentAxis = ID_AXIS_Z;
#ifdef VERBOSE
			Serial.print("Z-CurrentAxis:  ");
			Serial.println(currentAxis);
#endif // VERBOSE
			break;
		}
		case ID_AXIS_3: // Assumes the toolpath is on the outside of the workpiece generally moving in Z axis
		{

			currentAxis = ID_AXIS_X;
#ifdef VERBOSE
			Serial.print("X-CurrentAxis:  ");
			Serial.println(currentAxis);
#endif // VERBOSE
			break;
		}
		case ID_AXIS_4: // Assumes the toolpath is on the outside of the workpiece generally moving in Z axis
		{

			currentAxis = ID_AXIS_X;
#ifdef VERBOSE
			Serial.print("X-CurrentAxis:  ");
			Serial.println(currentAxis);
#endif // VERBOSE
			break;
		}
		}
	}
#ifdef VERBOSE
	Serial.print("Enter GreekKey_Move_Axis.......................:");
	Serial.println(currentAxis);
	Serial.println("+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=");
	Serial.print("Move_Axis:direction:  ");
	Serial.print(direction);
	Serial.print("   segmentSteps: ");
	Serial.print(segmentSteps);
	Serial.print("   multiplier: ");
	Serial.print(multiplier);
	Serial.print("   updatePosition: ");
	Serial.println(updatePosition);
	//Serial.println("+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=");
#endif // VERBOSE


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
		maxSpeed = configGreekKey.maxSpd_Z;
		accel = configGreekKey.accel_Z;
		speedPercentage = configGreekKey.speedPercent_Z;
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
		maxSpeed = configGreekKey.maxSpd_X;
		accel = configGreekKey.accel_X;
		speedPercentage = configGreekKey.speedPercent_X;
		break;
	}
	case ID_AXIS_3:
	{
		axisName = "M3";
		stepPin = PIN_AXIS_3_STEP;
		dirPin = PIN_AXIS_3_DIR;
		limitPin_Max = configSetup.limit_Max_M3;
		limitPin_Min = configSetup.limit_Min_M3;
		digitalWrite(configSetup.limit_Max_M3, HIGH);
		digitalWrite(configSetup.limit_Min_M3, HIGH);
		SetEnable(ID_AXIS_3, true, true);
		maxSpeed = configGreekKey.maxSpd_M3;
		accel = configGreekKey.accel_M3;
		speedPercentage = configGreekKey.speedPercent_M3;
		break;
	}
	case ID_AXIS_4:
	{
		axisName = "M4";
		stepPin = PIN_AXIS_4_STEP;
		dirPin = PIN_AXIS_4_DIR;
		limitPin_Max = configSetup.limit_Max_M4;
		limitPin_Min = configSetup.limit_Min_M4;
		digitalWrite(configSetup.limit_Max_M4, HIGH);
		digitalWrite(configSetup.limit_Min_M4, HIGH);
		SetEnable(ID_AXIS_4, true, true);
		maxSpeed = configGreekKey.maxSpd_M4;

		accel = configGreekKey.accel_M4;
		speedPercentage = configGreekKey.speedPercent_M4;
		break;
	}
	}

	stepperId_2 = configGreekKey.axisId;
	stepper2_step = stepPin;
	stepper2_dir = dirPin;

	// Set limit switches HIGH
	digitalWrite(limitPin_Max, HIGH);
	digitalWrite(limitPin_Min, HIGH);

	actualSpeed = maxSpeed * speedPercentage * .01;
	//TS4:StepperGroup stepGroup;
	Stepper stepperAxis(stepPin, dirPin);
	float targetSteps = (segmentSteps * multiplier * direction);

#ifdef DEBUG
	Serial.print("   segmentSteps * multiplier * direction: ");
	Serial.println(segmentSteps * multiplier * direction);
#endif // DEBUG


	targetSteps = round(targetSteps);

	checkedCount = 2;
	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		checkedCount = 3;
		stepperId_3 = ID_AXIS_3;
		stepper3_step = PIN_AXIS_3_STEP;
		stepper3_dir = PIN_AXIS_3_DIR;
	}
	stepperAxis.setPosition(0);
	stepperAxis
		.setMaxSpeed(actualSpeed)
		.setAcceleration(accel)
		.setTargetAbs(targetSteps);

#ifdef VERBOSE
	Serial.print("actualSpeed:  ");
	Serial.println(actualSpeed);
	Serial.println("+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=");
#endif // VERBOSE



	stepperAxis.setPosition(0);
	StepperGroup stepGroup{ stepperAxis };
	stepGroup.startMove();

	while (stepperAxis.isMoving)
	{
		if (limitPin_Max > 9 && limitPin_Max < 40)
		{
			if ((digitalRead(limitPin_Max) != configSetup.limit_NCorNO))// && axisSteps > 0)
			{
#ifdef DEBUG
				Serial.println("          limitPin_Max");
#endif // DEBUG

				if (targetSteps > 0)
				{
					stepperAxis.emergencyStop();
					stopSteppers = true;
					SetEnable(ID_SPINDLE, false, true);
					SetEnable(ID_AXIS_Z, false, true);
					SetEnable(ID_AXIS_X, false, true);
					SetEnable(ID_AXIS_3, false, true);
					SetEnable(ID_AXIS_4, false, true);
					SetLimitColors(PAGE_GRK, DIR_CW);
					if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
					{
						digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
					}

					return true;
				}
			}
		}
		if (limitPin_Min > 9 && limitPin_Min < 40)
		{
			if ((digitalRead(limitPin_Min) != configSetup.limit_NCorNO))// && axisSteps < 0)
			{
#ifdef DEBUG
				Serial.println("                          limitPin_Min");
#endif // DEBUG

				if (targetSteps < 0)
				{
					stepperAxis.emergencyStop();
					stopSteppers = true;
					SetEnable(ID_SPINDLE, false, true);
					SetEnable(ID_AXIS_Z, false, true);
					SetEnable(ID_AXIS_X, false, true);
					SetEnable(ID_AXIS_3, false, true);
					SetEnable(ID_AXIS_4, false, true);
					SetLimitColors(PAGE_GRK, DIR_CCW);
					if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
					{
						digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
					}

					return true;
				}
			}
		}

#ifdef ESTOP_ENABLED
		if (configSetup.eStop > 9 && configSetup.eStop < 40)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
#ifdef DEBUG
				Serial.println("  GreekKey_Move                        EStop ");
#endif // DEBUG
				stepperAxis.emergencyStop();
				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEnable(ID_AXIS_4, false, false);
				SetEStopColors(PAGE_GRK);
				if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
				{
					digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
				}
				return true;
			}
		}
#endif // estop
		if (StopGreekKey())
		{
#ifdef VERBOSE
			Serial.println("StopGreekKey() is true:");
#endif // VERBOSE

//			currentSpeed = stepperAxis.;
//#ifdef VERBOSE
//			Serial.print("currentSpeed: ");
//			Serial.println(currentSpeed);
//#endif // VERBOSE
//			if (currentSpeed < MIN_SPEED)
//			{
//				stepControllerAxis.emergencyStop();
//			}
//			else
//			{
				stepperAxis.stop();
				MilliDelay(200);
//			}

			// Disable all motors
			switch (configGreekKey.axisId)
			{
			case ID_AXIS_Z:
			{
				SetEnable(ID_AXIS_Z, false, true);
			}
			case ID_AXIS_X:
			{
				SetEnable(ID_AXIS_X, false, true);
			}
			case ID_AXIS_3:
			{
				SetEnable(ID_AXIS_3, false, true);
			}
			case ID_AXIS_4:
			{
				SetEnable(ID_AXIS_4, false, true);
			}
			}

			retVal = true;
			goto EndLoops;
			break;
		}
		else
		{
			//Serial.println("StopGreekKey() is false:");
		}

	}

EndLoops:
#ifdef VERBOSE
	Serial.print("endPosition_Axis:");
	Serial.println(endPosition_Axis);
#endif // VERBOSE
	if (updatePosition)
	{
		// Update position only for R(ight) and L(eft), not O(ut) or I(n).
		endPosition_Axis = endPosition_Axis + stepperAxis.getPosition();
		cumulative_Axis = cumulative_Axis + stepperAxis.getPosition();

#ifdef VERBOSE

		Serial.println("MoveMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMove: ");
		Serial.print("endPosition_Axis: ");
		Serial.println(endPosition_Axis);
		Serial.print("cumulative_Axis: ");
		Serial.println(cumulative_Axis);
#endif // VERBOSE
	}


#ifdef VERBOSE
	Serial.print("endPosition_Axis:");
	Serial.println(endPosition_Axis);

	Serial.print("axisId:               ....");
	Serial.println(currentAxis);
	Serial.println("Exit GreekKey_Move_Axis");
#endif // VERBOSE

	return retVal;
}

/// <summary>
/// GreekKey_Angular_AxisAndSpindle
/// </summary>
/// <comment>
/// </comment>
/// <param name="shortLegLength">Segment(leg) length</param>
/// <param name="multiplier">Number of segments</param>
/// <param name="direction">Direction to move</param>
/// <returns>True if stop button touched otherwise false</returns>
bool GreekKey_Angular_AxisAndSpindle(
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

	int32_t target_M3 = 0;
	int32_t m3MxSpeed = configGreekKey.maxSpd_M3 * configGreekKey.speedPercent_M3 * .01;

	bool retVal = false;
	//ToDo: Synchro endPosition

	stepperAxis_M3.setPosition(0);

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

		maxSpd_Axis = configGreekKey.maxSpd_Z;
		accel_Axis = configGreekKey.accel_Z;
		speedPercent_Axis = configGreekKey.speedPercent_Z;

		break;
	}
	case ID_AXIS_X:
	{
		stepPin_Axis = PIN_AXIS_X_STEP;
		dirPin_Axis = PIN_AXIS_X_DIR;
		limitPin_Max = configSetup.limit_Max_X;
		limitPin_Min = configSetup.limit_Min_X;
		SetEnable(ID_AXIS_X, true, true);

		maxSpd_Axis = configGreekKey.maxSpd_X;
		accel_Axis = configGreekKey.accel_X;
		speedPercent_Axis = configGreekKey.speedPercent_X;

		break;
	}
	case ID_AXIS_3:
	{
		stepPin_Axis = PIN_AXIS_3_STEP;
		dirPin_Axis = PIN_AXIS_3_DIR;
		limitPin_Max = configSetup.limit_Max_M3;
		limitPin_Min = configSetup.limit_Min_M3;
		SetEnable(ID_AXIS_3, true, true);

		maxSpd_Axis = configGreekKey.maxSpd_M3;

		accel_Axis = configGreekKey.accel_M3;
		speedPercent_Axis = configGreekKey.speedPercent_M3;
#ifdef VERBOSE
		Serial.print("MaxSpd:          ");
		Serial.println(configGreekKey.maxSpd_M3);
		Serial.print("Accel:           ");
		Serial.println(configGreekKey.accel_M3);
		Serial.print("SpeedPercent:    ");
		Serial.println(configGreekKey.speedPercent_M3);
#endif // VERBOSE
		break;
	}
	case ID_AXIS_4:
	{
		stepPin_Axis = PIN_AXIS_4_STEP;
		dirPin_Axis = PIN_AXIS_4_DIR;
		limitPin_Max = configSetup.limit_Max_M4;
		limitPin_Min = configSetup.limit_Min_M4;
		SetEnable(ID_AXIS_4, true, true);

		maxSpd_Axis = configGreekKey.maxSpd_M4;

		accel_Axis = configGreekKey.accel_M4;
		speedPercent_Axis = configGreekKey.speedPercent_M4;
#ifdef VERBOSE
		Serial.print("MaxSpd:          ");
		Serial.println(configGreekKey.maxSpd_M4);
		Serial.print("Accel:           ");
		Serial.println(configGreekKey.accel_M4);
		Serial.print("SpeedPercent:    ");
		Serial.println(configGreekKey.speedPercent_M4);
#endif // VERBOSE
		break;
	}
	}

	stepperId_2 = configGreekKey.axisId;
	stepper2_step = stepPin_Axis;
	stepper2_dir = dirPin_Axis;

	// Set limit switches HIGH
	digitalWrite(limitPin_Max, HIGH);
	digitalWrite(limitPin_Min, HIGH);

	// ToDo:
	AngularLengthAndMaxSpeed(maxSpd_Spindle, maxSpd_Axis);

	SetEnable(ID_SPINDLE, true, true);
	currentSpeedPercent_Axis = speedPercent_Axis * .01;
	currentSpeedPercent_Spindle = speedPercent_Spindle * .01;

	//TS4:StepperGroup stepGroup;
	Stepper stepper_Axis(stepPin_Axis, dirPin_Axis);

	stepper_Axis.setPosition(0);

	int32_t targetSteps_Axis = round((shortLegLength_Axis)*multiplier_Axis);

#ifdef VERBOSE
	Serial.print("shortLegLength_Axis: ");
	Serial.println(shortLegLength_Axis);
	Serial.print("   multiplier_Axis:  ");
	Serial.println(multiplier_Axis);
	Serial.print("   Axis targetSteps: ");
	Serial.println(targetSteps_Axis);
	Serial.print("   maxSpd_Axis: ");
	Serial.println(maxSpd_Axis);
#endif // VERBOSE

	stepperId_1 = ID_SPINDLE;
	stepper1_step = PIN_SPINDLE_STEP;
	stepper1_dir = PIN_SPINDLE_DIR;

	float spindleSteps = (shortLegLength_Spindle)*multiplier_Spindle;
	int32_t targetSteps_Spindle = (int32_t)round(spindleSteps);

	//if (spindleSteps < 0)
	//{
	//	spindleSteps = spindleSteps * -1;
	//	maxSpd_Spindle = maxSpd_Spindle * -1;
	//}

	/////////////////////////////////////////////////
	stepper_Axis
		.setMaxSpeed(maxSpd_Axis * currentSpeedPercent_Axis)
		.setAcceleration(accel_Axis)
		.setTargetAbs(targetSteps_Axis);

	stepperSpindle
		.setMaxSpeed(maxSpd_Spindle * currentSpeedPercent_Spindle)
		.setAcceleration(accel_Spindle);
	stepperSpindle.setPosition(0);


#ifdef DEBUG
	Serial.print("shortLegLength_Spindle: ");
	Serial.println(shortLegLength_Spindle);
	Serial.print("multiplier_Spindle:     ");
	Serial.println(multiplier_Spindle);
	Serial.print("Spindle targetSteps:    ");
	Serial.println(targetSteps_Spindle);
	Serial.print("Spindle speed:          ");
	Serial.println(maxSpd_Spindle * currentSpeedPercent_Spindle);
#endif // DEBUG

	stepperSpindle.setTargetAbs(targetSteps_Spindle);

#ifdef VERBOSE
	Serial.println("-=-=-=-=-=-=-=-=-=-=-=-===-=-=-==-=-=-=-");
	Serial.print("spindleSteps:        ");
	Serial.println(spindleSteps);
	Serial.print("targetSteps_Spindle: ");
	Serial.println(targetSteps_Spindle);
	Serial.print("targetSteps_Axis:    ");
	Serial.println(targetSteps_Axis);
	Serial.print("Axis speed:          ");
	Serial.println(maxSpd_Axis * currentSpeedPercent_Axis);
#endif // VERBOSE
	checkedCount = 2;
	StepperGroup stepGroup{ stepperSpindle, stepper_Axis };
	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		checkedCount = 3;
		stepperId_3 = ID_AXIS_3;
		stepper3_step = PIN_AXIS_3_STEP;
		stepper3_dir = PIN_AXIS_3_DIR;

		target_M3 = targetSteps_Spindle * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01);
		//if (maxSpd_Spindle < 0)
		//{
		//	m3MxSpeed = m3MxSpeed * -1;
		//}
#ifdef VERBOSE
		Serial.print("  target_M3:     ");
		Serial.println(target_M3);
		Serial.print("  speed_M3:      ");
		Serial.println(configGreekKey.maxSpd_M3 * configGreekKey.speedPercent_M3 * .01);

		Serial.print("targetSteps_Spindle: ");
		Serial.println(targetSteps_Spindle);

		Serial.print("synchro_Ratio:   ");
		Serial.println(synchro_Ratio);

		Serial.print("  configMain.synchro_M3_Percentage * 0.01:  ");
		Serial.println(configMain.synchro_M3_Percentage * 0.01);
		Serial.println("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
#endif // VERBOSE





		stepperAxis_M3.setPosition(0);
		stepperAxis_M3.setTargetAbs(target_M3);
		stepperAxis_M3.setMaxSpeed(m3MxSpeed);
		stepperAxis_M3.setAcceleration(configGreekKey.accel_M3);
		SetEnable(ID_AXIS_3, true, true);
		MilliDelay(5);

		StepperGroup stepGroup{ stepperSpindle, stepper_Axis, stepperAxis_M3 };
	}
	//else
	//{
	//	StepperGroup stepGroup{ stepperSpindle, stepper_Axis };
	//}

	stepGroup.startMove();
	MilliDelay(100);

	while (stepperSpindle.isMoving)
	{
//		currentSpeed = stepGroup.getCurrentSpeed();
//#ifdef VERBOSE
//		//Serial.print("currentSpeed: ");
//		//Serial.println(currentSpeed);
//#endif // VERBOSE
//
//		// Teensy may crash when Spindle and M3 are syncronized so call stop when current speed is less than MIN_SPEED
//		if (currentSpeed < MIN_SPEED)
//		{
//			MilliDelay(500);
//#ifdef DEBUG
//			Serial.println(">>>>>>>>>>>>>>>>MinSpeed Stop<<<<<<<<<<<<<<< ");
//#endif // Debug
//			stepGroup.stop();
//			goto endLoop;
//		}

		if (limitPin_Max > 9 && limitPin_Max < 40)
		{
			if (digitalRead(limitPin_Max) != configSetup.limit_NCorNO)
			{
#ifdef DEBUG
				Serial.println("                          limitPin");
#endif // DEBUG

				stepperSpindle.emergencyStop();
				stepper_Axis.emergencyStop();
				stepperAxis_M3.emergencyStop();

				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, true);
				SetEnable(ID_AXIS_Z, false, true);
				SetEnable(ID_AXIS_X, false, true);
				SetEnable(ID_AXIS_3, false, true);
				SetEnable(ID_AXIS_4, false, true);
				SetLimitColors(PAGE_PROGRAM, DIR_CW);
				if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
				{
					digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
				}

				return true;
			}
		}
		if (limitPin_Min > 9 && limitPin_Min < 40)
		{
			if (digitalRead(limitPin_Min) != configSetup.limit_NCorNO)
			{
#ifdef DEBUG
				Serial.println("                          limitPin");
#endif // DEBUG

				stepperSpindle.emergencyStop();
				stepper_Axis.emergencyStop();
				stepperAxis_M3.emergencyStop();

				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, true);
				SetEnable(ID_AXIS_Z, false, true);
				SetEnable(ID_AXIS_X, false, true);
				SetEnable(ID_AXIS_3, false, true);
				SetEnable(ID_AXIS_4, false, true);
				SetLimitColors(PAGE_PROGRAM, DIR_CCW);
				if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
				{
					digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
				}

				return true;
			}
		}

#ifdef ESTOP_ENABLED
		if (configSetup.eStop > 9 && configSetup.eStop < 40)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
#ifdef DEBUG
				Serial.println("                          EStop");
#endif // DEBUG
				stepperSpindle.emergencyStop();
				stepper_Axis.emergencyStop();
				stepperAxis_M3.emergencyStop();

				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEnable(ID_AXIS_4, false, false);
				SetEStopColors(PAGE_PROGRAM);
				if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
				{
					digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
				}

				return true;
			}
		}
#endif // estop
		if (StopGreekKey())
		{
#ifdef VERBOSE
			Serial.println("StopGreekKey() is true:");
#endif // VERBOSE
//			currentSpeed = stepGroup.getCurrentSpeed();
//#ifdef VERBOSE
//			Serial.print("currentSpeed: ");
//			Serial.println(currentSpeed);
//#endif // VERBOSE
//			if (currentSpeed < MIN_SPEED)
//			{
//				stepGroup.emergencyStop();
//			}
//			else
//			{
//				stepGroup.stop();
//			}
			stepperSpindle.stopAsync();
			stepper_Axis.stopAsync();
			stepperAxis_M3.stop();
			MilliDelay(200);
			retVal = true;
			break;
		}
		MilliDelay(5);
	}
//endLoop:
	SetEnable(ID_SPINDLE, false, true);

	switch (configGreekKey.axisId)
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
	case ID_AXIS_3:
	{
		SetEnable(ID_AXIS_3, false, true);
		break;
	}
	case ID_AXIS_4:
	{
		SetEnable(ID_AXIS_4, false, true);
		break;
	}
	}

	endPosition_Axis = endPosition_Axis + stepper_Axis.getPosition();
	endPosition_Spindle = endPosition_Spindle + stepperSpindle.getPosition();
	cumulative_Spindle = cumulative_Spindle + stepperSpindle.getPosition();
	cumulative_Axis = cumulative_Axis + stepper_Axis.getPosition();

	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		SetEnable(ID_AXIS_3, false, true);
		endPosition_M3 = endPosition_M3 + stepperAxis_M3.getPosition();
		cumulative_M3 = cumulative_M3 + stepperAxis_M3.getPosition();
	}

#ifdef VERBOSE
	Serial.print("endPosition_Axis:");
	Serial.println(endPosition_Axis);
	Serial.print("endPosition_Spindle:");
	Serial.println(endPosition_Spindle);
	Serial.print("endPosition_M3:");
	Serial.println(endPosition_M3);
	Serial.println("Exit GreekKey_Move_Angular_TeensyStep............................. ");
#endif // VERBOSE

	return retVal;
}

/// <summary>
/// GreekKey_Angular_TwoAxes
/// </summary>
/// <comment>
/// </comment>
/// <param name-"primaryAxis">Primary axis ID</param>
/// <param name="shortLegLength_Primary">Segment(leg) length</param>
/// <param name="multiplier_Primary">Number of segments</param>
/// <param name-"secondaryAxis">Secondary axis ID</param>
/// <param name="shortLegLength_Secondary">Segment(leg) length</param>
/// <param name="multiplier_Secondary">Number of segments</param>
/// <returns>True if stop button touched otherwise false</returns>
//bool GreekKey_Angular_TwoAxes(
//	uint32_t primaryAxisId,
//	float shortLegLength_Primary,
//	float multiplier_Primary,
//	uint32_t secondaryAxisId,
//	float shortLegLength_Secondary,
//	float multiplier_Secondary)
bool GreekKey_Angular_TwoAxes(int direction)
{
	int stepPin_Primary = 40;
	int dirPin_Primary = 41;
	int limitPin_Max_Primary = 55;
	int limitPin_Min_Primary = 55;
	int maxSpd_Primary = 0;
	int accel_Primary = 0;
	float speedPercent_Primary = 0;
	float currentSpeedPercent_Primary = 0;
	int32_t shortLegLength_Primary = 0;


	int stepPin_Secondary = 40;
	int dirPin_Secondary = 41;
	int limitPin_Max_Secondary = 55;
	int limitPin_Min_Secondary = 55;
	int maxSpd_Secondary = 0;
	int accel_Secondary = 0;
	float speedPercent_Secondary = 0;
	float currentSpeedPercent_Secondary = 0;
	int32_t shortLegLength_Secondary = 0;

	bool retVal = false;
	stepperAxis_M3.setPosition(0);

	switch (primaryAxisId)
	{
	case ID_AXIS_Z:
	{
		stepPin_Primary = PIN_AXIS_Z_STEP;
		dirPin_Primary = PIN_AXIS_Z_DIR;
		limitPin_Max_Primary = configSetup.limit_Max_Z;
		limitPin_Min_Primary = configSetup.limit_Min_Z;
		maxSpd_Primary = configGreekKey.maxSpd_Z;
		accel_Primary = configGreekKey.accel_Z;
		speedPercent_Primary = configGreekKey.speedPercent_Z;
		currentSpeedPercent_Primary = speedPercent_Primary * .01;
		shortLegLength_Primary = (int32_t)(((configGreekKey.segmentLength_Program / configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z)));
		SetEnable(ID_AXIS_Z, true, true);
		break;
	}
	case ID_AXIS_X:
	{
		stepPin_Primary = PIN_AXIS_X_STEP;
		dirPin_Primary = PIN_AXIS_X_DIR;
		limitPin_Max_Primary = configSetup.limit_Max_X;
		limitPin_Min_Primary = configSetup.limit_Min_X;
		maxSpd_Primary = configGreekKey.maxSpd_X;
		accel_Primary = configGreekKey.accel_X;
		speedPercent_Primary = configGreekKey.speedPercent_X;
		currentSpeedPercent_Primary = speedPercent_Primary * .01;
		shortLegLength_Primary = (int32_t)(((configGreekKey.segmentLength_Program / configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X)));

		SetEnable(ID_AXIS_X, true, true);
		break;
	}
	case ID_AXIS_3:
	{
		stepPin_Primary = PIN_AXIS_3_STEP;
		dirPin_Primary = PIN_AXIS_3_DIR;
		limitPin_Max_Primary = configSetup.limit_Max_M3;
		limitPin_Min_Primary = configSetup.limit_Min_M3;
		maxSpd_Primary = configGreekKey.maxSpd_M3;
		accel_Primary = configGreekKey.accel_M3;
		speedPercent_Primary = configGreekKey.speedPercent_M3;
		currentSpeedPercent_Primary = speedPercent_Primary * .01;
		shortLegLength_Primary = (int32_t)(((configGreekKey.segmentLength_Program / configSetup.distancePerRev_AxisM3) * (configSetup.steps360_Axis_M3 * configSetup.microsteps_Axis_M3)));

#ifdef DEBUG
		Serial.print("configSetup.radialOrLinear_Axis_M3: ");
		Serial.println(configSetup.radialOrLinear_Axis_M3);

#endif // DEBUG
		switch (configSetup.radialOrLinear_Axis_M3)
		{
		case RADIAL_M3:
		{
#ifdef DEBUG
			Serial.print("m3Radial_ShortLegSteps: ");
			Serial.println(m3Radial_ShortLegSteps);

#endif // DEBUG

			shortLegLength_Primary = m3Radial_ShortLegSteps;
			break;
		}
		case LINEAR_M3:
		{
			shortLegLength_Primary = (int32_t)(((configGreekKey.segmentLength_Program / configSetup.distancePerRev_AxisM3) * (configSetup.steps360_Axis_M3 * configSetup.microsteps_Axis_M3)));
			break;
		}
		}

#ifdef DEBUG
		Serial.print("shortLegLength_Primary: ");
		Serial.println(shortLegLength_Primary);

#endif // DEBUG
		SetEnable(ID_AXIS_3, true, true);
		break;
	}
	case ID_AXIS_4:
	{
		stepPin_Primary = PIN_AXIS_4_STEP;
		dirPin_Primary = PIN_AXIS_4_DIR;
		limitPin_Max_Primary = configSetup.limit_Max_M4;
		limitPin_Min_Primary = configSetup.limit_Min_M4;
		maxSpd_Primary = configGreekKey.maxSpd_M4;
		accel_Primary = configGreekKey.accel_M4;
		speedPercent_Primary = configGreekKey.speedPercent_M4;
		currentSpeedPercent_Primary = speedPercent_Primary * .01;

		switch (configSetup.radialOrLinear_Axis_M3)
		{
		case RADIAL_M3:
		{
			shortLegLength_Primary = m4Radial_ShortLegSteps;
			break;
		}
		case LINEAR_M3:
		{
			shortLegLength_Primary = (int32_t)(((configGreekKey.segmentLength_Program / configSetup.distancePerRev_AxisM4) * (configSetup.steps360_Axis_M4 * configSetup.microsteps_Axis_M4)));
			break;
		}
		}

		SetEnable(ID_AXIS_4, true, true);
		break;
	}
	case ID_SPINDLE:
	{
		stepPin_Primary = PIN_SPINDLE_STEP;
		dirPin_Primary = PIN_SPINDLE_DIR;
		//limitPin_Max_Primary = configSetup.limit_Max_M4;
		//limitPin_Min_Primary = configSetup.limit_Min_M4;
		maxSpd_Primary = configGreekKey.maxSpd_Spindle;
		accel_Primary = configGreekKey.accel_Spindle;
		speedPercent_Primary = configGreekKey.speedPercent_Spindle;
		currentSpeedPercent_Primary = speedPercent_Primary * .01;
		shortLegLength_Primary = (int32_t)(((configGreekKey.segmentLength_Program / configSetup.gearRatio_Spindle) * (configSetup.steps360_Spindle * configSetup.microsteps_Spindle)));
		SetEnable(ID_SPINDLE, true, true);
		break;
	}
	}
	switch (secondaryAxisId)
	{
	case ID_AXIS_Z:
	{
		stepPin_Secondary = PIN_AXIS_Z_STEP;
		dirPin_Secondary = PIN_AXIS_Z_DIR;
		limitPin_Max_Secondary = configSetup.limit_Max_Z;
		limitPin_Min_Secondary = configSetup.limit_Min_Z;
		maxSpd_Secondary = configGreekKey.maxSpd_Z;
		accel_Secondary = configGreekKey.accel_Z;
		speedPercent_Secondary = configGreekKey.speedPercent_Z;
		currentSpeedPercent_Secondary = speedPercent_Secondary * .01;
		shortLegLength_Secondary = (int32_t)(((configGreekKey.segmentLength_Program / configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z)));
		SetEnable(ID_AXIS_Z, true, true);
		break;
	}
	case ID_AXIS_X:
	{
		stepPin_Secondary = PIN_AXIS_X_STEP;
		dirPin_Secondary = PIN_AXIS_X_DIR;
		limitPin_Max_Secondary = configSetup.limit_Max_X;
		limitPin_Min_Secondary = configSetup.limit_Min_X;
		maxSpd_Secondary = configGreekKey.maxSpd_X;
		accel_Secondary = configGreekKey.accel_X;
		speedPercent_Secondary = configGreekKey.speedPercent_X;
		currentSpeedPercent_Secondary = speedPercent_Secondary * .01;
		shortLegLength_Secondary = (int32_t)(((configGreekKey.segmentLength_Program / configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X)));

		SetEnable(ID_AXIS_X, true, true);
		break;
	}
	case ID_AXIS_3:
	{
		stepPin_Secondary = PIN_AXIS_3_STEP;
		dirPin_Secondary = PIN_AXIS_3_DIR;
		limitPin_Max_Secondary = configSetup.limit_Max_M3;
		limitPin_Min_Secondary = configSetup.limit_Min_M3;
		maxSpd_Secondary = configGreekKey.maxSpd_M3;
		accel_Secondary = configGreekKey.accel_M3;
		speedPercent_Secondary = configGreekKey.speedPercent_M3;
		currentSpeedPercent_Secondary = speedPercent_Secondary * .01;
		shortLegLength_Secondary = (int32_t)(((configGreekKey.segmentLength_Program / configSetup.distancePerRev_AxisM3) * (configSetup.steps360_Axis_M3 * configSetup.microsteps_Axis_M3)));

		SetEnable(ID_AXIS_3, true, true);
		break;
	}
	case ID_AXIS_4:
	{
		stepPin_Secondary = PIN_AXIS_4_STEP;
		dirPin_Secondary = PIN_AXIS_4_DIR;
		limitPin_Max_Secondary = configSetup.limit_Max_M4;
		limitPin_Min_Secondary = configSetup.limit_Min_M4;
		maxSpd_Secondary = configGreekKey.maxSpd_M4;
		accel_Secondary = configGreekKey.accel_M4;
		speedPercent_Secondary = configGreekKey.speedPercent_M4;
		currentSpeedPercent_Secondary = speedPercent_Secondary * .01;
		shortLegLength_Secondary = (int32_t)(((configGreekKey.segmentLength_Program / configSetup.distancePerRev_AxisM4) * (configSetup.steps360_Axis_M4 * configSetup.microsteps_Axis_M4)));

		SetEnable(ID_AXIS_4, true, true);
		break;
	}
	case ID_SPINDLE:
	{
		stepPin_Secondary = PIN_SPINDLE_STEP;
		dirPin_Secondary = PIN_SPINDLE_DIR;
		//limitPin_Max_Secondary = configSetup.limit_Max_M4;
		//limitPin_Min_Secondary = configSetup.limit_Min_M4;
		maxSpd_Secondary = configGreekKey.maxSpd_Spindle;
		accel_Secondary = configGreekKey.accel_Spindle;
		speedPercent_Secondary = configGreekKey.speedPercent_Spindle;
		currentSpeedPercent_Secondary = speedPercent_Secondary * .01;
		shortLegLength_Secondary = (int32_t)(((configGreekKey.segmentLength_Program / configSetup.gearRatio_Spindle) * (configSetup.steps360_Spindle * configSetup.microsteps_Spindle)));

		SetEnable(ID_SPINDLE, true, true);
		break;
	}
	}

	stepperId_1 = primaryAxisId;
	stepper1_step = stepPin_Primary;
	stepper1_dir = dirPin_Primary;

	stepperId_2 = secondaryAxisId;
	stepper2_step = stepPin_Secondary;
	stepper2_dir = dirPin_Secondary;

	// Set limit switches HIGH
	digitalWrite(limitPin_Max_Primary, HIGH);
	digitalWrite(limitPin_Min_Primary, HIGH);
	digitalWrite(limitPin_Max_Secondary, HIGH);
	digitalWrite(limitPin_Min_Secondary, HIGH);

	// ToDo:
	//AngularLengthAndMaxSpeed(maxSpd_Secondary, maxSpd_Primary);

	StepperGroup stepGroup;
	Stepper stepper_Primary(stepPin_Primary, dirPin_Primary);
	int32_t targetSteps_Primary = round((shortLegLength_Primary)*multiplier_Primary * direction);

	stepper_Primary.setPosition(0);
	stepper_Primary
		.setMaxSpeed(maxSpd_Primary * currentSpeedPercent_Primary)
		.setAcceleration(accel_Primary)
		.setTargetAbs(targetSteps_Primary);

	Stepper stepper_Secondary(stepPin_Secondary, dirPin_Secondary);
	int32_t targetSteps_Secondary = round((shortLegLength_Secondary)*multiplier_Secondary * direction);

	stepper_Secondary.setPosition(0);
	stepper_Secondary
		.setMaxSpeed(maxSpd_Secondary * currentSpeedPercent_Secondary)
		.setAcceleration(accel_Secondary)
		.setTargetAbs(targetSteps_Secondary);

	int32_t targetSteps_SynchroM3 = 0;


#ifdef VERBOSE
	Serial.println("-=-=-=-=-=-=-=-=-=-=-=-===-=-=-==-=-=-=-");
	Serial.print("direction: ");
	Serial.println(direction);
	Serial.print("shortLegLength_Primary: ");
	Serial.print(shortLegLength_Primary);
	Serial.print("   multiplier_Primary:  ");
	Serial.print(multiplier_Primary);
	Serial.print("   targetSteps_Primary: ");
	Serial.println(targetSteps_Primary);

	Serial.print("   maxSpd: ");
	Serial.println(maxSpd_Primary * currentSpeedPercent_Primary);

	Serial.print("shortLegLength_Secondary: ");
	Serial.print(shortLegLength_Secondary);
	Serial.print("   multiplier_Secondary:  ");
	Serial.print(multiplier_Secondary);
	Serial.print("    targetSteps_Secondary:  ");
	Serial.println(targetSteps_Secondary);
	Serial.print("   maxSpd: ");
	Serial.println(maxSpd_Secondary * currentSpeedPercent_Secondary);
	Serial.println("-=-=-=-=-=-=-=-=-=-=-=-===-=-=-==-=-=-=-");

#endif // VERBOSE
	checkedCount = 2;
	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle. M3 is radial
	{
		checkedCount = 3;
		stepperId_3 = ID_AXIS_3;
		stepper3_step = PIN_AXIS_3_STEP;
		stepper3_dir = PIN_AXIS_3_DIR;

		if (primaryAxisId == ID_SPINDLE)
		{
			targetSteps_SynchroM3 = targetSteps_Primary;
		}
		else if (secondaryAxisId == ID_SPINDLE)
		{
			targetSteps_SynchroM3 = targetSteps_Secondary;
		}
#ifdef VERBOSE
		Serial.print("targetSteps_SynchroM3 * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01):  ");
		Serial.println(targetSteps_SynchroM3 * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01));
		Serial.print("targetSteps_SynchroM3:  ");
		Serial.println(targetSteps_SynchroM3);

		Serial.print("synchro_Ratio:  ");
		Serial.println(synchro_Ratio);

		Serial.print("configMain.synchro_M3_Percentage * 0.01:  ");
		Serial.println(configMain.synchro_M3_Percentage * 0.01);
#endif // VERBOSE

		stepperAxis_M3.setPosition(0);
		stepperAxis_M3.setTargetAbs(targetSteps_SynchroM3 * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01));
		stepperAxis_M3.setMaxSpeed(configGreekKey.maxSpd_M3 * configGreekKey.speedPercent_M3 * .01);
		stepperAxis_M3.setAcceleration(configGreekKey.accel_M3);
		SetEnable(ID_AXIS_3, true, true);
		MilliDelay(5);
		StepperGroup stepGroup{ stepper_Secondary, stepper_Primary, stepperAxis_M3 };
		//stepGroup.moveAsync(stepper_Secondary, stepper_Primary, stepperAxis_M3);
	}
	else
	{
		StepperGroup stepGroup{ stepper_Secondary, stepper_Primary };
		//stepGroup.moveAsync(stepper_Secondary, stepper_Primary);
	}

	stepGroup.startMove();
	MilliDelay(500);

	while (stepper_Primary.isMoving)
	{
//		currentSpeed = stepGroup.getCurrentSpeed();
//#ifdef VERBOSE
//		//Serial.print("currentSpeed: ");
//		//Serial.println(currentSpeed);
//#endif // VERBOSE
//
//		// Teensy crashes when Spindle and M3 are syncronized
//		if (currentSpeed < MIN_SPEED)
//		{
//			MilliDelay(500);
//#ifdef DEBUG
//			Serial.println(">>>>>>>>>>>>>>>>MinSpeed Stop<<<<<<<<<<<<<<< ");
//#endif // Debug
//			stepGroup.stop();
//			goto endLoop;
//		}

		if (limitPin_Max_Primary > 9 && limitPin_Max_Primary < 40)
		{
			if (digitalRead(limitPin_Max_Primary) != configSetup.limit_NCorNO)
			{
#ifdef DEBUG
				Serial.println("                          limitPin");
#endif // DEBUG

				stepper_Primary.emergencyStop();
				stepper_Secondary.emergencyStop();
				stepperAxis_M3.emergencyStop();

				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, true);
				SetEnable(ID_AXIS_Z, false, true);
				SetEnable(ID_AXIS_X, false, true);
				SetEnable(ID_AXIS_3, false, true);
				SetEnable(ID_AXIS_4, false, true);
				SetLimitColors(PAGE_PROGRAM, DIR_CW);
				if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
				{
					digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
				}

				return true;
			}
		}

		if (limitPin_Min_Primary > 9 && limitPin_Min_Primary < 40)
		{
			if (digitalRead(limitPin_Min_Primary) != configSetup.limit_NCorNO)
			{
#ifdef DEBUG
				Serial.println("                          limitPin");
#endif // DEBUG

				stepper_Primary.emergencyStop();
				stepper_Secondary.emergencyStop();
				stepperAxis_M3.emergencyStop();

				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, true);
				SetEnable(ID_AXIS_Z, false, true);
				SetEnable(ID_AXIS_X, false, true);
				SetEnable(ID_AXIS_3, false, true);
				SetEnable(ID_AXIS_4, false, true);
				SetLimitColors(PAGE_PROGRAM, DIR_CCW);
				if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
				{
					digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
				}

				return true;
			}
		}


		if (limitPin_Max_Secondary > 9 && limitPin_Max_Secondary < 40)
		{
			if (digitalRead(limitPin_Max_Secondary) != configSetup.limit_NCorNO)
			{
#ifdef DEBUG
				Serial.println("                          limitPin");
#endif // DEBUG
				stepper_Primary.emergencyStop();
				stepper_Secondary.emergencyStop();
				stepperAxis_M3.emergencyStop();

				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, true);
				SetEnable(ID_AXIS_Z, false, true);
				SetEnable(ID_AXIS_X, false, true);
				SetEnable(ID_AXIS_3, false, true);
				SetEnable(ID_AXIS_4, false, true);
				SetLimitColors(PAGE_PROGRAM, DIR_CW);
				if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
				{
					digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
				}

				return true;
			}
		}

		if (limitPin_Min_Secondary > 9 && limitPin_Min_Secondary < 40)
		{
			if (digitalRead(limitPin_Min_Secondary) != configSetup.limit_NCorNO)
			{
#ifdef DEBUG
				Serial.println("                          limitPin");
#endif // DEBUG

				stepper_Primary.emergencyStop();
				stepper_Secondary.emergencyStop();
				stepperAxis_M3.emergencyStop();
				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, true);
				SetEnable(ID_AXIS_Z, false, true);
				SetEnable(ID_AXIS_X, false, true);
				SetEnable(ID_AXIS_3, false, true);
				SetEnable(ID_AXIS_4, false, true);
				SetLimitColors(PAGE_PROGRAM, DIR_CCW);
				if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
				{
					digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
				}

				return true;
			}
		}



#ifdef ESTOP_ENABLED
		if (configSetup.eStop > 9 && configSetup.eStop < 40)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
#ifdef DEBUG
				Serial.println("                          EStop");
#endif // DEBUG
				stepper_Primary.emergencyStop();
				stepper_Secondary.emergencyStop();
				stepperAxis_M3.emergencyStop();
				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEnable(ID_AXIS_4, false, false);
				SetEStopColors(PAGE_PROGRAM);
				if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
				{
					digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
				}

				return true;
			}
		}
#endif // estop
		if (StopGreekKey())
		{
#ifdef VERBOSE
			Serial.println("StopGreekKey() is true:");
#endif // VERBOSE
//			currentSpeed = stepGroup.getCurrentSpeed();
//#ifdef VERBOSE
//			Serial.print("currentSpeed: ");
//			Serial.println(currentSpeed);
//#endif // VERBOSE
//			if (currentSpeed < MIN_SPEED)
//			{
//				stepGroup.emergencyStop();
//			}
//			else
//			{
//				stepGroup.stop();
//			}

			stepper_Primary.stopAsync();
			stepper_Secondary.stopAsync();
			stepperAxis_M3.stop();
			MilliDelay(200);
			retVal = true;
			break;
		}
		MilliDelay(5);
	}
//endLoop:

	switch (primaryAxisId)
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
	case ID_AXIS_3:
	{
		SetEnable(ID_AXIS_3, false, true);
		break;
	}
	case ID_AXIS_4:
	{
		SetEnable(ID_AXIS_4, false, true);
		break;
	}
	case ID_SPINDLE:
	{
		SetEnable(ID_SPINDLE, false, true);
		break;
	}
	}

	switch (secondaryAxisId)
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
	case ID_AXIS_3:
	{
		SetEnable(ID_AXIS_3, false, true);
		break;
	}
	case ID_AXIS_4:
	{
		SetEnable(ID_AXIS_4, false, true);
		break;
	}
	case ID_SPINDLE:
	{
		SetEnable(ID_SPINDLE, false, true);
		break;
	}
	}
	endPosition_Axis = endPosition_Axis + stepper_Primary.getPosition();
	//endPosition_Spindle = endPosition_Spindle + stepperSpindle.getPosition();
	//cumulative_Spindle = cumulative_Spindle + stepperSpindle.getPosition();
	cumulative_Axis = cumulative_Axis + stepper_Primary.getPosition();

	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		SetEnable(ID_AXIS_3, false, true);
		endPosition_M3 = endPosition_M3 + stepperAxis_M3.getPosition();
		cumulative_M3 = cumulative_M3 + stepperAxis_M3.getPosition();
	}

#ifdef VERBOSE
	Serial.print("endPosition_Axis:");
	Serial.println(endPosition_Axis);
	Serial.print("endPosition_Spindle:");
	Serial.println(endPosition_Spindle);
	Serial.print("endPosition_M3:");
	Serial.println(endPosition_M3);
	Serial.println("Exit GreekKey_Move_Angular_TeensyStep............................. ");
#endif // VERBOSE

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
#ifdef VERBOSE
	Serial.println("Enter GreekKey_Move_Spindle.......................");
#endif // VERBOSE

	bool retVal = false;

	SetEnable(ID_SPINDLE, true, true);
	StepperGroup stepGroup{ stepperSpindle,stepperAxis_M3 };

	stepperId_1 = ID_SPINDLE;
	stepper1_step = PIN_SPINDLE_STEP;
	stepper1_dir = PIN_SPINDLE_DIR;
	//Stepper stepper_1(stepper1_step, stepper1_dir);
	int32_t spindleMxSpeed = configGreekKey.maxSpd_Spindle * (configGreekKey.speedPercent_Spindle * .01);// *direction);

	stepperId_3 = ID_AXIS_3;
	stepper3_step = PIN_AXIS_3_STEP;
	stepper3_dir = PIN_AXIS_3_DIR;
	//Stepper stepper_3(stepper3_step, stepper3_dir);


	stepperAxis_M3.setPosition(0);
	//stepper_1.setPosition(0);
	stepperSpindle.setPosition(0);


	float targetSteps = segmentSteps * multiplier * direction;
	int32_t targetStepsRounded = round(targetSteps);
	float targetSteps_M3Synchro = round(targetStepsRounded * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01));
	int32_t targetSteps_M3Synchro_Rounded = round(targetSteps_M3Synchro);
	int32_t m3MxSpeed = configGreekKey.maxSpd_M3 * (configGreekKey.speedPercent_M3 * .01);// *direction;


	// Set direction in Speed instead of Position
	if (targetSteps < 0)
	{
		targetSteps = targetSteps * -1;
		spindleMxSpeed = spindleMxSpeed * -1;
		targetSteps_M3Synchro_Rounded = targetSteps_M3Synchro_Rounded * -1;
		m3MxSpeed = m3MxSpeed * -1;
	}

	stepperSpindle.setAcceleration(configGreekKey.accel_Spindle);
	stepperSpindle.setMaxSpeed(spindleMxSpeed);
#ifdef VERBOSE
	Serial.print("targetSteps-float:    ");
	Serial.println(targetSteps);
	Serial.print("targetStepsRounded:   ");
	Serial.println(targetStepsRounded);
	Serial.print("targetSteps:          ");
	Serial.println(targetSteps);
	Serial.print("shortLegLength:       ");
	Serial.println(segmentSteps);
	Serial.print("multiplier:           ");
	Serial.println(multiplier);
	Serial.print("direction:            ");
	Serial.println(direction);
	Serial.print("maxSpd_Axis_M3:       ");
	Serial.println(configGreekKey.maxSpd_M3);
	Serial.print("speedPercent_Axis_M3: ");
	Serial.println(configGreekKey.speedPercent_M3);
	Serial.print("m3MxSpeed:            ");
	Serial.println(m3MxSpeed);
#endif // VERBOSE

	stepperSpindle.setTargetAbs(targetStepsRounded);

	checkedCount = 2;
#ifdef VERBOSE
	Serial.print("configMain.synchro_M3_Spindle:      ");
	Serial.println(configMain.synchro_M3_Spindle);
#endif // VERBOSE

	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		checkedCount = 3;
		stepperId_3 = ID_AXIS_3;
#ifdef VERBOSE
		Serial.print("targetSteps_M3Synchro;          ");
		Serial.println(targetSteps_M3Synchro);
		Serial.print("targetSteps_M3Synchro_Rounded:  ");
		Serial.println(targetSteps_M3Synchro_Rounded);

		Serial.print("synchro_Ratio:                  ");
		Serial.println(synchro_Ratio);

		Serial.print("synchro_M3_Percentage:          ");
		Serial.println(configMain.synchro_M3_Percentage * 0.01);
#endif // VERBOSE

		stepperAxis_M3.setPosition(0);
		MilliDelay(5);
		stepperAxis_M3.setTargetAbs(targetSteps_M3Synchro_Rounded);
		stepperAxis_M3.setMaxSpeed(m3MxSpeed);
		stepperAxis_M3.setAcceleration(configGreekKey.accel_M3);
		SetEnable(ID_AXIS_3, true, true);

		MilliDelay(5);
#ifdef VERBOSE
		Serial.println("===========================================================");
		Serial.println("moveAsync(stepper_1, stepperAxis_M3)");
		Serial.print("targetSteps_M3Synchro_Rounded:  ");
		Serial.println(targetSteps_M3Synchro_Rounded);
		Serial.print("m3MxSpeed:  ");
		Serial.println(m3MxSpeed);
		Serial.print("configGreekKey.accel_Axis_M3:  ");
		Serial.println(configGreekKey.accel_M3);

		Serial.print("Spindle targetStepsRounded:  ");
		Serial.println(targetStepsRounded);
		Serial.print("spindleMxSpeed:  ");
		Serial.println(spindleMxSpeed);
		Serial.print("configGreekKey.accel_Spindle:  ");
		Serial.println(configGreekKey.accel_Spindle);
		Serial.println("===========================================================");
#endif // VERBOSE

		//StepperGroup stepGroup{ stepper_1, stepperAxis_M3 };
		//stepGroup.moveAsync(stepper_1, stepperAxis_M3);
	}
	else
	{
		stepperAxis_M3.setPosition(0);
		MilliDelay(5);
		stepperAxis_M3.setTargetAbs(0);
		stepperAxis_M3.setMaxSpeed(m3MxSpeed);
		stepperAxis_M3.setAcceleration(configGreekKey.accel_M3);
		SetEnable(ID_AXIS_3, true, true);
	}

	stepGroup.startMove();
	int iCounter = 0;
	MilliDelay(500);

	while (stepperSpindle.isMoving)
	{
#ifdef VERBOSE
		//Serial.print("iCounter: ");
		//Serial.print(iCounter);
		//Serial.print(" Spindle position: ");
		//Serial.print(stepper_1.getPosition());
		//Serial.print(" -- current speed: ");
		//Serial.println(stepControllerSpindle.getCurrentSpeed());
		//Serial.print("M3 position: ");
		//Serial.println(stepperAxis_M3.getPosition());
		MilliDelay(10);
		iCounter++;
#endif // DEBUG


//		currentSpeed = stepGroup.getCurrentSpeed();
//#ifdef VERBOSE
//		//Serial.print("currentSpeed: ");
//		//Serial.println(currentSpeed);
//#endif // VERBOSE
//		// Teensy crashes when Spindle and M3 are syncronized
//		if (currentSpeed < MIN_SPEED)
//		{
//			MilliDelay(500);
//#ifdef VERBOSE
//			//Serial.println(">>>>>>>>>>>>>>>>MinSpeed Stop<<<<<<<<<<<<<<< ");
//#endif // VERBOSE
//			stepGroup.stop();
//			goto endLoop;
//		}

#ifdef ESTOP_ENABLED
		//if (configSetup.eStop != 0)
		if (configSetup.eStop > 9 && configSetup.eStop < 40)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
#ifdef DEBUG
				Serial.println("                          EStop");
#endif // DEBUG
				stepperSpindle.emergencyStop();
				stepperAxis_M3.emergencyStop();

				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEStopColors(PAGE_GRK);
				if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
				{
					digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
				}
				return true;
			}
		}
#endif // estop
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
#ifdef VERBOSE
			//		Serial.print("incomingByte: ");
				//	Serial.println(incomingByte);
#endif // VERBOSE

			switch (incomingByte)
			{
			case 83: // - S
			{
//				currentSpeed = stepGroup.getCurrentSpeed();
//#ifdef VERBOSE
//				Serial.print("currentSpeed: ");
//				Serial.println(currentSpeed);
//#endif // VERBOSE
//				if (currentSpeed < MIN_SPEED)
//				{
//					stepGroup.stopAsync();
//				}

				stepperSpindle.stopAsync();
				stepperAxis_M3.stop();
				MilliDelay(200);
				SetEnable(ID_SPINDLE, false, true);
				SetEnable(ID_AXIS_3, false, true);
				retVal = true;
				break;
			}
			}
		}

		MilliDelay(5);
	}

//endLoop:
#ifdef VERBOSE

	Serial.print("endPosition_Spindle:");
	Serial.println(endPosition_Spindle);
#endif // VERBOSE

	endPosition_Spindle = endPosition_Spindle + stepperSpindle.getPosition();
	cumulative_Spindle = cumulative_Spindle + stepperSpindle.getPosition();

	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		SetEnable(ID_AXIS_3, false, true);
#ifdef VERBOSE
		Serial.print("endPosition_M3:");
		Serial.println(endPosition_M3);
#endif // VERBOSE
		endPosition_M3 = endPosition_M3 + stepperAxis_M3.getPosition();
		cumulative_M3 = cumulative_M3 + stepperAxis_M3.getPosition();
	}

#ifdef DEBUG
	Serial.print("M3 position: ");
	Serial.println(stepperAxis_M3.getPosition());

	Serial.print("endPosition_Spindle +:");
	Serial.println(endPosition_Spindle);
	Serial.print("endPosition_M3:");
	Serial.println(endPosition_M3);
	Serial.println("Exit GreekKey_Move_Spindle.......................");
#endif // DEBUG
	stepperSpindle.setPosition(0);
	stepperAxis_M3.setPosition(0);
	return retVal;
}

/// <summary>
/// Program_FromFile
/// </summary>
/// <comment>
/// Move spindle and axis independently from values stored in file
/// </comment>
/// <returns></returns>
void Program_FromFile(int direction)
{
	limitTriggered = false;
	eStopTriggered = false;

	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);

	int eePromAddress = 0;

#ifdef VERBOSE
	Serial.println("Enter GreekKeyFromFile");
#endif // VERBOSE
	bool stopAll = false;
	bool exitInnerForLoop = false;

	int lineCounter = 0;
	int fileLineCount = 500;
	float segmentMultiplier = 0;

	int shortSegmentStepsAxis = 0;
	int shortSegmentStepsAxisZ = 0;
	int shortSegmentStepsAxisX = 0;
	int shortSegmentStepsAxisM3 = 0;
	int shortSegmentStepsAxisM4 = 0;
	int axisSteps = 0;

	int spindleShortLegSteps = 0;
	int spindleSegmentCount = 0;
	int spindleSteps = 0;

	int m3SegmentCount = 0;
	int m4SegmentCount = 0;

	float stepsPer360 = 0;
	float patternDivisor = 0;

	int selectedAxis = 0;

	// Reset start and end positions
	startPositionAbs_Axis = 0;

	endPosition_Spindle = 0;
	endPosition_M3 = 0;
	endPosition_M4 = 0;
	endPosition_Axis = 0;

	cumulative_Spindle = 0;
	cumulative_Axis = 0;
	cumulative_M3 = 0;
	cumulative_M4 = 0;
	cumulative_Z = 0;
	cumulative_X = 0;	
	
	// Reset stopSteppers
	stopSteppers = false;

	// String variables
	//const char* pageBE_t3 = "pageBE.t3.txt="; // Spindle Begin
	//const char* pageBE_t1 = "pageBE.t1.txt="; // Spindle End
	//const char* pageBE_t4 = "pageBE.t3.txt="; // Axis Begin
	//const char* pageBE_t2 = "pageBE.t1.txt="; // Axis End
	const char* pageProgram_t15 = "pageProgram.t15.txt="; // Segments
	const char* pageProgram_t21 = "pageProgram.t21.txt="; // Direction
	const char* pageProgram_t22 = "pageProgram.t22.txt="; // Line Number

#ifdef VERBOSE
	Serial.print(">>>>>>>>>>>>>>>>>>>>segmentOrActual:");
	Serial.println(configGreekKey.segmentOrActual);
#endif // VERBOSE

	// Get Spindle segment count and line count from file :
	badFilename = false;
	badCommand = false;
	if (configGreekKey.segmentOrActual == 2) // 2: Segment 
	{
		for (lineCounter = 0; lineCounter <= fileLineCount; lineCounter++)
		{
			segmentMultiplier = GetProgramDataFromSD(lineCounter);
#ifdef VERBOSE
			Serial.print(segmentMultiplier);
			Serial.print("LineCounter:  ");
			Serial.println(lineCounter);
#endif // VERBOSE

			if (badFilename)
			{
#ifdef VERBOSE
				Serial.println("Exit Greek Key: File not found");
#endif // VERBOSE
				SerialPrint("pageProgram.tFileName.txt=");
				SerialWrite(0x22);
				SerialPrint("File not found");
				SerialPrint(nextionQuoteEnd);

				goto EndLoops;
			}

			if (badCommand)
			{
#ifdef VERBOSE
				Serial.println("Exit Greek Key: Bad Command");
#endif // VERBOSE
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

#ifdef VERBOSE
			//Serial.print(">>>segmentMultiplier:");
			//Serial.println(segmentMultiplier);
#endif // VERBOSE

			switch (moveType)
			{
				case 1: // WS Spindle segment count (Axial and Spindle segments reverse when Axial is selected.)
				{
					if (configGreekKey.radialOrAxial_File == RADIAL)
					{
						spindleSegmentCount = segmentMultiplier;
					}
					break;
				}
				case 2: // WA Axis segment count (Axial and Spindle segments reverse when Axial is selected.)
				{
					if (configGreekKey.radialOrAxial_File == AXIAL)
					{
						spindleSegmentCount = segmentMultiplier;
					}
					break;
				}
				case 3: // WB M3 radial segment count (Axial and Spindle segments reverse when Axial is selected.)
				{
					if (configGreekKey.radialOrAxial_File == RADIAL)
					{
						m3SegmentCount = segmentMultiplier;

					}
	#ifdef DEBUG
					Serial.print("m3SegmentCount: ");
					Serial.println(m3SegmentCount);
	#endif // DEBUG
					break;
				}
				case 4: // WD  M4 radial segment count (Axial and Spindle segments reverse when Axial is selected.)
				{
					if (configGreekKey.radialOrAxial_File == RADIAL)
					{
						m4SegmentCount = segmentMultiplier;
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

		int stepsPerRevolution_Spindle = (int)((configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle));
		patternDivisor = ((float)configGreekKey.countPatternPer360_Program * (float)spindleSegmentCount);
		spindleShortLegSteps = (int)(((stepsPerRevolution_Spindle)) / (patternDivisor));

#ifdef VERBOSE

		Serial.print("lineCounter:");
		Serial.println(lineCounter);
		Serial.print("patternDivisor:");
		Serial.println(patternDivisor);
		Serial.print("stepsPerRevolution_Spindle:");
		Serial.println(stepsPerRevolution_Spindle);
		Serial.print("patternDivisor:");
		Serial.println(patternDivisor);
		Serial.print("spindleSegmentCount:");
		Serial.println(spindleSegmentCount);
		Serial.print("spindleShortLegSteps:");
		Serial.println(spindleShortLegSteps);
#endif // VERBOSE
	}

	// Calculate axis steps
	shortSegmentStepsAxisZ = (int)(((configGreekKey.segmentLength_Program / configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z)));
#ifdef VERBOSE
	Serial.print("configGreekKey.segmentLength_File:");
	Serial.println(configGreekKey.segmentLength_Program);

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

#endif // VERBOSE
	if (configSetup.xAltX == 0)
	{
		shortSegmentStepsAxisX = (int)(((configGreekKey.segmentLength_Program / configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X)));
	}
	else
	{
		shortSegmentStepsAxisX = (int)(((configGreekKey.segmentLength_Program / configSetup.distancePerRev_AxisXAlt) * (configSetup.steps360_Axis_XAlt * configSetup.microsteps_Axis_XAlt)));
	}

	switch (configSetup.radialOrLinear_Axis_M3)
	{
		case RADIAL_M3:
		{
	#ifdef DEBUG
			Serial.print("m3SegmentCount: ");
			Serial.println(m3SegmentCount);
	#endif // DEBUG
			if (m3SegmentCount == 0)
			{
				shortSegmentStepsAxisM3 = (int)((DistanceToSteps_M3Radial(configGreekKey.segmentLength_Program)));
			}
			else
			{
				stepsPer360 = configSetup.steps360_Axis_M3 * configSetup.microsteps_Axis_M3 * configSetup.gearRatio_AxisM3;
				patternDivisor = ((float)configGreekKey.countPatternPer360_Program * (float)m3SegmentCount);
				m3Radial_ShortLegSteps = stepsPer360 / patternDivisor;
				shortSegmentStepsAxisM3 = m3Radial_ShortLegSteps;
	#ifdef DEBUG
				Serial.print("m3Radial_ShortLegSteps: ");
				Serial.println(m3Radial_ShortLegSteps);
	#endif // DEBUG
			}

			break;
		}
		case LINEAR_M3:
		{
			shortSegmentStepsAxisM3 = (int)((DistanceToSteps_M3Linear(configGreekKey.segmentLength_Program)));
			break;
		}
	
	}

	switch (configSetup.radialOrLinear_Axis_M4)
	{
		case RADIAL_M4:
		{
			if (m4SegmentCount == 0)
			{
				shortSegmentStepsAxisM4 = (int)((DistanceToSteps_M4Radial(configGreekKey.segmentLength_Program)));
			}
			else
			{
				stepsPer360 = configSetup.steps360_Axis_M4 * configSetup.microsteps_Axis_M4 * configSetup.gearRatio_AxisM4;
				patternDivisor = ((float)configGreekKey.countPatternPer360_Program * (float)m4SegmentCount);
				m4Radial_ShortLegSteps = stepsPer360 / patternDivisor;
				shortSegmentStepsAxisM4 = m4Radial_ShortLegSteps;
			}
			break;
		}
		case LINEAR_M4:
		{
			shortSegmentStepsAxisM4 = (int)((DistanceToSteps_M4Linear(configGreekKey.segmentLength_Program)));
			break;
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
#ifdef VERBOSE
		Serial.print("configGreekKey_Main.patternCount_File:");
		Serial.println(configGreekKey.patternCount_Program);
		Serial.print("segmentLength_File:");
		Serial.println(configGreekKey.segmentLength_Program);
		Serial.print("configSetup.distancePerRev_AxisZ:");
		Serial.println(configSetup.distancePerRev_AxisZ);
		Serial.print("configSetup.steps360_Axis_Z:");
		Serial.println(configSetup.steps360_Axis_Z);
		Serial.print("configSetup.microsteps_Axis_Z:");
		Serial.println(configSetup.microsteps_Axis_Z);
		Serial.print("shortSegmentStepsAxisZ:");
		Serial.println(shortSegmentStepsAxisZ);
#endif // VERBOSE
		break;
	}
	case ID_AXIS_X: // X Axis
	{
		shortSegmentStepsAxis = shortSegmentStepsAxisX;
#ifdef VERBOSE
		Serial.print("configGreekKey_Main.patternCount_File:");
		Serial.println(configGreekKey.patternCount_Program);
		Serial.print("segmentLength_File:");
		Serial.println(configGreekKey.segmentLength_Program);
		Serial.print("configSetup.distancePerRev_AxisX:");
		Serial.println(configSetup.distancePerRev_AxisX);
		Serial.print("configSetup.steps360_Axis_X:");
		Serial.println(configSetup.steps360_Axis_X);
		Serial.print("configSetup.microsteps_Axis_X:");
		Serial.println(configSetup.microsteps_Axis_X);
		Serial.print("shortSegmentStepsAxisX:");
		Serial.println(shortSegmentStepsAxisX);
#endif // VERBOSE
		break;
	}
	case ID_AXIS_3: // M3 Axis
	{
		shortSegmentStepsAxis = shortSegmentStepsAxisM3;
#ifdef VERBOSE
		Serial.print("configGreekKey_Main.patternCount_File:");
		Serial.println(configGreekKey.patternCount_Program);
		Serial.print("segmentLength_File:");
		Serial.println(configGreekKey.segmentLength_Program);
		Serial.print("configSetup.distancePerRev_AxisM3:");
		Serial.println(configSetup.distancePerRev_AxisM3);
		Serial.print("configSetup.steps360_Axis_M3:");
		Serial.println(configSetup.steps360_Axis_M3);
		Serial.print("configSetup.microsteps_Axis_M3:");
		Serial.println(configSetup.microsteps_Axis_M3);
		Serial.print("shortSegmentStepsAxisM3:");
		Serial.println(shortSegmentStepsAxisM3);
#endif // VERBOSE
		break;
	}
	case ID_AXIS_4: // M4 Axis
	{
		shortSegmentStepsAxis = shortSegmentStepsAxisM4;
#ifdef VERBOSE
		Serial.print("configGreekKey_Main.patternCount_File:");
		Serial.println(configGreekKey.patternCount_Program);
		Serial.print("segmentLength_File:");
		Serial.println(configGreekKey.segmentLength_Program);
		Serial.print("configSetup.distancePerRev_AxisM4:");
		Serial.println(configSetup.distancePerRev_AxisM4);
		Serial.print("configSetup.steps360_Axis_M4:");
		Serial.println(configSetup.steps360_Axis_M4);
		Serial.print("configSetup.microsteps_Axis_M4:");
		Serial.println(configSetup.microsteps_Axis_M4);
		Serial.print("shortSegmentStepsAxisM4:");
		Serial.println(shortSegmentStepsAxisM4);
#endif // VERBOSE
		break;
	}
	}

	for (uint32_t j = 1; j <= configGreekKey.patternCount_Program; j++)
	{
		exitInnerForLoop = false;

		SerialPrint("t200.txt=");
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
			segmentMultiplier = GetProgramDataFromSD(i);
#ifdef VERBOSE
			Serial.print(">>>>>badCommand Value:  ");
			Serial.println(badCommand);
#endif // VERBOSE
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

			//int moveDirection = DIR_CW * direction;
			//int moveDirection_H = DIR_CW * direction;;
			//int moveDirection_V = DIR_CW * direction;;

			int moveDirection = direction;
			int moveDirection_H = direction;
			int moveDirection_V = direction;
#ifdef VERBOSE
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
#endif // VERBOSE

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
#ifdef SHOW_POSITION
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
#endif // SHOW_POSITION
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
#ifdef VERBOSE
						Serial.print("----shortSegmentStepsAxis--------------------------------------------------");
						Serial.println(shortSegmentStepsAxis);
#endif // VERBOSE
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
						case ID_AXIS_3: // M3 Axis
						{
							switch (configSetup.radialOrLinear_Axis_M3)
							{
							case RADIAL_M3:
							{
								axisSteps = DistanceToSteps_M3Radial(segmentMultiplier);
								break;
							}
							case LINEAR_M3:
							{
								axisSteps = DistanceToSteps_M3Linear(segmentMultiplier);
								break;
							}
							}
							break;
						}
						case ID_AXIS_4: // M4 Axis
						{
							switch (configSetup.radialOrLinear_Axis_M4)
							{
							case RADIAL_M4:
							{
								axisSteps = DistanceToSteps_M4Radial(segmentMultiplier);
								break;
							}
							case LINEAR_M4:
							{
								axisSteps = DistanceToSteps_M4Linear(segmentMultiplier);
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
			case 66: // B - M3 axis
			{
				selectedAxis = configGreekKey.axisId;
				configGreekKey.axisId = ID_AXIS_3;


#ifdef DEBUG
				Serial.print("segmentMultiplier:        ");
				Serial.println(segmentMultiplier);
				Serial.print("shortSegmentStepsAxisM3:  ");
				Serial.println(shortSegmentStepsAxisM3);
				Serial.print("moveDirection:            ");
				Serial.println(moveDirection);
				Serial.print("m3Radial_ShortLegSteps:   ");
				Serial.println(m3Radial_ShortLegSteps);
#endif // DEBUG
				if (configGreekKey.segmentOrActual == 2) // 2: Segment  
				{
					if (m3SegmentCount == 0)
					{
						stopAll = GreekKey_Move_Axis(shortSegmentStepsAxisM3, segmentMultiplier, moveDirection, true);
					}
					else
					{
						switch (configSetup.radialOrLinear_Axis_M3)
						{
						case RADIAL_M3:
						{
							stopAll = GreekKey_Move_Axis(m3Radial_ShortLegSteps, segmentMultiplier, moveDirection, true);
							break;
						}
						case LINEAR_M3:
						{
							stopAll = GreekKey_Move_Axis(shortSegmentStepsAxisM3, segmentMultiplier, moveDirection, true);
							break;
						}
						}
					}
				}
				else  //3: Actual
				{
					switch (configSetup.radialOrLinear_Axis_M3)
					{
					case RADIAL_M3:
					{
						axisSteps = DistanceToSteps_M3Radial(segmentMultiplier);
						break;
					}
					case LINEAR_M3:
					{
						axisSteps = DistanceToSteps_M3Linear(segmentMultiplier);
						break;
					}
					}

					// Negative axisSteps will move CCW
					stopAll = GreekKey_Move_Axis(axisSteps, 1, moveDirection, true);
				}

				configGreekKey.axisId = selectedAxis;
				if (stopAll)
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
						case ID_AXIS_3: // M3 Axis
						{
							switch (configSetup.radialOrLinear_Axis_M3)
							{
							case RADIAL_M3:
							{
								axisSteps = round(DistanceToSteps_M3Radial(segmentMultiplier));
							}
							case LINEAR_M3:
							{
								axisSteps = round(DistanceToSteps_M3Linear(segmentMultiplier));
							}
							}

							break;
						}
						case ID_AXIS_4: // M4 Axis
						{
							switch (configSetup.radialOrLinear_Axis_M4)
							{
							case RADIAL_M4:
							{
								axisSteps = round(DistanceToSteps_M4Radial(segmentMultiplier));
							}
							case LINEAR_M4:
							{
								axisSteps = round(DistanceToSteps_M4Linear(segmentMultiplier));
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
			case 68: // D - M4 axis
			{
				selectedAxis = configGreekKey.axisId;
				configGreekKey.axisId = ID_AXIS_4;
				if (configGreekKey.segmentOrActual == 2) // 2: Segment  
				{
					if (m4SegmentCount == 0)
					{
						stopAll = GreekKey_Move_Axis(shortSegmentStepsAxisM4, segmentMultiplier, moveDirection, true);
					}
					else
					{
						switch (configSetup.radialOrLinear_Axis_M4)
						{
						case RADIAL_M4:
						{
							stopAll = GreekKey_Move_Axis(m4Radial_ShortLegSteps, segmentMultiplier, moveDirection, true);
							break;
						}
						case LINEAR_M4:
						{
							stopAll = GreekKey_Move_Axis(shortSegmentStepsAxisM4, segmentMultiplier, moveDirection, true);
							break;
						}
						}
					}
				}
				else  //3: Actual
				{
					switch (configSetup.radialOrLinear_Axis_M4)
					{
					case RADIAL_M3:
					{
						axisSteps = DistanceToSteps_M4Radial(segmentMultiplier);
						break;
					}
					case LINEAR_M3:
					{
						axisSteps = DistanceToSteps_M4Linear(segmentMultiplier);
						break;
					}
					}

					// Negative axisSteps will move CCW
					stopAll = GreekKey_Move_Axis(axisSteps, 1, moveDirection, true);
				}

				configGreekKey.axisId = selectedAxis;
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
					case ID_MOVE_AXIS_M3_1:
					{
						configMove.distance_MoveM3_1 = segmentMultiplier;
						eePromAddress = 332;
						break;
					}
					case ID_MOVE_AXIS_M3_2:
					{
						configMove.distance_MoveM3_2 = segmentMultiplier;
						eePromAddress = 500;
						break;
					}

					case ID_MOVE_AXIS_M4_1:
					{
						configMove.distance_MoveM4_1 = segmentMultiplier;
						eePromAddress = 12;
						break;
					}
					case ID_MOVE_AXIS_M4_2:
					{
						configMove.distance_MoveM4_2 = segmentMultiplier;
						eePromAddress = 36;
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

#ifdef VERBOSE
					Serial.print("wepo ");
					Serial.write(0x22);
					Serial.print(segmentMultiplier);
					Serial.write(0x22);
					Serial.print(",");
					Serial.print(eePromAddress);
					Serial.println(nextionEnd);
#endif // VERBOSE
					const char* pageMove_t10 = "pageMove.t10.txt=";
					SerialPrint(pageMove_t10);
					SerialPrint("\x22");
					SerialPrint(segmentMultiplier);
					SerialPrint(nextionQuoteEnd);

#ifdef VERBOSE
					Serial.print("Move Distance: ");
					Serial.print(segmentMultiplier);
					Serial.print("      eeprom:  ");
					Serial.println(eePromAddress);
#endif // VERBOSE
				}
				else if (commandMove == "F") // Run Move page
				{
					//if (segmentMultiplier * moveDirection < 0)
					if (segmentMultiplier < 0)
					{
#ifdef DEBUG
						Serial.println("MoveAxis(configMove.axisId, DIR_CCW)");
						Serial.print("segmentMultiplier:");
						Serial.println(segmentMultiplier);
						Serial.print("moveDirection:");
						Serial.println(moveDirection);
#endif //DEBUG
						MoveAxis(configMove.axisId, DIR_CCW);
					}
					else
					{
#ifdef DEBUG
						Serial.println("MoveAxis(configMove.axisId, DIR_CW)");
						Serial.print("segmentMultiplier:");
						Serial.println(segmentMultiplier);
						Serial.print("moveDirection:");
						Serial.println(moveDirection);
#endif //DEBUG
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
					//if (segmentMultiplier * moveDirection < 0) // Move in (towards headstock).
					if (segmentMultiplier < 0)
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
#ifdef VERBOSE
							Serial.print("shortSegmentStepsAxisZ: ");
							Serial.print(shortSegmentStepsAxisZ);
							Serial.print("   hv_AxisLegLength: ");
							Serial.print(abs(hv_AxisLegLength));
							Serial.print("   Axis moveDirection_H: ");
							Serial.println(moveDirection_H);

							Serial.print("spindleShortLegSteps: ");
							Serial.print(spindleShortLegSteps);
							Serial.print("   hv_SpindleLegLength: ");
							Serial.print(abs(hv_SpindleLegLength));
							Serial.print("   moveDirection_V: ");
							Serial.println(moveDirection_V);
#endif // VERBOSE
							stopAll = GreekKey_Angular_AxisAndSpindle((shortSegmentStepsAxisZ), (hv_AxisLegLength)*moveDirection_H, (spindleShortLegSteps), (hv_SpindleLegLength)*moveDirection_V);
							//stopAll = GreekKey_Angular_AxisAndSpindle((shortSegmentStepsAxisZ), (hv_AxisLegLength)*moveDirection_H, (spindleShortLegSteps), (hv_SpindleLegLength)*moveDirection_V);
							break;
						}
						case ID_AXIS_X: // X Axis is primary
						{
#ifdef VERBOSE
							Serial.print("shortSegmentStepsAxisX: ");
							Serial.print(shortSegmentStepsAxisX);
							Serial.print("   hv_AxisLegLength: ");
							Serial.print(abs(hv_AxisLegLength));
							Serial.print("   Axis moveDirection_H: ");
							Serial.println(moveDirection_H);

							Serial.print("spindleShortLegSteps: ");
							Serial.print(spindleShortLegSteps);
							Serial.print("   hv_SpindleLegLength: ");
							Serial.print(abs(hv_SpindleLegLength));
							Serial.print("   moveDirection_V: ");
							Serial.println(moveDirection_V);
#endif // VERBOSE

							stopAll = GreekKey_Angular_AxisAndSpindle(shortSegmentStepsAxisX, (hv_AxisLegLength)*moveDirection_H, spindleShortLegSteps, (hv_SpindleLegLength)*moveDirection_V);
							break;
						}
						case ID_AXIS_3: // M3 Axis is primary
						{
							stopAll = GreekKey_Angular_AxisAndSpindle(shortSegmentStepsAxisM3, (hv_AxisLegLength)*moveDirection_H, spindleShortLegSteps, (hv_SpindleLegLength)*moveDirection_V);
							break;
						}
						case ID_AXIS_4: // M4 Axis is primary
						{
							stopAll = GreekKey_Angular_AxisAndSpindle(shortSegmentStepsAxisM4, (hv_AxisLegLength)*moveDirection_H, spindleShortLegSteps, (hv_SpindleLegLength)*moveDirection_V);
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
						case ID_AXIS_3: // M3 Axis
						{
							switch (configSetup.radialOrLinear_Axis_M3)
							{
							case RADIAL_M3:
							{
								axisSteps = (int)(DistanceToSteps_M3Radial(hv_AxisLegLength));
								break;
							}
							case LINEAR_M3:
							{
								axisSteps = (int)(DistanceToSteps_M3Linear(hv_AxisLegLength));
								break;
							}
							}
							break;
						}
						case ID_AXIS_4: // M4 Axis
						{
							switch (configSetup.radialOrLinear_Axis_M4)
							{
							case RADIAL_M4:
							{
								axisSteps = (int)(DistanceToSteps_M4Radial(hv_AxisLegLength));
								break;
							}
							case LINEAR_M4:
							{
								axisSteps = (int)(DistanceToSteps_M4Linear(hv_AxisLegLength));
								break;
							}
							}
							break;
						}

						}

						axisSteps = axisSteps * direction;
						spindleSteps = round((configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (hv_SpindleLegLength / 360));
						stopAll = GreekKey_Angular_AxisAndSpindle(axisSteps, 1, spindleSteps, 1);
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
#ifdef VERBOSE
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
#endif // VERBOSE
							stopAll = GreekKey_Angular_AxisAndSpindle(shortSegmentStepsAxisZ, (hv_SpindleLegLength)*moveDirection_V, (spindleShortLegSteps), (hv_AxisLegLength)*moveDirection_H);
							break;
						}
						case ID_AXIS_X: // 
						{
							stopAll = GreekKey_Angular_AxisAndSpindle(shortSegmentStepsAxisX, hv_SpindleLegLength * direction, spindleShortLegSteps, hv_AxisLegLength);
							break;
						}
						case ID_AXIS_3: // 
						{
							stopAll = GreekKey_Angular_AxisAndSpindle(shortSegmentStepsAxisM3, hv_SpindleLegLength * direction, spindleShortLegSteps, hv_AxisLegLength);
							break;
						}
						case ID_AXIS_4: // 
						{
							stopAll = GreekKey_Angular_AxisAndSpindle(shortSegmentStepsAxisM4, hv_SpindleLegLength * direction, spindleShortLegSteps, hv_AxisLegLength);
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
						case ID_AXIS_3: // M3 Axis
						{
							switch (configSetup.radialOrLinear_Axis_M3)
							{
							case RADIAL_M3:
							{
								axisSteps = DistanceToSteps_M3Radial(hv_SpindleLegLength);
								break;
							}
							case LINEAR_M3:
							{
								axisSteps = DistanceToSteps_M3Linear(hv_SpindleLegLength);
								break;
							}
							}
							break;
						}
						case ID_AXIS_4: // M4 Axis
						{
							switch (configSetup.radialOrLinear_Axis_M4)
							{
							case RADIAL_M4:
							{
								axisSteps = DistanceToSteps_M4Radial(hv_SpindleLegLength);
								break;
							}
							case LINEAR_M3:
							{
								axisSteps = DistanceToSteps_M4Linear(hv_SpindleLegLength);
								break;
							}
							}
							break;
						}
						}

						axisSteps = axisSteps * direction;
						spindleSteps = round((configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (hv_AxisLegLength / 360));
						stopAll = GreekKey_Angular_AxisAndSpindle(axisSteps, 1, spindleSteps, 1);
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
				if (configGreekKey.segmentOrActual == 2) // 2: Segment  
				{
					switch (configGreekKey.axisId)
					{
					case ID_AXIS_Z: // Z Axis is primary
					{
						stopAll = GreekKey_Move_Axis(shortSegmentStepsAxisX, segmentMultiplier, DIR_CCW, false);
						SetEnable(ID_AXIS_X, false, true); // Allow depth of cut to be changed
						break;
					}
					case ID_AXIS_X: // X Axis is primary
					{
						stopAll = GreekKey_Move_Axis(shortSegmentStepsAxisZ, segmentMultiplier, DIR_CCW, false);
						SetEnable(ID_AXIS_Z, false, true); // Allow depth of cut to be changed
						break;
					}
					case ID_AXIS_3: // M3 Axis is primary
					{
						stopAll = GreekKey_Move_Axis(shortSegmentStepsAxisX, segmentMultiplier, DIR_CCW, false);
						SetEnable(ID_AXIS_X, false, true); // Allow depth of cut to be changed
						break;
					}
					case ID_AXIS_4: // M4 Axis is primary
					{
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
					case ID_AXIS_3: // M3 Axis
					{
						switch (configSetup.radialOrLinear_Axis_M3)
						{
						case RADIAL_M3:
						{
							axisSteps = DistanceToSteps_M3Radial(segmentMultiplier);
							break;
						}
						case LINEAR_M3:
						{
							axisSteps = DistanceToSteps_M3Linear(segmentMultiplier);
							break;
						}
						}
						break;
					}
					case ID_AXIS_4: // M4 Axis
					{
						switch (configSetup.radialOrLinear_Axis_M4)
						{
						case RADIAL_M4:
						{
							axisSteps = DistanceToSteps_M4Radial(segmentMultiplier);
							break;
						}
						case LINEAR_M4:
						{
							axisSteps = DistanceToSteps_M4Linear(segmentMultiplier);
							break;
						}
						}
						break;
					}
					}

					// Uses actual value
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
			case 74: // J - Set M3Radius 
			{
				configSetup.radiusM3 = segmentMultiplier;
				SerialPrint("wepo ");
				SerialWrite(0x22);
				SerialPrint(segmentMultiplier);
				SerialWrite(0x22);
				eePromAddress = 688;
				SerialPrint(",");
				SerialPrint(eePromAddress);
				SerialPrint(nextionEnd);

				if (configGreekKey.axisId == ID_AXIS_3)
				{
					SerialPrint("pageProgram.t40.txt=");
					SerialWrite(0x22);
					SerialPrint(segmentMultiplier);
					SerialWrite(0x22);
					SerialPrint(nextionEnd);
				}
				break;
			}
			case 75:// K - Enable/DisableStepper
			{
				if (enableDisableStepperID == "S")
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
				else if (enableDisableStepperID == "B") // ToDo Verify
				{
					if (segmentMultiplier == 0)
					{
						SetEnable(ID_AXIS_3, false, false);
					}
					else
					{
						SetEnable(ID_AXIS_3, true, false);
					}
				}
				else if (enableDisableStepperID == "D")// ToDo Verify
				{
					if (segmentMultiplier == 0)
					{
						SetEnable(ID_AXIS_4, false, false);
					}
					else
					{
						SetEnable(ID_AXIS_4, true, false);
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
					case ID_AXIS_3: // M3 Axis
					{
						if (segmentMultiplier == 0)
						{
							SetEnable(ID_AXIS_3, false, false);
						}
						else
						{
							SetEnable(ID_AXIS_3, true, false);
						}

						break;
					}
					case ID_AXIS_4: // M4 Axis
					{
						if (segmentMultiplier == 0)
						{
							SetEnable(ID_AXIS_4, false, false);
						}
						else
						{
							SetEnable(ID_AXIS_4, true, false);
						}

						break;
					}
					}
				}

				break;
			}
			case 76: // L - Set M4Radius 
			{
				configSetup.radiusM4 = segmentMultiplier;
				SerialPrint("wepo ");
				SerialWrite(0x22);
				SerialPrint(segmentMultiplier);
				SerialWrite(0x22);
				eePromAddress = 664;
				SerialPrint(",");
				SerialPrint(eePromAddress);
				SerialPrint(nextionEnd);

				if (configGreekKey.axisId == ID_AXIS_4)
				{
					SerialPrint("pageProgram.t40.txt=");
					SerialWrite(0x22);
					SerialPrint(segmentMultiplier);
					SerialWrite(0x22);
					SerialPrint(nextionEnd);
				}

				break;
			}
			case 78:  // N - Index page command
			{
				SerialPrint("wepo ");
				if (commandIndex == "J")
				{
					configIndex_Prime.indexId = segmentMultiplier;
					SerialPrint(segmentMultiplier);
					eePromAddress = 368;
				}
				else if (commandIndex == "N")
				{
					//if (abs(segmentMultiplier) * moveDirection < 0)
					if (segmentMultiplier < 0)
					{
#ifdef DEBUG
						Serial.println("Index(DIR_CCW)");
						Serial.print("segmentMultiplier:");
						Serial.println(segmentMultiplier);
						Serial.print("moveDirection:");
						Serial.println(moveDirection);
#endif //DEBUG
						Index(DIR_CCW);
					}
					else
					{
#ifdef DEBUG
						Serial.println("Index(DIR_CW)");
						Serial.print("segmentMultiplier:");
						Serial.println(segmentMultiplier);
						Serial.print("moveDirection:");
						Serial.println(moveDirection);
#endif //DEBUG
						Index(DIR_CW);
					}

				}
				else if (commandIndex == "T")
				{
					// 1 = Divisions, 2 = Degrees
					switch (configIndex_Prime.indexId)
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
					int stepsPerRevolution = (int)(configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle);
					double sizeInSteps_Degrees = stepsPerRevolution * (segmentMultiplier / 360);
					int32_t sizeInSteps_Divisions = stepsPerRevolution / segmentMultiplier;

					switch (configIndex_Prime.indexId)
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
				if (configGreekKey.segmentOrActual == 2) // 2: Segment  
				{
					switch (configGreekKey.axisId)
					{
					case ID_AXIS_Z: // Z Axis is primary, so use X to move cutter away from workpiece
					case ID_AXIS_3: // Assumes toolpath is on the outside of the workpiece
					case ID_AXIS_4: // Assumes toolpath is on the outside of the workpiece
					{
						stopAll = GreekKey_Move_Axis(shortSegmentStepsAxisX, segmentMultiplier, DIR_CW, false);
						break;
					}
					case ID_AXIS_X: // X Axis is primary, so use Z to move cutter away from workpiece
					{
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
					case ID_AXIS_3:
					case ID_AXIS_4:
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
			case 82: // R - Rose page command
			{
				SerialPrint("wepo ");
				if (commandRose == "R") // Run Rose page
				{
					//if (abs(segmentMultiplier) * moveDirection < 0)
					if (segmentMultiplier < 0)
					{
						RoseRadial(DIR_CCW);
					}
					else
					{
						RoseRadial(DIR_CW);
					}

				}
				else if (commandRose == "N") // Set n
				{

				}
				else if (commandRose == "D") // Set d
				{

				}
				else if (commandRose == "S") // Set spindle revolutions
				{

				}
				else if (commandRose == "A") // Set axis distance
				{

				}
			}
			case 84:  // T - Two axes synchronized
			{
				stopAll = GreekKey_Angular_TwoAxes(moveDirection);
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
			case 86:  // V - Recip page command
			{
				SerialPrint("wepo ");
				if (commandRecip == "T") // Radial or Axial
				{
					// 0 = Radial, 1 = Axial
					configRec.radial_Axial = segmentMultiplier;
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
					//if ((segmentMultiplier) * moveDirection < 0)
					if (segmentMultiplier < 0)
					{
						waveDir = DIR_CCW;
					}

					Reciprocate(waveDir);
					////switch (configRec.style)
					////{
					////case 1: // Triangle
					////{
					////	Reciprocate_Triangle(waveDir);
					////	break;
					////}
					////case 2: // Sawtooth
					////{
					////	Reciprocate(waveDir);
					////	break;
					////}
					////case 3: // Square
					////{
					////	Reciprocate_Square(waveDir);
					////	break;
					////}
					////}

				}
				else if (commandRecip == "W")
				{
					if (configRec.radial_Axial == 0)//Radial
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
					if (configRec.radial_Axial == 0)//Radial
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
					if (configRec.radial_Axial == 0)//Radial
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


			} // End Switch(moveType)

			stopAll = StopGreekKey();

			if (stopAll)
			{
				goto EndLoops;
			}

			if (exitInnerForLoop)
			{
#ifdef VERBOSE
				Serial.println("ExitInnerForLoop>>>>>>>>");
#endif // VERBOSE

				break;
			}
		}// End Inner loop

	}

EndLoops:
	// Update Nextion


	if (limitTriggered || eStopTriggered)
	{
#ifdef VERBOSE

		Serial.print("limitTriggered: ");
		Serial.print(limitTriggered);
		Serial.print("  ");
		Serial.println(eStopTriggered);
#endif // VERBOSE
	}
	else
	{
#ifdef VERBOSE
		Serial.print("limitTriggered: ");
		Serial.print(limitTriggered);
		Serial.print("  ");
		Serial.println(eStopTriggered);
#endif // VERBOSE

		SerialPrint("pageProgram.va0.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageProgram.bt1.val=0");
		SerialPrint(nextionEnd);
	}

	SerialPrint("pageProgram.bt3.val=0");
	SerialPrint(nextionEnd);
	SerialPrint("pageProgram.bt2.val=0");
	SerialPrint(nextionEnd);


	degrees_Spindle = StepsToDegrees(endPosition_Spindle, ID_SPINDLE);
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
	case ID_AXIS_3: // M3 Axis
	{
		// ToDo: Verify Linear
		distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_3);
		break;
	}
	case ID_AXIS_4: // M4 Axis
	{
		// ToDo: 
		distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_4);
		break;
	}
	}

	//returnSteps_Spindle = endPosition_Spindle;

#ifdef DEBUG
	Serial.print("endPosition_Spindle :");
	Serial.println(endPosition_Spindle);
	Serial.print("endPosition_Axis :");
	Serial.println(endPosition_Axis);


	Serial.print("cumulative_Spindle:  ");
	Serial.println(cumulative_Spindle);
	Serial.print("cumulative_Axis:  ");
	Serial.println(cumulative_Axis);
	Serial.print("stepperPosition_Cumulative_M3:  ");
	Serial.println(cumulative_M3);


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
	SetEnable(ID_AXIS_3, false, true);
	SetEnable(ID_AXIS_4, false, true);

	// 0 = Change to disabled, 1 = keep enabled
	if (configSetup.keepSteppersEnabled == 1)
	{
		SerialPrint("pageProgram.bt10.val=1"); // 1 = enabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageProgram.bt11.val=1"); // 1 = enabled
		SerialPrint("\xFF\xFF\xFF");
	}
	else
	{
		SerialPrint("pageProgram.bt10.val=0"); // 0 = disabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageProgram.bt10.txt="); // 0 = disabled
		SerialPrint("\x22");
		SerialPrint("Disabled");
		SerialPrint("\x22\xFF\xFF\xFF");

		SerialPrint("pageProgram.bt11.val=0"); // 0 = disabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageProgram.bt11.txt="); // 0 = disabled
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
	MilliDelay(timeout * 1000);
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
	//Serial.println("Enter SerialPrint");
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
	//case 2:
	//{
	//	Serial2.print(text);
	//	break;
	//}
	case 3:
	{
		Serial3.print(text);
		break;
	}
	case 4:
	{
		Serial4.print(text);
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
	//Serial.println(serialId);
	switch (serialId)
	{
	case 0:
	{
		Serial.print(number, trim);
		break;
	}
	case 1:
	{
		//Serial.println("Case 1  ");
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
	case 4:
	{
		Serial4.print(number, trim);
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
	//Serial.println("Enter PrintFloat");
	for (decimalPlaces = 0; decimalPlaces < 6; decimalPlaces++)
	{

		if (tempValue == (long)tempValue)
		{
			break;
		}
		tempValue *= 10.0;  // Shift left one decimal digit
	}
#ifdef VERBOSE
	//Serial.print("decimalPlaces:");
	//Serial.println(decimalPlaces);
	//Serial.print("PrintFloat: ");
	//Serial.println(floatValue, decimalPlaces);
#endif // VERBOSE
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
	//case 2:
	//{
	//	Serial2.write(text);
	//	break;
	//}
	case 3:
	{
		Serial3.write(text);
		break;
	}
	case 4:
	{
		Serial4.write(text);
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
	//Serial2.println(text);
	Serial3.println(text);
	Serial4.println(text);
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
	//case 2:
	//{
	//	incomingData = Serial2.read();
	//	break;
	//}
	case 3:
	{
		incomingData = Serial3.read();
		break;
	}
	case 4:
	{
		incomingData = Serial4.read();
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
	//else if (Serial2.available() > 0)
	//{
	//	serialId = 2;
	//}
	else if (Serial3.available() > 0)
	{
		serialId = 3;
	}
	else if (Serial4.available() > 0)
	{
		serialId = 4;
	}
	return serialId;
}


/// <summary>
/// Get a filename from serial
/// </summary>
/// <returns></returns>
void GetFilenameFromSerial()
{
	const int bufferLength = 13;
	char input = 0;
	int i;

#ifdef DEBUG
	Serial.println("Enter GetFilenameFromSerial ");
	Serial.print("pageCallerId: ");
	Serial.println(pageCallerId);
#endif // DEBUG
	Serial.print("0000. fileNameIndex: ");
	Serial.println(fileNameIndex);
	switch (pageCallerId)
	{
	case PAGE_INDEX:
	{
			// Clear fileNameIndex
			memset(fileNameIndex, 0, sizeof(fileNameIndex));
			break;
		}
		case PAGE_PROGRAM:
		{
			// Clear fileNameProgram
			memset(fileNameProgram, 0, sizeof(fileNameProgram));
			//memset(fileNameProgram, 0, 12);
			break;
		}
		case PAGE_INI:
		{
			// Clear fileNameIni
			memset(fileNameIni, 0, sizeof(fileNameIni));
			break;
		}
	}

	for (i = 0; i < bufferLength; i++)
	{
		input = SerialRead(serialId);
#ifdef DEBUG
		Serial.print("input: ");
		Serial.println(input);
#endif // DEBUG

		// Use 255 as end chars
		if (input == 255)
		{
#ifdef DEBUG
			Serial.print("i: ");
			Serial.println(i);
#endif // DEBUG

			break;
		}
		// Store filename
		Serial.print("0. pageCallerId: ");
		Serial.println(pageCallerId);
		switch (pageCallerId)
		{
			case PAGE_INDEX:
			{
				fileNameIndex[i] = input;
	#ifdef DEBUG
				Serial.print("1. fileNameIndex: ");
				Serial.println(fileNameIndex);
	#endif // DEBUG
				break;
			}
			case PAGE_PROGRAM:
			{
				fileNameProgram[i] = input;
	#ifdef DEBUG
				Serial.print("fileNameProgram: ");
				Serial.println(fileNameProgram);
				Serial.print("sizeof(fileNameProgram):  ");
				Serial.println(sizeof(fileNameProgram));
	#endif // DEBUG
				break;
			}
			case PAGE_INI:
			{
				fileNameIni[i] = input;
	#ifdef DEBUG
				Serial.print("fileNameIni: ");
				Serial.println(fileNameIni);
	#endif // DEBUG
				break;
			}
		}
	}

#ifdef DEBUG
	Serial.print("2. fileNameIndex: ");
	Serial.println(fileNameIndex);
	Serial.print("fileNameProgram: ");
	Serial.println(fileNameProgram);
	Serial.print("input: ");
	Serial.println(input);
#endif // DEBUG

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
//	int filename_Length;

#ifdef DEBUG
	Serial.print("-------------------LineNumber :  ");
	Serial.println(lineNumber);
	
	Serial.print("--fileNameIndex :  ");
	Serial.println(fileNameIndex);
#endif // DEBUG

	// Open file
	File sdFile = SD.open(fileNameIndex, FILE_READ);
	if (!sdFile)
	{
		badFilename = true;
#ifdef DEBUG
		Serial.print("File not found-01:");
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

		if (newSizeString == "E")
		{
#ifdef DEBUG
			Serial.print("==------------=The End :  ");
			Serial.println(newSizeString);
			endOfData_Index = true;
#endif // DEBUG
			break;

		}

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
	const char* newSizeChar = static_cast<const char*>(newSizeString.c_str());
#ifdef DEBUG
	Serial.print("^^^^newSizeChar :  ");
	Serial.println(newSizeChar);

#endif // DEBUG
	return atof(newSizeChar);
}

/// <summary>
/// GetIndexDataFromSD
/// </summary>
/// <comment>
/// Get a value from file on SD card
/// </comment>
/// <param name="lineNumber">Line containing data</param>
/// <returns>Double value</returns>
double GetIndexDataCountFromSD(int lineNumber)
{
	String newSizeString = "";
	currentLineNumber = 0;
//	int filename_Length;

#ifdef DEBUG
	Serial.print("-------------------LineNumber :  ");
	Serial.println(lineNumber);

	Serial.print("--fileNameIndex :  ");
	Serial.println(fileNameIndex);
#endif // DEBUG

	// Open file
	File sdFile = SD.open(fileNameIndex, FILE_READ);
	if (!sdFile)
	{
		badFilename = true;
#ifdef DEBUG
		Serial.print("File not found-01:");
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
	const char* newSizeChar = static_cast<const char*>(newSizeString.c_str());
#ifdef DEBUG
	Serial.print("^^^^newSizeChar :  ");
	Serial.println(newSizeChar);

#endif // DEBUG
	return atof(newSizeChar);
}


void GetFileList_SD(int fileIndex)
{
	char fullFileList[100][13];

	//int fileArrayIndex = 0;
	//String* fileNames = NULL;
	String iniExtension = ".ini";
	String tftExtension = ".tft";
	const char* pageFileList_t21 = "pageFileList.t21.txt=";
	const char* pageFileList_t22 = "pageFileList.t22.txt=";
	const char* pageFileList_t23 = "pageFileList.t23.txt=";
	const char* pageFileList_t24 = "pageFileList.t24.txt=";
	const char* pageFileList_t25 = "pageFileList.t25.txt=";
	const char* pageFileList_t26 = "pageFileList.t26.txt=";
	const char* pageFileList_t27 = "pageFileList.t27.txt=";

	// Clear fullFileList memory
	memset(fullFileList, 0, sizeof(fullFileList));
	File root = SD.open("/");
	int iCounter = 0;

	while (true)
	{
		File entry = root.openNextFile();
		String fileFullName = String(entry.name());
		if (!entry)
		{
#ifdef VERBOSE
			Serial.println("** no more files **");
#endif // VERBOSE
			break;
		}

		if (entry.isDirectory())
		{
#ifdef VERBOSE
			Serial.println("/");
#endif // VERBOSE
		}
		else if (fileFullName.indexOf(iniExtension) >= 0 || fileFullName.indexOf(tftExtension) >= 0)
		{

#ifdef VERBOSE
			Serial.println("File is .ini or .tft ");
#endif // VERBOSE
		}
		else
		{
#ifdef DEBUG
			Serial.print(entry.name());
			Serial.println();
#endif // DEBUG

			// Populate file list array
			if (iCounter >= 0 && iCounter < 99)
			{
				strncpy(fullFileList[iCounter], entry.name(), 13);

			}
			iCounter++;
		}
		entry.close();
	}

	SerialPrint("pageFileList.vaFileCount.val=");
	SerialPrint(iCounter);
	SerialPrint(nextionEnd);

	SerialPrint("pageFileList.n2.val=");
	SerialPrint(iCounter);
	SerialPrint(nextionEnd);
#ifdef DEBUG
	Serial.print("File Count: ");
	Serial.println(iCounter);
#endif // DEBUG

	for (int i = 0; i < iCounter + 1; i++)
	{
#ifdef DEBUG
		Serial.println(fullFileList[i]);
		Serial.println("----------");
#endif // DEBUG
	}

	int curIndex = 0;
	for (int j = 0; j < 7; j++)
	{
#ifdef VERBOSE
		Serial.print("j: ");
		Serial.print(j);
		Serial.print(" : ");
		Serial.print(fullFileList[j]);
		Serial.print("    fileIndex + j: ");
		Serial.println(fileIndex + j);
#endif // VERBOSE
		// Copy filenames to Nextion
		curIndex = j + fileIndex;
		switch (j)
		{
		case 0:
		{
			SerialPrint(pageFileList_t21);
			SerialPrint("\x22");
			SerialPrint(fullFileList[curIndex]);
			Serial.println(fullFileList[curIndex]);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case 1:
		{

			SerialPrint(pageFileList_t22);
			SerialPrint("\x22");
			SerialPrint(fullFileList[curIndex]);
			Serial.println(fullFileList[curIndex]);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageFileList.t13.txt=");
			SerialPrint("\x22");
			SerialPrint(fullFileList[curIndex]);
			Serial.println(fullFileList[curIndex]);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case 2:
		{
			SerialPrint(pageFileList_t23);
			SerialPrint("\x22");
			SerialPrint(fullFileList[curIndex]);
			Serial.println(fullFileList[curIndex]);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case 3:
		{
			SerialPrint(pageFileList_t24);
			SerialPrint("\x22");
			SerialPrint(fullFileList[curIndex]);
			Serial.println(fullFileList[curIndex]);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case 4:
		{

			SerialPrint(pageFileList_t25);
			SerialPrint("\x22");
			SerialPrint(fullFileList[curIndex]);
			Serial.println(fullFileList[curIndex]);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case 5:
		{

			SerialPrint(pageFileList_t26);
			SerialPrint("\x22");
			SerialPrint(fullFileList[curIndex]);
			Serial.println(fullFileList[curIndex]);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case 6:
		{
			SerialPrint(pageFileList_t27);
			SerialPrint("\x22");
			SerialPrint(fullFileList[curIndex]);
			Serial.println(fullFileList[curIndex]);
			SerialPrint(nextionQuoteEnd);
			break;
		}

		}
	}



}

/// <summary>
/// GetFileListFromSD
/// </summary>
/// <comment>
/// Gets file list from microSD card
/// </comment>
/// <param name="fileIndex">Index of file</param>
/// <returns></returns>
void GetFileListFromSD_XXX(int fileIndex)
{
	File dir;
	const char* pageFileList_t21 = "pageFileList.t21.txt=";
	const char* pageFileList_t22 = "pageFileList.t22.txt=";
	const char* pageFileList_t23 = "pageFileList.t23.txt=";
	const char* pageFileList_t24 = "pageFileList.t24.txt=";
	const char* pageFileList_t25 = "pageFileList.t25.txt=";
	const char* pageFileList_t26 = "pageFileList.t26.txt=";
	const char* pageFileList_t27 = "pageFileList.t27.txt=";

	String iniExtension = ".ini";
	String tftExtension = ".tft";

	//	int iCompare = 0;

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
#ifdef VERBOSE
		Serial.print("Dir Size: ");
		Serial.println(dirSize);
#endif // VERBOSE
	}
	else
	{
#ifdef DEBUG
		// print "Card not found"
		Serial.print("Card not found");
#endif // DEBUG
		return;
	}

#ifdef DEBUG
	Serial.print("fileIndex");
	Serial.println(fileIndex);
	Serial.print("lastFileIndex");
	Serial.println(lastFileIndex);
#endif // DEBUG

	// Reset lastFileIndex
	// fileIndex is set to 0 when file dropdown is first opened
	// Up and Down arrows reset the fileIndex 
	if (fileIndex == 0)
	{
		// Default maximum files is 100
		lastFileIndex = 100;
	}

	// Only 7 files can be shown at one time
	for (int iCounter = 0; iCounter < fileIndex + 7; iCounter++)
	{
		File entry = dir.openNextFile();
#ifdef DEBUG
		Serial.println(entry.name());
#endif // DEBUG

		if (entry.isDirectory())
		{
			iCounter--;
		}
		else
		{
			// Exclude ini and tft files
			String fileFullName = String(entry.name());
#ifdef DEBUG
			Serial.print("fileFullName: ");
			Serial.print(fileFullName);
			Serial.print("   :  ");
			Serial.println(fileFullName.indexOf(iniExtension));
#endif // DEBUG

			if (fileFullName.indexOf(iniExtension) >= 0 || fileFullName.indexOf(tftExtension) >= 0)
			{
#ifdef VERBOSE
				Serial.println("File is .ini or .tft ");
#endif // VERBOSE
				iCounter--;
				goto IniFileFound;
			}
			else
			{
				if (iCounter >= fileIndex)
				{
					fileNames[fileArrayIndex] = entry.name();
					fileArrayIndex++;
				}
			}

			// fileIndex is the top line in the dropdown list on the Nextion
#ifdef DEBUG
			Serial.print("iCounter: ");
			Serial.print(iCounter);
			Serial.print(" : ");
			Serial.print(entry.name());
			Serial.print(" : ");
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
			SerialPrint(iCounter - 1);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageFileList.t13.txt=");
			SerialWrite(0x22);
			SerialPrint(iCounter - 1);
			SerialPrint(nextionQuoteEnd);
		}
	IniFileFound:
		entry.close();
	}

	for (int j = 0; j < 7; j++)
	{
#ifdef VERBOSE
		Serial.print("j: ");
		Serial.print(j);
		Serial.print(" : ");
		Serial.println(fileNames[j]);
#endif // VERBOSE
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
/// GetFileListFromSD
/// </summary>
/// <comment>
/// Gets file list from microSD card
/// </comment>
/// <param name="fileIndex">Index of file</param>
/// <returns></returns>
void GetIniFileListFromSD(int fileIndex)
{
	File dir;
	const char* pageFileList_t21 = "pageFileList.t21.txt=";
	const char* pageFileList_t22 = "pageFileList.t22.txt=";
	const char* pageFileList_t23 = "pageFileList.t23.txt=";
	const char* pageFileList_t24 = "pageFileList.t24.txt=";
	const char* pageFileList_t25 = "pageFileList.t25.txt=";
	const char* pageFileList_t26 = "pageFileList.t26.txt=";
	const char* pageFileList_t27 = "pageFileList.t27.txt=";

	String iniExtension = ".ini";
	//int iCompare = 0;

	//int iCounter = 0;
	int fileArrayIndex = 0;
	String* fileNames = NULL;

	fileNames = new String[8];
#ifdef DEBUG
	Serial.println("Enter GetIniFileListFromSD");
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

#ifdef DEBUG
	Serial.print("fileIndex");
	Serial.println(fileIndex);
	Serial.print("lastFileIndex");
	Serial.println(lastFileIndex);
#endif // DEBUG

	// Reset lastFileIndex
	// fileIndex is set to 0 when file dropdown is first opened
	// Up and Down arrows reset the fileIndex 
	if (fileIndex == 0)
	{
		// Default maximum files is 100
		lastFileIndex = 100;
	}

	// Only 7 files can be shown at one time
	for (int iCounter = 0; iCounter < fileIndex + 7; iCounter++)
	{
		File entry = dir.openNextFile();
		Serial.print(iCounter);
		Serial.print(": ");
		Serial.println(entry.name());

		if (entry.isDirectory())
		{
			iCounter--;
#ifdef DEBUG
			Serial.println("entry.isDirectory");
#endif // DEBUG

		}
		else
		{
			String fileFullName = String(entry.name());
#ifdef VERBOSE
			Serial.print("fileFullName: ");
			Serial.print(fileFullName);
			Serial.print("   IndexOf .ini:  ");
			Serial.println(fileFullName.indexOf(iniExtension));
#endif // VERBOSE

			if (fileFullName.indexOf(iniExtension) >= 0)
			{
				if (iCounter >= fileIndex)
				{
					fileNames[fileArrayIndex] = entry.name();
					fileArrayIndex++;
				}
			}
			else
			{

				/*if (iCounter > 1)
				{
					iCounter--;
				}*/
			}



			// fileIndex is the top line in the dropdown list on the Nextion
#ifdef DEBUG
			Serial.print("iCounter: ");
			Serial.print(iCounter);
			Serial.print(" : ");
			Serial.print(entry.name());
			Serial.print(" : ");
#endif // DEBUG

		}

		// Exit when last file found
		if (!entry)
		{
			// no more files
#ifdef VERBOSE
			Serial.print("No More Files: ");
			Serial.print(iCounter);
			Serial.print(" : ");
			Serial.print(lastFileIndex);
#endif // VERBOSE
			lastFileIndex = iCounter + 8;
#ifdef VERBOSE
			Serial.print(" : ");
			Serial.println(lastFileIndex);
#endif // VERBOSE

			SerialPrint("pageSplash.t13.txt=");
			SerialWrite(0x22);
			SerialPrint(iCounter - 1);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		//IniFileFound:
		entry.close();
	}

	for (int j = 0; j < 7; j++)
	{
#ifdef VERBOSE
		Serial.print("j: ");
		Serial.print(j);
		Serial.print(" : ");
		Serial.println(fileNames[j]);
#endif // VERBOSE
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
double GetProgramDataFromSD(int lineNumber)
{
	double retVal = 0;
	currentLineNumber = 0;
	String newSizeString = "";
//	int filename_Length;

	String nStr;
	String nStrA;

	// Open file
#ifdef VERBOSE
	Serial.print("fileNameProgram:  ");
	Serial.println(fileNameProgram);
#endif // VERBOSE
	File sdFile = SD.open(fileNameProgram, FILE_READ);
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
		case 66: // B - M3
		case 67: // C
		case 68: // D - M4
		case 69: // E
		case 73: // I
		case 79: // O
		case 80: // P
		case 83: // S
		case 88: // X
		case 90: // Z
		{
			// Valid entries
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
			Serial.print("   vVal: ");
			Serial.println(hv_SpindleLegLength);
	#endif // DEBUG
			break;
		}

		case 74:// J - M4Radius
		{
			commandM3Radius = newSizeString[1];
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

		case 76:// L - M4Radius
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
			Serial.print("========================Invalid Command: ");
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

		case 82:// R - Rose page settings
		{
			commandRose = newSizeString[1];

			nStr = newSizeString.substring(2);
	#ifdef DEBUG
			Serial.print("page 82: ");
			Serial.println(commandRose);
			Serial.print("nStr 82: ");
			Serial.println(nStr);
	#endif // DEBUG
			break;
		}

		case 84:// T - Two axes synchronized move.
		{
	//		char* val;
			char charBuf[20];
			char charSizePrimary[10];

			char charSizeSecondary[10];
			String strPrimaryId = newSizeString[1];
			nStr = newSizeString.substring(2);
			nStr.toCharArray(charBuf, 20);
	#ifdef DEBUG
			Serial.print("nStr: ");
			Serial.println(nStr);
	#endif // DEBUG
			char keys[] = "BDSXZ";
			int i;
			i = strcspn(charBuf, keys);

			String sizePrimary = nStr.substring(0, i);
			String newStr = nStr.substring(i);
	#ifdef DEBUG
			Serial.print("newStr: ");
			Serial.println(newStr);
	#endif // DEBUG

			String strSecondaryId = newStr.substring(0, 1);

	#ifdef DEBUG
			Serial.print("newStr: ");
			Serial.println(newStr);
	#endif // DEBUG

			String sizeSecondary = newStr.substring(1);

	#ifdef DEBUG
			Serial.print("primaryId:      ");
			Serial.println(strPrimaryId);
			Serial.print("sizePrimary:    ");
			Serial.println(sizePrimary);

			Serial.print("secondaryId:    ");
			Serial.println(strSecondaryId);
			Serial.print("sizeSecondary:  ");
			Serial.println(sizeSecondary);
	#endif // DEBUG

			if (strPrimaryId == "S")
			{
				primaryAxisId = ID_SPINDLE;
			}
			else if (strPrimaryId == "Z")
			{
				primaryAxisId = ID_AXIS_Z;
			}
			else if (strPrimaryId == "X")
			{
				primaryAxisId = ID_AXIS_X;
			}
			else if (strPrimaryId == "B")
			{
				primaryAxisId = ID_AXIS_3;
			}
			else if (strPrimaryId == "D")
			{
				primaryAxisId = ID_AXIS_4;
			}


			if (strSecondaryId == "S")
			{
				secondaryAxisId = ID_SPINDLE;
			}
			else if (strSecondaryId == "Z")
			{
				secondaryAxisId = ID_AXIS_Z;
			}
			else if (strSecondaryId == "X")
			{
				secondaryAxisId = ID_AXIS_X;
			}
			else if (strSecondaryId == "B")
			{
				secondaryAxisId = ID_AXIS_3;
			}
			else if (strSecondaryId == "D")
			{
				secondaryAxisId = ID_AXIS_4;
			}

			sizePrimary.toCharArray(charSizePrimary, 10);
			multiplier_Primary = atof(charSizePrimary);

			sizeSecondary.toCharArray(charSizeSecondary, 10);
			multiplier_Secondary = atof(charSizeSecondary);

	#ifdef DEBUG
			Serial.print("charSizePrimary:   ");
			Serial.println(charSizePrimary);
			Serial.print("multiplier_Primary:   ");
			Serial.println(multiplier_Primary);

			Serial.print("charSizeSecondary: ");
			Serial.println(charSizeSecondary);
			Serial.print("multiplier_Secondary: ");
			Serial.println(multiplier_Secondary);
	#endif // DEBUG
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
	#endif // DEBUG
			}
			else if (countType == "A")
			{
				moveType = 2;

	#ifdef DEBUG
				Serial.print("countType A: ");
	#endif // DEBUG
			}
			else if (countType == "B")
			{
				moveType = 3;
			}
			else if (countType == "D")
			{
				moveType = 4;
			}
			nStr = newSizeString.substring(2);
	#ifdef DEBUG
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

	// Return size string as double
	const char* newSizeChar = static_cast<const char*>(nStr.c_str());

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
/// pageProgram,pageRec, pageGrk: Return Spindle and/or axes to start position
/// </summary>
/// <comment>
/// </comment>
/// <param name="axisId">Id of axis to move</param>
void Return_Cumulative()
{
	limitTriggered = false;
	eStopTriggered = false;

	if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
	{
		digitalWrite(configSetup.auxillaryPin, LOW);  // Enable
	}

	Stepper stepSpindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
	Stepper stepAxis_Z(PIN_AXIS_Z_STEP, PIN_AXIS_Z_DIR);
	Stepper stepAxis_X(PIN_AXIS_X_STEP, PIN_AXIS_X_DIR);
	Stepper stepAxis_M3(PIN_AXIS_3_STEP, PIN_AXIS_3_DIR);
	Stepper stepAxis_M4(PIN_AXIS_4_STEP, PIN_AXIS_4_DIR);

	stepAxis_Z.setAcceleration(0);
	stepAxis_X.setAcceleration(0);
	stepAxis_M3.setAcceleration(0);
	stepAxis_M4.setAcceleration(0);
	stepSpindle.setAcceleration(0);

	stepAxis_Z.setMaxSpeed(0);
	stepAxis_X.setMaxSpeed(0);
	stepAxis_M3.setMaxSpeed(0);
	stepAxis_M4.setMaxSpeed(0);
	stepSpindle.setMaxSpeed(0);

	stepAxis_Z.setPosition(cumulative_Z);
	stepAxis_X.setPosition(cumulative_X);
	stepAxis_M3.setPosition(cumulative_M3);
	stepAxis_M4.setPosition(cumulative_M4);
	stepSpindle.setPosition(cumulative_Spindle);

	/*stepAxis_Z.setTargetAbs(0);
	stepAxis_X.setTargetAbs(0);
	stepAxis_M3.setTargetAbs(0);
	stepAxis_M4.setTargetAbs(0);
	stepSpindle.setTargetAbs(0);*/


	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);
//	int curPosition = 0;

	Serial.print("cumulative_Spindle:  ");
	Serial.println(cumulative_Spindle);
	Serial.print("cumulative_Z:  ");
	Serial.println(cumulative_Z);
	Serial.print("cumulative_X:  ");
	Serial.println(cumulative_X);
	Serial.print("cumulative_M3:  ");
	Serial.println(cumulative_M3);
	Serial.print("cumulative_M4:  ");
	Serial.println(cumulative_M4);

	stepSpindle.setPosition(cumulative_Spindle);
	stepSpindle
		.setMaxSpeed(configSetup.maxSpd_Return_Spindle)
		.setAcceleration(configSetup.accel_Return_Spindle)
		.setTargetAbs(0);

	stepAxis_Z.setPosition(cumulative_Z);
	stepAxis_Z
		.setMaxSpeed(configSetup.maxSpd_Return_Axis_Z)
		.setAcceleration(configSetup.accel_Return_Axis_Z)
		.setTargetAbs(0);

	stepAxis_X.setPosition(cumulative_X);
	stepAxis_X
		.setMaxSpeed(configSetup.maxSpd_Return_Axis_X)
		.setAcceleration(configSetup.accel_Return_Axis_X)
		.setTargetAbs(0);

	stepAxis_M3.setPosition(cumulative_M3);
	stepAxis_M3
		.setMaxSpeed(configSetup.maxSpd_Return_Axis_M3)
		.setAcceleration(configSetup.accel_Return_Axis_M3)
		.setTargetAbs(0);

	stepAxis_M4.setPosition(cumulative_M4);
	stepAxis_M4
		.setMaxSpeed(configSetup.maxSpd_Return_Axis_M4)
		.setAcceleration(configSetup.accel_Return_Axis_M4)
		.setTargetAbs(0);

	SetEnable(ID_SPINDLE, true, true);
	SetEnable(ID_AXIS_Z, true, true);
	SetEnable(ID_AXIS_X, true, true);
	SetEnable(ID_AXIS_3, true, true);
	SetEnable(ID_AXIS_4, true, true);

	StepperGroup stepGroup{ stepAxis_Z,stepAxis_X,stepAxis_M3,stepAxis_M4,stepSpindle };
	stepGroup.startMove();

	while (stepAxis_Z.isMoving || stepAxis_X.isMoving || stepAxis_M3.isMoving || stepAxis_M4.isMoving || stepSpindle.isMoving)
	{

#ifdef ESTOP_ENABLED
		if (configSetup.eStop > 9 && configSetup.eStop < 40)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
#ifdef DEBUG
				Serial.println("                          EStop");
#endif // DEBUG
				stepAxis_Z.emergencyStop();
				stepAxis_X.emergencyStop();
				stepAxis_M3.emergencyStop();
				stepAxis_M4.emergencyStop();
				stepSpindle.emergencyStop();

				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEnable(ID_AXIS_4, false, false);
				SetEStopColors(PAGE_RETURNS);
				if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
				{
					digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
				}
				return;
			}
		}
#endif // estop
		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
				case 83: // - S
				{
					stepAxis_Z.stop();
					stepAxis_X.stop();
					stepAxis_M3.stop();
					stepAxis_M4.stop();
					stepSpindle.stop();
					MilliDelay(200);
					checkedCount = 0;
					break;
				}
			}
		}

		MilliDelay(10);
	}


	switch (pageCallerId)
	{
		case PAGE_GRK:
		{
	#ifdef DEBUG
			Serial.print("pageGrk axisId: ");
			Serial.println(configGreekKey.axisId);
	#endif // DEBUG
			SerialPrint("pageGrk.bt1.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageGrk.va0.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageGrk.bt0.val=0");
			SerialPrint(nextionEnd);

			SetEnable(configGreekKey.axisId, false, true);
			SetEnable(ID_SPINDLE, false, true);
			if (configSetup.keepSteppersEnabled != 1)
			{


				SerialPrint("pageGrk.bt10.val=0"); // 0 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageGrk.bt10.txt="); // 0 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");

				SerialPrint("pageGrk.bt11.val=0"); // 0 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageGrk.bt11.txt="); // 0 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");

			}

			break;
		}
		case PAGE_PROGRAM:
		{
	#ifdef DEBUG
			Serial.print("pageProgram axisId: ");
			Serial.println(configGreekKey.axisId);
	#endif // DEBUG
			SerialPrint("pageProgram.bt1.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageProgram.va0.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageProgram.bt0.val=0");
			SerialPrint(nextionEnd);

			SetEnable(configGreekKey.axisId, false, true);
			SetEnable(ID_SPINDLE, false, true);
			if (configSetup.keepSteppersEnabled != 1)
			{


				SerialPrint("pageProgram.bt10.val=0"); // 0 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageProgram.bt10.txt="); // 0 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");

				SerialPrint("pageProgram.bt11.val=0"); // 0 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageProgram.bt11.txt="); // 0 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");

			}

			break;
		}
		case PAGE_REC:
		{
			if (!limitTriggered)
			{
				SerialPrint("pageRec.va0.val=0");
				SerialPrint(nextionEnd);
				SerialPrint("pageRec.bt1.val=0");
				SerialPrint(nextionEnd);
				SerialPrint("pageRec.bt0.val=0");
				SerialPrint(nextionEnd);
			}

			MilliDelay(750);
			SerialPrint("pageRec.bt3.val=0");// The first one may be ignored by Nextion, so resend.
			SerialPrint(nextionEnd);
			SerialPrint("pageRec.bt2.val=0");
			SerialPrint(nextionEnd);

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

	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		SetEnable(ID_AXIS_3, false, true);
	}

#ifdef DEBUG
	Serial.print("stepSpindle.getPosition() : ");
	Serial.println(stepSpindle.getPosition());
	
	Serial.print("stepAxis_Z.getPosition() : ");
	Serial.println(stepAxis_Z.getPosition());

	Serial.print("stepAxis_X.getPosition() : ");
	Serial.println(stepAxis_X.getPosition());

	Serial.print("stepAxis_M3.getPosition() : ");
	Serial.println(stepAxis_M3.getPosition());

	Serial.print("stepAxis_M4.getPosition(): ");
	Serial.println(stepAxis_M4.getPosition());

	Serial.print("pageCallerId: ");
	Serial.println(pageCallerId);
#endif // DEBUG

	checkedCount = 0;
	if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
	{
		digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
	}
}

/// <summary>
/// PageMain: Return Spindle and/or axes to start position
/// </summary>
/// <comment>
/// </comment>
/// <param name="axisId">Id of axis to move</param>
void ReturnToStartPosition_MainPage()
{
	if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
	{
		digitalWrite(configSetup.auxillaryPin, LOW);  // Enable
	}
	eStopTriggered = false;
	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);

	int32_t maxSpeed_1 = 100000;
	int32_t maxSpeed_2 = 100000;
	int32_t maxSpeed_3 = 100000;
	int32_t maxSpeed_4 = 100000;
	int32_t maxSpeed_5 = 100000;
	int32_t accel_1 = 100000;
	int32_t accel_2 = 100000;
	int32_t accel_3 = 100000;
	int32_t accel_4 = 100000;
	int32_t accel_5 = 100000;
	int curPosition = 0;
	int dirPin_Reset = 55;
#ifdef VERBOSE

	Serial.print("stepper1_step:  ");
	Serial.println(stepper1_step);
	Serial.print("stepper1_dir:  ");
	Serial.println(stepper1_dir);

	Serial.print("stepper2_step:  ");
	Serial.println(stepper2_step);
	Serial.print("stepper2_dir:  ");
	Serial.println(stepper2_dir);

	Serial.print("stepper3_step:  ");
	Serial.println(stepper3_step);
	Serial.print("stepper3_dir:  ");
	Serial.println(stepper3_dir);

	Serial.print("stepper4_step:  ");
	Serial.println(stepper4_step);
	Serial.print("stepper4_dir:  ");
	Serial.println(stepper4_dir);

	Serial.print("stepper5_step:  ");
	Serial.println(stepper5_step);
	Serial.print("stepper5_dir:  ");
	Serial.println(stepper5_dir);
#endif // VERBOSE
	StepperGroup stepGroup;
	Stepper stepper_1(stepper1_step, stepper1_dir);
	Stepper stepper_2(stepper2_step, stepper2_dir);
	Stepper stepper_3(stepper3_step, stepper3_dir);
	Stepper stepper_4(stepper4_step, stepper4_dir);
	Stepper stepper_5(stepper5_step, stepper5_dir);
	stepper_1.setPosition(stepperPosition_1);
	stepper_2.setPosition(stepperPosition_2);
	stepper_3.setPosition(stepperPosition_3);
	stepper_4.setPosition(stepperPosition_4);
	stepper_5.setPosition(stepperPosition_5);

	Serial.print("checkedCount : ");
	Serial.println(checkedCount);

	// Set unused stepper values
	switch (checkedCount)
	{
		case 1:
		{
	#ifdef VERBOSE
			Serial.print("   Case 1");
	#endif // VERBOSE
			stepperId_2 = 44;
			stepperId_3 = 45;
			stepperId_4 = 46;
			stepperId_5 = 47;

			stepper2_step = 50;
			stepper2_dir = 51;

			stepper3_step = 52;
			stepper3_dir = 53;

			stepper4_step = 54;
			stepper4_dir = 55;

			stepper5_step = 56;
			stepper5_dir = 57;

			break;
		}
		case 2:
		{
	#ifdef VERBOSE
			Serial.print("   Case 2");
	#endif // VERBOSE
			stepperId_3 = 45;
			stepperId_4 = 46;
			stepperId_5 = 47;

			stepper3_step = 52;
			stepper3_dir = 53;

			stepper4_step = 54;
			stepper4_dir = 55;

			stepper5_step = 56;
			stepper5_dir = 57;

			break;
		}
		case 3:
		{
	#ifdef VERBOSE
			Serial.print("   Case 3");
	#endif // VERBOSE
			stepperId_4 = 46;
			stepperId_5 = 47;

			stepper4_step = 54;
			stepper4_dir = 55;

			stepper5_step = 56;
			stepper5_dir = 57;

			break;
		}
		case 4:
		{
	#ifdef VERBOSE
			Serial.print("   Case 4");
	#endif // VERBOSE
			stepperId_5 = 47;

			stepper5_step = 56;
			stepper5_dir = 57;

			break;
		}
	}

#ifdef VERBOSE



	Serial.print("stepper1_step:  ");
	Serial.println(stepper1_step);
	Serial.print("stepper1_dir:  ");
	Serial.println(stepper1_dir);

	Serial.print("stepper2_step:  ");
	Serial.println(stepper2_step);
	Serial.print("stepper2_dir:  ");
	Serial.println(stepper2_dir);

	Serial.print("stepper3_step:  ");
	Serial.println(stepper3_step);
	Serial.print("stepper3_dir:  ");
	Serial.println(stepper3_dir);

	Serial.print("stepper4_step:  ");
	Serial.println(stepper4_step);
	Serial.print("stepper4_dir:  ");
	Serial.println(stepper4_dir);

	Serial.print("stepper5_step:  ");
	Serial.println(stepper5_step);
	Serial.print("stepper5_dir:  ");
	Serial.println(stepper5_dir);
#endif // VERBOSE

	// Set MaxSpd and Accel
	switch (stepperId_1)
	{
		case ID_AXIS_Z:
		{
			maxSpeed_1 = configSetup.maxSpd_Return_Axis_Z;
			accel_1 = configSetup.accel_Return_Axis_Z;
			break;
		}
		case ID_AXIS_X:
		{
			maxSpeed_1 = configSetup.maxSpd_Return_Axis_X;
			accel_1 = configSetup.accel_Return_Axis_X;
			break;
		}
		case ID_AXIS_3:
		{
			maxSpeed_1 = configSetup.maxSpd_Return_Axis_M3;
			accel_1 = configSetup.accel_Return_Axis_M3;
			break;
		}
		case ID_AXIS_4:
		{
			maxSpeed_1 = configSetup.maxSpd_Return_Axis_M4;
			accel_1 = configSetup.accel_Return_Axis_M4;
			break;
		}
		case ID_SPINDLE:
		{
			maxSpeed_1 = configSetup.maxSpd_Return_Spindle;
			accel_1 = configSetup.accel_Return_Spindle;
			MilliDelay(15);
			break;
		}
	}
#ifdef DEBUG

	Serial.print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>stepperId_2: ");
	Serial.println(stepperId_2);
#endif // DEBUG
	switch (stepperId_2)
	{
		case ID_AXIS_Z:
		{
			maxSpeed_1 = configSetup.maxSpd_Return_Axis_Z;
			accel_1 = configSetup.accel_Return_Axis_Z;
			break;
		}
		case ID_AXIS_X:
		{
			maxSpeed_2 = configSetup.maxSpd_Return_Axis_X;
			accel_2 = configSetup.accel_Return_Axis_X;
			break;
		}
		case ID_AXIS_3:
		{
			maxSpeed_2 = configSetup.maxSpd_Return_Axis_M3;
			accel_2 = configSetup.accel_Return_Axis_M3;
			break;
		}
		case ID_AXIS_4:
		{
			maxSpeed_2 = configSetup.maxSpd_Return_Axis_M4;
			accel_2 = configSetup.accel_Return_Axis_M4;
			break;
		}
		case ID_SPINDLE:
		{
			maxSpeed_2 = configSetup.maxSpd_Return_Spindle;
			accel_2 = configSetup.accel_Return_Spindle;
			MilliDelay(15);
			break;
		}
	}
	switch (stepperId_3)
	{
		case ID_AXIS_Z:
		{
			maxSpeed_1 = configSetup.maxSpd_Return_Axis_Z;
			accel_1 = configSetup.accel_Return_Axis_Z;
			break;
		}
		case ID_AXIS_X:
		{
			maxSpeed_2 = configSetup.maxSpd_Return_Axis_X;
			accel_2 = configSetup.accel_Return_Axis_X;
			break;
		}
		case ID_AXIS_3:
		{
			maxSpeed_2 = configSetup.maxSpd_Return_Axis_M3;
			accel_2 = configSetup.accel_Return_Axis_M3;
			break;
		}
		case ID_AXIS_4:
		{
			maxSpeed_2 = configSetup.maxSpd_Return_Axis_M4;
			accel_2 = configSetup.accel_Return_Axis_M4;
			break;
		}
		case ID_SPINDLE:
		{
			maxSpeed_2 = configSetup.maxSpd_Return_Spindle;
			accel_2 = configSetup.accel_Return_Spindle;
			MilliDelay(15);
			break;
		}
	}
	switch (stepperId_4)
	{
		case ID_AXIS_Z:
		{
			maxSpeed_1 = configSetup.maxSpd_Return_Axis_Z;
			accel_1 = configSetup.accel_Return_Axis_Z;
			break;
		}
		case ID_AXIS_X:
		{
			maxSpeed_2 = configSetup.maxSpd_Return_Axis_X;
			accel_2 = configSetup.accel_Return_Axis_X;
			break;
		}
		case ID_AXIS_3:
		{
			maxSpeed_2 = configSetup.maxSpd_Return_Axis_M3;
			accel_2 = configSetup.accel_Return_Axis_M3;
			break;
		}
		case ID_AXIS_4:
		{
			maxSpeed_2 = configSetup.maxSpd_Return_Axis_M4;
			accel_2 = configSetup.accel_Return_Axis_M4;
			break;
		}
		case ID_SPINDLE:
		{
			maxSpeed_2 = configSetup.maxSpd_Return_Spindle;
			accel_2 = configSetup.accel_Return_Spindle;
			MilliDelay(15);
			break;
		}
	}
	switch (stepperId_5)
	{
		case ID_AXIS_Z:
		{
			maxSpeed_1 = configSetup.maxSpd_Return_Axis_Z;
			accel_1 = configSetup.accel_Return_Axis_Z;
			break;
		}
		case ID_AXIS_X:
		{
			maxSpeed_2 = configSetup.maxSpd_Return_Axis_X;
			accel_2 = configSetup.accel_Return_Axis_X;
			break;
		}
		case ID_AXIS_3:
		{
			maxSpeed_2 = configSetup.maxSpd_Return_Axis_M3;
			accel_2 = configSetup.accel_Return_Axis_M3;
			break;
		}
		case ID_AXIS_4:
		{
			maxSpeed_2 = configSetup.maxSpd_Return_Axis_M4;
			accel_2 = configSetup.accel_Return_Axis_M4;
			break;
		}
		case ID_SPINDLE:
		{
			maxSpeed_2 = configSetup.maxSpd_Return_Spindle;
			accel_2 = configSetup.accel_Return_Spindle;
			MilliDelay(15);
			break;
		}
	}

#ifdef VERBOSE

	Serial.print("checkedCount : ");
	Serial.println(checkedCount);
	Serial.print("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<stepperId_1:  ");
	Serial.println(stepperId_1);
	Serial.print("maxSpeed_1:  ");
	Serial.println(maxSpeed_1);
	Serial.print("accel_1:  ");
	Serial.println(accel_1);
	Serial.print("stepper1_step:  ");
	Serial.println(stepper1_step);
	Serial.print("stepper1_dir:  ");
	Serial.println(stepper1_dir);
	Serial.print("stepper_1.getPosition() : ");
	Serial.println(stepper_1.getPosition());
	Serial.print("stepperId_1--stepperPosition_1:  ");
	Serial.println(stepperPosition_1);

	Serial.print("<<<<<<<<stepperId_2:  ");
	Serial.println(stepperId_2);
	Serial.print("maxSpeed_2:  ");
	Serial.println(maxSpeed_2);
	Serial.print("accel_2:  ");
	Serial.println(accel_2);
	Serial.print("stepper2_step:  ");
	Serial.println(stepper2_step);
	Serial.print("stepper2_dir:  ");
	Serial.println(stepper2_dir);
	Serial.print("stepper_2.getPosition() : ");
	Serial.println(stepper_2.getPosition());
	Serial.print("stepperId_2--stepperPosition_2:  ");
	Serial.println(stepperPosition_2);

	Serial.print("<<<<<<<<stepperId_3:  ");
	Serial.println(stepperId_3);
	Serial.print("maxSpeed_3:  ");
	Serial.println(maxSpeed_3);
	Serial.print("accel_3:  ");
	Serial.println(accel_3);
	Serial.print("stepper3_step:  ");
	Serial.println(stepper3_step);
	Serial.print("stepper3_dir:  ");
	Serial.println(stepper3_dir);
	Serial.print("stepper_3.getPosition() : ");
	Serial.println(stepper_3.getPosition());
	Serial.print("stepperId_3--stepperPosition_3:  ");
	Serial.println(stepperPosition_3);

	Serial.print("<<<<<<<<stepperId_4:  ");
	Serial.println(stepperId_4);
	Serial.print("maxSpeed_4:  ");
	Serial.println(maxSpeed_4);
	Serial.print("accel_4:  ");
	Serial.println(accel_4);
	Serial.print("stepper4_step:  ");
	Serial.println(stepper4_step);
	Serial.print("stepper4_dir:  ");
	Serial.println(stepper4_dir);
	Serial.print("stepper_4.getPosition() : ");
	Serial.println(stepper_4.getPosition());
	Serial.print("stepperId_4--stepperPosition_4:  ");
	Serial.println(stepperPosition_4);

	Serial.print("<<<<<<<<stepperId_5:  ");
	Serial.println(stepperId_5);
	Serial.print("maxSpeed_5:  ");
	Serial.println(maxSpeed_5);
	Serial.print("accel_2:  ");
	Serial.println(accel_2);
	Serial.print("stepper5_step:  ");
	Serial.println(stepper5_step);
	Serial.print("stepper5_dir:  ");
	Serial.println(stepper5_dir);
	Serial.print("stepper_5.getPosition(): ");
	Serial.println(stepper_5.getPosition());
	Serial.print("stepperId_5--stepperPosition_5:  ");
	Serial.println(stepperPosition_5);
	Serial.println("--------------------------------------------------------------");
#endif // VERBOSE

	// Set Target to 0
	stepper_1
		.setMaxSpeed(maxSpeed_1)
		.setAcceleration(accel_1)
		.setTargetAbs(0);

	stepper_2
		.setMaxSpeed(maxSpeed_2)
		.setAcceleration(accel_2)
		.setTargetAbs(0);

	stepper_3
		.setMaxSpeed(maxSpeed_3)
		.setAcceleration(accel_3)
		.setTargetAbs(0);

	stepper_4
		.setMaxSpeed(maxSpeed_4)
		.setAcceleration(accel_4)
		.setTargetAbs(0);

	stepper_5
		.setMaxSpeed(maxSpeed_5)
		.setAcceleration(accel_5)
		.setTargetAbs(0);

	dirPin_Reset = digitalReadFast(PIN_AXIS_3_DIR);

	//if (stepperPosition_1 > 0)
	//{
	//	switch (stepperId_1)
	//	{
	//		case ID_AXIS_Z:
	//		{
	//			digitalWriteFast(PIN_AXIS_Z_DIR, LOW);
	//			break;
	//		}
	//		case ID_AXIS_X:
	//		{
	//			digitalWriteFast(PIN_AXIS_X_DIR, LOW);
	//			break;
	//		}
	//		case ID_AXIS_3:
	//		{
	//			digitalWriteFast(PIN_AXIS_3_DIR, LOW);
	//			break;
	//		}
	//		case ID_AXIS_4:
	//		{
	//			digitalWriteFast(PIN_AXIS_4_DIR, LOW);
	//			break;
	//		}
	//	}
	//}

	//if (stepperPosition_2 > 0) // Only valid on main page when synchro engaged
	//{

	//	digitalWriteFast(PIN_AXIS_3_DIR, LOW);
	//}

#ifdef VERBOSE
	Serial.print("dirPin_Reset: ");
	Serial.println(dirPin_Reset);
#endif // VERBOSE
	dirPin_Reset = digitalReadFast(PIN_SPINDLE_DIR);
#ifdef VERBOSE
	Serial.print("dirPin-Spindle: ");
	Serial.println(dirPin_Reset);
#endif // VERBOSE

	switch (checkedCount)
	{
		case 1:
		{
			SetEnable(stepperId_1, true, true);
	#ifdef VERBOSE
			Serial.print("   Case 1----");
			Serial.print("stepperId_1: ");
			Serial.println(stepperId_1);
			Serial.print("maxSpeed_1:  ");
			Serial.println(maxSpeed_1);
			Serial.print("accel_1:  ");
			Serial.println(accel_1);
			Serial.print("stepper1_step: ");
			Serial.println(stepper1_step);
			Serial.print("stepper1_dir: ");
			Serial.println(stepper1_dir);
			Serial.print("stepperPosition_1: ");
			Serial.println(stepperPosition_1);
			curPosition = stepper_1.getPosition();
			Serial.print("curPosition 1: ");
			Serial.println(curPosition);
			Serial.println("moveAsync(stepper_1)");
	#endif // VERBOSE
			StepperGroup stepGroup{ stepper_1 };
			stepGroup.startMove();
			break;
		}
		case 2:
		{
			SetEnable(stepperId_1, true, true);
			SetEnable(stepperId_2, true, true);

	#ifdef VERBOSE
			Serial.print("   Case 2----");
			Serial.print("stepperId_1: ");
			Serial.println(stepperId_1);
			Serial.print("maxSpeed_1:  ");
			Serial.println(maxSpeed_1);
			Serial.print("accel_1:  ");
			Serial.println(accel_1);
			Serial.print("stepper1_step: ");
			Serial.println(stepper1_step);
			Serial.print("stepper1_dir: ");
			Serial.println(stepper1_dir);
			Serial.print("stepperPosition_1: ");
			Serial.println(stepperPosition_1);
			curPosition = stepper_1.getPosition();
			Serial.print("curPosition 1: ");
			Serial.println(curPosition);

			Serial.print("   Case 2----");
			Serial.print("stepperId_2: ");
			Serial.println(stepperId_2);
			Serial.print("maxSpeed_2:  ");
			Serial.println(maxSpeed_2);
			Serial.print("accel_2:  ");
			Serial.println(accel_2);
			Serial.print("stepper2_step: ");
			Serial.println(stepper2_step);
			Serial.print("stepper2_dir: ");
			Serial.println(stepper2_dir);
			Serial.print("stepperPosition_2: ");
			Serial.println(stepperPosition_2);
			curPosition = stepper_2.getPosition();
			Serial.print("curPosition 2: ");
			Serial.println(curPosition);
			Serial.println("moveAsync(stepper_1, stepper_2)");
	#endif // VERBOSE

			StepperGroup stepGroup{ stepper_1,stepper_2 };
			//stepGroup.startMove();
			stepper_1.moveAbsAsync(0);
			stepper_2.moveAbsAsync(0);
			break;
		}
		case 3:
		{
			SetEnable(stepperId_1, true, true);
			SetEnable(stepperId_2, true, true);
			SetEnable(stepperId_3, true, true);

	#ifdef VERBOSE
			Serial.print("   Case 3----");
			Serial.print("stepperId_1: ");
			Serial.println(stepperId_1);
			Serial.print("maxSpeed_1:  ");
			Serial.println(maxSpeed_1);
			Serial.print("accel_1:  ");
			Serial.println(accel_1);
			Serial.print("stepper1_step: ");
			Serial.println(stepper1_step);
			Serial.print("stepper1_dir: ");
			Serial.println(stepper1_dir);
			Serial.print("stepperPosition_1: ");
			Serial.println(stepperPosition_1);
			curPosition = stepper_1.getPosition();
			Serial.print("curPosition 1: ");
			Serial.println(curPosition);

			Serial.print("   Case 3----");
			Serial.print("stepperId_2: ");
			Serial.println(stepperId_2);
			Serial.print("stepper2_step: ");
			Serial.println(stepper2_step);
			Serial.print("maxSpeed_2:  ");
			Serial.println(maxSpeed_2);
			Serial.print("accel_2:  ");
			Serial.println(accel_2);
			Serial.print("stepper2_dir: ");
			Serial.println(stepper2_dir);
			Serial.print("stepperPosition_2: ");
			Serial.println(stepperPosition_2);
			curPosition = stepper_2.getPosition();
			Serial.print("curPosition 2: ");
			Serial.println(curPosition);

			Serial.print("   Case 3----");
			Serial.print("stepperId_3: ");
			Serial.println(stepperId_3);
			Serial.print("maxSpeed_3:  ");
			Serial.println(maxSpeed_3);
			Serial.print("accel_3:  ");
			Serial.println(accel_3);
			Serial.print("stepper3_step: ");
			Serial.println(stepper3_step);
			Serial.print("stepper3_dir: ");
			Serial.println(stepper3_dir);
			Serial.print("stepperPosition_3: ");
			Serial.println(stepperPosition_3);
			curPosition = stepper_3.getPosition();
			Serial.print("curPosition 3: ");
			Serial.println(curPosition);
			Serial.println("moveAsync(stepper_1, stepper_2, stepper_3)");
	#endif // VERBOSE

			StepperGroup stepGroup{ stepper_1, stepper_2, stepper_3 };
			stepGroup.startMove();

			break;
		}
		case 4:
		{
			SetEnable(stepperId_1, true, true);
			SetEnable(stepperId_2, true, true);
			SetEnable(stepperId_3, true, true);
			SetEnable(stepperId_4, true, true);

			StepperGroup stepGroup{ stepper_1, stepper_2, stepper_3,stepper_4 };
			stepGroup.startMove();

			break;
		}
		case 5:
		{
			SetEnable(stepperId_1, true, true);
			SetEnable(stepperId_2, true, true);
			SetEnable(stepperId_3, true, true);
			SetEnable(stepperId_4, true, true);
			SetEnable(stepperId_5, true, true);
			StepperGroup stepGroup{ stepper_1, stepper_2, stepper_3,stepper_4, stepper_5 };
			stepGroup.startMove();
			break;
		}
	}

	while (stepper_1.isMoving)
	{

#ifdef ESTOP_ENABLED
		if (configSetup.eStop > 9 && configSetup.eStop < 40)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
#ifdef DEBUG
				Serial.println("                          EStop");
#endif // DEBUG

				stepper_1.emergencyStop();
				stepper_2.emergencyStop();
				stepper_3.emergencyStop();
				stepper_4.emergencyStop();
				stepper_5.emergencyStop();
				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEnable(ID_AXIS_4, false, false);
				SetEStopColors(PAGE_RETURNS);
				if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
				{
					digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
				}
				return;
			}
		}
#endif // estop
		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
			case 83: // - S
			{
//				currentSpeed = stepGroup.getCurrentSpeed();
//#ifdef VERBOSE
//				Serial.print("currentSpeed: ");
//				Serial.println(currentSpeed);
//#endif // VERBOSE
//				if (currentSpeed < MIN_SPEED)
//				{
//					stepGroup.emergencyStop();
//				}
//				else
//				{
//					stepGroup.stop();
//				}
				stepper_1.stopAsync();
				stepper_2.stopAsync();
				stepper_3.stopAsync();
				stepper_4.stopAsync();
				stepper_5.stop();
				MilliDelay(200);
				checkedCount = 0;
				break;
			}
			}
		}

		MilliDelay(10);
	}

	switch (pageCallerId)
	{
		case PAGE_MAIN:
		{

			switch (returnType)
			{
				case RETURN_SPINDLE:
				case RETURN_SPINDLE_M3:
				{
					SerialPrint("pageMain.bt1.val=0");
					SerialPrint(nextionEnd);
					SerialPrint("pageMain.bt1.val=0");
					SerialPrint(nextionEnd);
					SerialPrint("pageMain.va0.val=0");
					SerialPrint(nextionEnd);
					SerialPrint("pageMain.bt0.val=0");
					SerialPrint(nextionEnd);
					break;
				}
				case RETURN_AXIS:
				{
					SerialPrint("pageMain.bt4.val=0");
					SerialPrint(nextionEnd);
					SerialPrint("pageMain.bt4.val=0");
					SerialPrint(nextionEnd);
					SerialPrint("pageMain.va0.val=0");
					SerialPrint(nextionEnd);
					SerialPrint("pageMain.bt7.val=0");
					SerialPrint(nextionEnd);
					break;
				}
			}

	#ifdef VERBOSE
			Serial.print("configMain.axisId: ");
			Serial.println(configMain.axisId);
	#endif // VERBOSE
			if (configSetup.keepSteppersEnabled != 1)
			{
	#ifdef VERBOSE
				Serial.print("stepper1 axisId:  ");
				Serial.println(stepperId_1);
	#endif // VERBOSEVERBOSE
				switch (stepperId_1)
				{
				case ID_AXIS_Z:
				case ID_AXIS_X:
				case ID_AXIS_3:
				case ID_AXIS_4:
				{
					SetEnable(stepperId_1, false, true);

					SerialPrint("pageMain.bt11.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMain.bt11.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					Serial.print("Disable Axis:  ");

					break;
				}
				case ID_SPINDLE:
				{
					SetEnable(stepperId_1, false, true);
					if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
					{
						SetEnable(stepperId_3, false, true);
					}

					SerialPrint("pageOne.bt10.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageOne.bt10.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					break;
				}
				}
				switch (stepperId_2)
				{
				case ID_AXIS_3:
				{
					SetEnable(stepperId_2, false, true);

					SerialPrint("pageOne.bt303.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageOne.bt303.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					break;
				}
				}
			}
			else
			{
	#ifdef VERBOSE
				Serial.print("keepSteppersEnabled:  ");
				Serial.println(configSetup.keepSteppersEnabled);
	#endif // VERBOSE
			}
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

	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		SetEnable(ID_AXIS_3, false, true);
	}

#ifdef DEBUG
	Serial.print("stepper_1.getPosition() : ");
	Serial.println(stepper_1.getPosition());

	Serial.print("stepper_2.getPosition() : ");
	Serial.println(stepper_2.getPosition());

	Serial.print("stepper_3.getPosition() : ");
	Serial.println(stepper_3.getPosition());

	Serial.print("stepper_4.getPosition() : ");
	Serial.println(stepper_4.getPosition());

	Serial.print("stepper_5.getPosition(): ");
	Serial.println(stepper_5.getPosition());
#endif // DEBUG

	checkedCount = 0;
	if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
	{
		digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
	}
}

/// <summary>
/// Return Spindle and/or axes to start position
/// </summary>
/// <comment>
/// </comment>
/// <param name="axisId">Id of axis to move</param>
void Return_StartPosition()
{
	Serial.println("Enter ReturnToStartPosition");
	if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
	{
		digitalWrite(configSetup.auxillaryPin, LOW);  // Enable
	}
	eStopTriggered = false;
	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);

	StepperGroup stepGroup{ stepperAxis_Z,stepperAxis_X,stepperAxis_M3,stepperAxis_M4,stepperSpindle };


#ifdef DEBUG
	Serial.println("Begin ReturnToStartPosition()");
	Serial.print("endPosition_Z: ");
	Serial.println(endPosition_Z);
	Serial.print("endPosition_X: ");
	Serial.println(endPosition_X);
	Serial.print("endPosition_M3: ");
	Serial.println(endPosition_M3);
	Serial.print("endPosition_M4: ");
	Serial.println(endPosition_M4);
	Serial.print("	endPosition_Spindle: ");
	Serial.println(endPosition_Spindle);
#endif // DEBUG

	stepperAxis_Z.setPosition(endPosition_Z);
	stepperAxis_Z
		.setMaxSpeed(configSetup.maxSpd_Return_Axis_Z)
		.setAcceleration(configSetup.accel_Return_Axis_Z)
		.setTargetAbs(0);

	stepperAxis_X.setPosition(endPosition_X);
	stepperAxis_X
		.setMaxSpeed(configSetup.maxSpd_Return_Axis_X)
		.setAcceleration(configSetup.accel_Return_Axis_X)
		.setTargetAbs(0);

	stepperAxis_M3.setPosition(endPosition_M3);
	stepperAxis_M3
		.setMaxSpeed(configSetup.maxSpd_Return_Axis_M3)
		.setAcceleration(configSetup.accel_Return_Axis_M3)
		.setTargetAbs(0);

	stepperAxis_M4.setPosition(endPosition_M4);
	stepperAxis_M4
		.setMaxSpeed(configSetup.maxSpd_Return_Axis_M4)
		.setAcceleration(configSetup.accel_Return_Axis_M4)
		.setTargetAbs(0);

	stepperSpindle.setPosition(endPosition_Spindle);
	stepperSpindle
		.setMaxSpeed(configSetup.maxSpd_Return_Spindle)
		.setAcceleration(configSetup.accel_Return_Spindle)
		.setTargetAbs(0);

	SetEnable(ID_AXIS_Z, true, true);
	SetEnable(ID_AXIS_X, true, true);
	SetEnable(ID_AXIS_3, true, true);
	SetEnable(ID_AXIS_4, true, true);
	SetEnable(ID_SPINDLE, true, true);

	stepGroup.startMove();

	while (stepperAxis_Z.isMoving || stepperAxis_X.isMoving || stepperAxis_M3.isMoving || stepperAxis_M4.isMoving|| stepperSpindle.isMoving)
	{
#ifdef ESTOP_ENABLED
		//if (configSetup.eStop != 0)
		if (configSetup.eStop > 9 && configSetup.eStop < 40)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
				Serial.println("                          EStop");
				stepperAxis_Z.emergencyStop();
				stepperAxis_X.emergencyStop();
				stepperAxis_M3.emergencyStop();
				stepperAxis_M4.emergencyStop();
				stepperSpindle.emergencyStop();

				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEnable(ID_AXIS_4, false, false);
				SetEStopColors(PAGE_RETURNS);
				if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
				{
					digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
				}
				return;
			}
		}
#endif // estop
		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
				case 83: // - S
				{
					stepperAxis_Z.stopAsync();
					stepperAxis_X.stopAsync();
					stepperAxis_M3.stopAsync();
					stepperAxis_M4.stopAsync();
					stepperSpindle.stop();
					MilliDelay(200);
					break;
				}
			}
		}

		MilliDelay(10);
	}
	endPosition_Z = stepperAxis_Z.getPosition();
	endPosition_X = stepperAxis_X.getPosition();
	endPosition_M3 = stepperAxis_M3.getPosition();
	endPosition_M4 = stepperAxis_M4.getPosition();
	endPosition_Spindle = stepperSpindle.getPosition();
	SetEnable(ID_AXIS_Z, false, true);
	SetEnable(ID_AXIS_X, false, true);
	SetEnable(ID_AXIS_3, false, true);
	SetEnable(ID_AXIS_4, false, true);
	SetEnable(ID_SPINDLE, false, true);

	MilliDelay(500);
	switch (pageCallerId)
	{
		case PAGE_MULTI:
		{
			SerialPrint("pageMulti.bt0.val=0");
			SerialPrint(nextionEnd);

			SerialPrint("pageMulti.bt1.val=0");
			SerialPrint(nextionEnd);

			SerialPrint("pageMulti.bt1.val=0");
			SerialPrint(nextionEnd);

			SerialPrint("pageMulti.bt2.val=0");
			SerialPrint(nextionEnd);

			SerialPrint("pageMulti.va0.val=0");
			SerialPrint(nextionEnd);
	#ifdef VERBOSE
			Serial.print("configMulti.axisId: ");
			Serial.println(configMulti.axisId);
	#endif // VERBOSE
			if (configSetup.keepSteppersEnabled != 1)
			{
	#ifdef VERBOSE
				Serial.print("stepper1 axisId:  ");
				Serial.println(stepperId_1);
	#endif // VERBOSE
				switch (stepperId_1)
				{
				case ID_AXIS_Z:
				{
					SetEnable(ID_AXIS_Z, false, true);

					SerialPrint("pageMulti.bt1301.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMulti.bt1301.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					Serial.print("Disable Z:  ");

					break;
				}
				case ID_AXIS_X:
				{
					SetEnable(ID_AXIS_X, false, true);

					SerialPrint("pageMulti.bt1302.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMulti.bt1302.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					break;
				}
				case ID_AXIS_3:
				{
					SetEnable(ID_AXIS_3, false, true);

					SerialPrint("pageMulti.bt1303.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMulti.bt1303.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					break;
				}
				case ID_AXIS_4:
				{
					SetEnable(ID_AXIS_4, false, true);
					SerialPrint("pageMulti.bt1304.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMulti.bt1304.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					break;
				}
				case ID_SPINDLE:
				{
					SetEnable(ID_SPINDLE, false, true);

					SerialPrint("pageMulti.bt1300.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMulti.bt1300.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					break;
				}
				}
				switch (stepperId_2)
				{
				case ID_AXIS_Z:
				{
					SetEnable(ID_AXIS_Z, false, true);

					SerialPrint("pageMulti.bt1301.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMulti.bt1301.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					Serial.print("Disable Z:  ");

					break;
				}
				case ID_AXIS_X:
				{
					SetEnable(ID_AXIS_X, false, true);

					SerialPrint("pageMulti.bt1302.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMulti.bt1302.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					break;
				}
				case ID_AXIS_3:
				{
					SetEnable(ID_AXIS_3, false, true);

					SerialPrint("pageMulti.bt1303.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMulti.bt1303.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					break;
				}
				case ID_AXIS_4:
				{
					SetEnable(ID_AXIS_4, false, true);
					SerialPrint("pageMulti.bt1304.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMulti.bt1304.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					break;
				}
				case ID_SPINDLE:
				{
					SetEnable(ID_SPINDLE, false, true);

					SerialPrint("pageMulti.bt1300.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMulti.bt1300.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					break;
				}
				}
				switch (stepperId_3)
				{
				case ID_AXIS_Z:
				{
					SetEnable(ID_AXIS_Z, false, true);

					SerialPrint("pageMulti.bt1301.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMulti.bt1301.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					Serial.print("Disable Z:  ");

					break;
				}
				case ID_AXIS_X:
				{
					SetEnable(ID_AXIS_X, false, true);

					SerialPrint("pageMulti.bt1302.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMulti.bt1302.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					break;
				}
				case ID_AXIS_3:
				{
					SetEnable(ID_AXIS_3, false, true);
					SerialPrint("pageMulti.bt1303.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMulti.bt1303.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					break;
				}
				case ID_AXIS_4:
				{
					SetEnable(ID_AXIS_4, false, true);
					SerialPrint("pageMulti.bt1304.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMulti.bt1304.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					break;
				}
				case ID_SPINDLE:
				{
					SetEnable(ID_SPINDLE, false, true);

					SerialPrint("pageMulti.bt1300.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMulti.bt1300.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					break;
				}
				}
				switch (stepperId_4)
				{
				case ID_AXIS_Z:
				{
					SetEnable(ID_AXIS_Z, false, true);

					SerialPrint("pageMulti.bt1301.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMulti.bt1301.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					Serial.print("Disable Z:  ");

					break;
				}
				case ID_AXIS_X:
				{
					SetEnable(ID_AXIS_X, false, true);

					SerialPrint("pageMulti.bt1302.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMulti.bt1302.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					break;
				}
				case ID_AXIS_3:
				{
					SetEnable(ID_AXIS_3, false, true);
					SerialPrint("pageMulti.bt1303.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMulti.bt1303.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					break;
				}
				case ID_AXIS_4:
				{
					SetEnable(ID_AXIS_4, false, true);
					SerialPrint("pageMulti.bt1304.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMulti.bt1304.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					break;
				}
				case ID_SPINDLE:
				{
					SetEnable(ID_SPINDLE, false, true);

					SerialPrint("pageMulti.bt1300.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMulti.bt1300.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					break;
				}
				}
				switch (stepperId_5)
				{
				case ID_AXIS_Z:
				{
					SetEnable(ID_AXIS_Z, false, true);

					SerialPrint("pageMulti.bt1301.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMulti.bt1301.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					Serial.print("Disable Z:  ");

					break;
				}
				case ID_AXIS_X:
				{
					SetEnable(ID_AXIS_X, false, true);

					SerialPrint("pageMulti.bt1302.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMulti.bt1302.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					break;
				}
				case ID_AXIS_3:
				{
					SetEnable(ID_AXIS_3, false, true);
					SerialPrint("pageMulti.bt1303.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMulti.bt1303.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					break;
				}
				case ID_AXIS_4:
				{
					SetEnable(ID_AXIS_4, false, true);
					SerialPrint("pageMulti.bt1304.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMulti.bt1304.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					break;
				}
				case ID_SPINDLE:
				{
					SetEnable(ID_SPINDLE, false, true);

					SerialPrint("pageMulti.bt1300.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMulti.bt1300.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					break;
				}
				}
			}
			else
			{
	#ifdef DEBUG
				Serial.print("keepSteppersEnabled:  ");
				Serial.println(configSetup.keepSteppersEnabled);
	#endif // DEBUG
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
	#ifdef VERBOSE
			Serial.print("configMulti.axisId: ");
			Serial.println(configMulti.axisId);
	#endif // VERBOSE
			if (configSetup.keepSteppersEnabled != 1)
			{
	#ifdef VERBOSE
				Serial.print("stepper1 axisId:  ");
				Serial.println(stepperId_1);
	#endif // VERBOSE
				switch (stepperId_1)
				{
					case ID_AXIS_Z:
					{
						SetEnable(ID_AXIS_Z, false, true);

						SerialPrint("pageOne.bt301.val=0"); // 0 = disabled
						SerialPrint("\xFF\xFF\xFF");
						SerialPrint("pageOne.bt301.txt="); // 0 = disabled
						SerialPrint("\x22");
						SerialPrint("Disabled");
						SerialPrint("\x22\xFF\xFF\xFF");
						Serial.print("Disable Z:  ");

						break;
					}
					case ID_AXIS_X:
					{
						SetEnable(ID_AXIS_X, false, true);

						SerialPrint("pageOne.bt302.val=0"); // 0 = disabled
						SerialPrint("\xFF\xFF\xFF");
						SerialPrint("pageOne.bt302.txt="); // 0 = disabled
						SerialPrint("\x22");
						SerialPrint("Disabled");
						SerialPrint("\x22\xFF\xFF\xFF");
						break;
					}
					case ID_AXIS_3:
					{
						SetEnable(ID_AXIS_3, false, true);

						SerialPrint("pageOne.bt303.val=0"); // 0 = disabled
						SerialPrint("\xFF\xFF\xFF");
						SerialPrint("pageOne.bt303.txt="); // 0 = disabled
						SerialPrint("\x22");
						SerialPrint("Disabled");
						SerialPrint("\x22\xFF\xFF\xFF");
						break;
					}
					case ID_AXIS_4:
					{
						SetEnable(ID_AXIS_4, false, true);
						SerialPrint("pageOne.bt304.val=0"); // 0 = disabled
						SerialPrint("\xFF\xFF\xFF");
						SerialPrint("pageOne.bt304.txt="); // 0 = disabled
						SerialPrint("\x22");
						SerialPrint("Disabled");
						SerialPrint("\x22\xFF\xFF\xFF");
						break;
					}
					case ID_SPINDLE:
					{
						SetEnable(ID_SPINDLE, false, true);

						SerialPrint("pageOne.bt300.val=0"); // 0 = disabled
						SerialPrint("\xFF\xFF\xFF");
						SerialPrint("pageOne.bt300.txt="); // 0 = disabled
						SerialPrint("\x22");
						SerialPrint("Disabled");
						SerialPrint("\x22\xFF\xFF\xFF");
						if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
						{
							SetEnable(ID_AXIS_3, false, true);

							SerialPrint("pageOne.bt303.val=0"); // 0 = disabled
							SerialPrint("\xFF\xFF\xFF");
							SerialPrint("pageOne.bt303.txt="); // 0 = disabled
							SerialPrint("\x22");
							SerialPrint("Disabled");
							//SerialPrint("\x22\xFF\xFF\xFF");
						}
						break;
					}
				}
				//switch (stepperId_2)
				//{
				//case ID_AXIS_3:
				//{
				//	SetEnable(ID_AXIS_3, false, true);

				//	SerialPrint("pageOne.bt303.val=0"); // 0 = disabled
				//	SerialPrint("\xFF\xFF\xFF");
				//	SerialPrint("pageOne.bt303.txt="); // 0 = disabled
				//	SerialPrint("\x22");
				//	SerialPrint("Disabled");
				//	SerialPrint("\x22\xFF\xFF\xFF");
				//	break;
				//}
				//}
			}
			else
			{
	#ifdef DEBUG
				Serial.print("keepSteppersEnabled:  ");
				Serial.println(configSetup.keepSteppersEnabled);
	#endif // DEBUG
			}
			break;
		}
		case PAGE_MAIN:
		{
			// See ReturnToStartPosition_MultiMain
			switch (returnType)
			{
			case RETURN_SPINDLE:
			case RETURN_SPINDLE_M3:
			{
				SerialPrint("pageMain.bt1.val=0");
				SerialPrint(nextionEnd);
				SerialPrint("pageMain.bt1.val=0");
				SerialPrint(nextionEnd);
				SerialPrint("pageMain.va0.val=0");
				SerialPrint(nextionEnd);
				SerialPrint("pageMain.bt0.val=0");
				SerialPrint(nextionEnd);
				break;
			}
			case RETURN_AXIS:
			{
				SerialPrint("pageMain.bt4.val=0");
				SerialPrint(nextionEnd);
				SerialPrint("pageMain.bt4.val=0");
				SerialPrint(nextionEnd);
				SerialPrint("pageMain.va0.val=0");
				SerialPrint(nextionEnd);
				SerialPrint("pageMain.bt7.val=0");
				SerialPrint(nextionEnd);
				break;
			}
			}

	#ifdef VERBOSE
			Serial.print("configMain.axisId: ");
			Serial.println(configMain.axisId);
	#endif // VERBOSE
			if (configSetup.keepSteppersEnabled != 1)
			{
	#ifdef VERBOSE
				Serial.print("stepper1 axisId:  ");
				Serial.println(stepperId_1);
	#endif // VERBOSE
				switch (stepperId_1)
				{
				case ID_AXIS_Z:
				case ID_AXIS_X:
				case ID_AXIS_3:
				case ID_AXIS_4:
				{
					SetEnable(stepperId_1, false, true);

					SerialPrint("pageMain.bt11.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMain.bt11.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					Serial.print("Disable Axis:  ");

					break;
				}
				case ID_SPINDLE:
				{
					SetEnable(stepperId_1, false, true);
					if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
					{
						SetEnable(stepperId_3, false, true);
					}

					SerialPrint("pageOne.bt10.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageOne.bt10.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					break;
				}
				}
				switch (stepperId_2)
				{
				case ID_AXIS_3:
				{
					// M3 is not the active axis, so set Enabled/Disabled for other pages
					SetEnable(stepperId_2, false, true);

					SerialPrint("pageOne.bt303.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageOne.bt303.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					break;
				}
				}
			}
			else
			{
	#ifdef DEBUG
				Serial.print("keepSteppersEnabled:  ");
				Serial.println(configSetup.keepSteppersEnabled);
	#endif // DEBUG
			}
			break;
		}
		case PAGE_INDEX:
		{
	#ifdef VERBOSE
			Serial.print("pageIndex axisId: ");
			Serial.println(configIndex_Prime.axisId);
	#endif // VERBOSE
			SerialPrint("pageIndex.bt1.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageIndex.va0.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageIndex.bt0.val=0");
			SerialPrint(nextionEnd);

			if (configSetup.keepSteppersEnabled != 1)
			{
				SetEnable(configIndex_Prime.axisId, false, true);
				switch (configIndex_Prime.axisId)
				{
				case ID_SPINDLE:
				{
					SerialPrint("pageIndex.bt10.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageIndex.bt10.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					break;
				}
				case ID_AXIS_3:
				case ID_AXIS_4:
				{
					SerialPrint("pageIndex.bt11.val=0"); // 0 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageIndex.bt11.txt="); // 0 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");
					break;
				}
				}
			}

			break;
		}
		case PAGE_ROSE:
		{
	#ifdef VERBOSE
			Serial.print("pageRose axisId: ");
			Serial.println(configRose.axisId);
	#endif // VERBOSE
			SerialPrint("pageRose.bt1.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageRose.va0.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageRose.bt0.val=0");
			SerialPrint(nextionEnd);

			SetEnable(configRose.axisId, false, true);
			SetEnable(ID_SPINDLE, false, true);
			if (configSetup.keepSteppersEnabled != 1)
			{


				SerialPrint("pageRose.bt10.val=0"); // 0 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageRose.bt10.txt="); // 0 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");

				SerialPrint("pageRose.bt11.val=0"); // 0 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageRose.bt11.txt="); // 0 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");

			}

			break;
		}
		case PAGE_SYNC:
		{
	#ifdef VERBOSE
			Serial.print("pageSync axisId: ");
			Serial.println(configSync.axisId);
	#endif // VERBOSE
			SerialPrint("pageSync.bt1.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageSync.va0.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageSync.bt0.val=0");
			SerialPrint(nextionEnd);

			SetEnable(configSync.axisId, false, true);
			SetEnable(ID_SPINDLE, false, true);
			if (configSetup.keepSteppersEnabled != 1)
			{


				SerialPrint("pageSync.bt10.val=0"); // 0 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageSync.bt10.txt="); // 0 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");

				SerialPrint("pageSync.bt11.val=0"); // 0 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageSync.bt11.txt="); // 0 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");

			}

			break;
		}
		case PAGE_REC:
		{
	#ifdef VERBOSE
			Serial.print("pageRec axisId: ");
			Serial.println(configRec.axisId);
	#endif // VERBOSE
			SerialPrint("pageRec.bt1.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageRec.va0.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageRec.bt0.val=0");
			SerialPrint(nextionEnd);

			SetEnable(configRec.axisId, false, true);
			SetEnable(ID_SPINDLE, false, true);
			if (configSetup.keepSteppersEnabled != 1)
			{


				SerialPrint("pageRec.bt10.val=0"); // 0 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageRec.bt10.txt="); // 0 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");

				SerialPrint("pageRec.bt11.val=0"); // 0 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageRec.bt11.txt="); // 0 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");

			}

			break;
		}
		case PAGE_GRK:
		{
	#ifdef VERBOSE
			Serial.print("pageGrk axisId: ");
			Serial.println(configGreekKey.axisId);
	#endif // VERBOSE
			SerialPrint("pageGrk.bt1.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageGrk.va0.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageGrk.bt0.val=0");
			SerialPrint(nextionEnd);

			SetEnable(configGreekKey.axisId, false, true);
			SetEnable(ID_SPINDLE, false, true);
			if (configSetup.keepSteppersEnabled != 1)
			{
				SerialPrint("pageGrk.bt10.val=0"); // 0 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageGrk.bt10.txt="); // 0 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");

				SerialPrint("pageGrk.bt11.val=0"); // 0 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageGrk.bt11.txt="); // 0 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
			}

			break;
		}
		case PAGE_PROGRAM:
		{
	#ifdef VERBOSE
			Serial.print("pageProgram axisId: ");
			Serial.println(configGreekKey.axisId);
	#endif // VERBOSE
			SerialPrint("pageProgram.bt1.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageProgram.va0.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageProgram.bt0.val=0");
			SerialPrint(nextionEnd);

			SetEnable(configGreekKey.axisId, false, true);
			SetEnable(ID_SPINDLE, false, true);
			if (configSetup.keepSteppersEnabled != 1)
			{
				SerialPrint("pageProgram.bt10.val=0"); // 0 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageProgram.bt10.txt="); // 0 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");

				SerialPrint("pageProgram.bt11.val=0"); // 0 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageProgram.bt11.txt="); // 0 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
			}

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

			SetEnable(ID_AXIS_Z, false, true);
			SetEnable(ID_AXIS_X, false, true);
			SetEnable(ID_AXIS_3, false, true);
			SetEnable(ID_AXIS_4, false, true);
			break;
		}
	}

	stepperAxis_X.setPosition(0);
	stepperAxis_Z.setPosition(0);
	stepperAxis_M3.setPosition(0);
	stepperAxis_M4.setPosition(0);
	stepperSpindle.setPosition(0);
	endPosition_Axis = 0;
	if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
	{
		digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
	}
}

void Return_Main()
{
	switch (returnType)
	{
	case RETURN_SPINDLE: // Spindle only
	{
#ifdef DEBUG
		Serial.print("1-Return-spindle_Position: ");
		Serial.println(spindle_Position);
#endif // DEBUG
		checkedCount = 1;
		stepperId_1 = ID_SPINDLE;
		stepper1_step = PIN_SPINDLE_STEP;
		stepper1_dir = PIN_SPINDLE_DIR;
		stepperPosition_1 = spindle_Position;// *(-1);
		break;
	}
	case RETURN_SPINDLE_M3: // Spindle and M3-Synchro
	{
#ifdef DEBUG
		Serial.print("1-Return-spindle_Position: ");
		Serial.println(spindle_Position);
#endif // DEBUG

		checkedCount = 2;
		stepperId_1 = ID_SPINDLE;
		stepper1_step = PIN_SPINDLE_STEP;
		stepper1_dir = PIN_SPINDLE_DIR;
		stepperPosition_1 = spindle_Position;// *(-1);
#ifdef DEBUG
		Serial.print("2-Return-stepperPosition_1: ");
		Serial.println(stepperPosition_1);

		Serial.print("3-Return-m3_Position: ");
		Serial.println(m3_Position);
#endif // DEBUG

		stepperId_2 = ID_AXIS_3;
		stepper2_step = PIN_AXIS_3_STEP;
		stepper2_dir = PIN_AXIS_3_DIR;

		stepperPosition_2 = m3_Position;// *(-1);

#ifdef DEBUG
		Serial.print("4-Return-stepperPosition_2: ");
		Serial.println(stepperPosition_2);
#endif // DEBUG
		break;
	}
	case RETURN_AXIS: // Axis only
	{
		switch (configMain.axisId)
		{
		case ID_AXIS_Z:
		{
			stepperId_1 = ID_AXIS_Z;
			stepper1_step = PIN_AXIS_Z_STEP;
			stepper1_dir = PIN_AXIS_Z_DIR;
			break;
		}
		case ID_AXIS_X:
		{
			stepperId_1 = ID_AXIS_X;
			stepper1_step = PIN_AXIS_X_STEP;
			stepper1_dir = PIN_AXIS_X_DIR;
			break;
		}
		case ID_AXIS_3:
		{
			stepperId_1 = ID_AXIS_3;
			stepper1_step = PIN_AXIS_3_STEP;
			stepper1_dir = PIN_AXIS_3_DIR;
			break;
		}
		case ID_AXIS_4:
		{
			stepperId_1 = ID_AXIS_4;
			stepper1_step = PIN_AXIS_4_STEP;
			stepper1_dir = PIN_AXIS_4_DIR;
			break;
		}
		}
#ifdef DEBUG
		Serial.print("3-Return-axis_Position: ");
		Serial.println(axis_Position);
#endif // DEBUG

		checkedCount = 1;

		stepperPosition_1 = axis_Position;// *(-1);

#ifdef DEBUG
		Serial.print("3-Return-stepperPosition_1: ");
		Serial.println(stepperPosition_1);
#endif // DEBUG

		break;
	}
	}

	ReturnToStartPosition_MainPage();
}


//////////////////////////////////////////

/// <summary>
/// PageMulti: Return Spindle and/or axes to start position
/// </summary>
/// <comment>
/// </comment>
/// <param name="axisId">Id of axis to move</param>
void ReturnToStartPosition_Multi()
{
	if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
	{
		digitalWrite(configSetup.auxillaryPin, LOW);  // Enable
	}
	eStopTriggered = false;
	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);

	int32_t maxSpeed_1 = 10000;
	int32_t maxSpeed_2 = 10000;
	int32_t maxSpeed_3 = 10000;
	int32_t maxSpeed_4 = 10000;
	int32_t maxSpeed_5 = 10000;
	int32_t accel_1 = 10000;
	int32_t accel_2 = 10000;
	int32_t accel_3 = 10000;
	int32_t accel_4 = 10000;
	int32_t accel_5 = 10000;
	////int curPosition = 0;
#ifdef DEBUG
	Serial.print("stepper1_step:  ");
	Serial.println(stepper1_step);
	Serial.print("stepper1_dir:  ");
	Serial.println(stepper1_dir);
	Serial.print("stepperPosition_1:  ");
	Serial.println(stepperPosition_1);

	Serial.print("stepper2_step:  ");
	Serial.println(stepper2_step);
	Serial.print("stepper2_dir:  ");
	Serial.println(stepper2_dir);
	Serial.print("stepperPosition_2:  ");
	Serial.println(stepperPosition_2);

	//Serial.print("stepper3_step:  ");
	//Serial.println(stepper3_step);
	//Serial.print("stepper3_dir:  ");
	//Serial.println(stepper3_dir);
	//Serial.print("stepperPosition_3:  ");
	//Serial.println(stepperPosition_3);

	//Serial.print("stepper4_step:  ");
	//Serial.println(stepper4_step);
	//Serial.print("stepper4_dir:  ");
	//Serial.println(stepper4_dir);

	//Serial.print("stepper5_step:  ");
	//Serial.println(stepper5_step);
	//Serial.print("stepper5_dir:  ");
	//Serial.println(stepper5_dir);
	Serial.println("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
#endif // DEBUG
	//StepControl stepController;
	StepperGroup stepGroup;
	Stepper stepper_1(stepper1_step, stepper1_dir);
	Stepper stepper_2(stepper2_step, stepper2_dir);
	Stepper stepper_3(stepper3_step, stepper3_dir);
	Stepper stepper_4(stepper4_step, stepper4_dir);
	Stepper stepper_5(stepper5_step, stepper5_dir);

	StepperGroup stepGroup1{ stepper_1 };
	StepperGroup stepGroup2{ stepper_1, stepper_2 };
	StepperGroup stepGroup3{ stepper_1, stepper_2, stepper_3 };
	StepperGroup stepGroup4{ stepper_1, stepper_2, stepper_3, stepper_4 };
	StepperGroup stepGroup5{ stepper_1, stepper_2, stepper_3, stepper_4, stepper_5 };

	stepper_1.setPosition(0);
	stepper_2.setPosition(0);
	stepper_3.setPosition(0);
	stepper_4.setPosition(0);
	stepper_5.setPosition(0);

	// Set unused stepper values
	switch (checkedCount)
	{
		case 1:
		{
			stepperId_2 = 44;
			stepperId_3 = 45;
			stepperId_4 = 46;
			stepperId_5 = 47;

			stepper2_step = 50;
			stepper2_dir = 51;

			stepper3_step = 52;
			stepper3_dir = 53;

			stepper4_step = 54;
			stepper4_dir = 55;

			stepper5_step = 56;
			stepper5_dir = 57;

			break;
		}
		case 2:
		{
			stepperId_3 = 45;
			stepperId_4 = 46;
			stepperId_5 = 47;

			stepper3_step = 52;
			stepper3_dir = 53;

			stepper4_step = 54;
			stepper4_dir = 55;

			stepper5_step = 56;
			stepper5_dir = 57;

			break;
		}
		case 3:
		{
			stepperId_4 = 46;
			stepperId_5 = 47;

			stepper4_step = 54;
			stepper4_dir = 55;

			stepper5_step = 56;
			stepper5_dir = 57;

			break;
		}
		case 4:
		{
			stepperId_5 = 47;

			stepper5_step = 56;
			stepper5_dir = 57;

			break;
		}
	}

//#ifdef VERBOSE
//	Serial.print("stepperId_1:  ");
//	Serial.println(stepperId_1);
//	Serial.print("stepper1_step:  ");
//	Serial.println(stepper1_step);
//	Serial.print("stepper1_dir:  ");
//	Serial.println(stepper1_dir);
//
//	Serial.print("stepperId_2:  ");
//	Serial.println(stepperId_2);
//	Serial.print("stepper2_step:  ");
//	Serial.println(stepper2_step);
//	Serial.print("stepper2_dir:  ");
//	Serial.println(stepper2_dir);
//
//	Serial.print("stepperId_3:  ");
//	Serial.println(stepperId_3);
//	Serial.print("stepper3_step:  ");
//	Serial.println(stepper3_step);
//	Serial.print("stepper3_dir:  ");
//	Serial.println(stepper3_dir);
//
//	Serial.print("stepperId_4:  ");
//	Serial.println(stepperId_4);
//	Serial.print("stepper4_step:  ");
//	Serial.println(stepper4_step);
//	Serial.print("stepper4_dir:  ");
//	Serial.println(stepper4_dir);
//
//	Serial.print("stepperId_5:  ");
//	Serial.println(stepperId_5);
//	Serial.print("stepper5_step:  ");
//	Serial.println(stepper5_step);
//	Serial.print("stepper5_dir:  ");
//	Serial.println(stepper5_dir);
//#endif // VERBOSE

	// Set Speed and Accel
	switch (stepperId_1)
	{
		case ID_AXIS_Z:
		{
			maxSpeed_1 = configSetup.maxSpd_Return_Axis_Z;
			accel_1 = configSetup.accel_Return_Axis_Z;
			break;
		}
		case ID_AXIS_X:
		{
			maxSpeed_1 = configSetup.maxSpd_Return_Axis_X;
			accel_1 = configSetup.accel_Return_Axis_X;
			break;
		}
		case ID_AXIS_3:
		{
			maxSpeed_1 = configSetup.maxSpd_Return_Axis_M3;
			accel_1 = configSetup.accel_Return_Axis_M3;
			break;
		}
		case ID_AXIS_4:
		{
			maxSpeed_1 = configSetup.maxSpd_Return_Axis_M4;
			accel_1 = configSetup.accel_Return_Axis_M4;
			break;
		}
		case ID_SPINDLE:
		{
			maxSpeed_1 = configSetup.maxSpd_Return_Spindle;
			accel_1 = configSetup.accel_Return_Spindle;
			MilliDelay(15);
			break;
		}
	}
	switch (stepperId_2)
	{
	case ID_AXIS_Z:
	{
		maxSpeed_2 = configSetup.maxSpd_Return_Axis_Z;
		accel_2 = configSetup.accel_Return_Axis_Z;
		break;
	}
	case ID_AXIS_X:
	{
		maxSpeed_2 = configSetup.maxSpd_Return_Axis_X;
		accel_2 = configSetup.accel_Return_Axis_X;
		break;
	}
	case ID_AXIS_3:
	{
		maxSpeed_2 = configSetup.maxSpd_Return_Axis_M3;
		accel_2 = configSetup.accel_Return_Axis_M3;
		break;
	}
	case ID_AXIS_4:
	{
		maxSpeed_2 = configSetup.maxSpd_Return_Axis_M4;
		accel_2 = configSetup.accel_Return_Axis_M4;
		break;
	}
	case ID_SPINDLE:
	{
		maxSpeed_2 = configSetup.maxSpd_Return_Spindle;
		accel_2 = configSetup.accel_Return_Spindle;
		MilliDelay(15);
		break;
	}
	}
	switch (stepperId_3)
	{
	case ID_AXIS_Z:
	{
		maxSpeed_3 = configSetup.maxSpd_Return_Axis_Z;
		accel_3 = configSetup.accel_Return_Axis_Z;
		break;
	}
	case ID_AXIS_X:
	{
		maxSpeed_3 = configSetup.maxSpd_Return_Axis_X;
		accel_3 = configSetup.accel_Return_Axis_X;
		break;
	}
	case ID_AXIS_3:
	{
		maxSpeed_3 = configSetup.maxSpd_Return_Axis_M3;
		accel_3 = configSetup.accel_Return_Axis_M3;
		break;
	}
	case ID_AXIS_4:
	{
		maxSpeed_3 = configSetup.maxSpd_Return_Axis_M4;
		accel_3 = configSetup.accel_Return_Axis_M4;
		break;
	}
	case ID_SPINDLE:
	{
		maxSpeed_3 = configSetup.maxSpd_Return_Spindle;
		accel_3 = configSetup.accel_Return_Spindle;
		MilliDelay(15);
		break;
	}
	}
	switch (stepperId_4)
	{
	case ID_AXIS_Z:
	{
		maxSpeed_4 = configSetup.maxSpd_Return_Axis_Z;
		accel_4 = configSetup.accel_Return_Axis_Z;
		break;
	}
	case ID_AXIS_X:
	{
		maxSpeed_4 = configSetup.maxSpd_Return_Axis_X;
		accel_4 = configSetup.accel_Return_Axis_X;
		break;
	}
	case ID_AXIS_3:
	{
		maxSpeed_4 = configSetup.maxSpd_Return_Axis_M3;
		accel_4 = configSetup.accel_Return_Axis_M3;
		break;
	}
	case ID_AXIS_4:
	{
		maxSpeed_4 = configSetup.maxSpd_Return_Axis_M4;
		accel_4 = configSetup.accel_Return_Axis_M4;
		break;
	}
	case ID_SPINDLE:
	{
		maxSpeed_4 = configSetup.maxSpd_Return_Spindle;
		accel_4 = configSetup.accel_Return_Spindle;
		MilliDelay(15);
		break;
	}

	}
	switch (stepperId_5)
	{
	case ID_AXIS_Z:
	{
		maxSpeed_5 = configSetup.maxSpd_Return_Axis_Z;
		accel_5 = configSetup.accel_Return_Axis_Z;
		break;
	}
	case ID_AXIS_X:
	{
		maxSpeed_5 = configSetup.maxSpd_Return_Axis_X;
		accel_5 = configSetup.accel_Return_Axis_X;
		break;
	}
	case ID_AXIS_3:
	{
		maxSpeed_5 = configSetup.maxSpd_Return_Axis_M3;
		accel_5 = configSetup.accel_Return_Axis_M3;
		break;
	}
	case ID_AXIS_4:
	{
		maxSpeed_5 = configSetup.maxSpd_Return_Axis_M4;
		accel_5 = configSetup.accel_Return_Axis_M4;
		break;
	}
	case ID_SPINDLE:
	{
		maxSpeed_5 = configSetup.maxSpd_Return_Spindle;
		accel_5 = configSetup.accel_Return_Spindle;
		MilliDelay(15);
		break;
	}
	}


	stepper_1
		.setMaxSpeed(maxSpeed_1)
		.setAcceleration(accel_1)
		.setTargetAbs(stepperPosition_1);

	stepper_2
		.setMaxSpeed(maxSpeed_2)
		.setAcceleration(accel_2)
		.setTargetAbs(stepperPosition_2);

	stepper_3
		.setMaxSpeed(maxSpeed_3)
		.setAcceleration(accel_3)
		.setTargetAbs(stepperPosition_3);

	stepper_4
		.setMaxSpeed(maxSpeed_4)
		.setAcceleration(accel_4)
		.setTargetAbs(stepperPosition_4);

	stepper_5
		.setMaxSpeed(maxSpeed_5)
		.setAcceleration(accel_5)
		.setTargetAbs(stepperPosition_5);

	switch (checkedCount)
	{
	case 1:
	{
		SetEnable(stepperId_1, true, true);


		stepGroup1.startMove();
		break;
	}
	case 2:
	{
		SetEnable(stepperId_1, true, true);
		SetEnable(stepperId_2, true, true);

		Serial.print("Start Move.....");
		
		stepGroup2.startMove();
		Serial.print("Move Started.....");
		break;
	}
	case 3:
	{
		SetEnable(stepperId_1, true, true);
		SetEnable(stepperId_2, true, true);
		SetEnable(stepperId_3, true, true);

		stepGroup3.startMove();
		break;
	}
	case 4:
	{
		SetEnable(stepperId_1, true, true);
		SetEnable(stepperId_2, true, true);
		SetEnable(stepperId_3, true, true);
		SetEnable(stepperId_4, true, true);

		stepGroup4.startMove();
		break;
	}
	case 5:
	{
		SetEnable(stepperId_1, true, true);
		SetEnable(stepperId_2, true, true);
		SetEnable(stepperId_3, true, true);
		SetEnable(stepperId_4, true, true);
		SetEnable(stepperId_5, true, true);

		stepGroup5.startMove();
		break;
	}
	}

	while (stepper_1.isMoving)
	{
		Serial.println("Moving");
#ifdef ESTOP_ENABLED
		if (configSetup.eStop > 9 && configSetup.eStop < 40)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
#ifdef DEBUG
				Serial.println("                          EStop");
#endif // DEBUG


				stepper_1.emergencyStop();
				stepper_2.emergencyStop();
				stepper_3.emergencyStop();
				stepper_4.emergencyStop();
				stepper_5.emergencyStop();
				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEnable(ID_AXIS_4, false, false);
				SetEStopColors(PAGE_RETURNS);
				if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
				{
					digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
				}
				return;
			}
		}
#endif // estop
		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
				case 83: // - S
				{
					stepper_1.stopAsync();
					stepper_2.stopAsync();
					stepper_3.stopAsync();
					stepper_4.stopAsync();
					stepper_5.stop();
					MilliDelay(200);
					checkedCount = 0;
					break;
				}
			}
		}

		MilliDelay(10);
	}

	Serial.print("pageCallerId: ");
	Serial.println(pageCallerId);
	switch (pageCallerId)
	{
	case PAGE_MULTI:
	{
		SerialPrint("pageMulti.bt0.val=0");
		SerialPrint(nextionEnd);

		SerialPrint("pageMulti.bt1.val=0");
		SerialPrint(nextionEnd);

		SerialPrint("pageMulti.bt1.val=0");
		SerialPrint(nextionEnd);

		SerialPrint("pageMulti.bt2.val=0");
		SerialPrint(nextionEnd);

		SerialPrint("pageMulti.va0.val=0");
		SerialPrint(nextionEnd);
#ifdef VERBOSE
		Serial.print("configMulti.axisId: ");
		Serial.println(configMulti.axisId);
#endif // VERBOSE
		if (configSetup.keepSteppersEnabled != 1)
		{
#ifdef VERBOSE
			Serial.print("stepper1 axisId:  ");
			Serial.println(stepperId_1);
#endif // VERBOSE
			switch (stepperId_1)
			{
			case ID_AXIS_Z:
			{
				SetEnable(ID_AXIS_Z, false, true);

				SerialPrint("pageMulti.bt1301.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageMulti.bt1301.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				Serial.print("Disable Z:  ");

				break;
			}
			case ID_AXIS_X:
			{
				SetEnable(ID_AXIS_X, false, true);

				SerialPrint("pageMulti.bt1302.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageMulti.bt1302.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				break;
			}
			case ID_AXIS_3:
			{
				SetEnable(ID_AXIS_3, false, true);

				SerialPrint("pageMulti.bt1303.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageMulti.bt1303.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				break;
			}
			case ID_AXIS_4:
			{
				SetEnable(ID_AXIS_4, false, true);
				SerialPrint("pageMulti.bt1304.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageMulti.bt1304.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				break;
			}
			case ID_SPINDLE:
			{
				SetEnable(ID_SPINDLE, false, true);

				SerialPrint("pageMulti.bt1300.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageMulti.bt1300.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				break;
			}
			}
			switch (stepperId_2)
			{
			case ID_AXIS_Z:
			{
				SetEnable(ID_AXIS_Z, false, true);

				SerialPrint("pageMulti.bt1301.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageMulti.bt1301.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				Serial.print("Disable Z:  ");

				break;
			}
			case ID_AXIS_X:
			{
				SetEnable(ID_AXIS_X, false, true);

				SerialPrint("pageMulti.bt1302.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageMulti.bt1302.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				break;
			}
			case ID_AXIS_3:
			{
				SetEnable(ID_AXIS_3, false, true);

				SerialPrint("pageMulti.bt1303.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageMulti.bt1303.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				break;
			}
			case ID_AXIS_4:
			{
				SetEnable(ID_AXIS_4, false, true);
				SerialPrint("pageMulti.bt1304.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageMulti.bt1304.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				break;
			}
			case ID_SPINDLE:
			{
				SetEnable(ID_SPINDLE, false, true);

				SerialPrint("pageMulti.bt1300.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageMulti.bt1300.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				break;
			}
			}
			switch (stepperId_3)
			{
			case ID_AXIS_Z:
			{
				SetEnable(ID_AXIS_Z, false, true);

				SerialPrint("pageMulti.bt1301.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageMulti.bt1301.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				Serial.print("Disable Z:  ");

				break;
			}
			case ID_AXIS_X:
			{
				SetEnable(ID_AXIS_X, false, true);

				SerialPrint("pageMulti.bt1302.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageMulti.bt1302.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				break;
			}
			case ID_AXIS_3:
			{
				SetEnable(ID_AXIS_3, false, true);
				SerialPrint("pageMulti.bt1303.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageMulti.bt1303.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				break;
			}
			case ID_AXIS_4:
			{
				SetEnable(ID_AXIS_4, false, true);
				SerialPrint("pageMulti.bt1304.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageMulti.bt1304.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				break;
			}
			case ID_SPINDLE:
			{
				SetEnable(ID_SPINDLE, false, true);

				SerialPrint("pageMulti.bt1300.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageMulti.bt1300.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				break;
			}
			}
			switch (stepperId_4)
			{
			case ID_AXIS_Z:
			{
				SetEnable(ID_AXIS_Z, false, true);

				SerialPrint("pageMulti.bt1301.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageMulti.bt1301.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				Serial.print("Disable Z:  ");

				break;
			}
			case ID_AXIS_X:
			{
				SetEnable(ID_AXIS_X, false, true);

				SerialPrint("pageMulti.bt1302.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageMulti.bt1302.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				break;
			}
			case ID_AXIS_3:
			{
				SetEnable(ID_AXIS_3, false, true);
				SerialPrint("pageMulti.bt1303.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageMulti.bt1303.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				break;
			}
			case ID_AXIS_4:
			{
				SetEnable(ID_AXIS_4, false, true);
				SerialPrint("pageMulti.bt1304.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageMulti.bt1304.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				break;
			}
			case ID_SPINDLE:
			{
				SetEnable(ID_SPINDLE, false, true);

				SerialPrint("pageMulti.bt1300.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageMulti.bt1300.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				break;
			}
			}
			switch (stepperId_5)
			{
			case ID_AXIS_Z:
			{
				SetEnable(ID_AXIS_Z, false, true);

				SerialPrint("pageMulti.bt1301.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageMulti.bt1301.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				Serial.print("Disable Z:  ");

				break;
			}
			case ID_AXIS_X:
			{
				SetEnable(ID_AXIS_X, false, true);

				SerialPrint("pageMulti.bt1302.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageMulti.bt1302.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				break;
			}
			case ID_AXIS_3:
			{
				SetEnable(ID_AXIS_3, false, true);
				SerialPrint("pageMulti.bt1303.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageMulti.bt1303.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				break;
			}
			case ID_AXIS_4:
			{
				SetEnable(ID_AXIS_4, false, true);
				SerialPrint("pageMulti.bt1304.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageMulti.bt1304.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				break;
			}
			case ID_SPINDLE:
			{
				SetEnable(ID_SPINDLE, false, true);

				SerialPrint("pageMulti.bt1300.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageMulti.bt1300.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				break;
			}
			}
		}
		else
		{
#ifdef DEBUG
			Serial.print("keepSteppersEnabled:  ");
			Serial.println(configSetup.keepSteppersEnabled);
#endif // DEBUG
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

		if (configSetup.keepSteppersEnabled != 1)
		{
			switch (stepperId_1)
			{
			case ID_AXIS_Z:
			{
				SetEnable(ID_AXIS_Z, false, true);

				SerialPrint("pageOne.bt301.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageOne.bt301.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				Serial.print("Disable Z:  ");

				break;
			}
			case ID_AXIS_X:
			{
				SetEnable(ID_AXIS_X, false, true);

				SerialPrint("pageOne.bt302.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageOne.bt302.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				break;
			}
			case ID_AXIS_3:
			{
				SetEnable(ID_AXIS_3, false, true);

				SerialPrint("pageOne.bt303.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageOne.bt303.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				break;
			}
			case ID_AXIS_4:
			{
				SetEnable(ID_AXIS_4, false, true);
				SerialPrint("pageOne.bt304.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageOne.bt304.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				break;
			}
			case ID_SPINDLE:
			{
				SetEnable(ID_SPINDLE, false, true);

				SerialPrint("pageOne.bt300.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageOne.bt300.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				break;
			}
			}
			switch (stepperId_2)
			{
			case ID_AXIS_3:
			{
				SetEnable(ID_AXIS_3, false, true);

				SerialPrint("pageOne.bt303.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageOne.bt303.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				break;
			}
			}
		}
		else
		{
#ifdef DEBUG
			Serial.print("keepSteppersEnabled:  ");
			Serial.println(configSetup.keepSteppersEnabled);
#endif // DEBUG
		}
		break;
	}
	case PAGE_MAIN:
	{
		// See ReturnToStartPosition_MultiMain
		switch (returnType)
		{
		case RETURN_SPINDLE:
		case RETURN_SPINDLE_M3:
		{
			SerialPrint("pageMain.bt1.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageMain.bt1.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageMain.va0.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageMain.bt0.val=0");
			SerialPrint(nextionEnd);
			break;
		}
		case RETURN_AXIS:
		{
			SerialPrint("pageMain.bt4.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageMain.bt4.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageMain.va0.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageMain.bt7.val=0");
			SerialPrint(nextionEnd);
			break;
		}
		}

		if (configSetup.keepSteppersEnabled != 1)
		{
			switch (stepperId_1)
			{
			case ID_AXIS_Z:
			case ID_AXIS_X:
			case ID_AXIS_3:
			case ID_AXIS_4:
			{
				SetEnable(stepperId_1, false, true);

				SerialPrint("pageMain.bt11.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageMain.bt11.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				Serial.print("Disable Axis:  ");

				break;
			}
			case ID_SPINDLE:
			{
				SetEnable(stepperId_1, false, true);
				if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
				{
					SetEnable(stepperId_3, false, true);
				}

				SerialPrint("pageOne.bt10.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageOne.bt10.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				break;
			}
			}
			switch (stepperId_2)
			{
			case ID_AXIS_3:
			{
				// M3 is not the active axis, so set Enabled/Disabled for other pages
				SetEnable(stepperId_2, false, true);

				SerialPrint("pageOne.bt303.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageOne.bt303.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				break;
			}
			}
		}
		else
		{
#ifdef DEBUG
			Serial.print("keepSteppersEnabled:  ");
			Serial.println(configSetup.keepSteppersEnabled);
#endif // DEBUG
		}
		break;
	}
	case PAGE_INDEX:
	{
		SerialPrint("pageIndex.bt1.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageIndex.va0.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageIndex.bt0.val=0");
		SerialPrint(nextionEnd);

		if (configSetup.keepSteppersEnabled != 1)
		{
			SetEnable(configIndex_Prime.axisId, false, true);
			switch (configIndex_Prime.axisId)
			{
			case ID_SPINDLE:
			{
				SerialPrint("pageIndex.bt10.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageIndex.bt10.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				break;
			}
			case ID_AXIS_3:
			case ID_AXIS_4:
			{
				SerialPrint("pageIndex.bt11.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageIndex.bt11.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				break;
			}
			}
		}

		break;
	}
	case PAGE_ROSE:
	{
		SerialPrint("pageRose.bt1.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageRose.va0.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageRose.bt0.val=0");
		SerialPrint(nextionEnd);

		SetEnable(configRose.axisId, false, true);
		SetEnable(ID_SPINDLE, false, true);
		if (configSetup.keepSteppersEnabled != 1)
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

		break;
	}
	case PAGE_SYNC:
	{
		SerialPrint("pageSync.bt1.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageSync.va0.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageSync.bt0.val=0");
		SerialPrint(nextionEnd);

		SetEnable(configSync.axisId, false, true);
		SetEnable(ID_SPINDLE, false, true);
		if (configSetup.keepSteppersEnabled != 1)
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

		break;
	}
	case PAGE_REC:
	{
		SerialPrint("pageRec.bt1.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageRec.va0.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageRec.bt0.val=0");
		SerialPrint(nextionEnd);

		SetEnable(configRec.axisId, false, true);
		SetEnable(ID_SPINDLE, false, true);
		if (configSetup.keepSteppersEnabled != 1)
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

		break;
	}
	case PAGE_GRK:
	{
		SerialPrint("pageGrk.bt1.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageGrk.va0.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageGrk.bt0.val=0");
		SerialPrint(nextionEnd);

		SetEnable(configGreekKey.axisId, false, true);
		SetEnable(ID_SPINDLE, false, true);
		if (configSetup.keepSteppersEnabled != 1)
		{
			SerialPrint("pageGrk.bt10.val=0"); // 1 = disabled
			SerialPrint("\xFF\xFF\xFF");
			SerialPrint("pageGrk.bt10.txt="); // 1 = disabled
			SerialPrint("\x22");
			SerialPrint("Disabled");
			SerialPrint("\x22\xFF\xFF\xFF");

			SerialPrint("pageGrk.bt11.val=0"); // 1 = disabled
			SerialPrint("\xFF\xFF\xFF");
			SerialPrint("pageGrk.bt11.txt="); // 1 = disabled
			SerialPrint("\x22");
			SerialPrint("Disabled");
			SerialPrint("\x22\xFF\xFF\xFF");
		}

		break;
	}
	case PAGE_PROGRAM:
	{
		SerialPrint("pageProgram.bt1.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageProgram.va0.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageProgram.bt0.val=0");
		SerialPrint(nextionEnd);

		SetEnable(configGreekKey.axisId, false, true);
		SetEnable(ID_SPINDLE, false, true);
		if (configSetup.keepSteppersEnabled != 1)
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

	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		SetEnable(ID_AXIS_3, false, true);
	}

	checkedCount = 0;
	if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
	{
		digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
	}
}

/////// <summary>
/////// pageProgram and pageGrk: Return Spindle and/or axes to start position
/////// </summary>
/////// <comment>
/////// </comment>
/////// <param name="axisId">Id of axis to move</param>
////void ReturnToStartPosition_Cumulative()
////{
////	if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
////	{
////		digitalWrite(configSetup.auxillaryPin, LOW);  // Enable
////	}
////	eStopTriggered = false;
////	MilliDelay(10);
////	SerialPrint("pageSplash.vaEStop.val=0");
////	SerialPrint(nextionEnd);
////
////	int32_t maxSpeed_1 = 100000;
////	int32_t maxSpeed_2 = 100000;
////	int32_t maxSpeed_3 = 100000;
////	int32_t maxSpeed_4 = 100000;
////	int32_t maxSpeed_5 = 100000;
////	int32_t accel_1 = 100000;
////	int32_t accel_2 = 100000;
////	int32_t accel_3 = 100000;
////	int32_t accel_4 = 100000;
////	int32_t accel_5 = 100000;
////	////int curPosition = 0;
////
////	StepControl stepController;
////	Stepper stepper_1(stepper1_step, stepper1_dir);
////	Stepper stepper_2(stepper2_step, stepper2_dir);
////	Stepper stepper_3(stepper3_step, stepper3_dir);
////	Stepper stepper_4(stepper4_step, stepper4_dir);
////	Stepper stepper_5(stepper5_step, stepper5_dir);
////	stepper_1.setPosition(0);
////	stepper_2.setPosition(0);
////	stepper_3.setPosition(0);
////	stepper_4.setPosition(0);
////	stepper_5.setPosition(0);
////
////	// Set unused stepper values
////	switch (checkedCount)
////	{
////	case 1:
////	{
////		Serial.print("   Case 1");
////
////		stepperId_2 = 44;
////		stepperId_3 = 45;
////		stepperId_4 = 46;
////		stepperId_5 = 47;
////
////		stepper2_step = 50;
////		stepper2_dir = 51;
////
////		stepper3_step = 52;
////		stepper3_dir = 53;
////
////		stepper4_step = 54;
////		stepper4_dir = 55;
////
////		stepper5_step = 56;
////		stepper5_dir = 57;
////
////		break;
////	}
////	case 2:
////	{
////		Serial.print("   Case 2");
////
////		stepperId_3 = 45;
////		stepperId_4 = 46;
////		stepperId_5 = 47;
////
////		stepper3_step = 52;
////		stepper3_dir = 53;
////
////		stepper4_step = 54;
////		stepper4_dir = 55;
////
////		stepper5_step = 56;
////		stepper5_dir = 57;
////
////		break;
////	}
////	case 3:
////	{
////		Serial.print("   Case 3");
////
////		stepperId_4 = 46;
////		stepperId_5 = 47;
////
////		stepper4_step = 54;
////		stepper4_dir = 55;
////
////		stepper5_step = 56;
////		stepper5_dir = 57;
////
////		break;
////	}
////	case 4:
////	{
////		Serial.print("   Case 4");
////
////		stepperId_5 = 47;
////
////		stepper5_step = 56;
////		stepper5_dir = 57;
////
////		break;
////	}
////	}
////
////	// Set Speed and Accel
////	switch (stepperId_1)
////	{
////	case ID_AXIS_Z:
////	{
////		maxSpeed_1 = configSetup.maxSpd_Return_Axis_Z;
////		accel_1 = configSetup.accel_Return_Axis_Z;
////		break;
////	}
////	case ID_AXIS_X:
////	{
////		maxSpeed_1 = configSetup.maxSpd_Return_Axis_X;
////		accel_1 = configSetup.accel_Return_Axis_X;
////		break;
////	}
////	case ID_AXIS_3:
////	{
////		maxSpeed_1 = configSetup.maxSpd_Return_Axis_M3;
////		accel_1 = configSetup.accel_Return_Axis_M3;
////		break;
////	}
////	case ID_AXIS_4:
////	{
////		maxSpeed_1 = configSetup.maxSpd_Return_Axis_M4;
////		accel_1 = configSetup.accel_Return_Axis_M4;
////		break;
////	}
////	case ID_SPINDLE:
////	{
////		maxSpeed_1 = configSetup.maxSpd_Return_Spindle;
////		accel_1 = configSetup.accel_Return_Spindle;
////		MilliDelay(15);
////		break;
////	}
////	}
////	switch (stepperId_2)
////	{
////	case ID_AXIS_Z:
////	{
////		maxSpeed_1 = configSetup.maxSpd_Return_Axis_Z;
////		accel_1 = configSetup.accel_Return_Axis_Z;
////		break;
////	}
////	case ID_AXIS_X:
////	{
////		maxSpeed_2 = configSetup.maxSpd_Return_Axis_X;
////		accel_2 = configSetup.accel_Return_Axis_X;
////		break;
////	}
////	case ID_AXIS_3:
////	{
////		maxSpeed_2 = configSetup.maxSpd_Return_Axis_M3;
////		accel_2 = configSetup.accel_Return_Axis_M3;
////		break;
////	}
////	case ID_AXIS_4:
////	{
////		maxSpeed_2 = configSetup.maxSpd_Return_Axis_M4;
////		accel_2 = configSetup.accel_Return_Axis_M4;
////		break;
////	}
////	case ID_SPINDLE:
////	{
////		maxSpeed_2 = configSetup.maxSpd_Return_Spindle;
////		accel_2 = configSetup.accel_Return_Spindle;
////		MilliDelay(15);
////		break;
////	}
////	}
////	switch (stepperId_3)
////	{
////	case ID_AXIS_Z:
////	{
////		maxSpeed_1 = configSetup.maxSpd_Return_Axis_Z;
////		accel_1 = configSetup.accel_Return_Axis_Z;
////		break;
////	}
////	case ID_AXIS_X:
////	{
////		maxSpeed_2 = configSetup.maxSpd_Return_Axis_X;
////		accel_2 = configSetup.accel_Return_Axis_X;
////		break;
////	}
////	case ID_AXIS_3:
////	{
////		maxSpeed_3 = configSetup.maxSpd_Return_Axis_M3;
////		accel_3 = configSetup.accel_Return_Axis_M3;
////		break;
////	}
////	case ID_AXIS_4:
////	{
////		maxSpeed_3 = configSetup.maxSpd_Return_Axis_M4;
////		accel_3 = configSetup.accel_Return_Axis_M4;
////		break;
////	}
////	case ID_SPINDLE:
////	{
////		maxSpeed_3 = configSetup.maxSpd_Return_Spindle;
////		accel_3 = configSetup.accel_Return_Spindle;
////		MilliDelay(15);
////		break;
////	}
////	}
////	switch (stepperId_4)
////	{
////	case ID_AXIS_Z:
////	{
////		maxSpeed_1 = configSetup.maxSpd_Return_Axis_Z;
////		accel_1 = configSetup.accel_Return_Axis_Z;
////		break;
////	}
////	case ID_AXIS_X:
////	{
////		maxSpeed_2 = configSetup.maxSpd_Return_Axis_X;
////		accel_2 = configSetup.accel_Return_Axis_X;
////		break;
////	}
////	case ID_AXIS_3:
////	{
////		maxSpeed_3 = configSetup.maxSpd_Return_Axis_M3;
////		accel_3 = configSetup.accel_Return_Axis_M3;
////		break;
////	}
////	case ID_AXIS_4:
////	{
////		maxSpeed_4 = configSetup.maxSpd_Return_Axis_M4;
////		accel_4 = configSetup.accel_Return_Axis_M4;
////		break;
////	}
////	case ID_SPINDLE:
////	{
////		maxSpeed_4 = configSetup.maxSpd_Return_Spindle;
////		accel_4 = configSetup.accel_Return_Spindle;
////		MilliDelay(15);
////		break;
////	}
////	}
////	switch (stepperId_5)
////	{
////	case ID_AXIS_Z:
////	{
////		maxSpeed_1 = configSetup.maxSpd_Return_Axis_Z;
////		accel_1 = configSetup.accel_Return_Axis_Z;
////		break;
////	}
////	case ID_AXIS_X:
////	{
////		maxSpeed_2 = configSetup.maxSpd_Return_Axis_X;
////		accel_2 = configSetup.accel_Return_Axis_X;
////		break;
////	}
////	case ID_AXIS_3:
////	{
////		maxSpeed_3 = configSetup.maxSpd_Return_Axis_M3;
////		accel_3 = configSetup.accel_Return_Axis_M3;
////		break;
////	}
////	case ID_AXIS_4:
////	{
////		maxSpeed_4 = configSetup.maxSpd_Return_Axis_M4;
////		accel_4 = configSetup.accel_Return_Axis_M4;
////		break;
////	}
////	case ID_SPINDLE:
////	{
////		maxSpeed_4 = configSetup.maxSpd_Return_Spindle;
////		accel_4 = configSetup.accel_Return_Spindle;
////		MilliDelay(15);
////		break;
////	}
////	}
////
////	stepper_1
////		.setMaxSpeed(maxSpeed_1)
////		.setAcceleration(accel_1)
////		.setTargetAbs(cumulative_Spindle);
////
////	stepper_2
////		.setMaxSpeed(maxSpeed_2)
////		.setAcceleration(accel_2)
////		.setTargetAbs(cumulative_Axis);
////
////	stepper_3
////		.setMaxSpeed(maxSpeed_3)
////		.setAcceleration(accel_3)
////		.setTargetAbs(cumulative_M3);
////
////	stepper_4
////		.setMaxSpeed(maxSpeed_4)
////		.setAcceleration(accel_4)
////		.setTargetAbs(stepperPosition_4);
////
////	stepper_5
////		.setMaxSpeed(maxSpeed_5)
////		.setAcceleration(accel_5)
////		.setTargetAbs(stepperPosition_5);
////
////	SetEnable(stepperId_1, true, true);
////	if (stepperId_2 < 40)
////	{
////		SetEnable(stepperId_2, true, true);
////	}
////	if (stepperId_3 < 40)
////	{
////		SetEnable(stepperId_3, true, true);
////	}
////
////	stepController.moveAsync(stepper_1, stepper_2, stepper_3);
////
////	while (stepController.isRunning())
////	{
////
////#ifdef ESTOP_ENABLED
////		if (configSetup.eStop > 9 && configSetup.eStop < 40)
////		{
////			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
////			{
////#ifdef DEBUG
////				Serial.println("                          EStop");
////#endif // DEBUG
////
////
////				stepController.emergencyStop();
////				stopSteppers = true;
////				// Disable all motors
////				SetEnable(ID_SPINDLE, false, false);
////				SetEnable(ID_AXIS_Z, false, false);
////				SetEnable(ID_AXIS_X, false, false);
////				SetEnable(ID_AXIS_3, false, false);
////				SetEnable(ID_AXIS_4, false, false);
////				SetEStopColors(PAGE_RETURNS);
////				if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
////				{
////					digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
////				}
////				return;
////			}
////		}
////#endif // estop
////		// Check for Cancel code  
////		if (SerialAvailable() >= 0)
////		{
////			incomingByte = SerialRead(serialId);
////			switch (incomingByte)
////			{
////			case 83: // - S
////			{
////				currentSpeed = stepController.getCurrentSpeed();
////#ifdef DEBUG
////				Serial.print("currentSpeed: ");
////				Serial.println(currentSpeed);
////#endif // Debug
////				if (currentSpeed < MIN_SPEED)
////				{
////					stepController.emergencyStop();
////				}
////				else
////				{
////					stepController.stop();
////				}
////				checkedCount = 0;
////				break;
////			}
////			}
////		}
////
////		MilliDelay(10);
////	}
////
////
////	switch (pageCallerId)
////	{
////	case PAGE_GRK:
////	{
////		SerialPrint("pageGrk.bt1.val=0");
////		SerialPrint(nextionEnd);
////		SerialPrint("pageGrk.va0.val=0");
////		SerialPrint(nextionEnd);
////		SerialPrint("pageGrk.bt0.val=0");
////		SerialPrint(nextionEnd);
////
////		SetEnable(configGreekKey.axisId, false, true);
////		SetEnable(ID_SPINDLE, false, true);
////		if (configSetup.keepSteppersEnabled != 1)
////		{
////			SerialPrint("pageGrk.bt10.val=0"); // 1 = disabled
////			SerialPrint("\xFF\xFF\xFF");
////			SerialPrint("pageGrk.bt10.txt="); // 1 = disabled
////			SerialPrint("\x22");
////			SerialPrint("Disabled");
////			SerialPrint("\x22\xFF\xFF\xFF");
////
////			SerialPrint("pageGrk.bt11.val=0"); // 1 = disabled
////			SerialPrint("\xFF\xFF\xFF");
////			SerialPrint("pageGrk.bt11.txt="); // 1 = disabled
////			SerialPrint("\x22");
////			SerialPrint("Disabled");
////			SerialPrint("\x22\xFF\xFF\xFF");
////		}
////
////		break;
////	}
////	case PAGE_PROGRAM:
////	{
////		SerialPrint("pageProgram.bt1.val=0");
////		SerialPrint(nextionEnd);
////		SerialPrint("pageProgram.va0.val=0");
////		SerialPrint(nextionEnd);
////		SerialPrint("pageProgram.bt0.val=0");
////		SerialPrint(nextionEnd);
////
////		SetEnable(configGreekKey.axisId, false, true);
////		SetEnable(ID_SPINDLE, false, true);
////		if (configSetup.keepSteppersEnabled != 1)
////		{
////			SerialPrint("pageProgram.bt10.val=0"); // 1 = disabled
////			SerialPrint("\xFF\xFF\xFF");
////			SerialPrint("pageProgram.bt10.txt="); // 1 = disabled
////			SerialPrint("\x22");
////			SerialPrint("Disabled");
////			SerialPrint("\x22\xFF\xFF\xFF");
////
////			SerialPrint("pageProgram.bt11.val=0"); // 1 = disabled
////			SerialPrint("\xFF\xFF\xFF");
////			SerialPrint("pageProgram.bt11.txt="); // 1 = disabled
////			SerialPrint("\x22");
////			SerialPrint("Disabled");
////			SerialPrint("\x22\xFF\xFF\xFF");
////		}
////
////		break;
////	}
////	}
////
////	SerialPrint("pageBE.t1.txt=");
////	SerialWrite(0x22);
////	SerialPrint(endPosition_Spindle);
////	SerialPrint(nextionQuoteEnd);
////
////	SerialPrint("pageBE.t2.txt=");
////	SerialWrite(0x22);
////	SerialPrint(endPosition_Axis);
////	SerialPrint(nextionQuoteEnd);
////
////	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
////	{
////		SetEnable(ID_AXIS_3, false, true);
////	}
////
////	checkedCount = 0;
////	if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
////	{
////		digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
////	}
////}

/// <summary>
/// PageMain: Return Spindle and/or axes to start position
/// </summary>
/// <comment>
/// </comment>
/// <param name="axisId">Id of axis to move</param>
////void t35ReturnToStartPosition_MainPage()
////{
////	if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
////	{
////		digitalWrite(configSetup.auxillaryPin, LOW);  // Enable
////	}
////	eStopTriggered = false;
////	MilliDelay(10);
////	SerialPrint("pageSplash.vaEStop.val=0");
////	SerialPrint(nextionEnd);
////
////	int32_t maxSpeed_1 = 100000;
////	int32_t maxSpeed_2 = 100000;
////	int32_t maxSpeed_3 = 100000;
////	int32_t maxSpeed_4 = 100000;
////	int32_t maxSpeed_5 = 100000;
////	int32_t accel_1 = 100000;
////	int32_t accel_2 = 100000;
////	int32_t accel_3 = 100000;
////	int32_t accel_4 = 100000;
////	int32_t accel_5 = 100000;
////	////int curPosition = 0;
////	int dirPin_Reset = 55;
////
////	////StepControl stepController;
////    TS4:StepperGroup stepGroup;
////	Stepper stepper_1(stepper1_step, stepper1_dir);
////	Stepper stepper_2(stepper2_step, stepper2_dir);
////	Stepper stepper_3(stepper3_step, stepper3_dir);
////	Stepper stepper_4(stepper4_step, stepper4_dir);
////	Stepper stepper_5(stepper5_step, stepper5_dir);
////	stepper_1.setPosition(stepperPosition_1);
////	stepper_2.setPosition(stepperPosition_2);
////	stepper_3.setPosition(stepperPosition_3);
////	stepper_4.setPosition(stepperPosition_4);
////	stepper_5.setPosition(stepperPosition_5);
////
////	Serial.print("checkedCount : ");
////	Serial.println(checkedCount);
////
////	// Set unused stepper values
////	switch (checkedCount)
////	{
////	case 1:
////	{
////#ifdef VERBOSE
////		Serial.print("   Case 1");
////#endif // VERBOSE
////		stepperId_2 = 44;
////		stepperId_3 = 45;
////		stepperId_4 = 46;
////		stepperId_5 = 47;
////
////		stepper2_step = 50;
////		stepper2_dir = 51;
////
////		stepper3_step = 52;
////		stepper3_dir = 53;
////
////		stepper4_step = 54;
////		stepper4_dir = 55;
////
////		stepper5_step = 56;
////		stepper5_dir = 57;
////
////		break;
////	}
////	case 2:
////	{
////#ifdef VERBOSE
////		Serial.print("   Case 2");
////#endif // VERBOSE
////		stepperId_3 = 45;
////		stepperId_4 = 46;
////		stepperId_5 = 47;
////
////		stepper3_step = 52;
////		stepper3_dir = 53;
////
////		stepper4_step = 54;
////		stepper4_dir = 55;
////
////		stepper5_step = 56;
////		stepper5_dir = 57;
////
////		break;
////	}
////	case 3:
////	{
////#ifdef VERBOSE
////		Serial.print("   Case 3");
////#endif // VERBOSE
////		stepperId_4 = 46;
////		stepperId_5 = 47;
////
////		stepper4_step = 54;
////		stepper4_dir = 55;
////
////		stepper5_step = 56;
////		stepper5_dir = 57;
////
////		break;
////	}
////	case 4:
////	{
////#ifdef VERBOSE
////		Serial.print("   Case 4");
////#endif // VERBOSE
////		stepperId_5 = 47;
////
////		stepper5_step = 56;
////		stepper5_dir = 57;
////
////		break;
////	}
////	}
////
////	switch (stepperId_1)
////	{
////	case ID_AXIS_Z:
////	{
////		maxSpeed_1 = configSetup.maxSpd_Return_Axis_Z;
////		accel_1 = configSetup.accel_Return_Axis_Z;
////		break;
////	}
////	case ID_AXIS_X:
////	{
////		maxSpeed_1 = configSetup.maxSpd_Return_Axis_X;
////		accel_1 = configSetup.accel_Return_Axis_X;
////		break;
////	}
////	case ID_AXIS_3:
////	{
////		maxSpeed_1 = configSetup.maxSpd_Return_Axis_M3;
////		accel_1 = configSetup.accel_Return_Axis_M3;
////		break;
////	}
////	case ID_AXIS_4:
////	{
////		maxSpeed_1 = configSetup.maxSpd_Return_Axis_M4;
////		accel_1 = configSetup.accel_Return_Axis_M4;
////		break;
////	}
////	case ID_SPINDLE:
////	{
////		maxSpeed_1 = configSetup.maxSpd_Return_Spindle;
////		accel_1 = configSetup.accel_Return_Spindle;
////		MilliDelay(15);
////		break;
////	}
////	}
////
////	switch (stepperId_2)
////	{
////	case ID_AXIS_Z:
////	{
////		maxSpeed_1 = configSetup.maxSpd_Return_Axis_Z;
////		accel_1 = configSetup.accel_Return_Axis_Z;
////		break;
////	}
////	case ID_AXIS_X:
////	{
////		maxSpeed_2 = configSetup.maxSpd_Return_Axis_X;
////		accel_2 = configSetup.accel_Return_Axis_X;
////		break;
////	}
////	case ID_AXIS_3:
////	{
////		maxSpeed_2 = configSetup.maxSpd_Return_Axis_M3;
////		accel_2 = configSetup.accel_Return_Axis_M3;
////		break;
////	}
////	case ID_AXIS_4:
////	{
////		maxSpeed_2 = configSetup.maxSpd_Return_Axis_M4;
////		accel_2 = configSetup.accel_Return_Axis_M4;
////		break;
////	}
////	case ID_SPINDLE:
////	{
////		maxSpeed_2 = configSetup.maxSpd_Return_Spindle;
////		accel_2 = configSetup.accel_Return_Spindle;
////		MilliDelay(15);
////		break;
////	}
////
////	}
////	switch (stepperId_3)
////	{
////	case ID_AXIS_Z:
////	{
////		maxSpeed_1 = configSetup.maxSpd_Return_Axis_Z;
////		accel_1 = configSetup.accel_Return_Axis_Z;
////		break;
////	}
////	case ID_AXIS_X:
////	{
////		maxSpeed_2 = configSetup.maxSpd_Return_Axis_X;
////		accel_2 = configSetup.accel_Return_Axis_X;
////		break;
////	}
////	case ID_AXIS_3:
////	{
////		maxSpeed_2 = configSetup.maxSpd_Return_Axis_M3;
////		accel_2 = configSetup.accel_Return_Axis_M3;
////		break;
////	}
////	case ID_AXIS_4:
////	{
////		maxSpeed_2 = configSetup.maxSpd_Return_Axis_M4;
////		accel_2 = configSetup.accel_Return_Axis_M4;
////		break;
////	}
////	case ID_SPINDLE:
////	{
////		maxSpeed_2 = configSetup.maxSpd_Return_Spindle;
////		accel_2 = configSetup.accel_Return_Spindle;
////		MilliDelay(15);
////		break;
////	}
////	}
////	switch (stepperId_4)
////	{
////	case ID_AXIS_Z:
////	{
////		maxSpeed_1 = configSetup.maxSpd_Return_Axis_Z;
////		accel_1 = configSetup.accel_Return_Axis_Z;
////		break;
////	}
////	case ID_AXIS_X:
////	{
////		maxSpeed_2 = configSetup.maxSpd_Return_Axis_X;
////		accel_2 = configSetup.accel_Return_Axis_X;
////		break;
////	}
////	case ID_AXIS_3:
////	{
////		maxSpeed_2 = configSetup.maxSpd_Return_Axis_M3;
////		accel_2 = configSetup.accel_Return_Axis_M3;
////		break;
////	}
////	case ID_AXIS_4:
////	{
////		maxSpeed_2 = configSetup.maxSpd_Return_Axis_M4;
////		accel_2 = configSetup.accel_Return_Axis_M4;
////		break;
////	}
////	case ID_SPINDLE:
////	{
////		maxSpeed_2 = configSetup.maxSpd_Return_Spindle;
////		accel_2 = configSetup.accel_Return_Spindle;
////		MilliDelay(15);
////		break;
////	}
////	}
////	switch (stepperId_5)
////	{
////	case ID_AXIS_Z:
////	{
////		maxSpeed_1 = configSetup.maxSpd_Return_Axis_Z;
////		accel_1 = configSetup.accel_Return_Axis_Z;
////		break;
////	}
////	case ID_AXIS_X:
////	{
////		maxSpeed_2 = configSetup.maxSpd_Return_Axis_X;
////		accel_2 = configSetup.accel_Return_Axis_X;
////		break;
////	}
////	case ID_AXIS_3:
////	{
////		maxSpeed_2 = configSetup.maxSpd_Return_Axis_M3;
////		accel_2 = configSetup.accel_Return_Axis_M3;
////		break;
////	}
////	case ID_AXIS_4:
////	{
////		maxSpeed_2 = configSetup.maxSpd_Return_Axis_M4;
////		accel_2 = configSetup.accel_Return_Axis_M4;
////		break;
////	}
////	case ID_SPINDLE:
////	{
////		maxSpeed_2 = configSetup.maxSpd_Return_Spindle;
////		accel_2 = configSetup.accel_Return_Spindle;
////		MilliDelay(15);
////		break;
////	}
////	}
////
////	stepper_1
////		.setMaxSpeed(maxSpeed_1)
////		.setAcceleration(accel_1)
////		.setTargetAbs(0);
////
////	stepper_2
////		.setMaxSpeed(maxSpeed_2)
////		.setAcceleration(accel_2)
////		.setTargetAbs(0);
////
////	stepper_3
////		.setMaxSpeed(maxSpeed_3)
////		.setAcceleration(accel_3)
////		.setTargetAbs(0);
////
////	stepper_4
////		.setMaxSpeed(maxSpeed_4)
////		.setAcceleration(accel_4)
////		.setTargetAbs(0);
////
////	stepper_5
////		.setMaxSpeed(maxSpeed_5)
////		.setAcceleration(accel_5)
////		.setTargetAbs(0);
////
////	dirPin_Reset = digitalReadFast(PIN_AXIS_3_DIR);
////
////	if (stepperPosition_1 > 0)
////	{
////		switch (stepperId_1)
////		{
////		case ID_AXIS_Z:
////		{
////			digitalWriteFast(PIN_AXIS_Z_DIR, LOW);
////			break;
////		}
////		case ID_AXIS_X:
////		{
////			digitalWriteFast(PIN_AXIS_X_DIR, LOW);
////			break;
////		}
////		case ID_AXIS_3:
////		{
////			digitalWriteFast(PIN_AXIS_3_DIR, LOW);
////			break;
////		}
////		case ID_AXIS_4:
////		{
////			digitalWriteFast(PIN_AXIS_4_DIR, LOW);
////			break;
////		}
////		}
////	}
////
////	if (stepperPosition_2 > 0) // Only valid on main page when synchro engaged
////	{
////
////		digitalWriteFast(PIN_AXIS_3_DIR, LOW);
////	}
////
////	dirPin_Reset = digitalReadFast(PIN_SPINDLE_DIR);
////
////	switch (checkedCount)
////	{
////	case 1:
////	{
////		SetEnable(stepperId_1, true, true);
////		stepController.moveAsync(stepper_1);
////		break;
////	}
////	case 2:
////	{
////		SetEnable(stepperId_1, true, true);
////		SetEnable(stepperId_2, true, true);
////		stepController.moveAsync(stepper_1, stepper_2);
////		break;
////	}
////	case 3:
////	{
////		SetEnable(stepperId_1, true, true);
////		SetEnable(stepperId_2, true, true);
////		SetEnable(stepperId_3, true, true);
////
////		stepController.moveAsync(stepper_1, stepper_2, stepper_3);
////		break;
////	}
////	case 4:
////	{
////		SetEnable(stepperId_1, true, true);
////		SetEnable(stepperId_2, true, true);
////		SetEnable(stepperId_3, true, true);
////		SetEnable(stepperId_4, true, true);
////		stepController.moveAsync(stepper_1, stepper_2, stepper_3, stepper_4);
////		break;
////	}
////	case 5:
////	{
////		SetEnable(stepperId_1, true, true);
////		SetEnable(stepperId_2, true, true);
////		SetEnable(stepperId_3, true, true);
////		SetEnable(stepperId_4, true, true);
////		SetEnable(stepperId_5, true, true);
////		stepController.moveAsync(stepper_1, stepper_2, stepper_3, stepper_4, stepper_5);
////		break;
////	}
////	}
////
////	while (stepController.isRunning())
////	{
////
////#ifdef ESTOP_ENABLED
////		if (configSetup.eStop > 9 && configSetup.eStop < 40)
////		{
////			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
////			{
////#ifdef DEBUG
////				Serial.println("                          EStop");
////#endif // DEBUG
////
////
////				stepController.emergencyStop();
////				stopSteppers = true;
////				// Disable all motors
////				SetEnable(ID_SPINDLE, false, false);
////				SetEnable(ID_AXIS_Z, false, false);
////				SetEnable(ID_AXIS_X, false, false);
////				SetEnable(ID_AXIS_3, false, false);
////				SetEnable(ID_AXIS_4, false, false);
////				SetEStopColors(PAGE_RETURNS);
////				if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
////				{
////					digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
////				}
////				return;
////			}
////		}
////#endif // estop
////		// Check for Cancel code  
////		if (SerialAvailable() >= 0)
////		{
////			incomingByte = SerialRead(serialId);
////			switch (incomingByte)
////			{
////			case 83: // - S
////			{
////				currentSpeed = stepController.getCurrentSpeed();
////#ifdef VERBOSE
////				Serial.print("currentSpeed: ");
////				Serial.println(currentSpeed);
////#endif // VERBOSE
////				if (currentSpeed < MIN_SPEED)
////				{
////					stepController.emergencyStop();
////				}
////				else
////				{
////					stepController.stop();
////				}
////				checkedCount = 0;
////				break;
////			}
////			}
////		}
////
////		MilliDelay(10);
////	}
////
////	switch (pageCallerId)
////	{
////	case PAGE_MAIN:
////	{
////
////		switch (returnType)
////		{
////		case RETURN_SPINDLE:
////		case RETURN_SPINDLE_M3:
////		{
////			SerialPrint("pageMain.bt1.val=0");
////			SerialPrint(nextionEnd);
////			SerialPrint("pageMain.bt1.val=0");
////			SerialPrint(nextionEnd);
////			SerialPrint("pageMain.va0.val=0");
////			SerialPrint(nextionEnd);
////			SerialPrint("pageMain.bt0.val=0");
////			SerialPrint(nextionEnd);
////			break;
////		}
////		case RETURN_AXIS:
////		{
////			SerialPrint("pageMain.bt4.val=0");
////			SerialPrint(nextionEnd);
////			SerialPrint("pageMain.bt4.val=0");
////			SerialPrint(nextionEnd);
////			SerialPrint("pageMain.va0.val=0");
////			SerialPrint(nextionEnd);
////			SerialPrint("pageMain.bt7.val=0");
////			SerialPrint(nextionEnd);
////			break;
////		}
////		}
////
////#ifdef VERBOSE
////		Serial.print("configMain.axisId: ");
////		Serial.println(configMain.axisId);
////#endif // VERBOSE
////		if (configSetup.keepSteppersEnabled != 1)
////		{
////#ifdef VERBOSE
////			Serial.print("stepper1 axisId:  ");
////			Serial.println(stepperId_1);
////#endif // VERBOSEVERBOSE
////			switch (stepperId_1)
////			{
////			case ID_AXIS_Z:
////			case ID_AXIS_X:
////			case ID_AXIS_3:
////			case ID_AXIS_4:
////			{
////				SetEnable(stepperId_1, false, true);
////
////				SerialPrint("pageMain.bt11.val=0"); // 1 = disabled
////				SerialPrint("\xFF\xFF\xFF");
////				SerialPrint("pageMain.bt11.txt="); // 1 = disabled
////				SerialPrint("\x22");
////				SerialPrint("Disabled");
////				SerialPrint("\x22\xFF\xFF\xFF");
////				Serial.print("Disable Axis:  ");
////
////				break;
////			}
////			case ID_SPINDLE:
////			{
////				SetEnable(stepperId_1, false, true);
////				if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
////				{
////					SetEnable(stepperId_3, false, true);
////				}
////
////				SerialPrint("pageOne.bt10.val=0"); // 1 = disabled
////				SerialPrint("\xFF\xFF\xFF");
////				SerialPrint("pageOne.bt10.txt="); // 1 = disabled
////				SerialPrint("\x22");
////				SerialPrint("Disabled");
////				SerialPrint("\x22\xFF\xFF\xFF");
////				break;
////			}
////			}
////			switch (stepperId_2)
////			{
////			case ID_AXIS_3:
////			{
////				SetEnable(stepperId_2, false, true);
////
////				SerialPrint("pageOne.bt303.val=0"); // 1 = disabled
////				SerialPrint("\xFF\xFF\xFF");
////				SerialPrint("pageOne.bt303.txt="); // 1 = disabled
////				SerialPrint("\x22");
////				SerialPrint("Disabled");
////				SerialPrint("\x22\xFF\xFF\xFF");
////				break;
////			}
////			}
////		}
////		else
////		{
////#ifdef VERBOSE
////			Serial.print("keepSteppersEnabled:  ");
////			Serial.println(configSetup.keepSteppersEnabled);
////#endif // VERBOSE
////		}
////		break;
////	}
////	}
////
////	SerialPrint("pageBE.t1.txt=");
////	SerialWrite(0x22);
////	SerialPrint(endPosition_Spindle);
////	SerialPrint(nextionQuoteEnd);
////
////	SerialPrint("pageBE.t2.txt=");
////	SerialWrite(0x22);
////	SerialPrint(endPosition_Axis);
////	SerialPrint(nextionQuoteEnd);
////
////	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
////	{
////		SetEnable(ID_AXIS_3, false, true);
////	}
////
////	checkedCount = 0;
////	if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
////	{
////		digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
////	}
////}

/// <summary>
/// Return Spindle and/or axes to start position
/// </summary>
/// <comment>
/// </comment>
/// <param name="axisId">Id of axis to move</param>
void ReturnToStartPosition_MovePage(int axisId)
{
	if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
	{
		digitalWrite(configSetup.auxillaryPin, LOW);  // Enable
	}
	eStopTriggered = false;
	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);

	//StepControl stepController;
	StepperGroup stepGroup;
	Stepper stepper_Z(PIN_AXIS_Z_STEP, PIN_AXIS_Z_DIR);
	Stepper stepper_X(PIN_AXIS_X_STEP, PIN_AXIS_X_DIR);
	Stepper stepper_M3(PIN_AXIS_3_STEP, PIN_AXIS_3_DIR);
	Stepper stepper_M4(PIN_AXIS_4_STEP, PIN_AXIS_4_DIR);

#ifdef DEBUG
	Serial.println("Begin ReturnToStartPosition()");
	Serial.print("startPositionAbs_Axis: ");
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

		MilliDelay(20);
		Serial.print("Z position: ");
		Serial.println(stepper_Z.getPosition());
		stepper_Z
			.setMaxSpeed(configSetup.maxSpd_Return_Axis_Z)
			.setAcceleration(configSetup.accel_Return_Axis_Z)
			.setTargetAbs(0);

#ifdef DEBUG
		Serial.print("maxSpd_Return_Axis_Z: ");
		Serial.println(configSetup.accel_Return_Axis_Z);
#endif // DEBUG
		SetEnable(ID_AXIS_Z, true, true);

		// Only move the Z Axis
		//stepController.moveAsync(stepper_Z);
		stepper_Z.moveAsync();

		break;
	}
	case ID_MOVE_AXIS_X2:
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
			.setTargetAbs(0);

		SetEnable(ID_AXIS_X, true, true);

		// Only move the X Axis
		//stepController.moveAsync(stepper_X);
		stepper_X.moveAsync();

		break;
	}
	case ID_MOVE_AXIS_M3_2:
	case ID_AXIS_3:
	{
		// Axis uses absolute position
		// Ensure axis position is set to end position of last routine.
		stepper_M3.setPosition(endPosition_Axis);

		MilliDelay(20);
		stepper_M3
			.setMaxSpeed(configSetup.maxSpd_Return_Axis_M3)
			.setAcceleration(configSetup.accel_Return_Axis_M3)
			.setTargetAbs(0);

		SetEnable(ID_AXIS_3, true, true);
		//stepController.moveAsync(stepper_M3);
		stepper_M3.moveAsync();
		break;
	}
	case ID_MOVE_AXIS_M4_2:
	case ID_AXIS_4:
	{
		// Axis uses absolute position
		// Ensure axis position is set to end position of last routine.
		stepper_M4.setPosition(endPosition_Axis);

		MilliDelay(20);
		stepper_M4
			.setMaxSpeed(configSetup.maxSpd_Return_Axis_M4)
			.setAcceleration(configSetup.accel_Return_Axis_M4)
			.setTargetAbs(0);

		SetEnable(ID_AXIS_4, true, true);
		//stepController.moveAsync(stepper_M4);
		stepper_M4.moveAsync();

		break;
	}
	}

	while (stepper_Z.isMoving || stepper_X.isMoving || stepper_M3.isMoving || stepper_M4.isMoving)
	{
#ifdef ESTOP_ENABLED
		//if (configSetup.eStop != 0)
		if (configSetup.eStop > 9 && configSetup.eStop < 40)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
				Serial.println("                          EStop");
				stepper_Z.stopAsync();
				stepper_X.stopAsync();
				stepper_M3.stopAsync();
				stepper_M4.stopAsync();
				MilliDelay(200);
				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEnable(ID_AXIS_4, false, false);
				SetEStopColors(PAGE_RETURNS);
				if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
				{
					digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
				}
				return;
			}
		}
#endif // estop
		// Check for Cancel code  
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
				case 83: // - S
				{
					switch (axisId)
					{
						case ID_MOVE_AXIS_Z2:
						case ID_AXIS_Z:
						{
							stepper_Z.stop();
							break;
						}
						case ID_MOVE_AXIS_X2:
						case ID_AXIS_X:
						{
							break;
						}
						case ID_MOVE_AXIS_M3_2:
						case ID_AXIS_3:
						{
							stepper_M3.stop();
							break;
						}
						case ID_MOVE_AXIS_M4_2:
						case ID_AXIS_4:
						{
							stepper_M4.stop();
							break;
						}
					}
					MilliDelay(200);
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
		case ID_MOVE_AXIS_M3_2:
		case ID_AXIS_3:
		{
			endPosition_Axis = stepper_M3.getPosition();
			SetEnable(ID_AXIS_3, false, true);
			break;
		}
		case ID_MOVE_AXIS_M4_2:
		case ID_AXIS_4:
		{
			endPosition_Axis = stepper_M4.getPosition();
			SetEnable(ID_AXIS_4, false, true);
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
	stepper_M3.setPosition(0);
	stepper_M4.setPosition(0);
	endPosition_Axis = 0;
	if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
	{
		digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
	}
}


/////////////////////////////////////

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
	//float retVal = slideStepsAmplitude * (pow(sin(2.8 * spindleAngle * kRatio), 2) + cos(pow(2 * spindleAngle * kRatio, 2)));
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
	float spindleAngle = stepperSpindle.getPosition() * (TWO_PI / stepsPerRev_Spindle); //convert steps to angle
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
		case ID_AXIS_3:
		{
			slidePosition = stepperAxis_M3.getPosition();
			break;
		}
		case ID_AXIS_4:
		{
			slidePosition = stepperAxis_M4.getPosition();
			break;
		}
	}

	//float phi = fmodf(spindleAngle * kRatio, TWO_PI);
	float slideTarget = slideFunc_Axis(spindleAngle);//slideStepsAmplitude * cosf(phi);//

	float newSpeed = (slideTarget - slidePosition) / dtRose; // speed to reach target in given delta t (neglecting acceleration)
	float speedFac = newSpeed / newMaxSpd_RoseAxis;              // transform in relative factor (-1.0 .. 1.0)

	//Serial.print("spindleAngle: ");
	//Serial.print(spindleAngle);
	//Serial.print("slideTarget: ");
	//Serial.print(slideTarget);
	//Serial.print("  slidePosition: ");
	//Serial.println(slidePosition);

	//Serial.print("newSpeed: ");
	//Serial.print(newSpeed);
	//Serial.print("  newRoseAxis: ");
	//Serial.print(newMaxSpd_RoseAxis);
	Serial.print("  speedFac: ");
	Serial.println(speedFac);
	//groupAxis.overrideSpeed(speedFac);             // set new speed
	switch (configRose.axisId)
	{
		case ID_AXIS_Z:
		{
			stepperAxis_Z.overrideSpeed(speedFac);
			break;
		}
		case ID_AXIS_X:
		{
			stepperAxis_X.overrideSpeed(speedFac);
			break;
		}
		case ID_AXIS_3:
		{
			stepperAxis_M3.overrideSpeed(speedFac);
			break;
		}
		case ID_AXIS_4:
		{
			stepperAxis_M4.overrideSpeed(speedFac);
			break;
		}
	}
}

void RoseRadial(int direction)
{
	BeginMethodSettings();
	int32_t currentPosition = 0;
	int limitPin_Max = configSetup.limit_Max_Z;
	int limitPin_Min = configSetup.limit_Min_Z;
	bool runPattern = true;
	int32_t spindleCurrentPosition = 0;
	IntervalTimer tickTimer;

	StepperGroup groupSpindle{ stepperSpindle,stepperAxis_M3 };

	kRatio = (float)configRose.n / configRose.d;
	slideStepsAmplitude = 0;

	// Get spindle steps per revolution and max speed
	stepsPerRev_Spindle = configSetup.gearRatio_Spindle * configSetup.microsteps_Spindle * configSetup.steps360_Spindle;
	int32_t spindleMaxRotation = configRose.spindleRevolutions * stepsPerRev_Spindle;
	newMaxSpd_RoseSpindle = configRose.maxSpd_Spindle * configRose.speedPercent_Spindle * .01;//* direction;
	volatile int32_t spindleTarget = 214700000 * direction; //2147000000 * direction;

	// Set spindle max speed and acceleration
	stepperSpindle
		.setMaxSpeed(newMaxSpd_RoseSpindle)
		.setAcceleration(configRose.accel_Spindle)
		.setPosition(0);
	stepperSpindle.setTargetAbs(spindleTarget);
	SetEnable(ID_SPINDLE, true, true);
	/////////////////////////////////////////////////////////////
	checkedCount = 2;
	float targetSteps_M3Synchro = spindleTarget * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01);
	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		checkedCount = 3;
		stepperId_3 = ID_AXIS_3;
		//stepper3_step = PIN_AXIS_3_STEP;
		//stepper3_dir = PIN_AXIS_3_DIR;
#ifdef VERBOSE
		Serial.print("spindleTarget * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01):  ");
		Serial.println(targetSteps_M3Synchro);

		Serial.print("synchro_Ratio:  ");
		Serial.println(synchro_Ratio);

		Serial.print("configMain.synchro_M3_Percentage * 0.01:  ");
		Serial.println(configMain.synchro_M3_Percentage * 0.01);

		Serial.print("newMaxSpd_RoseSpindle:  ");
		Serial.println(newMaxSpd_RoseSpindle);
#endif // VERBOSE

		stepperAxis_M3.setPosition(0);

		stepperAxis_M3
			.setMaxSpeed(newMaxSpd_RoseSpindle)
			.setAcceleration(configGreekKey.accel_M3)
			.setTargetAbs(targetSteps_M3Synchro);

		SetEnable(ID_AXIS_3, true, true);
		MilliDelay(5);
	}


	stepperId_2 = configRose.axisId;

	switch (configRose.axisId)
	{
		case ID_AXIS_Z:
		{
			stepper2_step = PIN_AXIS_Z_STEP;
			stepper2_dir = PIN_AXIS_Z_DIR;
			limitPin_Max = configSetup.limit_Max_Z;
			limitPin_Min = configSetup.limit_Min_Z;

			// Get Z axis amplitude steps and max speed
			slideStepsAmplitude = direction * (((configRose.amplitude_Radial_Z / (configSetup.distancePerRev_AxisZ)) * configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z) / 2);  // Amplitude is normally measured from the middle to the top
			newMaxSpd_RoseAxis = configRose.maxSpd_Z * configRose.speedPercent_Z * .01;
			stepperAxis_Z.setPosition(0);
	#ifdef VERBOSE
			Serial.print("Z Initial position:  ");
			Serial.println(stepperAxis_Z.getPosition());
			Serial.print("Z Max position:  ");
			Serial.println(slideStepsAmplitude);

			// Use Absolute position for Return function
			// slideFunc_Axis initial call sets axis position to max amplitude (slideStepsAmplitude).
			Serial.print("startPositionAbs_Axis:  ");
	#endif // VERBOSE
			startPositionAbs_Axis = slideStepsAmplitude;
	#ifdef VERBOSE
			Serial.print(startPositionAbs_Axis);
	#endif // VERBOSE
			// Set Z axis max speed and acceleration
			stepperAxis_Z
				.setMaxSpeed(newMaxSpd_RoseAxis)
				.setAcceleration(configRose.accel_Z)
				.setPosition(slideFunc_Axis(0));
	#ifdef VERBOSE
			Serial.print("Z Initial position_2:  ");
			Serial.println(stepperAxis_Z.getPosition());
	#endif // VERBOSE
			SetEnable(ID_AXIS_Z, true, true);
			//stepperAxis_Z.rotateAsync();
			break;
		}
		case ID_AXIS_X:
		{
			stepper2_step = PIN_AXIS_X_STEP;
			stepper2_dir = PIN_AXIS_X_DIR;
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
			newMaxSpd_RoseAxis = configRose.maxSpd_X * configRose.speedPercent_X * .01;

			// Use Absolute position for Return function
			// slideFunc_Axis initial call sets axis position to max amplitude (slideStepsAmplitude).
			startPositionAbs_Axis = slideStepsAmplitude;
	#ifdef VERBOSE
			Serial.print("returnPositionAbs_Axis:  ");
			Serial.print(startPositionAbs_Axis);
	#endif // VERBOSE
			stepperAxis_X
				.setMaxSpeed(newMaxSpd_RoseAxis)
				.setAcceleration(configRose.accel_X)
				.setPosition(slideFunc_Axis(0)); // set start position of counter	
			SetEnable(ID_AXIS_X, true, true);
			//stepperAxis_X.rotateAsync();
			break;
		}
		case ID_AXIS_3:
		{
			stepper2_step = PIN_AXIS_3_STEP;
			stepper2_dir = PIN_AXIS_3_DIR;
			limitPin_Max = configSetup.limit_Max_M3;
			limitPin_Min = configSetup.limit_Min_M3;
			switch (configSetup.radialOrLinear_Axis_M3)
			{
				case RADIAL_M3:
				{
					slideStepsAmplitude = direction * (DistanceToSteps_M3Radial(configRose.amplitude_Radial_M3) / 2);
					break;
				}
				case LINEAR_M3:
				{
					slideStepsAmplitude = direction * (DistanceToSteps_M3Linear(configRose.amplitude_Radial_M3) / 2);
					break;
				}
			}

			newMaxSpd_RoseAxis = configRose.maxSpd_M3 * configRose.speedPercent_M3 * .01;

			// Use Absolute position for Return function
			// slideFunc_Axis initial call sets axis position to max amplitude (slideStepsAmplitude).
			startPositionAbs_Axis = slideStepsAmplitude;
	#ifdef VERBOSE
			Serial.print("returnPositionAbs_Axis:  ");
			Serial.print(startPositionAbs_Axis);
	#endif // VERBOSE
			stepperAxis_M3
				.setMaxSpeed(newMaxSpd_RoseAxis)
				.setAcceleration(configRose.accel_M3)
				.setPosition(slideFunc_Axis(0)); // set start position of counter
			SetEnable(ID_AXIS_3, true, true);
			//stepperAxis_M3.rotateAsync();
			break;
		}
		case ID_AXIS_4:
		{
			stepper2_step = PIN_AXIS_4_STEP;
			stepper2_dir = PIN_AXIS_4_DIR;
			limitPin_Max = configSetup.limit_Max_M4;
			limitPin_Min = configSetup.limit_Min_M4;
			switch (configSetup.radialOrLinear_Axis_M4)
			{
				case RADIAL_M4:
				{
					slideStepsAmplitude = direction * (DistanceToSteps_M4Radial(configRose.amplitude_Radial_M4) / 2);
					break;
				}
				case LINEAR_M4:
				{
					slideStepsAmplitude = direction * (DistanceToSteps_M4Linear(configRose.amplitude_Radial_M4) / 2);
					break;
				}
			}

			newMaxSpd_RoseAxis = configRose.maxSpd_M4 * configRose.speedPercent_M4 * .01;

			// Use Absolute position for Return function
			// slideFunc_Axis initial call sets axis position to max amplitude (slideStepsAmplitude).
			startPositionAbs_Axis = slideStepsAmplitude;
	#ifdef VERBOSE
			Serial.print("returnPositionAbs_Axis:  ");
			Serial.print(startPositionAbs_Axis);
	#endif // VERBOSE
			stepperAxis_M4
				.setMaxSpeed(newMaxSpd_RoseAxis)
				.setAcceleration(configRose.accel_M4)
				.setPosition(slideFunc_Axis(0)); // set start position of counter
			SetEnable(ID_AXIS_4, true, true);
			//stepperAxis_M4.rotateAsync();
			break;
		}
	}

	// Set limit switches HIGH
	digitalWrite(limitPin_Max, HIGH);
	digitalWrite(limitPin_Min, HIGH);

	ProgressReport(spindleMaxRotation, 0);
	Serial.println("Spindle Move Async");
	groupSpindle.startMove();
	//groupSpindle.startRotate();
	//stepperSpindle.moveAsync();
	switch (configRose.axisId)
	{
		case ID_AXIS_Z:
		{
			stepperAxis_Z.rotateAsync();
			break;
		}
		case ID_AXIS_X:
		{
			stepperAxis_X.rotateAsync();
			break;
		}
		case ID_AXIS_3:
		{
			stepperAxis_M3.rotateAsync();
			break;
		}
		case ID_AXIS_4:
		{
			stepperAxis_M4.rotateAsync();
			break;
		}
	}


	tick();

	// use a timer to periodically calculate new targets for the slide
	tickTimer.priority(priorityLevel); // lowest priority, potentially long caclulations need to be interruptable by TeensyStep
	tickTimer.begin(tick, recalcPeriod);
#ifdef VERBOSE
	Serial.print("spindleMaxRotation:");
	Serial.println(spindleMaxRotation);
#endif // VERBOSE
	while (runPattern)
	{
		spindleCurrentPosition = stepperSpindle.getPosition();

		if (spindleCurrentPosition < 0)
		{
			spindleCurrentPosition *= (-1);
		}

		if (spindleMaxRotation > 0)
		{
			MilliDelay(100);
			currentPosition = stepperSpindle.getPosition();

			ProgressReport(spindleMaxRotation, currentPosition);
			if (spindleCurrentPosition >= spindleMaxRotation)
			{
				tickTimer.end();

				stepperAxis_Z.stopAsync();
				stepperAxis_X.stopAsync();
				stepperAxis_M3.stopAsync();
				stepperAxis_M4.stopAsync();
				stepperSpindle.stopAsync();
				MilliDelay(200);
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

					stepperAxis_Z.stop();
					stepperAxis_X.stop();
					stepperAxis_M3.stop();
					stepperAxis_M4.stop();
					stepperSpindle.stop();
					MilliDelay(200);
					runPattern = false;
					break;
				}
			}
		}

		if (limitPin_Max > 9 && limitPin_Max < 40)
		{
			if ((digitalRead(limitPin_Max) != configSetup.limit_NCorNO))// && axisSteps > 0)
			{
#ifdef DEBUG
				Serial.println("                          limitPin_Max");
#endif // DEBUG
				tickTimer.end();
				stepperAxis_Z.stop();
				stepperAxis_X.stop();
				stepperAxis_M3.stop();
				stepperAxis_M4.stop();
				stepperSpindle.stop();
				SetLimitColors(PAGE_ROSE, DIR_CW);
				MilliDelay(200);
				runPattern = false;
			}
		}
		if (limitPin_Min > 9 && limitPin_Min < 40)
		{
			if ((digitalRead(limitPin_Min) != configSetup.limit_NCorNO))// && axisSteps < 0)
			{
#ifdef DEBUG
				Serial.println("                          limitPin_Min");
#endif // DEBUG
				tickTimer.end();

				stepperAxis_Z.stop();
				stepperAxis_X.stop();
				stepperAxis_M3.stop();
				stepperAxis_M4.stop();
				stepperSpindle.stop();
				SetLimitColors(PAGE_ROSE, DIR_CCW);
				MilliDelay(200);
				runPattern = false;
			}
		}
#ifdef ESTOP_ENABLED

		if (configSetup.eStop > 9 && configSetup.eStop < 40)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
#ifdef DEBUG
				Serial.println("                          EStop ");
#endif // DEBUG
				stepperAxis_Z.emergencyStop();
				stepperAxis_X.emergencyStop();
				stepperAxis_M3.emergencyStop();
				stepperAxis_M4.emergencyStop();
				stepperSpindle.emergencyStop();
				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEnable(ID_AXIS_4, false, false);
				SetEStopColors(PAGE_ROSE);
				if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
				{
					digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
				}
				return;
			}
		}
#endif // estop
		MilliDelay(50);
	} // End while

#ifdef VERBOSE
	Serial.println("===============");
	Serial.print("Spindle Reset Position: ");
	Serial.println(stepperSpindle.getPosition());
#endif // VERBOSE
	// Disable spindle
	//SetEnable(configRose.axisId, false, true);
	SetEnable(ID_SPINDLE, false, true);
	SetEnable(ID_AXIS_Z, false, true);
	SetEnable(ID_AXIS_X, false, true);
	SetEnable(ID_AXIS_3, false, true);
	SetEnable(ID_AXIS_4, false, true);

	stepperPosition_1 = endPosition_Spindle * (-1);
	endPosition_Spindle = stepperSpindle.getPosition();
	endPosition_Z = stepperAxis_Z.getPosition();
	endPosition_X = stepperAxis_X.getPosition();
	endPosition_M3 = stepperAxis_M3.getPosition();
	endPosition_M4 = stepperAxis_M4.getPosition();
	int32_t relativeEndPosition = slideStepsAmplitude - endPosition_Axis;
	Serial.print("slideStepsAmplitude - endPosition_Axis: ");
	Serial.println(relativeEndPosition);
	endPosition_Axis = relativeEndPosition;
	stepperPosition_2 = endPosition_Axis;// *(-1);


#ifdef SHOW_POSITION 
	degrees_Spindle = StepsToDegrees(endPosition_Spindle, ID_SPINDLE);
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
	Serial.print("endPosition_Spindle:");
	Serial.println(endPosition_Spindle);
	Serial.print("endPosition_Axis: ");
	Serial.println(endPosition_Axis);
	Serial.println("---------------");
#endif // DEBUG
	// 0 = Change to disabled, 1 = keep enabled
	if (configSetup.keepSteppersEnabled == 1)
	{
		SerialPrint("pageRose.bt10.val=1"); // 1 = enabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageRose.bt11.val=1"); // 1 = enabled
		SerialPrint("\xFF\xFF\xFF");
	}
	else
	{
		SerialPrint("pageRose.bt10.val=0"); // 0 = disabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageRose.bt10.txt="); // 0 = disabled
		SerialPrint("\x22");
		SerialPrint("Disabled");
		SerialPrint("\x22\xFF\xFF\xFF");

		SerialPrint("pageRose.bt11.val=0"); // 0 = disabled
		SerialPrint("\xFF\xFF\xFF");
		SerialPrint("pageRose.bt11.txt="); // 0 = disabled
		SerialPrint("\x22");
		SerialPrint("Disabled");
		SerialPrint("\x22\xFF\xFF\xFF");
	}
	if (configSetup.auxillaryPin > 9 && configSetup.auxillaryPin < 40)
	{
		digitalWrite(configSetup.auxillaryPin, HIGH);  // Disable
	}
}


// ========================================================
 //Test EEPROM
 //========================================================

/// <summary>
/// Write current Setup screen eeprom values to Nextion
/// </summary>
/// <comment>
/// </comment>
/// <returns></returns>
//void TestEEPROMSetup()
//{
//	//const char* pageSpindle_t42 = "pageSpindle.t42.txt=";
//	//const char* pageSpindle_t43 = "pageSpindle.t43.txt=";
//	//const char* pageSpindle_t44 = "pageSpindle.t44.txt=";
//	//const char* pageSpindle_t45 = "pageSpindle.t45.txt=";
//
//	const char* pageZ_t46 = "pageZ.t46.txt=";
//	const char* pageZ_t47 = "pageZ.t47.txt=";
//	const char* pageZ_t48 = "pageZ.t48.txt=";
//	const char* pageZ_t49 = "pageZ.t49.txt=";
//
//	const char* pageMotor_3_t56 = "pageMotor_3.t56.txt=";
//	const char* pageMotor_3_t57 = "pageMotor_3.t57.txt=";
//	const char* pageMotor_3_t58 = "pageMotor_3.t58.txt=";
//	const char* pageMotor_3_t59 = "pageMotor_3.t59.txt=";
//	const char* pageMotor_3_t64 = "pageMotor_3.t64.txt=";
//
//	const char* pageMotor_4_t56 = "pageMotor_4.t56.txt=";
//	const char* pageMotor_4_t57 = "pageMotor_4.t57.txt=";
//	const char* pageMotor_4_t58 = "pageMotor_4.t58.txt=";
//	const char* pageMotor_4_t59 = "pageMotor_4.t59.txt=";
//	const char* pageMotor_4_t64 = "pageMotor_4.t64.txt=";
//
//	// Radial
//	const char* pageMotor_3_c0_0 = "pageMotor_3.c0.val=0";
//	const char* pageMotor_3_c0_1 = "pageMotor_3.c0.val=1";
//
//	const char* pageMotor_4_c0_0 = "pageMotor_4.c0.val=0";
//	const char* pageMotor_4_c0_1 = "pageMotor_4.c0.val=1";
//
//	// Linear
//	const char* pageMotor_3_c1_0 = "pageMotor_3.c1.val=0";
//	const char* pageMotor_3_c1_1 = "pageMotor_3.c1.val=1";
//
//	const char* pageMotor_4_c1_0 = "pageMotor_4.c1.val=0";
//	const char* pageMotor_4_c1_1 = "pageMotor_4.c1.val=1";
//
//	const char* pageX_t60 = "pageX.t60.txt=";
//	const char* pageX_t61 = "pageX.t61.txt=";
//	const char* pageX_t62 = "pageX.t62.txt=";
//	const char* pageX_t63 = "pageX.t63.txt=";
//	const char* pageX_c2_0 = "pageX.c2.val=0";
//	const char* pageX_c2_1 = "pageX.c2.val=1";
//	const char* pageX_c3_0 = "pageX.c3.val=0";
//	const char* pageX_c3_1 = "pageX.c3.val=1";
//
//	//const char* pageAlternateX_c2_0 = "pageAlternateX.c2.val=0";
//	//const char* pageAlternateX_c2_1 = "pageAlternateX.c2.val=1";
//	//const char* pageAlternateX_c3_0 = "pageAlternateX.c3.val=0";
//	//const char* pageAlternateX_c3_1 = "pageAlternateX.c3.val=1";
//
//	//const char* pageAlternateX_t60 = "pageAlternateX.t60.txt=";
//	//const char* pageAlternateX_t61 = "pageAlternateX.t61.txt=";
//	//const char* pageAlternateX_t62 = "pageAlternateX.t62.txt=";
//	//const char* pageAlternateX_t63 = "pageAlternateX.t63.txt=";
//
//	// More
//	const char* pageMore_c1_0 = "pageMore.c1.val=0";
//	const char* pageMore_c1_1 = "pageMore.c1.val=1";
//	const char* pageMore_t8 = "pageMore.t8.txt=";
//
//	const char* lowChar = "Low";
//	const char* highChar = "High";
//
//	configPageSetup eePromPageSetup;
//	EEPROM.get(eePromAddress_Setup, eePromPageSetup);
//
//
//	// Write the settings to Nextion LCD
//	//SerialPrint(pageSpindle_t42);
//	//SerialWrite(0x22);
//	//SerialPrint(eePromPageSetup.microsteps_Spindle);
//	//SerialPrint(nextionQuoteEnd);
//
//	//SerialPrint(pageSpindle_t43);
//	//SerialWrite(0x22);
//	//SerialPrint(eePromPageSetup.steps360_Spindle);
//	//SerialPrint(nextionQuoteEnd);
//
//	//SerialPrint(pageSpindle_t44);
//	//SerialWrite(0x22);
//	//SerialPrint(eePromPageSetup.gearRatio_Spindle, 4);
//	//SerialPrint(nextionQuoteEnd);
//
//	//SerialPrint(pageSpindle_t45);
//	//SerialWrite(0x22);
//	//SerialPrint(eePromPageSetup.polarity_Spindle ? lowChar : highChar);
//	//SerialPrint(nextionQuoteEnd);
//
//	//SerialPrint(pageZ_t46);
//	//SerialWrite(0x22);
//	//SerialPrint(eePromPageSetup.microsteps_Axis_Z);
//	//SerialPrint(nextionQuoteEnd);
//
//	//SerialPrint(pageZ_t47);
//	//SerialWrite(0x22);
//	//SerialPrint(eePromPageSetup.steps360_Axis_Z);
//	//SerialPrint(nextionQuoteEnd);
//
//	//SerialPrint(pageZ_t48);
//	//SerialWrite(0x22);
//	//SerialPrint(eePromPageSetup.distancePerRev_AxisZ, 6);
//	//SerialPrint(nextionQuoteEnd);
//
//	//Serial.print("configSetup.distancePerRev_AxisZ:  ");
//	//Serial.println(configSetup.distancePerRev_AxisZ, 8);
//	//Serial.print("eePromPageSetup.distancePerRev_AxisZ:  ");
//	//Serial.println(eePromPageSetup.distancePerRev_AxisZ, 8);
//
//	//SerialPrint(pageZ_t49);
//	//SerialWrite(0x22);
//	//SerialPrint(eePromPageSetup.polarity_Axis_Z ? lowChar : highChar);
//	//SerialPrint(nextionQuoteEnd);
//
//	// Page X
//	//SerialPrint(pageX_t60);
//	//SerialWrite(0x22);
//	//SerialPrint(eePromPageSetup.microsteps_Axis_X);
//	//SerialPrint(nextionQuoteEnd);
//
//	//SerialPrint(pageX_t61);
//	//SerialWrite(0x22);
//	//SerialPrint(eePromPageSetup.steps360_Axis_X);
//	//SerialPrint(nextionQuoteEnd);
//
//	//SerialPrint(pageX_t62);
//	//SerialWrite(0x22);
//	//SerialPrint(eePromPageSetup.distancePerRev_AxisX, 5);
//	//SerialPrint(nextionQuoteEnd);
//
//	//SerialPrint(pageX_t63);
//	//SerialWrite(0x22);
//	//SerialPrint(eePromPageSetup.polarity_Axis_X ? lowChar : highChar);
//	//SerialPrint(nextionQuoteEnd);
//
//	// Page AlternateX
//	//SerialPrint(pageAlternateX_t60);
//	//SerialWrite(0x22);
//	//SerialPrint(eePromPageSetup.microsteps_Axis_XAlt);
//	//SerialPrint(nextionQuoteEnd);
//
//	//SerialPrint(pageAlternateX_t61);
//	//SerialWrite(0x22);
//	//SerialPrint(eePromPageSetup.steps360_Axis_XAlt);
//	//SerialPrint(nextionQuoteEnd);
//
//	//SerialPrint(pageAlternateX_t62);
//	//SerialWrite(0x22);
//	//SerialPrint(eePromPageSetup.distancePerRev_AxisXAlt, 5);
//	//SerialPrint(nextionQuoteEnd);
//
//	//SerialPrint(pageAlternateX_t63);
//	//SerialWrite(0x22);
//	//SerialPrint(eePromPageSetup.polarity_Axis_XAlt ? lowChar : highChar);
//	//SerialPrint(nextionQuoteEnd);
//
//	//if (eePromPageSetup.xAltX == 1) // X axis
//	//{
//	//	SerialPrint(pageX_c2_0);
//	//	SerialPrint(nextionEnd);
//	//	SerialPrint(pageX_c3_1);
//	//	SerialPrint(nextionEnd);
//
//	//	SerialPrint(pageAlternateX_c2_0);
//	//	SerialPrint(nextionEnd);
//	//	SerialPrint(pageAlternateX_c3_1);
//	//	SerialPrint(nextionEnd);
//	//}
//	//else  // AltX axis
//	//{
//	//	SerialPrint(pageX_c2_1);
//	//	SerialPrint(nextionEnd);
//	//	SerialPrint(pageX_c3_0);
//	//	SerialPrint(nextionEnd);
//
//	//	SerialPrint(pageAlternateX_c2_1);
//	//	SerialPrint(nextionEnd);
//	//	SerialPrint(pageAlternateX_c3_0);
//	//	SerialPrint(nextionEnd);
//	//}
//
//	// pageMotor_3
//	//SerialPrint(pageMotor_3_t56);
//	//SerialWrite(0x22);
//	//SerialPrint(eePromPageSetup.microsteps_Axis_M3);
//	//SerialPrint(nextionQuoteEnd);
//
//	//SerialPrint(pageMotor_3_t57);
//	//SerialWrite(0x22);
//	//SerialPrint(eePromPageSetup.steps360_Axis_M3);
//	//SerialPrint(nextionQuoteEnd);
//
//	//SerialPrint(pageMotor_3_t58);
//	//SerialWrite(0x22);
//	//SerialPrint(eePromPageSetup.gearRatio_AxisM3, 4);
//	//SerialPrint(nextionQuoteEnd);
//
//	//SerialPrint(pageMotor_3_t59);
//	//SerialWrite(0x22);
//	//SerialPrint(eePromPageSetup.polarity_Axis_M3 ? lowChar : highChar);
//	//SerialPrint(nextionQuoteEnd);
//
//	//// True=Linear, False = Radial
//	//if (eePromPageSetup.radialOrLinear_Axis_M3)
//	//{
//	//	SerialPrint(pageMotor_3_c0_0);
//	//	SerialPrint(nextionEnd);
//	//	SerialPrint(pageMotor_3_c1_1);
//	//	SerialPrint(nextionEnd);
//	//}
//	//else
//	//{
//	//	SerialPrint(pageMotor_3_c0_1);
//	//	SerialPrint(nextionEnd);
//	//	SerialPrint(pageMotor_3_c1_0);
//	//	SerialPrint(nextionEnd);
//	//}
//
//	//SerialPrint(pageMotor_3_t64);
//	//SerialWrite(0x22);
//	//SerialPrint(eePromPageSetup.distancePerRev_AxisM3, 4);
//	//SerialPrint(nextionQuoteEnd);
//
//	// pageMotor_4
//	SerialPrint(pageMotor_4_t56);
//	SerialWrite(0x22);
//	SerialPrint(eePromPageSetup.microsteps_Axis_M4);
//	SerialPrint(nextionQuoteEnd);
//
//	SerialPrint(pageMotor_4_t57);
//	SerialWrite(0x22);
//	SerialPrint(configSetup.steps360_Axis_M4);
//	SerialPrint(nextionQuoteEnd);
//
//	SerialPrint(pageMotor_4_t58);
//	SerialWrite(0x22);
//	SerialPrint(eePromPageSetup.gearRatio_AxisM4, 5);
//	SerialPrint(nextionQuoteEnd);
//
//	SerialPrint(pageMotor_4_t59);
//	SerialWrite(0x22);
//	SerialPrint(eePromPageSetup.polarity_Axis_M4 ? lowChar : highChar);
//	SerialPrint(nextionQuoteEnd);
//
//	// True=Linear, False = Radial
//	if (eePromPageSetup.radialOrLinear_Axis_M4)
//	{
//		SerialPrint(pageMotor_4_c0_0);
//		SerialPrint(nextionEnd);
//		SerialPrint(pageMotor_4_c1_1);
//		SerialPrint(nextionEnd);
//	}
//	else
//	{
//		SerialPrint(pageMotor_4_c0_1);
//		SerialPrint(nextionEnd);
//		SerialPrint(pageMotor_4_c1_0);
//		SerialPrint(nextionEnd);
//	}
//
//	SerialPrint(pageMotor_4_t64);
//	SerialWrite(0x22);
//	SerialPrint(eePromPageSetup.distancePerRev_AxisM4, 4);
//	SerialPrint(nextionQuoteEnd);
//
//	// pageMore
//	SerialPrint(pageMore_t8);
//	SerialWrite(0x22);
//	SerialPrint(eePromPageSetup.auxillaryPin);
//	SerialPrint(nextionQuoteEnd);
//
//	// 0 = Disabled, 1 = Enabled
//	if (eePromPageSetup.keepSteppersEnabled == 0)
//	{
//		SerialPrint(pageMore_c1_0);
//		SerialPrint(nextionEnd);
//	}
//	else
//	{
//		SerialPrint(pageMore_c1_1);
//		SerialPrint(nextionEnd);
//	}
//
//	// Update Nextion
//	SerialPrint("pageSetup.bt1.bco=23964");
//	SerialPrint(nextionEnd);
//	SerialPrint("pageSetup.bt1.val=0");
//	SerialPrint(nextionEnd);
//}

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

	const char* pageLimits_t6 = "pageLimits.t6.txt="; //M4 Max
	const char* pageLimits_t7 = "pageLimits.t7.txt="; //M4 Min

	const char* pageLimits_t13 = "pageLimits.t13.txt=";

	const char* pageLimits_c1_0 = "pageLimits.c1.val=0";
	const char* pageLimits_c1_1 = "pageLimits.c1.val=1";
	const char* pageLimits_c2_0 = "pageLimits.c2.val=0";
	const char* pageLimits_c2_1 = "pageLimits.c2.val=1";
	const char* pageLimits_c3_0 = "pageLimits.c3.val=0";
	const char* pageLimits_c3_1 = "pageLimits.c3.val=1";

	configPageSetup eePromPageSetup;
	EEPROM.get(eePromAddress_Setup, eePromPageSetup);

	MilliDelay(200);
	SerialPrint(pageLimits_t0);
	SerialWrite(0x22);
	SerialPrint(configSetup.limit_Min_Z);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageLimits_t1);
	SerialWrite(0x22);
	SerialPrint(configSetup.limit_Max_Z);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageLimits_t2);
	SerialWrite(0x22);
	SerialPrint(configSetup.limit_Min_X);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageLimits_t3);
	SerialWrite(0x22);
	SerialPrint(configSetup.limit_Max_X);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageLimits_t4);
	SerialWrite(0x22);
	SerialPrint(configSetup.limit_Min_M3);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageLimits_t5);
	SerialWrite(0x22);
	SerialPrint(configSetup.limit_Max_M3);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageLimits_t7);
	SerialWrite(0x22);
	SerialPrint(configSetup.limit_Min_M4);
	SerialPrint(nextionQuoteEnd);
	Serial.print("M4 Min:  ");
	Serial.println(configSetup.limit_Min_M4);

	SerialPrint(pageLimits_t6);
	SerialWrite(0x22);
	SerialPrint(configSetup.limit_Max_M4);
	SerialPrint(nextionQuoteEnd);
	Serial.print("M4 Max:  ");
	Serial.println(configSetup.limit_Max_M4);

	SerialPrint(pageLimits_t13);
	SerialWrite(0x22);
	SerialPrint(configSetup.eStop);
	SerialPrint(nextionQuoteEnd);

	Serial.print("StopSpindle: ");
	Serial.println(configSetup.limit_StopSpindle);
	if (configSetup.limit_StopSpindle)
	{
		SerialPrint(pageLimits_c1_1);
	}
	else
	{
		SerialPrint(pageLimits_c1_0);
	}
	SerialPrint(nextionEnd);

	if (configSetup.limit_NCorNO == 0)
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
	const char* pageReturns_t0 = "pageReturns.t0.txt=";
	const char* pageReturns_t1 = "pageReturns.t1.txt=";
	const char* pageReturns_t2 = "pageReturns.t2.txt=";
	const char* pageReturns_t3 = "pageReturns.t3.txt=";
	const char* pageReturns_t7 = "pageReturns.t7.txt=";
	const char* pageReturns_t8 = "pageReturns.t8.txt=";
	const char* pageReturns_t9 = "pageReturns.t9.txt=";
	const char* pageReturns_t10 = "pageReturns.t10.txt=";
	const char* pageReturns_t15 = "pageReturns.t15.txt=";
	const char* pageReturns_t16 = "pageReturns.t16.txt=";

	LoadPage_Returns();
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
	SerialPrint(eePromPageSetup.maxSpd_Return_Axis_M3);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageReturns_t10);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.accel_Return_Axis_M3);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageReturns_t15);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.maxSpd_Return_Axis_M4);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageReturns_t16);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.accel_Return_Axis_M4);
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
	Serial.print("TestAllTeensyEEPROMValues");
	Serial.println(pageCallerId);
	switch (pageCallerId)
	{
	case PAGE_MAIN:
	{
		value24 = 0;
		value16 = configSetup.vendorId;
		value8 = configMain.axisId;
		value0 = configMain.speedPercent_Spindle;

		packedValue = (value24 << 24) | (value16 << 16) | (value8 << 8) | (value0);

		SerialPrint("pageEEPROM.t301.txt=");
		SerialWrite(0x22);
		SerialPrint(configMain.maxSpd_Spindle);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t302.txt=");
		SerialWrite(0x22);
		SerialPrint(configMain.accel_Spindle);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t300.txt=");
		SerialWrite(0x22);
		SerialPrint(configMain.speedPercent_Spindle);
		SerialPrint(nextionQuoteEnd);

		switch (configMain.axisId)
		{
			case ID_AXIS_Z:
			{
				Serial.print("axisId=Z:  ");
				Serial.println(configMain.axisId);

				SerialPrint("pageEEPROM.t303.txt=");
				SerialWrite(0x22);
				SerialPrint("Z");
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t304.txt=");
				SerialWrite(0x22);
				SerialPrint(configMain.speedPercent_Z);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t305.txt=");
				SerialWrite(0x22);
				SerialPrint(configMain.maxSpd_Z);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t306.txt=");
				SerialWrite(0x22);
				SerialPrint(configMain.accel_Z);
				SerialPrint(nextionQuoteEnd);
				break;
			}
			case ID_AXIS_X: // X Axis
			{
				SerialPrint("pageEEPROM.t303.txt=");
				SerialWrite(0x22);
				SerialPrint("X");
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t304.txt=");
				SerialWrite(0x22);
				SerialPrint(configMain.speedPercent_X);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t305.txt=");
				SerialWrite(0x22);
				SerialPrint(configMain.maxSpd_X);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t306.txt=");
				SerialWrite(0x22);
				SerialPrint(configMain.accel_X);
				SerialPrint(nextionQuoteEnd);
				break;
			}
			case ID_AXIS_3: // M3 Axis
			{
				SerialPrint("pageEEPROM.t303.txt=");
				SerialWrite(0x22);
				SerialPrint("M3");
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t304.txt=");
				SerialWrite(0x22);
				SerialPrint(configMain.speedPercent_M3);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t305.txt=");
				SerialWrite(0x22);
				SerialPrint(configMain.maxSpd_M3);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t306.txt=");
				SerialWrite(0x22);
				SerialPrint(configMain.accel_M3);
				SerialPrint(nextionQuoteEnd);
				break;
			}
			case ID_AXIS_4: // M4 Axis
			{
				SerialPrint("pageEEPROM.t303.txt=");
				SerialWrite(0x22);
				SerialPrint("M4");
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t304.txt=");
				SerialWrite(0x22);
				SerialPrint(configMain.speedPercent_M4);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t305.txt=");
				SerialWrite(0x22);
				SerialPrint(configMain.maxSpd_M4);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t306.txt=");
				SerialWrite(0x22);
				SerialPrint(configMain.accel_M4);
				SerialPrint(nextionQuoteEnd);

				break;
			}
			default:
			{
				SerialPrint("pageEEPROM.t303.txt=");
				SerialWrite(0x22);
				SerialPrint(configMain.axisId);
				SerialPrint(nextionQuoteEnd);
				break;
			}
		}

		SerialPrint("pageEEPROM.t307.txt=");
		SerialWrite(0x22);
		if (configMain.synchro_M3_Spindle == 0)
		{
			SerialPrint("No");
		}
		else
		{
			SerialPrint("Yes");
		}
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t308.txt=");
		SerialWrite(0x22);
		SerialPrint(configMain.synchro_M3_Percentage);
		SerialPrint(nextionQuoteEnd);

		//SerialPrint("pageEEPROM.t311.txt=");
		//SerialWrite(0x22);
		//SerialPrint(configSetup.motorCount);
		//SerialPrint(nextionQuoteEnd);

		//SerialPrint("pageEEPROM.t312.txt=");
		//SerialWrite(0x22);
		//SerialPrint(configSetup.vendorId);
		//SerialPrint(nextionQuoteEnd);

		break;
	}

	case PAGE_ONE:
	{
		Serial.print("PageOne.......................: ");
		Serial.println(configOne.axisId);
		switch (configOne.axisId)
		{
			case ID_SPINDLE:
			{
				SerialPrint("pageEEPROM.t300.txt=");
				SerialWrite(0x22);
				SerialPrint("Spindle");
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t303.txt=");
				SerialWrite(0x22);
				SerialPrint(configOne.accel_Spindle);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t301.txt=");
				SerialWrite(0x22);
				SerialPrint(configOne.maxSpd_Spindle);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t302.txt=");
				SerialWrite(0x22);
				SerialPrint(configOne.speedPercent_Spindle);
				SerialPrint(nextionQuoteEnd);
				break;
			}
			case ID_AXIS_Z:
			{
				SerialPrint("pageEEPROM.t300.txt=");
				SerialWrite(0x22);
				SerialPrint("Z");
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t302.txt=");
				SerialWrite(0x22);
				SerialPrint(configOne.speedPercent_Z);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t301.txt=");
				SerialWrite(0x22);
				SerialPrint(configOne.maxSpd_Z);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t303.txt=");
				SerialWrite(0x22);
				SerialPrint(configOne.accel_Z);
				SerialPrint(nextionQuoteEnd);
				break;
			}
			case ID_AXIS_X: // X Axis
			{
				SerialPrint("pageEEPROM.t300.txt=");
				SerialWrite(0x22);
				SerialPrint("X");
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t302.txt=");
				SerialWrite(0x22);
				SerialPrint(configOne.speedPercent_X);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t301.txt=");
				SerialWrite(0x22);
				SerialPrint(configOne.maxSpd_X);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t303.txt=");
				SerialWrite(0x22);
				SerialPrint(configOne.accel_X);
				SerialPrint(nextionQuoteEnd);

				break;
			}
			case ID_AXIS_3: // M3 Axis
			{
				SerialPrint("pageEEPROM.t300.txt=");
				SerialWrite(0x22);
				SerialPrint("M3");
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t302.txt=");
				SerialWrite(0x22);
				SerialPrint(configOne.speedPercent_M3);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t301.txt=");
				SerialWrite(0x22);
				SerialPrint(configOne.maxSpd_M3);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t303.txt=");
				SerialWrite(0x22);
				SerialPrint(configOne.accel_M3);
				SerialPrint(nextionQuoteEnd);

				break;
			}
			case ID_AXIS_4: // M4 Axis
			{
				SerialPrint("pageEEPROM.t300.txt=");
				SerialWrite(0x22);
				SerialPrint("M4");
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t302.txt=");
				SerialWrite(0x22);
				SerialPrint(configOne.speedPercent_M4);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t301.txt=");
				SerialWrite(0x22);
				SerialPrint(configOne.maxSpd_M4);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t303.txt=");
				SerialWrite(0x22);
				SerialPrint(configOne.accel_M4);
				SerialPrint(nextionQuoteEnd);

				break;
			}
		}

		break;
	}

	case PAGE_MULTI:
	{
		// Axis checked
		//vMultiAxesChk.val = sys1 & 0x00FF0000 >> 16
		int checkedAxis = configMulti.multiAxesIDs;// &0x00FF0000 >> 16;
		//int zAxisChecked = 1 & checkedAxis;
		//int xAxisChecked = 2 & checkedAxis;
		//int m3AxisChecked = 4 & checkedAxis;
		//int m4AxisChecked = 8 & checkedAxis;
//		int spindleChecked = 16 & checkedAxis;

		//int axesDirection = configMulti.direction;
		//int zAxisDirection = 1 & axesDirection;
		//int xAxisDirection = 2 & axesDirection;
		//int m3AxisDirection = 4 & axesDirection;
		//int m4AxisDirection = 8 & axesDirection;

		//int spindleDirection = 16 & axesDirection; // Spindle
	int idAndChecked = 0;
		int dirAndChecked = 0;

		// Spindle
		SerialPrint("pageEEPROM.t306.txt=");
		SerialWrite(0x22);
		SerialPrint("Spindle");
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t305.txt=");
		SerialWrite(0x22);

		if (configMulti.direction_Spindle == (1))
		{
			SerialPrint("CCW");
		}
		else
		{
			SerialPrint("CW");
		}
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t304.txt=");
		SerialWrite(0x22);
		SerialPrint(configMulti.targetDistance_Spindle);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t303.txt=");
		SerialWrite(0x22);
		if (configMulti.checked_Spindle == 1)
		{
			SerialPrint("Yes");
		}
		else
		{
			SerialPrint("No");
		}
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t302.txt=");
		SerialWrite(0x22);
		SerialPrint(configMulti.accel_Spindle);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t301.txt=");
		SerialWrite(0x22);
		SerialPrint(configMulti.maxSpd_Spindle);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t300.txt=");
		SerialWrite(0x22);
		SerialPrint(configMulti.speedPercent_Spindle);
		SerialPrint(nextionQuoteEnd);

		Serial.print("configMulti.direction");
		Serial.println(configMulti.direction);
		// Z Axis
		SerialPrint("pageEEPROM.t306.txt=");
		SerialWrite(0x22);
		SerialPrint("Z");
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t307.txt=");
		SerialWrite(0x22);
		SerialPrint(configMulti.speedPercent_Z);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t308.txt=");
		SerialWrite(0x22);
		SerialPrint(configMulti.maxSpd_Z);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t309.txt=");
		SerialWrite(0x22);
		SerialPrint(configMulti.accel_Z);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t310.txt=");
		SerialWrite(0x22);
		idAndChecked = 1 & configMulti.multiAxesIDs;
		if (configMulti.checked_Z == 1)
		{
			SerialPrint("Yes");
		}
		else
		{
			SerialPrint("No");
		}
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t311.txt=");
		SerialWrite(0x22);
		SerialPrint(configMulti.targetDistance_Z);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t312.txt=");
		SerialWrite(0x22);

		if (configMulti.direction_Z == (1))
		{
			SerialPrint("Right");
		}
		else
		{
			SerialPrint("Left");
		}
		SerialPrint(nextionQuoteEnd);
		// X Axis

		SerialPrint("pageEEPROM.t506.txt=");
		SerialWrite(0x22);
		SerialPrint("X");
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t507.txt=");
		SerialWrite(0x22);
		SerialPrint(configMulti.speedPercent_X);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t508.txt=");
		SerialWrite(0x22);
		SerialPrint(configMulti.maxSpd_X);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t509.txt=");
		SerialWrite(0x22);
		SerialPrint(configMulti.accel_X);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t510.txt=");
		SerialWrite(0x22);
		idAndChecked = 2 & configMulti.multiAxesIDs;
		if (configMulti.checked_X == 1)
		{
			SerialPrint("Yes");
		}
		else
		{
			SerialPrint("No");
		}
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t511.txt=");
		SerialWrite(0x22);
		SerialPrint(configMulti.targetDistance_X);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t512.txt=");
		SerialWrite(0x22);
		dirAndChecked = 2 & configMulti.direction;

		if (configMulti.direction_X == (1))
		{
			SerialPrint("Right");
		}
		else
		{
			SerialPrint("Left");
		}
		SerialPrint(nextionQuoteEnd);

		// M3 Axis

		SerialPrint("pageEEPROM.t706.txt=");
		SerialWrite(0x22);
		SerialPrint("M3");
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t707.txt=");
		SerialWrite(0x22);
		SerialPrint(configMulti.speedPercent_M3);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t708.txt=");
		SerialWrite(0x22);
		SerialPrint(configMulti.maxSpd_M3);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t709.txt=");
		SerialWrite(0x22);
		SerialPrint(configMulti.accel_M3);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t710.txt=");
		SerialWrite(0x22);
		idAndChecked = 4 & configMulti.multiAxesIDs;
		if (configMulti.checked_M3 == 1)
		{
			SerialPrint("Yes");
		}
		else
		{
			SerialPrint("No");
		}
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t711.txt=");
		SerialWrite(0x22);
		SerialPrint(configMulti.targetDistance_M3);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t712.txt=");
		SerialWrite(0x22);
		dirAndChecked = 4 & configMulti.direction;
		if (configSetup.radialOrLinear_Axis_M3 != 0)
		{
			if (configMulti.direction_M3 == (1))
			{
				SerialPrint("Right");
			}
			else
			{
				SerialPrint("Left");
			}
		}
		else
		{
			if (configMulti.direction_M3 == (1))
			{
				SerialPrint("CW");
			}
			else
			{
				SerialPrint("CCW");
			}

		}
		SerialPrint(nextionQuoteEnd);

		Serial.print("Radial/Linear M3: ");
		Serial.println(configSetup.radialOrLinear_Axis_M3);
		Serial.print("  dirAndChecked:  ");
		Serial.println(dirAndChecked);

		// M4 Axis
		SerialPrint("pageEEPROM.t906.txt=");
		SerialWrite(0x22);
		SerialPrint("M4");
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t907.txt=");
		SerialWrite(0x22);
		SerialPrint(configMulti.speedPercent_M4);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t908.txt=");
		SerialWrite(0x22);
		SerialPrint(configMulti.maxSpd_M4);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t909.txt=");
		SerialWrite(0x22);
		SerialPrint(configMulti.accel_M4);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t910.txt=");
		SerialWrite(0x22);
		idAndChecked = 8 & configMulti.multiAxesIDs;
		if (configMulti.checked_M4 == 1)
		{
			SerialPrint("Yes");
		}
		else
		{
			SerialPrint("No");
		}
		SerialPrint(nextionQuoteEnd);
		Serial.print("Radial/Linear M4: ");
		Serial.println(configSetup.radialOrLinear_Axis_M4);
		Serial.print("  dirAndChecked:  ");
		Serial.println(dirAndChecked);

		SerialPrint("pageEEPROM.t911.txt=");
		SerialWrite(0x22);
		SerialPrint(configMulti.targetDistance_M4);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t912.txt=");
		SerialWrite(0x22);
		dirAndChecked = 8 & configMulti.direction;
		if (configSetup.radialOrLinear_Axis_M4 != 0)
		{
			if (configMulti.direction_M4 == (1))
			{
				SerialPrint("Right");
			}
			else
			{
				SerialPrint("Left");
			}
		}
		else
		{
			if (configMulti.direction_M4 == (1))
			{
				SerialPrint("CW");
			}
			else
			{
				SerialPrint("CCW");
			}

		}
		SerialPrint(nextionQuoteEnd);

		break;
	}
	case PAGE_INDEX:
	{
		//value24 = configSetup.cutterMotorPin;
		//value16 = configIndex_Prime.indexId;
		//value8 = configIndex_Prime.axisId;
		//value0 = configIndex_Prime.speedPercent_Spindle;

		//packedValue = (value24 << 24) | (value16 << 16) | (value8 << 8) | (value0);

		Serial.print("EEPROM Index axisId: ");
		Serial.println(configIndex_Prime.axisId);
		Serial.print("configIndex_Main.speedPercent_Spindle: ");
		Serial.println(configIndex_Prime.speedPercent_Spindle);

		switch (configIndex_Prime.axisId)
		{
			case ID_SPINDLE:
			{
				SerialPrint("pageEEPROM.t300.txt=");
				SerialWrite(0x22);
				SerialPrint("Spindle");
				SerialPrint(nextionQuoteEnd);

				Serial.print("configIndex_Main.speedPercent_Spindle  2: ");
				Serial.println(configIndex_Prime.speedPercent_Spindle);

				SerialPrint("pageEEPROM.t301.txt=");
				SerialWrite(0x22);
				SerialPrint(configIndex_Prime.speedPercent_Spindle);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t302.txt=");
				SerialWrite(0x22);
				SerialPrint(configIndex_Prime.maxSpd_Spindle);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t303.txt=");
				SerialWrite(0x22);
				SerialPrint(configIndex_Prime.accel_Spindle);
				SerialPrint(nextionQuoteEnd);
				break;
			}

			case ID_AXIS_3:
			{
				SerialPrint("pageEEPROM.t300.txt=");
				SerialWrite(0x22);
				SerialPrint("M3");
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t301.txt=");
				SerialWrite(0x22);
				SerialPrint(configIndex_Prime.speedPercent_M3);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t302.txt=");
				SerialWrite(0x22);
				SerialPrint(configIndex_Prime.maxSpd_M3);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t303.txt=");
				SerialWrite(0x22);
				SerialPrint(configIndex_Prime.accel_M3);
				SerialPrint(nextionQuoteEnd);
				break;
			}
			case ID_AXIS_4:
			{
				SerialPrint("pageEEPROM.t300.txt=");
				SerialWrite(0x22);
				SerialPrint("M4");
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t301.txt=");
				SerialWrite(0x22);
				SerialPrint(configIndex_Prime.speedPercent_M4);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t302.txt=");
				SerialWrite(0x22);
				SerialPrint(configIndex_Prime.maxSpd_M4);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t303.txt=");
				SerialWrite(0x22);
				SerialPrint(configIndex_Prime.accel_M4);
				SerialPrint(nextionQuoteEnd);
				break;
			}

		}

		switch (configIndex_Prime.indexId)
		{
		case ID_INDEX_1:
		{
			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint("1");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t305.txt=");
			SerialWrite(0x22);
			if (configIndex_1.degreeOrDivision == 0)
			{
				SerialPrint("Divisions");
			}
			else
			{
				SerialPrint("Degrees");
			}
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t306.txt=");
			SerialWrite(0x22);
			SerialPrint(configIndex_1.sizeInUnits, 5);
			SerialPrint(nextionQuoteEnd);

			switch (configIndex_1.fileOrFixed)
			{
				case 0:
				case 48:
				{
					SerialPrint("pageEEPROM.t307.txt=");
					SerialWrite(0x22);
					SerialPrint("Fixed");
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case 1:
				case 49:
				{
					SerialPrint("pageEEPROM.t307.txt=");
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
			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint("2");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t305.txt=");
			SerialWrite(0x22);
			if (configIndex_2.degreeOrDivision == 0)
			{
				SerialPrint("Divisions");
			}
			else
			{
				SerialPrint("Degrees");
			}
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t306.txt=");
			SerialWrite(0x22);
			SerialPrint(configIndex_2.sizeInUnits);
			SerialPrint(nextionQuoteEnd);

			switch (configIndex_2.fileOrFixed)
			{
				case 0:
				case 48:
				{
					SerialPrint("pageEEPROM.t307.txt=");
					SerialWrite(0x22);
					SerialPrint("Fixed");
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case 1:
				case 49:
				{
					SerialPrint("pageEEPROM.t307.txt=");
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
			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint("3");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t305.txt=");
			SerialWrite(0x22);
			if (configIndex_3.degreeOrDivision == 0)
			{
				SerialPrint("Divisions");
			}
			else
			{
				SerialPrint("Degrees");
			}
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t306.txt=");
			SerialWrite(0x22);
			SerialPrint(configIndex_3.sizeInUnits);
			SerialPrint(nextionQuoteEnd);

			switch (configIndex_3.fileOrFixed)
			{
				case 0:
				case 48:
				{
					SerialPrint("pageEEPROM.t307.txt=");
					SerialWrite(0x22);
					SerialPrint("Fixed");
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case 1:
				case 49:
				{
					SerialPrint("pageEEPROM.t307.txt=");
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
			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint("4");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t305.txt=");
			SerialWrite(0x22);
			if (configIndex_4.degreeOrDivision == 0)
			{
				SerialPrint("Divisions");
			}
			else
			{
				SerialPrint("Degrees");
			}
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t306.txt=");
			SerialWrite(0x22);
			SerialPrint(configIndex_4.sizeInUnits, 4);
			SerialPrint(nextionQuoteEnd);

			switch (configIndex_4.fileOrFixed)
			{
				case 0:
				case 48:
				{
					SerialPrint("pageEEPROM.t307.txt=");
					SerialWrite(0x22);
					SerialPrint("Fixed");
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case 1:
				case 49:
				{
					SerialPrint("pageEEPROM.t307.txt=");
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
			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint("5");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t305.txt=");
			SerialWrite(0x22);
			if (configIndex_5.degreeOrDivision == 0)
			{
				SerialPrint("Divisions");
			}
			else
			{
				SerialPrint("Degrees");
			}
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t306.txt=");
			SerialWrite(0x22);
			SerialPrint(configIndex_5.sizeInUnits);
			SerialPrint(nextionQuoteEnd);

			switch (configIndex_5.fileOrFixed)
			{
				case 0:
				case 48:
				{
					SerialPrint("pageEEPROM.t307.txt=");
					SerialWrite(0x22);
					SerialPrint("Fixed");
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case 1:
				case 49:
				{
					SerialPrint("pageEEPROM.t307.txt=");
					SerialWrite(0x22);
					SerialPrint("File");
					SerialPrint(nextionQuoteEnd);
					break;
				}
			}
			break;
		}
		}

		SerialPrint("pageEEPROM.t309.txt=");
		SerialWrite(0x22);
		if (configIndex_Prime.synchro_M3_Spindle == 0)
		{
			SerialPrint("No");
		}
		else
		{
			SerialPrint("Yes");
		}
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t310.txt=");
		SerialWrite(0x22);
		SerialPrint(configIndex_Prime.synchro_M3_Percentage);
		SerialPrint(nextionQuoteEnd);
		break;
	}

	case PAGE_MOVE:
	{
		switch (configMove.axisId)
		{
		case ID_MOVE_AXIS_Z1:
		{
			SerialPrint("pageEEPROM.t300.txt=");
			SerialWrite(0x22);
			SerialPrint("Z-1");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t302.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.maxSpd_Z);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t303.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.accel_Z);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t301.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.speedPercent_Z);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.distance_MoveZ1);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case ID_MOVE_AXIS_Z2:
		{
			SerialPrint("pageEEPROM.t300.txt=");
			SerialWrite(0x22);
			SerialPrint("Z-2");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t302.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.maxSpd_Z);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t303.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.accel_Z);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t301.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.speedPercent_Z);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.distance_MoveZ2);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case ID_MOVE_AXIS_X1: // X Axis
		{
			SerialPrint("pageEEPROM.t300.txt=");
			SerialWrite(0x22);
			SerialPrint("X-1");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t302.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.maxSpd_X);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t303.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.accel_X);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t301.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.speedPercent_X);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.distance_MoveX1);
			SerialPrint(nextionQuoteEnd);

			break;
		}
		case ID_MOVE_AXIS_X2: // X Axis
		{
			SerialPrint("pageEEPROM.t300.txt=");
			SerialWrite(0x22);
			SerialPrint("X-2");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t302.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.maxSpd_X);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t303.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.accel_X);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t301.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.speedPercent_X);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.distance_MoveX2);
			SerialPrint(nextionQuoteEnd);

			break;
		}
		case ID_MOVE_AXIS_M3_1: // M3 Axis
		{
			SerialPrint("pageEEPROM.t300.txt=");
			SerialWrite(0x22);
			SerialPrint("M3-1");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t302.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.maxSpd_M3);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t303.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.accel_M3);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t301.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.speedPercent_M3);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.distance_MoveM3_1);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case ID_MOVE_AXIS_M3_2: // M3 Axis
		{
			SerialPrint("pageEEPROM.t300.txt=");
			SerialWrite(0x22);
			SerialPrint("M3-2");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t302.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.maxSpd_M3);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t303.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.accel_M3);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t301.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.speedPercent_M3);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.distance_MoveM3_2);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case ID_MOVE_AXIS_M4_1: // M3 Axis
		{
			SerialPrint("pageEEPROM.t300.txt=");
			SerialWrite(0x22);
			SerialPrint("M4-1");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t302.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.maxSpd_M4);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t303.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.accel_M4);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t301.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.speedPercent_M4);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.distance_MoveM4_1);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case ID_MOVE_AXIS_M4_2: // M4 Axis
		{
			SerialPrint("pageEEPROM.t300.txt=");
			SerialWrite(0x22);
			SerialPrint("M4-2");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t302.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.maxSpd_M4);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t303.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.accel_M4);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t301.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.speedPercent_M4);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.distance_MoveM4_2);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		}
		break;
	}

	case PAGE_SYNC:
	{
		SerialPrint("pageEEPROM.t300.txt=");
		SerialWrite(0x22);
		SerialPrint(configSync.speedPercent_Spindle);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t301.txt=");
		SerialWrite(0x22);
		SerialPrint(configSync.maxSpd_Spindle);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t302.txt=");
		SerialWrite(0x22);
		SerialPrint(configSync.accel_Spindle);
		SerialPrint(nextionQuoteEnd);

		switch (configSync.helixType)
		{
		case 0: //Left
		case 48:
		{
			SerialPrint("pageEEPROM.t307.txt=");
			SerialWrite(0x22);
			SerialPrint("Left");
			SerialPrint(nextionQuoteEnd);

			break;
		}
		case 1: //Right
		case 49:
		{
			SerialPrint("pageEEPROM.t307.txt=");
			SerialWrite(0x22);
			SerialPrint("Right");
			SerialPrint(nextionQuoteEnd);
			break;
		}
		}

		SerialPrint("pageEEPROM.t308.txt=");
		SerialWrite(0x22);
		SerialPrint(configSync.revolutions_Spindle);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t309.txt=");
		SerialWrite(0x22);
		SerialPrint(configSync.distance);
		SerialPrint(nextionQuoteEnd);
		switch (configSync.axisId)
		{
		case ID_AXIS_Z:
		{
			SerialPrint("pageEEPROM.t303.txt=");
			SerialWrite(0x22);
			SerialPrint("Z");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t305.txt=");
			SerialWrite(0x22);
			SerialPrint(configSync.maxSpd_Z);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t306.txt=");
			SerialWrite(0x22);
			SerialPrint(configSync.accel_Z);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint(configSync.speedPercent_Z);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case ID_AXIS_X: // X Axis
		{
			SerialPrint("pageEEPROM.t303.txt=");
			SerialWrite(0x22);
			SerialPrint("X");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t305.txt=");
			SerialWrite(0x22);
			SerialPrint(configSync.maxSpd_X);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t306.txt=");
			SerialWrite(0x22);
			SerialPrint(configSync.accel_X);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint(configSync.speedPercent_X);
			SerialPrint(nextionQuoteEnd);

			break;
		}
		case ID_AXIS_3: // M3 Axis
		{
			SerialPrint("pageEEPROM.t303.txt=");
			SerialWrite(0x22);
			SerialPrint("M3");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t305.txt=");
			SerialWrite(0x22);
			SerialPrint(configSync.maxSpd_M3);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t306.txt=");
			SerialWrite(0x22);
			SerialPrint(configSync.accel_M3);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint(configSync.speedPercent_M3);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case ID_AXIS_4: // M4 Axis
		{
			SerialPrint("pageEEPROM.t303.txt=");
			SerialWrite(0x22);
			SerialPrint("M4");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t305.txt=");
			SerialWrite(0x22);
			SerialPrint(configSync.maxSpd_M4);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t306.txt=");
			SerialWrite(0x22);
			SerialPrint(configSync.accel_M4);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint(configSync.speedPercent_M4);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		}

		break;
	}
	case PAGE_REC:
	{
		SerialPrint("pageEEPROM.t301.txt=");
		SerialWrite(0x22);
		SerialPrint(configRec.maxSpd_Spindle);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t302.txt=");
		SerialWrite(0x22);
		SerialPrint(configRec.accel_Spindle);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t300.txt=");
		SerialWrite(0x22);
		SerialPrint(configRec.speedPercent_Spindle);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t311.txt=");
		SerialWrite(0x22);
		SerialPrint(configRec.style);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t313.txt=");
		SerialWrite(0x22);
		SerialPrint(configRec.grow_SpindleChange);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t314.txt=");
		SerialWrite(0x22);
		SerialPrint(configRec.grow_AxisChange);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t315.txt=");
		SerialWrite(0x22);
		SerialPrint(configRec.noLast);
		SerialPrint(nextionQuoteEnd);

		switch (configRec.radial_Axial)
		{
		case RADIAL: //Radial
		{
			SerialPrint("pageEEPROM.t307.txt=");
			SerialWrite(0x22);
			SerialPrint("Radial");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t308.txt=");
			SerialWrite(0x22);
			SerialPrint(configRec.waves_Radial);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t309.txt=");
			SerialWrite(0x22);
			SerialPrint(configRec.degrees_Radial_Spindle);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t310.txt=");
			SerialWrite(0x22);
			SerialPrint(configRec.amplitude_Radial_Axis);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case AXIAL: //Axial
		{
			SerialPrint("pageEEPROM.t307.txt=");
			SerialWrite(0x22);
			SerialPrint("Axial");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t308.txt=");
			SerialWrite(0x22);
			SerialPrint(configRec.waves_Axial);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t309.txt=");
			SerialWrite(0x22);
			SerialPrint(configRec.amplitude_Axial_Spindle);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t310.txt=");
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
			SerialPrint("pageEEPROM.t303.txt=");
			SerialWrite(0x22);
			SerialPrint("Z");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t305.txt=");
			SerialWrite(0x22);
			SerialPrint(configRec.maxSpd_Z);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t306.txt=");
			SerialWrite(0x22);
			SerialPrint(configRec.accel_Z);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint(configRec.speedPercent_Z);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case ID_AXIS_X: // X Axis
		{
			configPageRec eePromRecPage;
			EEPROM.get(eePromAddress_Rec, eePromRecPage);
			Serial.print("eePromRecPage.maxSpd_Axis_X: ");
			Serial.println(eePromRecPage.maxSpd_X);

			SerialPrint("pageEEPROM.t303.txt=");
			SerialWrite(0x22);
			SerialPrint("X");
			SerialPrint(nextionQuoteEnd);

			Serial.print("configRec.maxSpd_Axis_X: ");
			Serial.print(configRec.maxSpd_X);

			SerialPrint("pageEEPROM.t305.txt=");
			SerialWrite(0x22);
			SerialPrint(configRec.maxSpd_X);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t306.txt=");
			SerialWrite(0x22);
			SerialPrint(configRec.accel_X);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint(configRec.speedPercent_X);
			SerialPrint(nextionQuoteEnd);

			break;
		}
		case ID_AXIS_3: // M3 Axis
		{
			SerialPrint("pageEEPROM.t303.txt=");
			SerialWrite(0x22);
			SerialPrint("M3");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t305.txt=");
			SerialWrite(0x22);
			SerialPrint(configRec.maxSpd_M3);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t306.txt=");
			SerialWrite(0x22);
			SerialPrint(configRec.accel_M3);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint(configRec.speedPercent_M3);
			SerialPrint(nextionQuoteEnd);
			break;
		}

		case ID_AXIS_4: // M4 Axis
		{
			SerialPrint("pageEEPROM.t303.txt=");
			SerialWrite(0x22);
			SerialPrint("M4");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t305.txt=");
			SerialWrite(0x22);
			SerialPrint(configRec.maxSpd_M4);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t306.txt=");
			SerialWrite(0x22);
			SerialPrint(configRec.accel_M4);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint(configRec.speedPercent_M4);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		}

		break;
	}
	case PAGE_GRK:
	{
		SerialPrint("pageEEPROM.t300.txt=");
		SerialWrite(0x22);
		SerialPrint(configGreekKey.speedPercent_Spindle);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t301.txt=");
		SerialWrite(0x22);
		SerialPrint(configGreekKey.maxSpd_Spindle);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t302.txt=");
		SerialWrite(0x22);
		SerialPrint(configGreekKey.accel_Spindle);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t307.txt=");
		SerialWrite(0x22);
		SerialPrint(configGreekKey.patternCount_Grk);
		SerialPrint(nextionQuoteEnd);
		SerialPrint("pageEEPROM.t308.txt=");
		SerialWrite(0x22);
		SerialPrint(configGreekKey.countPatternPer360_Grk);
		SerialPrint(nextionQuoteEnd);
		SerialPrint("pageEEPROM.t309.txt=");
		SerialWrite(0x22);
		SerialPrint(configGreekKey.segmentLength_Grk);
		SerialPrint(nextionQuoteEnd);

		switch (configGreekKey.radialOrAxial_Pattern)
		{
		case RADIAL:
		{
			SerialPrint("pageEEPROM.t310.txt=");
			SerialWrite(0x22);
			SerialPrint("Radial");
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case AXIAL:
		{
			SerialPrint("pageEEPROM.t310.txt=");
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
			SerialPrint("pageEEPROM.t311.txt=");
			SerialWrite(0x22);
			SerialPrint("4a");
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case 3: // 4b
		{
			SerialPrint("pageEEPROM.t311.txt=");
			SerialWrite(0x22);
			SerialPrint("4b");
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case 4: // 2a
		{
			SerialPrint("pageEEPROM.t311.txt=");
			SerialWrite(0x22);
			SerialPrint("2a");
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case 5: // 2b
		{
			SerialPrint("pageEEPROM.t311.txt=");
			SerialWrite(0x22);
			SerialPrint("2b");
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case 6: // 3a
		{
			SerialPrint("pageEEPROM.t311.txt=");
			SerialWrite(0x22);
			SerialPrint("3a");
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case 7: // 3b
		{
			SerialPrint("pageEEPROM.t311.txt=");
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
			SerialPrint("pageEEPROM.t303.txt=");
			SerialWrite(0x22);
			SerialPrint("Z");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t305.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.maxSpd_Z);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t306.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.accel_Z);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.speedPercent_Z);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case ID_AXIS_X: // X Axis
		{
			configPageGreekKey eePromGrkPage;
			EEPROM.get(eePromAddress_Grk, eePromGrkPage);
			SerialPrint("pageEEPROM.t303.txt=");
			SerialWrite(0x22);
			SerialPrint("X");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t305.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.maxSpd_X);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t306.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.accel_X);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.speedPercent_X);
			SerialPrint(nextionQuoteEnd);

			break;
		}
		case ID_AXIS_3: // M3 Axis
		{
			SerialPrint("pageEEPROM.t303.txt=");
			SerialWrite(0x22);
			SerialPrint("M3");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t305.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.maxSpd_M3);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t306.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.accel_M3);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.speedPercent_M3);
			SerialPrint(nextionQuoteEnd);
			break;
		}

		case ID_AXIS_4: // M4 Axis
		{
			SerialPrint("pageEEPROM.t303.txt=");
			SerialWrite(0x22);
			SerialPrint("M4");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t305.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.maxSpd_M4);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t306.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.accel_M4);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.speedPercent_M4);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		}
		break;

	}
	case PAGE_PROGRAM:
	{
		SerialPrint("pageEEPROM.t301.txt=");
		SerialWrite(0x22);
		SerialPrint(configGreekKey.maxSpd_Spindle);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t302.txt=");
		SerialWrite(0x22);
		SerialPrint(configGreekKey.accel_Spindle);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t300.txt=");
		SerialWrite(0x22);
		SerialPrint(configGreekKey.speedPercent_Spindle);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t307.txt=");
		SerialWrite(0x22);
		SerialPrint(configGreekKey.patternCount_Program);
		SerialPrint(nextionQuoteEnd);
		Serial.print("configGreekKey.patternCount_Program");
		Serial.println(configGreekKey.patternCount_Program);

		SerialPrint("pageEEPROM.t308.txt=");
		SerialWrite(0x22);
		SerialPrint(configGreekKey.countPatternPer360_Program);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t309.txt=");
		SerialWrite(0x22);
		SerialPrint(configGreekKey.segmentLength_Program);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t312.txt=");
		SerialWrite(0x22);

		SerialPrint(fileNameProgram);
		SerialPrintln(nextionQuoteEnd);

		switch (configGreekKey.radialOrAxial_Pattern)
		{
		case RADIAL:
		{
			SerialPrint("pageEEPROM.t312.txt=");
			SerialWrite(0x22);
			SerialPrint("Radial");
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case AXIAL:
		{
			SerialPrint("pageEEPROM.t312.txt=");
			SerialWrite(0x22);
			SerialPrint("Axial");
			SerialPrint(nextionQuoteEnd);
			break;
		}
		}

		SerialPrint("pageEEPROM.t310.txt=");
		SerialWrite(0x22);
		switch (configGreekKey.radialOrAxial_File)
		{
		case RADIAL:
		{
			SerialPrint("Radial");
			break;
		}
		case AXIAL:
		{
			SerialPrint("Axial");
			break;
		}
		}

		SerialPrint(nextionQuoteEnd);

		switch (configGreekKey.segmentOrActual)
		{
		case 2:
		{
			SerialPrint("pageEEPROM.t311.txt=");
			SerialWrite(0x22);
			SerialPrint("Segments");
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case 3:
		{
			SerialPrint("pageEEPROM.t311.txt=");
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
			SerialPrint("pageEEPROM.t303.txt=");
			SerialWrite(0x22);
			SerialPrint("Z");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t305.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.maxSpd_Z);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t306.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.accel_Z);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.speedPercent_Z);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case ID_AXIS_X: // X Axis
		{
			SerialPrint("pageEEPROM.t303.txt=");
			SerialWrite(0x22);
			SerialPrint("X");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t305.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.maxSpd_X);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t306.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.accel_X);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.speedPercent_X);
			SerialPrint(nextionQuoteEnd);

			break;
		}
		case ID_AXIS_3: // M3 Axis
		{
			SerialPrint("pageEEPROM.t303.txt=");
			SerialWrite(0x22);
			SerialPrint("M3");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t305.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.maxSpd_M3);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t306.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.accel_M3);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.speedPercent_M3);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case ID_AXIS_4: // M4 Axis
		{
			SerialPrint("pageEEPROM.t303.txt=");
			SerialWrite(0x22);
			SerialPrint("M4");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t305.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.maxSpd_M4);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t306.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.accel_M4);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.speedPercent_M4);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		}
		break;
	}
	case PAGE_ROSE:
	{
		SerialPrint("pageEEPROM.t300.txt=");
		SerialWrite(0x22);
		SerialPrint(configRose.speedPercent_Spindle);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t301.txt=");
		SerialWrite(0x22);
		SerialPrint(configRose.maxSpd_Spindle);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t302.txt=");
		SerialWrite(0x22);
		SerialPrint(configRose.accel_Spindle);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t307.txt=");
		SerialWrite(0x22);
		SerialPrint(configRose.n);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t308.txt=");
		SerialWrite(0x22);
		SerialPrint(configRose.d);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageEEPROM.t309.txt=");
		SerialWrite(0x22);
		SerialPrint(configRose.spindleRevolutions);
		SerialPrint(nextionQuoteEnd);

		switch (configRose.axisId)
		{
		case ID_AXIS_Z:
		{
			SerialPrint("pageEEPROM.t303.txt=");
			SerialWrite(0x22);
			SerialPrint("Z");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint(configRose.speedPercent_Z);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t305.txt=");
			SerialWrite(0x22);
			SerialPrint(configRose.maxSpd_Z);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t306.txt=");
			SerialWrite(0x22);
			SerialPrint(configRose.accel_Z);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t310.txt=");
			SerialWrite(0x22);
			SerialPrint(configRose.amplitude_Radial_Z);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case ID_AXIS_X: // X Axis
		{
			SerialPrint("pageEEPROM.t303.txt=");
			SerialWrite(0x22);
			SerialPrint("X");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t305.txt=");
			SerialWrite(0x22);
			SerialPrint(configRose.maxSpd_X);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t306.txt=");
			SerialWrite(0x22);
			SerialPrint(configRose.accel_X);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint(configRose.speedPercent_X);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t310.txt=");
			SerialWrite(0x22);
			SerialPrint(configRose.amplitude_Radial_X);
			SerialPrint(nextionQuoteEnd);

			break;
		}
		case ID_AXIS_3: // M3 Axis
		{
			SerialPrint("pageEEPROM.t303.txt=");
			SerialWrite(0x22);
			SerialPrint("M3");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t305.txt=");
			SerialWrite(0x22);
			SerialPrint(configRose.maxSpd_M3);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t306.txt=");
			SerialWrite(0x22);
			SerialPrint(configRose.accel_M3);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint(configRose.speedPercent_M3);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t310.txt=");
			SerialWrite(0x22);
			SerialPrint(configRose.amplitude_Radial_M3);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case ID_AXIS_4: // M4 Axis
		{
			SerialPrint("pageEEPROM.t303.txt=");
			SerialWrite(0x22);
			SerialPrint("M4");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t305.txt=");
			SerialWrite(0x22);
			SerialPrint(configRose.maxSpd_M4);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t306.txt=");
			SerialWrite(0x22);
			SerialPrint(configRose.accel_M4);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint(configRose.speedPercent_M4);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t310.txt=");
			SerialWrite(0x22);
			SerialPrint(configRose.amplitude_Radial_M4);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		}

		break;
	}
	}
}


void LoadPage_Main()
{
	Serial.println("Enter LoadPage_Main");

	SerialPrint("pageMain.n51.val=");
	SerialPrint(configMain.maxSpd_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageMain.vMaxSpdSpindle.val=");
	SerialPrint(configMain.maxSpd_Spindle);
	SerialPrint(nextionEnd);
	
		
	SerialPrint("pageMain.n52.val=");
	SerialPrint(configMain.accel_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageMain.vAccelSpindle.val=");
	SerialPrint(configMain.accel_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageMain.vSpdP_Spindle.val=");
	SerialPrint(configMain.speedPercent_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageMain.vSpdT_Spindle.txt=");
	SerialWrite(0x22);
	SerialPrint(configMain.speedPercent_Spindle);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageMain.n1.val=");
	SerialPrint(configMain.speedPercent_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageMain.h1.val=");
	SerialPrint(configMain.speedPercent_Spindle);
	SerialPrint(nextionEnd);

	Serial.print("speedPercent_Spindle: ");
	Serial.println(configMain.speedPercent_Spindle);

	SerialPrint("pageMain.vSpdP_Z.val=");
	SerialPrint(configMain.speedPercent_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageMain.vMaxSpd_Z.val=");
	SerialPrint(configMain.maxSpd_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageMain.vAccel_Z.val=");
	SerialPrint(configMain.accel_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageMain.vSpdP_X.val=");			
	SerialPrint(configMain.speedPercent_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageMain.vMaxSpd_X.val=");
	SerialPrint(configMain.maxSpd_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageMain.vAccel_X.val=");
	SerialPrint(configMain.accel_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageMain.vSpdP_M3.val=");
	SerialPrint(configMain.speedPercent_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageMain.vMaxSpd_M3.val=");
	SerialPrint(configMain.maxSpd_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageMain.vAccel_M3.val=");
	SerialPrint(configMain.accel_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageMain.vSpdP_M4.val=");
	SerialPrint(configMain.speedPercent_M4);
	SerialPrint(nextionEnd);

	SerialPrint("pageMain.vMaxSpd_M4.val=");
	SerialPrint(configMain.maxSpd_M4);
	SerialPrint(nextionEnd);

	SerialPrint("pageMain.vAccel_M4.val=");
	SerialPrint(configMain.accel_M4);
	SerialPrint(nextionEnd);

	switch (configMain.axisId)
	{
		case ID_AXIS_Z:
		{
			SerialPrint("pageMain.n53.val=");
			SerialPrint(configMain.maxSpd_Z);
			SerialPrint(nextionEnd);

			SerialPrint("pageMain.n54.val=");
			SerialPrint(configMain.accel_Z);
			SerialPrint(nextionEnd);

			SerialPrint("pageMain.h0.val=");
			SerialPrint(configMain.speedPercent_Z);
			SerialPrint(nextionEnd);
			break;
		}
		case ID_AXIS_X: // X Axis
		{
			SerialPrint("pageMain.n53.val=");
			//SerialWrite(0x22);
			SerialPrint(configMain.maxSpd_X);
			SerialPrint(nextionEnd);

			SerialPrint("pageMain.n54.val=");
			SerialPrint(configMain.accel_X);
			SerialPrint(nextionEnd);

			SerialPrint("pageMain.h0.val=");
			SerialPrint(configMain.speedPercent_X);
			SerialPrint(nextionEnd);
			break;
		}
		case ID_AXIS_3: // M3 Axis
		{
			SerialPrint("pageMain.n53.val=");
			SerialPrint(configMain.maxSpd_M3);
			SerialPrint(nextionEnd);

			SerialPrint("pageMain.n54.val=");
			SerialPrint(configMain.accel_M3);
			SerialPrint(nextionEnd);

			SerialPrint("pageMain.h0.val=");
			SerialPrint(configMain.speedPercent_M3);
			SerialPrint(nextionEnd);

			if (configSetup.radialOrLinear_Axis_M3 == 0)
			{
				SerialPrint("pageMain.bt5.txt=");
				SerialWrite(0x22);
				SerialPrint("Q");
				SerialPrint(nextionEnd);

				SerialPrint("pageMain.bt6.txt=");
				SerialWrite(0x22);
				SerialPrint("P");
				SerialPrint(nextionEnd);
			}
			else
			{
				SerialPrint("pageMain.bt5.txt=");
				SerialWrite(0x22);
				SerialPrint("Z");
				SerialPrint(nextionEnd);

				SerialPrint("pageMain.bt6.txt=");
				SerialWrite(0x22);
				SerialPrint("[");
				SerialPrint(nextionEnd);
			}
			break;
		}
		case ID_AXIS_4: // M4 Axis
		{
			SerialPrint("pageMain.n53.val=");
			SerialPrint(configMain.maxSpd_M4);
			SerialPrint(nextionEnd);

			SerialPrint("pageMain.n54.val=");
			SerialPrint(configMain.accel_M4);
			SerialPrint(nextionEnd);

			SerialPrint("pageMain.h0.val=");
			SerialPrint(configMain.speedPercent_M4);
			SerialPrint(nextionEnd);


			if (configSetup.radialOrLinear_Axis_M4 == 0)
			{
				SerialPrint("pageMain.bt5.txt=");
				SerialWrite(0x22);
				SerialPrint("Q");
				SerialPrint(nextionEnd);

				SerialPrint("pageMain.bt6.txt=");
				SerialWrite(0x22);
				SerialPrint("P");
				SerialPrint(nextionEnd);
			}
			else
			{
				SerialPrint("pageMain.bt5.txt=");
				SerialWrite(0x22);
				SerialPrint("Z");
				SerialPrint(nextionEnd);

				SerialPrint("pageMain.bt6.txt=");
				SerialWrite(0x22);
				SerialPrint("[");
				SerialPrint(nextionEnd);
			}


			break;
		}

	}

		//SerialPrint("pageMain.bt8.val=");
		//SerialPrint(configMain.synchro_M3_Spindle);
		//SerialPrint(nextionEnd);

		//SerialPrint("pageSplash.vSynchro_Main.txt=");
		//SerialWrite(0x22);
		//SerialPrint(configMain.synchro_M3_Percentage);
		//SerialPrint(nextionQuoteEnd);

		//

		//	//xxxSerialPrint("pageSplash.vSynchroVal_M.val=");
		//SerialPrint(configMain.synchro_M3_Spindle);
		//SerialPrint(nextionEnd);

		//SerialPrint("pageEEPROM.t311.txt=");
		//SerialWrite(0x22);
		//SerialPrint(configSetup.motorCount);
		//SerialPrint(nextionQuoteEnd);

		//SerialPrint("pageEEPROM.t312.txt=");
		//SerialWrite(0x22);
		//SerialPrint(configSetup.vendorId);
		//SerialPrint(nextionQuoteEnd);


}

void LoadPage_One()
{
	Serial.println("Enter LoadPage_One");

	SerialPrint("pageOne.n51.val=");
	SerialPrint(configOne.maxSpd_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageOne.vMaxSpdSpindle.val=");
	SerialPrint(configOne.maxSpd_Spindle);
	SerialPrint(nextionEnd);


	SerialPrint("pageOne.n52.val=");
	SerialPrint(configOne.accel_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageOne.vAccelSpindle.val=");
	SerialPrint(configOne.accel_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageOne.vSpdP_Spindle.val=");
	SerialPrint(configOne.speedPercent_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageOne.vSpdT_Spindle.txt=");
	SerialWrite(0x22);
	SerialPrint(configOne.speedPercent_Spindle);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageOne.n1.val=");
	SerialPrint(configOne.speedPercent_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageOne.h1.val=");
	SerialPrint(configOne.speedPercent_Spindle);
	SerialPrint(nextionEnd);

	Serial.print("speedPercent_Spindle: ");
	Serial.println(configOne.speedPercent_Spindle);

	SerialPrint("pageOne.vSpdP_Z.val=");
	SerialPrint(configOne.speedPercent_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageOne.vMaxSpd_Z.val=");
	SerialPrint(configOne.maxSpd_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageOne.vAccel_Z.val=");
	SerialPrint(configOne.accel_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageOne.vSpdP_X.val=");
	SerialPrint(configOne.speedPercent_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageOne.vMaxSpd_X.val=");
	SerialPrint(configOne.maxSpd_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageOne.vAccel_X.val=");
	SerialPrint(configOne.accel_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageOne.vSpdP_M3.val=");
	SerialPrint(configOne.speedPercent_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageOne.vMaxSpd_M3.val=");
	SerialPrint(configOne.maxSpd_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageOne.vAccel_M3.val=");
	SerialPrint(configOne.accel_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageOne.vSpdP_M4.val=");
	SerialPrint(configOne.speedPercent_M4);
	SerialPrint(nextionEnd);

	SerialPrint("pageOne.vMaxSpd_M4.val=");
	SerialPrint(configOne.maxSpd_M4);
	SerialPrint(nextionEnd);

	SerialPrint("pageOne.vAccel_M4.val=");
	SerialPrint(configOne.accel_M4);
	SerialPrint(nextionEnd);

	switch (configOne.axisId)
	{
	case ID_AXIS_Z:
	{
		SerialPrint("pageOne.n53.val=");
		SerialPrint(configOne.maxSpd_Z);
		SerialPrint(nextionEnd);

		SerialPrint("pageOne.n54.val=");
		SerialPrint(configOne.accel_Z);
		SerialPrint(nextionEnd);

		SerialPrint("pageOne.h0.val=");
		SerialPrint(configOne.speedPercent_Z);
		SerialPrint(nextionEnd);
		break;
	}
	case ID_AXIS_X: // X Axis
	{
		SerialPrint("pageOne.n53.val=");
		//SerialWrite(0x22);
		SerialPrint(configOne.maxSpd_X);
		SerialPrint(nextionEnd);

		SerialPrint("pageOne.n54.val=");
		SerialPrint(configOne.accel_X);
		SerialPrint(nextionEnd);

		SerialPrint("pageOne.h0.val=");
		SerialPrint(configOne.speedPercent_X);
		SerialPrint(nextionEnd);
		break;
	}
	case ID_AXIS_3: // M3 Axis
	{
		SerialPrint("pageOne.n53.val=");
		SerialPrint(configOne.maxSpd_M3);
		SerialPrint(nextionEnd);

		SerialPrint("pageOne.n54.val=");
		SerialPrint(configOne.accel_M3);
		SerialPrint(nextionEnd);

		SerialPrint("pageOne.h0.val=");
		SerialPrint(configOne.speedPercent_M3);
		SerialPrint(nextionEnd);
		break;
	}
	case ID_AXIS_4: // M4 Axis
	{
		SerialPrint("pageOne.n53.val=");
		SerialPrint(configOne.maxSpd_M4);
		SerialPrint(nextionEnd);

		SerialPrint("pageOne.n54.val=");
		SerialPrint(configOne.accel_M4);
		SerialPrint(nextionEnd);

		SerialPrint("pageOne.h0.val=");
		SerialPrint(configOne.speedPercent_M4);
		SerialPrint(nextionEnd);

		break;
	}

	}

	SerialPrint("pageOne.n1.val=");
	SerialPrint(configSetup.radialOrLinear_Axis_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageOne.n3.val=");
	SerialPrint(configSetup.radialOrLinear_Axis_M4);
	SerialPrint(nextionEnd);
	//SerialPrint("pageOne.bt8.val=");
	//SerialPrint(configOne.synchro_M3_Spindle);
	//SerialPrint(nextionEnd);

	//SerialPrint("pageSplash.vSynchro_One.txt=");
	//SerialWrite(0x22);
	//SerialPrint(configOne.synchro_M3_Percentage);
	//SerialPrint(nextionQuoteEnd);

	//

	//	//xxxSerialPrint("pageSplash.vSynchroVal_M.val=");
	//SerialPrint(configOne.synchro_M3_Spindle);
	//SerialPrint(nextionEnd);

	//SerialPrint("pageEEPROM.t311.txt=");
	//SerialWrite(0x22);
	//SerialPrint(configSetup.motorCount);
	//SerialPrint(nextionQuoteEnd);

	//SerialPrint("pageEEPROM.t312.txt=");
	//SerialWrite(0x22);
	//SerialPrint(configSetup.vendorId);
	//SerialPrint(nextionQuoteEnd);


}

void LoadPage_Index()
{
	SerialPrint("pageIndex.vSpdT_Spindle.txt=");
	SerialWrite(0x22);
	SerialPrint(configIndex_Prime.speedPercent_Spindle);
	SerialPrint(nextionQuoteEnd);
	MilliDelay(6);

	SerialPrint("pageIndex.h1.val=");
	SerialPrint(configIndex_Prime.speedPercent_Spindle);
	SerialPrint(nextionEnd);
	MilliDelay(6);

	switch (configIndex_Prime.indexId)
	{
		case INDEX_1:
		{
			SerialPrint("pageIndex.t7.txt=");
			SerialWrite(0x22);
			SerialPrint(configIndex_1.sizeInUnits);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case INDEX_2:
		{
			SerialPrint("pageIndex.t7.txt=");
			SerialWrite(0x22);
			SerialPrint(configIndex_2.sizeInUnits);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case INDEX_3:
		{
			SerialPrint("pageIndex.t7.txt=");
			SerialWrite(0x22);
			SerialPrint(configIndex_3.sizeInUnits);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case INDEX_4:
		{
			SerialPrint("pageIndex.t7.txt=");
			SerialWrite(0x22);
			SerialPrint(configIndex_4.sizeInUnits);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case INDEX_5:
		{
			SerialPrint("pageIndex.t7.txt=");
			SerialWrite(0x22);
			SerialPrint(configIndex_5.sizeInUnits);
			SerialPrint(nextionQuoteEnd);
			break;
		}

	}

	MilliDelay(6);

	// Index Size
	SerialPrint("pageIndex.vaIndexSize_1.txt=");
	SerialWrite(0x22);
	SerialPrint(configIndex_1.sizeInUnits);
	SerialPrint(nextionQuoteEnd);
	MilliDelay(6);

	SerialPrint("pageIndex.t101.txt=");
	SerialWrite(0x22);
	SerialPrint(configIndex_1.sizeInUnits);
	SerialPrint(nextionQuoteEnd);
	MilliDelay(6);

	//2
	SerialPrint("pageIndex.vaIndexSize_2.txt=");
	SerialWrite(0x22);
	SerialPrint(configIndex_2.sizeInUnits);
	SerialPrint(nextionQuoteEnd);
	MilliDelay(6);

	SerialPrint("pageIndex.t102.txt=");
	SerialWrite(0x22);
	SerialPrint(configIndex_2.sizeInUnits);
	SerialPrint(nextionQuoteEnd);
	MilliDelay(6);

	//3
	SerialPrint("pageIndex.vaIndexSize_3.txt=");
	SerialWrite(0x22);
	SerialPrint(configIndex_3.sizeInUnits);
	SerialPrint(nextionQuoteEnd);
	MilliDelay(6);

	SerialPrint("pageIndex.t103.txt=");
	SerialWrite(0x22);
	SerialPrint(configIndex_3.sizeInUnits);
	SerialPrint(nextionQuoteEnd);
	MilliDelay(6);

	//4
	SerialPrint("pageIndex.vaIndexSize_4.txt=");
	SerialWrite(0x22);
	SerialPrint(configIndex_4.sizeInUnits);
	SerialPrint(nextionQuoteEnd);
	MilliDelay(6);

	SerialPrint("pageIndex.t104.txt=");
	SerialWrite(0x22);
	SerialPrint(configIndex_4.sizeInUnits);
	SerialPrint(nextionQuoteEnd);
	MilliDelay(6);

	//5
	SerialPrint("pageIndex.vaIndexSize_5.txt=");
	SerialWrite(0x22);
	SerialPrint(configIndex_5.sizeInUnits);
	SerialPrint(nextionQuoteEnd);
	MilliDelay(6);

	SerialPrint("pageIndex.t105.txt=");
	SerialWrite(0x22);
	SerialPrint(configIndex_5.sizeInUnits);
	SerialPrint(nextionQuoteEnd);
	MilliDelay(6);

	//// Fixed or File
	//SerialPrint("pageIndex.vaFixFile_1.val=");
	//SerialPrint(configIndex_1.fileOrFixed);
	//SerialPrint(nextionEnd);
	//MilliDelay(6);

	//SerialPrint("pageIndex.vaFixFile_2.val=");
	//SerialPrint(configIndex_2.fileOrFixed);
	//SerialPrint(nextionEnd);
	//MilliDelay(6);

	//SerialPrint("pageIndex.vaFixFile_3.val=");
	//SerialPrint(configIndex_3.fileOrFixed);
	//SerialPrint(nextionEnd);
	//MilliDelay(6);

	//SerialPrint("pageIndex.vaFixFile_4.val=");
	//SerialPrint(configIndex_4.fileOrFixed);
	//SerialPrint(nextionEnd);
	//MilliDelay(6);

	//SerialPrint("pageIndex.vaFixFile_5.val=");
	//SerialPrint(configIndex_5.fileOrFixed);
	//SerialPrint(nextionEnd);
	//MilliDelay(6);

	//// Division or Degrees
	//SerialPrint("pageIndex.vaDivDeg_1.val=");
	//SerialPrint(configIndex_1.degreeOrDivision);
	//SerialPrint(nextionEnd);
	//MilliDelay(6);

	//SerialPrint("pageIndex.vaDivDeg_2.val=");
	//SerialPrint(configIndex_2.degreeOrDivision);
	//SerialPrint(nextionEnd);
	//MilliDelay(6);

	//SerialPrint("pageIndex.vaDivDeg_3.val=");
	//SerialPrint(configIndex_3.degreeOrDivision);
	//SerialPrint(nextionEnd);
	//MilliDelay(6);

	//SerialPrint("pageIndex.vaDivDeg_4.val=");
	//SerialPrint(configIndex_4.degreeOrDivision);
	//SerialPrint(nextionEnd);
	//MilliDelay(6);

	//SerialPrint("pageIndex.vaDivDeg_5.val=");
	//SerialPrint(configIndex_5.degreeOrDivision);
	//SerialPrint(nextionEnd);
	//MilliDelay(6);

	//SerialPrint("pageIndex.vaIndexId.val=");
	//SerialPrint(configIndex_Prime.indexId);
	//SerialPrint(nextionEnd);
	//MilliDelay(6);

	SerialPrint("pageIndex.n51.val=");
	SerialPrint(configIndex_Prime.maxSpd_Spindle);
	SerialPrint(nextionEnd);
	MilliDelay(6);

	SerialPrint("pageIndex.n52.val=");
	SerialPrint(configIndex_Prime.accel_Spindle);
	SerialPrint(nextionEnd);
	MilliDelay(6);

	SerialPrint("pageIndex.vAccelSpindle.val=");
	SerialPrint(configIndex_Prime.accel_Spindle);
	SerialPrint(nextionEnd);
	MilliDelay(6);

	SerialPrint("pageIndex.vSpdP_M3.val=");
	SerialPrint(configIndex_Prime.speedPercent_M3);
	SerialPrint(nextionEnd);
	MilliDelay(6);

	SerialPrint("pageIndex.vMaxSpd_M3.val=");
	SerialPrint(configIndex_Prime.maxSpd_M3);
	SerialPrint(nextionEnd);
	MilliDelay(6);

	SerialPrint("pageIndex.vAccel_M3.val=");
	SerialPrint(configIndex_Prime.accel_M3);
	SerialPrint(nextionEnd);
	MilliDelay(6);

	SerialPrint("pageIndex.vSpdP_M4.val=");
	SerialPrint(configIndex_Prime.speedPercent_M4);
	SerialPrint(nextionEnd);
	MilliDelay(6);

	SerialPrint("pageIndex.vMaxSpd_M4.val=");
	SerialPrint(configIndex_Prime.maxSpd_M4);
	SerialPrint(nextionEnd);
	MilliDelay(6);

	SerialPrint("pageIndex.vAccel_M4.val=");
	SerialPrint(configIndex_Prime.accel_M4);
	SerialPrint(nextionEnd);
	MilliDelay(6);

	switch (configIndex_Prime.axisId)
	{

		case ID_AXIS_3: // M3 Axis
		{
			SerialPrint("pageIndex.n53.val=");
			SerialPrint(configIndex_Prime.maxSpd_M3);
			SerialPrint(nextionEnd);
			MilliDelay(6);

			SerialPrint("pageIndex.n54.val=");
			SerialPrint(configIndex_Prime.accel_M3);
			SerialPrint(nextionEnd);
			MilliDelay(6);

			SerialPrint("pageIndex.h0.val=");
			SerialPrint(configIndex_Prime.speedPercent_M3);
			SerialPrint(nextionEnd);
			break;
		}
		case ID_AXIS_4: // M4 Axis
		{
			SerialPrint("pageIndex.n53.val=");
			SerialPrint(configIndex_Prime.maxSpd_M4);
			SerialPrint(nextionEnd);
			MilliDelay(6);

			SerialPrint("pageIndex.n54.val=");
			SerialPrint(configIndex_Prime.accel_M4);
			SerialPrint(nextionEnd);
			MilliDelay(6);

			SerialPrint("pageIndex.h0.val=");
			SerialPrint(configIndex_Prime.speedPercent_M4);
			SerialPrint(nextionEnd);

			break;
		}

	}
	MilliDelay(6);


}

void LoadPage_Move()
{
	Serial.println("Enter LoadPage_Move");


	SerialPrint("pageMove.vSpdP_Z.val=");
	SerialPrint(configMove.speedPercent_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageMove.vMaxSpd_Z.val=");
	SerialPrint(configMove.maxSpd_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageMove.vAccel_Z.val=");
	SerialPrint(configMove.accel_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageMove.vDistance_Z_1.txt=");
	SerialWrite(0x22);
	SerialPrint(configMove.distance_MoveZ1);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageMove.t_Z1.txt=");
	SerialWrite(0x22);
	SerialPrint(configMove.distance_MoveZ1);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageMove.vDistance_Z_2.txt=");
	SerialWrite(0x22);
	SerialPrint(configMove.distance_MoveZ2);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageMove.t_Z2.txt=");
	SerialWrite(0x22);
	SerialPrint(configMove.distance_MoveZ2);
	SerialPrint(nextionQuoteEnd);

	// X
	SerialPrint("pageMove.vSpdP_X.val=");
	SerialPrint(configMove.speedPercent_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageMove.vMaxSpd_X.val=");
	SerialPrint(configMove.maxSpd_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageMove.vAccel_X.val=");
	SerialPrint(configMove.accel_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageMove.vDistance_X_1.txt=");
	SerialWrite(0x22);
	SerialPrint(configMove.distance_MoveX1);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageMove.t_X1.txt=");
	SerialWrite(0x22);
	SerialPrint(configMove.distance_MoveX1);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageMove.vDistance_X_2.txt=");
	SerialWrite(0x22);
	SerialPrint(configMove.distance_MoveX2);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageMove.t_X2.txt=");
	SerialWrite(0x22);
	SerialPrint(configMove.distance_MoveX2);
	SerialPrint(nextionQuoteEnd);

	// M3
	SerialPrint("pageMove.vSpdP_M3.val=");
	SerialPrint(configMove.speedPercent_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageMove.vMaxSpd_M3.val=");
	SerialPrint(configMove.maxSpd_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageMove.vAccel_M3.val=");
	SerialPrint(configMove.accel_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageMove.vDistance_M3_1.txt=");
	SerialWrite(0x22);
	SerialPrint(configMove.distance_MoveM3_1);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageMove.t_M3_1.txt=");
	SerialWrite(0x22);
	SerialPrint(configMove.distance_MoveM3_1);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageMove.vDistance_M3_2.txt=");
	SerialWrite(0x22);
	SerialPrint(configMove.distance_MoveM3_2);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageMove.t_M3_2.txt=");
	SerialWrite(0x22);
	SerialPrint(configMove.distance_MoveM3_2);
	SerialPrint(nextionQuoteEnd);

	// M4
	SerialPrint("pageMove.vSpdP_M4.val=");
	SerialPrint(configMove.speedPercent_M4);
	SerialPrint(nextionEnd);

	SerialPrint("pageMove.vMaxSpd_M4.val=");
	SerialPrint(configMove.maxSpd_M4);
	SerialPrint(nextionEnd);

	SerialPrint("pageMove.vAccel_M4.val=");
	SerialPrint(configMove.accel_M4);
	SerialPrint(nextionEnd);

	SerialPrint("pageMove.vDistance_M4_1.txt=");
	SerialWrite(0x22);
	SerialPrint(configMove.distance_MoveM4_1);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageMove.t_M4_1.txt=");
	SerialWrite(0x22);
	SerialPrint(configMove.distance_MoveM4_1);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageMove.vDistance_M4_2.txt=");
	SerialWrite(0x22);
	SerialPrint(configMove.distance_MoveM4_2);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageMove.t_M4_2.txt=");
	SerialWrite(0x22);
	SerialPrint(configMove.distance_MoveM4_2);
	SerialPrint(nextionQuoteEnd);
	switch (configMove.axisId)
	{
		case ID_MOVE_AXIS_Z1:
		case ID_MOVE_AXIS_Z2:
		{
			SerialPrint("pageMove.n53.val=");
			SerialPrint(configMove.maxSpd_Z);
			SerialPrint(nextionEnd);

			SerialPrint("pageMove.n54.val=");
			SerialPrint(configMove.accel_Z);
			SerialPrint(nextionEnd);

			SerialPrint("pageMove.h0.val=");
			SerialPrint(configMove.speedPercent_Z);
			SerialPrint(nextionEnd);
			break;
		}
		case ID_MOVE_AXIS_X1: // X Axis
		case ID_MOVE_AXIS_X2: // X Axis
		{
			SerialPrint("pageMove.n53.val=");
			//SerialWrite(0x22);
			SerialPrint(configMove.maxSpd_X);
			SerialPrint(nextionEnd);

			SerialPrint("pageMove.n54.val=");
			SerialPrint(configMove.accel_X);
			SerialPrint(nextionEnd);

			SerialPrint("pageMove.h0.val=");
			SerialPrint(configMove.speedPercent_X);
			SerialPrint(nextionEnd);
			break;
		}
		case ID_MOVE_AXIS_M3_1: // M3 Axis
		case ID_MOVE_AXIS_M3_2: // M3 Axis
		{
			SerialPrint("pageMove.n53.val=");
			SerialPrint(configMove.maxSpd_M3);
			SerialPrint(nextionEnd);

			SerialPrint("pageMove.n54.val=");
			SerialPrint(configMove.accel_M3);
			SerialPrint(nextionEnd);

			SerialPrint("pageMove.h0.val=");
			SerialPrint(configMove.speedPercent_M3);
			SerialPrint(nextionEnd);
			break;
		}
		case ID_MOVE_AXIS_M4_1: // M4 Axis
		case ID_MOVE_AXIS_M4_2: // M4 Axis
		{
			SerialPrint("pageMove.n53.val=");
			SerialPrint(configMove.maxSpd_M4);
			SerialPrint(nextionEnd);

			SerialPrint("pageMove.n54.val=");
			SerialPrint(configMove.accel_M4);
			SerialPrint(nextionEnd);

			SerialPrint("pageMove.h0.val=");
			SerialPrint(configMove.speedPercent_M4);
			SerialPrint(nextionEnd);

			break;
		}

	}

	switch (configMove.axisId)
	{
		case ID_MOVE_AXIS_Z1:
		{
			SerialPrint("pageMove.t7.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.distance_MoveZ1);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case ID_MOVE_AXIS_Z2:
		{
			SerialPrint("pageMove.t7.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.distance_MoveZ2);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case ID_MOVE_AXIS_X1: // X Axis
		{
			SerialPrint("pageMove.t7.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.distance_MoveX1);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case ID_MOVE_AXIS_X2: // X Axis
		{
			SerialPrint("pageMove.t7.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.distance_MoveX2);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case ID_MOVE_AXIS_M3_1: // M3 Axis
		{
			SerialPrint("pageMove.t7.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.distance_MoveM3_1);
			SerialPrint(nextionQuoteEnd);
			break;
		}
		case ID_MOVE_AXIS_M3_2: // M3 Axis
		{
			SerialPrint("pageMove.t7.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.distance_MoveM3_2);
			SerialPrint(nextionQuoteEnd);
			break;
		}

		case ID_MOVE_AXIS_M4_1: // M4 Axis
		{
			SerialPrint("pageMove.t7.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.distance_MoveM4_1);
			SerialPrint(nextionQuoteEnd);

			break;
		}

		case ID_MOVE_AXIS_M4_2: // M4 Axis
		{
			SerialPrint("pageMove.t7.txt=");
			SerialWrite(0x22);
			SerialPrint(configMove.distance_MoveM4_2);
			SerialPrint(nextionQuoteEnd);

			break;
		}

	}

	//SerialPrint("pageMove.bt8.val=");
	//SerialPrint(configMove.synchro_M3_Spindle);
	//SerialPrint(nextionEnd);

	//SerialPrint("pageSplash.vSynchro_Move.txt=");
	//SerialWrite(0x22);
	//SerialPrint(configMove.synchro_M3_Percentage);
	//SerialPrint(nextionQuoteEnd);

	//

	//	//xxxSerialPrint("pageSplash.vSynchroVal_M.val=");
	//SerialPrint(configMove.synchro_M3_Spindle);
	//SerialPrint(nextionEnd);

	//SerialPrint("pageEEPROM.t311.txt=");
	//SerialWrite(0x22);
	//SerialPrint(configSetup.motorCount);
	//SerialPrint(nextionQuoteEnd);

	//SerialPrint("pageEEPROM.t312.txt=");
	//SerialWrite(0x22);
	//SerialPrint(configSetup.vendorId);
	//SerialPrint(nextionQuoteEnd);


}

void LoadPage_Sync()
{

	Serial.println("Enter LoadPage_Sync");




	SerialPrint("pageSync.n51.val=");
	SerialPrint(configSync.maxSpd_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageSync.vMaxSpdSpindle.val=");
	SerialPrint(configSync.maxSpd_Spindle);
	SerialPrint(nextionEnd);


	SerialPrint("pageSync.n52.val=");
	SerialPrint(configSync.accel_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageSync.vAccelSpindle.val=");
	SerialPrint(configSync.accel_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageSync.vSpdP_Spindle.val=");
	SerialPrint(configSync.speedPercent_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageSync.h1.val=");
	SerialPrint(configSync.speedPercent_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageSync.vSpdT_Spindle.txt=");
	SerialWrite(0x22);
	SerialPrint(configSync.speedPercent_Spindle);
	SerialPrint(nextionQuoteEnd);

	Serial.print("speedPercent_Spindle: ");
	Serial.println(configSync.speedPercent_Spindle);

	SerialPrint("pageSync.vSpdP_Z.val=");
	SerialPrint(configSync.speedPercent_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageSync.vMaxSpd_Z.val=");
	SerialPrint(configSync.maxSpd_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageSync.vAccel_Z.val=");
	SerialPrint(configSync.accel_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageSync.vSpdP_X.val=");
	SerialPrint(configSync.speedPercent_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageSync.vMaxSpd_X.val=");
	SerialPrint(configSync.maxSpd_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageSync.vAccel_X.val=");
	SerialPrint(configSync.accel_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageSync.vSpdP_M3.val=");
	SerialPrint(configSync.speedPercent_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageSync.vMaxSpd_M3.val=");
	SerialPrint(configSync.maxSpd_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageSync.vAccel_M3.val=");
	SerialPrint(configSync.accel_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageSync.vSpdP_M4.val=");
	SerialPrint(configSync.speedPercent_M4);
	SerialPrint(nextionEnd);

	SerialPrint("pageSync.vMaxSpd_M4.val=");
	SerialPrint(configSync.maxSpd_M4);
	SerialPrint(nextionEnd);

	SerialPrint("pageSync.vAccel_M4.val=");
	SerialPrint(configSync.accel_M4);
	SerialPrint(nextionEnd);

	switch (configSync.axisId)
	{
		case ID_AXIS_Z:
		{
			SerialPrint("pageSync.n53.val=");
			SerialPrint(configSync.maxSpd_Z);
			SerialPrint(nextionEnd);

			SerialPrint("pageSync.n54.val=");
			SerialPrint(configSync.accel_Z);
			SerialPrint(nextionEnd);

			SerialPrint("pageSync.h0.val=");
			SerialPrint(configSync.speedPercent_Z);
			SerialPrint(nextionEnd);
			break;
		}
		case ID_AXIS_X: // X Axis
		{
			SerialPrint("pageSync.n53.val=");
			//SerialWrite(0x22);
			SerialPrint(configSync.maxSpd_X);
			SerialPrint(nextionEnd);

			SerialPrint("pageSync.n54.val=");
			SerialPrint(configSync.accel_X);
			SerialPrint(nextionEnd);

			SerialPrint("pageSync.h0.val=");
			SerialPrint(configSync.speedPercent_X);
			SerialPrint(nextionEnd);
			break;
		}
		case ID_AXIS_3: // M3 Axis
		{
			SerialPrint("pageSync.n53.val=");
			SerialPrint(configSync.maxSpd_M3);
			SerialPrint(nextionEnd);

			SerialPrint("pageSync.n54.val=");
			SerialPrint(configSync.accel_M3);
			SerialPrint(nextionEnd);

			SerialPrint("pageSync.h0.val=");
			SerialPrint(configSync.speedPercent_M3);
			SerialPrint(nextionEnd);
			break;
		}
		case ID_AXIS_4: // M4 Axis
		{
			SerialPrint("pageSync.n53.val=");
			SerialPrint(configSync.maxSpd_M4);
			SerialPrint(nextionEnd);

			SerialPrint("pageSync.n54.val=");
			SerialPrint(configSync.accel_M4);
			SerialPrint(nextionEnd);

			SerialPrint("pageSync.h0.val=");
			SerialPrint(configSync.speedPercent_M4);
			SerialPrint(nextionEnd);
			break;
		}
	}

	SerialPrint("pageSync.t13.txt=");
	SerialWrite(0x22);
	SerialPrint(configSync.revolutions_Spindle);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageSync.t14.txt=");
	SerialWrite(0x22);
	SerialPrint(configSync.distance);
	SerialPrint(nextionQuoteEnd);



	//SerialPrint("pageSync.bt8.val=");
	//SerialPrint(configSync.synchro_M3_Spindle);
	//SerialPrint(nextionEnd);

	//SerialPrint("pageSplash.vSynchro_Sync.txt=");
	//SerialWrite(0x22);
	//SerialPrint(configSync.synchro_M3_Percentage);
	//SerialPrint(nextionQuoteEnd);

	//

	//	//xxxSerialPrint("pageSplash.vSynchroVal_M.val=");
	//SerialPrint(configSync.synchro_M3_Spindle);
	//SerialPrint(nextionEnd);

	//SerialPrint("pageEEPROM.t311.txt=");
	//SerialWrite(0x22);
	//SerialPrint(configSetup.motorCount);
	//SerialPrint(nextionQuoteEnd);

	//SerialPrint("pageEEPROM.t312.txt=");
	//SerialWrite(0x22);
	//SerialPrint(configSetup.vendorId);
	//SerialPrint(nextionQuoteEnd);


}

void LoadPage_Rec()
{

	Serial.println("Enter LoadPage_Rec");

	SerialPrint("pageRec.n51.val=");
	SerialPrint(configRec.maxSpd_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageRec.vMaxSpdSpindle.val=");
	SerialPrint(configRec.maxSpd_Spindle);
	SerialPrint(nextionEnd);


	SerialPrint("pageRec.n52.val=");
	SerialPrint(configRec.accel_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageRec.vAccelSpindle.val=");
	SerialPrint(configRec.accel_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageRec.vSpdP_Spindle.val=");
	SerialPrint(configRec.speedPercent_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageRec.vSpdT_Spindle.txt=");
	SerialWrite(0x22);
	SerialPrint(configRec.speedPercent_Spindle);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageRec.n1.val=");
	SerialPrint(configRec.speedPercent_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageRec.h1.val=");
	SerialPrint(configRec.speedPercent_Spindle);
	SerialPrint(nextionEnd);

	Serial.print("speedPercent_Spindle: ");
	Serial.println(configRec.speedPercent_Spindle);

	SerialPrint("pageRec.vSpdP_Z.val=");
	SerialPrint(configRec.speedPercent_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageRec.vMaxSpd_Z.val=");
	SerialPrint(configRec.maxSpd_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageRec.vAccel_Z.val=");
	SerialPrint(configRec.accel_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageRec.vSpdP_X.val=");
	SerialPrint(configRec.speedPercent_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageRec.vMaxSpd_X.val=");
	SerialPrint(configRec.maxSpd_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageRec.vAccel_X.val=");
	SerialPrint(configRec.accel_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageRec.vSpdP_M3.val=");
	SerialPrint(configRec.speedPercent_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageRec.vMaxSpd_M3.val=");
	SerialPrint(configRec.maxSpd_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageRec.vAccel_M3.val=");
	SerialPrint(configRec.accel_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageRec.vSpdP_M4.val=");
	SerialPrint(configRec.speedPercent_M4);
	SerialPrint(nextionEnd);

	SerialPrint("pageRec.vMaxSpd_M4.val=");
	SerialPrint(configRec.maxSpd_M4);
	SerialPrint(nextionEnd);

	SerialPrint("pageRec.vAccel_M4.val=");
	SerialPrint(configRec.accel_M4);
	SerialPrint(nextionEnd);

	switch (configRec.axisId)
	{
		case ID_AXIS_Z:
		{
			SerialPrint("pageRec.n53.val=");
			SerialPrint(configRec.maxSpd_Z);
			SerialPrint(nextionEnd);

			SerialPrint("pageRec.n54.val=");
			SerialPrint(configRec.accel_Z);
			SerialPrint(nextionEnd);

			SerialPrint("pageRec.h0.val=");
			SerialPrint(configRec.speedPercent_Z);
			SerialPrint(nextionEnd);
			break;
		}
		case ID_AXIS_X: // X Axis
		{
			SerialPrint("pageRec.n53.val=");
			//SerialWrite(0x22);
			SerialPrint(configRec.maxSpd_X);
			SerialPrint(nextionEnd);

			SerialPrint("pageRec.n54.val=");
			SerialPrint(configRec.accel_X);
			SerialPrint(nextionEnd);

			SerialPrint("pageRec.h0.val=");
			SerialPrint(configRec.speedPercent_X);
			SerialPrint(nextionEnd);
			break;
		}
		case ID_AXIS_3: // M3 Axis
		{
			SerialPrint("pageRec.n53.val=");
			SerialPrint(configRec.maxSpd_M3);
			SerialPrint(nextionEnd);

			SerialPrint("pageRec.n54.val=");
			SerialPrint(configRec.accel_M3);
			SerialPrint(nextionEnd);

			SerialPrint("pageRec.h0.val=");
			SerialPrint(configRec.speedPercent_M3);
			SerialPrint(nextionEnd);
			break;
		}
		case ID_AXIS_4: // M4 Axis
		{
			SerialPrint("pageRec.n53.val=");
			SerialPrint(configRec.maxSpd_M4);
			SerialPrint(nextionEnd);

			SerialPrint("pageRec.n54.val=");
			SerialPrint(configRec.accel_M4);
			SerialPrint(nextionEnd);

			SerialPrint("pageRec.h0.val=");
			SerialPrint(configRec.speedPercent_M4);
			SerialPrint(nextionEnd);

			break;
		}

	}

	SerialPrint("pageRec.t21.txt=");
	SerialWrite(0x22);
	SerialPrint(configRec.grow_SpindleChange);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageRec.t15.txt=");
	SerialWrite(0x22);
	SerialPrint(configRec.grow_AxisChange);
	SerialPrint(nextionQuoteEnd);


	switch (configRec.radial_Axial)
	{
		case RADIAL: // Radial
		{
			SerialPrint("pageRec.t4.txt=");
			SerialWrite(0x22);
			SerialPrint(configRec.degrees_Radial_Spindle);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageRec.t5.txt=");
			SerialWrite(0x22);
			SerialPrint(configRec.amplitude_Radial_Axis);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageRec.n3.val=");
			SerialPrint(configRec.waves_Radial);
			SerialPrint(nextionEnd);


			break;

		}
		case AXIAL:
		{
			SerialPrint("pageRec.t4.txt=");
			SerialWrite(0x22);
			SerialPrint(configRec.amplitude_Axial_Spindle);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageRec.t5.txt=");
			SerialWrite(0x22);
			SerialPrint(configRec.distance_Axial_Axis);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageRec.n3.val=");
			SerialPrint(configRec.waves_Axial);
			SerialPrint(nextionEnd);



			break;
		}

	}

	SerialPrint("pageRec.vtAmplitudeRad.txt=");
	SerialWrite(0x22);
	SerialPrint(configRec.degrees_Radial_Spindle);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageRec.vtDistanceRad.txt=");
	SerialWrite(0x22);
	SerialPrint(configRec.amplitude_Radial_Axis);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageRec.vWavesRad.val=");
	SerialPrint(configRec.waves_Radial);
	SerialPrint(nextionEnd);



	SerialPrint("pageRec.vtAmplitudeAxi.txt=");
	SerialWrite(0x22);
	SerialPrint(configRec.amplitude_Axial_Spindle);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageRec.vtDistanceAxi.txt=");
	SerialWrite(0x22);
	SerialPrint(configRec.distance_Axial_Axis);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageRec.vWavesAxial.val=");
	SerialPrint(configRec.waves_Axial);
	SerialPrint(nextionEnd);


	//SerialPrint("pageRec.bt8.val=");
	//SerialPrint(configRec.synchro_M3_Spindle);
	//SerialPrint(nextionEnd);

	//SerialPrint("pageSplash.vSynchro_Rec.txt=");
	//SerialWrite(0x22);
	//SerialPrint(configRec.synchro_M3_Percentage);
	//SerialPrint(nextionQuoteEnd);

	//

	//	//xxxSerialPrint("pageSplash.vSynchroVal_M.val=");
	//SerialPrint(configRec.synchro_M3_Spindle);
	//SerialPrint(nextionEnd);

	//SerialPrint("pageEEPROM.t311.txt=");
	//SerialWrite(0x22);
	//SerialPrint(configSetup.motorCount);
	//SerialPrint(nextionQuoteEnd);

	//SerialPrint("pageEEPROM.t312.txt=");
	//SerialWrite(0x22);
	//SerialPrint(configSetup.vendorId);
	//SerialPrint(nextionQuoteEnd);


}

void LoadPage_Grk()
{
	Serial.println("Enter LoadPage_Grk");


	SerialPrint("pageGrk.n1.val=");
	SerialPrint(configGreekKey.patternCount_Grk);
	SerialPrint(nextionEnd);

	SerialPrint("pageGrk.n0.val=");
	SerialPrint(configGreekKey.countPatternPer360_Grk);
	SerialPrint(nextionEnd);

	SerialPrint("pageGrk.t7.txt=");
	SerialWrite(0x22);
	SerialPrint(configGreekKey.segmentLength_Grk);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageGrk.n51.val=");
	SerialPrint(configGreekKey.maxSpd_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageGrk.vMaxSpdSpindle.val=");
	SerialPrint(configGreekKey.maxSpd_Spindle);
	SerialPrint(nextionEnd);


	SerialPrint("pageGrk.n52.val=");
	SerialPrint(configGreekKey.accel_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageGrk.vAccelSpindle.val=");
	SerialPrint(configGreekKey.accel_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageGrk.vSpdP_Spindle.val=");
	SerialPrint(configGreekKey.speedPercent_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageGrk.vSpdT_Spindle.txt=");
	SerialWrite(0x22);
	SerialPrint(configGreekKey.speedPercent_Spindle);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageGrk.h1.val=");
	SerialPrint(configGreekKey.speedPercent_Spindle);
	SerialPrint(nextionEnd);

	Serial.print("speedPercent_Spindle: ");
	Serial.println(configGreekKey.speedPercent_Spindle);

	SerialPrint("pageGrk.vSpdP_Z.val=");
	SerialPrint(configGreekKey.speedPercent_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageGrk.vMaxSpd_Z.val=");
	SerialPrint(configGreekKey.maxSpd_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageGrk.vAccel_Z.val=");
	SerialPrint(configGreekKey.accel_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageGrk.vSpdP_X.val=");
	SerialPrint(configGreekKey.speedPercent_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageGrk.vMaxSpd_X.val=");
	SerialPrint(configGreekKey.maxSpd_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageGrk.vAccel_X.val=");
	SerialPrint(configGreekKey.accel_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageGrk.vSpdP_M3.val=");
	SerialPrint(configGreekKey.speedPercent_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageGrk.vMaxSpd_M3.val=");
	SerialPrint(configGreekKey.maxSpd_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageGrk.vAccel_M3.val=");
	SerialPrint(configGreekKey.accel_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageGrk.vSpdP_M4.val=");
	SerialPrint(configGreekKey.speedPercent_M4);
	SerialPrint(nextionEnd);

	SerialPrint("pageGrk.vMaxSpd_M4.val=");
	SerialPrint(configGreekKey.maxSpd_M4);
	SerialPrint(nextionEnd);

	SerialPrint("pageGrk.vAccel_M4.val=");
	SerialPrint(configGreekKey.accel_M4);
	SerialPrint(nextionEnd);



	switch (configGreekKey.axisId)
	{
		case ID_AXIS_Z:
		{
			SerialPrint("pageGrk.n53.val=");
			SerialPrint(configGreekKey.maxSpd_Z);
			SerialPrint(nextionEnd);

			SerialPrint("pageGrk.n54.val=");
			SerialPrint(configGreekKey.accel_Z);
			SerialPrint(nextionEnd);

			SerialPrint("pageGrk.h0.val=");
			SerialPrint(configGreekKey.speedPercent_Z);
			SerialPrint(nextionEnd);
			break;
		}
		case ID_AXIS_X: // X Axis
		{
			SerialPrint("pageGrk.n53.val=");
			//SerialWrite(0x22);
			SerialPrint(configGreekKey.maxSpd_X);
			SerialPrint(nextionEnd);

			SerialPrint("pageGrk.n54.val=");
			SerialPrint(configGreekKey.accel_X);
			SerialPrint(nextionEnd);

			SerialPrint("pageGrk.h0.val=");
			SerialPrint(configGreekKey.speedPercent_X);
			SerialPrint(nextionEnd);
			break;
		}
		case ID_AXIS_3: // M3 Axis
		{
			SerialPrint("pageGrk.n53.val=");
			SerialPrint(configGreekKey.maxSpd_M3);
			SerialPrint(nextionEnd);

			SerialPrint("pageGrk.n54.val=");
			SerialPrint(configGreekKey.accel_M3);
			SerialPrint(nextionEnd);

			SerialPrint("pageGrk.h0.val=");
			SerialPrint(configGreekKey.speedPercent_M3);
			SerialPrint(nextionEnd);
			break;
		}
		case ID_AXIS_4: // M4 Axis
		{
			SerialPrint("pageGrk.n53.val=");
			SerialPrint(configGreekKey.maxSpd_M4);
			SerialPrint(nextionEnd);

			SerialPrint("pageGrk.n54.val=");
			SerialPrint(configGreekKey.accel_M4);
			SerialPrint(nextionEnd);

			SerialPrint("pageGrk.h0.val=");
			SerialPrint(configGreekKey.speedPercent_M4);
			SerialPrint(nextionEnd);

			break;
		}

	}

	SerialPrint("pageGrk.vaRadial_Axial.val=");
	SerialPrint(configGreekKey.radialOrAxial_Pattern);
	SerialPrint(nextionEnd);

	switch (configGreekKey.radialOrAxial_Pattern)
	{
		case RADIAL:
		{

			SerialPrint("pageGrk.r10.val=1");
			SerialPrint(nextionEnd);

			SerialPrint("pageGrk.r11.val=0");
			SerialPrint(nextionEnd);
			break;
		}
		case AXIAL:
		{
			SerialPrint("pageGrk.r10.val=0");
			SerialPrint(nextionEnd);

			SerialPrint("pageGrk.r11.val=1");
			SerialPrint(nextionEnd);
			break;
		}
	}

	SerialPrint("pageGrk.r4a.val=0");
	SerialPrint(nextionEnd);

	SerialPrint("pageGrk.r4b.val=0");
	SerialPrint(nextionEnd);

	SerialPrint("pageGrk.r2a.val=0");
	SerialPrint(nextionEnd);

	SerialPrint("pageGrk.r2b.val=0");
	SerialPrint(nextionEnd);

	SerialPrint("pageGrk.r3a.val=0");
	SerialPrint(nextionEnd);

	SerialPrint("pageGrk.r3b.val=0");
	SerialPrint(nextionEnd);
	MilliDelay(50);
	//vaPatternId
	Serial.print("LoadPage-vaPatternId: ");
	Serial.println(configGreekKey.patternId);

	//SerialPrint("pageGrk.n3.val=");
	//SerialPrint(configGreekKey.patternId);
	//SerialPrint(nextionEnd);
	SerialPrint("pageGrk.vaPatternId.val=");
	SerialPrint(configGreekKey.patternId);
	SerialPrint(nextionEnd);
	switch (configGreekKey.patternId)
	{
		case 2:
		{
			SerialPrint("pageGrk.r4a.val=1");
			SerialPrint(nextionEnd);
			break;
		}
		case 3:
		{
			SerialPrint("pageGrk.r4b.val=1");
			SerialPrint(nextionEnd);
			break;
		}
		case 4:
		{
			SerialPrint("pageGrk.r2a.val=1");
			SerialPrint(nextionEnd);
			break;
		}
		case 5:
		{
			SerialPrint("pageGrk.r2b.val=1");
			SerialPrint(nextionEnd);
			break;
		}
		case 6:
		{
			SerialPrint("pageGrk.r3a.val=1");
			SerialPrint(nextionEnd);
			break;
		}
		case 7:
		{
			SerialPrint("pageGrk.r3b.val=1");
			SerialPrint(nextionEnd);
			break;
		}
	}
}

void LoadPage_Program()
{
	Serial.println("Enter LoadPage_Program");



	SerialPrint("pageProgram.n1.val=");
	SerialPrint(configGreekKey.patternCount_Program);
	SerialPrint(nextionEnd);

	SerialPrint("pageProgram.t4.txt=");
	SerialWrite(0x22);
	SerialPrint(configGreekKey.countPatternPer360_Program);
	SerialPrint(nextionQuoteEnd);
	

	SerialPrint("pageProgram.t7.txt=");
	SerialWrite(0x22);
	SerialPrint(configGreekKey.segmentLength_Program);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageProgram.n51.val=");
	SerialPrint(configGreekKey.maxSpd_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageProgram.vMaxSpdSpindle.val=");
	SerialPrint(configGreekKey.maxSpd_Spindle);
	SerialPrint(nextionEnd);


	SerialPrint("pageProgram.n52.val=");
	SerialPrint(configGreekKey.accel_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageProgram.vAccelSpindle.val=");
	SerialPrint(configGreekKey.accel_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageProgram.vSpdP_Spindle.val=");
	SerialPrint(configGreekKey.speedPercent_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageProgram.vSpdT_Spindle.txt=");
	SerialWrite(0x22);
	SerialPrint(configGreekKey.speedPercent_Spindle);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageProgram.h1.val=");
	SerialPrint(configGreekKey.speedPercent_Spindle);
	SerialPrint(nextionEnd);

	Serial.print("speedPercent_Spindle: ");
	Serial.println(configGreekKey.speedPercent_Spindle);

	SerialPrint("pageProgram.vSpdP_Z.val=");
	SerialPrint(configGreekKey.speedPercent_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageProgram.vMaxSpd_Z.val=");
	SerialPrint(configGreekKey.maxSpd_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageProgram.vAccel_Z.val=");
	SerialPrint(configGreekKey.accel_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageProgram.vSpdP_X.val=");
	SerialPrint(configGreekKey.speedPercent_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageProgram.vMaxSpd_X.val=");
	SerialPrint(configGreekKey.maxSpd_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageProgram.vAccel_X.val=");
	SerialPrint(configGreekKey.accel_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageProgram.vSpdP_M3.val=");
	SerialPrint(configGreekKey.speedPercent_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageProgram.vMaxSpd_M3.val=");
	SerialPrint(configGreekKey.maxSpd_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageProgram.vAccel_M3.val=");
	SerialPrint(configGreekKey.accel_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageProgram.vSpdP_M4.val=");
	SerialPrint(configGreekKey.speedPercent_M4);
	SerialPrint(nextionEnd);

	SerialPrint("pageProgram.vMaxSpd_M4.val=");
	SerialPrint(configGreekKey.maxSpd_M4);
	SerialPrint(nextionEnd);

	SerialPrint("pageProgram.vAccel_M4.val=");
	SerialPrint(configGreekKey.accel_M4);
	SerialPrint(nextionEnd);


	switch (configGreekKey.axisId)
	{
		case ID_AXIS_Z:
		{
			SerialPrint("pageProgram.n53.val=");
			SerialPrint(configGreekKey.maxSpd_Z);
			SerialPrint(nextionEnd);

			SerialPrint("pageProgram.n54.val=");
			SerialPrint(configGreekKey.accel_Z);
			SerialPrint(nextionEnd);

			SerialPrint("pageProgram.h0.val=");
			SerialPrint(configGreekKey.speedPercent_Z);
			SerialPrint(nextionEnd);
			break;
		}
		case ID_AXIS_X: // X Axis
		{
			SerialPrint("pageProgram.n53.val=");
			//SerialWrite(0x22);
			SerialPrint(configGreekKey.maxSpd_X);
			SerialPrint(nextionEnd);

			SerialPrint("pageProgram.n54.val=");
			SerialPrint(configGreekKey.accel_X);
			SerialPrint(nextionEnd);

			SerialPrint("pageProgram.h0.val=");
			SerialPrint(configGreekKey.speedPercent_X);
			SerialPrint(nextionEnd);
			break;
		}
		case ID_AXIS_3: // M3 Axis
		{
			SerialPrint("pageProgram.n53.val=");
			SerialPrint(configGreekKey.maxSpd_M3);
			SerialPrint(nextionEnd);

			SerialPrint("pageProgram.n54.val=");
			SerialPrint(configGreekKey.accel_M3);
			SerialPrint(nextionEnd);

			SerialPrint("pageProgram.h0.val=");
			SerialPrint(configGreekKey.speedPercent_M3);
			SerialPrint(nextionEnd);
			break;
		}
		case ID_AXIS_4: // M4 Axis
		{
			SerialPrint("pageProgram.n53.val=");
			SerialPrint(configGreekKey.maxSpd_M4);
			SerialPrint(nextionEnd);

			SerialPrint("pageProgram.n54.val=");
			SerialPrint(configGreekKey.accel_M4);
			SerialPrint(nextionEnd);

			SerialPrint("pageProgram.h0.val=");
			SerialPrint(configGreekKey.speedPercent_M4);
			SerialPrint(nextionEnd);

			break;
		}

	}

	SerialPrint("pageProgram.vaRadial_Axial.val=");
	SerialPrint(configGreekKey.radialOrAxial_File);
	SerialPrint(nextionEnd);

	switch (configGreekKey.radialOrAxial_File)
	{
		case RADIAL:
		{
			SerialPrint("pageProgram.r8.val=1");
			SerialPrint(nextionEnd);

			SerialPrint("pageProgram.r9.val=0");
			SerialPrint(nextionEnd);
			break;
		}
		case AXIAL:
		{
			SerialPrint("pageProgram.r8.val=0");
			SerialPrint(nextionEnd);

			SerialPrint("pageProgram.r9.val=1");
			SerialPrint(nextionEnd);
			break;
		}
	}

}

void LoadPage_Rose()
{

	Serial.println("Enter LoadPage_Rose");




	SerialPrint("pageRose.n51.val=");
	SerialPrint(configRose.maxSpd_Spindle);
	SerialPrint(nextionEnd);

	//SerialPrint("pageRose.vMaxSpdSpindle.val=");
	//SerialPrint(configRose.maxSpd_Spindle);
	//SerialPrint(nextionEnd);


	SerialPrint("pageRose.n52.val=");
	SerialPrint(configRose.accel_Spindle);
	SerialPrint(nextionEnd);

	//SerialPrint("pageRose.vAccelSpindle.val=");
	//SerialPrint(configRose.accel_Spindle);
	//SerialPrint(nextionEnd);

	SerialPrint("pageRose.vSpdP_Spindle.val=");
	SerialPrint(configRose.speedPercent_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageRose.h1.val=");
	SerialPrint(configRose.speedPercent_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageRose.vSpdT_Spindle.txt=");
	SerialWrite(0x22);
	SerialPrint(configRose.speedPercent_Spindle);
	SerialPrint(nextionQuoteEnd);

	Serial.print("speedPercent_Spindle: ");
	Serial.println(configRose.speedPercent_Spindle);

	SerialPrint("pageRose.vSpdP_Z.val=");
	SerialPrint(configRose.speedPercent_Z);
	SerialPrint(nextionEnd);


	SerialPrint("pageRose.vMaxSpd_Z.val=");
	SerialPrint(configRose.maxSpd_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageRose.vAccel_Z.val=");
	SerialPrint(configRose.accel_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageRose.vSpdP_X.val=");
	SerialPrint(configRose.speedPercent_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageRose.vMaxSpd_X.val=");
	SerialPrint(configRose.maxSpd_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageRose.vAccel_X.val=");
	SerialPrint(configRose.accel_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageRose.vSpdP_M3.val=");
	SerialPrint(configRose.speedPercent_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageRose.vMaxSpd_M3.val=");
	SerialPrint(configRose.maxSpd_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageRose.vAccel_M3.val=");
	SerialPrint(configRose.accel_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageRose.vSpdP_M4.val=");
	SerialPrint(configRose.speedPercent_M4);
	SerialPrint(nextionEnd);

	SerialPrint("pageRose.vMaxSpd_M4.val=");
	SerialPrint(configRose.maxSpd_M4);
	SerialPrint(nextionEnd);

	SerialPrint("pageRose.vAccel_M4.val=");
	SerialPrint(configRose.accel_M4);
	SerialPrint(nextionEnd);

	switch (configRose.axisId)
	{
	case ID_AXIS_Z:
	{
		SerialPrint("pageRose.n53.val=");
		SerialPrint(configRose.maxSpd_Z);
		SerialPrint(nextionEnd);

		SerialPrint("pageRose.n54.val=");
		SerialPrint(configRose.accel_Z);
		SerialPrint(nextionEnd);

		SerialPrint("pageRose.h0.val=");
		SerialPrint(configRose.speedPercent_Z);
		SerialPrint(nextionEnd);

		SerialPrint("pageRose.t6.txt=");
		SerialWrite(0x22);
		SerialPrint(configRose.amplitude_Radial_Z);
		SerialPrint(nextionQuoteEnd);
		break;
	}
	case ID_AXIS_X: // X Axis
	{
		SerialPrint("pageRose.n53.val=");
		//SerialWrite(0x22);
		SerialPrint(configRose.maxSpd_X);
		SerialPrint(nextionEnd);

		SerialPrint("pageRose.n54.val=");
		SerialPrint(configRose.accel_X);
		SerialPrint(nextionEnd);

		SerialPrint("pageRose.h0.val=");
		SerialPrint(configRose.speedPercent_X);
		SerialPrint(nextionEnd);

		SerialPrint("pageRose.t6.txt=");
		SerialWrite(0x22);
		SerialPrint(configRose.amplitude_Radial_X);
		SerialPrint(nextionQuoteEnd);
		break;
	}
	case ID_AXIS_3: // M3 Axis
	{
		SerialPrint("pageRose.n53.val=");
		SerialPrint(configRose.maxSpd_M3);
		SerialPrint(nextionEnd);

		SerialPrint("pageRose.n54.val=");
		SerialPrint(configRose.accel_M3);
		SerialPrint(nextionEnd);

		SerialPrint("pageRose.h0.val=");
		SerialPrint(configRose.speedPercent_M3);
		SerialPrint(nextionEnd);

		SerialPrint("pageRose.t6.txt=");
		SerialWrite(0x22);
		SerialPrint(configRose.amplitude_Radial_M3);
		SerialPrint(nextionQuoteEnd);
		break;
	}
	case ID_AXIS_4: // M4 Axis
	{
		SerialPrint("pageRose.n53.val=");
		SerialPrint(configRose.maxSpd_M4);
		SerialPrint(nextionEnd);

		SerialPrint("pageRose.n54.val=");
		SerialPrint(configRose.accel_M4);
		SerialPrint(nextionEnd);

		SerialPrint("pageRose.h0.val=");
		SerialPrint(configRose.speedPercent_M4);
		SerialPrint(nextionEnd);

		SerialPrint("pageRose.t6.txt=");
		SerialWrite(0x22);
		SerialPrint(configRose.amplitude_Radial_M4);
		SerialPrint(nextionQuoteEnd);
		break;
	}
	}

	SerialPrint("pageRose.t100.txt=");
	SerialWrite(0x22);
	SerialPrint(configRose.n);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageRose.t101.txt=");
	SerialWrite(0x22);
	SerialPrint(configRose.d);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageRose.t19.txt=");
	SerialWrite(0x22);
	SerialPrint(configRose.spindleRevolutions);
	SerialPrint(nextionQuoteEnd);

	// Only Radial is implemented
	SerialPrint("pageRose.vaRadial_Axial.val=0");
	SerialPrint(nextionEnd);

	SerialPrint("pageRose.t301.txt=");
	SerialWrite(0x22);
	SerialPrint(configRose.amplitude_Radial_Z);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageRose.t302.txt=");
	SerialWrite(0x22);
	SerialPrint(configRose.amplitude_Radial_X);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageRose.t303.txt=");
	SerialWrite(0x22);
	SerialPrint(configRose.amplitude_Radial_M3);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageRose.t304.txt=");
	SerialWrite(0x22);
	SerialPrint(configRose.amplitude_Radial_M4);
	SerialPrint(nextionQuoteEnd);

	//SerialPrint("pageRose.bt8.val=");
	//SerialPrint(configRose.synchro_M3_Spindle);
	//SerialPrint(nextionEnd);

	//SerialPrint("pageSplash.vRosehro_Rose.txt=");
	//SerialWrite(0x22);
	//SerialPrint(configRose.synchro_M3_Percentage);
	//SerialPrint(nextionQuoteEnd);

	//

	//	//xxxSerialPrint("pageSplash.vRosehroVal_M.val=");
	//SerialPrint(configRose.synchro_M3_Spindle);
	//SerialPrint(nextionEnd);

	//SerialPrint("pageEEPROM.t311.txt=");
	//SerialWrite(0x22);
	//SerialPrint(configSetup.motorCount);
	//SerialPrint(nextionQuoteEnd);

	//SerialPrint("pageEEPROM.t312.txt=");
	//SerialWrite(0x22);
	//SerialPrint(configSetup.vendorId);
	//SerialPrint(nextionQuoteEnd);


}

void LoadPage_Multi()
{
	Serial.println("Enter LoadPage_Multi");

	SerialPrint("pageMulti.vMultiAxesChk.val=");
	SerialPrint(configMulti.multiAxesIDs);
	SerialPrint(nextionEnd);

	// Spindle
	SerialPrint("pageMulti.n51.val=");
	SerialPrint(configMulti.maxSpd_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageMulti.n52.val=");
	SerialPrint(configMulti.accel_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageMulti.vSpdP_Spindle.val=");
	SerialPrint(configMulti.speedPercent_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageMulti.vSpdT_Spindle.txt=");
	SerialWrite(0x22);
	SerialPrint(configMulti.speedPercent_Spindle);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageMulti.h1.val=");
	SerialPrint(configMulti.speedPercent_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageMulti.c0.val=");
	SerialPrint(configMulti.checked_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageMulti.bt1100.txt=");
	SerialWrite(0x22);
	SerialPrint(configMulti.targetDistance_Spindle);
	SerialPrint(nextionQuoteEnd);

	if (configMulti.direction_Spindle != 1)
	{

		SerialPrint("pageMulti.bt1200.txt=");
		SerialWrite(0x22);
		SerialPrint("Q");

		SerialPrint("pageMulti.bt1200.val=1");
		SerialPrint(nextionEnd);

	}
	else
	{
		SerialPrint("pageMulti.bt1200.txt=");
		SerialWrite(0x22);
		SerialPrint("P");

		SerialPrint("pageMulti.bt1200.val=0");
		SerialPrint(nextionEnd);
	}

	Serial.print("speedPercent_Spindle: ");
	Serial.println(configMulti.speedPercent_Spindle);

	// Z
	SerialPrint("pageMulti.vSpdP_Z.val=");
	SerialPrint(configMulti.speedPercent_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageMulti.n1.val=");
	SerialPrint(configMulti.speedPercent_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageMulti.n1401.val=");
	SerialPrint(configMulti.maxSpd_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageMulti.vAccel_Z.val=");
	SerialPrint(configMulti.accel_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageMulti.c1.val=");
	SerialPrint(configMulti.checked_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageMulti.bt1101.txt=");
	SerialWrite(0x22);
	SerialPrint(configMulti.targetDistance_Z);
	SerialPrint(nextionQuoteEnd);

	if (configMulti.direction_Z != 1)
	{
		// Left arrow		
		SerialPrint("pageMulti.bt1201.txt=");
		SerialWrite(0x22);
		SerialPrint("Z");
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageMulti.bt1201.val=1");
		SerialPrint(nextionEnd);
	}
	else
	{
		// Right arrow 
		SerialPrint("pageMulti.bt1201.txt=");
		SerialWrite(0x22);
		SerialPrint("[");
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageMulti.bt1201.val=0");
		SerialPrint(nextionEnd);
	}

	// X
	SerialPrint("pageMulti.vSpdP_X.val=");
	SerialPrint(configMulti.speedPercent_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageMulti.n2.val=");
	SerialPrint(configMulti.speedPercent_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageMulti.n1402.val=");
	SerialPrint(configMulti.maxSpd_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageMulti.vAccel_X.val=");
	SerialPrint(configMulti.accel_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageMulti.c2.val=");
	SerialPrint(configMulti.checked_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageMulti.bt1102.txt=");
	SerialWrite(0x22);
	SerialPrint(configMulti.targetDistance_X);
	SerialPrint(nextionQuoteEnd);



	if (configMulti.direction_X != 1)
	{
		// Left arrow
		SerialPrint("pageMulti.bt1202.txt=");
		SerialWrite(0x22);
		SerialPrint("Z");
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageMulti.bt1202.val=1");
		SerialPrint(nextionEnd);
	}
	else
	{

		// Right arrow
		SerialPrint("pageMulti.bt1202.txt=");
		SerialWrite(0x22);
		SerialPrint("[");
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageMulti.bt1202.val=0");
		SerialPrint(nextionEnd);
	}

	// M3
	SerialPrint("pageMulti.vSpdP_M3.val=");
	SerialPrint(configMulti.speedPercent_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageMulti.n3.val=");
	SerialPrint(configMulti.speedPercent_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageMulti.n1403.val=");
	SerialPrint(configMulti.maxSpd_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageMulti.vAccel_M3.val=");
	SerialPrint(configMulti.accel_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageMulti.c3.val=");
	SerialPrint(configMulti.checked_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageMulti.bt1103.txt=");
	SerialWrite(0x22);
	SerialPrint(configMulti.targetDistance_M3);
	SerialPrint(nextionQuoteEnd);



	if(configSetup.radialOrLinear_Axis_M3== 0) // Radial
	{ 
		if (configMulti.direction_M3 != 1) // CCW
		{
			// CCW
			SerialPrint("pageMulti.bt1203.txt=");
			SerialWrite(0x22);
			SerialPrint("Q");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageMulti.bt1203.val=1");
			SerialPrint(nextionEnd);
		}
		else
		{
			// CW
			SerialPrint("pageMulti.bt1203.txt=");
			SerialWrite(0x22);
			SerialPrint("P");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageMulti.bt1203.val=0");
			SerialPrint(nextionEnd);
		}
	}
	else //Linear
	{
		if (configMulti.direction_M3 != 1) // 
		{

			// Left arrow
			SerialPrint("pageMulti.bt1203.txt=");
			SerialWrite(0x22);
			SerialPrint("Z");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageMulti.bt1203.val=1");
			SerialPrint(nextionEnd);
		}
		else
		{
			// Right arrow
			SerialPrint("pageMulti.bt1203.txt=");
			SerialWrite(0x22);
			SerialPrint("[");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageMulti.bt1203.val=0");
			SerialPrint(nextionEnd);
		}
	}


	//SerialPrint("pageMulti.vaDir_M3.val=");
	//SerialPrint(configMulti.direction_M3);
	//SerialPrint(nextionEnd);

	// M4
	SerialPrint("pageMulti.vSpdP_M4.val=");
	SerialPrint(configMulti.speedPercent_M4);
	SerialPrint(nextionEnd);

	SerialPrint("pageMulti.n4.val=");
	SerialPrint(configMulti.speedPercent_M4);
	SerialPrint(nextionEnd);

	SerialPrint("pageMulti.n1404.val=");
	SerialPrint(configMulti.maxSpd_M4);
	SerialPrint(nextionEnd);

	SerialPrint("pageMulti.vAccel_M4.val=");
	SerialPrint(configMulti.accel_M4);
	SerialPrint(nextionEnd);

	SerialPrint("pageMulti.c4.val=");
	SerialPrint(configMulti.checked_M4);
	SerialPrint(nextionEnd); 

	SerialPrint("pageMulti.bt1104.txt=");
	SerialWrite(0x22);
	SerialPrint(configMulti.targetDistance_M4);
	SerialPrint(nextionQuoteEnd);

	Serial.print("configMulti.direction_Spindle: ");
	Serial.println(configMulti.direction_Spindle);
	Serial.print("configMulti.direction_Z: ");
	Serial.println(configMulti.direction_Z);
	Serial.print("configMulti.direction_X: ");
	Serial.println(configMulti.direction_X);
	Serial.print("configMulti.direction_M3: ");
	Serial.println(configMulti.direction_M3);
	Serial.print("configMulti.direction_M4: ");
	Serial.println(configMulti.direction_M4);

	if (configSetup.radialOrLinear_Axis_M4 == 0) // Radial
	{
		if (configMulti.direction_M4 != 1) // CCW
		{
			// CCW
			SerialPrint("pageMulti.bt1204.txt=");
			SerialWrite(0x22);
			SerialPrint("Q");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageMulti.bt1204.val=1");
			SerialPrint(nextionEnd);
		}
		else
		{
			// CW
			SerialPrint("pageMulti.bt1204.txt=");
			SerialWrite(0x22);
			SerialPrint("P");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageMulti.bt1204.val=0");
			SerialPrint(nextionEnd);
		}
	}
	else //Linear
	{
		if (configMulti.direction_M4 != 1) // 
		{

			// Left arrow
			SerialPrint("pageMulti.bt1204.txt=");
			SerialWrite(0x22);
			SerialPrint("Z");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageMulti.bt1204.val=1");
			SerialPrint(nextionEnd);
		}
		else
		{
			// Right arrow
			SerialPrint("pageMulti.bt1204.txt=");
			SerialWrite(0x22);

			SerialPrint("[");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageMulti.bt1204.val=0");
			SerialPrint(nextionEnd);
		}
	}

	SerialPrint("pageMulti.n5.val=");
	SerialPrint(configMulti.direction_Z);
	SerialPrint(nextionEnd);
	SerialPrint("pageMulti.n6.val=");
	SerialPrint(configMulti.direction_X);
	SerialPrint(nextionEnd);
	SerialPrint("pageMulti.n7.val=");
	SerialPrint(configMulti.direction_M3);
	SerialPrint(nextionEnd);
	SerialPrint("pageMulti.n8.val=");
	SerialPrint(configMulti.direction_M4);
	SerialPrint(nextionEnd);
	SerialPrint("pageMulti.n9.val=");
	SerialPrint(configMulti.direction_Spindle);
	SerialPrint(nextionEnd);

	//SerialPrint("pageMulti.vaDir_M4.val=");
	//SerialPrint(configMulti.direction_M4);
	//SerialPrint(nextionEnd);

	switch (configMulti.axisId)
	{
		case ID_AXIS_Z:
		{
			SerialPrint("pageMulti.n53.val=");
			SerialPrint(configMulti.maxSpd_Z);
			SerialPrint(nextionEnd);

			SerialPrint("pageMulti.n54.val=");
			SerialPrint(configMulti.accel_Z);
			SerialPrint(nextionEnd);

			SerialPrint("pageMulti.h0.val=");
			SerialPrint(configMulti.speedPercent_Z);
			SerialPrint(nextionEnd);
			break;
		}
		case ID_AXIS_X: // X Axis
		{
			SerialPrint("pageMulti.n53.val=");
			SerialPrint(configMulti.maxSpd_X);
			SerialPrint(nextionEnd);

			SerialPrint("pageMulti.n54.val=");
			SerialPrint(configMulti.accel_X);
			SerialPrint(nextionEnd);

			SerialPrint("pageMulti.h0.val=");
			SerialPrint(configMulti.speedPercent_X);
			SerialPrint(nextionEnd);
			break;
		}
		case ID_AXIS_3: // M3 Axis
		{
			SerialPrint("pageMulti.n53.val=");
			SerialPrint(configMulti.maxSpd_M3);
			SerialPrint(nextionEnd);

			SerialPrint("pageMulti.n54.val=");
			SerialPrint(configMulti.accel_M3);
			SerialPrint(nextionEnd);

			SerialPrint("pageMulti.h0.val=");
			SerialPrint(configMulti.speedPercent_M3);
			SerialPrint(nextionEnd);
			break;
		}
		case ID_AXIS_4: // M4 Axis
		{
			SerialPrint("pageMulti.n53.val=");
			SerialPrint(configMulti.maxSpd_M4);
			SerialPrint(nextionEnd);

			SerialPrint("pageMulti.n54.val=");
			SerialPrint(configMulti.accel_M4);
			SerialPrint(nextionEnd);

			SerialPrint("pageMulti.h0.val=");
			SerialPrint(configMulti.speedPercent_M4);
			SerialPrint(nextionEnd);

			break;
		}
	}
}

// Setup pages

void LoadSetupPage()
{
	int setupPageCallerId = GetSerialInteger();
	Serial.print("++++++++++++setupPageCallerId: ");
	Serial.println(setupPageCallerId);
	switch (setupPageCallerId)
	{
		case PAGE_SPINDLE:
		{
			Serial.print("LoadPage_Spindle............");
			LoadPage_Spindle();
			break;
		}
		case PAGE_Z:
		{
			Serial.print("LoadPage_Z............");
			LoadPage_Z();
			break;
		}
		case PAGE_X:
		{
			Serial.print("LoadPage_X............");
			LoadPage_X();
			break;
		}
		case PAGE_ALTX:
		{
			Serial.print("LoadPage_AltX............");
			LoadPage_AltX();
			break;
		}
		case PAGE_M3:
		{
			Serial.print("LoadPage_M3............");
			LoadPage_M3();
			break;
		}
		case PAGE_M4:
		{
			Serial.print("LoadPage_M4............");
			LoadPage_M4();
			break;
		}
		case PAGE_LIMITS:
		{
			Serial.print("LoadPage_Limits............");
			LoadPage_Limits();
			break;
		}
	
		case PAGE_RETURNS:
		{
			Serial.print("LoadPage_Returns............");
			LoadPage_Returns();
			break;
		}
		case PAGE_MORE:
		{
			Serial.print("LoadPage_More............");
			LoadPage_More();
			break;
		}
	}
}
void LoadPage_Z()
{
	Serial.println("Enter LoadPage_Z");
	const char* pageZ_t46 = "pageZ.t46.txt=";
	const char* pageZ_t47 = "pageZ.t47.txt=";
	const char* pageZ_t48 = "pageZ.t48.txt=";
	const char* pageZ_t49 = "pageZ.t49.txt=";
	const char* lowChar = "Low";
	const char* highChar = "High";

	configPageSetup eePromPageSetup;
	EEPROM.get(eePromAddress_Setup, eePromPageSetup);

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
	SerialPrint(eePromPageSetup.distancePerRev_AxisZ, 6);
	SerialPrint(nextionQuoteEnd);

	Serial.print("configSetup.distancePerRev_AxisZ:  ");
	Serial.println(configSetup.distancePerRev_AxisZ, 8);
	Serial.print("eePromPageSetup.distancePerRev_AxisZ:  ");
	Serial.println(eePromPageSetup.distancePerRev_AxisZ, 8);

	SerialPrint(pageZ_t49);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.polarity_Axis_Z ? lowChar : highChar);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageZ.n0.val=");
	SerialPrint(configSetup.microsteps_Axis_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageZ.t9.txt=");
	SerialWrite(0x22);
	SerialPrint(configSetup.steps360_Axis_Z);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageZ.t10.txt=");
	SerialWrite(0x22);
	SerialPrint(configSetup.distancePerRev_AxisZ);
	SerialPrint(nextionQuoteEnd);

}

void LoadPage_X()
{
	Serial.println("Enter LoadPage_X");
	const char* pageX_t60 = "pageX.t60.txt=";
	const char* pageX_t61 = "pageX.t61.txt=";
	const char* pageX_t62 = "pageX.t62.txt=";
	const char* pageX_t63 = "pageX.t63.txt=";
	//const char* pageX_c2_0 = "pageX.c2.val=0";
	//const char* pageX_c2_1 = "pageX.c2.val=1";
	//const char* pageX_c3_0 = "pageX.c3.val=0";
	//const char* pageX_c3_1 = "pageX.c3.val=1";
	const char* lowChar = "Low";
	const char* highChar = "High";

	configPageSetup eePromPageSetup;
	EEPROM.get(eePromAddress_Setup, eePromPageSetup);

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

	SerialPrint("pageX.n0.val=");
	SerialPrint(configSetup.microsteps_Axis_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageX.t9.txt=");
	SerialWrite(0x22);
	SerialPrint(configSetup.steps360_Axis_X);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageX.t10.txt=");
	SerialWrite(0x22);
	SerialPrint(configSetup.distancePerRev_AxisX);
	SerialPrint(nextionQuoteEnd);

}

void LoadPage_AltX()
{
	Serial.println("Enter LoadPage_AltX");
	const char* pageAlternateX_c2_0 = "pageAlternateX.c2.val=0";
	const char* pageAlternateX_c2_1 = "pageAlternateX.c2.val=1";
	const char* pageAlternateX_c3_0 = "pageAlternateX.c3.val=0";
	const char* pageAlternateX_c3_1 = "pageAlternateX.c3.val=1";
	const char* pageX_c2_0 = "pageX.c2.val=0";
	const char* pageX_c2_1 = "pageX.c2.val=1";
	const char* pageX_c3_0 = "pageX.c3.val=0";
	const char* pageX_c3_1 = "pageX.c3.val=1";

	const char* pageAlternateX_t60 = "pageAlternateX.t60.txt=";
	const char* pageAlternateX_t61 = "pageAlternateX.t61.txt=";
	const char* pageAlternateX_t62 = "pageAlternateX.t62.txt=";
	const char* pageAlternateX_t63 = "pageAlternateX.t63.txt=";
	const char* lowChar = "Low";
	const char* highChar = "High";

	configPageSetup eePromPageSetup;
	EEPROM.get(eePromAddress_Setup, eePromPageSetup);

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

	SerialPrint("pageAlternateX.n0.val=");
	SerialPrint(configSetup.microsteps_Axis_XAlt);
	SerialPrint(nextionEnd);

	SerialPrint("pageAlternateX.t9.txt=");
	SerialWrite(0x22);
	SerialPrint(configSetup.steps360_Axis_XAlt);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageAlternateX.t10.txt=");
	SerialWrite(0x22);
	SerialPrint(configSetup.distancePerRev_AxisXAlt);
	SerialPrint(nextionQuoteEnd);

}

void LoadPage_M3()
{
	Serial.println("Enter LoadPage_M3");
	const char* pageMotor_3_t56 = "pageMotor_3.t56.txt=";
	const char* pageMotor_3_t57 = "pageMotor_3.t57.txt=";
	const char* pageMotor_3_t58 = "pageMotor_3.t58.txt=";
	const char* pageMotor_3_t59 = "pageMotor_3.t59.txt=";
	const char* pageMotor_3_t64 = "pageMotor_3.t64.txt=";
	// Radial
	const char* pageMotor_3_c0_0 = "pageMotor_3.c0.val=0";
	const char* pageMotor_3_c0_1 = "pageMotor_3.c0.val=1";
	// Linear
	const char* pageMotor_3_c1_0 = "pageMotor_3.c1.val=0";
	const char* pageMotor_3_c1_1 = "pageMotor_3.c1.val=1";
	const char* lowChar = "Low";
	const char* highChar = "High";

	configPageSetup eePromPageSetup;
	EEPROM.get(eePromAddress_Setup, eePromPageSetup);

	SerialPrint(pageMotor_3_t56);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.microsteps_Axis_M3);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageMotor_3_t57);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.steps360_Axis_M3);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageMotor_3_t58);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.gearRatio_AxisM3, 4);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageMotor_3_t59);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.polarity_Axis_M3 ? lowChar : highChar);
	SerialPrint(nextionQuoteEnd);

	// True=Linear, False = Radial
	SerialPrint("pageMotor_3.vRadialLinear.val=");
	SerialPrint(eePromPageSetup.radialOrLinear_Axis_M3);
	SerialPrint(nextionEnd);

	if (eePromPageSetup.radialOrLinear_Axis_M3)
	{
		SerialPrint(pageMotor_3_c0_0);
		SerialPrint(nextionEnd);
		SerialPrint(pageMotor_3_c1_1);
		SerialPrint(nextionEnd);

		SerialPrint("pageMotor_3.r2.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageMotor_3.r3.val=1");
		SerialPrint(nextionEnd);
	}
	else
	{
		SerialPrint(pageMotor_3_c0_1);
		SerialPrint(nextionEnd);
		SerialPrint(pageMotor_3_c1_0);
		SerialPrint(nextionEnd);

		SerialPrint("pageMotor_3.r2.val=1");
		SerialPrint(nextionEnd);
		SerialPrint("pageMotor_3.r3.val=0");
		SerialPrint(nextionEnd);
	}



	SerialPrint(pageMotor_3_t64);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.distancePerRev_AxisM3, 4);
	SerialPrint(nextionQuoteEnd);


	SerialPrint("pageMotor_3.n0.val=");
	SerialPrint(configSetup.microsteps_Axis_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageMotor_3.t9.txt=");
	SerialWrite(0x22);
	SerialPrint(configSetup.steps360_Axis_M3);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageMotor_3.t10.txt=");
	SerialWrite(0x22);
	SerialPrint(configSetup.distancePerRev_AxisM3);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageMotor_3.t15.txt=");
	SerialWrite(0x22);
	SerialPrint(configSetup.gearRatio_AxisM3);
	SerialPrint(nextionQuoteEnd);


	//	SerialPrint("pageMotor_3.vHand_M3.val=");
	//SerialPrint(configSetup.);
	//SerialPrint(nextionEnd);


}

void LoadPage_M4()
{
	Serial.println("Enter LoadPage_M4");
	const char* pageMotor_4_t56 = "pageMotor_4.t56.txt=";
	const char* pageMotor_4_t57 = "pageMotor_4.t57.txt=";
	const char* pageMotor_4_t58 = "pageMotor_4.t58.txt=";
	const char* pageMotor_4_t59 = "pageMotor_4.t59.txt=";
	const char* pageMotor_4_t64 = "pageMotor_4.t64.txt=";
	// Radial
	const char* pageMotor_4_c0_0 = "pageMotor_4.c0.val=0";
	const char* pageMotor_4_c0_1 = "pageMotor_4.c0.val=1";
	// Linear
	const char* pageMotor_4_c1_0 = "pageMotor_4.c1.val=0";
	const char* pageMotor_4_c1_1 = "pageMotor_4.c1.val=1";
	const char* lowChar = "Low";
	const char* highChar = "High";

	configPageSetup eePromPageSetup;
	EEPROM.get(eePromAddress_Setup, eePromPageSetup);

	SerialPrint(pageMotor_4_t56);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.microsteps_Axis_M4);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageMotor_4_t57);
	SerialWrite(0x22);
	SerialPrint(configSetup.steps360_Axis_M4);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageMotor_4_t58);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.gearRatio_AxisM4, 5);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageMotor_4_t59);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.polarity_Axis_M4 ? lowChar : highChar);
	SerialPrint(nextionQuoteEnd);

	// True=Linear, False = Radial
	SerialPrint("pageMotor_4.vRadialLinear.val=");
	SerialPrint(eePromPageSetup.radialOrLinear_Axis_M4);
	SerialPrint(nextionEnd);

	if (eePromPageSetup.radialOrLinear_Axis_M4)
	{
		SerialPrint(pageMotor_4_c0_0);
		SerialPrint(nextionEnd);
		SerialPrint(pageMotor_4_c1_1);
		SerialPrint(nextionEnd);

		SerialPrint("pageMotor_4.r2.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageMotor_4.r3.val=1");
		SerialPrint(nextionEnd);
	}
	else
	{
		SerialPrint(pageMotor_4_c0_1);
		SerialPrint(nextionEnd);
		SerialPrint(pageMotor_4_c1_0);
		SerialPrint(nextionEnd);

		SerialPrint("pageMotor_4.r2.val=1");
		SerialPrint(nextionEnd);
		SerialPrint("pageMotor_4.r3.val=0");
		SerialPrint(nextionEnd);
	}

	SerialPrint(pageMotor_4_t64);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.distancePerRev_AxisM4, 4);
	SerialPrint(nextionQuoteEnd);



	// Update Nextion
	SerialPrint("pageSetup.bt1.bco=23964");
	SerialPrint(nextionEnd);
	SerialPrint("pageSetup.bt1.val=0");
	SerialPrint(nextionEnd);

	SerialPrint("pageMotor_4.n0.val=");
	SerialPrint(configSetup.microsteps_Axis_M4);
	SerialPrint(nextionEnd);

	SerialPrint("pageMotor_4.t9.txt=");
	SerialWrite(0x22);
	SerialPrint(configSetup.steps360_Axis_M4);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageMotor_4.t10.txt=");
	SerialWrite(0x22);
	SerialPrint(configSetup.distancePerRev_AxisM4);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageMotor_4.t15.txt=");
	SerialWrite(0x22);
	SerialPrint(configSetup.gearRatio_AxisM4);
	SerialPrint(nextionQuoteEnd);

}

void LoadPage_Spindle()
{
	Serial.println("Enter LoadPage_Spindle");
	const char* pageSpindle_t42 = "pageSpindle.t42.txt=";
	const char* pageSpindle_t43 = "pageSpindle.t43.txt=";
	const char* pageSpindle_t44 = "pageSpindle.t44.txt=";
	const char* pageSpindle_t45 = "pageSpindle.t45.txt=";
	const char* lowChar = "Low";
	const char* highChar = "High";

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
	SerialPrint(eePromPageSetup.gearRatio_Spindle, 4);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageSpindle_t45);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.polarity_Spindle ? lowChar : highChar);
	SerialPrint(nextionQuoteEnd);

	

	SerialPrint("pageSpindle.n0.val=");
	SerialPrint(configSetup.microsteps_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageSpindle.t9.txt=");
	SerialWrite(0x22);
	SerialPrint(configSetup.steps360_Spindle);
	SerialPrint(nextionQuoteEnd);

	SerialPrint("pageSpindle.t15.txt=");
	SerialWrite(0x22);
	SerialPrint(configSetup.gearRatio_Spindle);
	SerialPrint(nextionQuoteEnd);

}

void LoadPage_Limits()
{
	Serial.println("Enter LoadPage_Limits");

	//const char* lowChar = "Low";
	//const char* highChar = "High";

	configPageSetup eePromPageSetup;
	EEPROM.get(eePromAddress_Setup, eePromPageSetup);

	TestEEPROM_Limits();

	SerialPrint("pageLimits.n0.val=");
	SerialPrint(configSetup.limit_Min_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageLimits.n1.val=");
	SerialPrint(configSetup.limit_Max_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageLimits.n2.val=");
	SerialPrint(configSetup.limit_Min_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageLimits.n3.val=");
	SerialPrint(configSetup.limit_Max_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageLimits.n4.val=");
	SerialPrint(configSetup.limit_Min_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageLimits.n5.val=");
	SerialPrint(configSetup.limit_Max_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageLimits.n7.val=");
	SerialPrint(configSetup.limit_Min_M4);
	SerialPrint(nextionEnd);

	SerialPrint("pageLimits.n8.val=");
	SerialPrint(configSetup.limit_Max_M4);
	SerialPrint(nextionEnd);

	SerialPrint("pageLimits.n6.val=");
	SerialPrint(configSetup.eStop);
	SerialPrint(nextionEnd);

	SerialPrint("pageLimits.c0.val=");
	SerialPrint(configSetup.limit_StopSpindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageLimits.vaNCNO.val=");
	SerialPrint(configSetup.limit_NCorNO);
	SerialPrint(nextionEnd);

	if (configSetup.limit_NCorNO == 1)
	{ 
		SerialPrint("pageLimits.r0.val=0");
		SerialPrint(nextionEnd);

		SerialPrint("pageLimits.r1.val=1");
		SerialPrint(nextionEnd);
	}
	else
	{
		SerialPrint("pageLimits.r0.val=1");
		SerialPrint(nextionEnd);

		SerialPrint("pageLimits.r1.val=0");
		SerialPrint(nextionEnd);
	}
}

void LoadPage_Returns()
{
	Serial.println("Enter LoadPage_Returns");

	//const char* lowChar = "Low";
	//const char* highChar = "High";

	configPageSetup eePromPageSetup;
	EEPROM.get(eePromAddress_Setup, eePromPageSetup);

	SerialPrint("pageReturns.n0.val=");
	SerialPrint(configSetup.maxSpd_Return_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageReturns.n1.val=");
	SerialPrint(configSetup.accel_Return_Spindle);
	SerialPrint(nextionEnd);

	SerialPrint("pageReturns.n2.val=");
	SerialPrint(configSetup.maxSpd_Return_Axis_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageReturns.n3.val=");
	SerialPrint(configSetup.accel_Return_Axis_Z);
	SerialPrint(nextionEnd);

	SerialPrint("pageReturns.n6.val=");
	SerialPrint(configSetup.maxSpd_Return_Axis_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageReturns.n7.val=");
	SerialPrint(configSetup.accel_Return_Axis_X);
	SerialPrint(nextionEnd);

	SerialPrint("pageReturns.n4.val=");
	SerialPrint(configSetup.maxSpd_Return_Axis_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageReturns.n5.val=");
	SerialPrint(configSetup.accel_Return_Axis_M3);
	SerialPrint(nextionEnd);

	SerialPrint("pageReturns.n8.val=");
	SerialPrint(configSetup.maxSpd_Return_Axis_M4);
	SerialPrint(nextionEnd);

	SerialPrint("pageReturns.n9.val=");
	SerialPrint(configSetup.accel_Return_Axis_M4);
	SerialPrint(nextionEnd);
}

void LoadPage_More()
{
	Serial.println("Enter LoadPage_More");
	const char* pageMore_c1_0 = "pageMore.c1.val=0";
	const char* pageMore_c1_1 = "pageMore.c1.val=1";
	const char* pageMore_t8 = "pageMore.t8.txt=";

	configPageSetup eePromPageSetup;
	EEPROM.get(eePromAddress_Setup, eePromPageSetup);

	// pageMore
	SerialPrint(pageMore_t8);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.auxillaryPin);
	SerialPrint(nextionQuoteEnd);

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

	SerialPrint("pageSplash.vaKeepEnabled.val=");
	SerialPrint(eePromPageSetup.keepSteppersEnabled);
	SerialPrint(nextionEnd);

	SerialPrint("pageSplash.vaAxisEnabledZ.val=");
	SerialPrint(eePromPageSetup.keepSteppersEnabled);
	SerialPrint(nextionEnd);

	SerialPrint("pageSplash.vaAxisEnabledX.val=");
	SerialPrint(eePromPageSetup.keepSteppersEnabled);
	SerialPrint(nextionEnd);

	SerialPrint("pageSplash.vAxisEnabledM3.val=");
	SerialPrint(eePromPageSetup.keepSteppersEnabled);
	SerialPrint(nextionEnd);

	SerialPrint("pageSplash.vAxisEnabledM4.val=");
	SerialPrint(eePromPageSetup.keepSteppersEnabled);
	SerialPrint(nextionEnd);


	SerialPrint("pageMore.n0.val=");
	SerialPrint(configSetup.auxillaryPin);
	SerialPrint(nextionEnd);


	
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

	// Verify ini file exists.
	//iniFile = ini_FileName;
	IniFile ini(fileNameIni);

	if (!ini.open())
	{
#ifdef DEBUG
		Serial.println("No Card.................");
#endif // Debug
		// Update Nextion
		SerialPrint("vis t1,1");
		SerialPrint(nextionEnd);
		SerialPrint("bt0.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("va0.val=0");
		SerialPrint(nextionEnd);
		return;
	}
	else
	{
#ifdef DEBUG

		Serial.println("Ini file found......");
		Serial.print("ini_FileName: ");
		Serial.println(fileNameIni);
#endif // Debug
		ini.close();
	}

	LoadSettings_PagePreferences();
	LoadSettings_PageReturns();
	LoadSettings_PageLimits();
	LoadSettings_PageMain();
	LoadSettings_PageOne();
	LoadSettings_PageMulti();
	LoadSettings_PageIndex();
	LoadSettings_PageMove();
	//xLoadSettings_Shared();
	LoadSettings_PageSync();
	LoadSettings_PageRec();
	LoadSettings_PageGrkAndProgram();
	LoadSettings_PageRose();

	// Update Nextion
	SerialPrint("va0.val=0");
	SerialPrint(nextionEnd);
	SerialPrint("bt0.val=0");
	SerialPrint(nextionEnd);
	SerialPrint("vis g0,1");
	SerialPrint(nextionEnd);
}


void WriteSettingsToSD()
{
	SD.remove("Settings.ini");
	fileSD = SD.open("Settings.ini", FILE_WRITE);
	fileSD.println("; Rose Engine Ini file");
	fileSD.println("; V4.1.0");


	WriteSettings_Preferences();
	WriteSettings_Limits();
	WriteSettings_Returns();
	WriteSettings_Main();
	WriteSettings_One();
	WriteSettings_Multi();
	WriteSettings_Index();
	WriteSettings_Move();
	WriteSettings_Sync();
	WriteSettings_Recip();
	WriteSettings_GreekKey();
	WriteSettings_Program();
	WriteSettings_Rose();

	fileSD.println(";End ini");
	fileSD.close();

	SerialPrint("bt1.val=0");
	SerialPrint(nextionEnd);
}


void WriteSettings_Preferences()
{
	char currentValue[60];

	// Preferences
	fileSD.println("[Preferences]");
	Serial.println("[Preferences]");
	fileSD.println(";");
	Serial.println("[;]");
	fileSD.println("; More");
	Serial.println("; More");
	fileSD.print("VendorId=");
	Serial.print("VendorId=");
	sprintf(currentValue, "%lu", configSetup.vendorId);
	fileSD.println(currentValue);
	Serial.println(currentValue);
	fileSD.println("; Auxillary_Pin must be greater than 24.");
	fileSD.print("Auxillary_Pin=");//47");
	sprintf(currentValue, "%lu", configSetup.auxillaryPin);
	fileSD.println(currentValue);
	fileSD.println("; 1: Keep enabled, 0 : Disable");
	fileSD.print("KeepSteppersEnabled=");//0");
	sprintf(currentValue, "%lu", configSetup.keepSteppersEnabled);
	fileSD.println(currentValue);
	fileSD.println("; Spindle");
	fileSD.print("Microsteps_Spindle=");//32");
	sprintf(currentValue, "%lu", configSetup.microsteps_Spindle);
	fileSD.println(currentValue);
	fileSD.print("StepsPer360_Spindle=");//200");
	sprintf(currentValue, "%lu", configSetup.steps360_Spindle);
	fileSD.println(currentValue);
	fileSD.print("GearRatio_Spindle=");//9");
	Serial.print("GearRatio_Spindle=");//9");
	sprintf(currentValue, "%.5g", configSetup.gearRatio_Spindle);
	fileSD.println(currentValue);
	Serial.println(currentValue);
	fileSD.println("; Polarity: Low=1, High=0");
	fileSD.print("Polarity_Spindle=");//1");
	sprintf(currentValue, "%d", configSetup.polarity_Spindle);
	fileSD.println(currentValue);
	fileSD.println("; Z Axis");
	fileSD.print("Microsteps_Z=");//32");
	sprintf(currentValue, "%lu", configSetup.microsteps_Axis_Z);
	fileSD.println(currentValue);
	fileSD.print("StepsPer360_Z=");//200");
	sprintf(currentValue, "%lu", configSetup.steps360_Axis_Z);
	fileSD.println(currentValue);
	fileSD.print("DistancePer360_Z=");//0.187");
	sprintf(currentValue, "%.5g", configSetup.distancePerRev_AxisZ);
	fileSD.println(currentValue);
	fileSD.print("Polarity_Z=");//1");
	sprintf(currentValue, "%d", configSetup.polarity_Axis_Z);
	fileSD.println(currentValue);
	fileSD.println("; Hand: 0=Left, 1=Right");
	fileSD.print("Leadscrew_Hand_Z=");//0");
	sprintf(currentValue, "%d", configSetup.leadscrewHand_Z);
	fileSD.println(currentValue);
	fileSD.println("; X Axis");
	fileSD.print("XorAltXSelected=");//0");
	sprintf(currentValue, "%lu", configSetup.xAltX);
	fileSD.println(currentValue);
	fileSD.print("Microsteps_X=");//32");
	sprintf(currentValue, "%lu", configSetup.microsteps_Axis_X);
	fileSD.println(currentValue);
	fileSD.print("StepsPer360_X=");//200");
	sprintf(currentValue, "%lu", configSetup.steps360_Axis_X);
	fileSD.println(currentValue);
	fileSD.print("DistancePer360_X=");//0.187");
	sprintf(currentValue, "%.5g", configSetup.distancePerRev_AxisX);
	fileSD.println(currentValue);
	fileSD.print("Polarity_X=");//1
	sprintf(currentValue, "%d", configSetup.polarity_Axis_X);
	fileSD.println(currentValue);
	fileSD.print("Leadscrew_Hand_X=");//0
	sprintf(currentValue, "%d", configSetup.leadscrewHand_X);
	fileSD.println(currentValue);
	fileSD.println("; Alternate X Axis");//
	fileSD.print("Microsteps_AltX=");//32
	sprintf(currentValue, "%lu", configSetup.microsteps_Axis_XAlt);
	fileSD.println(currentValue);
	fileSD.print("StepsPer360_AltX=");//200
	sprintf(currentValue, "%lu", configSetup.steps360_Axis_XAlt);
	fileSD.println(currentValue);
	fileSD.print("DistancePer360_AltX=");//0.283
	sprintf(currentValue, "%.5g", configSetup.distancePerRev_AxisXAlt);
	fileSD.println(currentValue);
	fileSD.print("Polarity_AltX=");//1
	sprintf(currentValue, "%d", configSetup.polarity_Axis_XAlt);
	fileSD.println(currentValue);


	fileSD.print("Leadscrew_Hand_AltX=");//1
	sprintf(currentValue, "%d", configSetup.leadscrewHand_AltX);
	fileSD.println(currentValue);

	fileSD.println("; M3 Axis");//
	fileSD.print("Microsteps_M3=");//32
	sprintf(currentValue, "%lu", configSetup.microsteps_Axis_M3);
	fileSD.println(currentValue);
	fileSD.print("StepsPer360_M3=");//200
	sprintf(currentValue, "%lu", configSetup.steps360_Axis_M3);
	fileSD.println(currentValue);
	fileSD.print("GearRatio_M3=");//30
	sprintf(currentValue, "%.5g", configSetup.gearRatio_AxisM3);
	fileSD.println(currentValue);
	fileSD.print("Polarity_M3=");//1
	sprintf(currentValue, "%d", configSetup.polarity_Axis_M3);
	fileSD.println(currentValue);
	fileSD.print("DistancePer360_M3=");//1.02354
	sprintf(currentValue, "%.5g", configSetup.distancePerRev_AxisM3);
	fileSD.println(currentValue);
	fileSD.print("RadialOrLineal_M3=");//0
	sprintf(currentValue, "%lu", configSetup.radialOrLinear_Axis_M3);
	fileSD.println(currentValue);
	fileSD.print("M3Radius=");//5.124
	sprintf(currentValue, "%.5g", configSetup.radiusM3);
	fileSD.println(currentValue);

	fileSD.print("Leadscrew_Hand_M3=");//1
	Serial.print("Leadscrew_Hand_M3=");
	sprintf(currentValue, "%lu", configSetup.leadscrewHand_M3);

	fileSD.println(currentValue);
	Serial.println(configSetup.leadscrewHand_M3);
	Serial.println(currentValue);

	fileSD.println("; M4 Axis");//
	fileSD.print("Microsteps_M4=");//32
	sprintf(currentValue, "%lu", configSetup.microsteps_Axis_M4);
	fileSD.println(currentValue);
	fileSD.print("StepsPer360_M4=");//200
	sprintf(currentValue, "%lu", configSetup.steps360_Axis_M4);
	fileSD.println(currentValue);
	fileSD.print("GearRatio_M4=");//30
	sprintf(currentValue, "%.5g", configSetup.gearRatio_AxisM4);
	fileSD.println(currentValue);
	fileSD.print("Polarity_M4=");//1
	sprintf(currentValue, "%d", configSetup.polarity_Axis_M4);
	fileSD.println(currentValue);
	fileSD.print("DistancePer360_M4=");//0.0254
	sprintf(currentValue, "%.5g", configSetup.distancePerRev_AxisM4);
	fileSD.println(currentValue);
	fileSD.print("RadialOrLineal_M4=");//1
	sprintf(currentValue, "%lu", configSetup.radialOrLinear_Axis_M4);
	fileSD.println(currentValue);
	fileSD.print("M4Radius=");//11.258
	sprintf(currentValue, "%.5g", configSetup.radiusM4);
	fileSD.println(currentValue);

	fileSD.print("Leadscrew_Hand_M4=");//1
	sprintf(currentValue, "%lu", configSetup.leadscrewHand_M4);
	fileSD.println(currentValue);
	Serial.print("Leadscrew_Hand_M4=");//1
	Serial.println(configSetup.leadscrewHand_M4);
	Serial.println(currentValue);

	fileSD.println(";");
}

void WriteSettings_Limits()
{
	char currentValue[60];

	fileSD.println("[Limits]");
	fileSD.println(";");


	fileSD.println("; Limit Switchesand Stop pins must be greater than 24.");
	fileSD.print("Min_Z="); // 24
	sprintf(currentValue, "%lu", configSetup.limit_Min_Z);
	fileSD.println(currentValue);
	fileSD.print("Max_Z="); // 26
	sprintf(currentValue, "%lu", configSetup.limit_Max_Z);
	fileSD.println(currentValue);
	fileSD.print("Min_X="); // 24
	sprintf(currentValue, "%lu", configSetup.limit_Min_X);
	fileSD.println(currentValue);
	fileSD.print("Max_X="); // 26
	sprintf(currentValue, "%lu", configSetup.limit_Max_X);
	fileSD.println(currentValue);
	fileSD.print("Min_M3="); // 24
	sprintf(currentValue, "%lu", configSetup.limit_Min_M3);
	fileSD.println(currentValue);
	fileSD.print("Max_M3="); // 26
	sprintf(currentValue, "%lu", configSetup.limit_Max_M3);
	fileSD.println(currentValue);
	fileSD.print("Min_M4="); // 24
	sprintf(currentValue, "%lu", configSetup.limit_Min_M4);
	fileSD.println(currentValue);
	fileSD.print("Max_M4="); //26
	sprintf(currentValue, "%lu", configSetup.limit_Max_M4);
	fileSD.println(currentValue);
	fileSD.println("; Stop spindle when limit hit = 1. Leave spindle running = 0");
	fileSD.print("StopSpindle="); //  1
	sprintf(currentValue, "%lu", configSetup.limit_StopSpindle);
	fileSD.println(currentValue);
	fileSD.print("eStop="); //28
	sprintf(currentValue, "%lu", configSetup.eStop);
	fileSD.println(currentValue);
	fileSD.println("; Normally Closed = 0, Normally Open = 1");
	fileSD.print("NC-NO="); //1
	sprintf(currentValue, "%lu", configSetup.limit_NCorNO);
	fileSD.println(currentValue);

	fileSD.println(";");


}

void WriteSettings_Returns()
{
	char currentValue[60];

	fileSD.println("[Returns]");
	fileSD.println(";");

	fileSD.println("; All speeds and accellerations are in steps per second");
	fileSD.print("MaxSpeed_Spindle="); //30000
	sprintf(currentValue, "%lu", configSetup.maxSpd_Return_Spindle);
	fileSD.println(currentValue);
	fileSD.print("Accel_Spindle="); //15000
	sprintf(currentValue, "%lu", configSetup.accel_Return_Spindle);
	fileSD.println(currentValue);
	fileSD.print("MaxSpeed_Axis_Z="); //30000
	sprintf(currentValue, "%lu", configSetup.maxSpd_Return_Axis_Z);
	fileSD.println(currentValue);
	fileSD.print("Accel_Axis_Z="); //15000
	sprintf(currentValue, "%lu", configSetup.accel_Return_Axis_Z);
	fileSD.println(currentValue);
	fileSD.print("MaxSpeed_Axis_X="); //30000
	sprintf(currentValue, "%lu", configSetup.maxSpd_Return_Axis_X);
	fileSD.println(currentValue);
	fileSD.print("Accel_Axis_X="); //15000
	sprintf(currentValue, "%lu", configSetup.accel_Return_Axis_X);
	fileSD.println(currentValue);
	fileSD.print("MaxSpeed_Axis_M3="); //30000
	sprintf(currentValue, "%lu", configSetup.maxSpd_Return_Axis_M3);
	fileSD.println(currentValue);
	fileSD.print("Accel_Axis_M3="); //15000
	sprintf(currentValue, "%lu", configSetup.accel_Return_Axis_M3);
	fileSD.println(currentValue);
	fileSD.print("MaxSpeed_Axis_M4="); //30000
	sprintf(currentValue, "%lu", configSetup.maxSpd_Return_Axis_M4);
	fileSD.println(currentValue);
	fileSD.print("Accel_Axis_M4="); //15000
	sprintf(currentValue, "%lu", configSetup.accel_Return_Axis_M4);
	fileSD.println(currentValue);

	fileSD.println(";");
}

void WriteSettings_Main()
{
	char currentValue[60];

	fileSD.println("[Main]");
	fileSD.println(";");
	fileSD.println("; Settings shared between Main, Sync, Recip, Rose, Greek Key, and Program pages");
	fileSD.println("; Synchronize M3and Spindle");
	fileSD.print("Synchro_M3_and_Spindle=");
	sprintf(currentValue, "%lu", configMain.synchro_M3_Spindle);
	fileSD.println(currentValue);
	fileSD.print("Synchro_Percentage=");
	sprintf(currentValue, "%.5g", configMain.synchro_M3_Percentage);
	fileSD.println(currentValue);
	fileSD.println("; Axis Ids : Spindle = 0, Z = 1, X = 2, M3 = 3, M4 = 4");
	fileSD.print("AxisId=");
	sprintf(currentValue, "%ld", configMain.axisId);
	fileSD.println(currentValue);
	fileSD.println("; Spindle");
	fileSD.print("MaxSpeed_Spindle=");
	sprintf(currentValue, "%ld", configMain.maxSpd_Spindle);
	fileSD.println(currentValue);
	fileSD.print("Accel_Spindle=");
	sprintf(currentValue, "%ld", configMain.accel_Spindle);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_Spindle=");
	sprintf(currentValue, "%ld", configMain.speedPercent_Spindle);
	fileSD.println(currentValue);
	fileSD.println("; Z Axis");
	fileSD.print("MaxSpeed_Z=");
	sprintf(currentValue, "%ld", configMain.maxSpd_Z);
	fileSD.println(currentValue);
	fileSD.print("Accel_Z=");
	sprintf(currentValue, "%ld", configMain.accel_Z);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_Z=");
	sprintf(currentValue, "%ld", configMain.speedPercent_Z);
	fileSD.println(currentValue);
	fileSD.println("; X Axis");
	fileSD.print("MaxSpeed_X=");
	sprintf(currentValue, "%ld", configMain.maxSpd_X);
	fileSD.println(currentValue);
	fileSD.print("Accel_X=");
	sprintf(currentValue, "%ld", configMain.accel_X);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_X=");
	sprintf(currentValue, "%ld", configMain.speedPercent_X);
	fileSD.println(currentValue);
	fileSD.println("; M3 Axis");
	fileSD.print("MaxSpeed_M3=");
	sprintf(currentValue, "%ld", configMain.maxSpd_M3);
	fileSD.println(currentValue);
	fileSD.print("Accel_M3=");
	sprintf(currentValue, "%ld", configMain.accel_M3);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_M3=");
	sprintf(currentValue, "%ld", configMain.speedPercent_M3);
	fileSD.println(currentValue);
	fileSD.println("; M4 Axis");
	fileSD.print("MaxSpeed_M4=");
	sprintf(currentValue, "%ld", configMain.maxSpd_M4);
	fileSD.println(currentValue);
	fileSD.print("Accel_M4=");
	sprintf(currentValue, "%ld", configMain.accel_M4);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_M4=");
	sprintf(currentValue, "%ld", configMain.speedPercent_M4);
	fileSD.println(currentValue);
	fileSD.println(";");

}

void WriteSettings_One()
{
	char currentValue[60];

	fileSD.println("[One]");
	fileSD.println(";");

	fileSD.print("AxisId="); //1
	sprintf(currentValue, "%ld", configOne.axisId);
	fileSD.println(currentValue);
	fileSD.println("; Spindle");
	fileSD.print("MaxSpeed_Spindle="); //25060
	sprintf(currentValue, "%lu", configOne.maxSpd_Spindle);
	fileSD.println(currentValue);
	fileSD.print("Accel_Spindle="); //20061
	sprintf(currentValue, "%ld", configOne.accel_Spindle);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_Spindle="); //61
	sprintf(currentValue, "%ld", configOne.speedPercent_Spindle);
	fileSD.println(currentValue);
	fileSD.println("; Z Axis");
	fileSD.print("MaxSpeed_Z="); //5062
	sprintf(currentValue, "%lu", configOne.maxSpd_Z);
	fileSD.println(currentValue);
	fileSD.print("Accel_Z="); //5063
	sprintf(currentValue, "%ld", configOne.accel_Z);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_Z="); //64
	sprintf(currentValue, "%ld", configOne.speedPercent_Z);
	fileSD.println(currentValue);
	fileSD.println("; X Axis");
	fileSD.print("MaxSpeed_X="); //5065
	sprintf(currentValue, "%lu", configOne.maxSpd_X);
	fileSD.println(currentValue);
	fileSD.print("Accel_X="); //5066
	sprintf(currentValue, "%ld", configOne.accel_X);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_X="); //67
	sprintf(currentValue, "%ld", configOne.speedPercent_X);
	fileSD.println(currentValue);
	fileSD.println("; M3 Axis");
	fileSD.print("MaxSpeed_M3="); //5068
	sprintf(currentValue, "%lu", configOne.maxSpd_M3);
	fileSD.println(currentValue);
	fileSD.print("Accel_M3="); //5069
	sprintf(currentValue, "%ld", configOne.accel_M3);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_M3="); //70
	sprintf(currentValue, "%ld", configOne.speedPercent_M3);
	fileSD.println(currentValue);
	fileSD.println("; M4 Axis");
	fileSD.print("MaxSpeed_M4="); //5068
	sprintf(currentValue, "%lu", configOne.maxSpd_M4);
	fileSD.println(currentValue);
	fileSD.print("Accel_M4="); //5069
	sprintf(currentValue, "%ld", configOne.accel_M4);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_M4="); //70
	sprintf(currentValue, "%ld", configOne.speedPercent_M4);
	fileSD.println(currentValue);

	fileSD.println(";");
}

void WriteSettings_Multi()
{
	char currentValue[60];

	fileSD.println("[Multi]");
	fileSD.println(";");

	fileSD.print("AxisId="); //1
	sprintf(currentValue, "%ld", configMulti.axisId);
	fileSD.println(currentValue);
	fileSD.println("; MultiAxesRunand Direction values are ANDed together.");
	fileSD.print("MultiAxesIDs="); //0
	sprintf(currentValue, "%ld", configMulti.multiAxesIDs);
	fileSD.println(currentValue);
	fileSD.println("; Spindle");
	fileSD.print("MaxSpeed_Spindle="); //25060
	sprintf(currentValue, "%lu", configMulti.maxSpd_Spindle);
	fileSD.println(currentValue);
	fileSD.print("Accel_Spindle="); //20061
	sprintf(currentValue, "%lu", configMulti.accel_Spindle);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_Spindle="); //61
	sprintf(currentValue, "%lu", configMulti.speedPercent_Spindle);
	fileSD.println(currentValue);
	fileSD.print("TargetDistance_Spindle="); //39.45
	sprintf(currentValue, "%.5g", configMulti.targetDistance_Spindle);
	fileSD.println(currentValue);

	Serial.print("TargetDistance_Spindle="); //39.45
	Serial.println(configMulti.targetDistance_Spindle,6);
	Serial.println(currentValue);

	fileSD.println("; CW = 0, CCW = 1");
	if (configMulti.direction_Spindle == 1)
	{
		fileSD.println("Direction_Spindle=0");
	}
	else
	{
		fileSD.println("Direction_Spindle=1");
	}


	fileSD.println("; Z Axis");
	fileSD.print("MaxSpeed_Z="); //5062
	sprintf(currentValue, "%lu", configMulti.maxSpd_Z);
	fileSD.println(currentValue);
	fileSD.print("Accel_Z="); //5063
	sprintf(currentValue, "%lu", configMulti.accel_Z);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_Z="); //64
	sprintf(currentValue, "%lu", configMulti.speedPercent_Z);
	fileSD.println(currentValue);
	fileSD.print("TargetDistance_Z="); //2.123
	sprintf(currentValue, "%.5g", configMulti.targetDistance_Z);
	fileSD.println(currentValue);
	fileSD.println("; Right = 0, Left = 1");

	if (configMulti.direction_Z == 1)
	{
		fileSD.println("Direction_Z=0");
	}
	else
	{
		fileSD.println("Direction_Z=1");
	}

	fileSD.println("; X Axis");
	fileSD.print("MaxSpeed_X="); //5065
	sprintf(currentValue, "%lu", configMulti.maxSpd_X);
	fileSD.println(currentValue);
	fileSD.print("Accel_X="); //5066
	sprintf(currentValue, "%lu", configMulti.accel_X);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_X="); //67
	sprintf(currentValue, "%lu", configMulti.speedPercent_X);
	fileSD.println(currentValue);
	fileSD.print("TargetDistance_X="); //3.514
	sprintf(currentValue, "%.5g", configMulti.targetDistance_X);
	fileSD.println(currentValue);
	fileSD.println("; Right = 0, Left = 1");

	if (configMulti.direction_X == 1)
	{
		fileSD.println("Direction_X=0");
	}
	else
	{
		fileSD.println("Direction_X=1");
	}

	fileSD.println("; M3 Axis");
	fileSD.print("MaxSpeed_M3="); //5068
	sprintf(currentValue, "%lu", configMulti.maxSpd_M3);
	fileSD.println(currentValue);
	fileSD.print("Accel_M3="); //5069
	sprintf(currentValue, "%lu", configMulti.accel_M3);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_M3="); //70
	sprintf(currentValue, "%lu", configMulti.speedPercent_M3);
	fileSD.println(currentValue);
	fileSD.print("TargetDistance_M3="); //1.369
	sprintf(currentValue, "%.5g", configMulti.targetDistance_M3);
	fileSD.println(currentValue);
	fileSD.println("; Right and CW = 0, Left and CCW = 1");

	if (configMulti.direction_M3 == 1)
	{
		fileSD.println("Direction_M3=0");
	}
	else
	{
		fileSD.println("Direction_M3=1");
	}

	fileSD.println("; M4");
	fileSD.print("MaxSpeed_M4="); //5068
	sprintf(currentValue, "%lu", configMulti.maxSpd_M4);
	fileSD.println(currentValue);
	fileSD.print("Accel_M4="); //5069
	sprintf(currentValue, "%lu", configMulti.accel_M4);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_M4="); //70
	sprintf(currentValue, "%lu", configMulti.speedPercent_M4);
	fileSD.println(currentValue);
	fileSD.print("TargetDistance_M4="); //0.478
	sprintf(currentValue, "%.5g", configMulti.targetDistance_M4);
	fileSD.println(currentValue);
	fileSD.println("; Right and CW = 0, Left and CCW = 1");

	if (configMulti.direction_M4 == 1)
	{
		fileSD.println("Direction_M4=0");
	}
	else
	{
		fileSD.println("Direction_M4=1");
	}


	fileSD.println(";");
}

void WriteSettings_Index()
{
	char currentValue[60];

	fileSD.println("[Index]");
	fileSD.println(";");

	fileSD.print("AxisId="); //1
	sprintf(currentValue, "%lu", configIndex_Prime.axisId);
	fileSD.println(currentValue);
	fileSD.print("IndexId="); //1
	sprintf(currentValue, "%ld", configIndex_Prime.indexId);
	fileSD.println(currentValue);
	fileSD.println("; Synchronize M3and Spindle");
	fileSD.print("Synchro_M3_and_Spindle="); //0
	sprintf(currentValue, "%lu", configIndex_Prime.synchro_M3_Spindle);
	fileSD.println(currentValue);
	fileSD.print("Synchro_Percentage="); //100
	sprintf(currentValue, "%.5g", configIndex_Prime.synchro_M3_Percentage);
	fileSD.println(currentValue);
	fileSD.println("; Spindle");
	fileSD.print("MaxSpeed_Spindle="); //5070
	sprintf(currentValue, "%lu", configIndex_Prime.maxSpd_Spindle);
	fileSD.println(currentValue);
	fileSD.print("Accel_Spindle="); //5071
	sprintf(currentValue, "%lu", configIndex_Prime.accel_Spindle);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_Spindle="); //72
	sprintf(currentValue, "%lu", configIndex_Prime.speedPercent_Spindle);
	fileSD.println(currentValue);
	fileSD.println("; M3");
	fileSD.print("MaxSpeed_M3="); //5072
	sprintf(currentValue, "%lu", configIndex_Prime.maxSpd_M3);
	fileSD.println(currentValue);
	fileSD.print("Accel_M3="); //5073
	sprintf(currentValue, "%lu", configIndex_Prime.accel_M3);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_M3="); //73
	sprintf(currentValue, "%lu", configIndex_Prime.speedPercent_M3);
	fileSD.println(currentValue);
	fileSD.println("; M4");
	fileSD.print("MaxSpeed_M4="); //5072
	sprintf(currentValue, "%lu", configIndex_Prime.maxSpd_M4);
	fileSD.println(currentValue);
	fileSD.print("Accel_M4="); //5073
	sprintf(currentValue, "%lu", configIndex_Prime.accel_M4);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_M4="); //85
	sprintf(currentValue, "%lu", configIndex_Prime.speedPercent_M4);
	fileSD.println(currentValue);
	fileSD.println("; ---------------- -");
	fileSD.println("; Index 1");
	fileSD.println("; Division:0  Degrees : 1");
	fileSD.print("DivisionsOrDegrees_1="); //1
	sprintf(currentValue, "%lu", configIndex_1.degreeOrDivision);
	fileSD.println(currentValue);
	fileSD.println("; Fixed:0  File : 1");
	fileSD.print("FixedOrFile_1="); //0
	sprintf(currentValue, "%lu", configIndex_1.fileOrFixed);
	fileSD.println(currentValue);

	Serial.print("FixedOrFile_1="); //0
	Serial.println(configIndex_1.fileOrFixed);
	Serial.println(currentValue);


	fileSD.print("Size_1="); //60.35
	sprintf(currentValue, "%.5g", configIndex_1.sizeInUnits);
	fileSD.println(currentValue);
	fileSD.println("; Index 2");
	fileSD.print("DivisionsOrDegrees_2="); //1
	sprintf(currentValue, "%lu", configIndex_2.degreeOrDivision);
	fileSD.println(currentValue);
	fileSD.print("FixedOrFile_2="); //0
	sprintf(currentValue, "%lu", configIndex_2.fileOrFixed);
	fileSD.println(currentValue);
	fileSD.print("Size_2="); //22.5
	sprintf(currentValue, "%.5g", configIndex_2.sizeInUnits);
	fileSD.println(currentValue);
	fileSD.println("; Index 3");
	fileSD.print("DivisionsOrDegrees_3="); //1
	sprintf(currentValue, "%lu", configIndex_3.degreeOrDivision);
	fileSD.println(currentValue);
	fileSD.print("FixedOrFile_3="); //0
	sprintf(currentValue, "%lu", configIndex_3.fileOrFixed);
	fileSD.println(currentValue);
	fileSD.print("Size_3="); //11.24
	sprintf(currentValue, "%.5g", configIndex_3.sizeInUnits);
	fileSD.println(currentValue);
	fileSD.println("; Index 4");
	fileSD.print("DivisionsOrDegrees_4="); //0
	sprintf(currentValue, "%lu", configIndex_4.degreeOrDivision);
	fileSD.println(currentValue);
	fileSD.print("FixedOrFile_4="); //0
	sprintf(currentValue, "%lu", configIndex_4.fileOrFixed);
	fileSD.println(currentValue);
	fileSD.print("Size_4="); //180
	sprintf(currentValue, "%.5g", configIndex_4.sizeInUnits);
	fileSD.println(currentValue);
	fileSD.println("; Index 5");
	fileSD.print("DivisionsOrDegrees_5="); //0
	sprintf(currentValue, "%lu", configIndex_5.degreeOrDivision);
	fileSD.println(currentValue);
	fileSD.print("FixedOrFile_5="); //0
	sprintf(currentValue, "%lu", configIndex_5.fileOrFixed);
	fileSD.println(currentValue);
	fileSD.print("Size_5="); //45
	sprintf(currentValue, "%.5g", configIndex_5.sizeInUnits);
	fileSD.println(currentValue);
	fileSD.println(";");
}

void WriteSettings_Move()
{
	char currentValue[60];

	fileSD.println("[Move]");
	fileSD.println(";");

	fileSD.print("AxisId="); //1
	sprintf(currentValue, "%lu", configMove.axisId);
	fileSD.println(currentValue);

	fileSD.println("; Z Axis");
	fileSD.print("MaxSpeed_Z="); //5080
	sprintf(currentValue, "%lu", configMove.maxSpd_Z);
	fileSD.println(currentValue);
	fileSD.print("Accel_Z="); //5081
	sprintf(currentValue, "%lu", configMove.accel_Z);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_Z="); //82
	sprintf(currentValue, "%lu", configMove.speedPercent_Z);
	fileSD.println(currentValue);
	fileSD.print("Distance_Z1="); //4.125
	sprintf(currentValue, "%.5g", configMove.distance_MoveZ1);
	fileSD.println(currentValue);

	Serial.print("Distance_Z1="); //4.125
	Serial.println(configMove.distance_MoveZ1,6);
	Serial.println(currentValue);

	fileSD.print("Distance_Z2="); //3.1
	sprintf(currentValue, "%.5g", configMove.distance_MoveZ2);
	fileSD.println(currentValue);
	fileSD.println("; X Axis");
	fileSD.print("MaxSpeed_X="); //5083
	sprintf(currentValue, "%lu", configMove.maxSpd_X);
	fileSD.println(currentValue);
	fileSD.print("Accel_X="); //5084
	sprintf(currentValue, "%lu", configMove.accel_X);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_X="); //85
	sprintf(currentValue, "%lu", configMove.speedPercent_X);
	fileSD.println(currentValue);
	fileSD.print("Distance_X1="); //5.75
	sprintf(currentValue, "%.5g", configMove.distance_MoveX1);
	fileSD.println(currentValue);
	fileSD.print("Distance_X2="); //5.76
	sprintf(currentValue, "%.5g", configMove.distance_MoveX2);
	fileSD.println(currentValue);
	fileSD.println("; M3 Axis");
	fileSD.print("MaxSpeed_M3="); //5086
	sprintf(currentValue, "%lu", configMove.maxSpd_M3);
	fileSD.println(currentValue);
	fileSD.print("Accel_M3="); //5087
	sprintf(currentValue, "%lu", configMove.accel_M3);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_M3="); //88
	sprintf(currentValue, "%lu", configMove.speedPercent_M3);
	fileSD.println(currentValue);
	fileSD.print("Distance_M3 - 1="); //5.78
	sprintf(currentValue, "%.5g", configMove.distance_MoveM3_1);
	fileSD.println(currentValue);
	fileSD.print("Distance_M3 - 2="); //5.79
	sprintf(currentValue, "%.5g", configMove.distance_MoveM3_2);
	fileSD.println(currentValue);
	fileSD.println("; M4 Axis");
	fileSD.print("MaxSpeed_M4="); //5088
	sprintf(currentValue, "%lu", configMove.maxSpd_M4);
	fileSD.println(currentValue);
	fileSD.print("Accel_M4="); //5089
	sprintf(currentValue, "%lu", configMove.accel_M4);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_M4="); //89
	sprintf(currentValue, "%lu", configMove.speedPercent_M4);
	fileSD.println(currentValue);
	fileSD.print("Distance_M4 - 1="); //4.78
	sprintf(currentValue, "%.5g", configMove.distance_MoveM4_1);
	fileSD.println(currentValue);
	fileSD.print("Distance_M4 - 2="); //4.79
	sprintf(currentValue, "%.5g", configMove.distance_MoveM4_2);
	fileSD.println(currentValue);

	fileSD.println(";");
}

void WriteSettings_Sync()
{
	char currentValue[60];

	fileSD.println("[Sync]");
	fileSD.println(";");


	fileSD.print("AxisId="); //1
	sprintf(currentValue, "%lu", configSync.axisId);
	fileSD.println(currentValue);

	fileSD.print("RevolutionsOrDegrees="); //0
	sprintf(currentValue, "%lu", configSync.spindle_RevolutionsOrDegrees);
	fileSD.println(currentValue);
	fileSD.println("; Helix Type : Left = 0, Right = 1");
	fileSD.print("HelixType="); //0
	sprintf(currentValue, "%lu", configSync.helixType);
	fileSD.println(currentValue);
	fileSD.println("; Revolutions = 0, Degrees = 1");
	fileSD.print("Revolutions="); //0.3
	sprintf(currentValue, "%.5g", configSync.revolutions_Spindle);
	fileSD.println(currentValue);
	fileSD.print("Distance="); //1.258
	sprintf(currentValue, "%.5g", configSync.distance);
	fileSD.println(currentValue);

	Serial.print("Distance="); //1.258
	Serial.println(configSync.distance,6);
	Serial.println(currentValue);

	fileSD.println("; Spindle");
	fileSD.print("MaxSpeed_Spindle="); //5040
	sprintf(currentValue, "%lu", configSync.maxSpd_Spindle);
	fileSD.println(currentValue);
	fileSD.print("Accel_Spindle="); //5041
	sprintf(currentValue, "%lu", configSync.accel_Spindle);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_Spindle="); //42
	sprintf(currentValue, "%lu", configSync.speedPercent_Spindle);
	fileSD.println(currentValue);
	fileSD.println("; Z Axis");
	fileSD.print("MaxSpeed_Z="); //5043
	sprintf(currentValue, "%lu", configSync.maxSpd_Z);
	fileSD.println(currentValue);
	fileSD.print("Accel_Z="); //5044
	sprintf(currentValue, "%lu", configSync.accel_Z);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_Z="); //45
	sprintf(currentValue, "%lu", configSync.speedPercent_Z);
	fileSD.println(currentValue);
	fileSD.println("; X Axis");
	fileSD.print("MaxSpeed_X="); //5046
	sprintf(currentValue, "%lu", configSync.maxSpd_X);
	fileSD.println(currentValue);
	fileSD.print("Accel_X="); //5047
	sprintf(currentValue, "%lu", configSync.accel_X);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_X="); //48
	sprintf(currentValue, "%lu", configSync.speedPercent_X);
	fileSD.println(currentValue);
	fileSD.println("; M3 Axis");
	fileSD.print("MaxSpeed_M3="); //5048
	sprintf(currentValue, "%lu", configSync.maxSpd_M3);
	fileSD.println(currentValue);
	fileSD.print("Accel_M3="); //5049
	sprintf(currentValue, "%lu", configSync.accel_M3);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_M3="); //49
	sprintf(currentValue, "%lu", configSync.speedPercent_M3);
	fileSD.println(currentValue);
	fileSD.println("; M4 Axis");
	fileSD.print("MaxSpeed_M4="); //5048
	sprintf(currentValue, "%lu", configSync.maxSpd_M4);
	fileSD.println(currentValue);
	fileSD.print("Accel_M4="); //5049
	sprintf(currentValue, "%lu", configSync.accel_M4);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_M4="); //49
	sprintf(currentValue, "%lu", configSync.speedPercent_M4);
	fileSD.println(currentValue);

	fileSD.println(";");
}

void WriteSettings_Recip()
{
	char currentValue[60];

	fileSD.println("[Recip]");
	fileSD.println(";");

	fileSD.print("AxisId="); //1
	sprintf(currentValue, "%lu", configRec.axisId);
	fileSD.println(currentValue);
	fileSD.println("; Style: 1 - Equal Angle, 2 - Square, 3 - Sawtooth");
	fileSD.print("Style" "="); //1
	sprintf(currentValue, "%lu", configRec.style);
	fileSD.println(currentValue);
	fileSD.println("; Radial=0, Axial=1");
	fileSD.print("Radial_Axial="); //0
	sprintf(currentValue, "%lu", configRec.radial_Axial);
	fileSD.println(currentValue);
	fileSD.println("; Radial");
	fileSD.print("Radial_Waves="); //5
	sprintf(currentValue, "%lu", configRec.waves_Radial);
	fileSD.println(currentValue);
	fileSD.print("Radial_Spindle_Degrees="); //18.124
	sprintf(currentValue, "%.5g", configRec.degrees_Radial_Spindle);
	fileSD.println(currentValue);


	Serial.print("Radial_Spindle_Degrees="); //18.124
	Serial.println(configRec.degrees_Radial_Spindle,6);
	Serial.println(currentValue);

	fileSD.print("Radial_Axis_Amplitude="); //4.258
	sprintf(currentValue, "%.5g", configRec.amplitude_Radial_Axis);
	fileSD.println(currentValue);
	fileSD.println("; Axial");
	fileSD.print("Axial_Waves="); //4
	sprintf(currentValue, "%lu", configRec.waves_Axial);
	fileSD.println(currentValue);
	fileSD.print("Axial_Spindle_Amplitude="); //22.258
	sprintf(currentValue, "%.5g", configRec.amplitude_Axial_Spindle);
	fileSD.println(currentValue);
	fileSD.print("Axial_Axis_Distance="); //7.896
	sprintf(currentValue, "%.5g", configRec.distance_Axial_Axis);
	fileSD.println(currentValue);
	fileSD.println("; No Last Segment : None = 1, Segment = 0");
	fileSD.print("NoLastLeg="); //0
	sprintf(currentValue, "%lu", configRec.noLast);
	fileSD.println(currentValue);
	fileSD.print("GrowAxisDistance="); //0
	sprintf(currentValue, "%.5g", configRec.grow_AxisChange);
	fileSD.println(currentValue);
	fileSD.print("GrowSpindleDistance="); //0
	sprintf(currentValue, "%.5g", configRec.grow_SpindleChange);
	fileSD.println(currentValue);
	fileSD.println("; Spindle");
	fileSD.print("MaxSpeed_Spindle="); //5040
	sprintf(currentValue, "%lu", configRec.maxSpd_Spindle);
	fileSD.println(currentValue);
	fileSD.print("Accel_Spindle="); //5041
	sprintf(currentValue, "%lu", configRec.accel_Spindle);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_Spindle="); //42
	sprintf(currentValue, "%lu", configRec.speedPercent_Spindle);
	fileSD.println(currentValue);
	fileSD.println("; Z Axis");
	fileSD.print("MaxSpeed_Z="); //5043
	sprintf(currentValue, "%lu", configRec.maxSpd_Z);
	fileSD.println(currentValue);
	fileSD.print("Accel_Z="); //5044
	sprintf(currentValue, "%lu", configRec.accel_Z);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_Z="); //45
	sprintf(currentValue, "%lu", configRec.speedPercent_Z);
	fileSD.println(currentValue);
	fileSD.println("; X Axis");
	fileSD.print("MaxSpeed_X="); //5046
	sprintf(currentValue, "%lu", configRec.maxSpd_X);
	fileSD.println(currentValue);
	fileSD.print("Accel_X="); //5047
	sprintf(currentValue, "%lu", configRec.accel_X);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_X="); //48
	sprintf(currentValue, "%lu", configRec.speedPercent_X);
	fileSD.println(currentValue);
	fileSD.println("; M3 Axis");
	fileSD.print("MaxSpeed_M3="); //5048
	sprintf(currentValue, "%lu", configRec.maxSpd_M3);
	fileSD.println(currentValue);
	fileSD.print("Accel_M3="); //5049
	sprintf(currentValue, "%lu", configRec.accel_M3);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_M3="); //49
	sprintf(currentValue, "%lu", configRec.speedPercent_M3);
	fileSD.println(currentValue);
	fileSD.println("; M4 Axis");
	fileSD.print("MaxSpeed_M4="); //5048
	sprintf(currentValue, "%lu", configRec.maxSpd_M4);
	fileSD.println(currentValue);
	fileSD.print("Accel_M4="); //5049
	sprintf(currentValue, "%lu", configRec.accel_M4);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_M4="); //49
	sprintf(currentValue, "%lu", configRec.speedPercent_M4);
	fileSD.println(currentValue);

	fileSD.println(";");
}

void WriteSettings_GreekKey()
{
	char currentValue[60];

	fileSD.println("[GreekKey]");
	fileSD.println(";");

	fileSD.print("AxisId="); //1
	sprintf(currentValue, "%lu", configGreekKey.axisId);
	fileSD.println(currentValue);


	fileSD.println("; Radial = 0, Axial = 1");
	fileSD.print("RadialOrAxial="); //0
	sprintf(currentValue, "%lu", configGreekKey.radialOrAxial_Pattern);
	fileSD.println(currentValue);
	fileSD.println("; PatternType: 4a : 2, 4b : 3, 3a : 6, 3b : 7, 2a : 4, 2b : 5");
	fileSD.print("PatternType="); //4
	sprintf(currentValue, "%lu", configGreekKey.patternId);
	fileSD.println(currentValue);
	fileSD.print("PatternsPer360="); //8
	sprintf(currentValue, "%.5g", configGreekKey.countPatternPer360_Grk);
	fileSD.println(currentValue);
	fileSD.print("PatternCount="); //24
	sprintf(currentValue, "%lu", configGreekKey.patternCount_Grk);
	fileSD.println(currentValue);
	fileSD.print("SegmentLength="); //1.2
	sprintf(currentValue, "%.5g", configGreekKey.segmentLength_Grk);
	fileSD.println(currentValue);
	fileSD.println("; Spindle");
	fileSD.print("MaxSpeed_Spindle="); //5040
	sprintf(currentValue, "%lu", configGreekKey.maxSpd_Spindle);
	fileSD.println(currentValue);
	fileSD.print("Accel_Spindle="); //5041
	sprintf(currentValue, "%lu", configGreekKey.accel_Spindle);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_Spindle="); //42
	sprintf(currentValue, "%lu", configGreekKey.speedPercent_Spindle);
	fileSD.println(currentValue);
	fileSD.println("; Z Axis");
	fileSD.print("MaxSpeed_Z="); //5043
	sprintf(currentValue, "%lu", configGreekKey.maxSpd_Z);
	fileSD.println(currentValue);
	fileSD.print("Accel_Z="); //5044
	sprintf(currentValue, "%lu", configGreekKey.accel_Z);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_Z="); //45
	sprintf(currentValue, "%lu", configGreekKey.speedPercent_Z);
	fileSD.println(currentValue);
	fileSD.println("; X Axis");
	fileSD.print("MaxSpeed_X="); //5046
	sprintf(currentValue, "%lu", configGreekKey.maxSpd_X);
	fileSD.println(currentValue);
	fileSD.print("Accel_X="); //5047
	sprintf(currentValue, "%lu", configGreekKey.accel_X);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_X="); //48
	sprintf(currentValue, "%lu", configGreekKey.speedPercent_X);
	fileSD.println(currentValue);
	fileSD.println("; M3 Axis");
	fileSD.print("MaxSpeed_M3="); //5048
	sprintf(currentValue, "%lu", configGreekKey.maxSpd_M3);
	fileSD.println(currentValue);
	fileSD.print("Accel_M3="); //5049
	sprintf(currentValue, "%lu", configGreekKey.accel_M3);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_M3="); //49
	sprintf(currentValue, "%lu", configGreekKey.speedPercent_M3);
	fileSD.println(currentValue);
	fileSD.println("; M4 Axis");
	fileSD.print("MaxSpeed_M4="); //5048
	sprintf(currentValue, "%lu", configGreekKey.maxSpd_M4);
	fileSD.println(currentValue);
	fileSD.print("Accel_M4="); //5049
	sprintf(currentValue, "%lu", configGreekKey.accel_M4);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_M4="); //49
	sprintf(currentValue, "%lu", configGreekKey.speedPercent_M4);
	fileSD.println(currentValue);


	fileSD.println(";");
}

void WriteSettings_Program()
{
	char currentValue[60];

	fileSD.println("[Program]");
	fileSD.println(";");

	//fileSD.print("AxisId="); //1
	//sprintf(currentValue, "%lu", configGreekKey.axisId);
	//fileSD.println(currentValue);

	fileSD.print("Radial_Axial="); //0
	sprintf(currentValue, "%lu", configGreekKey.radialOrAxial_File);
	fileSD.println(currentValue);
	fileSD.print("PatternsPer360="); //8.5
	sprintf(currentValue, "%.5g", configGreekKey.countPatternPer360_Program);
	fileSD.println(currentValue);
	fileSD.print("PatternCount="); //12
	sprintf(currentValue, "%lu", configGreekKey.patternCount_Program);
	fileSD.println(currentValue);
	fileSD.print("SegmentLength="); //0.032
	sprintf(currentValue, "%.5g", configGreekKey.segmentLength_Program);
	fileSD.println(currentValue);
	fileSD.println("; Segments: 2  Actual : 3");
	fileSD.print("SegmentsOrActual="); // 2
	sprintf(currentValue, "%lu", configGreekKey.segmentOrActual);
	fileSD.println(currentValue);
	fileSD.println(";");
	fileSD.println("; Spindle");
	fileSD.print("MaxSpeed_Spindle="); //5040
	sprintf(currentValue, "%lu", configGreekKey.maxSpd_Spindle);
	fileSD.println(currentValue);
	fileSD.print("Accel_Spindle="); //5041
	sprintf(currentValue, "%lu", configGreekKey.accel_Spindle);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_Spindle="); //42
	sprintf(currentValue, "%lu", configGreekKey.speedPercent_Spindle);
	fileSD.println(currentValue);
	fileSD.println("; Z Axis");
	fileSD.print("MaxSpeed_Z="); //5043
	sprintf(currentValue, "%lu", configGreekKey.maxSpd_Z);
	fileSD.println(currentValue);
	fileSD.print("Accel_Z="); //5044
	sprintf(currentValue, "%lu", configGreekKey.accel_Z);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_Z="); //45
	sprintf(currentValue, "%lu", configGreekKey.speedPercent_Z);
	fileSD.println(currentValue);
	fileSD.println("; X Axis");
	fileSD.print("MaxSpeed_X="); //5046
	sprintf(currentValue, "%lu", configGreekKey.maxSpd_X);
	fileSD.println(currentValue);
	fileSD.print("Accel_X="); //5047
	sprintf(currentValue, "%lu", configGreekKey.accel_X);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_X="); //48
	sprintf(currentValue, "%lu", configGreekKey.speedPercent_X);
	fileSD.println(currentValue);
	fileSD.println("; M3 Axis");
	fileSD.print("MaxSpeed_M3="); //5048
	sprintf(currentValue, "%lu", configGreekKey.maxSpd_M3);
	fileSD.println(currentValue);
	fileSD.print("Accel_M3="); //5049
	sprintf(currentValue, "%lu", configGreekKey.accel_M3);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_M3="); //49
	sprintf(currentValue, "%lu", configGreekKey.speedPercent_M3);
	fileSD.println(currentValue);
	fileSD.println("; M4 Axis");
	fileSD.print("MaxSpeed_M4="); //5048
	sprintf(currentValue, "%lu", configGreekKey.maxSpd_M4);
	fileSD.println(currentValue);
	fileSD.print("Accel_M4="); //5049
	sprintf(currentValue, "%lu", configGreekKey.accel_M4);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_M4="); //49
	sprintf(currentValue, "%lu", configGreekKey.speedPercent_M4);
	fileSD.println(currentValue);

	fileSD.println(";");
}

void WriteSettings_Rose()
{
	char currentValue[60];

	fileSD.println("[Rose]");
	fileSD.println(";");

	fileSD.print("AxisId="); //1
	sprintf(currentValue, "%lu", configRose.axisId);
	fileSD.println(currentValue);
	fileSD.print("Rose_n="); //9
	sprintf(currentValue, "%.5g", configRose.n);
	fileSD.println(currentValue);
	fileSD.print("Rose_d="); //5
	sprintf(currentValue, "%.5g", configRose.d);
	fileSD.println(currentValue);
	fileSD.println("; Spindle");
	fileSD.print("MaxSpeed_Spindle="); //5040
	sprintf(currentValue, "%lu", configRose.maxSpd_Spindle);
	fileSD.println(currentValue);
	fileSD.print("Accel_Spindle="); //5041
	sprintf(currentValue, "%lu", configRose.accel_Spindle);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_Spindle="); //42
	sprintf(currentValue, "%lu", configRose.speedPercent_Spindle);
	fileSD.println(currentValue);
	fileSD.print("SpindleRevolutions="); //3.125
	sprintf(currentValue, "%.5g", configRose.spindleRevolutions);
	fileSD.println(currentValue);
	fileSD.println("; Z Axis");
	fileSD.print("MaxSpeed_Z="); //5043
	sprintf(currentValue, "%lu", configRose.maxSpd_Z);
	fileSD.println(currentValue);
	fileSD.print("Accel_Z="); //5044
	sprintf(currentValue, "%lu", configRose.accel_Z);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_Z="); //45
	sprintf(currentValue, "%lu", configRose.speedPercent_Z);
	fileSD.println(currentValue);
	fileSD.print("AxialAmplitude_Z="); //2.687
	sprintf(currentValue, "%.5g", configRose.amplitude_Axial_Z);
	fileSD.println(currentValue);
	fileSD.println("; X Axis");
	fileSD.print("MaxSpeed_X="); //5046
	sprintf(currentValue, "%lu", configRose.maxSpd_X);
	fileSD.println(currentValue);
	fileSD.print("Accel_X="); //5047
	sprintf(currentValue, "%lu", configRose.accel_X);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_X="); //48
	sprintf(currentValue, "%lu", configRose.speedPercent_X);
	fileSD.println(currentValue);
	fileSD.print("AxialAmplitude_X="); //2.5
	sprintf(currentValue, "%.5g", configRose.amplitude_Axial_X);
	fileSD.println(currentValue);
	fileSD.println("; M3 Axis");
	fileSD.print("MaxSpeed_M3="); //5048
	sprintf(currentValue, "%lu", configRose.maxSpd_M3);
	fileSD.println(currentValue);
	fileSD.print("Accel_M3="); //5049
	sprintf(currentValue, "%lu", configRose.accel_M3);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_M3="); //49
	sprintf(currentValue, "%lu", configRose.speedPercent_M3);
	fileSD.println(currentValue);
	fileSD.print("AxialAmplitude_M3="); //3.5
	sprintf(currentValue, "%.5g", configRose.amplitude_Axial_M3);
	fileSD.println(currentValue);
	fileSD.println("; M4 Axis");
	fileSD.print("MaxSpeed_M4="); //5048
	sprintf(currentValue, "%lu", configRose.maxSpd_M4);
	fileSD.println(currentValue);
	fileSD.print("Accel_M4="); //5049
	sprintf(currentValue, "%lu", configRose.accel_M4);
	fileSD.println(currentValue);
	fileSD.print("SpeedPercentage_M4="); //49
	sprintf(currentValue, "%lu", configRose.speedPercent_M4);
	fileSD.println(currentValue);
	fileSD.print("AxialAmplitude_M4="); //3.568
	sprintf(currentValue, "%.5g", configRose.amplitude_Axial_M4);
	fileSD.println(currentValue);

	//fileSD.println(";");
}

/// <summary>
/// SendPackedData
/// </summary>
/// <comment>
/// Send 4 uint8_t values for conversion to uint32 in Nextion
/// </comment>
/// <returns></returns>
void SendPackedData(uint32_t eePromAddress)
{

	SerialPrint("pageIni.vEEPROM.val=");
	SerialPrint(eePromAddress);
	SerialPrint(nextionEnd);

	SerialPrint("pageIni.v00.val=");
	SerialPrint(value0);
	SerialPrint(nextionEnd);

	SerialPrint("pageIni.v08.val=");
	SerialPrint(value8);
	SerialPrint(nextionEnd);

	SerialPrint("pageIni.v16.val=");
	SerialPrint(value16);
	SerialPrint(nextionEnd);

	SerialPrint("pageIni.v24.val=");
	SerialPrint(value24);
	SerialPrint(nextionEnd);

	SerialPrint("click m0,0");
	SerialPrint(nextionEnd);

#ifdef DEBUG
	Serial.print("SendPackedData: ");
	Serial.println(eePromAddress);
	Serial.println(value0);
	Serial.println(value8);
	Serial.println(value16);
	Serial.println(value24);
#endif // DEBUG
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

	// EEPROM 104
	value0 = (uint8_t)configIndex_Prime.speedPercent_Spindle;
	value8 = (uint8_t)configIndex_Prime.axisId;
	value16 = (uint8_t)configIndex_Prime.indexId;
	value24 = (uint8_t)configSetup.auxillaryPin;

	int eePromAddress_Nextion = 104;
	const char* iniValue = "IndexId";
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_Prime.indexId = (uint32_t)returnVal;

	iniValue = "AxisId";
	eePromAddress_Nextion = 44;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_Prime.axisId = (uint32_t)returnVal;

	iniValue = "SpeedPercentage_Spindle";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configIndex_Prime.speedPercent_Spindle = (uint32_t)returnVal;

	iniValue = "MaxSpeed_Spindle";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configIndex_Prime.maxSpd_Spindle = (uint32_t)returnVal;

	iniValue = "Accel_Spindle";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configIndex_Prime.accel_Spindle = (uint32_t)returnVal;

	iniValue = "Synchro_M3_and_Spindle";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configIndex_Prime.synchro_M3_Spindle = (uint32_t)returnVal;

	iniValue = "Synchro_Percentage";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configIndex_Prime.synchro_M3_Percentage = returnVal;

	iniValue = "Accel_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configIndex_Prime.accel_M3 = (uint32_t)returnVal;

	iniValue = "MaxSpeed_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configIndex_Prime.maxSpd_M3 = (uint32_t)returnVal;

	iniValue = "SpeedPercentage_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configIndex_Prime.speedPercent_M3 = (uint32_t)returnVal;

	iniValue = "Accel_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configIndex_Prime.accel_M4 = (uint32_t)returnVal;

	iniValue = "MaxSpeed_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configIndex_Prime.maxSpd_M4 = (uint32_t)returnVal;

	iniValue = "SpeedPercentage_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configIndex_Prime.speedPercent_M4 = (uint32_t)returnVal;

	// Index 1
	iniValue = "DivisionsOrDegrees_1";
	eePromAddress_Nextion = 500;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_1.degreeOrDivision = (uint32_t)returnVal;

	iniValue = "FixedOrFile_1";
	eePromAddress_Nextion = 580;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_1.fileOrFixed = (uint32_t)returnVal;

	iniValue = "Size_1";
	returnVal = ReturnIniValue(iniKey, iniValue);
	IndexSize(returnVal, ID_INDEX_1);

	Serial.print("returnVal:  ");
	Serial.println(returnVal);
	Serial.print("configIndex_1.sizeInSteps: ");
	Serial.println(configIndex_1.sizeInSteps);

	Serial.print("configIndex_1.sizeInUnits: ");
	Serial.println(configIndex_1.sizeInUnits);

	// Index 2
	iniValue = "DivisionsOrDegrees_2";
	eePromAddress_Nextion = 504;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_2.degreeOrDivision = (uint32_t)returnVal;

	iniValue = "FixedOrFile_2";
	eePromAddress_Nextion = 584;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_2.fileOrFixed = (uint32_t)returnVal;

	iniValue = "Size_2";
	returnVal = ReturnIniValue(iniKey, iniValue);
	IndexSize(returnVal, ID_INDEX_2);

	Serial.print("returnVal:  ");
	Serial.println(returnVal);
	Serial.print("configIndex_2.sizeInSteps: ");
	Serial.println(configIndex_2.sizeInSteps);

	Serial.print("configIndex_2.sizeInUnits: ");
	Serial.println(configIndex_2.sizeInUnits);

	// Index 3
	iniValue = "DivisionsOrDegrees_3";
	eePromAddress_Nextion = 508;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_3.degreeOrDivision = (uint32_t)returnVal;

	iniValue = "FixedOrFile_3";
	eePromAddress_Nextion = 588;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_3.fileOrFixed = (uint32_t)returnVal;

	iniValue = "Size_3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	IndexSize(returnVal, ID_INDEX_3);

	// Index 4
	iniValue = "DivisionsOrDegrees_4";
	eePromAddress_Nextion = 536;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_4.degreeOrDivision = (uint32_t)returnVal;

	iniValue = "FixedOrFile_4";
	eePromAddress_Nextion = 592;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_4.fileOrFixed = (uint32_t)returnVal;

	iniValue = "Size_4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	IndexSize(returnVal, ID_INDEX_4);

	// Index 5
	iniValue = "DivisionsOrDegrees_5";
	eePromAddress_Nextion = 540;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_5.degreeOrDivision = (uint32_t)returnVal;

	iniValue = "FixedOrFile_5";
	eePromAddress_Nextion = 596;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_5.fileOrFixed = (uint32_t)returnVal;

	iniValue = "Size_5";
	returnVal = ReturnIniValue(iniKey, iniValue);
	IndexSize(returnVal, ID_INDEX_5);

	EEPROM.put(eePromAddress_Ind_Prime, configIndex_Prime);
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



	//iniValue = "AxisId";
	//returnVal = ReturnIniValue(iniKey, iniValue);
	//configRose.axisId = (int)returnVal;
	//value8 = (uint8_t)configRose.axisId;
	//// Send to Nextion
	//SendPackedData(eePromAddress_Nextion);


	const char* iniValue = "AxisId";
	int eePromAddress_Nextion = 100;
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.axisId = (uint32_t)returnVal;


	iniValue = "Rose_n";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRose.n = returnVal;

	iniValue = "Rose_d";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRose.d = returnVal;

	//iniValue = "RadialOrAxial";
	//eePromAddress_Nextion = 240;
	//returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	//configRose.radial_Axial = (uint32_t)returnVal;

	iniValue = "SpindleRevolutions";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRose.spindleRevolutions = returnVal;

	iniValue = "MaxSpeed_Spindle";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRose.maxSpd_Spindle = (uint32_t)returnVal;

	iniValue = "Accel_Spindle";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRose.accel_Spindle = (uint32_t)returnVal;

	iniValue = "SpeedPercentage_Spindle";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRose.speedPercent_Spindle = (uint32_t)returnVal;


	// Axis Speed %
	iniValue = "SpeedPercentage_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRose.speedPercent_Z = (uint32_t)returnVal;

	iniValue = "SpeedPercentage_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRose.speedPercent_X = (uint32_t)returnVal;

	iniValue = "SpeedPercentage_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRose.speedPercent_M3 = (uint32_t)returnVal;

	iniValue = "SpeedPercentage_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRose.speedPercent_M4 = (uint32_t)returnVal;

	// Z axis
	iniValue = "MaxSpeed_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRose.maxSpd_Z = (uint32_t)returnVal;

	iniValue = "Accel_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRose.accel_Z = (uint32_t)returnVal;

	//iniValue = "RadialAmplitude_Z";
	//returnVal = ReturnIniValue(iniKey, iniValue);
	//configRose.amplitude_Radial_Z = returnVal;
	//MilliDelay(3000);

	iniValue = "AxialAmplitude_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRose.amplitude_Axial_Z = returnVal;

	// X axis
	iniValue = "MaxSpeed_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRose.maxSpd_X = (uint32_t)returnVal;

	iniValue = "Accel_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRose.accel_X = (uint32_t)returnVal;

	//iniValue = "RadialAmplitude_X";
	//returnVal = ReturnIniValue(iniKey, iniValue);
	//configRose.amplitude_Radial_X = returnVal;

	iniValue = "AxialAmplitude_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRose.amplitude_Axial_X = returnVal;

	// M3 axis
	iniValue = "MaxSpeed_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRose.maxSpd_M3 = (uint32_t)returnVal;

	iniValue = "Accel_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRose.accel_M3 = (uint32_t)returnVal;

	//iniValue = "RadialAmplitude_M3";
	//returnVal = ReturnIniValue(iniKey, iniValue);
	//configRose.amplitude_Radial_M3 = returnVal;

	iniValue = "AxialAmplitude_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRose.amplitude_Axial_M3 = returnVal;

	// M4 axis
	iniValue = "MaxSpeed_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRose.maxSpd_M4 = (uint32_t)returnVal;

	iniValue = "Accel_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRose.accel_M4 = (uint32_t)returnVal;

	//iniValue = "RadialAmplitude_M4";
	//returnVal = ReturnIniValue(iniKey, iniValue);
	//configRose.amplitude_Radial_M4 = returnVal;

	iniValue = "AxialAmplitude_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRose.amplitude_Axial_M4 = returnVal;

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

	const char* iniValue = "AxisId"; // 24
	int eePromAddress_Nextion = 436;
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMove.axisId = (uint32_t)returnVal;

	// Speed %
	iniValue = "SpeedPercentage_Z"; // 0
	returnVal = (uint32_t)ReturnIniValue(iniKey, iniValue);
	configMove.speedPercent_Z = returnVal;

	iniValue = "SpeedPercentage_X"; // 8
	returnVal = (uint32_t)ReturnIniValue(iniKey, iniValue);
	configMove.speedPercent_X = returnVal;

	iniValue = "SpeedPercentage_M3";  // 16
	returnVal = (uint32_t)ReturnIniValue(iniKey, iniValue);
	configMove.speedPercent_M3 = returnVal;

	iniValue = "SpeedPercentage_M4"; // 24
	returnVal = (uint32_t)ReturnIniValue(iniKey, iniValue);
	configMove.speedPercent_M4 = returnVal;

	// Z axis
	iniValue = "MaxSpeed_Z";
	returnVal = (uint32_t)ReturnIniValue(iniKey, iniValue);
	configMove.maxSpd_Z = (uint32_t)returnVal;

	iniValue = "Accel_Z";
	returnVal = (uint32_t)ReturnIniValue(iniKey, iniValue);
	configMove.accel_Z = (uint32_t)returnVal;

	iniValue = "Distance_Z1";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMove.distance_MoveZ1 = returnVal;

	iniValue = "Distance_Z2";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMove.distance_MoveZ2 = returnVal;

	// X axis
	iniValue = "MaxSpeed_X";
	returnVal = (uint32_t)ReturnIniValue(iniKey, iniValue);
	configMove.maxSpd_X = (int)returnVal;

	iniValue = "Accel_X";
	returnVal = (uint32_t)ReturnIniValue(iniKey, iniValue);
	configMove.accel_X = (int)returnVal;

	iniValue = "Distance_X1";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMove.distance_MoveX1 = returnVal;

	iniValue = "Distance_X2";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMove.distance_MoveX2 = returnVal;

	// M3 axis
	iniValue = "MaxSpeed_M3";
	returnVal = (uint32_t)ReturnIniValue(iniKey, iniValue);
	configMove.maxSpd_M3 = (int)returnVal;

	iniValue = "Accel_M3";
	returnVal = (uint32_t)ReturnIniValue(iniKey, iniValue);
	configMove.accel_M3 = (int)returnVal;

	iniValue = "Distance_M3-1";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMove.distance_MoveM3_1 = returnVal;

	iniValue = "Distance_M3-2";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMove.distance_MoveM3_2 = returnVal;

	// M4 axis
	iniValue = "MaxSpeed_M4";
	returnVal = (uint32_t)ReturnIniValue(iniKey, iniValue);
	configMove.maxSpd_M4 = (int)returnVal;

	iniValue = "Accel_M4";
	returnVal = (uint32_t)ReturnIniValue(iniKey, iniValue);
	configMove.accel_M4 = (int)returnVal;

	iniValue = "Distance_M4-1";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMove.distance_MoveM4_1 = returnVal;

	iniValue = "Distance_M4-2";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMove.distance_MoveM4_2 = returnVal;

	EEPROM.put(eePromAddress_Mov, configMove);
}

/// <summary>
/// LoadSettings_PageGrk
/// </summary>
/// <comment>
/// Load ini file settings for Grk page
/// </comment>
/// <returns></returns>
void LoadSettings_PageGrkAndProgram()
{
	// Greek Key Main
	const char* iniKey = "GreekKey";

	const char* iniValue = "AxisId"; 
	int eePromAddress_Nextion = 160;
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.axisId = (uint32_t)returnVal;

	iniValue = "MaxSpeed_Spindle";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configGreekKey.maxSpd_Spindle = (int)returnVal;

	iniValue = "Accel_Spindle";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configGreekKey.accel_Spindle = (int)returnVal;

	iniValue = "SpeedPercentage_Spindle"; // 0
	returnVal = ReturnIniValue(iniKey, iniValue);
	configGreekKey.speedPercent_Spindle = (int)returnVal;

	iniValue = "SpeedPercentage_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configGreekKey.speedPercent_Z = (int)returnVal;

	iniValue = "SpeedPercentage_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configGreekKey.speedPercent_X = (int)returnVal;

	iniValue = "SpeedPercentage_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configGreekKey.speedPercent_M3 = (int)returnVal;

	iniValue = "SpeedPercentage_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configGreekKey.speedPercent_M4 = (int)returnVal;

	iniValue = "MaxSpeed_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configGreekKey.maxSpd_Z = (int)returnVal;

	iniValue = "Accel_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configGreekKey.accel_Z = (int)returnVal;

	iniValue = "MaxSpeed_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configGreekKey.maxSpd_X = (int)returnVal;

	iniValue = "Accel_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configGreekKey.accel_X = (int)returnVal;

	iniValue = "MaxSpeed_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configGreekKey.maxSpd_M3 = (int)returnVal;

	iniValue = "Accel_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configGreekKey.accel_M3 = (int)returnVal;

	iniValue = "MaxSpeed_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configGreekKey.maxSpd_M4 = (int)returnVal;

	iniValue = "Accel_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configGreekKey.accel_M4 = (int)returnVal;

	iniValue = "PatternsPer360";
	eePromAddress_Nextion = 372;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.countPatternPer360_Grk = returnVal;

	iniValue = "RadialOrAxial"; // 16
	returnVal = ReturnIniValue(iniKey, iniValue);
	configGreekKey.radialOrAxial_Pattern = (uint32_t)returnVal;
	configGreekKey.radialOrAxial_File = (uint32_t)returnVal;

	iniValue = "PatternType";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configGreekKey.patternId = (uint32_t)returnVal;

	iniValue = "PatternCount";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configGreekKey.patternCount_Grk = (int)returnVal;

	iniValue = "SegmentLength";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configGreekKey.segmentLength_Grk = returnVal;

	// Greek Key Main
	iniKey = "Program";

	eePromAddress_Nextion = 32;
	iniValue = "SegmentsOrActual"; // 0
	returnVal = (uint32_t)ReturnIniValue(iniKey, iniValue);
	configGreekKey.segmentOrActual = returnVal;

	iniKey = "Program";
	iniValue = "SegmentLength";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configGreekKey.segmentLength_Program = returnVal;

	iniValue = "PatternsPer360";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configGreekKey.countPatternPer360_Program = returnVal;

	iniValue = "PatternCount";
	returnVal = (uint32_t)ReturnIniValue(iniKey, iniValue);
	configGreekKey.patternCount_Program = (int)returnVal;

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
	int eePromAddress_Nextion = 164;
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.axisId = (uint32_t)returnVal;

	iniValue = "MaxSpeed_Spindle";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRec.maxSpd_Spindle = (int)returnVal;

	iniValue = "Accel_Spindle";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRec.accel_Spindle = (int)returnVal;

	iniValue = "SpeedPercentage_Spindle"; // 0
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRec.speedPercent_Spindle = (int)returnVal;

	iniValue = "SpeedPercentage_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRec.speedPercent_Z = (int)returnVal;

	iniValue = "SpeedPercentage_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRec.speedPercent_X = (int)returnVal;

	iniValue = "SpeedPercentage_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRec.speedPercent_M3 = (int)returnVal;

	iniValue = "SpeedPercentage_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRec.speedPercent_M4 = (int)returnVal;

	iniValue = "MaxSpeed_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRec.maxSpd_Z = (int)returnVal;

	iniValue = "Accel_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRec.accel_Z = (int)returnVal;

	iniValue = "MaxSpeed_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRec.maxSpd_X = (int)returnVal;

	iniValue = "Accel_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRec.accel_X = (int)returnVal;

	iniValue = "MaxSpeed_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRec.maxSpd_M3 = (int)returnVal;

	iniValue = "Accel_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRec.accel_M3 = (int)returnVal;

	iniValue = "MaxSpeed_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRec.maxSpd_M4 = (int)returnVal;

	iniValue = "Accel_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRec.accel_M4 = (int)returnVal;

	iniValue = "Radial_Waves";
	returnVal = (uint32_t)ReturnIniValue(iniKey, iniValue);
	configRec.waves_Radial = (int)returnVal;

	iniValue = "Radial_Spindle_Degrees"; // Radial Degrees
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRec.degrees_Radial_Spindle = returnVal;

	iniValue = "Radial_Axis_Amplitude";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRec.amplitude_Radial_Axis = returnVal;

	iniValue = "Axial_Waves";
	returnVal = (uint32_t)ReturnIniValue(iniKey, iniValue);
	configRec.waves_Axial = (int)returnVal;

	iniValue = "Axial_Spindle_Amplitude";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRec.amplitude_Axial_Spindle = returnVal;

	iniValue = "Axial_Axis_Distance";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRec.distance_Axial_Axis = returnVal;

	iniValue = "GrowAxisDistance";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRec.grow_AxisChange = returnVal;

	iniValue = "GrowSpindleDistance";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRec.grow_SpindleChange = returnVal;

	iniValue = "NoLastLeg";
	eePromAddress_Nextion = 192;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.noLast = (int32_t)returnVal;

	iniValue = "Style"; // 16
	eePromAddress_Nextion = 168;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.style = (uint32_t)returnVal;

	iniValue = "Radial_Axial"; // 16
	eePromAddress_Nextion = 172;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRec.radial_Axial = (uint32_t)returnVal;


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
	int eePromAddress_Nextion = 572;
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configOne.axisId = (uint32_t)returnVal;

	iniValue = "MaxSpeed_Spindle";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configOne.maxSpd_Spindle = (int)returnVal;

	iniValue = "Accel_Spindle";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configOne.accel_Spindle = (int)returnVal;

	iniValue = "SpeedPercentage_Spindle"; // 0
	returnVal = ReturnIniValue(iniKey, iniValue);
	configOne.speedPercent_Spindle = (int)returnVal;

	iniValue = "SpeedPercentage_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configOne.speedPercent_Z = (int)returnVal;

	iniValue = "SpeedPercentage_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configOne.speedPercent_X = (int)returnVal;

	iniValue = "SpeedPercentage_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configOne.speedPercent_M3 = (int)returnVal;

	iniValue = "SpeedPercentage_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configOne.speedPercent_M4 = (int)returnVal;

	iniValue = "MaxSpeed_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configOne.maxSpd_Z = (int)returnVal;

	iniValue = "Accel_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configOne.accel_Z = (int)returnVal;

	iniValue = "MaxSpeed_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configOne.maxSpd_X = (int)returnVal;

	iniValue = "Accel_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configOne.accel_X = (int)returnVal;

	iniValue = "MaxSpeed_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configOne.maxSpd_M3 = (int)returnVal;

	iniValue = "Accel_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configOne.accel_M3 = (int)returnVal;

	iniValue = "MaxSpeed_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configOne.maxSpd_M4 = (int)returnVal;

	iniValue = "Accel_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configOne.accel_M4 = (int)returnVal;

	

	EEPROM.put(eePromAddress_One, configOne);
}







/// <summary>
/// LoadSettings_PageMulti
/// </summary>
/// <comment>
/// Load ini file settings for Multi page
/// </comment>
/// <returns></returns>
void LoadSettings_PageMulti()
{
	const char* iniKey = "Multi";

	int eePromAddress_Nextion = 420;
	const char* iniValue = "AxisId";
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMulti.axisId = (int)returnVal;
	
	iniValue = "SpeedPercentage_Spindle"; // pageMulti
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMulti.speedPercent_Spindle = (int32_t)returnVal;

	iniValue = "MultiAxesIds";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMulti.multiAxesIDs = (int32_t)returnVal;

	iniValue = "SpeedPercentage_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMulti.speedPercent_Z = (int32_t)returnVal;

	iniValue = "SpeedPercentage_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMulti.speedPercent_X = (int32_t)returnVal;

	iniValue = "SpeedPercentage_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMulti.speedPercent_M3 = (int32_t)returnVal;

	iniValue = "SpeedPercentage_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMulti.speedPercent_M4 = (int32_t)returnVal;

	// MaxSpeed
	iniValue = "MaxSpeed_Spindle";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMulti.maxSpd_Spindle = (int)returnVal;

	iniValue = "MaxSpeed_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMulti.maxSpd_Z = (int)returnVal;

	iniValue = "MaxSpeed_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMulti.maxSpd_X = (int)returnVal;

	iniValue = "MaxSpeed_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMulti.maxSpd_M3 = (int)returnVal;

	iniValue = "MaxSpeed_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMulti.maxSpd_M4 = (int)returnVal;

	// Accel
	iniValue = "Accel_Spindle";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMulti.accel_Spindle = (int)returnVal;

	iniValue = "Accel_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMulti.accel_Z = (int)returnVal;

	iniValue = "Accel_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMulti.accel_X = (int)returnVal;

	iniValue = "Accel_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMulti.accel_M3 = (int)returnVal;

	iniValue = "Accel_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMulti.accel_M4 = (int)returnVal;

	// TargetDistance
	iniValue = "TargetDistance_Spindle";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMulti.targetDistance_Spindle = returnVal;

	iniValue = "TargetDistance_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMulti.targetDistance_Z = returnVal;

	iniValue = "TargetDistance_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMulti.targetDistance_X = returnVal;

	iniValue = "TargetDistance_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMulti.targetDistance_M3 = returnVal;

	iniValue = "TargetDistance_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMulti.targetDistance_M4 = returnVal;

	// Direction
	iniValue = "Direction_Spindle";
	returnVal = ReturnIniValue(iniKey, iniValue);
	if (returnVal == 0)
	{
		configMulti.direction_Spindle = (1);
	}
	else
	{
		configMulti.direction_Spindle = (-1);
	}

	iniValue = "Direction_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	if (returnVal == 0)
	{
		configMulti.direction_Z = (1);
	}
	else
	{
		configMulti.direction_Z = (-1);
	}

	iniValue = "Direction_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	if (returnVal == 0)
	{
		configMulti.direction_X = (1);
	}
	else
	{
		configMulti.direction_X = (-1);
	}

	iniValue = "Direction_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	if (returnVal == 0)
	{
		configMulti.direction_M3 = (1);
	}
	else
	{
		configMulti.direction_M3 = (-1);
	}

	iniValue = "Direction_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	if (returnVal == 0)
	{
		configMulti.direction_M4 = (1);
	}
	else
	{
		configMulti.direction_M4 = (-1);
	}

	EEPROM.put(eePromAddress_Multi, configMulti);
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
	const char* iniValue = "AxisId"; // (8)
	int eePromAddress_Nextion = 8;
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMain.axisId = (int)returnVal;

	//MilliDelay(1000);
	iniValue = "MaxSpeed_Spindle";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMain.maxSpd_Spindle = (int)returnVal;
	//MilliDelay(1000);
	iniValue = "Accel_Spindle";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMain.accel_Spindle = (int)returnVal;
	//MilliDelay(1000);
	iniValue = "SpeedPercentage_Spindle";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMain.speedPercent_Spindle = (int)returnVal;
	//MilliDelay(1000);
	iniValue = "SpeedPercentage_Z"; // 0
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMain.speedPercent_Z = (int)returnVal;
	//MilliDelay(1000);
	iniValue = "SpeedPercentage_X"; // 8
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMain.speedPercent_X = (int)returnVal;

	iniValue = "SpeedPercentage_M3"; // 16
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMain.speedPercent_M3 = (int)returnVal;

	iniValue = "SpeedPercentage_M4"; // 24
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMain.speedPercent_M4 = (int)returnVal;

	iniValue = "MaxSpeed_Z";
	eePromAddress_Nextion = 952;
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMain.maxSpd_Z = (int)returnVal;

	iniValue = "Accel_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMain.accel_Z = (int)returnVal;

	iniValue = "MaxSpeed_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMain.maxSpd_X = (int)returnVal;

	iniValue = "Accel_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMain.accel_X = (int)returnVal;

	iniValue = "MaxSpeed_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMain.maxSpd_M3 = (int)returnVal;

	iniValue = "Accel_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMain.accel_M3 = (int)returnVal;

	iniValue = "MaxSpeed_M4"; 
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMain.maxSpd_M4 = (int)returnVal;

	iniValue = "Accel_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMain.accel_M4 = (int)returnVal;

	iniValue = "Synchro_M3_and_Spindle";
	eePromAddress_Nextion = 4;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMain.synchro_M3_Spindle = (int)returnVal;

	iniValue = "Synchro_Percentage";
	eePromAddress_Nextion = 24;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configMain.synchro_M3_Percentage = returnVal;

	EEPROM.put(eePromAddress_Main, configMain);
}

/// <summary>
/// LoadSettings_PageSync
/// </summary>
/// <comment>
/// Load ini file settings for Sync page
/// </comment>
/// <returns></returns>
void LoadSettings_Shared()
{
	const char* iniKey = "Shared";// 0
	int eePromAddress_Nextion = 496;
	const char* iniValue = "AxisId";  // 8
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.axisId = (int)returnVal;



	// Axis Speed %
	// EEPROM 48
	value0 = (uint8_t)configSync.speedPercent_Z;
	value8 = (uint8_t)configSync.speedPercent_X;
	value16 = (uint8_t)configSync.speedPercent_M3;
	value24 = (uint8_t)configSync.speedPercent_M4;
	eePromAddress_Nextion = 48;

	iniValue = "SpeedPercentage_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.speedPercent_Z = (int)returnVal;
	configGreekKey.speedPercent_Z = (int)returnVal;
	configRec.speedPercent_Z = (int)returnVal;

	iniValue = "SpeedPercentage_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.speedPercent_X = (int)returnVal;
	configGreekKey.speedPercent_X = (int)returnVal;
	configRec.speedPercent_X = (int)returnVal;

	iniValue = "SpeedPercentage_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.speedPercent_M3 = (int)returnVal;
	configGreekKey.speedPercent_M3 = (int)returnVal;
	configRec.speedPercent_M3 = (int)returnVal;

	iniValue = "SpeedPercentage_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.speedPercent_M4 = (int)returnVal;
	configGreekKey.speedPercent_M4 = (int)returnVal;
	configRec.speedPercent_M4 = (int)returnVal;


	iniValue = "MaxSpeed_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.maxSpd_Z = (int)returnVal;
	configGreekKey.maxSpd_Z = (int)returnVal;
	configRec.maxSpd_Z = (int)returnVal;

	iniValue = "Accel_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.accel_Z = (int)returnVal;
	configGreekKey.accel_Z = (int)returnVal;
	configRec.accel_Z = (int)returnVal;

	iniValue = "MaxSpeed_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.maxSpd_X = (int)returnVal;
	configGreekKey.maxSpd_X = (int)returnVal;
	configRec.maxSpd_X = (int)returnVal;

	iniValue = "Accel_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.accel_X = (int)returnVal;
	configGreekKey.accel_X = (int)returnVal;
	configRec.accel_X = (int)returnVal;

	iniValue = "MaxSpeed_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.maxSpd_M3 = (int)returnVal;
	configGreekKey.maxSpd_M3 = (int)returnVal;
	configRec.maxSpd_M3 = (int)returnVal;

	iniValue = "Accel_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.accel_M3 = (int)returnVal;
	configGreekKey.accel_M3 = (int)returnVal;
	configRec.accel_M3 = (int)returnVal;

	iniValue = "MaxSpeed_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.maxSpd_M4 = (int)returnVal;
	configGreekKey.maxSpd_M4 = (int)returnVal;
	configRec.maxSpd_M4 = (int)returnVal;

	iniValue = "Accel_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.accel_M4 = (int)returnVal;
	configGreekKey.accel_M4 = (int)returnVal;
	configRec.accel_M4 = (int)returnVal;



	EEPROM.put(eePromAddress_Main, configMain);
	EEPROM.put(eePromAddress_Sync, configSync);
	EEPROM.put(eePromAddress_Rec, configRec);
	EEPROM.put(eePromAddress_Grk, configGreekKey);
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
	int eePromAddress_Nextion = 496;
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.axisId = (int)returnVal;

	iniValue = "MaxSpeed_Spindle";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.maxSpd_Spindle = (int)returnVal;

	iniValue = "Accel_Spindle";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.accel_Spindle = (int)returnVal;

	iniValue = "SpeedPercentage_Spindle"; // 0
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.speedPercent_Spindle = (int)returnVal;

	iniValue = "SpeedPercentage_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.speedPercent_Z = (int)returnVal;

	iniValue = "SpeedPercentage_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.speedPercent_X = (int)returnVal;

	iniValue = "SpeedPercentage_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.speedPercent_M3 = (int)returnVal;

	iniValue = "SpeedPercentage_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.speedPercent_M4 = (int)returnVal;

	iniValue = "MaxSpeed_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.maxSpd_Z = (int)returnVal;

	iniValue = "Accel_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.accel_Z = (int)returnVal;

	iniValue = "MaxSpeed_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.maxSpd_X = (int)returnVal;

	iniValue = "Accel_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.accel_X = (int)returnVal;

	iniValue = "MaxSpeed_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.maxSpd_M3 = (int)returnVal;

	iniValue = "Accel_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.accel_M3 = (int)returnVal;

	iniValue = "MaxSpeed_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.maxSpd_M4 = (int)returnVal;

	iniValue = "Accel_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.accel_M4 = (int)returnVal;

	iniValue = "HelixType"; //Helix Type
	eePromAddress_Nextion = 108;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.helixType = (int)returnVal;

	iniValue = "RevolutionsOrDegrees";
	eePromAddress_Nextion = 84;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.spindle_RevolutionsOrDegrees = (int)returnVal;

	iniValue = "Revolutions";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.revolutions_Spindle = returnVal;

	iniValue = "Distance";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.distance = returnVal;

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
	const char* iniKey = "Preferences";

	const char* iniValue = "Polarity_Spindle";
	int eePromAddress_Nextion = 520;
	float returnVal = ReturnIniValue(iniKey, iniValue);
	returnVal >= 1 ? (configSetup.polarity_Spindle = true) : (configSetup.polarity_Spindle = false);

	iniValue = "Polarity_Z";
	eePromAddress_Nextion = 524;
	returnVal = ReturnIniValue(iniKey, iniValue);
	returnVal >= 1 ? (configSetup.polarity_Axis_Z = true) : (configSetup.polarity_Axis_Z = false);

	iniValue = "Polarity_X";
	eePromAddress_Nextion = 528;
	returnVal = ReturnIniValue(iniKey, iniValue);
	returnVal >= 1 ? (configSetup.polarity_Axis_X = true) : (configSetup.polarity_Axis_X = false);

	iniValue = "Polarity_AltX";
	eePromAddress_Nextion = 176;
	returnVal = ReturnIniValue(iniKey, iniValue);
	returnVal >= 1 ? (configSetup.polarity_Axis_XAlt = true) : (configSetup.polarity_Axis_XAlt = false);

	eePromAddress_Nextion = 396;
	iniValue = "Polarity_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	returnVal >= 1 ? (configSetup.polarity_Axis_M3 = true) : (configSetup.polarity_Axis_M3 = false);

	// EEPROM 460
	iniValue = "Polarity_M4";
	eePromAddress_Nextion = 460;
	returnVal = ReturnIniValue(iniKey, iniValue);
	returnVal >= 1 ? (configSetup.polarity_Axis_M4 = true) : (configSetup.polarity_Axis_M4 = false);

	iniValue = "RadialOrLineal_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.radialOrLinear_Axis_M4 = returnVal;

	iniValue = "RadialOrLineal_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.radialOrLinear_Axis_M3 = (int)returnVal;

	// Keep steppers enabled
	iniValue = "KeepSteppersEnabled";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.keepSteppersEnabled = (int)returnVal;

	iniValue = "Auxillary_Pin";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.auxillaryPin = (int)returnVal;

	// Leadscrew Hand
		// EEPROM 252
	eePromAddress_Nextion = 252;
	iniKey = "Preferences";
	iniValue = "Leadscrew_Hand_Z";
	eePromAddress_Nextion = 244;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.leadscrewHand_Z = (uint32_t)returnVal;

	iniValue = "Leadscrew_Hand_X";
	eePromAddress_Nextion = 248;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.leadscrewHand_X = (uint32_t)returnVal;

	iniValue = "Leadscrew_Hand_AltX";
	eePromAddress_Nextion = 260;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.leadscrewHand_AltX = (uint32_t)returnVal;

	
	iniValue = "Leadscrew_Hand_M3";
	eePromAddress_Nextion = 252;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.leadscrewHand_M3 = (uint32_t)returnVal;

	// Send to Nextion
	SendPackedData(eePromAddress_Nextion);

	// Leadscrew_Hand_M4
	iniValue = "Leadscrew_Hand_M4";
	eePromAddress_Nextion = 256;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.leadscrewHand_M4 = (uint32_t)returnVal;

	// Show Vendor Splashscreen
	iniValue = "VendorId";
	eePromAddress_Nextion = 0;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.vendorId = (int)returnVal;

	// pageSpindle
	iniValue = "Microsteps_Spindle";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.microsteps_Spindle = (int)returnVal;

	iniValue = "StepsPer360_Spindle";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.steps360_Spindle = (int)returnVal;

	iniValue = "GearRatio_Spindle";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.gearRatio_Spindle = returnVal;

	// Page Z Axis
	iniValue = "Microsteps_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.microsteps_Axis_Z = (int)returnVal;

	iniValue = "StepsPer360_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.steps360_Axis_Z = (int)returnVal;

	iniValue = "DistancePer360_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.distancePerRev_AxisZ = returnVal;

	// Page X Axis
	// Set default to X axis
	// pageX.vaXaltX
	eePromAddress_Nextion = 188;
	SerialPrint("wepo 0,188");
	SerialPrint(nextionEnd);
	configSetup.xAltX = 0;

	iniValue = "Microsteps_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.microsteps_Axis_X = (int)returnVal;

	iniValue = "StepsPer360_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.steps360_Axis_X = (int)returnVal;

	iniValue = "DistancePer360_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.distancePerRev_AxisX = returnVal;

	// Page Alternate X Axis
	iniValue = "Microsteps_AltX";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.microsteps_Axis_XAlt = (int)returnVal;

	iniValue = "StepsPer360_AltX";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.steps360_Axis_XAlt = (int)returnVal;

	iniValue = "DistancePer360_AltX";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.distancePerRev_AxisXAlt = returnVal;

	// M3 Axis
	iniValue = "Microsteps_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.microsteps_Axis_M3 = (int)returnVal;

	iniValue = "StepsPer360_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.steps360_Axis_M3 = (int)returnVal;

	iniValue = "GearRatio_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.gearRatio_AxisM3 = returnVal;

	iniValue = "DistancePer360_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.distancePerRev_AxisM3 = returnVal;

	iniValue = "M3Radius";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.radiusM3 = returnVal;



	// M4 Axis
	iniValue = "Microsteps_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.microsteps_Axis_M4 = (int)returnVal;

	iniValue = "StepsPer360_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.steps360_Axis_M4 = (int)returnVal;

	iniValue = "GearRatio_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.gearRatio_AxisM4 = returnVal;

	iniValue = "DistancePer360_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.distancePerRev_AxisM4 = returnVal;

	iniValue = "M4Radius";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.radiusM4 = returnVal;
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
	const char* iniValue = "Min_M3";
	float returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.limit_Min_M3 = (int)returnVal;

	iniValue = "Max_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.limit_Max_M3 = (int)returnVal;

	iniValue = "eStop";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.eStop = (int)returnVal;

	iniValue = "StopSpindle";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.limit_StopSpindle = (int)returnVal;

	iniValue = "Min_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.limit_Min_Z = (int)returnVal;

	iniValue = "Max_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.limit_Max_Z = (int)returnVal;

	iniValue = "Min_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.limit_Min_X = (int)returnVal;

	iniValue = "Max_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.limit_Max_X = (int)returnVal;

	iniValue = "Min_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.limit_Min_M4 = (int)returnVal;

	iniValue = "Max_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.limit_Max_M4 = (int)returnVal;

	iniValue = "NC-NO";
	returnVal = ReturnIniValue(iniKey, iniValue);
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
	float returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.maxSpd_Return_Spindle = (int)returnVal;

	iniValue = "Accel_Spindle";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.accel_Return_Spindle = (int)returnVal;

	iniValue = "MaxSpeed_Axis_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.maxSpd_Return_Axis_Z = (int)returnVal;

	iniValue = "Accel_Axis_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.accel_Return_Axis_Z = (int)returnVal;

	iniValue = "MaxSpeed_Axis_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.maxSpd_Return_Axis_X = (int)returnVal;

	iniValue = "Accel_Axis_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.accel_Return_Axis_X = (int)returnVal;

	iniValue = "MaxSpeed_Axis_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.maxSpd_Return_Axis_M3 = (int)returnVal;

	iniValue = "Accel_Axis_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.accel_Return_Axis_M3 = (int)returnVal;

	iniValue = "MaxSpeed_Axis_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.maxSpd_Return_Axis_M4 = (int)returnVal;

	iniValue = "Accel_Axis_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.accel_Return_Axis_M4 = (int)returnVal;

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
	const size_t bufferLen = 80;
	char buffer[bufferLen];

	Serial.print("ini_FileName: ");
	Serial.println(fileNameIni);

	IniFile ini(fileNameIni);

	if (!ini.open())
	{
#ifdef DEBUG
		Serial.print("Ini file ");
		Serial.print(fileNameIni);
		Serial.println(" does not exist");
#endif // DEBUG
		SerialPrint("vis t1,1");
		SerialPrint(nextionEnd);
		SerialPrint("bt0.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("va0.val=0");
		SerialPrint(nextionEnd);

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
		//	Serial.println(buffer);
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

#ifdef VERBOSE
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
		Serial.print("buffer-Text: ");
		Serial.println(buffer);

#endif // DEBUG

		SerialPrint("pageIni.t11.txt=");
		SerialWrite(0x22);
		SerialPrint(iniKey);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageIni.t12.txt=");
		SerialWrite(0x22);
		SerialPrint(iniValue);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageIni.t13.txt=");
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

/// <summary>
/// GetIniValue
/// </summary>
/// <comment>
/// Get value from ini file. Don't send to Nextion
/// </comment>
/// <param name="iniKey">Key name</param>
/// <param name="iniValue">Value name</param>
/// <param name="eePromNextion">Nextion EEProm address</param>
/// <param name="asString">True:Get value as string. False: Value as number</param>
/// <returns>float</returns>
float ReturnIniValue(const char* iniKey, const char* iniValue)//, int eePromNextion, bool asString)
{
	const size_t bufferLen = 80;
	char buffer[bufferLen];
	

	MilliDelay(50);
	//Serial.print("ini_FileName: ");
	//Serial.print(fileNameIni);

	IniFile ini(fileNameIni);
	// Verify ini file exists.
	if (!ini.open())
	{
#ifdef DEBUG
		Serial.print("Ini file ");
		Serial.print(fileNameIni);
		Serial.println(" does not exist");
#endif // DEBUG
		SerialPrint("vis t1,1");
		SerialPrint(nextionEnd);
		SerialPrint("bt0.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("va0.val=0");
		SerialPrint(nextionEnd);

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
		Serial.print(iniKey);
		Serial.print(" : ");
		Serial.print(iniValue);
		Serial.print(" : ");
		Serial.println(buffer);
#endif // DEBUG

		SerialPrint("pageIni.t11.txt=");
		SerialWrite(0x22);
		SerialPrint(iniKey);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageIni.t12.txt=");
		SerialWrite(0x22);
		SerialPrint(iniValue);
		SerialPrint(nextionQuoteEnd);

		SerialPrint("pageIni.t13.txt=");
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

	MilliDelay(150);
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

		SerialPrint("pageMain.bt7.bco=55715");
		SerialPrint(nextionEnd);
		MilliDelay(10);
		SerialPrint("pageMain.bt7.val=0");
		SerialPrint(nextionEnd);

		SerialPrint("pageMain.bt0.bco=55715");
		SerialPrint(nextionEnd);
		MilliDelay(10);
		SerialPrint("pageMain.bt0.val=0");
		SerialPrint(nextionEnd);

		SerialPrint("pageMain.bt1.bco=55715");
		SerialPrint(nextionEnd);
		MilliDelay(10);
		SerialPrint("pageMain.bt1.val=0");
		SerialPrint(nextionEnd);

		SerialPrint("pageMain.bt2.bco=55715");
		SerialPrint(nextionEnd);
		MilliDelay(10);
		SerialPrint("pageMain.bt2.val=0");
		SerialPrint(nextionEnd);

		SerialPrint("pageMain.bt3.bco=55715");
		SerialPrint(nextionEnd);
		MilliDelay(10);
		SerialPrint("pageMain.bt3.val=0");
		SerialPrint(nextionEnd);

		MilliDelay(10);
		SerialPrint("pageMain.va2.val=10");
		SerialPrint(nextionEnd);
	}
	else if (page == PAGE_MULTI)
	{
		// Change Btn color on Nextion
		SerialPrint("pageMulti.bt1001.bco=55715");
		SerialPrint(nextionEnd);
		MilliDelay(10);
		SerialPrint("pageMulti.bt1001.val=0");
		SerialPrint(nextionEnd);

		SerialPrint("pageMulti.bt1002.bco=55715");
		SerialPrint(nextionEnd);
		MilliDelay(10);
		SerialPrint("pageMulti.bt1002.val=0");
		SerialPrint(nextionEnd);

		SerialPrint("pageMulti.bt1003.bco=55715");
		SerialPrint(nextionEnd);
		MilliDelay(10);
		SerialPrint("pageMulti.bt1003.val=0");
		SerialPrint(nextionEnd);

		SerialPrint("pageMulti.bt1004.bco=55715");
		SerialPrint(nextionEnd);
		MilliDelay(10);
		SerialPrint("pageMulti.bt1004.val=0");
		SerialPrint(nextionEnd);

		SerialPrint("pageMulti.bt1000.bco=55715");
		SerialPrint(nextionEnd);
		MilliDelay(10);
		SerialPrint("pageMulti.bt1000.val=0");
		SerialPrint(nextionEnd);

		SerialPrint("pageMulti.bt0.bco=55715");
		SerialPrint(nextionEnd);
		MilliDelay(10);
		SerialPrint("pageMulti.bt0.val=0");
		SerialPrint(nextionEnd);

		SerialPrint("pageMulti.bt1.bco=55715");
		SerialPrint(nextionEnd);
		MilliDelay(10);
		SerialPrint("pageMulti.bt1.val=0");
		SerialPrint(nextionEnd);

		SerialPrint("pageMulti.bt2.bco=55715");
		SerialPrint(nextionEnd);
		MilliDelay(10);
		SerialPrint("pageMulti.bt2.val=0");
		SerialPrint(nextionEnd);


		MilliDelay(10);
		SerialPrint("pageMulti.va0.val=10");
		SerialPrint(nextionEnd);
	}
	else
	{
		switch (page)
		{

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
		case PAGE_ONE:
		{
			pageName = "pageOne";
			break;
		}
		}

		printString = bt0_bco;
#ifdef VERBOSE
		Serial.println(printString);
#endif // VERBOSE

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

	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=1");
	SerialPrint(nextionEnd);
}

void SetLimitColors(uint32_t page, int32_t direction)
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
	else if (page == PAGE_MULTI)
	{
		// Handled in SetEStopColors
	}
	else
	{
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