/* *****************************************************************
* Rose Engine Spindle definitions
* Author: Edward French
******************************************************************/
#pragma once
#define SHOW_POSITION
#define DEBUG
//#define TEENSY_32
#define ESTOP_ENABLED
//==================================================================
// Pin assignments
//==================================================================

//// Spindle
#define ID_SPINDLE 0  // ToDo: Verify Nextion sends 4 for spindle
#define PIN_SPINDLE_DIR 2  //Stepper direction
#define PIN_SPINDLE_STEP 3  //Stepper step
#define PIN_SPINDLE_ENABLE 4 //Enable

//#define PIN_SPINDLE_DIR 23  // Stepper direction
//#define PIN_SPINDLE_STEP 16  // Stepper step
//#define PIN_SPINDLE_ENABLE 17 // Enable 

// Z axis
#define ID_AXIS_Z 1
#define PIN_AXIS_Z_DIR 5 // Stepper direction
#define PIN_AXIS_Z_STEP 6 // Stepper step
#define PIN_AXIS_Z_ENABLE 14 // Enable 


// X axis
#define ID_AXIS_X 2
#define PIN_AXIS_X_DIR 20  // Stepper direction
#define PIN_AXIS_X_STEP 21 // Stepper step
#define PIN_AXIS_X_ENABLE 22 // Enable 

// M3 axis
#define ID_AXIS_3 3
#define PIN_AXIS_3_DIR 23  // Stepper direction
#define PIN_AXIS_3_STEP 16  // Stepper step
#define PIN_AXIS_3_ENABLE 17 // Enable 

// M4 axis
#define ID_AXIS_4 4
#define PIN_AXIS_4_DIR 19  // Stepper direction
#define PIN_AXIS_4_STEP 18  // Stepper step
#define PIN_AXIS_4_ENABLE 15 // Enable 



/////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////


// Move page
#define ID_MOVE_AXIS_Z1 1
#define ID_MOVE_AXIS_Z2 11
#define ID_MOVE_AXIS_X1 2
#define ID_MOVE_AXIS_X2 12
#define ID_MOVE_AXIS_M3_1 3
#define ID_MOVE_AXIS_M3_2 13

#define ID_MOVE_AXIS_M4_1 4
#define ID_MOVE_AXIS_M4_2 14

// SPI
#define PIN_SPI_CS_10 10  // Primary pin for SPI
#define PIN_SPI_CS_9 9
// ToDo: Allow on 4 motor board
//#define PIN_SPI_CS_15 15
//#define PIN_SPI_CS_24 24

// Index page ID
#define ID_INDEX_1 1
#define ID_INDEX_2 2
#define ID_INDEX_3 3
#define ID_INDEX_4 4
#define ID_INDEX_5 5

#define DIR_CCW -1
#define DIR_CW 1

#define BY_DEGREES 1 // ToDo: Change to 1 and 0
#define BY_DIVISIONS 0

#define FIXED 0
#define FILE_SD 1

#define INCHES 0
#define MILLIMETERS 1

#define WAVE_IN -1
#define WAVE_OUT 1

#define RADIAL 0
#define AXIAL 1
#define RADIAL_M3 0
#define LINEAR_M3 1
#define RADIAL_M4 0
#define LINEAR_M4 1

#define INI_4AXES 0
#define INI_RESET 1

#define TRIANGLE_STYLE 1
#define SQUARE_STYLE 2
#define SAWTOOTH_STYLE 3

#define INDEX_1 1
#define INDEX_2 2
#define INDEX_3 3
#define INDEX_4 4
#define INDEX_5 5
/////////////////////////////////////////////////////////////////////////
// Page defines
/////////////////////////////////////////////////////////////////////////
#define PAGE_SPLASH 0
#define PAGE_MAIN 1
#define PAGE_ONE 2
#define PAGE_INDEX 3
#define PAGE_MOVE 4
#define PAGE_BE 5
#define PAGE_SYNC 6
#define PAGE_REC 7
#define PAGE_GRK 8
#define PAGE_PROGRAM 9
#define PAGE_ROSE 10 
#define PAGE_MULTI 11 
#define PAGE_NUMBERPAD 12 
#define PAGE_SPINDLE 13
#define PAGE_Z 14
#define PAGE_X 15
#define PAGE_ALTERNATEX 16
#define PAGE_MOTOR_3 17
#define PAGE_MOTOR_4 18
#define PAGE_LIMITS 19
#define PAGE_MORE 21
#define PAGE_RETURNS 20

