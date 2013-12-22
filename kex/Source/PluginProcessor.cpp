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
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PluginHelper.h"
#include <algorithm>

KexAudioProcessor::KexAudioProcessor() : 
	params(),
	p_flags(),
	p_wet(1.f),
	p_gain(0.7403f),
	p_att(0.2f),
	p_rel(0.2f),
	p_ct(),
	p_cr(),
	p_et(),
	p_er(),
	p_rms(1.f),
	p_stereo(1.f),
	p_p_g(0.5119f),

	envs(),
	e_lvl(0.f),
	e_env(1.f),

	wet(_wet(p_wet)),
	gain(_gain(p_gain)),
	att(),
	rel(),
	ct(_ct(p_ct)),
	cs(_cs(p_cr)),
	et(_et(p_et)),
	es(_es(p_er)),
	rms(_rms(p_rms)),
	stereo(_stereo(p_stereo)),
	p_g(_p_g(p_p_g)),
	lvl(),
	env(),
	g()

{
}

KexAudioProcessor::~KexAudioProcessor()
{
	delete[] env;
	delete[] lvl;
	delete[] g;
}

const String KexAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int KexAudioProcessor::getNumParameters()
{
    return 11;
}

float KexAudioProcessor::getParameter (int index)
{
	{
		ScopedLock lock(params);
		
		switch (index){
		case 0: return p_wet;
		case 1: return p_gain;
		case 2: return p_att;
		case 3: return p_rel;
		case 4: return p_ct;
		case 5: return p_cr;
		case 6: return p_et;
		case 7: return p_er;
		case 8: return p_rms;
		case 9: return p_stereo;
		case 10: return p_p_g;
		default: return 0.0f;
		}
	}
}

void KexAudioProcessor::setParameter (int index, float newValue)
{
	{
		ScopedLock lock(params);
		switch (index) {
		case 0: p_wet = newValue;	p_flags |= 1; break;
		case 1: p_gain = newValue;	p_flags |= 2; break;
		case 2: p_att = newValue;	p_flags |= 4; break;
		case 3: p_rel = newValue;	p_flags |= 8; break;
		case 4: p_ct = newValue;	p_flags |= 16; break;
		case 5: p_cr = newValue;	p_flags |= 32; break;
		case 6: p_et = newValue;    p_flags |= 64; break;
		case 7: p_er = newValue;	p_flags |= 128; break;
		case 8: p_rms = newValue;	p_flags |= 256; break;
		case 9:	p_stereo = newValue;	p_flags |= 512; break;
		case 10: p_p_g = newValue; p_flags |= 1024; break;
		}
	}
}

const String KexAudioProcessor::getParameterName(int index)
{
	switch (index){
	case 0: return String("Dry/Wet"); break;
	case 1: return String("Gain"); break;
	case 2: return String("Attack"); break;
	case 3: return String("Release"); break;
	case 4: return String("Comp. Thres."); break;
	case 5: return String("Comp. Ratio"); break;
	case 6: return String("Exp. Thres."); break;
	case 7: return String("Exp. Ratio"); break;
	case 8: return String("Level Meas."); break;
	case 9: return String("Envelope"); break;
	case 10: return String("Preamp"); break;
	default: return String::empty;
	}
}

const String KexAudioProcessor::getParameterText (int index)
{
	ScopedLock lock(params);
	switch (index){
	case 0: return _wet_str(p_wet); break;
	case 1: return _gain_str(p_gain); break;
	case 2: return _attms_str(p_att); break;
	case 3: return _relms_str(p_rel); break;
	case 4: return _ct_str(p_ct); break;
	case 5: return _cr_str(p_cr); break;
	case 6: return _et_str(p_et); break;
	case 7: return _er_str(p_er); break;
	case 8: return _rms_str(p_rms); break;
	case 9: return _stereo_str(p_stereo); break;
	case 10: return _p_g_str(p_p_g); break;
	default: return String::empty;
	}
}

const String KexAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String KexAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool KexAudioProcessor::isInputChannelStereoPair (int) const
{
    return true;
}

bool KexAudioProcessor::isOutputChannelStereoPair (int) const
{
    return true;
}

bool KexAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool KexAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool KexAudioProcessor::silenceInProducesSilenceOut() const
{
    return true;
}

double KexAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int KexAudioProcessor::getNumPrograms()
{
    return 0;
}

int KexAudioProcessor::getCurrentProgram()
{
    return 0;
}

void KexAudioProcessor::setCurrentProgram (int)
{
}

const String KexAudioProcessor::getProgramName (int)
{
    return String::empty;
}

void KexAudioProcessor::changeProgramName (int, const String&)
{
}

void KexAudioProcessor::prepareToPlay(double sampleRate, int)
{
	{
		ScopedLock lock(params);
		att = _impinv(_attms(p_att), (float)sampleRate);
		rel = _impinv(_relms(p_rel), (float)sampleRate);
		matt = _impinv(5.f, (float)sampleRate);
		mrel = _impinv(130.f, (float)sampleRate);
	}

	int numChannels = getNumInputChannels();
	if (env == 0)		env = new float[numChannels];
	if (lvl == 0)		lvl = new float[numChannels];
	if (g == 0)			g = new float[numChannels];

	memset(lvl, 0, sizeof(float)* numChannels);

	for (int i = 0; i < numChannels; ++i)
		env[i] = g[i] = 1;


}

