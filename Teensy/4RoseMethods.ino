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
//	const char * configSteppers_Char = "configSteppers:";
	const char * configPageReci_Char = "configPageReci:";
	const char * configPageRose_Char = "configPageRose:";
	const char * configPageMainMulti_Char = "configPageMainMulti:";
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
	//Serial.print(configSteppers_Char);
	//sizeS = sizeof(configSteppers);
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
//	else if (SD.begin(PIN_SPI_CS_24)) // SPI microSD CS on pin 24
//	{
//		microSD_Id = 24;
//#ifdef DEBUG
//		Serial.println("microSD-24");
//#endif // DEBUG
//	}
//	else if (SD.begin(PIN_SPI_CS_15)) // SPI microSD CS on pin 15
//	{
//		microSD_Id = 15;
//#ifdef DEBUG
//		Serial.println("microSD-15");
//#endif // DEBUG
//	}

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
		case ID_AXIS_3:
		{
			if (enabled)
			{
				digitalWrite(PIN_AXIS_3_ENABLE, configSetup.polarity_Axis_M3 ? (LOW) : (HIGH)); // Enable 
			}
			else
			{
				if (!ignoreDisable)
				{
					digitalWrite(PIN_AXIS_3_ENABLE, configSetup.polarity_Axis_M3 ? (HIGH) : (LOW)); // Disable 
				}
			}
			break;
		}
		case ID_AXIS_4:
		{
			if (enabled)
			{
				digitalWrite(PIN_AXIS_4_ENABLE, configSetup.polarity_Axis_M4 ? (LOW) : (HIGH)); // Enable 
			}
			else
			{
				if (!ignoreDisable)
				{
					digitalWrite(PIN_AXIS_4_ENABLE, configSetup.polarity_Axis_M4 ? (HIGH) : (LOW)); // Disable 
				}
			}
			break;
		}
	}

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
#ifndef TEENSY_32
		case 4:
		{
			retVal = GetSerialFloatA(Serial4);
			break;
		}
#endif // Teensy 32
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
	double retVal = 0;
	int i;
	for (i = 0; i <= bufferLength; i++)
	{
#ifdef DEBUG
		Serial.println(in);
#endif //Debug
		inputChar = port.read();
#ifdef DEBUG
		Serial.println("");
		Serial.print("InputChar:---------------------- ");
		Serial.print(i);
		Serial.print(" :  ");
		Serial.print(inputChar);
		Serial.print("  ");
		Serial.println(int(inputChar));
#endif //DEBUG
		switch (inputChar)
		{
			case 255:
			{
				// Read and discard anything more on serial.
				while (port.available() > 0)
				{
					port.read();
				}

				retVal = atof(in);
#ifdef DEBUG
				Serial.print("In: ");
				Serial.println(in);
				Serial.print("Float: ");
				Serial.println(retVal, 4);
#endif //DEBUG
				return atof(in);
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

	retVal = atof(in);

#ifdef DEBUG
	Serial.print("In: ");
	Serial.println(in);
	Serial.print("Float: ");
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
#ifdef DEBUG
	Serial.print("inputHex:   ");
	Serial.println(inputHex);
#endif // DEBUG
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
#ifdef DEBUG
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
#endif // DEBUG
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

	float stepsPer360 = configSetup.steps360_Axis_M4* configSetup.microsteps_Axis_M4* configSetup.gearRatio_AxisM4;
	float circumference = 2 * M_PI * configSetup.radiusM4;
	float angle = linearDistance / circumference;
	retVal = stepsPer360 * angle;
#ifdef DEBUG
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
#endif // DEBUG
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

	//float retVal = (configMove.distance_MoveB / (configSetup.distancePerRev_AxisB) * (configSetup.steps360_Axis_B * configSetup.microsteps_Axis_B));
	float retVal = (distance / (configSetup.distancePerRev_AxisM3) * (configSetup.steps360_Axis_M3 * configSetup.microsteps_Axis_M3));
#ifdef DEBUG
	Serial.print("distance: ");
	Serial.println(distance);
	Serial.print("DistanceToSteps_M3Linear: ");
	Serial.println(retVal);
#endif // Debug	
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
#ifdef DEBUG
	Serial.print("distance: ");
	Serial.println(distance);
	Serial.print("DistanceToSteps_M4Linear: ");
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
			configMain.speedPercent_Spindle = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Main, configMain);
			break;
		}

		case PAGE_MULTI:
		case PAGE_ONE:
		{
			configMulti.speedPercent_Spindle = (int)GetSerialFloat(serialId);
#ifdef DEBUG
			Serial.println(configMulti.speedPercent_Spindle);
#endif // DEBUG
			EEPROM.put(eePromAddress_Multi, configMulti);
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
		case PAGE_SYNC:
		case PAGE_REC:
		case PAGE_GRK:
		case PAGE_PROGRAM:
		{
			configSync.speedPercent_Spindle = (int)GetSerialFloat(serialId);
			configRec.speedPercent_Spindle = configSync.speedPercent_Spindle;
			configGreekKey.speedPercent_Spindle = configSync.speedPercent_Spindle;

			EEPROM.put(eePromAddress_Sync, configSync);
			EEPROM.put(eePromAddress_Rec, configRec);
			EEPROM.put(eePromAddress_Grk, configGreekKey);
	#ifdef DEBUG
			Serial.print(spindle_SpeedPercent_Char);
			Serial.println(configSync.speedPercent_Spindle);
	#endif // DEBUG
			break;
		}
		case PAGE_ROSE:
		{
			configRose.speedPercent_Spindle = (int)GetSerialFloat(serialId);
			EEPROM.put(eePromAddress_Rose, configRose);
	#ifdef DEBUG
			Serial.print(spindle_SpeedPercent_Char);
			Serial.println(configRose.speedPercent_Spindle);
	#endif // DEBUG
			break;
		}
	}
}

void GetAxisSpeed()
{
#ifdef DEBUG

	Serial.println("Enter case 111: >>>>>>>>>>>>>>>>>>>>>>>>>>");
	Serial.print("pageCallerId: ");
	Serial.println(pageCallerId);
#endif // DEBUG
	const char* x_SpeedPercent_Char = "X axis Speed Percent:";
	const char* m3_SpeedPercent_Char = "M3 axis Speed Percent:";
	const char* m4_SpeedPercent_Char = "M4 axis Speed Percent:";
	const char* z_SpeedPercent_Char = "Z axis Speed Percent:";
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
					configMain.speedPercent_Axis_Z = newSpeedPercentage;
		#ifdef DEBUG
					Serial.print(z_SpeedPercent_Char);
					Serial.println(configMain.speedPercent_Axis_Z);
		#endif // DEBUG
					EEPROM.put(eePromAddress_Main, configMain);
					break;
				}
				case ID_AXIS_X:
				{
					configMain.speedPercent_Axis_X = newSpeedPercentage;
					EEPROM.put(eePromAddress_Main, configMain);
		#ifdef DEBUG
					Serial.print(x_SpeedPercent_Char);
					Serial.println(configMain.speedPercent_Axis_X);
		#endif // DEBUG
					break;
				}
				case ID_AXIS_3:
				{
					configMain.speedPercent_Axis_M3 = newSpeedPercentage;
					EEPROM.put(eePromAddress_Main, configMain);
		#ifdef DEBUG
					Serial.print(m3_SpeedPercent_Char);
					Serial.println(configMain.speedPercent_Axis_M3);
		#endif // DEBUG
					break;
				}
				case ID_AXIS_4:
				{
					configMain.speedPercent_Axis_M4 = newSpeedPercentage;
					EEPROM.put(eePromAddress_Main, configMain);
#ifdef DEBUG
					Serial.print(m4_SpeedPercent_Char);
					Serial.println(configMain.speedPercent_Axis_M4);
#endif // DEBUG
					break;
				}
			}
			break;
		}
		case PAGE_MULTI:
		case PAGE_ONE:
		{
			int newSpeedPercentage = (int)GetSerialFloat(serialId);
			switch (configMulti.axisId)
			{
				case ID_AXIS_Z:
				{
					configMulti.speedPercent_Axis_Z = newSpeedPercentage;
					break;
				}
				case ID_AXIS_X:
				{
					configMulti.speedPercent_Axis_X = newSpeedPercentage;
					break;
				}
				case ID_AXIS_3:
				{
					configMulti.speedPercent_Axis_M3 = newSpeedPercentage;
					break;
				}
				case ID_AXIS_4:
				{
					configMulti.speedPercent_Axis_M4 = newSpeedPercentage;
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
		case PAGE_INDEX:
		{
			switch (configIndex_Prime.axisId)
			{
				case ID_AXIS_3:
				{
					configIndex_Prime.speedPercent_Axis_M3 = (int)GetSerialFloat(serialId);
	#ifdef DEBUG
					Serial.print(m3_SpeedPercent_Char);
					Serial.println(configIndex_Prime.speedPercent_Axis_M3);
	#endif // DEBUG
					break;
				}
				case ID_AXIS_4:
				{
					configIndex_Prime.speedPercent_Axis_M4 = (int)GetSerialFloat(serialId);
	#ifdef DEBUG
					Serial.print(m3_SpeedPercent_Char);
					Serial.println(configIndex_Prime.speedPercent_Axis_M4);
	#endif // DEBUG
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
					configMove.speedPercent_Axis_Z = (int)GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Mov, configMove);
		#ifdef DEBUG
					Serial.print(z_SpeedPercent_Char);
					Serial.println(configMove.speedPercent_Axis_Z);
		#endif // DEBUG
					break;
				}
				case ID_MOVE_AXIS_X1:
				case ID_MOVE_AXIS_X2:
				{
					configMove.speedPercent_Axis_X = (int)GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Mov, configMove);
		#ifdef DEBUG
					Serial.print(x_SpeedPercent_Char);
					Serial.println(configMove.speedPercent_Axis_X);
		#endif // DEBUG
					break;
				}
				case ID_MOVE_AXIS_M3_1:
				case ID_MOVE_AXIS_M3_2:
				{
					configMove.speedPercent_Axis_M3 = (int)GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Mov, configMove);
		#ifdef DEBUG
					Serial.print(m3_SpeedPercent_Char);
					Serial.println(configMove.speedPercent_Axis_M3);
		#endif // DEBUG
					break;
				}
				case ID_MOVE_AXIS_M4_1:
				case ID_MOVE_AXIS_M4_2:
				{
					configMove.speedPercent_Axis_M4 = (int)GetSerialFloat(serialId);
					EEPROM.put(eePromAddress_Mov, configMove);
#ifdef DEBUG
					Serial.print(m3_SpeedPercent_Char);
					Serial.println(configMove.speedPercent_Axis_M4);
#endif // DEBUG
					break;
				}
			}
			break;
		}
		case PAGE_BE:
		{
			break;
		}
		case PAGE_SYNC:
		case PAGE_REC:
		case PAGE_GRK:
		case PAGE_PROGRAM:
		{
			switch (configSync.axisId)
			{
				case ID_AXIS_Z:
				{
					configSync.speedPercent_Axis_Z = (int)GetSerialFloat(serialId);
					configRec.speedPercent_Axis_Z = configSync.speedPercent_Axis_Z;
					configGreekKey.speedPercent_Axis_Z = configSync.speedPercent_Axis_Z;
					
		#ifdef DEBUG
					Serial.print(z_SpeedPercent_Char);
					Serial.println(configSync.speedPercent_Axis_Z);
		#endif // DEBUG
					break;
				}
				case ID_AXIS_X:
				{
					configSync.speedPercent_Axis_X = (int)GetSerialFloat(serialId);
					configRec.speedPercent_Axis_X = configSync.speedPercent_Axis_X;
					configGreekKey.speedPercent_Axis_X = configSync.speedPercent_Axis_X;
		#ifdef DEBUG
					Serial.print(x_SpeedPercent_Char);
					Serial.println(configSync.speedPercent_Axis_X);
		#endif // DEBUG
					break;
				}
				case ID_AXIS_3:
				{
					configSync.speedPercent_Axis_M3 = (int)GetSerialFloat(serialId);
					configRec.speedPercent_Axis_M3 = configSync.speedPercent_Axis_M3;
					configGreekKey.speedPercent_Axis_M3 = configSync.speedPercent_Axis_M3;
		#ifdef DEBUG
					Serial.print(m3_SpeedPercent_Char);
					Serial.println(configSync.speedPercent_Axis_M3);
		#endif // DEBUG
					break;
				}
				case ID_AXIS_4:
				{
					configSync.speedPercent_Axis_M4 = (int)GetSerialFloat(serialId);
					configRec.speedPercent_Axis_M4 = configSync.speedPercent_Axis_M4;
					configGreekKey.speedPercent_Axis_M4 = configSync.speedPercent_Axis_M4;
#ifdef DEBUG
					Serial.print(m3_SpeedPercent_Char);
					Serial.println(configSync.speedPercent_Axis_M4);
#endif // DEBUG
					break;
				}
			}
			EEPROM.put(eePromAddress_Sync, configSync);
			EEPROM.put(eePromAddress_Rec, configRec);
			EEPROM.put(eePromAddress_Grk, configGreekKey);
			break;
		}
		case PAGE_ROSE:
		{
			if (configRose.axisId == ID_AXIS_Z)
			{
				configRose.speedPercent_Axis_Z = (int)GetSerialFloat(serialId);
			}
			else if (configRose.axisId == ID_AXIS_X)
			{
				configRose.speedPercent_Axis_X = (int)GetSerialFloat(serialId);
			}
			else if (configRose.axisId == ID_AXIS_3)
			{
				configRose.speedPercent_Axis_M3 = (int)GetSerialFloat(serialId);
			}
			else if (configRose.axisId == ID_AXIS_4)
			{
				configRose.speedPercent_Axis_M4 = (int)GetSerialFloat(serialId);
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
	StepControl stepController;
	Stepper stepper_Z(PIN_AXIS_Z_STEP, PIN_AXIS_Z_DIR);
	Stepper stepper_X(PIN_AXIS_X_STEP, PIN_AXIS_X_DIR);
	Stepper stepper_M3(PIN_AXIS_3_STEP, PIN_AXIS_3_DIR);
	Stepper stepper_M4(PIN_AXIS_4_STEP, PIN_AXIS_4_DIR);

	int limitPin_Max = configSetup.limit_Max_Z;
	int limitPin_Min = configSetup.limit_Min_Z;
	limitTriggered = false;

	double stepsToMove = 0;
	float nextSpeed = 0;
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
	const char* pageMove_bt2_val_Char = "pageMove.bt2.val=0";
	const char* pageMove_va0_Char = "pageMove.va0.val=0";
	const char* pageMove_bt1_Char = "pageMove.bt1.val=0";
	const char* pageMove_bt3_Char = "pageMove.bt3.val=0";
	const char* pageMove_t6_Char = "pageMove.t6.txt=";

	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		digitalWrite(configSetup.cutterMotorPin, LOW);  // Enable
	}
	eStopTriggered = false;
	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);
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

			SetEnable(ID_AXIS_Z, true, true);

			MilliDelay(6);

			stepsToMove = (configMove.distance_MoveZ1 / (configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z));

			// Set speed and acceleration
			nextSpeed = configMove.speedPercent_Axis_Z * configMove.maxSpd_Axis_Z * .01;
			stepper_Z.setMaxSpeed(nextSpeed);
			stepper_Z.setAcceleration(configMove.accel_Axis_Z);
			stepper_Z.setTargetRel(stepsToMove * directionAxis);
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

			stepController.moveAsync(stepper_Z);

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

		case ID_MOVE_AXIS_M3_1:
		{
			limitPin_Max = configSetup.limit_Max_M3;
			limitPin_Min = configSetup.limit_Min_M3;
			SetEnable(ID_AXIS_3, true, true);
			if (configSetup.radialOrLinear_Axis_M3 == RADIAL_M3)
			{
				stepsToMove = DistanceToSteps_M3Radial(configMove.distance_MoveM3_1);
			}
			else
			{
				stepsToMove = DistanceToSteps_M3Linear(configMove.distance_MoveM3_1);// *(-1);
			}

			stepper_M3.setPosition(0);

			// Set speed and acceleration
			nextSpeed = configMove.speedPercent_Axis_M3 * configMove.maxSpd_Motor_3 * .01;
			stepper_M3
				.setMaxSpeed(nextSpeed)
				.setAcceleration(configMove.accel_Motor_3)
				.setTargetRel(stepsToMove * directionAxis);

	#ifdef DEBUG
			Serial.print(distancePer360_Char);
			Serial.println(configSetup.distancePerRev_AxisM3);
			Serial.print(stepsPer360_Char);
			Serial.println(configSetup.steps360_Axis_M3);
			Serial.print(microsteps_Char);
			Serial.println(configSetup.microsteps_Axis_M3);
			Serial.print(distance_Char);
			Serial.println(configMove.distance_MoveM3_1, 4);

			Serial.print(steps_Char);
			Serial.println(stepsToMove);

			Serial.print(maxSpd_Char);
			Serial.println(configMove.maxSpd_Motor_3);
			Serial.print(speedPercent_Char);
			Serial.println(configMove.speedPercent_Axis_M3);
			Serial.print(nextSpeed_Char);
			Serial.println(nextSpeed);
			Serial.print(accel_Char);
			Serial.println(configMove.accel_Motor_3);
	#endif // DEBUG

			stepController.moveAsync(stepper_M3);
			break;
		}
		case ID_MOVE_AXIS_M3_2:
		{
			limitPin_Max = configSetup.limit_Max_M3;
			limitPin_Min = configSetup.limit_Min_M3;
			SetEnable(ID_AXIS_3, true, true);
			if (configSetup.radialOrLinear_Axis_M3 == RADIAL_M3)
			{
				stepsToMove = DistanceToSteps_M3Radial(configMove.distance_MoveM3_2);
			}
			else
			{
				stepsToMove = DistanceToSteps_M3Linear(configMove.distance_MoveM3_2);// *(-1);
			}

			stepper_M3.setPosition(0);

			// Set speed and acceleration
			nextSpeed = configMove.speedPercent_Axis_M3 * configMove.maxSpd_Motor_3 * .01;
			stepper_M3
				.setMaxSpeed(nextSpeed)
				.setAcceleration(configMove.accel_Motor_3)
				.setTargetRel(stepsToMove * directionAxis);

	#ifdef DEBUG
			Serial.print(distancePer360_Char);
			Serial.println(configSetup.distancePerRev_AxisM3);
			Serial.print(stepsPer360_Char);
			Serial.println(configSetup.steps360_Axis_M3);
			Serial.print(microsteps_Char);
			Serial.println(configSetup.microsteps_Axis_M3);
			Serial.print(distance_Char);
			Serial.println(configMove.distance_MoveM3_2, 4);

			Serial.print(steps_Char);
			Serial.println(stepsToMove);

			Serial.print(maxSpd_Char);
			Serial.println(configMove.maxSpd_Motor_3);
			Serial.print(speedPercent_Char);
			Serial.println(configMove.speedPercent_Axis_M3);
			Serial.print(nextSpeed_Char);
			Serial.println(nextSpeed);
			Serial.print(accel_Char);
			Serial.println(configMove.accel_Motor_3);
	#endif // DEBUG

			stepController.moveAsync(stepper_M3);
			break;
		}

		case ID_MOVE_AXIS_M4_1:
		{
			limitPin_Max = configSetup.limit_Max_M4;
			limitPin_Min = configSetup.limit_Min_M4;
			SetEnable(ID_AXIS_4, true, true);
			if (configSetup.radialOrLinear_Axis_M4 == RADIAL_M4)
			{
				stepsToMove = DistanceToSteps_M4Radial(configMove.distance_MoveM4_1);
			}
			else
			{
				stepsToMove = DistanceToSteps_M4Linear(configMove.distance_MoveM4_1);// *(-1);
			}

			stepper_M4.setPosition(0);

			// Set speed and acceleration
			nextSpeed = configMove.speedPercent_Axis_M4 * configMove.maxSpd_Motor_4 * .01;
			stepper_M4
				.setMaxSpeed(nextSpeed)
				.setAcceleration(configMove.accel_Motor_4)
				.setTargetRel(stepsToMove * directionAxis);

#ifdef DEBUG
			Serial.print(distancePer360_Char);
			Serial.println(configSetup.distancePerRev_AxisM4);
			Serial.print(stepsPer360_Char);
			Serial.println(configSetup.steps360_Axis_M4);
			Serial.print(microsteps_Char);
			Serial.println(configSetup.microsteps_Axis_M4);
			Serial.print(distance_Char);
			Serial.println(configMove.distance_MoveM4_1, 4);

			Serial.print(steps_Char);
			Serial.println(stepsToMove);

			Serial.print(maxSpd_Char);
			Serial.println(configMove.maxSpd_Motor_4);
			Serial.print(speedPercent_Char);
			Serial.println(configMove.speedPercent_Axis_M4);
			Serial.print(nextSpeed_Char);
			Serial.println(nextSpeed);
			Serial.print(accel_Char);
			Serial.println(configMove.accel_Motor_4);
#endif // DEBUG

			stepController.moveAsync(stepper_M4);
			break;
		}
		case ID_MOVE_AXIS_M4_2:
		{
			limitPin_Max = configSetup.limit_Max_M4;
			limitPin_Min = configSetup.limit_Min_M4;
			SetEnable(ID_AXIS_4, true, true);
			if (configSetup.radialOrLinear_Axis_M4 == RADIAL_M4)
			{
				stepsToMove = DistanceToSteps_M4Radial(configMove.distance_MoveM4_2);
			}
			else
			{
				stepsToMove = DistanceToSteps_M4Linear(configMove.distance_MoveM4_2);// *(-1);
			}

			stepper_M4.setPosition(0);

			// Set speed and acceleration
			nextSpeed = configMove.speedPercent_Axis_M4 * configMove.maxSpd_Motor_4 * .01;
			stepper_M4
				.setMaxSpeed(nextSpeed)
				.setAcceleration(configMove.accel_Motor_4)
				.setTargetRel(stepsToMove * directionAxis);

#ifdef DEBUG
			Serial.print(distancePer360_Char);
			Serial.println(configSetup.distancePerRev_AxisM4);
			Serial.print(stepsPer360_Char);
			Serial.println(configSetup.steps360_Axis_M4);
			Serial.print(microsteps_Char);
			Serial.println(configSetup.microsteps_Axis_M4);
			Serial.print(distance_Char);
			Serial.println(configMove.distance_MoveM4_2, 4);

			Serial.print(steps_Char);
			Serial.println(stepsToMove);

			Serial.print(maxSpd_Char);
			Serial.println(configMove.maxSpd_Motor_4);
			Serial.print(speedPercent_Char);
			Serial.println(configMove.speedPercent_Axis_M4);
			Serial.print(nextSpeed_Char);
			Serial.println(nextSpeed);
			Serial.print(accel_Char);
			Serial.println(configMove.accel_Motor_4);
#endif // DEBUG

			stepController.moveAsync(stepper_M4);
			break;
		}
	}

	// Set limit switches HIGH
	digitalWrite(limitPin_Max, HIGH);
	digitalWrite(limitPin_Min, HIGH);

	while (stepController.isRunning())
	{
		if (limitPin_Max > 9 && limitPin_Max < 40)
		{
			if ((digitalRead(limitPin_Max) != configSetup.limit_NCorNO) && directionAxis > 0)
			{
#ifdef DEBUG
				Serial.println("                          limitPin_Max");
#endif // DEBUG
				stepController.emergencyStop();
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
				stepController.emergencyStop();
				stopSteppers = true;
				SetLimitColors(PAGE_MOVE, directionAxis);
				goto endLoop;
			}
		}

		Serial.print(" configSetup.eStop:  ");

		//if(configSetup.eStop !=0)
		if (configSetup.eStop > 9 && configSetup.eStop < 40)
		{ 
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
#ifdef DEBUG
				Serial.println("                          EStop");
#endif // DEBUG
				stepController.emergencyStop();
				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEStopColors(PAGE_MOVE);
				if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
				{
					digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
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
				currentSpeed = stepController.getCurrentSpeed();
#ifdef DEBUG
				Serial.print("currentSpeed: ");
				Serial.println(currentSpeed);
#endif // Debug
				if (currentSpeed < MIN_SPEED)
				{
					stepController.emergencyStop();
				}
				else
				{
					stepController.stop();
				}
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

#ifdef DEBUG
			Serial.print(position_Char);
			Serial.println(endPosition_Axis);
#endif // DEBUG
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
			startPositionAbs_Axis = endPosition_Axis;
	#ifdef DEBUG
			Serial.print(position_Char);
			Serial.println(endPosition_Axis);
	#endif // DEBUG

			SetEnable(ID_AXIS_X, false, true);
			break;
		}
		case ID_MOVE_AXIS_M3_1:
		case ID_MOVE_AXIS_M3_2:
		{
			endPosition_Axis = stepper_M3.getPosition();
			startPositionAbs_Axis = endPosition_Axis;
	#ifdef DEBUG
			Serial.print(position_Char);
			Serial.println(endPosition_Axis);
	#endif // DEBUG

			SetEnable(ID_AXIS_3, false, true);
			break;
		}
		case ID_MOVE_AXIS_M4_1:
		case ID_MOVE_AXIS_M4_2:
		{
			endPosition_Axis = stepper_M4.getPosition();
			startPositionAbs_Axis = endPosition_Axis;
#ifdef DEBUG
			Serial.print(position_Char);
			Serial.println(endPosition_Axis);
#endif // DEBUG

			SetEnable(ID_AXIS_4, false, true);
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

	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
	}
}

/// <summary>
/// RunOneStepper
/// </summary>
/// <comment>
/// </comment>
/// <param name="direction">Counterclockwise: -1, Clockwise: 1</param>
/// <returns></returns>
void OneRunStepper(int direction) // pageMulti
{
	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		digitalWrite(configSetup.cutterMotorPin, LOW);  // Enable
	}
	Serial.print("1.pageCallerId:  ");
	Serial.println(pageCallerId);

	eStopTriggered = false;
	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);

	RotateControl rotateController;

	// Reset end positions
	startPositionAbs_Axis = 0;
	endPosition_Spindle = 0;
	endPosition_Axis = 0;
	stepperPosition_1 = 0;
	stepperPosition_2 = 0;

#ifdef DEBUG
	Serial.println("Enter RunOneStepper.");
	Serial.print("10.axisId:  ");
	Serial.println(configMulti.axisId);
#endif // DEBUG

	stepPin = PIN_AXIS_Z_STEP;
	dirPin = PIN_AXIS_Z_DIR;

	int limitPin_Max = configSetup.limit_Max_Z;
	int limitPin_Min = configSetup.limit_Min_Z;

	int maxSpd = 0;
	int accel = 0;

	int newSpeedPercent = 0;
	float speedPercent = 0;
	float currentSpeedPercent = 0;
	int maxSpeedAndDirection = 0;
	//int32_t targetPosition = 2147000000;// *direction;

	int32_t speed_M3 = 1;
	
#ifdef DEBUG
	Serial.print("ActiveAxis");
	Serial.println(configMulti.axisId);
#endif // DEBUG
	switch (configMulti.axisId)
	{
		case ID_AXIS_Z:
		{
			stepPin = PIN_AXIS_Z_STEP;
			dirPin = PIN_AXIS_Z_DIR;
			maxSpd = configMulti.maxSpd_Axis_Z;
			accel = configMulti.accel_Axis_Z;
			speedPercent = configMulti.speedPercent_Axis_Z;
			currentSpeedPercent = (speedPercent) * .01;// *direction;
			limitPin_Max = configSetup.limit_Max_Z;
			limitPin_Min = configSetup.limit_Min_Z;

			break;
		}
		case ID_AXIS_X:
		{
			stepPin = PIN_AXIS_X_STEP;
			dirPin = PIN_AXIS_X_DIR;
			maxSpd = configMulti.maxSpd_Axis_X;
			accel = configMulti.accel_Axis_X;
			speedPercent = configMulti.speedPercent_Axis_X;
			currentSpeedPercent = speedPercent * .01;// *direction;
			limitPin_Max = configSetup.limit_Max_X;
			limitPin_Min = configSetup.limit_Min_X;
			break;
		}
		case ID_AXIS_3:
		{
			stepPin = PIN_AXIS_3_STEP;
			dirPin = PIN_AXIS_3_DIR;
			maxSpd = configMulti.maxSpd_Axis_M3;
			accel = configMulti.accel_Axis_M3;
			speedPercent = configMulti.speedPercent_Axis_M3;
			currentSpeedPercent = speedPercent * .01;// *direction;
			Serial.print("configSetup.radialOrLinear_Axis_M3:  ");
			Serial.println(configSetup.radialOrLinear_Axis_M3);
			//if (configSetup.radialOrLinear_Axis_M3 == 1) // Linear
			//{
			//	maxSpd = maxSpd * (-1);
			//}
			limitPin_Max = configSetup.limit_Max_M3;
			limitPin_Min = configSetup.limit_Min_M3;
			break;
		}
		case ID_AXIS_4:
		{
			stepPin = PIN_AXIS_4_STEP;
			dirPin = PIN_AXIS_4_DIR;
			maxSpd = configMulti.maxSpd_Axis_M4;
			accel = configMulti.accel_Axis_M4;
			speedPercent = configMulti.speedPercent_Axis_M4;
			currentSpeedPercent = speedPercent * .01;// *direction;
			Serial.print("configSetup.radialOrLinear_Axis_M4:  ");
			Serial.println(configSetup.radialOrLinear_Axis_M4);
			Serial.print("1-maxSpd:  ");
			Serial.println(maxSpd);
			//if (configSetup.radialOrLinear_Axis_M4 == 1) // Linear
			//{
			//	maxSpd = maxSpd * (-1);
			//}
			Serial.print("2-maxSpd:  ");
			Serial.println(maxSpd);
			limitPin_Max = configSetup.limit_Max_M4;
			limitPin_Min = configSetup.limit_Min_M4;
			break;
		}
		case ID_SPINDLE:
		{
			stepPin = PIN_SPINDLE_STEP;
			dirPin = PIN_SPINDLE_DIR;
			maxSpd = configMulti.maxSpd_Spindle;
			accel = configMulti.accel_Spindle;
			speedPercent = configMulti.speedPercent_Spindle;
			currentSpeedPercent = speedPercent * .01;// *direction;

			break;
		}
	}

	stepperId_1 = configMulti.axisId;
	stepper1_step = stepPin;
	stepper1_dir = dirPin;


	// Set limit switches HIGH
	digitalWrite(limitPin_Max, HIGH);
	digitalWrite(limitPin_Min, HIGH);

	Serial.print("maxSpd:  ");
	Serial.println(maxSpd);
	maxSpeedAndDirection = maxSpd * direction;
	Serial.print("maxSpd * direction: ");
	Serial.println(maxSpeedAndDirection);


	Stepper stepper_One(stepPin, dirPin);
	stepper_One.setPosition(0);
	MilliDelay(5);
	stepper_One
		.setMaxSpeed(maxSpd * direction)
		.setAcceleration(accel);

	SetEnable(configMulti.axisId, true, true);
	////////////////////////////////////////////////
	if (configMulti.axisId == ID_SPINDLE)
	{
		Serial.print(" configMain.synchro_M3_Spindle:  ");
		Serial.println(configMain.synchro_M3_Spindle);
		if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
		{
			stepperId_2 = ID_AXIS_3;
			stepper2_step = PIN_AXIS_3_STEP;
			stepper2_dir = PIN_AXIS_3_DIR;
			Serial.print("maxSpd * direction: ");
			Serial.println(maxSpd * direction);
			speed_M3 = round((maxSpd * direction) * configMain.synchro_M3_Percentage * .01); // * synchro_Ratio

			Serial.print("maxSpd * direction: ");
			Serial.println(maxSpd * direction);
			Serial.print("speed_M3:  ");
			Serial.println(speed_M3);
			Serial.print("configMain.synchro_M3_Percentage:  ");
			Serial.println(configMain.synchro_M3_Percentage);

			SetEnable(ID_AXIS_3, true, true);
			stepperAxis_M3.setPosition(0);
			stepperAxis_M3
				.setMaxSpeed(speed_M3)
				.setAcceleration(configRec.accel_Spindle);
			MilliDelay(5);

			rotateController.rotateAsync(stepper_One, stepperAxis_M3);
		}
		else
		{
			rotateController.rotateAsync(stepper_One);
			MilliDelay(5);
			rotateController.overrideSpeed(currentSpeedPercent);
		}
	}
	else
	{
		rotateController.rotateAsync(stepper_One);
		MilliDelay(5);
		rotateController.overrideSpeed(currentSpeedPercent);
	}
	//////////////////////////////////////////


	while (rotateController.isRunning())
	{
		// Check for Cancel code  
		if (SerialAvailable() > 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
			    case 83: // - S
				{
					rotateController.overrideSpeed(0);
					MilliDelay(5);
					rotateController.stop();
					MilliDelay(100);
					goto endLoop;
					break;
				}
				case 110:  // Spindle speed
				{
					newSpeedPercent = (int)GetSerialFloat(serialId);
					configMulti.speedPercent_Spindle = newSpeedPercent;
					currentSpeedPercent = (float)(newSpeedPercent * .01 * direction);
					Serial.print("currentSpeedPercent:  ");
					Serial.println(currentSpeedPercent);

					EEPROM.put(eePromAddress_Multi, configMulti);

					rotateController.overrideSpeed(currentSpeedPercent); // set new speed
					break;
				}
				case 111:  // Axis Speed
				{
					newSpeedPercent = (int)GetSerialFloat(serialId);
					switch (configMulti.axisId)
					{
						case ID_AXIS_Z:
						{
							configMulti.speedPercent_Axis_Z = newSpeedPercent;
							currentSpeedPercent = float((newSpeedPercent) * .01 * direction);
							break;
						}
						case ID_AXIS_X:
						{
							configMulti.speedPercent_Axis_X = newSpeedPercent;
							currentSpeedPercent = (float)(newSpeedPercent * .01 * direction);
							break;
						}
						case ID_AXIS_3:
						{
							configMulti.speedPercent_Axis_M3 = newSpeedPercent;
							currentSpeedPercent = (float)(newSpeedPercent * .01 * direction);
							break;
						}
						case ID_AXIS_4:
						{
							configMulti.speedPercent_Axis_M4 = newSpeedPercent;
							currentSpeedPercent = (float)(newSpeedPercent * .01 * direction);
							break;
						}
					}

					EEPROM.put(eePromAddress_Multi, configMulti);

					rotateController.overrideSpeed(currentSpeedPercent); // set new speed
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
				rotateController.overrideSpeed(0);
				rotateController.emergencyStop();
				SetLimitColors(pageCallerId, DIR_CW);
				goto endLoop;
			}
		}
		if (limitPin_Min > 9 && limitPin_Min < 40)
		{
			if ((digitalRead(limitPin_Min) != configSetup.limit_NCorNO) && direction == DIR_CCW)
			{
				rotateController.overrideSpeed(0);
				rotateController.emergencyStop();
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
				Serial.println("                          EStop ");

				rotateController.emergencyStop();
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEnable(ID_AXIS_4, false, false);

				SetEStopColors(pageCallerId);
				if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
				{
					digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
				}
				return;
			}
		}
#endif

		MilliDelay(10);
	}

