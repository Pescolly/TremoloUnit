/*
     File: TremoloUnit.cpp
 Abstract: TremoloUnit.h
  Version: 1.0.1
 
 Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple
 Inc. ("Apple") in consideration of your agreement to the following
 terms, and your use, installation, modification or redistribution of
 this Apple software constitutes acceptance of these terms.  If you do
 not agree with these terms, please do not use, install, modify or
 redistribute this Apple software.
 
 In consideration of your agreement to abide by the following terms, and
 subject to these terms, Apple grants you a personal, non-exclusive
 license, under Apple's copyrights in this original Apple software (the
 "Apple Software"), to use, reproduce, modify and redistribute the Apple
 Software, with or without modifications, in source and/or binary forms;
 provided that if you redistribute the Apple Software in its entirety and
 without modifications, you must retain this notice and the following
 text and disclaimers in all such redistributions of the Apple Software.
 Neither the name, trademarks, service marks or logos of Apple Inc. may
 be used to endorse or promote products derived from the Apple Software
 without specific prior written permission from Apple.  Except as
 expressly stated in this notice, no other rights or licenses, express or
 implied, are granted by Apple herein, including but not limited to any
 patent rights that may be infringed by your derivative works or by other
 works in which the Apple Software may be incorporated.
 
 The Apple Software is provided by Apple on an "AS IS" basis.  APPLE
 MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
 THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
 FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND
 OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
 
 IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
 OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
 MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED
 AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
 STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 
 Copyright (C) 2014 Apple Inc. All Rights Reserved.
 
*/


#include "TremoloUnit.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Standard DSP AudioUnit implementation