void KexAudioProcessor::releaseResources()
{
	{
		ScopedLock lock(envs);
		e_lvl = 0;
		e_env = 1;
	}

	delete[] env;
	env = 0;
	delete[] lvl;
	lvl = 0;
	delete[] g;
	g = 0;

}

void KexAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer&)
{
	_process_params((float)getSampleRate());

	if (stereo)
		_process_stereo(buffer);
	else
		_process_lr(buffer);

	_process_envs();
}

bool KexAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* KexAudioProcessor::createEditor()
{
    return new KexAudioProcessorEditor (this);
}

void KexAudioProcessor::getStateInformation (MemoryBlock&)
{
}

void KexAudioProcessor::setStateInformation (const void*, int)
{
}


void KexAudioProcessor::_process_params(float sr) 
{
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
				att = _impinv(_attms(p_att), sr);
			if (p_flags & 8)
				rel = _impinv(_relms(p_rel), sr);
			if (p_flags & 16)
				ct = _ct(p_ct);
			if (p_flags & 32)
				cs = _cs(p_cr);
			if (p_flags & 64)
				et = _et(p_et);
			if (p_flags & 128)
				es = _es(p_er);
			if (p_flags & 256)
				rms = _rms(p_rms);
			if (p_flags & 512)
				stereo = _stereo(p_stereo);
			if (p_flags & 1024)
				p_g = _p_g(p_p_g);
			p_flags = 0;
		}
	}
}

void KexAudioProcessor::_process_stereo(AudioSampleBuffer& buffer) {
	for (int sample = 0; sample < buffer.getNumSamples(); sample++){
		for (int ch = 0; ch < buffer.getNumChannels(); ++ch){
			float in = buffer.getSampleData(ch)[sample] * p_g;
			float w = (rms ? (in * in) : fabs(in));
			lvl[ch] = w > lvl[ch] ?
				(1.0f - matt) * lvl[ch] + matt * w :
				(1.0f - mrel) * lvl[ch];
		}
		float _lvl= 0;

		for (int i = 0; i < getNumInputChannels(); ++i) {
			_lvl = std::max(_lvl, lvl[i]);
		}

		float _env = _lvl > ct ?
			powf(10, -cs * (log10f(_lvl) - log10f(ct))) :
			1.0f;
		_env = (_lvl > 0 && _lvl < et) ?
			_env * powf(10.0f, -es * (log10f(_lvl) - log10f(et))) :
			_env;
		
		for (int ch = 0; ch < buffer.getNumChannels(); ++ch){
			float* ins = buffer.getSampleData(ch);
			float* outs = buffer.getSampleData(ch);
			
			float c = env[ch] - _env;
			float k;
			if (c > 0.04) {
				k = att;
			}
			else {
				if (c < -0.04)
					k = rel;
				else
					k = (rel + att) *0.5f;
			}
			g[ch] = (1.0f - k) *g[ch] + k * _env;
			env[ch] = _env;
			outs[sample] = wet * g[ch] * ins[sample] * gain * p_g + (1.f - wet) * ins[sample];
		}		
	}
}

void KexAudioProcessor::_process_lr(AudioSampleBuffer& buffer) {
	for (int ch = 0; ch < buffer.getNumChannels(); ++ch){
		float* ins = buffer.getSampleData(ch);
		float* outs = buffer.getSampleData(ch);

		for (int sample = 0; sample < buffer.getNumSamples(); sample++){

			float in = ins[sample] * p_g;
			float w = (rms ? (in * in) : fabs(in));
			lvl[ch] = w > lvl[ch] ?
				(1.0f - matt) * lvl[ch] + matt * w :
				(1.0f - mrel) * lvl[ch];


			float _env = lvl[ch] > ct ?
				powf(10, -cs * (log10f(lvl[ch]) - log10f(ct))) :
				1.0f;
			_env = (lvl[ch] > 0 && lvl[ch] < et) ?
				_env * powf(10, -es * (log10f(lvl[ch]) - log10f(et))) :
				_env;
	

			float c = env[ch] - _env;
			float k;
			if (c > 0.04) {
				k = att;
			}
			else {
				if (c < -0.04)
					k = rel;
				else
					k = (rel + att) *0.5f;
			}
			g[ch] = (1.0f - k) *g[ch] + k * _env;
			env[ch] = _env;

			outs[sample] = wet * g[ch] * ins[sample] * p_g * gain + (1.f - wet) * ins[sample];
		}

	}
}

void KexAudioProcessor::_process_envs() {
	const ScopedTryLock lock(envs);
	if (!lock.isLocked())
		return;
	else {
		int c = getNumInputChannels();
		e_lvl = 0;
		for (int i = 0; i < c; ++i)
			e_lvl += lvl[i];
		e_lvl /= c;
		e_env = 0;
		for (int i = 0; i < c; ++i)
			e_env += g[i];
		e_env /= c;
	}
}

float KexAudioProcessor::getInputLevel() const {
	ScopedLock lock(envs);
	return e_lvl;
}
float KexAudioProcessor::getEnvelopeLevel() const {
	ScopedLock lock(envs);
	return e_env;
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new KexAudioProcessor();
}
