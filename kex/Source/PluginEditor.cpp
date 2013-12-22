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

KexAudioProcessorEditor::KexAudioProcessorEditor(KexAudioProcessor* ownerFilter)
: AudioProcessorEditor(ownerFilter), ownerFilter(ownerFilter)
{
	addAndMakeVisible(atk_rel = new GroupComponent("atk_rel",
		"Atk/Rel"));
	addAndMakeVisible(in_g = new GroupComponent("in_g",
		"Input"));

	addAndMakeVisible(e_g = new GroupComponent("e_g",
		"Expander"));

	addAndMakeVisible(e_c = new GroupComponent("e_c",
		"Compressor"));

	addAndMakeVisible(out = new GroupComponent("out",
		"Output"));

	addAndMakeVisible(wave = new WaveformDisplay());

	addAndMakeVisible(rms = new TextButton("rms"));
	rms->setButtonText("RMS");
	rms->addListener(this);
	rms->setClickingTogglesState(true);
	rms->setConnectedEdges(TextButton::ConnectedOnBottom);

	addAndMakeVisible(ct_s = new Slider("ct_s"));
	ct_s->setRange(0, 1, 0);
	ct_s->setSliderStyle(Slider::LinearHorizontal);
	ct_s->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
	ct_s->setColour(Slider::textBoxBackgroundColourId, Colour(0x00ffffff));
	ct_s->addListener(this);

	addAndMakeVisible(ct_l = new Label("ct_l",
		"Threshold (dB)"));
	ct_l->setFont(Font(15.00f, Font::plain));
	ct_l->setJustificationType(Justification::centredLeft);
	ct_l->setEditable(false, false, false);
	ct_l->setColour(TextEditor::textColourId, Colours::black);
	ct_l->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	addAndMakeVisible(cr_l = new Label("cr_l",
		"Ratio"));
	cr_l->setFont(Font(15.00f, Font::plain));
	cr_l->setJustificationType(Justification::centredLeft);
	cr_l->setEditable(false, false, false);
	cr_l->setColour(TextEditor::textColourId, Colours::black);
	cr_l->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	addAndMakeVisible(cr_s = new Slider("cr_s"));
	cr_s->setRange(0, 1, 0);
	cr_s->setSliderStyle(Slider::LinearHorizontal);
	cr_s->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
	cr_s->setColour(Slider::textBoxBackgroundColourId, Colour(0x00ffffff));
	cr_s->addListener(this);

	addAndMakeVisible(et_l = new Label("et_l",
		"Threshold (dB)"));
	et_l->setFont(Font(15.00f, Font::plain));
	et_l->setJustificationType(Justification::centredLeft);
	et_l->setEditable(false, false, false);
	et_l->setColour(TextEditor::textColourId, Colours::black);
	et_l->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	addAndMakeVisible(et_s = new Slider("et_s"));
	et_s->setRange(0, 1, 0);
	et_s->setSliderStyle(Slider::LinearHorizontal);
	et_s->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
	et_s->setColour(Slider::textBoxBackgroundColourId, Colour(0x00ffffff));
	et_s->addListener(this);

	addAndMakeVisible(er_s = new Slider("cr_s"));
	er_s->setRange(0, 1, 0);
	er_s->setSliderStyle(Slider::LinearHorizontal);
	er_s->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
	er_s->setColour(Slider::textBoxBackgroundColourId, Colour(0x00ffffff));
	er_s->addListener(this);

	addAndMakeVisible(er_l = new Label("er_l",
		"Ratio"));
	er_l->setFont(Font(15.00f, Font::plain));
	er_l->setJustificationType(Justification::centredLeft);
	er_l->setEditable(false, false, false);
	er_l->setColour(TextEditor::textColourId, Colours::black);
	er_l->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	addAndMakeVisible(g_l = new Label("g_l",
		"\nGain"));
	g_l->setFont(Font(15.00f, Font::plain));
	g_l->setJustificationType(Justification::centred);
	g_l->setEditable(false, false, false);
	g_l->setColour(TextEditor::textColourId, Colours::black);
	g_l->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	addAndMakeVisible(g_s = new Slider("g_s"));
	g_s->setRange(0, 1, 0);
	g_s->setSliderStyle(Slider::RotaryVerticalDrag);
	g_s->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
	g_s->setColour(Slider::textBoxBackgroundColourId, Colour(0x00ffffff));
	g_s->addListener(this);

	addAndMakeVisible(dw_l = new Label("dw_l",
		"Dry/Wet"));
	dw_l->setFont(Font(15.00f, Font::plain));
	dw_l->setJustificationType(Justification::centredLeft);
	dw_l->setEditable(false, false, false);
	dw_l->setColour(TextEditor::textColourId, Colours::black);
	dw_l->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	addAndMakeVisible(dw_s = new Slider("dw_s"));
	dw_s->setRange(0, 1, 0);
	dw_s->setSliderStyle(Slider::RotaryVerticalDrag);
	dw_s->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
	dw_s->setColour(Slider::textBoxBackgroundColourId, Colour(0x00ffffff));
	dw_s->addListener(this);

	addAndMakeVisible(stereo = new TextButton("stereo"));
	stereo->setButtonText("Stereo");
	stereo->addListener(this);
	stereo->setClickingTogglesState(true);
	stereo->setConnectedEdges(TextButton::ConnectedOnTop);

	addAndMakeVisible(kex = new Label("kex",
		"Kex"));
	kex->setFont(Font(61.50f, Font::plain));
	kex->setJustificationType(Justification::centred);
	kex->setEditable(false, false, false);
	kex->setColour(TextEditor::textColourId, Colours::black);
	kex->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	addAndMakeVisible(atk = new Slider("atk"));
	atk->setRange(0, 1, 0);
	atk->setSliderStyle(Slider::RotaryVerticalDrag);
	atk->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
	atk->setColour(Slider::textBoxBackgroundColourId, Colour(0x00ffffff));
	atk->addListener(this);

	addAndMakeVisible(rel = new Slider("rel"));
	rel->setRange(0, 1, 0);
	rel->setSliderStyle(Slider::RotaryVerticalDrag);
	rel->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
	rel->setColour(Slider::textBoxBackgroundColourId, Colour(0x00ffffff));
	rel->addListener(this);

	addAndMakeVisible(p_g_s = new Slider("p_g_s"));
	p_g_s->setRange(0, 1, 0);
	p_g_s->setSliderStyle(Slider::RotaryVerticalDrag);
	p_g_s->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
	p_g_s->setColour(Slider::textBoxBackgroundColourId, Colour(0x00ffffff));
	p_g_s->addListener(this);

	addAndMakeVisible(p_g_val = new Label("p_g_val",
		" dB"));
	p_g_val->setFont(Font(15.00f, Font::plain));
	p_g_val->setJustificationType(Justification::centred);
	p_g_val->setEditable(false, false, false);
	p_g_val->setColour(TextEditor::textColourId, Colours::black);
	p_g_val->setColour(TextEditor::backgroundColourId, Colour(0x00000000));


	addAndMakeVisible(a_val = new Label("a_val",
		"ms"));
	a_val->setFont(Font(15.00f, Font::plain));
	a_val->setJustificationType(Justification::centred);
	a_val->setEditable(false, false, false);
	a_val->setColour(TextEditor::textColourId, Colours::black);
	a_val->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	addAndMakeVisible(r_val = new Label("r_val",
		"ms"));
	r_val->setFont(Font(15.00f, Font::plain));
	r_val->setJustificationType(Justification::centred);
	r_val->setEditable(false, false, false);
	r_val->setColour(TextEditor::textColourId, Colours::black);
	r_val->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	addAndMakeVisible(ct_val = new Label("ct_val",
		"dB"));
	ct_val->setFont(Font(15.00f, Font::plain));
	ct_val->setJustificationType(Justification::centred);
	ct_val->setEditable(false, false, false);
	ct_val->setColour(TextEditor::textColourId, Colours::black);
	ct_val->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	addAndMakeVisible(cr_val = new Label("cr_val",
		String::empty));
	cr_val->setFont(Font(15.00f, Font::plain));
	cr_val->setJustificationType(Justification::centred);
	cr_val->setEditable(false, false, false);
	cr_val->setColour(TextEditor::textColourId, Colours::black);
	cr_val->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	addAndMakeVisible(et_val = new Label("et_val",
		"dB"));
	et_val->setFont(Font(15.00f, Font::plain));
	et_val->setJustificationType(Justification::centred);
	et_val->setEditable(false, false, false);
	et_val->setColour(TextEditor::textColourId, Colours::black);
	et_val->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	addAndMakeVisible(er_val = new Label("er_val",
		String::empty));
	er_val->setFont(Font(15.00f, Font::plain));
	er_val->setJustificationType(Justification::centred);
	er_val->setEditable(false, false, false);
	er_val->setColour(TextEditor::textColourId, Colours::black);
	er_val->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	addAndMakeVisible(g_val = new Label("g_val",
		"dB"));
	g_val->setFont(Font(15.00f, Font::plain));
	g_val->setJustificationType(Justification::centred);
	g_val->setEditable(false, false, false);
	g_val->setColour(TextEditor::textColourId, Colours::black);
	g_val->setColour(TextEditor::backgroundColourId, Colour(0x00000000));
    
	setSize(980, 140);

	_recalc_params();

	startTimer(50);
	
}

