/* *****************************************************************
* Rose Engine Spindle definitions
* Author: Edward French
******************************************************************/
#pragma once
#define FOUR_AXES  // Teensy 3.5
//#define THREE_AXES // Teensy 3.2
//#define TWO_AXES_V2 // Teensy 3.2
//#define SHOW_POSITION
#define DEBUG

//==================================================================
// Pin assignments
//==================================================================

#ifdef FOUR_AXES
// Spindle
#define ID_SPINDLE 3
#define PIN_SPINDLE_DIR 2  //Stepper direction
#define PIN_SPINDLE_STEP 3  //Stepper step
#define PIN_SPINDLE_ENABLE 4 //Enable
//  User runtime settable microstepping: Uncomment all microstepping #defines. 
//  Uncomment calls to SetMicrosteppingMode().
#define PIN_SPINDLE_MS0 24
#define PIN_SPINDLE_MS1 25 
#define PIN_SPINDLE_MS2 26


// Z axis
#define ID_AXIS_Z 0
#define PIN_AXIS_Z_DIR 5 // Stepper direction
#define PIN_AXIS_Z_STEP 6 // Stepper step
#define PIN_AXIS_Z_ENABLE 14 // Enable 
#define PIN_AXIS_Z_MS0 27
#define PIN_AXIS_Z_MS1 28 
#define PIN_AXIS_Z_MS2 29

// X axis
#define ID_AXIS_X 1
#define PIN_AXIS_X_DIR 20  // Stepper direction
#define PIN_AXIS_X_STEP 21 // Stepper step
#define PIN_AXIS_X_ENABLE 22 // Enable 
#define PIN_AXIS_X_MS0 30
#define PIN_AXIS_X_MS1 31 
#define PIN_AXIS_X_MS2 32


// B axis
#define ID_AXIS_B 2
#define PIN_AXIS_B_DIR 23  // Stepper direction
#define PIN_AXIS_B_STEP 16  // Stepper step
#define PIN_AXIS_B_ENABLE 17 // Enable 
#define PIN_AXIS_B_MS0 33
#define PIN_AXIS_B_MS1 34
#define PIN_AXIS_B_MS2 35

#elif defined(TWO_AXES_V2)
// Spindle
#define ID_SPINDLE 3
#define PIN_SPINDLE_DIR 2 // Stepper direction
#define PIN_SPINDLE_STEP 3  // Stepper step
#define PIN_SPINDLE_ENABLE 4 // Enable
// User runtime settable microstepping requires inner pins on Teensy 3.2
#define PIN_SPINDLE_MS0 18
#define PIN_SPINDLE_MS1 19 
#define PIN_SPINDLE_MS2 20

// Z axis
#define ID_AXIS_Z 0
#define PIN_AXIS_Z_DIR 5  // Stepper direction
#define PIN_AXIS_Z_STEP 6  // Stepper step
#define PIN_AXIS_Z_ENABLE 14 // Enable 
// User runtime settable microstepping requires inner pins on Teensy 3.2
#define PIN_AXIS_Z_MS0 21
#define PIN_AXIS_Z_MS1 22
#define PIN_AXIS_Z_MS2 23

// X axis
#define ID_AXIS_X 1
#define PIN_AXIS_X_DIR 24  // Stepper direction
#define PIN_AXIS_X_STEP 25  // Stepper step
#define PIN_AXIS_X_ENABLE 26 // Enable 
//#define PIN_AXIS_X_MS0 30
//#define PIN_AXIS_X_MS1 31 
//#define PIN_AXIS_X_MS2 32

// B axis
#define ID_AXIS_B 2
#define PIN_AXIS_B_DIR 24  // Stepper direction
#define PIN_AXIS_B_STEP 25  // Stepper step
#define PIN_AXIS_B_ENABLE 26 // Enable 
//#define PIN_AXIS_B_MS0 27
//#define PIN_AXIS_B_MS1 28 
//#define PIN_AXIS_B_MS2 29

#elif defined(THREE_AXES) // Three axes board
// Spindle
#define ID_SPINDLE 3
#define PIN_SPINDLE_DIR 3 // Stepper direction
#define PIN_SPINDLE_STEP 2  // Stepper step
#define PIN_SPINDLE_ENABLE 6 // Enable
// User runtime settable microstepping requires inner pins on Teensy 3.2
#define PIN_SPINDLE_MS0 24
#define PIN_SPINDLE_MS1 25 
#define PIN_SPINDLE_MS2 26

