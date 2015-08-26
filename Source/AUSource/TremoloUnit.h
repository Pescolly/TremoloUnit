/*
     File: TremoloUnit.h
 Abstract: TremoloUnit.h
  Version: 1.0.1
 
	Tremolo Tutorial from Apple's DIY AU Tutorial
 
*/

#include "AUEffectBase.h"
#include <AudioToolbox/AudioUnitUtilities.h>
#include "TremoloUnitVersion.h"
#include <math.h>





#pragma mark ____TremoloUnitKernel

class TremoloUnitKernel : public AUKernelBase		// the actual TremoloUnit DSP happens here
{
public:
	TremoloUnitKernel(AUEffectBase *inAudioUnit );
	virtual ~TremoloUnitKernel();
	
		// processes one channel of non-interleaved samples
	virtual void 		Process(	const Float32 	*inSourceP,
								Float32		 	*inDestP,
								UInt32 			inFramesToProcess,
								UInt32			inNumChannels,
								bool &			ioSilence);
	
		// resets the TremoloUnit state
	virtual void		Reset();
};


#pragma mark ____TremoloUnit

class TremoloUnit : public AUEffectBase
{
public:
	TremoloUnit(AudioUnit component);
	
	virtual OSStatus			Version() { return kTremoloUnitVersion; }
	
	virtual OSStatus			Initialize();
	
	virtual AUKernelBase *		NewKernel() { return new TremoloUnitKernel(this); }
	
		// for custom property
	virtual OSStatus			GetPropertyInfo(	AudioUnitPropertyID		inID,
												AudioUnitScope			inScope,
												AudioUnitElement		inElement,
												UInt32 &				outDataSize,
												Boolean	&				outWritable );
	
	virtual OSStatus			GetProperty(		AudioUnitPropertyID 	inID,
											AudioUnitScope 			inScope,
											AudioUnitElement 		inElement,
											void 					* outData );
	
	
	virtual OSStatus			GetParameterInfo(	AudioUnitScope			inScope,
													AudioUnitParameterID	inParameterID,
													AudioUnitParameterInfo	&outParameterInfo );
	
		// handle presets:
	virtual OSStatus			GetPresets(	CFArrayRef	*outData	)	const;
	virtual OSStatus			NewFactoryPresetSet (	const AUPreset & inNewFactoryPreset	);
	
		// we'll report a 1ms tail.   A reverb effect would have a much more substantial tail on
		// the order of several seconds....
		//
	virtual	bool				SupportsTail () { return true; }
	virtual Float64				GetTailTime() {return 0.001;}
	
		// we have no latency
		//
		// A lookahead compressor or FFT-based processor should report the true latency in seconds
	virtual Float64				GetLatency() {return 0.0;}
	
		//let audio units generic view display options as popup
	ComponentResult 			GetParameterValueStrings (
														   AudioUnitScope          inScope,
														   AudioUnitParameterID    inParameterID,
														   CFArrayRef              *outStrings
														   );
	
};


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
