/* *****************************************************************
* Rose Engine Spindle definitions
* Author: Edward French
******************************************************************/
#pragma once
#define FOUR_AXES  // Teensy 3.5
//#define THREE_AXES // Teensy 3.2
//#define TWO_AXES_V2 // Teensy 3.2
#define SHOW_POSITION
//#define DEBUG

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
#define PIN_SPINDLE_MS0 46
#define PIN_SPINDLE_MS1 47 
#define PIN_SPINDLE_MS2 48

// Z axis
#define ID_AXIS_Z 0
#define PIN_AXIS_Z_DIR 5 // Stepper direction
#define PIN_AXIS_Z_STEP 6 // Stepper step
#define PIN_AXIS_Z_ENABLE 14 // Enable 
#define PIN_AXIS_Z_MS0 49
#define PIN_AXIS_Z_MS1 50 
#define PIN_AXIS_Z_MS2 51

// X axis
#define ID_AXIS_X 1
#define PIN_AXIS_X_DIR 20  // Stepper direction
#define PIN_AXIS_X_STEP 21 // Stepper step
#define PIN_AXIS_X_ENABLE 22 // Enable 
#define PIN_AXIS_X_MS0 52
#define PIN_AXIS_X_MS1 53 
#define PIN_AXIS_X_MS2 54

// B axis
#define ID_AXIS_B 2
#define PIN_AXIS_B_DIR 23  // Stepper direction
#define PIN_AXIS_B_STEP 16  // Stepper step
#define PIN_AXIS_B_ENABLE 17 // Enable 
#define PIN_AXIS_B_MS0 55
#define PIN_AXIS_B_MS1 56
#define PIN_AXIS_B_MS2 57

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

/////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////
// SPI
#define PIN_SPI_CS 10

// Index page ID
#define ID_INDEX_1 1
#define ID_INDEX_2 2
#define ID_INDEX_3 3

#define DIR_CCW -1
#define DIR_CW 1

#define BY_DEGREES 2
#define BY_DIVISIONS 0

#define FIXED 0
#define FILE_SD 1

#define INCHES 0
#define MILLIMETERS 1

#define WAVE_IN -1
#define WAVE_OUT 1

#define RADIAL 0
#define AXIAL 1

/////////////////////////////////////////////////////////////////////////
// Page defines
/////////////////////////////////////////////////////////////////////////

#define PAGE_MAIN 0
#define PAGE_SPLASH 1
#define PAGE_ONE 2
#define PAGE_INDEX 3
#define PAGE_MOVE 4
#define PAGE_BE 5
#define PAGE_SYNCZ 6
#define PAGE_REC 7
#define PAGE_GRK 8
#define PAGE_GRKFILE 9
#define PAGE_GEO 10 

/////////////////////////////////////////////////////////////////////////
// Structs
/////////////////////////////////////////////////////////////////////////
struct configPageMov
{
	uint32_t axisId;
	int32_t maxSpd_Axis_Z;
	uint32_t accel_Axis_Z;
	int32_t maxSpd_Axis_X;
	uint32_t accel_Axis_X;
	int32_t speedPercent_Axis_Z;
	int32_t speedPercent_Axis_X;
	float distance_MoveZ;
	float distance_MoveX;

};

struct configPageSync
{
	uint32_t axisId;
	int32_t maxSpd_Spindle;
	uint32_t accel_Spindle;
	int32_t maxSpd_Axis_Z;
	uint32_t accel_Axis_Z;
	int32_t maxSpd_Axis_X;
	uint32_t accel_Axis_X;
	int32_t speedPercent_Spindle;
	int32_t speedPercent_Axis_Z;
	int32_t speedPercent_Axis_X;
	uint32_t helixType;
	float distance;
	float revolutions_Spindle;
};

