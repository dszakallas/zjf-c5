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
#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "WaveFormDisplay.h"
#include "PluginHelper.h"

class KexAudioProcessorEditor  :	public AudioProcessorEditor,
									public ButtonListener,
									public SliderListener,
									public Timer
{
public:
    KexAudioProcessorEditor (KexAudioProcessor* ownerFilter);
    ~KexAudioProcessorEditor();

    void paint (Graphics& g);
	void resized();
	void timerCallback();
	void buttonClicked(Button* buttonThatWasClicked);
	void sliderValueChanged(Slider* sliderThatWasMoved);
private:
	void _recalc_params();

	KexAudioProcessor * ownerFilter;
	ScopedPointer<WaveformDisplay> wave;
	ScopedPointer<GroupComponent> atk_rel;
	ScopedPointer<GroupComponent> e_g;
	ScopedPointer<GroupComponent> e_c;
	ScopedPointer<GroupComponent> out;
	ScopedPointer<TextButton> rms;
	ScopedPointer<Slider> ct_s;
	ScopedPointer<Label> ct_l;
	ScopedPointer<Label> cr_l;
	ScopedPointer<Slider> cr_s;
	ScopedPointer<Label> et_l;
	ScopedPointer<Slider> et_s;
	ScopedPointer<Slider> er_s;
	ScopedPointer<Label> er_l;
	ScopedPointer<Label> g_l;
	ScopedPointer<Slider> g_s;
	ScopedPointer<Label> dw_l;
	ScopedPointer<Slider> dw_s;
	ScopedPointer<TextButton> stereo;
	ScopedPointer<Label> kex;
	ScopedPointer<Slider> atk;
	ScopedPointer<Slider> rel;
	ScopedPointer<GroupComponent> in_g;
	ScopedPointer<Slider> p_g_s;
	ScopedPointer<Label> p_g_val;
	ScopedPointer<Label> a_val;
	ScopedPointer<Label> r_val;
	ScopedPointer<Label> ct_val;
	ScopedPointer<Label> cr_val;
	ScopedPointer<Label> et_val;
	ScopedPointer<Label> er_val;
	ScopedPointer<Label> g_val;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KexAudioProcessorEditor)
};


#endif
