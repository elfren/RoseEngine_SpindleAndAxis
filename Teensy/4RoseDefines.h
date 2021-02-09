/* *****************************************************************
* Rose Engine Spindle definitions
* Author: Edward French
******************************************************************/
#pragma once
#define SHOW_POSITION
#define DEBUG

//==================================================================
// Pin assignments
//==================================================================

// Spindle
#define ID_SPINDLE 3
#define PIN_SPINDLE_DIR 2  //Stepper direction
#define PIN_SPINDLE_STEP 3  //Stepper step
#define PIN_SPINDLE_ENABLE 4 //Enable

// Z axis
#define ID_AXIS_Z 0
#define PIN_AXIS_Z_DIR 5 // Stepper direction
#define PIN_AXIS_Z_STEP 6 // Stepper step
#define PIN_AXIS_Z_ENABLE 14 // Enable 

// X axis
#define ID_AXIS_X 1
#define PIN_AXIS_X_DIR 20  // Stepper direction
#define PIN_AXIS_X_STEP 21 // Stepper step
#define PIN_AXIS_X_ENABLE 22 // Enable 

// B axis
#define ID_AXIS_B 2
#define PIN_AXIS_B_DIR 23  // Stepper direction
#define PIN_AXIS_B_STEP 16  // Stepper step
#define PIN_AXIS_B_ENABLE 17 // Enable 

/////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////

// Move page
#define ID_MOVE_AXIS_Z1 0
#define ID_MOVE_AXIS_Z2 1
#define ID_MOVE_AXIS_X1 2
#define ID_MOVE_AXIS_X2 3
#define ID_MOVE_AXIS_B1 4
#define ID_MOVE_AXIS_B2 5

// SPI
#define PIN_SPI_CS_10 10  // Primary pin for SPI
#define PIN_SPI_CS_9 9
#define PIN_SPI_CS_15 15
#define PIN_SPI_CS_24 24

// Index page ID
#define ID_INDEX_1 1
#define ID_INDEX_2 2
#define ID_INDEX_3 3
#define ID_INDEX_4 4
#define ID_INDEX_5 5

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
#define RADIAL_B 0
#define LINEAR_B 1

#define INI_4AXES 0
#define INI_RESET 1
/////////////////////////////////////////////////////////////////////////
// Page defines
/////////////////////////////////////////////////////////////////////////

#define PAGE_MAIN 0
#define PAGE_SPLASH 1
#define PAGE_ONE 2
#define PAGE_INDEX 3
#define PAGE_MOVE 4
#define PAGE_BE 5
#define PAGE_SYNC 6
#define PAGE_REC 7
#define PAGE_GRK 8
#define PAGE_GRKFILE 9
#define PAGE_GEO 10 
#define PAGE_MORE 11

/////////////////////////////////////////////////////////////////////////
// Structs
/////////////////////////////////////////////////////////////////////////

struct configPageMov
{
	uint32_t axisId;
	int32_t maxSpd_Axis_Z;
	uint32_t accel_Axis_Z;
	int32_t speedPercent_Axis_Z;
	int32_t maxSpd_Axis_X;
	uint32_t accel_Axis_X;
	int32_t speedPercent_Axis_X;
	int32_t maxSpd_Axis_B;
	uint32_t accel_Axis_B;
	int32_t speedPercent_Axis_B;
	float distance_MoveZ1;
	float distance_MoveX1;
	float distance_MoveB1;
	float distance_MoveZ2;
	float distance_MoveX2;
	float distance_MoveB2;
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
	int32_t maxSpd_Axis_B;
	uint32_t accel_Axis_B;
	int32_t speedPercent_Spindle;
	int32_t speedPercent_Axis_Z;
	int32_t speedPercent_Axis_X;
	int32_t speedPercent_Axis_B;
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
	uint32_t radialOrLinear_Axis_B;
	uint32_t microsteps_Axis_B;
	uint32_t steps360_Axis_B;
	float gearRatio_AxisB;
	float distancePerRev_AxisB;
	float radiusB;
	uint32_t limit_Min_B;
	uint32_t limit_Max_B;
	int32_t maxSpd_Return_Spindle;
	uint32_t accel_Return_Spindle;
	int32_t maxSpd_Return_Axis_Z;
	uint32_t accel_Return_Axis_Z;
	int32_t maxSpd_Return_Axis_X;
	uint32_t accel_Return_Axis_X;
	int32_t maxSpd_Return_Axis_B;
	uint32_t accel_Return_Axis_B;
	bool polarity_Spindle;
	bool polarity_Axis_Z;
	bool polarity_Axis_X;
	bool polarity_Axis_B;
	uint32_t home_Z;
	uint32_t home_X;
	uint32_t home_B;
	uint32_t pulseWidth_Spindle;
	uint32_t speedUpdatePeriod_Spindle;
	uint32_t limit_StopSpindle;