/////////////////////////////////////////////////////////////////////////
// Structs
/////////////////////////////////////////////////////////////////////////

struct nextionData
{
	uint8_t value24;
	uint8_t value16;
	uint8_t value08;
	uint8_t value00;
};

struct nextionData2
{
	uint8_t value00;
	uint8_t value08;
	uint8_t value16;
	uint8_t value24;
};


struct configPageMov
{
	uint32_t axisId;
	uint32_t maxSpd_Axis_Z;
	uint32_t accel_Axis_Z;
	uint32_t speedPercent_Axis_Z;
	uint32_t maxSpd_Axis_X;
	uint32_t accel_Axis_X;
	uint32_t speedPercent_Axis_X;
	uint32_t maxSpd_Motor_3;
	uint32_t accel_Motor_3;
	uint32_t speedPercent_Axis_M3;
	uint32_t maxSpd_Motor_4;
	uint32_t accel_Motor_4;
	uint32_t speedPercent_Axis_M4;
	float distance_MoveZ1;
	float distance_MoveX1;
	float distance_MoveM3_1;
	float distance_MoveM4_1;
	float distance_MoveZ2;
	float distance_MoveX2;
	float distance_MoveM3_2;
	float distance_MoveM4_2;
};

struct configPageSync
{
	uint32_t axisId;
	uint32_t maxSpd_Spindle;
	uint32_t accel_Spindle;
	uint32_t maxSpd_Axis_Z;
	uint32_t accel_Axis_Z;
	uint32_t maxSpd_Axis_X;
	uint32_t accel_Axis_X;
	uint32_t maxSpd_Axis_M3;
	uint32_t accel_Axis_M3;
	uint32_t maxSpd_Axis_M4;
	uint32_t accel_Axis_M4;
	uint32_t speedPercent_Spindle;
	uint32_t speedPercent_Axis_Z;
	uint32_t speedPercent_Axis_X;
	uint32_t speedPercent_Axis_M3;
	uint32_t speedPercent_Axis_M4;
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
	uint32_t radialOrLinear_Axis_M3;
	uint32_t microsteps_Axis_M3;
	uint32_t steps360_Axis_M3;
	float gearRatio_AxisM3;
	float distancePerRev_AxisM3;
	float radiusM3;
	uint32_t limit_Min_M3;
	uint32_t limit_Max_M3;

	uint32_t radialOrLinear_Axis_M4;
	uint32_t microsteps_Axis_M4;
	uint32_t steps360_Axis_M4;
	float gearRatio_AxisM4;
	float distancePerRev_AxisM4;
	float radiusM4;
	uint32_t limit_Min_M4;
	uint32_t limit_Max_M4;

	uint32_t maxSpd_Return_Spindle;
	uint32_t accel_Return_Spindle;
	uint32_t maxSpd_Return_Axis_Z;
	uint32_t accel_Return_Axis_Z;
	uint32_t maxSpd_Return_Axis_X;
	uint32_t accel_Return_Axis_X;
	uint32_t maxSpd_Return_Axis_M3;
	uint32_t accel_Return_Axis_M3;

	uint32_t maxSpd_Return_Axis_M4;
	uint32_t accel_Return_Axis_M4;

	bool polarity_Spindle;
	bool polarity_Axis_Z;
	bool polarity_Axis_X;
	bool polarity_Axis_M3;

	bool polarity_Axis_M4;

	uint32_t home_Z;
	uint32_t home_X;
	uint32_t home_M3;
	uint32_t home_M4;

	uint32_t limit_StopSpindle;

	uint32_t xAltX;
	uint32_t microsteps_Axis_XAlt;
	uint32_t steps360_Axis_XAlt;
	float distancePerRev_AxisXAlt;
	bool polarity_Axis_XAlt;
	uint32_t keepSteppersEnabled;
	uint32_t eStop;
	uint32_t limit_NCorNO; // 0 (LOW) is NC, 1 (HIGH) is NO
	uint32_t cutterMotorPin;
	uint32_t vendorId;
	uint32_t motorCount;
};