endLoop:

	
	// Set Return steps values
	if (configMulti.axisId == ID_SPINDLE)
	{
		endPosition_Spindle = stepper_One.getPosition();
		endPosition_Axis = 0;
		returnSteps_Spindle = endPosition_Spindle;
		stepperPosition_1 = endPosition_Spindle * (-1);
#ifdef SHOW_POSITION 
		degrees_Spindle = StepsToDegrees(returnSteps_Spindle, ID_SPINDLE);
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
		stepperPosition_1 = endPosition_Axis * (-1);
#ifdef SHOW_POSITION 
		distance_Axis = StepsToDistance_Axis(endPosition_Axis, configMulti.axisId);
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

	Serial.print("11.axisId:  ");
	Serial.println(configMulti.axisId);
	// Reset enable/disable status on Nextion
	SetEnable(configMulti.axisId, false, true);

#ifdef DEBUG
	Serial.print("keepSteppersEnabled:  ");
	Serial.println(configSetup.keepSteppersEnabled);
	Serial.print("axisId:  ");
	Serial.println(configMulti.axisId);
	Serial.print("pageCallerId:  ");
	Serial.println(pageCallerId);
#endif // DEBUG
	switch (pageCallerId)
	{
		case PAGE_ONE:
		{
			checkedCount = 1;
#ifdef DEBUG
			Serial.print("ONE--configMain.synchro_M3_Spindle:  ");
			Serial.println(configMain.synchro_M3_Spindle);
#endif // DEBUG
			if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
			{
				SetEnable(ID_AXIS_3, false, true);
				stepperPosition_2 = stepperAxis_M3.getPosition() * (-1);
				checkedCount = 2;
			}
#ifdef DEBUG



			Serial.print("ONE--configMulti.axisId:  ");
			Serial.println(configMulti.axisId);
			Serial.print("stepperPosition_1: ");
			Serial.println(stepperPosition_1);
			Serial.print("stepperPosition_2: ");
			Serial.println(stepperPosition_2);

			Serial.print("configSetup.keepSteppersEnabled: ");
			Serial.println(configSetup.keepSteppersEnabled);
#endif // DEBUG
			switch (configMulti.axisId)
			{
				case ID_AXIS_Z:
				{
					// 0 = Change to disabled, 1 = keep enabled
					if (configSetup.keepSteppersEnabled != 1)
					{
						Serial.print("2.configSetup.keepSteppersEnabled: ");
						Serial.println(configSetup.keepSteppersEnabled);
						SerialPrint("pageOne.bt301.val=0"); // 1 = disabled
						SerialPrint("\xFF\xFF\xFF");
						SerialPrint("pageOne.bt301.val=0"); // 1 = disabled
						SerialPrint("\xFF\xFF\xFF");
						SerialPrint("pageOne.bt301.txt="); // 1 = disabled
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
						SerialPrint("pageOne.bt302.val=0"); // 1 = disabled
						SerialPrint("\xFF\xFF\xFF");
						SerialPrint("pageOne.bt302.txt="); // 1 = disabled
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
						SerialPrint("pageOne.bt303.val=0"); // 1 = disabled
						SerialPrint("\xFF\xFF\xFF");
						SerialPrint("pageOne.bt303.txt="); // 1 = disabled
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
						SerialPrint("pageOne.bt304.val=0"); // 1 = disabled
						SerialPrint("\xFF\xFF\xFF");
						SerialPrint("pageOne.bt304.txt="); // 1 = disabled
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
						SerialPrint("pageOne.bt300.val=0"); // 1 = disabled
						SerialPrint("\xFF\xFF\xFF");
						SerialPrint("pageOne.bt300.txt="); // 1 = disabled
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
			switch (configMulti.axisId)
			{
			case ID_AXIS_Z:
			{
				// 0 = Change to disabled, 1 = keep enabled
				if (configSetup.keepSteppersEnabled != 1)
				{
					SerialPrint("pageMulti.bt1301.val=1"); // 1 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMulti.bt1301.txt="); // 1 = disabled
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
					SerialPrint("pageMulti.bt1302.val=0"); // 1 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMulti.bt1302.txt="); // 1 = disabled
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
					SerialPrint("pageMulti.bt1303.val=0"); // 1 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMulti.bt1303.txt="); // 1 = disabled
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
					SerialPrint("pageMulti.bt1304.val=0"); // 1 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMulti.bt1304.txt="); // 1 = disabled
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
					SerialPrint("pageMulti.bt1300.val=0"); // 1 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageMulti.bt1300.txt="); // 1 = disabled
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
	Serial.print("stepperPosition_1:  ");
	Serial.println(stepperPosition_1);
	Serial.print("stepperPosition_2:  ");
	Serial.println(stepperPosition_2);
#endif // DEBUG


	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
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
	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		digitalWrite(configSetup.cutterMotorPin, LOW);  // Enable
	}
	RotateControl rotateController_MainSpindle;
	RotateControl rotateController_MainAxis;
	Stepper stepper_Spindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
	
	stepPin = PIN_AXIS_Z_STEP;
	dirPin = PIN_AXIS_Z_DIR;
	int limitPin_Max = configSetup.limit_Max_Z;
	int limitPin_Min = configSetup.limit_Min_Z;
	int maxSpd_Spindle = 0;
	int accel_Spindle = 0;
	float speedPercentAxis = 0;
	int32_t maxSpeed_Axis = 0;
	int32_t accel_Axis = 0;
	float speedPercentSpindle = 0;
	float currentSpeedPercentSpindle = 0;
	//int32_t targetPosition_Axis = 2147000000;
	//int32_t targetPosition_Spindle = 2147000000;

	// initialCaller: Spindle or axis
	bool stepper_Axis_Go = false;
	bool stepper_Spindle_Go = false;
	bool keepAxisEnabledState = true;  

	const char * initialCaller_Char = "Initial Caller: ";
	const char * spindle_Char = "Spindle-";
	const char * maxSpd_Char = "MaxSpeed: ";
	const char * accel_Char = "Accel: ";
//	const char * targetPosition_Char = "Target Position: ";
	const char * stop_Char = "Stop: ";
	const char * stopped_Char = "Stopped";
	const char * go_Char = "Go: ";
	const char * axis_Char = "Axis-";

	eStopTriggered = false;
	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);

#ifdef DEBUG
	Serial.print(initialCaller_Char);
	Serial.println(initialCaller);
#endif // DEBUG
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

	Stepper stepper_Z(PIN_AXIS_Z_STEP, PIN_AXIS_Z_DIR);
	Stepper stepper_X(PIN_AXIS_X_STEP, PIN_AXIS_X_DIR);
	Stepper stepper_M3(PIN_AXIS_3_STEP, PIN_AXIS_3_DIR);
	Stepper stepper_M4(PIN_AXIS_4_STEP, PIN_AXIS_4_DIR);

	stepper_Z.setPosition(0);
	stepper_X.setPosition(0);
	stepper_M3.setPosition(0);
	stepper_M4.setPosition(0);
	stepper_Spindle.setPosition(0);

	switch (configMain.axisId)
	{
		case ID_AXIS_Z:
		{
			speedPercentAxis = configMain.speedPercent_Axis_Z;
			stepPin = PIN_AXIS_Z_STEP;
			dirPin = PIN_AXIS_Z_DIR;

			limitPin_Max = configSetup.limit_Max_Z;
			limitPin_Min = configSetup.limit_Min_Z;
			maxSpeed_Axis = configMain.maxSpd_Axis_Z * direction_Axis;
			accel_Axis = configMain.accel_Axis_Z;
			stepper_Z.setPosition(0);
			stepper_Z
				.setMaxSpeed(maxSpeed_Axis)
				.setAcceleration(accel_Axis);

			MilliDelay(5);

			break;
		}
		case ID_AXIS_X:
		{

			speedPercentAxis = configMain.speedPercent_Axis_X;
			stepPin = PIN_AXIS_X_STEP;
			dirPin = PIN_AXIS_X_DIR;

			limitPin_Max = configSetup.limit_Max_X;
			limitPin_Min = configSetup.limit_Min_X;
			maxSpeed_Axis = configMain.maxSpd_Axis_X * direction_Axis;
			accel_Axis = configMain.accel_Axis_X;
			stepper_X.setPosition(0);
			stepper_X
				.setMaxSpeed(maxSpeed_Axis)
				.setAcceleration(accel_Axis);

			MilliDelay(5);

			break;
		}
		case ID_AXIS_3:
		{
			speedPercentAxis = configMain.speedPercent_Axis_M3;
			stepPin = PIN_AXIS_3_STEP;
			dirPin = PIN_AXIS_3_DIR;

			limitPin_Max = configSetup.limit_Max_M3;
			limitPin_Min = configSetup.limit_Min_M3;
			maxSpeed_Axis = configMain.maxSpd_Axis_M3 * direction_Axis;

			//if (configSetup.radialOrLinear_Axis_M3 == 1) // Linear
			//{
			//	maxSpeed_Axis = maxSpeed_Axis * (-1);
			//}

			accel_Axis = configMain.accel_Axis_M3;
			stepper_M3.setPosition(0);
			stepper_M3
				.setMaxSpeed(maxSpeed_Axis)
				.setAcceleration(accel_Axis);
		
			MilliDelay(5);

			break;
		}
		case ID_AXIS_4:
		{
			speedPercentAxis = configMain.speedPercent_Axis_M4;
			stepPin = PIN_AXIS_4_STEP;
			dirPin = PIN_AXIS_4_DIR;

			limitPin_Max = configSetup.limit_Max_M4;
			limitPin_Min = configSetup.limit_Min_M4;
			maxSpeed_Axis = configMain.maxSpd_Axis_M4 * direction_Axis;

			//if (configSetup.radialOrLinear_Axis_M4 == 1) // Linear
			//{
			//	maxSpeed_Axis = maxSpeed_Axis * (-1);
			//}

			accel_Axis = configMain.accel_Axis_M4;
			stepper_M4.setPosition(0);
			stepper_M4
				.setMaxSpeed(maxSpeed_Axis)
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

	speedPercentSpindle = configMain.speedPercent_Spindle;
	maxSpd_Spindle = configMain.maxSpd_Spindle;
	accel_Spindle = configMain.accel_Spindle;
	currentSpeedPercentSpindle = speedPercentSpindle * .01;

	MilliDelay(5);

	Serial.print("maxSpd_Spindle * direction_Spindle:  ");
	Serial.println(maxSpd_Spindle * direction_Spindle);
	stepper_Spindle
		.setMaxSpeed(maxSpd_Spindle * direction_Spindle)
		.setAcceleration(accel_Spindle);

	checkedCount = 2;
	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		checkedCount = 3;
		stepperId_3 = ID_AXIS_3;
		stepper3_step = PIN_AXIS_3_STEP;
		stepper3_dir = PIN_AXIS_3_DIR;
#ifdef DEBUG
		Serial.print("maxSpd_Spindle * direction_Spindle * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01):  ");
		Serial.println(maxSpd_Spindle * direction_Spindle * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01));
		//Serial.print("targetSteps:  ");
		//Serial.println(targetPosition_Spindle);

		Serial.print("synchro_Ratio:  ");
		Serial.println(synchro_Ratio);

		Serial.print("configMain.synchro_M3_Percentage * 0.01:  ");
		Serial.println(configMain.synchro_M3_Percentage * 0.01);
#endif // DEBUG

		stepper_M3.setPosition(0);
		stepper_M3
			.setMaxSpeed(maxSpd_Spindle* direction_Spindle* synchro_Ratio* (configMain.synchro_M3_Percentage * 0.01))
			.setAcceleration(accel_Spindle);
		MilliDelay(5);
	}

#ifdef DEBUG
	Serial.print(spindle_Char);
	Serial.print(maxSpd_Char);
	Serial.println(maxSpd_Spindle);
	Serial.print(spindle_Char);
	Serial.print(accel_Char);
	Serial.println(accel_Spindle);
	Serial.print(spindle_Char);
	//Serial.print(targetPosition_Char);
	//Serial.println(targetPosition_Spindle);
#endif // DEBUG

	if (stepper_Axis_Go)
	{
		keepAxisEnabledState = false;
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
			case ID_AXIS_3:
			{
				SetEnable(ID_AXIS_3, true, true);
				rotateController_MainAxis.rotateAsync(stepper_M3);
				rotateController_MainAxis.overrideSpeed(0);
				MilliDelay(10);
				rotateController_MainAxis.overrideSpeed(configMain.speedPercent_Axis_M3 * .01);
				break;
			}
			case ID_AXIS_4:
			{
				SetEnable(ID_AXIS_4, true, true);
				rotateController_MainAxis.rotateAsync(stepper_M4);
				rotateController_MainAxis.overrideSpeed(0);
				MilliDelay(10);
				rotateController_MainAxis.overrideSpeed(configMain.speedPercent_Axis_M4 * .01);
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

		if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
		{
			SetEnable(ID_AXIS_3, true, true);
			rotateController_MainSpindle.rotateAsync(stepper_Spindle, stepper_M3);
		}
		else
		{ 
			rotateController_MainSpindle.rotateAsync(stepper_Spindle);
		}

		rotateController_MainSpindle.overrideSpeed(0);
		MilliDelay(10);
		rotateController_MainSpindle.overrideSpeed(currentSpeedPercentSpindle);
#ifdef DEBUG
		Serial.print(spindle_Char);
		Serial.print(go_Char);
		Serial.println(stepper_Spindle_Go);
#endif // DEBUG
	}

	int dirPin_1 = digitalReadFast(PIN_AXIS_3_DIR);
	Serial.print("dirPin-M3: ");
	Serial.println(dirPin_1);

	dirPin_1 = digitalReadFast(PIN_SPINDLE_DIR);
	Serial.print("dirPin-Spindle: ");
	Serial.println(dirPin_1);
	while (stepper_Axis_Go || stepper_Spindle_Go)
	{
#ifdef ESTOP_ENABLED
		//if (configSetup.eStop != 0)
		if (configSetup.eStop > 9 && configSetup.eStop < 40)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
#ifdef DEBUG
				Serial.println("                          EStop ");
#endif // DEBUG
				rotateController_MainSpindle.emergencyStop();
				rotateController_MainAxis.emergencyStop();
				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEnable(ID_AXIS_4, false, false);
				// Change Btn color on Nextion
				SetEStopColors(PAGE_MAIN);
				if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
				{
					digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
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

					if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
					{
						SetEnable(ID_AXIS_3, false, true);
					}

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
						//targetPosition_Axis = 2147000000;
						speedPercentAxis = (float)((configMain.speedPercent_Axis_Z) * .01);
						stepper_Z.setMaxSpeed(configMain.maxSpd_Axis_Z* direction_Axis);

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
						//targetPosition_Axis = -2147000000;
						speedPercentAxis = (float)((configMain.speedPercent_Axis_Z) * .01);
						stepper_Z.setMaxSpeed(configMain.maxSpd_Axis_Z * direction_Axis);
	
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
#ifdef DEBUG
					Serial.print("configMain.axisId:  ");
					Serial.println(configMain.axisId);
#endif // DEBUG
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
						//targetPosition_Spindle = 2147000000;
						speedPercentSpindle = (float)(configMain.speedPercent_Spindle * .01);
						stepper_Spindle.setMaxSpeed(maxSpd_Spindle);
						stepper_Spindle_Go = true;
						SetEnable(ID_SPINDLE, true, true);

						if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
						{
#ifdef DEBUG
							Serial.print("maxSpd_Spindle * direction_Spindle * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01):  ");
							Serial.println(maxSpd_Spindle * direction_Spindle * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01));
							//Serial.print("targetSteps:  ");
							//Serial.println(targetPosition_Spindle);

							Serial.print("synchro_Ratio:  ");
							Serial.println(synchro_Ratio);

							Serial.print("configMain.synchro_M3_Percentage * 0.01:  ");
							Serial.println(configMain.synchro_M3_Percentage * 0.01);
#endif // DEBUG
							stepper_M3.setPosition(0);
							stepper_M3
								.setMaxSpeed(maxSpd_Spindle* direction_Spindle* synchro_Ratio* (configMain.synchro_M3_Percentage * 0.01))
								.setAcceleration(accel_Spindle);

							MilliDelay(5);
							SetEnable(ID_AXIS_3, true, true);
							rotateController_MainSpindle.rotateAsync(stepper_Spindle, stepper_M3);
						}
						else
						{ 
							rotateController_MainSpindle.rotateAsync(stepper_Spindle);
						}
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
						//targetPosition_Spindle = 2147000000;
						speedPercentSpindle = (float)(configMain.speedPercent_Spindle * .01);
						stepper_Spindle.setMaxSpeed(maxSpd_Spindle* DIR_CCW);
						stepper_Spindle_Go = true;

						SetEnable(ID_SPINDLE, true, true);

						if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
						{
#ifdef DEBUG
							Serial.print("maxSpd_Spindle * direction_Spindle * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01):  ");
							Serial.println(maxSpd_Spindle* direction_Spindle* synchro_Ratio* (configMain.synchro_M3_Percentage * 0.01));
							//Serial.print("targetSteps:  ");
							//Serial.println(targetPosition_Spindle);

							Serial.print("synchro_Ratio:  ");
							Serial.println(synchro_Ratio);

							Serial.print("configMain.synchro_M3_Percentage * 0.01:  ");
							Serial.println(configMain.synchro_M3_Percentage * 0.01);
#endif // DEBUG
							stepper_M3.setPosition(0);
							stepper_M3
								.setMaxSpeed(maxSpd_Spindle* direction_Spindle* synchro_Ratio* (configMain.synchro_M3_Percentage * 0.01)* DIR_CCW)
								.setAcceleration(accel_Spindle);

							MilliDelay(5);
							SetEnable(ID_AXIS_3, true, true);
							rotateController_MainSpindle.rotateAsync(stepper_Spindle, stepper_M3);
						}
						else
						{
							rotateController_MainSpindle.rotateAsync(stepper_Spindle);
						}

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
						case ID_AXIS_3:
						{
							configMain.speedPercent_Axis_M3 = newSpeedPercent;
							speedPercentAxis = (float)((newSpeedPercent) * .01);
							break;
						}
						case ID_AXIS_4:
						{
							configMain.speedPercent_Axis_M4 = newSpeedPercent;
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
						//targetPosition_Axis = -2147000000;
						speedPercentAxis = (float)(configMain.speedPercent_Axis_X * .01);
						stepper_X.setMaxSpeed(configMain.maxSpd_Axis_X* direction_Axis);

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
						//targetPosition_Axis = 2147000000;
						speedPercentAxis = (float)(configMain.speedPercent_Axis_X * .01);
						stepper_X.setMaxSpeed(configMain.maxSpd_Axis_X* direction_Axis);

						SetEnable(ID_AXIS_X, true, true);
						rotateController_MainAxis.rotateAsync(stepper_X);
						rotateController_MainAxis.overrideSpeed(0);
						MilliDelay(5);
						rotateController_MainAxis.overrideSpeed(speedPercentAxis); 
					}

					break;
				}
				case 171: // « - M3 Axis CCW
				{
					if (!rotateController_MainAxis.isRunning())
					{
						stepper_Axis_Go = true;
						direction_Axis = DIR_CCW;

						if (configSetup.radialOrLinear_Axis_M3 == 1) // Linear
						{
							direction_Axis = DIR_CW;
						}

						//targetPosition_Axis = -2147000000;
						Serial.print("configMain.maxSpd_Axis_M3* direction_Axis:  ");
						Serial.println(configMain.maxSpd_Axis_M3* direction_Axis);
						speedPercentAxis = (float)((configMain.speedPercent_Axis_M3) * .01);
						stepper_M3.setMaxSpeed(configMain.maxSpd_Axis_M3* direction_Axis);

						SetEnable(ID_AXIS_3, true, true);
						rotateController_MainAxis.rotateAsync(stepper_M3);
						rotateController_MainAxis.overrideSpeed(0);
						MilliDelay(5);
						rotateController_MainAxis.overrideSpeed(speedPercentAxis);
					}
					break;
				}
				case 172: // ¬ - M3 Axis CW
				{
					if (!rotateController_MainAxis.isRunning())
					{
						stepper_Axis_Go = true;
						direction_Axis = DIR_CW;

						if (configSetup.radialOrLinear_Axis_M3 == 1) // Linear
						{
							direction_Axis = DIR_CCW;
						}

						//targetPosition_Axis = 2147000000;
						Serial.print("configMain.maxSpd_Axis_M3* direction_Axis:  ");
						Serial.println(configMain.maxSpd_Axis_M3 * direction_Axis);
						speedPercentAxis = (float)((configMain.speedPercent_Axis_M3) * .01);
						stepper_M3.setMaxSpeed(configMain.maxSpd_Axis_M3* direction_Axis);

						SetEnable(ID_AXIS_3, true, true);
						rotateController_MainAxis.rotateAsync(stepper_M3);
						rotateController_MainAxis.overrideSpeed(0);
						MilliDelay(5);
						rotateController_MainAxis.overrideSpeed(speedPercentAxis);
					}
					break;
				}

				case 209: // « - M4 Axis CCW
				{
					if (!rotateController_MainAxis.isRunning())
					{
						stepper_Axis_Go = true;
						direction_Axis = DIR_CCW;

						if (configSetup.radialOrLinear_Axis_M4 == 1) // Linear
						{
							direction_Axis = DIR_CW;
						}
						//targetPosition_Axis = -2147000000;

						speedPercentAxis = (float)((configMain.speedPercent_Axis_M4) * .01);
						stepper_M4.setMaxSpeed(configMain.maxSpd_Axis_M4* direction_Axis);

						SetEnable(ID_AXIS_4, true, true);
						rotateController_MainAxis.rotateAsync(stepper_M4);
						rotateController_MainAxis.overrideSpeed(0);
						MilliDelay(5);
						rotateController_MainAxis.overrideSpeed(speedPercentAxis);
					}
					break;
				}
				case 210: // ¬ - M4 Axis CW
				{
					if (!rotateController_MainAxis.isRunning())
					{
						stepper_Axis_Go = true;
						direction_Axis = DIR_CW;

						if (configSetup.radialOrLinear_Axis_M4 == 1) // Linear
						{
							direction_Axis = DIR_CCW;
						}

						//targetPosition_Axis = 2147000000;
						speedPercentAxis = (float)((configMain.speedPercent_Axis_M4) * .01);
						stepper_M4.setMaxSpeed(configMain.maxSpd_Axis_M4* direction_Axis);

						SetEnable(ID_AXIS_4, true, true);
						rotateController_MainAxis.rotateAsync(stepper_M4);
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
			if (limitPin_Max > 9 && limitPin_Max < 40)
			{
				if ((digitalRead(limitPin_Max) != configSetup.limit_NCorNO) && direction_Axis == DIR_CW)
				{
#ifdef DEBUG
					Serial.print("Limit_Max - CW - Tripped:  ");
					Serial.println(configSetup.limit_NCorNO);
#endif // DEBUG
					rotateController_MainAxis.overrideSpeed(0);
					rotateController_MainAxis.emergencyStop();
					if (configSetup.limit_StopSpindle)
					{
						// Stop the spindle
						rotateController_MainSpindle.overrideSpeed(0);
						MilliDelay(5);
						rotateController_MainSpindle.emergencyStop();

						if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
						{
							SetEnable(ID_AXIS_3, false, true);
						}
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
			}
			if (limitPin_Min > 9 && limitPin_Min < 40)
			{
				if ((digitalRead(limitPin_Min) != configSetup.limit_NCorNO) && direction_Axis == DIR_CCW)
				{
#ifdef DEBUG
					Serial.println("Limit_Min - CCW - Tripped:  ");
					Serial.println(configSetup.limit_NCorNO);
#endif // DEBUG
					rotateController_MainAxis.overrideSpeed(0);
					rotateController_MainAxis.emergencyStop();
					stepper_Axis_Go = false;

					if (configSetup.limit_StopSpindle)
					{
						// Stop the spindle
						rotateController_MainSpindle.overrideSpeed(0);
						MilliDelay(5);
						rotateController_MainSpindle.emergencyStop();

						if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
						{
							SetEnable(ID_AXIS_3, false, true);
						}
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

	int32_t endPosition_M3 = stepper_M3.getPosition();
	Serial.print("endPosition_M3: ");
	Serial.println(endPosition_M3);
	if(!keepAxisEnabledState)
	{ 
		switch (configMain.axisId)
		{
			case ID_AXIS_Z:
			{
				endPosition_Axis = stepper_Z.getPosition();
				//stepper_Z.setPosition(0);
				SetEnable(ID_AXIS_Z, false, true);
				break;
			}
			case ID_AXIS_X:
			{
				endPosition_Axis = stepper_X.getPosition();
				//stepper_X.setPosition(0);
				SetEnable(ID_AXIS_X, false, true);
				break;
			}
			case ID_AXIS_3:
			{
				endPosition_Axis = stepper_M3.getPosition();
				Serial.print("2.endPosition-M3: ");
				Serial.println(endPosition_Axis);
				//stepper_M3.setPosition(0);
				SetEnable(ID_AXIS_3, false, true);
				break;
			}
			case ID_AXIS_4:
			{
				endPosition_Axis = stepper_M4.getPosition();
				//stepper_M4.setPosition(0);
				SetEnable(ID_AXIS_4, false, true);
				break;
			}
		}
	}
	// ToDo: Use Multi Return method
	//returnSteps_Spindle = stepper_Spindle.getPosition();

	//endPosition_Spindle = stepper_Spindle.getPosition();
	//stepperPosition_1 = endPosition_Spindle * (-1);
	//stepperPosition_2 = endPosition_Axis * (-1);

	spindle_Position = stepper_Spindle.getPosition();// *(-1);
	Serial.print("spindle_Position: ");
	Serial.println(spindle_Position);

	axis_Position = endPosition_Axis;// *(-1);
	Serial.print("axis_Position: ");
	Serial.println(axis_Position);

	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		SetEnable(ID_AXIS_3, false, true);
		m3_Position = stepper_M3.getPosition();// * (-1);
		//stepperPosition_3 = stepper_M3.getPosition() * (-1);
		Serial.print("m3_Position: ");
		Serial.println(m3_Position);
	}

#ifdef SHOW_POSITION 
	degrees_Spindle = StepsToDegrees(returnSteps_Spindle, ID_SPINDLE);
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

//	stepper_Spindle.setPosition(0);
//	startPositionAbs_Axis = 0;

	Serial.print("stepper_Spindle.getPosition() : ");
	Serial.println(stepper_Spindle.getPosition());

	Serial.print("stepper_Z.getPosition() : ");
	Serial.println(stepper_Z.getPosition());

	Serial.print("stepper_X.getPosition() : ");
	Serial.println(stepper_X.getPosition());

	Serial.print("stepper_M3.getPosition() : ");
	Serial.println(stepper_M3.getPosition());

	Serial.print("stepper_M4.getPosition(): ");
	Serial.println(stepper_M4.getPosition());
	


	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
	}
}


void Return_Main()
{

	switch (returnType)
	{
		case RETURN_SPINDLE: // Spindle only
		{
			
			Serial.print("1-Return-spindle_Position: ");
			Serial.println(spindle_Position);
			checkedCount = 1;
			stepperId_1 = ID_SPINDLE;
			stepper1_step = PIN_SPINDLE_STEP;
			stepper1_dir = PIN_SPINDLE_DIR;
			stepperPosition_1 = spindle_Position;// *(-1);
			break;
		}
		case RETURN_SPINDLE_M3: // Spindle and M3-Synchro
		{
			Serial.print("2-Return-spindle_Position: ");
			Serial.println(spindle_Position);


			checkedCount = 2;
			stepperId_1 = ID_SPINDLE;
			stepper1_step = PIN_SPINDLE_STEP;
			stepper1_dir = PIN_SPINDLE_DIR;
			stepperPosition_1 = spindle_Position;// *(-1);

			Serial.print("2-Return-stepperPosition_1: ");
			Serial.println(stepperPosition_1);

			Serial.print("2-Return-m3_Position: ");
			Serial.println(m3_Position);

			stepperId_2 = ID_AXIS_3;
			stepper2_step = PIN_AXIS_3_STEP;
			stepper2_dir = PIN_AXIS_3_DIR;

			stepperPosition_2 = m3_Position;// *(-1);
			//if (stepperPosition_2 < 0)
			//{
			//	stepperPosition_2 = stepperPosition_2;// *(-1);
			//}
			Serial.print("2-Return-stepperPosition_2: ");
			Serial.println(stepperPosition_2);
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
			Serial.print("3-Return-axis_Position: ");
			Serial.println(axis_Position);
			checkedCount = 1;

			stepperPosition_1 = axis_Position;// *(-1);
			//if (stepperPosition_1 > 0)
			//{
			//	stepperPosition_1 = stepperPosition_1 * (-1);
			//}
			Serial.print("3-Return-stepperPosition_1: ");
			Serial.println(stepperPosition_1);
			break;
		}
	}

	ReturnToStartPosition_MultiMain();
}

///<summary>
/// Multi_SimultaneousTargets
/// </summary>
/// <comment>
/// Run multiple steppers simultaneously
/// </comment>
void Multi_SimultaneousTargets()
{
	Serial.println("Enter Multi_SimultaneousTargets");
	eStopTriggered = false;
	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		digitalWrite(configSetup.cutterMotorPin, LOW);  // Enable
	}

	StepControl stepController;

	// Reset global variables
	stepperId_1 = 40;
	stepperId_2 = 40;
	stepperId_3 = 40;
	stepperId_4 = 40;
	stepperId_5 = 40;

	stepper1_step = 48;
	stepper1_dir = 49;

	stepper2_step = 50;
	stepper2_dir = 51;

	stepper3_step = 52;
	stepper3_dir = 53;

	stepper4_step = 54;
	stepper4_dir = 55;

	stepper5_step = 56;
	stepper5_dir = 57;
	// End global variables

	int limitPin_Max_1 = configSetup.limit_Max_Z;
	int limitPin_Min_1 = configSetup.limit_Min_Z;
	int limitPin_Max_2 = configSetup.limit_Max_Z;
	int limitPin_Min_2 = configSetup.limit_Min_Z;
	int limitPin_Max_3 = configSetup.limit_Max_Z;
	int limitPin_Min_3 = configSetup.limit_Min_Z;
	int limitPin_Max_4 = configSetup.limit_Max_Z;
	int limitPin_Min_4 = configSetup.limit_Min_Z;

	limitTriggered = false;

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

	stepperPosition_1 = 0;
	stepperPosition_2 = 0;
	stepperPosition_3 = 0;
	stepperPosition_4 = 0;
	stepperPosition_5 = 0;

	eStopTriggered = false;
	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);

	// Z Axis
	multiAxisChecked = 1 & configMulti.multiAxesIDs; // Z
#ifdef DEBUG
	Serial.print("multiAxisChecked  Z:  ");
	Serial.println(multiAxisChecked);
#endif // DEBUG
	if (multiAxisChecked == 1)
	{
		checkedCount += 1;
		stepper1_step = PIN_AXIS_Z_STEP;
		stepper1_dir = PIN_AXIS_Z_DIR;
		stepperId_1 = ID_AXIS_Z;

		limitPin_Max_1 = configSetup.limit_Max_Z;
		limitPin_Min_1 = configSetup.limit_Min_Z;
		digitalWrite(limitPin_Max_1, HIGH);
		digitalWrite(limitPin_Min_1, HIGH);

		direction = 1 & configMulti.direction; // Z
		if (direction == 1)
		{
			direction_1 = 1;
		}
		else
		{
			direction_1 = -1;
		}
		stepsToMove_1 = (configMulti.targetDistance_Z / (configSetup.distancePerRev_AxisZ) * (configSetup.steps360_Axis_Z * configSetup.microsteps_Axis_Z));
#ifdef DEBUG		
		Serial.print("stepsToMove_1:  ");
		Serial.println(stepsToMove_1);
#endif // DEBUG
		stepsToMove_1 = stepsToMove_1 * direction_1;

		// Set speed and acceleration
		newSpeed_1 = configMulti.speedPercent_Axis_Z * configMulti.maxSpd_Axis_Z * .01;
		accel_1 = configMulti.accel_Axis_Z;
#ifdef DEBUG
		Serial.print("configMulti.accel_Axis_Z:  ");
		Serial.println(configMulti.accel_Axis_Z);
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
		Serial.println(configMulti.speedPercent_Axis_Z);
		Serial.print("configMulti.maxSpd_Axis_Z:  ");
		Serial.println(configMulti.maxSpd_Axis_Z);
#endif // DEBUG
		SetEnable(ID_AXIS_Z, true, true);
	}

	// X Axis
	multiAxisChecked = 2 & configMulti.multiAxesIDs; // X
#ifdef DEBUG
	Serial.print("multiAxisChecked X:  ");
	Serial.println(multiAxisChecked);
#endif // DEBUG
	if (multiAxisChecked == 2)
	{
		//checked_X = 1;
		checkedCount += 1;
		direction = 2 & configMulti.direction; // X
		if (checkedCount == 1)
		{
			stepper1_step = PIN_AXIS_X_STEP;
			stepper1_dir = PIN_AXIS_X_DIR;
			stepperId_1 = ID_AXIS_X;
			if (direction == 2)
			{
				direction_1 = 1;
			}
			else
			{
				direction_1 = -1;
			}

			Serial.print("X direction: ");
			Serial.println(direction_1);
			stepsToMove_1 = (configMulti.targetDistance_X / (configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X)) * direction_1;
			
			// Set speed and acceleration
			newSpeed_1 = configMulti.speedPercent_Axis_X * configMulti.maxSpd_Axis_X * .01;
			accel_1 = configMulti.accel_Axis_X;

			limitPin_Max_1 = configSetup.limit_Max_X;
			limitPin_Min_1 = configSetup.limit_Min_X;
			digitalWrite(limitPin_Max_1, HIGH);
			digitalWrite(limitPin_Min_1, HIGH);
		}
		else // checkedCount = 2
		{
			stepper2_step = PIN_AXIS_X_STEP;
			stepper2_dir = PIN_AXIS_X_DIR;
			stepperId_2 = ID_AXIS_X;
			if (direction == 2)
			{
				direction_2 = 1;
			}
			else
			{
				direction_2 = -1;
			}
			stepsToMove_2 = (configMulti.targetDistance_X / (configSetup.distancePerRev_AxisX) * (configSetup.steps360_Axis_X * configSetup.microsteps_Axis_X)) * direction_2;
			
			// Set speed and acceleration
			newSpeed_2 = configMulti.speedPercent_Axis_X * configMulti.maxSpd_Axis_X * .01;
			accel_2 = configMulti.accel_Axis_X;

			limitPin_Max_2 = configSetup.limit_Max_X;
			limitPin_Min_2 = configSetup.limit_Min_X;
			digitalWrite(limitPin_Max_2, HIGH);
			digitalWrite(limitPin_Min_2, HIGH);
		}

		SetEnable(ID_AXIS_X, true, true);
	}

	// M3 Axis
	multiAxisChecked = 4 & configMulti.multiAxesIDs; // M3
#ifdef DEBUG
	Serial.print("multiAxisChecked M3:   ");
	Serial.println(multiAxisChecked);
#endif // DEBUG
	if (multiAxisChecked == 4)
	{
		//checked_M3 = 1;
		checkedCount += 1;

		direction = 4 & configMulti.direction; // M3
		if (checkedCount == 1)
		{
			stepper1_step = PIN_AXIS_3_STEP;
			stepper1_dir = PIN_AXIS_3_DIR;
			stepperId_1 = ID_AXIS_3;
			if (direction == 4)
			{
				direction_1 = -1;
			}
			else
			{
				direction_1 = 1;
			}

			if (configSetup.radialOrLinear_Axis_M3 == RADIAL_M3)
			{
				stepsPerRevolution = (int)round(configSetup.microsteps_Axis_M3 * configSetup.steps360_Axis_M3 * configSetup.gearRatio_AxisM3);
				stepsToMove_1 = stepsPerRevolution * (configMulti.targetDistance_M3 / 360) * direction_1;
			}
			else
			{
				limitPin_Max_1 = configSetup.limit_Max_M3;
				limitPin_Min_1 = configSetup.limit_Min_M3;
				digitalWrite(limitPin_Max_1, HIGH);
				digitalWrite(limitPin_Min_1, HIGH);

				stepsToMove_1 = (configMulti.targetDistance_M3 / (configSetup.distancePerRev_AxisM3) * (configSetup.steps360_Axis_M3 * configSetup.microsteps_Axis_M3)) * direction_1;
			}

			newSpeed_1 = configMulti.speedPercent_Axis_M3 * configMulti.maxSpd_Axis_M3 * .01;
			accel_1 = configMulti.accel_Axis_M3;
		}
		else if (checkedCount == 2)
		{
			stepper2_step = PIN_AXIS_3_STEP;
			stepper2_dir = PIN_AXIS_3_DIR;
			stepperId_2 = ID_AXIS_3;
			if (direction == 4)
			{
				direction_2 = -1;
			}
			else
			{
				direction_2 = 1;
			}

			if (configSetup.radialOrLinear_Axis_M3 == RADIAL_M3)
			{
				stepsPerRevolution = (int)round(configSetup.microsteps_Axis_M3 * configSetup.steps360_Axis_M3 * configSetup.gearRatio_AxisM3);
				stepsToMove_2 = stepsPerRevolution * (configMulti.targetDistance_M3 / 360) * direction_2;
			}
			else
			{
				limitPin_Max_2 = configSetup.limit_Max_M3;
				limitPin_Min_2 = configSetup.limit_Min_M3;
				digitalWrite(limitPin_Max_2, HIGH);
				digitalWrite(limitPin_Min_2, HIGH);

				stepsToMove_2 = (configMulti.targetDistance_M3 / (configSetup.distancePerRev_AxisM3) * (configSetup.steps360_Axis_M3 * configSetup.microsteps_Axis_M3)) * direction_2;
			}

			newSpeed_2 = configMulti.speedPercent_Axis_M3 * configMulti.maxSpd_Axis_M3 * .01;
			accel_2 = configMulti.accel_Axis_M3;

		}
		else // checkedCount = 3
		{
			stepper3_step = PIN_AXIS_3_STEP;
			stepper3_dir = PIN_AXIS_3_DIR;
			stepperId_3 = ID_AXIS_3;
			if (direction == 4)
			{
				direction_3 -1;
			}
			else
			{
				direction_3 = 1;
			}
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

			newSpeed_3 = configMulti.speedPercent_Axis_M3 * configMulti.maxSpd_Axis_M3 * .01;
			accel_3 = configMulti.accel_Axis_M3;
		}

		SetEnable(ID_AXIS_3, true, true);
	}

	// M4 Axis
	multiAxisChecked = 8 & configMulti.multiAxesIDs; // M4
#ifdef DEBUG
	Serial.print("multiAxisChecked M4:  ");
	Serial.println(multiAxisChecked);
#endif // DEBUG
	if (multiAxisChecked == 8)
	{
		//checked_M4 = 1;
		checkedCount += 1;

		direction = 8 & configMulti.direction; // M4
		if (checkedCount == 1)
		{
			stepper1_step = PIN_AXIS_4_STEP;
			stepper1_dir = PIN_AXIS_4_DIR;
			stepperId_1 = ID_AXIS_4;
			if (direction == 8)
			{
				direction_1 = -1;
			}
			else
			{
				direction_1 = 1;
			}
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
			newSpeed_1 = configMulti.speedPercent_Axis_M4 * configMulti.maxSpd_Axis_M4 * .01;
			accel_1 = configMulti.accel_Axis_M4;
		}
		else if (checkedCount == 2)
		{
			stepper2_step = PIN_AXIS_4_STEP;
			stepper2_dir = PIN_AXIS_4_DIR;
			stepperId_2 = ID_AXIS_4;
			if (direction == 8)
			{
				direction_2 = -1;
			}
			else
			{
				direction_2 = 1;
			}
			if (configSetup.radialOrLinear_Axis_M4 == RADIAL_M4)
			{
				stepsPerRevolution = (int)round(configSetup.microsteps_Axis_M4 * configSetup.steps360_Axis_M4 * configSetup.gearRatio_AxisM4);
				stepsToMove_2 = stepsPerRevolution * (configMulti.targetDistance_M4 / 360) * direction_2;
			}
			else
			{
				limitPin_Max_2 = configSetup.limit_Max_M4;
				limitPin_Min_2 = configSetup.limit_Min_M4;
				digitalWrite(limitPin_Max_2, HIGH);
				digitalWrite(limitPin_Min_2, HIGH);

				stepsToMove_2 = (configMulti.targetDistance_M4 / (configSetup.distancePerRev_AxisM4) * (configSetup.steps360_Axis_M4 * configSetup.microsteps_Axis_M4)) * direction_2;
			}
			newSpeed_2 = configMulti.speedPercent_Axis_M4 * configMulti.maxSpd_Axis_M4 * .01;
			accel_2 = configMulti.accel_Axis_M4;

		}
		else if (checkedCount == 3)
		{
			stepper3_step = PIN_AXIS_4_STEP;
			stepper3_dir = PIN_AXIS_4_DIR;
			stepperId_3 = ID_AXIS_4;
			if (direction == 8)
			{
				direction_3 = -1;
			}
			else
			{
				direction_3 = 1;
			}
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

			newSpeed_3 = configMulti.speedPercent_Axis_M4 * configMulti.maxSpd_Axis_M4 * .01;
			accel_3 = configMulti.accel_Axis_M4;

		}
		else //(checkedCount == 4)
		{
			stepper4_step = PIN_AXIS_4_STEP;
			stepper4_dir = PIN_AXIS_4_DIR;
			stepperId_4 = ID_AXIS_4;
			if (direction == 8)
			{
				direction_4 = -1;
			}
			else
			{
				direction_4 = 1;
			}
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

			newSpeed_4 = configMulti.speedPercent_Axis_M4 * configMulti.maxSpd_Axis_M4 * .01;
			accel_4 = configMulti.accel_Axis_M4;
		}

		SetEnable(ID_AXIS_4, true, true);
	}

	// Spindle
	multiAxisChecked = 16 & configMulti.multiAxesIDs; // Spindle
#ifdef DEBUG
	Serial.print("multiAxisChecked Spindle:  ");
	Serial.println(multiAxisChecked);
	Serial.print("666666666666666666666666666666666666666666666666666666666666666666checkedCount: ");
	Serial.println(checkedCount);
#endif // DEBUG
	if (multiAxisChecked == 16)
	{
		checkedCount += 1;

		direction = 16 & configMulti.direction; // M4

		if (checkedCount == 1)
		{
			stepper1_step = PIN_SPINDLE_STEP;
			stepper1_dir = PIN_SPINDLE_DIR;
			stepperId_1 = ID_SPINDLE;
			if (direction == 16)
			{
				direction_1 = -1;
			}
			else
			{
				direction_1 = 1;
			}

			stepsPerRevolution = (int)round(configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle);
#ifdef DEBUG
			Serial.print("stepsPerRevolution:  ");
			Serial.println(stepsPerRevolution);
#endif // DEBUG
			stepsToMove_1 = stepsPerRevolution * (configMulti.targetDistance_Spindle / 360) * direction_1;

			// Set speed and acceleration
			newSpeed_1 = configMulti.speedPercent_Spindle * configMulti.maxSpd_Spindle * .01;
			accel_1 = configMulti.accel_Spindle;
#ifdef DEBUG

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
#endif // DEBUG
		}
		else if (checkedCount == 2)
		{
			stepper2_step = PIN_SPINDLE_STEP;
			stepper2_dir = PIN_SPINDLE_DIR;
			stepperId_2 = ID_SPINDLE;
			if (direction == 16)
			{
				direction_2 = -1;
			}
			else
			{
				direction_2 = 1;
			}
			stepsPerRevolution = (int)round(configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle);
			stepsToMove_2 = stepsPerRevolution * (configMulti.targetDistance_Spindle / 360) * direction_2;

			// Set speed and acceleration
			newSpeed_2 = configMulti.speedPercent_Spindle * configMulti.maxSpd_Spindle * .01;
			accel_2 = configMulti.accel_Spindle;
		}
		else if (checkedCount == 3)
		{
			stepper3_step = PIN_SPINDLE_STEP;
			stepper3_dir = PIN_SPINDLE_DIR;
			stepperId_3 = ID_SPINDLE;
			if (direction == 16)
			{
				direction_3 = -1;
			}
			else
			{
				direction_3 = 1;
			}
			stepsPerRevolution = (int)round(configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle);
			stepsToMove_3 = stepsPerRevolution * (configMulti.targetDistance_Spindle / 360) * direction_3;

			// Set speed and acceleration
			newSpeed_3 = configMulti.speedPercent_Spindle * configMulti.maxSpd_Spindle * .01;
			accel_3 = configMulti.accel_Spindle;
		}
		else if (checkedCount == 4)
		{
			stepper4_step = PIN_SPINDLE_STEP;
			stepper4_dir = PIN_SPINDLE_DIR;
			stepperId_4 = ID_SPINDLE;
			if (direction == 16)
			{
				direction_4 = -1;
			}
			else
			{
				direction_4 = 1;
			}
			stepsPerRevolution = (int)round(configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle);
			stepsToMove_4 = stepsPerRevolution * (configMulti.targetDistance_Spindle / 360) * direction_4;

			// Set speed and acceleration
			newSpeed_4 = configMulti.speedPercent_Spindle * configMulti.maxSpd_Spindle * .01;
			accel_4 = configMulti.accel_Spindle;
		}
		else //(checkedCount == 5)
		{
			stepper5_step = PIN_SPINDLE_STEP;
			stepper5_dir = PIN_SPINDLE_DIR;
			stepperId_5 = ID_SPINDLE;
			if (direction == 16)
			{
				direction_5 = -1;
			}
			else
			{
				direction_5 = 1;
			}
			stepsPerRevolution = (int)round(configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle);
			stepsToMove_5 = stepsPerRevolution * (configMulti.targetDistance_Spindle / 360) * direction_5;

			// Set speed and acceleration
			newSpeed_5 = configMulti.speedPercent_Spindle * configMulti.maxSpd_Spindle * .01;
			accel_5 = configMulti.accel_Spindle;
		}

		SetEnable(ID_SPINDLE, true, true);
	}

	Serial.print("2:checkedCount: ");
	Serial.println(checkedCount);
	Stepper stepper_1(stepper1_step, stepper1_dir);
	Stepper stepper_2(stepper2_step, stepper2_dir);
	Stepper stepper_3(stepper3_step, stepper3_dir);
	Stepper stepper_4(stepper4_step, stepper4_dir);
	Stepper stepper_5(stepper5_step, stepper5_dir);

	if (checkedCount == 1)
	{
		newSpeed_2 = newSpeed_1;
		newSpeed_3 = newSpeed_1;
		newSpeed_4 = newSpeed_1;
		newSpeed_5 = newSpeed_1;
		accel_2 = accel_1;
		accel_3 = accel_1;
		accel_4 = accel_1;
		accel_5 = accel_1;
		stepsToMove_2 = stepsToMove_1;
		stepsToMove_3 = stepsToMove_1;
		stepsToMove_4 = stepsToMove_1;
		stepsToMove_5 = stepsToMove_1;
	}
	else if (checkedCount == 2)
	{
		newSpeed_3 = newSpeed_1;
		newSpeed_4 = newSpeed_1;
		newSpeed_5 = newSpeed_1;
		accel_3 = accel_1;
		accel_4 = accel_1;
		accel_5 = accel_1;
		stepsToMove_3 = stepsToMove_1;
		stepsToMove_4 = stepsToMove_1;
		stepsToMove_5 = stepsToMove_1;
	}
	else if (checkedCount == 3)
	{
		newSpeed_4 = newSpeed_1;
		newSpeed_5 = newSpeed_1;
		accel_4 = accel_1;
		accel_5 = accel_1;
		stepsToMove_4 = stepsToMove_1;
		stepsToMove_5 = stepsToMove_1;
	}
	else if (checkedCount == 4)
	{
		newSpeed_5 = newSpeed_1;
		accel_5 = accel_1;
		stepsToMove_5 = stepsToMove_1;
	}

	stepper_1.setPosition(0);
	stepper_1
		.setMaxSpeed(newSpeed_1)
		.setAcceleration(accel_1)
		.setTargetRel(stepsToMove_1);

	stepper_2.setPosition(0);
	stepper_2
		.setMaxSpeed(newSpeed_2)
		.setAcceleration(accel_2)
		.setTargetRel(stepsToMove_2);

	stepper_3.setPosition(0);
	stepper_3
		.setMaxSpeed(newSpeed_3)
		.setAcceleration(accel_3)
		.setTargetRel(stepsToMove_3);

	stepper_4.setPosition(0);
	stepper_4
		.setMaxSpeed(newSpeed_4)
		.setAcceleration(accel_4)
		.setTargetRel(stepsToMove_4);

	stepper_5.setPosition(0);
	stepper_5
		.setMaxSpeed(newSpeed_5)
		.setAcceleration(accel_5)
		.setTargetRel(stepsToMove_5);
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
	stepController.moveAsync(stepper_1, stepper_2, stepper_3, stepper_4, stepper_5);

	int eStop = 0;
	//int nCorNO = configSetup.limit_NCorNO;
	while (stepController.isRunning())
	{
		MilliDelay(100);
		currentSpeed = stepController.getCurrentSpeed();
		Serial.print("currentSpeed: ");
		Serial.print(currentSpeed);

		if (limitPin_Max_1 > 9 && limitPin_Max_1 < 40)
		{
			if ((digitalRead(limitPin_Max_1) != configSetup.limit_NCorNO) && direction_1 > 0)
			{
#ifdef DEBUG
				Serial.println("                          limitPin_Max_1");
#endif // DEBUG
				stepController.emergencyStop();
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
		if (limitPin_Max_1 > 9 && limitPin_Max_1 < 40)
		{
			if ((digitalRead(limitPin_Min_1) != configSetup.limit_NCorNO) && direction_1 < 0)
			{
#ifdef DEBUG
				Serial.println("                          limitPin_Min_1");
#endif // DEBUG
				stepController.emergencyStop();
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
				stepController.emergencyStop();
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
				stepController.emergencyStop();
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
				stepController.emergencyStop();
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
				stepController.emergencyStop();
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
				stepController.emergencyStop();
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
				stepController.emergencyStop();
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
		//else if (configSetup.eStop != 0 && configSetup.eStop < 40)
		if (configSetup.eStop > 9 && configSetup.eStop < 40)
		{
			eStop = digitalRead(configSetup.eStop);
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
				Serial.println("                          EStop");

				stepController.emergencyStop();
				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEnable(ID_AXIS_4, false, false);
				SetEStopColors(PAGE_MULTI);
				if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
				{
					digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
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
				currentSpeed = stepController.getCurrentSpeed();
#ifdef DEBUG
				Serial.print("currentSpeed: ");
				Serial.println(currentSpeed);
#endif // Debug
				if (currentSpeed < MIN_SPEED)
				{
					stepController.emergencyStop();
				}
				else
				{
					stepController.stop();
				}
				stopSteppers = true;
				break;
			}
			}
		}

		MilliDelay(2);
	}

endLoop:
#ifdef DEBUG
	Serial.print("keepSteppersEnabled:  ");
	Serial.println(configSetup.keepSteppersEnabled);
#endif // DEBUG
	if (configSetup.keepSteppersEnabled != 1)
	{
#ifdef DEBUG
		Serial.print("stepper1 axisId:  ");
		Serial.println(stepperId_1);
#endif // DEBUG
		switch (stepperId_1)
		{
			case ID_AXIS_Z:
			{
				switch (pageCallerId)
				{
					case PAGE_MULTI:
					{
						SerialPrint("pageMulti.bt1301.val=0"); // 1 = disabled
						SerialPrint("\xFF\xFF\xFF");
						SerialPrint("pageMulti.bt1301.txt="); // 1 = disabled
						SerialPrint("\x22");
						SerialPrint("Disabled");
						SerialPrint("\x22\xFF\xFF\xFF");
						Serial.print("Disable Z:  ");
						break;
					}
				}

				break;
			}
			case ID_AXIS_X:
			{
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
				SerialPrint("pageMulti.bt1404.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageMulti.bt1404.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");
				break;
			}
			case ID_SPINDLE:
			{
				switch (pageCallerId)
				{
					case PAGE_MULTI:
					{
						SerialPrint("pageMulti.bt1300.val=0"); // 1 = disabled
						SerialPrint("\xFF\xFF\xFF");
						SerialPrint("pageMulti.bt1300.txt="); // 1 = disabled
						SerialPrint("\x22");
						SerialPrint("Disabled");
						SerialPrint("\x22\xFF\xFF\xFF");
						break;
					}
					case PAGE_INDEX:
					{
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
						break;
					}
				}
				break;
			}
		}
		switch (stepperId_2)
		{
			case ID_AXIS_X:
			{
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
			case ID_AXIS_3:
			{
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
			case ID_AXIS_4:
			{
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
			case ID_SPINDLE:
			{
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

	// Set Return page values
	stepperPosition_1 = stepper_1.getPosition() * (-1);
	stepperPosition_2 = stepper_2.getPosition() * (-1);
	stepperPosition_3 = stepper_3.getPosition() * (-1);
	stepperPosition_4 = stepper_4.getPosition() * (-1);
	stepperPosition_5 = stepper_5.getPosition() * (-1);

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
	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
	}
}


/// <summary>
/// Index spindle
/// </summary>
/// <comment>
/// Only used in ProgramFromFile
/// </comment>
/// <param name="directionSpindle">Counterclockwise: -1, Clockwise: 1</param>
/// <returns></returns>
void IndexSpindle(int directionSpindle)
{
	StepControl stepController;
	Stepper stepper_Spindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);

	const char* index1_Char = "Index 1-";
	const char* index2_Char = "Index 2-";
	const char* index3_Char = "Index 3-";
	const char* index4_Char = "Index 4-";
	const char* index5_Char = "Index 5-";
	const char* size_Char = "Size:";
	const char* direction_Char = "Direction:";
	const char* stepsToMove_Char = "Steps to Move:";
	const char* pageIndex_bt3_val_Char = "pageIndex.bt3.val=0";
	const char* pageIndex_va0_Char = "pageIndex.va0.val=0";
	const char* pageIndex_bt1_val_Char = "pageIndex.bt1.val=0";
	const char* pageIndex_bt2_val_Char = "pageIndex.bt2.val=0";
	const char* pageBE_t1_Char = "pageBE.t1.txt=";
	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		digitalWrite(configSetup.cutterMotorPin, LOW);  // Enable
	}
	stepper_Spindle.setPosition(0);
	int stepsToMove = 0;

	switch (configIndex_Prime.indexId)
	{
		case INDEX_1:
		{
	#ifdef DEBUG
			Serial.print(index1_Char);
			Serial.print(size_Char);
			Serial.println(configIndex_1.sizeInSteps);
	#endif // DEBUG

			stepsToMove = (int)round(configIndex_1.sizeInSteps);
			break;
		}
		case INDEX_2:
		{
	#ifdef DEBUG
			Serial.print(index2_Char);
			Serial.print(size_Char);
			Serial.println(configIndex_2.sizeInSteps);
	#endif // DEBUG

			stepsToMove = (int)round(configIndex_2.sizeInSteps);
			break;
		}
		case INDEX_3:
		{
	#ifdef DEBUG
			Serial.print(index3_Char);
			Serial.print(size_Char);
			Serial.println(configIndex_3.sizeInSteps);
	#endif // DEBUG
			stepsToMove = (int)round(configIndex_3.sizeInSteps);
			break;
		}
		case INDEX_4:
		{
	#ifdef DEBUG
			Serial.print(index4_Char);
			Serial.print(size_Char);
			Serial.println(configIndex_4.sizeInSteps);
	#endif // DEBUG
			stepsToMove = (int)round(configIndex_4.sizeInSteps);
			break;
		}
		case INDEX_5:
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


	Serial.print(stepsToMove_Char);
	Serial.println(stepsToMove);
#endif // DEBUG

	// Set speed and acceleration
	stepper_Spindle.setMaxSpeed(configIndex_Prime.maxSpd_Spindle);
	stepper_Spindle.setAcceleration(configIndex_Prime.accel_Spindle);
	stepper_Spindle.setTargetRel(stepsToMove * directionSpindle);
#ifdef DEBUG
	Serial.print("setTargetRel:   ");
	Serial.println(stepsToMove * directionSpindle);
#endif // DEBUG
	SetEnable(ID_SPINDLE, true, true);
	stepController.moveAsync(stepper_Spindle);

	while (stepController.isRunning())
	{
		MilliDelay(5);
#ifdef ESTOP_ENABLED
		//if (configSetup.eStop != 0)
		if (configSetup.eStop > 9 && configSetup.eStop < 40)
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
				SetEnable(ID_AXIS_3, false, false);
				SetEStopColors(PAGE_INDEX);
				if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
				{
					digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
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
				currentSpeed = stepController.getCurrentSpeed();
#ifdef DEBUG
				Serial.print("currentSpeed: ");
				Serial.println(currentSpeed);
#endif // Debug
				if (currentSpeed < MIN_SPEED)
				{
					stepController.emergencyStop();
				}
				else
				{
					stepController.stop();
				}
				stopSteppers = true;
				break;
			}
			}
		}
		MilliDelay(10);
	}


	// Update Index flag on Nextion. 
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
	degrees_Spindle = StepsToDegrees(returnSteps_Spindle, ID_SPINDLE);

	SerialPrint(pageBE_t1_Char);
	SerialWrite(0x22);
	SerialPrint(degrees_Spindle);
	SerialPrint(nextionQuoteEnd);
#endif // Show Position
	SetEnable(configIndex_Prime.indexId, false, true);
	// 0 = Change to disabled, 1 = keep enabled
	if (configSetup.keepSteppersEnabled == 0)
	{
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

	if (configIndex_Prime.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		SetEnable(ID_AXIS_3, false, true);
	}

	SerialPrint("pageIndex.bt0.val=0");
	SerialPrint(nextionEnd);
	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
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
	StepControl stepController;
	Stepper stepper_M3(PIN_AXIS_3_STEP, PIN_AXIS_3_DIR);
	Stepper stepper_M4(PIN_AXIS_4_STEP, PIN_AXIS_4_DIR);

	uint32_t maxSpd_M3 = 0;

	const char* direction_Char = "Direction:";
	const char* stepsToMove_Char = "Steps to Move:";

	const char* pageIndex_bt3_val_Char = "pageIndex.bt3.val=0";
	const char* pageIndex_va0_Char = "pageIndex.va0.val=0";
	const char* pageIndex_bt1_val_Char = "pageIndex.bt1.val=0";
	const char* pageIndex_bt2_val_Char = "pageIndex.bt2.val=0";
	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		digitalWrite(configSetup.cutterMotorPin, LOW);  // Enable
	}
	stepperId_1 = configIndex_Prime.axisId;
	checkedCount = 1;
	stepper1_step = 48;
	stepper1_dir = 49;
	stepperId_2 = ID_AXIS_3;
	stepper2_step = 50;
	stepper2_dir = 51;
	int stepsPerRevolution = 0;
	int stepsToMove = 0;
	int stepsToMoveSynchro = 0;
	int enableAxis = 0;
	uint32_t maxSpeed = 0;
	uint32_t accel = 0;
	double indexSize = 0;
	int degreesOrDivisions = 0;

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

	Serial.print("IndexId: ");
	Serial.println(configIndex_Prime.indexId);
	Serial.print("axisId: ");
	Serial.println(configIndex_Prime.axisId);
	switch (configIndex_Prime.axisId)
	{
		case ID_SPINDLE:
		{
			stepsToMove = 0;
			stepper1_step = PIN_SPINDLE_STEP;
			stepper1_dir = PIN_SPINDLE_DIR;
			
			enableAxis = ID_SPINDLE;
			stepsPerRevolution = (int)round(configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle);
			//maxSpeed
			maxSpeed = configIndex_Prime.maxSpd_Spindle * (configIndex_Prime.speedPercent_Spindle * .01);
			//accel
			accel = configIndex_Prime.accel_Spindle;
#ifdef DEBUG			
			Serial.print("configSetup.microsteps_Spindle:  ");
			Serial.println(configSetup.microsteps_Spindle);

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
#endif // DEBUG
			break;
		}
		case ID_AXIS_3:
		{
			stepsToMove = 0;
			stepper1_step = PIN_AXIS_3_STEP;
			stepper1_dir = PIN_AXIS_3_DIR;
			enableAxis = ID_AXIS_3;
			stepsPerRevolution = (int)round(configSetup.microsteps_Axis_M3 * configSetup.steps360_Axis_M3 * configSetup.gearRatio_AxisM3);
			//maxSpeed
			maxSpeed = configIndex_Prime.maxSpd_Axis_M3 * (configIndex_Prime.speedPercent_Axis_M3 *.01);
			
			//accel
			accel = configIndex_Prime.accel_Axis_M3;
#ifdef DEBUG
			Serial.print("configIndex_Main.maxSpd_Axis_M3:  ");
			Serial.println(configIndex_Prime.maxSpd_Axis_M3);
			Serial.print("configIndex_Main.speedPercent_Axis_M3:  ");
			Serial.println(configIndex_Prime.speedPercent_Axis_M3);

			Serial.print("configIndex_Main.accel_Axis_M3:  ");
			Serial.println(configIndex_Prime.accel_Axis_M3);
#endif // DEBUG
			break;
		}
		case ID_AXIS_4:
		{
			stepsToMove = 0;
			stepper1_step = PIN_AXIS_4_STEP;
			stepper1_dir = PIN_AXIS_4_DIR;
			enableAxis = ID_AXIS_4;
			stepsPerRevolution = (int)round(configSetup.microsteps_Axis_M4 * configSetup.steps360_Axis_M4 * configSetup.gearRatio_AxisM4);
			//maxSpeed
			maxSpeed = configIndex_Prime.maxSpd_Axis_M4 * (configIndex_Prime.speedPercent_Axis_M4 * .01);
			//accel
			accel = configIndex_Prime.accel_Axis_M4;
			
			break;
		}
	}

	if (degreesOrDivisions == BY_DEGREES) // Degrees
	{
		stepsToMove = stepsPerRevolution * (indexSize / 360) * directionAxis;
	}
	else
	{
		stepsToMove = stepsPerRevolution / (indexSize) * directionAxis;
	}

	Stepper stepper_1(stepper1_step, stepper1_dir);
	stepper_1.setPosition(0);

#ifdef DEBUG
	Serial.print(direction_Char);
	Serial.println(directionAxis);
	Serial.print("maxSpeed: ");
	Serial.println(maxSpeed);
	Serial.print("accel: ");
	Serial.println(accel);
	Serial.print(stepsToMove_Char);
	Serial.println(stepsToMove);
#endif // DEBUG

	// Set speed and acceleration
	stepper_1
		.setMaxSpeed(maxSpeed)
		.setAcceleration(accel)
		.setTargetRel(stepsToMove);

	SetEnable(enableAxis, true, true);


	checkedCount = 1;
	if (configIndex_Prime.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		checkedCount = 2;
//		if (configIndex_Prime.axisId==ID_SPINDLE)
//		{ 
			stepsToMoveSynchro = stepsToMove * synchro_Ratio * (configIndex_Prime.synchro_M3_Percentage * 0.01);
			//stepsToMoveSynchro = stepsToMoveSynchro * (-1);
			maxSpd_M3 = configIndex_Prime.maxSpd_Axis_M3 * (configIndex_Prime.speedPercent_Axis_M3 * .01);

	#ifdef DEBUG
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

	#endif // DEBUG

			//stepper2_step = PIN_AXIS_3_STEP;
			//stepper2_dir = PIN_AXIS_3_DIR;
			stepperId_2 = ID_AXIS_3;
			stepper2_dir = PIN_AXIS_3_DIR;
			stepper2_step = PIN_AXIS_3_STEP;
			SetEnable(ID_AXIS_3, true, true);
			stepper_M3.setPosition(0);
			stepper_M3
				.setMaxSpeed(maxSpd_M3)
				.setAcceleration(accel)
				.setTargetRel(stepsToMoveSynchro);


	#ifdef DEBUG
			Serial.print("maxSpeed: ");
			Serial.println(maxSpeed);
			Serial.print("accel: ");
			Serial.println(accel);
	#endif // DEBUG

			MilliDelay(5);
			stepController.moveAsync(stepper_1, stepper_M3);



		//}
	}
	else
	{
		stepController.moveAsync(stepper_1);
	}

	
	while (stepController.isRunning())
	{
		MilliDelay(5);
#ifdef ESTOP_ENABLED
		//if (configSetup.eStop != 0)
		if (configSetup.eStop > 9 && configSetup.eStop < 40)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
#ifdef DEBUG
				Serial.println("                          EStop ");
#endif // DEBUG
				stepController.emergencyStop();
				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEStopColors(PAGE_INDEX);
				if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
				{
					digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
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
				currentSpeed = stepController.getCurrentSpeed();
#ifdef DEBUG
				Serial.print("currentSpeed: ");
				Serial.println(currentSpeed);
#endif // Debug
				if(currentSpeed < MIN_SPEED)
				{ 
					stepController.emergencyStop();
				}
				else
				{ 
					stepController.stop();
				}
				Serial.print("-----------------Stopped---------------------------------");
				stopSteppers = true;
				break;
			}
			}
		}
		MilliDelay(10);
	}


	// Update Index flag on Nextion. 
	SerialPrint(pageIndex_va0_Char);
	SerialPrint(nextionEnd);
	SerialPrint(pageIndex_bt3_val_Char);
	SerialPrint(nextionEnd);
	SerialPrint(pageIndex_bt2_val_Char);
	SerialPrint(nextionEnd);
	SerialPrint(pageIndex_bt1_val_Char);
	SerialPrint(nextionEnd);

	stepperPosition_1 = stepper_1.getPosition() * (-1);

	SetEnable(configIndex_Prime.axisId, false, true);

	// 0 = Change to disabled, 1 = keep enabled
	if (configSetup.keepSteppersEnabled == 0)
	{
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

	if (configIndex_Prime.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		stepperPosition_2 = stepper_M3.getPosition() * (-1);
		SetEnable(ID_AXIS_3, false, true);
	}


	SerialPrint("pageIndex.bt0.val=0");
	SerialPrint(nextionEnd);
	SerialPrint("pageIndex.bt1.val=0");
	SerialPrint(nextionEnd);
	SerialPrint("pageIndex.bt2.val=0");
	SerialPrint(nextionEnd);
	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
	}
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
	const char* fileOrFixed_Char = " File or Fixed: ";
	const char* lineNumber_Char = "Line Number: ";
	const char* pageIndex_t7_Char = "pageIndex.t7.txt=";

	const char* pageIndex_va0_Char = "pageIndex.va0.val=0";
	const char* pageIndex_bt3_Char = "pageIndex.bt3.val=0";
	const char* pageIndex_bt2_Char = "pageIndex.bt2.val=0";
	const char* pageIndex_bt1_Char = "pageIndex.bt1.val=0";
	const char* pageSync_b6_Char = "pageSync.b6.pco=0";
	const char* pageSync_b5_Char = "pageSync.b5.pco=0";

	eStopTriggered = false;
	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);

	double newIndexSize = 0;
	int lineNumber = 0;
	badFilename = false;

	lineNumber = GetSerialInteger();
#ifdef DEBUG
	Serial.print(lineNumber_Char);
	Serial.println(lineNumber);
#endif // DEBUG
	if (lineNumber == 255)
	{
		lineNumber = 0;
	}

#ifdef DEBUG
	Serial.print(indexId_Char);
	Serial.println(configIndex_Prime.indexId);
	Serial.print(lineNumber_Char);
	Serial.println(lineNumber);
#endif // DEBUG

	newIndexSize = GetIndexDataFromSD(lineNumber);

	switch (configIndex_Prime.indexId)
	{
		case INDEX_1:
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
		case INDEX_2:
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
		case INDEX_3:
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
		case INDEX_4:
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
		case INDEX_5:
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
#ifdef DEBUG
	Serial.print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	Serial.print(indexId_Char);
	Serial.println(currentIndex);
	Serial.println(newIndexSize);
	Serial.print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
#endif // DEBUG
	

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
	#ifdef DEBUG
			Serial.print(index1_Char);
			Serial.print(indexSizeChar);
			Serial.println(configIndex_1.sizeInSteps);
	#endif // DEBUG
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
	#ifdef DEBUG
			Serial.print(index2_Char);
			Serial.print(indexSizeChar);
			Serial.println(configIndex_2.sizeInSteps);
	#endif // DEBUG
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

	#ifdef DEBUG
			Serial.print(index3_Char);
			Serial.print(indexSizeChar);
			Serial.println(configIndex_3.sizeInSteps);
	#endif // DEBUG
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

	#ifdef DEBUG
			Serial.print(index4_Char);
			Serial.print(indexSizeChar);
			Serial.println(configIndex_4.sizeInSteps);
	#endif // DEBUG
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
	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		digitalWrite(configSetup.cutterMotorPin, LOW);  // Enable
	}
	eStopTriggered = false;
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
		case ID_AXIS_3:
		{
			stepPin = PIN_AXIS_3_STEP;
			dirPin = PIN_AXIS_3_DIR;
			limitPin_Max = configSetup.limit_Max_M3;
			limitPin_Min = configSetup.limit_Min_M3;
			maxSpeed = configSync.maxSpd_Axis_M3 * configSync.speedPercent_Axis_M3 * .01;
			accel = configSync.accel_Axis_M3;
			switch (configSetup.radialOrLinear_Axis_M3)
			{
				case RADIAL_M3:
				{
					axisSteps = DistanceToSteps_M3Radial(configSync.distance)* directionAxis;
					break;
				}
				case LINEAR_M3:
				{
					axisSteps = DistanceToSteps_M3Linear(configSync.distance) * directionAxis * (-1);
					break;
				}
			}
			
			break;
		}
		case ID_AXIS_4:
		{
			stepPin = PIN_AXIS_4_STEP;
			dirPin = PIN_AXIS_4_DIR;
			limitPin_Max = configSetup.limit_Max_M4;
			limitPin_Min = configSetup.limit_Min_M4;
			maxSpeed = configSync.maxSpd_Axis_M4 * configSync.speedPercent_Axis_M4 * .01;
			accel = configSync.accel_Axis_M4;
			switch (configSetup.radialOrLinear_Axis_M4)
			{
			case RADIAL_M3:
			{
				axisSteps = DistanceToSteps_M4Radial(configSync.distance) * directionAxis;
				break;
			}
			case LINEAR_M3:
			{
				axisSteps = DistanceToSteps_M4Linear(configSync.distance) * directionAxis * (-1);
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

	stepperId_1 = ID_SPINDLE;
	stepper1_step = PIN_SPINDLE_STEP;
	stepper1_dir = PIN_SPINDLE_DIR;

	stepperId_2 = configSync.axisId;
	stepper2_step = stepPin;
	stepper2_dir = dirPin;


	float targetSteps_Spindle = 0;
	targetSteps_Spindle = RevolutionsToSteps(configSync.revolutions_Spindle, directionSpindle,ID_SPINDLE);
	stepper_Spindle
		.setMaxSpeed(configSync.maxSpd_Spindle * configSync.speedPercent_Spindle * .01)
		.setAcceleration(configSync.accel_Spindle)
		.setTargetRel(targetSteps_Spindle);

	stepper_Axis
		.setMaxSpeed(maxSpeed)
		.setAcceleration(accel)
		.setTargetRel(axisSteps);

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
	SetEnable(configSync.axisId, true, true);

#ifdef DEBUG
	Serial.println("MoveAsync..............................................");
	Serial.print("configMain.synchro_M3_Spindle: ");
	Serial.println(configMain.synchro_M3_Spindle);
#endif // Debug

	checkedCount = 2;
	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		checkedCount = 3;
		stepperId_3 = ID_AXIS_3;
		stepper3_step = PIN_AXIS_3_STEP;
		stepper3_dir = PIN_AXIS_3_DIR;
#ifdef DEBUG
		Serial.print("targetSteps_Spindle * synchro_Ratio: ");
		Serial.println(targetSteps_Spindle* synchro_Ratio);
#endif // Debug
		SetEnable(ID_AXIS_3, true, true);
		stepperAxis_M3.setPosition(0);
		stepperAxis_M3
			.setMaxSpeed((configSync.maxSpd_Spindle* configSync.speedPercent_Spindle * .01))
			.setAcceleration(configSync.accel_Spindle)
			.setTargetAbs(targetSteps_Spindle * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01));
		MilliDelay(5);
		stepController.moveAsync(stepper_Spindle, stepper_Axis, stepperAxis_M3);
	}
	else
	{ 
		stepController.moveAsync(stepper_Spindle, stepper_Axis);
	}

	while (stepController.isRunning())
	{
		if (limitPin_Max > 9 && limitPin_Max < 40)
		{
			if ((digitalRead(limitPin_Max) != configSetup.limit_NCorNO))// && axisSteps > 0)
			{
#ifdef DEBUG
				Serial.println("                          limitPin_Max");
#endif // Debug
				if (axisSteps > 0)
				{
					stepController.emergencyStop();
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
					stepController.emergencyStop();
					stopSteppers = true;
					SetLimitColors(PAGE_SYNC, DIR_CCW);
					goto endLoop;
				}
			}
		}

#ifdef ESTOP_ENABLED
		//if (configSetup.eStop != 0)
		if (configSetup.eStop > 9 && configSetup.eStop < 40)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
#ifdef DEBUG
				Serial.println("                          EStop");
#endif // Debug
				stepController.emergencyStop();
				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEnable(ID_AXIS_4, false, false);
				SetEStopColors(PAGE_SYNC);
				if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
				{
					digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
				}
				return;
			}
		}
#endif // estop
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
			SerialPrint(pageSync_bt3_pco_Char);//ToDo
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

	//stepper_Spindle.setPosition(0);
	//stepper_Axis.setPosition(0);
	stepperPosition_1 = endPosition_Spindle * (-1);
	stepperPosition_2 = endPosition_Axis * (-1);

	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		SetEnable(ID_AXIS_3, false, true);
		stepperPosition_3 = stepperAxis_M3.getPosition() * (-1);
	}
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
	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
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
	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		digitalWrite(configSetup.cutterMotorPin, LOW);  // Enable
	}
	eStopTriggered = false;
	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);

	stepPin = 0;
	dirPin = 0;
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
		case ID_AXIS_3:
		{
			stepPin = PIN_AXIS_3_STEP;
			dirPin = PIN_AXIS_3_DIR;
			limitPin_Max = configSetup.limit_Max_M3;
			limitPin_Min = configSetup.limit_Min_M3;
			maxSpeed = configRec.maxSpd_Axis_M3 * configRec.speedPercent_Axis_M3 * .01;
			//if (configSetup.radialOrLinear_Axis_M3 == 1) // Linear
			//{
			//	maxSpeed = maxSpeed * (-1);
			//}
			accel = configRec.accel_Axis_M3;
			speedPercent = configRec.speedPercent_Axis_M3;
#ifdef DEBUG
			Serial.print("M3---AxisMaxSpd:");
			Serial.println(configRec.maxSpd_Axis_M3);
			Serial.print("M3---AxisSpeedPercent:");
			Serial.println(configRec.speedPercent_Axis_M3);
			Serial.print("M3---AxisAccel:");
			Serial.println(configRec.accel_Axis_M3);
#endif // DEBUG
			break;
		}
		case ID_AXIS_4:
		{
			stepPin = PIN_AXIS_4_STEP;
			dirPin = PIN_AXIS_4_DIR;
			limitPin_Max = configSetup.limit_Max_M4;
			limitPin_Min = configSetup.limit_Min_M4;
			maxSpeed = configRec.maxSpd_Axis_M4 * configRec.speedPercent_Axis_M4 * .01;
			//if (configSetup.radialOrLinear_Axis_M4 == 1) // Linear
			//{
			//	maxSpeed = maxSpeed * (-1);
			//}
			accel = configRec.accel_Axis_M4;
			speedPercent = configRec.speedPercent_Axis_M4;
#ifdef DEBUG
			Serial.print("M4---AxisMaxSpd:");
			Serial.println(configRec.maxSpd_Axis_M4);
			Serial.print("M4---AxisSpeedPercent:");
			Serial.println(configRec.speedPercent_Axis_M4);
			Serial.print("M4---AxisAccel:");
			Serial.println(configRec.accel_Axis_M3);
#endif // DEBUG
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

	StepControl stepController;
	Stepper stepper_Spindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
	Stepper stepper_Axis(stepPin, dirPin);

	// Set speed and acceleration
	stepper_Spindle
		.setMaxSpeed(configRec.maxSpd_Spindle * configRec.speedPercent_Spindle * .01)
		.setAcceleration(configRec.accel_Spindle);

	int spindleSteps = 0;
	long axisSteps = 0;
	switch (configRec.radial_axial)
	{
		case RADIAL: // Radial
		{ 
			waves = configRec.waves_Radial;
			float spindleDegrees = configRec.degrees_Radial_Spindle / (configRec.waves_Radial * 2);
			spindleSteps = round((configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle) * (spindleDegrees / 360) * wavDir);
#ifdef DEBUG
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
#endif // Debug
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


	switch (configRec.axisId)
	{
	case ID_AXIS_3:
	{
		if (configSetup.radialOrLinear_Axis_M3 == 1) // Linear
		{
			axisSteps = axisSteps * (-1);
		}
		break;
	}
	case ID_AXIS_4:
	{
		if (configSetup.radialOrLinear_Axis_M4 == 1) // Linear
		{
			axisSteps = axisSteps * (-1);
		}
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
	checkedCount = 2;
	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		checkedCount = 3;
		stepperId_3 = ID_AXIS_3;
		stepper3_step = PIN_AXIS_3_STEP;
		stepper3_dir = PIN_AXIS_3_DIR;
#ifdef DEBUG
		Serial.print("spindleSteps * synchro_Ratio: ");
		Serial.println(spindleSteps * synchro_Ratio);
#endif // DEBUG
		SetEnable(ID_AXIS_3, true, true);
		stepperAxis_M3.setPosition(0);
		stepperAxis_M3
			.setMaxSpeed((configRec.maxSpd_Spindle * configRec.speedPercent_Spindle * .01))
			.setAcceleration(configRec.accel_Spindle);
		MilliDelay(5);
	}

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
			Serial.println("M3 Enabled..........................");
			break;
		}
		case ID_AXIS_4:
		{
			SetEnable(ID_AXIS_4, true, true);
			Serial.println("M4 Enabled..........................");
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


		if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
		{
			checkedCount = 3;
			stepperId_3 = ID_AXIS_3;
			stepper3_step = PIN_AXIS_3_STEP;
			stepper3_dir = PIN_AXIS_3_DIR;
#ifdef DEBUG
			Serial.print("spindleSteps * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01):  ");
			Serial.println(spindleSteps* synchro_Ratio* (configMain.synchro_M3_Percentage * 0.01));
#endif // DEBUG
			stepperAxis_M3.setTargetRel(spindleSteps * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01));
			MilliDelay(5);
			stepController.moveAsync(stepper_Spindle, stepper_Axis, stepperAxis_M3);
		}
		else
		{
			stepController.moveAsync(stepper_Spindle, stepper_Axis);
		}

		while (stepController.isRunning())
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
						stepController.emergencyStop();
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
						stepController.emergencyStop();
						stopSteppers = true;
						limitTriggered = true;
						SetLimitColors(PAGE_REC, DIR_CCW);
						goto endLoop;
					}
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
					stepController.emergencyStop();
					stopSteppers = true;
					// Disable all motors
					SetEnable(ID_SPINDLE, false, false);
					SetEnable(ID_AXIS_Z, false, false);
					SetEnable(ID_AXIS_X, false, false);
					SetEnable(ID_AXIS_3, false, false);
					SetEnable(ID_AXIS_4, false, false);
					SetEStopColors(PAGE_REC);
					if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
					{
						digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
					}
					return;
				}
			}
#endif // estop
			// Check for Cancel code  
			if (SerialAvailable() > 0)
			{
				incomingByte = SerialRead(serialId);
				switch (incomingByte)
				{
					case 83: // - S
					{
						currentSpeed = stepController.getCurrentSpeed();
#ifdef DEBUG
						Serial.print("currentSpeed: ");
						Serial.println(currentSpeed);
#endif // Debug
						if (currentSpeed < MIN_SPEED)
						{
							stepController.emergencyStop();
						}
						else
						{
							stepController.stop();
						}
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
	stepperPosition_1 = endPosition_Spindle * (-1);
	stepperPosition_2 = endPosition_Axis * (-1);
#ifdef SHOW_POSITION 
	degrees_Spindle = StepsToDegrees(endPosition_Spindle, ID_SPINDLE);
	distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_X);//Todo
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
	Serial.print("Spindle:");
	Serial.println(endPosition_Spindle);
	Serial.print("Axis:");
	Serial.println(endPosition_Axis);
#endif // DEBUG
	returnSteps_Spindle = endPosition_Spindle;
	SetEnable(ID_SPINDLE, false, true);
	SetEnable(configRec.axisId, false, true);
	//SetEnable(ID_AXIS_X, false, true);
	//SetEnable(ID_AXIS_3, false, true);
	//SetEnable(ID_AXIS_4, false, true);
	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		SetEnable(ID_AXIS_3, false, true);
		stepperPosition_3 = stepperAxis_M3.getPosition() * (-1);
	}

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
	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
	}
}

void Reciprocate_Sawtooth(int wavDir)
{
	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		digitalWrite(configSetup.cutterMotorPin, LOW);  // Enable
	}
	checkedCount = 2;
	eStopTriggered = false;
	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);

	int limitPin_Max = configSetup.limit_Max_Z;
	int limitPin_Min = configSetup.limit_Min_Z;
	stepPin = 0;
	dirPin = 0;
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
		case ID_AXIS_3:
		{
			stepPin = PIN_AXIS_3_STEP;
			dirPin = PIN_AXIS_3_DIR;
			limitPin_Max = configSetup.limit_Max_M3;
			limitPin_Min = configSetup.limit_Min_M3;
			maxSpeed = configRec.maxSpd_Axis_M3 * configRec.speedPercent_Axis_M3 * .01;
			//if (configSetup.radialOrLinear_Axis_M3 == 1) // Linear
			//{
			//	maxSpeed = maxSpeed * (-1);
			//}
			accel = configRec.accel_Axis_M3;
			speedPercent = configRec.speedPercent_Axis_M3;
	#ifdef DEBUG
			Serial.print("M3---AxisMaxSpd:");
			Serial.println(configRec.maxSpd_Axis_M3);
			Serial.print("M3---AxisSpeedPercent:");
			Serial.println(configRec.speedPercent_Axis_M3);
			Serial.print("M3---AxisAccel:");
			Serial.println(configRec.accel_Axis_M3);
	#endif // DEBUG
			break;
		}
		case ID_AXIS_4:
		{
			stepPin = PIN_AXIS_4_STEP;
			dirPin = PIN_AXIS_4_DIR;
			limitPin_Max = configSetup.limit_Max_M4;
			limitPin_Min = configSetup.limit_Min_M4;
			maxSpeed = configRec.maxSpd_Axis_M4 * configRec.speedPercent_Axis_M4 * .01;
			//if (configSetup.radialOrLinear_Axis_M4 == 1) // Linear
			//{
			//	maxSpeed = maxSpeed * (-1);
			//}
			accel = configRec.accel_Axis_M4;
			speedPercent = configRec.speedPercent_Axis_M4;
#ifdef DEBUG
			Serial.print("M4---AxisMaxSpd:");
			Serial.println(configRec.maxSpd_Axis_M4);
			Serial.print("M4---AxisSpeedPercent:");
			Serial.println(configRec.speedPercent_Axis_M4);
			Serial.print("M4---AxisAccel:");
			Serial.println(configRec.accel_Axis_M3);
#endif // DEBUG
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

#ifdef DEBUG
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
#endif // DEBUG
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


	switch (configRec.axisId)
	{
		case ID_AXIS_3:
		{
			if (configSetup.radialOrLinear_Axis_M3 == 1) // Linear
			{
				axisSteps = axisSteps * (-1);
			}
			break;
		}
		case ID_AXIS_4:
		{
			if (configSetup.radialOrLinear_Axis_M4 == 1) // Linear
			{
				axisSteps = axisSteps * (-1);
			}
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
	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		checkedCount = 3;
		stepperId_3 = ID_AXIS_3;
		stepper3_step = PIN_AXIS_3_STEP;
		stepper3_dir = PIN_AXIS_3_DIR;
#ifdef DEBUG
		Serial.print("spindleSteps * synchro_Ratio: ");
		Serial.println(spindleSteps * synchro_Ratio);
#endif // DEBUG
		SetEnable(ID_AXIS_3, true, true);
		stepperAxis_M3.setPosition(0);
		stepperAxis_M3
			.setMaxSpeed((configRec.maxSpd_Spindle * configRec.speedPercent_Spindle * .01))
			.setAcceleration(configRec.accel_Spindle);
		MilliDelay(5);
	}

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
			Serial.println("M3 Enabled..........................");
			break;
		}
		case ID_AXIS_4:
		{
			SetEnable(ID_AXIS_4, true, true);
			Serial.println("M4 Enabled..........................");
			break;
		}
	}

	int32_t i_mod_2 = 0;
	for (int i = 1; i <= (waves * 2) + 2; i++)
	{
		i_mod_2 = i % 2;
#ifdef DEBUG
		Serial.print("i: ");
		Serial.println(i);
		Serial.print("i%2: ");
		Serial.println(i % 2);
#endif // DEBUG
		switch(i_mod_2)
		{ 
			case 0:
			{
				switch (configRec.radial_axial)
				{
					case RADIAL: // Radial
					{
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
						if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
						{
							checkedCount = 3;
							stepperId_3 = ID_AXIS_3;
							stepper3_step = PIN_AXIS_3_STEP;
							stepper3_dir = PIN_AXIS_3_DIR;
#ifdef DEBUG
							Serial.print("Even-Axial-spindleSteps * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01):  ");
							Serial.println(-spindleSteps* synchro_Ratio* (configMain.synchro_M3_Percentage * 0.01));
#endif // DEBUG
							stepperAxis_M3.setTargetRel(-spindleSteps * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01));
							MilliDelay(5);
#ifdef DEBUG
							Serial.println("stepController.moveAsync(stepper_Spindle, stepperAxis_M3);");
#endif // DEBUG
							stepController.moveAsync(stepper_Spindle, stepperAxis_M3);
						}
						else
						{
#ifdef DEBUG
							Serial.println("stepController.moveAsync(stepper_Spindle);");
#endif // DEBUG
							stepController.moveAsync(stepper_Spindle);
						}
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
				
				if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
				{
					checkedCount = 3;
					stepperId_3 = ID_AXIS_3;
					stepper3_step = PIN_AXIS_3_STEP;
					stepper3_dir = PIN_AXIS_3_DIR;

#ifdef DEBUG
					Serial.print("Odd-Default-spindleSteps * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01):  ");
					Serial.println(spindleSteps * synchro_Ratio* (configMain.synchro_M3_Percentage * 0.01));
#endif // DEBUG
					stepperAxis_M3.setTargetRel(spindleSteps * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01));
					MilliDelay(5);
					stepController.moveAsync(stepper_Spindle, stepper_Axis, stepperAxis_M3);
				}
				else
				{
					stepController.moveAsync(stepper_Spindle, stepper_Axis);
				};
#ifdef DEBUG
				Serial.print("Spindle&Axis i: ");
				Serial.println(i);
				Serial.println(i_mod_2);
#endif // DEBUG
				break;
			}
		}

		while (stepController.isRunning())
		{
			if (limitPin_Max > 9 && limitPin_Max < 40)
			{
				if ((digitalRead(limitPin_Max) != configSetup.limit_NCorNO))// && axisSteps > 0)
				{
#ifdef DEBUG
					Serial.println("                          limitPin_Max");
#endif // DEBUG
					if (axisSteps > 0)
					{
						stepController.emergencyStop();
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
						stepController.emergencyStop();
						stopSteppers = true;
						limitTriggered = true;
						SetLimitColors(PAGE_REC, DIR_CCW);
						goto endLoop;
					}
				}
			}
#ifdef ESTOP_ENABLED
			//if (configSetup.eStop != 0)
			if (configSetup.eStop > 9 && configSetup.eStop < 40)
			{
				if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
				{
#ifdef DEBUG
					Serial.println("                          EStop");
#endif // DEBUG
					stepController.emergencyStop();
					stopSteppers = true;
					// Disable all motors
					SetEnable(ID_SPINDLE, false, false);
					SetEnable(ID_AXIS_Z, false, false);
					SetEnable(ID_AXIS_X, false, false);
					SetEnable(ID_AXIS_3, false, false);
					SetEnable(ID_AXIS_4, false, false);
					SetEStopColors(PAGE_REC);
					if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
					{
						digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
					}
					return;
				}
			}
#endif //estop

			// Check for Cancel code  
			if (SerialAvailable() > 0)
			{
				incomingByte = SerialRead(serialId);
				switch (incomingByte)
				{
				case 83: // - S
				{
					currentSpeed = stepController.getCurrentSpeed();
#ifdef DEBUG
					Serial.print("currentSpeed: ");
					Serial.println(currentSpeed);
#endif // Debug
					if (currentSpeed < MIN_SPEED)
					{
						stepController.emergencyStop();
					}
					else
					{
						stepController.stop();
					}
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
	Serial.print("Limit triggered: ");
	Serial.println(limitTriggered);
	Serial.print("Spindle:");
	Serial.println(endPosition_Spindle);
	Serial.print("Axis:");
	Serial.println(endPosition_Axis);
#endif // DEBUG



	returnSteps_Spindle = endPosition_Spindle;
	stepperPosition_1 = endPosition_Spindle * (-1);
	stepperPosition_2 = endPosition_Axis * (-1);

	Serial.print("stepperPosition_1: ");
	Serial.print(stepperPosition_1);
	Serial.print("stepperPosition_2: ");
	Serial.print(stepperPosition_2);

	SetEnable(ID_SPINDLE, false, true);
	SetEnable(configRec.axisId, false, true);
	//SetEnable(ID_AXIS_X, false, true);
	//SetEnable(ID_AXIS_3, false, true);
	//SetEnable(ID_AXIS_4, false, true);

	Serial.print("configMain.synchro_M3_Spindle: ");
	Serial.print(configMain.synchro_M3_Spindle);
	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		SetEnable(ID_AXIS_3, false, true);
		stepperPosition_3 = stepperAxis_M3.getPosition() * (-1);
		Serial.print("stepperPosition_3: ");
		Serial.print(stepperPosition_3);
	}

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
	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
	}
}

void Reciprocate_Square(int wavDir)
{
	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		digitalWrite(configSetup.cutterMotorPin, LOW);  // Enable
	}
	eStopTriggered = false;
	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);

	stepPin = 0;
	dirPin = 0;
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
		case ID_AXIS_3:
		{
			stepPin = PIN_AXIS_3_STEP;
			dirPin = PIN_AXIS_3_DIR;
			limitPin_Max = configSetup.limit_Max_M3;
			limitPin_Min = configSetup.limit_Min_M3;
			maxSpeed = configRec.maxSpd_Axis_M3 * configRec.speedPercent_Axis_M3 * .01;
			//if (configSetup.radialOrLinear_Axis_M3 == 1) // Linear
			//{
			//	maxSpeed = maxSpeed * (-1);
			//}
			accel = configRec.accel_Axis_M3;
			speedPercent = configRec.speedPercent_Axis_M3;
	#ifdef DEBUG
			Serial.print("M3---AxisMaxSpd:");
			Serial.println(configRec.maxSpd_Axis_M3);
			Serial.print("M3---AxisSpeedPercent:");
			Serial.println(configRec.speedPercent_Axis_M3);
			Serial.print("M3---AxisAccel:");
			Serial.println(configRec.accel_Axis_M3);
	#endif // DEBUG
			break;
		}
		case ID_AXIS_4:
		{
			stepPin = PIN_AXIS_4_STEP;
			dirPin = PIN_AXIS_4_DIR;
			limitPin_Max = configSetup.limit_Max_M4;
			limitPin_Min = configSetup.limit_Min_M4;
			maxSpeed = configRec.maxSpd_Axis_M4 * configRec.speedPercent_Axis_M4 * .01;
			//if (configSetup.radialOrLinear_Axis_M4 == 1) // Linear
			//{
			//	maxSpeed = maxSpeed * (-1);
			//}
			accel = configRec.accel_Axis_M4;
			speedPercent = configRec.speedPercent_Axis_M4;
#ifdef DEBUG
			Serial.print("M4---AxisMaxSpd:");
			Serial.println(configRec.maxSpd_Axis_M4);
			Serial.print("M4---AxisSpeedPercent:");
			Serial.println(configRec.speedPercent_Axis_M4);
			Serial.print("M4---AxisAccel:");
			Serial.println(configRec.accel_Axis_M4);
#endif // DEBUG
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

	switch (configRec.axisId)
	{
		case ID_AXIS_3:
		{
			if (configSetup.radialOrLinear_Axis_M3 == 1) // Linear
			{
				axisSteps = axisSteps * (-1);
			}
			break;
		}
		case ID_AXIS_4:
		{
			if (configSetup.radialOrLinear_Axis_M4 == 1) // Linear
			{
				axisSteps = axisSteps * (-1);
			}
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

	checkedCount = 2;
	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		checkedCount = 3;
		stepperId_3 = ID_AXIS_3;
		stepper3_step = PIN_AXIS_3_STEP;
		stepper3_dir = PIN_AXIS_3_DIR;
		Serial.print("spindleSteps * synchro_Ratio: ");
		Serial.println(spindleSteps * synchro_Ratio);
		SetEnable(ID_AXIS_3, true, true);
		stepperAxis_M3.setPosition(0);
		stepperAxis_M3
			.setMaxSpeed((configRec.maxSpd_Spindle * configRec.speedPercent_Spindle * .01))
			.setAcceleration(configRec.accel_Spindle);
		MilliDelay(5);
	}


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
			Serial.println("M4 Enabled..........................");
			break;
		}
		case ID_AXIS_4:
		{
			SetEnable(ID_AXIS_4, true, true);
			Serial.println("M4 Enabled..........................");
			break;
		}
	}

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


						if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
						{
							checkedCount = 3;
							stepperId_3 = ID_AXIS_3;
							stepper3_step = PIN_AXIS_3_STEP;
							stepper3_dir = PIN_AXIS_3_DIR;
							Serial.print("0 spindleSteps * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01):  ");
							Serial.println(spindleSteps* synchro_Ratio* (configMain.synchro_M3_Percentage * 0.01));
							stepperAxis_M3.setTargetRel(spindleSteps* synchro_Ratio* (configMain.synchro_M3_Percentage * 0.01));
							MilliDelay(5);
							stepController.moveAsync(stepper_Spindle, stepperAxis_M3);
						}
						else
						{
							stepController.moveAsync(stepper_Spindle, stepper_Axis);
						}
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
						if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
						{
							Serial.print("1 or 3 spindleSteps * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01):  ");
							Serial.println(spindleSteps * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01));
							stepperAxis_M3.setTargetRel(spindleSteps * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01));
							MilliDelay(5);
							stepController.moveAsync(stepper_Spindle, stepperAxis_M3);
						}
						else
						{
							stepController.moveAsync(stepper_Spindle, stepper_Axis);
						}
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
						if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
						{
							Serial.print("2 spindleSteps * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01):  ");
							Serial.println(spindleSteps* synchro_Ratio* (configMain.synchro_M3_Percentage * 0.01));
							stepperAxis_M3.setTargetRel(spindleSteps* (-1) * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01));
							MilliDelay(5);
							stepController.moveAsync(stepper_Spindle, stepperAxis_M3);
						}
						else
						{
							stepController.moveAsync(stepper_Spindle, stepper_Axis);
						}
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
			if (limitPin_Max > 9 && limitPin_Max < 40)
			{
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
			}
			if (limitPin_Min > 9 && limitPin_Min < 40)
			{
				if ((digitalRead(limitPin_Min) != configSetup.limit_NCorNO))// && axisSteps < 0)
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
			}
#ifdef ESTOP_ENABLED
			//if (configSetup.eStop != 0)
			if (configSetup.eStop > 9 && configSetup.eStop < 40)
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
					SetEnable(ID_AXIS_3, false, false);
					SetEnable(ID_AXIS_4, false, false);
					SetEStopColors(PAGE_REC);
					if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
					{
						digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
					}
					return;
				}
			}
#endif // estop
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
					currentSpeed = stepController.getCurrentSpeed();
#ifdef DEBUG
					Serial.print("currentSpeed: ");
					Serial.println(currentSpeed);
#endif // Debug
					if (currentSpeed < MIN_SPEED)
					{
						stepController.emergencyStop();
					}
					else
					{
						stepController.stop();
					}
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
	stepperPosition_1 = endPosition_Spindle * (-1);
	stepperPosition_2 = endPosition_Axis * (-1);
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
	SetEnable(configRec.axisId, false, true);
	//SetEnable(ID_AXIS_X, false, true);
	//SetEnable(ID_AXIS_3, false, true);
	//SetEnable(ID_AXIS_4, false, true);
	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		SetEnable(ID_AXIS_3, false, true);
		stepperPosition_3 = stepperAxis_M3.getPosition() * (-1);
	}

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
	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
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
	bool stopAll = false;
	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	String nextionEnd = "\xFF\xFF\xFF";

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

	for (uint32_t i = 1; i <= configGreekKey.patternCount_Pattern;i++)// / 2; i++)
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
	String nextionEnd = "\xFF\xFF\xFF";
	eStopTriggered = false;

	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);

	if (configGreekKey.radialOrAxial_Pattern == AXIAL)
	{
		totalSpindleSegments = 4;
	}
	GreekKeyPattern_Initial(totalSpindleSegments);

	for (uint32_t i = 1; i <= configGreekKey.patternCount_Pattern; i++)
	{
		if (eStopTriggered || limitTriggered)
		{
			Serial.println("GreekKey 4b eStop or limit triggered.......................");
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
	String nextionEnd = "\xFF\xFF\xFF";
	eStopTriggered = false;

	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);

	if (configGreekKey.radialOrAxial_Pattern == AXIAL)
	{
		totalSpindleSegments = 3;
	}
	GreekKeyPattern_Initial(totalSpindleSegments);

	for (uint32_t i = 1; i <= configGreekKey.patternCount_Pattern; i++)
	{
		if (eStopTriggered || limitTriggered)
		{
			Serial.println("GreekKey 3a eStop or limit triggered.......................");
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
	String nextionEnd = "\xFF\xFF\xFF";
	eStopTriggered = false;

	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);

	if (configGreekKey.radialOrAxial_Pattern == AXIAL)
	{
		totalSpindleSegments = 3;
	}
	GreekKeyPattern_Initial(totalSpindleSegments);

	for (uint32_t i = 1; i <= configGreekKey.patternCount_Pattern; i++)
	{
		if (eStopTriggered || limitTriggered)
		{
			Serial.println("GreekKey 3b eStop or limit triggered.......................");
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
//	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	int totalSpindleSegments = 3;
	limitTriggered = false;
	String nextionEnd = "\xFF\xFF\xFF";
	eStopTriggered = false;

	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);
	if (configGreekKey.radialOrAxial_Pattern == AXIAL)
	{
		totalSpindleSegments = 2;
	}

	GreekKeyPattern_Initial(totalSpindleSegments);
#ifdef DEBUG
	Serial.print("1.axisShortLegSteps:");
	Serial.println(axisShortLegSteps);
#endif // DEBUG

	for (uint32_t i = 1; i <= configGreekKey.patternCount_Pattern; i++)
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
		SerialPrint("t200.txt=");
		SerialWrite(0x22);
		SerialPrint(i);
		SerialPrint(nextionQuoteEnd);
		Serial.print(" >>>>>>>>>>>>>>>              reverseDirection: ");
		Serial.println(reverseDirection);
		switch (configGreekKey.radialOrAxial_Pattern)
		{
			case RADIAL: // Axis Left CCW
			{

				stopAll = GreekKey_Move_Axis(axisShortLegSteps, 2, reverseDirection * DIR_CW, true); //1
				if (StopGreekKey() || stopAll)
				{
					goto EndLoop;

				}
				Serial.println("DIR_CCW ");
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
				Serial.println("DIR_CW ");
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
				Serial.println("DIR_CCW ");
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
	String nextionEnd = "\xFF\xFF\xFF";
	eStopTriggered = false;

	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);

	if (configGreekKey.radialOrAxial_Pattern == AXIAL)
	{
		totalSpindleSegments = 2;
	}
	GreekKeyPattern_Initial(totalSpindleSegments);

	for (uint32_t i = 1; i <= configGreekKey.patternCount_Pattern; i++)
	{
		if (eStopTriggered || limitTriggered)
		{
			Serial.println("GreekKey 2b eStop or limit triggered.......................");
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
	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		digitalWrite(configSetup.cutterMotorPin, LOW);  // Enable
	}
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
		case ID_AXIS_3: // M3 Axis
		{
			distance_Axis = configGreekKey.segmentLength_Pattern;// / configSetup.distancePerRev_AxisB;
#ifdef DEBUG
			Serial.print("segmentLengthPattern:                 ...: ");
			Serial.println(configGreekKey.segmentLength_Pattern);
#endif // DEBUG
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
			distance_Axis = configGreekKey.segmentLength_Pattern;
#ifdef DEBUG
			Serial.print("segmentLengthPattern:                 ...: ");
			Serial.println(configGreekKey.segmentLength_Pattern);
#endif // DEBUG
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
	//const char* nextionEnd = "\xFF\xFF\xFF";

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


	degrees_Spindle = StepsToDegrees(endPosition_Spindle, ID_SPINDLE);
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
		case ID_AXIS_3: // M3 Axis
		{
			distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_3);
			SetEnable(ID_AXIS_3, false, true);
			break;
		}
		case ID_AXIS_4: // M4 Axis
		{
			distance_Axis = StepsToDistance_Axis(endPosition_Axis, ID_AXIS_4);
			SetEnable(ID_AXIS_4, false, true);
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
	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
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
	//int newDirection = direction; //* reverseDirection;

	//Serial.print("newDirection: ");
	//Serial.println(newDirection);
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
			case ID_AXIS_3: // Assumes the toolpath is on the outside of the workpiece generally moving in Z axis
			{

				currentAxis = ID_AXIS_X;
#ifdef DEBUG
				Serial.print("X-CurrentAxis:  ");
				Serial.println(currentAxis);
#endif // Debug
				break;
			}
			case ID_AXIS_4: // Assumes the toolpath is on the outside of the workpiece generally moving in Z axis
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
//#ifdef DEBUG
//	Serial.print("Enter GreekKey_Move_Axis.......................:");
//	Serial.println(currentAxis);
//	Serial.print("Move_Axis:segmentSteps ");
//	Serial.println(segmentSteps);
//	Serial.print("Move_Axis:multiplier ");
//	Serial.println(multiplier);
//	Serial.print("Move_Axis:direction ");
//	Serial.println(direction);
//	Serial.print("Move_Axis:updatePosition ");
//	Serial.println(updatePosition);
//#endif // DEBUG


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
			maxSpeed = configGreekKey.maxSpd_Axis_M3;
			accel = configGreekKey.accel_Axis_M3;
			speedPercentage = configGreekKey.speedPercent_Axis_M3;
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
			maxSpeed = configGreekKey.maxSpd_Axis_M4;

			accel = configGreekKey.accel_Axis_M4;
			speedPercentage = configGreekKey.speedPercent_Axis_M4;
			break;
		}
	}

	stepperId_2 = configGreekKey.axisId;
	stepper2_step = stepPin;
	stepper2_dir = dirPin;
	// Set limit switches HIGH
	digitalWrite(limitPin_Max, HIGH);
	digitalWrite(limitPin_Min, HIGH);

//	uint8_t maxLimitSetting = digitalRead(limitPin_Min);
	//Serial.print("maxLimitSetting:                   ");
	//Serial.println(maxLimitSetting);

	actualSpeed = maxSpeed * speedPercentage * .01;
	StepControl stepControllerAxis;
	Stepper stepperAxis(stepPin, dirPin);
	float targetSteps = (segmentSteps * multiplier * direction);

	targetSteps = round(targetSteps);

	switch (currentAxis)
	{
		case ID_AXIS_3:
		{
			if (configSetup.radialOrLinear_Axis_M3 == 1) // Linear
			{
				targetSteps = targetSteps * (-1);
			}
			break;
		}
		case ID_AXIS_4:
		{
			if (configSetup.radialOrLinear_Axis_M4 == 1) // Linear
			{
				targetSteps = targetSteps * (-1);
			}
			break;
		}

	}

	checkedCount = 2;
	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		checkedCount = 3;
		stepperId_3 = ID_AXIS_3;
		stepper3_step = PIN_AXIS_3_STEP;
		stepper3_dir = PIN_AXIS_3_DIR;
	}


	stepperAxis
		.setMaxSpeed(actualSpeed)
		.setAcceleration(accel)
		.setTargetRel(targetSteps);

	stepperAxis.setPosition(0);

//#ifdef DEBUG
//	Serial.print("+++");
//	Serial.print(axisName);
//	Serial.print("-------------------------");
//	Serial.print(axisName);
//	Serial.println("+++");
//	Serial.print("segmentSteps: ");
//	Serial.println(segmentSteps);
//	Serial.print("multiplier: ");
//	Serial.println(multiplier);
//	Serial.print("direction: ");
//	Serial.println(direction);
//	Serial.print("targetSteps: ");
//	Serial.println(targetSteps);
//	Serial.print("actualSpeed: ");
//	Serial.println(actualSpeed);
//	Serial.print("MaxSpeed: ");
//	Serial.println(maxSpeed);
//	Serial.print("Acceleration: ");
//	Serial.println(accel);
//	Serial.print("speedPercentage: ");
//	Serial.println(speedPercentage);
//	Serial.print("===");
//	Serial.print(axisName);
//	Serial.print("-------------------------");
//	Serial.print(axisName);
//	Serial.println("===");
//
//#endif // DEBUG

	stepControllerAxis.moveAsync(stepperAxis);
	while (stepControllerAxis.isRunning())
	{
		if (limitPin_Max > 9 && limitPin_Max < 40)
		{
			if ((digitalRead(limitPin_Max) != configSetup.limit_NCorNO))// && axisSteps > 0)
			{
				Serial.println("       a                   limitPin_Max");
				if (targetSteps > 0)
				{

					stepControllerAxis.emergencyStop();
					stopSteppers = true;
					SetEnable(ID_SPINDLE, false, true);
					SetEnable(ID_AXIS_Z, false, true);
					SetEnable(ID_AXIS_X, false, true);
					SetEnable(ID_AXIS_3, false, true);
					SetEnable(ID_AXIS_4, false, true);
					SetLimitColors(PAGE_GRK, DIR_CW);
					if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
					{
						digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
					}
					//goto endLoop;
					return true;
				}
			}
		}
		if (limitPin_Min > 9 && limitPin_Min < 40)
		{
			if ((digitalRead(limitPin_Min) != configSetup.limit_NCorNO))// && axisSteps < 0)
			{
				Serial.println("                          limitPin_Min");
				if (targetSteps < 0)
				{
					stepControllerAxis.emergencyStop();
					stopSteppers = true;
					SetEnable(ID_SPINDLE, false, true);
					SetEnable(ID_AXIS_Z, false, true);
					SetEnable(ID_AXIS_X, false, true);
					SetEnable(ID_AXIS_3, false, true);
					SetEnable(ID_AXIS_4, false, true);
					SetLimitColors(PAGE_GRK, DIR_CCW);
					if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
					{
						digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
					}
					return true;
				}
			}
		}

#ifdef ESTOP_ENABLED
		//if (configSetup.eStop != 0)
		if (configSetup.eStop > 9 && configSetup.eStop < 40)
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
				SetEnable(ID_AXIS_3, false, false);
				SetEnable(ID_AXIS_4, false, false);
				SetEStopColors(PAGE_GRK);
				if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
				{
					digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
				}
				return true;
			}
		}
#endif // estop
		if (StopGreekKey())
		{
#ifdef DEBUG
			Serial.println("StopGreekKey() is true:");
#endif // DEBUG

			currentSpeed = stepControllerAxis.getCurrentSpeed();
#ifdef DEBUG
			Serial.print("currentSpeed: ");
			Serial.println(currentSpeed);
#endif // Debug
			if (currentSpeed < MIN_SPEED)
			{
				stepControllerAxis.emergencyStop();
			}
			else
			{
				stepControllerAxis.stop();
			}
			// Disable all motors
			//SetEnable(ID_SPINDLE, false, true);
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
//#ifdef DEBUG
//	Serial.print("endPosition_Axis:");
//	Serial.println(endPosition_Axis);
//#endif // DEBUG
	if (updatePosition)
	{
		// Update position only for R(ight) and L(eft), not O(ut) or I(n).
		endPosition_Axis = endPosition_Axis + stepperAxis.getPosition();
		stepperPosition_2 = endPosition_Axis * (-1);
	}
//#ifdef DEBUG
//	Serial.print("endPosition_Axis:");
//	Serial.println(endPosition_Axis);
//#endif // DEBUG
//
//#ifdef DEBUG
//	Serial.print("axisId:               ....");
//	Serial.println(currentAxis);
//	Serial.println("Exit GreekKey_Move_Axis");
//#endif // DEBUG

	return retVal;
}


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
		case ID_AXIS_3:
		{
			stepPin_Axis = PIN_AXIS_3_STEP;
			dirPin_Axis = PIN_AXIS_3_DIR;
			limitPin_Max = configSetup.limit_Max_M3;
			limitPin_Min = configSetup.limit_Min_M3;
			SetEnable(ID_AXIS_3, true, true);

			maxSpd_Axis = configGreekKey.maxSpd_Axis_M3;

			accel_Axis = configGreekKey.accel_Axis_M3;
			speedPercent_Axis = configGreekKey.speedPercent_Axis_M3;
#ifdef DEBUG
			Serial.print("MaxSpd:");
			Serial.println(configGreekKey.maxSpd_Axis_M3);
			Serial.print("Accel:");
			Serial.println(configGreekKey.accel_Axis_M3);
			Serial.print("SpeedPercent:");
			Serial.println(configGreekKey.speedPercent_Axis_M3);
#endif // DEBUG
			break;
		}
		case ID_AXIS_4:
		{
			stepPin_Axis = PIN_AXIS_4_STEP;
			dirPin_Axis = PIN_AXIS_4_DIR;
			limitPin_Max = configSetup.limit_Max_M4;
			limitPin_Min = configSetup.limit_Min_M4;
			SetEnable(ID_AXIS_4, true, true);

			maxSpd_Axis = configGreekKey.maxSpd_Axis_M4;

			accel_Axis = configGreekKey.accel_Axis_M4;
			speedPercent_Axis = configGreekKey.speedPercent_Axis_M4;
#ifdef DEBUG
			Serial.print("MaxSpd:");
			Serial.println(configGreekKey.maxSpd_Axis_M4);
			Serial.print("Accel:");
			Serial.println(configGreekKey.accel_Axis_M4);
			Serial.print("SpeedPercent:");
			Serial.println(configGreekKey.speedPercent_Axis_M4);
#endif // DEBUG
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
	
	StepControl stepController;
	Stepper stepper_Axis(stepPin_Axis, dirPin_Axis);
	stepper_Axis
		.setMaxSpeed(maxSpd_Axis * currentSpeedPercent_Axis)
		.setAcceleration(accel_Axis);
	stepper_Axis.setPosition(0);


	//int32_t targetSteps_Axis = round(abs(shortLegLength_Axis) * multiplier_Axis);
	int32_t targetSteps_Axis = round((shortLegLength_Axis) * multiplier_Axis);


	switch (configGreekKey.axisId)
	{
		case ID_AXIS_3:
		{
			if (configSetup.radialOrLinear_Axis_M3 == 1) // Linear
			{
				targetSteps_Axis = targetSteps_Axis * (-1);
			}
			break;
		}
		case ID_AXIS_4:
		{
			if (configSetup.radialOrLinear_Axis_M4 == 1) // Linear
			{
				targetSteps_Axis = targetSteps_Axis * (-1);
			}
			break;
		}
	}




	Serial.print("shortLegLength_Axis: ");
	Serial.print(shortLegLength_Axis);
	Serial.print("   multiplier_Axis: ");
	Serial.print(multiplier_Axis);
	Serial.print("   Axis targetSteps: ");
	Serial.println(targetSteps_Axis);

	stepperId_1 = ID_SPINDLE;
	stepper1_step = PIN_SPINDLE_STEP;
	stepper1_dir = PIN_SPINDLE_DIR;

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
	checkedCount = 2;
	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		checkedCount = 3;
		stepperId_3 = ID_AXIS_3;
		stepper3_step = PIN_AXIS_3_STEP;
		stepper3_dir = PIN_AXIS_3_DIR;
		Serial.print("targetSteps_Spindle * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01):  ");
		Serial.println(targetSteps_Spindle* synchro_Ratio* (configMain.synchro_M3_Percentage * 0.01));
		Serial.print("targetSteps_Spindle:  ");
		Serial.println(targetSteps_Spindle);

		Serial.print("synchro_Ratio:  ");
		Serial.println(synchro_Ratio);

		Serial.print("configMain.synchro_M3_Percentage * 0.01:  ");
		Serial.println(configMain.synchro_M3_Percentage * 0.01);

		stepperAxis_M3.setPosition(0);
		stepperAxis_M3.setTargetRel(targetSteps_Spindle* synchro_Ratio* (configMain.synchro_M3_Percentage * 0.01));
		stepperAxis_M3.setMaxSpeed(configGreekKey.maxSpd_Axis_M3* configGreekKey.speedPercent_Axis_M3 * .01);
		stepperAxis_M3.setAcceleration(configGreekKey.accel_Axis_M3);
		SetEnable(ID_AXIS_3, true, true);
		MilliDelay(5);

		stepController.moveAsync(stepperSpindle, stepper_Axis, stepperAxis_M3);
	}
	else
	{
		stepController.moveAsync(stepperSpindle, stepper_Axis);
	}



	//stepController.moveAsync(stepperSpindle,stepper_Axis);
	while (stepController.isRunning())
	{
		if (limitPin_Max > 9 && limitPin_Max < 40)
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
				SetEnable(ID_AXIS_3, false, true);
				SetEnable(ID_AXIS_4, false, true);
				SetLimitColors(PAGE_GRK, DIR_CW);
				if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
				{
					digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
				}
				return true;
				//break;

			}
		}
		if (limitPin_Min > 9 && limitPin_Min < 40)
		{
			if (digitalRead(limitPin_Min) != configSetup.limit_NCorNO)
			{
				Serial.println("                          limitPin");

				stepController.emergencyStop();
				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, true);
				SetEnable(ID_AXIS_Z, false, true);
				SetEnable(ID_AXIS_X, false, true);
				SetEnable(ID_AXIS_3, false, true);
				SetEnable(ID_AXIS_4, false, true);
				SetLimitColors(PAGE_GRK, DIR_CCW);
				if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
				{
					digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
				}
				return true;
				//break;

			}
		}

#ifdef ESTOP_ENABLED
		//if (configSetup.eStop != 0)
		if (configSetup.eStop > 9 && configSetup.eStop < 40)
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
				SetEnable(ID_AXIS_3, false, false);
				SetEnable(ID_AXIS_4, false, false);
				SetEStopColors(PAGE_GRK);
				if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
				{
					digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
				}
				return true;
			}
		}