KexAudioProcessorEditor::~KexAudioProcessorEditor()
{
	atk_rel = nullptr;
	e_g = nullptr;
	e_c = nullptr;
	out = nullptr;
	rms = nullptr;
	ct_s = nullptr;
	ct_l = nullptr;
	cr_l = nullptr;
	cr_s = nullptr;
	et_l = nullptr;
	et_s = nullptr;
	er_s = nullptr;
	er_l = nullptr;
	g_l = nullptr;
	g_s = nullptr;
	dw_l = nullptr;
	dw_s = nullptr;
	stereo = nullptr;
	kex = nullptr;
	atk = nullptr;
	rel = nullptr;
	p_g_s = nullptr;
	p_g_val = nullptr;
	a_val = nullptr;
	r_val = nullptr;
	ct_val = nullptr;
	cr_val = nullptr;
	et_val = nullptr;
	er_val = nullptr;
	g_val = nullptr;
}

void KexAudioProcessorEditor::paint (Graphics& g)
{
	g.fillAll(Colour(0xff9d9d9d));
}

void KexAudioProcessorEditor::resized(){
	atk_rel->setBounds(184, 0, 72, 136);
	in_g->setBounds(109, 0, 72, 136);
	e_g->setBounds(496, 0, 216, 136);
	e_c->setBounds(264, 0, 224, 136);
	wave->setBounds(715, 20, 100, 100);
	out->setBounds(820, 0, 160, 136);
	rms->setBounds(113, 80, 64, 24);
	ct_s->setBounds(272, 40, 160, 24);
	ct_l->setBounds(272, 16, 150, 24);
	cr_l->setBounds(272, 64, 150, 24);
	cr_s->setBounds(272, 88, 160, 24);
	et_l->setBounds(504, 16, 150, 24);
	et_s->setBounds(504, 40, 152, 24);
	er_s->setBounds(504, 88, 152, 24);
	er_l->setBounds(504, 64, 150, 24);
	g_l->setBounds(828, 24, 72, 24);
	g_s->setBounds(828, 48, 64, 56);
	dw_l->setBounds(908, 24, 64, 24);
	dw_s->setBounds(908, 48, 64, 56);
	stereo->setBounds(113, 104, 64, 24);
	kex->setBounds(16, 8, 80, 64);
	atk->setBounds(200, 16, 40, 40);
	rel->setBounds(200, 72, 40, 40);
	p_g_s->setBounds(125, 16, 40, 40);
	p_g_val->setBounds(117, 48, 56, 24);
	a_val->setBounds(192, 48, 56, 24);
	r_val->setBounds(192, 104, 56, 24);
	ct_val->setBounds(424, 40, 56, 24);
	cr_val->setBounds(424, 88, 56, 24);
	et_val->setBounds(648, 40, 56, 24);
	er_val->setBounds(648, 88, 56, 24);
	g_val->setBounds(828, 104, 64, 24);
}