AUDIOCOMPONENT_ENTRY(AUBaseProcessFactory, TremoloUnit)




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#pragma mark ____Construction_Initialization


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TremoloUnit::TremoloUnit
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TremoloUnit::TremoloUnit(AudioUnit component) : AUEffectBase(component)
{
	// all the parameters must be set to their initial values here
	//
	// these calls have the effect both of defining the parameters for the first time
	// and assigning their initial values
	
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	
	SetParameter(kParameter_Frequency, kDefaultValue_Tremolo_Freq);
	SetParameter(kParameter_Depth, kDefaultValue_Tremolo_Depth);
	SetParameter(kParameter_Waveform, kDefaultValue_Tremolo_Waveform);
	
	SetAFactoryPresetAsCurrent(kPresets[kPreset_Default]);
	
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher(this);
#endif

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TremoloUnit::Initialize
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus TremoloUnit::Initialize()
{
	OSStatus result = AUEffectBase::Initialize();
	
	if(result == noErr )
	{
		// in case the AU was un-initialized and parameters were changed, the view can now
		// be made aware it needs to update the frequency response curve
	}
	
	return result;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#pragma mark ____Parameters

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TremoloUnit::GetParameterInfo
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus TremoloUnit::GetParameterInfo( AudioUnitScope inScope, AudioUnitParameterID inParameterID,
									   AudioUnitParameterInfo &outParameterInfo )
{
	ComponentResult result = noErr;
	
	outParameterInfo.flags = kAudioUnitParameterFlag_IsWritable | kAudioUnitParameterFlag_IsReadable;
		
	if (inScope == kAudioUnitScope_Global)
	{
		switch(inParameterID)
		{
			case kParameter_Frequency:
				AUBase::FillInParameterName(outParameterInfo, kParamName_Tremolo_Freq, false);
				outParameterInfo.unit = kAudioUnitParameterUnit_Hertz;
				outParameterInfo.defaultValue = kDefaultValue_Tremolo_Freq;
				outParameterInfo.maxValue = kMaximumValue_Tremolo_Freq;
				outParameterInfo.minValue = kMinimumValue_Tremolo_Freq;
				outParameterInfo.flags |= kAudioUnitParameterFlag_DisplayLogarithmic;
				break;
				
			case kParameter_Depth:
				AUBase::FillInParameterName (outParameterInfo, kParamName_Tremolo_Depth, false);
				outParameterInfo.unit = kAudioUnitParameterUnit_Percent;
				outParameterInfo.defaultValue = kDefaultValue_Tremolo_Depth;
				outParameterInfo.minValue = kMinimumValue_Tremolo_Depth;
				outParameterInfo.maxValue = kMaximumValue_Tremolo_Depth;
				break;
				
			case kParameter_Waveform:
				AUBase::FillInParameterName(outParameterInfo, kParamName_Tremolo_Waveform, false);
				outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
				outParameterInfo.defaultValue = kDefaultValue_Tremolo_Waveform;
				outParameterInfo.minValue = kSineWave_Tremolo_Waveform;
				outParameterInfo.maxValue = kSquareWave_Tremolo_Waveform;
				break;
				
			default:
				result = kAudioUnitErr_InvalidParameter;
				break;
		}
	}
	else
	{
		result = kAudioUnitErr_InvalidParameter;
	}
	
	return result;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#pragma mark ____Properties

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TremoloUnit::GetPropertyInfo
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus TremoloUnit::GetPropertyInfo (	AudioUnitPropertyID	inID, AudioUnitScope inScope,
									AudioUnitElement inElement,UInt32 &outDataSize, Boolean &outWritable)
{
			// return default UI properties
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TremoloUnit::GetProperty
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus			TremoloUnit::GetProperty (	AudioUnitPropertyID 		inID,
											AudioUnitScope 				inScope,
											AudioUnitElement			inElement,
											void *						outData)
{
	// return default UI properties
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

#pragma mark ____Presets
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TremoloUnit::GetPresets
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus TremoloUnit::GetPresets(CFArrayRef *outData) const
{
		// this is used to determine if presets are supported 
		// which in this unit they are so we implement this method!
	if (outData == NULL) return noErr;
	
	CFMutableArrayRef presetsArray = CFArrayCreateMutable(NULL, kNumberPresets, NULL);
	for (int i = 0; i < kNumberPresets; ++i)
	{
		CFArrayAppendValue (presetsArray, &kPresets[i]);
    }
    
	*outData = (CFArrayRef)presetsArray;	// client is responsible for releasing the array
	return noErr;
}

#pragma mark ____TremoloUnitNewFactoryPresetSet
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TremoloUnit::NewFactoryPresetSet
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus TremoloUnit::NewFactoryPresetSet(const AUPreset &inNewFactoryPreset)
{
	SInt32 chosenPreset = inNewFactoryPreset.presetNumber;
	
	if (chosenPreset == kPreset_Fast || chosenPreset == kPreset_Slow)
	{
		for (int i = 0; i < kNumberPresets; ++i)
		{
			if (chosenPreset == kPresets[i].presetNumber)
			{
					// set whatever state you need to based on this preset's selection
					//
					// Here we use a switch statement, but it would also be possible to
					// use chosenPreset as an index into an array (if you publish the preset
					// numbers as indices in the GetPresets() method)
					//
				switch(chosenPreset)
				{
					case kPreset_Fast:
						SetParameter(kParameter_Waveform, kParameter_Preset_Waveform_Fast);
						SetParameter(kParameter_Frequency, kParameter_Preset_Frequency_Fast);
						SetParameter(kParameter_Depth, kParameter_Preset_Depth_Fast);
						break;
						
					case kPreset_Slow:
						SetParameter(kParameter_Waveform, kParameter_Preset_Waveform_Slow);
						SetParameter(kParameter_Frequency, kParameter_Preset_Frequency_Slow);
						SetParameter(kParameter_Depth, kParameter_Preset_Depth_Slow);
						break;
				}
				
				SetAFactoryPresetAsCurrent(kPresets[i]);
				return noErr;
			}
		}
	}
	
	return kAudioUnitErr_InvalidPropertyValue;
}

#pragma mark ____TremoloUnitGetParameterAccessStrings

/*	
 *TremoloUnit::GetParameterValueStrings()
 */
ComponentResult TremoloUnit::GetParameterValueStrings(AudioUnitScope inScope, AudioUnitParameterID inParameterID,
													  CFArrayRef *outStrings)
{
	if((inScope == kAudioUnitScope_Global) && (inParameterID == kParameter_Waveform))
	{
		if (outStrings == NULL)
		{
			return noErr;
		}
		
		CFStringRef strings[] = { kMenuItem_Tremolo_Sine, kMenuItem_Tremolo_Square };
		
		*outStrings = CFArrayCreate(NULL, (const void **) strings, (sizeof(strings) / sizeof(strings[0])), NULL);
		
		return noErr;
	}
	
	return kAudioUnitErr_InvalidParameter;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#pragma mark ____TremoloUnitKernel


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TremoloUnitKernel::TremoloUnitKernel()
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TremoloUnitKernel::TremoloUnitKernel(AUEffectBase *inAudioUnit ) : AUKernelBase(inAudioUnit), mSamplesProcessed(0), mCurrentScale(0)	//call superclasses and init two member vars
{
	for (int i = 0; i < kWaveArraySize; ++i)	//generates a wave table that represnets one cycle of a sin wave, normalized to 0<->1
	{
		double radians = i * 2.0 * M_PI / kWaveArraySize;
		mSineWavetable[i] = (sin (radians) + 1.0) * 0.5;
	}
	
	for (int i = 0; i < kWaveArraySize; ++i)	//generates a wave table that represents one cycle of a pseudo square wave normalized to 0<->1
	{
		double radians = i * 2.0 * M_PI / kWaveArraySize;
		radians += 0.32;
		mSquareWavetable[i] =
		(
			sin(radians) +
			0.3 * sin(3 * radians) +
			0.15 * sin(5 * radians) +
			0.075 * sin(7 * radians) +
			0.0375 * sin(9 * radians) +
			0.01875 * sin(11 * radians) +
			0.009375 * sin(13 * radians) +
			0.8
		 ) * .63;
	}
	
	mSampleFrequency = GetSampleRate();					//gets the sample rate for audio stream to be processed

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TremoloUnitKernel::~TremoloUnitKernel()
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TremoloUnitKernel::~TremoloUnitKernel( )
{
}

#pragma mark ____TremoloUnitKernelReset
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TremoloUnitKernel::Reset()
//
//		It's very important to fully reset all TremoloUnit state variables to their
//		initial settings here.  For delay/reverb effects, the delay buffers must
//		also be cleared here.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		TremoloUnitKernel::Reset()
{
	mCurrentScale = 0;	//Resets the mCurrentScale member variable to its freshly initialized value.
	mSamplesProcessed = 0;	//Resets the mSamplesProcessed member variable to its freshly initialized value.
}



#pragma mark ____TremoloUnitKernelProcess
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TremoloUnitKernel::Process(int inFramesToProcess)
//
//		We process one non-interleaved stream at a time
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TremoloUnitKernel::Process(const Float32 	*inSourceP, Float32 *inDestP, UInt32 inSamplesToProcess,
							UInt32 inNumChannels, bool &ioSilence)
{
	if (!ioSilence)														//ignore request if ioSilence
	{
		const Float32 *sourceP = inSourceP;								//assign pointer to start of sample input buffer
		
		Float32 *destP = inDestP, inputSample, outputSample, tremoloFrequency, tremoloDepth,
		samplesPerCycle, rawTremoloGain, tremoloGain;
		
		int tremoloWaveform;
		
		tremoloFrequency = GetParameter(kParameter_Frequency);			//get freq/depth/waveform from UI params
		tremoloDepth = GetParameter(kParameter_Depth);
		tremoloWaveform = (int) GetParameter(kParameter_Waveform);
	
		if (tremoloWaveform != kSineWave_Tremolo_Waveform && tremoloWaveform != kSquareWave_Tremolo_Waveform)
			tremoloWaveform = kSquareWave_Tremolo_Waveform;									//assign default waveform
		
		if (tremoloWaveform == kSineWave_Tremolo_Waveform)							//set waveArrayPointer to wavetable
		{
			waveArrayPointer = &mSineWavetable[0];
		}
		else
		{
			waveArrayPointer = &mSquareWavetable[0];
		}
		
		if (tremoloFrequency < kMinimumValue_Tremolo_Freq)							//constrain parameters
			tremoloFrequency = kMinimumValue_Tremolo_Freq;
		if (tremoloFrequency > kMaximumValue_Tremolo_Freq)
			tremoloFrequency = kMaximumValue_Tremolo_Freq;
		
		
		if (tremoloDepth < kMinimumValue_Tremolo_Depth)
			tremoloDepth = kMinimumValue_Tremolo_Depth;
		if (tremoloDepth > kMaximumValue_Tremolo_Depth)
			tremoloDepth = kMaximumValue_Tremolo_Depth;
	
		samplesPerCycle = mSampleFrequency / tremoloFrequency;					//Calculates the number of audio samples per cycle of tremolo frequency.
		mNextScale = kWaveArraySize / samplesPerCycle;				//Calculates the scaling factor to use for applying the wave table to the current sampling frequency and tremolo frequency.

		
			//sample processing loop
		for (int i = inSamplesToProcess; i > 0; i--)
		{
				//Calculates the point in the wave table to use for the current sample.
			int index = static_cast<long>(mSamplesProcessed * mCurrentScale) % kWaveArraySize;
			
				//Tests if the scaling factor should change, and if itÕs safe to change it at the current sample to avoid artifacts.
				//If both conditions are met, switches the scaling factor and resets the mSamplesProcessed variable.
			if ((mNextScale != mCurrentScale) && (index == 0))
			{
				mCurrentScale = mNextScale;
				mSamplesProcessed = 0;
			}
			
				//Tests if the mSamplesProcessed variable has grown to a large value, and if it's safe to reset it at the current sample to avoid artifacts.
				//If both conditions are met, resets the mSamplesProcessed variable.
			if ((mSamplesProcessed >= sampleLimit) && (index == 0))
			{
				mSamplesProcessed = 0;
			}
			
			rawTremoloGain = waveArrayPointer[index];				//Gets the tremolo gain from the appropriate point in the wave table.
			
			tremoloGain = (rawTremoloGain * tremoloDepth - tremoloDepth + 100.0) * 0.01;  //Adjusts the tremolo gain by applying the Depth parameter.
			inputSample = *sourceP;										//Gets an audio sample from the appropriate spot in the audio sample input buffer.
			outputSample = inputSample * tremoloGain;				//Calculates the corresponding output audio sample.
			*destP = outputSample;						//Places the output audio sample at the appropriate spot in the audio sample output buffer.
			
			sourceP++;						//Increments the position counter for the audio sample input buffer.
			destP++;						//Increments the position counter for the audio sample output buffer.
			mSamplesProcessed++;			//increments the count of processed audio samples
		}
	}
}
