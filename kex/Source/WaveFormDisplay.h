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

#ifndef __JUCE_HEADER_383EB4D3954553AE__
#define __JUCE_HEADER_383EB4D3954553AE__

#include "JuceHeader.h"
#include <list>


class WaveformDisplay  : public Component,
                         public OpenGLRenderer
{
public:
    WaveformDisplay ();
    ~WaveformDisplay();

	void newOpenGLContextCreated() {
		ScopedLock m(cs);
		for (int i = 0; i < 30; ++i) {
			input.push_back(0.0f);
			env.push_back(1.0f);
		}

	}
	void openGLContextClosing()
	{
		ScopedLock m(cs);
		input.clear();
		env.clear();
	}
	void addInputLevelValue(float value) {
		ScopedLock m(cs);
		input.push_front(value);
		input.pop_back();
	}
	void addEnvelopeValue(float value){
		ScopedLock m(cs);
		env.push_front(value);
		env.pop_back();
	}
	void renderOpenGL() {
		OpenGLHelpers::clear(Colours::black.withAlpha(1.0f));
		ScopedPointer<LowLevelGraphicsContext> glRenderer(createOpenGLGraphicsContext(openGLContext, 100, 100));
		if (glRenderer != nullptr) {
			Graphics g(*glRenderer);
			Path inputPath;
			
			{
				ScopedLock m(cs);
				auto i = input.cbegin();
				int j = 0;
				inputPath.startNewSubPath(0, 100);
				for (; i != input.cend(); ++i, j++)
					inputPath.lineTo(j * 100.0f / (float)input.size() + 1, 100.0f - (100.0f * *i));
				inputPath.lineTo(100, 100);
				inputPath.closeSubPath();
				g.setFillType(FillType(Colours::lightgrey));
				g.fillPath(inputPath);
			}

			Path envPath;
			{
				ScopedLock m(cs);
				auto i = env.cbegin();
				envPath.startNewSubPath(1, 50.f - (50.f * (*i - 1.f)));
				++i;
				int j = 1;
				for (; i != env.cend(); ++i, j++)
					envPath.lineTo(j * 100 / (float)input.size() + 1, 50.f - (50.f *( *i - 1.f)));
				g.setFillType(FillType(Colours::cadetblue));
				g.strokePath(envPath, PathStrokeType(4.0f));
			}
		}

	}
    void paint (Graphics& g);
    void resized();
private:
	CriticalSection cs;
	OpenGLContext openGLContext;
	std::list<float> input;
	std::list<float> env;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};


#endif   // __JUCE_HEADER_383EB4D3954553AE__
