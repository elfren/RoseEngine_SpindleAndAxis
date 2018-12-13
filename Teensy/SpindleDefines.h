/* *****************************************************************
* Rose Engine Spindle definitions
* Author: Edward French
******************************************************************/
//==================================================================
// Pin assignments
//==================================================================
//#define TEENSY_35
#define TWO_AXES_V2
//#define MICRO_SD
//#define DEBUG

#ifdef TEENSY_35
// Spindle
#define ID_SPINDLE 3
#define PIN_SPINDLE_DIR 2//3//2  //Stepper direction
#define PIN_SPINDLE_STEP 3//2//3  //Stepper step
#define PIN_SPINDLE_ENABLE 4//6//4 //Enable
#define PIN_SPINDLE_MS0 24
#define PIN_SPINDLE_MS1 25 
#define PIN_SPINDLE_MS2 26

// Axes
#define PIN_LIMIT_MIN 18//16//18 // Limit switch: Moving towards headstock
#define PIN_LIMIT_MAX 19//17//19  // Limit switch: Moving away from headstock

// Z axis
#define ID_Z_AXIS 0
#define PIN_AXIS_Z_DIR 5//4//5  //Stepper direction
#define PIN_AXIS_Z_STEP 6//5//6  //Stepper step
#define PIN_AXIS_Z_ENABLE 14//20//14 // Enable 
#define PIN_AXIS_Z_MS0 27
#define PIN_AXIS_Z_MS1 28 
#define PIN_AXIS_Z_MS2 29

// X axis
#define ID_AXIS_X 1
#define PIN_AXIS_X_DIR 20  //Stepper direction
#define PIN_AXIS_X_STEP 21  //Stepper step
#define PIN_AXIS_X_ENABLE 22 // Enable 
#define PIN_AXIS_X_MS0 30
#define PIN_AXIS_X_MS1 31 
#define PIN_AXIS_X_MS2 32

// B axis
#define ID_AXIS_B 2
#define PIN_AXIS_B_DIR 23  //Stepper direction
#define PIN_AXIS_B_STEP 16  //Stepper step
#define PIN_AXIS_B_ENABLE 17 // Enable 
#define PIN_AXIS_B_MS0 27
#define PIN_AXIS_B_MS1 28 
#define PIN_AXIS_B_MS2 29
#elif defined(TWO_AXES_V2)
// Spindle
#define ID_SPINDLE 3
#define PIN_SPINDLE_DIR 2 //Stepper direction
#define PIN_SPINDLE_STEP 3  //Stepper step
#define PIN_SPINDLE_ENABLE 4 //Enable

// Axes
#define PIN_LIMIT_MIN 16// Limit switch: Moving towards headstock
#define PIN_LIMIT_MAX 17 // Limit switch: Moving away from headstock

// Z axis
#define ID_Z_AXIS 0
#define PIN_AXIS_Z_DIR 5  //Stepper direction
#define PIN_AXIS_Z_STEP 6  //Stepper step
#define PIN_AXIS_Z_ENABLE 14 // Enable 

#else // Three axes board
// Spindle
#define ID_SPINDLE 3
#define PIN_SPINDLE_DIR 3 //Stepper direction
#define PIN_SPINDLE_STEP 2  //Stepper step
#define PIN_SPINDLE_ENABLE 6 //Enable

// Axes
#define PIN_LIMIT_MIN 16// Limit switch: Moving towards headstock
#define PIN_LIMIT_MAX 17 // Limit switch: Moving away from headstock

// Z axis
#define ID_Z_AXIS 0
#define PIN_AXIS_Z_DIR 4  //Stepper direction
#define PIN_AXIS_Z_STEP 5  //Stepper step
#define PIN_AXIS_Z_ENABLE 20 // Enable 



#endif //TEENSY_35/Two Axes/3 Axes

// SPI
#define PIN_SPI_CS 10
#define PIN_LED 13  // Builtin LED

// Index page ID
#define ID_INDEX_1 1
#define ID_INDEX_2 2

#define DIR_CCW -1
#define DIR_CW 1

#define BY_DEGREES 0
#define BY_DIVISIONS 2

#define FIXED 0
#define FILE_SD 1

#define INCHES 0
#define MILLIMETERS 1

#define WAVE_IN -1
#define WAVE_OUT 1

