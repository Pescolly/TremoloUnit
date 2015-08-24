/*
     File: TremoloUnit.h
 Abstract: TremoloUnit.h
  Version: 1.0.1
 
	Tremolo Tutorial from Apple's DIY AU Tutorial
 
*/

#pragma mark ____TremoloUnit Parameter Constants

	// frequency of tremolo effect
static CFStringRef kParamName_Tremolo_Freq      = CFSTR ("Frequency");
static const float kDefaultValue_Tremolo_Freq   = 2.0;
static const float kMinimumValue_Tremolo_Freq   = 0.5;
static const float kMaximumValue_Tremolo_Freq   = 20.0;

	// percentage of audio signal, from 0 to unity
static CFStringRef kParamName_Tremolo_Depth     = CFSTR ("Depth");
static const float kDefaultValue_Tremolo_Depth  = 50.0;
static const float kMinimumValue_Tremolo_Depth  = 0.0;
static const float kMaximumValue_Tremolo_Depth  = 100.0;

	// LFO waveform used
static CFStringRef kParamName_Tremolo_Waveform  = CFSTR ("Waveform");
static const int kSineWave_Tremolo_Waveform     = 1;
static const int kSquareWave_Tremolo_Waveform   = 2;
static const int kDefaultValue_Tremolo_Waveform = kSineWave_Tremolo_Waveform;

	// menu item names for the waveform parameter
static CFStringRef kMenuItem_Tremolo_Sine       = CFSTR ("Sine");
static CFStringRef kMenuItem_Tremolo_Square     = CFSTR ("Square");

	// parameter identifiers
enum
{
	kParameter_Frequency  = 0,
	kParameter_Depth      = 1,
	kParameter_Waveform   = 2,
	kNumberOfParameters   = 3
};
