/*
==============================================================================

This file is part of the LiNg Compressor/Expander Audio Effect.
Copyright (c) 2013 - Dávid Szakállas.

Permission is granted to use this software under the GPL v2 (or any later version).

Details of these licenses can be found at: www.gnu.org/licenses

This software is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
LiNgAudioProcessor::LiNgAudioProcessor() : 
	params(),
	p_flags(),
	p_wet(1.0f),
	p_gain(0.7403f),
	p_t(),
	p_p_g(0.5119f),
	p_ln(1.0f),

	wet(_wet(p_wet)),
	gain(_gain(p_gain)),
	latt(),
	lrel(),
	natt(),
	nrel(),
	t(_t(p_t)),
	p_g(_p_g(p_p_g)),
	ln(_ln(p_ln)),
	lvl(),
	env(),
	g()
{
}

LiNgAudioProcessor::~LiNgAudioProcessor()
{
	delete[] env;
	delete[] lvl;
	delete[] g;
}

//==============================================================================
const String LiNgAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int LiNgAudioProcessor::getNumParameters()
{
    return 5;
}

float LiNgAudioProcessor::getParameter (int index)
{
	{
		ScopedLock lock(params);

		switch (index){
		case 0: return p_wet;
		case 1: return p_gain;
		case 2: return p_t;
		case 3: return p_p_g;
		case 4: return p_ln;
		default: return 0.0f;
		}
	}
}

void LiNgAudioProcessor::setParameter (int index, float newValue)
{
	{
		ScopedLock lock(params);
		switch (index) {
		case 0: p_wet = newValue;	p_flags |= 1; break;
		case 1: p_gain = newValue;	p_flags |= 2; break;
		case 2: p_t = newValue;		p_flags |= 4; break;
		case 3: p_p_g = newValue;	p_flags |= 8; break;
		case 4: p_ln = newValue;	p_flags |= 16; break;
		}
	}
}

const String LiNgAudioProcessor::getParameterName (int index)
{
	switch (index){
	case 0: return String("Dry/Wet"); break;
	case 1: return String("Gain"); break;
	case 2: return String("Threshold"); break;
	case 3: return String("Preamp"); break;
	case 4: return String("Function"); break;
	default: return String::empty;
	}
}

const String LiNgAudioProcessor::getParameterText (int index)
{
	ScopedLock lock(params);
	switch (index){
	case 0: return _wet_str(p_wet); break;
	case 1: return _gain_str(p_gain); break;
	case 2: return _t_str(p_t); break;
	case 3: return _p_g_str(p_p_g); break;
	case 4: return _ln_str(p_ln); break;
	default: return String::empty;
	}
}

const String LiNgAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String LiNgAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool LiNgAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool LiNgAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool LiNgAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LiNgAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LiNgAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double LiNgAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LiNgAudioProcessor::getNumPrograms()
{
    return 0;
}

int LiNgAudioProcessor::getCurrentProgram()
{
    return 0;
}

void LiNgAudioProcessor::setCurrentProgram (int index)
{
}

const String LiNgAudioProcessor::getProgramName (int index)
{
    return String::empty;
}

void LiNgAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void LiNgAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	{
		ScopedLock lock(params);
		latt = _impinv(.005f, (float)sampleRate);
		lrel = _impinv(250.0f, (float)sampleRate);
		natt = _impinv(60.0f, (float)sampleRate);
		nrel = _impinv(300.0f, (float)sampleRate);

		matt = _impinv(0.001f, (float)sampleRate);
		mrel = _impinv(100.f, (float)sampleRate);

		int numChannels = getNumInputChannels();
		if (env == 0)		env = new float[numChannels];
		if (lvl == 0)		lvl = new float[numChannels];
		if (g == 0)			g = new float[numChannels];

		memset(lvl, 0, sizeof(float)* numChannels);

		for (int i = 0; i < numChannels; ++i)
			env[i] = g[i] = 1;

	}
}

void LiNgAudioProcessor::releaseResources()
{

	delete[] env;
	env = 0;
	delete[] lvl;
	lvl = 0;
	delete[] g;
	g = 0;
}

void LiNgAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	_process_params((float)getSampleRate());

	_process_stereo(buffer);
}

//==============================================================================
bool LiNgAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* LiNgAudioProcessor::createEditor()
{
    return new LiNgAudioProcessorEditor (this);
}

//==============================================================================
void LiNgAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void LiNgAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void LiNgAudioProcessor::_process_stereo(AudioSampleBuffer& buffer) {
	float _att = ln ? latt : natt;
	float _rel = ln ? lrel : nrel;
	for (int sample = 0; sample < buffer.getNumSamples(); sample++){
		for (int ch = 0; ch < buffer.getNumChannels(); ++ch){
			float in = buffer.getSampleData(ch)[sample] * p_g;
			float w = fabs(in);
			lvl[ch] = w > lvl[ch] ?
				(1.0f - matt) * lvl[ch] + matt * w :
				(1.0f - mrel) * lvl[ch];
		}
		float _lvl = 0;

		for (int i = 0; i < getNumInputChannels(); ++i) {
			_lvl = std::max(_lvl, lvl[i]);
		}
		float _env = 0;
		if (ln){
			_env = _lvl > t ? t / _lvl : 1.0f;
		}
		else{
			_env = _lvl < t ? 0.0f : 1.0f;
		}
		for (int ch = 0; ch < buffer.getNumChannels(); ++ch){
			float* ins = buffer.getSampleData(ch);
			float* outs = buffer.getSampleData(ch);

			float c = env[ch] - _env;
			float k;
			if (c > 0.04) {
				k = _att;
			}
			else {
				if (c < -0.04)
					k = _rel;
				else
					k = (_rel + _att) *0.5f;
			}
			g[ch] = (1.0f - k) *g[ch] + k * _env;
			env[ch] = _env;
			outs[sample] = wet * g[ch] * ins[sample] * gain * p_g + (1.f - wet) * ins[sample];
		}
	}
}

void LiNgAudioProcessor::_process_params(float sr){
	{
		const ScopedTryLock lock(params);
		if (!lock.isLocked())
			return;
		else {
			if (!p_flags)
				return;
			if (p_flags & 1)
				wet = _wet(p_wet);
			if (p_flags & 2)
				gain = _gain(p_gain);
			if (p_flags & 4)
				t = _t(p_t);
			if (p_flags & 8)
				p_g = _p_g(p_p_g);
			if (p_flags & 16)
				ln = _ln(p_ln);
			p_flags = 0;
		}
	}
}


//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LiNgAudioProcessor();
}