#endif // estop
		if (StopGreekKey())
		{
#ifdef DEBUG
			Serial.println("StopGreekKey() is true:");
#endif // DEBUG
			currentSpeed = stepController.getCurrentSpeed();
#ifdef DEBUG
			Serial.print("currentSpeed: ");
			Serial.println(currentSpeed);
#endif // Debug
			if (currentSpeed < MIN_SPEED)
			{
				stepController.emergencyStop();
			}
			else
			{
				stepController.stop();
			}
			retVal = true;
			break;
		}
		MilliDelay(5);
	}



	SetEnable(ID_SPINDLE, false, true);

	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		SetEnable(ID_AXIS_3, false, true);
		endPosition_M3 = endPosition_M3 +  stepperAxis_M3.getPosition();
		stepperPosition_3 = endPosition_M3 * (-1);
	}





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


#ifdef DEBUG
	Serial.print("endPosition_Axis:");
	Serial.println(endPosition_Axis);
#endif // DEBUG

	endPosition_Axis = endPosition_Axis + stepper_Axis.getPosition();
	stepperPosition_2 = endPosition_Axis * (-1);
	endPosition_Spindle = endPosition_Spindle + stepperSpindle.getPosition();
	stepperPosition_1 = endPosition_Spindle * (-1);

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

	int32_t mSpeed = configGreekKey.maxSpd_Spindle * configGreekKey.speedPercent_Spindle * .01;

	//Stepper stepperSpindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
	//stepperSpindle.setPosition(0);

	stepperSpindle.setAcceleration(configGreekKey.accel_Spindle);
	stepperSpindle.setMaxSpeed(mSpeed);

	stepperId_1 = ID_SPINDLE;
	stepper1_step = PIN_SPINDLE_STEP;
	stepper1_dir = PIN_SPINDLE_DIR;

	float targetSteps = segmentSteps * multiplier * direction;
	Serial.print("targetSteps-float:  ");
	Serial.println(targetSteps);
	float targetStepsRounded = round(targetSteps);
	
	targetSteps = targetStepsRounded;
	float targetSteps_M3Synchro = round(targetSteps * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01));

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

	stepperSpindle.setTargetRel(targetSteps);

	checkedCount = 2;
	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		checkedCount = 3;
		stepperId_3 = ID_AXIS_3;
		stepper3_step = PIN_AXIS_3_STEP;
		stepper3_dir = PIN_AXIS_3_DIR;
		Serial.print("round(targetSteps * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01)):  ");
		Serial.println(targetSteps_M3Synchro);
		Serial.print("targetSteps:  ");
		Serial.println(targetSteps);

		Serial.print("synchro_Ratio:  ");
		Serial.println(synchro_Ratio);

		Serial.print("configMain.synchro_M3_Percentage * 0.01:  ");
		Serial.println(configMain.synchro_M3_Percentage * 0.01);

		stepperAxis_M3.setPosition(0);
		stepperAxis_M3.setTargetRel(targetSteps_M3Synchro);
		//stepperAxis_M3.setMaxSpeed(direction * configGreekKey.maxSpd_Axis_M3 * configGreekKey.speedPercent_Axis_M3 * .01);
		stepperAxis_M3.setMaxSpeed(mSpeed*direction);
		stepperAxis_M3.setAcceleration(configGreekKey.accel_Axis_M3);
		SetEnable(ID_AXIS_3, true, true);
		MilliDelay(5);
		stepControllerSpindle.moveAsync(stepperSpindle, stepperAxis_M3);
	}
	else
	{
		stepControllerSpindle.moveAsync(stepperSpindle);
	}



	while (stepControllerSpindle.isRunning())
	{
#ifdef ESTOP_ENABLED
		//if (configSetup.eStop != 0)
		if (configSetup.eStop > 9 && configSetup.eStop < 40)
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
				SetEnable(ID_AXIS_3, false, false);
				SetEStopColors(PAGE_GRK);
				if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
				{
					digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
				}
				return true;
			}
		}