struct stepperConfig
{
	int maxSpeedSpindle_SpZ;
	int maxSpeedSpindle_SpX;
	int maxSpeedSpindle_SpB;
	int maxSpeedSpindle_SyncZ;
	int maxSpeedSpindle_SyncX;
	int maxSpeed_Sp2;
	int maxSpeed_Index;
	int maxSpeed_Axis_Z;
	int maxSpeed_Axis_X;
	int maxSpeed_Axis_B;
	int maxSpeed_ZiZa_Z_Spindle;
	int maxSpeed_ZiZa_Z_Axis;
	int maxSpeed_ZigZag_SpZ_Spindle;
	int maxSpeed_ZigZag_SpZ_Axis;
	int accelerationSpindle_SpZ;
	int accelerationSpindle_SpX;
	int accelerationSpindle_SpB;
	int acceleration_Sp2;
	int acceleration_Index;
	int accelerationSpindle_SyncZ;
	int accelerationSpindle_SyncX;
	int acceleration_Axis_Z;
	int acceleration_Axis_X;
	int acceleration_Axis_B;
	int acceleration_ZiZa_Z_Spindle;
	int acceleration_ZiZa_Z_Axis;
	int acceleration_ZigZag_SpZ_Spindle;
	int acceleration_ZigZag_SpZ_Axis;
	int speedPercentageSpindle_SpZ;
	int speedPercentageSpindle_SpX;
	int speedPercentageSpindle_SpB;
	int speedPercentageSpindle_SyncZ;
	int speedPercentageSpindle_SyncX;
	int speedPercentageAxis_SyncZ;
	int speedPercentageAxis_SyncX;
	int speedPercentage_Index1;
	int speedPercentage_Index2;
	int speedPercentage_Sp2;
	int speedPercentage_Axis_Z;
	int speedPercentage_Axis_X;
	int speedPercentage_Axis_B;
	int speedPercentage_MoveZ;
	int speedPercentage_MoveX;
	int microsteps_Spindle;
	int microsteps_Axis_Z;
	int microsteps_Axis_X;
	int microsteps_Axis_B;
	int steps360_Spindle;
	int steps360_Axis_Z;
	int steps360_Axis_X;
	int steps360_Axis_B;
	int spindleDir_SyncZ;
	int spindleDir_SyncX;
	int axisDir_SyncZ;
	int axisDir_SyncX;
	int helixType_SyncZ;
	int helixType_SyncX;
	int type_Index1;
	int type_Index2;
	int source_Index1;
	int source_Index2;
	int repeats_ZiZa_Z;
	int repeats_ZigZag_SpZ;
	double size_Index1;
	double size_Index2;
	float distance_MoveZ;
	float distance_MoveX;
	float distance_ZiZa_Z;
	float amplitude_ZiZa_Z;
	float amplitude_ZigZag_SpZ;
	float degrees_ZigZag_SpZ;
	float gearRatio_Spindle;
	float gearRatio_AxisB;
	float distancePerRev_AxisZ;
	float distancePerRev_AxisX;
	float distanceSyncZ;
	float distanceSyncX;
	float revolutionsSyncZ_Spindle;
	float revolutionsSyncX_Spindle;
};

//==================================================================
// Global Variables
//==================================================================
int serialId = 9; // Initialize with unused serial id.  Serial-0, Serial1-1
byte incomingByte = 0;	// store incoming Serial data
unsigned int eePromAddress = 0;  // EEProm address for spindleConfig values
float distanceTotal_MoveZ = 0;
float distanceTotal_MoveX = 0;

String filename_Index1;
int filenameLength = 0;
String filename_Index2;

// End Global Variables
//==================================================================

// TeensyStep variables
//int t1, t2, t3;
//constexpr int targetPositions[]{ 2500, 0, 2500, 0 };
//int targetPositions[4];
//constexpr int nrOfPositions = sizeof(targetPositions) / sizeof(targetPositions[0]);
int targetPositions[30];
int nrOfPositions = 0;

elapsedMillis stopwatch = 0;
int cycleWidth;
int direction;
int cycleCounter;
//int numberOfCycles;
//int maxSpeed;

double returnSteps_ZiZa_Z = 0;
double returnSteps_ZigZag_SpZ = 0;
enum class state
{
	waiting,
	starting,
	startingCycle,
	runningCycle,
	stoppingCycle,
	stopping,
} currentstate;