// Config page Setup
struct configPageSetup
{
	uint32_t microsteps_Spindle;
	uint32_t steps360_Spindle;
	float gearRatio_Spindle;
	uint32_t microsteps_Axis_Z;
	uint32_t steps360_Axis_Z;
	float distancePerRev_AxisZ;
	uint32_t limit_Min_Z;
	uint32_t limit_Max_Z;
	uint32_t microsteps_Axis_X;
	uint32_t steps360_Axis_X;
	float distancePerRev_AxisX;
	uint32_t limit_Min_X;
	uint32_t limit_Max_X;
	uint32_t microsteps_Axis_B;
	uint32_t steps360_Axis_B;
	float gearRatio_AxisB;
	uint32_t limit_Min_B;
	uint32_t limit_Max_B;
	int32_t maxSpd_Return_Spindle;
	uint32_t accel_Return_Spindle;
	int32_t maxSpd_Return_Axis;
	uint32_t accel_Return_Axis;
	bool polarity_Spindle;
	bool polarity_Axis_Z;
	bool polarity_Axis_X;
	bool polarity_Axis_B;
};

// Config Structs
struct configSteppers // 
{
	int32_t maxSpd_Spindle;
	uint32_t accel_Spindle;
	int32_t maxSpd_Axis;
	uint32_t accel_Axis;
	int32_t speedPercent_Spindle;
	int32_t speedPercent_Axis;
};

struct configPageRec // page 5,6,13,14
{
	int32_t maxSpd_Spindle;
	uint32_t accel_Spindle;
	int32_t maxSpd_Axis_Z;
	int32_t maxSpd_Axis_X;
	int32_t speedPercent_Spindle;
	int32_t speedPercent_Axis_Z;
	int32_t speedPercent_Axis_X;
	uint32_t accel_Axis_Z;
	uint32_t accel_Axis_X;
	int32_t waves_AxialZ;
	int32_t waves_AxialX;
	int32_t waves_RadialZ;
	int32_t waves_RadialX;

	float distance_AxialZ;
	float distance_AxialX;
	float amplitude_AxialZ;
	float amplitude_AxialX;
	float amplitude_RadialZ;
	float amplitude_RadialX;
	float degrees_RadialZ;
	float degrees_RadialX;
	uint32_t axisId;
	uint32_t radial_axial;
};

struct configPageRose // page 12
{
	int32_t maxSpd_Spindle;
	uint32_t accel_Spindle;
	int32_t maxSpd_Axis_Z;
	uint32_t accel_Axis_Z;
	uint32_t maxSpd_Axis_X;
	uint32_t accel_Axis_X;
	int32_t speedPercent_Spindle;
	int32_t n;
	int32_t d;
	uint32_t axisId;
	uint32_t radial_Axial;
	int32_t speedPercent_Axis_Z;
	int32_t speedPercent_Axis_X;
	float amplitude_Axial_Z;
	float amplitude_Axial_X;
	float amplitude_Radial_Z;
	float amplitude_Radial_X;
};

struct configPageMainOne  // page 7 and page 0 (pageMain)
{
	int32_t maxSpd_Spindle;
	int32_t maxSpd_Axis_Z;
	int32_t maxSpd_Axis_X;
	int32_t maxSpd_Axis_B;
	int32_t speedPercent_Spindle;
	int32_t speedPercent_Axis_Z;
	int32_t speedPercent_Axis_X;
	int32_t speedPercent_Axis_B;
	uint32_t accel_Spindle;
	uint32_t accel_Axis_Z;
	uint32_t accel_Axis_X;
	uint32_t accel_Axis_B;
	uint32_t axisId; // Z:0, X:1, B:2, Spindle:3
};

struct configPageGreekKey
{
	uint32_t axisId;
	uint32_t fileOrPattern;
	// pageGrk settings
	uint32_t patternId;
	float countPattern360;
	int32_t countPattern;
	int32_t radialOrAxial_Pattern;
	float segmentLengthPattern;

	// pageGrkFile settings
	int32_t countPatternFile;
	float countPatternFile360;
	int32_t segmentOrActual;
	int32_t radialOrAxial_File;
	float segmentLengthForFile;
};

struct configPageIndex
{
	int32_t indexId;
	int32_t maxSpd;
	int32_t speedPercent;
	uint32_t accel;
	 
};

struct configIndex
{
	uint32_t degreeOrDivision; // Type
	uint32_t fileOrFixed; // Source
	int32_t size;
};


