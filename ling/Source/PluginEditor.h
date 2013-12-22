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

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class LiNgAudioProcessorEditor  :	public AudioProcessorEditor,
									public ButtonListener,
									public SliderListener,
									public Timer
{
public:
    LiNgAudioProcessorEditor (LiNgAudioProcessor* ownerFilter);
    ~LiNgAudioProcessorEditor();

    //==============================================================================
    // This is just a standard Juce paint method...
    void paint (Graphics& g);
	void resized();
	void timerCallback();
	void buttonClicked(Button* buttonThatWasClicked);
	void sliderValueChanged(Slider* sliderThatWasMoved);
	void _recalc_params();
private:
	LiNgAudioProcessor * ownerFilter;
	ScopedPointer<TextButton> ln_b;
	ScopedPointer<Slider> t_s;
	ScopedPointer<Label> t_l;
	ScopedPointer<Label> dw_l;
	ScopedPointer<Slider> dw_s;
	ScopedPointer<Slider> p_g_s;
	ScopedPointer<Label> p_g_val;
	ScopedPointer<Label> g_l;
	ScopedPointer<Slider> g_s;






};


#endif  // PLUGINEDITOR_H_INCLUDED