#endif // estop
		if (SerialAvailable() >= 0)
		{
			incomingByte = SerialRead(serialId);
			switch (incomingByte)
			{
				case 83: // - S
				{
					currentSpeed = stepControllerSpindle.getCurrentSpeed();
#ifdef DEBUG
					Serial.print("currentSpeed: ");
					Serial.println(currentSpeed);
#endif // Debug
					if (currentSpeed < MIN_SPEED)
					{
						stepControllerSpindle.emergencyStop();
					}
					else
					{
						stepControllerSpindle.stop();
					}

					SetEnable(ID_SPINDLE, false, true);

					retVal = true;
					break;
				}

				case 110: // Change Spindle Speed
				{
					GetSpindleSpeed();
					mSpeed = configGreekKey.maxSpd_Spindle * configGreekKey.speedPercent_Spindle * .01;


					stepperSpindle.setMaxSpeed(mSpeed);
					break;
				}
				case 111: // Change Axis Speed
				{
					GetAxisSpeed();
				}
			}
		}

		

		MilliDelay(5);
	}
//#ifdef DEBUG
//	Serial.print("endPosition_Spindle:");
//	Serial.println(endPosition_Spindle);
//#endif // DEBUG
//
//
	endPosition_Spindle = endPosition_Spindle + stepperSpindle.getPosition();
	stepperPosition_1 = endPosition_Spindle * (-1);