struct configSpindle
{
	uint32_t microsteps_Spindle;
	uint32_t steps360_Spindle;
	float gearRatio_Spindle;
	bool polarity_Spindle;
};

struct configZ
{
	uint32_t microsteps_Axis_Z;
	uint32_t steps360_Axis_Z;
	float distancePerRev_AxisZ;
	bool polarity_Axis_Z;
};
struct configX
{
	uint32_t microsteps_Axis_X;
	uint32_t steps360_Axis_X;
	float distancePerRev_AxisX;
	bool polarity_Axis_X;
};

struct configAltX
{

	uint32_t microsteps_Axis_XAlt;
	uint32_t steps360_Axis_XAlt;
	float distancePerRev_AxisXAlt;
	bool polarity_Axis_XAlt;
};

struct configM3
{
	uint32_t microsteps_Axis_M3;
	uint32_t steps360_Axis_M3;
	float gearRatio_AxisM3;
	float distancePerRev_AxisM3;
	bool polarity_Axis_M3;
	uint32_t radialOrLinear_Axis_M3;
	float radiusM3;

};

struct configM4
{
	
	uint32_t microsteps_Axis_M4;
	uint32_t steps360_Axis_M4;
	float gearRatio_AxisM4;
	float distancePerRev_AxisM4;
	bool polarity_Axis_M4;
	uint32_t radialOrLinear_Axis_M4;
	float radiusM4;

};

struct configPageRec // page 5,6,13,14
{
	uint32_t maxSpd_Spindle;
	uint32_t maxSpd_Axis_Z;
	uint32_t maxSpd_Axis_X;
	uint32_t maxSpd_Axis_M3;
	uint32_t maxSpd_Axis_M4;
	uint32_t speedPercent_Spindle;
	uint32_t speedPercent_Axis_Z;
	uint32_t speedPercent_Axis_X;
	uint32_t speedPercent_Axis_M3;
	uint32_t speedPercent_Axis_M4;
	uint32_t accel_Spindle;
	uint32_t accel_Axis_Z;
	uint32_t accel_Axis_X;
	uint32_t accel_Axis_M3;
	uint32_t accel_Axis_M4;
	uint32_t waves_Axial;
	uint32_t waves_Radial;
	float distance_Axial_Axis;
	float amplitude_Axial_Spindle;
	float amplitude_Radial_Axis;
	float degrees_Radial_Spindle;
	uint32_t axisId;
	uint32_t radial_axial;
	uint32_t style;
};

struct configPageRose // page 12
{
	uint32_t maxSpd_Spindle;
	uint32_t maxSpd_Axis_Z;
	uint32_t maxSpd_Axis_X;
	uint32_t maxSpd_Axis_M3;
	uint32_t maxSpd_Axis_M4;

	uint32_t accel_Spindle;
	uint32_t accel_Axis_Z;
	uint32_t accel_Axis_X;
	uint32_t accel_Axis_M3;
	uint32_t accel_Axis_M4;

	uint32_t speedPercent_Spindle;
	uint32_t speedPercent_Axis_Z;
	uint32_t speedPercent_Axis_X;
	uint32_t speedPercent_Axis_M3;
	uint32_t speedPercent_Axis_M4;

	uint32_t n;
	uint32_t d;
	uint32_t axisId;
	uint32_t radial_Axial;

	float amplitude_Axial_Z;
	float amplitude_Axial_X;
	float amplitude_Axial_M3;
	float amplitude_Axial_M4;

	float amplitude_Radial_Z;
	float amplitude_Radial_X;
	float amplitude_Radial_M3;
	float amplitude_Radial_M4;

	float spindleRevolutions;
};

struct configPageMainMulti  // page 2 (pageMulti) and page 0 (pageMain)
{

