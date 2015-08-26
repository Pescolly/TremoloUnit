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
	if (inScope == kAudioUnitScope_Global)
	{
		switch (inID)
		{
			case kAudioUnitProperty_CocoaUI:
				outWritable = false;
				outDataSize = sizeof (AudioUnitCocoaViewInfo);
				return noErr;

		}
	}
	
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
	if (inScope == kAudioUnitScope_Global)
	{
		switch (inID)
		{
			// This property allows the host application to find the UI associated with this
			// AudioUnit
			//
			case kAudioUnitProperty_CocoaUI:
			{
				// Look for a resource in the main bundle by name and type.
				CFBundleRef bundle = CFBundleGetBundleWithIdentifier( CFSTR("com.greypool.audiounit.TremoloUnit") );
				
				if (bundle == NULL) return fnfErr;
                
				CFURLRef bundleURL = CFBundleCopyResourceURL( bundle, 
                    CFSTR("CocoaTremoloUnitView"),	// this is the name of the cocoa bundle as specified in the CocoaViewFactory.plist
                    CFSTR("bundle"),			// this is the extension of the cocoa bundle
                    NULL);
                
                if (bundleURL == NULL) return fnfErr;
                
				CFStringRef className = CFSTR("AppleDemoTremoloUnit_ViewFactory");	// name of the main class that implements the AUCocoaUIBase protocol
				AudioUnitCocoaViewInfo cocoaInfo = { bundleURL, { className } };
				*((AudioUnitCocoaViewInfo *)outData) = cocoaInfo;
				
				return noErr;
			}

			// This is our custom property which reports the current frequency response curve
			//
/*			case kAudioUnitCustomProperty_TremoloUnitFrequencyResponse:
			{
				if(inScope != kAudioUnitScope_Global) 	return kAudioUnitErr_InvalidScope;

				// the kernels are only created if we are initialized
				// since we're using the kernels to get the curve info, let
				// the caller know we can't do it if we're un-initialized
				// the UI should check for the error and not draw the curve in this case
				if(!IsInitialized() ) return kAudioUnitErr_Uninitialized;

				FrequencyResponse *freqResponseTable = ((FrequencyResponse*)outData);

				// each of our TremoloUnit kernel objects (one per channel) will have an identical frequency response
				// so we arbitrarilly use the first one...
				//
				TremoloUnitKernel *TremoloUnitKernel = dynamic_cast<TremoloUnitKernel*>(mKernelList[0]);


				double cutoff = GetParameter(kTremoloUnitParam_CutoffFrequency);
				double resonance = GetParameter(kTremoloUnitParam_Resonance );

				float srate = GetSampleRate();
				
				cutoff = 2.0 * cutoff / srate;
				if(cutoff > 0.99) cutoff = 0.99;		// clip cutoff to highest allowed by sample rate...

				TremoloUnitKernel->CalculateLopassParams(cutoff, resonance);
				
				for(int i = 0; i < kNumberOfResponseFrequencies; i++ )
				{
					double frequency = freqResponseTable[i].mFrequency;
					
					freqResponseTable[i].mMagnitude = TremoloUnitKernel->GetFrequencyResponse(frequency);
				}

				return noErr;
			}*/
		}
	}
	
	// if we've gotten this far, handles the standard properties
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
TremoloUnitKernel::TremoloUnitKernel(AUEffectBase *inAudioUnit )
	: AUKernelBase(inAudioUnit)
{
	Reset();
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

}



#pragma mark ____TremoloUnitKernelProcess
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TremoloUnitKernel::Process(int inFramesToProcess)
//
//		We process one non-interleaved stream at a time
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TremoloUnitKernel::Process( const Float32 	*inSourceP, Float32 *inDestP, UInt32 inFramesToProcess,
							UInt32 inNumChannels, bool &ioSilence)
{
	
	// do bounds checking on parameters
	//
}