//#ifdef DEBUG
//	Serial.print("endPosition_Spindle:");
//	Serial.println(endPosition_Spindle);
//	Serial.println("Exit GreekKey_Move_Spindle.......................");
//#endif // DEBUG
	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		SetEnable(ID_AXIS_3, false, true);
		stepperPosition_3 = stepperAxis_M3.getPosition() * (-1);
	}
	return retVal;
}

/// <summary>
/// GreekKeyFromFile
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
	int shortSegmentStepsAxisM3 = 0;
	int shortSegmentStepsAxisM4 = 0;
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

	switch (configSetup.radialOrLinear_Axis_M3)
	{
		case RADIAL_M3:
		{

			shortSegmentStepsAxisM3 = (int)((DistanceToSteps_M3Radial(configGreekKey.segmentLength_File)));
		}
		case LINEAR_M3:
		{

			shortSegmentStepsAxisM3 = (int)((DistanceToSteps_M3Linear(configGreekKey.segmentLength_File)));
		}
	}

	switch (configSetup.radialOrLinear_Axis_M4)
	{
	case RADIAL_M4:
	{

		shortSegmentStepsAxisM4 = (int)((DistanceToSteps_M4Radial(configGreekKey.segmentLength_File)));
	}
	case LINEAR_M4:
	{

		shortSegmentStepsAxisM4 = (int)((DistanceToSteps_M3Linear(configGreekKey.segmentLength_File)));
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
		case ID_AXIS_3: // M3 Axis
		{

			shortSegmentStepsAxis = shortSegmentStepsAxisM3;
#ifdef DEBUG
			Serial.print("configGreekKey_Main.patternCount_File:");
			Serial.println(configGreekKey.patternCount_File);
			Serial.print("segmentLength_File:");
			Serial.println(configGreekKey.segmentLength_File);
			Serial.print("configSetup.distancePerRev_AxisM3:");
			Serial.println(configSetup.distancePerRev_AxisM3);
			Serial.print("configSetup.steps360_Axis_M3:");
			Serial.println(configSetup.steps360_Axis_M3);
			Serial.print("configSetup.microsteps_Axis_M3:");
			Serial.println(configSetup.microsteps_Axis_M3);
			Serial.print("shortSegmentStepsAxisM3:");
			Serial.println(shortSegmentStepsAxisM3);
#endif // DEBUG
			break;
		}
		case ID_AXIS_4: // M4 Axis
		{

			shortSegmentStepsAxis = shortSegmentStepsAxisM4;
#ifdef DEBUG
			Serial.print("configGreekKey_Main.patternCount_File:");
			Serial.println(configGreekKey.patternCount_File);
			Serial.print("segmentLength_File:");
			Serial.println(configGreekKey.segmentLength_File);
			Serial.print("configSetup.distancePerRev_AxisM4:");
			Serial.println(configSetup.distancePerRev_AxisM4);
			Serial.print("configSetup.steps360_Axis_M4:");
			Serial.println(configSetup.steps360_Axis_M4);
			Serial.print("configSetup.microsteps_Axis_M4:");
			Serial.println(configSetup.microsteps_Axis_M4);
			Serial.print("shortSegmentStepsAxisM4:");
			Serial.println(shortSegmentStepsAxisM4);
#endif // DEBUG
			break;
		}
	}



	for (uint32_t j = 1; j <= configGreekKey.patternCount_File; j++)
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

			int moveDirection_H = DIR_CW;

			int moveDirection_V = DIR_CW;

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
									case ID_AXIS_3: // M3 Axis is primary
									{
										////stopAll = GreekKey_Move_Angular_TeensyStep(shortSegmentStepsAxisB, abs(hv_AxisLegLength) * moveDirection_H, spindleShortLegSteps, abs(hv_SpindleLegLength) * moveDirection_V);
										stopAll = GreekKey_Move_Angular_TeensyStep(shortSegmentStepsAxisM3, (hv_AxisLegLength) * moveDirection_H, spindleShortLegSteps, (hv_SpindleLegLength) * moveDirection_V);
										break;
									}
									case ID_AXIS_4: // M4 Axis is primary
									{
										stopAll = GreekKey_Move_Angular_TeensyStep(shortSegmentStepsAxisM4, (hv_AxisLegLength)*moveDirection_H, spindleShortLegSteps, (hv_SpindleLegLength)*moveDirection_V);
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
									case ID_AXIS_3: // 
									{
										stopAll = GreekKey_Move_Angular_TeensyStep(shortSegmentStepsAxisM3, hv_SpindleLegLength * direction, spindleShortLegSteps, hv_AxisLegLength);
										break;
									}
									case ID_AXIS_4: // 
									{
										stopAll = GreekKey_Move_Angular_TeensyStep(shortSegmentStepsAxisM4, hv_SpindleLegLength * direction, spindleShortLegSteps, hv_AxisLegLength);
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
							case ID_AXIS_3: // M3 Axis is primary
							{
								//stopAll = GreekKey_Move_Axis_Alt(shortSegmentStepsAxisX, segmentMultiplier, DIR_CCW);
								stopAll = GreekKey_Move_Axis(shortSegmentStepsAxisX, segmentMultiplier, DIR_CCW, false);
								SetEnable(ID_AXIS_X, false, true); // Allow depth of cut to be changed
								break;
							}
							case ID_AXIS_4: // M4 Axis is primary
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
				// ToDo M4Radius
				case 74: // J - M3Radius 
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
					else if (enableDisableStepperID == "M3") // ToDo Verify
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
					else if (enableDisableStepperID == "M4")// ToDo Verify
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
				// ToDo: Add M3 and M4
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
						////int stepsPerRevolution = (int)round(configSetup.microsteps_Spindle * configSetup.steps360_Spindle * configSetup.gearRatio_Spindle);
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
					//SerialPrint(pageProgram_t21);
					//SerialWrite(0x22);
					//SerialPrint("Out");
					//SerialPrint(nextionQuoteEnd);
					if (configGreekKey.segmentOrActual == 2) // 2: Segment  
					{
						switch (configGreekKey.axisId)
						{
							case ID_AXIS_Z: // Z Axis is primary, so use X to move cutter away from workpiece
							case ID_AXIS_3: // Assumes toolpath is on the outside of the workpiece
							case ID_AXIS_4: // Assumes toolpath is on the outside of the workpiece
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
				// ToDo: Add M4 to commands
				case 66: // B - B axis
				{
					selectedAxis = configGreekKey.axisId;
					configGreekKey.axisId = ID_AXIS_3;
					if (configGreekKey.segmentOrActual == 2) // 2: Segment  
					{
						stopAll = GreekKey_Move_Axis(shortSegmentStepsAxisM3, segmentMultiplier, DIR_CW, true);
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
	SetEnable(ID_AXIS_3, false, true);
	SetEnable(ID_AXIS_4, false, true);

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
#ifndef TEENSY_32

		case 4:
		{
			Serial4.print(text);
			break;
		}
#endif // !TEENSY_32
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
	//case 2:
	//{
	//	Serial2.print(number, trim);
	//	break;
	//}
	case 3:
	{
		Serial3.print(number, trim);
		break;
	}
#ifndef TEENSY_32
	case 4:
	{
		Serial4.print(number, trim);
		break;
	}
#endif // Teensy_32
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
#ifndef TEENSY_32
		case 4:
		{
			Serial4.write(text);
			break;
		}
#endif //Teensy 32
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
#ifndef TEENSY_32
	Serial4.println(text);
#endif // Teensy 32
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
#ifndef TEENSY_32
		case 4:
		{
			incomingData = Serial4.read();
			break;
		}
#endif // Teensy_32
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
#ifndef TEENSY_32
	else if (Serial4.available() > 0)
	{
		serialId = 4;
	}
#endif // Teensy 32
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
			// Get length of filename
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

		// Store filename
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

	Serial.print("fileIndex");
	Serial.println(fileIndex);
	Serial.print("lastFileIndex");
	Serial.println(lastFileIndex);

	// Reset lastFileIndex
	// fileIndex is set to 0 when file dropdown is first opened
	// Up and Down arrows reset the fileIndex 
	if (fileIndex == 0)
	{
		// Default maximum files is 100
		lastFileIndex = 100;
	}

	//////////////////////////////////////////////

	// Only 7 files can be shown at one time
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


String GetFilenameFromEEPROM()
{
	int filename_Length;


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

	String programFilename(arChar);

	// Open file
	Serial.print("arChar:  ");
	Serial.println(arChar);

	return programFilename;


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
		//ToDo Add M4
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
		// ToDo Add M4
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
	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		digitalWrite(configSetup.cutterMotorPin, LOW);  // Enable
	}
	eStopTriggered = false;
	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);

	// Get shortest path to the spindle start position.
	int32_t target_Spindle = returnSteps_Spindle % spindleStepsPerRev * (-1);

	StepControl stepController;
	Stepper stepper_Spindle(PIN_SPINDLE_STEP, PIN_SPINDLE_DIR);
	Stepper stepper_Z(PIN_AXIS_Z_STEP, PIN_AXIS_Z_DIR);
	Stepper stepper_X(PIN_AXIS_X_STEP, PIN_AXIS_X_DIR);
	Stepper stepper_M3(PIN_AXIS_3_STEP, PIN_AXIS_3_DIR);
	Stepper stepper_M4(PIN_AXIS_4_STEP, PIN_AXIS_4_DIR);

	// Spindle uses relative position to start from.
	stepper_Spindle.setPosition(0);

	MilliDelay(15);
	stepper_Spindle
		.setMaxSpeed(configSetup.maxSpd_Return_Spindle)
		.setAcceleration(configSetup.accel_Return_Spindle)
		.setTargetRel(target_Spindle);

	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		stepper_M3
			.setMaxSpeed(configSetup.maxSpd_Return_Spindle)
			.setAcceleration(configSetup.accel_Return_Spindle)
			.setTargetRel(target_Spindle);
		SetEnable(ID_AXIS_3, true, true);
	}


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
	case ID_AXIS_3:
	{
		// Axis uses absolute position
		// Ensure axis position is set to end position of last routine.
		stepper_M3.setPosition(endPosition_Axis);
		MilliDelay(20);
#ifdef DEBUG
		Serial.print("M3 position: ");
		Serial.println(stepper_M3.getPosition());
#endif // Debug
		stepper_M3
			.setMaxSpeed(configSetup.maxSpd_Return_Axis_M3)
			.setAcceleration(configSetup.accel_Return_Axis_M3)
			.setTargetAbs(startPositionAbs_Axis);
#ifdef DEBUG
		Serial.print("maxSpd_Return_Axis_M3: ");
		Serial.println(configSetup.accel_Return_Axis_M3);
#endif // DEBUG
		SetEnable(ID_AXIS_3, true, true);
		if (pageCallerId == PAGE_MAIN)
		{
			// Only move the M3 Axis
			stepController.moveAsync(stepper_M3);
		}
		else
		{
			stepController.moveAsync(stepper_M3, stepper_Spindle);
		}

		break;
	}
	case ID_AXIS_4:
	{
		// Axis uses absolute position
		// Ensure axis position is set to end position of last routine.
		stepper_M4.setPosition(endPosition_Axis);
		MilliDelay(20);
#ifdef DEBUG
		Serial.print("M4 position: ");
		Serial.println(stepper_M4.getPosition());
#endif // Debug
		stepper_M4
			.setMaxSpeed(configSetup.maxSpd_Return_Axis_M4)
			.setAcceleration(configSetup.accel_Return_Axis_M4)
			.setTargetAbs(startPositionAbs_Axis);
#ifdef DEBUG
		Serial.print("maxSpd_Return_Axis_M4: ");
		Serial.println(configSetup.accel_Return_Axis_M4);
#endif // DEBUG
		SetEnable(ID_AXIS_4, true, true);
		if (pageCallerId == PAGE_MAIN)
		{
			// Only move the M4 Axis
			stepController.moveAsync(stepper_M4);
		}
		else
		{
			stepController.moveAsync(stepper_M4, stepper_Spindle);
		}

		break;
	}
	case ID_SPINDLE:
	{
		if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
		{
			stepController.moveAsync(stepper_Spindle, stepper_M3);
		}
		else
		{ 
			stepController.moveAsync(stepper_Spindle);
		}
		break;
	}
	}

	while (stepController.isRunning())
	{
#ifdef ESTOP_ENABLED
		//if (configSetup.eStop != 0)
		if (configSetup.eStop > 9 && configSetup.eStop < 40)
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
				SetEnable(ID_AXIS_3, false, false);
				SetEnable(ID_AXIS_4, false, false);
				SetEStopColors(PAGE_RETURNS);
				if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
				{
					digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
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
					currentSpeed = stepController.getCurrentSpeed();
#ifdef DEBUG
					Serial.print("currentSpeed: ");
					Serial.println(currentSpeed);
#endif // Debug
					if (currentSpeed < MIN_SPEED)
					{
						stepController.emergencyStop();
					}
					else
					{
						stepController.stop();
					}
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
		case ID_AXIS_3:
		{
			endPosition_Axis = stepper_M3.getPosition();
			break;
		}
		case ID_AXIS_4:
		{
			endPosition_Axis = stepper_M4.getPosition();
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
	case PAGE_MULTI:  
	{
		SerialPrint("pageMulti.bt1.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageMulti.bt1.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageMulti.va0.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageMulti.bt0.val=0");
		SerialPrint(nextionEnd);

		Serial.print("configMulti.axisId: ");
		Serial.println(configMulti.axisId);
		//ToDo: ******************

		if (configSetup.keepSteppersEnabled != 1)
		{
			Serial.print("axisId:  ");
			Serial.println(axisId);

			switch (axisId)
				{
				case ID_AXIS_Z:
				{

					SerialPrint("pageMulti.bt1301.val=1");
					SerialPrint(nextionEnd);
					SerialPrint("pageMulti.bt1301.txt=");
					SerialWrite(0x22);
					SerialPrint("Disabled");
					SerialPrint(nextionQuoteEnd);
					SetEnable(ID_AXIS_Z, false, true);
					break;
				}
				case ID_AXIS_X:
				{
					SerialPrint("pageMulti.bt1302.val=1");
					SerialPrint(nextionEnd);
					SerialPrint("pageMulti.bt1302.txt=");
					SerialWrite(0x22);
					SerialPrint("Disabled");
					SerialPrint(nextionQuoteEnd);
					SetEnable(ID_AXIS_X, false, true);
					break;
				}
				case ID_AXIS_3:
				{
					SerialPrint("pageMulti.bt1303.val=1");
					SerialPrint(nextionEnd);
					SerialPrint("pageMulti.bt1303.txt=");
					SerialWrite(0x22);
					SerialPrint("Disabled");
					SerialPrint(nextionQuoteEnd);
					SetEnable(ID_AXIS_3, false, true);
					break;
				}
				case ID_AXIS_4:
				{
					SerialPrint("pageMulti.bt1304.val=1");
					SerialPrint(nextionEnd);
					SerialPrint("pageMulti.bt1304.txt=");
					SerialWrite(0x22);
					SerialPrint("Disabled");
					SerialPrint(nextionQuoteEnd);
					SetEnable(ID_AXIS_4, false, true);
					break;
				}
				case ID_SPINDLE:
				{
					SerialPrint("pageMulti.bt1300.val=1");
					SerialPrint(nextionEnd);
					SerialPrint("pageMulti.bt1300.txt=");
					SerialWrite(0x22);
					SerialPrint("Disabled");
					SerialPrint(nextionQuoteEnd);
					SetEnable(ID_SPINDLE, false, true);
					break;
				}
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
			case ID_AXIS_3:
			{
				stepper_M3.setPosition(0);
				endPosition_Axis = 0;
				break;
			}
			case ID_AXIS_4:
			{
				stepper_M4.setPosition(0);
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
		stepper_M3.setPosition(0);
		stepper_M4.setPosition(0);
		endPosition_Axis = 0;
		endPosition_Spindle = 0;
		returnSteps_Spindle = 0;
	}

	SetEnable(ID_SPINDLE, false, true);
	SetEnable(axisId, false, true);
	//SetEnable(ID_AXIS_X, false, true);
	//SetEnable(ID_AXIS_3, false, true);
	//SetEnable(ID_AXIS_4, false, true);
	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		SetEnable(ID_AXIS_3, false, true);
	}

	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
	}
}

/// <summary>
/// PageMulti: Return Spindle and/or axes to start position
/// </summary>
/// <comment>
/// </comment>
/// <param name="axisId">Id of axis to move</param>
void ReturnToStartPosition_Multi()
{
	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		digitalWrite(configSetup.cutterMotorPin, LOW);  // Enable
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
#ifdef DEBUG



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
#endif // DEBUG
	StepControl stepController;
	Stepper stepper_1(stepper1_step, stepper1_dir);
	Stepper stepper_2(stepper2_step, stepper2_dir);
	Stepper stepper_3(stepper3_step, stepper3_dir);
	Stepper stepper_4(stepper4_step, stepper4_dir);
	Stepper stepper_5(stepper5_step, stepper5_dir);
	stepper_1.setPosition(0);
	stepper_2.setPosition(0);
	stepper_3.setPosition(0);
	stepper_4.setPosition(0);
	stepper_5.setPosition(0);

	Serial.print("checkedCount : ");
	Serial.println(checkedCount);

	// Set unused stepper values
	switch (checkedCount)
	{
	case 1:
	{
		Serial.print("   Case 1");

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
		Serial.print("   Case 2");

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
		Serial.print("   Case 3");

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
		Serial.print("   Case 4");

		stepperId_5 = 47;

		stepper5_step = 56;
		stepper5_dir = 57;

		break;
	}
	}

#ifdef DEBUG



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
#endif // DEBUG
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

	break;
	}
#ifdef DEBUG

	Serial.print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>stepperId_2: ");
	Serial.println(stepperId_2);
#endif // DEBUG
	switch (stepperId_2)
	{

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

	break;
	}
	switch (stepperId_3)
	{
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
		// Spindle uses relative position to start from.
		//stepper_Spindle.setPosition(0);
		MilliDelay(15);
		//stepper_Spindle
		//.setMaxSpeed(configSetup.maxSpd_Return_Spindle)
			//.setAcceleration(configSetup.accel_Return_Spindle)
			//.setTargetRel(target_Spindle);
		break;
	}

	break;
	}
	switch (stepperId_4)
	{
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
		// Spindle uses relative position to start from.
		//stepper_Spindle.setPosition(0);
		MilliDelay(15);
		//stepper_Spindle
		//.setMaxSpeed(configSetup.maxSpd_Return_Spindle)
			//.setAcceleration(configSetup.accel_Return_Spindle)
			//.setTargetRel(target_Spindle);
		break;
	}

	break;
	}
	switch (stepperId_5)
	{
	case ID_SPINDLE:
	{
		maxSpeed_1 = configSetup.maxSpd_Return_Spindle;
		accel_1 = configSetup.accel_Return_Spindle;
		// Spindle uses relative position to start from.
		//stepper_Spindle.setPosition(0);
		MilliDelay(15);
		//stepper_Spindle
		//.setMaxSpeed(configSetup.maxSpd_Return_Spindle)
			//.setAcceleration(configSetup.accel_Return_Spindle)
			//.setTargetRel(target_Spindle);
		break;
	}

	break;
	}

#ifdef DEBUG

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
#endif // DEBUG
	stepper_1
		.setMaxSpeed(maxSpeed_1)
		.setAcceleration(accel_1)
		.setTargetAbs(stepperPosition_1);
	//.setTargetAbs(stepperPosition_1);
//.setTargetAbs(stepperPosition_1);

	stepper_2
		.setMaxSpeed(maxSpeed_2)
		.setAcceleration(accel_2)
		.setTargetAbs(stepperPosition_2);
	//.setTargetAbs(stepperPosition_2);

	stepper_3
		.setMaxSpeed(maxSpeed_3)
		.setAcceleration(accel_3)
		.setTargetAbs(stepperPosition_3);
	//.setTargetAbs(stepperPosition_3);

	stepper_4
		.setMaxSpeed(maxSpeed_4)
		.setAcceleration(accel_4)
		.setTargetAbs(stepperPosition_4);
	//.setTargetAbs(stepperPosition_4);

	stepper_5
		.setMaxSpeed(maxSpeed_5)
		.setAcceleration(accel_5)
		.setTargetAbs(stepperPosition_5);
	//.setTargetAbs(stepperPosition_5);

	switch (checkedCount)
	{
	case 1:
	{
		SetEnable(stepperId_1, true, true);
#ifdef DEBUG
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
#endif
		Serial.println("moveAsync(stepper_1)");
		stepController.moveAsync(stepper_1);
		break;
	}
	case 2:
	{
		SetEnable(stepperId_1, true, true);
		SetEnable(stepperId_2, true, true);

#ifdef DEBUG
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
#endif
		Serial.println("moveAsync(stepper_1, stepper_2)");
		stepController.moveAsync(stepper_1, stepper_2);
		Serial.println("2222222--------------------------------------------------------------");
		break;
	}
	case 3:
	{
		SetEnable(stepperId_1, true, true);
		SetEnable(stepperId_2, true, true);
		SetEnable(stepperId_3, true, true);

#ifdef DEBUG
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
#endif // Debug
		Serial.println("moveAsync(stepper_1, stepper_2, stepper_3)");
		stepController.moveAsync(stepper_1, stepper_2, stepper_3);
		//}
		break;
	}
	case 4:
	{
		SetEnable(stepperId_1, true, true);
		SetEnable(stepperId_2, true, true);
		SetEnable(stepperId_3, true, true);
		SetEnable(stepperId_4, true, true);
		stepController.moveAsync(stepper_1, stepper_2, stepper_3, stepper_4);
		break;
	}
	case 5:
	{
		SetEnable(stepperId_1, true, true);
		SetEnable(stepperId_2, true, true);
		SetEnable(stepperId_3, true, true);
		SetEnable(stepperId_4, true, true);
		SetEnable(stepperId_5, true, true);
		stepController.moveAsync(stepper_1, stepper_2, stepper_3, stepper_4, stepper_5);
		break;
	}
	}

	while (stepController.isRunning())
	{

#ifdef ESTOP_ENABLED
		if (configSetup.eStop > 9 && configSetup.eStop < 40)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
#ifdef DEBUG
				Serial.println("                          EStop");
#endif // DEBUG


				stepController.emergencyStop();
				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEnable(ID_AXIS_4, false, false);
				SetEStopColors(PAGE_RETURNS);
				if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
				{
					digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
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
				currentSpeed = stepController.getCurrentSpeed();
#ifdef DEBUG
				Serial.print("currentSpeed: ");
				Serial.println(currentSpeed);
#endif // Debug
				if (currentSpeed < MIN_SPEED)
				{
					stepController.emergencyStop();
				}
				else
				{
					stepController.stop();
				}
				checkedCount = 0;
				break;
			}
			}
		}

		MilliDelay(10);
	}

	switch (pageCallerId)
	{
	case PAGE_MULTI:
	{
		SerialPrint("pageMulti.bt1.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageMulti.bt1.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageMulti.va0.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageMulti.bt0.val=0");
		SerialPrint(nextionEnd);
#ifdef DEBUG
		Serial.print("configMulti.axisId: ");
		Serial.println(configMulti.axisId);
#endif // DEBUG
		if (configSetup.keepSteppersEnabled != 1)
		{
#ifdef DEBUG
			Serial.print("stepper1 axisId:  ");
			Serial.println(stepperId_1);
#endif // DEBUG
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
#ifdef DEBUG
		Serial.print("configMulti.axisId: ");
		Serial.println(configMulti.axisId);
#endif // DEBUG
		if (configSetup.keepSteppersEnabled != 1)
		{
#ifdef DEBUG
			Serial.print("stepper1 axisId:  ");
			Serial.println(stepperId_1);
#endif // DEBUG
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

#ifdef DEBUG
		Serial.print("configMain.axisId: ");
		Serial.println(configMain.axisId);
#endif // DEBUG
		if (configSetup.keepSteppersEnabled != 1)
		{
#ifdef DEBUG
			Serial.print("stepper1 axisId:  ");
			Serial.println(stepperId_1);
#endif // DEBUG
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
				SetEnable(stepperId_2, false, true);

				SerialPrint("pageOne.bt303.val=0"); // 1 = disabled
				SerialPrint("\xFF\xFF\xFF");
				SerialPrint("pageOne.bt303.txt="); // 1 = disabled
				SerialPrint("\x22");
				SerialPrint("Disabled");
				SerialPrint("\x22\xFF\xFF\xFF");

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
#ifdef DEBUG
		Serial.print("pageIndex axisId: ");
		Serial.println(configIndex_Prime.axisId);
#endif // DEBUG
		SerialPrint("pageIndex.bt1.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageIndex.va0.val=0");
		SerialPrint(nextionEnd);
		SerialPrint("pageIndex.bt0.val=0");
		SerialPrint(nextionEnd);

		//ToDo: Synchro

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
#ifdef DEBUG
		Serial.print("pageRose axisId: ");
		Serial.println(configRose.axisId);
#endif // DEBUG
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
#ifdef DEBUG
		Serial.print("pageSync axisId: ");
		Serial.println(configSync.axisId);
#endif // DEBUG
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
#ifdef DEBUG
		Serial.print("pageRec axisId: ");
		Serial.println(configRec.axisId);
#endif // DEBUG
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


	//SetEnable(ID_SPINDLE, false, true);
	//SetEnable(ID_AXIS_Z, false, true);
	//SetEnable(ID_AXIS_X, false, true);
	//SetEnable(ID_AXIS_3, false, true);
	//SetEnable(ID_AXIS_4, false, true);


	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		SetEnable(ID_AXIS_3, false, true);
	}

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
	checkedCount = 0;
	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
	}
}


/// <summary>
/// PageMulti: Return Spindle and/or axes to start position
/// </summary>
/// <comment>
/// </comment>
/// <param name="axisId">Id of axis to move</param>
void ReturnToStartPosition_MultiMain()
{
	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		digitalWrite(configSetup.cutterMotorPin, LOW);  // Enable
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
#ifdef DEBUG



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
#endif // DEBUG
	StepControl stepController;
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
			Serial.print("   Case 1");

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
			Serial.print("   Case 2");

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
			Serial.print("   Case 3");

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
			Serial.print("   Case 4");

			stepperId_5 = 47;

			stepper5_step = 56;
			stepper5_dir = 57;

			break;
		}
	}

#ifdef DEBUG



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
#endif // DEBUG
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

		break;
	}
#ifdef DEBUG

	Serial.print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>stepperId_2: ");
	Serial.println(stepperId_2);
#endif // DEBUG
	switch (stepperId_2)
	{

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

		break;
	}
	switch (stepperId_3)
	{
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
			// Spindle uses relative position to start from.
			//stepper_Spindle.setPosition(0);
			MilliDelay(15);
			//stepper_Spindle
			//.setMaxSpeed(configSetup.maxSpd_Return_Spindle)
				//.setAcceleration(configSetup.accel_Return_Spindle)
				//.setTargetRel(target_Spindle);
			break;
		}

		break;
	}
	switch (stepperId_4)
	{
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
			// Spindle uses relative position to start from.
			//stepper_Spindle.setPosition(0);
			MilliDelay(15);
			//stepper_Spindle
			//.setMaxSpeed(configSetup.maxSpd_Return_Spindle)
				//.setAcceleration(configSetup.accel_Return_Spindle)
				//.setTargetRel(target_Spindle);
			break;
		}

		break;
	}
	switch (stepperId_5)
	{
		case ID_SPINDLE:
		{
			maxSpeed_1 = configSetup.maxSpd_Return_Spindle;
			accel_1 = configSetup.accel_Return_Spindle;
			// Spindle uses relative position to start from.
			//stepper_Spindle.setPosition(0);
			MilliDelay(15);
			//stepper_Spindle
			//.setMaxSpeed(configSetup.maxSpd_Return_Spindle)
				//.setAcceleration(configSetup.accel_Return_Spindle)
				//.setTargetRel(target_Spindle);
			break;
		}

		break;
	}

#ifdef DEBUG

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
#endif // DEBUG
	stepper_1
		.setMaxSpeed(maxSpeed_1)
		.setAcceleration(accel_1)
		.setTargetAbs(0);
		//.setTargetAbs(stepperPosition_1);
	//.setTargetAbs(stepperPosition_1);

	//Serial.print("kkkkkkkkkkkkkkkkkkkk            stepperPosition_2:  ");
	//Serial.println(stepperPosition_2);
	//if (stepperPosition_2 > 0)
	//{
	//	stepperPosition_2 = stepperPosition_2 * (-1);
	//	stepper_2.setPosition(stepperPosition_2);
	//}

	Serial.print("kkkkkkkkkkkkkkkkkkkk            stepperPosition_2:  ");
	Serial.println(stepperPosition_2);

	stepper_2
		.setMaxSpeed(maxSpeed_2)
		.setAcceleration(accel_2)
		.setTargetAbs(0);
		//.setTargetAbs(stepperPosition_2);

	stepper_3
		.setMaxSpeed(maxSpeed_3)
		.setAcceleration(accel_3)
		.setTargetAbs(0);
		//.setTargetAbs(stepperPosition_3);

	stepper_4
		.setMaxSpeed(maxSpeed_4)
		.setAcceleration(accel_4)
		.setTargetAbs(0);
		//.setTargetAbs(stepperPosition_4);

	stepper_5
		.setMaxSpeed(maxSpeed_5)
		.setAcceleration(accel_5)
		.setTargetAbs(0);
		//.setTargetAbs(stepperPosition_5);

	int dirPin_Reset = digitalReadFast(PIN_AXIS_3_DIR);

	if (stepperPosition_1 > 0)
	{
		switch (stepperId_1)
		{
			case ID_AXIS_Z:
			{
				digitalWriteFast(PIN_AXIS_Z_DIR, LOW);
				break;
			}
			case ID_AXIS_X:
			{
				digitalWriteFast(PIN_AXIS_X_DIR, LOW);
				break;
			}
			case ID_AXIS_3:
			{
				digitalWriteFast(PIN_AXIS_3_DIR, LOW);
				break;
			}
			case ID_AXIS_4:
			{
				digitalWriteFast(PIN_AXIS_4_DIR, LOW);
				break;
			}
		}
	}

	if (stepperPosition_2 > 0) // Only valid on main page when synchro engaged
	{

		digitalWriteFast(PIN_AXIS_3_DIR, LOW);
	}



	Serial.print("dirPin_Reset: ");
	Serial.println(dirPin_Reset);

	dirPin_Reset = digitalReadFast(PIN_SPINDLE_DIR);
	Serial.print("dirPin-Spindle: ");
	Serial.println(dirPin_Reset);




	switch(checkedCount)
	{
		case 1:
		{
			SetEnable(stepperId_1, true, true);
#ifdef DEBUG
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
#endif
			Serial.println("moveAsync(stepper_1)");
			stepController.moveAsync(stepper_1);
			break;
		}
		case 2:
		{
			SetEnable(stepperId_1, true, true);
			SetEnable(stepperId_2, true, true);

#ifdef DEBUG
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
#endif
			Serial.println("moveAsync(stepper_1, stepper_2)");
			stepController.moveAsync(stepper_1, stepper_2);
			Serial.println("2222222--------------------------------------------------------------");
			break;
		}
		case 3:
		{
			SetEnable(stepperId_1, true, true);
			SetEnable(stepperId_2, true, true);
			SetEnable(stepperId_3, true, true);

#ifdef DEBUG
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
#endif // Debug
			Serial.println("moveAsync(stepper_1, stepper_2, stepper_3)");
				stepController.moveAsync(stepper_1, stepper_2, stepper_3);
			//}
			break;
		}
		case 4:
		{
			SetEnable(stepperId_1, true, true);
			SetEnable(stepperId_2, true, true);
			SetEnable(stepperId_3, true, true);
			SetEnable(stepperId_4, true, true);
			stepController.moveAsync(stepper_1, stepper_2, stepper_3, stepper_4);
			break;
		}
		case 5:
		{
			SetEnable(stepperId_1, true, true);
			SetEnable(stepperId_2, true, true);
			SetEnable(stepperId_3, true, true);
			SetEnable(stepperId_4, true, true);
			SetEnable(stepperId_5, true, true);
			stepController.moveAsync(stepper_1, stepper_2, stepper_3, stepper_4, stepper_5);
			break;
		}
	}

	while (stepController.isRunning())
	{

#ifdef ESTOP_ENABLED
		if (configSetup.eStop > 9 && configSetup.eStop < 40)
		{
			if (digitalRead(configSetup.eStop) != configSetup.limit_NCorNO)
			{
#ifdef DEBUG
				Serial.println("                          EStop");
#endif // DEBUG

				
				stepController.emergencyStop();
				stopSteppers = true;
				// Disable all motors
				SetEnable(ID_SPINDLE, false, false);
				SetEnable(ID_AXIS_Z, false, false);
				SetEnable(ID_AXIS_X, false, false);
				SetEnable(ID_AXIS_3, false, false);
				SetEnable(ID_AXIS_4, false, false);
				SetEStopColors(PAGE_RETURNS);
				if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
				{
					digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
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
					currentSpeed = stepController.getCurrentSpeed();
#ifdef DEBUG
					Serial.print("currentSpeed: ");
					Serial.println(currentSpeed);
#endif // Debug
					if (currentSpeed < MIN_SPEED)
					{
						stepController.emergencyStop();
					}
					else
					{
						stepController.stop();
					}
					checkedCount = 0;
					break;
				}
			}
		}

		MilliDelay(10);
	}

	switch (pageCallerId)
	{
		case PAGE_MULTI:
		{
			SerialPrint("pageMulti.bt1.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageMulti.bt1.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageMulti.va0.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageMulti.bt0.val=0");
			SerialPrint(nextionEnd);
#ifdef DEBUG
			Serial.print("configMulti.axisId: ");
			Serial.println(configMulti.axisId);
#endif // DEBUG
			if (configSetup.keepSteppersEnabled != 1)
			{
#ifdef DEBUG
				Serial.print("stepper1 axisId:  ");
				Serial.println(stepperId_1);
#endif // DEBUG
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
#ifdef DEBUG
			Serial.print("configMulti.axisId: ");
			Serial.println(configMulti.axisId);
#endif // DEBUG
			if (configSetup.keepSteppersEnabled != 1)
			{
#ifdef DEBUG
				Serial.print("stepper1 axisId:  ");
				Serial.println(stepperId_1);
#endif // DEBUG
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

#ifdef DEBUG
			Serial.print("configMain.axisId: ");
			Serial.println(configMain.axisId);
#endif // DEBUG
			if (configSetup.keepSteppersEnabled != 1)
			{
#ifdef DEBUG
				Serial.print("stepper1 axisId:  ");
				Serial.println(stepperId_1);
#endif // DEBUG
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
					SetEnable(stepperId_2, false, true);

					SerialPrint("pageOne.bt303.val=0"); // 1 = disabled
					SerialPrint("\xFF\xFF\xFF");
					SerialPrint("pageOne.bt303.txt="); // 1 = disabled
					SerialPrint("\x22");
					SerialPrint("Disabled");
					SerialPrint("\x22\xFF\xFF\xFF");

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
#ifdef DEBUG
			Serial.print("pageIndex axisId: ");
			Serial.println(configIndex_Prime.axisId);
#endif // DEBUG
			SerialPrint("pageIndex.bt1.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageIndex.va0.val=0");
			SerialPrint(nextionEnd);
			SerialPrint("pageIndex.bt0.val=0");
			SerialPrint(nextionEnd);

			//ToDo: Synchro

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
#ifdef DEBUG
			Serial.print("pageRose axisId: ");
			Serial.println(configRose.axisId);
#endif // DEBUG
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
#ifdef DEBUG
			Serial.print("pageSync axisId: ");
			Serial.println(configSync.axisId);
#endif // DEBUG
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
#ifdef DEBUG
			Serial.print("pageRec axisId: ");
			Serial.println(configRec.axisId);
#endif // DEBUG
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


	//SetEnable(ID_SPINDLE, false, true);
	//SetEnable(ID_AXIS_Z, false, true);
	//SetEnable(ID_AXIS_X, false, true);
	//SetEnable(ID_AXIS_3, false, true);
	//SetEnable(ID_AXIS_4, false, true);


	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		SetEnable(ID_AXIS_3, false, true);
	}

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
	checkedCount = 0;
	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
	}
}

/// <summary>
/// Return Spindle and/or axes to start position
/// </summary>
/// <comment>
/// </comment>
/// <param name="axisId">Id of axis to move</param>
void ReturnToStartPosition_MovePage(int axisId)
{
	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		digitalWrite(configSetup.cutterMotorPin, LOW);  // Enable
	}
	eStopTriggered = false;
	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);

	StepControl stepController;
	Stepper stepper_Z(PIN_AXIS_Z_STEP, PIN_AXIS_Z_DIR);
	Stepper stepper_X(PIN_AXIS_X_STEP, PIN_AXIS_X_DIR);
	Stepper stepper_M3(PIN_AXIS_3_STEP, PIN_AXIS_3_DIR);
	Stepper stepper_M4(PIN_AXIS_4_STEP, PIN_AXIS_4_DIR);

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
		case ID_MOVE_AXIS_M3_2:
		case ID_AXIS_3:
		{
			// Axis uses absolute position
			// Ensure axis position is set to end position of last routine.
			stepper_M3.setPosition(endPosition_Axis);

			MilliDelay(20);
	#ifdef DEBUG
			Serial.print("M3 position: ");
			Serial.println(stepper_M3.getPosition());
	#endif // Debug
			stepper_M3
				.setMaxSpeed(configSetup.maxSpd_Return_Axis_M3)
				.setAcceleration(configSetup.accel_Return_Axis_M3)
				.setTargetAbs(0);

	#ifdef DEBUG
			Serial.print("maxSpd_Return_Axis_M3: ");
			Serial.println(configSetup.accel_Return_Axis_M3);
	#endif // DEBUG
			SetEnable(ID_AXIS_3, true, true);
				stepController.moveAsync(stepper_M3);

			break;
		}
		case ID_MOVE_AXIS_M4_2:
		case ID_AXIS_4:
		{
			// Axis uses absolute position
			// Ensure axis position is set to end position of last routine.
			stepper_M4.setPosition(endPosition_Axis);

			MilliDelay(20);
#ifdef DEBUG
			Serial.print("M4 position: ");
			Serial.println(stepper_M4.getPosition());
#endif // Debug
			stepper_M4
				.setMaxSpeed(configSetup.maxSpd_Return_Axis_M4)
				.setAcceleration(configSetup.accel_Return_Axis_M4)
				.setTargetAbs(0);

#ifdef DEBUG
			Serial.print("maxSpd_Return_Axis_M4: ");
			Serial.println(configSetup.accel_Return_Axis_M4);
#endif // DEBUG
			SetEnable(ID_AXIS_4, true, true);
			stepController.moveAsync(stepper_M4);

			break;
		}
	}

	while (stepController.isRunning())
	{
#ifdef ESTOP_ENABLED
		//if (configSetup.eStop != 0)
		if (configSetup.eStop > 9 && configSetup.eStop < 40)
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
				SetEnable(ID_AXIS_3, false, false);
				SetEnable(ID_AXIS_4, false, false);
				SetEStopColors(PAGE_RETURNS);
				if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
				{
					digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
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
				currentSpeed = stepController.getCurrentSpeed();
#ifdef DEBUG
				Serial.print("currentSpeed: ");
				Serial.println(currentSpeed);
#endif // Debug
				if (currentSpeed < MIN_SPEED)
				{
					stepController.emergencyStop();
				}
				else
				{
					stepController.stop();
				}
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
		if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
		{
			digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
		}
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
	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		digitalWrite(configSetup.cutterMotorPin, LOW);  // Enable
	}
	eStopTriggered = false;
	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=0");
	SerialPrint(nextionEnd);

	int limitPin_Max = configSetup.limit_Max_Z;
	int limitPin_Min = configSetup.limit_Min_Z;
	limitTriggered = false;
	bool runPattern = true;
	int32_t spindleCurrentPosition = 0;
	IntervalTimer tickTimer;

	StepControl controllerRoseSpindle;

	kRatio = (float)configRose.n / configRose.d;

	// Initialize variables to 0
	returnSteps_Spindle = 0;
	slideStepsAmplitude = 0;
	startPositionAbs_Axis = 0;
	// Set stepper positions to 0
	stepperSpindle.setPosition(0);
	stepperAxis_Z.setPosition(0);
	stepperAxis_X.setPosition(0);
	stepperAxis_M3.setPosition(0);
	stepperAxis_M4.setPosition(0);

	// Get spindle steps per revolution and max speed
	spindleStepsPerRev = configSetup.gearRatio_Spindle * configSetup.microsteps_Spindle * configSetup.steps360_Spindle;
	int32_t spindleMaxRotation = configRose.spindleRevolutions*spindleStepsPerRev;
	newMaxSpd_RoseSpindle = configRose.maxSpd_Spindle * configRose.speedPercent_Spindle * .01;//* direction;
	volatile int32_t spindleTarget = 2147000000 * direction;
#ifdef DEBUG
	Serial.print("Spindle Revolutions: ");
	Serial.println(configRose.spindleRevolutions);
	Serial.print("spindleTarget: ");
	Serial.println(spindleTarget);

	Serial.print("newMaxSpd_RoseSpindle: ");
	Serial.println(newMaxSpd_RoseSpindle);
#endif // Debug
	// Set spindle max speed and acceleration
	stepperSpindle
		.setMaxSpeed(newMaxSpd_RoseSpindle)
		.setAcceleration(configRose.accel_Spindle)
		.setPosition(0);
	stepperSpindle.setTargetRel(spindleTarget);

	// Enable spindle
	SetEnable(ID_SPINDLE, true, true);
	stepperId_1 = ID_SPINDLE;
	stepper1_step = PIN_SPINDLE_STEP;
	stepper1_dir = PIN_SPINDLE_DIR;

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

			newMaxSpd_RoseAxis = configRose.maxSpd_Axis_M3 * configRose.speedPercent_Axis_M3 * .01;

			// Use Absolute position for Return function
			// slideFunc_Axis initial call sets axis position to max amplitude (slideStepsAmplitude).
			startPositionAbs_Axis = slideStepsAmplitude;
#ifdef DEBUG
			Serial.print("returnPositionAbs_Axis:  ");
			Serial.print(startPositionAbs_Axis);
#endif // Debug
			stepperAxis_M3
				.setMaxSpeed(newMaxSpd_RoseAxis)
				.setAcceleration(configRose.accel_Axis_M3)
				.setPosition(slideFunc_Axis(0)); // set start position of counter
			SetEnable(ID_AXIS_3, true, true);
			controllerAxis.rotateAsync(stepperAxis_M3);
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

			newMaxSpd_RoseAxis = configRose.maxSpd_Axis_M4 * configRose.speedPercent_Axis_M4 * .01;

			// Use Absolute position for Return function
			// slideFunc_Axis initial call sets axis position to max amplitude (slideStepsAmplitude).
			startPositionAbs_Axis = slideStepsAmplitude;
#ifdef DEBUG
			Serial.print("returnPositionAbs_Axis:  ");
			Serial.print(startPositionAbs_Axis);
#endif // Debug
			stepperAxis_M4
				.setMaxSpeed(newMaxSpd_RoseAxis)
				.setAcceleration(configRose.accel_Axis_M4)
				.setPosition(slideFunc_Axis(0)); // set start position of counter
			SetEnable(ID_AXIS_4, true, true);
			controllerAxis.rotateAsync(stepperAxis_M4);
			break;
		}
	}

	// Set limit switches HIGH
	digitalWrite(limitPin_Max, HIGH);
	digitalWrite(limitPin_Min, HIGH);

	float targetSteps_M3Synchro = spindleTarget * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01);

	checkedCount = 2;
	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		checkedCount = 3;
		stepperId_3 = ID_AXIS_3;
		stepper3_step = PIN_AXIS_3_STEP;
		stepper3_dir = PIN_AXIS_3_DIR;

		Serial.print("spindleTarget * synchro_Ratio * (configMain.synchro_M3_Percentage * 0.01):  ");
		Serial.println(targetSteps_M3Synchro);

		Serial.print("synchro_Ratio:  ");
		Serial.println(synchro_Ratio);

		Serial.print("configMain.synchro_M3_Percentage * 0.01:  ");
		Serial.println(configMain.synchro_M3_Percentage * 0.01);

		Serial.print("newMaxSpd_RoseSpindle:  ");
		Serial.println(newMaxSpd_RoseSpindle);

		stepperAxis_M3.setPosition(0);
		//stepperAxis_M3.setTargetRel(targetSteps_M3Synchro);// *synchro_Ratio* (configMain.synchro_M3_Percentage * 0.01));
		////stepperAxis_M3.setMaxSpeed(configGreekKey.maxSpd_Axis_M3 * configGreekKey.speedPercent_Axis_M3 * .01 );
		//stepperAxis_M3.setMaxSpeed(newMaxSpd_RoseSpindle);// *synchro_Ratio* (configMain.synchro_M3_Percentage * 0.01));
		//stepperAxis_M3.setAcceleration(configGreekKey.accel_Axis_M3);
		stepperAxis_M3
			.setMaxSpeed(newMaxSpd_RoseSpindle)
			.setAcceleration(configGreekKey.accel_Axis_M3)
			.setTargetRel(targetSteps_M3Synchro);

		SetEnable(ID_AXIS_3, true, true);
		MilliDelay(5);
		controllerRoseSpindle.moveAsync(stepperSpindle, stepperAxis_M3);
	}
	else
	{
		controllerRoseSpindle.moveAsync(stepperSpindle);
	}


	//controllerRoseSpindle.rotateAsync(stepperSpindle); // let the spindle run with constant speed
	//controllerRoseSpindle.moveAsync(stepperSpindle);
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
		if (limitPin_Max > 9 && limitPin_Max < 40)
		{
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
		}
		if (limitPin_Min > 9 && limitPin_Min < 40)
		{
			if ((digitalRead(limitPin_Min) != configSetup.limit_NCorNO))// && axisSteps < 0)
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
		}