	uint32_t xAltX;
	uint32_t microsteps_Axis_XAlt;
	uint32_t steps360_Axis_XAlt;
	float distancePerRev_AxisXAlt;
	bool polarity_Axis_XAlt;
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
	//int32_t waves_AxialZ;
	//int32_t waves_AxialX;
	int32_t waves_Axial;
	//int32_t waves_RadialZ;
	//int32_t waves_RadialX;
	int32_t waves_Radial;
	//float distance_AxialZ;
	//float distance_AxialX;
	float distance_Axial_Axis;
	//float amplitude_AxialZ;
	//float amplitude_AxialX;
	float amplitude_Axial_Spindle;
	//float amplitude_RadialZ;
	//float amplitude_RadialX;
	float amplitude_Radial_Axis;
	//float degrees_RadialZ;
	//float degrees_RadialX;
	float degrees_Radial_Spindle;
	uint32_t axisId;
	uint32_t radial_axial;
};

struct configPageRose // page 12
{
	int32_t maxSpd_Spindle;
	int32_t maxSpd_Axis_Z;
	uint32_t maxSpd_Axis_X;
	uint32_t maxSpd_Axis_B;

	uint32_t accel_Spindle;
	uint32_t accel_Axis_Z;
	uint32_t accel_Axis_X;
	uint32_t accel_Axis_B;

	int32_t speedPercent_Spindle;
	int32_t speedPercent_Axis_Z;
	int32_t speedPercent_Axis_X;
	int32_t speedPercent_Axis_B;

	int32_t n;
	int32_t d;
	uint32_t axisId;
	uint32_t radial_Axial;

	float amplitude_Axial_Z;
	float amplitude_Axial_X;
	float amplitude_Axial_B;

	float amplitude_Radial_Z;
	float amplitude_Radial_X;
	float amplitude_Radial_B;

	float spindleRevolutions;
};

struct configPageMainOne  // page 2 (pageOne) and page 0 (pageMain)
{
	int32_t accel_Axis_B;
	int32_t maxSpd_Spindle;
	int32_t maxSpd_Axis_Z;
	int32_t maxSpd_Axis_X;
	int32_t maxSpd_Axis_B;
	int32_t speedPercent_Spindle;
	int32_t speedPercent_Axis_Z;
	int32_t speedPercent_Axis_X;
	int32_t speedPercent_Axis_B;
	int32_t accel_Spindle;
	int32_t accel_Axis_Z;
	int32_t accel_Axis_X;
	
	int32_t axisId; // Z:0, X:1, B:2, Spindle:3
};

struct configPageGreekKey
{
	uint32_t axisId;
	uint32_t fileOrPattern;

	// pageGrk settings
	uint32_t patternId;
	float countPatternPer360_Pattern;
	int32_t patternCount_Pattern;
	int32_t radialOrAxial_Pattern;
	float segmentLength_Pattern;

	// pageGrkFile settings
	int32_t patternCount_File;
	float countPatternPer360_File;
	int32_t segmentOrActual;
	int32_t radialOrAxial_File;
	float segmentLength_File;

	// Stepper settings
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
	int32_t sizeInSteps;
	double sizeInUnits;

};

/////////////////////////////////////////////////////////////////////////
// Config and Setup variables
/////////////////////////////////////////////////////////////////////////
configPageSetup configSetup;
configPageMainOne configMain;
configPageSync configSync;
configPageRec configRec;
configPageMainOne configOne;
configPageIndex configIndex_Main;
configIndex configIndex_1;
configIndex configIndex_2;
configIndex configIndex_3;
configIndex configIndex_4;
configIndex configIndex_5;
configPageMov configMove;
configPageRose configRose;
configPageGreekKey configGreekKey;
//configSteppers configGreekKey_Z;
//configSteppers configGreekKey_X;
//configSteppers configGreekKey_B;

unsigned int eePromAddress_Setup = 0; // EEProm address for Setup
unsigned int eePromAddress_Sync = 300;  // EEProm address for Sync
unsigned int eePromAddress_Main = 400;  // EEProm address for Main
unsigned int eePromAddress_Mov = 500; // EEProm address for Move
unsigned int eePromAddress_Rose = 600; // EEProm address for Rose
unsigned int eePromAddress_Rec = 700;
unsigned int eePromAddress_One = 900;  // EEProm address for Sp 
unsigned int eePromAddress_Grk = 1000;  // EEProm address for Greek Key Main
//unsigned int eePromAddress_Grk_Z = 1100; //  EEProm address for Greek Key Z
//unsigned int eePromAddress_Grk_X = 1200; //  EEProm address for Greek Key X 
//unsigned int eePromAddress_Grk_B = 1300; //  EEProm address for Greek Key B
unsigned int eePromAddress_Ind_Main = 1400;  // EEProm address for Index_Main
unsigned int eePromAddress_Ind_1 = 1500;  // EEProm address for Index_1
unsigned int eePromAddress_Ind_2 = 1550;  // EEProm address for Index_2
unsigned int eePromAddress_Ind_3 = 1600;  // EEProm address for Index_3 
unsigned int eePromAddress_Ind_4 = 1650;  // EEProm address for Index_3 
unsigned int eePromAddress_Ind_5 = 1700;  // EEProm address for Index_3 

