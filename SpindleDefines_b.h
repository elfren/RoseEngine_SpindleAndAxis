/* *****************************************************************
* Rose Engine Spindle definitions
* Author: Edward French
******************************************************************/
//==================================================================
// Pin assignments
//==================================================================
// Spindle
#define ID_SPINDLE 3
#define PIN_SPINDLE_DIR 2  //Stepper direction
#define PIN_SPINDLE_STEP 3  //Stepper step
#define PIN_SPINDLE_ENABLE 4 //Yellow: Enable

// Z axis
#define ID_AXIS 0
#define PIN_AXIS_DIR 5  //Stepper direction
#define PIN_AXIS_STEP 6  //Stepper step
#define PIN_AXIS_ENABLE 14// Enable 
#define PIN_AXIS_LIMIT_MIN 16 // Limit switch inboard
#define PIN_AXIS_LIMIT_MAX 17 // Limit switch out board

#define PIN_LED 13  // Builtin LED

#define DIR_CCW 0
#define DIR_CW 1

#define BY_DEGREES 0
#define BY_DIVISIONS 2

struct stepperConfig
{
	int microsteps_Spindle;
	int steps360_Spindle;
	float gearRatio_Spindle;
	int maxSpeedSpindle_Main;
	int accelerationSpindle_Main;
	int speedPercentageSpindle_Main;
	int maxSpeedSpindle_Sync;
	int accelerationSpindle_Sync;
	int speedPercentageSpindle_Sync;
	int speedPercentageAxis_Sync;
	int spindleDir_Sync;
	int axisDir_Sync;
	int helixType_Sync;
	int maxSpeed_Sp2;
	int acceleration_Sp2;
	int speedPercentage_Sp2;
	int maxSpeed_Index;
	int acceleration_Index;
	int type_Index1;
	double size_Index1;
	int speedPercentage_Index1;
	int type_Index2;
	double size_Index2;
	int speedPercentage_Index2;
	int maxSpeed_Axis;
	int acceleration_Axis;
	int microsteps_Axis;
	int speedPercentage_Axis;
	int steps360_Axis;
	float gearRatio_Axis;

};

//==================================================================
// Global Variables
//==================================================================
int moveDirection = DIR_CCW; // 0:Default direction-counter clockwise    1: Clockwise
int serialId = 9; // Initialize with unused serial id.  Serial-0, Serial1-1
byte incomingByte = 0;	// store incoming Serial data
unsigned int eePromAddress = 0;  // EEProm address for spindleConfig values

// End Global Variables
//==================================================================