#ifdef ESTOP_ENABLED
		//if (configSetup.eStop != 0)
		if (configSetup.eStop > 9 && configSetup.eStop < 40)
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
				SetEnable(ID_AXIS_3, false, false);
				SetEnable(ID_AXIS_4, false, false);
				SetEStopColors(PAGE_ROSE);
				if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
				{
					digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
				}
				return;
			}
		}
#endif // estop
		MilliDelay(20);
	} // End while

	// endPosition_Spindle and endPosition_Axis may be used in future for reporting purposes
	// Store current spindle position for possible use in Return to 0 method
	endPosition_Spindle = stepperSpindle.getPosition();
	returnSteps_Spindle = endPosition_Spindle;



#ifdef DEBUG
	Serial.println("===============");
	Serial.print("Spindle Reset Position: ");
	Serial.println(stepperSpindle.getPosition());
#endif // Debug
	// Disable spindle
	SetEnable(ID_SPINDLE, false, true);
	SetEnable(configRose.axisId, false, true);
	switch (configRose.axisId)
	{
		case ID_AXIS_Z:
		{
			// Store Z axis position for possible use in Return to 0 method
			endPosition_Axis = stepperAxis_Z.getPosition();
			break;
		}
		case ID_AXIS_X:
		{
			endPosition_Axis = stepperAxis_X.getPosition();
			break;
		}
		case ID_AXIS_3:
		{
			endPosition_Axis = stepperAxis_M3.getPosition();
			break;
		}
		case ID_AXIS_4:
		{
			endPosition_Axis = stepperAxis_M4.getPosition();
			break;
		}
	}

	stepperPosition_1 = endPosition_Spindle * (-1);


	int32_t relativeEndPosition = slideStepsAmplitude - endPosition_Axis;
	Serial.print("slideStepsAmplitude - endPosition_Axis: ");
	Serial.println(relativeEndPosition);
	endPosition_Axis = relativeEndPosition;
	stepperPosition_2 = endPosition_Axis;// *(-1);

	//relativeEndPosition = slideStepsAmplitude + endPosition_Axis;
	//Serial.print("slideStepsAmplitude + endPosition_Axis: ");
	//Serial.println(relativeEndPosition); 

	//relativeEndPosition = endPosition_Axis - slideStepsAmplitude;
	//Serial.print("endPosition_Axis - slideStepsAmplitude: ");
	//Serial.println(relativeEndPosition);

	//relativeEndPosition = endPosition_Axis + slideStepsAmplitude;
	//Serial.print("endPosition_Axis + slideStepsAmplitude: ");
	//Serial.println(relativeEndPosition);

	if (configMain.synchro_M3_Spindle == 1)//Synchronized M3 and Spindle
	{
		SetEnable(ID_AXIS_3, false, true);
		stepperPosition_3 = stepperAxis_M3.getPosition() * (-1);

	}

	// Set stepper positions to 0
	//stepperSpindle.setPosition(0);
	//stepperAxis_Z.setPosition(0);
	//stepperAxis_X.setPosition(0);
	//stepperAxis_M3.setPosition(0);
	//stepperAxis_M4.setPosition(0);

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
	if (configSetup.cutterMotorPin > 9 && configSetup.cutterMotorPin < 40)
	{
		digitalWrite(configSetup.cutterMotorPin, HIGH);  // Disable
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

	//const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	//const char* nextionEnd = "\xFF\xFF\xFF";

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
			configPageMainMulti eePageMain;
			EEPROM.get(eePromAddress_Main, eePageMain);

			configPageMainMulti configMain2;
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
				case ID_AXIS_3:
				{
#ifdef DEBUG
					Serial.print("configMain.accel_Axis_B:");
					Serial.println(configMain.accel_Axis_M3);
					Serial.print("configMain2.accel_Axis_B:");
					Serial.println(configMain2.accel_Axis_M3);
					Serial.print("eePageMain.accel_Axis_B:");
					Serial.println(eePageMain.accel_Axis_M3);
#endif // DEBUG
					SerialPrint(pageConfig_t12);
					SerialWrite(0x22);
					SerialPrint(eePageMain.maxSpd_Axis_M3);
					SerialPrint(nextionQuoteEnd);
					delay(100);
					SerialPrint(pageConfig_t13);
					SerialWrite(0x22);
					SerialPrint(eePageMain.accel_Axis_M3);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case ID_AXIS_4:  //ToDo
				{
#ifdef DEBUG
					Serial.print("configMain.accel_Axis_M4:");
					Serial.println(configMain.accel_Axis_M4);
					Serial.print("configMain2.accel_Axis_M4:");
					Serial.println(configMain2.accel_Axis_M4);
					Serial.print("eePageMain.accel_Axis_M4:");
					Serial.println(eePageMain.accel_Axis_M4);
#endif // DEBUG
					SerialPrint(pageConfig_t12);
					SerialWrite(0x22);
					SerialPrint(eePageMain.maxSpd_Axis_M4);
					SerialPrint(nextionQuoteEnd);
					delay(100);
					SerialPrint(pageConfig_t13);
					SerialWrite(0x22);
					SerialPrint(eePageMain.accel_Axis_M4);
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
				case ID_AXIS_3:
				{
					SerialPrint(pageConfig_t12);
					SerialWrite(0x22);
					SerialPrint(eePageSync.maxSpd_Axis_M3);
					SerialPrint(nextionQuoteEnd);;

					SerialPrint(pageConfig_t13);
					SerialWrite(0x22);
					SerialPrint(eePageSync.accel_Axis_M3);
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
				case ID_AXIS_3:
				{
					SerialPrint(pageConfig_t12);
					SerialWrite(0x22);
					SerialPrint(configRec.maxSpd_Axis_M3);
					SerialPrint(nextionQuoteEnd);

					SerialPrint(pageConfig_t13);
					SerialWrite(0x22);
					SerialPrint(eePageRec.accel_Axis_M3);
					SerialPrint(nextionQuoteEnd);
					break;
				}
			}
			break;
		}
		case PAGE_MULTI:
		{
			configPageMainMulti eePageOne;
			EEPROM.get(eePromAddress_Multi, eePageOne);
			maxSpd_Spindle = eePageOne.maxSpd_Spindle;
			accel_Spindle = eePageOne.accel_Spindle;
			maxSpd_Axis_Z = eePageOne.maxSpd_Axis_Z;
			accel_Axis_Z = eePageOne.accel_Axis_Z;
			maxSpd_Axis_X = eePageOne.maxSpd_Axis_X;
			accel_Axis_X = eePageOne.accel_Axis_X;
			maxSpd_Axis_B = eePageOne.maxSpd_Axis_M3;
			accel_Axis_B = eePageOne.accel_Axis_M3;
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
				case ID_AXIS_3:
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
			EEPROM.get(eePromAddress_Ind_Prime, eepageIndex);
			SerialPrint(pageConfig_t10);
			SerialWrite(0x22);
			SerialPrint(eepageIndex.maxSpd_Spindle);
			SerialPrint(nextionQuoteEnd);

			SerialPrint(pageConfig_t11);
			SerialWrite(0x22);
			SerialPrint(eepageIndex.accel_Spindle);
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
				case ID_MOVE_AXIS_M3_1:
				case ID_MOVE_AXIS_M3_2:
				{
					SerialPrint(pageConfig_t12);
					SerialWrite(0x22);
					SerialPrint(eepageMove.maxSpd_Motor_3);
					SerialPrint(nextionQuoteEnd);

					SerialPrint(pageConfig_t13);
					SerialWrite(0x22);
					SerialPrint(eepageMove.accel_Motor_3);
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
				case ID_AXIS_3:
				{
					SerialPrint(pageConfig_t12);
					SerialWrite(0x22);
					SerialPrint(eePageRose.maxSpd_Axis_M3);
					SerialPrint(nextionQuoteEnd);

					SerialPrint(pageConfig_t13);
					SerialWrite(0x22);
					SerialPrint(eePageRose.accel_Axis_M3);
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
				case ID_AXIS_3: // B Axis
				{
					SerialPrint(pageConfig_t12);
					SerialWrite(0x22);
					SerialPrint(eePageGrk.maxSpd_Axis_M3);
					SerialPrint(nextionQuoteEnd);

					SerialPrint(pageConfig_t13);
					SerialWrite(0x22);
					SerialPrint(eePageGrk.accel_Axis_M3);
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
void TestEEPROMSetup()  //ToDo
{

	const char* pageSpindle_t42 = "pageSpindle.t42.txt=";
	const char* pageSpindle_t43 = "pageSpindle.t43.txt=";
	const char* pageSpindle_t44 = "pageSpindle.t44.txt=";
	const char* pageSpindle_t45 = "pageSpindle.t45.txt=";
//	const char* pageSpindle_t8 = "pageSpindle.t8.txt=";
//	const char* pageSpindle_t12 = "pageSpindle.t12.txt=";
	const char* pageZ_t46 = "pageZ.t46.txt=";
	const char* pageZ_t47 = "pageZ.t47.txt=";
	const char* pageZ_t48 = "pageZ.t48.txt=";
	const char* pageZ_t49 = "pageZ.t49.txt=";
	const char* pageMotor_3_t56 = "pageMotor_3.t56.txt=";
	const char* pageMotor_3_t57 = "pageMotor_3.t57.txt=";
	const char* pageMotor_3_t58 = "pageMotor_3.t58.txt=";
	const char* pageMotor_3_t59 = "pageMotor_3.t59.txt=";
	const char* pageMotor_3_t64 = "pageMotor_3.t64.txt=";

	const char* pageMotor_4_t56 = "pageMotor_4.t56.txt=";
	const char* pageMotor_4_t57 = "pageMotor_4.t57.txt=";
	const char* pageMotor_4_t58 = "pageMotor_4.t58.txt=";
	const char* pageMotor_4_t59 = "pageMotor_4.t59.txt=";
	const char* pageMotor_4_t64 = "pageMotor_4.t64.txt=";

	// Radial
	const char* pageMotor_3_c0_0 = "pageMotor_3.c0.val=0";
	const char* pageMotor_3_c0_1 = "pageMotor_3.c0.val=1";

	const char* pageMotor_4_c0_0 = "pageMotor_4.c0.val=0";
	const char* pageMotor_4_c0_1 = "pageMotor_4.c0.val=1";

	// Linear
	const char* pageMotor_3_c1_0 = "pageMotor_3.c1.val=0";
	const char* pageMotor_3_c1_1 = "pageMotor_3.c1.val=1";

	const char* pageMotor_4_c1_0 = "pageMotor_4.c1.val=0";
	const char* pageMotor_4_c1_1 = "pageMotor_4.c1.val=1";

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
	const char* pageMore_t8 = "pageMore.t8.txt=";

	const char* lowChar = "Low";
	const char* highChar = "High";
	//const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	//const char* nextionEnd = "\xFF\xFF\xFF";

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
	Serial.println(configSetup.distancePerRev_AxisZ,8);
	Serial.print("eePromPageSetup.distancePerRev_AxisZ:  ");
	Serial.println(eePromPageSetup.distancePerRev_AxisZ,8);

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

	// pageMotor_3
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
	if (eePromPageSetup.radialOrLinear_Axis_M3) 
	{
		SerialPrint(pageMotor_3_c0_0);
		SerialPrint(nextionEnd);
		SerialPrint(pageMotor_3_c1_1);
		SerialPrint(nextionEnd);
	}
	else
	{
		SerialPrint(pageMotor_3_c0_1);
		SerialPrint(nextionEnd);
		SerialPrint(pageMotor_3_c1_0);
		SerialPrint(nextionEnd);
	}

	SerialPrint(pageMotor_3_t64);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.distancePerRev_AxisM3, 4);
	SerialPrint(nextionQuoteEnd);

	// pageMotor_4
	SerialPrint(pageMotor_4_t56);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.microsteps_Axis_M4);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageMotor_4_t57);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.steps360_Axis_M4);
	SerialPrint(nextionQuoteEnd);

	SerialPrint(pageMotor_4_t58);
	//SerialPrint("t58.txt=");
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.gearRatio_AxisM4, 5);
	SerialPrint(nextionQuoteEnd);

	Serial.println(pageMotor_4_t58);
	Serial.print("(eePromPageSetup.gearRatio_AxisM4, 5):  ");
	Serial.println(eePromPageSetup.gearRatio_AxisM4);

	SerialPrint(pageMotor_4_t59);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.polarity_Axis_M4 ? lowChar : highChar);
	SerialPrint(nextionQuoteEnd);

	// True=Linear, False = Radial
	if (eePromPageSetup.radialOrLinear_Axis_M4)
	{
		SerialPrint(pageMotor_4_c0_0);
		SerialPrint(nextionEnd);
		SerialPrint(pageMotor_4_c1_1);
		SerialPrint(nextionEnd);
	}
	else
	{
		SerialPrint(pageMotor_4_c0_1);
		SerialPrint(nextionEnd);
		SerialPrint(pageMotor_4_c1_0);
		SerialPrint(nextionEnd);
	}

	Serial.print("configSetup.distancePerRev_AxisM4:  ");
	Serial.println(configSetup.distancePerRev_AxisM4);


	SerialPrint(pageMotor_4_t64);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.distancePerRev_AxisM4, 4);
	SerialPrint(nextionQuoteEnd);

	Serial.println(pageMotor_4_t64);
	Serial.print("(eePromPageSetup.distancePerRev_AxisM4, 4):  ");
	Serial.println(eePromPageSetup.distancePerRev_AxisM4);

	// pageMore
	SerialPrint(pageMore_t8);
	SerialWrite(0x22);
	SerialPrint(eePromPageSetup.cutterMotorPin);
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
void TestEEPROM_Limits() //ToDo
{
	const char* pageLimits_t0 = "pageLimits.t0.txt=";
	const char* pageLimits_t1 = "pageLimits.t1.txt=";
	const char* pageLimits_t2 = "pageLimits.t2.txt=";
	const char* pageLimits_t3 = "pageLimits.t3.txt=";
	const char* pageLimits_t4 = "pageLimits.t4.txt=";
	const char* pageLimits_t5 = "pageLimits.t5.txt=";

	const char* pageLimits_t6 = "pageLimits.t6.txt="; //M4 Max
	const char* pageLimits_t7 = "pageLimits.t7.txt="; //M4 Min

	//const char* pageLimits_t8 = "pageLimits.t8.txt=";
	const char* pageLimits_t13 = "pageLimits.t13.txt=";

	const char* pageLimits_c1_0 = "pageLimits.c1.val=0";
	const char* pageLimits_c1_1 = "pageLimits.c1.val=1";
	const char* pageLimits_c2_0 = "pageLimits.c2.val=0";
	const char* pageLimits_c2_1 = "pageLimits.c2.val=1";
	const char* pageLimits_c3_0 = "pageLimits.c3.val=0";
	const char* pageLimits_c3_1 = "pageLimits.c3.val=1";

	//const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	//const char* nextionEnd = "\xFF\xFF\xFF";

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
	const char* pageReturns_t15 = "pageReturns.t15.txt=";
	const char* pageReturns_t16 = "pageReturns.t16.txt=";

	//const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	//const char* nextionEnd = "\xFF\xFF\xFF";

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
void TestAllTeensyEEPROMValues() // ToDo
{

	//value24 = 5;
	//value16 = 4;
	//value8 = 1;
	//value0 = 51;
	//packedValue = (value24 << 24) | (value16 << 16) | (value8 << 8) | (value0);
	//Serial.print("24 to 0: ");
	//Serial.println(packedValue);
	//packedValue = (value24 << 8) | (value16 << 8) | (value8 << 8) | (value0);
	//Serial.print("8: ");
	//Serial.println(packedValue);

	switch (pageCallerId)
	{
		case PAGE_MAIN:
		{
			value24 = configSetup.motorCount;
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
					SerialPrint(configMain.speedPercent_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t305.txt=");
					SerialWrite(0x22);
					SerialPrint(configMain.maxSpd_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t306.txt=");
					SerialWrite(0x22);
					SerialPrint(configMain.accel_Axis_Z);
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
					SerialPrint(configMain.speedPercent_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t305.txt=");
					SerialWrite(0x22);
					SerialPrint(configMain.maxSpd_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t306.txt=");
					SerialWrite(0x22);
					SerialPrint(configMain.accel_Axis_X);
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
					SerialPrint(configMain.speedPercent_Axis_M3);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t305.txt=");
					SerialWrite(0x22);
					SerialPrint(configMain.maxSpd_Axis_M3);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t306.txt=");
					SerialWrite(0x22);
					SerialPrint(configMain.accel_Axis_M3);
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
					SerialPrint(configMain.speedPercent_Axis_M4);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t305.txt=");
					SerialWrite(0x22);
					SerialPrint(configMain.maxSpd_Axis_M4);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t306.txt=");
					SerialWrite(0x22);
					SerialPrint(configMain.accel_Axis_M4);
					SerialPrint(nextionQuoteEnd);

					break;
				}
				default:
				{
					SerialPrint("pageEEPROM.t303.txt=");
					SerialWrite(0x22);
					//SerialPrint("Z");
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



			SerialPrint("pageEEPROM.t311.txt=");
			SerialWrite(0x22);
			SerialPrint(configSetup.motorCount);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t312.txt=");
			SerialWrite(0x22);
			SerialPrint(configSetup.vendorId);
			SerialPrint(nextionQuoteEnd);



			break;
		}

		case PAGE_ONE:
		{
			switch (configMulti.axisId)
			{
				case ID_SPINDLE:
				{
					SerialPrint("pageEEPROM.t300.txt=");
					SerialWrite(0x22);
					SerialPrint("Spindle");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t303.txt=");
					SerialWrite(0x22);
					SerialPrint(configMulti.accel_Spindle);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t301.txt=");
					SerialWrite(0x22);
					SerialPrint(configMulti.maxSpd_Spindle);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t302.txt=");
					SerialWrite(0x22);
					SerialPrint(configMulti.speedPercent_Spindle);
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
				SerialPrint(configMulti.speedPercent_Axis_Z);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t301.txt=");
				SerialWrite(0x22);
				SerialPrint(configMulti.maxSpd_Axis_Z);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t303.txt=");
				SerialWrite(0x22);
				SerialPrint(configMulti.accel_Axis_Z);
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
				SerialPrint(configMulti.speedPercent_Axis_X);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t301.txt=");
				SerialWrite(0x22);
				SerialPrint(configMulti.maxSpd_Axis_X);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t303.txt=");
				SerialWrite(0x22);
				SerialPrint(configMulti.accel_Axis_X);
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
				SerialPrint(configMulti.speedPercent_Axis_M3);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t301.txt=");
				SerialWrite(0x22);
				SerialPrint(configMulti.maxSpd_Axis_M3);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t303.txt=");
				SerialWrite(0x22);
				SerialPrint(configMulti.accel_Axis_M3);
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
				SerialPrint(configMulti.speedPercent_Axis_M4);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t301.txt=");
				SerialWrite(0x22);
				SerialPrint(configMulti.maxSpd_Axis_M4);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t303.txt=");
				SerialWrite(0x22);
				SerialPrint(configMulti.accel_Axis_M4);
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
			int spindleChecked = 16 & checkedAxis;

			int axesDirection = configMulti.direction;
			//int zAxisDirection = 1 & axesDirection;
			//int xAxisDirection = 2 & axesDirection;
			//int m3AxisDirection = 4 & axesDirection;
			//int m4AxisDirection = 8 & axesDirection;
			
			int spindleDirection = 16 & axesDirection; // Spindle
			int idAndChecked = 0;
			int dirAndChecked = 0;

			// Spindle
			SerialPrint("pageEEPROM.t306.txt=");
			SerialWrite(0x22);
			SerialPrint("Spindle");
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t305.txt=");
			SerialWrite(0x22);
			if(spindleDirection==16)
			{ 
				SerialPrint("CW");
			}
			else
			{
				SerialPrint("CCW");
			}
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t304.txt=");
			SerialWrite(0x22);
			SerialPrint(configMulti.targetDistance_Spindle);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t303.txt=");
			SerialWrite(0x22);
			if(spindleChecked == 16)
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
					SerialPrint(configMulti.speedPercent_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t308.txt=");
					SerialWrite(0x22);
					SerialPrint(configMulti.maxSpd_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t309.txt=");
					SerialWrite(0x22);
					SerialPrint(configMulti.accel_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t310.txt=");
					SerialWrite(0x22);
					idAndChecked = 1 & configMulti.multiAxesIDs;
					if (idAndChecked == 1)
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
					dirAndChecked = 1 & configMulti.direction;
					if (dirAndChecked == 1)
					{
						SerialPrint("Left");
					}
					else
					{
						SerialPrint("Right");
					}
					SerialPrint(nextionQuoteEnd);
					// X Axis

					SerialPrint("pageEEPROM.t506.txt=");
					SerialWrite(0x22);
					SerialPrint("X");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t507.txt=");
					SerialWrite(0x22);
					SerialPrint(configMulti.speedPercent_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t508.txt=");
					SerialWrite(0x22);
					SerialPrint(configMulti.maxSpd_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t509.txt=");
					SerialWrite(0x22);
					SerialPrint(configMulti.accel_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t510.txt=");
					SerialWrite(0x22);
					idAndChecked = 2 & configMulti.multiAxesIDs;
					if (idAndChecked == 2)
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
					if (dirAndChecked == 2)
					{
						SerialPrint("Left");
					}
					else
					{
						SerialPrint("Right");
					}
					SerialPrint(nextionQuoteEnd);

				// M3 Axis

					SerialPrint("pageEEPROM.t706.txt=");
					SerialWrite(0x22);
					SerialPrint("M3");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t707.txt=");
					SerialWrite(0x22);
					SerialPrint(configMulti.speedPercent_Axis_M3);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t708.txt=");
					SerialWrite(0x22);
					SerialPrint(configMulti.maxSpd_Axis_M3);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t709.txt=");
					SerialWrite(0x22);
					SerialPrint(configMulti.accel_Axis_M3);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t710.txt=");
					SerialWrite(0x22);
					idAndChecked = 4 & configMulti.multiAxesIDs;
					if (idAndChecked == 4)
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
						if (dirAndChecked == 4)
						{
							SerialPrint("Left");
						}
						else
						{
							SerialPrint("Right");
						}
					}
					else
					{
						if (dirAndChecked == 4)
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
					SerialPrint(configMulti.speedPercent_Axis_M4);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t908.txt=");
					SerialWrite(0x22);
					SerialPrint(configMulti.maxSpd_Axis_M4);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t909.txt=");
					SerialWrite(0x22);
					SerialPrint(configMulti.accel_Axis_M4);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t910.txt=");
					SerialWrite(0x22);
					idAndChecked = 8 & configMulti.multiAxesIDs;
					if (idAndChecked == 8)
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
						if (dirAndChecked == 8)
						{
							SerialPrint("Left");
						}
						else
						{
							SerialPrint("Right");
						}
					}
					else
					{
						if (dirAndChecked == 8)
						{
							SerialPrint("CW");
						}
						else
						{
							SerialPrint("CCW");
						}

					}
					SerialPrint(nextionQuoteEnd);
				//	break;
				//}
			//}
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
					SerialPrint(configIndex_Prime.speedPercent_Axis_M3);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t302.txt=");
					SerialWrite(0x22);
					SerialPrint(configIndex_Prime.maxSpd_Axis_M3);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t303.txt=");
					SerialWrite(0x22);
					SerialPrint(configIndex_Prime.accel_Axis_M3);
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
					SerialPrint(configIndex_Prime.speedPercent_Axis_M4);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t302.txt=");
					SerialWrite(0x22);
					SerialPrint(configIndex_Prime.maxSpd_Axis_M4);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t303.txt=");
					SerialWrite(0x22);
					SerialPrint(configIndex_Prime.accel_Axis_M4);
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
					SerialPrint(configIndex_1.sizeInUnits,5);
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
					SerialPrint(configIndex_4.sizeInUnits,4);
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
					SerialPrint(configMove.maxSpd_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t303.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.accel_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t301.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.speedPercent_Axis_Z);
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
					SerialPrint(configMove.maxSpd_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t303.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.accel_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t301.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.speedPercent_Axis_Z);
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
					SerialPrint(configMove.maxSpd_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t303.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.accel_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t301.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.speedPercent_Axis_X);
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
					SerialPrint(configMove.maxSpd_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t303.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.accel_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t301.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.speedPercent_Axis_X);
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
					SerialPrint(configMove.maxSpd_Motor_3);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t303.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.accel_Motor_3);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t301.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.speedPercent_Axis_M3);
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
					SerialPrint(configMove.maxSpd_Motor_3);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t303.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.accel_Motor_3);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t301.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.speedPercent_Axis_M3);
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
					SerialPrint(configMove.maxSpd_Motor_4);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t303.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.accel_Motor_4);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t301.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.speedPercent_Axis_M4);
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
					SerialPrint(configMove.maxSpd_Motor_4);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t303.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.accel_Motor_4);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t301.txt=");
					SerialWrite(0x22);
					SerialPrint(configMove.speedPercent_Axis_M4);
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
				SerialPrint(configSync.maxSpd_Axis_Z);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t306.txt=");
				SerialWrite(0x22);
				SerialPrint(configSync.accel_Axis_Z);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t304.txt=");
				SerialWrite(0x22);
				SerialPrint(configSync.speedPercent_Axis_Z);
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
				SerialPrint(configSync.maxSpd_Axis_X);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t306.txt=");
				SerialWrite(0x22);
				SerialPrint(configSync.accel_Axis_X);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t304.txt=");
				SerialWrite(0x22);
				SerialPrint(configSync.speedPercent_Axis_X);
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
				SerialPrint(configSync.maxSpd_Axis_M3);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t306.txt=");
				SerialWrite(0x22);
				SerialPrint(configSync.accel_Axis_M3);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t304.txt=");
				SerialWrite(0x22);
				SerialPrint(configSync.speedPercent_Axis_M3);
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
				SerialPrint(configSync.maxSpd_Axis_M4);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t306.txt=");
				SerialWrite(0x22);
				SerialPrint(configSync.accel_Axis_M4);
				SerialPrint(nextionQuoteEnd);

				SerialPrint("pageEEPROM.t304.txt=");
				SerialWrite(0x22);
				SerialPrint(configSync.speedPercent_Axis_M4);
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

			switch (configRec.radial_axial)
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
					SerialPrint(configRec.maxSpd_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t306.txt=");
					SerialWrite(0x22);
					SerialPrint(configRec.accel_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t304.txt=");
					SerialWrite(0x22);
					SerialPrint(configRec.speedPercent_Axis_Z);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case ID_AXIS_X: // X Axis
				{


					configPageRec eePromRecPage;
					EEPROM.get(eePromAddress_Rec, eePromRecPage);
					Serial.print("eePromRecPage.maxSpd_Axis_X: ");
					Serial.println(eePromRecPage.maxSpd_Axis_X);


					SerialPrint("pageEEPROM.t303.txt=");
					SerialWrite(0x22);
					SerialPrint("X");
					SerialPrint(nextionQuoteEnd);

					Serial.print("configRec.maxSpd_Axis_X: ");
					Serial.print(configRec.maxSpd_Axis_X);

					SerialPrint("pageEEPROM.t305.txt=");
					SerialWrite(0x22);
					SerialPrint(configRec.maxSpd_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t306.txt=");
					SerialWrite(0x22);
					SerialPrint(configRec.accel_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t304.txt=");
					SerialWrite(0x22);
					SerialPrint(configRec.speedPercent_Axis_X);
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
					SerialPrint(configRec.maxSpd_Axis_M3);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t306.txt=");
					SerialWrite(0x22);
					SerialPrint(configRec.accel_Axis_M3);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t304.txt=");
					SerialWrite(0x22);
					SerialPrint(configRec.speedPercent_Axis_M3);
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
					SerialPrint(configRec.maxSpd_Axis_M4);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t306.txt=");
					SerialWrite(0x22);
					SerialPrint(configRec.accel_Axis_M4);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t304.txt=");
					SerialWrite(0x22);
					SerialPrint(configRec.speedPercent_Axis_M4);
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
			SerialPrint(configGreekKey.patternCount_Pattern);
			SerialPrint(nextionQuoteEnd);
			SerialPrint("pageEEPROM.t308.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.countPatternPer360_Pattern);
			SerialPrint(nextionQuoteEnd);
			SerialPrint("pageEEPROM.t309.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.segmentLength_Pattern);
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
					SerialPrint(configGreekKey.maxSpd_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t306.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.accel_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t304.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.speedPercent_Axis_Z);
					SerialPrint(nextionQuoteEnd);
					break;
				}
				case ID_AXIS_X: // X Axis
				{
					configPageGreekKey eePromGrkPage;
					EEPROM.get(eePromAddress_Grk, eePromGrkPage);
					Serial.print("eePromGrkPage.maxSpd_Axis_X: ");
					Serial.println(eePromGrkPage.maxSpd_Axis_X);

					SerialPrint("pageEEPROM.t303.txt=");
					SerialWrite(0x22);
					SerialPrint("X");
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t305.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.maxSpd_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t306.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.accel_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t304.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.speedPercent_Axis_X);
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
					SerialPrint(configGreekKey.maxSpd_Axis_M3);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t306.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.accel_Axis_M3);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t304.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.speedPercent_Axis_M3);
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
					SerialPrint(configGreekKey.maxSpd_Axis_M4);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t306.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.accel_Axis_M4);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t304.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.speedPercent_Axis_M4);
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
			SerialPrint(configGreekKey.patternCount_File);
			SerialPrint(nextionQuoteEnd);
			SerialPrint("pageEEPROM.t308.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.countPatternPer360_File);
			SerialPrint(nextionQuoteEnd);
			SerialPrint("pageEEPROM.t309.txt=");
			SerialWrite(0x22);
			SerialPrint(configGreekKey.segmentLength_File);
			SerialPrint(nextionQuoteEnd);

			SerialPrint("pageEEPROM.t312.txt=");
			SerialWrite(0x22);
			String programFilename =  GetFilenameFromEEPROM();
			SerialPrint(programFilename);
			SerialPrintln(nextionQuoteEnd);
			//////////////////////////////////////////////////
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





			///////////////////////////////

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
					SerialPrint(configGreekKey.maxSpd_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t306.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.accel_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t304.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.speedPercent_Axis_Z);
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
					SerialPrint(configGreekKey.maxSpd_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t306.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.accel_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t304.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.speedPercent_Axis_X);
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
					SerialPrint(configGreekKey.maxSpd_Axis_M3);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t306.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.accel_Axis_M3);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t304.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.speedPercent_Axis_M3);
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
					SerialPrint(configGreekKey.maxSpd_Axis_M4);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t306.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.accel_Axis_M4);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t304.txt=");
					SerialWrite(0x22);
					SerialPrint(configGreekKey.speedPercent_Axis_M4);
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
					SerialPrint(configRose.speedPercent_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t305.txt=");
					SerialWrite(0x22);
					SerialPrint(configRose.maxSpd_Axis_Z);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t306.txt=");
					SerialWrite(0x22);
					SerialPrint(configRose.accel_Axis_Z);
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
					SerialPrint(configRose.maxSpd_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t306.txt=");
					SerialWrite(0x22);
					SerialPrint(configRose.accel_Axis_X);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t304.txt=");
					SerialWrite(0x22);
					SerialPrint(configRose.speedPercent_Axis_X);
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
					SerialPrint(configRose.maxSpd_Axis_M3);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t306.txt=");
					SerialWrite(0x22);
					SerialPrint(configRose.accel_Axis_M3);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t304.txt=");
					SerialWrite(0x22);
					SerialPrint(configRose.speedPercent_Axis_M3);
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
					SerialPrint(configRose.maxSpd_Axis_M4);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t306.txt=");
					SerialWrite(0x22);
					SerialPrint(configRose.accel_Axis_M4);
					SerialPrint(nextionQuoteEnd);

					SerialPrint("pageEEPROM.t304.txt=");
					SerialWrite(0x22);
					SerialPrint(configRose.speedPercent_Axis_M4);
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

/// <summary>
/// LoadSettings
/// </summary>
/// <comment>
/// Load ini file settings
/// </comment>
/// <returns></returns>
void LoadSettings() // ToDo
{
	//const char* nextionEnd = "\xFF\xFF\xFF";
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
	LoadSettings_PageMulti();
	LoadSettings_PageIndex();
	LoadSettings_PageMove();
	LoadSettings_Shared();
	LoadSettings_PageSync();
	LoadSettings_PageRec();
	LoadSettings_PageGrk();
	LoadSettings_PageProgram();
	LoadSettings_PageRose();

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

	SerialPrint("click n1,0");
	SerialPrint(nextionEnd);
	//SerialPrint("click m0,0");
	//SerialPrint(nextionEnd);

	SerialPrint("click n4,0");
	SerialPrint(nextionEnd);
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

	SerialPrint("pageMore.vEEPROM.val=");
	SerialPrint(eePromAddress);
	SerialPrint(nextionEnd);

	SerialPrint("pageMore.v00.val=");
	SerialPrint(value0);
	SerialPrint(nextionEnd);

	SerialPrint("pageMore.v08.val=");
	SerialPrint(value8);
	SerialPrint(nextionEnd);

	SerialPrint("pageMore.v16.val=");
	SerialPrint(value16);
	SerialPrint(nextionEnd);

	SerialPrint("pageMore.v24.val=");
	SerialPrint(value24);
	SerialPrint(nextionEnd);

	SerialPrint("click m0,0");
	SerialPrint(nextionEnd);

	Serial.print("SendPackedData: ");
	Serial.println(eePromAddress);
	Serial.println(value0);
	Serial.println(value8);
	Serial.println(value16);
	Serial.println(value24);
}

/// <summary>
/// LoadSettings_PageIndex
/// </summary>
/// <comment>
/// Load ini file settings for Index page
/// </comment>
/// <returns></returns>
void LoadSettings_PageIndex() // ToDo
{
	// Index Main
	const char* iniKey = "Index";

	// EEPROM 104
	value0 = (uint8_t)configIndex_Prime.speedPercent_Spindle;
	value8 = (uint8_t)configIndex_Prime.axisId;
	value16 = (uint8_t)configIndex_Prime.indexId;
	value24 = (uint8_t)configSetup.cutterMotorPin;

	
	int eePromAddress_Nextion = 104;
	const char* iniValue = "SpeedPercentage_Spindle";
	float returnVal = ReturnIniValue(iniKey, iniValue);
	configIndex_Prime.speedPercent_Spindle = (int)returnVal;
	value0 = (uint8_t)configIndex_Prime.speedPercent_Spindle;

	iniValue = "AxisId";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configIndex_Prime.indexId = (int)returnVal;
	value8 = (uint8_t)configIndex_Prime.axisId;

	iniValue = "IndexId";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configIndex_Prime.indexId = (int)returnVal;
	value16 = (uint8_t)configIndex_Prime.indexId;

	iniKey = "Preferences";
	iniValue = "CutterMotorPin";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.cutterMotorPin = (int)returnVal;
	value24 = (uint8_t)configSetup.cutterMotorPin;

	// Send to Nextion
	SendPackedData(eePromAddress_Nextion);


	iniKey = "Index";
	iniValue = "MaxSpeed_Spindle";
	eePromAddress_Nextion = 904;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_Prime.maxSpd_Spindle = (int)returnVal;

	iniValue = "Accel_Spindle";
	eePromAddress_Nextion = 944;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_Prime.accel_Spindle = (int)returnVal;


	iniValue = "Synchro_M3_and_Spindle";
	eePromAddress_Nextion = 156;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_Prime.synchro_M3_Spindle = (int)returnVal;

	iniValue = "Synchro_Percentage";
	eePromAddress_Nextion = 180;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configIndex_Prime.synchro_M3_Percentage = returnVal;

	// Processed in LoadSettings_PageLimits
	//// Axis Speed %
	//// EEPROM 368
	//value0 = configSetup.limit_Min_M4;
	//value8 = configSetup.limit_Max_M4;
	//value16 = configIndex_Prime.speedPercent_Axis_M3; 
	//value24 = configIndex_Prime.speedPercent_Axis_M4; 

	//iniValue = "SpeedPercentage_M3";
	//eePromAddress_Nextion = 368;
	//returnVal = SetIniValueCombined(iniKey, iniValue, eePromAddress_Nextion, 16, value0, value8, value16, value24);
	//configIndex_Prime.speedPercent_Axis_M3 = (int)returnVal;
	//value16 = configIndex_Prime.speedPercent_Axis_M3;

	//iniValue = "SpeedPercentage_M4";
	//eePromAddress_Nextion = 368;
	//returnVal = SetIniValueCombined(iniKey, iniValue, eePromAddress_Nextion, 24, value0, value8, value16, value24);
	//configIndex_Prime.speedPercent_Axis_M4 = (int)returnVal;

	iniValue = "Accel_M3";
	eePromAddress_Nextion = 44;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_Prime.accel_Axis_M3 = (int)returnVal;

	iniValue = "MaxSpeed_M3";
	eePromAddress_Nextion = 32;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_Prime.maxSpd_Axis_M3 = (int)returnVal;

	iniValue = "Accel_M4";
	eePromAddress_Nextion = 560;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_Prime.accel_Axis_M4 = (int)returnVal;

	iniValue = "MaxSpeed_M4";
	eePromAddress_Nextion = 508;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configIndex_Prime.maxSpd_Axis_M4 = (int)returnVal;


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
	IndexSize(returnVal, ID_INDEX_1);

	Serial.print("returnVal:  ");
	Serial.println(returnVal);
	Serial.print("configIndex_1.sizeInSteps: ");
	Serial.println(configIndex_1.sizeInSteps);

	Serial.print("configIndex_1.sizeInUnits: ");
	Serial.println(configIndex_1.sizeInUnits);

	// Index 2
	iniValue = "DivisionsOrDegrees_2";
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
	//configIndex_2.sizeInUnits = returnVal;
	IndexSize(returnVal, ID_INDEX_2);

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
	//configIndex_3.sizeInUnits = returnVal;
	IndexSize(returnVal, ID_INDEX_3);

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
	//configIndex_4.sizeInUnits = returnVal;
	IndexSize(returnVal, ID_INDEX_4);

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
	//configIndex_5.sizeInUnits = returnVal;
	IndexSize(returnVal, ID_INDEX_5);

	EEPROM.put(eePromAddress_Ind_Prime, configIndex_Prime);
	EEPROM.put(eePromAddress_Ind_1, configIndex_1);
	EEPROM.put(eePromAddress_Ind_2, configIndex_2);
	EEPROM.put(eePromAddress_Ind_3, configIndex_3);
	EEPROM.put(eePromAddress_Ind_4, configIndex_4);
	EEPROM.put(eePromAddress_Ind_5, configIndex_5);
	EEPROM.put(eePromAddress_Setup, configSetup);
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

	// EEPROM 100
	value0 = (uint8_t)configRose.speedPercent_Spindle;
	value8 = (uint8_t)configRose.axisId;
	value16 = (uint8_t)configRose.n;
	value24 = (uint8_t)configRose.d;

	int eePromAddress_Nextion = 100;
	const char* iniValue = "SpeedPercentage_Spindle";
	float returnVal = ReturnIniValue(iniKey, iniValue);
	configRose.speedPercent_Spindle = (int)returnVal;
	value0 = (uint8_t)configRose.speedPercent_Spindle;

	iniValue = "AxisId";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRose.axisId = (int)returnVal;
	value8 = (uint8_t)configRose.axisId;

	iniValue = "Rose_n";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRose.n = (int)returnVal;
	value16 = (uint8_t)configRose.n;

	iniValue = "Rose_d";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRose.d = (int)returnVal;
	value24 = (uint8_t)configRose.d;

	// Send to Nextion
	SendPackedData(eePromAddress_Nextion);

	//// Pack data and send to Nextion
	//packedValue = value24 << 24 | value16 << 16 | value8 << 8 | value0;
	//SerialPrint("wepo ");
	//SerialPrint(packedValue);
	//SerialPrint(",");
	//SerialPrint(eePromAddress_Nextion);
	//SerialPrint(nextionEnd);



	iniValue = "RadialOrAxial";
	eePromAddress_Nextion = 240;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.radial_Axial = (int)returnVal;

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



	// Axis Speed %
    // EEPROM 52
	value0 = (uint8_t)configRose.speedPercent_Axis_Z;
	value8 = (uint8_t)configRose.speedPercent_Axis_X;
	value16 = (uint8_t)configRose.speedPercent_Axis_M3;
	value24 = (uint8_t)configRose.speedPercent_Axis_M4;

	eePromAddress_Nextion = 52;
	iniValue = "SpeedPercentage_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRose.speedPercent_Axis_Z = (int)returnVal;
	value0 = (uint8_t)configRose.speedPercent_Axis_Z;

	iniValue = "SpeedPercentage_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRose.speedPercent_Axis_X = (int)returnVal;
	value8 = (uint8_t)configRose.speedPercent_Axis_X;

	iniValue = "SpeedPercentage_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRose.speedPercent_Axis_M3 = (int)returnVal;
	value16 = (uint8_t)configRose.speedPercent_Axis_M3;

	iniValue = "SpeedPercentage_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRose.speedPercent_Axis_M4 = (int)returnVal;
	value24 = (uint8_t)configRose.speedPercent_Axis_M4;

	// Send to Nextion
	SendPackedData(eePromAddress_Nextion);
	//// Pack data and send to Nextion
	//packedValue = value24 << 24 | value16 << 16 | value8 << 8 | value0;
	//SerialPrint("wepo ");
	//SerialPrint(packedValue);
	//SerialPrint(",");
	//SerialPrint(eePromAddress_Nextion);
	//SerialPrint(nextionEnd);


	// Z axis
	iniValue = "MaxSpeed_Z";
	eePromAddress_Nextion = 880;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.maxSpd_Axis_Z = (int)returnVal;

	iniValue = "Accel_Z";
	eePromAddress_Nextion = 948;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.accel_Axis_Z = (int)returnVal;

	iniValue = "RadialAmplitude_Z";
	eePromAddress_Nextion = 276;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configRose.amplitude_Radial_Z = returnVal;

	iniValue = "AxialAmplitude_Z";
	eePromAddress_Nextion = 292;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configRose.amplitude_Axial_Z = returnVal;

	// X axis
	iniValue = "MaxSpeed_X";
	eePromAddress_Nextion = 876;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.maxSpd_Axis_X = (int)returnVal;

	iniValue = "Accel_X";
	eePromAddress_Nextion = 868;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.accel_Axis_X = (int)returnVal;

	iniValue = "RadialAmplitude_X";
	eePromAddress_Nextion = 284;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configRose.amplitude_Radial_X = returnVal;

	iniValue = "AxialAmplitude_X";
	eePromAddress_Nextion = 224;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configRose.amplitude_Axial_X = returnVal;

	// M3 axis
	iniValue = "MaxSpeed_M3";
	eePromAddress_Nextion = 824;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.maxSpd_Axis_M3 = (int)returnVal;

	iniValue = "Accel_M3";
	eePromAddress_Nextion = 820;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.accel_Axis_M3 = (int)returnVal;

	iniValue = "RadialAmplitude_M3";
	eePromAddress_Nextion = 520;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configRose.amplitude_Radial_M3 = returnVal;

	iniValue = "AxialAmplitude_M3";
	eePromAddress_Nextion = 528;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configRose.amplitude_Axial_M3 = returnVal;


	// M4 axis
	iniValue = "MaxSpeed_M4";
	eePromAddress_Nextion = 452;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.maxSpd_Axis_M4 = (int)returnVal;

	iniValue = "Accel_M4";
	eePromAddress_Nextion = 208;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configRose.accel_Axis_M4 = (int)returnVal;

	iniValue = "RadialAmplitude_M4";
	eePromAddress_Nextion = 68;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configRose.amplitude_Radial_M4 = returnVal;

	iniValue = "AxialAmplitude_M4";
	eePromAddress_Nextion = 60;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
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

	// Speed %
	// EEPROM 348
	value0 = (uint8_t)configMove.speedPercent_Axis_Z;
	value8 = (uint8_t)configMove.speedPercent_Axis_X;
	value16 = (uint8_t)configMove.speedPercent_Axis_M3;
	value24 = (uint8_t)configMove.speedPercent_Axis_M4;

	const char* iniValue = "SpeedPercentage_Z"; // 0
	int eePromAddress_Nextion = 348;
	float returnVal = (uint8_t)ReturnIniValue(iniKey, iniValue);
	configMove.speedPercent_Axis_Z = returnVal;
	value0 = configMove.speedPercent_Axis_Z;

	iniValue = "SpeedPercentage_X"; // 8
	eePromAddress_Nextion = 348;
	returnVal = (uint8_t)ReturnIniValue(iniKey, iniValue);
	configMove.speedPercent_Axis_X = returnVal;
	value8 = configMove.speedPercent_Axis_X;

	iniValue = "SpeedPercentage_M3";  // 16
	eePromAddress_Nextion = 348;
	returnVal = (uint8_t)ReturnIniValue(iniKey, iniValue);
	configMove.speedPercent_Axis_M3 = returnVal;
	value16 = configMove.speedPercent_Axis_M3;

	iniValue = "SpeedPercentage_M4"; // 24
	eePromAddress_Nextion = 348;
	returnVal = (uint8_t)ReturnIniValue(iniKey, iniValue);
	configMove.speedPercent_Axis_M4 = returnVal;
	value24 = configMove.speedPercent_Axis_M4;

	// Send to Nextion
	SendPackedData(eePromAddress_Nextion);

	// Z axis
	iniValue = "MaxSpeed_Z";
	eePromAddress_Nextion = 968;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMove.maxSpd_Axis_Z = (uint32_t)returnVal;

	iniValue = "Accel_Z";
	eePromAddress_Nextion = 988;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMove.accel_Axis_Z = (uint32_t)returnVal;



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


	iniValue = "Distance_X1";
	eePromAddress_Nextion = 340;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configMove.distance_MoveX1 = returnVal;

	iniValue = "Distance_X2";
	eePromAddress_Nextion = 316;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configMove.distance_MoveX2 = returnVal;

	// M3 axis
	iniValue = "MaxSpeed_M3";
	eePromAddress_Nextion = 888;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMove.maxSpd_Motor_3 = (int)returnVal;

	iniValue = "Accel_M3";
	eePromAddress_Nextion = 892;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMove.accel_Motor_3 = (int)returnVal;

	iniValue = "Distance_M3-1";
	eePromAddress_Nextion = 332;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configMove.distance_MoveM3_1 = returnVal;
	EEPROM.put(eePromAddress_Mov, configMove);

	iniValue = "Distance_M3-2";
	eePromAddress_Nextion = 500;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configMove.distance_MoveM3_2 = returnVal;

	// M4 axis
	iniValue = "MaxSpeed_M4";
	eePromAddress_Nextion = 212;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMove.maxSpd_Motor_4 = (int)returnVal;

	iniValue = "Accel_M4";
	eePromAddress_Nextion = 420;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMove.accel_Motor_4 = (int)returnVal;

	iniValue = "Distance_M4-1";
	eePromAddress_Nextion = 12;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configMove.distance_MoveM4_1 = returnVal;
	EEPROM.put(eePromAddress_Mov, configMove);

	iniValue = "Distance_M4-2";
	eePromAddress_Nextion = 36;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
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
void LoadSettings_PageGrk()
{
	// Greek Key Main
	const char* iniKey = "GreekKey";

	const char* iniValue = "PatternsPer360";
	uint32_t eePromAddress_Nextion = 372;
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.countPatternPer360_Pattern = returnVal;

	iniValue = "PatternCount";
	eePromAddress_Nextion = 496;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.patternCount_Pattern = (int)returnVal;

	iniValue = "SegmentLength";
	eePromAddress_Nextion = 620;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configGreekKey.segmentLength_Pattern = returnVal;

	EEPROM.put(eePromAddress_Grk, configGreekKey);

}

void LoadSettings_PageProgram()
{
	// Greek Key Main
	const char* iniKey = "Program";

	int eePromAddress_Nextion = 436;
	const char* iniValue = "SegmentsOrActual"; // 0
	float returnVal = (uint8_t)ReturnIniValue(iniKey, iniValue);
	configGreekKey.segmentOrActual = returnVal;
	value0 = configGreekKey.segmentOrActual;

	iniKey = "GreekKey";
	iniValue = "PatternType"; // 8
	returnVal = ReturnIniValue(iniKey, iniValue);
	configGreekKey.patternId = (uint8_t)returnVal;
	value8 = configGreekKey.patternId;

	iniKey = "Recip";
	iniValue = "Style"; // 16
	eePromAddress_Nextion = 436;
	returnVal = (uint8_t)ReturnIniValue(iniKey, iniValue);
	configRec.style = returnVal;
	value16 = configRec.style;

	iniKey = "Move";
	iniValue = "AxisId"; // 24
	eePromAddress_Nextion = 436;
	returnVal = (uint8_t)ReturnIniValue(iniKey, iniValue);
	configMove.axisId = returnVal;
	value24 = configMove.axisId;

	// Send to Nextion
	SendPackedData(eePromAddress_Nextion);

	iniKey = "Program";
	iniValue = "SegmentLength";
	eePromAddress_Nextion = 300;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configGreekKey.segmentLength_File = returnVal;

	iniValue = "PatternsPer360";
	eePromAddress_Nextion = 636;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configGreekKey.countPatternPer360_File = returnVal;

	iniValue = "PatternCount";
	eePromAddress_Nextion = 756;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configGreekKey.patternCount_File = (int)returnVal;

	EEPROM.put(eePromAddress_Mov, configMove);
	EEPROM.put(eePromAddress_Rec, configRec);
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

	const char* iniValue = "Radial_Waves";
	int eePromAddress_Nextion = 556;
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
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
/// LoadSettings_PageMulti
/// </summary>
/// <comment>
/// Load ini file settings for Multi page
/// </comment>
/// <returns></returns>
void LoadSettings_PageMulti()
{
	const char* iniKey = "One_Multi";

	// EEPROM 88
	int eePromAddress_Nextion = 88;

	const char* iniValue = "SpeedPercentage_Spindle"; // 0
	float returnVal = ReturnIniValue(iniKey, iniValue);
	configMulti.speedPercent_Spindle = (int)returnVal;
	value0 = (uint8_t)configMulti.speedPercent_Spindle;

	iniValue = "AxisId";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMulti.axisId = (int)returnVal;
	value8 = (uint8_t)configMulti.axisId;

	iniValue = "MultiAxesIds";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMulti.accel_Spindle = (int)returnVal;
	value16 = (uint8_t)configMulti.multiAxesIDs;

	iniValue = "Direction";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMulti.direction = (uint32_t)returnVal;
	//configMulti.accel_Spindle = (int)returnVal;
	value24 = (uint8_t)configMulti.direction;

	// Send to Nextion
	SendPackedData(eePromAddress_Nextion);

	// EEPROM 76
	iniValue = "SpeedPercentage_Z";
	eePromAddress_Nextion = 76;
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMulti.speedPercent_Axis_Z = (int)returnVal;
	value0 = (uint8_t)configMulti.speedPercent_Axis_Z;

	iniValue = "SpeedPercentage_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMulti.speedPercent_Axis_X = (int)returnVal;
	value8 = (uint8_t)configMulti.speedPercent_Axis_X;

	iniValue = "SpeedPercentage_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMulti.speedPercent_Axis_M3 = (int)returnVal;
	value16 = (uint8_t)configMulti.speedPercent_Axis_M3;

	iniValue = "SpeedPercentage_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMulti.speedPercent_Axis_M4 = (int)returnVal;
	value24 = (uint8_t)configMulti.speedPercent_Axis_M4;

	// Send to Nextion
	SendPackedData(eePromAddress_Nextion);

	// MaxSpeed
	iniValue = "MaxSpeed_Spindle";
	eePromAddress_Nextion = 480;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMulti.maxSpd_Spindle = (int)returnVal;

	iniValue = "MaxSpeed_Z";
	eePromAddress_Nextion = 148;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMulti.maxSpd_Axis_Z = (int)returnVal;

	iniValue = "MaxSpeed_X";
	eePromAddress_Nextion = 164;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMulti.maxSpd_Axis_X = (int)returnVal;

	iniValue = "MaxSpeed_M3";
	eePromAddress_Nextion = 108;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMulti.maxSpd_Axis_M3 = (int)returnVal;

	iniValue = "MaxSpeed_M4";
	eePromAddress_Nextion = 376;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMulti.maxSpd_Axis_M4 = (int)returnVal;

	// Accel
	iniValue = "Accel_Spindle";
	eePromAddress_Nextion = 648;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMulti.accel_Spindle = (int)returnVal;

	iniValue = "Accel_Z";
	eePromAddress_Nextion = 432;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMulti.accel_Axis_Z = (int)returnVal;

	iniValue = "Accel_X";
	eePromAddress_Nextion = 456;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMulti.accel_Axis_X = (int)returnVal;

	iniValue = "Accel_M3";
	eePromAddress_Nextion = 476;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMulti.accel_Axis_M3 = (int)returnVal;

	iniValue = "Accel_M4";
	eePromAddress_Nextion = 644;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMulti.accel_Axis_M4 = (int)returnVal;

	// TargetDistance
	iniValue = "TargetDistance_Spindle";
	eePromAddress_Nextion = 388;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configMulti.targetDistance_Spindle = returnVal;

	iniValue = "TargetDistance_Z";
	eePromAddress_Nextion = 652;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configMulti.targetDistance_Z = returnVal;

	iniValue = "TargetDistance_X";
	eePromAddress_Nextion = 140;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configMulti.targetDistance_X = returnVal;

	iniValue = "TargetDistance_M3";
	eePromAddress_Nextion = 244;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configMulti.targetDistance_M3 = returnVal;

	iniValue = "TargetDistance_M4";
	eePromAddress_Nextion = 256;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configMulti.targetDistance_M4 = returnVal;

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
	//const char* nextionEnd = "\xFF\xFF\xFF";
	value0 = (uint8_t)configMain.speedPercent_Spindle;
	value8 = (uint8_t)configMain.axisId;
	value16 = (uint8_t)configSetup.vendorId;
	value24 =(uint8_t)configSetup.motorCount;

	const char* iniKey = "Main";
	int eePromAddress_Nextion = 0;

	const char* iniValue = "SpeedPercentage_Spindle"; // (0)
	float returnVal = ReturnIniValue(iniKey, iniValue);//, eePromAddress_Nextion, false);
	configMain.speedPercent_Spindle = (int)returnVal;
	value0 = (uint8_t)returnVal;

	iniValue = "AxisId"; // (8)
	returnVal = ReturnIniValue(iniKey, iniValue);// , eePromAddress_Nextion, false);
	configMain.axisId = (int)returnVal;
	value8 = (uint8_t)returnVal;

	iniKey = "Preferences";
	// Show Vendor Splashscreen
	iniValue = "VendorId";// (16)
	returnVal = ReturnIniValue(iniKey, iniValue);//, eePromAddress_Nextion, false);
	configSetup.vendorId = (int)returnVal;
	value16 = (uint8_t)returnVal;

	iniValue = "MotorCount";// (24)
	returnVal = ReturnIniValue(iniKey, iniValue);//, eePromAddress_Nextion, false);
	configSetup.motorCount = (int)returnVal;
	value24 = (uint8_t)returnVal;

	// Send to Nextion
	SendPackedData(eePromAddress_Nextion);

	//// Pack data and send to Nextion
	//packedValue = (uint8_t)configSetup.motorCount << 24 | (uint8_t)configSetup.vendorId << 16 | (uint8_t)configMain.axisId << 8 | (uint8_t)configMain.speedPercent_Spindle;
	//SerialPrint("wepo ");
	//SerialPrint(packedValue);
	//SerialPrint(",");
	//SerialPrint(eePromAddress_Nextion);
	//SerialPrint(nextionEnd);

//#ifdef DEBUG
//
//	Serial.println(iniKey);
//	Serial.println(iniValue);
//	Serial.print("wepo ");
//	Serial.print(packedValue);
//	Serial.print(",");
//	Serial.println(eePromAddress_Nextion);
//
//	Serial.print("value0: ");
//	Serial.println(value0);
//	Serial.print("value8: ");
//	Serial.println(value8);
//	Serial.print("value16: ");
//	Serial.println(value16);
//	Serial.print("value24: ");
//	Serial.println(value24);
//#endif // DEBUG

	iniKey = "Main";
	eePromAddress_Nextion = 872;
	iniValue = "MaxSpeed_Spindle";
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMain.maxSpd_Spindle = (int)returnVal;

	iniValue = "Accel_Spindle";
	eePromAddress_Nextion = 912;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMain.accel_Spindle = (int)returnVal;

	value0 = configMain.speedPercent_Axis_Z;
	value8 = configMain.speedPercent_Axis_X;
	value16 = configMain.speedPercent_Axis_M3;
	value24 = configMain.speedPercent_Axis_M4;
	eePromAddress_Nextion = 8;

	iniValue = "SpeedPercentage_Z"; // 0
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMain.speedPercent_Axis_Z = (int)returnVal;
	value0 = (uint8_t)configMain.speedPercent_Axis_Z;

	iniValue = "SpeedPercentage_X"; // 8
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMain.speedPercent_Axis_X = (int)returnVal;
	value8 = (uint8_t)configMain.speedPercent_Axis_X;

	iniValue = "SpeedPercentage_M3"; // 16
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMain.speedPercent_Axis_M3 = (int)returnVal;
	value16 = (uint8_t)configMain.speedPercent_Axis_M3;

	iniValue = "SpeedPercentage_M4"; // 24
	returnVal = ReturnIniValue(iniKey, iniValue);
	configMain.speedPercent_Axis_M4 = (int)returnVal;
	value24 = (uint8_t)configMain.speedPercent_Axis_M4;

	// Send to Nextion
	SendPackedData(eePromAddress_Nextion);

	//// Pack data and send to Nextion
	//packedValue = (uint8_t)configMain.speedPercent_Axis_M4 << 24 | (uint8_t)configMain.speedPercent_Axis_M3 << 16 | (uint8_t)configMain.speedPercent_Axis_X << 8 | (uint8_t)configMain.speedPercent_Axis_Z;
	//SerialPrint("wepo ");
	//SerialPrint(packedValue);
	//SerialPrint(",");
	//SerialPrint(eePromAddress_Nextion);
	//SerialPrint(nextionEnd);

	iniValue = "MaxSpeed_Z";
	eePromAddress_Nextion = 952;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMain.maxSpd_Axis_Z = (int)returnVal;

	iniValue = "Accel_Z";
	eePromAddress_Nextion = 972;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMain.accel_Axis_Z = (int)returnVal;

	iniValue = "MaxSpeed_X";
	eePromAddress_Nextion = 992;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMain.maxSpd_Axis_X = (int)returnVal;

	iniValue = "Accel_X";
	eePromAddress_Nextion = 1004;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMain.accel_Axis_X = (int)returnVal;

	iniValue = "MaxSpeed_M3";
	eePromAddress_Nextion = 1016;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMain.maxSpd_Axis_M3 = (int)returnVal;

	iniValue = "Accel_M3";
	eePromAddress_Nextion = 1020;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMain.accel_Axis_M3 = (int)returnVal;

	iniValue = "MaxSpeed_M4";
	eePromAddress_Nextion = 916;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMain.maxSpd_Axis_M4 = (int)returnVal;

	iniValue = "Accel_M4";
	eePromAddress_Nextion = 920;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMain.accel_Axis_M4 = (int)returnVal;

	EEPROM.put(eePromAddress_Main, configMain);
	EEPROM.put(eePromAddress_Setup, configSetup);
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
	value0 = (uint8_t)configSync.speedPercent_Spindle;
	value8 = (uint8_t)configSync.axisId;
	value16 = (uint8_t)configRec.radial_axial;
	value24 = (uint8_t)configSync.helixType;

	///////////////////////////////////////////////////////////////
	int eePromAddress_Nextion = 20;

	const char* iniValue = "SpeedPercentage_Spindle"; // 0
	float returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.speedPercent_Spindle = (int)returnVal;
	configGreekKey.speedPercent_Spindle = (int)returnVal;
	configRec.speedPercent_Spindle = (int)returnVal;
	value0 = (uint8_t)configSync.speedPercent_Spindle;

	iniValue = "AxisId";  // 8
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.axisId = (int)returnVal;
	configGreekKey.axisId = (int)returnVal;
	configRec.axisId = (int)returnVal;
	value8 = (uint8_t)configSync.axisId;

	iniValue = "RadialOrAxial"; // 16
	returnVal = ReturnIniValue(iniKey, iniValue);
	configRec.radial_axial = (uint32_t)returnVal;
	configGreekKey.radialOrAxial_Pattern = (uint32_t)returnVal;
	configGreekKey.radialOrAxial_File = (uint32_t)returnVal;
	value16 = (uint8_t)configRec.radial_axial;

	iniKey = "Sync";  // 24
	iniValue = "HelixType"; //Helix Type
	eePromAddress_Nextion = 20;
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.helixType = (int)returnVal;
	value24 = (uint8_t)configSync.helixType;

	// Send to Nextion
	SendPackedData(eePromAddress_Nextion);

	//// Pack data and send to Nextion
	//packedValue = value24 << 24 | value16 << 16 | value8 << 8 | value0;
	//SerialPrint("wepo ");
	//SerialPrint(packedValue);
	//SerialPrint(",");
	//SerialPrint(eePromAddress_Nextion);
	//SerialPrint(nextionEnd);
	/////////////////////////////////////////////////////////////////////////

	iniKey = "Shared";// 0
	iniValue = "MaxSpeed_Spindle";
	eePromAddress_Nextion = 896;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.maxSpd_Spindle = (int)returnVal;
	configGreekKey.maxSpd_Spindle = (int)returnVal;
	configRec.maxSpd_Spindle = (int)returnVal;

	iniValue = "Accel_Spindle";
	eePromAddress_Nextion = 936;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.accel_Spindle = (int)returnVal;
	configGreekKey.accel_Spindle = (int)returnVal;
	configRec.accel_Spindle = (int)returnVal;

	// Axis Speed %
    // EEPROM 48
	value0 = (uint8_t)configSync.speedPercent_Axis_Z;
	value8 = (uint8_t)configSync.speedPercent_Axis_X;
	value16 = (uint8_t)configSync.speedPercent_Axis_M3;
	value24 = (uint8_t)configSync.speedPercent_Axis_M4;
	eePromAddress_Nextion = 48;

	iniValue = "SpeedPercentage_Z";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.speedPercent_Axis_Z = (int)returnVal;
	configGreekKey.speedPercent_Axis_Z = (int)returnVal;
	configRec.speedPercent_Axis_Z = (int)returnVal;
	value0 = (uint8_t)configSync.speedPercent_Axis_Z;

	iniValue = "SpeedPercentage_X";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.speedPercent_Axis_X = (int)returnVal;
	configGreekKey.speedPercent_Axis_X = (int)returnVal;
	configRec.speedPercent_Axis_X = (int)returnVal;
	value8 = (uint8_t)configSync.speedPercent_Axis_X;

	iniValue = "SpeedPercentage_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.speedPercent_Axis_M3 = (int)returnVal;
	configGreekKey.speedPercent_Axis_M3 = (int)returnVal;
	configRec.speedPercent_Axis_M3 = (int)returnVal;
	value16 = (uint8_t)configSync.speedPercent_Axis_M3;

	iniValue = "SpeedPercentage_M4";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSync.speedPercent_Axis_M4 = (int)returnVal;
	configGreekKey.speedPercent_Axis_M4 = (int)returnVal;
	configRec.speedPercent_Axis_M4 = (int)returnVal;
	value24 = (uint8_t)returnVal;

	// Send to Nextion
	SendPackedData(eePromAddress_Nextion);

	//// Pack data and send to Nextion
	//packedValue = value24 << 24 | value16 << 16 | value8 << 8 | value0;
	//SerialPrint("wepo ");
	//SerialPrint(packedValue);
	//SerialPrint(",");
	//SerialPrint(eePromAddress_Nextion);
	//SerialPrint(nextionEnd);
	///////////////////////////

	iniValue = "MaxSpeed_Z";
	eePromAddress_Nextion = 996;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.maxSpd_Axis_Z = (int)returnVal;
	configGreekKey.maxSpd_Axis_Z = (int)returnVal;
	configRec.maxSpd_Axis_Z = (int)returnVal;

	iniValue = "Accel_Z";
	eePromAddress_Nextion = 1008;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.accel_Axis_Z = (int)returnVal;
	configGreekKey.accel_Axis_Z = (int)returnVal;
	configRec.accel_Axis_Z = (int)returnVal;


	iniValue = "MaxSpeed_X";
	eePromAddress_Nextion = 960;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.maxSpd_Axis_X = (int)returnVal;
	configGreekKey.maxSpd_Axis_X = (int)returnVal;
	configRec.maxSpd_Axis_X = (int)returnVal;

	iniValue = "Accel_X";
	eePromAddress_Nextion = 980;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.accel_Axis_X = (int)returnVal;
	configGreekKey.accel_Axis_X = (int)returnVal;
	configRec.accel_Axis_X = (int)returnVal;


	iniValue = "MaxSpeed_M3";
	eePromAddress_Nextion = 796;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.maxSpd_Axis_M3 = (int)returnVal;
	configGreekKey.maxSpd_Axis_M3 = (int)returnVal;
	configRec.maxSpd_Axis_M3 = (int)returnVal;

	iniValue = "Accel_M3";
	eePromAddress_Nextion = 800;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.accel_Axis_M3 = (int)returnVal;
	configGreekKey.accel_Axis_M3 = (int)returnVal;
	configRec.accel_Axis_M3 = (int)returnVal;


	iniValue = "MaxSpeed_M4";
	eePromAddress_Nextion = 964;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.maxSpd_Axis_M4 = (int)returnVal;
	configGreekKey.maxSpd_Axis_M4 = (int)returnVal;
	configRec.maxSpd_Axis_M4 = (int)returnVal;

	iniValue = "Accel_M4";
	eePromAddress_Nextion = 976;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSync.accel_Axis_M4 = (int)returnVal;
	configGreekKey.accel_Axis_M4 = (int)returnVal;
	configRec.accel_Axis_M4 = (int)returnVal;


	iniValue = "Synchro_M3_and_Spindle";
	eePromAddress_Nextion = 4;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configMain.synchro_M3_Spindle = (int)returnVal;

	iniValue = "Synchro_Percentage";
	eePromAddress_Nextion = 24;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configMain.synchro_M3_Percentage = returnVal;

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

	const char* iniValue = "Revolutions";
	int eePromAddress_Nextion = 612;
	float returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configSync.revolutions_Spindle = returnVal;

	iniValue = "Distance"; 
	eePromAddress_Nextion = 628;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
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

	// EEPROM 460
	const char* iniValue = "Polarity_M4";
	int eePromAddress_Nextion = 460;
	float returnVal = ReturnIniValue(iniKey, iniValue);
	returnVal >= 1 ? (configSetup.polarity_Axis_M3 = true) : (configSetup.polarity_Axis_M3 = false);
	value24 = (uint8_t)returnVal;

	iniValue = "Polarity_Spindle";
	eePromAddress_Nextion = 460;
	returnVal = ReturnIniValue(iniKey, iniValue);
	returnVal >= 1 ? (configSetup.polarity_Spindle = true) : (configSetup.polarity_Spindle = false);
	value16 = (uint8_t)returnVal;

	iniValue = "RadialOrLineal_M4";
	eePromAddress_Nextion = 460;
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.radialOrLinear_Axis_M4 = returnVal;
	value8 = (uint8_t)configSetup.radialOrLinear_Axis_M4;

	iniValue = "Polarity_Z";
	eePromAddress_Nextion = 460;
	returnVal = ReturnIniValue(iniKey, iniValue);
	returnVal >= 1 ? (configSetup.polarity_Axis_Z = true) : (configSetup.polarity_Axis_Z = false);
	value0 = (uint8_t)returnVal;

	// Send to Nextion
	SendPackedData(eePromAddress_Nextion);

	//// Pack data and send to Nextion
	//packedValue = value24 << 24 | value16 << 16 | value8 << 8 | value0;
	//SerialPrint("wepo ");
	//SerialPrint(packedValue);
	//SerialPrint(",");
	//SerialPrint(eePromAddress_Nextion);
	//SerialPrint(nextionEnd);

	// EEPROM 176
	iniValue = "Polarity_AltX";
	eePromAddress_Nextion = 176;
	returnVal = ReturnIniValue(iniKey, iniValue);
	returnVal >= 1 ? (configSetup.polarity_Axis_XAlt = true) : (configSetup.polarity_Axis_XAlt = false);
	value0 = (uint8_t)configSetup.polarity_Axis_XAlt;

	iniValue = "Polarity_X";
	eePromAddress_Nextion = 176;
	returnVal = ReturnIniValue(iniKey, iniValue);
	returnVal >= 1 ? (configSetup.polarity_Axis_X = true) : (configSetup.polarity_Axis_X = false);
	value16 = (uint8_t)configSetup.polarity_Axis_X;

	// Send to Nextion
	SendPackedData(eePromAddress_Nextion);

	// EEPROM 540
	eePromAddress_Nextion = 540;
	iniKey = "Preferences";
	iniValue = "Polarity_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	returnVal >= 1 ? (configSetup.polarity_Axis_M3 = true) : (configSetup.polarity_Axis_M3 = false);
	value0 = (uint8_t)returnVal;

	iniKey = "Limits";
	iniValue = "NC-NO";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.limit_NCorNO = (int)returnVal;
	value8 = (uint8_t)configSetup.limit_NCorNO;

	iniKey = "Preferences";
	iniValue = "RadialOrLineal_M3";
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.radialOrLinear_Axis_M3 = (int)returnVal;
	value16 = (uint8_t)configSetup.radialOrLinear_Axis_M3;

	// Keep steppers enabled
	iniValue = "KeepSteppersEnabled";
	eePromAddress_Nextion = 540;
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.keepSteppersEnabled = (int)returnVal;
	value24 = (uint8_t)configSetup.keepSteppersEnabled;

	// Send to Nextion
	SendPackedData(eePromAddress_Nextion);

	// pageSpindle
	iniValue = "Microsteps_Spindle";
	eePromAddress_Nextion = 200;
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

	// Page Z Axis
	iniValue = "Microsteps_Z";
	eePromAddress_Nextion = 196;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.microsteps_Axis_Z = (int)returnVal;

	iniValue = "StepsPer360_Z";
	eePromAddress_Nextion = 468;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.steps360_Axis_Z = (int)returnVal;

	iniValue = "DistancePer360_Z";
	eePromAddress_Nextion = 512;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configSetup.distancePerRev_AxisZ = returnVal;

	// Page X Axis
	// Set default to X axis
	// pageX.vaXaltX
	eePromAddress_Nextion = 188;
	SerialPrint("wepo 0,188");
	SerialPrint(nextionEnd);
	configSetup.xAltX = 0;

	iniValue = "Microsteps_X";
	eePromAddress_Nextion = 564;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.microsteps_Axis_X = (int)returnVal;

	iniValue = "StepsPer360_X";
	eePromAddress_Nextion = 484;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.steps360_Axis_X = (int)returnVal;

	iniValue = "DistancePer360_X";
	eePromAddress_Nextion = 488;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configSetup.distancePerRev_AxisX = returnVal;

	// Page Alternate X Axis
	iniValue = "Microsteps_AltX";
	eePromAddress_Nextion = 568;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.microsteps_Axis_XAlt = (int)returnVal;

	iniValue = "StepsPer360_AltX";
	eePromAddress_Nextion = 204;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.steps360_Axis_XAlt = (int)returnVal;

	iniValue = "DistancePer360_AltX";
	eePromAddress_Nextion = 216;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configSetup.distancePerRev_AxisXAlt = returnVal;

	// M3 Axis
	iniValue = "Microsteps_M3";
	eePromAddress_Nextion = 92;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.microsteps_Axis_M3 = (int)returnVal;

	iniValue = "StepsPer360_M3";
	eePromAddress_Nextion = 544;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.steps360_Axis_M3 = (int)returnVal;

	iniValue = "GearRatio_M3";
	eePromAddress_Nextion = 548;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configSetup.gearRatio_AxisM3 = returnVal;

	iniValue = "DistancePer360_M3";
	eePromAddress_Nextion = 408;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configSetup.distancePerRev_AxisM3 = returnVal;

	iniValue = "M3Radius";
	eePromAddress_Nextion = 688;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configSetup.radiusM3 = returnVal;

	// M4 Axis
	iniValue = "Microsteps_M4";
	eePromAddress_Nextion = 96;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.microsteps_Axis_M4 = (int)returnVal;


	iniValue = "StepsPer360_M4";
	eePromAddress_Nextion = 404;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.steps360_Axis_M4 = (int)returnVal;

	iniValue = "GearRatio_M4";
	eePromAddress_Nextion = 380;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configSetup.gearRatio_AxisM4 = returnVal;
	Serial.print("configSetup.gearRatio_AxisM4:  ");
		Serial.println(configSetup.gearRatio_AxisM4);
		Serial.println("lkjsda;ljasdl;fkjsa;ldkfja;sljfals;kjfla;skjfl;sakjfasdf;lksjdf");

	iniValue = "DistancePer360_M4";
	eePromAddress_Nextion = 600;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
	configSetup.distancePerRev_AxisM4 = returnVal;

	iniValue = "M4Radius";
	eePromAddress_Nextion = 664;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, true);
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

	// EEPROM 576
	//value0 = configSetup.limit_Min_M3;
	//value8 = configSetup.limit_Max_M3;
	//value16 = configSetup.limit_StopSpindle;
	//value24 = configSetup.eStop;

	const char* iniValue = "Min_M3";
	int eePromAddress_Nextion = 576;
	float returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.limit_Min_M3 = (int)returnVal;
	value0 = (uint8_t)configSetup.limit_Min_M3;

	iniValue = "Max_M3";
	eePromAddress_Nextion = 576;
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.limit_Max_M3 = (int)returnVal;
	value8 = (uint8_t)configSetup.limit_Max_M3;

	iniValue = "Stop";
	eePromAddress_Nextion = 576;
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.eStop = (int)returnVal;
	value16 = (uint8_t)configSetup.eStop;

	iniValue = "StopSpindle";
	eePromAddress_Nextion = 576;
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.limit_StopSpindle = (int)returnVal;
	value24 = (uint8_t)configSetup.limit_StopSpindle;

	// Send to Nextion
	SendPackedData(eePromAddress_Nextion);

	//// Pack data and send to Nextion
	//packedValue = value24 << 24 | value16 << 16 | value8 << 8 | value0;
	//SerialPrint("wepo ");
	//SerialPrint(packedValue);
	//SerialPrint(",");
	//SerialPrint(eePromAddress_Nextion);
	//SerialPrint(nextionEnd);
	//Serial.print("packedValue-576: ");
	//Serial.println(packedValue);

	//SerialPrint("pageMore.n9.val=");
	//SerialPrint(packedValue);
	//SerialPrint(nextionEnd);

	// EEPROM 424
	//value0 = configSetup.limit_Min_Z;
	//value8 = configSetup.limit_Max_Z;
	//value16 = configSetup.limit_Min_X;
	//value24 = configSetup.limit_Max_X;

	MilliDelay(100);
	iniValue = "Min_Z";
	eePromAddress_Nextion = 424;
	returnVal = ReturnIniValue(iniKey, iniValue);
	Serial.print("---------ReturnVal-------------------------------Min_z:");
	Serial.println(returnVal);
	MilliDelay(100);
	configSetup.limit_Min_Z = (int)returnVal;
	MilliDelay(100);
	//value0 = (uint8_t)configSetup.limit_Min_Z;
	value0 = (uint8_t)returnVal;
	//uint8_t newZero = (uint8_t)returnVal;

	Serial.print("----------------------------------------value0:");
	Serial.println(value0);
	MilliDelay(100);

	iniValue = "Max_Z";
	eePromAddress_Nextion = 424;
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.limit_Max_Z = (int)returnVal;
	value8 = (uint8_t)configSetup.limit_Max_Z;

	iniValue = "Min_X";
	eePromAddress_Nextion = 424;
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.limit_Min_X = (int)returnVal;
	value16 = (uint8_t)configSetup.limit_Min_X;

	iniValue = "Max_X";
	eePromAddress_Nextion = 424;
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.limit_Max_X = (int)returnVal;
	value24 = (uint8_t)configSetup.limit_Max_X;

	// Send to Nextion
	SendPackedData(eePromAddress_Nextion);

	//// Pack data and send to Nextion
	//uint32_t packedValue424 = value24 << 24 | value16 << 16 | value8 << 8 | newZero;
	////packedValue424 = 35 << 24 | 31 << 16 | 34 << 8 | 30;

	//Serial.print("packedValue424 = ");
	//Serial.print(value24);
	//Serial.print(" << 24 | ");
	//Serial.print(value16);
	//Serial.print(" << 16 | ");
	//Serial.print(value8);
	//Serial.print(" << 8 | ");
	//Serial.print(value0);
	//Serial.println("kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk");



	//SerialPrint("wepo ");
	//SerialPrint(packedValue424);
	//SerialPrint(",");
	//SerialPrint(eePromAddress_Nextion);
	//SerialPrint(nextionEnd);

	//Serial.print("packedValue-424: ");
	//Serial.println(packedValue424);

	//SerialPrint("pageMore.n3.val=");
	//SerialPrint(packedValue424);
	//SerialPrint(nextionEnd);
	////////////////////////////////////////////////////////////////////////////

	//SerialPrint("pageMore.vEEPROM.val=");
	//SerialPrint(eePromAddress_Nextion);
	//SerialPrint(nextionEnd);

	//SerialPrint("pageMore.v00.val=");
	//SerialPrint(value0);
	//SerialPrint(nextionEnd);

	//SerialPrint("pageMore.v08.val=");
	//SerialPrint(value8);
	//SerialPrint(nextionEnd);

	//SerialPrint("pageMore.v16.val=");
	//SerialPrint(value16);
	//SerialPrint(nextionEnd);

	//SerialPrint("pageMore.v24.val=");
	//SerialPrint(value24);
	//SerialPrint(nextionEnd);

	//SerialPrint("click m0,0");
	//SerialPrint(nextionEnd);

	/////////////////////////////////////////////////

	// EEPROM 368
	//value0 = configSetup.limit_Min_M4;
	//value8 = (uint8_t)configSetup.limit_Max_M4;
	//value16 = configIndex_Prime.speedPercent_Axis_M3;
	//value24 = configIndex_Prime.speedPercent_Axis_M4;
	MilliDelay(100);
	iniValue = "Min_M4";
	eePromAddress_Nextion = 368;
	returnVal = ReturnIniValue(iniKey, iniValue);
	Serial.print("---------ReturnVal-------------------------------Min_M4:");
	Serial.println(returnVal);
	
	MilliDelay(100);
	configSetup.limit_Min_M4 = (int)returnVal;
	MilliDelay(100);
	value0 = (uint8_t)configSetup.limit_Min_M4;
	MilliDelay(100);

	Serial.print("----------------------------------------value0:");
	Serial.println(value0);

	iniValue = "Max_M4";
	eePromAddress_Nextion = 368;
	returnVal = ReturnIniValue(iniKey, iniValue);
	configSetup.limit_Max_M4 = (int)returnVal;
	value8 = (uint8_t)configSetup.limit_Max_M4;

	iniKey = "Index";
	iniValue = "SpeedPercentage_M3";
	eePromAddress_Nextion = 368;
	returnVal = ReturnIniValue(iniKey, iniValue);
	configIndex_Prime.speedPercent_Axis_M3 = (int)returnVal;
	value16 = (uint8_t)configIndex_Prime.speedPercent_Axis_M3;

	iniValue = "SpeedPercentage_M4";
	eePromAddress_Nextion = 368;
	returnVal = ReturnIniValue(iniKey, iniValue);
	configIndex_Prime.speedPercent_Axis_M4 = (int)returnVal;
	value24 = (uint8_t)configIndex_Prime.speedPercent_Axis_M4;

	// Send to Nextion
	SendPackedData(eePromAddress_Nextion);

	//// Pack data and send to Nextion
	//MilliDelay(100);
	//uint32_t packedValue368 = value24 << 24 | value16 << 16 | value8 << 8 | value0;
	//SerialPrint("wepo ");
	//SerialPrint(packedValue368);
	//SerialPrint(",");
	//SerialPrint(eePromAddress_Nextion);
	//SerialPrint(nextionEnd);

	//Serial.print("packedValue-368: ");
	//Serial.println(packedValue368);
	//SerialPrint("pageMore.n1.val=");
	//SerialPrint(packedValue368);
	//SerialPrint(nextionEnd);

	// Combined setting in LoadSettings_PagePreferences
	//// EEPROM 540
	//value0 = configSetup.limit_NCorNO; 
	//value8 = configSetup.microsteps_Axis_M3;// Set in LoadSettings_PagePreferences
	//value16 = configSetup.radialOrLinear_Axis_M3;// Set in LoadSettings_PagePreferences
	//value24 = configSetup.polarity_Axis_M3;// Set in LoadSettings_PagePreferences

	//iniValue = "NC-NO";
	//eePromAddress_Nextion = 540;
	//returnVal = SetIniValueCombined(iniKey, iniValue, eePromAddress_Nextion, 0, value0, value8, value16, value24);
	//configSetup.limit_NCorNO = (int)returnVal;

	EEPROM.put(eePromAddress_Setup, configSetup);
	EEPROM.put(eePromAddress_Ind_Prime, configIndex_Prime);
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

	iniValue = "MaxSpeed_Axis_M3";
	eePromAddress_Nextion = 776;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.maxSpd_Return_Axis_M3 = (int)returnVal;

	iniValue = "Accel_Axis_M3";
	eePromAddress_Nextion = 780;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.accel_Return_Axis_M3 = (int)returnVal;

	iniValue = "MaxSpeed_Axis_M4";
	eePromAddress_Nextion = 56;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
	configSetup.maxSpd_Return_Axis_M4 = (int)returnVal;

	iniValue = "Accel_Axis_M4";
	eePromAddress_Nextion = 84;
	returnVal = GetIniValue(iniKey, iniValue, eePromAddress_Nextion, false);
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
	//const char* nextionEnd = "\xFF\xFF\xFF";
	//const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
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
#endif // DEBUG
		SerialPrint("vis t1,1");
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

/// <summary>
/// SetIniValueCombined
/// </summary>
/// <comment>
/// Get value from ini file
/// </comment>
/// <param name="iniKey">Key name</param>
/// <param name="iniValue">Value name</param>
/// <param name="eePromNextion">Nextion EEProm address</param>
/// <param name="asString">True:Get value as string. False: Value as number</param>
/// <returns>float</returns>
float SetIniValueCombined(const char* iniKey, const char* iniValue, int eePromNextion, int valuePosition, uint8_t value0, uint8_t value8, uint8_t value16, uint8_t value24)
{
	//const char* nextionEnd = "\xFF\xFF\xFF";
	//const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
	const size_t bufferLen = 80;
	char buffer[bufferLen];
	const char* filename = filename_4Axes;
	float bufferValue = 0;
	uint8_t bufferAsInt = 0;
	long combinedValue = 0;

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
#endif // DEBUG
		SerialPrint("vis t1,1");
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
		Serial.println(buffer);
#endif // DEBUG
		bufferValue = atof(buffer);
		bufferAsInt = (uint8_t)bufferValue;
		switch (valuePosition)
		{
			case 0:
			{
				combinedValue = value24 << 8 | value16 << 8 | value8 << 8 | bufferAsInt;
				break;
			}
			case 8:
			{
				combinedValue = value24 << 8 | value16 << 8 | bufferAsInt << 8 | value0;
				break;
			}
			case 16:
			{
				combinedValue = value24 << 8 | bufferAsInt << 8 | value8 << 8 | value0;
				break;
			}
			case 24:
			{
				combinedValue = bufferAsInt << 8 | value16 << 8 | value8 << 8 | value0;
				break;
			}

		}

		// sys0=vaShift_24.val<<8|vaShift_16.val<<8|vaAxisId.val<<8|vaShift_0.val

		SerialPrint("wepo ");
		SerialPrint(combinedValue);
		SerialPrint(",");
		SerialPrint(eePromNextion);
		SerialPrint(nextionEnd);



#ifdef DEBUG
		
		Serial.println(iniKey);
		Serial.println(iniValue);
		Serial.print("wepo ");
		Serial.print(combinedValue);
		Serial.print(",");
		Serial.println(eePromNextion);

		Serial.print("bufferAsInt: ");
		Serial.println(bufferAsInt);

		Serial.print("value0: ");
		Serial.println(value0);
		Serial.print("value8: ");
		Serial.println(value8);
		Serial.print("value16: ");
		Serial.println(value16);
		Serial.print("value24: ");
		Serial.println(value24);
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
	//const char* nextionEnd = "\xFF\xFF\xFF";
	//const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
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
#endif // DEBUG
		SerialPrint("vis t1,1");
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
		Serial.println(iniKey);
		Serial.println(iniValue);
		Serial.println(buffer);
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
	//iniKey = "Main";
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
//	const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
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

			////////////////////////////////////
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

			//SerialPrint("pageMain.t0.txt=");
			//SerialWrite(0x22);
			//SerialPrint("10");
			//SerialPrint(nextionQuoteEnd);

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

		////////////////////////////////////
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

		//SerialPrint("pageMulti.t0.txt=");
		//SerialWrite(0x22);
		//SerialPrint("10");
		//SerialPrint(nextionQuoteEnd);
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

	MilliDelay(10);
	SerialPrint("pageSplash.vaEStop.val=1");
	SerialPrint(nextionEnd);
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
	else if (page == PAGE_MULTI)
	{
		// Handled in SetEStopColors
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
