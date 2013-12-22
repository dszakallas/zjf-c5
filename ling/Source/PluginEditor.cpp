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
LiNgAudioProcessorEditor::LiNgAudioProcessorEditor (LiNgAudioProcessor* ownerFilter)
: AudioProcessorEditor(ownerFilter), ownerFilter(ownerFilter)
{
	addAndMakeVisible(ln_b = new TextButton("ln_b"));
	ln_b->setButtonText("Li");
	ln_b->addListener(this);
	ln_b->setClickingTogglesState(true);
	ln_b->setConnectedEdges(Button::ConnectedOnBottom | Button::ConnectedOnTop);

	addAndMakeVisible(t_s = new Slider("t_s"));
	t_s->setRange(0, 1, 0);
	t_s->setSliderStyle(Slider::LinearHorizontal);
	t_s->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
	t_s->setColour(Slider::textBoxBackgroundColourId, Colour(0x00ffffff));
	t_s->addListener(this);

	addAndMakeVisible(g_s = new Slider("g_s"));
	g_s->setRange(0, 1, 0);
	g_s->setSliderStyle(Slider::RotaryVerticalDrag);
	g_s->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
	g_s->setColour(Slider::textBoxBackgroundColourId, Colour(0x00ffffff));
	g_s->addListener(this);

	addAndMakeVisible(dw_s = new Slider("dw_s"));
	dw_s->setRange(0, 1, 0);
	dw_s->setSliderStyle(Slider::RotaryVerticalDrag);
	dw_s->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
	dw_s->setColour(Slider::textBoxBackgroundColourId, Colour(0x00ffffff));
	dw_s->addListener(this);

	addAndMakeVisible(p_g_s = new Slider("p_g_s"));
	p_g_s->setRange(0, 1, 0);
	p_g_s->setSliderStyle(Slider::RotaryVerticalDrag);
	p_g_s->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
	p_g_s->setColour(Slider::textBoxBackgroundColourId, Colour(0x00ffffff));
	p_g_s->addListener(this);

	addAndMakeVisible(t_l = new Label("t_l",
		""));
	t_l->setFont(Font(15.00f, Font::plain));
	t_l->setJustificationType(Justification::centred);
	t_l->setEditable(false, false, false);
	t_l->setColour(TextEditor::textColourId, Colours::black);
	t_l->setColour(TextEditor::backgroundColourId, Colour(0x00000000));
	
	addAndMakeVisible(g_l = new Label("g_l",
		""));
	g_l->setFont(Font(15.00f, Font::plain));
	g_l->setJustificationType(Justification::centred);
	g_l->setEditable(false, false, false);
	g_l->setColour(TextEditor::textColourId, Colours::black);
	g_l->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	addAndMakeVisible(p_g_val = new Label("p_g_val",
		""));
	p_g_val->setFont(Font(15.00f, Font::plain));
	p_g_val->setJustificationType(Justification::centred);
	p_g_val->setEditable(false, false, false);
	p_g_val->setColour(TextEditor::textColourId, Colours::black);
	p_g_val->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	addAndMakeVisible(dw_l = new Label("dw_l",
		"Dry/Wet"));
	dw_l->setFont(Font(15.00f, Font::plain));
	dw_l->setJustificationType(Justification::centred);
	dw_l->setEditable(false, false, false);
	dw_l->setColour(TextEditor::textColourId, Colours::black);
	dw_l->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    // This is where our plugin's editor size is set.
    setSize (320, 100);

	_recalc_params();

	startTimer(30);
}

void LiNgAudioProcessorEditor::timerCallback() {
	_recalc_params();
}

void LiNgAudioProcessorEditor::resized(){
	t_s->setBounds(0, 76, 250, 24);
	t_l->setBounds(250, 76, 70, 24);
	g_s->setBounds(230, 5, 90, 50);
	g_l->setBounds(230, 55, 90, 20);
	ln_b->setBounds(130, 5, 100, 70);
	dw_s->setBounds(5, 5, 50, 50);
	dw_l->setBounds(0, 55, 60, 20);
	p_g_s->setBounds(70, 5, 50, 50);
	p_g_val->setBounds(60, 55, 70, 20);
}

LiNgAudioProcessorEditor::~LiNgAudioProcessorEditor()
{
}

void LiNgAudioProcessorEditor::_recalc_params() {
	float dw = ownerFilter->getParameter(0);
	float g = ownerFilter->getParameter(1);
	float t = ownerFilter->getParameter(2);
	float p_g = ownerFilter->getParameter(3);
	float ln = ownerFilter->getParameter(4);

	dw_s->setValue(dw, juce::dontSendNotification);
	g_s->setValue(g, juce::dontSendNotification);
	g_l->setText(String("Post: " + _gain_str(g)), juce::dontSendNotification);
	t_s->setValue(t, juce::dontSendNotification);
	t_l->setText(_t_str(t), juce::dontSendNotification);
	p_g_s->setValue(p_g, juce::dontSendNotification);
	p_g_val->setText(String("Pre: " +_p_g_str(p_g)), juce::dontSendNotification);
	ln_b->setToggleState(_ln(ln), juce::dontSendNotification);
	ln_b->setButtonText(_ln_str(ln));
}

void LiNgAudioProcessorEditor::buttonClicked(Button * buttonThatWasClicked) {
	if (buttonThatWasClicked == ln_b)
	{
		ownerFilter->beginParameterChangeGesture(4);
		if (ln_b->getToggleState() == true){
			ln_b->setButtonText("Limiter");
			ownerFilter->setParameterNotifyingHost(4, 1.0f);
		}
		else{
			ln_b->setButtonText("Noise Gate");
			ownerFilter->setParameterNotifyingHost(4, 0.0f);
		}
		ownerFilter->endParameterChangeGesture(8);
	}
}

void LiNgAudioProcessorEditor::sliderValueChanged(Slider* sliderThatWasMoved) {
	if (sliderThatWasMoved == t_s)
	{
		ownerFilter->beginParameterChangeGesture(2);
		ownerFilter->setParameterNotifyingHost(2, (float)t_s->getValue());
		ownerFilter->endParameterChangeGesture(2);
	}
	else if (sliderThatWasMoved == g_s)
	{
		ownerFilter->beginParameterChangeGesture(1);
		ownerFilter->setParameterNotifyingHost(1, (float)g_s->getValue());
		ownerFilter->endParameterChangeGesture(1);
	}
	else if (sliderThatWasMoved == dw_s)
	{
		ownerFilter->beginParameterChangeGesture(0);
		ownerFilter->setParameterNotifyingHost(0, (float)dw_s->getValue());
		ownerFilter->endParameterChangeGesture(0);
	}
	else if (sliderThatWasMoved == p_g_s)
	{
		ownerFilter->beginParameterChangeGesture(3);
		ownerFilter->setParameterNotifyingHost(3, (float)p_g_s->getValue());
		ownerFilter->endParameterChangeGesture(3);
	}
}

//==============================================================================
void LiNgAudioProcessorEditor::paint (Graphics& g)
{
	g.fillAll(Colours::white);
}
