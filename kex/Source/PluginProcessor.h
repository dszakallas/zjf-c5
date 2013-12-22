/*
==============================================================================

This file is part of the Kex Compressor/Expander Audio Effect.
Copyright (c) 2013 - Dávid Szakállas.

Permission is granted to use this software under the GPL v2 (or any later version).

Details of these licenses can be found at: www.gnu.org/licenses

This software is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class KexAudioProcessor  : public AudioProcessor
{
public:
    KexAudioProcessor();
    ~KexAudioProcessor();

    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();
    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);
    AudioProcessorEditor* createEditor();
    bool hasEditor() const;
    const String getName() const;
    int getNumParameters();
    float getParameter (int index);
    void setParameter (int index, float newValue);
    const String getParameterName (int index);
    const String getParameterText (int index);
    const String getInputChannelName (int channelIndex) const;
    const String getOutputChannelName (int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;
    bool acceptsMidi() const;
    bool producesMidi() const;
    bool silenceInProducesSilenceOut() const;
    double getTailLengthSeconds() const;
    int getNumPrograms();
    int getCurrentProgram();
    void setCurrentProgram (int index);
    const String getProgramName (int index);
    void changeProgramName (int index, const String& newName);
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);
	float getInputLevel() const;
	float getEnvelopeLevel() const;

private:
	void _process_params(float sr);
	void _process_stereo(AudioSampleBuffer& buffer);
	void _process_lr(AudioSampleBuffer& buffer);
	void _process_envs();

	CriticalSection params;
	int p_flags;
	float p_wet, p_gain, p_att, p_rel, p_ct, p_cr, p_et, p_er, p_rms,
		p_stereo, p_p_g;
	
	CriticalSection envs;
	float e_lvl, e_env;


	//STATE VARIABLES -- ONLY THE PROCESSING THREAD CAN TOUCH THEM!
	//YOU CANNOT READ OR WRITE THEM ANYWHERE ELSE ONLY IN THE 
	//processBlock() CALLBACK DURING PROCESSING
	float wet, gain, att, rel, ct, cs, et, es, matt, mrel;
	bool rms, stereo;
	float p_g;
	float *lvl, *env, *g;
 
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KexAudioProcessor)
};

#endif