/////////////////////////////////////////////////////////////////////////
// Config and Setup variables
/////////////////////////////////////////////////////////////////////////
configPageSetup configSetup;
configPageMainOne configPageMain;
configPageSync configSync;
configPageRec configRec;
configPageMainOne configOne;
configPageIndex configIndex_Main;
configIndex configIndex_1;
configIndex configIndex_2;
configIndex configIndex_3;
configPageMov configMove;
configPageRose configRose;
configPageGreekKey configGreekKey_Main;
configSteppers configGreekKey_Z;
configSteppers configGreekKey_X;

unsigned int eePromAddress_Setup = 100; // EEProm address for Setup
unsigned int eePromAddress_Sync = 225;  // EEProm address for Sync
unsigned int eePromAddress_Main = 300;  // EEProm address for Main
unsigned int eePromAddress_Mov = 375; // EEProm address for Move
unsigned int eePromAddress_Rose = 420; // EEProm address for Rose
unsigned int eePromAddress_Rec = 500;
unsigned int eePromAddress_One = 600;  // EEProm address for Sp 
unsigned int eePromAddress_Grk_Main = 675;  // EEProm address for Greek Key Main
unsigned int eePromAddress_Grk_Z = 735; //  EEProm address for Greek Key Z
unsigned int eePromAddress_Grk_X = 780; //  EEProm address for Greek Key X 

unsigned int eePromAddress_Ind_Main = 810;  // EEProm address for Index_Main
unsigned int eePromAddress_Ind_1 = 840;  // EEProm address for Index_1
unsigned int eePromAddress_Ind_2 = 860;  // EEProm address for Index_2
unsigned int eePromAddress_Ind_3 = 880;  // EEProm address for Index_3 

unsigned int eePromAddress_Filename_Ind = 900; // EEProm address for Index2 filename
unsigned int eePromAddress_Filename_Length_Ind = 920; // EEProm address for length of Index2 filename
unsigned int eePromAddress_Filename_Grk = 925; // EEProm address for Index2 filename
unsigned int eePromAddress_Filename_Length_Grk = 945; // EEProm address for length of Index2 filename

//==================================================================
// Global Variables
//==================================================================
int pageCallerId = 20;
int serialId = 9; // Initialize with unused serial id.  Serial-0, Serial3-1
byte incomingByte = 0;	// store incoming Serial data



float distanceTotal_MoveZ = 0;
float distanceTotal_MoveX = 0;

String filename_Index1;
int filenameLength = 0;
String filename_Index2a;
char * filename_Index2;

int32_t returnSteps_Axis = 0;
int32_t returnSteps_Spindle = 0;

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
constexpr unsigned recalcPeriod = 25'000; //?s  period for calculation of new target points. Straight lines between those points. 
constexpr float dtRose = recalcPeriod / 1E6;  //seconds  1E6 = 1,000,000 

constexpr unsigned PID_Interval = 10; // ms  
constexpr float P = 0.01;             // (P)roportional constant of the regulator needs to be adjusted (depends on speed and acceleration setting)
constexpr unsigned priorityLevel = 64; //128

/////////////////////////////////////////////////////////////////////////
// Greek Key
/////////////////////////////////////////////////////////////////////////

int reverseDirection = 1;
int moveType = 0;
String axisAndDirection;
String axisAndDirectionA;
bool fileDataAvailable = false;
int currentLineNumber = 0;
bool exitGreekKey = false;
String comment;
int spindleShortLegSteps = 0;
int axisShortLegSteps = 0;
String sdFilename;
bool badFilename = false;
float hVal = 0; // Horizontal component of angular move
float vVal = 0; // Vertical component of angular move

/////////////////////////////////////////////////////////////////////////
// FreeMemory
/////////////////////////////////////////////////////////////////////////
//#ifdef __arm__
//// should use uinstd.h to define sbrk but Due causes a conflict
//extern "C" char* sbrk(int incr);
//#else  // __ARM__
//extern char *__brkval;
//#endif  // __arm__
//




// End Global Variables
//==================================================================