void KexAudioProcessorEditor::_recalc_params() {
	float dw = ownerFilter->getParameter(0);
	float g = ownerFilter->getParameter(1);
	float att = ownerFilter->getParameter(2);
	float re = ownerFilter->getParameter(3);
	float ct = ownerFilter->getParameter(4);
	float cs = ownerFilter->getParameter(5);
	float et = ownerFilter->getParameter(6);
	float es = ownerFilter->getParameter(7);
	float __rms = ownerFilter->getParameter(8);
	float __stereo = ownerFilter->getParameter(9);
	float p_g = ownerFilter->getParameter(10);

	dw_s->setValue(dw, juce::dontSendNotification);
	g_s->setValue(g, juce::dontSendNotification);
	g_val->setText(_gain_str(g), juce::dontSendNotification);
	atk->setValue(att, juce::dontSendNotification);
	a_val->setText(_attms_str(att), juce::dontSendNotification);
	rel->setValue(re, juce::dontSendNotification);
	r_val->setText(_relms_str(re), juce::dontSendNotification);
	ct_s->setValue(ct, juce::dontSendNotification);
	ct_val->setText(_ct_str(ct), juce::dontSendNotification);
	cr_s->setValue(cs, juce::dontSendNotification);
	cr_val->setText(_cr_str(cs), juce::dontSendNotification);
	er_s->setValue(es, juce::dontSendNotification);
	er_val->setText(_er_str(es), juce::dontSendNotification);
	et_s->setValue(et, juce::dontSendNotification);
	et_val->setText(_et_str(et), juce::dontSendNotification);
	rms->setToggleState(_rms(__rms), juce::dontSendNotification);
	rms->setButtonText(_rms_str(__rms));
	stereo->setToggleState(_stereo(__stereo), juce::dontSendNotification);
	stereo->setButtonText(_stereo_str(__stereo));
	p_g_s->setValue(p_g, juce::dontSendNotification);
	p_g_val->setText(_p_g_str(p_g), juce::dontSendNotification);
}

