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

#include "WaveFormDisplay.h"

WaveformDisplay::WaveformDisplay ()
{
    setSize (100, 100);
	openGLContext.setRenderer(this);
	openGLContext.setComponentPaintingEnabled(true);
	openGLContext.setContinuousRepainting(true);
	openGLContext.attachTo(*this);

}

WaveformDisplay::~WaveformDisplay() { }

void WaveformDisplay::paint (Graphics&) { }

void WaveformDisplay::resized() { }