	uint32_t maxSpd_Spindle;
	uint32_t maxSpd_Axis_Z;
	uint32_t maxSpd_Axis_X;
	uint32_t maxSpd_Axis_M3;
	uint32_t maxSpd_Axis_M4;
	uint32_t speedPercent_Spindle;
	uint32_t speedPercent_Axis_Z;
	uint32_t speedPercent_Axis_X;
	uint32_t speedPercent_Axis_M3;
	uint32_t speedPercent_Axis_M4;
	uint32_t accel_Spindle;
	uint32_t accel_Axis_Z;
	uint32_t accel_Axis_X;
	uint32_t accel_Axis_M3;
	uint32_t accel_Axis_M4;
	uint32_t direction;
	uint32_t multiAxesIDs;
	uint32_t axisId; // Z:0, X:1, M3:2, M4:3,Spindle:4
	float targetDistance_Spindle;
	float targetDistance_Z;
	float targetDistance_X;
	float targetDistance_M3;
	float targetDistance_M4;
	float synchro_M3_Percentage;
	uint32_t synchro_M3_Spindle;
};

struct configPageGreekKey
{
	uint32_t axisId;
	uint32_t fileOrPattern;

	// pageGrk settings
	uint32_t patternId;
	float countPatternPer360_Pattern;
	uint32_t patternCount_Pattern;
	uint32_t radialOrAxial_Pattern;
	float segmentLength_Pattern;

	// pageProgram settings
	uint32_t patternCount_File;
	float countPatternPer360_File;
	uint32_t segmentOrActual;
	uint32_t radialOrAxial_File;
	float segmentLength_File;

	// Stepper settings
	uint32_t maxSpd_Spindle;
	uint32_t maxSpd_Axis_Z;
	uint32_t maxSpd_Axis_X;
	uint32_t maxSpd_Axis_M3;
	uint32_t maxSpd_Axis_M4;
	uint32_t speedPercent_Spindle;
	uint32_t speedPercent_Axis_Z;
	uint32_t speedPercent_Axis_X;
	uint32_t speedPercent_Axis_M3;
	uint32_t speedPercent_Axis_M4;
	uint32_t accel_Spindle;
	uint32_t accel_Axis_Z;
	uint32_t accel_Axis_X;
	uint32_t accel_Axis_M3;
	uint32_t accel_Axis_M4;
};

struct configPageIndex
{
	uint32_t axisId;
	uint32_t indexId;
	uint32_t maxSpd_Spindle;
	uint32_t maxSpd_Axis_M3;
	uint32_t maxSpd_Axis_M4;
	uint32_t speedPercent_Spindle;
	uint32_t speedPercent_Axis_M3;
	uint32_t speedPercent_Axis_M4;
	uint32_t accel_Spindle;
	uint32_t accel_Axis_M3;
	uint32_t accel_Axis_M4;
	float synchro_M3_Percentage;
	uint32_t synchro_M3_Spindle;
};

struct configIndex
{
	uint32_t degreeOrDivision; // Type
	uint32_t fileOrFixed; // Source
	uint32_t sizeInSteps;
	float sizeInUnits;

};

// Nextion data structures
nextionData nexData1;
nextionData2 nexData2;
/////////////////////////////////////////////////////////////////////////
// Config and Setup variables
/////////////////////////////////////////////////////////////////////////
configIndex configIndex_1;
configIndex configIndex_2;
configIndex configIndex_3;
configIndex configIndex_4;
configIndex configIndex_5;
configPageMov configMove;
configPageSync configSync;
configPageSetup configSetup;
configPageRec configRec;
configPageRose configRose;
configPageMainMulti configMain;
configPageMainMulti configMulti;
configPageIndex configIndex_Prime;
configPageGreekKey configGreekKey;

unsigned int eePromAddress_Setup = 0; // EEProm address for Setup (220)
unsigned int eePromAddress_Sync = 300;  // EEProm address for Sync (76)

unsigned int eePromAddress_Mov = 500; // EEProm address for Move (84)
unsigned int eePromAddress_Main = 600;  // EEProm address for Main (100)
unsigned int eePromAddress_Rec = 750; //   (96)
unsigned int eePromAddress_Multi = 900;  // EEProm address for Multi (100)
unsigned int eePromAddress_Rose = 1100; // EEProm address for Rose (112)

unsigned int eePromAddress_Grk = 1250;  // EEProm address for Greek Key and Program (108)