// Z axis
#define ID_AXIS_Z 0
#define PIN_AXIS_Z_DIR 4  // Stepper direction
#define PIN_AXIS_Z_STEP 5  // Stepper step
#define PIN_AXIS_Z_ENABLE 20 //PCB jumper:33>>20 // Enable  

// User runtime settable microstepping requires inner pins on Teensy 3.2
#define PIN_AXIS_Z_MS0 27
#define PIN_AXIS_Z_MS1 28 
#define PIN_AXIS_Z_MS2 29

// X axis
#define ID_AXIS_X 1
#define PIN_AXIS_X_DIR 23  //PCB jumper:24>>23 // Stepper direction
#define PIN_AXIS_X_STEP 22  //PCB jumper:30>>22// Stepper step
#define PIN_AXIS_X_ENABLE 21 //PCB jumper:29>>21 // Enable 
// User runtime settable microstepping requires inner pins on Teensy 3.2
#define PIN_AXIS_X_MS0 30
#define PIN_AXIS_X_MS1 31 
#define PIN_AXIS_X_MS2 32

// B axis
#define ID_AXIS_B 2
#define PIN_AXIS_B_DIR 24  // Stepper direction
#define PIN_AXIS_B_STEP 25  // Stepper step
#define PIN_AXIS_B_ENABLE 26 // Enable 
//#define PIN_AXIS_B_MS0 27
//#define PIN_AXIS_B_MS1 28 
//#define PIN_AXIS_B_MS2 29

#endif //FOUR_AXES/Two Axes/3 Axes

// SPI
#define PIN_SPI_CS 10

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

struct configs
{
	int speedPercentSpindle_SpZ;
	int speedPercentSpindle_SpX;
	int speedPercentSpindle_SpB;
	int speedPercentSpindle_SyncZ;
	int speedPercentSpindle_SyncX;
	int speedPercentAxis_SyncZ;
	int speedPercentAxis_SyncX;
	int speedPercent_Index1;
	int speedPercent_Index2;
	int speedPercent_Sp2; // Not used
	int speedPercent_Axis_Z;
	int speedPercent_Axis_X;
	int speedPercent_Axis_B;
	int speedPercent_MoveZ;
	int speedPercent_MoveX;
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
	double size_Index1;
	double size_Index2;
	float distance_MoveZ;
	float distance_MoveX;
	float gearRatio_Spindle;
	float gearRatio_AxisB;
	float distancePerRev_AxisZ;
	float distancePerRev_AxisX;
	float distanceSyncZ;
	float distanceSyncX;
	float revolutionsSyncZ_Spindle;
	float revolutionsSyncX_Spindle;
	bool polarity_Spindle;
	bool polarity_Axis_Z;
	bool polarity_Axis_X;
	bool polarity_Axis_B;
	int limit_Min_Z;
	int limit_Max_Z;
	int limit_Min_X;
	int limit_Max_X;
	int limit_Min_B;
	int limit_Max_B;
	int maxSpd_Return_Spindle;
	int accel_Return_Spindle;
	int maxSpd_Return_Axis;
	int accel_Return_Axis;
	int trackPositions;
};

// Config Structs
struct configSteppers // 
{
	int maxSpd_Spindle;
	int accel_Spindle;
	int maxSpd_Axis;
	int accel_Axis;
};

struct configStepper //
{
	int maxSpd;
	int accel;
};

struct configPageReci // page 5,6,13,14
{
	int maxSpd_Spindle;
	int accel_Spindle;
	int maxSpd_Axis_Z;
	int maxSpd_Axis_X;
	int speedPercent_Spindle;
	int speedPercent_Axis_Z;
	int speedPercent_Axis_X;
	int accel_Axis_Z;
	int accel_Axis_X;
	int waves_AxialZ;
	int waves_AxialX;
	int waves_RadialZ;
	int waves_RadialX;

	float distance_AxialZ;
	float distance_AxialX;
	float amplitude_AxialZ;
	float amplitude_AxialX;
	float amplitude_RadialZ;
	float amplitude_RadialX;
	float degrees_RadialZ;
	float degrees_RadialX;
};


struct configPageRose // page 12
{
	int maxSpd_Spindle;
	int accel_Spindle;
	int maxSpd_Axis_Z;
	int accel_Axis_Z;
	int maxSpd_Axis_X;
	int accel_Axis_X;
	int speedPercent_Spindle;
	int n;
	int d;
	int speedPercent_Axis;
	float amplitude_Axis_Z;
	float amplitude_Axis_X;
	float amplitude_Spindle;
};