void KexAudioProcessorEditor::timerCallback() {
	_recalc_params();
	wave->addInputLevelValue(ownerFilter->getInputLevel());
	wave->addEnvelopeValue(ownerFilter->getEnvelopeLevel());
}

void KexAudioProcessorEditor::buttonClicked(Button * buttonThatWasClicked) {
	if (buttonThatWasClicked == rms)
	{
		ownerFilter->beginParameterChangeGesture(8);
		if (rms->getToggleState() == true){
			rms->setButtonText("RMS");
			ownerFilter->setParameterNotifyingHost(8, 1.0f);
		}
		else{
			rms->setButtonText("Peak");
			ownerFilter->setParameterNotifyingHost(8, 0.0f);
		}
		ownerFilter->endParameterChangeGesture(8);
	}
	else if (buttonThatWasClicked == stereo)
	{
		ownerFilter->beginParameterChangeGesture(9);
		if (stereo->getToggleState() == true) {
			stereo->setButtonText("Stereo");
			ownerFilter->setParameterNotifyingHost(9, 1.0f);
		}
		else {
			stereo->setButtonText("L/R");
			ownerFilter->setParameterNotifyingHost(9, 0.0f);
		}
		ownerFilter->endParameterChangeGesture(9);
	}
}

void KexAudioProcessorEditor::sliderValueChanged(Slider* sliderThatWasMoved) {
	if (sliderThatWasMoved == ct_s)
	{
		ownerFilter->beginParameterChangeGesture(4);
		ownerFilter->setParameterNotifyingHost(4, (float)ct_s->getValue());
		ownerFilter->endParameterChangeGesture(4);
	}
	else if (sliderThatWasMoved == cr_s)
	{
		ownerFilter->beginParameterChangeGesture(5);
		ownerFilter->setParameterNotifyingHost(5, (float)cr_s->getValue());
		ownerFilter->endParameterChangeGesture(5);
	}
	else if (sliderThatWasMoved == et_s)
	{
		ownerFilter->beginParameterChangeGesture(6);
		ownerFilter->setParameterNotifyingHost(6, (float)et_s->getValue());
		ownerFilter->endParameterChangeGesture(6);
	}
	else if (sliderThatWasMoved == er_s)
	{
		ownerFilter->beginParameterChangeGesture(7);
		ownerFilter->setParameterNotifyingHost(7, (float)er_s->getValue());
		ownerFilter->endParameterChangeGesture(7);
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
	else if (sliderThatWasMoved == atk)
	{
		ownerFilter->beginParameterChangeGesture(2);
		ownerFilter->setParameterNotifyingHost(2, (float)atk->getValue());
		ownerFilter->endParameterChangeGesture(2);
	}
	else if (sliderThatWasMoved == rel)
	{
		ownerFilter->beginParameterChangeGesture(3);
		ownerFilter->setParameterNotifyingHost(3, (float)rel->getValue());
		ownerFilter->endParameterChangeGesture(3);
	}
	else if (sliderThatWasMoved == p_g_s)
	{
		ownerFilter->beginParameterChangeGesture(10);
		ownerFilter->setParameterNotifyingHost(10, (float)p_g_s->getValue());
		ownerFilter->endParameterChangeGesture(10);
	}
}