unsigned int eePromAddress_Filename_Ind = 1800; // EEProm address for filename
unsigned int eePromAddress_Filename_Length_Ind = 1900; // EEProm address for length of filename
unsigned int eePromAddress_Filename_Grk = 2000; // EEProm address for filename
unsigned int eePromAddress_Filename_Length_Grk = 2100; // EEProm address for length of filename

//==================================================================
// Global Variables
//==================================================================
int pageCallerId = 20;
int serialId = 9; // Initialize with unused serial id.  Serial: 0 (Usb cable to PC), Serial1: 1, Serial2: 2, Serial3: 3
byte incomingByte = 0;	// store incoming Serial data

int enableTimeout = 200; // Workaround for DM542T external driver
float distanceTotal_MoveZ = 0;
float distanceTotal_MoveX = 0;

// microSD id
int microSD_Id = 0; // 0 = card not found, 1 = Builtin SD, 9 = PIN_SPI_CS_9, 10 = PIN_SPI_CS_10, 15 = PIN_SPI_CS_15,24 = PIN_SPI_CS_24,  
String filename_Index1;
int filenameLength = 0;
int currentFileIndex = 0;
int lastFileIndex = -1;
int maxFiles = 100;
String sdFilename;
bool badFilename = false;
bool fileDataAvailable = false;

String filename_Index2a;
char * filename_Index2;

int32_t returnSteps_Spindle = 0;
int32_t startPositionAbs_Axis = 0;

/////////////////////////////////////////////////////////////////////////
// Rose settings
/////////////////////////////////////////////////////////////////////////

volatile int32_t newMaxSpd_RoseSpindle;
volatile int32_t newMaxSpd_RoseAxis;
volatile int32_t spindleStepsPerRev;
volatile float slideStepsAmplitude;
volatile float spindleStepsAmplitude;
volatile float kRatio;

//float initialPosition_Axis = 0;
int32_t endPosition_Spindle = 0;
int32_t endPosition_Axis = 0;
float degrees_Spindle = 0;
float distance_Axis = 0;

// Accuracy interval 
constexpr unsigned recalcPeriod = 25'000; //?s  period for calculation of new target points. Straight lines between those points. 
constexpr float dtRose = recalcPeriod / 1E6;  //seconds  1E6 = 1,000,000 

constexpr unsigned PID_Interval = 10; // ms  
constexpr float P = 0.01;             // (P)roportional constant of the regulator needs to be adjusted (depends on speed and acceleration setting)
constexpr unsigned priorityLevel = 128;// 255;//64; //// 

/////////////////////////////////////////////////////////////////////////
// Greek Key
/////////////////////////////////////////////////////////////////////////

int reverseDirection = 1;
int moveType = 0;
String axisAndDirection;
String axisAndDirectionA;

int currentLineNumber = 0;
String currentCommand = "";
bool exitGreekKey = false;
String comment;
int spindleShortLegSteps = 0;
int axisShortLegSteps = 0;

float angularAxisLegLength = 0; // Horizontal component of angular move
float angularSpindleLegLength = 0; // Vertical component of angular move
long angularAxisSpeed = 0;
long angularSpindleSpeed = 0;

const char* filename_4Axes = "/4Axes.ini";
const char* filename_Reset = "/Reset.ini";
int iniFileType = INI_4AXES;

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

//==================================================================
// Prototypes
//==================================================================
void SerialPrint(String text, int decimalPlaces = 0);
void SerialPrint(float number, int decimalPlaces = 0);
//==================================================================

//==================================================================
// DRV8825 microstep settings
//==================================================================

	//MODE0 	MODE1 	MODE2 	Microstep Resolution
	//Low 	Low 	Low 	Full step
	//High 	Low 	Low 	Half step
	//Low 	High 	Low 	1 / 4 step
	//High 	High 	Low 	1 / 8 step
	//Low 	Low 	High 	1 / 16 step
	//High 	Low 	High 	1 / 32 step
	//Low 	High 	High 	1 / 32 step
	//High 	High 	High 	1 / 32 step
//==================================================================