struct configPageSp  // page 7
{
	int activeAxis;
	int maxSpd_Spindle;
	int maxSpd_Axis_Z;
	int maxSpd_Axis_X;
	int maxSpd_Axis_B;
	int speedPercent_Spindle;
	int speedPercent_Axis_Z;
	int speedPercent_Axis_X;
	int speedPercent_Axis_B;
	int accel_Spindle;
	int accel_Axis_Z;
	int accel_Axis_X;
	int accel_Axis_B;
};
//==================================================================
// Global Variables
//==================================================================

int serialId = 9; // Initialize with unused serial id.  Serial-0, Serial3-1
byte incomingByte = 0;	// store incoming Serial data
unsigned int eePromAddress_Setup = 0;  // EEProm address for spindleConfig values
unsigned int eePromAddress_Filename = 900; // EEProm address for Index2 filename
unsigned int eePromAddress_Filename_Length = 996; // EEProm address for length of Index2 filename

unsigned int eePromAddress_SpZ = 1000;  // EEProm address for SpZ
unsigned int eePromAddress_SpX = 1016;  // EEProm address for SpX  
unsigned int eePromAddress_SpB = 1032;  // EEProm address for SpB 
unsigned int eePromAddress_SyncZ = 1048;  // EEProm address for SyncZ 
unsigned int eePromAddress_SyncX = 1064;  // EEProm address for SyncX 

unsigned int eePromAddress_Index1 = 1128;  // EEProm address for Index1 
unsigned int eePromAddress_Index2 = 1144;  // EEProm address for Index2 
unsigned int eePromAddress_MoveZ = 1160;  // EEProm address for MoveZ 
unsigned int eePromAddress_MoveX = 1176;  // EEProm address for MoveX 
unsigned int eePromAddress_Rose = 1200; // EEProm address for Rose

unsigned int eePromAddress_Reci = 1300;
unsigned int eePromAddress_Sp = 1500;  // EEProm address for Sp 

float distanceTotal_MoveZ = 0;
float distanceTotal_MoveX = 0;

String filename_Index1;
int filenameLength = 0;
String filename_Index2a;
char * filename_Index2;

double returnSteps_Axis = 0;
double returnSteps_Spindle = 0;


/////////////////////////////////////////////////////////////////////////
// Rose settings
/////////////////////////////////////////////////////////////////////////

volatile int32_t newMaxSpd_RoseSpindle;
volatile int32_t newMaxSpd_RoseAxis;
volatile int32_t spindleStepsPerRev;
volatile float slideStepsAmplitude;
volatile float spindleStepsAmplitude;
volatile float kRatio;

float initialPosition_Axis = 0;
int32_t endPosition_Spindle = 0;
int32_t endPosition_Axis = 0;
float degrees_Spindle = 0;
float distance_Axis = 0;

// Accuracy interval 
constexpr unsigned recalcPeriod = 35'000;
//constexpr unsigned recalcPeriod = 25'000; //?s  period for calculation of new target points. Straight lines between those points. 
constexpr float dtRose = recalcPeriod / 1E6;  //seconds  1E6 = 1,000,000 

constexpr unsigned PID_Interval = 10; // ms  
constexpr float P = 0.01;             // (P)roportional constant of the regulator needs to be adjusted (depends on speed and acceleration setting)

// Page defines
int pageCallerId = 20;
#define PAGE_SPZ 0
#define PAGE_SPX 1
#define PAGE_SPB 2
#define PAGE_SYNCZ 3
#define PAGE_SYNCX 4
#define PAGE_RECI_AXIAL_Z 5
#define PAGE_RECI_RADIAL_Z 6
#define PAGE_SP 7
#define PAGE_INDEX1 8
#define PAGE_INDEX2 9
#define PAGE_MOVEZ 10
#define PAGE_MOVEX 11
#define PAGE_ROSE 12
#define PAGE_RECI_AXIAL_X 13
#define PAGE_RECI_RADIAL_X 14

// Config and Setup variables
configs configMain;
configSteppers configSpZ;
configSteppers configSpX;
configSteppers configSpB;
configSteppers configSyncZ;
configSteppers configSyncX;
configPageReci configReci;
configPageSp configSp;
configStepper configIndex1;
configStepper configIndex2;
configStepper configMoveZ;
configStepper configMoveX;
configPageRose configRose;

// End Global Variables
//==================================================================