unsigned int eePromAddress_Ind_Prime = 1400;  // EEProm address for Index_Main (20)
unsigned int eePromAddress_Ind_1 = 1500;  // EEProm address for Index_1 (24)
unsigned int eePromAddress_Ind_2 = 1550;  // EEProm address for Index_2 (24)
unsigned int eePromAddress_Ind_3 = 1600;  // EEProm address for Index_3 (24)
unsigned int eePromAddress_Ind_4 = 1650;  // EEProm address for Index_4 (24)
unsigned int eePromAddress_Ind_5 = 1700;  // EEProm address for Index_5 (24)

unsigned int eePromAddress_Filename_Ind = 1950; // EEProm address for filename (13)
unsigned int eePromAddress_Filename_Length_Ind = 2000; // EEProm address for length of filename (13)
unsigned int eePromAddress_Filename_Program = 2050; // EEProm address for filename (13)
unsigned int eePromAddress_Filename_Length_Program = 2100; // EEProm address for length of filename (13)

//==================================================================
// Global Variables
//==================================================================
const char* nextionEnd = "\xFF\xFF\xFF";
const char* nextionQuoteEnd = "\x22\xFF\xFF\xFF";
bool limitTriggered = false;
bool eStopTriggered = false;
int pageCallerId = 20;
int serialId = 9; // Initialize with unused serial id.  Serial: 0 (Usb cable to PC), Serial1: 1, Serial2: 2, Serial3: 3
byte incomingByte = 0;	// store incoming Serial data

int enableTimeout = 300;// 200; // Workaround for DM542T external driver
float distanceTotal_MoveZ = 0;
float distanceTotal_MoveX = 0;

// microSD id
int microSD_Id = 0; // 0 = card not found, 1 = Builtin SD, 9 = PIN_SPI_CS_9, 10 = PIN_SPI_CS_10, 15 = PIN_SPI_CS_15,24 = PIN_SPI_CS_24,  
String filename_Index1;
int filenameLength = 0;
int currentFileIndex = 0;
int lastFileIndex = -1;
int maxFiles = 100;
String filename_Program;
bool badFilename = false;
bool badCommand = false;
bool fileDataAvailable = false;

String filename_Index2a;
char * filename_Index2;

int32_t returnSteps_Spindle = 0;
int32_t returnSteps_M3 = 0;
int32_t returnSteps_M4 = 0;

int32_t startPositionAbs_Axis = 0;


uint8_t value0 = 0;
uint8_t value8 = 0;
uint8_t value16 = 0;
uint8_t value24 = 0;
uint32_t packedValue = 0;

float synchro_Ratio = 0;


// PageMulti
int stepperPosition_1 = 30;
int stepperPosition_2 = 30;
int stepperPosition_3 = 30;
int stepperPosition_4 = 30;
int stepperPosition_5 = 30;
int stepperId_1 = 40;
int stepperId_2 = 40;
int stepperId_3 = 40;
int stepperId_4 = 40;
int stepperId_5 = 40;
int stepper1_step = 48;
int stepper1_dir = 49;

int stepper2_step = 50;
int stepper2_dir = 51;

int stepper3_step = 52;
int stepper3_dir = 53;

int stepper4_step = 54;
int stepper4_dir = 55;

int stepper5_step = 56;
int stepper5_dir = 57;

int checkedCount = 0;




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
int32_t endPosition_M3 = 0;
int32_t endPosition_M4 = 0;
int32_t endPosition_Axis = 0;
float degrees_Spindle = 0;
float degrees_M3 = 0;
float degrees_M4 = 0;
float distance_Axis = 0;

bool stopSteppers = false;

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

float hv_AxisLegLength = 0; // Horizontal component of angular move
float hv_SpindleLegLength = 0; // Vertical component of angular move
long angularAxisSpeed = 0;
long angularSpindleSpeed = 0;

const char* filename_4Axes = "/4Axes.ini";
const char* filename_Reset = "/Reset.ini";
int iniFileType = INI_4AXES;

int runPageID = 0;

String commandRecip = "";
String commandMove = "";
String commandIndex = "";
String commandSync = "";
String commandBRadius = "";

String enableDisableStepperID = "";


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